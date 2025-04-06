# 排序演算法效能分析報告

## 一、時鐘精度

本程式使用 C++ 的 `<chrono>` 標準函式庫中的 `high_resolution_clock` 進行計時，使用 `duration_cast<microseconds>` 將執行時間換算為「微秒」(1 微秒 = 0.000001 秒)。這提供了足夠高的解析度以正確測量常見排序演算法在中等規模資料量（如 5000 筆資料）下的執行效能。

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

### Insertion Sort（最壞情況）

| 測試編號 | 時間 (μs) |
|----------|-----------|
| Case 1   | 37396     |
| Case 2   | 36590     |
| Case 3   | 35893     |
| Case 4   | 37442     |
| Case 5   | 35985     |
| Case 6   | 37257     |
| Case 7   | 43953     |
| Case 8   | 39285     |
| Case 9   | 47480     |
| Case 10  | 36604     |

**平均：38788.5 μs**

---

### Quick Sort（最壞情況）

| 測試編號 | 時間 (μs) |
|----------|-----------|
| Case 1   | 13886     |
| Case 2   | 13953     |
| Case 3   | 13662     |
| Case 4   | 13528     |
| Case 5   | 14390     |
| Case 6   | 15614     |
| Case 7   | 14577     |
| Case 8   | 18947     |
| Case 9   | 14388     |
| Case 10  | 13786     |

**平均：14773.1 μs**

---

### Merge Sort（最壞情況）

| 測試編號 | 時間 (μs) |
|----------|-----------|
| Case 1   | 29374     |
| Case 2   | 23874     |
| Case 3   | 33588     |
| Case 4   | 30741     |
| Case 5   | 32597     |
| Case 6   | 35475     |
| Case 7   | 33081     |
| Case 8   | 30647     |
| Case 9   | 31019     |
| Case 10  | 30487     |

**平均：31088.3 μs**

---

### Heap Sort（平均情況）

| 測試編號 | 時間 (μs) |
|----------|-----------|
| Case 1   | 940       |
| Case 2   | 871       |
| Case 3   | 903       |
| Case 4   | 1103      |
| Case 5   | 1123      |
| Case 6   | 941       |
| Case 7   | 946       |
| Case 8   | 831       |
| Case 9   | 946       |
| Case 10  | 918       |

**平均：952.2 μs**

