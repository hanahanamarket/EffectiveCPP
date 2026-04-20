# Effective C++ Item 3
---
## 設問

#### Q1 次の関数について、「この関数は文字列を書き換えるかもしれない」という**誤った印象**を与えているのはどの部分か。

```cpp
void printName(char* name) {
    std::cout << name << '\n';
}
```

- **A.** `void`（戻り値がないこと）  
- **B.** 関数名 `printName`  
- **C.** 仮引数の型 `char*`  
- **D.** `std::cout` を使っていること  

---

#### Q2 次の修正によって、**何が**変更不能（`*` 経由で書き換えられない）になるか。

#### 修正前

```cpp
char greeting[] = "Hello";
char* p = greeting;
```

#### 修正後

```cpp
char greeting[] = "Hello";
const char* p = greeting;
```

- **A.** `greeting` 配列の各要素そのもの（配列宣言に `const` が付いたのと同じ）  
- **B.** ポインタ `p` 自体（`p` に別のアドレスを代入できなくなる）  
- **C.** `greeting` の宣言全体  
- **D.** `p` が指す先のデータ（`*p` でたどる内容）  

---

#### Q3 `operator*` の戻り値を `const Rational` にしたときの主な狙いとして、最も適切なのはどれか。

#### 修正前

```cpp
Rational operator*(
    const Rational& lhs,
    const Rational& rhs
);
```

#### 修正後

```cpp
const Rational operator*(
    const Rational& lhs,
    const Rational& rhs
);
```

- **A.** 比較 `==` のつもりで `=` を書いてしまう事故（例: `if (a * b = c)`）を防ぐ  
- **B.** ゼロ除算をコンパイル時に検出する  
- **C.** 演算子の優先順位ミスを防ぐ  
- **D.** `Rational` のコピーコンストラクタ呼び出しを省略する  

---

#### Q4 次の `TextBlock` に `operator[]` を追加するとき、以下の条件1, 2を満たす**適切な宣言の組み合わせ**は次のうちどれか。実装は `text[position]` にアクセスするものとする。

```cpp
class TextBlock {
    public:
        // ここに operator[] を宣言する
    private:
        std::string text;
};
```

- (条件1) `tb[i]` で文字を読める  
- (条件2) `tb[i] = 'x';` はコンパイルエラーにしたい  


**A.** 
```cpp
char& operator[](std::size_t position) const;
```
**B.** 
```cpp
const char& operator[](std::size_t position);
```
**C.** 
```cpp
char& operator[](std::size_t position);
```
**D.**
```cpp
const char& operator[](std::size_t position) const;
char& operator[](std::size_t position);
```

---

#### Q5 `char* pText` が指す文字列の長さを返す **`std::size_t length() const`** を、このクラスが**自分で宣言するメンバ関数**とする。

`length() const` の実装では、求めた長さと「その値が信頼できるか」を、それぞれメンバ `textLength` と `lengthIsValid` に書き込んでキャッシュしたい。

次の宣言では、1行目のメンバに付ける指定を **X**、2行目のメンバに付ける指定を **Y** とする。
**X** と **Y** に記載する予約語の組み合わせとして適切なのは次のうちどれか。

```cpp
class CTextBlock {
    public:
        std::size_t length() const;  // pText の文字列長を返す（実装ではキャッシュを更新する想定）
    private:
        char* pText;
        __X__ std::size_t textLength;
        __Y__ bool lengthIsValid;
};
```

- **A.** __X__: `mutable`, __Y__: `mutable`  
- **B.** __X__: `static`, __Y__: `static`  
- **C.** __X__: `mutable`, __Y__: `static`  
- **D.** __X__: `constexpr`, __Y__: `volatile`  

---

#### Q6 次のような `TextBlock` があり、`operator[]` は次の2つを宣言するものとする（実装はいずれも `text[position]` にアクセスする想定）。

```cpp
class TextBlock {
    public:
        const char& operator[](std::size_t position) const;  // const な TextBlock 向け（読み取り）
        char& operator[](std::size_t position);              // 非 const の TextBlock 向け（書き換え可能な参照）

    private:
        std::string text;
};
```

境界チェック・ログ・整合性チェックなどの処理を**二重に書かない**ために、**どちらのオーバーロードの実装を「本体」（ロジックの唯一の置き場）にすべきか**。  
下の **コード1** と **コード2** は、その方針を示した **`char& operator[](std::size_t position)` 側の実装例**である（2択）。

#### コード1

```cpp
char& operator[](std::size_t position) {
    // 境界チェック
    // ログ
    // 整合性チェック
    return text[position];
}
```

#### コード2

```cpp
char& operator[](std::size_t position) {
    return const_cast<char&>(
        static_cast<const TextBlock&>(*this)[position]
    );
}
```

- **コード1：** 本体は **`char& operator[](std::size_t position)`** にだけ書き、**`const char& operator[](std::size_t position) const`** は別の実装でよい  
- **コード2：** 本体は **`const char& operator[](std::size_t position) const`** にまとめ、**`char& operator[](std::size_t position)`** はその実装を呼び出して再利用する  

---

#### Q7 次の **コード1** と **コード2** を比べたとき、**コード2が安全とされる主な理由**はどれか。

#### コード1

```cpp
const char& operator[](std::size_t position) const {
    return const_cast<TextBlock&>(*this)[position];
}
```

#### コード2

```cpp
const char& operator[](std::size_t position) const {
    return text[position];
}
```

- **A.** `const_cast` が使えなくなるため、実行時オーバーヘッドが消える  
- **B.** `text[position]` が常に例外を投げないため  
- **C.** 暗黙の型変換が起きないため  
- **D.** const メンバ関数の契約（オブジェクトを変えないという期待）を壊しにくい  
