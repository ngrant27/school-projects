// Nicklous J. Grant
// ni841699

#include "LonelyPartyArray.h"

#include <stdio.h>
#include <stdlib.h>

// rating Functions
double difficultyRating(void)
{
    return 1.0;
}

double hoursSpent(void)
{
    return 3.0;
}


// Helper Functions
int initFragment(LonelyPartyArray *lonely, int fragNum)
{

    lonely->fragments[fragNum] = malloc(sizeof(int) * lonely->fragment_length);

    if (lonely->fragments[fragNum] == NULL)
    {
        return 1;
    }

    for(int i = 0; i < lonely->fragment_length; i++)
    {
        lonely->fragments[fragNum][i] = UNUSED;
    }

    lonely->num_active_fragments++;

    return 0;

}

void deleFragment(LonelyPartyArray *party, int fragNum)
{

    free(party->fragments[fragNum]);
    party->fragments[fragNum] = NULL;
    party->num_active_fragments--;

}


// Required  Functions
LonelyPartyArray *createLonelyPartyArray(int num_fragments, int fragment_length)
{

    LPA *lonely = NULL;

    if (num_fragments <= 0 || fragment_length <= 0)
    {
        return NULL;
    }

    lonely = malloc(sizeof(LPA));

    if (lonely == NULL)
    {
        free(lonely);
        return NULL;
    }

    lonely->size                 = 0;
    lonely->num_active_fragments = 0;

    lonely->num_fragments        = num_fragments;
    lonely->fragment_length      = fragment_length;

    lonely->fragment_sizes       = malloc(sizeof(int)  * num_fragments);
    lonely->fragments            = malloc(sizeof(int*) * num_fragments);

    if (lonely->fragments == NULL || lonely->fragment_sizes == NULL)
    {
        free(lonely->fragment_sizes);
        free(lonely->fragments);
        free(lonely);
        return NULL;
    }

    for(int i = 0; i < num_fragments; i++)
    {
        lonely->fragments[i] = NULL;
        lonely->fragment_sizes[i] = 0;
    }

    printf("-> A new LonelyPartyArray has emerged from the void. (capacity: %d, fragments: %d)\n", num_fragments*fragment_length, num_fragments);

    return lonely;

}

LonelyPartyArray *destroyLonelyPartyArray(LonelyPartyArray *party)
{

    if (party == NULL)
    {
        return NULL;
    }

    for(int i = 0; i < party->num_fragments; i++)
    {
        free(party->fragments[i]);
    }

    free(party->fragment_sizes);
    free(party->fragments);
    free(party);

    printf("-> The LonelyPartyArray has returned to the void.\n");

    return NULL;

}

int set(LonelyPartyArray *party, int index, int key)
{

    int fragIndex   = 0;  // The index of fragment
    int inIndex     = 0;  // The index within fragment

    if (party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in set().\n");
        return LPA_FAILURE;
    }

    fragIndex = index / party->fragment_length;
    inIndex   = index % party->fragment_length;

    if (fragIndex >= party->num_fragments || index < 0)
    {
        printf("-> Bloop! Invalid access in set(). (index: %d, fragment: %d, offset: %d)\n", index, fragIndex, inIndex);
        return LPA_FAILURE;
    }

    if (party->fragments[fragIndex] == NULL)
    {
        if (initFragment(party, fragIndex))
            return LPA_FAILURE;

        printf("-> Spawned fragment %d. (capacity: %d, indices: %d..%d)\n", fragIndex, party->fragment_length, (fragIndex*party->fragment_length), ((fragIndex+1)*party->fragment_length)-1);
    }

    if (party->fragments[fragIndex][inIndex] == UNUSED)
    {
        party->size++;
        party->fragment_sizes[fragIndex]++;
    }

    party->fragments[fragIndex][inIndex] = key;

    return LPA_SUCCESS;

}

int get(LonelyPartyArray *party, int index)
{

    int fragIndex   = 0;
    int inIndex     = 0;

    if (party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in get().\n");
        return LPA_FAILURE;
    }

    fragIndex   = index / party->fragment_length;
    inIndex     = index % party->fragment_length;

    if (index < 0 || fragIndex >= party->num_fragments)
    {
        printf("-> Bloop! Invalid access in get(). (index: %d, fragment: %d, offset: %d)\n", index, fragIndex, inIndex);
        return LPA_FAILURE;
    }

    if (party->fragments[fragIndex] == NULL)
    {
        return UNUSED;
    }

    return party->fragments[fragIndex][inIndex];

}

int delete(LonelyPartyArray *party, int index)
{

    int fragIndex   = 0;
    int inIndex     = 0;

    if (party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in delete().\n");
        return LPA_FAILURE;
    }

    fragIndex   = index / party->fragment_length;
    inIndex     = index % party->fragment_length;

    if (fragIndex >= party->num_fragments || index < 0)
    {
        printf("-> Bloop! Invalid access in delete(). (index: %d, fragment: %d, offset: %d)\n", index, fragIndex, inIndex);
        return LPA_FAILURE;
    }
    if (party->fragments[fragIndex] == NULL || party->fragments[fragIndex][inIndex] == UNUSED)
    {
        return LPA_FAILURE;
    }

    party->size--;
    party->fragment_sizes[fragIndex]--;

    if (party->fragment_sizes[fragIndex] == 0)
    {
        deleFragment(party, fragIndex);
        printf("-> Deallocated fragment %d. (capacity: %d, indices: %d..%d)\n", fragIndex, party->fragment_length, (fragIndex*party->fragment_length), ((fragIndex+1)*party->fragment_length)-1);
        return LPA_SUCCESS;
    }

    party->fragments[fragIndex][inIndex] = UNUSED;

    return LPA_SUCCESS;

}

