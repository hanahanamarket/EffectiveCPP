# 3項 可能ならいつでも `const` を使おう

## Things to Remember
- `const` をつけて宣言すると、コンパイラがそのオブジェクトの誤用を検出してくれる。
- `const` は、あらゆるスコープのオブジェクト、関数の仮引数と戻り値、メンバ関数自体につけることができる。
- コンパイラは `const` に対し、「ビットレベルの不変性」を保証するが、「論理的不変性」を保証するようにコードを書くべきである。
- `const` と `非const` なメンバ関数で本質的に同じ実装をする必要がある場合、`非const` なメンバ関数内で `const` なメンバ関数を呼び出し、コードの重複を避けることができる。

## オブジェクトの値を変更するつもりがないのであれば、`const` をつけるべき
- `const` : "変更してはいけない" という意図的な制約を他のプログラマやコンパイラにはっきりさせることで、コンパイラがその制約を守ってくれるようにできる。
- `constexpr` (`const expression`) : リテラル型（整数型、浮動小数点）、リテラル型の配列、リテラル型のみをメンバ変数として持つクラスに対して `constexpr` を使うと、コンパイル時に値を決定するので、多少効率が良い（C++11 以降）。配列の要素数などにも使える。

## `const` の使用範囲
- クラスの外では、グローバルな定数、あるいは名前空間での定数の宣言
- ファイル、関数、ブロックの `static` オブジェクト
- クラス内では、`static`・`非static` 両方のデータメンバ、メンバ関数
- ポインタ

### ポインタと `const`
- `const` が `*` の左にあるとき、「ポインタが指し示すデータ」が不変
- `const` が `*` の右にあるとき、「ポインタそのもの」が不変

```cpp
int a = 42;
int *p1 = &a;              // ポインタは非 const、データも非 const
const int *p2 = &a;        // ポインタは非 const、データは const
int const *p2 = &a;        // 同上
int * const p3 = &a;       // ポインタは const、データは非 const
const int * const p4 = &a; // ポインタは const、データも const

int b = 0;
p2 = &b;   // 可
*p2 = b;   // 不可
p3 = &b;   // 不可
*p3 = b;   // 可
p4 = &b;   // 不可
*p4 = b;   // 不可
```

- `const` の右側にあるものをあとから変えられない、と理解できる
  - `const int *p2 = &a` は `*p2 = hoge` が不可
  - `int * const p3 = &a` は `p3 = hoge` が不可

```cpp
int** x = nullptr;
const int* const* const a = x;
const int* const* b = x;
int* const* c = x;
int** const d = x;

// a = nullptr; *a = nullptr; **a = 1; // 不可
b = nullptr;        // ok
// *b = nullptr; **b = 1;             // 不可
c = nullptr; *c = 1; // ok
// **c = nullptr; // 不可
*d = nullptr; **d = 1; // ok
// d = nullptr; // 不可
```

### STL 反復子 `iterator` と `const`
- `const` 付き反復子
  - `const` が `*` の右にある、場合と似た挙動
  - ポインタは `const`、データは `非const`
- `const_iterator`
  - `const` が `*` の左にある、場合と似た挙動
  - ポインタは `非const`、データは `const`

```cpp
std::vector<int> v{0,1};

const std::vector<int>::iterator iter = v.begin();
*iter = 2; // 可
// ++iter; // 不可

std::vector<int>::const_iterator cIter = v.cbegin();
// *cIter = 3; // 不可
++cIter; // 可
```

#### 注意: `using` を用いた場合
```cpp
using PInt = int*;
int main() {
    const PInt ptr = nullptr; // これは int* const と同義になる！
    int a = 1;                // PInt const も同様
    ptr = &a;                 // 不可
}
```

### 関数宣言と `const`

#### 仮引数
- ユーザー定義型などは `const` 参照渡し（20項）
  - 参照渡しの方がコピーしない分、効率が良い。変更させたくない・変更するつもりがない場合には `const` をつける
- 組み込み型は値渡し
  - コピーで渡すので、関数宣言時にわざわざ `const` をつける必要はない
  - 一方、定義においては、関数内で仮引数を書き換えないという意味で `const` をつけてもよい
  - 宣言と定義でシグネチャが変わるのを嫌がる人もいる（古いコンパイラでは定義にジャンプできなくなったりする）

