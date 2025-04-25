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
#include <string>

#pragma comment( lib, "psapi.lib" )
using namespace std;

#define CASE_ITEMS 500 // number of items in each case
#define CASES 5      // number of cases
#define RNGKEYS rand() % CASE_ITEMS
#define INSKEYS CASE_ITEMS - i // worst case for insertion sort
#define UNSORTED "D:/work/sort/tosort.txt"
#define SORTED "D:/work/sort/sorted.txt"
#define TIMEREC "D:/work/sort/timer.txt"
```

---

#### 說明：

##### 引入標頭檔
- `iostream`, `vector`, `algorithm`, `string`：常用的 C++ 標準庫，處理輸出、容器、演算法與字串。
- `ctime`, `cstdlib`：處理隨機數與時間。
- `chrono`：計時用，計算排序時間。
- `windows.h`, `psapi.h`：Windows API，用來查詢記憶體使用情況。

##### `#pragma comment(lib, "psapi.lib")`
讓 `psapi.h` 裡的函式在編譯時自動連結 `psapi.lib`，這是 Windows 的程式記憶體管理函式庫。

##### #define 指引
- `CASE_ITEMS`：每筆測試資料裡要排序的元素數量（500）。
- `CASES`：總共要做幾組測試（5 組）。
- `RNGKEYS`：隨機 key，用於亂數測資。
- `INSKEYS`：用於 Insertion Sort 最差情境的資料（由大到小，`CASE_ITEMS - i`）。
- `UNSORTED`, `SORTED`, `TIMEREC`：三個檔案的儲存路徑（未排序、排序後、計時紀錄）。

---

### 二. 記憶體與時間紀錄函式定義

```cpp
// memory usage check
SIZE_T memoryUsage() {
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    return pmc.WorkingSetSize;
}

// time recording
double recordTime(function<void()> sortFunc) {
    auto start = chrono::high_resolution_clock::now();
    sortFunc();  // 執行排序
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;
    return duration.count();
}
```

#### 說明：

##### `memoryUsage()` — 取得目前記憶體使用量

- 使用 Windows API：
  - `GetProcessMemoryInfo()`：取得目前執行程序的記憶體資訊。
  - `pmc.WorkingSetSize`：代表目前正在使用的實體記憶體（Working Set），以「位元組 (bytes)」為單位。
- 回傳型別是 `SIZE_T`，通常對應 `size_t`，足以儲存記憶體大小。

這個函式的用途是在排序前後量測記憶體用量。

---

#####  `recordTime()` — 執行排序並記錄所花時間

- 接收一個 `function<void()>` 作為參數，也就是接受任何無參數、無回傳值的函式（或 lambda）。
- 使用 `chrono::high_resolution_clock` 精準地記錄時間。
- 回傳值是毫秒（`duration<double, milli>`）。

讓每種排序方法都可以包裝成 lambda 傳入這個函式中，方便統一計時。

---

### 三. 排序函式實作與統一測試介面 `runSort()`

```cpp
// 實作排序方法
void insertionSort(vector<int>& arr);
void quickSort(vector<int>& arr);
void mergeSort(vector<int>& arr);
void heapSort(vector<int>& arr);

// 測試用排序介面
void runSort(string sortName, void (*sortFunc)(vector<int>&), vector<int> data, ofstream& logFile) {
    SIZE_T memBefore = memoryUsage();  // 排序前記憶體
    double time = recordTime([&]() { sortFunc(data); });  // 排序時間
    SIZE_T memAfter = memoryUsage();   // 排序後記憶體

    // 結果輸出
    cout << sortName << ": " << time << " ms, ";
    cout << "Memory used: " << (memAfter - memBefore) / 1024 << " KB" << endl;
    logFile << sortName << ": " << time << " ms, ";
    logFile << "Memory used: " << (memAfter - memBefore) / 1024 << " KB" << endl;
}
```

---

#### 說明：

##### 排序函式宣告

