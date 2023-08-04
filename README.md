
# project-22
research report on MPT

# What is MPT  
MPT（Merkle Patricia Tree）是一种经过改良的、融合了默克尔树和前缀树两种树结构优点的数据结构。

这种数据结构具有三种节点类型：

### 1.分支结点（branch node）：包含16个分支，以及1个value

### 2.扩展结点（extension node）：只有1个子结点

### 3.叶子结点（leaf node）：没有子结点，包含一个value
![899206ba5c3bafa1fb1dc5c654b9368b_1460364-20181128194442769-522481305](https://github.com/jlwdfq/project-22/assets/129512207/b760ed59-794e-4c54-a904-176c3238ed1d)

如图所示：

其中叶子节点，表示为[key,value]的一个键值对，其中key是key的一种特殊十六进制编码。

扩展节点，也是[key，value]的一个键值对，但是这里的value是其他节点的hash值，这个hash可以被用来查询数据库中的节点。也就是说通过hash链接到其他节点。

分支节点，因为MPT树中的key被编码成一种特殊的16进制的表示，再加上最后的value，所以分支节点是一个长度为17的list，前16个元素对应着key中的16个可能的十六进制字符，如果有一个[key,value]对在这个分支节点终止，最后一个元素代表一个值，即分支节点既可以搜索路径的终止也可以是路径的中间节点。

# Why MPT
在以太坊中，帐户的交易信息、状态以及相应的状态变更，还有相关的交易信息等都使用MPT来进行管理，其是整个数据存储的重要一环。交易树，收据树，状态树都是采用的MPT结构。

为此以太坊对此进行了许多改进。
1.为了保证树的加密安全，MPT指向下一级节点的指针是使用节点的确定性加密hash，而不是传统意义上下一级节点地址的指针。每个节点通过他的hash被引用，而非32bit或64bit的内存地址，即树的Merkle部分是一个节点的确定性加密的hash。一个非叶节点存储在leveldb关系型数据库中，数据库中的key是节点的RLP编码的sha3哈希，value是节点的RLP编码。如果给定的trie的根哈希是公开的，则任何人都可以通过给出给定path上的所有节点, 来证明在给定path上存在一个给定值 ，对于攻击者,不可能提供一个不存在的（key，value）对的证明, 因为根哈希最终基于它下面的所有哈希，所以任何修改都会改变根哈希。想要获得一个非叶节点的子节点，只需要根据子节点的hash访问数据库获得节点的RLP编码，然后解码就行了。通过这种模式，根节点就成为了整个树的加密签名，如果一颗给定trie的跟hash是公开的，那么所有人都可以提供一种证明，通过提供每步向上的路径证明特定的key是否含有给定的值。
![4e9f18c80b532bae708a55e34f2706fe_1460364-20181128201742562-1038078308](https://github.com/jlwdfq/project-22/assets/129512207/a8778560-2391-46fa-a3e7-5d353b6b18f2)

2.MPT中众多的节点类型能够提高效率。MPT树中的节点包括空节点、叶子节点、扩展节点和分支节点。

其中有空节点，简单的表示空，在代码中是一个空串。

标准的叶子节点，表示为[key,value]的一个list，其中key是key的一种特殊十六进制编码，value是value的RLP编码。

扩展节点，也是[key，value]的列表，但是这里的value是其他节点的hash，这个hash可以被用来查询数据库中的节点。也就是说通过hash链接到其他节点。

最后分支节点，因为MPT树中的key被编码成一种特殊的16进制的表示，再加上最后的value，所以分支节点是一个长度为17的list，前16个元素对应着key中的16个可能的十六进制字符，如果有一个[key,value]对在这个分支节点终止，最后一个元素代表一个值，即分支节点既可以搜索路径的终止也可以是路径的中间节点。

3.除了四种节点，MPT树中另外一个重要的概念是一个特殊的十六进制前缀(hex-prefix, HP)编码，用来对key进行编码。因为字母表是16进制的，所以每个节点可能有16个孩子。因为有两种[key,value]节点(叶节点和扩展节点)，引进一种特殊的终止符标识来标识key所对应的是值是真实的值，还是其他节点的hash。如果终止符标记被打开，那么key对应的是叶节点，对应的值是真实的value。如果终止符标记被关闭，那么值就是用于在数据块中查询对应的节点的hash。无论key奇数长度还是偶数长度，HP多可以对其进行编码。最后我们注意到一个单独的hex字符或者4bit二进制数字，即一个nibble。
![f8537c18eba2df1276656ff22082f0d4_1460364-20181128202100492-1833040532](https://github.com/jlwdfq/project-22/assets/129512207/c8d7a6cb-14ac-434c-8118-2843a2c6e266)

如图所示，prefix就是HP编码！对终止符的状态和奇偶性进行编码。最低位表示奇偶性，第二低位编码终止符状态。
总共有2个扩展节点，2个分支节点，4个叶子节点。 右边是叶子节点的情况，左边代表的是节点的prefix（HP编码）
这就是一个状态树的存储形式，其实他应当长的样子，我们可以细细想一下，他的key被编码成一种特殊的16进制的表示，value是一些rlp后的数据，而且比上图要大的多的多。

总的来说相对于普通的前缀树，MPT树能有效减少Trie树的深度，增加Trie树的平衡性。而且通过节点的hash值进行树的节点的链接，有助于提高树的安全性和可验证性，是Trie和Merkle树混合加上平衡操作后的产物。
# How MPT work
### 1.update  _update_and_delete_storage(self, node, key, value)
i. 如果node是空节点，直接返回[pack_nibbles(with_terminator(key)), value]，即对key加上终止符，然后进行HP编码。

ii. 如果node是分支节点，如果key为空，则说明更新的是分支节点的value，直接将node[-1]设置成value就行了。如果key不为空，则递归更新以key[0]位置为根的子树，即沿着key往下找，即调_update_and_delete_storage(self._decode_to_node(node[key[0]]), key[1:], value)。

iii. 如果node是kv节点（叶子节点或者扩展节点），调用_update_kv_node(self, node, key, value)，见步骤iv

iv. curr_key是node的key，找到curr_key和key的最长公共前缀，长度为prefix_length。Key剩余的部分为remain_key，curr_key剩余的部分为remain_curr_key。

a)    如果remain_key==[]== remain_curr_key，即key和curr_key相等，那么如果node是叶子节点，直接返回[node[0], value]。如果node是扩展节点，那么递归更新node所链接的子节点，即调用_update_and_delete_storage(self._decode_to_node(node[1]), remain_key, value)

b)    如果remain_curr_key == []，即curr_key是key的一部分。如果node是扩展节点，递归更新node所链接的子节点，即调用_update_and_delete_storage(self._decode_to_node(node[1]), remain_key, value)；如果node是叶子节点，那么创建一个分支节点，分支节点的value是当前node的value，分支节点的remain_key[0]位置指向一个叶子节点，这个叶子节点是[pack_nibbles(with_terminator(remain_key[1:])), value]

c)    否则，创建一个分支节点。如果curr_key只剩下了一个字符，并且node是扩展节点，那么这个分支节点的remain_curr_key[0]的分支是node[1]，即存储node的value。否则，这个分支节点的remain_curr_key[0]的分支指向一个新的节点，这个新的节点的key是remain_curr_key[1:]的HP编码，value是node[1]。如果remain_key为空，那么新的分支节点的value是要参数中的value，否则，新的分支节点的remain_key[0]的分支指向一个新的节点，这个新的节点是[pack_nibbles(with_terminator(remain_key[1:])), value]

