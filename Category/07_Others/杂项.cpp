// priority_queue 是`大顶堆`

// set 与 multiset 中的删除:
// multiset 的 extract 是去掉一个元素，元素不存在则无操作，返回值为 node_type。
// multiset 的 erase 的数值删除用法，会把相同的值都删掉，返回值为 size_type，返回一个指向被删除元素的下一个元素的迭代器。

// 可以利用 map 的 emplace 方法返回值

// 输出控制： fixed, setprecision, setw, setfill

// array 相当于普通数组，局部需要初始化； vector 嵌套 array 则不需要。
// 对于结构体的存储，vector 会进行初始化（int 为 0；string 为空；指针地址为 0，解引用无意义；array 也会进行初始化）

// 关联容器 (set, map, multiset, multimap)
// 无序关联容器 (unordered_set, unordered_map, unordered_multiset, unordered_multimap):
// 在这些容器中，erase只会让指向被删除元素的迭代器失效，其它迭代器仍然可以安全使用。

/* random */
mt19937_64 gen{random_device{}()};


/* pdbtree */
#include <bits/extc++.h>
// int
tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> t;
// pair<>
using PII = pair<int, int>;
__gnu_pbds::tree<PII, __gnu_pbds::null_type, less<PII>, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update> rbt;



#define functor(func) ([&](auto&&... val) noexcept(noexcept(func(std::forward<decltype(val)>(val)...))) -> decltype(auto) { return func(std::forward<decltype(val)>(val)...); })
#define expr(ret, ...) ([&](__VA_ARGS__) { return (ret); })

//---------------------------------------------------------------------------------------------------------------------------------------
// 凡是求方案数的问题一定要初始化。
//
// 对于相邻元素差的绝对值之和，考虑能否通过排序简化。
