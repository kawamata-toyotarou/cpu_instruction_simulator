#include <iostream>
#include <vector>
#include <string>

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

        cout << "PC = " << PC << endl;
        cout << "命令 = " << instruction << endl;

        //LDI命令の場合
        if (instruction == "LDI 3") {
            A = 3;
        }

        PC++;

        if (PC > program.size()) {
            break;
        }
    }

    cout << "A= " << A << endl;

    return 0;

}