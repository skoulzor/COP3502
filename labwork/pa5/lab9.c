// Sorting Lab Skeleton Code

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXARRAYS 6
#define MAXVALUE 1000000

void randArray(int arr[], int size, int maxValue);
void bubbleSort(int arr[], int n);
void insertionSort(int arr[], int n);
void selectionSort(int arr[], int n);
void merge(int arr[], int l, int m, int r);
void mergeSort(int arr[], int l, int r);
void arrayCopy(int original[], int copy[], int size);
long timediff(clock_t t1, clock_t t2);
void swap(int *xp, int *yp);
int partition(int *vals, int low, int high);
void quickSort(int *numbers, int low, int high);
void insertionSortModified(int arr[], int l, int r);
void mergeInsertionSort(int arr[], int l, int r);

int main()
{
    int sizes[MAXARRAYS] = {10000, 20000, 30000, 40000, 50000, 60000};
    int *originalArray, *sortedArray;
    clock_t start, end;
    long duration;

    for (int i = 0; i < MAXARRAYS; i++) 
    {
        // create the arrays and fill them with random numbers
        originalArray = malloc(sizeof(int) * sizes[i]);
        sortedArray = malloc(sizeof(int) * sizes[i]);

        randArray(originalArray, sizes[i], MAXVALUE);
        arrayCopy(originalArray, sortedArray, sizes[i]);

        // SORTING 1: BUBBLE SORT
        start = clock();
        bubbleSort(sortedArray, sizes[i]);
        end = clock();
        duration = timediff(start, end);
        printf("\nSorting %d values takes %ld milliseconds using Bubble Sort\n", sizes[i], duration);
        arrayCopy(originalArray, sortedArray, sizes[i]);

        // SORTING 2: INSERTION SORT
        start = clock();
        insertionSort(sortedArray, sizes[i]);
        end = clock();
        duration = timediff(start, end);
        printf("\nSorting %d values takes %ld milliseconds using Insertion Sort\n", sizes[i], duration);
        arrayCopy(originalArray, sortedArray, sizes[i]);

        // SORTING 3: SELECTION SORT
        start = clock();
        selectionSort(sortedArray, sizes[i]);
        end = clock();
        duration = timediff(start, end);
        printf("\nSorting %d values takes %ld milliseconds using Selection Sort\n", sizes[i], duration);
        arrayCopy(originalArray, sortedArray, sizes[i]);

        // SORTING 4: MERGE SORT
        start = clock();
        mergeSort(sortedArray, 0, sizes[i] - 1);
        end = clock();
        duration = timediff(start, end);
        printf("\nSorting %d values takes %ld milliseconds using Merge Sort\n", sizes[i], duration);
        arrayCopy(originalArray, sortedArray, sizes[i]);

        // SORTING 5: MERGE INSERTION SORT
        start = clock();
        mergeInsertionSort(sortedArray, 0, sizes[i] - 1);
        end = clock();
        duration = timediff(start, end);
        printf("\nSorting %d values takes %ld milliseconds using Merge Insertion Sort\n", sizes[i], duration);
        arrayCopy(originalArray, sortedArray, sizes[i]);

        // SORTING 6: QUICK SORT
        start = clock();
        quickSort(sortedArray, 0, sizes[i] - 1);
        end = clock();
        duration = timediff(start, end);
        printf("\nSorting %d values takes %ld milliseconds using Quick Sort\n", sizes[i], duration);
        
        // Free memory after all six sorting algorithms finish
        free(originalArray);
        free(sortedArray);
    }

    return 0;
}

long timediff(clock_t t1, clock_t t2)
{
    //calculate the elapsed time of the passed clock_t variables using the formula below and return the value
    long elapsed = ((double) t2-t1) / CLOCKS_PER_SEC * 1000;

    return elapsed;
}

