# CPU Instruction Simulator

授業で使用しているCPUの命令セットをC++で再現するための学習用CPUシミュレータ。

## 目的

CPUの命令がどのように実行されるのかを理解する。

## 実行の仕方
g++ -finput-charset=UTF-8 -fexec-charset=UTF-8 cpu_simulator.cpp -o cpu_simulator
-oは出力する実行ファイルの名前を指定するというオプション。
-finput-charset=UTF-8でC++のソースコードをUTF-8として読み込む
-finput-charset=UTF-8でこのソースコードはUTF-8で書かれていると認識
このコマンドで実行ファイルcpu_simulator.exeを作成する。

.\cpu_simulator.exe
行ファイルを実行する