```cpp
void insertionSort(vector<int>& arr);
void quickSort(vector<int>& arr);
void mergeSort(vector<int>& arr);
void heapSort(vector<int>& arr);
```

這四個函式是用來排序整數陣列的，採用傳參考方式以便直接在原始資料上操作。它們的實作在程式後面，這裡僅宣告。

---

##### `runSort(...)` — 統一測試框架

**目的**：統一測量排序時間與記憶體，並印出與紀錄結果。

**參數解析**：
- `sortName`: 排序法名稱，用於輸出。
- `sortFunc`: 指向排序函式的函式指標（`void (*)(vector<int>&)`）。
- `data`: 排序資料，傳值進來，以避免修改原資料。
- `logFile`: 日誌檔案參考，用來紀錄輸出結果。

---

##### 運作流程：

**記錄排序前記憶體**：
   ```cpp
   SIZE_T memBefore = memoryUsage();
   ```

**計時排序函式執行時間**：
   ```cpp
   double time = recordTime([&]() { sortFunc(data); });
   ```
   - 使用 lambda 封裝 `sortFunc(data)` 呼叫，以符合 `recordTime()` 的格式。
   - Lambda 捕捉 `[&]` 表示引用捕捉，確保 `data` 被修改（這裡其實是複製進來的副本，不會影響原資料）。

**記錄排序後記憶體**：
   ```cpp
   SIZE_T memAfter = memoryUsage();
   ```

**輸出與紀錄測試結果**：
   ```cpp
   cout << ...;     // 印到螢幕
   logFile << ...;  // 寫入檔案
   ```
   記憶體差異以 KB 為單位輸出，方便閱讀。

---

### 四. 排序法實作

```cpp
void insertionSort(vector<int>& arr) {
    for (int i = 1; i < arr.size(); ++i) {
        int key = arr[i], j = i - 1;
        while (j >= 0 && arr[j] > key) arr[j + 1] = arr[j--];
        arr[j + 1] = key;
    }
}

void quickSort(vector<int>& arr) {
    function<void(int, int)> sort = [&](int l, int r) {
        if (l >= r) return;
        int pivot = arr[r], i = l - 1;
        for (int j = l; j < r; ++j)
            if (arr[j] < pivot) swap(arr[++i], arr[j]);
        swap(arr[++i], arr[r]);
        sort(l, i - 1);
        sort(i + 1, r);
    };
    sort(0, arr.size() - 1);
}

void mergeSort(vector<int>& arr) {
    function<void(int, int)> sort = [&](int l, int r) {
        if (l >= r) return;
        int m = (l + r) / 2;
        sort(l, m); sort(m + 1, r);
        vector<int> tmp;
        int i = l, j = m + 1;
        while (i <= m && j <= r)
            tmp.push_back(arr[i] < arr[j] ? arr[i++] : arr[j++]);
        while (i <= m) tmp.push_back(arr[i++]);
        while (j <= r) tmp.push_back(arr[j++]);
        for (int k = 0; k < tmp.size(); ++k) arr[l + k] = tmp[k];
    };
    sort(0, arr.size() - 1);
}

void heapSort(vector<int>& arr) {
    function<void(int, int)> heapify = [&](int n, int i) {
        int largest = i, l = 2 * i + 1, r = 2 * i + 2;
        if (l < n && arr[l] > arr[largest]) largest = l;
        if (r < n && arr[r] > arr[largest]) largest = r;
        if (largest != i) {
            swap(arr[i], arr[largest]);
            heapify(n, largest);
        }
    };
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; --i) heapify(n, i);
    for (int i = n - 1; i > 0; --i) {
        swap(arr[0], arr[i]);
        heapify(i, 0);
    }
}
```

#### 說明：

##### 1.Insertion Sort — 插入排序（O(n²)）

```cpp
for (int i = 1; i < arr.size(); ++i) {
    int key = arr[i], j = i - 1;
    while (j >= 0 && arr[j] > key) arr[j + 1] = arr[j--];
    arr[j + 1] = key;
}
```

