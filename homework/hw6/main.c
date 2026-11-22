/* COP 3502C PA6
This program is written by: Hailey Simpson */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 5
#define LOAD_FACTOR_THRESHOLD 0.75

// Monster Structure
typedef struct Monster 
{
    char *name;
    char element[31];
    int power;
} Monster;

// Hash Node Structure
typedef struct HashNode 
{
    Monster *monster;
    struct HashNode *next;
} HashNode;

// Hash Table Structure
typedef struct HashTable 
{
    int capacity;
    int size;
    HashNode **buckets;
} HashTable;

// Function Prototypes 
unsigned long hashFunction(char *name, int capacity);
void rehashTable(HashTable *ht);
HashTable *createHashTable();
Monster *createMonster(char *name, char *element, int power);
void insertMonster(HashTable *ht, char *name, char *element, int power);
void removeMonster(HashTable *ht, char *name);
void findMonster(HashTable *ht, char *name);
void displayBucket(HashTable *ht, int index);
void displayStatistics(HashTable *ht);
void displayCodex(HashTable *ht);
void freeHashTable(HashTable *ht);

int main(void)
{
    int num;
    int operation;

    // Create the hash table
    HashTable *ht = createHashTable();

    // Read the number of operations
    scanf("%d", &num);

    // Process each operation
    for (int i = 0; i < num; i++) 
    {
        scanf("%d", &operation);

        // Operation 1: Register Or Update A Monster
        if (operation == 1)
        {

            char name[31];
            char element[31];
            int power;
            // Read the monster's name, element, and power score
            scanf("%30s %30s %d", name, element, &power);
            // Insert the monster or update it if the name already exists
            insertMonster(ht, name, element, power);
        }

        // Operation 2: Remove A Monster
        else if (operation == 2) 
        {
            char name[31];
            // Read the monster's name
            scanf("%30s", name);
            // Search for the monster, remove it, and free its memory
            removeMonster(ht, name);
        }

        // Operation 3: Find A Monster
        else if (operation == 3) 
        {
            char name[31];
            // Read the monster's name
            scanf("%30s", name);
            // Search for the monster and print its information if found
            findMonster(ht, name);
        }

        // Operation 4: Display A Specific Bucket Chain
        else if (operation == 4) 
        {
            int index;
            // Read the bucket index
            scanf("%d", &index);
            // Display the selected bucket or print an error if invalid
            displayBucket(ht, index);
        }

        // Operation 5: Display Codex Statistics
        else if (operation == 5) 
        {
            // Print the current capacity, size, and load factor
            displayStatistics(ht);
        }

        // Operation 6: Display Entire Codex
        else if (operation == 6) 
        {
            // Print the entire codex, including all stored monsters
            displayCodex(ht);
        }
    }

    // Free all dynamically allocated memory.
    freeHashTable(ht);

    return 0;
}

// Calculates the bucket index for a monster using its name
unsigned long hashFunction(char *name, int capacity)
{
    // Initialize the hash value to 0
    unsigned long hash = 0;
    // Go through each character in the monster's name
    for (int i = 0; name[i] != '\0'; i++) 
    {
        // Calculate the new hash value using the current character
        hash = (hash * 31 + (unsigned char) name[i]) % capacity;
    }
    // Return the calculated bucket index
    return hash;
}

