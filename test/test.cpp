// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/2
// Description: 
#include "test.h"
#include "data.h"
#include <iostream>

using namespace std;
using namespace global_data;

namespace test {
void test_read() {
    cout << n << " " << p << endl;
    for (auto &s : cover) {
        cout << s.size() << endl;
        for (auto x : s) {
            cout << x << " ";
        }
        cout << endl;
    }
    cout << r << endl;
    for (auto &s : uncover) {
        cout << s.size() << " ";
        for (auto x : s) {
            cout << x << " ";
        }
        cout << endl;
    }
}

void test_init_nodes(const std::vector<Node> &nodes) {
    for (int i = 0; i < n; ++i) {
        cout << i << ":\n" << "V: " << nodes[i].V << "\t" << "C: " << nodes[i].C << endl;
    }
}

}
