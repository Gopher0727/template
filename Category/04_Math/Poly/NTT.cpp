// 数论变换 NTT
// FFT 依赖于复数运算，引入浮点精度问题，而 NTT 将运算放在了模素数的有限域上，用原根代替复平面的单位复数根

// 一个整数 g 如果被称为模数 p 的原根，那么 g 的 0~p-2 的幂次两两不同（构成长度为 p-1 的排列）
// 原根存在的充要条件：模数 p 形如 2，4，pow(q, k)，2*pow(q, k)，其中 q 是奇素数，k >= 1


vector<Mint> roots {0, 1};
vector<int> rev;

const Mint primitiveRoot = []() {
    Mint i = 2;
    int k = __builtin_ctz(MOD - 1);
    while (true) {
        if (qpow(i, (MOD - 1) / 2) != 1) {
            break;
        }
        i++;
    }
    return qpow(i, (MOD - 1) >> k);
}();

void dft(vector<Mint>& a) {
    int n = a.size();
    assert((n & (n - 1)) == 0);

    // 预处理单位根 roots 数组
    if (roots.size() < n) {
        roots.resize(n);
        for (int k = __builtin_ctz(roots.size()); (1 << k) < n; k++) {
            auto e = qpow(primitiveRoot, 1 << (__builtin_ctz(MOD - 1) - k - 1));
            for (int i = 1 << (k - 1); i < (1 << k); i++) {
                roots[2 * i] = roots[i];
                roots[2 * i + 1] = roots[i] * e;
            }
        }
    }

    // 位逆序置换 (Bit-Reversal Permutation)
    if (rev.size() != n) {
        rev.resize(n);
        int k = __builtin_ctz(n) - 1;
        for (int i = 0; i < n; i++) {
            rev[i] = rev[i >> 1] >> 1 | (i & 1) << k;
        }
    }

    // 重排
    for (int i = 0; i < n; i++) {
        if (rev[i] < i) {
            swap(a[i], a[rev[i]]);
        }
    }

    // 核心操作：蝶形合并
    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; j++) {
                Mint u = a[i + j];
                Mint v = a[i + j + k] * roots[k + j];
                a[i + j] = u + v;
                a[i + j + k] = u - v;
            }
        }
    }
}

void idft(vector<Mint>& a) {
    int n = a.size();

    // 在模数意义下，复数域的 “取共轭” 对应把根的幂次取反
    // 就是把所有的旋转因子变成对应的逆元 w^-j
    reverse(a.begin() + 1, a.end());
    dft(a);
    Mint inv = Mint(1 - MOD) / n; // 隐含了 reverse 带来的整体的 -1
    for (int i = 0; i < n; i++) {
        a[i] *= inv;
    }
}
