//  Created by Harry James Hocker on 8/23/21.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// instead of typing 104 constantly
#define ARRAY_SIZE 104
 
 // functions:

// checks to see if the character at the index is ++ value. 'A' is the lowest value and 'M' is the highest value.
bool ascendingValues(char array[], int i) {
    if (array[i] == (array[i-2]+1)) return true;
    // hardcoded link of A and M to continue ascension
    if (array[i] == 'A' && array[i-2] == 'M') return true;
    else return false;
}

//function: convert deck into an easier-to-read format for comparing values IE: 2 = A, 3 = B etc.
void transformDecks(char * array) {
    
    // loop through every even-index character in array and change the card# values into ascending letters
    for (int i = 0; i < ARRAY_SIZE; (i+=2)) {
        // if 2, make A; if 3 make B; etc
        switch (array[i]) {
            case '2': array[i] = 'A'; break;
            case '3': array[i] = 'B'; break;
            case '4': array[i] = 'C'; break;
            case '5': array[i] = 'D'; break;
            case '6': array[i] = 'E'; break;
            case '7': array[i] = 'F'; break;
            case '8': array[i] = 'G'; break;
            case '9': array[i] = 'H'; break;
            case 'T': array[i] = 'I'; break;
            // this line for "J" is missing because J turns into J, redundant.
            case 'Q': array[i] = 'K'; break;
            case 'K': array[i] = 'L'; break;
            case 'A': array[i] = 'M'; break;
            default:break;
        }
        // loop through every odd-index character in array and change suite letter to a number
        for (int i = 1; i < ARRAY_SIZE; i+=2) {
            switch (array[i]) {
                case 'C' : array[i] = '1'; break;
                case 'D' : array[i] = '2'; break;
                case 'H' : array[i] = '3'; break;
                case 'S' : array[i] = '4'; break;
                default:break;
            }}}
    // purpose: this helps with debugging
}

void deckCompute(int input, char input_array[], int outputAscend[], int outputSuite[]) {
    
    int tempAscend = 1, tempSuite = 1;
    
    int j = 0; // to count through the decks
    int index = 0; // for appending values
    int eachValue; // each card value in a deck
    
    // get card data by scanning each new line and appending it to the static array
    for (eachValue = 0; eachValue<input*ARRAY_SIZE; eachValue++) {
        
        //every time i reaches the next deck of cards, take in input and transform it
        if (j == 0) {
            // append each line to the array at index in chunks of 52 indices
            scanf(" %s", &input_array[0]); // first half of deck
            scanf("%s", &input_array[52]); // second half
            // make the decks easier to understand
            transformDecks(input_array);
        }
        
        if (j > 1) {
            // if even, do ascending card value count
            if (j%2 == 0) {
                if (ascendingValues(input_array, j)) {
                    tempAscend++;
                    if (tempAscend > outputAscend[index]) outputAscend[index] = tempAscend;
                } else {
                    tempAscend = 1;
                }
                // if odd, do same-suit count
            } else {
                if (input_array[j] == input_array[j-2]) {
                    tempSuite++;
                    if (tempSuite > outputSuite[index]) outputSuite[index] = tempSuite;
                } else {
                    tempSuite = 1;
                }
            }
        }
        
        // when at the end of deck (index of 103)
        if(j == (ARRAY_SIZE-1)) {
            // finalize the values
            if (tempAscend > outputAscend[index]) outputAscend[index] = tempAscend;
            if (tempSuite > outputSuite[index]) outputSuite[index] = tempSuite;
            // reset counters
            tempAscend = 1; tempSuite = 1;
            // new deck
            j = 0;
            // next output index
            index++;
        } else {
            // continue counting
            j++;
        }
    }
}

int main() {
        
    int input;
    int outputSuite[25];
    int outputAscend[25];
    char input_array[ARRAY_SIZE+1];
    
    // get input for amount of decks (must be less than or equal to 25 else it repeats)
    do {
        printf("Enter the amount of decks (25 or fewer): \n");
        scanf(" %d", &input);
        printf("\n");
    } while (input > 25);
    
    printf("Enter deck data and press ENTER: \n");
    // get deck inputs and compute values
    deckCompute(input, input_array, outputAscend, outputSuite);

    // a spacer to make the console look nicer.
    printf("\n");

    // print out values
    for (int k = 0; k < input; k++) {
        printf("%d   %d\n", outputSuite[k], outputAscend[k]);
    }
    printf("\n"); return 0;
}
