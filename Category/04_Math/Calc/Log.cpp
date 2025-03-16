namespace Log {
    // a^t >= b
    int log_a(int a, int b) {
        int t = 0;
        i64 v = 1;
        while (v < b) {
            v *= a;
            t++;
        }
        return t;
    }
    // >= __lg(t)
    int llog(int a, int b) {
        if (a <= b) {
            int l = log_a(a, b);
            return (l == 0 ? 0 : __lg(2 * l - 1));
        }
        int l = log_a(b, a + 1) - 1;
        assert(l > 0);
        return -__lg(l);
    }
} // namespace Log
using namespace Log;
