## why use Union-Find?

《算法4》：处理「图」问题中的「动态连通性」问题。例如求一个图结构中的「连通分量」数量问题。

实际应用：「分类」问题。例如判断两个人是否属于同一朋友圈（图中的两个结点是否连通），求社交网络中「独立」朋友圈的数量（连通分量的数量，leetcode 547. 朋友圈）。

## what and how to use Union-Find?

总的来看，并查集(Union-Find)是在「连通关系」上将图等价变换为「森林」，而不管原始图结构的拓扑关系。具体来说，并查集根据原图的邻接矩阵，将图中的各独立的连通分量重构成一棵「树」，整个图看作「森林」，每棵树的根结点看作这棵树的「代表元」。

设图G(V, E)有N个结点，邻接矩阵为A。最简单的并查集维护两个数据成员：

+ 容量为N的数组`parent`，`parent[i]`存储结点i的父节点指针。
+ 整型数connected_components_count，记录独立连通分量的数量。

### 初始化

初始化阶段，并查集将图中所有结点都视为独立的连通分量，即任意两结点间都没有「边」，因此认为有N个独立的连通分量。每个结点的父节点指针`parent[i]`初始化为结点自身i（自环初始化）。

![并查集初始化](./3W%20of%20Union-Find/初始化.png)

代码描述：

``` c++
class UnionFind{
private:
	vector<int> parent;
	int connected_components_count;
public:
	UnionFind(int n){
		parent.resize(n);
		connected_components_count = n;
		for(int i = 0; i < n; i++)
			parent[i] = i;
	}
}
```

### 连接(并)与建图

建图时，就是不断进行`connect`操作，将图中存在边的结点两两连接，这就是「并查集」中的「并」。因此遍历图的邻接矩阵，将有边的结点连接。在并查集中，这种连接操作很特殊：if `A[i][j] == 1`，则将i结点的所在树的根与j结点所在树的根相连（因此先要查找i和j结点所在树的根结点）。至于谁是儿子谁是爸爸，在最简单版本的并查集中可以互换。「平衡性优化」（下一节的优化方法之一，现在先忽略）将在这个儿子和爸爸的关系上做文章，避免连接后树的分支过重。

![connect](./3W%20of%20Union-Find/connect操作.png)

代码描述：

``` c++
class UnionFind{
	void connect(int p, int q){
		int root_p = find_root(p);		// find_root操作见下面的查找操作
		int root_q = find_root(q);
		if(root_p == root_q)
			return;
		parent[root_q] = root_p;	// 最简单版本中，parent[root_q] = root_p也可以。
		connected_components_count--;
	}
}
```

这种连接操作下，由于整棵树的根结点满足`parent[i] = i`，所以从任意结点`x`开始，以`parent`数组存储的父子关系为根据，一直向父节点回溯，最终一定会收敛到结点`x`所在整棵树的根结点。所以判断任意两结点是否属于一个连通分量时，只需判断这两个结点所在树的根结点是否相同即可。

另外，不难发现上面的连接操作几乎必然丢失了原始图的拓扑结构。实际上，我们是按照结点间的连通关系，将整张图**简化**为一个森林。也就是说，尽管我们破坏了原始图结构的拓扑关系，但**在连通关系上，并查集与原图等价**。这也正是我们能正确且高效地处理「动态连通性」问题的原因。

### 寻根

两棵树的连接是将他们的根相连，因此「寻根」是关键。

「寻根」操作也很简单，从给定结点开始，向父节点移动指针，直到收敛。

``` c++
class UnionFind{
	int find_root(int x){
		while(x != parent[x])
			x = parent[x];
		return x;
	}
}
```

### 判断结点间的连接性

两结点同根，则他们必属同一个连通分量，反之处于不同的连通分量中。

``` c++
class UnionFind{
	bool is_connected(int p, int q){
		int root_p = find(p);
		int root_q = find(q);
		return root_p == root_q;
	}
}
```

「寻根」和「判断结点连接性」都是并查集「查」的部分。

到此为止，基本并查集涉及的算法全部完成。

## 优化：平衡性优化与路径压缩

### 平衡性优化

连接操作会将两棵树合到一起，使他们的根节点root相同。不难发现，如果要连接的两棵树一轻一重，若将重的连到轻的根节点上，会造成严重的分支重量不平衡问题。解决这个问题的方法并不难：**在connect函数连接两棵树前，先判断哪棵树更重**。因此我们需要额外维护一个数组`weight`，`weight[i]`保存以i为根结点的树的「重量」。如果i不是根结点，那么`weight[i]`就是一个don't care的量。这种优化方式称为「平衡性优化」。

那么问题来了，什么是树的「重量」？这里有两种定义方式：

1. 树的结点总数size
2. 树的最大深度depth

`weight`保存size的优化方式称为**基于size的优化**，而保存depth的优化方式称为**基于rank(秩)的优化**。

基于size优化的代码描述：

``` c++
class UnionFind{
private:
	vector<int> parent;
	vector<int> weight;
	int connected_components_count;
public:
	void connect(int p, int q){
		int root_p = find_root(p);
		int root_q = find_root(q);
		if(root_p == root_q)
			return;
		if(weight[root_p] > wight[root_q]){
			parent[root_q] = root_p;
			weight[root_p] += weight[root_q];
		}
		else{
			parent[root_p] = root_q;
			weight[root_q] += weight[root_p];
		}
	}
}
```

### 路径压缩

如前所述，并查集只是在连通关系上与原图等价，原始拓扑关系并没有保留。每个连通分量都是一棵树，那么这棵树除了根结点之外的其他结点的父子关系其实并不重要。那么为了保证「寻根」操作的高效，我们何不将树进行压缩，使其深度尽可能地小？这种压缩有两种：

1. 隔代压缩。将当前结点接到其父节点的父节点。压缩后，树的最大深度不超过3。
2. 完全压缩。将当前结点直接接到整棵树的根节点下。压缩后，树的最大深度只有2。

这两种压缩方式都是直接在「寻根」操作上直接修改。

隔代压缩的代码：

``` c++
class UnionFind{
	int find_root(int x){
		while(x != parent[x]){
			parent[x] = parent[parent[x]];		// 当前结点接到父节点的父节点
			x = parent[x];
		}
		return x;
	}
}
```

完全压缩的图示和代码：

![完全压缩](./3W%20of%20Union-Find/完全路径压缩.gif)

``` c++
class UnionFind{
	int find_root(int x){
		int root = x;
		while(root != parent[root]){
			root = parent[root];		// 找到整棵树的根
		}
		while(x != parent[x]){
			int temp = parent[x];	// 暂存当前结点的父节点
			parent[x] = root;		// 当前结点直接接到整棵树的根下面
			x = temp;
		}
	}
}
```

## Get Started

547\. 朋友圈
924\. 尽量减少恶意软件的传播
1319\. 连通网络的操作次数
