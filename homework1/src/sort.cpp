#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include <string>
#pragma comment(lib, "psapi.lib")

using namespace std;
using namespace chrono;

// 設定常數
#define CASE_ITEMS 6000
#define CASES 5
#define UNSORTED "./tosort.txt"
#define SORTED "./sorted.txt"
#define TIMEREC "./timer.txt"
#define COMPOSITE_TIMEREC "./composite_timer.txt"

// 結構與類別
struct node { int data; };

class entry {
public:
    long key;
    entry(long k, node* d) { key = k; data = d; }
    void outputkey(FILE* file) { fprintf(file, " key: %ld\n", key); }
private:
    node* data;
};

struct result {
    int64_t timer;
    vector<entry> arr2;
};

// 記憶體分析
string printMem(int state) {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    unsigned long long memUsage = memInfo.WorkingSetSize / 1024;
    unsigned long long memPeak = memInfo.PeakWorkingSetSize / 1024;
    if (state == 0) return "Memory usage: " + to_string(memUsage) + " KB\n";
    else return "Peak memory usage: " + to_string(memPeak) + " KB\n";
}

// 插入排序
result InsertionSort(vector<entry> arr, int casenum) {
    result r;
    FILE* file = fopen(TIMEREC, "a");
    auto timer = high_resolution_clock::now();
    string memInit = printMem(0);
    fprintf(file, "Start Insertion case %d\n[Init] %s", casenum, memInit.c_str());

    for (int i = 1; i < arr.size(); i++) {
        entry temp = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].key > temp.key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }

    auto stop = high_resolution_clock::now();
    auto dur = duration_cast<microseconds>(stop - timer);
    r.arr2 = arr;
    r.timer = dur.count();
    string memFin = printMem(1);
    fprintf(file, "Finished in %lld us\n[Final] %s\n", r.timer, memFin.c_str());
    fclose(file);
    return r;
}

// 快速排序
void QuickSortCore(vector<entry>& arr, int left, int right) {
    if (left < right) {
        entry pivot = arr[left];
        int i = left, j = right + 1;
        do {
            do i++; while (i <= right && arr[i].key < pivot.key);
            do j--; while (arr[j].key > pivot.key);
            if (i < j) swap(arr[i], arr[j]);
        } while (i < j);
        swap(arr[left], arr[j]);
        QuickSortCore(arr, left, j - 1);
        QuickSortCore(arr, j + 1, right);
    }
}

result QuickSort(vector<entry> arr, int casenum) {
    result r;
    FILE* file = fopen(TIMEREC, "a");
    auto timer = high_resolution_clock::now();
    string memInit = printMem(0);
    fprintf(file, "Start Quick case %d\n[Init] %s", casenum, memInit.c_str());

    QuickSortCore(arr, 0, arr.size() - 1);

    auto stop = high_resolution_clock::now();
    auto dur = duration_cast<microseconds>(stop - timer);
    r.arr2 = arr;
    r.timer = dur.count();
    string memFin = printMem(1);
    fprintf(file, "Finished in %lld us\n[Final] %s\n", r.timer, memFin.c_str());
    fclose(file);
    return r;
}

// 合併排序
vector<entry> MergeCore(vector<entry> a, vector<entry> b) {
    vector<entry> c;
    auto it_a = a.begin(), it_b = b.begin();
    while (it_a != a.end() && it_b != b.end()) {
        if (it_a->key < it_b->key) c.push_back(*it_a++);
        else c.push_back(*it_b++);
    }
    while (it_a != a.end()) c.push_back(*it_a++);
    while (it_b != b.end()) c.push_back(*it_b++);
    return c;
}

vector<entry> MergeCut(vector<entry> arr) {
    if (arr.size() <= 1) return arr;
    int mid = arr.size() / 2;
    vector<entry> left(arr.begin(), arr.begin() + mid);
    vector<entry> right(arr.begin() + mid, arr.end());
    return MergeCore(MergeCut(left), MergeCut(right));
}

result MergeSort(vector<entry> arr, int casenum) {
    result r;
    FILE* file = fopen(TIMEREC, "a");
    auto timer = high_resolution_clock::now();
    string memInit = printMem(0);
    fprintf(file, "Start Merge case %d\n[Init] %s", casenum, memInit.c_str());

    arr = MergeCut(arr);

    auto stop = high_resolution_clock::now();
    auto dur = duration_cast<microseconds>(stop - timer);
    r.arr2 = arr;
    r.timer = dur.count();
    string memFin = printMem(1);
    fprintf(file, "Finished in %lld us\n[Final] %s\n", r.timer, memFin.c_str());
    fclose(file);
    return r;
}

