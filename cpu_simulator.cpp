#include <iostream>

using namespace std;

int main(void) {

    //レジスタ
    int A = 0;

    //メモリ
    int a = 0;
    int b = 0;
    int c = 0;

    //LDI 3
    A = 3;

    //STO a
    a =A;

    //LDI 2
    A = 2;

    //STO b
    b = A;

    //LOAD a
    A = a;

    //ADD b
    A = A + b;

    //STO c
    c = A;

    //HALT
    cout << "停止しました" << endl;

    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "c = " << c << endl;

    return 0;
}