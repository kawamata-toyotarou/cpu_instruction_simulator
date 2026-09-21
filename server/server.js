//HTTPサーバーを簡単に作れるライブラリであるExpressというフレームワークを読み込む
const express = require("express");

const child_process = require("child_process");
const spawn = child_process.spawn;

//フォルダの位置を正しく指定するためにバスを扱うための標準モジュールを読み込む
const path = require("path");

//Expressのアプリケーション、サーバー本体を作る
const app = express();

//ポート番号
const PORT = 3000;

//JSON形式のリクエストボディを読み取れるようにする設定
app.use(express.json());

//publicフォルダの中身をそのままブラウザに公開する設定
app.use(express.static(path.join(__dirname, "..", "public")));

// c++を実行して状態一覧を返す関数
function runCpu(program) {
    return new Promise((resolve, reject) => {
        // cpu_simulator.exeの場所を、server.jsから見た相対パスで正しく指定する
        const exePath = path.join(__dirname, "..", "cpp", "cpu_simulator.exe");
        const cpu = spawn(exePath);

        const states = [];
        let buffer = "";
        cpu.stdout.on("data", (chunk) => {
            buffer += chunk.toString("utf8");

            let lines = buffer.split("\n");
            buffer = lines.pop();

            for (const line of lines) {
                const trimmed = line.trim();
                if (trimmed === "") continue;

                try {
                    const state = JSON.parse(trimmed);
                    states.push(state);
                } catch (e) {
                    console.log("(skip) 非JSON行:", trimmed);
                }
            }
        });

        cpu.stderr.on("data", (chunk) => {
            console.error("stderr:", chunk.toString("utf8"));
        });

        cpu.on("close", (code) => {
            resolve(states);
        });

        cpu.on("error", (err) => {
            reject(err);
        });

        cpu.stdin.write(program.length + "\n");
        for (const line of program) {
            cpu.stdin.write(line + "\n");
        }
        cpu.stdin.end();
    });
}

//POST /executeというAPIを作る
app.post("/execute", async (req, res) => {     //app.postはこのURLにPOSTというやり方でリクエストが来たら、この関数を実行してねという予約、async (req, res)がその実行される関数
    try {
        const program = req.body.program;      //ブラウザから送られてきた命令配列を取り出す

        if (!Array.isArray(program) || program.length === 0) {        //Array.isArray(program) はprogramが本当に配列かどうかをtrue/falseで返す関数
            return res.status(400).json({ error:"programが不正です"}); //res.status(400) は、ブラウザに400番のエラーという合図を返す設定
        }

        const states = await runCpu(program);                         //C++を実行し、結果が返ってくるまで待つ
        res.json({ states});
    } catch (err) {
        res.status(500).json({error: err.message});                   //500はサーバー側で何か問題が起きましたという意味のHTTP番号
    }
});

app.listen(PORT, () => {                                              //app.listenを呼んで初めて、サーバーが実際に動き出す。
    console.log(`サーバー起動: http://localhost:${PORT}`);
});