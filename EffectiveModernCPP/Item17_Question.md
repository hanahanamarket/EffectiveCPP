# 設問

C++11 のクラスには、条件を満たすとコンパイラが用意する特殊メンバ関数が6つある。デフォルトコンストラクタ、デストラクタ、コピーコンストラクタ、コピー代入演算子、ムーブコンストラクタ、ムーブ代入演算子である。どの関数をユーザがクラス定義に書いたかで、残りの関数が呼び出せるか、宣言されないか、delete されるかが決まる。

各問は独立している。コードは C++11 の規則で判定し、必要なヘッダはインクルード済みとする。コンパイラの最適化は考えない。警告が出ても、規則上呼び出せる関数は呼び出せるものとする。

このファイルでは次を共通の規則とする。

- ユーザ宣言とは、その関数の宣言をクラス定義の中に書くことである。関数本体をその場に書く宣言、`= default`、`= delete` は、いずれもユーザ宣言である
- 宣言されていない関数は、オーバーロード解決の候補にならない
- delete された関数は候補になる。選ばれた関数が delete なら、その呼び出しはコンパイルエラーとなる
- `std::move(x)` は `x` を右辺値参照へキャストするだけであり、このキャスト自体はメンバをムーブしない
- 各クラスに、コードへ書いた以外の基底、メンバ、関数宣言は無い。参照メンバや const メンバは無い
- コピーが呼ばれたあと、コピー元の `std::string` と `std::map` は変化しない。このファイルでは、ムーブが呼ばれたあと、ムーブ元は空になると決める

---

## (1)

`Widget` がユーザ宣言している特殊メンバ関数は、次のコピーコンストラクタだけである。`Widget(std::string)` は特殊メンバではない。

```cpp
#include <iostream>
#include <string>
#include <utility>

class Widget {
public:
    explicit Widget(std::string n)  // 特殊メンバではない
        : name(std::move(n)) {}

    Widget(const Widget& rhs)  // コピーコンストラクタを宣言。コピー代入演算子は宣言しない。
        : name(rhs.name) {}

    const std::string& getName() const {
        return name;
    }

private:
    std::string name;
};

int main()
{
    Widget a("widget");
    Widget b(std::move(a));  // 右辺値からの初期化
    Widget c("other");
    c = std::move(a);        // 右辺値からの代入

    std::cout << a.getName() << '\n';
    std::cout << b.getName() << '\n';
    std::cout << c.getName() << '\n';
}
```

この `main` の結果として最も適切なものはどれか。

A. コンパイルできる。`b` の初期化も `c` への代入もムーブされ、`a.name` は空、`b.name` と `c.name` は `"widget"` になる

B. コンパイルできる。`b` の初期化はコピーコンストラクタ、`c` への代入はコピー代入演算子であり、`a.name` も `b.name` も `c.name` も `"widget"` のままである

C. `b` の初期化はコンパイルできコピーになるが、`c` への代入はコンパイルエラーとなる

D. コンパイルエラーとなる。ムーブコンストラクタとムーブ代入演算子が delete として宣言され、コピーより優先して選ばれる

---

## (2)

`Widget` がユーザ宣言している特殊メンバ関数は、デフォルトコンストラクタとムーブコンストラクタだけである。文1と文2は独立に、コンパイルエラーになるかを判定する。

```cpp
#include <string>
#include <utility>

class Widget {
public:
    Widget() {}

    Widget(Widget&& rhs)  // ムーブコンストラクタを宣言。ムーブ代入演算子は宣言しない。
        : name(std::move(rhs.name)) {}

private:
    // std::string はコピーもムーブも代入もできる。
    // 代入演算子が delete になるとしても、原因はメンバ name ではなく、特殊メンバの規則である。
    std::string name;
};

int main()
{
    Widget a;
    Widget b;

    b = a;             // 文1
    b = std::move(a);  // 文2
}
```

