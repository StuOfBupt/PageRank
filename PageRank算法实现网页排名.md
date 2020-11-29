# PageRank算法实现网页排名

**参考链接**：

[**PageRank算法**](https://blog.csdn.net/u013007900/article/details/88961913)

## 1.图结构实现和存储

稀疏矩阵存储结构：ELLPACK（**ELL**）

<img src="/Users/wangshangrong/Library/Application Support/typora-user-images/image-20201114154030037.png" alt="image-20201114154030037" style="zoom: 25%;" />

进行稀疏矩阵-矢量乘积运算时效率很快，访问Matrix[i] [j]时可以直接定位到行索引，然后遍历当前行所有的元素查看是否存在列值为j的元素。每一行使用变长数组（vector）存储，解决了当某一行元素过多导致矩阵臃肿的弊端

矩阵构造完毕，以三元组的格式存入txt文件中，第一行存节点总数，从第二行开始以三元组格式存非零元素（i, j ,value）

同时将所有的URL存入urls.txt中，第一行存URL数量（等于节点总数），第二行开始存入每个URL，其顺序与对应的节点值相同。

## 2.网页URL提取

使用AC自动机提取网页中的URL。

第一次递归查询webdir目录下所有shtml/html文件，通过访问的顺序给每个文件编号，作为其节点值（URL映射到图节点），并将其作为模式串插入构造AC自动机。

第二次递归开始对webdir目录下每个shtml/html文件读取进行匹配，找到其链接出的所有有效URL（也就是之前插入的模式串），然后转换成图节点构造稀疏矩阵Matrix。

## 3.迭代计算出排名前20的网页



