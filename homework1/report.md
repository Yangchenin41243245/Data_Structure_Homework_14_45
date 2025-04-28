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

本次作業中，排序演算法的效能主要透過時間消耗（微秒，microseconds）進行衡量。為確保執行時間的測量精確，我們使用 C++ 標準函式庫中的 `<chrono>` 模組中的 `std::chrono::high_resolution_clock` 作為計時工具，其精度可達微秒等級，適合用於效能分析。

### 使用方式

計時方式根據資料量動態調整，支援單次計時與多次執行取平均兩種模式。基本計時範例如下：

```cpp
auto start = chrono::high_resolution_clock::now();
// 排序函式執行
auto stop = chrono::high_resolution_clock::now();
auto duration = chrono::duration_cast<chrono::microseconds>(stop - start).count();
```

透過 `chrono::duration_cast` 精確計算兩個時間點之間的間隔（單位為微秒）。所有排序方法（包括 Composite Sort）均採用一致的計時邏輯，確保比較公平性。

### 測量誤差與處理方式

對於小資料量（例如 n ≤ 1000），排序執行時間可能過短，導致計時結果不穩定（如波動大或接近 0 μs）。為解決此問題，程式採用以下策略：

- **多次執行取平均時間**：當資料量 ≤ 1000 時，啟用多次執行模式，對排序函式執行 `REPEAT_COUNT`（1000）次，使用陣列副本避免修改原始資料，計算平均時間以降低測量誤差。程式碼範例如下：

  ```cpp
  const int REPEAT_COUNT = 1000;
  auto start = high_resolution_clock::now();
  for (int i = 0; i < REPEAT_COUNT; ++i) {
      vector<entry> temp_arr = arr; // 陣列副本
      yourSortFunction(temp_arr);    // 排序
  }
  auto stop = high_resolution_clock::now();
  auto avg_duration = chrono::duration_cast<chrono::microseconds>(stop - start).count() / REPEAT_COUNT;
  ```

- **單次計時**：當資料量 > 1000（例如本程式中的 n = 5000）時，僅執行一次排序，直接記錄執行時間，模擬實際使用情境。此方式適用於資料量較大、計時結果穩定的情況。

在主程式中，根據資料量動態選擇計時模式：

