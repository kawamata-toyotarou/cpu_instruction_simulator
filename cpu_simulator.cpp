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

    for (int i = 0; i < program.size(); i++) {
        cout << program.at(i) << endl;
    }

    return 0;
}