#include "data.h"
#include "work.h"
#include "test.h"
#include "Solution.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <libgen.h>     //windows下basename的头文件

using namespace std;

int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);    //统一使用C++流式输出，关闭与stdio的同步
    cin.tie(nullptr);   //关闭cin和cout的绑定，加速流

    if (argc != 3) {
        cout << "[wrong arguments]: you should run program like ["
            << basename(argv[0]) << " 运行时间上限(s) 随机数种子" << endl;
        exit(1);
    }

    global_data::timeChecker.setTimeLimit(atoi(argv[1]));
    global_data::getRandom.setSeed(atoi(argv[2]));

//    ifstream is("../Instance/pmed01.n100p005.txt");
//    ifstream is("../data/data3.in");
    work::read(cin);
//    work::read(is);
//    test::test_read();
    auto X = work::algorithm();
//    cout << X.X << endl;
    work::write(X);

    return 0;
}
