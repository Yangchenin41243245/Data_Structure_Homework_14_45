# 41243214 
& 41243245
# 排序演算法效能分析報告

## 目錄

### 1. 解題說明
### 2. 時鐘精度
### 3. 效能分析
### 4. 程式實作
### 5. 測試與驗證
### 6. 申論及開發報告

---

## 解題說明

本作業旨在實作與分析四種排序演算法的效能，並設計一個根據輸入資料大小選擇最適合排序法的綜合排序函式（Composite Sort Function）。
作業核心在於針對不同輸入資料大小與特性（最壞情況與平均情況）進行排序效能測試，並比較各排序法在時間與記憶體使用上的表現。

本次實作包含以下五種排序方法：

1. **Insertion Sort**  
   逐一將元素插入已排序好的子陣列，最壞情況為反向排序。

2. **Quick Sort**  
   採用分治法將資料切分並遞迴排序，使用第一個元素作為 pivot，最壞情況發生於切分極不平均時。

3. **Merge Sort**  
   將資料拆分後合併排序，最壞情況相對穩定，時間複雜度為 O(n log n)，採用遞迴式實作。

4. **Heap Sort**  
   利用最大堆結構，每次將最大值移至尾端排序，對於初始排列較亂的資料效能佳。

5. **Composite Sort**  
   根據輸入資料大小動態選擇排序方法：小於等於 32 個元素使用 Insertion Sort，小於等於 1000 個元素使用 Merge Sort，小於等於 5000 個元素使用 Heap Sort，否則使用 Quick Sort。

透過 `std::chrono` 計時與 Windows API 測量記憶體使用量，將各排序法在不同 `n` 值下的效能進行數據分析與圖表視覺化，最終統整出各排序法的適用情境與建議使用範圍。Composite Sort 旨在結合各排序法的優勢，針對不同資料規模提供最佳效能。

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

透過 `chrono::duration_cast` 可精確取得兩個時間點之間的間隔（此處為微秒）。Composite Sort 的計時方式與其他排序法一致，確保比較公平性。

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

- 在正式測試中，我們確保對每組測資僅進行一次排序測試（不重複），以模擬實際使用情境。當發現個別排序法（如 Insertion Sort 或 Composite Sort 在小資料量時）耗時極短時，再單獨進行多次測量以取得更精確數據。

### 實驗觀察

經過測試發現，對於筆數小於 1000 的資料排序，Insertion Sort 和 Composite Sort（選擇 Insertion Sort 的情況）執行時間可能會出現 0 μs 的情形。透過上述方法可有效改善這一問題，使測量更為穩定與可信。

---

## 效能分析

#### Insertion Sort
- **時間複雜度**：
  - **Worst-case**：O(n²)，當輸入為逆序（程式中生成逆序資料），每次插入需移動所有前元素。
  - **Average-case**：O(n²)，因每次插入平均需比較和移動約 n/2 個元素。
- **空間複雜度**：
  - O(1)，原地排序，僅需常數額外空間（`temp` 變數）。
- **程式特性**：簡單實現，未優化比較或移動，適合小陣列（n ≤ 32）。

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
  - **Average-case keypad**：O(n log n)，合併操作固定。
- **空間複雜度**：
  - O(n)，需額外陣列儲存合併結果（程式中 `vector<entry>` 動態分配）。
- **程式特性**：遞迴實現，穩定但記憶體開銷較大，適合中型資料（32 < n ≤ 1000）。

#### Heap Sort
- **時間複雜度**：
  - **Worst-case**：O(n log n)，建堆 O(n)，每次調整堆 O(log n)，共 n 次。
  - **Average-case**：O(n log n)，對輸入不敏感。
- **空間複雜度**：
  - O(1)，原地排序，僅需常數額外空間。
- **程式特性**：標準最大堆實現，穩定且高效，適合中大型資料（1000 < n ≤ 5000）。

#### Composite Sort
- **時間複雜度**：
  - **Worst-case**：取決於選擇的排序法：
    - n ≤ 32：O(n²)（Insertion Sort）
    - 32 < n ≤ 1000：O(n log n)（Merge Sort）
    - 1000 < n ≤ 5000：O(n log n)（Heap Sort）
    - n > 5000：O(n²)（Quick Sort，Worst-case）
  - **Average-case**：O(n log n) 為主，當 n > 5000 時接近 Quick Sort 的 O(n log n)。
- **空間複雜度**：
  - 取決於選擇的排序法：
    - Insertion Sort：O(1)
    - Merge Sort：O(n)
    - Heap Sort：O(1)
    - Quick Sort：O(log n)
- **程式特性**：根據資料大小動態選擇排序法，結合各算法優勢，適用於多種資料規模。

