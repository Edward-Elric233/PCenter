// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/2
// Description: 
#ifndef PCENTER_SOLUTION_H
#define PCENTER_SOLUTION_H

#include "boost/dynamic_bitset.hpp"
#include "Node.h"
#include <vector>

class Solution {
    using dynamic_bitset = boost::dynamic_bitset<>;
public:
    int target;     //目标函数解f(X)
    dynamic_bitset X;  //解的中心集合
    dynamic_bitset U;  //解无法服务的客户集合
    Solution(): X(global_data::n), U(global_data::n) {}
    /*!
     * 对解进行初始化操作
     * @param nodes 保存节点信息，需要对delta进行初始化
     */
    void init(std::vector<Node> &nodes);
    /*!
     * 如果传入nodes参数，则使用init函数进行初始化
     * @param nodes
     */
    Solution(std::vector<Node> &nodes);
};


#endif //PCENTER_SOLUTION_H
