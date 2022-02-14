//
//  main.c
//  P4
//
//  Created by Harry James Hocker on 10/16/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void merge(long long int* array, long long int start, long long int middle, long long int end)
{
    // a: iteration through first half
    // b: iteration through second half
    // i: iteration through entire array
    long long int a = start, b = middle + 1, i = 0;
    // create a temporary array for sorting
    long long int length = end - start + 1;
    long long int* sorted = calloc(length, sizeof(long long int));
    // use a and b to prevent out-of-bounds of either array half
    while (a < middle+1 && b < end+1)
    {
        if (array[a] <= array[b])
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

// a is first index, z is final index
void sort(long long int* array, long long int a, long long int z)
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

// this function takes in an ALREADY-SORTED array, the max number of pages, and the array length
long long int returnNumBooks(long long int* array, long long int numPages, long long int length)
{
    long long int addPages = 0, i = 0;
    while (addPages < numPages && i < length)
    {
        addPages += array[i];
        if (addPages > numPages) continue;
        i++;
    }
    return i;
}

int main(void)
{
    int cases = 0;
    scanf(" %d", &cases);
    long long int answers[cases];
    // repeat for X amount of cases.
    for (int i = 0; i < cases; i++)
    {
        long long int numBooks = 0, numPages = 0;
        // get the number of books and how many pages person is willing to read
        scanf(" %lld %lld", &numBooks, &numPages);
        // create the array based on the number of books
        long long int* allBookPages = calloc(numBooks, sizeof(long long int));
        // populate the array
        for (int j = 0; j < numBooks; j++) {
            scanf(" %lld", &allBookPages[j]);
        }
        // sort the array
        sort(allBookPages, 0, numBooks-1);
        // find how many books are needed to satisfy numPages
        answers[i] = returnNumBooks(allBookPages, numPages, numBooks);
        // free the array
        free(allBookPages);
    }
    // print out all the answers in order
    for (int j = 0; j < cases; j++) {
        printf("%lld\n", answers[j]);
    }
    return 0;
}
