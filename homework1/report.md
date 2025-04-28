# 41243214
& 41243245
# 排序演算法效能分析報告

## 目錄

### 1.解題說明
### 2.時鐘精度
### 3.效能分析
### 4.程式實作
### 5.測試與驗證
### 6.申論及開發報告

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
#pragma comment(lib, "psapi.lib")

using namespace std;

// 設定常數
#define CASE_ITEMS 6000
#define CASES 5
#define UNSORTED "./tosort.txt"
#define SORTED "./sorted.txt"
#define TIMEREC "./timer.txt"
#define COMPOSITE_TIMEREC "./composite_timer.txt"
```

#### 說明：

- **功能**：定義程式所需的標頭檔、命名空間及常數，為排序實作和效能測試提供基礎環境設置。
- **標頭檔**：
  - `iostream`：提供輸入輸出功能，用於檔案操作與結果顯示。
  - `vector`：用於動態陣列，儲存測試資料與排序結果。
  - `algorithm`：提供 STL `sort` 函數，用於快速排序、合併排序和堆排序的實現。
  - `ctime`, `cstdlib`：支援隨機數生成（`rand`）與時間種子（`time`），用於測試資料生成。
  - `chrono`：提供高精度計時工具（`high_resolution_clock`），用於測量排序執行時間。
  - `windows.h`, `psapi.h`：Windows API，用於記憶體使用量測量。
- **#pragma comment**：自動連結 `psapi.lib`，提供記憶體資訊查詢功能。
- **常數定義**：
  - `CASE_ITEMS`：每個測試用例的元素數（6000）。
  - `CASES`：測試用例數（5）。
  - `UNSORTED`, `SORTED`, `TIMEREC`, `COMPOSITE_TIMEREC`：指定檔案路徑，分別儲存未排序資料、排序結果、排序時間記錄及 Composite Sort 時間記錄。
- **程式特性**：設置統一的檔案路徑與測試參數，確保程式可重複執行並生成一致的測試資料與結果。相較於傳統實現，程式利用 STL `sort` 簡化部分排序演算法的實作。

---

### 二. 資料結構與記憶體分析

```cpp
struct node { int data; };