d)    如果key和curr_key有公共部分，为公共部分创建一个扩展节点，此扩展节点的value链接到上面步骤创建的新节点，返回这个扩展节点；否则直接返回上面步骤创建的新节点
v. 删除老的node，返回新的node

### 2.delete  _delete_and_delete_storage(self, key)
i. 如果node为空节点，直接返回空节点

ii. 如果node为分支节点。如果key为空，表示删除分支节点的值，直接另node[-1]=‘’, 返回node的正规化的结果。如果key不为空，递归查找node的子节点，然后删除对应的value，即调用self._delete_and_delete_storage(self._decode_to_node(node[key[0]]), key[1:])。返回新节点

iii. 如果node为kv节点，curr_key是当前node的key。

a) 如果key不是以curr_key开头，说明key不在node为根的子树内，直接返回node。

b) 否则，如果node是叶节点，返回BLANK_NODE if key == curr_key else node。

c)如果node是扩展节点，递归删除node的子节点，即调用_delete_and_delete_storage(self._decode_to_node(node[1]), key[len(curr_key):])。如果新的子节点和node[-1]相等直接返回node。否则，如果新的子节点是kv节点，将curr_key与新子节点的可以串联当做key，新子节点的value当做vlaue，返回。如果新子节点是branch节点，node的value指向这个新子节点，返回。
### 3.get  _get(self, node, key)
i. 如果node是空节点，返回空节点

ii. 如果node是分支节点，如果key为空，返回分支节点的value；否则递归查找node的子节点，即调用_get(self._decode_to_node(node[key[0]]), key[1:])

iii. 如果node是叶子节点，返回node[1] if key == curr_key else ‘’

iv. 如果node是扩展节点，如果key以curr_key开头，递归查找node的子节点，即调用_get(self._decode_to_node(node[1]), key[len(curr_key):])；否则，说明key不在以node为根的子树里，返回空

### 4.insert
插入操作基于查找完成，首先找到与新插入节点拥有最长相同路径前缀的节点，之后基于此节点将剩下的节点进行封装插入。
# 实现方式
1.首先在MPT_Node.h中定义结构体，里面包括：prefix、shared_nibbles、value、leaf（是否是叶节点）以及子节点，以此作为 树上的单个节点。

2.根据课堂例子内容：1）根据信息，求取信息的hash值2）将hash值作为路径对hash值所对应的值进行记录来创建具体的MPT。

3.存在性证明以及不存在性证明。
# 实验结果

![image](https://github.com/jlwdfq/project-22/assets/129512207/45b1bc3c-940e-4b95-a126-7e24b06d25c3)
# 实验环境
Windows10

vs 2022

CPU：11th Gen Intel(R) Core(TM) i7-11800H @ 2.30GHz
# 小组分工 
戴方奇202100460092 单人组完成project22
