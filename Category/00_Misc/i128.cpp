namespace Algo_i128 {
    using i128 = __int128;
    using u128 = unsigned __int128;

    ostream& operator<<(ostream& os, i128 n) {
        if (n == 0) {
            return os << 0;
        }
        string s;
        while (n > 0) {
            s += static_cast<char>('0' + n % 10);
            n /= 10;
        }
        std::reverse(s.begin(), s.end());
        return os << s;
    }

    i128 stoi128(const string& s) {
        i128 n = 0;
        for (auto c : s) {
            n = n * 10 + (c - '0');
        }
        return n;
    }

    i128 mul128(i128 a, i128 b) {
        if (a == 0 || b == 0) {
            return 0;
        }
        i128 result = a * b;
        if (result / b != a) {
            throw std::overflow_error("i128 multiplication overflow");
        }
        return result;
    }

    i128 abs128(const i128 n) {
        return n < 0 ? -n : n;
    }

    i128 sqrt128(const i128 n) {
        i128 lo = 0, hi = 1E16;
        while (lo < hi) {
            i128 x = (lo + hi + 1) / 2;
            if (x * x <= n) {
                lo = x;
            } else {
                hi = x - 1;
            }
        }
        return lo;
    }

    i128 gcd128(i128 a, i128 b) {
        while (b) {
            a %= b;
            std::swap(a, b);
        }
        return a;
    }

    i128 lcm128(i128 a, i128 b) {
        if (a == 0 || b == 0) {
            return 0;
        }
        i128 res = mul128(a / gcd128(a, b), b);
        return abs128(res);
    }
} // namespace Algo_i128
using namespace Algo_i128;
