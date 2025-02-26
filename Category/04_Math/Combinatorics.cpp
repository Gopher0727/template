// 杨辉三角（左对齐）
const int MX = 34;
vector<int> c[MX];
auto init = []() {
    for (int i = 0; i < MX; i++) {
        c[i].resize(i + 1, 1);
        for (int j = 1; j < i; j++) {
            c[i][j] = c[i - 1][j - 1] + c[i - 1][j];
        }
    }
    return 0;
}();


// 判断组合数奇偶性
//（也可以 Lucas，模数为 2）
bool check(i64 n, i64 i) {
    return (n & i) == i;
}
