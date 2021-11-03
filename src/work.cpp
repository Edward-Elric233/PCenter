// Copyright(C), Edward-Elric233
// Author: Edward-Elric233
// Version: 1.0
// Date: 2021/11/2
// Description: 工作函数定义
#include "work.h"
#include "data.h"
#include "Node.h"
#include "test.h"
#include "Solution.h"
#include <climits>

using namespace std;
using dynamic_bitset = boost::dynamic_bitset<>;

namespace work {

void read(std::istream &is) {
    using namespace global_data;
    is >> n >> p;
    cover.resize(n);
    int c, t;
    for (int i = 0; i < n; ++i) {
        is >> c;
        for (int j = 0;j < c; ++j) {
            is >> t;
            cover[i].push_back(t);
        }
    }
    int u, l;
    is >> u >> l;
    r = u - l;
    uncover.resize(r);
    for (int i = 0; i < r; ++i) {
        is >> c;
        for (int j = 0; j < c; ++j) {
            is >> t;
            uncover[i].push_back(t);
        }
    }
    delta.resize(n);
}

bool init_nodes(vector<Node> &nodes) {
    for (auto &node : nodes) {
        //在每次运行前都需要将节点服务和被服务的信息清零，然后后面再录入
        node.init();
    }
    using global_data::cover;
    using global_data::n;
    for (int j = 0; j < n; ++j) {
        for (int i : cover[j]) {
            //表示中心j服务i
            nodes[j].addClient(i);
            nodes[i].addCenter(j);
        }
    }
    for (int j = 0; j < n; ++j) {
        //说明该点没有中心可以对其进行服务，所以不用运行算法，肯定是无法得到解的
        if (nodes[j].C.none()) return false;
    }
    return true;
}



Solution algorithm() {
    vector<Node> nodes(global_data::n);
    Solution Xstar;
    int ir = 0;
    while (global_data::timeChecker()) {
        if (!init_nodes(nodes)) {
            //初始化失败
            return std::move(Xstar);
        }
//        test::test_init_nodes(nodes);
        if (VMTS(nodes, Xstar)) {
//            cout << "Xstar.X:" << Xstar.X << endl;
            if (ir < global_data::r) {
                for (auto x : global_data::uncover[ir]) {
                    global_data::cover[x].pop_back();
                }
                ++ir;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    return std::move(Xstar);
}

/*!
 * 如果将节点i加入X，则会对X中的节点的delta值有所影响，之所以只考虑X中的是因为X外的其他节点目前不用考虑，一会只需要再从X中剔除一个，真正的修改通过make_move函数
 * @param nodes 节点信息
 * @param X 解信息
 * @param i 加入的节点
 */
void try_to_open_center(vector<Node> &nodes, const Solution& X, int i) {
    dynamic_bitset T(global_data::n);  //临时集合
    auto &node = nodes[i];
    for (size_t v = node.V.find_first(); v != dynamic_bitset::npos; v = node.V.find_next(v)) {
        T = X.X & nodes[v].C;
        if (T.count() == 1) {
            for (size_t l = T.find_first(); l != dynamic_bitset::npos; l = T.find_next(l)) {
                nodes[l].delta -= nodes[v].w;
            }
        }
    }
}

/*!
 * 禁忌搜索，找到邻域中的最优方向
 * @param nodes 节点信息
 * @param X 当前解
 * @param tabuList 禁忌表
 * @return {i, j}  f(X) = f(X' + swap(i, j))
 */
pair<int, int> tabu_search(vector<Node> &nodes, const Solution& X, pair<int, int> &tabuList) {
    vector<pair<int, int>> M;   //保存候选结果
    int obj = INT_MAX;
    int t = global_data::getRandom(X.U.count());
    size_t v;
//    std::cout << __FILE__ << ":" << __LINE__ << "\t\t" << X.U << std::endl;
//    std::cout << __FILE__ << ":" << __LINE__ << "\t\t" << X.U.count() << std::endl;
    for (v = X.U.find_first(); v != dynamic_bitset::npos && t--; v = X.U.find_next(v));
    for (int i = 0; i < global_data::n; ++i) global_data::delta[i] = nodes[i].delta;

    auto &node = nodes[v];
//    cout << "nodes[v].C:" << node.C << endl;
    for (size_t i = node.C.find_first(); i != dynamic_bitset::npos; i = node.C.find_next(i)) {
        if (i == tabuList.first || i == tabuList.second) continue;      //禁忌动作
        try_to_open_center(nodes, X, i);
        for (size_t j = X.X.find_first(); j != dynamic_bitset::npos; j = X.X.find_next(j)) {
            if (j == tabuList.first || j == tabuList.second) continue;  //禁忌动作
            t = X.target + nodes[i].delta - nodes[j].delta;
            if (t < obj) {
                obj = t;
                M.clear();
                M.push_back({i, j});
            } else if (t == obj) {
                M.push_back({i, j});
            }
        }
        for (int i = 0; i < global_data::n; ++i) nodes[i].delta = global_data::delta[i];    //与try_to_open_center对应
    }
    if (M.size() == 1) {
        return M[0];
    } else if (M.size() > 1) {
        return M[global_data::getRandom(M.size())];
    } else {
        throw runtime_error("tabu search error");
    }
}

/*!
 * 进行移动，并修改X、U、f(X)值和delta值
 * @param nodes
 * @param X
 * @param mv
 */
void make_move(std::vector<Node> &nodes, Solution &X, pair<int, int> mv) {
    int i = mv.first, j = mv.second;
    dynamic_bitset T(global_data::n);

    //加入i，对delta_i没有影响，对delta_j有影响
    for (size_t v = nodes[i].V.find_first(); v != dynamic_bitset::npos; v = nodes[i].V.find_next(v)) {
        T = X.X & nodes[v].C;
        if (T.count() == 1) {
            //v在X中有中心在进行服务
            for (size_t l = T.find_first(); l != dynamic_bitset::npos; l = T.find_next(l)) {
                nodes[l].delta -= nodes[v].w;
            }
        } else if (T.count() == 0) {
            //v在X中没有中心在进行服务，但是现在加上i以后就有了
            //X.U.reset(v);  //可以这样做，但是太麻烦，不如直接在U中删去Vi
            for (size_t l = nodes[v].C.find_first(); l != dynamic_bitset::npos; l = nodes[v].C.find_next(l)) {
                if (l == i) continue;
                nodes[l].delta -= nodes[v].w;
            }
        }
    }
    //对于新加入i，可以在U中直接删去Vi，因为就算有其他中心服务了Vi中的节点也不会有影响（已经删除过了）
    X.U &= ~nodes[i].V;
    X.X.set(i).reset(j);
    //但是对于新删除的j，不能直接在U中加上Vj，因为如果有其他中心服务了Vj中的节点，不能直接认为删去j就没有其他节点服务了

    X.target = X.target + nodes[i].delta - nodes[j].delta;

    //删除j，对delta_j没有影响，但是对delta_i有影响
    for (size_t v = nodes[j].V.find_first(); v != dynamic_bitset::npos; v = nodes[j].V.find_next(v)) {
        T = X.X & nodes[v].C;
        if (T.size() == 0) {
            //v在X中没有中心在服务，删除j以后所有可以服务v的中心都可以服务了
            X.U.set(v); //以前是可以服务v的，现在不行了
            for (size_t l = nodes[v].C.find_first(); l != dynamic_bitset::npos; l = nodes[v].C.find_next(l)) {
                if (l == j) continue;
                nodes[l].delta += nodes[v].w;
            }
        } else if (T.size() == 1) {
            //v在X中有中心在服务，删除j以后他们就成唯一的了
            for (size_t l = T.find_first(); l != dynamic_bitset::npos; l = T.find_next(l)) {
                nodes[l].delta += nodes[v].w;
            }
        }
    }

}

bool VMTS(std::vector<Node> &nodes, Solution &Xstar) {
    Solution X(nodes);  //初始化
    Solution X_(X);     //保存X的一个拷贝
    if (X.U.none()) {
        //X是一个可行解，不用再进行搜索，直接将其保存到Xstar中
//        cout << "X.X:" << X.X << endl;
//        cout << "X.U:" << X.U << endl;
        Xstar = X;
        return true;
    }
    Solution Xstar_(X); //保存当前最优解
    pair<int, int> tabuList = {-1, -1};
    int iter = 1;
    while (global_data::timeChecker()) {
        auto mv = tabu_search(nodes, X, tabuList);
        make_move(nodes, X, mv);
        if (X.U.count() < Xstar_.U.count()) {
            Xstar_ = X;
            if (Xstar_.U.none()) {
                Xstar = Xstar_;
                return true;
            }
        } else if (X.U.count() >= X_.U.count()) {
            for (size_t v = X.U.find_first(); v != dynamic_bitset::npos; v = X.U.find_next(v)) {
                ++nodes[v].w;
            }
        }
        tabuList = mv;
        X_ = X; ++iter;
    }
    return false;
}


void write(const Solution &X) {
    for (size_t j = X.X.find_first(); j != dynamic_bitset::npos; j = X.X.find_next(j)) {
        cout << j << "\n";
    }
}

}
