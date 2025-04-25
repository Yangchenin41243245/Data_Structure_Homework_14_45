# 41243214
& 41243245
# 排序演算法效能分析報告

## 目錄

### 解題說明
### 時鐘精度
### 效能分析
### 程式實作
### 測試與驗證
### 申論及開發報告

---

## 解題說明

本作業旨在實作與分析四種排序演算法的效能，並設計一個根據輸入資料特性選擇最適合排序法的綜合排序函式（Composite Sort Function）。作業核心在於針對不同輸入資料大小與特性（最壞情況與平均情況）進行排序效能測試，並比較各排序法在時間與記憶體使用上的表現。

本次實作包含以下四種排序方法：

1. **Insertion Sort**  
   逐一將元素插入已排序好的子陣列，最壞情況為反向排序。

2. **Quick Sort**（使用 median-of-three 中值選擇 pivot）  
   採用分治法將資料切分並遞迴排序，最壞情況發生於切分極不平均時。

3. **Merge Sort**（使用迭代式實作）  
   將資料拆分後合併排序，最壞情況相對穩定，時間複雜度為 O(n log n)。

4. **Heap Sort**  
   利用最大堆結構，每次將最大值移至尾端排序，對於初始排列較亂的資料效能佳。

透過 `std::chrono` 計時與 Windows API 測量記憶體使用量，將各排序法在不同 `n` 值下的效能進行數據分析與圖表視覺化，最終統整出各排序法的適用情境與建議使用範圍。

---

## 時鐘精度

本次作業中，排序演算法的效能主要透過時間消耗（microseconds）進行衡量。為了準確記錄各排序方法的執行時間，我們使用 C++ 標準函式庫中的 `<chrono>` 模組中的 `std::chrono::high_resolution_clock` 作為計時工具，其精度可達微秒等級，適合用於微小時間單位的效能分析。

### 使用方式

計時範例如下：

```cpp
auto start = chrono::high_resolution_clock::now();
// 排序函式執行
auto end = chrono::high_resolution_clock::now();
auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
```

透過 `chrono::duration_cast` 可精確取得兩個時間點之間的間隔（此處為微秒）。

### 測量誤差與處理方式

由於在小筆數資料（如 n = 500）時，排序執行時間可能低於計時器可穩定測量的範圍，導致計時結果為 0 或波動大。為解決此問題，採用以下策略：

- **重複執行多次後取平均時間**，以降低測量誤差。例如對某個排序函式執行 1000 次並除以總次數：
  
  ```cpp
  int repeat = 1000;
  auto start = chrono::high_resolution_clock::now();
  for (int i = 0; i < repeat; ++i)
      yourSortFunction(arr);
  auto end = chrono::high_resolution_clock::now();
  auto avg_duration = chrono::duration_cast<chrono::microseconds>(end - start).count() / repeat;
  ```

- 此外，在正式測試中，我們確保對每組測資僅進行一次排序測試（不重複），以模擬實際使用情境。當發現個別排序法耗時極短時，再單獨進行多次測量以取得更精確數據。

### 實驗觀察

經過測試發現，對於筆數小於 1000 的資料排序，Insertion Sort 執行時間可能會出現 0 μs 的情形。透過上述方法可有效改善這一問題，使測量更為穩定與可信。

---

## 效能分析

#### Insertion Sort
- **時間複雜度**：
  - **Worst-case**：O(n²)，當輸入為逆序（程式中 `INSKEYS` 生成逆序），每次插入需移動所有前元素。
  - **Average-case**：O(n²)，因每次插入平均需比較和移動約 n/2 個元素。
- **空間複雜度**：
  - O(1)，原地排序，僅需常數額外空間（`temp` 變數）。
- **程式特性**：簡單實現，未優化比較或移動，適合小陣列。

