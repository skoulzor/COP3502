/* COP 3502C PA3
This program is written by: Hailey Simpson */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "leak_detector_c.h"

/* Monster Struct */
typedef struct Monster 
{
    char name[51];
    char element[31];
    int level;
    int hp;
} Monster;

/* Node Struct */
typedef struct Node 
{
    Monster* monster;
    struct Node* next;
} Node;

/* Queue Struct */
typedef struct Queue 
{
    Node* front;
    Node* back;
    int size;
} Queue;

/* WillsPC Struct */
typedef struct WillsPC 
{
    Queue* storageBox; // FIFO Queue for Will's PC Storage Box
    Node* activeRosterHead; //Singly Linked List for sorted active roster
} WillsPC;

/* Dynamically allocates and initializes an empty Queue */
Queue* createQueue(void) 
{
    // Dynamically allocates memory for a Queue
    Queue* q = malloc(sizeof(Queue));
    // The queue starts with no front node, no back node and zero monsters
    q->front = NULL;
    q->back = NULL;
    q->size = 0;
    // Returns the new empty queue
    return q;
}

/* Wraps m inside a new Node and appends it to the back of q */
void enqueue(Queue* q, Monster* m) 
{
    // Dynamically allocates memory for a new node
    Node* temp = malloc(sizeof(Node));
    // Stores the Monster pointer in the node
    temp->monster = m;
    // Makes the new node the last node
    temp->next = NULL;

    // If the queue if empty, the new node becomes the front and the back
    if (q->front == NULL) 
    {
        q->front = temp;
        q->back = temp;
    }
    // Else, connect the old back node to the new node and move the back pointer to the new node
    else 
    {
        q->back->next = temp;
        q->back = temp;
    }
    
    // Increases the number of nodes in the queue 
    q->size++;
}

/* Removes the front node from q */
Monster* dequeue(Queue* q) 
{
    // Pointers for the node being removed and saving the Monster
    Node* temp;
    Monster* m;

    // If the queue is empty, return NULL
    if (q->front == NULL) 
    {
        return NULL;
    }

    // Stores the current front node
    temp = q->front;
    // Stores the Monster pointer from that node
    m = temp->monster;
    // Moves the front pointer to the next node
    q->front = q->front->next;

    // If the queue is empty, the back must become NULL
    if (q->front == NULL) 
    {
        q->back = NULL;
    }

    // Decreases the queue size
    q->size--;
    // Frees the removed Node container
    free(temp);
    // Returns the Monster pointer
    return m;
}

/* Inserts m into the list maintained in descending order of level 
 * If levels are equal, higher HP comes first */
Node* insertRosterSorted(Node* head, Monster* m) 
{
    // Dynamically allocates memory for the new roster node
    Node* temp = malloc(sizeof(Node));
    // Pointer for searching the roster
    Node* curr;
    // Stores the Monster pointer in the new node
    temp->monster = m;
    // New node does not point anywhere
    temp->next = NULL;

    // If the roster is empty, OR if the new monster has a higher level than the current first monster,
    // OR if the two monsters have the same level and the new monster has higher hp than the current first monster, proceed
    if (head == NULL || m->level > head->monster->level ||
        (m->level == head->monster->level && m->hp > head->monster->hp))
    {
        // New node points to the old first node and becomes the the new head
        temp->next = head;
        head = temp;
        // Returns the updated head
        return head;
    }

    // Start searching from the first node
    curr = head;

    // Continue moving through the roster while there is another node AND the next monster has a higher level than m 
    // or the next monster has the same level as m and has equal or higher HP
    while (curr->next != NULL && (curr->next->monster->level > m->level || 
        (curr->next->monster->level == m->level && curr->next->monster->hp >= m->hp))) 
    {
        // Moves to the next node
        curr = curr->next;
    }

    // New node points to the next node and current node points to the new node
    temp->next = curr->next;
    curr->next = temp;
    // Returns the unchanged head
    return head;
}

