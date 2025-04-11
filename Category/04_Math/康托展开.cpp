// 康托展开（Cantor Expansion）
//
// 返回所给排列 perm（元素在 [1,n]）的字典序名次（可以从 0 或从 1 开始，具体看代码末尾）
// 核心思想：对于第 i 个位置，若该位置的数是未出现在其左侧的数中第 k 大的，那么有 (k−1)×(N−i)! 种方案在该位置上比这个排列小
//
auto rankPermutation(const vector<int>& prem) {
    int n = prem.size();
    vector<int> F(n);
    F[0] = 1;
    for (int i = 1; i < n; i++) {
        F[i] = F[i - 1] * i % MOD;
    }

    vector<int> tree(n + 1);
    auto add = [&](int i, int val) -> void {
        for (; i <= n; i += i & -i) {
            tree[i] += val;
        }
    };
    auto sum = [&](int i) -> int {
        int res = 0;
        for (; i > 0; i &= i - 1) {
            res += tree[i];
        }
        return res;
    };
    for (int i = 1; i <= n; i++) {
        add(i, 1);
    }

    int ans = 0;
    for (int i = 0; i < prem.size(); i++) {
        ans += sum(prem[i] - 1) * F[n - 1 - i] % MOD;
        add(prem[i], -1);
    }
    ans++; // 从 1 开始的排名
    ans %= MOD;
    return ans;
}


// 逆康托展开（Inverse Cantor Expansion）
//
// 返回字典序第 k 小的排列，元素范围为 [1,n]
//
auto kthPermutation(int n, int k) {
    vector<int> F(n);
    F[0] = 1;
    for (int i = 1; i < n; i++) {
        F[i] = F[i - 1] * i;
    }

    k--; // 如果输入从 1 开始
    vector<int> perm;
    perm.reserve(n);
    vector<int> valid(n + 1, 1);
    valid[0] = 0;
    for (int i = 1; i <= n; i++) {
        int order = k / F[n - i] + 1;
        for (int j = 1; j <= n; j++) { // 从 1 开始的排列
            order -= valid[j];
            if (order == 0) {
                perm.push_back(j);
                valid[j] = 0;
                break;
            }
        }
        k %= F[n - i];
    }
    return perm;
}
