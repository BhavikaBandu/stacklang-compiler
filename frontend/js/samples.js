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

    ifelse: {
        name: "If-Else Conditions",
        code:
`15 x =
x 10 > if
    x print
else
    0 print
endif

5 y =
y 10 > if
    y print
else
    0 print
endif`
    },

    fullProgram: {
        name: "Full Program",
        code:
`10 a =
20 b =
a b + sum =
sum 2 / avg =
avg 10 > if
    avg print
else
    0 print
endif`
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