void swap(int *xp, int *yp)
{
    //swap the passed integers using pass by reference
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void randArray(int arr[], int size, int maxValue)
{
    for (int i = 0; i < size; i++)
    {
        arr[i] = rand() % maxValue + 1;
    }
}

void arrayCopy(int original[], int copy[], int size)
{
    for (int i = 0; i < size; i++)
    {
        copy[i] = original[i];
    }
}

void bubbleSort(int arr[], int n)
{
	int i, j;
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }
		
}

void insertionSort(int arr[], int n)
{
    int i, item, j;
    for (i = 1; i < n; i++)
    {
        item = arr[i];
        
        /* Move elements of arr[0..i-1], that are
        greater than key, to one position ahead
        of their current position */
        for (j = i - 1; j >= 0; j--)
        {
            if (arr[j] > item)
            {
                arr[j + 1] = arr[j];
            }
            else
            {
                break;
            }
        }
        arr[j + 1] = item;
    }
}

void insertionSortModified(int arr[], int l, int r)
{
    int i, item, j;
    // Sort only the elements between indexes l and r
    for (i = l + 1; i <= r; i++)
    {
        item = arr[i];
        
        // Compare item with elements before it in the subarray
        for (j = i - 1; j >= l; j--)
        {
            // Move the larger element one position to the right
            if (arr[j] > item)
            {
                arr[j + 1] = arr[j];
            }
            else
            {
                // Stop when the correct postion is found 
                break;
            }
        }
        // Place item into its correct position 
        arr[j + 1] = item;
    }
}

void mergeInsertionSort(int arr[], int l, int r)
{
    // Check if the subarray has 25 or fewer elements
    if (r - l + 1 <= 25)
    {
        // Use insertion sort for small subarrays
        insertionSortModified(arr, l, r);
    }
    else
    {
        // Get the mid point
        int m = (l + r) / 2;
        // Sort first and second halves
        mergeInsertionSort(arr, l, m);
        mergeInsertionSort(arr, m + 1, r);
        // Merge the two sorted halves
        merge(arr, l, m, r);
    }
    
}

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    
    /* create temp arrays */
    int *L = (int*) malloc(n1 * sizeof(int));
    int *R = (int*) malloc(n2 * sizeof(int));
    
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
    {
        L[i] = arr[l + i];
    }
    for (j = 0; j < n2; j++) 
    {
        R[j] = arr[m + 1+ j];
    }
    
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    /* Copy the remaining elements of L[], if there are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    /* Copy the remaining elements of R[], if there are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        // get the mid point
        int m = (l + r) / 2;
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        // Merge the two sorted halves
        merge(arr, l, m, r);
    }
}

int partition(int *vals, int low, int high)
{
    // Pick a random partition element and swap it into index low.
    int i = low + rand() % (high - low + 1);
    swap(&vals[low], &vals[i]);
    
    int lowpos = low; //here is our pivot located.
    low++; //our starting point is after the pivot.
    
    // Run the partition so long as the low and high counters don't cross.
    while (low <= high)
    {
        // Move the low pointer until we find a value too large for this side.
        while (low <= high && vals[low] <= vals[lowpos])
        {
            low++;
        } 
        // Move the high pointer until we find a value too small for this side.
        while (high >= low && vals[high] > vals[lowpos])
        {
            high--;
        } 
        // Now that we've identified two values on the wrong side, swap them.
        if (low < high) 
        {
            swap(&vals[low], &vals[high]);
        }
    }

    // Swap the pivot element element into its correct location.
    swap(&vals[lowpos], &vals[high]);
    return high; //return the partition point
}

void quickSort(int* numbers, int low, int high) 
{
    // Only have to sort if we are sorting more than one number
    if (low < high) 
    {
        int split = partition(numbers, low, high);
        quickSort(numbers, low, split - 1);
        quickSort(numbers, split + 1, high);
    }
}

void selectionSort(int arr[], int n)
{
    int i, j, min_idx;
    // One by one move boundary of unsorted subarray
    for (i = 0; i < n - 1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[min_idx])
            {
                min_idx = j;
            }
        }
        
        // Swap the found minimum element with the first element
        swap(&arr[i], &arr[min_idx]);
    }
}