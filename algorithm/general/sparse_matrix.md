# 基于unordered_map的稀疏矩阵的实现

## unordered_map
复习一下基础知识：
pair是STL提供的二元组合模板，可以把两个数据绑定在一起。

unordered_map是一种关联容器，能够存储带有键值和映射值的组合，例如刚才提到的pair。

unordered_map和map相似却又不同：

1. map是红黑树实现，unordered_map是哈希表实现，哈希表查找时候要算hash，这个最坏时间复杂度是O(key的长度)；。
2. map保证按键值有序遍历，因此可以做范围查找，而unordered_map不可以。
3. map的迭代器不会失效，除非所指元素被删除。unordered_map的iterator在修改元素时可能会失效。所以对关联容器的遍历与修改在一定程度上可并行。
