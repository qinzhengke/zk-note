# 拓扑空间


## topology_structure 集合上的拓扑

对于集合 $$ X $$，　则 $$ X $$的子集簇（子集的集合） $$ \mathscr{T} $$ 称为$$ X $$上的拓扑，也称为拓扑结构，若：

1. $$ \emptyset, X \in \mathscr{T} $$ 
2. 任意个$$ \mathscr{T} $$的元素的并集属于$$ \mathscr{T} $$
3. 有限个$$ \mathscr{T} $$的元素的的交集属于$$ \mathscr{T} $$

该定义来自于\cite liangcanbin2007 和 \cite wiki:topologyspace

有一点需要注意，一般来说，我们不太会提＂元素和元素的并集或者交集＂这种概念，但是因为这里的元素恰好也是集合，所以他们有了并集和交集的概念．

例：下图所示了4个拓扑结构和2个非拓扑结构，其中，第一行和第二行４个子图均表示拓扑结构．
第三行的两个子图表示的不是拓扑结构，左图是因为 $${2} \cap {3} = {2,3} \not\in \mathscr{T} $$，违反了定义2，
右图是因为 $$ {1,2} \cup {2,3} = {2} \not\in \mathscr{T} $$，违反了定义3．
![](./figures/Topological_space_examples.svg)

直观的说，对于一个集合，其满足一些条件的子集的集合就是拓扑结构，定义只给出了必要条件，实际上拓扑结构的选取方式可以有多种．


## 拓扑空间　

拓扑空间是一种 \ref math_structure ，对于一个集合$$ X $$，若在其上定义一种拓扑结构 $$ 
\mathscr{T} $$，则二元组 ($$ X,\mathscr{T}) $$称为拓扑空间，$$ X $$的元素 $$  $$通常称为拓扑空间$$ (X, \mathscr{T}) $$的点.

