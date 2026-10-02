/* COP 3502C PA2
This program is written by: Hailey Simpson */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MONSTERS 12
#define MAX_CONSTRAINTS 100
#define MAX_STRING 31

/* Monster Struct */ 
typedef struct
{
    char *name;
    char element[MAX_STRING];
} Monster;

/* Constraint Struct */ 
typedef struct
{
    char type[MAX_STRING];
    int a;
    int b;
    int position;
    char elementA[MAX_STRING];
    char elementB[MAX_STRING];
} Constraint;

/* Global arrays for storing monsters and constraints */ 
Monster monsters[MAX_MONSTERS];
Constraint constraints[MAX_CONSTRAINTS];

/* This recursive function checks a BEFORE constraint.
 * Monster A must appear somewhere before monster B. */
int recursiveCheckBefore(int perm[], int n, int a, int b, int k, int foundA)
{
    // If we reach the end without finding monster B, the constraint is invalid
    if (k == n)
    {
        return 0;
    }
    // If the current monster is monster B, return foundA
    if (perm[k] == b)
    {
        // The constraint is only valid if monster A was found first
        return foundA;
    }
    // If the current monster is monster A, monster A has been found
    if (perm[k] == a)
    {
        foundA = 1;
    }
    // Recursively check the next position in the permutation
    return recursiveCheckBefore(perm, n, a, b, k + 1, foundA);
}

/* This recursive function checks ELEMENT_BEFORE_ALL constraint.
 * Every monster with element A must appear before every monsters with element B. */
int recursiveCheckElementOrder(int perm[], int n, const char *elementA, const char *elementB, int k, int foundB)
{ 
    // If we reach the end without finding a problem, the constraint is valid
    if (k == n)
    {
        return 1;
    }
    // If the current monster has element B, element B has been found
    if (strcmp(monsters[perm[k]].element, elementB) == 0)
    {
        foundB = 1;
    }
    // If element A apperars after element B, the constraint is invalid
    if (strcmp(monsters[perm[k]].element, elementA) == 0 && foundB)
    {
        return 0;
    }
    // Recursively check the next position
    return recursiveCheckElementOrder(perm, n, elementA, elementB, k + 1, foundB);
}

/* This function finds the position of a monster in the permutation. */
int findPosition(int perm[], int n, int monsterIdx)
{
    // Search through every position in the permutation
    for (int i = 0; i < n; i++)
    {
        // If the position constains the requested monster, return the postion
        if (perm[i] == monsterIdx)
        {
            return i;
        }
    }
    // If the monster was not found, return -1
    return -1;
}

/* This function checks all constraints after a complete permutation has been created.
 * Note: main1.c checks constraints ONLY here, at the base case. */
int checkConstraints(int perm[], int n)
{
    // Monster position variables
    int posA, posB;

    // Go through every stored constraint 
    for (int i = 0; i < MAX_CONSTRAINTS; i++)
    {
        // Stop when the empty constraint marks the end
        if (constraints[i].type[0] == '\0')
        {
            break;
        }

        // 1. BEFORE
        if (strcmp(constraints[i].type, "BEFORE") == 0)
        {
            // If monster A does not appear before monster B, return 0
            if (!recursiveCheckBefore(perm, n, constraints[i].a, constraints[i].b, 0, 0)) 
            { 
                return 0; 
            }
        }

        // 2. IMMEDIATELY_BEFORE 
        else if (strcmp(constraints[i].type, "IMMEDIATELY_BEFORE") == 0)
        {
            // Find position of monster A
            posA = findPosition(perm, n, constraints[i].a);
            // Find position of monster B
            posB = findPosition(perm, n, constraints[i].b);
            // If monster B is not exactly one postion after monster A, return 0
            if (posB != posA + 1)
            {
                return 0;
            }
        }

        // 3. POSITION 
        else if (strcmp(constraints[i].type, "POSITION") == 0)
        {
            // Find position of monster A
            posA = findPosition(perm, n, constraints[i].a);
            // If monster A is not in the required 1-based postion, return 0
            if (posA != constraints[i].position - 1)
            {
                return 0;
            }
        }

        // 4. FIRST_ELEMENT 
        else if (strcmp(constraints[i].type, "FIRST_ELEMENT") == 0)
        {
            // If the first monster's element does not match the required element, return 0
            if (strcmp(monsters[perm[0]].element, constraints[i].elementA) != 0)
            {
                return 0;
            }
        }

        // 5. LAST_ELEMENT 
        else if (strcmp(constraints[i].type, "LAST_ELEMENT") == 0)
        {
            // If the last monster's element does not match the required element, return 0
            if (strcmp(monsters[perm[n - 1]].element, constraints[i].elementA) != 0)
            {
                return 0;
            }
        }

        // 6. NO_ADJACENT_ELEMENT 
        else if (strcmp(constraints[i].type, "NO_ADJACENT_ELEMENT") == 0)
        {
            // Check every pair of neighbouring monsters 
            for (int j = 0; j < n - 1; j++)
            {
                // If two monsters with the specified element appear next to each other, return 0
                if (strcmp(monsters[perm[j]].element,constraints[i].elementA) == 0 &&
                    strcmp(monsters[perm[j + 1]].element, constraints[i].elementA) == 0)
                {
                    return 0;
                }
            }
        }

        // 7. ELEMENT_BEFORE_ALL 
        else if (strcmp(constraints[i].type, "ELEMENT_BEFORE_ALL") == 0)
        {
            // If every monster with element A does not appear before every monster with element B, return 0
            if (!recursiveCheckElementOrder(perm, n, constraints[i].elementA, constraints[i].elementB, 0, 0))
            {
                return 0;
            }
        }
    }
    // Return 1 because every constraint was satisfied 
    return 1;
}

