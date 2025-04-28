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
#define CASE_ITEMS 5000
#define CASES 5
#define UNSORTED "D:/work/sort/tosort.txt"
#define SORTED "D:/work/sort/sorted.txt"
#define TIMEREC "D:/work/sort/timer.txt"
#define COMPOSITE_TIMEREC "D:/work/sort/composite_timer.txt"
#define REPEAT_COUNT 100 // For small dataset averaging
#define SMALL_DATA_THRESHOLD 100 // Threshold for small datasets

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
result InsertionSort(vector<entry> arr, int casenum, bool useAverage = false) {
    result r;
    FILE* file = fopen(TIMEREC, "a");
    int64_t total_duration = 0;
    string memInit = printMem(0);

    if (useAverage) {
        fprintf(file, "Start Insertion case %d (Averaged over %d runs)\n[Init] %s", casenum, REPEAT_COUNT, memInit.c_str());
        auto start = high_resolution_clock::now();
        for (int i = 0; i < REPEAT_COUNT; ++i) {
            vector<entry> temp_arr = arr; // Copy to avoid modifying original
            for (int j = 1; j < temp_arr.size(); j++) {
                entry temp = temp_arr[j];
                int k = j - 1;
                while (k >= 0 && temp_arr[k].key > temp.key) {
                    temp_arr[k + 1] = temp_arr[k];
                    k--;
                }
                temp_arr[k + 1] = temp;
            }
            if (i == 0) r.arr2 = temp_arr; // Save result from first run
        }
        auto stop = high_resolution_clock::now();
        total_duration = duration_cast<microseconds>(stop - start).count() / REPEAT_COUNT;
    } else {
        fprintf(file, "Start Insertion case %d\n[Init] %s", casenum, memInit.c_str());
        auto start = high_resolution_clock::now();
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
        total_duration = duration_cast<microseconds>(stop - start).count();
        r.arr2 = arr;
    }

    r.timer = total_duration;
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

result QuickSort(vector<entry> arr, int casenum, bool useAverage = false) {
    result r;
    FILE* file = fopen(TIMEREC, "a");
    int64_t total_duration = 0;
    string memInit = printMem(0);

    if (useAverage) {
        fprintf(file, "Start Quick case %d (Averaged over %d runs)\n[Init] %s", casenum, REPEAT_COUNT, memInit.c_str());
        auto start = high_resolution_clock::now();
        for (int i = 0; i < REPEAT_COUNT; ++i) {
            vector<entry> temp_arr = arr;
            QuickSortCore(temp_arr, 0, temp_arr.size() - 1);
            if (i == 0) r.arr2 = temp_arr;
        }
        auto stop = high_resolution_clock::now();
        total_duration = duration_cast<microseconds>(stop - start).count() / REPEAT_COUNT;
    } else {
        fprintf(file, "Start Quick case %d\n[Init] %s", casenum, memInit.c_str());
        auto start = high_resolution_clock::now();
        QuickSortCore(arr, 0, arr.size() - 1);
        auto stop = high_resolution_clock::now();
        total_duration = duration_cast<microseconds>(stop - start).count();
        r.arr2 = arr;
    }

    r.timer = total_duration;
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

result MergeSort(vector<entry> arr, int casenum, bool useAverage = false) {
    result r;
    FILE* file = fopen(TIMEREC, "a");
    int64_t total_duration = 0;
    string memInit = printMem(0);

    if (useAverage) {
        fprintf(file, "Start Merge case %d (Averaged over %d runs)\n[Init] %s", casenum, REPEAT_COUNT, memInit.c_str());
        auto start = high_resolution_clock::now();
        for (int i = 0; i < REPEAT_COUNT; ++i) {
            vector<entry> temp_arr = arr;
            temp_arr = MergeCut(temp_arr);
            if (i == 0) r.arr2 = temp_arr;
        }
        auto stop = high_resolution_clock::now();
        total_duration = duration_cast<microseconds>(stop - start).count() / REPEAT_COUNT;
    } else {
        fprintf(file, "Start Merge case %d\n[Init] %s", casenum, memInit.c_str());
        auto start = high_resolution_clock::now();
        arr = MergeCut(arr);
        auto stop = high_resolution_clock::now();
        total_duration = duration_cast<microseconds>(stop - start).count();
        r.arr2 = arr;
    }

    r.timer = total_duration;
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

result HeapSort(vector<entry> arr, int casenum, bool useAverage = false) {
    result r;
    FILE* file = fopen(TIMEREC, "a");
    int64_t total_duration = 0;
    string memInit = printMem(0);

    if (useAverage) {
        fprintf(file, "Start Heap case %d (Averaged over %d runs)\n[Init] %s", casenum, REPEAT_COUNT, memInit.c_str());
        auto start = high_resolution_clock::now();
        for (int i = 0; i < REPEAT_COUNT; ++i) {
            vector<entry> temp_arr = arr;
            int n = temp_arr.size();
            for (int j = n / 2 - 1; j >= 0; j--) heapify(temp_arr, n, j);
            for (int j = n - 1; j > 0; j--) {
                swap(temp_arr[0], temp_arr[j]);
                heapify(temp_arr, j, 0);
            }
            if (i == 0) r.arr2 = temp_arr;
        }
        auto stop = high_resolution_clock::now();
        total_duration = duration_cast<microseconds>(stop - start).count() / REPEAT_COUNT;
    } else {
        fprintf(file, "Start Heap case %d\n[Init] %s", casenum, memInit.c_str());
        auto start = high_resolution_clock::now();
        int n = arr.size();
        for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
        for (int i = n - 1; i > 0; i--) {
            swap(arr[0], arr[i]);
            heapify(arr, i, 0);
        }
        auto stop = high_resolution_clock::now();
        total_duration = duration_cast<microseconds>(stop - start).count();
        r.arr2 = arr;
    }

    r.timer = total_duration;
    string memFin = printMem(1);
    fprintf(file, "Finished in %lld us\n[Final] %s\n", r.timer, memFin.c_str());
    fclose(file);
    return r;
}

// Composite Sort
result CompositeSort(vector<entry> arr, int casenum, bool useAverage = false) {
    // 開啟記錄檔
    FILE* file = fopen(COMPOSITE_TIMEREC, "a");
    string memInit = printMem(0);
    fprintf(file,
            "Start Composite case %d%s\n[Init] %s",
            casenum,
            useAverage ? " (Averaged)" : "",
            memInit.c_str());

    result r;
    int64_t sum_timer = 0;

    // 單次執行 Composite 的封裝：根據大小選擇最適算法
    auto runOnce = [&](const vector<entry>& data) -> result {
        if (data.size() <= 32)         return InsertionSort(data, casenum, false);
        else if (data.size() <= 5000)  return HeapSort    (data, casenum, false);
        else                           return QuickSort   (data, casenum, false);
    };

    if (useAverage) {
        // 小資料多次跑，取平均
        for (int i = 0; i < REPEAT_COUNT; ++i) {
            result tmp = runOnce(arr);
            sum_timer += tmp.timer;
            if (i == 0) {
                // 只保存第一趟排序結果
                r.arr2 = std::move(tmp.arr2);
            }
        }
        r.timer = sum_timer / REPEAT_COUNT;
    } else {
        // 正常單次執行
        r = runOnce(arr);
    }

    // 輸出結束時間與記憶體
    string memFin = printMem(1);
    fprintf(file,
            "Composite finished in %lld us\n[Final] %s\n",
            r.timer,
            memFin.c_str());
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
            bool useAverage = (superarray[type][i].size() <= SMALL_DATA_THRESHOLD);
            switch (type) {
            case 0: result = InsertionSort(superarray[type][i], caseNum, useAverage); break;
            case 1: result = QuickSort(superarray[type][i], caseNum, useAverage); break;
            case 2: result = MergeSort(superarray[type][i], caseNum, useAverage); break;
            case 3: result = HeapSort(superarray[type][i], caseNum, useAverage); break;
            }
            fprintf(f_Sorted, "\nCase %d finished in %lld us\n", caseNum, result.timer);
            for (auto& e : result.arr2) e.outputkey(f_Sorted);
        }
    }

    // 跑 Composite Sort
    for (int i = 0, caseNum = 1; i < superarray[4].size(); i++, caseNum++) {
        bool useAverage = (superarray[4][i].size() <= SMALL_DATA_THRESHOLD);
        result = CompositeSort(superarray[4][i], caseNum, useAverage);
    }

    fclose(f_Unsorted);
    fclose(f_Sorted);
    return 0;
}
