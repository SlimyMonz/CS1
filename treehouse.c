//  treehouse.c
//  Project 2
//
//  Created by Harry James Hocker on 9/18/21.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

// calculate the distance between two coordinates
double calcDist(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(fabs((double) (x2-x1)), 2) + pow(fabs((double) (y2-y1)), 2));
}

// single (x,y) coordinate of a tree
struct tree
{
    int x, y;
} tree;

// do all the logic for scanning in the data into the given array
void getCoordinates(struct tree** array, int amount)
{
    // for each case from input, scan two coordinates and put them into the array of structs
    for (int i = 0; i < amount; i++)
    {
        struct tree* tree = calloc(1, sizeof(struct tree));
        scanf(" %d", &(tree->x));
        scanf(" %d", &(tree->y));
        array[i] = tree;
    }
}

// brute force method to find the smallest distance between points
double bruteForceDistance(struct tree** array, int* flagged, int amount, int k)
{
    // return 0 if all coordinates have been used
    if (k == amount/2) return 0;
    //a pair of tree coordinates
    int tree1 = 0, tree2 = 0;
    // this value is just here to be re-written with a true minimum value
    double minDistance = LLONG_MAX;
    // for each pair, calculate distance until found the minimum
    for (int i = 0; i < amount; i++)
    {
        // if i = a flagged value, skip
        if (flagged[i] == 1) continue;
        for (int j = i+1; j < amount; j++)
        {
            // if j = a flagged value or i equals j, skip
            if (flagged[j] == 1) continue;
            if (i == j) continue;
            double calcDistance = calcDist(array[i]->x, array[i]->y, array[j]->x, array[j]->y);
            if (calcDistance < minDistance)
            {
                minDistance = calcDistance;
                tree1 = i;
                tree2 = j;
            }
        }
    }
    flagged[tree1] = 1; flagged[tree2] = 1;
    // RECURSION HERE!!!
    minDistance += bruteForceDistance(array, flagged, amount, k+1);
    
    // returns the SMALLEST sum of distances
    return minDistance;
}

// BE FREE!
void freeMemory(struct tree** array, int amount)
{
    for(int i = 0; i < amount; i++)
    {
        free(array[i]);
    }
    free(array);
}


void runProgram(void)
{
    // scan in the input and double it to represent the total trees
    int input = 0;
    scanf(" %d", &input);
    int numTrees = input*2;
    
    // dynamically create tree array
    struct tree** treeArray = calloc(numTrees, sizeof(struct tree*));
    // dynamically create flag array and set each value to 0
    int* flagArray = calloc(numTrees, sizeof(int));
    for (int i = 0; i < input*2; i++) {
        flagArray[i] = 0;
    }
    // get the coordinates
    getCoordinates(treeArray, numTrees);
    // calculate the distance
    double distance = bruteForceDistance(treeArray, flagArray, numTrees, 0);
    
    // print the distance
    printf("%.3f\n", distance);
    
    // free the memory. BE FREE!
    freeMemory(treeArray, input);
    free(flagArray);
}

int main(void)
{
    int cases = 0;
    scanf(" %d", &cases);
    
    for (int i = 0; i < cases; i++)
    {
        runProgram();
    }
    return 0;
}

