#include <stdio.h>
#include <string.h>

char stack[50];
int top = -1;

// Stack operations
void push(char c) {
    stack[++top] = c;
    stack[top+1] = '\0';
}

void pop() {
    if(top >= 0) top--;
    stack[top+1] = '\0';
}

char peek() {
    if(top >= 0) return stack[top];
    return '\0';
}

// Operator precedence table
// order: +, *, i, (, ), $
char precedence[6][6] = {
    // +    *    i    (    )    $
    {'>', '<', '<', '<', '>', '>'}, // +
    {'>', '>', '<', '<', '>', '>'}, // *
    {'>', '>', ' ', ' ', '>', '>'}, // i
    {'<', '<', '<', '<', '=', ' '}, // (
    {'>', '>', ' ', ' ', '>', '>'}, // )
    {'<', '<', '<', '<', ' ', '='}  // $
};

int getIndex(char c) {
    switch(c) {
        case '+': return 0;
        case '*': return 1;
        case 'i': return 2;
        case '(': return 3;
        case ')': return 4;
        case '$': return 5;
    }
    return -1;
}

int main() {
    char input[50];
    printf("Enter expression (end without $): ");
    scanf("%s", input);
    strcat(input, "$"); // add end marker

    push('$'); // initialize stack
    int i = 0;
    char a = input[i];

    printf("\nSTACK\t\tINPUT\t\tACTION\n");

    while(1) {
        char topSym = peek();
        printf("%-10s\t%-10s\t", stack, input+i);

        // Accept condition
        if(topSym == '$' && a == '$') {
            printf("ACCEPTED\n");
            break;
        }

        int row = getIndex(topSym);
        int col = getIndex(a);

        if(row == -1 || col == -1) {
            printf("INVALID SYMBOL\n");
            break;
        }

        char rel = precedence[row][col];

        if(rel == '<' || rel == '=') {
            if(a == '$') { // don't shift $
                printf("REDUCE\n");
                pop();
                continue;
            }
            printf("SHIFT %c\n", a);
            push(a);
            a = input[++i];
        } 
        else if(rel == '>') {
            printf("REDUCE\n");
            pop();
        } 
        else {
            printf("INVALID RELATION\n");
            break;
        }
    }

    return 0;
}
