// priority_queue 是`大顶堆`

// set 与 multiset 中的删除:
// multiset 的 extract 是去掉一个元素，元素不存在则无操作，返回值为 node_type。
// multiset 的 erase 的数值删除用法，会把相同的值都删掉，返回值为 size_type，返回一个指向被删除元素的下一个元素的迭代器。

// 可以利用 map 的 emplace 方法返回值

// 输出控制： fixed, setprecision, setw, setfill

// array 相当于普通数组，局部需要初始化； vector 嵌套 array 则不需要。
// 对于结构体的存储，vector 会进行初始化（int 为 0；string 为空；指针地址为 0，解引用无意义；array 也会进行初始化）

/* random */
mt19937_64 rnd{random_device{}()};
// or
mt19937_64 rd(chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());


/* pdbtree */
#include <bits/extc++.h>
// int
tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> t;
// pair<>
using PII = pair<int, int>;
__gnu_pbds::tree<PII, __gnu_pbds::null_type, less<PII>, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update> rbt;



#define functor(func) ([&](auto&&... val) noexcept(noexcept(func(std::forward<decltype(val)>(val)...))) -> decltype(auto) { return func(std::forward<decltype(val)>(val)...); })
#define expr(ret, ...) ([&](__VA_ARGS__) { return (ret); })

//---------------------------------------------------------------------------------------------------------------------------------------
// 凡是求方案数的问题一定要初始化。
//
// 对于相邻元素差的绝对值之和，考虑能否通过排序简化。



// 蛇形路径
//
vector<string> snakeWay(const vector<string>& g) {
    int n = g.size(), m = g[0].size();
    for (int r = 0; r * 2 <= min(n, m) - 1; ++r) {
        string t;
        for (int i = r + 1; i < m - r; ++i) {
            t.push_back(g[r][i]);
        }
        for (int i = r + 1; i < n - r; ++i) {
            t.push_back(g[i][m - 1 - r]);
        }
        for (int i = m - 2 - r; i >= r; --i) {
            t.push_back(g[n - 1 - r][i]);
        }
        for (int i = n - 2 - r; i >= r; --i) {
            t.push_back(g[i][r]);
        }
        cout << t << "\n";
    }
}

namespace Date {
    const int d[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    bool isLeap(int y) {
        return y % 400 == 0 || (y % 4 == 0 && y % 100 != 0);
    }
    int daysInMonth(int y, int m) {
        return d[m - 1] + (isLeap(y) && m == 2);
    }
    int getDay(int y, int m, int d) {
        int ans = 0;
        for (int i = 1970; i < y; i++) {
            ans += 365 + isLeap(i);
        }
        for (int i = 1; i < m; i++) {
            ans += daysInMonth(y, i);
        }
        ans += d;
        return (ans + 2) % 7 + 1;
    }
} // namespace Date
using namespace Date;


namespace Sqrt {
    // x * x <= n
    ll mSqrt(ll n) {
        ll x = sqrt(n);
        while (x * x > n) {
            x--;
        }
        while ((x + 1) * (x + 1) <= n) {
            x++;
        }
        return x;
    }
    // x * (x+1) / 2 <= x
    ll mGet(ll n) {
        ll x = sqrt(2.0L * n);
        while (x * (x + 1) / 2 < n) {
            x++;
        }
        while (x * (x - 1) / 2 + 1 > n) {
            x--;
        }
        return x;
    }
} // namespace Sqrt
using namespace Sqrt;


namespace Log {
    // a ^ t >= b
    int log_a(int a, int b) {
        int t = 0;
        ll v = 1;
        while (v < b) {
            v *= a;
            t++;
        }
        return t;
    }
    // >= __lg(t)
    int llog(int a, int b) {
        if (a <= b) {
            int l = log_a(a, b);
            return (l == 0 ? 0 : __lg(2 * l - 1));
        }
        int l = log_a(b, a + 1) - 1;
        assert(l > 0);
        return -__lg(l);
    }
} // namespace Log
using namespace Log;


namespace baseConvert {
    map<char, int> ch2i;
    map<int, char> i2ch;
    auto init = []() {
        for (char i = '0'; i <= '9'; ++i) {
            ch2i[i] = i - '0', i2ch[i - '0'] = i;
        }
        for (char i = 'A'; i <= 'F'; ++i) {
            ch2i[i] = i - 'A' + 10, i2ch[i - 'A' + 10] = i;
        }
        return 0;
    }();
}
