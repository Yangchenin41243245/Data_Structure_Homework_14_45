#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <chrono>
using namespace std;
using namespace std::chrono;

#define CASE_ITEMS 15000 // number of items in each case
#define CASES 10      // number of cases
#define RNGKEYS rand() % CASE_ITEMS
#define INSKEYS CASE_ITEMS - i // worst case for insertion sort
#define UNSORTED "./tosort.txt"
#define SORTED "./sorted.txt"
#define TIMEREC "./timer.txt"

struct node
{
    int data;
};

class entry
{
public:
    friend class node;
    long key;
    entry(long k, node *d)
    {
        key = k;
        data = d;
    }
    void setkey(int k) 
    {
        key = k; 
    }
    void outputkey(FILE *file)
    {
        fprintf(file, " key: %ld\n", key);
    }

private:
    node *data;
};

void makeCases(int, vector<vector<entry>> &, FILE *, string); // forward declare

// sort a case of array with insertion
// returns runtime in microseconds
unsigned long InsertionSort(vector<entry> &arr, int casenum)
{
    cout << "Start insertion sort\n";
    auto timer = high_resolution_clock::now();
    for (long i = 0; i < arr.size(); i++)
    {
        long min = i;
        for (long j = i + 1; j < arr.size(); j++) // get min key and swap then go forward and exclude sorted
        {
            if (arr[j].key < arr[min].key)
            {
                min = j;
            }
        }
        swap(arr[i], arr[min]);
    }
    auto stop = high_resolution_clock::now();
    auto dur = duration_cast<milliseconds>(stop - timer);
    // output performance record
    cout << "Sorted array in " << dur.count() << " microseconds\n";
    FILE *file = fopen(TIMEREC, "a");
    fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Insertion\n",
            casenum, CASE_ITEMS, dur.count());
    if (file != nullptr)
    {
        fclose(file);
    }
    return dur.count();
}

void QuickSortCore(vector<entry>&arr,int left,int right)
{
    
    // Start sort
    if (left < right)
    {
        entry *pivot = &arr[left];
        int i = left, j = right + 1;
        do
        {
            do
                i++;
            while (arr[i].key < pivot->key); // left push towards mid
            do
                j--;
            while (arr[j].key > pivot->key);
            if (i < j)
                swap(arr[i], arr[j]); // found both elements belonging to other side
        } while (i < j);
        swap(arr[left], arr[j]); // swap pivot with last element of left partition
        QuickSortCore(arr, left, j - 1);  // left partition
        QuickSortCore(arr, j + 1, right); // right partition
        }
}

unsigned long QuickSort(vector<entry> &arr, int casenum) // provide arr and specify the case num for print
{                                                        // returns run time in microseconds
    cout << "Start quick sort\n";
    auto timer = high_resolution_clock::now(); // record run time dur
    QuickSortCore(arr,0,arr.size()-1); //actual sort from 0 to n - 1

    auto stop = high_resolution_clock::now();
    auto dur = duration_cast<milliseconds>(stop - timer);
    // output performance record
    cout << "Sorted array in " << dur.count() << " microseconds\n";
    FILE *file = fopen(TIMEREC, "a");
    fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Quick\n",
            casenum, CASE_ITEMS, dur.count());
    if(file!=nullptr)
        fclose(file);
    return dur.count();
}

vector<entry> MergeCore(vector<entry>& a, vector<entry>& b) // core sorting func, uses iterators
{
    vector<entry> c;
    c.reserve(a.size() + b.size()); // reserve memory

    auto it_a = a.begin();
    auto it_b = b.begin();

    while (it_a != a.end() && it_b != b.end()) // compare and push to merged array
    {
        if (it_a->key < it_b->key)
        {
            c.push_back(std::move(*it_a)); // move smaller key to merged array
            ++it_a;
        }
        else
        {
            c.push_back(std::move(*it_b));
            ++it_b;
        }
    }

    // move remaining elements from either array
    while (it_a != a.end())
    {
        c.push_back(std::move(*it_a));
        ++it_a;
    }
    while (it_b != b.end())
    {
        c.push_back(std::move(*it_b));
        ++it_b;
    }

    return c; // return merged array
}

