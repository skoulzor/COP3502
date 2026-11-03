/* COP 3502C PA4
This program is written by: Hailey Simpson */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 30      // Maximum element name length
#define NUMSCORES 4     // Number of scores stored per monster
#define POWER 0         // Index of the power score
#define SPEED 1         // Index of the speed score
#define STAMINA 2       // Index of the stamina score
#define TOTAL 3         // Index of the total score
#define BASECASESIZE 30 // Use insertion sort for sections of at most 30

// Monster struct
typedef struct Monster 
{
    char *name;
    char element[MAXSIZE + 1];
    int scores[NUMSCORES];
} Monster;

// Function prototypes
Monster *createMonster(char *name, char element[], int scores[]);
Monster **readMonsters(int monster_cnt);
int compareTo(Monster *m1, Monster *m2, int key);
void insertionSort(Monster **list, int low, int high, int key);
void merge(Monster **list, int low, int mid, int high, int key);
void mergeSort(Monster **list, int n, int key);
void mergeSortRec(Monster **list, int low, int high, int key);
void swap(Monster **a, Monster **b);
int partition(Monster **list, int low, int high, int key);
void quickSort(Monster **list, int n, int key);
void quickSortRec(Monster **list, int low, int high, int key);
void freeMemory(Monster **list, int monster_cnt);

int main() 
{
    // Reads and stores the umber of monsters
    int monster_cnt;
    scanf("%d", &monster_cnt);

    // Reads the monsters and stores their pointers for the total ranking
    Monster **overallList = readMonsters(monster_cnt);
    
    // Allocates another pointer array for the speed ranking
    Monster **speedList = malloc (monster_cnt * sizeof(Monster*));
    // Copies the pointers so both arrays reference the same monsters
    for (int i = 0; i < monster_cnt; i++)
    {
        speedList[i] = overallList[i];
    }

    // Sorts by total using merge sort
    mergeSort(overallList, monster_cnt, TOTAL);
    // Sorts by speed score using quick sort
    quickSort(speedList, monster_cnt, SPEED);
    
    // Prints the monsters sorted by total 
    printf("Overall Expedition Ranking\n");
    for(int i = 0; i < monster_cnt; i++)
    {
        printf("%d. %-15s %d\n", i + 1, overallList[i]->name, overallList[i]->scores[TOTAL]);
    }
    // Prints the monsters sorted by speed
    printf("\nSpeed Challenge Ranking\n");
    for(int i = 0; i < monster_cnt; i++)
    {
        printf("%d. %-15s %d\n", i + 1, speedList[i]->name, speedList[i]->scores[SPEED]);
    }

    // Free the seperate array of pointers used for speed sorting
    free(speedList);
    // Free each monster and the overall pointer array
    freeMemory(overallList, monster_cnt);

    // Ends the program
    return 0;
}

// Creates a monster and allocates memory for its information
Monster *createMonster(char *name, char element[], int scores[]) 
{
    // Allocate memory for one Monster structure
    Monster *m = malloc(sizeof(Monster));
    
    // Allocate exact memory for the monster name and copy it
    m->name = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(m->name, name);
    // Copy the element into the structure 
    strcpy(m->element, element);
    // Copy the three individual scores
    for (int i = 0; i < NUMSCORES - 1; i++) 
    {
        m->scores[i] = scores[i];
    }
    // Calculate the total score
    m->scores[TOTAL] = m->scores[POWER] + m->scores[SPEED] + m->scores[STAMINA];

    // Return the pointer to the completed monster.
    return m;
}

// Reads the monsters and returns an array of Monster pointers
Monster **readMonsters(int monster_cnt)
{
    // Allocate an array containing one pointer per monster.
    Monster **list = malloc(monster_cnt * sizeof(Monster*));
    
    // Read each monster's information.
    for (int i = 0; i < monster_cnt; i++)
    {
        char name[100];
        char element[MAXSIZE + 1];
        int scores[NUMSCORES];

        // Read the name, element, power, speed, and stamina.
        scanf("%99s %30s %d %d %d", name, element, &scores[POWER], &scores[SPEED], &scores[STAMINA]);
        // Create the monster and save its pointer.
        list[i] = createMonster(name, element, scores);
    }

    // Return the array of Monster pointers.
    return list;
}

// Compares monsters by score and then alphabetically by name
int compareTo(Monster *m1, Monster *m2, int key) 
{
    // If m2's score is higher than m1's, return -1
    if (m1->scores[key] > m2->scores[key])
    {
        return -1;
    }
    // If m2's score is lower than m1's, return 1
    if (m1->scores[key] < m2->scores[key])
    {
        return 1;
    }
    // If the scores tie, compara the names alphabetically and return the result
    return strcmp(m1->name, m2->name);
}