#### Quick Sort
- **時間複雜度**：
  - **Worst-case**：O(n²)，當 pivot 選擇不佳（程式中固定選第一元素）且輸入逆序，分割極不平衡。
  - **Average-case**：O(n log n)，隨機輸入下，分割接近均等。
- **空間複雜度**：
  - O(log n)，因遞迴調用棧（平均 O(log n) 層，Worst-case 可達 O(n)）。
- **程式特性**：固定 pivot（`arr[left]`）導致 Worst-case 頻發，未採用隨機 pivot 或小陣列插入排序優化。

#### Merge Sort
- **時間複雜度**：
  - **Worst-case**：O(n log n)，分割和合併均穩定，與輸入無關。
  - **Average-case**：O(n log n)，合併操作固定。
- **空間複雜度**：
  - O(n)，需額外陣列儲存合併結果（程式中 `c.reserve(a.size() + b.size())`）。
- **程式特性**：使用 `std::move` 減少複製開銷，但未實現原地合併或自底向上排序。

#### Heap Sort
- **時間複雜度**：
  - **Worst-case**：O(n log n)，建堆 O(n)，每次調整堆 O(log n)，共 n 次。
  - **Average-case**：O(n log n)，對輸入不敏感。
- **空間複雜度**：
  - O(1)，原地排序，僅需常數額外空間。
- **程式特性**：標準最大堆實現，穩定且高效，無顯著優化空間。

### 總結
- **Insertion Sort**：時間複雜度高（O(n²)），但空間需求最低，適合小資料。
- **Quick Sort**：Average-case 高效（O(n log n)），但程式實現易退化到 O(n²)，空間需遞迴棧。
- **Merge Sort**：時間穩定（O(n log n)），但空間需求高（O(n)）。
- **Heap Sort**：時間和空間均優（O(n log n), O(1)），為大規模資料最佳選擇。

---

## 程式實作

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

## 測試與驗證

### 最壞執行時間表格 Worst-case Performance (Time in Microseconds)

| Data Size | Insertion Sort | Quick Sort | Merge Sort | Heap Sort |
|-----------|----------------|------------|------------|-----------|
| 500       | 847            | 440        | 2326       | 311       |
| 1000      | 2766           | 1106       | 5090       | 320       |
| 2000      | 6395           | 2381       | 15428      | 615       |
| 3000      | 14413          | 5218       | 23567      | 849       |
| 4000      | 24047          | 9181       | 28200      | 1125      |
| 5000      | 39733          | 20349      | 30139      | 1377      |

![image](https://github.com/Yangchenin41243245/Data_Structure_Homework_14_45/blob/proto2/homework1/src/pics/worst.png)

### 平均執行時間表格 Average-case Performance (Time in Microseconds)

| Data Size | Insertion Sort | Quick Sort | Merge Sort | Heap Sort |
|-----------|----------------|------------|------------|-----------|
| 500       | 701.0          | 325.8      | 2253.0     | 240.0     |
| 1000      | 1906.4         | 792.6      | 4777.0     | 283.8     |
| 2000      | 6149.2         | 2261.8     | 13970.4    | 540.6     |
| 3000      | 13777.4        | 5050.6     | 20875.4    | 717.6     |
| 4000      | 23150.0        | 8675.4     | 25028.8    | 941.2     |
| 5000      | 38502.8        | 15283.0    | 26602.8    | 1223.2    |

![image](https://github.com/Yangchenin41243245/Data_Structure_Homework_14_45/blob/proto2/homework1/src/pics/average.png)

### 時間複雜度趨勢分析

- 插入排序：明顯呈現 $O(n^2)$ 成長，效能下降快速。
- 快速排序：雖然最壞情況為 $O(n^2)$，但在平均情況下趨近 $O(n \log n)$，表現穩定。
- 合併排序：始終維持 $O(n \log n)$，在大型資料下表現穩定但稍高。
- 堆積排序：執行時間隨 $n$ 緩慢上升，符合 $O(n \log n)$，在測試中表現最佳。

---

## 申論及開發報告
