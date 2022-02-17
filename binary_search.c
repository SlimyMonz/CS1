//
//  Binary Search
//
//  Created by Harry James Hocker on 10/10/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MIN 0
#define MAX 128

int binarySearch(int val, int low_index, int high_index, int* sorted_array)
{
    // set the midpoint to be between the high and low index
    int mid = (low_index+high_index)/2;
    
    // while the high index is greater than the low index, do binary search
    if (high_index >= low_index) {
        if (sorted_array[mid] == val) return mid;
        else if (sorted_array[mid] > val)
            return binarySearch(val, low_index, mid-1, sorted_array);
        else if (sorted_array[mid] < val)
            return binarySearch(val, mid+1, high_index, sorted_array);
    }
    // returns -1 if the high index is ever less than low index, meaning the number does not exist.
    return -1;
}

int main(int argc, const char * argv[]) {
    
    int found = -1;
    
    do {
    
    int array[MAX];
    
    srand(0);
    
    for (int i = 0; i < MAX; i++) {
        array[i] = (int) (pow(2, i));
    }
    
    found = binarySearch(32, MIN, MAX-1, array);
    
    if (found != -1) {
        printf("Found at index %d.\n", found);
    } else {
        printf("Not found.\n");
    }
        
    }while (found == -1);
    
    return 0;
}
