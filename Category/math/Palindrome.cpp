// 按从小到大的顺序生成回文数（1 ~ 1e9 之间）
vector<int> palindrome;
auto init = [] {
    palindrome.emplace_back(0); // 哨兵

    for (int base = 1; base <= 10000; base *= 10) {
        // 生成奇数长度的回文数
        for (int i = base; i < base * 10; ++i) {
            int x = i;
            for (int t = i / 10; t; t /= 10) {
                x = x * 10 + t % 10;
            }
            palindrome.emplace_back(x);
        }
        // 生成偶数长度的回文数
        if (base <= 1000) {
            for (int i = base; i < base * 10; ++i) {
                int x = i;
                for (int t = i; t; t /= 10) {
                    x = x * 10 + t % 10;
                }
                palindrome.emplace_back(x);
            }
        }
    }
    palindrome.emplace_back(1e9 + 1); // 哨兵

    return 0;
}();
