
#include <stdio.h>
#include <string.h>

#define MAX 50

char stack[MAX], input[MAX];
char lhs[MAX][10], rhs[MAX][20];
int prodCount;
int top = -1, i = 0;

void push(char c) {
    stack[++top] = c;
    stack[top + 1] = '\0';
}

void pop(int n) {
    top -= n;
    stack[top + 1] = '\0';
}

void check() {
    for (int p = 0; p < prodCount; p++) {
        int len = strlen(rhs[p]);
        if (top + 1 >= len) {
            // Compare last 'len' characters of stack with rhs
            if (strncmp(&stack[top - len + 1], rhs[p], len) == 0) {
                pop(len);
                push(lhs[p][0]);
                printf("\tReduce: %s->%s\n", lhs[p], rhs[p]);
                // After a reduction, check again (nested reductions)
                check();
                break;
            }
        }
    }
}

int main() {
    printf("Dynamic Shift-Reduce Parser\n");
    printf("Enter number of productions: ");
    scanf("%d", &prodCount);
    getchar();

    printf("Enter productions (e.g., E->E+E):\n");
    for (int j = 0; j < prodCount; j++) {
        char temp[30];
        fgets(temp, sizeof(temp), stdin);
        temp[strcspn(temp, "\n")] = 0;
        // Split into LHS and RHS
        lhs[j][0] = temp[0];
        lhs[j][1] = '\0';
        strcpy(rhs[j], &temp[3]);
    }

    printf("\nEnter the input expression ending with $: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    printf("\nSTACK\tINPUT\tACTION\n");
    printf("-------------------------------\n");

    while (input[i] != '\0') {
        push(input[i]);
        printf("%s\t%s\tShift '%c'\n", stack, &input[i + 1], input[i]);
        i++;
        check();
    }

    if (top == 0 && stack[top] == lhs[0][0])
        printf("\nAccepted ✅\n");
    else
        printf("\nRejected ❌\n");

    return 0;
}
