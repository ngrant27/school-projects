// Nicklous J. Grant
// ni841669

#include "SneakyRooks.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>


// Stat Functions
double difficultyRating(void)
{

	return 2.0;

}

double hoursSpent(void)
{

	return 3.0;

}


// Required Functions
void parseCoordinateString(char *rookString, Coordinate *rookCoordinate)
{

	int col = 0;
	int row = 0;
	int i = 0;

	for(;;)
	{

		if((rookString[i] - 80) < 0)
			break;

		col *= 26;
		col += (rookString[i] - 96);

		i++;

	}

	row = atoi(&rookString[i]);

	rookCoordinate->col = col;
	rookCoordinate->row = row;

}

int allTheRooksAreSafe(char **rookStrings, int numRooks, int boardSize)
{

	int i;
	int hashIndexRow;
	int hashIndexCol;

	int *hashTable = NULL;
	Coordinate *temp = NULL;

	if (numRooks == 0 || boardSize < numRooks)
		return 0;


	temp = malloc(sizeof(Coordinate));
	hashTable = calloc((2*boardSize), sizeof(int));

	for(i = 0; i < numRooks; i++)
	{

		parseCoordinateString(rookStrings[i], temp);

		hashIndexRow = (temp->row-1);
		hashIndexCol = (temp->col+boardSize-1);

		if (hashTable[hashIndexRow] != 0 || hashTable[hashIndexCol] != 0)
		{
			free(temp);
			free(hashTable);
			return 0;
		}
		else
		{
			hashTable[hashIndexRow] = 1;
			hashTable[hashIndexCol] = 1;
		}

	}

	free(temp);
	free(hashTable);

	return 1;

}
