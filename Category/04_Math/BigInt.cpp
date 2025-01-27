const int base = 1000000000;
const int base_digits = 9; // 分解为九个数位一个数字
struct BigInt {
    vector<int> a;
    int sign;

public:
    BigInt() : sign(1) {}
    BigInt(i64 v) { *this = v; }
    BigInt(const string& s) { read(s); }

private:
    void trimLeadingZero() {
        while (!a.empty() && !a.back()) {
            a.pop_back();
        }
        if (a.empty()) {
            sign = 1;
        }
    }

    void read(const string& s) {
        sign = 1;
        a.clear();
        int pos = 0;
        while (pos < (int) s.size() && (s[pos] == '-' || s[pos] == '+')) {
            if (s[pos] == '-') {
                sign = -sign;
            }
            ++pos;
        }
        for (int i = s.size() - 1; i >= pos; i -= base_digits) {
            int x = 0;
            for (int j = max(pos, i - base_digits + 1); j <= i; j++) {
                x = x * 10 + s[j] - '0';
            }
            a.push_back(x);
        }
        trimLeadingZero();
    }

    static vector<int> convert_base(const vector<int>& a, int old_digits, int new_digits) {
        vector<i64> p(max(old_digits, new_digits) + 1);
        p[0] = 1;
        for (int i = 1; i < (int) p.size(); ++i) {
            p[i] = p[i - 1] * 10;
        }

        vector<int> res;
        i64 cur = 0;
        int cur_digits = 0;
        for (int i = 0; i < (int) a.size(); ++i) {
            cur += a[i] * p[cur_digits];
            cur_digits += old_digits;
            while (cur_digits >= new_digits) {
                res.push_back(cur % p[new_digits]);
                cur /= p[new_digits];
                cur_digits -= new_digits;
            }
        }
        res.push_back((int) cur);
        while (!res.empty() && !res.back()) {
            res.pop_back();
        }
        return res;
    }

public:
    BigInt operator-() const {
        BigInt res = *this;
        res.sign = -sign;
        return res;
    }

    // BigInt and int
    BigInt operator*(int v) const {
        BigInt res = *this;
        res *= v;
        return res;
    }
    BigInt operator/(int v) const {
        BigInt res = *this;
        res /= v;
        return res;
    }
    int operator%(int v) const {
        if (v < 0) {
            v = -v;
        }
        int m = 0;
        for (int i = a.size() - 1; i >= 0; --i) {
            m = (a[i] + m * (i64) base) % v;
        }
        return m * sign;
    }
    void operator*=(int v) {
        check(v);
        for (int i = 0, carry = 0; i < (int) a.size() || carry; ++i) {
            if (i == (int) a.size()) {
                a.push_back(0);
            }
            i64 cur = a[i] * (i64) v + carry;
            carry = (int) (cur / base);
            a[i] = (int) (cur % base);
        }
        trimLeadingZero();
    }
    void operator/=(int v) {
        check(v);
        for (int i = (int) a.size() - 1, rem = 0; i >= 0; --i) {
            i64 cur = a[i] + rem * 1ll * base;
            a[i] = (int) (cur / v);
            rem = (int) (cur % v);
        }
        trimLeadingZero();
    }
    void operator%=(const int& v) { *this = *this % v; }

    // BigInt and BigInt
    BigInt operator+(const BigInt& v) const {
        if (sign == v.sign) {
            BigInt res = v;
            for (int i = 0, carry = 0; i < (int) max(a.size(), v.a.size()) || carry; ++i) {
                if (i == (int) res.a.size()) {
                    res.a.push_back(0);
                }
                res.a[i] += carry + (i < (int) a.size() ? a[i] : 0);
                carry = res.a[i] >= base;
                if (carry) {
                    res.a[i] -= base;
                }
            }
            return res;
        }
        return *this - (-v);
    }
    BigInt operator-(const BigInt& v) const {
        if (sign == v.sign) {
            if (abs() >= v.abs()) {
                BigInt res = *this;
                for (int i = 0, carry = 0; i < (int) v.a.size() || carry; ++i) {
                    res.a[i] -= carry + (i < (int) v.a.size() ? v.a[i] : 0);
                    carry = res.a[i] < 0;
                    if (carry) {
                        res.a[i] += base;
                    }
                }
                res.trimLeadingZero();
                return res;
            }
            return -(v - *this);
        }
        return *this + (-v);
    }
    BigInt operator*(const BigInt& v) const {
        vector<int> a6 = convert_base(this->a, base_digits, 6);
        vector<int> b6 = convert_base(v.a, base_digits, 6);
        vector<i64> a(a6.begin(), a6.end());
        vector<i64> b(b6.begin(), b6.end());
        while (a.size() < b.size()) {
            a.push_back(0);
        }
        while (b.size() < a.size()) {
            b.push_back(0);
        }
        while (a.size() & (a.size() - 1)) {
            a.push_back(0), b.push_back(0);
        }

        vector<i64> c = karatsubaMultiply(a, b);
        BigInt res;
        res.sign = sign * v.sign;
        for (int i = 0, carry = 0; i < (int) c.size(); ++i) {
            i64 cur = c[i] + carry;
            res.a.push_back((int) (cur % 1000000));
            carry = (int) (cur / 1000000);
        }
        res.a = convert_base(res.a, 6, base_digits);
        res.trimLeadingZero();
        return res;
    }
    BigInt operator/(const BigInt& v) const { return divmod(*this, v).first; }
    BigInt operator%(const BigInt& v) const { return divmod(*this, v).second; }