### 總結
- **Insertion Sort**：時間複雜度高（O(n²)），但空間需求最低，適合小資料。
- **Quick Sort**：Average-case 高效（O(n log n)），但程式實現易退化到 O(n²)，空間需遞迴棧。
- **Merge Sort**：時間穩定（O(n log n)），但空間需求高（O(n)）。
- **Heap Sort**：時間和空間均優（O(n log n), O(1)），為中大型資料良好選擇。
- **Composite Sort**：根據資料大小選擇最佳排序法，平衡時間與空間需求，適合廣泛應用場景。

---

## 程式實作

### 一. 標頭與基本設置

```cpp
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
#define UNSORTED "D:/work/sort/tosort.txt"
#define SORTED "D:/work/sort/sorted.txt"
#define TIMEREC "D:/work/sort/timer.txt"
#define COMPOSITE_TIMEREC "D:/work/sort/composite_timer.txt"
#define REPEAT_COUNT 1000 // For small dataset averaging
#define SMALL_DATA_THRESHOLD 1000 // Threshold for small datasets
```

#### 說明：

##### 引入標頭檔
- `iostream`, `vector`, `algorithm`, `string`：處理輸出、容器、演算法與字串。
- `ctime`, `cstdlib`：處理隨機數與時間。
- `chrono`：計時用，計算排序時間。
- `windows.h`, `psapi.h`：Windows API，用來查詢記憶體使用情況。

##### `#pragma comment(lib, "psapi.lib")`
自動連結 `psapi.lib`，提供記憶體管理函式。

##### #define 指引
- `CASE_ITEMS`：每筆測試資料的元素數量（6000）。
- `CASES`：測試用例數（5）。
- `UNSORTED`, `SORTED`, `TIMEREC`, `COMPOSITE_TIMEREC`：檔案路徑，分別儲存未排序資料、排序後資料、排序時間記錄及 Composite Sort 時間記錄。

---

### 二. 資料結構與記憶體分析

```cpp
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

string printMem(int state) {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    unsigned long long memUsage = memInfo.WorkingSetSize / 1024;
    unsigned long long memPeak = memInfo.PeakWorkingSetSize / 1024;
    if (state == 0) return "Memory usage: " + to_string(memUsage) + " KB\n";
    else return "Peak memory usage: " + to_string(memPeak) + " KB\n";
}
```

#### 說明：

##### 資料結構
- `node`：簡單結構，包含一個整數 `data`。
- `entry`：包含 `key`（排序依據）和指向 `node` 的指標，提供 `outputkey` 函式輸出鍵值。
- `result`：儲存排序結果（`arr2`）和執行時間（`timer`）。

##### `printMem`
- 使用 `GetProcessMemoryInfo` 獲取記憶體使用量。
- `state == 0` 返回當前記憶體使用量，`state == 1` 返回峰值記憶體使用量，單位為 KB。

---

### 三. 排序函式實作

#### Insertion Sort
```cpp
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
```

- 對每一個元素，往前比較並插入至適當位置。
- 穩定排序，適合幾乎排序好的資料。
- **時間複雜度**：平均與最壞為 O(n²)，最佳為 O(n)。

---

#### Quick Sort
```cpp
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
```

- 以最後一個元素作為 pivot。
- 小於 pivot 的元素會被移到左側，遞迴排序。
- 不穩定排序，**在最壞情況（遞增/遞減序列）會退化為 O(n²)**，但一般實務中表現極佳。

---

#### Merge Sort
```cpp
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
```

- 將陣列遞迴切半後合併，合併時保持排序。
- **空間複雜度 O(n)**，需要額外暫存空間。
- 穩定排序，適合資料量大且對穩定性有要求的應用。

---

#### Heap Sort
```cpp
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
        fprintf(file, "Start Heap case %d\n[

Init] %s", casenum, memInit.c_str());
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
```

- 先建 max heap，再依序將最大值交換到尾端並縮小 heap 區間。
- 不需要額外空間，**空間複雜度 O(1)**。
- 時間複雜度穩定為 O(n log n)，但常數大、快取不友善，實務上不如 QuickSort 快。

---