// Sorts a section of the array using insertion sort
void insertionSort(Monster **list, int low, int high, int key)
{
    Monster *item;
    int i, j;

    // Start with the second element of the selected section
    for (i = low + 1; i <= high; i++)
    {
        // Save the current monster
        item = list[i];
        
        // Move elements of list[0..i-1], that are greater than key, to one position ahead of their current position 
        for(j = i - 1; j >= low; j--)
        {
            if(compareTo(list[j], item, key) > 0)
            {
                list[j + 1] = list[j];
            }
            else
            {
                break;
            }
        }
        // Insert the saved monster into the open position
        list[j + 1] = item;
    }
}

// Merges two sorted sections of the array
void merge(Monster **list, int low, int mid, int high, int key)
{
    // Merges two subarrays of list[].
    // First subarray is list[low..mid]
    // Second subarray is list[mid+1..high] 
    int i, j, k;
    int n1 = mid - low + 1;
    int n2 = high - mid;

    // Create temp arrays
    Monster **L = malloc(n1 * sizeof(Monster*));
    Monster **H = malloc(n2 * sizeof(Monster*));
    
    // Copy data to temp arrays L[] and H[] 
    for (i = 0; i < n1; i++)
    {
        L[i] = list[low + i];
    }
    for (j = 0; j < n2; j++)
    {
        H[j] = list[mid + 1 + j];
    }
   
    // Merge the temp arrays back into arr[low..high]
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = low; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (compareTo(L[i], H[j], key) <= 0)
        {
            list[k] = L[i];
            i++;
        }
        else
        {
            list[k] = H[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any 
    while (i < n1)
    {
        list[k] = L[i];
        i++;
        k++;
    }
    // Copy the remaining elements of H[], if there are any 
    while (j < n2)
    {
        list[k] = H[j];
        j++;
        k++;
    }

    // Release the temporary arrays.
    free(L);
    free(H);
}

// Starts merge sort on the entire array
void mergeSort(Monster **list, int n, int key)
{
    // Starts merge sort on the entire array and sorts from index 0 through index n - 1
    mergeSortRec(list, 0, n - 1, key);
}

// Recursively sorts sections using merge sort
void mergeSortRec(Monster **list, int low, int high, int key)
{
    // Continue while the section has at least two elements
    if (low < high)
    {
        // Use insertion sort for a small section
        if (high - low + 1 <= BASECASESIZE)
        {
            insertionSort(list, low, high, key); 
        }
        else 
        {
            // Get the mid point
            int mid = (low + high) / 2;
            
            // Sort first and second halves
            mergeSortRec(list, low, mid, key);
            mergeSortRec(list, mid + 1, high, key);
        
            // Merge the two sorted halves.
            merge(list, low, mid, high, key);
        }
    }
}

// Swaps two Monster pointers
void swap(Monster **a, Monster **b)
{
    // Temporarily save the first pointer
    Monster *temp = *a;
    // Make the first pointer reference the second monster
    *a = *b;
    // Make the second pointer reference the original first monster
    *b = temp; 
}

// Partitions the array around a randomly selected pivot
int partition(Monster **list, int low, int high, int key)
{
    // Pick a random partition element and swap it into index low
    int i = low + rand() % (high - low + 1);
    swap(&list[low], &list[i]);

    int lowpos = low; // here is our piot location
    low++; // our starting point is after the pivot

    // Run the partition so long as the low and high counters don't cross.
    while (low <= high)
    {
        // Move the low pointer until we find a value too large for this side.
        while(low <= high && compareTo(list[low], list[lowpos], key) < 0)
        {
            low++;
        }
        // Move the high pointer until we find a value too small for this side.
        while(high >= low && compareTo(list[high], list[lowpos], key) >= 0)
        {
            high--;
        }
        // Now that we've identified two values on the wrong side, swap them.
        if (low < high)
        {
            swap(&list[low], &list[high]);
        }
    }
    // Swap the pivot element element into its correct location.
    swap(&list[lowpos], &list[high]);

    // Return the partition point
    return high;
}

// Starts quick sort on the entire array
void quickSort(Monster **list, int n, int key)
{
    // Sort from index 0 through index n - 1
    quickSortRec(list, 0, n - 1, key);
}

// Recursively sorts sections using quick sort
void quickSortRec(Monster **list, int low, int high, int key)
{
    // Only have to sort if we are sorting more than one number
    if (low < high)
    {
        // Use insertion sort for a small section
        if (high - low + 1 <= BASECASESIZE)
        {
            insertionSort(list, low, high, key); 
        }
        else 
        {
            // Partition the section and obtain the pivot position
            int split = partition(list, low, high, key);
            // Sort the section before the pivot
            quickSortRec(list, low, split - 1, key);
            // Sort the section after the pivot
            quickSortRec(list, split + 1, high, key);
        }
    }
}

// Frees the allocated memory used by the monsters
void freeMemory(Monster **list, int monster_cnt) 
{
    // Loop through every monster in the array
    for (int i = 0; i < monster_cnt; i++) 
    {
        // Free the dynamically allocated name
        free(list[i]->name);
        // Free the Monster structure
        free(list[i]);
    }
    // Free the array of Monster pointers
    free(list);
}