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
#define CASES 5      // number of cases
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

struct result
{
    int64_t timer;
    vector<entry> arr2;
};

void makeCases(int, vector<vector<entry>> &, FILE *, string);

string printMem(int state)
{
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    unsigned long long memUsage = memInfo.WorkingSetSize / 1024;
    unsigned long long memPeak = memInfo.PeakWorkingSetSize / 1024;
    string ret0 = "Memory usage: " + to_string(memUsage) + " KB\n";
    string ret1 = "Peak memory usage: " + to_string(memPeak) + " KB\n";
    if (state == 0) return ret0;
    else if (state == 1) return ret1;
    return "";
}

result InsertionSort(vector<entry> arr, int casenum)
{
    result r;
    cout << "Start insertion sort\n";
    auto timer = chrono::high_resolution_clock::now();
    string recMem_Init = printMem(0);
    cout << recMem_Init << endl;
    FILE *file = fopen(TIMEREC, "a");
    fprintf(file, "Start Insertion sort case # %d \n[Init] %s", casenum, recMem_Init.c_str());

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
    fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Insertion\n[Final] %s\n",
            casenum, CASE_ITEMS, dur.count(), recMem_Fin.c_str());
    if (file != nullptr)
    {
        fclose(file);
    }
    return r;
}

void QuickSortCore(vector<entry> &arr, int left, int right)
{
    if (left < right)
    {
        entry pivot = arr[left];
        int i = left, j = right + 1;
        do
        {
            do i++; while (i <= right && arr[i].key < pivot.key);
            do j--; while (arr[j].key > pivot.key);
            if (i < j) swap(arr[i], arr[j]);
        } while (i < j);
        swap(arr[left], arr[j]);
        QuickSortCore(arr, left, j - 1);
        QuickSortCore(arr, j + 1, right);
    }
}

result QuickSort(vector<entry> arr, int casenum)
{
    result r;
    cout << "Start quick sort\n";
    auto timer = chrono::high_resolution_clock::now();
    string recMem_Init = printMem(0);
    cout << recMem_Init << endl;
    FILE *file = fopen(TIMEREC, "a");
    fprintf(file, "Start Quick sort case # %d \n[Init] %s", casenum, recMem_Init.c_str());

    QuickSortCore(arr, 0, arr.size() - 1);

    auto stop = chrono::high_resolution_clock::now();
    auto dur = chrono::duration_cast<chrono::microseconds>(stop - timer);
    r.arr2 = arr;
    r.timer = dur.count();
    string recMem_Fin = printMem(1);

    cout << "Sorted array in " << dur.count() << " microseconds\n";
    cout << recMem_Fin << endl;
    fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Quick\n[Final] %s\n",
            casenum, CASE_ITEMS, dur.count(), recMem_Fin.c_str());
    if (file != nullptr)
        fclose(file);
    return r;
}

vector<entry> MergeCore(vector<entry> a, vector<entry> b)
{
    vector<entry> c;
    c.reserve(a.size() + b.size());

    auto it_a = a.begin();
    auto it_b = b.begin();

    while (it_a != a.end() && it_b != b.end())
    {
        if (it_a->key < it_b->key)
        {
            c.push_back(std::move(*it_a));
            ++it_a;
        }
        else
        {
            c.push_back(std::move(*it_b));
            ++it_b;
        }
    }

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

    return c;
}

vector<entry> MergeCut(vector<entry> arr)
{
    if (arr.size() <= 1) { return arr; }

    vector<entry> left, right;
    int mid = arr.size() / 2;
    for (int i = 0; i < mid; i++)
        left.push_back(arr[i]);
    for (int i = mid; i < arr.size(); i++)
        right.push_back(arr[i]);

    left = MergeCut(left);
    right = MergeCut(right);

    return MergeCore(left, right);
}

result MergeSort(vector<entry> arr, int casenum)
{
    result r;
    cout << "Start merge sort\n";
    auto timer = chrono::high_resolution_clock::now();
    string recMem_Init = printMem(0);
    cout << recMem_Init << endl;
    FILE *file = fopen(TIMEREC, "a");
    fprintf(file, "Start Merge sort case # %d \n[Init] %s", casenum, recMem_Init.c_str());

    arr = MergeCut(arr);

    auto stop = chrono::high_resolution_clock::now();
    auto dur = chrono::duration_cast<chrono::microseconds>(stop - timer);
    r.arr2 = arr;
    r.timer = dur.count();
    string recMem_Fin = printMem(1);

    cout << "Sorted array in " << dur.count() << " microseconds\n";
    cout << recMem_Fin << endl;
    fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Merge\n[Final] %s\n",
            casenum, CASE_ITEMS, dur.count(), recMem_Fin.c_str());
    if (file != nullptr)
        fclose(file);
    return r;
}

void heapify(vector<entry> &arr, int n, int i)
{
    int max = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].key > arr[max].key)
        max = left;

    if (right < n && arr[right].key > arr[max].key)
        max = right;

    if (max != i)
    {
        swap(arr[i], arr[max]);
        heapify(arr, n, max);
    }
}

