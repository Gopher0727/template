// KD-Tree 是一种高效处理 k 维空间信息的数据结构，维护 k 维空间 n 个点的信息，是一棵平衡二叉树。
// 树上的每个节点对应 k 维空间的一个点。
// 在结点数 n 远大于 2^k 时，应用 k-D Tree 的时间效率很好。

// 交替建树：
// 1. 选择切割维度：交替选择垂直 x 轴和垂直 y 轴方向切割平面。
// 2. 选择切割点：每次选择该维度上的中位数作为切割点。
// 3. 递归切割平面子区域，直到区域内没有点。

// 主要用于多维空间关键数据的搜索：
// 1. 最临近搜索：找出在树中与目标点最接近的点。
// 2. 范围搜索：找出所有位于给定区域中的点。

// 参考链接：
// https://www.luogu.com/article/cod5s33o
// https://www.geeksforgeeks.org/search-and-insertion-in-k-dimensional-tree/
//

static constexpr int k = 2;

// A structure to represent node of kd tree
struct Node {
    int point[k]; // To store k dimensional point
    Node *left, *right;
};

// A method to create a node of K D tree
struct Node* newNode(int arr[]) {
    struct Node* temp = new Node();
    for (int i = 0; i < k; i++) {
        temp->point[i] = arr[i];
    }
    temp->left = temp->right = nullptr;
    return temp;
}

// Inserts a new node and returns root of modified tree
// The parameter depth is used to decide axis of comparison
Node* insertRec(Node* root, int point[], unsigned depth) {
    if (root == nullptr) {
        return newNode(point);
    }
    // Calculate current dimension (cd) of comparison
    unsigned cd = depth % k;
    // Compare the new point with root on current dimension 'cd'
    // and decide the left or right subtree
    if (point[cd] < (root->point[cd])) {
        root->left = insertRec(root->left, point, depth + 1);
    } else {
        root->right = insertRec(root->right, point, depth + 1);
    }
    return root;
}

// Function to insert a new point with given point in KD Tree and return new root.
// It mainly uses above recursive function "insertRec()"
Node* insert(Node* root, int point[]) {
    return insertRec(root, point, 0);
}

// A utility method to determine if two Points are same in K Dimensional space
bool arePointsSame(int point1[], int point2[]) {
    // Compare individual pointinate values
    for (int i = 0; i < k; ++i) {
        if (point1[i] != point2[i]) {
            return false;
        }
    }
    return true;
}

// Searches a Point represented by "point[]" in the K D tree.
// The parameter depth is used to determine current axis.
bool searchRec(Node* root, int point[], unsigned depth) {
    if (root == nullptr) {
        return false;
    }
    if (arePointsSame(root->point, point)) {
        return true;
    }

    // Current dimension is computed using current depth and total dimensions (k)
    unsigned cd = depth % k;
    // Compare point with root with respect to cd (Current dimension)
    if (point[cd] < root->point[cd]) {
        return searchRec(root->left, point, depth + 1);
    }
    return searchRec(root->right, point, depth + 1);
}

// Searches a Point in the K D tree. It mainly uses searchRec()
bool search(Node* root, int point[]) {
    return searchRec(root, point, 0); // Pass current depth as 0
}

// Driver program to test above functions
int main() {
    struct Node* root = nullptr;

    int points[][k] = {
        {3,  6 },
        {17, 15},
        {13, 15},
        {6,  12},
        {9,  1 },
        {2,  7 },
        {10, 19}
    };

    int n = sizeof(points) / sizeof(points[0]);
    for (int i = 0; i < n; i++) {
        root = insert(root, points[i]);
    }

    auto find = [&](this auto&& find, int point[]) {
        if (search(root, point)) {
            cout << "Found\n";
        } else {
            cout << "Not Found\n";
        }
    };

    int point1[] = {10, 19};
    find(point1);

    int point2[] = {12, 19};
    find(point2);

    return 0;
}



