let editor;

function initializeEditor() {
    require.config({
        paths: {
            vs: "https://cdnjs.cloudflare.com/ajax/libs/monaco-editor/0.45.0/min/vs"
        }
    });

    require(["vs/editor/editor.main"], function () {
        monaco.languages.register({ id: "stacklang" });

        monaco.languages.setMonarchTokensProvider("stacklang", {
            tokenizer: {
                root: [
                    [/\b(if|else|endif|print)\b/, "keyword"],
                    [/[0-9]+/, "number"],
                    [/[a-zA-Z_][a-zA-Z0-9_]*/, "identifier"],
                    [/[\+\-\*\/><=!]+/, "operator"],
                    [/\/\/.*/, "comment"]
                ]
            }
        });

        monaco.editor.defineTheme("stacklang-dark", {
            base: "vs-dark",
            inherit: true,
            rules: [
                { token: "keyword", foreground: "c084fc" },
                { token: "number", foreground: "f59e0b" },
                { token: "identifier", foreground: "93c5fd" },
                { token: "operator", foreground: "f9fafb" },
                { token: "comment", foreground: "6b7280", fontStyle: "italic" }
            ],
            colors: {
                "editor.background": "#030712"
            }
        });

        editor = monaco.editor.create(document.getElementById("editor"), {
            value: samples.arithmetic.code,
            language: "stacklang",
            theme: "stacklang-dark",
            fontSize: 16,
            minimap: { enabled: false },
            automaticLayout: true
        });
    });
}

function getCode() {
    return editor ? editor.getValue() : "";
}

function setCode(code) {
    if (editor) {
        editor.setValue(code);
    }
}

function clearEditor() {
    if (editor) {
        editor.setValue("");
    }
}
