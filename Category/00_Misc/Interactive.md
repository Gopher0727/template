```cpp []
template <typename T = int32_t, typename... Args>
T query(Args... args) {
    cout << "? ";
    ((cout << args << ' '), ...) << endl;

    T t;
    cin >> t;
    return t;
}
template <typename... Args>
void respond(Args... args) {
    cout << "! ";
    ((cout << args << ' '), ...) << endl;
}
```

```py []
def query(*args):
    print("?", *args, flush=True)
    return II()


def respond(*args):
    print("!", *args, flush=True)
```
