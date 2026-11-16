/* COP 3502C PA5
This program is written by: Hailey Simpson */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 30

// Monster structure
typedef struct Monster
{
    char *name;
    char element[MAXSIZE + 1];
    int power;
} Monster;

// BST node structure
typedef struct BST_Node
{
    Monster *monster;
    struct BST_Node *left;
    struct BST_Node *right;
} BST_Node;

// Function prototypes
Monster *createMonster(char *name, char *element, int scores);
BST_Node *createNode(Monster *monster);
BST_Node *insert(BST_Node *root, BST_Node *element);
BST_Node *findNode(BST_Node *current_ptr, char *name);
BST_Node *parent(BST_Node *root, BST_Node *node);
BST_Node *minVal(BST_Node *root);
int isLeaf(BST_Node *node);
int hasOnlyLeftChild(BST_Node *node);
int hasOnlyRightChild(BST_Node *node);
BST_Node *deleteMonster(BST_Node *root, char *name);
BST_Node *findSuccessor(BST_Node *root, char *name);
int countRange(BST_Node *root, char *low, char *high);
void inorder(BST_Node *current_ptr);
void freeTree(BST_Node *root);
void freeMonster(Monster *monster);

int main(void)
{
    int num;
    int operation;

    char name[MAXSIZE + 1];
    char element[MAXSIZE + 1];
    char low[MAXSIZE + 1];
    char high[MAXSIZE + 1];

    int power;

    BST_Node *root = NULL;
    BST_Node *temp_node;
    BST_Node *found;

    // Read the number of operations
    scanf("%d", &num);

    // Process each operation
    for (int i = 0; i < num; i++)
    {
        scanf("%d", &operation);

        // Operation 1: Register a monster
        if (operation == 1)
        {
            
            scanf("%30s %30s %d", name, element, &power);
            // Check whether the name already exists
            found = findNode(root, name);

            // Check whether a monster with this name was found
            if (found != NULL)
            {
                // Display a message if the name is a duplicate
                printf("DUPLICATE\n");
            }
            else
            {
                // Create the Monster
                Monster *newMonster = createMonster(name, element, power);
                // Create the BST node
                temp_node = createNode(newMonster);
                // Insert the node into the tree
                root = insert(root, temp_node);
                printf("INSERTED\n");
            }
        }

        // Operation 2: Remove a monster
        else if (operation == 2)
        {
            // Read the name of the monster that needs to be deleted
            scanf("%30s", name);
            // Check whether the monster exists
            found = findNode(root, name);

            if (found == NULL)
            {
                // Display a message because there is no monster to delete
                printf("NOT FOUND\n");
            }
            else
            {
                // Delete the monster from the BST
                root = deleteMonster(root, name);
                // Display a message indicating that the monster was deleted
                printf("DELETED\n");
            }
        }

        // Operation 3: Find a monster
        else if (operation == 3)
        {
            // Read the name of the monster to search for
            scanf("%30s", name);
            // Search the BST for a node containing the given name
            found = findNode(root, name);

            if (found != NULL)
            {
                // Display the monster's element and power
                printf("FOUND %s %d\n", found->monster->element, found->monster->power);
            }
            else
            {
                // Display a message when the monster does not exist
                printf("NOT FOUND\n");
            }
        }

        // Operation 4: Find the next monster alphabetically
        else if (operation == 4)
        {
            // Read the name used to find the next alphabetical monster
            scanf("%30s", name);
            // Finds the next monster alphabetically
            found = findSuccessor(root, name);

            // Check whether a successor exists
            if (found != NULL)
            {
                // Display the name of the successor
                printf("%s\n", found->monster->name);
            }
            else
            {
                // Display a message if no greater name exists
                printf("NO SUCCESSOR\n");
            }
        }

        // Operation 5: Count monsters in a name range
        else if (operation == 5)
        {
            // Read the lower and upper boundaries of the range
            scanf("%30s %30s", low, high);

            // If low is greater than high, the range is invalid
            if (strcmp(low, high) > 0)
            {
                printf("0\n");
            }
            else
            {
                // Count all monsters whose names are between low and high, inclusive
                printf("%d\n", countRange(root, low, high));
            }
        }

        // Operation 6: Display the registry
        else if (operation == 6)
        {
            if (root == NULL)
            {
                // Display EMPTY when there are no monsters in the registry
                printf("EMPTY\n");
            }
            else
            {
                // Display the monsters in alphabetical order
                inorder(root);
            }
        }
    }

    // Free all remaining dynamically allocated memory
    freeTree(root);

    return 0;
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

    // Return the pointer to the completed monster.
    return m;
}

