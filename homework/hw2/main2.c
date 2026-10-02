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

/* This recursive function finds the position of a monster in the current permutation. */
int recursiveFindPosition(int perm[], int k, int monsterIdx)
{
    // If there are no more positions to search, the monster was not found
    if (k < 0)
    {
        return -1;
    }
    // Check whether the current position contains the requested monster
    if (perm[k] == monsterIdx)
    {
        return k;
    }
    // Recursively search the previous position
    return recursiveFindPosition(perm, k - 1, monsterIdx);
}

/* This recursive function checks ELEMENT_BEFORE_ALL constraint.
 * Every monster with element A must appear before every monsters with element B. */
int recursiveCheckElementOrder(int perm[], const char *elementA, const char *elementB, int k, int n, int foundB)
{
    // If every position has been checked, the constraint is valid
    if (k == 0)
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
    // Recursively check the next position toward the beginning
    return recursiveCheckElementOrder(perm, elementA, elementB, k + 1, n, foundB);
}

/* This function checks all constraints after a complete permutation has been created.
 * Note: main2.c also performs partial checking while creating the permutation, 
 * but all constraints are checked here once the permutation is complete. */
int checkConstraints(int perm[], int n, int c)
{
    // Monster position variables
    int posA, posB;

    // Go through every stored constraint
    for (int i = 0; i < c; i++)
    {
        // 1. BEFORE
        if (strcmp(constraints[i].type, "BEFORE") == 0)
        {
            // Find position of monster A
            posA = recursiveFindPosition(perm, n - 1, constraints[i].a);
            // Find position of monster B
            posB = recursiveFindPosition(perm, n - 1, constraints[i].b);
            // If monster A does not appear before monster B, return 0
            if (posA >= posB)
            {
                return 0;
            }
        }

        // 2. IMMEDIATELY_BEFORE 
        else if (strcmp(constraints[i].type, "IMMEDIATELY_BEFORE") == 0)
        {
            // Find position of monster A
            posA = recursiveFindPosition(perm, n - 1, constraints[i].a);
            // Find position of monster B
            posB = recursiveFindPosition(perm, n - 1, constraints[i].b);
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
            posA = recursiveFindPosition(perm, n - 1, constraints[i].a);
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
            // Check every pair of neighboring monsters
            for (int j = 0; j < n - 1; j++)
            {
                // If two monsters with the specified element appear next to each other, return 0
                if (strcmp(monsters[perm[j]].element, constraints[i].elementA) == 0 &&
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
            if (!recursiveCheckElementOrder(perm, constraints[i].elementA, constraints[i].elementB, 0, n, 0))
            {
                return 0;
            }
        }
    }

    return 1;
}

/* This function checks whether adding a monster to the current partial permutation would violate a constraint.
 * k is the number of monsters currently in the permutation.
 * The new monster has already been placed at perm[k - 1]. */
int partialCheck(int perm[], int k, int n, int c)
{
    // Monster position variables
    int posA, posB;
    // Stores index of newest monster
    int newMonster;

    // If the permutation is empty, return 1
    if (k == 0)
    {
        return 1;
    }

    // Get the monster that was most recently added
    newMonster = perm[k - 1];

    for (int i = 0; i < c; i++)
    {
        // FIRST_ELEMENT 
        if (strcmp(constraints[i].type, "FIRST_ELEMENT") == 0)
        {
            // If the new monster's element does not match the required element, return 0
            if (k == 1 && strcmp(monsters[newMonster].element, constraints[i].elementA) != 0)
            {
                return 0;
            }
        }

        // NO_ADJACENT_ELEMENT
        // Only the newest monster needs to be compared with the monster immediately before it.
        else if (strcmp(constraints[i].type, "NO_ADJACENT_ELEMENT") == 0)
        {
            // Make sure there are at least two monsters to compare
            if (k >= 2)
            {
                // If two monsters with the specified element appear next to each other, return 0
                if (strcmp(monsters[perm[k - 1]].element, constraints[i].elementA) == 0 &&
                strcmp(monsters[perm[k - 2]].element, constraints[i].elementA) == 0)
                {
                    return 0;
                }
            }
        }

        // BEFORE
        else if (strcmp(constraints[i].type, "BEFORE") == 0)
        {
            // Find A in the current partial permutation
            posA = recursiveFindPosition(perm, k - 1, constraints[i].a);
            // Find B in the current partial permutation
            posB = recursiveFindPosition(perm, k - 1, constraints[i].b);
            // If B appeared first, this branch cannot become valid
            if (posB != -1 && posA == -1)
            {
                return 0;
            }
            // If both appeared, make sure A is before B
            if (posA != -1 && posB != -1 && posA >= posB)
            {
                return 0;
            }
        }

        // IMMEDIATELY_BEFORE
        else if (strcmp(constraints[i].type, "IMMEDIATELY_BEFORE") == 0)
        {
            // Find A in the current partial permutation
            posA = recursiveFindPosition(perm, k - 1, constraints[i].a);
            // Find B in the current partial permutation
            posB = recursiveFindPosition(perm, k - 1, constraints[i].b);
            // If B appeared without A before it, this branch is invalid
            if (posB != -1 && posA == -1)
            {
                return 0;
            }
            // If both have appeared, B must immediately follow A
            if (posA != -1 && posB != -1)
            {
                if (posB != posA + 1)
                {
                    return 0;
                }
            }
            // If A is currently the last monster and there are no more positions available, B can never follow A.
            if (posA == k - 1 &&
                posB == -1 &&
                k == n)
            {
                return 0;
            }
        }

        // ELEMENT_BEFORE_ALL
        else if (strcmp(constraints[i].type, "ELEMENT_BEFORE_ALL") == 0)
        {
            // Only check when the newest monster is elementA
            if (strcmp(monsters[newMonster].element, constraints[i].elementA) == 0)
            {
                // Search the earlier positions for elementB
                for (int j = 0; j < k - 1; j++)
                {
                    // If elementB already appeared, this branch is invalid
                    if (strcmp(monsters[perm[j]].element, constraints[i].elementB) == 0)
                    {
                        return 0;
                    }
                }
            }
        }

        // LAST_ELEMENT
        else if (strcmp(constraints[i].type, "LAST_ELEMENT") == 0)
        {
            // Only check the last element when all monsters are placed and return 0 if the last element is wrong
            if (k == n && strcmp(monsters[newMonster].element, constraints[i].elementA) != 0)
            {
                return 0;
            }
        }
    }

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
 * main2.c checks constraints while building the permutation
 * so that invalid branches can be pruned early. */
void printperms(int perm[], int used[], int k, int n, int c, int *found)
{
    // Base case: a complete permutation has been created
    if (k == n)
    {
        /// Check all constraints after the permutation is complete
        if (checkConstraints(perm, n, c))
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
        // Assume the monster is allowed at this position
        int allowed = 1;

        // Check POSITION constraints before placing the monster.
        for (int j = 0; j < c; j++)
        {
            // Check whether the current constraint is POSITION
            if (strcmp(constraints[j].type, "POSITION") == 0)
            {
                // Check whether this constraint applies to the current position
                if (constraints[j].position == k + 1 && constraints[j].a != i)
                {
                    // This monster cannot be placed here
                    allowed = 0;
                    // Stop checking POSITION constraints for this monster
                    break;
                }
            }
        }

        // Skip this monster if it is not allowed in the current position
        if (!allowed)
        {
            continue;
        }

        // Only use a monster if it has not already been used
        if (!used[i])
        {
            // Mark this monster as used
            used[i] = 1;
            // Place the monster in the current position
            perm[k] = i;
            // Check whether the partial permutation is still valid
            if (partialCheck(perm, k + 1, n, c))
            {
                // Recursively fill the next position
                printperms(perm, used, k + 1, n, c, found);
            }
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
    char name[MAX_STRING], element[MAX_STRING], type[MAX_STRING];

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

    // Read each constraint. 
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
        // Check for FIRST_ELEMENT, LAST_ELEMENT, or NO_ADJACENT_ELEMENT
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

    // Start generating permutations. 
    printperms(perm, used, 0, n, c, &found);

    // Free all dynamically allocated memory. 
    freeMemory(n);

    return 0;
}