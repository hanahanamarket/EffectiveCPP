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

**以下は第9項の改善案を簡略化したコードです。** `___X___` と `___Y___` についてそれぞれ、本文の改良方針に最も合う選択肢はどれですか。

```cpp
#include <iostream>

class Transaction {
public:
    explicit Transaction(const char* logInfo)
    {
        logTransaction(logInfo);
    }

    void logTransaction(const char* logInfo) const
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
    BuyTransaction b;
}
```


**（1）`___X___` の位置に置く構成として、本文の改善案（派生側の情報を基底のコンストラクターへ **`Transaction(createLogString())` と渡すパターン**）に一致するものはどれですか。**

`___X___` はコンストラクター本文より前で基底を初期化する **メンバ初期化子リスト**を置く行です。

- A. `: Transaction(createLogString())`
- B. メンバ初期化子リストを書かず、コンストラクター本文のみを `{ Transaction(createLogString()); }` とする。
- C. `: createLogString()`
- D. メンバ初期化子リストを書かず、コンストラクター本文のみを `{ logTransaction(createLogString()); }` とする。

**（2）`___Y___` の位置に書く記述として、本文の改善案に沿うものはどれですか。**  
（派生側のログ文字列を、構築途中のオブジェクトへの依存を増やさない形で用意する、という観点です。）

- A. この位置には何も書かない。
- B. `static`
- C. `virtual`
- D. `virtual static`
