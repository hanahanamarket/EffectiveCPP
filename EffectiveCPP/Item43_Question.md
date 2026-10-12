# 設問

クラステンプレートが、テンプレートパラメータに依存する基底クラス（例：`MsgSender<Company>`）から派生しているとき、基底クラスのメンバ名をそのまま書いても見つからないことがある。特化によって基底のインターフェースが変わり得るため、コンパイラは既定ではそのような基底の中を調べない。

問1と問2は独立している。問1の小問は、直後に示す共通コードを前提とする。

---

## 問1

次のコードを考える（`MsgInfo` などは宣言済みとする）。

```cpp
template<typename Company>
class MsgSender {
public:
    void sendClear(const MsgInfo& info);
};

template<typename Company>
class LoggingMsgSender : public MsgSender<Company> {
public:
    void sendClearMsg(const MsgInfo& info)
    {
        sendClear(info); // ここでコンパイルエラーになり得る
    }
};
```

### (1)

`sendClear(info);` がエラーになり得る**主な理由**として最も適切なものはどれか。

A. 派生クラステンプレートでは、いかなる基底クラスのメンバも呼べないから

B. `sendClear` は非仮想関数なので、派生クラスから呼べないから

C. 基底がテンプレートパラメータに依存するため、特化で `sendClear` が存在しない可能性があり、コンパイラは既定ではその基底の中を名前探索しないから

D. `sendClear` は `private` なので、派生クラスから呼べないから

### (2)

`CompanyZ` には平文送信がなく、次のように `MsgSender` の**全特化**が定義されているとする。

```cpp
template<>
class MsgSender<CompanyZ> {
public:
    void sendSecret(const MsgInfo& info);
    // sendClear は持たない
};
```

この全特化が、共通コードで `sendClear` が見つからない問題とどう関係するか、最も適切な説明はどれか。

A. 全特化があると、一次テンプレートの `MsgSender` は一切使えなくなる

B. `Company` が `CompanyZ` のとき基底に `sendClear` が無い例になり、「テンプレート化された基底にメンバがあるとは限らない」ことの根拠になる

C. 全特化はメンバ関数の追加だけが許され、削除はできない

D. 全特化は実行時に選ばれるため、コンパイル時の名前探索には影響しない

### (3)

上記の `LoggingMsgSender` で、基底の `sendClear` を呼べるようにする書き方として**正しいものだけ**を集めた組はどれか。

1. `this->sendClear(info);`
2. `using MsgSender<Company>::sendClear;` をクラス内に書き、その後 `sendClear(info);`
3. `MsgSender<Company>::sendClear(info);`

A. 1 のみ

B. 1 と 2 のみ

C. 2 と 3 のみ

D. 1、2、3 すべて

---

## 問2

次のクラステンプレートを考える（`MsgInfo` などは宣言済みとする）。

```cpp
template<typename Company>
class MsgSender {
public:
    virtual void sendClear(const MsgInfo& info);
};

template<typename Company>
class LoggingMsgSender : public MsgSender<Company> {
public:
    void sendClearMsg(const MsgInfo& info)
    {
        // 次の3通りのいずれかで基底の sendClear を呼ぶ
        this->sendClear(info);
        // または
        // using MsgSender<Company>::sendClear; をクラス内に書いたうえで
        // sendClear(info);
        // または
        // MsgSender<Company>::sendClear(info);
    }
};
```

`sendClear` が仮想関数であるとき、上記3通りの違いとして最も適切なものはどれか。

A. 3つとも仮想呼び出しになる点で完全に同じである

B. `this->` と `using` 経由の呼び出しは動的束縛の対象になり得るが、`MsgSender<Company>::sendClear(info)` は仮想関数の動的束縛を抑止する

C. `using` だけが仮想呼び出しを無効にする

D. `this->` だけが仮想呼び出しを無効にする
