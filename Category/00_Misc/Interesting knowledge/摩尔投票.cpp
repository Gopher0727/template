// 摩尔投票算法（Boyer–Moore majority vote algorithm）
//
// 用来寻找一组元素中占多数元素的常数空间复杂度、线性时间复杂度算法
//
// 【Note】摩尔投票可以并发执行

int f(const vector<int>& a) {
    int cand = -1;
    int cnt = 0;
    for (int v : a) {
        if (cand == -1) {
            v = cand;
            cnt++;
            continue;
        }
        if (v == cand) {
            cnt++;
        } else {
            if (--cnt == 0) {
                cand = -1;
            }
        }
    }
    if (cand == -1) {
        return -1;
    }
    return cand;
}
