const int MX = 1 << 22; // 必须是2的幂
const double pi = acos(-1);

vector<complex<double>> w;
auto init = []() {
    w.resize(MX);
    for (int i = 0; i < MX; ++i) {
        w[i] = {cos(pi * 2 * i / MX), sin(pi * 2 * i / MX)};
    }
    return 0;
}();

void fft(vector<complex<double>>& a, bool inverse) {
    int n = a.size();
    for (int i = 0, j = 0; i < n; ++i) {
        if (i < j) {
            swap(a[i], a[j]);
        }
        int k = n >> 1;
        while ((j ^= k) < k) {
            k >>= 1;
        }
    }
    for (int step = 2; step <= n; step *= 2) {
        int h = step / 2, d = MX / step;
        for (int i = 0; i < n; i += step) {
            for (int j = 0; j < h; ++j) {
                auto& x = a[i + j];
                auto& y = a[i + j + h];
                auto t = w[d * j] * y;
                y = x - t;
                x = x + t;
            }
        }
    }
    if (inverse) {
        reverse(a.begin() + 1, a.end());
        for (auto& x : a) {
            x /= n;
        }
    }
}
vector<double> operator*(const vector<double>& a, const vector<double>& b) {
    int n = a.size() + b.size();
    int size = 2;
    while (size < n) {
        size <<= 1;
    }

    vector<complex<double>> x(size), y(size);
    copy(a.begin(), a.end(), x.begin());
    copy(b.begin(), b.end(), y.begin());

    fft(x, false);
    fft(y, false);
    for (int i = 0; i < size; i++) {
        x[i] *= y[i];
    }

    fft(x, true); // IFFT

    vector<double> res(n - 1);
    for (int i = 0; i < res.size(); ++i) {
        res[i] = x[i].real();
    }
    return res;
}

void solve() {
    int n, m;
    cin >> n >> m;

    vector<double> a(n + 1), b(m + 1);
    for (int i = 0; i <= n; ++i) {
        cin >> a[i];
    }
    for (int i = 0; i <= m; ++i) {
        cin >> b[i];
    }

    auto ans = a * b;
    for (auto& v : ans) {
        cout << fixed << setprecision(0) << v + 1e-8 << " ";
    }
}
