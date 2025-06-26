// 冒泡排序：每次检查相邻两个元素（可以用 flag 标记是否交换过元素）
void bubble_sort(vector<int>& a) {
    int n = a.size();
    if (n < 2) {
        return;
    }
    bool flag = true;
    while (flag) {
        flag = false;
        for (int i = 0; i < n - 1; i++) {
            if (a[i] > a[i + 1]) {
                flag = true;
                swap(a[i], a[i + 1]);
            }
        }
    }
}

// 选择排序：每次找出第 i 小的元素，与数组第 i 个位置上的元素交换
void selection_sort(vector<int>& a) {
    int n = a.size();
    if (n < 2) {
        return;
    }
    for (int i = 0; i < n - 1; i++) {
        int idx = i;
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[idx]) { // 稳定
                idx = j;
            }
        }
        if (idx != i) {
            swap(a[i], a[idx]);
        }
    }
}

// 插入排序：从后往前找，比当前值大的右移一位（将数组分成了有序和无序两部分）
void insertion_sort(vector<int>& a) {
    int n = a.size();
    if (n < 2) {
        return;
    }
    for (int i = 1; i < n; i++) {
        int v = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > v) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = v;
    }
}

// 折半插入排序  `rotate`
void half_insertion_sort(vector<int>& a) {
    for (auto i = a.begin(); i != a.end(); i++) {
        rotate(upper_bound(a.begin(), i, *i), i, i + 1);
    }
}

// 希尔排序
void shell_sort(vector<int>& a) {
    // Marcin Ciura's gap sequence
    vector<int> gaps = {701, 301, 132, 57, 23, 10, 4, 1};

    // Start with the largest gap and work down to a gap of 1
    for (int gap : gaps) {
        if (gap >= a.size()) {
            continue;
        }
        for (int i = gap; i < a.size(); i++) {
            int temp = a[i];
            int j = i;
            for (; j >= gap && a[j - gap] > temp; j -= gap) {
                a[j] = a[j - gap];
            }
            a[j] = temp;
        }
    }
}

// 快速排序：找基准，分治
int partition(vector<int>& a, int low, int high) {
    int pivot = a[high]; // 选择最后一个元素作为枢轴
    int i = low - 1; // 较小元素的索引
    for (int j = low; j < high; j++) {
        if (a[j] <= pivot) {
            swap(a[++i], a[j]);
        }
    }
    swap(a[++i], a[high]);
    return i;
}
void quick_sort(vector<int>& a, int low = 0, int high = -1) {
    if (high == -1) {
        high = (int) a.size() - 1;
    }
    if (low < high) {
        int pi = partition(a, low, high);
        quick_sort(a, low, pi - 1);
        quick_sort(a, pi + 1, high);
    }
}

// 桶排序
void bucket_sort(vector<int>& a) {
    vector<vector<int>> buckets(20); // 数据范围 0 ~ 199
    for (int v : a) {
        buckets[v / 10].push_back(v);
    }
    // 桶内部排序
    for (auto& bucket : buckets) {
        ranges::sort(bucket);
    }
    a.clear();
    for (auto& bucket : buckets) {
        for (int v : bucket) {
            a.push_back(v);
        }
    }
}

// 计数排序：需要一个额外空间存储待排序数组中相应元素的个数，还需要计算前缀和解决元素重复的情况
// 也可以给索引排序
void counting_sort(vector<int>& a) {
    int n = a.size(), w = ranges::max(a);
    vector<int> cnt(w + 1);
    vector<int> b(n);
    for (int v : a) {
        cnt[v]++;
    }
    for (int i = 1; i <= w; i++) {
        cnt[i] += cnt[i - 1];
    }
    for (int v : a | views::reverse) {
        b[--cnt[v]] = v;
    }
    a = std::move(b);
}

// 基数排序
// 计数排序辅助函数，用于基数排序
void counting_sort_for_radix(vector<int>& a, int exp) {
    const int N = 10;
    vector<int> cnt(N);
    for (int& v : a) {
        cnt[(v / exp) % 10]++;
    }
    for (int i = 1; i < N; i++) {
        cnt[i] += cnt[i - 1];
    }

    int n = a.size();
    vector<int> b(n);
    for (int i = n - 1; i >= 0; i--) {
        b[--cnt[(a[i] / exp) % 10]] = a[i];
    }
    a = std::move(b);
}
void radix_sort(vector<int>& a) {
    int max = ranges::max(a);
    for (int exp = 1; max / exp > 0; exp *= 10) {
        counting_sort_for_radix(a, exp);
    }
}

// 堆排序
void heapify(vector<int>& a, int n, int i) {
    int idx = i;
    if (int L = 2 * i + 1; L < n && a[L] > a[idx]) {
        idx = L;
    }
    if (int R = 2 * i + 2; R < n && a[R] > a[idx]) {
        idx = R;
    }
    if (idx != i) {
        swap(a[i], a[idx]);
        heapify(a, n, idx);
    }
}
void heap_sort(vector<int>& a) {
    int n = a.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(a, n, i);
    }
    for (int i = n - 1; i > 0; i--) {
        swap(a[0], a[i]);
        heapify(a, i, 0);
    }
}

// 测试验证
void solve() {
    mt19937 gen(chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());
    uniform_int_distribution<int> dis(1, 100);

    for (int i = 0; i < 10; i++) {
        vector<int> a(10);
        for (int& v : a) {
            v = dis(gen);
        }
        debug(a);
        counting_sort(a);
        debug(a);
        if (ranges::is_sorted(a)) {
            cout << "is_sorted: True\n";
        }
    }
}
