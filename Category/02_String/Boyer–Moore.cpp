// Boyer–Moore

// KMP 算法将前缀匹配的信息用到了极致，而 BM 算法基本思想是通过后缀匹配获得比前缀匹配更多的信息来实现更快的字符跳转。
// 通过使用坏字符规则和好后缀规则来快速跳过不可能匹配的位置。

// 构建坏字符偏移表（bad character shift table）
// 构建好后缀偏移表（good suffix shift table）
// 查找所有模式出现位置
