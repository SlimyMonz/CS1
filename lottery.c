//
//  main.c
//  P3
//
//  Created by Harry James Hocker on 9/28/21.
//
#include <stdio.h>
#include <stdlib.h>

/*
 This program is built off of these premises:
 
 1) An array of linked lists can be seen as an array of groups.
 2) Each index of the array will point to the first person of each group
 3) The first person of each group is also the person with the smallest ID #.
 4) After the process of elimination in Phase 1, Phase 2 is only checking for the smallest ID #. Thus, anyone other than the person at the index of that group is not needed.
 5) With everyone except the people of group array's each index eliminated, compare and find the smallest ID #.
 6) If an ID# is #1, that is the smallest possible ID. Thus, print them out and quit program.
 7) Lastly: We assume that a person's nextPerson node will NEVER be NULL. The function "insertAllPeople" never allows for a NULL case.
 */

// a single person node
typedef struct person
{
    int ID;
    struct person* nextPerson;
} person;

// creates new person node and stores their ID
person* insertPerson(int ID)
{
    person* newPerson = calloc(1, sizeof(person));
    newPerson -> ID = ID;
    newPerson -> nextPerson = NULL;
    return newPerson;
}

// given the initialPerson (S-person) remove the person after them.
void deletePerson(person** groupArray, person* initialPerson, int index, int flag)
{
    // if the circle has reduced down to a single person, quit.
    if (initialPerson -> nextPerson == initialPerson) return;
    // save the person to be removed
    person* removePerson = initialPerson -> nextPerson;
    // set the next person for initial to be the next of the person to be removed
    initialPerson -> nextPerson = removePerson -> nextPerson;
    // if the person to be removed is the first person of the group, make the person after the new first person
    if (removePerson -> ID == groupArray[index] -> ID)
    {
        groupArray[index] = removePerson -> nextPerson;
    }
    // flag is 1: prints out the person being removed.
    // flag is 0: this is being recycled to free memory and should not print.
    if (flag == 1) printf("%d\n", removePerson -> ID);
    // remove the person
    free(removePerson);
}

// generates a looping line of people based on an integer and returns the first person
person* insertAllPeople(int numPeople)
{
    // set the baseline person as a new person
    person* first = insertPerson(1);
    // create the last person and set it to the current first. They are effectively the same right now.
    person* last = first;
    for (int i = 1; i < numPeople; i++) {
        // generate a new person at the end of the list
        last -> nextPerson = insertPerson(i+1);
        // set the last person to the new end-of-list person
        last = last -> nextPerson;
    }
    // close the loop when done
    last -> nextPerson = first;
    // return the starting person
    return first;
}

// this process eliminates people until reaching the threshold number of people
void phaseOne(person** groupArray, int group, int numPeople, int skipNum, int threshNum)
{
    // get the initial person
    person* person = groupArray[group];
    // do until the threshold is reached
    while (numPeople > threshNum)
    {
        // move through the queue skipNum times
        for (int i = 1; i < skipNum; i++ )
        {
            person = person -> nextPerson;
        }
        deletePerson(groupArray, person, group, 1);
        person = person -> nextPerson;
        numPeople--;
    }
}

// find the person with the lowest ID
int phaseTwo(person** groupArray, int groups)
{
    int index = 0;
    
    for (int i = 1; i < groups; i++)
    {
        // if the person's ID is 1, there's no point in continuing the loop
        if (groupArray[i-1] -> ID == 1) { index = i-1; return index; }
        // if the ID of the current group is less than previous, set the lowest at index
        if (groupArray[i] -> ID < groupArray[i-1] -> ID) index = i;
    }
    // returns the group index with the "winner"
    return index;
}

// clears the nodes
void freeNodes(person* person, int ID) {
    
    if (person -> nextPerson -> ID == ID) {
        free(person);
        return;
    }
    freeNodes(person -> nextPerson, ID);
    
    free(person);
    
    return;
}

int main(void)
{
    int cases = 0;
    scanf(" %d", &cases);
    
    // cases loop
    for (int i = 0; i < cases; i++)
    {
        int groups = 0;
        scanf(" %d", &groups);
        // this fixes a potential NULL pointer issue. Quit loop if there are no groups.
        if (groups < 1) {
            printf("No groups. No winner.\n\n");
            continue;
        }
        person** groupArray = calloc(groups, sizeof(person*));
        // groups loop and phase 1
        for (int j = 0; j < groups; j++)
        {
            int numPeople = 0, skipNum = 0, threshNum = 0;
            scanf(" %d %d %d", &numPeople, &skipNum, &threshNum);
            groupArray[j] = insertAllPeople(numPeople);
            printf("Group #%d:\n", j+1);
            phaseOne(groupArray, j, numPeople, skipNum, threshNum);
        }
        // phase 2 and free memory
        int winner = phaseTwo(groupArray, groups);
        printf("Lottery winner is person %d from group %d.\n\n", groupArray[winner] -> ID, winner+1);
        
        for (int j = 0; j < groups; j++) {
            freeNodes(groupArray[j], groupArray[j] -> ID);
        }
        free(groupArray);
    }
    return 0;
}