- 對每一個元素，往前比較並插入至適當位置。
- 穩定排序，適合幾乎排序好的資料。
- **時間複雜度**：平均與最壞為 O(n²)，最佳為 O(n)。

---

##### 2.Quick Sort — 快速排序（平均 O(n log n)）

```cpp
function<void(int, int)> sort = [&](int l, int r) {
    if (l >= r) return;
    int pivot = arr[r], i = l - 1;
    for (int j = l; j < r; ++j)
        if (arr[j] < pivot) swap(arr[++i], arr[j]);
    swap(arr[++i], arr[r]);
    sort(l, i - 1);
    sort(i + 1, r);
};
sort(0, arr.size() - 1);
```

- 以最後一個元素作為 pivot。
- 小於 pivot 的元素會被移到左側，遞迴排序。
- 不穩定排序，**在最壞情況（遞增/遞減序列）會退化為 O(n²)**，但一般實務中表現極佳。

---

##### 3.Merge Sort — 合併排序（穩定、O(n log n)）

```cpp
function<void(int, int)> sort = [&](int l, int r) {
    if (l >= r) return;
    int m = (l + r) / 2;
    sort(l, m); sort(m + 1, r);
    ...
};
```

- 將陣列遞迴切半後合併，合併時保持排序。
- **空間複雜度 O(n)**，需要額外暫存空間。
- 穩定排序，適合資料量大且對穩定性有要求的應用。

---

##### 4.Heap Sort — 堆積排序（不穩定、O(n log n)）

```cpp
function<void(int, int)> heapify = [&](int n, int i) { ... };
```

- 先建 max heap，再依序將最大值交換到尾端並縮小 heap 區間。
- 不需要額外空間，**空間複雜度 O(1)**。
- 時間複雜度穩定為 O(n log n)，但常數大、快取不友善，實務上不如 QuickSort 快。

---

### 五. makeCases 函數 — 測資生成器

```cpp
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
            if (mode == "INSERTION" || mode == "QUICK" || mode == "MERGE") // generate key nums by mode
                key = INSKEYS; //do worst case = max -> min
            else if (mode == "RANDOM")
                key = RNGKEYS;
            else
                key = i;     // will process after array is created
            entry e(key, n); // create entry with key and node
            array.push_back(e);
        }
        if (mode == "HEAP") // do the Permutation() provided
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
```
#### 說明：

- **參數**：
  - `cases`：用例數。
  - `superarray`：儲存用例（傳引用）。
  - `unsortedfile`：未排序檔案。
  - `mode`：鍵值模式。
- **資料結構**：`vector<entry>` 儲存用例，`entry` 含 `key` 和 `node`。
- **控制流程**：雙層迴圈，條件分支設置鍵值。
- **檔案 I/O**：格式化輸出鍵值。

#### 運作流程：

**迴圈生成用例**：
   - 外層迴圈迭代 `cases` 次（5 次）。

**初始化陣列**：
   - 創建 `vector<entry> array`。
   - 寫入用例標頭到 `unsortedfile`（"Case X with 6000 items:"）。

**生成元素**：
   - 內層迴圈生成 6000 個 `entry`：
     - 分配 `node` 物件。
     - 根據 `mode` 設置鍵值：
       - "INSERTION"/"QUICK"/"MERGE"：逆序（`INSKEYS`，6000, 5999, ..., 1）。
       - "RANDOM"：隨機（`RNGKEYS`，未使用）。
       - 其他（"HEAP"）：序數（0, 1, ..., 5999）。
     - 創建 `entry` 並加入 `array`。

**隨機排列（Heap）**：
   - 若 `mode` 為 "HEAP"，從索引 5999 到 2 隨機交換元素。

**輸出鍵值**：
   - 遍歷 `array`，寫入鍵值到 `unsortedfile`。
   - 控制台確認寫入。

**儲存用例**：
   - 將 `array` 加入 `superarray`，控制台確認創建。

