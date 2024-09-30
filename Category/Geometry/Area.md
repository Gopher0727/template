# 计算面积

## 向量叉乘
```cpp
double area(int a, int b, int x1, int y1, int x2, int y2) {
    return (double)abs((x1 - a) * (y2 - b) - (x2 - a) * (y1 - b)) / 2; // S = 1/2 * |(x1 - y2) * (x2 - y1)|
}
```

## 海伦公式
```cpp
double area(int ax, int ay, int bx, int by, int cx, int cy) {
    double ab = sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
    double ac = sqrt((ax - cx) * (ax - cx) + (ay - cy) * (ay - cy));
    double bc = sqrt((bx - cx) * (bx - cx) + (by - cy) * (by - cy));
    double q = (ab + ac + bc) / 2;
    return sqrt(q * (q - ab) * (q - ac) * (q - bc)); // 海伦公式
}
```


# 判断点在三角形内部还是外部

> 用该点与三角形的三个顶点连线构成的三个三角形的面积（S1，S2，S3）之和与原来三角形的面积（S）比较
  相等： 内部 或 边界
  大于： 外部

> 叉积： 顺时针或逆时针求一遍叉积
    所有结果均正或者均负，则在三角形内部；
    既有正又有负，则在三角形外部；
    两个 0（正负叉积共有一个），则在三角形顶点；
    其余情况在三角形边界（不含顶点）。
