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
        string value;
        ss  >> command >> value;

        cout << "PC = " << PC << endl;
        cout << "命令 = " << instruction << endl;
        cout << "値 = " << value << endl;

        //LDI命令の場合
        if (command == "LDI") {
            A = stoi(value);
        }

        //STO命令の場合
        else if (command == "STO") {
            
            if (value == "a") {
                a = A;
            }
            else if (value == "b") {
                b = A;
            }
            else if (value == "c") {
                c = A;
            }
        }

        //LOAD命令の場合
        else if (command == "LOAD") {

            if (value == "a") {
                A = a;
            }
            else if (value == "b") {
                A = b;
            } 
            else if (value == "c") {
                A = c;
            }
        }

        //ADD命令
        else if (command == "ADD") {

            if (value == "a") {
                A = A + a;
            }
            else if (value == "b") {
                A = A + b;
            }
            else if (value == "c") {
                A = A + c;
            }
            
        }

        PC++;

        if (PC >= program.size()) {
            break;
        }
    }

    cout << endl;

    cout << "A = " << A << endl;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "c = " << c << endl;

    return 0;

}