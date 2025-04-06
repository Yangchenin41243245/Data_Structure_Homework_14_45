# 排序演算法效能分析報告

## 目錄

### 一、時鐘精度
### 二、Heap Sort 最壞情況測試資料的隨機排列方式
### 三、Merge Sort 最壞情況測試資料生成方式
### 四、所有測試資料的時間表（最壞情況 + 平均情況）
### 五、程式解析

---

## 一、時鐘精度

本程式使用 C++ 的 `<chrono>` 標準函式庫中的 `high_resolution_clock` 進行計時，使用 `duration_cast<microseconds>` 將執行時間換算為「微秒」(1 微秒 = 0.000001 秒)。提供足夠高的解析度，以正確測量常見排序演算法在中等規模資料量（如 5000 筆資料）下的執行效能。

---

## 二、Heap Sort 最壞情況測試資料的隨機排列方式

為了模擬 Heap Sort 的最壞情況，本程式在 `makeCases()` 函數中對 HEAP 模式使用了 **隨機排列【Random Permutation】** 方式生成輸入資料：

```cpp
for (int i = CASE_ITEMS - 1; i >= 2; i--)
{
    int j = rand() % i + 1;
    swap(array[i], array[j]);
}
```

這段程式碼等像於 **Fisher-Yates shuffle** 隨機打亂法，使輸入數列在統計上接近 Heap Sort 的最壞或平均情況。

---

## 三、Merge Sort 最壞情況測試資料生成方式

Merge Sort 的最壞情況並不會明顯受特定輸入順序影響，因此程式中使用了 **遞減排序【由大至小】** 的資料生成方式作為最壞情況的模擬輸入：

```cpp
if (mode == "INSERTION" || mode == "QUICK" || mode == "MERGE")
    key = CASE_ITEMS - i;
```

這樣生成的輸入數列為遞減序列，例如 [5000, 4999, 4998, ..., 1]，對 Merge Sort 難以顯著抽慮效能，但可視為統一的 worst-case 測試資料來源。

---

## 四、所有測試資料的時間表（最壞情況 + 平均情況）

### 平均執行時間表格

| 資料數量 (n) | 插入排序 (Insertion) | 快速排序 (Quick) | 合併排序 (Merge) | 堆積排序 (Heap) |
|--------------|-----------------------|-------------------|------------------|-----------------|
| 500          | 419.2                 | 151.5             | 1921.1           | 73.8            |
| 1000         | 1688.7                | 609.7             | 4306.6           | 140.9           |
| 2000         | 6238.2                | 2264.2            | 11828.0          | 336.8           |
| 3000         | 14819.7               | 5525.9            | 17096.2          | 562.4           |
| 4000         | 23682.4               | 8861.6            | 24187.3          | 739.1           |
| 5000         | 38788.5               | 14426.6           | 30609.3          | 946.4           |