// Doubles the capacity and redistributes existing nodes
void rehashTable(HashTable *ht)
{
    // Calculate the new capacity
    int newCapacity = ht->capacity * 2;

    // Allocate memory for a new bucket array
    HashNode **newBuckets = malloc(newCapacity * sizeof(HashNode *));

    // Initialize every new bucket to NULL
    for (int i = 0; i < newCapacity; i++) 
    {
        newBuckets[i] = NULL;
    }

    // Go through every bucket in the old array
    for (int i = 0; i < ht->capacity; i++) 
    {
        // Pointer to the first node in the current bucket
        HashNode *curr = ht->buckets[i];

        // Go through the current bucket's linked list.
        while (curr != NULL) 
        {
            // Save the next node before changing curr's next pointer
            HashNode *next = curr->next;

            // Calculate the new bucket index using the hashFunction
            int index = hashFunction(curr->monster->name, newCapacity);

            // Insert the existing node at the head of the new bucket
            curr->next = newBuckets[index];
            newBuckets[index] = curr;

            // Move to the next node from the original linked list
            curr = next;
        }
    }

    // Free the old bucket array
    free(ht->buckets);
    // Update the hash table to use the new bucket array
    ht->buckets = newBuckets;
    // Update the table capacity
    ht->capacity = newCapacity;

    // Print REHASHED TO <newCapacity>
    printf("REHASHED TO %d\n", newCapacity);
}

// Creates a hash table and allocates memory for its information
HashTable *createHashTable() 
{

    // Allocate memory for the hash table
    HashTable *ht = malloc(sizeof(HashTable));

    // Set the initial capacity and size
    ht->capacity = INITIAL_CAPACITY;
    ht->size = 0;

    // Allocate the bucket array
    ht->buckets = malloc(ht->capacity * sizeof(HashNode *));

    // Initialize every bucket to NULL
    for (int i = 0; i < ht->capacity; i++) {
        ht->buckets[i] = NULL;
    }

    // Return the pointer to the completed hash table
    return ht;
}

// Creates a monster and allocates memory for its information
Monster *createMonster(char *name, char *element, int power)
{
    // Allocate memory for one Monster structure
    Monster *m = malloc(sizeof(Monster));
    
    // Allocate exact memory for the monster name and copy it
    m->name = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(m->name, name);

    // Copy the element into the structure 
    strcpy(m->element, element);

    // Copy the power score into the structure 
    m->power = power;

    // Return the pointer to the completed monster
    return m;
}

// Inserts a monster or updates an existing one
void insertMonster(HashTable *ht, char *name, char *element, int power) 
{
    // Calculate the bucket index using the hash function
    int index = hashFunction(name, ht->capacity);

    // Pointer to the first node in the bucket's linked list
    HashNode *curr = ht->buckets[index];

    // Search for an existing monster with the same name
    while (curr != NULL) 
    {
        // Check whether the names match
        if (strcmp(curr->monster->name, name) == 0) 
        {
            // Update the existing monster's information
            strcpy(curr->monster->element, element);
            curr->monster->power = power;
            // Print UPDATED
            printf("UPDATED\n");
            return;
        }
        // Move to the next node
        curr = curr->next;
    }

    // Monster does not exist, so allocate memory for a Monster structure and a HashNode
    Monster *monster = createMonster(name, element, power);
    HashNode *newNode = malloc(sizeof(HashNode));

    // Store the monster inside the node
    newNode->monster = monster;
    // Insert the node at the head of the bucket and increase the number of monsters
    newNode->next = ht->buckets[index];
    ht->buckets[index] = newNode;
    ht->size++;
    // Print INSERTED
    printf("INSERTED\n");

    // Calculate the current load factor
    double loadFactor = (double) ht->size / ht->capacity;
    // Rehash if the load factor exceeds the threshold
    if (loadFactor > LOAD_FACTOR_THRESHOLD) 
    {
        rehashTable(ht);
    }
}

