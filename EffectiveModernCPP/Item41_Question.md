# 設問

コピー可能でムーブできる引数を関数が別のオブジェクトへ必ず格納するとき、値渡しは、const 左辺値参照と右辺値参照の二つのオーバーロードの代わりに検討できる受け取り方である。値渡しは実引数から仮引数を初期化し、関数本体ではその仮引数が名前のある左辺値になり、ムーブが要素を複製する型、格納しない経路、既存バッファへの代入では二つのオーバーロードよりコストが増える。

各問は独立している。コードは C++11 として扱い、必要なヘッダはインクルード済みとする。コンパイラの最適化は考えない。呼ばれると書いたコンストラクタと代入演算子は、その回数どおりに実行される。

このファイルでは次を共通の規則とする。

- `std::move(x)` は `x` を右辺値参照へキャストするだけであり、このキャスト自体はコピーもムーブもしない
- 名前のあるオブジェクトを指す式は左辺値である。`std::move` を適用した式は右辺値である
- 値渡しの仮引数は、実引数が左辺値ならコピーコンストラクタで、実引数が右辺値ならムーブコンストラクタで初期化される。この初期化は関数本体より前に行われる
- 関数本体の中では、値渡しの仮引数は名前があるので左辺値である
- `const T&` は左辺値にも右辺値にも束縛できる。`T&&` は右辺値にだけ束縛できる。参照を束縛すること自体は、コピーもムーブもしない
- オーバーロードが `const T&` と `T&&` の二つのときは、左辺値の実引数には `const T&` が選ばれ、右辺値の実引数には `T&&` が選ばれる。選ばれなかったオーバーロードの本体は実行されない

---

## (1)

`names` への追加では、vector の再確保は起きない。既に入っている要素のコピーやムーブは数えない。実引数を用意するまでのコストも数えない。

`push_back` に左辺値を渡すと、新しい要素はコピーコンストラクタで構築される。`push_back` に右辺値を渡すと、新しい要素はムーブコンストラクタで構築される。

数えるのは、値渡しの仮引数を初期化するコンストラクタと、`push_back` が新しい要素を構築するコンストラクタだけである。コピーコンストラクタ 1 回をコピー 1 回、ムーブコンストラクタ 1 回をムーブ 1 回とする。下の 4 行は、順に実行する一つのプログラムではない。各行だけを、構築直後の `src` に対して実行する。

```cpp
std::vector<std::string> names;

void addByValue(std::string newName) {
    names.push_back(std::move(newName));
}

void addByOverload(const std::string& newName) {
    names.push_back(newName);
}
void addByOverload(std::string&& newName) {
    names.push_back(std::move(newName));
}

std::string src("widget");
addByValue(src);               // V-L
addByValue(std::move(src));    // V-R
addByOverload(src);            // O-L
addByOverload(std::move(src)); // O-R
```

4 つの呼び出しの回数として最も適切なものはどれか。

A. V-L はコピー 1、V-R はムーブ 1、O-L はコピー 1、O-R はムーブ 1 である。値渡しの仮引数は vector の要素として構築され、本体の `push_back` は追加のコンストラクタを呼ばない

B. V-L はコピー 1 とムーブ 1、V-R はムーブ 2、O-L はコピー 1、O-R はムーブ 1 である。値渡しは、左辺値でも右辺値でも、対応するオーバーロードよりムーブが 1 回多い

C. V-L はコピー 2、V-R はコピー 1 とムーブ 1、O-L はコピー 1、O-R はムーブ 1 である。値渡しの仮引数へ `std::move` を書いても、その仮引数からの `push_back` はコピーになる

D. V-L はコピー 1 とムーブ 1、V-R はコピー 1 とムーブ 1、O-L はコピー 1、O-R はコピー 1 である。右辺値から値渡しする仮引数はコピーで初期化され、右辺値参照のオーバーロードの `push_back` もコピーする

---

## (2)

`names` への追加では、vector の再確保は起きない。既に入っている要素のコピーやムーブは数えない。実引数を用意するまでのコストも数えない。

`push_back` に左辺値を渡すと、新しい要素はコピーコンストラクタで構築される。`push_back` に右辺値を渡すと、新しい要素はムーブコンストラクタで構築される。

数えるのは、値渡しの仮引数を初期化するコンストラクタと、`push_back` が新しい要素を構築するコンストラクタだけである。コピーコンストラクタ 1 回をコピー 1 回、ムーブコンストラクタ 1 回をムーブ 1 回とする。下の 2 行は、順に実行する一つのプログラムではない。各行だけを、構築直後の `src` に対して実行する。

```cpp
std::vector<std::string> names;

void addByValue(std::string newName) {
    names.push_back(newName);
}

std::string src("widget");
addByValue(src);            // L
addByValue(std::move(src)); // R
```