vector<entry> MergeCut(vector<entry>arr) //cut array func
{
    if(arr.size() <= 1) { return arr; } // nothing to sort, return the input array

    vector<entry> left, right;
    int mid = arr.size() / 2; // get mid point
    for (int i = 0; i < mid; i++)
        left.push_back(arr[i]); // cut left part
    for (int i = mid; i < arr.size(); i++)
        right.push_back(arr[i]); // cut right part

    //start recursion
    left = MergeCut(left); // cut left and assign sorted left part
    right = MergeCut(right); // cut right and assign sorted right part

    return MergeCore(left, right); //run sorting and join sorted parts (bottom layer: 1+1=2)
}

unsigned long MergeSort(vector<entry>&arr, int casenum) //run and record time
{
    cout << "Start merge sort\n";
    auto timer = high_resolution_clock::now(); // record run time dur
    arr = MergeCut(arr); // start sorting, assign sorted array back to arr

    auto stop = high_resolution_clock::now();
    auto dur = duration_cast<milliseconds>(stop - timer);
    // output performance record
    cout << "Sorted array in " << dur.count() << " microseconds\n";
    FILE *file = fopen(TIMEREC, "a");
    fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Merge\n",
            casenum, CASE_ITEMS, dur.count());
    if(file!=nullptr)
        fclose(file);
    return dur.count();
}

int main(void)
{
    remove(SORTED);
    cout << "Removed old " << SORTED << endl; // remove old files
    remove(UNSORTED);
    cout << "Removed old " << UNSORTED << endl;
    remove(TIMEREC);
    cout << "Removed old " << TIMEREC << endl;

    FILE *f_Unsorted = fopen(UNSORTED, "a"); // create new files
    FILE *f_Sorted = fopen(SORTED, "a");

    srand(time(0));                   // set random
    vector<vector<entry>> superarray; // all cases stored here

    makeCases(CASES, superarray, f_Unsorted, "MERGE"); // create cases, provide file path for unsorted cases

    for (int i = 0, c = 1; i < superarray.size(); i++, c++) // sort each case
    {   //CHANGE THIS BLOCK///////////////////////////////////////////////////////
        auto duration = MergeSort(superarray[i], c); // returns runtime after sorting
        //////////////////////////////////////////////////////////////////////////
        fprintf(f_Sorted, "\nCase %d of %lu items finished in %lu microseconds\n", c, CASE_ITEMS, duration);
        for (int j = 0; j < CASE_ITEMS; j++) // output keys of sorted array
        {
            superarray[i][j].outputkey(f_Sorted); // i for array, j for entry
        }
        cout << "output sorted array to file" << SORTED << endl;
    }

    fclose(f_Unsorted);
    fclose(f_Sorted);
    return 0;
}

void makeCases(int cases, vector<vector<entry>> &superarray, FILE *unsortedfile, string mode)
{
    for (int c = 0; c < cases; c++) // create cases
    {
        vector<entry> array;
        long key;
        fprintf(unsortedfile, "\nCase %d with %lu items:\n", c + 1, CASE_ITEMS);
        for (int i = 0; i < CASE_ITEMS; i++) // create array with nodes
        {
            node *n = new node;
            if (mode == "INSERTION" || mode == "MERGE") // generate key nums by mode
                key = INSKEYS; //do worst case = reverse list
            else if (mode == "RANDOM")
                key = RNGKEYS;
            else
                key = i;     // will process after array is created
            entry e(key, n); // create entry with key and node
            array.push_back(e);
        }
        if (mode == "QUICK" || mode == "HEAP") // do the Permutation() provided
        {
            for (int i = CASE_ITEMS - 1; i >= 2; i--)
            {
                int j = rand() % i + 1;
                swap(array[i], array[j]);
            }
        }
        for (int i = 0; i < CASE_ITEMS; i++) // output key of each node in array just made
        {
            array[i].outputkey(unsortedfile);
        }
        cout << "output unsorted array to file" << UNSORTED << endl;
        superarray.push_back(array); // add array to cases
        cout << "Created unsorted array for case #" << c + 1 << endl;
    }
}