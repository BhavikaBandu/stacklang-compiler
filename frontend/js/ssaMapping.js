function renderSSAMapping(mapping) {
    const box = document.getElementById("ssaMapping");

    if (!mapping || mapping.length === 0) {
        box.innerHTML = "No Stack → SSA mapping available.";
        return;
    }

    let html = `
        <div class="mapping-row mapping-header">
            <div>Operation</div>
            <div>Stack Transition</div>
            <div>LLVM / SSA Value</div>
        </div>
    `;

    mapping.forEach(row => {
        html += `
            <div class="mapping-row">
                <div class="mapping-op">
                    ${row.operation}
                </div>

                <div class="mapping-stack">
                    ${row.stack}
                </div>

                <div class="mapping-ssa">
                    ${row.ssa}
                </div>
            </div>
        `;
    });

    box.innerHTML = html;
}

function clearSSAMapping() {
    document.getElementById("ssaMapping").innerHTML = "";
}
