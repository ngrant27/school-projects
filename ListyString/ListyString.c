// Nicklous J. Grant
// ni841669

#include "ListyString.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// difficulty functions
double difficultyRating(void)
{
	return 2.0;
}
double hoursSpent(void)
{
	return 3.0;
}

// helper functions
ListyNode *createNode(char data)
{

	ListyNode *temp = NULL;
	temp = malloc(sizeof(ListyNode));

	temp->data = data;
	temp->next = NULL;

	return temp;

}
ListyNode *deleteNode(ListyNode *node)
{

	ListyNode *temp = NULL;

	if (node == NULL)
		return NULL;

	temp = node->next;

	free(node);
	return temp;

}
ListyNode *insertHead(ListyString *listy, char data)
{

	ListyNode *temp = NULL;

	if (listy == NULL)
		return NULL;

	temp = malloc(sizeof(ListyNode));

	temp->data = data;
	temp->next = listy->head;

	listy->head = temp;

	return listy->head;

}
ListyNode *deleteHead(ListyString *listy)
{

	ListyNode *temp = NULL;

	if (listy == NULL || listy->head == NULL)
		return NULL;

	temp = listy->head->next;

	free(listy->head);
	listy->length--;
	listy->head = temp;

	return listy->head;

}
ListyNode *getTail(ListyString *listy)
{

	ListyNode *temp = NULL;

	if (listy->head == NULL)
		return NULL;

	temp = listy->head;

	while (temp->next != NULL)
		temp = temp->next;

	return temp;

}



int main(int argc, char **argv)
{

	processInputFile(argv[1]);

	return 0;

}



int processInputFile(char *fileName)
{

	FILE *input = NULL;

	char buffer[1024];    // buffer for reading strings
	char cbuffer;         // buffer for commands and keys

	ListyString *workingString = NULL;

	if ((input = fopen(fileName, "r")) == NULL)
		return 1;

	// reads intial string
	fscanf(input, "%s", buffer);
	workingString = createListyString(buffer);

	// processes following string manipulation commands
	while(fscanf(input, "%c", &cbuffer) != EOF)
	{

		if (cbuffer == '@')
		{
			fscanf(input, " ");
			fscanf(input, "%c", &cbuffer);
			fscanf(input, " ");
			fscanf(input, "%s", buffer);
			replaceChar(workingString, cbuffer, buffer);

		}
		else if (cbuffer == '+')
		{
			fscanf(input, " ");
			fscanf(input, "%s", buffer);
			listyCat(workingString, buffer);
		}
		else if (cbuffer == '-')
		{
			fscanf(input, " ");
			fscanf(input, "%c", &cbuffer);
			replaceChar(workingString, cbuffer, NULL);
		}
		else if (cbuffer == '~')
		{
			reverseListyString(workingString);
		}
		else if (cbuffer == '?')
		{
			printf("%d\n", listyLength(workingString));
		}
		else if (cbuffer == '!')
		{
			printListyString(workingString);
		}

	}

	// cleaning up
	destroyListyString(workingString);
	fclose(input);
	input = NULL;

	return 0;

}

ListyString *createListyString(char *str)
{

	ListyString *temp  = NULL;
	ListyNode   *temp1 = NULL;
	int i 			   = 0;

	temp  = malloc(sizeof(ListyString));

	temp->length = 0;
	temp->head   = NULL;

	if (str == NULL || str[0] == '\0')
		return temp;

	// creates head of ListyString
	temp->head = createNode(str[i++]);
	temp1 = temp->head;
	temp->length++;

	// populates the rest of the ListyString
	while (str[i] != '\0')
	{

		temp1->next = createNode(str[i]);
		temp1 		= temp1->next;

		temp->length++;
		i++;

	}

	return temp;

}

ListyString *destroyListyString(ListyString *listy)
{

	ListyNode *temp  = NULL;
	ListyNode *temp1 = NULL;

	if (listy == NULL)
	{
		return NULL;
	}
	if (listy->head == NULL)
	{
		free(listy);
		return NULL;
	}

	temp = listy->head;

	for (int i = 0; i < listy->length; i++)
	{

		temp1 = temp->next;
		free(temp);
		temp = temp1;

	}

	free(listy);
	return NULL;

}

