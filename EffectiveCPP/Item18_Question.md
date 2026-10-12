# 設問
## 設問1
次の `Date` クラスには、クライアントが誤用しやすい問題があります。<br>
この設計の本質的な問題として最も適切なものを選択肢から選んでください。
```cpp
class Date {
    public:
        Date(int month, int day, int year);
};
```

- A. 引数名が `month` / `day` / `year` なので、呼び出し側の引数の取り違えはコンパイル時に防げる
- B. `int` が「月」「日」「年」という意味を表現できていない
- C. `Date` が struct ではなく class になっている
- D. `year` 最初に置くべきだから

---

## 設問2
次の `Month` 型には問題があります。

```cpp
class Month {
    public:
        explicit Month(int m);
};
```

このままだと`Month m(13);`とすることで、12月以上の月を生成できてしまいます。<br>
存在してよい `Month` の値だけを生成可能にするという目的に最も適した設計はどれですか。


A.
```cpp
class Month {
public:
    explicit Month(int m) {
        assert(1 <= m && m <= 12);
        month_ = m;
    }
private:
    int month_;
};
```

B.
```cpp
enum Month {Jan = 1, Feb, ... , Dec };
```

C.
```cpp
class Month {
public:
    static Month Jan() { return Month(1); }
    static Month Feb() { return Month(2); }

    ... // 省略
    
    static Month Dec() { return Month(12); }
private:
    explicit Month(int m) : month_(m) {}
    int month_;
};
```

---

## 設問3

次の `Money`クラスとクライアントコードがある。

```cpp
class Money {
public:
    explicit Money(int yen) : yen_(yen) {}
    int value() const { return yen_; }
    friend Money operator+(const Money& lhs, int rhs) {
        return Money(lhs.value() + rhs);
    }
private:
    int yen_;
};
```

```cpp
Money price(1000);
Money a = price + 100;   // コンパイルできる
Money b = 100 + price;   // コンパイルできない
```

このコードにおける設計上の問題点として、最も適切なものを選択肢から選んでください。

- A. `explicit` コンストラクタのため `100 + price` がコンパイルできない
- B. `operator+` が `Money + int` にしか定義されておらず、`int + Money` が書けないなど、組み込み型の `+` のような対称性・一貫性がない
- C. `value()` が `const` なので金額を変更できない
- D. `Money` が class なので組み込み型のように扱えない

---

## 設問4

次のコードでは、DLL A で `new` されたオブジェクトを DLL B で `delete` している。

```cpp
// DLL A
Investment* createInvestment() {
    return new StockInvestment();
}

// DLL B
Investment* p = createInvestment();
// ... p を使う ...
delete p;
```

次の2つの変更のうち、このクロスDLL問題を避けるものとして適切なものを選んでください。
なお、クロスDLL問題とは、DLL A から DLL B に作用させるような状況で、オブジェクトを作ったDLLと破棄するDLLが違うと、`new` / `delete` の組み合わせが壊れて、未定義動作やクラッシュすることを指します。

A.
```cpp
// DLL A
std::shared_ptr<Investment> createInvestment() {
    Investment* p = new StockInvestment();
    auto deleter = [](Investment* ptr) { delete ptr; };
    return std::shared_ptr<Investment>(p, deleter);
}

// DLL B
std::shared_ptr<Investment> p = createInvestment();
```

B.
```cpp
// DLL A
Investment* createInvestment() {
    return new StockInvestment();
}

// DLL B
std::shared_ptr<Investment> p(createInvestment());
```
