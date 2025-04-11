// 李超线段树 （LiChao）
//
// 用来维护在平面直角坐标系上的线段关系，可以用于最大化 f(x)

static constexpr i64 inf = 1E18;

class LiChao {
    struct Line { // 直线 f(x) = k * x + b
        i64 k, b;
    };

    struct Node {
        Line line;
        Node *l = nullptr, *r = nullptr;
        Node(Line line) : line(line) {}
    }* root = nullptr;

    i64 L, R; // 查询x的范围

    // 评估直线
    i64 eval(const Line& line, i64 x) const { return line.k * x + line.b; }

    void addLine(Line newline, Node*& node, i64 l, i64 r) {
        if (node == nullptr) {
            node = new Node(newline);
            return;
        }
        i64 mid = (l + r) >> 1;
        bool left_better = eval(newline, l) > eval(node->line, l);
        bool mid_better = eval(newline, mid) > eval(node->line, mid);
        if (mid_better) {
            swap(newline, node->line);
        }
        if (l + 1 == r) {
            return;
        }
        if (left_better != mid_better) {
            addLine(newline, node->l, l, mid);
        } else {
            addLine(newline, node->r, mid, r);
        }
    }

    i64 query(Node* node, i64 l, i64 r, i64 x) const {
        if (node == nullptr) {
            return -inf;
        }
        i64 res = eval(node->line, x);
        if (l + 1 == r) {
            return res;
        }
        i64 mid = (l + r) >> 1;
        if (x < mid) {
            res = max(res, query(node->l, l, mid, x));
        } else {
            res = max(res, query(node->r, mid, r, x));
        }
        return res;
    }

public:
    LiChao(i64 L, i64 R) : L(L), R(R) {}

    void addLine(i64 k, i64 b) { addLine({k, b}, root, L, R); }

    i64 query(i64 x) const { return query(root, L, R, x); }
};
