# 41243245 & 41243214

作業一

## 解題說明

作業要求使用不同排序法找出在最壞情況下表現最好的方法
1.Insertion
2.Quick
3.Merge
4.Heap

### 步驟

1.實作四種排序法，針對key(整數)由小到大排序內容<br>
2.產生對各種方式最壞場合的測資<br>
。Insertion & Merge: 由大排到小
。Quick & Heap: 使用題目提供的排列函式
```
if (mode == "QUICK" || mode == "HEAP") // do the Permutation() provided
        {
            for (int i = CASE_ITEMS - 1; i >= 2; i--)
            {
                int j = rand() % i + 1;
                swap(array[i], array[j]);
            }
        }
```
<br>
3.設定迴圈數後執行排序法並記錄耗時<br>
4.整理資料筆數 & 執行時間的關係
## 程式實作

以下為主要程式碼：

```
//in main
srand(time(0));  // set random seed
    vector<vector<entry>> superarray; // all cases stored here

    makeCases(CASES, superarray, f_Unsorted, "MERGE"); // create cases, provide file path for unsorted cases

    for (int i = 0, c = 1; i < superarray.size(); i++, c++) // sort each case
    {   //CHANGE THIS BLOCK///////////////////////////////////////////////////////
        auto duration = MergeSort(superarray[i], c); // returns runtime after sorting
        //////////////////////////////////////////////////////////////////////////
        fprintf(f_Sorted, "\nCase %d of %lu items finished in %lu microseconds\n", c, CASE_ITEMS, duration);
        for (int j = 0; j < CASE_ITEMS; j++) // output keys of sorted array
        {
            superarray[i][j].outputkey(f_Sorted); // i for array, j for entry
        }
        cout << "output sorted array to file" << SORTED << endl;
    }
```
```
//class entry內部結構
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
    node *data; //only includes an int atm
};
```
```
 cout << "Start insertion sort\n";
    auto timer = high_resolution_clock::now();
    for (long i = 0; i < arr.size(); i++)
    {
        long min = i;
        for (long j = i + 1; j < arr.size(); j++) // get min key and swap then go forward and exclude sorted
        {
            if (arr[j].key < arr[min].key)
            {
                min = j;
            }
        }
        swap(arr[i], arr[min]);
    }
    auto stop = high_resolution_clock::now();
    auto dur = duration_cast<milliseconds>(stop - timer);
```
```
void QuickSortCore(vector<entry>&arr,int left,int right)
{
    
    // Start sort
    if (left < right)
    {
        entry *pivot = &arr[left];
        int i = left, j = right + 1;
        do
        {
            do
                i++;
            while (arr[i].key < pivot->key); // left push towards mid
            do
                j--;
            while (arr[j].key > pivot->key);
            if (i < j)
                swap(arr[i], arr[j]); // found both elements belonging to other side
        } while (i < j);
        swap(arr[left], arr[j]); // swap pivot with last element of left partition
        QuickSortCore(arr, left, j - 1);  // left partition
        QuickSortCore(arr, j + 1, right); // right partition
        }
}
```
```
vector<entry> MergeCut(vector<entry>arr) //cut array func
{
    if(arr.size() <= 1) { return arr; } // nothing to sort, return the input array

    vector<entry> left, right;
    int mid = arr.size() / 2; // get mid point
    for (int i = 0; i < mid; i++)
        left.push_back(arr[i]); // cut left part
    for (int i = mid; i < arr.size(); i++)
        right.push_back(arr[i]); // cut right part

    //start recursion
    left = MergeCut(left); // cut left and assign sorted left part
    right = MergeCut(right); // cut right and assign sorted right part

    return MergeCore(left, right); //run sorting and join sorted parts (bottom layer: 1+1=2)
}
```
```
vector<entry> MergeCore(vector<entry>& a, vector<entry>& b) // core sorting func, uses iterators
{
    vector<entry> c;
    c.reserve(a.size() + b.size()); // reserve memory

    auto it_a = a.begin();
    auto it_b = b.begin();

    while (it_a != a.end() && it_b != b.end()) // compare and push to merged array
    {
        if (it_a->key < it_b->key)
        {
            c.push_back(std::move(*it_a)); // move smaller key to merged array
            ++it_a;
        }
        else
        {
            c.push_back(std::move(*it_b));
            ++it_b;
        }
    }

    // move remaining elements from either array
    while (it_a != a.end())
    {
        c.push_back(std::move(*it_a));
        ++it_a;
    }
    while (it_b != b.end())
    {
        c.push_back(std::move(*it_b));
        ++it_b;
    }

    return c; // return merged array
}
```
## 效能分析

1. 時間複雜度：程式的時間複雜度為 $O(\log n)$。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $n = 0$      | 0        | 0        |
| 測試二   | $n = 1$      | 1        | 1        |
| 測試三   | $n = 3$      | 6        | 6        |
| 測試四   | $n = 5$      | 15       | 15       |
| 測試五   | $n = -1$     | 異常拋出 | 異常拋出 |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o sigma sigma.cpp
$ ./sigma
6
```

### 結論

1. 程式能正確計算 $n$ 到 $1$ 的連加總和。  
2. 在 $n < 0$ 的情況下，程式會成功拋出異常，符合設計預期。  
3. 測試案例涵蓋了多種邊界情況（$n = 0$、$n = 1$、$n > 1$、$n < 0$），驗證程式的正確性。

## 申論及開發報告

### 選擇遞迴的原因

在本程式中，使用遞迴來計算連加總和的主要原因如下：

1. **程式邏輯簡單直觀**  
   遞迴的寫法能夠清楚表達「將問題拆解為更小的子問題」的核心概念。  
   例如，計算 $\Sigma(n)$ 的過程可分解為：  

   $$
   \Sigma(n) = n + \Sigma(n-1)
   $$

   當 $n$ 等於 1 或 0 時，直接返回結果，結束遞迴。

2. **易於理解與實現**  
   遞迴的程式碼更接近數學公式的表示方式，特別適合新手學習遞迴的基本概念。  
   以本程式為例：  

   ```cpp
   int sigma(int n) {
       if (n < 0)
           throw "n < 0";
       else if (n <= 1)
           return n;
       return n + sigma(n - 1);
   }
   ```

3. **遞迴的語意清楚**  
   在程式中，每次遞迴呼叫都代表一個「子問題的解」，而最終遞迴的返回結果會逐層相加，完成整體問題的求解。  
   這種設計簡化了邏輯，不需要額外變數來維護中間狀態。

透過遞迴實作 Sigma 計算，程式邏輯簡單且易於理解，特別適合展示遞迴的核心思想。然而，遞迴會因堆疊深度受到限制，當 $n$ 值過大時，應考慮使用迭代版本來避免 Stack Overflow 問題。