// Creates a new BST node
BST_Node *createNode(Monster *monster)
{
    // Allocate space for the node
    BST_Node *newNode = malloc(sizeof(BST_Node));
    // Store the monster in the node
    newNode->monster = monster;

    // Initialize both children to NULL
    newNode->left = NULL;
    newNode->right = NULL;

    // Return a pointer to the created node
    return newNode;
}

// Inserts a node into the BST
BST_Node *insert(BST_Node *root, BST_Node *element) 
{
    // Inserting into an empty tree.
    if (root == NULL)
    {
        return element;
    }
    else 
    {
        // Element should be inserted to the right.
        if (strcmp(element->monster->name, root->monster->name) > 0) 
        {
            // There is a right subtree to insert the node.
            if (root->right != NULL)
            {
                root->right = insert(root->right, element);
            }
            // Place the node directly to the right of root.
            else
            {
                root->right = element;
            } 
        }
        // Element should be inserted to the left.
        else 
        {
            // There is a left subtree to insert the node.
            if (root->left != NULL)
            {
                root->left = insert(root->left, element);
            }
            // Place the node directly to the left of root.
            else
            {
                root->left = element;
            }
        }
        // Return the root pointer of the updated tree.
        return root;
    }
}

// Finds a node containing the specified monster name
BST_Node *findNode(BST_Node *current_ptr, char *name)
{
    // Check if there are nodes in the tree
    if (current_ptr != NULL) 
    {

        // Found the value at the root
        if (strcmp(current_ptr->monster->name, name) == 0)
        {
            return current_ptr;
        }
        // Search to the left
        if (strcmp(name, current_ptr->monster->name) < 0)
        {
            return findNode(current_ptr->left, name);
        }
        // Search to the right.
        else
        {
            return findNode(current_ptr->right, name);
        }
    }
    else
    {
        // No node found
        return NULL; 
    }
}

// Finds the parent of a specified node
BST_Node *parent(BST_Node *root, BST_Node *node)
{
    // Take care of NULL cases
    if (root == NULL || root == node)
    {
        return NULL;
    }
    // The root is the direct parent of node
    if (root->left == node || root->right == node)
    {
        return root;
    }
    // Search for the parent in the left subtree
    if (strcmp(node->monster->name, root->monster->name) < 0)
    {
        return parent(root->left, node);
    }
    // Search for the parent in the right subtree
    else
    {
        return parent(root->right, node);
    }
}

// Finds the minimum node in a subtree
BST_Node *minVal(BST_Node *root)
{
    // Root stores the minimum value
    if (root->left == NULL)
    {
        return root;
    }
    // The left subtree of the root stores the minimal value
    else
    {
        return minVal(root->left);
    }
}

// Returns 1 if node is a leaf node, 0 otherwise
int isLeaf(BST_Node *node)
{
    return (node->left == NULL && node->right == NULL);
}

// Returns 1 iff node has a left child and no right child
int hasOnlyLeftChild(BST_Node *node)
{
    return (node->left != NULL && node->right == NULL);
}

// Returns 1 iff node has a right child and no left child.
int hasOnlyRightChild(BST_Node *node)
{
    return (node->left == NULL && node->right != NULL);
}

