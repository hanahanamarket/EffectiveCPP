# 設問

ユニヴァーサル参照とは、テンプレートの型引数 `T` を推論する仮引数 `T&&` である。左辺値を渡すと、仮引数の型はその左辺値の型への左辺値参照になり、`const` があれば `const` も残る。この仮引数をとる関数を、別の引数をとる同名の関数と同じオーバーロード集合に置くと、仮引数の型が引数と完全に一致する側が、変換や `const` の付加が必要な側より優先されることがある。

各問は独立している。コードは C++11 として扱い、必要なヘッダはインクルード済みとする。各問に書いた関数以外のオーバーロードは存在しない。オーバーロードの解決は、次の規則だけを使う。

- 候補になるのは、その引数で仮引数を初期化できる関数だけである。関数本体がその引数で適格かどうかは、候補の判定にも適合度の比較にも使わない。
- 非const 左辺値を同じ型の非const 左辺値参照で受ける束縛と、const 左辺値を同じ型の const 左辺値参照で受ける束縛を、完全一致と呼ぶ。
- 完全一致は、非const 左辺値を同じ型の const 左辺値参照で受ける束縛より適合度が高い。
- 完全一致は、`short` の左辺値を値渡しの `int` で受ける束縛より適合度が高い。
- 完全一致は、派生クラスの左辺値を基底クラスの const 左辺値参照で受ける束縛より適合度が高い。
- 適合度が異なるときは、最も高い候補が選ばれる。この比較では、非テンプレート関数か関数テンプレートかを使わない。
- 適合度が同じ候補が複数あるときは、非テンプレート関数が関数テンプレートより優先される。呼び出しは曖昧にならない。
- 選ばれた関数が関数テンプレートのとき、その特殊化だけが実体化される。本体が不適格なら、その呼び出しを含むプログラムは不適格である。選ばれなかった候補は実体化されず、次点の候補が代わりに選ばれることもない。

---

## (1)

`names.emplace` は、渡された引数から `std::string` を構築して格納する。`short` から `std::string` を構築するコンストラクタはなく、その構築は不適格である。

`nameFromIdx` の定義は別の場所にあり、`int` を渡せば `std::string` を返す。`logAndAdd(int)` が選ばれた場合、外側の呼び出しは適格である。その本体から行われる内側の呼び出しは、この問では判定しない。

`nameIdx` は非const の左辺値である。この引数に対するユニヴァーサル参照の仮引数の型は `short&` である。候補は、このユニヴァーサル参照のオーバーロードと `logAndAdd(int)` だけである。

```cpp
std::multiset<std::string> names;

std::string nameFromIdx(int idx);

template<typename T>
void logAndAdd(T&& name) {
    names.emplace(std::forward<T>(name));
}

void logAndAdd(int idx) {
    logAndAdd(nameFromIdx(idx));
}

short nameIdx = 0;
logAndAdd(nameIdx);
```

`logAndAdd(nameIdx)` について最も適切なものはどれか。

A. `short` から `int` への変換が必要でも、非テンプレート関数は関数テンプレートより常に優先される。`logAndAdd(int)` が選ばれ、呼び出しは適格である

B. 仮引数 `short&` と仮引数 `int` の適合度は同じであり、非テンプレートの `logAndAdd(int)` が選ばれる。呼び出しは適格である

C. 仮引数 `short&` の方が適合度が高い。ユニヴァーサル参照のオーバーロードが選ばれ、`short` から `std::string` を構築できないため、呼び出しは不適格である。`logAndAdd(int)` は選ばれない

D. 仮引数 `short&` の方が適合度が高いが、その本体が不適格なので解決がやり直され、`logAndAdd(int)` が選ばれる。呼び出しは適格である

---

## (2)

下の `Person(const Person&)` は非テンプレートである。これが選ばれた場合、`name` はコピーされ、その初期化は適格である。このクラスにムーブコンストラクタは存在しない。

`original` の初期化は適格であり、この問の判定対象ではない。`original` は非const の左辺値である。`Person copy(original)` は直接初期化であり、`explicit` なコンストラクタも候補になる。

この引数に対する転送コンストラクタ（`T&&` を取り、その引数で `name` を初期化するコンストラクタ）の仮引数の型は `Person&` である。`Person` から `std::string` を初期化するコンストラクタはない。候補は、転送コンストラクタと `Person(const Person&)` だけである。

```cpp
class Person {
public:
    template<typename T>
    explicit Person(T&& n) : name(std::forward<T>(n)) {}

    Person(const Person& rhs) : name(rhs.name) {}

private:
    std::string name;
};

Person original("Nancy");
Person copy(original);
```