    void operator+=(const BigInt& v) { *this = *this + v; }
    void operator-=(const BigInt& v) { *this = *this - v; }
    void operator*=(const BigInt& v) { *this = *this * v; }
    void operator/=(const BigInt& v) { *this = *this / v; }
    void operator%=(const BigInt& v) { *this = *this % v; }

    // Assignment operator
    void operator=(i64 v) {
        a.clear();
        sign = 1;
        if (v < 0) {
            sign = -1, v = -v;
        }
        while (v > 0) {
            a.push_back(v % base);
            v = v / base;
        }
    }
    void operator=(const BigInt& v) {
        sign = v.sign;
        a = v.a;
    }

    // Comparison operator
    bool operator<(const BigInt& v) const {
        if (sign != v.sign) {
            return sign < v.sign;
        }
        if (a.size() != v.a.size()) {
            return a.size() * sign < v.a.size() * v.sign;
        }
        for (int i = a.size() - 1; i >= 0; i--) {
            if (a[i] != v.a[i]) {
                return a[i] * sign < v.a[i] * sign;
            }
        }
        return false;
    }
    bool operator>(const BigInt& v) const { return v < *this; }
    bool operator<=(const BigInt& v) const { return !(v < *this); }
    bool operator>=(const BigInt& v) const { return !(*this < v); }
    bool operator==(const BigInt& v) const { return !(*this < v) && !(v < *this); }
    bool operator!=(const BigInt& v) const { return *this < v || v < *this; }

public:
    friend istream& operator>>(istream& is, BigInt& v) {
        string s;
        is >> s;
        v.read(s);
        return is;
    }
    friend ostream& operator<<(ostream& os, const BigInt& v) {
        if (v.sign == -1) {
            os << '-';
        }
        os << (v.a.empty() ? 0 : v.a.back());
        for (int i = (int) v.a.size() - 2; i >= 0; --i) {
            os << setw(base_digits) << setfill('0') << v.a[i];
        }
        return os;
    }

public:
    BigInt abs() const {
        BigInt res = *this;
        res.sign *= res.sign;
        return res;
    }

    bool check(int v) { // 检查输入的是否为负数
        if (v < 0) {
            sign = -sign;
            v = -v;
            return true;
        }
        return false;
    }

    bool isZero() const { return a.empty() || (a.size() == 1 && !a[0]); }

    friend BigInt gcd(const BigInt& a, const BigInt& b) { return b.isZero() ? a : gcd(b, a % b); }
    friend BigInt lcm(const BigInt& a, const BigInt& b) { return a / gcd(a, b) * b; }

    friend pair<BigInt, BigInt> divmod(const BigInt& a1, const BigInt& b1) {
        int norm = base / (b1.a.back() + 1);
        BigInt a = a1.abs() * norm;
        BigInt b = b1.abs() * norm;
        BigInt q, r;
        q.a.resize(a.a.size());
        for (int i = (int) a.a.size() - 1; i >= 0; i--) {
            r *= base;
            r += a.a[i];
            int s1 = r.a.size() <= b.a.size() ? 0 : r.a[b.a.size()];
            int s2 = r.a.size() <= b.a.size() - 1 ? 0 : r.a[b.a.size() - 1];
            int d = ((i64) base * s1 + s2) / b.a.back();
            r -= b * d;
            while (r < 0) {
                r += b, --d;
            }
            q.a[i] = d;
        }
        q.sign = a1.sign * b1.sign;
        r.sign = a1.sign;
        q.trimLeadingZero();
        r.trimLeadingZero();
        return make_pair(q, r / norm);
    }

    static vector<i64> karatsubaMultiply(const vector<i64>& a, const vector<i64>& b) {
        int n = a.size();
        vector<i64> res(n + n);
        if (n <= 32) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; j++) {
                    res[i + j] += a[i] * b[j];
                }
            }
            return res;
        }

        int k = n >> 1;
        vector<i64> a1(a.begin(), a.begin() + k), a2(a.begin() + k, a.end());
        vector<i64> b1(b.begin(), b.begin() + k), b2(b.begin() + k, b.end());
        vector<i64> a1b1 = karatsubaMultiply(a1, b1);
        vector<i64> a2b2 = karatsubaMultiply(a2, b2);
        for (int i = 0; i < k; ++i) {
            a2[i] += a1[i];
            b2[i] += b1[i];
        }

        vector<i64> r = karatsubaMultiply(a2, b2);
        for (int i = 0; i < (int) a1b1.size(); ++i) {
            r[i] -= a1b1[i];
            res[i] += a1b1[i];
        }
        for (int i = 0; i < (int) a2b2.size(); ++i) {
            r[i] -= a2b2[i];
            res[i + n] += a2b2[i];
        }
        for (int i = 0; i < (int) r.size(); ++i) {
            res[i + k] += r[i];
        }
        return res;
    }
};