// Deletes a monster from the BST
BST_Node *deleteMonster(BST_Node *root, char *name)
{
    BST_Node *delnode, *new_del_node, *save_node, *par;
    Monster *saveMonster;
    char successorName[MAXSIZE + 1];

    // Get a pointer to the node to delete
    delnode = findNode(root, name);
    // Get the parent of this node
    par = parent(root, delnode);

    // Take care of the case where the node to delete is a leaf node
    if (isLeaf(delnode))
    {
        // Delete the only node in the tree
        if (par == NULL)
        {
            // Free Monster and BST node
            freeMonster(delnode->monster);
            free(delnode);
            return NULL;
        }
        // Delete the node if it's a left child
        if (strcmp(name, par->monster->name) < 0)
        {
            // Free Monster and BST node (left child)
            freeMonster(par->left->monster);
            free(par->left);
            par->left = NULL;
        }
        // Delete the node if it's a right child
        else
        {
            // Free Monster and BST node (right child)
            freeMonster(par->right->monster);
            free(par->right);
            par->right = NULL;
        }
        // Return the root of the new tree
        return root;
    }

    // Take care of the case where the node to be deleted only has a left child
    if (hasOnlyLeftChild(delnode))
    {
        // Delete the root node of the tree
        if (par == NULL)
        {
            // Save the left child so it can become the new root
            save_node = delnode->left;
            // Free Monster and BST node
            freeMonster(delnode->monster);
            free(delnode);
            // Return the left child as the new root
            return save_node;
        }
        // Delete the node if it's a left child
        if (strcmp(name, par->monster->name) < 0)
        {
            // Save the node being deleted
            save_node = par->left;
            // Connect the parent's left pointer to the deleted node's left child
            par->left = save_node->left;
            // Free Monster and BST node that are being deleted
            freeMonster(save_node->monster);
            free(save_node);
        }
        // Delete the node if it's a right child
        else
        {
            // Save the node being deleted
            save_node = par->right;
            // Connect the parent's right pointer to the deleted node's left child
            par->right = save_node->left;
            // Free Monster and BST node that are being deleted
            freeMonster(save_node->monster);
            free(save_node);
        }
        // Return the root of the tree after the deletion
        return root;
    }

    // Takes care of the case where the deleted node only has a right child
    if (hasOnlyRightChild(delnode))
    {
        // Delete the root node of the tree
        if (par == NULL)
        {
            // Save the right child so it can become the new root
            save_node = delnode->right;
            // Free Monster and BST node
            freeMonster(delnode->monster);
            free(delnode);
            // Return the right child as the new root
            return save_node;
        }
        // Delete the node if it's a left child
        if (strcmp(name, par->monster->name) < 0)
        {
            // Save the node being deleted
            save_node = par->left;
            // Connect the parent's left pointer to the deleted node's right child
            par->left = save_node->right;
            // Free Monster and BST node that are being deleted
            freeMonster(save_node->monster);
            free(save_node);
        }
        // Delete the node if it's a right child
        else
        {
            // Save the node being deleted
            save_node = par->right;
            // Connect the parent's right pointer to the deleted node's right child
            par->right = save_node->right;
            // Free Monster and BST node that are being deleted
            freeMonster(save_node->monster);
            free(save_node);
        }

        return root;
    }
    
    // Takes care of the case where the node has two children
    // Find the in-order successor
    new_del_node = minVal(delnode->right);

    // Save the successor's name before deleting its node
    strcpy(successorName, new_del_node->monster->name);

    // Save a copy of the successor's Monster
    saveMonster = createMonster(
        new_del_node->monster->name,
        new_del_node->monster->element,
        new_del_node->monster->power
    );

    // Recursively delete the successor from the tree
    root = deleteMonster(root, successorName);

    // Free the original Monster being replaced
    freeMonster(delnode->monster);

    // Replace the original node's Monster with the saved successor
    delnode->monster = saveMonster;

    return root;
}

// Finds the next monster alphabetically
BST_Node *findSuccessor(BST_Node *root, char *name)
{
    BST_Node *successor = NULL;

    // Continue while there are nodes to examine
    while (root != NULL)
    {
        // Check whether the current name is greater
        if (strcmp(root->monster->name, name) > 0)
        {
            // Save the current node as a possible successor
            successor = root;
            // Look for a smaller possible successor
            root = root->left;
        }
        else
        {
            // Move to the right subtree
            root = root->right;
        }
    }

    // Return the pointer to the successor
    return successor;
}

// Counts monsters between low and high, inclusive
int countRange(BST_Node *root, char *low, char *high)
{
    // An empty subtree contains zero monsters
    if (root == NULL)
    {
        return 0;
    }
    // The current name is smaller than low
    if (strcmp(root->monster->name, low) < 0)
    {
        return countRange(root->right, low, high);
    }
    // The current name is greater than high
    else if (strcmp(root->monster->name, high) > 0)
    {
        return countRange(root->left, low, high);
    }
    // The current name is within the range
    else
    {
        return 1 + countRange(root->left, low, high) + countRange(root->right, low, high);
    }
}

// Displays all monsters alphabetically
void inorder(BST_Node *current_ptr)
{
    // Only traverse if the node is not NULL
    if (current_ptr != NULL)
    {
        // Go left
        inorder(current_ptr->left);

        // Print the current monster
        printf("%s %s %d\n",
               current_ptr->monster->name,
               current_ptr->monster->element,
               current_ptr->monster->power);

        // Go right
        inorder(current_ptr->right);
    }
}

// Frees the entire BST
void freeTree(BST_Node *root)
{
    // Stop if the current node is NULL
    if (root != NULL)
    {
        // Free the left subtree
        freeTree(root->left);
        // Free the right subtree
        freeTree(root->right);
        // Free the Monster and its name
        freeMonster(root->monster);
        // Free the BST node
        free(root);
    }
}

// Frees a Monster and its dynamically allocated name
void freeMonster(Monster *monster)
{
    free(monster->name);
    free(monster);
}
