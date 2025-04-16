auto groupby(const string& s) {
    vector<pair<char, int>> ans {{}};
    for (int i = 0; i < s.size(); i++) {
        if (i == 0 || s[i] == s[i - 1]) {
            ans.back().first = s[i];
            ans.back().second++;
        } else {
            ans.push_back({s[i], 1});
        }
    }
    return ans;
}