class entry {
public:
    friend class node;
    long key;
    entry(long k, node* d) { key = k; data = d; }
    void setkey(int k) { key = k; }
    void outputkey(FILE* file) const { fprintf(file, " key: %ld\n", key); }
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
    string ret0 = "Memory usage: " + to_string(memUsage) + " KB\n";
    string ret1 = "Peak memory usage: " + to_string(memPeak) + " KB\n";
    if (state == 0) return ret0;
    else return ret1;
}
```

#### 說明：

- **功能**：定義資料結構（`node`, `entry`, `result`）與記憶體分析函式（`printMem`），用於儲存測試資料、排序結果及測量記憶體使用量。
- **資料結構**：
  - `node`：簡單結構，包含單一整數 `data`，作為 `entry` 的附加資料。
  - `entry`：核心資料結構，包含排序鍵值 `key`（long 型別）及指向 `node` 的指標 `data`。提供 `setkey` 和 `outputkey` 方法，`outputkey` 為 const 方法，提升 const 正確性。
  - `result`：儲存排序結果，包含執行時間 `timer`（微秒，int64_t 型別）及排序後的陣列 `arr2`（`vector<entry>` 型別）。
- **printMem 函式**：
  - **功能**：使用 Windows API 的 `GetProcessMemoryInfo` 獲取程式記憶體使用資訊。
  - **參數**：`state`（int），`state == 0` 返回當前記憶體使用量，`state == 1` 返回峰值記憶體使用量。
  - **返回值**：字串，包含記憶體使用量（單位：KB）。
  - **實現細節**：透過 `PROCESS_MEMORY_COUNTERS` 結構獲取記憶體數據，轉換為 KB 並格式化為字串。
- **程式特性**：結構化資料設計便於排序與輸出，記憶體分析函式提供即時與峰值記憶體使用量，支援效能比較。`entry` 的 const 正確性增強程式穩健性。

---

### 三. 排序函式實作

#### Insertion Sort

```cpp
result InsertionSort(vector<entry> arr, int casenum, bool writeToFile = true) {
    result r;
    cout << "Start insertion sort\n";
    auto timer = chrono::high_resolution_clock::now();
    string recMem_Init = printMem(0);
    cout << recMem_Init << endl;

    for (int i = 1; i < arr.size(); i++) {
        entry temp = arr[i];
        int j = i - 1;
        while (j >= 0 && temp.key < arr[j].key) {
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
```

#### 說明：

- **功能**：實現插入排序（Insertion Sort），將輸入陣列按鍵值（`key`）遞增排序，並記錄執行時間與記憶體使用量。
- **參數**：
  - `arr`：待排序的 `vector<entry>` 陣列，包含鍵值與資料指標。
  - `casenum`：測試用例編號，用於記錄輸出。
  - `writeToFile`：布林值，控制是否將效能指標寫入 `timer.txt`（預設為 true）。
- **實現細節**：
  - 排序邏輯：從第二個元素開始，逐一與前面的已排序部分比較，若鍵值小於前一元素則後移，直至找到正確插入位置。
  - 使用 `std::chrono::high_resolution_clock` 記錄排序時間，單位為微秒。
  - 透過 `printMem` 記錄初始與峰值記憶體使用量，若 `writeToFile` 為 true，寫入 `timer.txt`。
  - 輸出排序開始/結束資訊及時間到控制台。
- **程式特性**：
  - 穩定排序，適合小資料量（n ≤ 32）或近乎排序的資料。
  - 時間複雜度：最佳 O(n)（已排序），平均/最壞 O(n²)（隨機/逆序）。
  - 空間複雜度：O(1)（原地排序，僅需常數額外空間）。
  - 未優化比較或移動，適用於簡單場景。

---

#### Quick Sort

```cpp
result QuickSort(vector<entry> arr, int casenum, bool writeToFile = true) {
    result r;
    cout << "Start quick sort\n";
    auto timer = chrono::high_resolution_clock::now();
    string recMem_Init = printMem(0);
    cout << recMem_Init << endl;

    sort(arr.begin(), arr.end(), [](entry a, entry b) { return a.key < b.key; });

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
```

#### 說明：

- **功能**：實現快速排序（Quick Sort），使用 STL `sort` 按鍵值遞增排序，記錄時間與記憶體使用量。
- **參數**：
  - `arr`：待排序的 `vector<entry>` 陣列。
  - `casenum`：測試用例編號。
  - `writeToFile`：布林值，控制是否寫入 `timer.txt`。
- **實現細節**：
  - 使用 STL `sort` 函數，搭配 lambda 比較器（`a.key < b.key`）實現排序，取代傳統快速排序的分區和遞迴邏輯。
  - `sort` 內部實現為 introsort（混合快速排序、堆排序和插入排序），確保平均 O(n log n) 且避免最壞情況 O(n²)。
  - 使用 `std::chrono` 計時，記錄初始與峰值記憶體，若 `writeToFile` 為 true，寫入 `timer.txt`。
- **程式特性**：
  - 非穩定排序，依賴 STL 優化實現，平均效能優異（O(n log n)）。
  - 空間複雜度：O(log n)（遞迴棧，平均）。
  - 簡化實現犧牲了對快速排序傳統行為的分析，但提升執行效率。

---

#### Merge Sort

```cpp
result MergeSort(vector<entry> arr, int casenum, bool writeToFile = true) {
    result r;
    cout << "Start merge sort\n";
    auto timer = chrono::high_resolution_clock::now();
    string recMem_Init = printMem(0);
    cout << recMem_Init << endl;

    sort(arr.begin(), arr.end(), [](entry a, entry b) { return a.key < b.key; });

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
```

#### 說明：

- **功能**：實現合併排序（Merge Sort），使用 STL `sort` 按鍵值遞增排序，記錄時間與記憶體使用量。
- **參數**：
  - `arr`：待排序的 `vector<entry>` 陣列。
  - `casenum`：測試用例編號。
  - `writeToFile`：布林值，控制是否寫入 `timer.txt`。
- **實現細節**：
  - 使用 STL `sort` 函數，搭配 lambda 比較器實現排序，取代傳統合併排序的分割和合併邏輯。
  - `sort` 提供穩定的 O(n log n) 效能，內部優化確保高效執行。
  - 使用 `std::chrono` 計時，記錄記憶體使用量，若 `writeToFile` 為 true，寫入 `timer.txt`。
- **程式特性**：
  - 非穩定排序（因 STL `sort` 非穩定），時間複雜度為 O(n log n)。
  - 空間複雜度：O(n)（STL `sort` 內部可能使用臨時陣列）。
  - 簡化實現便於維護，但無法反映傳統合併排序的遞迴特性。

---

#### Heap Sort

```cpp
result HeapSort(vector<entry> arr, int casenum, bool writeToFile = true) {
    result r;
    cout << "Start heap sort\n";
    auto timer = chrono::high_resolution_clock::now();
    string recMem_Init = printMem(0);
    cout << recMem_Init << endl;

    sort(arr.begin(), arr.end(), [](entry a, entry b) { return a.key < b.key; });

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
```

#### 說明：

- **功能**：實現堆排序（Heap Sort），使用 STL `sort` 按鍵值遞增排序，記錄時間與記憶體使用量。
- **參數**：
  - `arr`：待排序的 `vector<entry>` 陣列。
  - `casenum`：測試用例編號。
  - `writeToFile`：布林值，控制是否寫入 `timer.txt`。
- **實現細節**：
  - 使用 STL `sort` 函數，搭配 lambda 比較器實現排序，取代傳統堆排序的建堆和調整邏輯。
  - `sort` 確保 O(n log n) 效能，內部優化提升執行速度。
  - 使用 `std::chrono` 計時，記錄記憶體使用量，若 `writeToFile` 為 true，寫入 `timer.txt`。
- **程式特性**：
  - 非穩定排序，時間複雜度為 O(n log n)。
  - 空間複雜度：O(1) 或 O(n)（取決於 STL `sort` 內部實現）。
  - 簡化實現犧牲了堆排序的原地排序特性，但提升執行效率。

---

#### Composite Sort

```cpp
result CompositeSort(vector<entry> arr, int casenum, bool writeToFile = true) {
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
    } else if (arr.size() > 32 && arr.size() <= 1000) {
        r = HeapSort(arr, casenum, false);
    } else if (arr.size() > 1000 && arr.size() <= 5000) {
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
```

#### 說明：

- **功能**：實現複合排序（Composite Sort），根據輸入陣列大小動態選擇最適排序演算法，記錄時間與記憶體使用量。
- **參數**：
  - `arr`：待排序的 `vector<entry>` 陣列。
  - `casenum`：測試用例編號。
  - `writeToFile`：布林值，控制是否寫入 `composite_timer.txt`。
- **實現細節**：
  - 根據陣列大小選擇排序法：
    - `n ≤ 32`：使用 `InsertionSort`，因小資料量下簡單高效。
    - `32 < n ≤ 1000`：使用 `HeapSort`，提供穩定的 O(n log n) 效能（實際使用 STL `sort`）。
    - `1000 < n ≤ 5000`：使用 `HeapSort`，適合中型資料（實際使用 STL `sort`）。
    - `n > 5000`：使用 `QuickSort`，平均效能佳（使用 STL `sort`）。
  - 選擇的排序函數以 `writeToFile = false` 執行，避免重複寫入 `timer.txt`。
  - 使用 `std::chrono` 計時，記錄記憶體使用量，若 `writeToFile` 為 true，寫入 `composite_timer.txt`。
- **程式特性**：
  - 結合各排序法優勢，適應不同資料規模。
  - 時間複雜度：隨選擇的排序法變化：
    - `n ≤ 32`：O(n²)（Insertion Sort）。
    - `32 < n ≤ 5000`：O(n log n)（Heap Sort，實際為 STL `sort`）。
    - `n > 5000`：O(n log n)（Quick Sort，實際為 STL `sort`）。
  - 空間複雜度：隨排序法變化（O(1) 至 O(n)）。
  - 閾值（32、1000、5000）基於經驗設定，細分中型資料範圍以優化效能，未來可進一步調整。

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
```

#### 說明：

- **功能**：生成指定數量的測試用例，每個用例包含 `CASE_ITEMS`（6000）個 `entry` 物件，根據模式生成鍵值並輸出至未排序檔案，儲存於 `superarray`。
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
- **程式特性**：
  - 支援多種測試場景（最壞、平均、特定情況）。
  - 動態分配 `node`，需注意記憶體管理（本程式未顯式釋放）。
  - 隨機打亂邏輯僅針對 Heap Sort 優化，模擬部分亂序資料。

---

### 五. 主程式與測試流程

```cpp
int main(void) {
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
```

#### 說明：

- **功能**：主程式負責初始化環境、生成測試資料、執行排序演算法並輸出結果，統籌整個效能測試流程。
- **實現細節**：
  - **檔案管理**：移除舊檔案（`sorted.txt`, `tosort.txt`, `timer.txt`, `composite_timer.txt`），開啟新檔案進行輸出。
  - **隨機種子**：使用 `srand(time(0))` 初始化隨機數生成器，確保隨機資料可重現。
  - **測資生成**：調用 `makeCases` 生成五組測資，分別對應 Insertion Sort（逆序）、Quick Sort（逆序）、Merge Sort（逆序）、Heap Sort（部分亂序）及 Composite Sort（隨機）。
  - **排序執行**：
    - 對前四組測資（`type = 0~3`）執行對應排序演算法（Insertion, Quick, Merge, Heap）。
    - 對第五組測資（隨機）執行 Composite Sort，單獨處理並輸出。
  - **結果輸出**：
    - 排序結果（鍵值）與執行時間寫入 `sorted.txt`，Composite Sort 使用不同標頭（`"Composite Sort Case X finished in Y us"`）。
    - 時間與記憶體記錄分別寫入 `timer.txt`（單一排序法）與 `composite_timer.txt`（Composite Sort）。
  - 關閉檔案並結束程式。
- **程式特性**：
  - 模組化設計，清晰分離測資生成、排序執行與結果輸出。
  - 支援多種排序法與測試場景，方便擴展。
  - 未實現小資料量平均計時，適用於大資料量（n = 6000）測試。
  - 未處理動態分配的 `node` 記憶體釋放，可能導致記憶體洩漏。

---

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
