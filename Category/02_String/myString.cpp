// 单字符分割字符串
//
auto split(const string& s, char sep) {
    vector<string> res;
    string sub;
    istringstream subStream(s);
    while (getline(subStream, sub, sep)) {
        res.emplace_back(sub);
    }
    return res;
}


// O(n^2) 求子串是否回文
// 一般用于 DP 预处理
//
auto isPalindrome(const string& s) {
    int n = s.size();
    vector isPal(n, vector<int>(n));
    for (int l = n - 1; l >= 0; --l) {
        for (int r = l; r < n; ++r) {
            isPal[l][r] = (s[l] == s[r] && (r - l < 3 || isPal[l + 1][r - 1]));
        }
    }
    return isPal;
}
