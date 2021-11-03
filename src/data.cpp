// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/2
// Description: 全局数据定义文件

#include "data.h"

namespace global_data {
    int n;   //节点数
    int p;   //中心数
    int time;    //程序运行上限
    int r;   //半径长度 = U - L
    std::vector<std::vector<int>> cover; //合能够覆盖的元素
    std::vector<std::vector<int>> uncover;   //每缩小一次覆盖半径新增的无法覆盖的元素
    std::vector<int> delta;  //delta辅助空间，用来寄存delta
    Random getRandom;    //随机数类
    TimeChecker timeChecker; //检查是否还在运行时间上限内，如果不是则退出程序
}
