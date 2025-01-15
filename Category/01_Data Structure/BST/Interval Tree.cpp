// 区间树，几何数据结构，常用于窗口查询
//
// 区间树是一种用于组织和搜索区间（即值的范围）的数据结构。以下是可以在区间树上执行的一些操作：
//
// > 插入：向树中添加新的间隔。
// > 删除：从树中删除一个间隔。
// > 搜索：查找与给定间隔重叠的所有间隔。
// > 查询：查找树中包含给定点的区间。
// > 范围查询：查找与给定范围重叠的所有区间。
// > 合并：将两个或多个区间树合并为一棵树。
// > 分裂：根据给定的间隔将一棵树分成两棵或多棵较小的树。
// > 平衡：保持树的平衡，以确保其性能达到优化。
// > 遍历：按特定顺序访问树中的所有间隔，例如中序、前序或后序。
//
// 除了这些基本操作外，区间树还可以扩展以支持更高级的操作，例如搜索具有特定长度的区间、查找距离给定点最近的区间等等。


// Structure to represent an interval
struct Interval {
    int low, high;
};

// Structure to represent a node in Interval Search Tree
struct ITNode {
    Interval* i; // 'i' could also be a normal variable
    int max;
    ITNode *left, *right;

    ITNode(Interval i) {
        this.i = new Interval(i.low, i.high);
        this.max = i.high;
        this.left = this.right = null;
    }
};

// A utility function to create a new Interval Search Tree Node
ITNode* newNode(Interval i) {
    ITNode* temp = new ITNode;
    temp->i = new Interval(i);
    temp->max = i.high;
    temp->left = temp->right = nullptr;
    return temp;
}

// A utility function to insert a new Interval Search Tree Node
// This is similar to BST Insert.  Here the low value of interval
// is used tomaintain BST property
ITNode* insert(ITNode* root, Interval i) {
    // Base case: Tree is empty, new node becomes root
    if (root == nullptr) {
        return newNode(i);
    }
    // Get low value of interval at root
    int l = root->i->low;
    // If root's low value is smaller, then new interval goes to left subtree
    if (i.low < l) {
        root->left = insert(root->left, i);
    } else { // Else, new node goes to right subtree.
        root->right = insert(root->right, i);
    }
    // Update the max value of this ancestor if needed
    if (root->max < i.high) {
        root->max = i.high;
    }
    return root;
}

// A utility function to check if given two intervals overlap
bool doOverlap(Interval i1, Interval i2) {
    return (i1.low <= i2.high && i2.low <= i1.high);
}

// The main function that searches a given interval i in a given
// Interval Tree.
Interval* overlapSearch(ITNode* root, Interval i) {
    // Base Case, tree is empty
    if (root == nullptr) {
        return nullptr;
    }
    // If given interval overlaps with root
    if (doOverlap(*(root->i), i)) {
        return root->i;
    }
    // If left child of root is present and max of left child is
    // greater than or equal to given interval, then i may
    // overlap with an interval is left subtree
    if (root->left != nullptr && root->left->max >= i.low) {
        return overlapSearch(root->left, i);
    }
    // Else interval can only overlap with right subtree
    return overlapSearch(root->right, i);
}

void inorder(ITNode* root) {
    if (root == nullptr) {
        return;
    }
    inorder(root->left);
    cout << "[" << root->i->low << ", " << root->i->high << "]"
         << " max = " << root->max << endl;
    inorder(root->right);
}

// Driver program to test above functions
int main() {
    // Let us create interval tree shown in above figure
    Interval ints[] = {
        {15, 20},
        {10, 30},
        {17, 19},
        {5,  20},
        {12, 15},
        {5,  40}
    };

    int n = sizeof(ints) / sizeof(ints[0]);
    ITNode* root = nullptr;
    for (int i = 0; i < n; i++) {
        root = insert(root, ints[i]);
    }

    cout << "Inorder traversal of constructed Interval Tree is\n";
    inorder(root);

    Interval x = {6, 7};

    cout << "\nSearching for interval [" << x.low << "," << x.high << "]";
    Interval* res = overlapSearch(root, x);
    if (res == nullptr) {
        cout << "\nNo Overlapping Interval";
    } else {
        cout << "\nOverlaps with [" << res->low << ", " << res->high << "]";
    }

    return 0;
}


// 一个简单的类实现
class IntervalTree {
public:
    ITNode* root;

    IntervalTree() { this->root = nullptr; }

    ITNode* insert(ITNode* root, Interval i) {
        if (root == nullptr) {
            return new ITNode(i);
        }
        int l = root->i->low;
        if (i.low < l) {
            root->left = insert(root->left, i);
        } else {
            root->right = insert(root->right, i);
        }
        root->max = std::max(root->max, i.high);
        return root;
    }
};