int containsKey(LonelyPartyArray *party, int key)
{

    if (party == NULL)
    {
        return 0;
    }

    for(int i = 0; i < party->num_fragments; i++)
    {

        if (party->fragments[i] == NULL)
            continue;

        for(int j = 0; j < party->fragment_length; j++)
        {

            if (key == party->fragments[i][j])
                return 1;

        }

    }

    return 0;

}

int isSet(LonelyPartyArray *party, int index)
{

    int fragIndex   = 0;
    int inIndex     = 0;

    if (party == NULL)
    {
        return 0;
    }

    fragIndex   = index / party->fragment_length;
    inIndex     = index % party->fragment_length;

    if (index >= (party->num_fragments*party->fragment_length) || index < 0)
    {
        return 0;
    }
    if (party->fragments[fragIndex] == NULL)
    {
        return 0;
    }
    if (party->fragments[fragIndex][inIndex] == UNUSED)
    {
        return 0;
    }

    return 1;

}

int printIfValid(LonelyPartyArray *party, int index)
{

    int fragIndex   = 0;
    int inIndex     = 0;

    if (party == NULL)
    {
        return LPA_FAILURE;
    }

    fragIndex   = index / party->fragment_length;
    inIndex     = index % party->fragment_length;

    if (index >= (party->num_fragments*party->fragment_length) || index < 0)
    {
        return LPA_FAILURE;
    }
    if (party->fragments[fragIndex] == NULL)
    {
        return LPA_FAILURE;
    }
    if (party->fragments[fragIndex][inIndex] == UNUSED)
    {
        return LPA_FAILURE;
    }

    printf("%d\n", party->fragments[fragIndex][inIndex]);

    return LPA_SUCCESS;

}

LonelyPartyArray *resetLonelyPartyArray(LonelyPartyArray *party)
{

    if (party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in resetLonelyPartyArray().\n");
        return party;
    }

    party->size = 0;

    for(int i = 0; i < party->num_fragments; i++)
    {

        if (party->fragments[i] == NULL)
            continue;

        party->fragment_sizes[i] = 0;
        deleFragment(party, i);

    }

    printf("-> The LonelyPartyArray has returned to its nascent state. (capacity: %d, fragments: %d)\n", (party->num_fragments * party->fragment_length), party->num_fragments);

    return party;

}

int getSize(LonelyPartyArray *party)
{

    if (party == NULL)
    {
        return -1;
    }

    return party->size;

}

int getCapacity(LonelyPartyArray *party)
{

    if (party == NULL)
    {
        return -1;
    }

    return (party->num_fragments * party->fragment_length);

}

int getAllocatedCellCount(LonelyPartyArray *party)
{

    if (party == NULL)
    {
        return -1;
    }

    return (party->num_active_fragments * party->fragment_length);

}

long long unsigned int getArraySizeInBytes(LonelyPartyArray *party)
{

    long long unsigned int sum = 0;

    if (party == NULL)
    {
        return 0;
    }

    sum = sizeof(int) * (party->num_fragments * party->fragment_length);

    return sum;

}

long long unsigned int getCurrentSizeInBytes(LonelyPartyArray *party)
{

    long long unsigned int sum = 0;

    if (party == NULL)
    {
        return 0;
    }

    sum = sizeof(LPA*) + sizeof(LPA) + (sizeof(int) * party->num_fragments) + (sizeof(int*) * party->num_fragments) + (party->num_active_fragments * (party->fragment_length * sizeof(int)));

    return sum;

}

LonelyPartyArray *cloneLonelyPartyArray(LonelyPartyArray *party)
{

    LPA *clone = NULL;

    clone = malloc(sizeof(LPA));

    if (party == NULL || clone == NULL)
    {
        free(clone);
        return NULL;
    }

    clone->size                 = party->size;
    clone->num_active_fragments = 0;

    clone->num_fragments        = party->num_fragments;
    clone->fragment_length      = party->fragment_length;

    clone->fragment_sizes       = malloc(sizeof(int)  * party->num_fragments);
    clone->fragments            = malloc(sizeof(int*) * party->num_fragments);

    if (clone->fragments == NULL || clone->fragment_sizes == NULL)
    {
        free(clone->fragment_sizes);
        free(clone->fragments);
        free(clone);
        return NULL;
    }

    for(int i = 0; i < party->num_fragments; i++)
    {
        clone->fragments[i] = NULL;
        clone->fragment_sizes[i] = 0;
    }

    for(int i = 0; i < party->num_fragments; i++)
    {

        if (party->fragments[i] == NULL)
            continue;

        clone->fragment_sizes[i] = party->fragment_sizes[i];
        initFragment(clone, i);

        for(int j = 0; j < party->fragment_length; j++)
            clone->fragments[i][j] = party->fragments[i][j];

    }

    printf("-> Successfully cloned the LonelyPartyArray. (capacity: %d, fragments: %d)\n", (clone->num_fragments * clone->fragment_length), clone->num_fragments);

    return clone;

}
