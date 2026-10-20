//Lab #7: Stack Implementation Skeleton Code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> //needed for isdigit

// The array will store the items in the stack, first in
// index 0, then 1, etc. top will represent the index
// to the top element in the stack. If the stack is
// empty top will be -1.

#define SIZE 100
#define EMPTY -1

struct stack 
{
    char items[SIZE];
    int top;
};

//Provided functions
void initialize(struct stack* stackPtr);
int full(struct stack* stackPtr);
int push(struct stack* stackPtr, char value);
int empty(struct stack* stackPtr);
char pop(struct stack* stackPtr);
int peek(struct stack* stackPtr);

//---------------------------------------------
//Functions you need to complete (in addition to main)
int checkBalance(char exp[]);
int priority(char ch);
int isOperator(char ch);
char *infixToPostfix(char infix[]);
int isParentheses(char ch1);

int main() 
{
    char exp[SIZE];
    int valid;
    printf("Enter Expression: ");
    fgets(exp, SIZE, stdin);
    exp[strcspn(exp, "\n")] = '\0';

    printf("Your input expression: %s\n", exp);

    valid = checkBalance(exp);
    
    if (valid) {
        char *postfix = infixToPostfix(exp);
        printf("The postfix is: %s\n", postfix);
        free(postfix);
    }
    return 0;
}

void initialize(struct stack* stackPtr) 
{
    stackPtr->top = -1;
}

// If the push occurs, 1 is returned. If the
// stack is full and the push can't be done, 0 is
// returned.
int push(struct stack* stackPtr, char value) 
{
    // Check if the stack is full.
    if (full(stackPtr))
        return 0;

    // Add value to the top of the stack and adjust the value of the top.
    stackPtr->items[stackPtr->top+1] = value;
    (stackPtr->top)++;
    return 1;
}

// Returns true iff the stack pointed to by stackPtr is full.
int full(struct stack* stackPtr) 
{
    return (stackPtr->top == SIZE - 1);
}

// Returns true iff the stack pointed to by stackPtr is empty.
int empty(struct stack* stackPtr) 
{
    return (stackPtr->top == -1);
}

// Pre-condition: The stack pointed to by stackPtr is NOT empty.
// Post-condition: The value on the top of the stack is popped and returned.
// Note: If the stack pointed to by stackPtr is empty, -1 is returned.
char pop(struct stack* stackPtr)
{
    char retval;

    // Check the case that the stack is empty.
    if (empty(stackPtr))
        return 'I';

    // Retrieve the item from the top of the stack, adjust the top and return
    // the item.
    retval = stackPtr->items[stackPtr->top];
    (stackPtr->top)--;
    return retval;
}

// Pre-condition: The stack pointed to by stackPtr is NOT empty.
// Post-condition: The value on the top of the stack is returned.
// Note: If the stack pointed to by stackPtr is empty, -1 is returned.
int peek(struct stack* stackPtr) 
{
    // Take care of the empty case.
    if (empty(stackPtr))
        return EMPTY;

    // Return the desired item.
    return stackPtr->items[stackPtr->top];
}

/*
 * -------------------------------------------------------------------
 *
 * This marks the start of the functions you must complete
 */

int checkBalance(char exp[])
{
    struct stack mine;
    initialize(&mine);

    printf("Checking balance...\n");

    for (int i = 0; exp[i] != '\0'; i++)
    {
        if (exp[i] == '(' || exp[i] == '{' || exp[i] == '[')
        {
            push(&mine, exp[i]);
        }
        else if (exp[i] == ')' || exp[i] == '}' || exp[i] == ']')
        {
            char a = pop(&mine);

            if (a == 'I')
            {
                if (exp[i] == ')')
                    printf("INVALID for )!!!\n");
                else if (exp[i] == '}')
                    printf("INVALID for }!!!\n");
                else
                    printf("INVALID for ]!!!\n");

                return 0;
            }

            if ((exp[i] == ')' && a != '(') ||
                (exp[i] == '}' && a != '{') ||
                (exp[i] == ']' && a != '['))
            {
                if (exp[i] == ')')
                    printf("INVALID for )!!!\n");
                else if (exp[i] == '}')
                    printf("INVALID for }!!!\n");
                else
                    printf("INVALID for ]!!!\n");

                return 0;
            }
        }
    }

    if (!empty(&mine))
    {
        printf("INVALID. You have extra opening parentheses!!!\n");
        return 0;
    }

    printf("VALID\n");
    return 1;
}

int priority(char ch)
{
    if (ch == '(' || ch == '['|| ch == '{') { 
        return 0;
    }

    if (ch == '+' || ch == '-') {
        return 1;
    }

    if (ch == '*' || ch == '/' || ch == '%') {
        return 2;
    }

    if (ch == '^') {
        return 3;
    }

    return -1;
}

int isOperator(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '%') {
        return 1;
    }
    return 0;
}

char *infixToPostfix(char infix[])
{
    struct stack operators;
    initialize(&operators);

    int count = 0; // keeps track of postfix index

    // dynamically allocates meory for postfix
    char *postfix = (char *) malloc((strlen(infix) * 2 + 1) * sizeof(char));

    // iterating through the infix and evaluating each char
    int len = strlen(infix);

    for (int i = 0; i < len; i++) {
        // if char is a digit, put it into the postfix
        if (isdigit(infix[i])) {
            // deals with multiple digits
            while (isdigit(infix[i])) {
                postfix[count] = infix[i];
                i++;
                count++;
            }
            i--;
            // adds white spece between numbers
            postfix[count] = ' ';
            count++;
        }
        // if the char is an operators and the stack is empty, push it. 
        // if the stack isn't empty, check for priority
        else if (isOperator(infix[i]))
        {
            if (empty(&operators)) {
                push(&operators, infix[i]);
            }
            // if the priority is higher than the top of the stack, push it to operators stack
            else if (priority(infix[i]) > priority(peek(&operators))) {
                push(&operators, infix[i]);
            }
            // if priority is lower than or equal to the top of the stack, pop the stack into the postfix 
            // until it is empty or the property of the char is higher than the one from the top of the stack
            else {
                while (!empty(&operators) && peek(&operators) != '(' && priority(infix[i]) <= priority(peek(&operators))) {
                    postfix[count] = pop(&operators);
                    count++;
                    postfix[count] = ' ';
                    count++;
                }
                push(&operators, infix[i]);
            }
        }
        // checks if the char is a parentheses
        else if (infix[i] == '(' || infix[i] == ')') {
            // if it is an open parentheses, push it to operators
            if (infix[i] == '(') {
                push(&operators, infix[i]);
            }
            // else, pop the operator stack into the postfix until it gets to an open parentheses
            else {
                while (peek(&operators) != '(') {
                    postfix[count] = pop(&operators);
                    count++;
                    postfix[count] = ' ';
                    count++;
                }
                pop(&operators);
            }
        }
    } // end of iterating through the infix

    // loops to pop all the operators from the stack and adds white space between them
    while (!empty(&operators)) {
        postfix[count] = pop(&operators);
        count++;
        postfix[count] = ' ';
        count++;
    }

    // add the null terminator
    postfix[count] = '\0';

    // returns postfix
    return postfix;
}

int isParentheses(char ch1)
{
    if (ch1 == '{' || ch1 == '}' || ch1 == '(' || ch1 == ')' || ch1 == '[' || ch1 == ']') {
        return 1;
    }
    return 0;
}