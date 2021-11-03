// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/2
// Description: 
#ifndef PCENTER_NODE_H
#define PCENTER_NODE_H

#include "boost/dynamic_bitset.hpp"
#include "data.h"
#include <vector>

class Node {
public:
    int w;  //权重
    int delta;  //打开、关闭中心所需要的代价
    boost::dynamic_bitset<> V;  //该中心所服务的客户集合
    boost::dynamic_bitset<> C;  //服务到该客户的中心集合
    /*!
     * 要求Node必须在n读入后再初始化，虽然耦合度很高，但是效率高
     */
    Node(): V(global_data::n), C(global_data::n) { }
    void init() { V.reset(); C.reset(); w = 1;}
    void addClient(int i) { V.set(i); }
    void addCenter(int j) { C.set(j); }
};


#endif //PCENTER_NODE_H
