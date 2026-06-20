# 設問
## (1)

次の関数を考える。

```cpp
#include <iostream>
#include <string>

using namespace std;

std::string encryptPassword(const std::string& password)
{
    std::string encrypted;
    if (password.length() < 8)
    {
        throw std::logic_error("too short");
    }

    encrypted = password;
    return encrypted;
}

int main()
{
    cout << encryptPassword("password123") << endl;
    return 0;
}
```

このコードを改善する理由として最も適切なものはどれか。

A. `encrypted` を後ろで定義すると、例外発生時に不要なオブジェクト生成を避けられる

B. `encrypted` を後ろで定義すると、return文が不要になる

C. `encrypted` を後ろで定義すると、例外が発生しなくなる

D. `encrypted` を後ろで定義すると、コピーコンストラクタが呼ばれなくなる

---

## (2)

次の `Widget` 型を考える。

```cpp
class Widget {
public:
    Widget();                 // デフォルト構築
    Widget(int value);        // valueで初期化
    Widget& operator=(int);   // intの代入
};

Widget::Widget() {}
Widget::Widget(int value) {}
Widget& Widget::operator=(int value) {
    return *this;
}

int main(void) {
    #_____________
}
```

mainのプレースホルダーに関して、次の2つのコードを比較する。

### コードA

```cpp
Widget w;
w = 10;
```

### コードB

```cpp
Widget w(10);
```

コードBが優れている理由として最も適切なものはどれか。

A. コードBではデストラクタが呼ばれないため

B. コードBではデフォルト構築後の代入を避け、目的の値で直接初期化できるため

C. コードBではオブジェクト生成自体が行われないため

D. コードBではスコープが狭くなるため

---

## (3)

次の `Widget` 型と `makeWidget` 関数を考える（`makeWidget(i)` は `i` に対応する `Widget` を返す）。

```cpp
class Widget {
public:
    Widget();                          // デフォルト構築
    Widget(const Widget& rhs);         // コピー構築
    Widget& operator=(const Widget&);  // 代入
    ~Widget();                         // 破棄
};
```

また、

- デフォルト構築、コピー構築、代入、破棄にはそれぞれコストがかかる
- 1回の代入のコストは、1回のコピー構築と1回の破棄を合わせたコストよりずっと小さい
- ループ回数 `n` は大きい正の整数である
- コンパイラ最適化は考えない

ものとする。

次の2つの実装を比較する。

### 方法A

```cpp
Widget w;

for (int i = 0; i < n; ++i)
{
    w = makeWidget(i);
}
```

### 方法B

```cpp
for (int i = 0; i < n; ++i)
{
    Widget w(makeWidget(i));
}
```

この前提のもとで、**性能面から**より適切な実装はどれか。

A. 方法A。ループ外の1つの `Widget` に繰り返し代入するため、コピー構築と破棄の回数を抑えられる

B. 方法B。各反復で `w` の有効範囲がループ本体に限定されるため

C. 方法Aと方法B。どちらも同じ回数のコピー構築と破棄が発生するため

D. 方法B。デフォルト構築を省略できるため