`Person copy(original)` について最も適切なものはどれか。

A. 仮引数 `Person&` の方が、仮引数 `const Person&` より適合度が高い。転送コンストラクタが選ばれ、`Person` から `std::string` を初期化できないため、初期化は不適格である。コピーコンストラクタは選ばれない

B. 仮引数 `Person&` と仮引数 `const Person&` の適合度は同じであり、非テンプレートのコピーコンストラクタが選ばれる。`name` はコピーされ、初期化は適格である

C. 仮引数 `Person&` の方が適合度が高いが、その本体が不適格なのでコピーコンストラクタが選ばれる。`name` はコピーされ、初期化は適格である

D. 仮引数 `const Person&` の方が適合度が高い。コピーコンストラクタが選ばれ、`name` はコピーされ、初期化は適格である

---

## (3)

下の `Person(const Person&)` は非テンプレートである。これが選ばれた場合、`name` はコピーされ、その初期化は適格である。このクラスにムーブコンストラクタは存在しない。

`original` の初期化は適格であり、この問の判定対象ではない。`original` は const の左辺値である。`Person copy(original)` は直接初期化であり、`explicit` なコンストラクタも候補になる。

この引数に対する転送コンストラクタ（`T&&` を取り、その引数で `name` を初期化するコンストラクタ）の仮引数の型は `const Person&` である。`const Person` から `std::string` を初期化するコンストラクタはない。候補は、転送コンストラクタと `Person(const Person&)` だけである。

```cpp
class Person {
public:
    template<typename T>
    explicit Person(T&& n) : name(std::forward<T>(n)) {}

    Person(const Person& rhs) : name(rhs.name) {}

private:
    std::string name;
};

const Person original("Nancy");
Person copy(original);
```

`Person copy(original)` について最も適切なものはどれか。

A. 仮引数の型はどちらも `const Person&` で適合度は同じだが、関数テンプレートが非テンプレートより優先される。転送コンストラクタが選ばれ、初期化は不適格である

B. 仮引数の型はどちらも `const Person&` で適合度は同じなので、呼び出しは曖昧になる。初期化は不適格である

C. 転送コンストラクタの方が適合度が高い。転送コンストラクタが選ばれ、`const Person` から `std::string` を初期化できないため、初期化は不適格である

D. 仮引数の型はどちらも `const Person&` で適合度は同じであり、非テンプレートのコピーコンストラクタが選ばれる。`name` はコピーされ、初期化は適格である。転送コンストラクタは実体化されない

---

## (4)

`Person` のコピーコンストラクタ `Person(const Person&)` は非テンプレートである。これが選ばれた場合、基底部分の `name` はコピーされ、その初期化は適格である。`Person` にムーブコンストラクタは存在しない。

`SpecialPerson` のコンストラクタの仮引数 `rhs` を式として使うと、`const SpecialPerson` の左辺値である。`: Person(rhs)` は基底部分の直接初期化であり、`explicit` なコンストラクタも候補になる。

この引数に対する `Person` の転送コンストラクタ（`T&&` を取り、その引数で `name` を初期化するコンストラクタ）の仮引数の型は `const SpecialPerson&` である。`SpecialPerson` から `std::string` を初期化するコンストラクタはない。候補は、`Person` の転送コンストラクタと `Person(const Person&)` だけである。

```cpp
class Person {
public:
    template<typename T>
    explicit Person(T&& n) : name(std::forward<T>(n)) {}

    Person(const Person& rhs) : name(rhs.name) {}

private:
    std::string name;
};

class SpecialPerson : public Person {
public:
    SpecialPerson(const SpecialPerson& rhs)
        : Person(rhs) {}
};
```

`SpecialPerson` のコピーコンストラクタにある `: Person(rhs)` について最も適切なものはどれか。

A. 仮引数 `const SpecialPerson&` と仮引数 `const Person&` の適合度は同じであり、非テンプレートのコピーコンストラクタが選ばれる。基底部分の `name` はコピーされ、この初期化は適格である

B. 仮引数 `const SpecialPerson&` の方が適合度が高い。転送コンストラクタが選ばれ、`SpecialPerson` から `std::string` を初期化できないため、この初期化は不適格である。コピーコンストラクタは選ばれない

C. 派生クラスから基底クラスへの変換が必要でも、非テンプレートのコピーコンストラクタが転送コンストラクタより常に優先される。基底部分の `name` はコピーされ、この初期化は適格である

D. 仮引数 `const SpecialPerson&` の方が適合度が高いが、その本体が不適格なのでコピーコンストラクタが選ばれる。基底部分の `name` はコピーされ、この初期化は適格である
