let stackStates = [];
let stackSteps = [];
let currentStackStep = 0;
let playTimer = null;

function extractStackFromCodegenLine(line) {
    const match = line.match(/\[(.*)\]$/);

    if (!match) {
        return [];
    }

    const inside = match[1].trim();

    if (!inside) {
        return [];
    }

    return inside.split(",").map(item => {
        return item
            .trim()
            .replace(/^i32\s+/, "")
            .replace(/^i1\s+/, "");
    });
}

function extractOperationFromCodegenLine(line) {
    const cols = line.trim().split(/\s{2,}/);

    if (cols.length >= 2) {
        return cols[1].trim();
    }

    return line.trim();
}

function buildStackStatesFromCodegenTrace(codegenTrace) {
    stackStates = [[]];
    stackSteps = ["Initial Stack"];

    if (!codegenTrace || codegenTrace.length === 0) {
        return;
    }

    codegenTrace.forEach(line => {
        const operation = extractOperationFromCodegenLine(line);
        const stack = extractStackFromCodegenLine(line);

        stackSteps.push(operation);
        stackStates.push(stack);
    });
}

function renderStackStep(index) {
    const stepsBox = document.getElementById("executionSteps");
    const stackBox = document.getElementById("stackBox");

    stepsBox.innerHTML = "";
    stackBox.innerHTML = "";

    if (stackStates.length <= 1) {
        stepsBox.innerHTML = "No stack operations.";
        stackBox.innerHTML = `<div class="empty-stack">Empty Stack</div>`;
        return;
    }

    stackSteps.forEach((step, i) => {
        const item = document.createElement("div");
        item.className = "step-item";

        if (i === index) {
            item.classList.add("active");
        }

        item.textContent = `${i}. ${step}`;
        item.onclick = () => {
            pauseStackAnimation();
            currentStackStep = i;
            renderStackStep(currentStackStep);
        };

        stepsBox.appendChild(item);
    });

    const currentStack = stackStates[index];

    if (!currentStack || currentStack.length === 0) {
        stackBox.innerHTML = `<div class="empty-stack">Empty Stack</div>`;
        return;
    }

    currentStack.forEach((value, position) => {
        const div = document.createElement("div");
        div.className = "stack-item";

        if (position === currentStack.length - 1) {
            div.classList.add("top-item");
        }

        div.textContent = value;
        stackBox.appendChild(div);
    });
}

function renderStackVisualizer(codegenTrace) {
    pauseStackAnimation();

    buildStackStatesFromCodegenTrace(codegenTrace);

    currentStackStep = 0;
    renderStackStep(currentStackStep);
}

function nextStackStep() {
    if (currentStackStep < stackStates.length - 1) {
        currentStackStep++;
        renderStackStep(currentStackStep);
    } else {
        pauseStackAnimation();
    }
}

function previousStackStep() {
    if (currentStackStep > 0) {
        currentStackStep--;
        renderStackStep(currentStackStep);
    }
}

function playStackAnimation() {
    pauseStackAnimation();

    playTimer = setInterval(() => {
        nextStackStep();
    }, 800);
}

function pauseStackAnimation() {
    if (playTimer) {
        clearInterval(playTimer);
        playTimer = null;
    }
}

function initializeStackControls() {
    document.getElementById("prevStepBtn").addEventListener("click", previousStackStep);
    document.getElementById("playStepBtn").addEventListener("click", playStackAnimation);
    document.getElementById("pauseStepBtn").addEventListener("click", pauseStackAnimation);
    document.getElementById("nextStepBtn").addEventListener("click", nextStackStep);
}

function clearStackVisualizer() {
    pauseStackAnimation();

    stackStates = [];
    stackSteps = [];
    currentStackStep = 0;

    document.getElementById("executionSteps").innerHTML = "";
    document.getElementById("stackBox").innerHTML = "";
}
