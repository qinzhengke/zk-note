拓扑空间{#topology_space}
=======================

<hr>
\section topology_structure 集合上的拓扑

对于集合 \f$ X \f$，　则 \f$ X \f$的子集簇（子集的集合） \f$ \mathscr{T} \f$ 称为\f$ X \f$上的拓扑，也称为拓扑结构，若：

1. \f$ \emptyset, X \in \mathscr{T} \f$ 
2. 任意个\f$ \mathscr{T} \f$的元素的并集属于\f$ \mathscr{T} \f$
3. 有限个\f$ \mathscr{T} \f$的元素的的交集属于\f$ \mathscr{T} \f$

该定义来自于\cite liangcanbin2007 和 \cite wiki:topologyspace

有一点需要注意，一般来说，我们不太会提＂元素和元素的并集或者交集＂这种概念，但是因为这里的元素恰好也是集合，所以他们有了并集和交集的概念．

例：下图所示了4个拓扑结构和2个非拓扑结构，其中，第一行和第二行４个子图均表示拓扑结构．
第三行的两个子图表示的不是拓扑结构，左图是因为 \f${2} \cap {3} = {2,3} \not\in \mathscr{T} \f$，违反了定义2，
右图是因为 \f$ {1,2} \cup {2,3} = {2} \not\in \mathscr{T} \f$，违反了定义3．
![](./figures/Topological_space_examples.svg)

直观的说，对于一个集合，其满足一些条件的子集的集合就是拓扑结构，定义只给出了必要条件，实际上拓扑结构的选取方式可以有多种．

<hr>
\section 拓扑空间　

拓扑空间是一种 \ref math_structure ，对于一个集合\f$ X \f$，若在其上定义一种拓扑结构 \f$ 
\mathscr{T} \f$，则二元组 (\f$ X,\mathscr{T}) \f$称为拓扑空间，\f$ X \f$的元素 \f$  \f$通常称为拓扑空间\f$ (X, \mathscr{T}) \f$的点.

