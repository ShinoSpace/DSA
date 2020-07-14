## 「Tire」: what and why

「Trie」又称「前缀树」、「字典树」。如其名，是一种「树」结构，用于各类「检索」任务以降低查询的时间复杂度。根据「关键词」(query)和匹配模式的不同，我们可以将「检索」任务分类：

1. 直接由数组下标查找值。这是最简单的检索方式。
2. 哈希查找，即key-value模式的检索。query = key，查询对应的value。这是典型的哈希表的查找。
3. 模糊匹配。哈希查找是根据哈希函数将query映射到数组下标，并根据冲突处理策略进行准确定位。这要求query必须与key完整匹配。模糊查找中的一种情况是：可能只给出一些「前缀」信息。例如给出query="结城"，查询记录中所有以query开头的关键词，如"结城明日奈"。显然，哈希表做不到这种模糊查找。这种模糊匹配的典型应用场景就是搜索引擎。
4. 词典匹配。给定一个包含若干个词的词典，query是一个长句，找出词典中的词语在query中出现的所有位置。例如给出一个敏感词列表，一个长句query，找出query中所有敏感词出现的位置。

后两种模式的查找，可以使用Trie来解决。另外，即便是精确查找，如果要存储的数据量很大，那么哈希表很可能出现很多冲突，这种情况下效率可能甚至还不如Trie高。

Trie也可以视为一棵树「决策树」。

### Trie的结点(ADT)

+ 根结点是树的开始，其「状态」没有实际意义。
+ 每个结点都维护一个「子结点列表」，存储子结点指针。从决策树角度来看就是「选择列表」。
+ 每一个结点代表一个字符，但一般不在结点中存储该字符，因为我们总是先在「选择列表」中查找下一个要查询的字符是否存在，如果存在才向下走。
+ 结点的其他成员可根据具体问题自定义。

在画图分析时，我们可以将结点代表的字符作为「状态」标在节点上，但一般我们不在节点中存储实际的字符。

例如词典\["Shino", "Asuna", "Asagi", "Isla"\]，以该词典建树：

![Trie](./WWW(what-why-how)%20of%20Trie/Trie.png)

## how to use Trie?

仍以词典\["Shino", "Asuna", "Asagi", "Isla"\]为例：

![Trie](./WWW(what-why-how)%20of%20Trie/Trie.png)

数据结构无非「增删改查」四件事。Trie的目的就是为了进行查询，因此着重增、查操作。更改操作的逻辑与查是相同的。而删除操作极少使用，先行跳过。

+ 初始化Trie（建树）

初始化根结点即可。

``` c++
class Trie{
private:
	bool isEnd;		// 根据问题定义的成员。标记是否为单词结尾。
	bool isCapital;		// 根据问题定义的成员。本问题中存在大写字母，标记是否为大写。
	Trie *alphabet[26];		// 子结点列表。本问题中就是下一个位置所有可能的字母。
public:
	Trie(){
		isEnd = false;
		isCapital = false;
		memset(alphabet, 0, sizeof(alphabet));
	}
};
```

+ 增：插入

遍历字符串，每次查询当前字符是否存在于当前结点的子结点中，即判断对应的子结点指针是否为空。

``` c++
class Trie{
	void insert(string s){
		Trie *iter = this;
		for(auto c : s){
			if(!iter -> alphabet[c - 'a'])
				iter -> alphabet[c - 'a'] = new Trie();
			iter = iter -> alphabet[c - 'a'];
			iter -> isCapital = c >= 41 && c <= 90;		// 41 <= A~Z <= 90
		}
		iter -> isEnd = true;
	}
}
```

+ 查

``` c++
class Trie{
	bool search(string s){
		Trie *iter = this;
		for(auto c : s){
			if(!iter -> alphabet[c - 'a'])
				return false;
			iter = iter -> alphabet[c - 'a'];
		}
		return iter -> isEnd;
	}
}
```

## 总结：Trie的应用

1. 模糊查找。查找以query为前缀的词。
2. 大规模词典的查找。此时哈希表很可能冲突过多。
3. 给定词典，查找query中所有出现在词典中的单词。
4. 一次建立，**多次查找**。
