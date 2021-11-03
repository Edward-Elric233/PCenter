// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/2
// Description: 工作函数声明

#ifndef PCENTER_WORK_H
#define PCENTER_WORK_H

#include "Node.h"
#include "Solution.h"
#include <iostream>
#include <vector>

namespace work {

/*!
 * 从isteam中读入数据
 * @param is 输入流，默认为标准输入
 */
void read(std::istream &is = std::cin);

/*!
 * 开始运行算法
 * 每次运行首先都要对nodes进行初始化，然后运行VMTS算法，如果半径还可以收缩就继续运行下一次，否则退出循环
 */
Solution algorithm();

/*!
 * VMTS algorithm，得到当前情况下的最优解
 * @param nodes 节点信息
 * @param Xstar 最优解，传出参数，只有成功了才会对Xstar进行修改，否则Xstar保存的是上个半径的解
 * @return 如果找到可行解返回成功，否则返回失败
 */
bool VMTS(std::vector<Node> &nodes, Solution &Xstar);

/*!
 * 将解打印输出
 * @param X
 */
void write(const Solution &X);

}

#endif //PCENTER_WORK_H
