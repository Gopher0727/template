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
