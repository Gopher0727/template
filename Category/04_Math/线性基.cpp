// 线性基  Linear basis

template <typename T>
struct xorBasis {
    static_assert(is_same_v<T, int> || is_same_v<T, long long>, "T must be int or long long");
    static const int B = sizeof(T) * 8;

    array<T, B> b {};
    int cnt = 0;
    bool canBeZero = false;

    // "插入失败" 表明原数集（至少选择一个数字）能异或出 0
    bool insert(T x) {
        for (int i = B - 1; i >= 0; i--) {
            if (x >> i & 1) {
                if (b[i] == 0) {
                    b[i] = x;
                    cnt++;
                    return true;
                }
                x ^= b[i];
            }
        }
        canBeZero = true;
        return false;
    }

    T maxXor() {
        T ans {};
        for (int i = B - 1; i >= 0; i--) {
            ans = max(ans, ans ^ b[i]);
        }
        return ans;
    }

    T minXor() {
        if (!canBeZero) {
            for (int i = 0; i < B; i++) {
                if (b[i] > 0) {
                    return b[i];
                }
            }
        }
        return 0;
    }

    void merge(const xorBasis<T>& other) {
        for (int i = other.B - 1; i >= 0; i--) {
            if (other.b[i] > 0) {
                insert(other.b[i]);
            }
        }
    }

    // x 能否由线性基表出
    bool decompose(T x) {
        for (int i = B - 1; i >= 0; i--) {
            if (x >> i & 1) {
                if (b[i] == 0) {
                    return false;
                }
                x ^= b[i];
            }
        }
        return true;
    }
};
