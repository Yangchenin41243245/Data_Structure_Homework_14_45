# 排序演算法效能分析報告

## 1. 時鐘精度
本次測試中，時鐘精度設定為 **8 毫秒（ms）**。

## 2. Heap Sort 隨機排列次數
為了確保測試的準確性，我們對 Heap Sort 進行至少 **10 次** 隨機排列測試。

## 3. Merge Sort 最壞情況的數據產生方式
Merge Sort 的最壞情況數據是通過逆向模擬合併過程來產生的，即使合併過程盡可能地耗時。

## 4. 排序時間測試結果
### 測試數據（單位：微秒）
| n | Insertion Sort | Quick Sort | Merge Sort | Heap Sort | Composite Sort |
|---|---------------|------------|------------|-----------|---------------|
| 500  | 1523  | 65   | 910  | 115  | 62   |
| 1000 | 5167  | 135  | 1782 | 252  | 139  |
| 2000 | 20301 | 319  | 3600 | 557  | 3667 |
| 3000 | 46695 | 1025 | 5969 | 1005 | 7381 |
| 4000 | 78090 | 685  | 7202 | 1248 | 7402 |
| 5000 | 131192| 850  | 11597| 2353 | 13819|

## 5. 圖表分析
（請插入測試結果的圖表來比較不同排序方法的效能）

## 6. C++ 程式碼

### 6.1 引入標頭與命名空間
```cpp
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace std::chrono;
```

### 6.2 插入排序（Insertion Sort）
```cpp
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
```
**說明：** 插入排序適用於小規模數據，最壞情況下時間複雜度為 O(n²)。

### 6.3 快速排序（Quick Sort，三數中值選取）
```cpp
int medianOfThree(vector<int>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    if (arr[mid] < arr[low]) swap(arr[low], arr[mid]);
    if (arr[high] < arr[low]) swap(arr[low], arr[high]);
    if (arr[mid] < arr[high]) swap(arr[mid], arr[high]);
    return arr[high];
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = medianOfThree(arr, low, high);
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void quickSortWrapper(vector<int>& arr) {
    quickSort(arr, 0, arr.size() - 1);
}
```
**說明：** 採用三數中值法來選取基準點，可降低最壞情況發生的概率。

### 6.4 迭代版合併排序（Merge Sort）
```cpp
void mergeSort(vector<int>& arr) {
    int n = arr.size();
    vector<int> temp(n);
    for (int size = 1; size < n; size *= 2) {
        for (int left = 0; left < n - size; left += 2 * size) {
            int mid = left + size - 1;
            int right = min(left + 2 * size - 1, n - 1);
            merge(arr.begin() + left, arr.begin() + mid + 1, arr.begin() + mid + 1, arr.begin() + right + 1, temp.begin() + left);
        }
        arr = temp;
    }
}
```
**說明：** 迭代版合併排序避免了遞迴調用，可降低遞迴開銷。

### 6.5 堆排序（Heap Sort）
```cpp
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}
```
**說明：** 堆排序具有穩定的 O(n log n) 複雜度，適用於大數據量。

### 6.6 組合排序（Composite Sort）
```cpp
void compositeSort(vector<int>& arr) {
    int n = arr.size();
    if (n <= 20) insertionSort(arr);
    else if (n <= 1000) quickSort(arr, 0, n - 1);
    else mergeSort(arr);
}
```
**說明：** 根據數據量選擇最佳排序方法，以達到最佳效能。

### 6.7 測試與計時函數
```cpp
void measureTime(void (*sortFunc)(vector<int>&), vector<int> arr, string name) {
    auto start = high_resolution_clock::now();
    sortFunc(arr);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << name << " took " << duration.count() << " microseconds" << endl;
}
```
**說明：** 用於計算各種排序算法的執行時間。

### 6.8 主程式
```cpp
int main() {
    vector<int> sizes = { 500, 1000, 2000, 3000, 4000, 5000 };
    for (int n : sizes) {
        cout << "\nTesting n = " << n << "\n";
        vector<int> arr = randomPermutation(n);
        measureTime(compositeSort, arr, "Composite Sort");
    }
    return 0;
}
