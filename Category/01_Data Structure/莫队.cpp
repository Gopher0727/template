// 带修莫队
//
// 带修改的莫队算法将“时间”这一维度引入，既要处理区间边界的调整，也要处理更新操作对答案的影响。

void solve() {
    int n, m;
    cin >> n >> m;

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    // 分离保存 Q 和 U
    //
    struct Q {
        int L, R, t, id; // t 表示此查询前完成的更新数量
    };
    vector<Q> queries;

    struct U {
        int pos, old_val, new_val;
    };
    vector<U> updates;

    // 保存副本并应用更新
    vector<int> na(a);
    int q = 0, t = 0;
    for (int i = 0; i < m; i++) {
        //
        if () { // Q
            int l, r;
            cin >> l >> r;
            l--, r--;
            queries.push_back({l, r, t, q++});
        } else {
            int p, c;
            cin >> p >> c;
            p--;
            updates.push_back({p, a[p], c});
            a[p] = c;
            t++;
        }
    }

    // 当前区间维护数据
    vector<int> freq(ranges::max(a) + 1);
    i64 cur = 0;

    // 根据具体问题设计 add 与 remove 函数
    // 如统计同色对：先 cur += freq[c], 再 freq[c]++
    auto add = [&](int pos) {
        int c = a[pos];
        cur += freq[c];
        freq[c]++;
    };
    auto remove = [&](int pos) {
        int c = a[pos];
        freq[c]--;
        cur -= freq[c];
    };

    // 应用更新操作 k（更新 k-th 更新操作，将其 new_val 应用到 pos 位置）
    auto apply_update = [&](int k, int curL, int curR) {
        int pos = updates[k].pos;
        if (curL <= pos && pos <= curR) {
            remove(pos);
        }
        a[pos] = updates[k].new_val;
        if (curL <= pos && pos <= curR) {
            add(pos);
        }
    };
    // 回退更新操作 k（将 pos 位置恢复到更新前的 old_val）
    auto rollback_update = [&](int k, int curL, int curR) {
        int pos = updates[k].pos;
        if (curL <= pos && pos <= curR) {
            remove(pos);
        }
        a[pos] = updates[k].old_val;
        if (curL <= pos && pos <= curR) {
            add(pos);
        }
    };

    // 初始化块大小，建议取 n^(2/3)，根据经验调整
    // 将查询排序
    int blockSize = pow(n, 2.0 / 3);
    sort(queries.begin(), queries.end(), [&](const Q& a, const Q& b) {
        int blockL_a = a.L / blockSize, blockL_b = b.L / blockSize;
        if (blockL_a != blockL_b) {
            return blockL_a < blockL_b;
        }
        int blockR_a = a.R / blockSize, blockR_b = b.R / blockSize;
        if (blockR_a != blockR_b) {
            return blockR_a < blockR_b;
        }
        return a.t < b.t;
    });

    // 初始化三个指针并恢复原数组
    int curL = 0, curR = -1, curT = 0;
    a = std::move(na);
    vector<i64> ans(queries.size());
    for (auto& [L, R, t, id] : queries) {
        while (curT < t) {
            apply_update(curT++, curL, curR);
        }
        while (curT > t) {
            rollback_update(--curT, curL, curR);
        }
        while (curR < R) {
            add(++curR);
        }
        while (curR > R) {
            remove(curR--);
        }
        while (curL < L) {
            remove(curL++);
        }
        while (curL > L) {
            add(--curL);
        }

        // 此时 cur 为区间 [L,R] 对应的答案
        ans[id] = cur;
    }

    // 输出每个查询答案
    for (auto v : ans) {
        //
    }
}