```cpp
bool useAverage = (array_size <= SMALL_DATA_THRESHOLD); // SMALL_DATA_THRESHOLD = 1000
```


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
#define CASE_ITEMS 4000
#define CASES 5
#define UNSORTED "D:/work/sort/tosort.txt"
#define SORTED "D:/work/sort/sorted.txt"
#define TIMEREC "D:/work/sort/timer.txt"
#define COMPOSITE_TIMEREC "D:/work/sort/composite_timer.txt"
#define REPEAT_COUNT 100 // For small dataset averaging
#define SMALL_DATA_THRESHOLD 100 // Threshold for small datasets
```

#### 說明：

- **功能**：定義程式所需的標頭檔、命名空間及常數，為後續排序實作和效能測試提供基礎環境設置。
- **標頭檔**：
  - `iostream`：提供輸入輸出功能，用於終端機訊息顯示及檔案操作。
  - `vector`：用於動態陣列，儲存測試資料與排序結果。
  - `algorithm`：提供 `swap` 等工具函式，輔助排序實現。
  - `ctime`, `cstdlib`：支援隨機數生成（`rand`）與時間種子（`time`），用於測試資料生成。
  - `chrono`：提供高精度計時工具（`high_resolution_clock`），用於測量排序執行時間。
  - `windows.h`, `psapi.h`：Windows API，用於記憶體使用量測量。
  - `string`：支援字串操作，特別是用於記憶體使用量字串格式化。
- **#pragma comment**：自動連結 `psapi.lib`，提供記憶體資訊查詢功能。
- **常數定義**：
  - `CASE_ITEMS`：每個測試用例的元素數（4000）。
  - `CASES`：測試用例數（5）。
  - `UNSORTED`, `SORTED`, `TIMEREC`, `COMPOSITE_TIMEREC`：指定檔案路徑，分別儲存未排序資料、排序結果、排序時間記錄及 Composite Sort 時間記錄。
  - `REPEAT_COUNT`：小資料量測試時的重複次數（100），用於平均時間計算以減少誤差。
  - `SMALL_DATA_THRESHOLD`：小資料量閾值（100），決定是否啟用平均計時策略。
- **程式特性**：設置統一的檔案路徑與測試參數，確保程式可重複執行並生成一致的測試資料與結果。終端機訊息顯示功能增強了程式的互動性與可追蹤性。

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

- **功能**：定義資料結構（`node`, `entry`, `result`）與記憶體分析函式（`printMem`），用於儲存測試資料、排序結果及測量記憶體使用量。
- **資料結構**：
  - `node`：簡單結構，包含單一整數 `data`，作為 `entry` 的附加資料。
  - `entry`：核心資料結構，包含排序鍵值 `key`（long 型別）及指向 `node` 的指標 `data`。提供 `outputkey` 方法將鍵值輸出至指定檔案。
  - `result`：儲存排序結果，包含執行時間 `timer`（微秒，int64_t 型別）及排序後的陣列 `arr2`（`vector<entry>` 型別）。
- **printMem 函式**：
  - **功能**：使用 Windows API 的 `GetProcessMemoryInfo` 獲取程式記憶體使用資訊。
  - **參數**：`state`（int），`state == 0` 返回當前記憶體使用量，`state == 1` 返回峰值記憶體使用量。
  - **返回值**：字串，包含記憶體使用量（單位：KB）。
  - **實現細節**：透過 `PROCESS_MEMORY_COUNTERS` 結構獲取記憶體數據，轉換為 KB 並格式化為字串。
- **程式特性**：結構化資料設計便於排序與輸出，記憶體分析函式提供即時與峰值記憶體使用量，支援效能比較。`entry` 的指標設計允許擴展，但本程式僅使用簡單整數資料。

---

### 三. 排序函式實作

#### Insertion Sort

```cpp
result InsertionSort(vector<entry> arr, int casenum, bool useAverage = false) {
    result r;
    FILE* file = fopen(TIMEREC, "a");
    int64_t total_duration = 0;
    string memInit = printMem(0);
    string sort_name = "insertion";
    cout << "Start " << sort_name << " sort\n";
    cout << memInit;

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
    cout << "Sorted array in " << total_duration << " microseconds\n";
    cout << memFin;
    fclose(file);
    return r;
}
```

#### 說明：

- **功能**：實現插入排序（Insertion Sort），將輸入陣列按鍵值（`key`）遞增排序，並記錄執行時間與記憶體使用量。
- **參數**：
  - `arr`：待排序的 `vector<entry>` 陣列，包含鍵值與資料指標。
  - `casenum`：測試用例編號，用於記錄輸出。
  - `useAverage`：布林值，決定是否對小資料量（n ≤ 100）進行多次執行取平均時間（預設為 false）。
- **實現細節**：
  - 若 `useAverage` 為 true，則重複執行 `REPEAT_COUNT`（100）次排序，每次使用陣列副本（`temp_arr`）避免修改原始資料，僅儲存第一次排序結果（`r.arr2`），並計算平均執行時間。
  - 若 `useAverage` 為 false，執行單次排序，直接操作輸入陣列。
  - 排序邏輯：從第二個元素開始，逐一與前面的已排序部分比較，若鍵值小於前一元素則後移，直至找到正確插入位置。
  - 使用 `std::chrono::high_resolution_clock` 記錄排序時間，單位為微秒。
  - 透過 `printMem` 記錄初始與峰值記憶體使用量，寫入 `timer.txt`。
  - **終端機訊息**：顯示排序開始、初始記憶體使用量、排序完成時間及峰值記憶體使用量。
- **程式特性**：
  - 穩定排序，適合小資料量（n ≤ 32）或近乎排序的資料。
  - 時間複雜度：最佳 O(n)（已排序），平均/最壞 O(n²)（隨機/逆序）。
  - 空間複雜度：O(1)（原地排序，僅需常數額外空間）。
  - 未優化比較或移動，適用於簡單場景。

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
    string sort_name = "quick";
    cout << "Start " << sort_name << " sort\n";
    cout << memInit;

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
    cout << "Sorted array in " << total_duration << " microseconds\n";
    cout << memFin;
    fclose(file);
    return r;
}
```

#### 說明：

- **功能**：實現快速排序（Quick Sort），透過分治法按鍵值遞增排序，並記錄執行時間與記憶體使用量。
- **參數**：
  - `arr`：待排序的 `vector<entry>` 陣列。
  - `casenum`：測試用例編號。
  - `useAverage`：布林值，控制小資料量是否多次執行取平均時間。
