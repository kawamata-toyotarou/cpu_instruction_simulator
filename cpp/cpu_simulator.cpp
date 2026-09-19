#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <windows.h>
#include <limits>
#include <stdexcept>

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


/*tryとcatchとは*/
//tryとはエラーが起こるかもしれない処理のついて記述し、実際にエラーが起こった時はcatchの処理が行われる
bool try_parse_int(const string& value, int& result) {
    try {
        size_t pos;

        result = stoi(value, &pos);

        // 文字列全体が数字であるか確認
        if (pos != value.size()) {
            return false;
        }

        return true;
    }

    catch (const invalid_argument&) {  //invalid_argumentとは渡された引数が不正であるということ
        return false;
    }

    catch (const out_of_range&) {      //out_of_rangeとは渡された引数が範囲を超えているということ
        return false;
    }
}

bool execute_instruction(const string& command, const string& value, int& A, int& PC, vector<int>& memory, vector<int>& stack) {
    
    //LDI命令の場合
    if (command == "LDI") {

        if (value.empty()) {
            cout << "error 値がありません。" << endl;
            return false;
        }

        int number;
        
        if (!try_parse_int(value, number)) {
            cout << "エラー: LDIの値が数値ではありません: " << value << endl;
            return false;
        }

        A = number;
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

        int number;

        if (!try_parse_int(value, number)) {
            cout << "エラー: JMPの値が数値ではありません: " << value << endl;
            return false;
        }

        PC = number;

    }

    //STO命令の場合
    else if (command == "STO") {

        if (value.empty()) {
            cout << "error 値がありません。" << endl;
            return false;
        }

        int number;

        if (!try_parse_int(value, number)) {
            cout << "エラー: STOの値が数値ではありません: " << value << endl;
            return false;
        }

        int address = number;

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

        int number;

        if (!try_parse_int(value, number)) {
            cout << "エラー: LOADの値が数値ではありません: " << value << endl;
            return false;
        }

        int address = number;

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

        int number;

        if (!try_parse_int(value, number)) {
            cout << "エラー: ADDの値が数値ではありません: " << value << endl;
            return false;
        }

        int address = number;

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

        int number;

        if (!try_parse_int(value, number)) {
            cout << "エラー: SUBの値が数値ではありません: " << value << endl;
            return false;
        }

        int address = number;

        if (address < 0 || address >= memory.size()) {
            cout << "error メモリアドレスが範囲外です。" << endl;
            return false;
        }

        A = A - memory.at(address);
        PC++;
    }


    //read命令の追加
    else if (command == "READ") {
        cout << "数値を入力してください: ";

        if (!(cin >> A)) {
            cout << "エラー: 数値を入力してください。" << endl;

            cin.clear();                                         //cinがエラー状態になったとき、そのエラー状態を解除する。
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //ignoreは入力ストリームに残っている文字を読み飛ばす numeric_limits<streamsize>::max()はstreamsize 型で扱える最大値を取得する

            return false;
        }

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

        int number;

        if (!try_parse_int(value, number)) {
            cout << "エラー: JZEROの値が数値ではありません: " << value << endl;
            return false;
        }

        if (A == 0) {
            PC = number;
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

        int number;

        if (!try_parse_int(value, number)) {
            cout << "エラー: JGTZの値が数値ではありません: " << value << endl;
            return false;
        }

        if (A > 0) {
            PC = number;
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

        int number;

        if (!try_parse_int(value, number)) {
            cout << "エラー: BSSの値が数値ではありません: " << value << endl;
            return false;
        }

        stack.push_back(PC + 1);

        PC = number;

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

void print_state_json(int A, int PC, const vector<int>& memory, const vector<int>& stack, bool running, const string& message) {
    cout << "{";
    cout << "\"A\":" << A << ",";
    cout << "\"PC\":" << PC << ",";

    cout << "\"memory\":[";
    for (int i = 0; i < memory.size(); i++) {
        cout << memory[i];
        if (i != memory.size() - 1) {
            cout << ",";
        }
    }
        
    cout << "],";
    
    cout << "\"stack\":[";
    for (size_t i = 0; i < stack.size(); i++) {
        cout << stack[i];
        if (i != stack.size() - 1) cout << ",";
    }
    cout << "],";

    cout << "\"running\":" << (running ? "true" : "false") << ",";
    cout << "\"message\":\"" << message << "\"";
    cout << "}" << endl;
    
}

int main(void) {

                                 //CP_UTF8は、WindowsでUTF-8を表す定数。
    SetConsoleOutputCP(CP_UTF8); //Windowsコンソールに出力する文字のコードページをUTF-8に設定
    SetConsoleCP(CP_UTF8);       //Windowsコンソールから入力される文字のコードページをUTF-8に設定

    //CPUが実行する命令を入力
    vector<string> program;

    int N;
    cin >> N;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < N; i++) {
        string instruction;
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

    // 初期状態を1行出力
    print_state_json(A, PC, memory, stack, true, "初期状態");

    while (true) {

        if (PC < 0 || PC >= (int)program.size()) {
            print_state_json(A, PC, memory, stack, false, "PCが範囲外です。");
            break;
        }

        string instruction = program.at(PC);

        //<sstream>のstringstreamを用いれは "LDI 3"を"LDI"と3にできる
        stringstream ss(instruction);

        string command;
        string value;
        ss  >> command >> value;

        bool running = execute_instruction(command, value, A, PC, memory, stack);

        if (!running) {
            print_state_json(A, PC, memory, stack, false, command);
            break;
        }

        print_state_json(A, PC, memory, stack, true, command);

    }

    return 0;

}