result HeapSort(vector<entry> arr, int casenum)
{
    result r;
    cout << "Start heap sort\n";
    auto timer = chrono::high_resolution_clock::now();
    string recMem_Init = printMem(0);
    cout << recMem_Init << endl;
    FILE *file = fopen(TIMEREC, "a");
    fprintf(file, "Start Heap sort case # %d \n[Init] %s", casenum, recMem_Init.c_str());

    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--)
    {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }

    auto stop = chrono::high_resolution_clock::now();
    auto dur = chrono::duration_cast<chrono::microseconds>(stop - timer);
    r.arr2 = arr;
    r.timer = dur.count();
    string recMem_Fin = printMem(1);

    cout << "Sorted array in " << dur.count() << " microseconds\n";
    cout << recMem_Fin << endl;
    fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Heap\n[Final] %s\n",
            casenum, CASE_ITEMS, dur.count(), recMem_Fin.c_str());
    if (file != nullptr)
        fclose(file);
    return r;
}

result CompositeSort(vector<entry> arr, int casenum)
{
    result r;
    cout << "Start composite sort\n";
    auto timer = chrono::high_resolution_clock::now();
    string recMem_Init = printMem(0);
    cout << recMem_Init << endl;
    FILE *file = fopen(TIMEREC, "a");
    fprintf(file, "Start Composite sort case # %d \n[Init] %s", casenum, recMem_Init.c_str());

    if (arr.size() <= 32)
    {
        r = InsertionSort(arr, casenum);
    }
    else if (arr.size() <= 1000)
    {
        r = QuickSort(arr, casenum);
    }
    else if (arr.size() <= 5000)
    {
        r = MergeSort(arr, casenum);
    }
    else
    {
        r = HeapSort(arr, casenum);
    }

    auto stop = chrono::high_resolution_clock::now();
    auto dur = chrono::duration_cast<chrono::microseconds>(stop - timer);
    r.timer = dur.count();
    string recMem_Fin = printMem(1);

    cout << "Sorted array in " << dur.count() << " microseconds\n";
    cout << recMem_Fin << endl;
    fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Composite\n[Final] %s\n",
            casenum, CASE_ITEMS, dur.count(), recMem_Fin.c_str());
    if (file != nullptr)
        fclose(file);
    return r;
}

int main(void)
{
    remove(SORTED);
    cout << "Removed old " << SORTED << endl;
    remove(UNSORTED);
    cout << "Removed old " << UNSORTED << endl;
    remove(TIMEREC);
    cout << "Removed old " << TIMEREC << endl;

    FILE *f_Unsorted = fopen(UNSORTED, "a");
    FILE *f_Sorted = fopen(SORTED, "a");

    srand(time(0));
    vector<vector<entry>> superarray[5]; // 增加一個用於 Composite Sort
    result result;

    makeCases(CASES, superarray[0], f_Unsorted, "INSERTION");
    makeCases(CASES, superarray[1], f_Unsorted, "QUICK");
    makeCases(CASES, superarray[2], f_Unsorted, "MERGE");
    makeCases(CASES, superarray[3], f_Unsorted, "HEAP");
    makeCases(CASES, superarray[4], f_Unsorted, "RANDOM"); // 用於 Composite Sort

    for (int type = 0; type < 5; type++)
    {
        for (int i = 0, caseNum = 1; i < superarray[type].size(); i++, caseNum++)
        {
            auto duration = 0;
            switch (type)
            {
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
            case 4:
                result = CompositeSort(superarray[type][i], caseNum);
                break;
            default:
                break;
            }
            duration = result.timer;
            fprintf(f_Sorted, "\nCase %d of %lu items finished in %lu microseconds\n", caseNum, CASE_ITEMS, duration);
            for (int j = 0; j < CASE_ITEMS; j++)
            {
                result.arr2[j].outputkey(f_Sorted);
            }
            cout << "output sorted array to file" << SORTED << endl;
        }
    }

    fclose(f_Unsorted);
    fclose(f_Sorted);
    return 0;
}

void makeCases(int cases, vector<vector<entry>> &superarray, FILE *unsortedfile, string mode)
{
    for (int c = 0; c < cases; c++)
    {
        vector<entry> array;
        long key;
        fprintf(unsortedfile, "\nCase %d with %lu items:\n", c + 1, CASE_ITEMS);
        for (int i = 0; i < CASE_ITEMS; i++)
        {
            node *n = new node;
            if (mode == "INSERTION" || mode == "QUICK" || mode == "MERGE")
                key = INSKEYS;
            else if (mode == "RANDOM")
                key = RNGKEYS;
            else
                key = i;
            entry e(key, n);
            array.push_back(e);
        }
        if (mode == "HEAP")
        {
            for (int i = CASE_ITEMS - 1; i >= 2; i--)
            {
                int j = rand() % i + 1;
                swap(array[i], array[j]);
            }
        }
        for (int i = 0; i < CASE_ITEMS; i++)
        {
            array[i].outputkey(unsortedfile);
        }
        cout << "output unsorted array to file" << UNSORTED << endl;
        superarray.push_back(array);
        cout << "Created unsorted array for case #" << c + 1 << endl;
    }
}
