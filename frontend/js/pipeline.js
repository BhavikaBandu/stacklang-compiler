const stages = [
    { id: "source", label: "Source", icon: "⌘" },
    { id: "lexer", label: "Lexer", icon: "T" },
    { id: "stack", label: "Stack Validator", icon: "▤" },
    { id: "llvm", label: "LLVM IR", icon: "λ" },
    { id: "clang", label: "Clang", icon: "C" },
    { id: "execute", label: "Execute", icon: "▶" }
];

function initializePipeline() {
    const pipeline = document.getElementById("pipeline");

    pipeline.innerHTML = stages.map((stage, index) => {
        return `
            <div class="stage" id="stage-${stage.id}">
                <div class="stage-icon">${stage.icon}</div>
                <div>${stage.label}</div>
            </div>
            ${index < stages.length - 1 ? `<div class="arrow">→</div>` : ""}
        `;
    }).join("");
}

function resetPipeline() {
    stages.forEach(stage => {
        const el = document.getElementById(`stage-${stage.id}`);
        el.classList.remove("success", "error");
    });

    document.getElementById("pipelineStatus").textContent = "Waiting for source code...";
}

function markCompileSuccess() {
    ["source", "lexer", "stack", "llvm"].forEach(id => {
        document.getElementById(`stage-${id}`).classList.add("success");
    });

    document.getElementById("pipelineStatus").innerHTML =
        `<span class="success-text">Compilation completed successfully.</span>`;
}

function markRunSuccess() {
    stages.forEach(stage => {
        document.getElementById(`stage-${stage.id}`).classList.add("success");
    });

    document.getElementById("pipelineStatus").innerHTML =
        `<span class="success-text">Program executed successfully.</span>`;
}

function markPipelineError(message) {
    document.getElementById("stage-stack").classList.add("error");

    document.getElementById("pipelineStatus").innerHTML =
        `<span class="error-text">${message}</span>`;
}