// 堆排序
void heapify(vector<entry>& arr, int n, int i) {
    int largest = i, l = 2 * i + 1, r = 2 * i + 2;
    if (l < n && arr[l].key > arr[largest].key) largest = l;
    if (r < n && arr[r].key > arr[largest].key) largest = r;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

result HeapSort(vector<entry> arr, int casenum) {
    result r;
    FILE* file = fopen(TIMEREC, "a");
    auto timer = high_resolution_clock::now();
    string memInit = printMem(0);
    fprintf(file, "Start Heap case %d\n[Init] %s", casenum, memInit.c_str());

    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }

    auto stop = high_resolution_clock::now();
    auto dur = duration_cast<microseconds>(stop - timer);
    r.arr2 = arr;
    r.timer = dur.count();
    string memFin = printMem(1);
    fprintf(file, "Finished in %lld us\n[Final] %s\n", r.timer, memFin.c_str());
    fclose(file);
    return r;
}

// Composite Sort
result CompositeSort(vector<entry> arr, int casenum) {
    FILE* file = fopen(COMPOSITE_TIMEREC, "a");
    auto timer = high_resolution_clock::now();
    string memInit = printMem(0);
    fprintf(file, "Start Composite case %d\n[Init] %s", casenum, memInit.c_str());

    result r;
    if (arr.size() <= 32) r = InsertionSort(arr, casenum);
    else if (arr.size() <= 1000) r = MergeSort(arr, casenum);
    else if (arr.size() <= 5000) r = HeapSort(arr, casenum);
    else r = QuickSort(arr, casenum);

    auto stop = high_resolution_clock::now();
    auto dur = duration_cast<microseconds>(stop - timer);
    string memFin = printMem(1);
    fprintf(file, "Composite finished in %lld us\n[Final] %s\n", dur.count(), memFin.c_str());
    fclose(file);
    return r;
}

// 測資產生器
void makeCases(int cases, vector<vector<entry>>& superarray, FILE* unsortedfile, string mode) {
    for (int c = 0; c < cases; c++) {
        vector<entry> array;
        for (int i = 0; i < CASE_ITEMS; i++) {
            node* n = new node;
            long key;
            if (mode == "INSERTION" || mode == "QUICK" || mode == "MERGE")
                key = CASE_ITEMS - i;
            else if (mode == "RANDOM")
                key = rand() % CASE_ITEMS;
            else
                key = i;
            array.emplace_back(key, n);
        }
        if (mode == "HEAP") {
            for (int i = CASE_ITEMS - 1; i >= 2; i--) {
                int j = rand() % i + 1;
                swap(array[i], array[j]);
            }
        }
        for (auto& e : array) e.outputkey(unsortedfile);
        superarray.push_back(array);
    }
}

// 主程式
int main() {
    remove(SORTED);
    remove(UNSORTED);
    remove(TIMEREC);
    remove(COMPOSITE_TIMEREC);

    FILE* f_Unsorted = fopen(UNSORTED, "a");
    FILE* f_Sorted = fopen(SORTED, "a");

    srand(time(0));
    vector<vector<entry>> superarray[5];
    result result;

    makeCases(CASES, superarray[0], f_Unsorted, "INSERTION");
    makeCases(CASES, superarray[1], f_Unsorted, "QUICK");
    makeCases(CASES, superarray[2], f_Unsorted, "MERGE");
    makeCases(CASES, superarray[3], f_Unsorted, "HEAP");
    makeCases(CASES, superarray[4], f_Unsorted, "RANDOM");

    for (int type = 0; type < 4; type++) {
        for (int i = 0, caseNum = 1; i < superarray[type].size(); i++, caseNum++) {
            switch (type) {
            case 0: result = InsertionSort(superarray[type][i], caseNum); break;
            case 1: result = QuickSort(superarray[type][i], caseNum); break;
            case 2: result = MergeSort(superarray[type][i], caseNum); break;
            case 3: result = HeapSort(superarray[type][i], caseNum); break;
            }
            fprintf(f_Sorted, "\nCase %d finished in %lld us\n", caseNum, result.timer);
            for (auto& e : result.arr2) e.outputkey(f_Sorted);
        }
    }

    // 跑 Composite Sort
    for (int i = 0, caseNum = 1; i < superarray[4].size(); i++, caseNum++) {
        result = CompositeSort(superarray[4][i], caseNum);
    }

    fclose(f_Unsorted);
    fclose(f_Sorted);
    return 0;
}