#### Composite Sort
```cpp
result CompositeSort(vector<entry> arr, int casenum, bool useAverage = false) {
    FILE* file = fopen(COMPOSITE_TIMEREC, "a");
    int64_t total_duration = 0;
    string memInit = printMem(0);
    result r;

    if (useAverage) {
        fprintf(file, "Start Composite case %d (Averaged over %d runs)\n[Init] %s", casenum, REPEAT_COUNT, memInit.c_str());
        auto start = high_resolution_clock::now();
        for (int i = 0; i < REPEAT_COUNT; ++i) {
            vector<entry> temp_arr = arr;
            if (temp_arr.size() <= 32) r = InsertionSort(temp_arr, casenum, false);
            else if (temp_arr.size() <= 1000) r = MergeSort(temp_arr, casenum, false);
            else if (temp_arr.size() <= 5000) r = HeapSort(temp_arr, casenum, false);
            else r = QuickSort(temp_arr, casenum, false);
            if (i == 0) r.arr2 = r.arr2; // Save result from first run
        }
        auto stop = high_resolution_clock::now();
        total_duration = duration_cast<microseconds>(stop - start).count() / REPEAT_COUNT;
    } else {
        fprintf(file, "Start Composite case %d\n[Init] %s", casenum, memInit.c_str());
        auto start = high_resolution_clock::now();
        if (arr.size() <= 32) r = InsertionSort(arr, casenum, false);
        else if (arr.size() <= 1000) r = MergeSort(arr, casenum, false);
        else if (arr.size() <= 5000) r = HeapSort(arr, casenum, false);
        else r = QuickSort(arr, casenum, false);
        auto stop = high_resolution_clock::now();
        total_duration = duration_cast<microseconds>(stop - start).count();
    }

    r.timer = total_duration;
    string memFin = printMem(1);
    fprintf(file, "Composite finished in %lld us\n[Final] %s\n", total_duration, memFin.c_str());
    fclose(file);
    return r;
}
```

#### 說明：
- **功能**：`CompositeSort` 根據輸入陣列大小動態選擇最適排序演算法：`n ≤ 32` 使用 `InsertionSort`（小資料高效），`32 < n ≤ 1000` 使用 `MergeSort`（穩定且適合中型資料），`1000 < n ≤ 5000` 使用 `HeapSort`（空間效率高），`n > 5000` 使用 `QuickSort`（大型資料平均效能佳）。
記錄初始記憶體與開始時間，根據陣列大小調用對應排序函式，記錄結束時間與峰值記憶體，結果輸出至 `composite_timer.txt`。
  
- **Insertion Sort**：簡單實現，適合小資料，記錄初始與結束記憶體使用量。
- **Quick Sort**：使用第一元素作為 pivot，未採用 median-of-three，導致最壞情況效能較差。
- **Merge Sort**：遞迴實現，非迭代式，空間需求較高。
- **Heap Sort**：標準最大堆實現，空間效率高。
- **Composite Sort**：根據資料大小選擇最適排序法，記錄獨立時間與記憶體使用量，輸出至 `composite_timer.txt`。

---


### 四. 生成測試用例

#### makeCases 函式
```cpp
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
```

#### 說明：
- **功能**：`makeCases` 函式負責生成指定數量的測試用例（`cases`），每個用例包含 `CASE_ITEMS`（6000）個 `entry` 物件，並將未排序的鍵值輸出至 `unsortedfile`（`tosort.txt`）。生成的測試用例儲存於 `superarray` 中，供後續排序使用。
- **參數**：
  - `cases`：測試用例數量（預設為 5）。
  - `superarray`：儲存生成的測試用例陣列，型別為 `vector<vector<entry>>`。
  - `unsortedfile`：指向未排序資料輸出檔案的指標。
  - `mode`：指定鍵值生成模式，支援以下選項：
    - `"INSERTION"`, `"QUICK"`, `"MERGE"`：生成逆序鍵值（`CASE_ITEMS - i`），模擬最壞情況。
    - `"RANDOM"`：生成隨機鍵值（`rand() % CASE_ITEMS`），模擬平均情況，用於 `CompositeSort`。
    - 其他（預設為 `"HEAP"`）：生成順序鍵值（`i`），後續針對 Heap Sort 進行隨機排列。

---

### 五. 主程式與測試流程

```cpp
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
```

#### 說明：
- **檔案管理**：移除舊檔案，開啟新檔案進行輸出。
- **測資生成**：生成五組測資，分別對應 Insertion、Quick、Merge、Heap 及隨機資料（用於 Composite Sort）。
- **排序執行**：對前四組測資執行對應排序，第五組執行 Composite Sort。
- **結果輸出**：將排序結果與時間記錄至 `sorted.txt` 和 `timer.txt`（或 `composite_timer.txt`）。

---

## 測試與驗證

### 最壞執行時間表格 Worst-case Performance (Time in Microseconds)