L と R の回数として最も適切なものはどれか。

A. L はコピー 2、R はムーブ 2 である。実引数が右辺値なら、名前のある仮引数も右辺値として `push_back` に渡る

B. L はコピー 1、R はムーブ 1 である。仮引数の初期化が vector への格納そのものであり、本体の `push_back` は追加のコピーもムーブもしない

C. L はコピー 1 とムーブ 1、R はムーブ 2 である。`push_back` は、`std::move` を書かなくても値渡しの仮引数からムーブする

D. L はコピー 2、R はムーブ 1 とコピー 1 である。名前のある仮引数は左辺値なので、`std::move` していない `push_back` はコピーする

---

## (3)

`isDuplicate` は引数を判定するだけで、その仮引数 `const std::string&` への束縛はコピーもムーブもしない。この問の呼び出しでは `isDuplicate` は `true` を返す。したがって、どちらの関数でも `push_back` は実行されない。

`names` の再確保は起きない。実引数を用意するまでのコストは数えない。数えるのは、実行される仮引数の初期化と、実行される `push_back` だけである。下の 2 行は、順に実行する一つのプログラムではない。各行だけを、構築直後の `src` に対して実行する。

```cpp
bool isDuplicate(const std::string& name);

std::vector<std::string> names;

void addByValue(std::string newName) {
    if (!isDuplicate(newName))
        names.push_back(std::move(newName));
}

void addByOverload(const std::string& newName) {
    if (!isDuplicate(newName))
        names.push_back(newName);
}
void addByOverload(std::string&& newName) {
    if (!isDuplicate(newName))
        names.push_back(std::move(newName));
}

std::string src("widget");
addByValue(src);     // V
addByOverload(src);  // O
```

V と O の回数として最も適切なものはどれか。

A. V はコピー 1 でムーブ 0、O はコピー 0 でムーブ 0 である。値渡しの仮引数は、本体が格納しない場合でもコピーで初期化される

B. V も O もコピー 0 でムーブ 0 である。格納しない分岐では、値渡しの仮引数の初期化も行われない

C. V も O もコピー 1 でムーブ 0 である。`const std::string&` で受け取る左辺値は、呼び出しの時点でコピーされる

D. V はムーブ 1 でコピー 0、O はコピー 0 でムーブ 0 である。左辺値から値渡しする仮引数はムーブで初期化される

---

## (4)

この問では、新しいバッファの確保回数だけを数える。解放は数えない。二つの呼び出しは別々のプログラムであり、どちらも呼び出し前の `name` は同じ状態である。`s` は左辺値である。実引数を用意するまでのコストは数えない。

呼び出し前の状態と、この状態で各操作が確保する回数は次のとおりである。

- `name` の `capacity` は 100、`size` は 10 である。`s` の `size` は 20 である
- `s` からのコピーコンストラクタは、新しいバッファを 1 つ確保する
- ムーブコンストラクタは、新しいバッファを確保しない
- `name = s` というコピー代入は、`name.capacity()` が `s.size()` 以上なので、新しいバッファを確保しない
- `name = std::move(newName)` というムーブ代入は、新しいバッファを確保しない。この代入は、`name` が既に持っているバッファを、`newName` の文字のコピー先としては使わない
- 参照の束縛は、新しいバッファを確保しない
- 値渡しの仮引数を破棄するとき、新しいバッファは確保されない

```cpp
class Widget {
    std::string name;
public:
    void setByValue(std::string newName) {
        name = std::move(newName);
    }
    void setByConstRef(const std::string& newName) {
        name = newName;
    }
};

Widget w;
std::string s(20, 'a');
w.setByValue(s);     // この1行だけのプログラム
w.setByConstRef(s);  // この1行だけのプログラム。直前の呼び出しの結果は引き継がない
```

二つの呼び出しが新しいバッファを確保する回数として最も適切なものはどれか。

A. どちらも 0 回である。`name` の `capacity` が `s` の `size` 以上なので、値渡しの仮引数も `name` の既存バッファの中に構築される

B. `setByConstRef` だけが 1 回である。`const std::string&` 経由のコピー代入は、`capacity` が足りていても新しいバッファを確保する

C. `setByValue` だけが 1 回である。仮引数のコピーコンストラクタが確保し、その後のムーブ代入は `name` の既存バッファへ文字をコピーしない

D. `setByValue` は 2 回、`setByConstRef` は 0 回である。値渡しではコピーコンストラクタとムーブ代入が 1 回ずつ確保する

---

## (5)