/* Removes a monster from the active roster and puts it at the back of the PC queue. */
Node* depositBackToPC(Node* head, Queue* q, const char* monsterName) 
{
    // Pointers for searching the roster, the node before curr, and storing the Monster being moved
    Node* curr;
    Node* prev;
    Monster* m;

    // The search starts from the head and there is not previous node initially 
    curr = head;
    prev = NULL;

    // Searches through the active roster
    while (curr != NULL) 
    {
        // If the current monster matches the requested name, proceed  
        if (strcmp(curr->monster->name, monsterName) == 0) 
        {
            // Saves the Monster pointer
            m = curr->monster;
            // If the monster is the first node, move the head to the next node
            if(prev == NULL)
            {
                head = curr->next;
            }
            // Else, skip over the current node
            else 
            {
                prev->next = curr->next;
            }
            // Frees the removed Node container
            free(curr);
            // Puts the Monster at the back of the queue 
            enqueue(q, m);
            // Prints the return statement
            printf("[EVENT] Returned %s from Active Roster back to Will's PC.\n", monsterName);
            // Returns the updated roster head
            return head;
        }
        // Moves the previous pointer forward and current pointer forward
        prev = curr;
        curr = curr->next;
    }

    // Prints error statement if monster was not found
    printf("[ERROR] %s is not in the Active Roster.\n", monsterName);
    // Returns the roster head
    return head;
}

/* Removes and frees all monsters whose HP is below minHp */
Node* releaseFainted(Node* head, int minHp, int* releasedCount) 
{
    // Pointers for the current node, node before curr and node being removed
    Node* curr;
    Node* prev;
    Node* temp;

    // The search starts from the head and there is not previous node initially 
    curr = head;
    prev = NULL;

    // Searches through the entire roster
    while (curr != NULL) 
    {
        // If the current monster has less HP than th e minimum, proceed
        if (curr->monster->hp < minHp)
        {
            // Saves the node that will be removed
            temp = curr;
            // If the first node is being removed, move the head to the next node and continue from the new head
            if (prev == NULL)
            {
                head = curr->next;
                curr = head;
            }
            // Else, remove curr from the linked list and move to the next remaining node
            else
            {
                prev->next = curr->next;
                curr = prev->next;
            }
            // Frees the Monster structure
            free(temp->monster);
            // Frees the Node structure 
            free(temp);
            // Increases the number of released monsters 
            (*releasedCount)++;
        }
        // Else, move the previous pointer forward and current pointer forward
        else
        {
            prev = curr;
            curr = curr->next;
        }
    }

    // Returns the updated roster head
    return head;
}

/* Prints the current status of both storageBox and activeRosterHead */
void displayStorage(WillsPC* pc) 
{
    // Pointer to tranverse the lists
    Node* curr;
    // Stores the number of active roster nodes
    int rosterSize;

    // Prints the beginning of the status display and current queue size 
    printf("--- WILL'S PC STATUS ---\n");
    printf("Storage Box Queue (%d): ", pc->storageBox->size);

    // If the storage queue is empty, print EMPTY 
    if (pc->storageBox->front == NULL)
    {
        printf("EMPTY\n");
    }
    // Else, proceed
    else 
    {
        // Starts at the front of the queue 
        curr = pc->storageBox->front;
        // Searches the queue from front to back
        while (curr != NULL) 
        {
            // Prints the current monster's name and level
            printf("%s [Lv: %d]", curr->monster->name, curr->monster->level);
            // Prints an arrow if another node follows
            if (curr->next != NULL)
            {
                printf(" -> ");
            }
            // Moves to the next queue node
            curr = curr->next;
        }
        // Moves to the next line
        printf("\n");
    }

    // Start the roster size at zero and start at the roster head
    rosterSize = 0;
    curr = pc->activeRosterHead;

    // Count the number of active roster nodes
    while (curr != NULL)
    {
        // Increases the count and move to the next roster node
        rosterSize++;
        curr = curr->next;
    }
    // Prints the roster size
    printf("Active Roster (%d): ", rosterSize);

    // If the active roster is empty, print EMPTY
    if (pc->activeRosterHead == NULL)
    {
        printf("EMPTY\n");
    }
    // Else, proceed 
    else
    {
        // Starts at the first active monster
        curr = pc->activeRosterHead;
        // Searches through the active roster 
        while (curr != NULL)
        {
            // Prints the monster's name and level
            printf("%s [Lv: %d]", curr->monster->name, curr->monster->level);
            // Prints an arrow if another node follows
            if (curr->next != NULL)
            {
                printf(" -> ");
            }
            // Moves to the next roster node
            curr = curr->next;
        }
        // Moves to the next line
        printf("\n");
    }
    // Prints the bottom border
    printf("------------------------\n");
}

/* Creates Will's PC */
WillsPC* createWillsPC(void) 
{
    // Dynamically allocates memory for the WillsPC structure 
    WillsPC* pc = malloc(sizeof(WillsPC));
    // Creates the storage queue 
    pc->storageBox = createQueue();
    // The active roster starts empty
    pc->activeRosterHead = NULL;
    // Returns the newly created PC
    return pc;
}