ListyString *cloneListyString(ListyString *listy)
{

	ListyString * temp  = NULL;
	ListyNode   * temp1 = NULL;
	ListyNode   * temp2 = NULL;

	if (listy == NULL)
		return NULL;

	temp = createListyString(NULL);

	if (listy->head == NULL)
		return temp;

	// creates head of cloned ListyString
	temp->head = createNode(listy->head->data);
	temp2 = listy->head->next;
	temp1 = temp->head;
	temp->length++;

	for (int i = 1; i < listy->length; i++)
	{

		temp1->next = createNode(temp2->data);
		temp1       = temp1->next;
		temp2       = temp2->next;

		temp->length++;
		i++;

	}

	return temp;

}

void replaceChar(ListyString *listy, char key, char *str)
{

	int i			  = 0;
	int j			  = 0;
	int len 		  = 0;
	ListyNode *temp1  = NULL;
	ListyNode *temp2  = NULL;

	if (listy == NULL || listy->head == NULL)
		return;

	len   = listy->length;
	temp2 = listy->head;

	// deletes all instances of the key
	if (str == NULL || str[0] == '\0')
	{

		for (i = 0; i < len; i++)
		{

			if (temp2->data == key)
			{
				if (temp1 == NULL)
				{
					temp2 = deleteHead(listy);
					continue;
				}

				temp1->next = deleteNode(temp2);
				temp2 = temp1->next;
				listy->length--;
				continue;

			}

			temp1 = temp2;
			temp2 = temp2->next;

		}

	}
	else
	{

		for (i = 0; i < len; i++)
		{

			if (temp2->data == key)
			{

				temp2->data = str[j++];
				temp1 		= temp2->next;

				while (str[j] != '\0')
				{

					temp2->next = createNode(str[j]);
					temp2 		= temp2->next;
					listy->length++;
					j++;


				}

				temp2->next	= temp1;
				temp2		= temp2->next;
				j = 0;
				continue;

			}

			temp2 = temp2->next;

		}

	}

}

void reverseListyString(ListyString *listy)
{

	ListyNode *head  = NULL;
	ListyNode *temp  = NULL;

	if (listy == NULL || listy->head == NULL)
		return;

	head = listy->head;
	temp = head->next;
 
	while (temp != NULL)
	{

		insertHead(listy, temp->data);
		head->next  = deleteNode(temp);
		temp        = head->next;

	}

}

ListyString *listyCat(ListyString *listy, char *str)
{

	ListyNode *temp = NULL;

	int i 			= 0;


	if (listy == NULL)
	{
		if (str == NULL)
			return NULL;

		return createListyString(str);
	}

	if (str == NULL || str[0] == '\0')
		return listy;

	temp = getTail(listy);

	// if there's no listy->head, create one
	if (temp == NULL)
	{
		listy->head = createNode(str[i++]);
		temp 		= listy->head;
		listy->length++;
	}

	// puts str at the end of listy
	while (str[i] != '\0')
	{

		temp->next = createNode(str[i]);
		temp 		= temp->next;

		listy->length++;
		i++;

	}

	return listy;

}

int listyCmp(ListyString *listy1, ListyString *listy2)
{

	ListyNode *temp1 = NULL;
	ListyNode *temp2 = NULL;

	if (listy1 == NULL || listy2 == NULL)
	{
		if (listy1 == NULL && listy2 == NULL)
			return 0;

		return 5;
	}
	if (listy1->head == NULL || listy2->head == NULL)
	{
		if (listy1->head == NULL && listy2->head == NULL)
			return 0;

		return 5;
	}

	if (listy1->length != listy2->length)
		return 5;

	temp1 = listy1->head;
	temp2 = listy2->head;

	for (int i = 0; i < listy1->length; i++)
	{

		if (temp1->data != temp2->data)
			return 5;

		temp1 = temp1->next;
		temp2 = temp2->next;

	}

	return 0;

}

int listyLength(ListyString *listy)
{

	if (listy == NULL)
		return -1;
	else if (listy->head == NULL)
		return  0;
	else
		return listy->length;

}

void printListyString(ListyString *listy)
{

	ListyNode *temp = NULL;

	if (listy == NULL || listy->head == NULL)
	{
		printf("(empty string)\n");
		return;
	}

	temp = listy->head;

	for (int i = 0; i < listy->length; i++)
	{

		printf("%c", temp->data);
		temp = temp->next;

	}

	printf("\n");

}
