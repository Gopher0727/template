// 最多交换一次字符能获得的字典序最小的字符串    https://codeforces.com/contest/1281/problem/B
//
auto get(string s) {
    int n = s.size();
    vector<int> a(n);
    a[n - 1] = n - 1;
    for (int i = n - 2; i >= 0; --i) {
        if (s[i] >= s[a[i + 1]]) {
            a[i] = a[i + 1];
        } else {
            a[i] = i;
        }
    }
    for (int i = 0; i < n; ++i) {
        if (s[i] != s[a[i]]) {
            swap(s[i], s[a[i]]);
            break;
        }
    }
    return s;
}


// 反悔贪心
//
// https://atcoder.jp/contests/aising2020/tasks/aising2020_e