文1と文2の結果として最も適切なものはどれか。

A. 文1も文2もコンパイルエラーとなる

B. 文1も文2もコンパイルできる。文1はコピー代入演算子、文2はムーブ代入演算子を呼ぶ

C. 文1はコンパイルエラーとなる。文2はコンパイルでき、ムーブ代入演算子を呼ぶ

D. 文1はコンパイルでき、コピー代入演算子を呼ぶ。文2はコンパイルエラーとなる

---

## (3)

次の `StringTable` がある。このクラスでは、`StringTable b(std::move(a));` はムーブコンストラクタを呼び、`values` をムーブする。

```cpp
#include <map>
#include <string>
#include <utility>

class StringTable {
public:
    StringTable() {}
private:
    // values の型自体はコピーもムーブもできるので、
    // 問題になるのは StringTable 側の特殊メンバの規則だけと考える
    std::map<int, std::string> values;
};

int main()
{
    StringTable a;
    StringTable b(std::move(a));  // ムーブコンストラクタの呼び出し
}
```

このクラスに、ログ用のデストラクタだけをユーザ宣言すると次のようになる。デストラクタの定義はログを1行出すだけで、`values` は変更しない。

```cpp
#include <iostream>
#include <map>
#include <string>
#include <utility>

class StringTable {
public:
    StringTable() {}

    ~StringTable() {  // ログ用。values は変更しない
        std::cout << "destroyed\n";
    }

private:
    // values の型自体はコピーもムーブもできるので、
    // 問題になるのは StringTable 側の特殊メンバの規則だけと考える
    std::map<int, std::string> values;
};

int main()
{
    StringTable a;
    StringTable b(std::move(a));
}
```

このとき、`StringTable b(std::move(a));` として最も適切なものはどれか。

A. デストラクタを加える前と同じく、ムーブコンストラクタが呼ばれ、`values` はムーブされる

B. コンパイルエラーとなる。デストラクタをユーザ宣言すると、コピーコンストラクタは delete される

C. コピーコンストラクタが呼ばれ、`values` はコピーされる。ムーブコンストラクタは宣言されない

D. ムーブコンストラクタが呼ばれ、`values` はムーブされる。コピーコンストラクタは delete される

---

## (4)

文書エディタを想定する。次の穴埋め `(★)` を埋め、`main` がすべて意図どおり動くようにする。右辺値はムーブで受けること（コピーでは不可）。`virtual` なデストラクタは残す。

この問では次を仮定する。`title` の型 `std::string` はコピーもムーブもできる。参照メンバや const メンバは無い。そのため、`= default` と書いたコピー／ムーブは delete にならず、呼べば `title` はそれぞれコピー／ムーブされる。同じクラスでムーブも `= default` にしても、コピーの `= default` が delete になることはない。

```cpp
#include <string>
#include <utility>

class Document {
public:
    Document() {}
    virtual ~Document() = default;  // ユーザ宣言
    // (★)
private:
    std::string title;
};

class TextDocument : public Document {};

int main()
{
    Document draft;
    Document backup = draft;              // 退避用コピー。title はコピー
    Document scratch;
    scratch = backup;                     // 作業用へのコピー代入。title はコピー

    Document outgoing(std::move(draft));  // 送信キューへムーブ必須。title はムーブ
    scratch = std::move(outgoing);        // キューへムーブ代入必須。title はムーブ

    Document* p = new TextDocument;
    delete p;  // 派生を基底ポインタで破棄するため virtual のまま
}
```

`(★)` に入れるものとして最も適切なのはどれか。

A. 何も書かない

B.
```cpp
Document(const Document&) = default;
Document& operator=(const Document&) = default;
Document(Document&&) = default;
Document& operator=(Document&&) = default;
```

C.
```cpp
Document(Document&&) = default;
Document& operator=(Document&&) = default;
```

D.
```cpp
Document(const Document&) = default;
Document& operator=(const Document&) = default;
```
