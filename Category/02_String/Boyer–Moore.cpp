// Boyer–Moore

// KMP 算法将前缀匹配的信息用到了极致，而 BM 算法基本思想是通过后缀匹配获得比前缀匹配更多的信息来实现更快的字符跳转。
// 通过使用坏字符规则和好后缀规则来快速跳过不可能匹配的位置。

// 构建坏字符偏移表（bad character shift table）
// 构建好后缀偏移表（good suffix shift table）
// 查找所有模式出现位置


// Todo
class BoyerMoore {
private:
    string pattern;
    vector<int> badCharTable; // 坏字符偏移表
    vector<int> goodSuffixShift; // 好后缀偏移表

    void buildBadCharTable() {
        for (int i = 0; i < pattern.length(); ++i) {
            badCharTable[pattern[i]] = static_cast<int>(i);
        }
    }

    void buildGoodSuffixTable() {
        int m = pattern.length();
        goodSuffixShift.resize(m + 1, m); // 默认值为模式长度

        // 计算前缀函数
        vector<int> prefix(m + 1, 0);
        for (int i = m - 1, j = -1; i >= 0; --i) {
            while (j >= 0 && pattern[i] != pattern[j]) {
                j = prefix[j];
            }
            prefix[i] = ++j;
        }

        // 计算好后缀偏移
        for (int i = 0; i <= m; ++i) {
            if (int suffixLength = m - prefix[i]; suffixLength > 0) {
                goodSuffixShift[suffixLength] = m - i;
            }
        }
        // 处理重复的好后缀
        for (int i = m - 1; i >= 0; --i) {
            goodSuffixShift[i] = min(goodSuffixShift[i], goodSuffixShift[i + 1]);
        }
    }

public:
    explicit BoyerMoore(const string& pattern) : pattern(pattern), badCharTable(256, -1) {
        buildBadCharTable();
        buildGoodSuffixTable();
    }

    // 查找所有模式出现的位置
    vector<int> findAll(const string& text) const {
        vector<int> occurrences;
        int n = text.length();
        int m = pattern.length();
        if (m == 0 || n < m) {
            return occurrences;
        }

        int s = 0; // 模式在文本中的起始位置
        while (s <= n - m) {
            // 开始从右向左比较
            int j = m - 1;
            while (j >= 0 && pattern[j] == text[s + j]) {
                j--;
            }
            if (j < 0) { // 找到匹配
                occurrences.push_back(s);
                s += goodSuffixShift[0];
            } else { // 使用坏字符规则或好后缀规则移动
                int badCharSkip = j - badCharTable[static_cast<unsigned char>(text[s + j])];
                int goodSuffixSkip = goodSuffixShift[j];
                s += max(badCharSkip, goodSuffixSkip);
            }
        }
        return occurrences;
    }
};

// 测试样例
int main() {
    string text = "HERE IS A SIMPLE EXAMPLE";
    string pattern = "EXAMPLE";

    BoyerMoore bm(pattern);
    vector<int> occurrences = bm.findAll(text);

    cout << "Pattern found at positions: ";
    for (int pos : occurrences) {
        cout << pos << " ";
    }
    cout << endl;

    return 0;
}
