template <typename T>
class Fenwick {
private:
    vector<T> f;
    T Op(T a, T b) { return a + b; } // add, max

public:
    explicit Fenwick(int n) : f(n + 1, 0) {}
    explicit Fenwick(vector<T>& vec) : f(vec.size() + 1) {
        for (int i = 0; i < vec.size(); ++i) {
            add(i + 1, vec[i]);
        }
    }

    void add(int id, T val) { // a[i] += val, i in [1, n]
        for (int i = id; i <= f.size(); i += i & -i) {
            f[i] = Op(f[i], val);
        }
    }

    T query(int id) { // [1, id]
        T res = 0;
        for (int i = id; i > 0; i &= i - 1) { // i -= i & -i
            res += f[i];
        }
        return res;
    }

    T rangeQuery(int l, int r) { return query(r) - query(l - 1); } // [l, r]
};

// 差分树状数组
// 利用差分数组，实现 O(log n) 的区间加、区间查询
// a[1] = diff[1]
// a[2] = diff[1] + diff[2]
// a[m] = diff[1] + ... + diff[m]
// 所以   a[1] + ... + a[m]
//     = ∑((m-i+1)* diff[i])
//     = (m+1)∑diff[i] - ∑(i* diff[i])
// [0] 维护 ∑ diff[i]
// [1] 维护 ∑ i*diff[i]
class FenwickDiff {
public:
    vector<vector<int>> fd;

public:
    explicit FenwickDiff(int n) : fd(n + 1, vector<int>(2)) {}
    explicit FenwickDiff(vector<int>& vec) : fd(vec.size() + 1, vector<int>(2)) {
        for (int i = 0; i < vec.size(); ++i) {
            add(i + 1, vec[i]);
        }
    }

    void add(int id, int val) {
        for (int i = id, iv = i * val; i < fd.size(); i += i & -i) {
            fd[i][0] += val;
            fd[i][1] += iv;
        }
    }

    void add(int l, int r, int val) {
        add(l, val);
        add(r + 1, -val);
    }

    int preSum(int id) {
        int s0 = 0, s1 = 0;
        for (int i = id; i > 0; i &= i - 1) {
            s0 += fd[i][0];
            s1 += fd[i][1];
        }
        return (id + 1) * s0 - s1;
    }

    int rangeSum(int l, int r) { return preSum(r) - preSum(l - 1); }
};
