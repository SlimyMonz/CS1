
//  Created by Harry James Hocker on 9/8/21.
//  Program 1  Smoothies
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// a single ingredient
struct item
{
    int itemID; // the index of the item, used to read in char** and print
    unsigned long int numParts; // how many "parts" needed in a recipe
} item;

// a recipe that stores an array of ingredient structs, number of ingredients, and total parts
struct recipe
{
    unsigned int numItems; //stores the number of different ingredients
    struct item* itemList; // dynamically allocated, each slot stores one ingredient item
    unsigned long long int totalParts; // from each ingredient, add all the numParts and put value here
} recipe;

// Pre-condition: 0 < numIngredients <= 100000
char** readIngredients(unsigned long long numIngredients)
{
    // Reads in numIngredients number of strings from standard input, allocates an array of strings
    char** ingredientArray = calloc(numIngredients, sizeof(char*));
    // for each numIngredient, scanf input and dynamically allocate string input
    for (int i = 0; i < numIngredients; i++) {
        char input[21];
        scanf(" %s", input);
        // sizes each individual string dynamically to be the proper size (string length plus 1)
        unsigned int wordSize = sizeof(input);
        ingredientArray[i] = calloc(wordSize+1, sizeof(char));
        // copy input to array
        strcpy(ingredientArray[i], input);
    }
    // returns a pointer to the array.
    return ingredientArray;
}

// Pre-condition: 0 < numItems <= 100
// reads in integer, scans for more inputs, returns a pointer to a recipe
struct recipe* readRecipe(unsigned int numItems)
{
    // create an itemList struct pointer
    struct item* itemList = calloc(numItems, sizeof(struct item));
    
    // create a recipe struct pointer
    struct recipe* recipePointer = malloc(sizeof(struct recipe));
    
    // make the itemList inside of recipe point to the itemList
    recipePointer -> itemList = itemList;
    
    // updates the struct value with the parameter value
    recipePointer -> numItems = numItems;
    
    // initialize totalparts
    recipePointer -> totalParts = 0;
    
    for (int i = 0; i < numItems; i++)
    {
        int itemID = 0;
        unsigned long int numParts = 0;
        
        // start scanf-ing in ingredient integers
        scanf(" %d", &itemID);
        scanf(" %ld", &numParts);
        
        // store each in item
        itemList[i].itemID = itemID;
        itemList[i].numParts = numParts;
        
        // for each item in the array of parts, add to total parts
        recipePointer -> totalParts += recipePointer -> itemList[i].numParts;
    }
    // return POINTER to a recipe
    return recipePointer;
};

// simply loop through the readRecipe function after allocating memory
struct recipe** readAllRecipes(int numRecipes)
{
    struct recipe** allRecipeArrays = calloc(numRecipes, sizeof(struct recipe*));
    for (int i = 0; i < numRecipes; i++)
    {
        int input = 0;
        scanf(" %d", &input);
        allRecipeArrays[i] = readRecipe(input);
    }
    return allRecipeArrays;
}

// take in nummber of smoothies and begin calculations
double* calculateOrder(int numSmoothies, struct recipe** recipeList, int numIngredients)
{
    // make an array that's as big as the amount of ingredients possible
    double* orderList = calloc(numIngredients, sizeof(double));
    
    for (int i = 0; i < numSmoothies; i++)
    {
        int index = 0;
        int weight = 0;
        
        scanf(" %d", &index);
        scanf(" %d", &weight);
        
        // get the total parts from the recipe
        unsigned long long int parts = recipeList[index] -> totalParts;
        // calculate the weight for each part
        double weightPerPart = (double) weight/parts;
        
        // for each item in the itemList, calculate weightPerPart * parts for each ingredient and add to array
        for (int j = 0; j < (recipeList[index] -> numItems); j++)
        {
            // store the calculation at the same index as the itemID
            orderList[recipeList[index] -> itemList[j].itemID] += (double) weightPerPart * (recipeList[index] -> itemList[j].numParts);
        }
    }
    return orderList;
}

// print out each ingredient name and associated number
void printOrder(char** ingredientNames, double* orderList, int numIngredients)
{
    for (int i = 0; i < numIngredients; i++)
    {
        if(orderList[i] != 0)
        {
            printf("%s  %.6f\n", ingredientNames[i], orderList[i]);
        }
    }
}

// frees all the ingredients and whole array
void freeIngredients(char** ingredientList, int numIngredients)
{
    for (int i = 0; i < numIngredients; i++) {
        // free each ingredient
        free(ingredientList[i]);
    }
    // free entire array
    free(ingredientList);
}

// frees all the itemLists, recipe structs, and entire array
void freeRecipes(struct recipe** allRecipes, int numRecipes)
{
    for (int i = 0; i < numRecipes; i++) {
        // free the array within the struct
        free(allRecipes[i] -> itemList);
        // free the entire struct
    }
    // free the entire array
    free(allRecipes);
}

// now everything runs as planned
int main()
{
    unsigned int numIngredients = 0;
    scanf(" %d", &numIngredients);
    char** ingredientNames = readIngredients(numIngredients);
    
    unsigned int numRecipes = 0;
    scanf(" %d", &numRecipes);
    struct recipe** recipeList = readAllRecipes(numRecipes);
    
    unsigned int numStores = 0;
    scanf(" %d", &numStores);
    // dynamically allocate an array of double pointers
    double** allOrders = calloc(numStores, sizeof(double*));
    for (int i = 0; i < numStores; i++)
    {
        int numSmoothies = 0;
        scanf(" %d", &numSmoothies);
        // set an array of pointers to each index
        allOrders[i] = calculateOrder(numSmoothies, recipeList, numIngredients);
    }
    
    // print all the individual Store orders
    for (int j = 0; j < numStores; j++)
    {
        printf("Store #%d:\n", (j+1));
        printOrder(ingredientNames, allOrders[j], numIngredients);
    }
    
    // free up all the memory
    freeIngredients(ingredientNames, numIngredients);
    freeRecipes(recipeList, numRecipes);
    free(allOrders);
    
    // end program
    return 0;
}
