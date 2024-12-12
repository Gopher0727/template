# 斜截式直线方程
```cpp []
double lk = 0, lb = 0;
if (x1 == x2) {
    lk = lb = INT_MIN; // 不合法
} else {
    lk = (double) (y1 - y2) / (x1 - x2);
    lb = y1 - k * x1;
}
```

# 判断点在直线上
```cpp []
if (x1 == x2 && x1 == a || (y1 - y2) * (x1 - a) == (y1 - b) * (x1 - x2)) {
    cout << "Yes\n";
    return;
}
```
