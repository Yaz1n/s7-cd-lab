#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STATES 20
#define MAX_SYMBOLS 10

int dfa[MAX_STATES][MAX_SYMBOLS];
bool final[MAX_STATES];
bool distinguish[MAX_STATES][MAX_STATES];
int group[MAX_STATES];
int num_states, num_symbols;
char symbols[MAX_SYMBOLS];

int main() {
    printf("Number of states: ");
    scanf("%d", &num_states);
    printf("Number of symbols: ");
    scanf("%d", &num_symbols);

    printf("Enter symbols:\n");
    for (int i = 0; i < num_symbols; i++)
        scanf(" %c", &symbols[i]);

    printf("Enter DFA transitions (state x symbol -> next state):\n");
    for (int i = 0; i < num_states; i++) {
        for (int j = 0; j < num_symbols; j++) {
            scanf("%d", &dfa[i][j]);
        }
    }

    // Input final states
    memset(final, 0, sizeof(final));
    int n;
    printf("Number of final states: ");
    scanf("%d", &n);
    printf("Enter final states:\n");
    for (int i = 0; i < n; i++) {
        int f;
        scanf("%d", &f);
        final[f] = true;
    }

    // Step 1: Initialize distinguishability table
    memset(distinguish, 0, sizeof(distinguish));
    for (int i = 0; i < num_states; i++) {
        for (int j = 0; j < i; j++) {
            distinguish[i][j] = (final[i] != final[j]);
        }
    }

    // Step 2: Iteratively mark distinguishable pairs
    bool changed;
    do {
        changed = false;
        for (int i = 0; i < num_states; i++) {
            for (int j = 0; j < i; j++) {
                if (distinguish[i][j]) continue;
                for (int k = 0; k < num_symbols; k++) {
                    int a = dfa[i][k];
                    int b = dfa[j][k];
                    if (a == b) continue;
                    if (a < 0 || b < 0) continue; // skip undefined transitions
                    if (a > b ? distinguish[a][b] : distinguish[b][a]) {
                        distinguish[i][j] = true;
                        changed = true;
                        break;
                    }
                }
            }
        }
    } while (changed);

    // Step 3: Group equivalent states
    memset(group, -1, sizeof(group));
    int gid = 0;
    for (int i = 0; i < num_states; i++) {
        if (group[i] != -1) continue;
        group[i] = gid;
        for (int j = 0; j < num_states; j++) {
            if (i != j && !(i > j ? distinguish[i][j] : distinguish[j][i])) {
                group[j] = gid;
            }
        }
        gid++;
    }

    // Step 4: Print groupings
    printf("\n=== State Groups (Equivalence Classes) ===\n");
    for (int g = 0; g < gid; g++) {
        printf("Group Q%d: { ", g);
        for (int s = 0; s < num_states; s++) {
            if (group[s] == g)
                printf("%d ", s);
        }
        printf("}\n");
    }

    // Step 5: Print minimized DFA transitions
    printf("\n=== Minimized DFA (%d states) ===\n", gid);
    printf("State\t");
    for (int s = 0; s < num_symbols; s++)
        printf("%c\t", symbols[s]);
    printf("\n---------------------------------\n");

    for (int g = 0; g < gid; g++) {
        printf("Q%d\t", g);
        // pick representative of this group
        int rep = -1;
        for (int s = 0; s < num_states; s++) {
            if (group[s] == g) {
                rep = s;
                break;
            }
        }
        for (int s = 0; s < num_symbols; s++) {
            int next = dfa[rep][s];
            printf("Q%d\t", group[next]);
        }
        printf("\n");
    }

    // Step 6: Print minimized DFA final states
    printf("\nFinal states: ");
    bool printed[MAX_STATES] = {0};
    for (int i = 0; i < num_states; i++) {
        if (final[i] && !printed[group[i]]) {
            printf("Q%d ", group[i]);
            printed[group[i]] = true;
        }
    }
    printf("\n");

    return 0;
}
