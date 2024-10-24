// Lyndon 分解
//
// Lyndon 串：s 的字典序严格小于 s 的所有后缀的字典序，即 s 的字典序严格小于 s 的所有非平凡的循环同构串。
// [Note] 非平凡：非空且不同于自身。
//
// https://www.luogu.com.cn/article/lt2rnl6d
// https://oi-wiki.org/string/lyndon/

vector<string> duval(const string& s) {
    int n = s.size();

    vector<string> res;
    int i = 0;
    while (i < n) {
        int j = i + 1; // s3 首字符
        int k = i; // 当前字符
        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j]) {
                k = i;
            } else {
                k++;
            }
            j++;
        }
        while (i <= k) {
            res.emplace_back(s.substr(i, j - k));
            i += j - k;
        }
    }
    return res;
}


// 最小表示法
template <typename T>
int minimalRepresentation(const T& s) {
    int n = s.size();
    int i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        if (s[(i + k) % n] == s[(j + k) % n]) {
            k++;
        } else {
            if (s[(i + k) % n] > s[(j + k) % n]) {
                i += k + 1;
            } else {
                j += k + 1;
            }
            k = 0;
            if (i == j) {
                j++;
            }
        }
    }
    return min(i, j);
}

