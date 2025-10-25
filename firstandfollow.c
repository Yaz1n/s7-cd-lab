#include <stdio.h>
#include <string.h>
#include <ctype.h>

int n;
char prod[10][10];
char firstSet[10], followSet[10];
int m;

// helper: add symbol to set if not already there
void addToSet(char *arr, char c) {
    for (int i = 0; i < strlen(arr); i++)
        if (arr[i] == c) return;
    int len = strlen(arr);
    arr[len] = c;
    arr[len + 1] = '\0';
}

void FIRST(char c);
void FOLLOW(char c);

void FIRST(char c) {
    if (!isupper(c)) { // terminal
        addToSet(firstSet, c);
        return;
    }

    for (int i = 0; i < n; i++) {
        if (prod[i][0] == c) {
            if (prod[i][2] == 'e') {
                addToSet(firstSet, 'e');
            } else {
                int j = 2;
                while (prod[i][j] != '\0') {
                    if (prod[i][j] == c) break;
                    FIRST(prod[i][j]);
                    if (strchr(firstSet, 'e') == NULL)
                        break;
                    j++;
                }
            }
        }
    }
}

void FOLLOW(char c) {
    if (prod[0][0] == c)
        addToSet(followSet, '$');

    for (int i = 0; i < n; i++) {
        for (int j = 2; j < strlen(prod[i]); j++) {
            if (prod[i][j] == c) {
                if (prod[i][j + 1] != '\0') {
                    char next = prod[i][j + 1];
                    char temp[10] = "";
                    strcpy(firstSet, "");
                    FIRST(next);

                    for (int k = 0; k < strlen(firstSet); k++) {
                        if (firstSet[k] != 'e')
                            addToSet(followSet, firstSet[k]);
                    }

                    if (strchr(firstSet, 'e'))
                        FOLLOW(prod[i][0]);
                } else if (prod[i][0] != c)
                    FOLLOW(prod[i][0]);
            }
        }
    }
}

int main() {
    printf("Enter the number of productions:\n");
    scanf("%d", &n);

    printf("Enter the productions (use e for epsilon):\n");
    for (int i = 0; i < n; i++)
        scanf("%s", prod[i]);

    char done[10] = "";
    for (int i = 0; i < n; i++) {
        char nt = prod[i][0];
        if (strchr(done, nt)) continue; // skip duplicates
        strncat(done, &nt, 1);

        strcpy(firstSet, "");
        FIRST(nt);
        printf("FIRST(%c) =  ", nt);
        for (int j = 0; j < strlen(firstSet); j++)
            printf("\t%c ", firstSet[j]);
        printf("\t\n");

        strcpy(followSet, "");
        FOLLOW(nt);
        printf("FOLLOW(%c) =  ", nt);
        for (int j = 0; j < strlen(followSet); j++)
            printf("\t%c ", followSet[j]);
        printf("\t\n\n");
    }
}
