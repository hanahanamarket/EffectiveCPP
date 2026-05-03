# Effective C++ 第9項

---

## 設問

### Q1

**以下のコードを実行したときの挙動として、正しいものはどれですか。**

```cpp
#include <iostream>

class Transaction {
public:
    Transaction()
    {
        init();
    }

    virtual void logTransaction() const = 0;  // 純粋仮想関数

private:
    void init()
    {
        logTransaction();
    }
};

void Transaction::logTransaction() const
{
    std::cout << "[Transaction] Transaction\n";  // 基底クラスのメッセージ
}


class BuyTransaction : public Transaction {
public:
    void logTransaction() const override
    {
        std::cout << "[BuyTransaction] BuyTransaction\n";  // 派生クラスのメッセージ
    }
};

int main()
{
    BuyTransaction b;  // 派生クラスをインスタンス化
}
```

- A. 基底クラスの`[Transaction] Transaction` というメッセージが表示される。
- B. 派生クラスの`[BuyTransaction] BuyTransaction` というメッセージが表示される。
- C. `[Transaction] Transaction` と `[BuyTransaction] BuyTransaction` という両方のメッセージが表示される。
- D. 実行時異常終了となる（環境により `pure virtual method called` / `terminate called without an active exception` / `Aborted` / core dump などが観察される）。

---

### Q2

**以下のコードを実行したときの挙動として、正しいものはどれですか。**

```cpp
#include <iostream>

class Transaction {
public:
    Transaction()
    {
        init();
    }

    virtual void logTransaction() const;  // (純粋でない)仮想関数

private:
    void init()
    {
        logTransaction();
    }
};

void Transaction::logTransaction() const
{
    std::cout << "[Transaction] Transaction\n";  // 基底クラスのメッセージ
}

class BuyTransaction : public Transaction {
public:
    void logTransaction() const override
    {
        std::cout << "[BuyTransaction] BuyTransaction\n";  // 派生クラスのメッセージ
    }
};

int main()
{
    BuyTransaction b;  // 派生クラスをインスタンス化
}
```

- A. 基底クラスの`[Transaction] Transaction` というメッセージが表示される。
- B. 派生クラスの`[BuyTransaction] BuyTransaction` というメッセージが表示される。
- C. `[Transaction] Transaction` と `[BuyTransaction] BuyTransaction` という両方のメッセージが表示される。
- D. 実行時異常終了となる（環境により `pure virtual method called` / `terminate called without an active exception` / `Aborted` / core dump などが観察される）。

---

### Q3

**本文のページ順に沿った「次の一手」として、`logTransaction` を仮想関数のまま残しつつ名前だけ変えた次の設計を考えます。第9項の観点から、最も適切な評価はどれですか。**

```cpp
class Transaction {
public:
    Transaction() {
        init();
    }

    virtual void init() const;
};
```

- A. `init` という名前に変えたので、第9項の問題は解消される  
- B. `init` が `const` なので、第9項の問題は解消される  
- C. 仮想関数をコンストラクタから呼ぶ構図が残っているので、本質的には改善していない  
- D. `init` はコンストラクタ補助関数という慣習名なので、派生側へ安全に飛ぶ  

---

### Q4

**本文の改善案に最も忠実なのはどれですか。**

- A.

```cpp
class Transaction {
public:
    Transaction() {
        logTransaction();
    }

    virtual void logTransaction() const;
};
```

- B.

```cpp
class Transaction {
public:
    explicit Transaction(const std::string& logInfo) {
        logTransaction(logInfo);
    }

    void logTransaction(const std::string& logInfo) const;
};

class BuyTransaction: public Transaction {
public:
    BuyTransaction(parameters)
        : Transaction(createLogString(parameters))
    {}

private:
    static std::string createLogString(parameters);
};
```

- C.

```cpp
class Transaction {
public:
    explicit Transaction(const std::string& logInfo) {
        logTransaction(logInfo);
    }

    virtual void logTransaction(const std::string& logInfo) const;
};
```

- D.

```cpp
class Transaction {
public:
    Transaction() {
        logTransaction(createLogString());
    }

    virtual std::string createLogString() const;
    virtual void logTransaction(const std::string& logInfo) const;
};
```

---

### Q5

**次のコードについて、`createLogString` を `static` にしている理由として最も適切なものはどれですか。**

```cpp
class Transaction {
public:
    explicit Transaction(const std::string& logInfo) {
        logTransaction(logInfo);
    }

    void logTransaction(const std::string& logInfo) const;
};

class BuyTransaction: public Transaction {
public:
    BuyTransaction(parameters)
        : Transaction(createLogString(parameters))
    {}

private:
    static std::string createLogString(parameters);
};
```

- A. `static` にすると、その関数は仮想関数のように動的束縛される  
- B. `static` にすると、まだ完全に構築されていない `BuyTransaction` オブジェクト状態に依存せずにログ文字列を作れる  
- C. `static` にすると、基底クラスの private メンバへ自動的にアクセスできる  
- D. `static` にすると、メンバ初期化子より後に必ず評価される  

---

### Q6

**本文終盤の主張に最も沿うものはどれですか。**

- A. 「コンストラクタ内の仮想関数呼び出し」を安全に使いこなす方法を覚えるのが第9項の主眼である  
- B. `explicit` を付ければ、コンストラクタ中の仮想関数呼び出し問題は実質的に避けられる  
- C. ログに必要な情報を派生クラス側で先に作り、基底クラスでは非仮想処理にして、そもそもコンストラクタ中の仮想関数依存をなくす  
- D. `private` な補助関数を使えば、仮想関数をコンストラクタから呼んでも安全になる  
