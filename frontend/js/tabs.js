function initializeTabs() {
    document.querySelectorAll(".tab").forEach(button => {
        button.addEventListener("click", () => {

            document
                .querySelectorAll(".tab")
                .forEach(btn => btn.classList.remove("active"));

            document
                .querySelectorAll(".tab-content")
                .forEach(tab => tab.classList.remove("active"));

            button.classList.add("active");

            document
                .getElementById(button.dataset.tab)
                .classList.add("active");
        });
    });
}

function renderTokens(tokens) {
    if (!tokens || tokens.length === 0) {
        document.getElementById("tokens").innerHTML =
            "No tokens generated.";
        return;
    }

    let html = `
        <table>
            <thead>
                <tr>
                    <th>#</th>
                    <th>Type</th>
                    <th>Value</th>
                </tr>
            </thead>
            <tbody>
    `;

    tokens.forEach(token => {
        html += `
            <tr>
                <td>${token.no}</td>
                <td>${token.type}</td>
                <td>${token.value}</td>
            </tr>
        `;
    });

    html += `
            </tbody>
        </table>
    `;

    document.getElementById("tokens").innerHTML = html;
}

function renderStackTrace(trace) {
    document.getElementById("stack").textContent =
        trace && trace.length
            ? trace.join("\n")
            : "No parser stack trace generated.";
}

function renderLLVMInstructions(instructions) {
    document.getElementById("llvmInstructions").textContent =
        instructions && instructions.length
            ? instructions
            : "No LLVM instructions generated.";
}

function renderLLVMIR(ir) {
    document.getElementById("llvmIR").textContent =
        ir
            ? ir
            : "No LLVM IR generated.";
}

function renderOutput(output) {
    document.getElementById("output").innerHTML =
        output
            ? `<span class="success-text">${output}</span>`
            : "No program output.";
}

function renderErrors(errors) {
    document.getElementById("errors").innerHTML =
        errors && errors.length
            ? `<span class="error-text">${errors.join("\n")}</span>`
            : `<span class="success-text">No errors. Program compiled successfully.</span>`;
}

function openTab(tabId) {
    document.querySelectorAll(".tab").forEach(btn => btn.classList.remove("active"));
    document.querySelectorAll(".tab-content").forEach(tab => tab.classList.remove("active"));

    document.querySelector(`.tab[data-tab="${tabId}"]`).classList.add("active");
    document.getElementById(tabId).classList.add("active");
}

function clearTabs() {
    document.getElementById("tokens").innerHTML = "";

    document.getElementById("stack").textContent = "";

    document.getElementById("llvmInstructions").textContent = "";

    document.getElementById("llvmIR").textContent = "";

    document.getElementById("output").textContent = "";

    document.getElementById("errors").textContent = "";
}
