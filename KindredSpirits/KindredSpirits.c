// Nicklous J. Grant
// ni841669

#include "KindredSpirits.h"

#include <stdio.h>
#include <stdlib.h>

// Stack data structure & functions used for kindredspirit function
typedef struct Stack
{

	int capacity;
	int *array;
	int size;

} stack;

stack *createStack(int capacity)
{

	stack *temp = NULL;

	temp = malloc(sizeof(stack));

	temp->array    = malloc(sizeof(int) * capacity);
	temp->capacity = capacity;
	temp->size     = 0;

	return temp;

}

stack *destroyStack(stack *s)
{

	if (s == NULL)
		return NULL;

	free(s->array);
	free(s);

	return NULL;

}

void push(stack *s, int data)
{

	s->array[s->size] = data;
	s->size++;

}

int pop(stack *s)
{

	return s->array[--s->size];

}



// statistical data functions
double difficultyRating(void)
{

	return 3.0;

}
double hoursSpent(void)
{

	return 3.0;

}



// helper functions
node *createNode(int data)
{

	node *temp  = NULL;

	temp        = malloc(sizeof(node));

	temp->data  = data;
	temp->left  = NULL;
	temp->right = NULL;

	return temp;

}

int nodeCount(node *root)
{

	if (root == NULL)
		return 0;

	return 1 + nodeCount(root->left) + nodeCount(root->right);

}

void preOrderLoader(stack *s, node *root)
{

	if (root == NULL)
		return;

	push(s, root->data);
	preOrderLoader(s, root->left);
	preOrderLoader(s, root->right);

}

void postOrderLoader(stack *s, node *root)
{

	if (root == NULL)
		return;

	postOrderLoader(s, root->left);
	postOrderLoader(s, root->right);
	push(s, root->data);

}



// Required Functions
int isReflection(node *a, node *b)
{

	if (a == NULL && b == NULL)
		return 1;
	else if (a == NULL || b == NULL)
		return 0;
	else if (a->data != b->data)
		return 0;

	return (isReflection(a->left, b->right) + isReflection(a->right, b->left)) / 2;

}

node *makeReflection(node *root)
{

	node *temp = NULL;

	if (root == NULL)
		return temp;

	temp        = createNode(root->data);

	temp->left  = makeReflection(root->right);
	temp->right = makeReflection(root->left);

	return temp;

}

int kindredSpirits(node *a, node *b)
{

	int kindredFlag = 0;
	int aLen = 0;
	int bLen = 0;

	stack *sA = NULL;
	stack *sB = NULL;

	if (a == NULL && b == NULL)
		return 1;
	else if (a == NULL || b == NULL)
		return 0;

	aLen = nodeCount(a);
	bLen = nodeCount(b);

	// checks to make sure the tree have the same amount of nodes
	if (aLen != bLen)
		return 0;

	// makes two stacks to hold preorder and postorder traversals
	// of each tree for comparisom
	sA = createStack(2 * aLen);
	sB = createStack(2 * bLen);

	preOrderLoader(sA , a);
	postOrderLoader(sA, a);
	postOrderLoader(sB, b);
	preOrderLoader(sB , b);

	// 1st loop: compares postOrder trav. of tree a to postOrder trav. of tree b
	// 2nd loop: compares preOrder  trav. of tree a to preOrder  trav. of tree b
	for (int i = 0; i < 2; i++)
	{

		// tress are kindred until proven otherwise
		kindredFlag = 1;

		for (int j = 0; j < bLen; j++)
		{

			// I don't break early beacuse I need to
			// pop off the extra values
			if (pop(sA) != pop(sB))
				kindredFlag = 0;

		}

		if (kindredFlag)
			break;

	}

	// cleaning up
	destroyStack(sA);
	destroyStack(sB);

	return kindredFlag;

}
