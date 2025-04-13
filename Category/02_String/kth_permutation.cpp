// 饱和：一旦大于 k，则返回一个比 k 大的数
__int128 Binom(int n, int r, long long k) {
    __int128 ans = 1;
    if (r > n - r) {
        r = n - r;
    }
    for (int i = 1; i <= r; i++) {
        ans = ans * (n - i + 1);
        ans /= i;
        if (ans > k) {
            return k + 1;
        }
    }
    return ans;
}

__int128 multinomial(const vector<int>& freq, int total, long long k) {
    __int128 ans = 1;
    for (int cnt : freq) {
        if (cnt > 0) {
            __int128 ways = Binom(total, cnt, k);
            ans *= ways;
            if (ans > k) {
                return k + 1;
            }
            total -= cnt;
        }
    }
    return ans;
}

// 求第 k 小排列，s 为字典序最小的字符串，k 从 1 开始计数
string kth_permutation(const string& s, i64 k) {
    int n = s.size();

    vector<int> freq(26, 0);
    for (char ch : s) {
        freq[ch - 'a']++;
    }

    __int128 total = multinomial(freq, n, k);
    if (k > total) {
        return "";
    }

    string ans;
    for (int pos = 0; pos < n; ++pos) {
        for (int c = 0; c < 26; ++c) {
            if (freq[c] == 0) {
                continue;
            }
            freq[c]--;

            i64 perms = multinomial(freq, n - pos - 1, k);
            if (k > perms) {
                k -= perms;
                freq[c]++;
            } else {
                ans.push_back('a' + c);
                break;
            }
        }
    }
    return ans;
}
