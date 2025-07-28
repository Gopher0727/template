// [LogTrick 入门教程](https://zhuanlan.zhihu.com/p/1933215367158830792)

// 给定一个数组 a，计算所有子数组的按位或
// O(n^2) -> O(nlogU)
void get(const vector<int>& a) {
    int n = a.size();
    for (int i = 0; i < n; i++) {
        // 内层循环，如果 (a[j] | a[i]) == a[j]，后续的 a[j] 就没必要更新了
        for (int j = i - 1; j >= 0 && (a[j] | a[i]) != a[j]; j--) {
            a[j] |= a[i];
        }
    }
    // 此时，a[j] 保存的是 [j..i] 子数组的按位或
}
