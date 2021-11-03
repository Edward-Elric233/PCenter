// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/2
// Description: 全局数据声明文件

#ifndef PCENTER_DATA_H
#define PCENTER_DATA_H

#include "Random.h"
#include "TimeChecker.h"
#include <vector>

namespace global_data {
    extern int n;   //节点数
    extern int p;   //中心数
    extern int time;    //程序运行上限
    extern int r;   //半径长度 = U - L
    extern std::vector<std::vector<int>> cover; //合能够覆盖的元素
    extern std::vector<std::vector<int>> uncover;   //每缩小一次覆盖半径新增的无法覆盖的元素
    extern std::vector<int> delta;  //delta辅助空间，用来寄存delta
    extern Random getRandom;    //随机数类
    extern TimeChecker timeChecker; //检查是否还在运行时间上限内，如果不是则退出程序
}

#endif //PCENTER_DATA_H