![image](https://github.com/Yangchenin41243245/Data_Structure_Homework_14_45/blob/main/pics/pic01.png)

### 最壞執行時間表格

| 資料數量 (n) | 插入排序 (Insertion) | 快速排序 (Quick) | 合併排序 (Merge) | 堆積排序 (Heap) |
|--------|-----------------|-----------------|-----------------|-----------------|
| 500    | 623             | 155             | 2643            | 101             |
| 1000   | 2478            | 792             | 5206            | 165             |
| 2000   | 7742            | 2738            | 14552           | 418             |
| 3000   | 17826           | 5405            | 19880           | 754             |
| 4000   | 26746           | 9366            | 31271           | 790             |
| 5000   | 47480           | 18947           | 30609           | 946             |

![image](https://github.com/Yangchenin41243245/Data_Structure_Homework_14_45/blob/main/pics/pic02.png)

### 時間複雜度趨勢分析

- 插入排序：明顯呈現 $O(n^2)$ 成長，效能下降快速。
- 快速排序：雖然最壞情況為 $O(n^2)$，但在平均情況下趨近 $O(n \log n)$，表現穩定。
- 合併排序：始終維持 $O(n \log n)$，在大型資料下表現穩定但稍高。
- 堆積排序：執行時間隨 $n$ 緩慢上升，符合 $O(n \log n)$，在測試中表現最佳。

## 五、程式解析

### 1. 頭文件與定義

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <chrono>
using namespace std;
using namespace std::chrono;

#define CASE_ITEMS 5000 // number of items in each case
#define CASES 10      // number of cases
#define RNGKEYS rand() % CASE_ITEMS
#define INSKEYS CASE_ITEMS - i // worst case for insertion sort
#define UNSORTED "./tosort.txt"
#define SORTED "./sorted.txt"
#define TIMEREC "./timer.txt"
```

#### 解釋
- **功能**：引入必要的 C++ 標準庫，包括輸入輸出（`<iostream>`）、動態陣列（`<vector>`）、計時（`<chrono>`）等，並定義常數。
- **常數定義**：
  - `CASE_ITEMS`：每個測試案例的資料量（5000）。
  - `CASES`：測試案例數（10）。
  - `RNGKEYS` 和 `INSKEYS`：用於生成隨機鍵值和插入排序的最壞情況鍵值。
  - 文件路徑（`UNSORTED`, `SORTED`, `TIMEREC`）：用於儲存未排序、已排序和計時結果。
---

### 2. 資料結構定義

```cpp
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
```

#### 解釋
- **功能**：
  - `node`：簡單結構，包含一個整數資料欄位。
  - `entry`：主要資料結構，包含鍵值（`key`）和指向 `node` 的指標，用於排序和輸出。

---

### 3. 插入排序實現

```cpp
void InsertionCore(entry *temp, vector<entry>&arr, int i)
{
    entry a = *temp;
    int pos = i;
    while(i >= 0 && a.key < arr[i].key)
    {
        arr[i + 1] = arr[i];
        i--;
    }
    arr[i+1] = a;
}

unsigned long InsertionSort(vector<entry> &arr, int casenum)
{
    cout << "Start insertion sort\n";
    auto timer = high_resolution_clock::now();
    for (int j = 2; j < arr.size(); j++)
    {
        entry*temp = &arr[j];
        InsertionCore(temp, arr, j-1);
    }
    auto stop = high_resolution_clock::now();
    auto dur = duration_cast<microseconds>(stop - timer);
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
```

#### 解釋
- **功能**：
  - `InsertionCore`：核心插入邏輯，將元素插入到正確位置。
  - `InsertionSort`：完整的插入排序實現，記錄執行時間並輸出到文件。
- **計時**：使用 `<chrono>` 的高精度計時器，單位為微秒。

---

### 4. 快速排序實現

```cpp
void QuickSortCore(vector<entry>&arr, int left, int right)
{
    if (left < right)
    {
        entry *pivot = &arr[left];
        int i = left, j = right + 1;
        do
        {
            do i++; while (arr[i].key < pivot->key);
            do j--; while (arr[j].key > pivot->key);
            if (i < j) swap(arr[i], arr[j]);
        } while (i < j);
        swap(arr[left], arr[j]);
        QuickSortCore(arr, left, j - 1);
        QuickSortCore(arr, j + 1, right);
    }
}

unsigned long QuickSort(vector<entry> &arr, int casenum)
{
    cout << "Start quick sort\n";
    auto timer = high_resolution_clock::now();
    QuickSortCore(arr, 0, arr.size()-1);
    auto stop = high_resolution_clock::now();
    auto dur = duration_cast<microseconds>(stop - timer);
    cout << "Sorted array in " << dur.count() << " microseconds\n";
    FILE *file = fopen(TIMEREC, "a");
    fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Quick\n",
            casenum, CASE_ITEMS, dur.count());
    if(file!=nullptr) fclose(file);
    return dur.count();
}
```

#### 解釋
- **功能**：
  - `QuickSortCore`：遞迴實現快速排序，使用第一個元素作為樞軸（pivot）。
  - `QuickSort`：啟動排序並計時。
---

### 5. 合併排序實現

```cpp
vector<entry> MergeCore(vector<entry>& a, vector<entry>& b)
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
    while (it_a != a.end()) { c.push_back(std::move(*it_a)); ++it_a; }
    while (it_b != b.end()) { c.push_back(std::move(*it_b)); ++it_b; }
    return c;
}

vector<entry> MergeCut(vector<entry>arr)
{
    if(arr.size() <= 1) { return arr; }
    vector<entry> left, right;
    int mid = arr.size() / 2;
    for (int i = 0; i < mid; i++) left.push_back(arr[i]);
    for (int i = mid; i < arr.size(); i++) right.push_back(arr[i]);
    left = MergeCut(left);
    right = MergeCut(right);
    return MergeCore(left, right);
}

unsigned long MergeSort(vector<entry>&arr, int casenum)
{
    cout << "Start merge sort\n";
    auto timer = high_resolution_clock::now();
    arr = MergeCut(arr);
    auto stop = high_resolution_clock::now();
    auto dur = duration_cast<microseconds>(stop - timer);
    cout << "Sorted array in " << dur.count() << " microseconds\n";
    FILE *file = fopen(TIMEREC, "a");
    fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Merge\n",
            casenum, CASE_ITEMS, dur.count());
    if(file!=nullptr) fclose(file);
    return dur.count();
}
```

#### 解釋
- **功能**：
  - `MergeCore`：合併兩個已排序子陣列。
  - `MergeCut`：遞迴分割陣列。
  - `MergeSort`：啟動排序並計時。

---

## 6. 堆積排序實現

```cpp
void heapify(vector<entry>& arr, int n, int i)
{
    int max = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && arr[left].key > arr[max].key) max = left;
    if (right < n && arr[right].key > arr[max].key) max = right;
    if (max != i)
    {
        swap(arr[i], arr[max]);
        heapify(arr, n, max);
    }
}

