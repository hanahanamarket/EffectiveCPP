# Effective C++ 第9項

---

## 設問

### Q1

**以下のコードを実行したときの挙動として、正しいものはどれですか。** (回答目安約 4 分)

```cpp
#include <iostream>

class Transaction {
    public:
        Transaction() {
            init();
        }

        virtual void logTransaction() const = 0;  // 純粋仮想関数

    private:
        void init() {
            logTransaction();
        }
    };

void Transaction::logTransaction() const {
    std::cout << "[Transaction] Transaction\n";  // 基底クラスのメッセージ
}

class BuyTransaction : public Transaction {
    public:
        void logTransaction() const override {
            std::cout << "[BuyTransaction] BuyTransaction\n";  // 派生クラスのメッセージ
        }
};

int main() {
    BuyTransaction b;  // 派生クラスをインスタンス化
}
```

- A. 基底クラスの`[Transaction] Transaction` というメッセージが表示される。
- B. 派生クラスの`[BuyTransaction] BuyTransaction` というメッセージが表示される。
- C. `[Transaction] Transaction` と `[BuyTransaction] BuyTransaction` という両方のメッセージが表示される。
- D. 実行時異常終了となる（環境により `pure virtual method called` / `terminate called without an active exception` / `Aborted` / core dump などが観察される）。

---

### Q2

**以下のコードを実行したときの挙動として、正しいものはどれですか。**(回答目安約 4 分)

```cpp
#include <iostream>

class Transaction {
    public:
        Transaction() {
            init();
        }

        virtual void logTransaction() const;  // (純粋でない)仮想関数

    private:
        void init() {
            logTransaction();
        }
};

void Transaction::logTransaction() const {
    std::cout << "[Transaction] Transaction\n";  // 基底クラスのメッセージ
}

class BuyTransaction : public Transaction {
    public:
        void logTransaction() const override {
            std::cout << "[BuyTransaction] BuyTransaction\n";  // 派生クラスのメッセージ
        }
};

int main() {
    BuyTransaction b;  // 派生クラスをインスタンス化
}
```

- A. 基底クラスの`[Transaction] Transaction` というメッセージが表示される。
- B. 派生クラスの`[BuyTransaction] BuyTransaction` というメッセージが表示される。
- C. `[Transaction] Transaction` と `[BuyTransaction] BuyTransaction` という両方のメッセージが表示される。
- D. 実行時異常終了となる（環境により `pure virtual method called` / `terminate called without an active exception` / `Aborted` / core dump などが観察される）。

---

### Q3

**金融商品の取引をモデル化する、継承関係でつながったクラス群があったとします。
このクラスは、買い(BuyTransaction)や売り(SellTransaction)など取引は監査の対象となるので、取引オブジェクトが生成されるように監査ログに記録するためのコードです。
**`___X___` と `___Y___` の組み合わせとして適切なものはどれですか。** (回答目安約 5 分)

```cpp
#include <iostream>

class Transaction {
public:
    explicit Transaction(const char* logInfo)
    {
        logTransaction(logInfo);
    }

    void logTransaction(const char* logInfo) const  // 仮想関数でないメンバー関数
    {
        std::cout << logInfo;
    }
};

class BuyTransaction : public Transaction {
public:
    BuyTransaction()
        ___X___
    {}

private:
    ___Y___ const char* createLogString()
    {
        return "[BuyTransaction] BuyTransaction\n";
    }
};

int main()
{
    BuyTransaction b;  // 派生クラスをインスタンス化
}
```

- A. `___X___` は  `: Transaction(createLogString())` 、 `___Y___` は `static` とする。
- B. `___X___` は  `: Transaction(createLogString())` 、 `___Y___` は空白とする。(非 `static` のメンバ関数として宣言する。)
- C. `___X___` は  `: createLogString()` 、              `___Y___` は `static` とする。
- D. `___X___` は  `: createLogString()` 、              `___Y___` は空白とする。(非 `static` のメンバ関数として宣言する。)
