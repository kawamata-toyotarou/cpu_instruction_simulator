#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <windows.h>
#include <limits>

/*標準ライブラリ関数忘れた時用メモ*/
//push_back() データを末尾に追加
//pop_back() 末尾のデータを削除
//back() 末尾のデータを取得
//size() データの個数を取得
//empty() 空かどうかを確認
//sort() データを並べ替え
//reverse() 順番を逆にする
//ignore() 入力の中に残っている不要な文字を読み飛ばす
//getline() 1行全体を読み取る

using namespace std;

void show_cpu_state(int A, int PC, const vector<int> & memory, const vector<int> & stack) {
    cout << endl;
    cout << "========== CPUの状態 ==========" << endl;

    //レジスタ
    cout << "レジスタ" << endl;
    cout << "A  : " << A << endl;
    cout << "PC : " << PC << endl;

    // メモリ
    cout << endl;
    cout << "メモリ" << endl;

    for (int i = 0; i < memory.size(); i++) {
        cout << "memory[" << i << "] = " << memory.at(i) << endl;
    }

    // スタック
    cout << endl;
    cout << "スタック" << endl;

    if (stack.empty()) {
        cout << "空です。" << endl;
    }
    else {
        for (int i = 0; i < stack.size(); i++) {
            cout << "stack[" << i << "] = " << stack.at(i) << endl;
        }
    }

    cout << "===============================" << endl;
}

bool execute_instruction(const string& command, const string& value, int& A, int& PC, vector<int>& memory, vector<int>& stack) {
    
    //LDI命令の場合
    if (command == "LDI") {

        if (value.empty()) {
            cout << "error 値がありません。" << endl;
            return false;
        }

        A = stoi(value);
        PC++;

    }

    //INC命令の実行
    else if (command == "INC") {
        A = A + 1;
        PC++;
    }

    //DEC命令の実行
    else if (command == "DEC") {
        A = A - 1;
        PC++;
    }

    //JMPの追加
    else if (command == "JMP") {

        if (value.empty()) {
            cout << "error 値がありません。" << endl;
            return false;
        }

        PC = stoi(value);

    }

    //STO命令の場合
    else if (command == "STO") {

        if (value.empty()) {
            cout << "error 値がありません。" << endl;
            return false;
        }

        int address = stoi(value);

        if (address < 0 || address >= memory.size()) {
            cout << "error メモリアドレスが範囲外です。" << endl;
                return false;
        }

        memory.at(address) = A;
        PC++;

    }

    //LOAD命令の場合
    else if (command == "LOAD") {

        if (value.empty()) {
            cout << "error 値がありません。" << endl;
            return false;
        }

        int address = stoi(value);

        if (address < 0 || address >= memory.size()) {
            cout << "error メモリアドレスが範囲外です。" << endl;
            return false;
        }

        A = memory.at(address);
        PC++;
    }
 
    //ADD命令
    else if (command == "ADD") {

        if (value.empty()) {
            cout << "error 値がありません。" << endl;
            return false;
        }

        int address = stoi(value);

        if (address < 0 || address >= memory.size()) {
            cout << "error メモリアドレスが範囲外です。" << endl;
            return false;
        }

        A = A + memory.at(address);
        PC++;
    }

    //SUB命令の場合
    else if (command == "SUB") {

        if (value.empty()) {
            cout << "error 値がありません。" << endl;
            return false;
        }

        int address = stoi(value);

        if (address < 0 || address >= memory.size()) {
            cout << "error メモリアドレスが範囲外です。" << endl;
            return false;
        }

        A = A - memory.at(address);
        PC++;
    }


    //read命令の追加
    else if (command == "READ") {
        cin >> A;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        PC++;
    }

    //write命令の追加
    else if (command == "WRITE") {
        cout << A << endl;
        PC++;
    }

    //JZERO命令の実装
    else if (command == "JZERO") {

        if (value.empty()) {
            cout << "error 値がありません。" << endl;
            return false;
        }

        if (A == 0) {
            PC = stoi(value);
        }
        else {
            PC++;
        }

    }

    //JGTZ命令
    else if (command == "JGTZ") {

        if (value.empty()) {
            cout << "error 値がありません。" << endl;
            return false;
        }

        if (A > 0) {
            PC = stoi(value);
        }
        else {
            PC++;
        }
    }

    //BSS命令
    else if (command == "BSS") {

        if (value.empty()) {
            cout << "error 値がありません。" << endl;
            return false;
        }

        stack.push_back(PC + 1);

        PC = stoi(value);

    }

    //BRSA命令
    else if (command == "BRSA") {

        if (stack.empty()) {
            cout << "error 戻り先が存在しません" << endl;
            return false;
        }

        PC = stack.back();
        stack.pop_back();

    }

    //HALT命令の場合
    else if (command == "HALT") {
        cout << "CPU停止" << endl;
        return false;
    }

    else {
        cout << "error 未対応の命令です。" << endl;
        return false;
    }

    return true;
}

int main(void) {

                                 //CP_UTF8は、WindowsでUTF-8を表す定数。
    SetConsoleOutputCP(CP_UTF8); //Windowsコンソールに出力する文字のコードページをUTF-8に設定
    SetConsoleCP(CP_UTF8);       //Windowsコンソールから入力される文字のコードページをUTF-8に設定

    //CPUが実行する命令を入力
    vector<string> program;

    int N;
    cout << "命令数を入力してください";
    cin >> N;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "命令を入力してください" << endl;

    for (int i = 0; i < N; i++) {
        string instruction;

        cout << i << ": ";
        getline(cin, instruction);

        program.push_back(instruction);
    }

    //レジスタ
    int A = 0;

    //メモリ
    vector<int> memory(10, 0);

    // 戻り先を保存するスタック
    vector<int> stack;

    //プログラムカウンタ
    int PC = 0;

    while (true) {

        if (PC < 0 || PC >= program.size()) {
            cout << "PCが範囲外です。" << endl;
            break;
        }

        string instruction = program.at(PC);

        //<sstream>のstringstreamを用いれは "LDI 3"を"LDI"と3にできる
        stringstream ss(instruction);

        string command;
        string value;
        ss  >> command >> value;

        cout << "PC = " << PC << endl;
        cout << "命令 = " << instruction << endl;
        cout << "値 = " << value << endl;
        cout << "Enterキーを押すと実行します..." << endl;
        cin.get();

        bool running = execute_instruction(command, value, A, PC, memory, stack);

        if (!running) {
            break;
        }

        show_cpu_state(A, PC, memory, stack);

    }

    cout << endl;
    
    return 0;

}