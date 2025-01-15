// 线性基  Linear basis
//
// （异或空间的极大线性无关子集）
// 可以用来解决「子序列异或和」相关问题
//
class xorBasis {
private:
    vector<int> b;

    int num, or_;
    bool canBeZero; // minXor, kthXor
    vector<int> basis; // initOnce
    vector<int> rightMost;

    bool insert(int v) {
        or_ |= v;
        for (int i = b.size() - 1; i >= 0; --i) {
            if ((v >> i & 1) == 0) {
                continue;
            }
            if (b[i] == 0) {
                b[i] = v;
                num++;
                return true;
            }
            v ^= b[i];
        }
        canBeZero = true; // 没有找到线性基，但说明了可以选择一些数使得异或和为 0
        return false;
    }

    bool insertRightMost(int idx, int v) { // 遇到线性相关的基，保留位置最靠右的
        for (int i = b.size() - 1; i >= 0; --i) {
            if ((v >> i & 1) == 0) {
                continue;
            }
            if (b[i] == 0) {
                b[i] = v;
                rightMost[i] = idx;
                num++;
                return true;
            }
            if (idx >= rightMost[i]) {
                swap(idx, rightMost[i]);
                swap(v, b[i]);
            }
            v ^= b[i];
        }
        canBeZero = true; // 没有找到线性基，但说明了可以选择一些数使得异或和为 0
        return false;
    }

    void initOnce() {
        if (basis.empty()) {
            return;
        }
        vector<int> tmp(b);
        for (int i = 0; i < tmp.size(); ++i) {
            if (tmp[i] == 0) {
                continue;
            }
            for (int j = i - 1; j >= 0; --j) {
                if (tmp[i] >> j & 1) {
                    tmp[i] ^= tmp[j];
                }
            }
            basis[i] = tmp[i];
        }
    }

    void merge(xorBasis& other) {
        for (int i = other.b.size() - 1; i >= 0; --i) {
            int x = other.b[i];
            if (x > 0) {
                insert(x);
            }
        }
    }

public:
    xorBasis() = default;
    xorBasis(vector<int>& a) : b(64), rightMost(b.size()) {
        for (int& v : a) {
            insert(v);
        }
    }

    int maxXor(int val = 0) {
        int res = 0;
        for (int i = b.size() - 1; i >= 0; --i) {
            // (res >> i & 1) == 0 && rightMost[i] >= lowerIndex && res ^ b[i] > res
            if ((res ^ b[i]) > res) {
                res ^= b[i];
            }
        }
        return res;
    }

    int minXor() {
        if (canBeZero) {
            return 0;
        }
        for (int i = 0;; ++i) {
            if (b[i] > 0) {
                return b[i];
            }
        }
    }

    // 从 1 开始，第 k 小值（不允许空）
    int kthXor(int k) {
        initOnce();
        if (canBeZero) {
            k--;
        }
        if (k >= (1 << basis.size())) {
            return -1;
        }

        int res = 0;
        for (int i = 0; i < basis.size(); ++i) {
            if (k >> i & 1) {
                res ^= basis[i];
            }
        }
        return res;
    }

    // v 能否由线性基表出
    bool decompose(int v) {
        for (int i = b.size() - 1; i >= 0; --i) {
            if ((v >> i & 1) == 0) {
                continue;
            }
            if (b[i] == 0) {
                return false;
            }
            v ^= b[i];
        }
        return true;
    }
};

