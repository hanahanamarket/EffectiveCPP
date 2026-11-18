# 設問

C++11 のクラスには、条件を満たすとコンパイラが用意する特殊メンバ関数が6つある。デフォルトコンストラクタ、デストラクタ、コピーコンストラクタ、コピー代入演算子、ムーブコンストラクタ、ムーブ代入演算子である。どの関数をユーザがクラス定義に書いたかで、残りの関数が呼び出せるか、宣言されないか、delete されるかが決まる。

各問は独立している。コードは C++11 の規則で判定し、必要なヘッダはインクルード済みとする。コンパイラの最適化は考えない。警告が出ても、規則上呼び出せる関数は呼び出せるものとする。

このファイルでは次を共通の規則とする。

- ユーザ宣言とは、その関数の宣言をクラス定義の中に書くことである。関数本体をその場に書く宣言、`= default`、`= delete` は、いずれもユーザ宣言である
- 宣言されていない関数は、オーバーロード解決の候補にならない
- delete された関数は候補になる。選ばれた関数が delete なら、その呼び出しは不適格である
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

A. 適格である。`b` の初期化も `c` への代入もムーブされ、`a.name` は空、`b.name` と `c.name` は `"widget"` になる

B. 適格である。`b` の初期化はコピーコンストラクタ、`c` への代入はコピー代入演算子であり、`a.name` も `b.name` も `c.name` も `"widget"` のままである

C. `b` の初期化は適格でコピーになるが、`c` への代入は不適格である

D. 不適格である。ムーブコンストラクタとムーブ代入演算子が delete として宣言され、コピーより優先して選ばれる

---

## (2)

`Widget` がユーザ宣言している特殊メンバ関数は、デフォルトコンストラクタとムーブコンストラクタだけである。文1と文2の適格性は独立に判定する。

```cpp
#include <string>
#include <utility>

class Widget {
public:
    Widget() {}

    Widget(Widget&& rhs)  // ムーブコンストラクタを宣言。ムーブ代入演算子は宣言しない。
        : name(std::move(rhs.name)) {}

private:
    std::string name;  // コピーもムーブも可能。name を理由に代入演算子は delete されない
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

A. 文1も文2も不適格である

B. 文1も文2も適格である。文1はコピー代入演算子、文2はムーブ代入演算子を呼ぶ

C. 文1は不適格である。文2は適格で、ムーブ代入演算子を呼ぶ

D. 文1は適格で、コピー代入演算子を呼ぶ。文2は不適格である

---

## (3)

追加前の `StringTable` は次のとおりである。追加前の `StringTable b(std::move(a));` はムーブコンストラクタを呼び、`values` をムーブする。

```cpp
#include <map>
#include <string>
#include <utility>

class StringTable {
public:
    StringTable() {}
private:
    std::map<int, std::string> values;  // コピーもムーブも可能。delete されていない
};

int main()
{
    StringTable a;
    StringTable b(std::move(a));  // 追加前: ムーブコンストラクタ
}
```

追加後は、ログ用のデストラクタだけをユーザ宣言する。判定するのは、追加後のクラスによる初期化だけである。

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
    std::map<int, std::string> values;  // コピーもムーブも可能。delete されていない
};

int main()
{
    StringTable a;
    StringTable b(std::move(a));  // 追加後: 判定対象
}
```

追加後のこの初期化として最も適切なものはどれか。

A. 追加前と同じく、ムーブコンストラクタが呼ばれ、`values` はムーブされる

B. 不適格である。デストラクタをユーザ宣言すると、コピーコンストラクタは delete される

C. コピーコンストラクタが呼ばれ、`values` はコピーされる。ムーブコンストラクタは宣言されない

D. ムーブコンストラクタが呼ばれ、`values` はムーブされる。コピーコンストラクタは delete される

---

## (4)

`Base` のデストラクタは virtual でなければならない。`name` のコピーとムーブは、どちらも可能である。

```cpp
class Base {
public:
    virtual ~Base() = default;
private:
    std::string name;
};
```

このクラスを変更し、次の3つを同時に満たしたい。

1. デストラクタは virtual のままである
2. コピーコンストラクタとコピー代入演算子を呼べる。呼ぶと `name` はコピーされる
3. ムーブコンストラクタとムーブ代入演算子を呼べる。呼ぶと `name` はムーブされる

この問では次を仮定する。

- デフォルトコンストラクタが生成されるかは、正誤に影響しない
- 右辺値をコピーコンストラクタやコピー代入演算子で受けることは、要件3を満たさない
- クラス定義に `= default` と書いたコピー操作やムーブ操作は、`name` を理由に delete されない。同じクラスでムーブ操作も `= default` にしていても、`= default` と書いたコピー操作は delete されない
- `= default` のコピー操作が呼ばれると `name` はコピーされ、`= default` のムーブ操作が呼ばれると `name` はムーブされる

最も適切な変更はどれか。

A. 宣言を追加しない

B. 次の4つを追加する

```cpp
Base(const Base&) = default;
Base& operator=(const Base&) = default;
Base(Base&&) = default;
Base& operator=(Base&&) = default;
```

C. 次の2つを追加する

```cpp
Base(Base&&) = default;
Base& operator=(Base&&) = default;
```

D. 次の2つを追加する

```cpp
Base(const Base&) = default;
Base& operator=(const Base&) = default;
```
