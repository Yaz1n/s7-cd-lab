#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

char input[100];
int pos = 0;

// Function declarations
void E();
void E_prime();
void T();
void T_prime();
void F();
void P();

// Utility to match current symbol
void match(char expected) {
    if (input[pos] == expected) {
        pos++;
    } else {
        printf("Syntax Error at '%c'\n", input[pos]);
        exit(1);
    }
}

// P -> (E) | id
void P() {
    if (input[pos] == '(') {
        match('(');
        E();
        match(')');
    } else if (isalpha(input[pos])) {
        printf("Matched id: %c\n", input[pos]);
        pos++;
    } else {
        printf("Syntax Error at '%c'\n", input[pos]);
        exit(1);
    }
}

// F -> P ^ F | P
void F() {
    P();
    while (input[pos] == '^') {
        match('^');
        printf("Operator ^\n");
        F(); // right-associative
    }
}

// T -> F T'
void T() {
    F();
    T_prime();
}

// T' -> * F T' | / F T' | ε
void T_prime() {
    while (input[pos] == '*' || input[pos] == '/') {
        char op = input[pos];
        match(op);
        printf("Operator %c\n", op);
        F();
    }
}

// E -> T E'
void E() {
    T();
    E_prime();
}

// E' -> + T E' | - T E' | ε
void E_prime() {
    while (input[pos] == '+' || input[pos] == '-') {
        char op = input[pos];
        match(op);
        printf("Operator %c\n", op);
        T();
    }
}

int main() {
    printf("Recursive Descent Parser\n");
    printf("Enter the expression (end with $): ");
    fgets(input, sizeof(input), stdin);

    // Remove newline
    input[strcspn(input, "\n")] = 0;

    // Add $ at the end to mark input end
    strcat(input, "$");

    E(); // start symbol

    if (input[pos] == '$') {
        printf("\nParsing Successful — Expression Accepted!\n");
    } else {
        printf("\nParsing Failed — Unexpected symbol '%c'\n", input[pos]);
    }

    return 0;
}
