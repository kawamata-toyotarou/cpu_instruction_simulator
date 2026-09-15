#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

int main(void) {


    //CPUが実行する命令
    vector<string> program = {
    "LDI 0",
    "JZERO 4",
    "LDI 100",
    "JMP 5",
    "LDI 200",
    "WRITE",
    "HALT"
    };

    //レジスタ
    int A = 0;

    //メモリ
    vector<int> memory(10, 0);

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
            int address = stoi(value);
            memory.at(address) = A;
        }

        //LOAD命令の場合
        else if (command == "LOAD") {
            int address = stoi(value);
            A = memory.at(address);
        }

        //ADD命令
        else if (command == "ADD") {
            int address = stoi(value);
             A = A + memory.at(address);
        }

        //SUB命令の場合
        else if (command == "SUB") {
            int address = stoi(value);
            A = A - memory.at(address);
        }

        //INC命令の実行
        else if (command == "INC") {
            A = A + 1;
        }

        //DEC命令の実行
        else if (command == "DEC") {
            A = A - 1;
        }

        //read命令の追加
        else if (command == "READ") {
            cin >> A;
        }

        //write命令の追加
        else if (command == "WRITE") {
            cout << A << endl;
        }

        //JMPの追加
        else if (command == "JMP") {
            PC = stoi(value);
            continue;
        }

        //JZERO命令の実装
        else if (command == "JZERO") {
            if (A == 0) {
                PC = stoi(value);
                continue;
            }
        }

        //HALT命令の場合
        else if (command == "HALT") {
            cout << "CPU停止" << endl;
            break;
        }

        PC++;

        if (PC >= program.size()) {
            break;
        }

    }

    cout << endl;
    
    return 0;

}