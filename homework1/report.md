# 排序演算法效能比較報告

## 1. 時鐘精度
本次測試使用的時鐘精度為 **8 毫秒**。

## 2. Heap Sort 隨機排列次數
為了確保獲得接近最壞情況的數據，每個 `n` 值至少進行 **10 次** 隨機排列測試。

## 3. Merge Sort 最壞情況數據產生方式
Merge Sort 的最壞情況數據是透過逆向合併過程生成的，使每次合併操作都達到最不利的情境。

## 4. 排序時間測試結果
### 不同 `n` 值的排序時間（單位：微秒）

| `n` 值 | Insertion Sort | Quick Sort | Merge Sort | Heap Sort | Composite Sort |
|--------|---------------|------------|------------|-----------|---------------|
| 500    | 1523          | 65         | 910        | 115       | 62            |
| 1000   | 5167          | 135        | 1782       | 252       | 139           |
| 2000   | 20301         | 319        | 3600       | 557       | 3667          |
| 3000   | 46695         | 1025       | 5969       | 1005      | 7381          |
| 4000   | 78090         | 685        | 7202       | 1248      | 7402          |
| 5000   | 131192        | 850        | 11597      | 2353      | 13819         |

## 5. 排序法效能比較圖表
**(之後再加排序時間圖表)**

## 6. 最佳組合排序函數的時間表與圖表
Composite Sort 透過選擇最適合的排序方法來優化效能，其結果如下：

| `n` 值 | Composite Sort 時間 (微秒) |
|--------|---------------------------|
| 500    | 62                        |
| 1000   | 139                       |
| 2000   | 3667                      |
| 3000   | 7381                      |
| 4000   | 7402                      |
| 5000   | 13819                     |

**(同上)**

## 7. C++ 程式碼
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

// Insertion Sort
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

// Quick Sort (Median-of-Three)
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

// Merge Sort (Iterative)
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

// Heap Sort
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

// Composite Sort
void compositeSort(vector<int>& arr) {
    int n = arr.size();
    if (n <= 20) insertionSort(arr);
    else if (n <= 1000) quickSort(arr, 0, n - 1);
    else mergeSort(arr);
}

// 測試排序時間
void measureTime(void (*sortFunc)(vector<int>&), vector<int> arr, string name) {
    auto start = high_resolution_clock::now();
    sortFunc(arr);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << name << " took " << duration.count() << " microseconds" << endl;
}

int main() {
    vector<int> sizes = { 500, 1000, 2000, 3000, 4000, 5000 };
    for (int n : sizes) {
        cout << "\nTesting n = " << n << "\n";
        vector<int> arr;
        arr = randomPermutation(n);
        measureTime(compositeSort, arr, "Composite Sort");
    }
    return 0;
}
