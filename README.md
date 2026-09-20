# CPU Instruction Simulator

授業で使用しているCPUの命令セットをC++で再現するための学習用CPUシミュレータ。

## 目的

CPUの命令がどのように実行されるのかを理解する。

## 実行の仕方

cd \Users\kawam\cpu_instruction_simulator\cpp

g++ -finput-charset=UTF-8 -fexec-charset=UTF-8 cpu_simulator.cpp -o cpu_simulator
-oは出力する実行ファイルの名前を指定するというオプション。
-finput-charset=UTF-8でC++のソースコードをUTF-8として読み込む
-finput-charset=UTF-8でこのソースコードはUTF-8で書かれていると認識
このコマンドで実行ファイルcpu_simulator.exeを作成する。

<!-- .\cpu_simulator.exe
行ファイルを実行する -->

node run_cpu.jsでrun_cpu.jsを起動

## 足し算プログラム
5 + 3 = 8をするプログラム
6
LDI 5
STO 0
LDI 3
ADD 0
WRITE
HALT

## ループプログラム
Aの値を3から1ずつ減らし、0になるまで繰り返す。
6
LDI 3
WRITE
DEC
JZERO 6
JMP 1
HALT

## サブルーチンプログラム
BSS でサブルーチンへ移動し、BRSA で元の場所へ戻る。
6
LDI 5
BSS 4
WRITE
HALT
INC
BRSA