unsigned long HeapSort(vector<entry>&arr, int casenum)
{
    cout << "Start heap sort\n";
    auto timer = high_resolution_clock::now();
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--)
    {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
    auto stop = high_resolution_clock::now();
    auto dur = duration_cast<microseconds>(stop - timer);
    cout << "Sorted array in " << dur.count() << " microseconds\n";
    FILE *file = fopen(TIMEREC, "a");
    fprintf(file, "Sorted case #%d with %lu items in %ld microseconds with Heap\n",
            casenum, CASE_ITEMS, dur.count());
    if(file!=nullptr) fclose(file);
    return dur.count();
}
```

#### 解釋
- **功能**：
  - `heapify`：維護最大堆性質。
  - `HeapSort`：建構最大堆並逐步排序。

---

### 7. 主程式與測試資料生成

```cpp
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
                key = INSKEYS; // worst case = max -> min
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
            array[i].outputkey(unsortedfile);
        superarray.push_back(array);
    }
}

int main(void)
{
    remove(SORTED); remove(UNSORTED); remove(TIMEREC);
    FILE *f_Unsorted = fopen(UNSORTED, "a");
    FILE *f_Sorted = fopen(SORTED, "a");
    srand(time(0));
    vector<vector<entry>> superarray[4];
    makeCases(CASES, superarray[0], f_Unsorted, "INSERTION");
    makeCases(CASES, superarray[1], f_Unsorted, "QUICK");
    makeCases(CASES, superarray[2], f_Unsorted, "MERGE");
    makeCases(CASES, superarray[3], f_Unsorted, "HEAP");
    for (int m = 0; m < 4; m++)
    {
        for (int i = 0, c = 1; i < superarray[m].size(); i++, c++)
        {
            auto duration = 0;
            switch (m)
            {
            case 0: duration = InsertionSort(superarray[m][i], c); break;
            case 1: duration = QuickSort(superarray[m][i], c); break;
            case 2: duration = MergeSort(superarray[m][i], c); break;
            case 3: duration = HeapSort(superarray[m][i], c); break;
            }
            fprintf(f_Sorted, "\nCase %d of %lu items finished in %lu microseconds\n", c, CASE_ITEMS, duration);
            for (int j = 0; j < CASE_ITEMS; j++)
                superarray[m][i][j].outputkey(f_Sorted);
        }
    }
    fclose(f_Unsorted);
    fclose(f_Sorted);
    return 0;
}
```

#### 解釋
- **功能**：
  - `makeCases`：生成測試資料，根據模式選擇遞減序列（插入、快速、合併）或隨機排列（堆積）。
  - `main`：初始化文件、生成測試案例、執行排序並記錄結果。

---
