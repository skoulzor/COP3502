//Binary search tree insertion code written in the class
#include <stdio.h>
#include<stdlib.h>

typedef struct tree_node {
  int data;
  struct tree_node *left;
  struct tree_node *right;
}tree_node;

int c = 0;
tree_node *create_node(int val);
void inorder(tree_node *current_ptr);
tree_node* insert(tree_node *root,
                         tree_node *element);

int add(tree_node *myTree) {
  if(myTree == NULL) 
    return 0;
  else
    return myTree->data + add(myTree->right) + add(myTree->left);
}

int count(tree_node *myTree) {
  if(myTree == NULL) 
    return 0;
  else
    return 1 + count(myTree->right) + count(myTree->left);
}

int main(void) {
   tree_node *my_root=NULL, *temp_node;

   int element;

   printf("Enter a number to insert in the tree: ");
   scanf("%d", &element);

   while(element!=0)
   {
      temp_node = create_node(element); // Create the node.
      // Insert the value.
      my_root = insert(my_root, temp_node);
      
      printf("In-order traversal: ");
      inorder(my_root);

      printf("\nEnter a number to insert in the tree: ");
      scanf("%d", &element);
   }

   
  return 0;
}

//take the root of the tree and insert the node element in the tree. Returns the root of the tree

tree_node* insert(tree_node* root, tree_node* element)
{
  if(root==NULL)
    return element;
  else
  {
    if(element->data > root->data)
    {
      if(root->right != NULL)
        root->right = insert(root->right, element);
      else
        root->right = element;      

    }
    else
    {
      if(root->left != NULL)
        root->left  = insert(root->left, element);
      else
        root->left = element;
    }

    return root;


  }


}

void inorder(tree_node *current_ptr)
{
  if(current_ptr != NULL)
  {
    inorder(current_ptr->left);
    printf("%d ", current_ptr->data);
    inorder(current_ptr->right);
  }
}

//take an integer and return a new node filling up with the integer with left and right pointers as NULL
tree_node* create_node(int val) {

  // Allocate space for the node, set the fields.
  tree_node* temp;
  temp = (tree_node*)malloc(sizeof(tree_node));
  temp->data = val;
  temp->left = NULL;
  temp->right = NULL;

  return temp; // Return a pointer to the created node.
}




  