// Removes a monster and frees its memory
void removeMonster(HashTable *ht, char *name) 
{
    // Calculate the bucket index using the hash function
    int index = hashFunction(name, ht->capacity);

    // Pointer to the first node in the bucket's linked list
    HashNode *curr = ht->buckets[index];
    // Pointer to the node before curr
    HashNode *prev = NULL;

    // Search the linked list for the monster that will be removed
    while (curr != NULL) 
    {
        // Check whether the monster names match
        if (strcmp(curr->monster->name, name) == 0)
        {
            // If prev is NULL, the monster is the first node
            if (prev == NULL) 
            {
                ht->buckets[index] = curr->next;
            }
            // Otherwise, skip over the node being deleted
            else 
            {
                prev->next = curr->next;
            }

            // Free the monster's dynamically allocated name
            free(curr->monster->name);
            // Free the Monster structure
            free(curr->monster);
            // Free the HashNode
            free(curr);
            // Decrease the number of monsters
            ht->size--;
            // Print DELETED
            printf("DELETED\n");
            return;
        }
        // Move prev to the current node
        prev = curr;
        // Move curr to the next node
        curr = curr->next;
    }
    // Print NOT FOUND
    printf("NOT FOUND\n");
}

// Searches for a monster by name
void findMonster(HashTable *ht, char *name) 
{
    // Calculate the bucket index using the hash function
    int index = hashFunction(name, ht->capacity);

    // Pointer to the first node in the bucket's linked list
    HashNode *curr = ht->buckets[index];

    // Search through the linked list
    while (curr != NULL) 
    {
        // Check whether the monster names match
        if (strcmp(curr->monster->name, name) == 0) 
        {
            // Print FOUND with the monster's element and power.
            printf("FOUND %s %d\n", curr->monster->element, curr->monster->power);
            return;
        }
        // Move curr to the next node
        curr = curr->next;
    }
    // Print NOT FOUND
    printf("NOT FOUND\n");
}

// Prints one bucket's linked list
void displayBucket(HashTable *ht, int index) 
{   
    // Check whether the bucket index is valid
    if (index < 0 || index >= ht->capacity) 
    {
        // Print INVALID BUCKET
        printf("INVALID BUCKET\n");
        return;
    }
    // Pointer to the first node in the selected bucket
    HashNode *curr = ht->buckets[index];
    
    // Check whether the bucket is empty
    if (curr == NULL) 
    {
        // Print EMPTY
        printf("EMPTY\n");
        return;
    }

    // Print the bucket index
    printf("Bucket %d: ", index);
    // Go through the linked list
    while (curr != NULL) 
    {
        // Print the monster's information
        printf("%s (%s, %d)", curr->monster->name, curr->monster->element, curr->monster->power);
        // Print an arrow if another node follows
        if (curr->next != NULL) 
        {
            printf(" -> ");
        }
        // Move curr to the next node.
        curr = curr->next;
    }
    printf("\n");
}

// Prints capacity, size, and load factor
void displayStatistics(HashTable *ht) 
{
    // Calculate the current load factor
    double loadFactor = (double) ht->size / ht->capacity;
    // Print the table statistics
    printf("Capacity: %d, Size: %d, Load Factor: %.2f\n", ht->capacity, ht->size, loadFactor);
}

// Prints every nonempty bucket
void displayCodex(HashTable *ht) 
{
    // Check whether the entire hash table is empty
    if (ht->size == 0)
    {
        // Print EMPTY CODEX
        printf("EMPTY CODEX\n");
        return;
    }

    // Go through every bucket in the hash table
    for (int i = 0; i < ht->capacity; i++) 
    {

        // Display only buckets that contain monsters
        if (ht->buckets[i] != NULL) 
        {
            displayBucket(ht, i);
        }
    }
}

// Frees all remaining dynamically allocated memory
void freeHashTable(HashTable *ht) 
{
    // Go through every bucket in the hash table
    for (int i = 0; i < ht->capacity; i++) 
    {
        // Pointer to the first node in the current bucket
        HashNode *curr = ht->buckets[i];

        // Go through the linked list
        while (curr != NULL) 
        {
            // Save the next node before freeing curr
            HashNode *next = curr->next;

            // Free the monster's dynamically allocated name
            free(curr->monster->name);
            // Free the Monster structure
            free(curr->monster);
            // Free the HashNode
            free(curr);

            // Move to the next node
            curr = next;
        }
    }

    // Free the bucket pointer array
    free(ht->buckets);
    // Free the HashTable structure
    free(ht);
}
