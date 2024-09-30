vector memo(m, vector<int>(1 << 10, -1));
auto dfs = [&](auto&& dfs, int i, int mask, bool isLimit, bool isNum) -> int {
    if (i == m) {
        return isNum;
    }

    if (!isLimit && isNum && memo[i][mask] != -1) {
        return memo[i][mask];
    }

    int res = 0;
    if (!isNum) { // 可以跳过当前位
        res = dfs(dfs, i + 1, mask, false, false);
    }

    int up = isLimit ? s[i] - '0' : 9;
    for (int k = isNum ? 0 : 1; k <= up; ++k) {
        if (mask >> k & 1) {
            continue;
        }
        res += dfs(dfs, i + 1, mask | (1 << k), isLimit && k == up, true);
    }

    if (!isLimit && isNum) {
        memo[i][mask] = res;
    }
    return res;
};

return dfs(dfs, 0, 0, true, false);
