namespace myString {
    vector<string> split(const string& s, char sep) {
        vector<string> result;
        string sub;
        istringstream subStream(s);
        while (getline(subStream, sub, sep)) {
            result.emplace_back(sub);
        }
        return result;
    }
} // namespace myString
using namespace myString;
