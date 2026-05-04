# Effective C++ 第9項

---

## 設問

### Q1

**以下のコードを実行したときの挙動として、正しいものはどれですか。**

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

**以下のコードを実行したときの挙動として、正しいものはどれですか。**

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

**本文の改善案に最も忠実なのはどれですか。**

```cpp
#include <iostream>

class Transaction {
public:
    explicit Transaction(const char* logInfo)
    {
        logTransaction(logInfo);
    }

    ___X___ logTransaction(const char* logInfo) const
    {
        std::cout << logInfo;
    }
};

class BuyTransaction : public Transaction {
public:
    BuyTransaction(): Transaction(createLogString())
    {}

private:
    static const char* createLogString()
    {
        return "[BuyTransaction] BuyTransaction\n";
    }
};

int main()
{
    BuyTransaction b;
}
```