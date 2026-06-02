const samples = {
    arithmetic: {
        name: "Arithmetic Operations",
        code:
`10 5 + print
20 4 / print
7 3 - print
6 8 * print`
    },

    nestedArithmetic: {
        name: "Nested Arithmetic",
        code:
`10 5 + 2 * print
10 5 2 * + print`
    },

    variables: {
        name: "Variable Assignment",
        code:
`5 3 + x =
x 2 * y =
y print`
    },

    comparisons: {
        name: "All Comparisons",
        code:
`10 5 > print
10 5 < print
10 10 == print
10 5 != print
10 10 >= print
5 10 <= print`
    },
    stackUnderflow: {
        name: "Error: Stack Underflow",
        code:
`5 +`
    },

    undefinedVariable: {
        name: "Error: Undefined Variable",
        code:
`x print`
    },

    missingEndif: {
        name: "Error: Missing Endif",
        code:
`5 10 < if
    1 print
else
    0 print`
    }
};
