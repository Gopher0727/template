// 对顶堆查询中位数
template <typename T = ll>
class DualHeap {
private:
    priority_queue<T> maxHeap; // 大根堆
    priority_queue<T, vector<T>, greater<T>> minHeap; // 小根堆

    void adjust() {
        while (maxHeap.size() > minHeap.size() + 1) {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
        }
        while (minHeap.size() > maxHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }

public:
    DualHeap() {}

    void addNum(T num) {
        if (maxHeap.empty() || num <= maxHeap.top()) {
            maxHeap.push(num);
        } else {
            minHeap.push(num);
        }
        // 调整堆的大小
        adjust();
    }

    void removeNum(T num) {
        vector<T> temp;
        if (maxHeap.size() > 0 && num <= maxHeap.top()) { // 从大根堆中移除
            while (!maxHeap.empty()) {
                if (maxHeap.top() == num) {
                    maxHeap.pop();
                    break;
                }
                temp.push_back(maxHeap.top());
                maxHeap.pop();
            }
            for (auto& x : temp) {
                maxHeap.push(x);
            }
        } else { // 从小根堆中移除
            while (!minHeap.empty()) {
                if (minHeap.top() == num) {
                    minHeap.pop();
                    break;
                }
                temp.push_back(minHeap.top());
                minHeap.pop();
            }
            for (auto& x : temp) {
                minHeap.push(x);
            }
        }
        // 重新平衡堆
        adjust();
    }

    template <typename U = double>
    U findMedian() {
        if (maxHeap.size() > minHeap.size()) {
            return maxHeap.top();
        } else {
            return (maxHeap.top() + minHeap.top()) / 2.0L;
        }
    }
};


// 定长滑窗中位数，with 懒删除
class DualHeap_slidingWindow {
private:
    priority_queue<ll> small; // 大根堆，维护较小的一半元素
    priority_queue<ll, vector<ll>, greater<ll>> large; // 小根堆，维护较大的一半元素
    unordered_map<ll, int> delayed; // 记录「延迟删除」的元素，key 为元素，value 为需要删除的次数
    int k;
    int smallSize, largeSize; // small 和 large 当前包含的元素个数，需要扣除被「延迟删除」的元素

private:
    template <typename Heap>
    void prune(Heap& heap) {
        while (!heap.empty()) {
            int num = heap.top();
            if (delayed.count(num)) {
                --delayed[num];
                if (!delayed[num]) {
                    delayed.erase(num);
                }
                heap.pop();
            } else {
                break;
            }
        }
    }

    void makeBalance() {
        if (smallSize > largeSize + 1) {
            // small 比 large 元素多 2 个
            large.push(small.top());
            small.pop();
            --smallSize;
            ++largeSize;
            prune(small);
        } else if (smallSize < largeSize) {
            // large 比 small 元素多 1 个
            small.push(large.top());
            large.pop();
            ++smallSize;
            --largeSize;
            prune(large);
        }
    }

    template <typename T>
    T getMedian() {
        return k & 1 ? small.top() : ((T) small.top() + large.top()) / 2;
    }

public:
    DualHeap_slidingWindow(int _k) : k(_k), smallSize(0), largeSize(0) {}

    void insert(int num) {
        if (small.empty() || num <= small.top()) {
            small.push(num);
            ++smallSize;
        } else {
            large.push(num);
            ++largeSize;
        }
        makeBalance();
    }

    void erase(int num) {
        ++delayed[num];
        if (num <= small.top()) {
            --smallSize;
            if (num == small.top()) {
                prune(small);
            }
        } else {
            --largeSize;
            if (num == large.top()) {
                prune(large);
            }
        }
        makeBalance();
    }

    // 大小为 k 的滑动窗口中位数
    template <typename T = double, typename U = int>
    vector<T> medianSlidingWindow(vector<U>& nums) {
        vector<T> medians;
        for (int i = 0; i < nums.size(); i++) {
            insert(nums[i]);
            if (i >= k - 1) {
                medians.push_back(getMedian<T>());
                erase(nums[i - k + 1]);
            }
        }
        return medians;
    }
};
