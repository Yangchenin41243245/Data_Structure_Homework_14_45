#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
using namespace std;

#define CASE_ITEMS 6000 // number of items in each case
#define CASES 5         // number of cases
#define RNGKEYS rand() % CASE_ITEMS
#define INSKEYS CASE_ITEMS - i // worst case for insertion sort
#define UNSORTED "./tosort.txt"
#define SORTED "./sorted.txt"
#define TIMEREC "./timer.txt"
#define COMPOSITE_TIMEREC "./composite_timer.txt" // Added for Composite Sort

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
    void outputkey(FILE *file) const // Modified to const
    {
        fprintf(file, " key: %ld\n", key);
    }
private:
    node *data;
};

struct result
{
    int64_t timer;
    vector<entry> arr2;
};

void makeCases(int, vector<vector<entry>> &, FILE *, string);

// Utility function to print memory usage
string printMem(int state)
{
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    unsigned long long memUsage = memInfo.WorkingSetSize / 1024; // in KB
    unsigned long long memPeak = memInfo.PeakWorkingSetSize / 1024; 
    string ret0 = "Memory usage: " + to_string(memUsage) + " KB\n";
    string ret1 = "Peak memory usage: " + to_string(memPeak) + " KB\n";
    if(state == 0) return ret0;
    else if(state == 1) return ret1;
    return "";
}

// Modified InsertionSort with writeToFile parameter
result InsertionSort(vector<entry> arr, int casenum, bool writeToFile = true)
{
    result r;
    cout << "Start insertion sort\n";
    auto timer = chrono::high_resolution_clock::now();
    string recMem_Init = printMem(0);
    cout << recMem_Init << endl;

    for (int i = 1; i < arr.size(); i++)
    {
        entry temp = arr[i];
        int j = i - 1;
        while (j >= 0 && temp.key < arr[j].key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
    
    auto stop = chrono::high_resolution_clock::now();
    auto dur = chrono::duration_cast<chrono::microseconds>(stop - timer);
    r.arr2 = arr;
    r.timer = dur.count();
    string recMem_Fin = printMem(1);

    cout << "Sorted array in " << dur.count() << " microseconds\n";
    cout << recMem_Fin << endl;

    if (writeToFile) {
        FILE *file = fopen(TIMEREC, "a");
        fprintf(file, "Start insertion sort case # %d \n[Init] %s", casenum, recMem_Init.c_str());
        fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Insertion\n[Final] %s\n",
                casenum, CASE_ITEMS, dur.count(), recMem_Fin.c_str());
        fclose(file);
    }
    return r;
}

// Placeholder for QuickSort (modified similarly)
result QuickSort(vector<entry> arr, int casenum, bool writeToFile = true)
{
    result r;
    cout << "Start quick sort\n";
    auto timer = chrono::high_resolution_clock::now();
    string recMem_Init = printMem(0);
    cout << recMem_Init << endl;

    sort(arr.begin(), arr.end(), [](entry a, entry b) { return a.key < b.key; }); // Simplified

    auto stop = chrono::high_resolution_clock::now();
    auto dur = chrono::duration_cast<chrono::microseconds>(stop - timer);
    r.arr2 = arr;
    r.timer = dur.count();
    string recMem_Fin = printMem(1);

    cout << "Sorted array in " << dur.count() << " microseconds\n";
    cout << recMem_Fin << endl;

    if (writeToFile) {
        FILE *file = fopen(TIMEREC, "a");
        fprintf(file, "Start quick sort case # %d \n[Init] %s", casenum, recMem_Init.c_str());
        fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Quick\n[Final] %s\n",
                casenum, CASE_ITEMS, dur.count(), recMem_Fin.c_str());
        fclose(file);
    }
    return r;
}

// Placeholder for MergeSort (modified similarly)
result MergeSort(vector<entry> arr, int casenum, bool writeToFile = true)
{
    result r;
    cout << "Start merge sort\n";
    auto timer = chrono::high_resolution_clock::now();
    string recMem_Init = printMem(0);
    cout << recMem_Init << endl;

    sort(arr.begin(), arr.end(), [](entry a, entry b) { return a.key < b.key; }); // Simplified

    auto stop = chrono::high_resolution_clock::now();
    auto dur = chrono::duration_cast<chrono::microseconds>(stop - timer);
    r.arr2 = arr;
    r.timer = dur.count();
    string recMem_Fin = printMem(1);

    cout << "Sorted array in " << dur.count() << " microseconds\n";
    cout << recMem_Fin << endl;

    if (writeToFile) {
        FILE *file = fopen(TIMEREC, "a");
        fprintf(file, "Start merge sort case # %d \n[Init] %s", casenum, recMem_Init.c_str());
        fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Merge\n[Final] %s\n",
                casenum, CASE_ITEMS, dur.count(), recMem_Fin.c_str());
        fclose(file);
    }
    return r;
}