`Table` にはポインタのメンバは無い。コピーコンストラクタ 1 回は、`cells` の `int` を 10000 個複製する。ムーブコンストラクタ 1 回も、`cells` の `int` を 10000 個複製する。`int` 1 個の複製は 1 個と数える。複製される個数は、実行されたコンストラクタごとの複製個数の合計である。

`items` への追加では、vector の再確保は起きない。既に入っている要素の複製は数えない。参照の束縛が複製する `int` は 0 個である。実引数を用意するまでのコストは数えない。下の 4 行は、順に実行する一つのプログラムではない。各行だけを、構築直後の `src` に対して実行する。

`push_back` に左辺値を渡すと、新しい要素はコピーコンストラクタで構築される。`push_back` に右辺値を渡すと、新しい要素はムーブコンストラクタで構築される。

```cpp
struct Table {
    int cells[10000];
};

std::vector<Table> items;

void storeByValue(Table value) {
    items.push_back(std::move(value));
}

void storeByOverload(const Table& value) {
    items.push_back(value);
}
void storeByOverload(Table&& value) {
    items.push_back(std::move(value));
}

Table src;
storeByValue(src);               // V-L
storeByValue(std::move(src));    // V-R
storeByOverload(src);            // O-L
storeByOverload(std::move(src)); // O-R
```

4 つの呼び出しで複製される `int` の個数として最も適切なものはどれか。

A. V-L は 10000、V-R は 0、O-L は 10000、O-R は 0 である。ムーブコンストラクタは `cells` の `int` を複製しない

B. V-L は 20000、V-R は 20000、O-L は 10000、O-R は 10000 である。値渡しは、左辺値でも右辺値でも、対応するオーバーロードより `int` を 10000 個多く複製する

C. V-L は 30000、V-R は 20000、O-L は 20000、O-R は 10000 である。オーバーロードの左辺値はコピーコンストラクタ 1 回とムーブコンストラクタ 1 回を呼び、値渡しの左辺値はそれに加えてコピーコンストラクタがもう 1 回呼ばれる

D. V-L は 10000、V-R は 20000、O-L は 10000、O-R は 20000 である。右辺値のときは、値渡しも右辺値参照のオーバーロードも、ムーブコンストラクタを 2 回呼ぶ

---

## (6)

`Handle` のコピーコンストラクタは `= delete` である。ムーブコンストラクタは存在し、`= delete` ではない。

この問では次を仮定する。

- 左辺値から値渡しの仮引数を初期化するとき選ばれるのはコピーコンストラクタであり、ムーブコンストラクタは選ばれない
- 右辺値から値渡しの仮引数を初期化するとき選ばれるのはムーブコンストラクタである
- 選ばれたコンストラクタが `= delete` なら、その呼び出しは不適格である。不適格な呼び出しでは、仮引数の初期化は完了せず、関数本体も実行されない
- 左辺値を `Handle&&` に束縛する呼び出しは不適格である。不適格な呼び出しでは関数本体は実行されない
- 右辺値を `Handle&&` に束縛することはでき、その束縛はムーブコンストラクタを呼ばない
- `push_back` に右辺値を渡すと、新しい要素はムーブコンストラクタで構築される
- `items` の再確保は起きない。ムーブコンストラクタ 1 回をムーブ 1 回と数える。実引数を用意するまでのコストは数えない

下の 4 行は、順に実行する一つのプログラムではない。各行だけを、構築直後の `h` に対して実行する。

```cpp
struct Handle {
    Handle();
    Handle(const Handle&) = delete;
    Handle(Handle&&);
};

std::vector<Handle> items;

void storeByValue(Handle value) {
    items.push_back(std::move(value));
}

void storeByRvalueRef(Handle&& value) {
    items.push_back(std::move(value));
}

Handle h;
storeByValue(h);                 // 文1
storeByValue(std::move(h));      // 文2
storeByRvalueRef(h);             // 文3
storeByRvalueRef(std::move(h));  // 文4
```

4 つの文の結果として最も適切なものはどれか。

A. 4 つとも適格である。文1 はムーブ 1、文2 はムーブ 1、文3 はムーブ 0、文4 はムーブ 1 である。左辺値の値渡しではムーブコンストラクタが選ばれる

B. 文1 と文3 は不適格である。文2 はムーブ 1、文4 はムーブ 1 である。値渡しの仮引数の初期化が、vector の要素の構築を兼ねる

C. 文1 は不適格、文3 は適格でムーブ 1 である。文2 はムーブ 2、文4 はムーブ 2 である。右辺値参照の仮引数もムーブコンストラクタで初期化される

D. 文1 と文3 は不適格である。文2 はムーブ 2、文4 はムーブ 1 である。どちらの受け取り方も左辺値は受け取れず、右辺値では値渡しの方がムーブコンストラクタが 1 回多い
