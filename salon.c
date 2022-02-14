//
//  main.c
//  P6-2
//
//  Created by Harry Hocker on 11/28/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD 21
#define T 1
#define F 0
#define STYLISTS 10
#define ROOT 1

typedef struct customer
{
    char name[WORD];
    char stylist[WORD];
    unsigned points;
    unsigned arrivalTime;
    unsigned startTime;
    unsigned hairTime;
    unsigned endTime;
} customer;

typedef struct stylist
{
    char name[WORD];
    unsigned size;
    struct customer** heap;
    
} stylist;

// ********** ADD-NEW FUNCTIONS **********

// return a new customer pointer with default values
customer* newCustomer(void)
{
    customer* newCustomer = malloc(sizeof(customer));
    
    strcpy(newCustomer -> name, "EMPTY");
    strcpy(newCustomer -> stylist, "NONE");
    
    newCustomer -> points = 0;
    newCustomer -> arrivalTime = 0;
    newCustomer -> startTime = 0;
    newCustomer -> hairTime = 0;
    newCustomer -> endTime = 0;
    
    return newCustomer;
}

// returns a new stylist with default values
stylist* newStylist(void)
{
    stylist* newStylist = malloc(sizeof(stylist));
    
    strcpy(newStylist -> name, "EMPTY");
    newStylist -> size = 0;
    newStylist -> heap = malloc(sizeof(customer*));
    
    return newStylist;
}

// Stylist setters

void set_stylist_name(stylist* stylist, char* name) {
    strcpy(stylist -> name, name);
}

void resize_heap(stylist* stylist, unsigned size) {
    if (size < 1) return;
    stylist -> heap = realloc(stylist -> heap, (size+1) * sizeof(customer*));
}


// ************** FUNCTIONS *************

// wrapper function to assign customer info
void scan_customer_info(customer* customer)
{
    char name[WORD], stylist[WORD];
    unsigned arrival = 0, points = 0, hair = 0;
    
    scanf("%u %s %s %u %u", &arrival, name, stylist, &points, &hair);
    
    customer -> arrivalTime = arrival;
    strcpy(customer -> name, name);
    strcpy(customer -> stylist, stylist);
    customer -> points = points;
    customer -> hairTime = hair;
}

// given heap and customer indexes, swap addresses
void swapCustomers(customer** heap, unsigned index1, unsigned index2)
{
    customer* temp = heap[index1];
    heap[index1] = heap[index2];
    heap[index2] = temp;
}

// checks if customer1 has higher priority than customer2. 1 if true, 0 if false.
unsigned higher_priority(customer* customer1, customer* customer2)
{
    // store points values into temporary variables
    unsigned points1 = customer1 -> points;
    unsigned points2 = customer2 -> points;
    
    // if c1 points are greater than c2, return 1 else return 0 or continue
    if (points1 > points2) {
        return T;
    } else if (points1 < points2) {
        return F;
    }
    
    // store stylist names into temporary values
    char stylist1[WORD]; strcpy(stylist1, customer1 -> stylist);
    char stylist2[WORD]; strcpy(stylist2, customer2 -> stylist);
    // flags for if the stylist name is "NONE"
    unsigned flag1 = strcmp(stylist1, "NONE");
    unsigned flag2 = strcmp(stylist2, "NONE");
    
    // if c1 has a preference and c2 does not, then return 1
    if (!flag1 && flag2) return T;
    // if c1 has no preference and c2 does, then return 0
    if (flag1 && !flag2) return F;
    
    // store names in temp variables for easier comparison
    char name1[WORD]; strcpy(name1, customer1 -> name);
    char name2[WORD]; strcpy(name2, customer2 -> name);
    // calculate string lengths for each name once
    unsigned long strlen1 = strlen(name1);
    unsigned long strlen2 = strlen(name2);
    
    // compare names alphabetically as a last-resort
    for (int i = 0; i < strlen1 && i < strlen2; i++) {
        if (name1[i] < name2[i]) return T;
        if (name1[i] > name2[i]) return F;
    }
    // this should never be reached, but we should assume that the shorter name is alphabetically higher
    if (strlen1 < strlen2) return T; else return F;
}

// function to percolate upwards when added to heap
void percolate_UP(customer** heap, unsigned max_index)
{
    // if we're right at the root, then return
    if (max_index == 1 || max_index/2 == 1) return;
    
    unsigned node = max_index, root = max_index/2;
    
    if (higher_priority(heap[node], heap[root])) {
        swapCustomers(heap, node, root);
        percolate_UP(heap, root);
    }
}

// use this after "removing" max value and swapping with last node
void percolate_DOWN(customer** heap, unsigned maxIndex, unsigned index)
{
    unsigned node = index;
    unsigned left = 2 * index;
    unsigned right = 2 * index + 1;
    
    // if the first leaf is greater than the max index, return
    if (left > maxIndex) return;
    
    // if there is only a left node, do operation and return
    if (left == maxIndex)
    {
        if (higher_priority(heap[left], heap[node]))
        {
            swapCustomers(heap, left, node);
        }
        // whether or not we swap, we return
        return;
    }
    // finds the highest priority value node of the two
    unsigned swap;
    if (higher_priority(heap[right], heap[left])) {
        swap = right;
    } else {
        swap = left;
    }
    // if a child node is greater than current node, swap and percolate down
    if (higher_priority(heap[swap], heap[node])) {
        swapCustomers(heap, swap, node);
        // percolate down to the swapped
        percolate_DOWN(heap, maxIndex, swap);
    }
}

