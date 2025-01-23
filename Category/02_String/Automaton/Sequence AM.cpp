// 序列自动机

// 构造
// a 是字符集，|s| = n，nxt[i][j] 表示 i 以后的第一个字符 j 的位置
// 0 为根节点，可以建立一个 DAG

// 扩展构建
// 字符集较大时，可以套用持久化，在叶子节点上放一个 id，表示出边

// > 判断是否是原字符串的子序列
// > 求子序列个数
// > 求两串的公共子序列个数
// > 求字符串的回文子序列个数
// > 求 A、B 最长公共子序列 S，使得 C 是 S 的子序列

// 例题
// https://www.luogu.com.cn/problem/CF17C

class FSM {
    struct State {
        map<char, int> transitions; // 转移表，字符 -> 状态ID
        bool isAccepting = false; // 是否为接受状态
    };
    vector<State> states; // 所有状态
    int currentState; // 当前状态ID

public:
    FSM() : currentState(0) {
        addState(); // 添加初始状态
    }

    // 添加新状态并返回其ID
    int addState(bool isAccepting = false) {
        states.push_back({{}, isAccepting});
        return states.size() - 1;
    }

    // 添加从状态`from`通过字符`ch`转移到状态`to`
    void addTransition(int from, char ch, int to) {
        if (from >= states.size() || to >= states.size()) {
            throw runtime_error("Invalid state ID");
        }
        states[from].transitions[ch] = to;
    }

    // 设置状态`stateID`为接受状态
    void setAccepting(int stateID, bool accepting) {
        if (stateID >= states.size()) {
            throw runtime_error("Invalid state ID");
        }
        states[stateID].isAccepting = accepting;
    }

    // 尝试处理输入字符串`input`
    bool processInput(const string& input) {
        currentState = 0; // 总是从初始状态开始

        for (char ch : input) {
            if (states[currentState].transitions.find(ch) == states[currentState].transitions.end()) {
                // 如果没有对应的转移，则无法处理此输入
                return false;
            }
            currentState = states[currentState].transitions[ch];
        }

        // 检查最终状态是否是接受状态
        return states[currentState].isAccepting;
    }
};
