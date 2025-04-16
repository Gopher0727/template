/* random */
mt19937_64 gen{random_device{}()};


/* pdbtree */
#include <bits/extc++.h>
// int
tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> t;
// pair<>
using PII = pair<int, int>;
__gnu_pbds::tree<PII, __gnu_pbds::null_type, less<PII>, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update> rbt;


// How? 查询一段区间中某个数的出现次数
// > 哈希表 + 二分查找
// > 可持久化线段树
// > 分块 + 二分查找
