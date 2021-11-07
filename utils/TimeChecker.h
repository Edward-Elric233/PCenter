// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/2
// Description: 函数对象，检查是否还在运行时间上限内
#ifndef PCENTER_TIMECHECKER_H
#define PCENTER_TIMECHECKER_H

#include <chrono>

class TimeChecker {
    using system_clock = std::chrono::system_clock;
    system_clock::time_point start, end;
    using milliseconds = std::chrono::milliseconds;
    static constexpr int RESERVE = 1;   //预留1毫秒的时间用于程序退出
public:
    TimeChecker(int sec_ = 10):start(system_clock::now()) {
        end = start + milliseconds(sec_ * 1000 - RESERVE);
    }
    void setTimeLimit(int sec_) {
        end = start + milliseconds(sec_ * 1000 - RESERVE);
    }
    bool operator() () const {
        return system_clock::now() < end;
    }
};


#endif //PCENTER_TIMECHECKER_H
