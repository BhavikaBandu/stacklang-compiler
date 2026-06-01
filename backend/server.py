from flask import Flask, request, jsonify, send_from_directory
from flask_cors import CORS
import subprocess
import os
import uuid

BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
FRONTEND_DIR = os.path.join(BASE_DIR, "frontend")
WORK_DIR = os.path.join(BASE_DIR, "backend", "runs")

COMPILER = os.path.join(BASE_DIR, "stack_compiler")

app = Flask(__name__, static_folder=FRONTEND_DIR)
CORS(app)

os.makedirs(WORK_DIR, exist_ok=True)


@app.route("/")
def index():
    return send_from_directory(FRONTEND_DIR, "index.html")


@app.route("/css/<path:path>")
def css(path):
    return send_from_directory(os.path.join(FRONTEND_DIR, "css"), path)


@app.route("/js/<path:path>")
def js(path):
    return send_from_directory(os.path.join(FRONTEND_DIR, "js"), path)


def extract_section(text, start_marker, end_marker=None):
    if start_marker not in text:
        return ""

    part = text.split(start_marker, 1)[1]

    if end_marker and end_marker in part:
        part = part.split(end_marker, 1)[0]

    return part.strip()


def parse_tokens(stdout):
    lexer = extract_section(stdout, "LEXER PHASE", "PARSER PHASE")
    tokens = []

    for line in lexer.splitlines():
        parts = line.split()

        if len(parts) >= 3 and parts[0].isdigit():
            tokens.append({
                "no": parts[0],
                "type": parts[1],
                "value": parts[2]
            })

    return tokens


def parse_stack_trace(stdout):
    trace = extract_section(stdout, "STACK INSTRUCTION LIST", "LLVM IR GENERATION PHASE")
    result = []

    for line in trace.splitlines():
        line = line.strip()

        if not line:
            continue

        if line.startswith("="):
            continue

        result.append(line)

    return result


def parse_codegen_trace(stdout):
    trace = extract_section(stdout, "LLVM IR GENERATION TRACE", "GENERATED LLVM IR INSTRUCTIONS")
    result = []

    for line in trace.splitlines():
        line = line.strip()

        if not line:
            continue

        if line.startswith("=") or line.startswith("-"):
            continue

        if line.startswith("STEP"):
            continue

        result.append(line)

    return result


def parse_llvm_instructions(stdout):
    compact = extract_section(
        stdout,
        "GENERATED LLVM IR INSTRUCTIONS",
        "STACK TO SSA MAPPING"
    )

    result = []

    for line in compact.splitlines():
        line = line.strip()

        if not line:
            continue

        if line.startswith("="):
            continue

        result.append(line)

    return "\n".join(result)


def parse_stack_ssa_mapping(stdout):
    mapping = extract_section(
        stdout,
        "STACK TO SSA MAPPING",
        "LLVM IR file written to:"
    )

    result = []

    for line in mapping.splitlines():
        line = line.rstrip()

        if not line:
            continue

        if line.startswith("=") or line.startswith("-"):
            continue

        if line.startswith("STACK OPERATION"):
            continue

        parts = line.split("  ")

        clean_parts = []
        for part in parts:
            part = part.strip()
            if part:
                clean_parts.append(part)

        if len(clean_parts) >= 3:
            result.append({
                "operation": clean_parts[0],
                "stack": clean_parts[1],
                "ssa": " ".join(clean_parts[2:])
            })

    return result

def compile_source(code):
    run_id = str(uuid.uuid4())
    run_dir = os.path.join(WORK_DIR, run_id)
    os.makedirs(run_dir, exist_ok=True)

    source_file = os.path.join(run_dir, "input.stack")
    output_ll = os.path.join(run_dir, "output.ll")

    with open(source_file, "w") as f:
        f.write(code)

    if not os.path.exists(COMPILER):
        return {
            "success": False,
            "tokens": [],
            "stackTrace": [],
            "codegenTrace": [],
            "stackSSAMapping": [],
            "llvmInstructions": "",
            "llvmIR": "",
            "output": "",
            "errors": [f"Compiler executable not found: {COMPILER}"],
            "runDir": run_dir
        }

    result = subprocess.run(
        [COMPILER, source_file],
        cwd=run_dir,
        capture_output=True,
        text=True
    )

    llvm_ir = ""
    if os.path.exists(output_ll):
        with open(output_ll, "r") as f:
            llvm_ir = f.read()

    errors = []
    if result.stderr.strip():
        errors.append(result.stderr.strip())

    success = result.returncode == 0

    return {
        "success": success,
        "tokens": parse_tokens(result.stdout),
        "stackTrace": parse_stack_trace(result.stdout),
        "codegenTrace": parse_codegen_trace(result.stdout),
        "stackSSAMapping": parse_stack_ssa_mapping(result.stdout),
        "llvmInstructions": parse_llvm_instructions(result.stdout),
        "llvmIR": llvm_ir,
        "output": "",
        "errors": errors,
        "stdout": result.stdout,
        "runDir": run_dir,
        "outputLL": output_ll
    }


@app.route("/compile", methods=["POST"])
def compile_code():
    data = request.get_json()
    code = data.get("code", "")

    result = compile_source(code)

    return jsonify(result)


@app.route("/run", methods=["POST"])
def run_code():
    data = request.get_json()
    code = data.get("code", "")

    result = compile_source(code)

    if not result["success"]:
        return jsonify(result)

    clang_result = subprocess.run(
        ["clang", "output.ll", "-o", "program"],
        cwd=result["runDir"],
        capture_output=True,
        text=True
    )

    if clang_result.returncode != 0:
        result["success"] = False
        result["errors"].append(clang_result.stderr)
        return jsonify(result)

    program_path = os.path.join(result["runDir"], "program")

    if not os.path.exists(program_path):
        result["success"] = False
        result["errors"].append("Executable was not created.")
        return jsonify(result)

    program_result = subprocess.run(
        [program_path],
        cwd=result["runDir"],
        capture_output=True,
        text=True
    )

    result["output"] = program_result.stdout

    if program_result.returncode != 0:
        result["success"] = False
        result["errors"].append(
            f"Program exited with code {program_result.returncode}"
        )

    if program_result.stderr.strip():
        result["errors"].append(program_result.stderr)

    return jsonify(result)

if __name__ == "__main__":
    app.run(debug=True, port=5000)
