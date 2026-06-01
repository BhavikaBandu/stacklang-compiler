async function compileCode(code) {
    const response = await fetch("/compile", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({ code })
    });

    return await response.json();
}

async function runCode(code) {
    const response = await fetch("/run", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({ code })
    });

    return await response.json();
}