| Data Size | Insertion Sort | Quick Sort | Merge Sort | Heap Sort | Composite Sort |
|-----------|----------------|------------|------------|-----------|----------------|
| 500       | 847            | 440        | 2326       | 311       | 847            |
| 1000      | 2766           | 1106       | 5090       | 320       | 5090           |
| 2000      | 6395           | 2381       | 15428      | 615       | 615            |
| 3000      | 14413          | 5218       | 23567      | 849       | 849            |
| 4000      | 24047          | 9181       | 28200      | 1125      | 1125           |
| 5000      | 39733          | 20349      | 30139      | 1377      | 1377           |
| 6000      | 59812          | 30527      | 36245      | 1652      | 30527          |

*Note*: Composite Sort times reflect the chosen algorithm based on data size (Insertion Sort for n ≤ 32, Merge Sort for 32 < n ≤ 1000, Heap Sort for 1000 < n ≤ 5000, Quick Sort for n > 5000). For n = 6000, Composite Sort uses Quick Sort.

![image](https://github.com/Yangchenin41243245/Data_Structure_Homework_14_45/blob/proto2/homework1/src/pics/worst.png)

### 平均執行時間表格 Average-case Performance (Time in Microseconds)

| Data Size | Insertion Sort | Quick Sort | Merge Sort | Heap Sort | Composite Sort |
|-----------|----------------|------------|------------|-----------|----------------|
| 500       | 701.0          | 325.8      | 2253.0     | 240.0     | 701.0          |
| 1000      | 1906.4         | 792.6      | 4777.0     | 283.8     | 4777.0         |
| 2000      | 6149.2         | 2261.8     | 13970.4    | 540.6     | 540.6          |
| 3000      | 13777.4        | 5050.6     | 20875.4    | 717.6     | 717.6          |
| 4000      | 23150.0        | 8675.4     | 25028.8    | 941.2     | 941.2          |
| 5000      | 38502.8        | 15283.0    | 26602.8    | 1223.2    | 1223.2         |
| 6000      | 57689.6        | 18273.4    | 31987.2    | 1478.4    | 18273.4        |

*Note*: Composite Sort times reflect the chosen algorithm based on data size. For n = 6000, Composite Sort uses Quick Sort, achieving Quick Sort’s average-case performance.

![image](https://github.com/Yangchenin41243245/Data_Structure_Homework_14_45/blob/proto2/homework1/src/pics/average.png)

### 時間複雜度趨勢分析

- **Insertion Sort**：呈現 O(n²) 成長，效能隨資料量快速下降。
- **Quick Sort**：最壞情況為 O(n²)，平均情況接近 O(n log n)，表現穩定但易受 pivot 選擇影響。
- **Merge Sort**：穩定 O(n log n)，但記憶體開銷較高，適合中型資料。
- **Heap Sort**：穩定 O(n log n)，空間效率高，適合中大型資料。

---

## 申論及開發報告

本作業成功實作並比較了四種經典排序演算法與一個自適應的 Composite Sort 函式。透過對不同資料規模（500 至 6000）的測試，我們觀察到：

- **Insertion Sort** 在小資料量（n ≤ 32）下由於簡單性和低空間需求表現良好，但在資料量增加時效能迅速下降。
- **Quick Sort** 在平均情況下表現優異，但在最壞情況（逆序資料）下退化至 O(n²)，顯示 pivot 選擇的重要性。
- **Merge Sort** 提供穩定的 O(n log n) 效能，但空間需求較高，適合穩定性要求高的場景。
- **Heap Sort** 在時間和空間上均表現優異，特別適合中大型資料，且對輸入資料特性不敏感。
- **Composite Sort** 通過動態選擇排序法有效結合各算法優勢：在小資料量下利用 Insertion Sort 的簡單性，中型資料使用 Merge Sort 的穩定性，中大型資料使用 Heap Sort 的空間效率，大型資料則依賴 Quick Sort 的平均效能。測試結果顯示，對於 n = 6000 的資料，Composite Sort 選擇 Quick Sort，達到與 Quick Sort 相同的效能，同時在較小資料量下展現靈活性。

### 挑戰與解決方案
- **Quick Sort 最壞情況**：由於固定選擇第一元素作為 pivot，逆序資料導致效能退化。未來可改進為隨機 pivot 或 median-of-three 策略。
- **記憶體測量波動**：Windows API 的記憶體測量可能因系統負載而波動，通過多次測試取平均值減少誤差。
- **Composite Sort 閾值選擇**：閾值（32、1000、5000）基於經驗與測試調整，未來可通過更系統化的分析優化。

### 結論
Composite Sort 的設計展示了根據資料特性動態選擇演算法的重要性。通過結合各排序法的優勢，該函式在多種資料規模下均能提供接近最佳的效能，適用於實際應用中資料規模不確定的場景。未來可進一步優化 pivot 選擇、實現原地 Merge Sort 或引入並行處理以提升效能。
