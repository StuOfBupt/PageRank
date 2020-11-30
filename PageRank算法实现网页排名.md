# PageRank算法实现网页排名

**参考链接**：

[**PageRank算法**](https://blog.csdn.net/u013007900/article/details/88961913)

## 1.图结构实现和存储

稀疏矩阵存储结构：ELLPACK（**ELL**）

![image-20201130105327185](https://raw.githubusercontent.com/StuOfBupt/MyTypora/master/img/image-20201130105327185.png?token=AHMLWBN3RGYKBMOPDBHFFY27YRPOO)

进行稀疏矩阵-矢量乘积运算时效率很快，访问Matrix[i] [j]时可以直接定位到行索引，然后遍历当前行所有的元素查看是否存在列值为j的元素。每一行使用变长数组（vector）存储，解决了当某一行元素过多导致矩阵臃肿的弊端

矩阵构造完毕，以三元组的格式存入txt文件中，第一行存节点总数，从第二行开始以三元组格式存非零元素（i, j ,value）

同时将所有的URL存入urls.txt中，第一行存URL数量（等于节点总数），第二行开始存入每个URL，其顺序与对应的节点值相同。

## 2.网页URL提取

- 使用AC自动机提取网页中的有效URL（这里指的是在当前目录下的url是有效的，因为其他的链接如广告链接等没有相应的节点，将会导致排名泄露）

- 第一次递归查询webdir目录下所有shtml/html文件，通过访问的顺序给每个文件编号，作为其节点值（URL映射到图节点），并将其作为模式串插入构造AC自动机

- 第二次递归开始对webdir目录下每个shtml/html文件读取进行匹配，找到其链接出的所有有效URL（也就是之前插入的模式串），然后转换成图节点构造稀疏矩阵Matrix


## 3.迭代计算出排名前20的网页

迭代的计算公式：

​							Pn+1 = APn （P为N维列向量）

终止条件			|Pn+1 - Pn| < e

由于方阵的维度很大（14万个点），如果直接对矩阵A的每一行遍历与向量P相乘：Pn+1[i] = sum(A[i, : ] * P[ : ]) ，那时间复杂度就ON平方,开销过大。

利用已知A为稀疏矩阵（尽管引入了随机跳转概率，但仍可以把A看成是一个系数矩阵与一个单位矩阵*概率p相加）的性质：

> 根据改进后的矩阵A，A[i,j] = M[i,j] + x (x为一常数)
>
> Pn+1[i] = A[i, 0] * P[0] + A[i,1] * P[1] + ... + A[i, N] * p[N]
>
> ​			 = (M[i, 0] + x) * P[0] + (M[i,1] + x ) * P[1] + ... + (M[i, N] + x) * p[N]
>
> ​			 = M[i, 0] * P[0] + M[i,1] * P[1] + ... + M[i, N] * p[N] + x * (P[0] + P[1] + P[2]... + P[N])
>
> ​			 = sum(M[i, : ] * P[ : ]) + x * sum(P[ : ])

每一行只有少数非零值，因此可以对矩阵迭代计算进行优化：

利用ELL格式的稀疏矩阵存储格式特点，对每一行只遍历存储的非零值与对应的P[i]相乘并求和得到sum(M[i, : ] * P[ : ])

然后再计算x * sum(P[ : ])，相加之后就是Pn+1[i],因此时间复杂度就成了O（N）

```c++
/**
 * 优化：矩阵与列向量相乘
 * @param p 列向量
 * @return
 */
vector<double> Matrix::operator*(const vector<double>& p) const {
    vector<double> p1(UrlNum);
    double sum = accumulate(p.begin(), p.end(), 0.0);
    sum *= base;
    for(int i = 0; i < UrlNum; i++){
        double val = 0;
        const vector<int>& col = vector<int>(column_idx[i]);
        for(int j = 0; j < col.size(); j++){
            val += values[i][j] * p[col[j]];
        }
        p1[i] = val + base;
    }
    return p1;
}
```

迭代结束之后将P排序即可