// return the stylistList index for where to insert a customer based on preference and heap size
unsigned choose_stylist_index(stylist** list, char* stylist_name, unsigned num_stylists)
{
    // index to be given
    unsigned index = 0;
    // only one stylist, return index
    if (num_stylists == 1) return 0;
    
    // if there is a preference search for the stylist
    if (strcmp(stylist_name, "NONE") != 0) {
        for (int i = 0; i < num_stylists; i++)
        {
            // if the strings compare, return the index of said string
            if (strcmp(list[i] -> name, stylist_name) == 0) return i;
        }
    }
    
    // if the first index has no queue, add to it
    if (list[0] -> size == 0) return 0;
    
    // if no stylist was found or there was no preference:
    for (int i = 1; i < num_stylists; i++)
    {
        unsigned size1 = list[index] -> size;
        unsigned size2 = list[i] -> size;
        // if the queue is empty, just place it in
        if (size2 == 0) return i;
        // otherwise, find the smallest queue
        if (size1 > size2) index = i;
    }
    // if a queue wasn't empty, return the smallest queue
    return index;
}

// returns the stylist index of which customer has the next lowest time
unsigned find_next_lowest_time(stylist** list, unsigned stylists)
{
    unsigned index = 0;
    unsigned time1 = 0, time2 = 0;
    
    // check each root customer time and return the index
    for (int i = 1; i < stylists; i++)
    {
        customer customer1 = *list[index] -> heap[ROOT];
        customer customer2 = *list[i] -> heap[ROOT];
        
        time1 = customer1.endTime;
        time2 = customer2.endTime;
        
        if (time1 > time2) index = i;
    }
    return index;
}

// given the customer node, print out all the required information
void print_customer(customer* customer, char* stylist)
{
    // get strings
    char name[WORD]; strcpy(name, customer -> name);
    // get unsigned integers
    unsigned time = customer -> endTime;
    
    // calculate points
    unsigned hairTime = customer -> hairTime;
    unsigned points = customer -> points;
    unsigned calcPoints = points + (hairTime/10);
    // print them out
    printf("%s %u %u %s\n", name, time, calcPoints, stylist);
}

// wrapper function to add a customer to a stylist
void add_to_stylist(stylist **list, unsigned int stylists) {
    customer* new = newCustomer();
    // scan in customer info
    scan_customer_info(new);
    // find next most-empty stylist index or preferred stylist
    unsigned next = choose_stylist_index(list, new -> stylist, stylists);
    // increase their size
    list[next] -> size++;
    // increase stylist heapsize
    resize_heap(list[next], list[next] -> size);
    // add customer to heap index
    list[next] -> heap[list[next] -> size] = new;
    // percolate up after adding
    percolate_UP(list[next] -> heap, list[next] -> size);
}

// the big main program
int main(void) {
    
    // get base values
    unsigned num_customers = 0, num_stylists = 0;
    scanf("%u %u", &num_customers, &num_stylists);
    if (num_customers == 0 || num_stylists == 0) return -1;
    
    // create empty list of stylists
    stylist** list = malloc(num_stylists * sizeof(stylist*));
    for (int i = 0; i < num_stylists; i++) list[i] = NULL;
    // populate the list of stylists
    for (int j = 0; j < num_stylists; j++)
    {
        // make a new stylist
        list[j] = newStylist();
        // scan in name
        char name[WORD];
        scanf("%s", name);
        // copy name into stylist
        strcpy(list[j] -> name, name);
    }
    
    // start adding customers to the stylists
    for (int k = 0; k < num_customers; k++)
    {
        add_to_stylist(list, num_stylists);
    }
    
    // after setting the heaps, calculate each root endTime
    for (int l = 0; l < num_stylists; l++)
    {
        unsigned start = list[l] -> heap[ROOT] -> arrivalTime;
        unsigned hair = list[l] -> heap[ROOT] -> hairTime;
        list[l] -> heap[ROOT] -> endTime = (start + hair);
    }
    
    // start printing customers out
    for (int m = 0; m < num_customers; m++)
    {
        // get the customer that is next to be finished
        unsigned next = find_next_lowest_time(list, num_stylists);
        // set variables for easier modifications
        stylist* stylist = list[next];
        customer** heap = stylist -> heap;
        customer* root = heap[ROOT];
        // save the end time to eventually add to the new root
        unsigned time = root -> endTime;
        // print out the customer that finished
        print_customer(root, stylist -> name);
        // swap with last customer
        swapCustomers(heap, ROOT, stylist -> size);
        // free the last customer
        free(heap[stylist -> size]);
        stylist -> size--;
        // resize stylist heap
        resize_heap(list[next], stylist -> size);
       
        // if there are no more customers for the stylist:
        if (stylist -> size == 0) {
            // free the heap
            free(heap);
            // free the stylist
            free(stylist);
            // reorder the list of stylists after freeing one
            for (int n = next + 1; n < num_stylists; n++) {
                list[n-1] = list[n];
            }
            num_stylists--;
            continue;
        }
        // percolate the root down
        percolate_DOWN(stylist -> heap, stylist -> size, ROOT);
        // reset the root to modify stuff
        root = stylist -> heap[ROOT];
        // add the time
        root -> endTime = root -> hairTime + time;
    }
    // all done, free the list
    free(list);
    
    return 0;
}

