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

// Generate Worst-case Data
vector<int> worstCaseInsertion(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; i++) arr[i] = n - i;
    return arr;
}

vector<int> randomPermutation(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; i++) arr[i] = i + 1;
    random_shuffle(arr.begin(), arr.end());
    return arr;
}

// Composite Sort
void compositeSort(vector<int>& arr) {
    int n = arr.size();
    if (n <= 20) insertionSort(arr);
    else if (n <= 1000) quickSort(arr, 0, n - 1);
    else mergeSort(arr);
}

// Measure Execution Time
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

        arr = worstCaseInsertion(n);
        measureTime(insertionSort, arr, "Insertion Sort");

        arr = randomPermutation(n);
        measureTime(quickSortWrapper, arr, "Quick Sort");

        arr = randomPermutation(n);
        measureTime(mergeSort, arr, "Merge Sort");

        arr = randomPermutation(n);
        measureTime(heapSort, arr, "Heap Sort");

        arr = randomPermutation(n);
        measureTime(compositeSort, arr, "Composite Sort");
    }
    return 0;
}