// 封装实现
//
struct KDT {
    constexpr static int N = 1e5 + 10, K = 2;
    double alpha = 0.725;
    struct node {
        int info[K];
        int mn[K], mx[K];
    };
    vector<node> tr;
    vector<int> ls, rs, siz, id, d;
    int idx, rt, cur;
    int ans;
    KDT() {
        tr.resize(N);
        ls.resize(N);
        rs.resize(N);
        siz.resize(N);
        id.resize(N);
        d.resize(N);
        rt = 0;
        cur = 0;
    }
    void apply(int p, int son) {
        if (son) {
            for (int i = 0; i < K; i++) {
                tr[p].mn[i] = min(tr[p].mn[i], tr[son].mn[i]);
                tr[p].mx[i] = max(tr[p].mx[i], tr[son].mx[i]);
            }
            siz[p] += siz[son];
        }
    }
    void maintain(int p) {
        for (int i = 0; i < K; i++) {
            tr[p].mn[i] = tr[p].info[i];
            tr[p].mx[i] = tr[p].info[i];
        }
        siz[p] = 1;
        apply(p, ls[p]);
        apply(p, rs[p]);
    }
    int build(int l, int r) {
        if (l > r) return 0;
        vector<double> avg(K);
        for (int i = 0; i < K; i++) {
            for (int j = l; j <= r; j++) {
                avg[i] += tr[id[j]].info[i];
            }
            avg[i] /= (r - l + 1);
        }
        vector<double> var(K);
        for (int i = 0; i < K; i++) {
            for (int j = l; j <= r; j++) {
                var[i] += (tr[id[j]].info[i] - avg[i]) * (tr[id[j]].info[i] - avg[i]);
            }
        }
        int mid = (l + r) / 2;
        int x = max_element(var.begin(), var.end()) - var.begin();
        nth_element(id.begin() + l, id.begin() + mid, id.begin() + r + 1, [&](int a, int b) { return tr[a].info[x] < tr[b].info[x]; });
        d[id[mid]] = x;
        ls[id[mid]] = build(l, mid - 1);
        rs[id[mid]] = build(mid + 1, r);
        maintain(id[mid]);
        return id[mid];
    }
    void print(int p) {
        if (!p) return;
        print(ls[p]);
        id[++idx] = p;
        print(rs[p]);
    }
    void rebuild(int& p) {
        idx = 0;
        print(p);
        p = build(1, idx);
    }
    bool bad(int p) { return alpha * siz[p] <= max(siz[ls[p]], siz[rs[p]]); }
    void insert(int& p, int cur) {
        if (!p) {
            p = cur;
            maintain(p);
            return;
        }
        if (tr[p].info[d[p]] > tr[cur].info[d[p]]) {
            insert(ls[p], cur);
        } else {
            insert(rs[p], cur);
        }
        maintain(p);
        if (bad(p)) {
            rebuild(p);
        }
    }
    void insert(vector<int>& a) {
        cur++;
        for (int i = 0; i < K; i++) {
            tr[cur].info[i] = a[i];
        }
        insert(rt, cur);
    }
    bool out(int p, vector<int>& a) {
        for (int i = 0; i < K; i++) {
            if (a[i] < tr[p].mn[i]) {
                return true;
            }
        }
        return false;
    }
    bool in(int p, vector<int>& a) {
        for (int i = 0; i < K; i++) {
            if (a[i] < tr[p].info[i]) {
                return false;
            }
        }
        return true;
    }
    bool all(int p, vector<int>& a) {
        for (int i = 0; i < K; i++) {
            if (a[i] < tr[p].mx[i]) {
                return false;
            }
        }
        return true;
    }
    void query(int p, vector<int>& a) {
        if (!p) return;
        if (out(p, a)) return;
        if (all(p, a)) {
            ans += siz[p];
            return;
        }
        if (in(p, a)) ans++;
        query(ls[p], a);
        query(rs[p], a);
    }
    int query(vector<int>& a) {
        ans = 0;
        query(rt, a);
        return ans;
    }
};
