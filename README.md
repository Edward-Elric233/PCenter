<script type="text/x-mathjax-config">
  MathJax.Hub.Config({
    tex2jax: {
      inlineMath: [ ['$','$'], ["\\(","\\)"] ],
      processEscapes: true
    }
  });
</script>
<script src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML" type="text/javascript"></script>

# VMTS algorithm for P-Center problem

## 参考文献

> Q. Zhang, Z. Lü, Z. Su, C. Li, Y. Fang, and F. Ma, “Vertex Weighting-Based Tabu Search for p-Center Problem,” in Proceedings of the Twenty-Ninth International Joint Conference on Artificial Intelligence, IJCAI-20, 2020, pp. 1481–1487. doi: 10.24963/ijcai.2020/206.

## 算法结构

首先将P-Center问题转换为判定问题：通过对边长的上界进行递减，判断当前上界是否能够通过P个中心覆盖所有的点，最小的通过测试的上界就是P-Center问题所求最小值

然后对每次判定使用VMTS算法

## 数据结构设计

因为算法多次需要计算集合的交集、并集、势、遍历，因此设计一个集合数据结构是算法的核心。

我们考虑使用位图作为集合的实现，使用位图加速计算集合交集、并集的速度，将原本$O((n+m)log(n+m))$的复杂度优化为$O(logn)$的复杂度。

初步的想法是自己实现一个可以动态确定大小的集合，底层用`uint64_t`实现。

经过调研后，了解到C++标准库中支持`bitset`，已经有了非常优雅的实现。但是标准库中的`bitset`要求集合的大小在编译时确定，也就是大小必须是一个常数。这和我们必须要在读入`n`后才能确定集合的大小有所冲突。

为了找到能够动态确定大小的`bitset`，了解到`boost`库中的`dynamic_bitset`实现了动态确定大小的`bitset`。既然已经有珠玉在前，那么我就直接使用`boost::dynamic_bitset`作为实现集合的数据结构。

关于`boost::dynamic_bitset`常用操作，这里有一个[博客](https://izualzhy.cn/boost-dynamic-bitset)有较好的整理

## 数据类别

点的性质：

- $V_j$：表示`j`所服务的客户集合	`const`
- $C_i$：表示可以服务`i`的中心集合   `const`
- $W_i$：点`i`的权重  `mutable`
- $\delta_j$：打开/关闭中心`j`所需要的代价，用于转移$f(X)$ `mutable`，在解初始化后确定

解的性质：

- $X$：表示一个解，每个元素对应一个中心  `mutable`
  - $x_j$：是否选择第`j`个中心，在解初始化后确定
  - $u_i$：第`i`个客户是否没有被服务，在解初始化后确定
- $U(X)$：表示解$X$不能服务的客户集合  `mutable`，在解初始化后确定
- $f(X)$：表示解$X$的目标函数，越小越好（最优解为0），在解初始化后确定

搜索的性质：

- $TabuList$：禁忌表，是一个集合，里面有两个元素，可以用`pair`记录

## 输入

> 命令行参数：  运行时间上限（秒） 随机种子
>
> N（节点数）		P（中心数）
>
> N组：
>
> ​	C（集合能够覆盖的元素数量）
>
> ​		覆盖元素的编号
>
> U（覆盖半径的上界）	L（覆盖半径的下界）
>
> U - L组：
>
> ​	每缩小一次覆盖半径新增的无法覆盖的元素信息：
>
> ​	K（本次缩小半径将导致不再覆盖的元素个数） 
>
> ​		K个：集合S表示S中的最后一个元素将不被覆盖

## 输出

> P个：换行符分隔，表示挑选出的P个中心
>
> ​	S

## 编码设计

覆盖元素的编号使用`vector`进行存储

主循环需要运行至多U-L次，而且最后不用输出最长半径

先将每次覆盖半径新增的无法覆盖的元素信息存储在一个`vector`中，而后每次遍历都将覆盖元素的`vector`进行删除`pop_back`。

主循环中需要检测程序运行时间，如果距离上限比较接近就直接退出。用1s的时间打印退出，尽可能完成搜索。

## 目录结构

- `CMakeLists.txt`：CMake文件
- `src`：包括面向过程的源文件，控制算法流程
- `class`：包括面向对象的源文件，例如`Point`类、`Solution`类
- `utils`：工具函数、工具类
- `test`：测试文件
- `lib`：库文件
- `data`：数据文件夹