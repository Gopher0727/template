// 李超线段树
//
// 用来维护在平面直角坐标系上的线段关系
//
const int MX = 200005;
const int inf = 1 << 30;
int cur = 0;

struct segment {
    int l, r, lc, rc;
    double k, b;
} t[MX * 4];

void init() {
    cur = 0;
}

// 新建一棵线段树，并返回根结点的编号
int build(int L, int R) {
    int p = ++cur;
    t[p].l = L;
    t[p].r = R;
    if (L < R) {
        int mid = (L + R) >> 1;
        t[p].lc = build(L, mid);
        t[p].rc = build(mid + 1, R);
    }
    t[p].k = 0;
    t[p].b = -inf;
    return p;
}

void push(int p, double k, double b) {
    double l1 = k * t[p].l + b;
    double r1 = k * t[p].r + b;
    double l2 = t[p].k * t[p].l + t[p].b;
    double r2 = t[p].k * t[p].r + t[p].b;

    if (l1 >= l2 && r1 >= r2) {
        t[p].k = k, t[p].b = b;
    } else if (l2 < l1 || r2 < r1) {
        double pos = (b - t[p].b) / (t[p].k - k);
        int mid = (t[p].l + t[p].r) >> 1;
        if (pos <= mid) {
            if (r1 > r2) {
                swap(t[p].k, k);
                swap(t[p].b, b);
            }
            push(t[p].lc, k, b);
        } else {
            if (l1 > l2) {
                swap(t[p].k, k);
                swap(t[p].b, b);
            }
            push(t[p].rc, k, b);
        }
    }
}

// 在区间 [L, R] 中插入直线 y=kx+b
void insert(int p, int L, int R, double k, double b) {
    if (L <= t[p].l && R >= t[p].r) {
        push(p, k, b);
    } else {
        int mid = (t[p].l + t[p].r) >> 1;
        if (L <= mid) {
            insert(t[p].lc, L, R, k, b);
        }
        if (R > mid) {
            insert(t[p].rc, L, R, k, b);
        }
    }
}

// p 是线段树的根结点，x 是查询的横坐标，返回所有直线在 x 处的最大值
double query(int p, int x) {
    double ans = t[p].k * x + t[p].b;
    if (t[p].l < t[p].r) {
        int mid = (t[p].l + t[p].r) >> 1;
        if (x <= mid) {
            ans = max(ans, query(t[p].lc, x));
        } else {
            ans = max(ans, query(t[p].rc, x));
        }
    }
    return ans;
}

const double eps = 1e-9;
struct Seg {
    double k, b;
    int id;
    Seg() : k(), b(), id(1) {}
    Seg(double k, double b) : k(k), b(b) {}
} A[MX];

int main() {
    srand(time(0));
    int n = 200000, m = 300;
    for (int i = 1; i <= m; ++i) {
        A[i].k = rand() % 10 + 1;
        A[i].b = rand() % 1000 - 500;
        A[i].id = i;
    }

    init();

    int root = build(1, n);
    for (int i = 1; i <= m; ++i) {
        insert(root, 1, n, A[i].k, A[i].b);
    }

    for (int i = 1; i <= n; ++i) {
        i64 ans = query(root, i) + eps;
        i64 res = -inf;
        for (int j = 1; j <= m; ++j) {
            res = max(res, (i64) (A[j].k * i + A[j].b + eps));
        }
        if (res != ans) {
            cout << ans << " " << res << "\n";
        }
    }
    return 0;
}