---

### 6. `main` 主程式

```cpp
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

    srand(time(0)); // set random
    vector<vector<entry>> superarray[4]; // all cases stored here
    result result;
    //MAKE CASES HERE/////////////////////////////////////
    makeCases(CASES, superarray[0], f_Unsorted, "INSERTION"); 
    makeCases(CASES, superarray[1], f_Unsorted, "QUICK"); 
    makeCases(CASES, superarray[2], f_Unsorted, "MERGE"); 
    makeCases(CASES, superarray[3], f_Unsorted, "HEAP"); // create cases, provide file path for unsorted cases
    //////////////////////////////////////////////////////
    //SORT ALL/////////////////////////////////////////////////////////////////
    for (int type = 0; type < 4;type++){ //type m
        for (int i = 0, caseNum = 1; i < superarray[type].size(); i++, caseNum++) // SA of type m with i cases
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
            default:
                break;
            }
            duration = result.timer; // get duration from result struct
            //OUTPUT TIME
            fprintf(f_Sorted, "\nCase %d of %lu items finished in %lu microseconds\n", caseNum, CASE_ITEMS, duration);
            for (int j = 0; j < CASE_ITEMS; j++) // output keys of sorted array
            {
                result.arr2[j].outputkey(f_Sorted); // i for array, j for entry
            }
            cout << "output sorted array to file" << SORTED << endl;
        }
    }
    /////////////////////////////////////////////////////////////////////////////////
    fclose(f_Unsorted);
    fclose(f_Sorted);
    return 0;
}
```

---

#### 說明：

##### 移除舊檔案（清空資料）
```cpp
remove(SORTED);
remove(UNSORTED);
remove(TIMEREC);
```
這段會刪除之前執行時產生的三個檔案：
- `sorted.txt`：儲存排序後資料
- `tosort.txt`：儲存原始（未排序）資料
- `timer.txt`：儲存每次排序的時間與記憶體紀錄

---

##### 開啟檔案進行輸出
```cpp
FILE *f_Unsorted = fopen(UNSORTED, "a");
FILE *f_Sorted = fopen(SORTED, "a");
```
開啟（或建立）用來輸出未排序與已排序資料的檔案。

---

##### 初始化亂數與資料結構
```cpp
srand(time(0));
vector<vector<entry>> superarray[4];
result result;
```
- `srand(time(0))`：初始化亂數種子。
- `superarray[4]`：四種排序法的所有測試案例存放區，每個排序法都會有 5 組測資。
- `result`：記錄每次排序後的時間與排序結果。

---

##### 產生排序測試案例
```cpp
makeCases(CASES, superarray[0], f_Unsorted, "INSERTION"); 
makeCases(CASES, superarray[1], f_Unsorted, "QUICK"); 
makeCases(CASES, superarray[2], f_Unsorted, "MERGE"); 
makeCases(CASES, superarray[3], f_Unsorted, "HEAP");
```
每次呼叫 `makeCases()` 會生成 5 組測試資料，依不同排序類型使用不同排序特性資料：
- Insertion、Quick、Merge：worst-case 由大排到小（INSKEYS）
- Heap：隨機打亂

---

##### 執行排序與輸出結果
```cpp
for (int type = 0; type < 4;type++){
    for (int i = 0, caseNum = 1; i < superarray[type].size(); i++, caseNum++){
        ...
        result = InsertionSort(...); // 或其他排序
        ...
        fprintf(f_Sorted, "...", caseNum, CASE_ITEMS, duration);
        ...
        result.arr2[j].outputkey(f_Sorted);
    }
}
```

- 外層迴圈跑四種排序。
- 內層跑每種排序對應的五筆資料。
- 執行排序後：
  - 印出花費的時間
  - 記錄記憶體用量
  - 將排序結果寫入檔案

---

##### 關閉檔案
```cpp
fclose(f_Unsorted);
fclose(f_Sorted);
```
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