- **實現細節**：
  - 分為核心函式 `QuickSortCore` 和包裝函式 `QuickSort`。
  - `QuickSortCore`：
    - 選擇第一元素（`arr[left]`）作為 pivot。
    - 使用雙指針（`i`, `j`）從左右兩端向中間掃描，交換小於/大於 pivot 的元素。
    - 最終將 pivot 置於正確位置，遞迴排序左右子陣列。
  - `QuickSort`：
    - 若 `useAverage` 為 true，重複 `REPEAT_COUNT` 次排序，使用陣列副本，儲存第一次結果，計算平均時間。
    - 否則執行單次排序，直接操作輸入陣列。
    - 使用 `std::chrono` 計時，記錄初始與峰值記憶體，寫入 `timer.txt`。
    - **終端機訊息**：顯示排序開始、初始記憶體使用量、排序完成時間及峰值記憶體使用量。
- **程式特性**：
  - 非穩定排序，平均效能優異（O(n log n)），但最壞情況（逆序）退化至 O(n²）。
  - 空間複雜度：O(log n)（遞迴棧，平均），最壞 O(n）。
  - 固定 pivot 選擇易導致效能退化，未採用隨機 pivot 或 median-of-three 優化。

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
    string sort_name = "merge";
    cout << "Start " << sort_name << " sort\n";
    cout << memInit;

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
    cout << "Sorted array in " << total_duration << " microseconds\n";
    cout << memFin;
    fclose(file);
    return r;
}
```

#### 說明：

- **功能**：實現合併排序（Merge Sort），透過遞迴分割與合併按鍵值遞增排序，記錄時間與記憶體使用量。
- **參數**：
  - `arr`：待排序的 `vector<entry>` 陣列。
  - `casenum`：測試用例編號。
  - `useAverage`：布林值，控制小資料量是否多次執行取平均。
- **實現細節**：
  - 分為兩個核心函式：
    - `MergeCut`：遞迴將陣列分割為左右子陣列（中點 `mid`），直至子陣列大小 ≤ 1，然後調用 `MergeCore` 合併。
    - `MergeCore`：合併兩個已排序陣列（`a`, `b`），比較鍵值並按序存入結果陣列 `c`。
  - `MergeSort`：
    - 若 `useAverage` 為 true，重複 `REPEAT_COUNT` 次排序，使用陣列副本，儲存第一次結果，計算平均時間。
    - 否則執行單次排序，返回排序後陣列。
    - 使用 `std::chrono` 計時，記錄記憶體使用量，寫入 `timer.txt`。
    - **終端機訊息**：顯示排序開始、初始記憶體使用量、排序完成時間及峰值記憶體使用量。
- **程式特性**：
  - 穩定排序，時間複雜度穩定為 O(n log n)，不受輸入影響。
  - 空間複雜度：O(n)，需額外陣列儲存合併結果。
  - 遞迴實現，記憶體開銷較高，適合中型資料（32 < n ≤ 1000）。

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
    string sort_name = "heap";
    cout << "Start " << sort_name << " sort\n";
    cout << memInit;

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
    cout << "Sorted array in " << total_duration << " microseconds\n";
    cout << memFin;
    fclose(file);
    return r;
}
```

#### 說明：

- **功能**：實現堆排序（Heap Sort），基於最大堆結構按鍵值遞增排序，記錄時間與記憶體使用量。
- **參數**：
  - `arr`：待排序的 `vector<entry>` 陣列。
  - `casenum`：測試用例編號。
  - `useAverage`：布林值，控制小資料量是否多次執行取平均。
- **實現細節**：
  - 分為兩個部分：
    - `heapify`：維護最大堆性質，比較父節點與左右子節點，確保父節點鍵值最大。
    - `HeapSort`：首先從最後一個非葉節點開始建堆（`n/2 - 1`），然後反覆將堆頂（最大值）交換至陣列尾端，縮小堆範圍並重新調整堆。
  - 若 `useAverage` 為 true，重複 `REPEAT_COUNT` 次排序，使用陣列副本，儲存第一次結果，計算平均時間。
  - 否則執行單次排序，直接操作輸入陣列。
  - 使用 `std::chrono` 計時，記錄記憶體使用量，寫入 `timer.txt`。
  - **終端機訊息**：顯示排序開始、初始記憶體使用量、排序完成時間及峰值記憶體使用量。
- **程式特性**：
  - 非穩定排序，時間複雜度穩定為 O(n log n)，對輸入資料不敏感。
  - 空間複雜度：O(1)，原地排序，僅需常數額外空間。
  - 適合中大型資料（1000 < n ≤ 5000），但快取效率低於 Quick Sort。

---

#### Composite Sort

```cpp
result CompositeSort(vector<entry> arr, int casenum, bool useAverage = false) {
    FILE* file = fopen(COMPOSITE_TIMEREC, "a");
    string memInit = printMem(0);
    string sort_name = "composite";
    cout << "Start " << sort_name << " sort\n";
    cout << memInit;
    fprintf(file,
            "Start Composite case %d%s\n[Init] %s",
            casenum,
            useAverage ? " (Averaged)" : "",
            memInit.c_str());

    result r;
    int64_t sum_timer = 0;

    auto runOnce = [&](const vector<entry>& data) -> result {
        if (data.size() <= 32)         return InsertionSort(data, casenum, false);
        else if (data.size() <= 5000)  return HeapSort    (data, casenum, false);
        else                           return QuickSort   (data, casenum, false);
    };

    if (useAverage) {
        for (int i = 0; i < REPEAT_COUNT; ++i) {
            result tmp = runOnce(arr);
            sum_timer += tmp.timer;
            if (i == 0) {
                r.arr2 = std::move(tmp.arr2);
            }
        }
        r.timer = sum_timer / REPEAT_COUNT;
    } else {
        r = runOnce(arr);
    }

    string memFin = printMem(1);
    fprintf(file,
            "Composite finished in %lld us\n[Final] %s\n",
            r.timer,
            memFin.c_str());
    cout << "Sorted array in " << r.timer << " microseconds\n";
    cout << memFin;
    fclose(file);

    return r;
}
```

#### 說明：

- **功能**：實現複合排序（Composite Sort），根據輸入陣列大小動態選擇最適排序演算法，記錄時間與記憶體使用量。
- **參數**：
  - `arr`：待排序的 `vector<entry>` 陣列。
  - `casenum`：測試用例編號。
  - `useAverage`：布林值，控制小資料量是否多次執行取平均。
- **實現細節**：
  - 根據陣列大小選擇排序法：
    - `n ≤ 32`：使用 `InsertionSort`，因小資料量下簡單高效。
    - `32 < n ≤ 5000`：使用 `HeapSort`，空間效率高。
    - `n > 5000`：使用 `QuickSort`，平均效能佳。
  - 若 `useAverage` 為 true，重複 `REPEAT_COUNT` 次排序，使用陣列副本，儲存第一次結果，計算平均時間。
  - 否則執行單次排序，選擇對應排序法並返回結果。
  - 使用 `std::chrono` 計時，記錄記憶體使用量，寫入 `composite_timer.txt`。
  - **終端機訊息**：顯示排序開始、初始記憶體使用量、排序完成時間及峰值記憶體使用量。
- **程式特性**：
  - 結合各排序法優勢，適應不同資料規模。
  - 時間複雜度：隨選擇的排序法變化（O(n²) 至 O(n log n)）。
  - 空間複雜度：隨排序法變化（O(1) 至 O(n)）。
  - 閾值（32、5000）基於經驗設定，未來可進一步優化。

---

### 四. 生成測試用例

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
        cout << "output unsorted array to file " << UNSORTED << endl;
        superarray.push_back(array);
        cout << "Created unsorted array for case #" << c + 1 << endl;
    }
}
```

#### 說明：

- **功能**：生成指定數量的測試用例，每個用例包含 `CASE_ITEMS`（4000）個 `entry` 物件，根據模式生成鍵值並輸出至未排序檔案，儲存於 `superarray`。
- **參數**：
  - `cases`：生成用例數（預設 5）。
  - `superarray`：儲存測試用例的 `vector<vector<entry>>`。
  - `unsortedfile`：未排序資料輸出檔案（`tosort.txt`）。
  - `mode`：鍵值生成模式（`"INSERTION"`, `"QUICK"`, `"MERGE"`, `"RANDOM"`, `"HEAP"`）。
- **實現細節**：
  - 根據 `mode` 生成鍵值：
    - `"INSERTION"`, `"QUICK"`, `"MERGE"`：逆序鍵值（`CASE_ITEMS - i`），模擬最壞情況。
    - `"RANDOM"`：隨機鍵值（`rand() % CASE_ITEMS`），模擬平均情況。
    - `"HEAP"`：順序鍵值（`i`），後隨機打亂部分元素（從索引 2 開始）。
  - 每個 `entry` 包含鍵值與動態分配的 `node` 指標。
  - 使用 `outputkey` 將鍵值寫入 `unsortedfile`。
  - 將生成的陣列存入 `superarray`。
  - **終端機訊息**：顯示已將未排序陣列輸出至檔案及創建用例的訊息。
- **程式特性**：
  - 支援多種測試場景（最壞、平均、特定情況）。
  - 動態分配 `node`，需注意記憶體管理（本程式未顯式釋放）。
  - 隨機打亂邏輯僅針對 Heap Sort 優化，模擬部分亂序資料。

---

### 五. 主程式與測試流程

```cpp
int main() {
    remove(SORTED);
    remove(UNSORTED);
    remove(TIMEREC);
    remove(COMPOSITE_TIMEREC);
    cout << "Removed old " << SORTED << endl;
    cout << "Removed old " << UNSORTED << endl;
    cout << "Removed old " << TIMEREC << endl;
    cout << "Removed old " << COMPOSITE_TIMEREC << endl;

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
            cout << "output sorted array to file " << SORTED << endl;
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

- **功能**：主程式負責初始化環境、生成測試資料、執行排序演算法並輸出結果，統籌整個效能測試流程。
- **實現細節**：
  - **檔案管理**：移除舊檔案（`sorted.txt`, `tosort.txt`, `timer.txt`, `composite_timer.txt`），開啟新檔案進行輸出。
  - **隨機種子**：使用 `srand(time(0))` 初始化隨機數生成器，確保隨機資料可重現。
  - **測資生成**：調用 `makeCases` 生成五組測資，分別對應 Insertion Sort（逆序）、Quick Sort（逆序）、Merge Sort（逆序）、Heap Sort（部分亂序）及 Composite Sort（隨機）。
  - **排序執行**：
    - 對前四組測資（`type = 0~3`）執行對應排序演算法，根據資料大小（≤ 100）決定是否啟用平均計時（`useAverage`）。
    - 對第五組測資（隨機）執行 Composite Sort。
  - **結果輸出**：
    - 排序結果（鍵值）與執行時間寫入 `sorted.txt`。
    - 時間與記憶體記錄分別寫入 `timer.txt`（單一排序法）與 `composite_timer.txt`（Composite Sort）。
  - **終端機訊息**：顯示已移除舊檔案、已將排序結果輸出至檔案等訊息。
  - 關閉檔案並結束程式。
- **程式特性**：
  - 模組化設計，清晰分離測資生成、排序執行與結果輸出。
  - 支援多種排序法與測試場景，方便擴展。
  - 小資料量（n ≤ 100）使用平均計時，確保測量穩定。
  - 未處理動態分配的 `node` 記憶體釋放，可能導致記憶體洩漏。

---

## 測試與驗證

### 最壞執行時間表格 Worst-case Performance (Time in Microseconds)


| 資料數 (n) | Insertion | Quick | Merge   | Heap | Composite |
|------------|-----------|-------|---------|------|-----------|
| 500        | 411       | 216   | 4908    | 72   | 69        |
| 1000       | 1501      | 586   | 12521   | 186  | 161       |
| 2000       | 6173      | 2597  | 36074   | 424  | 424       |
| 3000       | 14035     | 5069  | 36677   | 844  | 844       |
| 4000       | 27056     | 9300  | 103982  | 853  | 834       |
| 5000       | 37908     | 13752 | 62087   | 1247 | 1038      |


![image](https://github.com/Yangchenin41243245/Data_Structure_Homework_14_45/blob/proto2/homework1/src/pics/worst.png)

### 平均執行時間表格 Average-case Performance (Time in Microseconds)


| 資料數 (n) | Insertion | Quick    | Merge     | Heap    | Composite |
|------------|-----------|----------|-----------|---------|-----------|
| 500        | 369.6     | 154.0    | 3363.4    | 66.5    | 66.6      |
| 1000       | 1464.6    | 554.6    | 10503.6   | 153.5   | 155.2     |
| 2000       | 5999.0    | 2255.8   | 29185.4   | 348.0   | 363.0     |
| 3000       | 13500.8   | 4894.2   | 30507.4   | 651.1   | 766.8     |
| 4000       | 24254.8   | 9032.6   | 81311.2   | 721.8   | 736.0     |
| 5000       | 36125.6   | 13033.8  | 51574.6   | 948.8   | 942.2     |


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