/* This function prints the monsters in the valid permutation. */
void print(int perm[], int n)
{
    // Go through each position in the permutation 
    for (int i = 0; i < n; i++)
    {
        // Print the monster's name and element 
        printf("%s %s\n", monsters[perm[i]].name, monsters[perm[i]].element);
    }
}

/* This is the required recursive permutation function. 
 * Note: main1.c must generate the complete permutation before checking any constraints. */
void printperms(int perm[], int used[], int k, int n, int *found)
{
    // Base case: a complete permutation has been created
    if (k == n)
    {
        // Check all constraints after the permutation is complete
        if (checkConstraints(perm, n))
        {
            // Print the permutation 
            print(perm, n);
            // A valid solution was found
            *found = 1;
        }
        // Return to previous recursive call
        return;
    }

    // Go through every monster at the current positon 
    for (int i = 0; i < n; i++)
    {
        // Only use a monster if it has not already been used
        if (!used[i])
        {
            // Mark this monster as used
            used[i] = 1;
            // Place the monster in the current position
            perm[k] = i;
            // Recursively fill the next position
            printperms(perm, used, k + 1, n, found);
            // Unmark the monster so it can be used in another permutation
            used[i] = 0;
            // If a valid permutation has been found, stop searching
            if (*found)
            {
                return;
            }
        }
    }
}

/* This function frees all dynamically allocated monster names. */
void freeMemory(int n)
{
    // Go through every monster
    for (int i = 0; i < n; i++)
    {
        // Free the memory that was allocated for the monster name
        free(monsters[i].name);
    }
}

int main(void)
{
    // Variables for monster couunt, constraint count and name length 
    int n, c, length;

    // Stores the current permutation
    int perm[MAX_MONSTERS];
    // Keeps track of which monsters have been used
    int used[MAX_MONSTERS] = {0};
    // Stores whether a valid permutation has been found
    int found = 0;

    // Temporary variables used when reading monster information
    char type[MAX_STRING], name[MAX_STRING], element[MAX_STRING];

    // Read the number of monsters
    scanf("%d", &n);
    // Read the monster information 
    for (int i = 0; i < n; i++)
    {
        // Read the monster name and element
        scanf("%30s %30s", name, element);
        // Dynamically allocate the monster name based on the length of the name.
        length = (int)strlen(name);
        // Allocate enough memory for the name and null terminator
        monsters[i].name = malloc((length + 1) * sizeof(char));
        // If the the memory allocation failed, free any names that were already allocated
        if (monsters[i].name == NULL)
        {
            freeMemory(i);
            return 1;
        }
        // Copy the name into the newly allocated memory
        strcpy(monsters[i].name, name);
        // Copy the element into the monster struct
        strcpy(monsters[i].element, element);
    }

    // Read the number of constraints.
    scanf("%d", &c);
    // Read each constraint 
    for (int i = 0; i < c; i++)
    {
        // Read the constraint type
        scanf("%30s", type);
        // Store the constraint type in the current constraint
        strcpy(constraints[i].type, type);
        // Check for BEFORE or IMMEDIATELY_BEFORE
        if (strcmp(type, "BEFORE") == 0 || strcmp(type, "IMMEDIATELY_BEFORE") == 0)
        {
            // Read the two monster indices
            scanf("%d %d", &constraints[i].a, &constraints[i].b);
        }
        // Check for POSITION
        else if (strcmp(type, "POSITION") == 0)
        {
            // Read the monster index and required position
            scanf("%d %d", &constraints[i].a, &constraints[i].position);
        }
        // Check for FIRST_ELEMENT, LAST_ELEMENT or NO_ADJACENT_ELEMENT
        else if (strcmp(type, "FIRST_ELEMENT") == 0 ||
                 strcmp(type, "LAST_ELEMENT") == 0 ||
                 strcmp(type, "NO_ADJACENT_ELEMENT") == 0)
        {
            // Read the required element
            scanf("%30s", constraints[i].elementA);
        }
        // Check for ELEMENT_BEFORE_ALL
        else if (strcmp(type, "ELEMENT_BEFORE_ALL") == 0)
        {
            // Read both elements
            scanf("%30s %30s", constraints[i].elementA, constraints[i].elementB);
        }
    }

    // Mark the end of the constraints
    if (c < MAX_CONSTRAINTS)
    {
        // Add an empty constraint after the last real constraint
        constraints[c].type[0] = '\0';
    }

    // Start generating permutations
    printperms(perm, used, 0, n, &found);

    // Free all dynamically allocated memory
    freeMemory(n);

    return 0;
}