/* Frees all remaining memory */
void releaseWillsPC(WillsPC* pc) 
{
    // Pointers to tranverse the lists and temporarily store nodes 
    Node* curr;
    Node* temp;

    // Starts at the front of the storage queue 
    curr = pc->storageBox->front;
    // Frees every remaining queue node and Monster
    while (curr != NULL)
    {
        // Saves the current node
        temp = curr;
        // Moves to the next node
        curr = curr->next;
        // Frees the Monster stored in the node
        free(temp->monster);
        // Frees the Node itself
        free(temp);
    }
    
    // Starts at the head of the active roster
    curr = pc->activeRosterHead;
    // Frees every remaining roster node and Monster
    while (curr != NULL)
    {
        // Saves the current node
        temp = curr;
        // Moves to the next node
        curr = curr->next;
        // Frees the Monster stored in the node
        free(temp->monster);
        // Frees the Node itself
        free(temp);
    }

    // Frees the queue structure
    free(pc->storageBox);
    // Frees the WillsPC structure
    free(pc);
}

int main(void) 
{
    // for memory leak detector atexit(report_mem_leak);
    // Pointer to Will's PC
    WillsPC* pc;
    // Stores the command entered by the user
    char command[51];
    // Creates and initializes Will's PC
    pc = createWillsPC();

    // Continues reading commands until there is no more input
    while (scanf("%50s", command) == 1)
    {
        // DEPOSIT_PC command
        if (strcmp(command, "DEPOSIT_PC") == 0)
        {
            // Dynamically allocates a new Monster
            Monster* m = malloc(sizeof(Monster));
            // Reads the monster's information 
            scanf("%50s %30s %d %d", m->name, m->element, &m->level, &m->hp);
            // Adds the monster to the back of the PC queue 
            enqueue(pc->storageBox, m);
            // Prints the required event message
            printf("[EVENT] Deposited to Will's PC: %s (%s, Lv: %d, HP: %d)\n",
                m->name, m->element, m->level, m->hp);
        }
        // WITHDRAW_PARTY command
        else if (strcmp(command, "WITHDRAW_PARTY") == 0)
        {
            // Number of monsters requested, number withdrawn and pointer to the withdrawn Monster
            int count;
            int withdrawn = 0;
            Monster* m;

            // Reads the requested number
            scanf("%d", &count);

            // Withdraw monsters while there are still monsters requested and the queue is not empty 
            while (withdrawn < count && pc->storageBox->front != NULL)
            {
                // Removes one monster from the front of the queue 
                m = dequeue(pc->storageBox);
                // Inserts it into the active roster in sorted order
                pc->activeRosterHead = insertRosterSorted(pc->activeRosterHead, m);
                // Increases the number withdrawn
                withdrawn++;
            }

            // Prints the number transferred 
            printf("[EVENT] Withdrew %d monster(s) from Will's PC to Active Roster.\n", withdrawn);
        }
        // DEPOSIT_BACK command
        else if (strcmp(command, "DEPOSIT_BACK") == 0)
        {
            // Stores the name to search for
            char monsterName[51];
            // Reads the monster name
            scanf("%50s", monsterName);
            // Removes the monster from the roster and returns it to the queue 
            pc->activeRosterHead = depositBackToPC(pc->activeRosterHead, pc->storageBox, monsterName);
        }
        // RELEASE_FAINTED command
        else if (strcmp(command, "RELEASE_FAINTED") == 0)
        {
            // Minimum HP allowed and number of monsters released
            int minHp;
            int releasedCount = 0;
            // Reads the minimum HP
            scanf("%d", &minHp);
            // Removes monsters below minHP
            pc->activeRosterHead = releaseFainted(pc->activeRosterHead, minHp, &releasedCount);
            // Prints the number of released monsters
            printf("[EVENT] Released %d fainted monster(s) with HP below %d.\n", releasedCount, minHp);
        }
        // DISPLAY_STORAGE command
        else if (strcmp(command, "DISPLAY_STORAGE") == 0) 
        {
            // Displays the current queue and active roster
            displayStorage(pc);
        }
        // LOGOFF_PC command
        else if (strcmp(command, "LOGOFF_PC") == 0) 
        {
            // Prints the required logoff message
            printf("[EVENT] Logged off Will's PC.\n");
            // Frees all dynamically allocated memory
            releaseWillsPC(pc);
            // Stops processing commands
            break;
        }
    }

    // Ends the program
    return 0;
}