// Placeholder for HeapSort (modified similarly)
result HeapSort(vector<entry> arr, int casenum, bool writeToFile = true)
{
    result r;
    cout << "Start heap sort\n";
    auto timer = chrono::high_resolution_clock::now();
    string recMem_Init = printMem(0);
    cout << recMem_Init << endl;

    sort(arr.begin(), arr.end(), [](entry a, entry b) { return a.key < b.key; }); // Simplified

    auto stop = chrono::high_resolution_clock::now();
    auto dur = chrono::duration_cast<chrono::microseconds>(stop - timer);
    r.arr2 = arr;
    r.timer = dur.count();
    string recMem_Fin = printMem(1);

    cout << "Sorted array in " << dur.count() << " microseconds\n";
    cout << recMem_Fin << endl;

    if (writeToFile) {
        FILE *file = fopen(TIMEREC, "a");
        fprintf(file, "Start heap sort case # %d \n[Init] %s", casenum, recMem_Init.c_str());
        fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Heap\n[Final] %s\n",
                casenum, CASE_ITEMS, dur.count(), recMem_Fin.c_str());
        fclose(file);
    }
    return r;
}

// New CompositeSort function
result CompositeSort(vector<entry> arr, int casenum, bool writeToFile = true)
{
    result r;
    string sort_name = "composite";
    cout << "Start " << sort_name << " sort\n";
    string memInit = printMem(0);
    cout << memInit;

    FILE* file = nullptr;
    if (writeToFile) {
        file = fopen(COMPOSITE_TIMEREC, "a");
        fprintf(file, "Start Composite sort case # %d \n[Init] %s", casenum, memInit.c_str());
    }

    if (arr.size() <= 32) {
        r = InsertionSort(arr, casenum, false);
    }
    else if (arr.size() > 32 && arr.size() <= 1000) {
        r = HeapSort(arr, casenum, false);
    } 
    else if (arr.size() > 1000 &&arr.size() <= 5000) {
        r = HeapSort(arr, casenum, false);
    } else {
        r = QuickSort(arr, casenum, false);
    }

    int64_t duration = r.timer;
    string memFin = printMem(1);

    if (writeToFile) {
        fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Composite\n[Final] %s\n",
                casenum, arr.size(), duration, memFin.c_str());
        fclose(file);
    }

    cout << "Sorted array in " << duration << " microseconds\n";
    cout << memFin;

    return r;
}

void makeCases(int cases, vector<vector<entry>> &superarray, FILE *unsortedfile, string mode)
{
    for (int c = 0; c < cases; c++) {
        vector<entry> array;
        for (int i = 0; i < CASE_ITEMS; i++) {
            node *n = new node;
            long key;
            if (mode == "INSERTION" || mode == "QUICK" || mode == "MERGE")
                key = INSKEYS;
            else if (mode == "RANDOM")
                key = RNGKEYS;
            else
                key = i;
            entry e(key, n);
            array.push_back(e);
        }
        if (mode == "HEAP") {
            for (int i = CASE_ITEMS - 1; i >= 2; i--) {
                int j = rand() % i + 1;
                swap(array[i], array[j]);
            }
        }
        for (int i = 0; i < CASE_ITEMS; i++) {
            array[i].outputkey(unsortedfile);
        }
        cout << "output unsorted array to file " << UNSORTED << endl;
        superarray.push_back(array);
        cout << "Created unsorted array for case #" << c + 1 << endl;
    }
}

int main(void)
{
    remove(SORTED);
    cout << "Removed old " << SORTED << endl;
    remove(UNSORTED);
    cout << "Removed old " << UNSORTED << endl;
    remove(TIMEREC);
    cout << "Removed old " << TIMEREC << endl;
    remove(COMPOSITE_TIMEREC);
    cout << "Removed old " << COMPOSITE_TIMEREC << endl;

    FILE *f_Unsorted = fopen(UNSORTED, "a");
    FILE *f_Sorted = fopen(SORTED, "a");

    srand(time(0));
    vector<vector<entry>> superarray[5]; // Changed from [4] to [5]
    result result;

    makeCases(CASES, superarray[0], f_Unsorted, "INSERTION"); 
    makeCases(CASES, superarray[1], f_Unsorted, "QUICK"); 
    makeCases(CASES, superarray[2], f_Unsorted, "MERGE"); 
    makeCases(CASES, superarray[3], f_Unsorted, "HEAP"); 
    makeCases(CASES, superarray[4], f_Unsorted, "RANDOM"); // Added for Composite Sort

    for (int type = 0; type < 4; type++) {
        for (int i = 0, caseNum = 1; i < superarray[type].size(); i++, caseNum++) {
            switch (type) {
            case 0:
                result = InsertionSort(superarray[type][i], caseNum);
                break;
            case 1:
                result = QuickSort(superarray[type][i], caseNum);
                break;
            case 2:
                result = MergeSort(superarray[type][i], caseNum);
                break;
            case 3:
                result = HeapSort(superarray[type][i], caseNum);
                break;
            }
            fprintf(f_Sorted, "\nCase %d of %lu items finished in %lu microseconds\n", caseNum, CASE_ITEMS, result.timer);
            for (int j = 0; j < CASE_ITEMS; j++) {
                result.arr2[j].outputkey(f_Sorted);
            }
            cout << "output sorted array to file " << SORTED << endl;
        }
    }

    // Run Composite Sort on random cases
    for (int i = 0; i < superarray[4].size(); i++) {
        int caseNum = i + 1;
        result = CompositeSort(superarray[4][i], caseNum);
        fprintf(f_Sorted, "\nComposite Sort Case %d finished in %ld us\n", caseNum, result.timer);
        for (const auto& e : result.arr2) {
            e.outputkey(f_Sorted);
        }
        cout << "output sorted array to file " << SORTED << endl;
    }

    fclose(f_Unsorted);
    fclose(f_Sorted);
    return 0;
}
