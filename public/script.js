const programInput = document.getElementById("program-input");
const runButton = document.getElementById("run-button");
const stepButton = document.getElementById("step-button");
const resetButton = document.getElementById("reset-button");

const regA = document.getElementById("reg-a");
const regPC = document.getElementById("reg-pc");
const memoryView = document.getElementById("memory-view");
const stackView = document.getElementById("stack-view");
const messageView = document.getElementById("message-view");

//サーバーから受け取った全ステップ分の状態を保持しておく配列
let states = [];

//今、何ステップ目を表示しているかを覚えていく変数
let currentIndex = 0;

// addEventListenerは、指定したイベントが発生した際に実行される関数を登録するためのJavaScriptメソッド

// // ボタン要素を取得
// const button = document.getElementById('myButton');
// // クリックイベントを登録
// button.addEventListener('click', () => {
// console.log('ボタンがクリックされました！');
// });

runButton.addEventListener("click", async () => {
    
    //textareaの中身を取得
    const text = programInput.value;                  //今回はvalueはprogramInput の入力欄に今入っている値を表している　value = 値

    const program = text.split("\n").filter((line) => line.trim() !== "");   //.filter(条件) は、JavaScriptで配列から条件に合う要素だけを取り出して、新しい配列を作るメソッド

    // fetchはブラウザからサーバーにリクエストを送る関数
    // 第1引数: 送り先のURL、第2引数: リクエストの詳細設定
    const response = await fetch("/execute", {
        method: "POST",                              // サーバー側でapp.post("/execute", ...)としていたので合わせる
        headers: { "Content-Type": "application/json" }, // 「JSON形式のデータを送りますよ」という宣言
        body: JSON.stringify({ program: program })    // body は、実際にサーバーへ送るデータ JSON.stringify() は、JavaScriptのオブジェクト → JSON形式の文字列に変換するもの
    });

    //サーバーから帰ってきたJSONデータを取り出す
    const data = await response.json();  //レスポンスの中に入っているJSONデータを読み取り、JavaScriptのデータに変換する処理

    states = data.states;
    currentIndex = 0;

    //最初の状態を画面に表示する
    showState(currentIndex);
});

//次のステップボタンが押された時の処理
stepButton.addEventListener("click", () => {

    //次のステップが残っていれば1つ進める
    if (currentIndex < states.length - 1) {
        currentIndex++;
        showState(currentIndex);
    }
});

//リセットボタンが押された時の処理
resetButton.addEventListener("click", () => {
    states = [];
    currentIndex = 0;
    showState(null);
});

//指定したインデックスの状態を画面に反映する関数
function showState(index) {

    //indexがnull,またはstatesが空の場合は-を表示
    if (index === null || states.length === 0) {
        regA.textContent = "-";
        regPC.textContent = "-";
        memoryView.textContent = "-";
        stackView.textContent = "-";
        messageView.textContent = "-";
        return;
    }

    //該当ステップの状態オブジェクトを取り出す
    const state = states[index];

    // .textContent に値を入れると、画面上のその要素の文字が書き換わる
    regA.textContent = state.A;
    regPC.textContent = state.PC;
    memoryView.textContent = state.memory.join(", "); // 配列を文字列に変換
    stackView.textContent = state.stack.join(", ");
    messageView.textContent = state.message;
}