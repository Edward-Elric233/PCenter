// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/2
// Description: 
#include "Solution.h"
#include <vector>
#include <iostream>
#include <stdexcept>
#include <unordered_set>

void Solution::init(std::vector<Node> &nodes) {
    X.reset(); U.set();
    std::vector<int> s;
    dynamic_bitset T(global_data::n);  //临时集合
    dynamic_bitset X_(global_data::n); //X_ = ~X，用来记录未访问的中心
    X_.set();
    int val;    //val = arg max(V & U)
    int x;      //选择的节点
    int count;  //保存count的返回值
    for (int iter = 0; iter < global_data::p; ++iter) {
        //选择p个中心
        s.clear();
        val = -1;
        for (size_t j = X_.find_first(); j != dynamic_bitset::npos; j = X_.find_next(j)) {
            //中心j还没有被选择
            T = nodes[j].V & U;
//            std::cout << T << std::endl;
            //这里出现了bug，原因是count的返回值是unsigned类型的，和-1进行比较后出现了问题
            count = T.count();
            if (count > val) {
                //更好的节点出现了，清空候选集合，并重新记录
                s.clear();
                s.push_back(j);
                val = T.count();
            } else if (count == val) {
                //和目前最优节点相同
                s.push_back(j);
            }
        }

        if (s.size() > 1) {
            //多于1个候选，需要随机选择一个
            x = s[global_data::getRandom(s.size())];
        } else if (s.size() == 1) {
            x = s[0];
        } else {
            //应该到不了这个条件的
            throw std::runtime_error(std::string("init error in ") + std::string(__FILE__) + ":" + std::to_string(__LINE__));
        }

        X.set(x);
        X_.reset(x);
        //U = U - Vx = U & (~Vx)
        U &= ~nodes[x].V;
    }

//    std::cout << __FILE__ << ":" << __LINE__ << "\t\t" << X << std::endl;

    //初始化f(X)函数值
    target = 0;
    for (size_t j = U.find_first(); j != dynamic_bitset::npos; j = U.find_next(j)) {
        target += nodes[j].w;
    }

    //初始化delta值
    for (int j = 0; j < global_data::n; ++j) {
        auto &node = nodes[j];
        node.delta = 0;
        if (X.test(j)) {
            //中心j在解X中，delta值 = 去掉中心j后，不能服务的节点w之和
//            X_ = X.reset(j);    //X_ = X \ {j}
            T.set();
            for (size_t i = X.find_first(); i != dynamic_bitset::npos; i = X.find_next(i)) {
                if (i == j) continue;
                //计算T = U(X\{j})
                T &= ~nodes[i].V;
            }
            T &= node.V;    // T = V & U(X\{j}
            for (size_t i = T.find_first(); i != dynamic_bitset::npos; i = T.find_next(i)) {
                node.delta += node.w;
            }
        } else {
            //中心j不在解X中，delta值 = 新增加节点的w之和
            T = node.V & U;
            for (size_t i = T.find_first(); i != dynamic_bitset::npos; i = T.find_next(i)) {
                node.delta += node.w;
            }
        }
    }
//    std::cout << __FILE__ << ":" << __LINE__ << "\t\t" << X << std::endl;
}

Solution::Solution(std::vector<Node> &nodes):Solution() {
    init(nodes);
}
