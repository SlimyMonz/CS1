//
//  main.c
//  P5
//
//  Created by Harry Hocker on 10/28/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// the data of the node
typedef struct nodeData
{
    char string[100];
    int frequency;
    int depth;
} nodeData;

// the node of the tree
typedef struct treeNode
{
    struct nodeData* data;
    struct treeNode* left;
    struct treeNode* right;
} treeNode;

// this will perform a tree search and print out the depth and frequency
void search(treeNode* root, char* word)
{
    if (root == NULL) {
        printf("-1 -1\n");
        return;
    }
    // compare strings
    int compare = strcmp(word, root -> data -> string);
    // if compare = 0, then the value has been found
    if (compare == 0) {
        printf("%d %d\n",root -> data -> frequency, root -> data -> depth);
        return;
    } else if (compare < 0) {
        search(root->left, word);
    } else if (compare > 0){
        search(root->right, word);
    }
}

int nodeCounter(treeNode* root) {
    //base case
    if (root == NULL) return 0;
    // if it's not null, then we have 1 node
    int value = 1;
    value += nodeCounter(root -> left);
    value += nodeCounter(root -> right);
    // return the nodes each recursion
    return value;
}


treeNode* newNode(int depth, char *word) {
    nodeData* newData = calloc(1, sizeof(nodeData));
    strcpy(newData -> string, word);
    newData -> frequency = 1;
    newData -> depth = depth;
    // initialize the node and add data node
    treeNode* newNode = calloc(1, sizeof(treeNode));
    newNode -> data = newData;
    newNode -> left = NULL;
    newNode -> right = NULL;
    return newNode;
}


// adds to the tree
void addToTree(treeNode* root, char* word, int depth)
{
    // increases first because this assumes it is either the beginning or a recursion occured
    int depthCount = depth+1;
    
    // compare the strings. -1 if less than, 0 if equal to, 1 if greater than
    int compare = strcmp(word, root -> data -> string);
    
    // if it's less than the node, add to left branch
    if (compare < 0) {
        if (root -> left == NULL) root -> left = newNode(depthCount, word);
        else addToTree(root -> left, word, depthCount);
        // if it's greater than the node, add to right branch
    } else if (compare > 0) {
        if (root -> right == NULL) root -> right = newNode(depthCount, word);
        else addToTree(root -> right, word, depthCount);
        // if it is the same value, increment the frequency value
    } else if (compare == 0) {
        root -> data -> frequency += 1;
    }
}

// a function to gather all the inputs and run the appropriate functions
void getInputs(int input, treeNode* root)
{
    for (int i = 0; i < input; i++)
    {
        int action = 0;
        char word[100];
        scanf("%d %s", &action, word);
        // if the action is 1, then add to the tree
        if (action == 1) {
            addToTree(root, word, 0);
            // if action is 2, query the word
        } else if (action == 2) {
            search(root, word);
        }
    }
}

// takes in root node and adds to array in-order
void addToArray(treeNode* root, nodeData** array, int* index)
{
    if (root == NULL) return;
    addToArray(root -> left, array, index);
    array[*index] = root -> data; (*index) += 1;
    addToArray(root -> right, array, index);
    free(root);
}

// part of mergesort
void merge(nodeData** array, long long int start, long long int middle, long long int end)
{
    long long int a = start, b = middle + 1, i = 0;
    // create a temporary array for sorting
    long long int length = end - start + 1;
    nodeData** sorted = calloc(length, sizeof(nodeData*));
    // use a and b to prevent out-of-bounds of either array half
    while (a < middle+1 && b < end+1)
    {
        if ( (array[a] -> frequency) >= (array[b] -> frequency) )
        {
            sorted[i] = array[a];
            a++;
        }
        else
        {
            sorted[i] = array[b];
            b++;
        }
        i++;
    }
    // run each loop to add remaining values to array and avoid OOB
    if (i < length)
    {
        while (a < middle+1)
        {
            sorted[i] = array[a];
            a++; i++;
        }
        while (b < end+1)
        {
            sorted[i] = array[b];
            b++; i++;
        }
    }
    // add values to original array
    for (i = start; i < end+1; i++)
    {
        array[i] = sorted[i-start];
    }
    // finally, free the array
    free(sorted);
}

// part of mergesort
void sort(nodeData** array, long long int a, long long int z)
{
    // recursion base case: first index must be less than last index
    if (!(a<z)) return;
    // create middle value
    long long int middle = (a+z)/2;
    // split into halves and throw into recursions
    sort(array, a, middle);
    sort(array, middle+1, z);
    // at the end of each recursion, merge together
    merge(array, a, middle, z);
}

void freeMemory(nodeData** array, int numWords)
{
    for (int i = 0; i < numWords; i++) {
        free(array[i]);
    }
    free(array);
}


int main()
{
    // get the amount of case inputs
    int input = 0;
    scanf("%d", &input);
    // initialize the first number and string to create the root node
    int firstNum = 0;
    char firstString[100];
    
    // if the firstNum starts with 2, keep printing and scanning until a valid node can be made
    do {
        scanf("%d %s", &firstNum, firstString);
        if (firstNum == 2) printf("-1 -1\n");
        input--;
    } while (firstNum != 1);
    // once a valid input is made, initialize the root node
    treeNode* root = newNode(0, firstString);
    
    // with the remaining inputs, do a series of functions to build a tree
    getInputs(input, root);
    
    // count the number of nodes in a tree
    int numWords = nodeCounter(root);
    // create an array of size n words
    nodeData** array = calloc(numWords, sizeof(nodeData*));
    // add words to the array in-order
    int index = 0;
    addToArray(root, array, &index);
    // sort the array based on frequency
    sort(array, 0, numWords-1);
    
    // print the array of words
    for (int i = 0; i < numWords; i++) {
        printf("%s %d\n", array[i] -> string, array[i] -> frequency);
    }
    
    // free the memory
    freeMemory(array, numWords);
    // quit

    return 0;
}

// ISSUES:
/*
 
 -1 -1 is always returned when doing a search. It has been found that adding to tree is broken and always thinks ROOT is NULL.
 
 */
