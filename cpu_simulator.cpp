#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

int main(void) {


    //CPUが実行する命令
    vector<string> program = {
        "LDI 3",
        "STO a",
        "LDI 2",
        "STO b",
        "LOAD a",
        "ADD b",
        "STO c",
        "HALT"
    };

    //レジスタ
    int A = 0;

    //メモリ
    int a = 0;
    int b = 0;
    int c = 0;

    //プログラムカウンタ
    int PC = 0;

    while (true) {


        string instruction = program.at(PC);

        //<sstream>のstringstreamを用いれは "LDI 3"を"LDI"と3にできる
        stringstream ss(instruction);

        string command;
        int value;
        ss  >> command >> value;

        cout << "PC = " << PC << endl;
        cout << "命令 = " << instruction << endl;

        //LDI命令の場合
        if (instruction == "LDI") {
            A = value;
        }

        PC++;

        if (PC > program.size()) {
            break;
        }
    }

    cout << "A= " << A << endl;

    return 0;

}