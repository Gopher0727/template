template <typename T>
struct Basis {
    static_assert(is_same_v<T, int> || is_same_v<T, long long>, "T must be int or long long");

    static constexpr int B = is_same_v<T, int> ? 31 : 63;

    array<T, B> basis {};

    // “插入失败”表明原数集（至少选择一个数字）能异或出 0
    bool insert(T v) {
        for (int i = B - 1; i >= 0; i--) {
            if (v >> i & 1) {
                if (basis[i] == 0) {
                    basis[i] = v;
                    return true;
                }
                v ^= basis[i];
            }
        }
        return false;
    }

    // 求解最大异或和
    T query() {
        T ans {};
        for (int i = B - 1; i >= 0; i--) {
            ans = max(ans, ans ^ basis[i]);
        }
        return ans;
    }
};
