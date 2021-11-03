// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/2
// Description: 
#ifndef PCENTER_RANDOM_H
#define PCENTER_RANDOM_H

#include <random>

class Random {
    std::default_random_engine engine;
    std::uniform_int_distribution<size_t> distribution;
public:
    explicit Random(int seed_ = time(nullptr)):engine(seed_) {}
    size_t operator() (int maxn = -1) {
        return maxn == -1 ? distribution(engine) :  distribution(engine, decltype(distribution)::param_type(0, maxn));
    }
    void setSeed(int seed_) {
        engine.seed(seed_);
    }
};


#endif //PCENTER_RANDOM_H
