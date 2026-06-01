document.addEventListener("DOMContentLoaded", () => {
    initializeEditor();
    initializeTabs();
    initializePipeline();
    initializeStackControls();
    loadSamples();

    document.getElementById("compileBtn").addEventListener("click", handleCompile);
    document.getElementById("runBtn").addEventListener("click", handleRun);
    document.getElementById("resetBtn").addEventListener("click", handleReset);
});

function loadSamples() {
    const select = document.getElementById("sampleSelect");

    Object.keys(samples).forEach(key => {
        const option = document.createElement("option");
        option.value = key;
        option.textContent = samples[key].name;
        select.appendChild(option);
    });

    select.addEventListener("change", () => {
        const selected = select.value;

        if (selected && samples[selected]) {
            setCode(samples[selected].code);
            handleResetOutputsOnly();
        }
    });
}

async function handleCompile() {
    resetPipeline();

    const code = getCode();

    if (!code.trim()) {
        renderErrors(["Source code is empty."]);
        return;
    }

    try {
        const result = await compileCode(code);

        renderTokens(result.tokens);
        renderStackTrace(result.stackTrace);
        renderLLVMInstructions(result.llvmInstructions);
        renderLLVMIR(result.llvmIR);
        renderOutput("");
        renderErrors(result.errors);
        
        if (result.errors && result.errors.length > 0) {
	    openTab("errors");
	}

        renderStackVisualizer(result.codegenTrace);
        renderSSAMapping(result.stackSSAMapping);

        if (result.success) {
            markCompileSuccess();
        } else {
            markPipelineError("Compilation failed.");
        }
    } catch (error) {
        renderErrors([error.message]);
        markPipelineError("Backend connection failed.");
    }
}

async function handleRun() {
    resetPipeline();

    const code = getCode();

    if (!code.trim()) {
        renderErrors(["Source code is empty."]);
        return;
    }

    try {
        const result = await runCode(code);

        renderTokens(result.tokens);
        renderStackTrace(result.stackTrace);
        renderLLVMInstructions(result.llvmInstructions);
        renderLLVMIR(result.llvmIR);
        renderOutput(result.output);
        renderErrors(result.errors);
        
        if (result.errors && result.errors.length > 0) {
	    openTab("errors");
	}

        renderStackVisualizer(result.codegenTrace);
        renderSSAMapping(result.stackSSAMapping);

        if (result.success) {
            markRunSuccess();
        } else {
            markPipelineError("Run failed.");
        }
    } catch (error) {
        renderErrors([error.message]);
        markPipelineError("Backend connection failed.");
    }
}

function handleReset() {
    clearEditor();
    clearTabs();
    clearStackVisualizer();
    clearSSAMapping();
    resetPipeline();

    const select = document.getElementById("sampleSelect");

    if (select) {
        select.value = "";
    }
}

function handleResetOutputsOnly() {
    clearTabs();
    clearStackVisualizer();
    clearSSAMapping();
    resetPipeline();
}
