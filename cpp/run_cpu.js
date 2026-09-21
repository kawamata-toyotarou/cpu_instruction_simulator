//child_process とはNode.js標準モジュールの child_process は、Node.jsから別のプログラム(プロセス)を起動するための機能
//child_processにあるchild_process.spawn関数を用いている。
const child_process = require("child_process");
const spawn = child_process.spawn;

// CPUに実行させたい命令を配列(C++でいうvector<string>)として順番に並べる
const program = [
    "LDI 5",
    "STO 0",
    "LOAD 0",
    "WRITE",
    "HALT"
];

// program(命令の配列)を受け取り、CPUを実行してその結果(states)を返す関数
function runCpu(program) {

    //Promiseとは他のプログラムの処理が実行された後に行われるもので処理が成功した時はresolve、失敗した時はreject
    //./cpu_simulator.exeの実行が終わるまで時間がかかるので、「終わったら教えてね」という予約をしている
    return new Promise((resolve, reject) => {

        const cpu = spawn("./cpu_simulator.exe");          //spawn("./cpu_simulator.exe") で、実際に ./cpu_simulator.exe を別プロセスとして起動

        const states = [];                       //states は、C++から送られてきたJSONを解析した結果を貯めていく空の配列
        let buffer = "";                         //まだ処理していない生の文字列データを一時的にためておく変数

        cpu.stdout.on("data", (chunk) => {       //cpu.stdoutは、C++プログラムが標準出力に出したデータで.on("data", 関数) はデータが送られてくるたびに、この関数を実行してねという予約
            buffer += chunk.toString("utf8");    //chunkはバイナリなので.toString("utf8")で文字列に変換してbufferに追加

            let lines = buffer.split("\n");      //.split("\n") は、文字列を改行文字で分割して配列にするメソッド

            buffer = lines.pop();                //.pop() は配列の末尾の要素を取り出して削除するメソッド
                                                  //最後の要素は改行がまだ来ていない書きかけの行かもしれないのでbufferに戻しておく

            for (const line of lines) {          //lines 配列の中身を1つずつ line に入れて繰り返す(C++の範囲forと同じ)
                const trimmed = line.trim();     //.trim() は、文字列の前後の余分な空白や改行を取り除くメソッド
                if (trimmed === "") continue;    //空行は何もせず次のループへ(===は「代入」ではなく「比較」)

                try {
                    const state = JSON.parse(trimmed);  //JSON形式の文字列をJavaScriptのオブジェクトに変換
                    states.push(state);                 //変換できたものだけstates配列の末尾に追加
                } catch (e) {
                    console.log("(skip) 非JSON行:", trimmed); //JSONとして読めなかった行(errorなど)は無視してスキップ
                }
            }
        });   // ← "}"の後に")"をつけてon("data", 関数)の呼び出しを正しく閉じる

        // C++のエラー出力も一応見えるようにする
        cpu.stderr.on("data", (chunk) => {
            console.error("stderr:", chunk.toString("utf8"));  //console.log のエラー版、内容はほぼ同じで、見た目の色が変わる
        });

        // C++プロセスが終了したら、貯めたstatesを返す
        cpu.on("close", (code) => {                //cpu.on("close", 関数) はC++プログラムが終了したら、この関数を実行してねという予約
            resolve(states);                        //Promiseに「成功として、statesという結果を渡す」と伝える
        });

        // C++プログラムの起動自体に失敗した場合(ファイルが見つからない等)
        cpu.on("error", (err) => {
            reject(err);                            //Promiseに「失敗した」と伝える
        });

        cpu.stdin.write(program.length + "\n");     // cpu.stdin は、C++プログラムの標準入力。.write(...) で文字列を書き込む
                                                      // 1行目として命令数(program.length)を送る。C++側の「cin >> N;」に対応

        // 2行目以降: 命令を1行ずつ標準入力へ書き込む(C++側のgetline(cin, instruction)に対応)
        for (const line of program) {
            cpu.stdin.write(line + "\n");
        }

        // 入力終了(これを呼ばないと、READ命令などでcinが永遠に待ち続ける)
        cpu.stdin.end();
    });
}

// 実行してみる
runCpu(program).then((states) => {                  //.then(関数) は「Promiseの処理が成功して結果(states)が来たらこれを実行してね」
    console.log("=== 取得できた状態一覧 ===");
    states.forEach((s, i) => {                       //forEachは配列の全要素に対して繰り返す。sは中身、iはインデックス番号
        console.log(`[${i}]`, s);                    //`[${i}]`はテンプレートリテラル。バッククォート内の${}に変数の値が埋め込まれる
    });
});