#### 戻り値
- 安全性や効率を犠牲にすることなく、クライアント（関数の利用者）がその関数を誤用する可能性を低くできる
- 有理数を表すクラス `Rational` を考え、`*` 演算子を次のように宣言できる。

```cpp
class Rational { ... };
const Rational operator*(const Rational& lhs, const Rational& rhs);
```

- `*` の結果がなぜ `const` なのか？
  - `const` でないと次のようなコードが可能となってしまう（タイポミス）

```cpp
Rational a, b, c;
if (a * b = c){}    // あっと比較のつもりだったのに
```

- もし `Rational` が `bool` に暗黙の型変換をできてしまえばエラーは起きず実行可能となってしまう
- `a * b` が組み込みの場合、上のコードはコンパイルエラーとなる（error: lvalue required as left operand of assignment）
- 一般にプログラマが自分で定義した型は、組み込み型と異なる振る舞いをするべきではない（13項参照）。したがって、`const` をつけるべき。

## `const` なメンバ関数
- `const` なメンバ関数は重要
1. クラスのインターフェースを理解しやすくなる  
   どのメンバ関数がオブジェクトを変更し、どのメンバ関数が変更しないか、という区別を容易につけられる。
2. `const` なオブジェクトに対して使える  
   関数の仮引数では `const` 参照を使ったほうが効率的（20項）  
   このテクニックのためには、`const` な参照を操作するための `const` なメンバ関数 が必要

- オーバーロードで、`const` のあるなしだけが異なる2つのバージョンのメンバ関数を書くことができる

```cpp
class TextBlock{
public:
    const char& operator[](std::size_t position) const { return text[position]; }

    char& operator[](std::size_t position) { return text[position]; }

private:
    std::string text;
};
```

- このように2つのバージョンの `[]` 演算子を書くと、`const` でない `TextBlock` に対しても、`const` な `TextBlock` に対しても、`[]` 演算子を使える

```cpp
TextBlock tb("Hello");
std::cout << tb[0];    // 非const TextBlock::operator[]

const TextBlock ctb("Hello");
std::cout << ctb[0];   // const TextBlock::operator[]
```

- 実際のプログラムでは、`const` 参照渡しで、`const` なオブジェクトが使われるケースが最も多い。

```cpp
void print(const TextBlock& ctb){ // この関数内で ctb は const
    std::cout << ctb[0];          // const TextBlock::operator[]
}
```

## ビットレベルの不変性と論理的な不変性

### 1. 「ビットレベルでの不変性」
- メンバ関数がオブジェクトの（`static` でない）データメンバを変更しない（オブジェクト内部の1ビットも変更しない）ときに限って、そのメンバ関数に `const` をつけるべき。
- 長所: コンパイラがルール違反を知らせてくれる。
- 短所: 「ビットレベルでの不変性」を保証しながらも「論理的な不変性」を保証しない。

```cpp
#include <iostream>

class CTextBlock{
public:
    CTextBlock(char* inText) : mpText(inText) {}

    char& operator[](std::size_t position) const {
        return mpText[position];
    }

private:
    char *mpText;
};
```

### 2. 「論理的な不変性」
- `const` なメンバ関数は、仮にオブジェクトのビットを少し変更したとしても、そのクライアントにわからないようにすればよい。

```cpp
class CTextBlock{
public:
    ...
    std::size_t length() const;
private:
    char *pText;
    std::size_t textLength;   // 最後に調べられた時の文字列の長さ
    bool lengthIsValid;       // 長さが変更されていないか
};
```

### `mutable` の使用
- `mutable` は `static` でないデータメンバを「ビットレベルの不変性」から解き放つ。

```cpp
class CTextBlock{
public:
    ...
    std::size_t length() const;
private:
    char *pText;
    mutable std::size_t textLength;
    mutable bool lengthIsValid;
};
```

## `const` なメンバ関数と `非const` なメンバ関数の実装の重複を取り除く

```cpp
class TextBlock{
public:
    ...
    const char& operator[](std::size_t position) const{
        ...
        return text[position];
    }

    char& operator[](std::size_t position) {
        return
            const_cast<char&>(
                static_cast<const TextBlock&>(*this)
                    [position]
            );
    }

private:
    std::string text;
};
```

