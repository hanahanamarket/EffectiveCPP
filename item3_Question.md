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
- **D.** `p` が指す先のデータ（`*p` で辿る内容）  

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

#### Q4 次の `TextBlock` に `operator[]` を追加するとき、以下の条件1, 2を満たす**適切な宣言の組み合わせ**は次のうちどれか。実装は `text[pos]` にアクセスするものとする。

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
char& operator[](std::size_t pos) const;
```
**B.** 
```cpp
const char& operator[](std::size_t pos);
```
**C.** 
```cpp
char& operator[](std::size_t pos);
```
**D.**
```cpp
const char& operator[](std::size_t pos) const;
char& operator[](std::size_t pos);
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

#### Q6 `operator[]` の実装を二重に持たないようにする場合、**どちらを「本体」（実装の唯一の置き場）にすべきか**。

#### Before

```cpp
char& operator[](std::size_t pos) {
    // 境界チェック
    // ログ
    // 整合性チェック
    return text[pos];
}
```

#### After

```cpp
char& operator[](std::size_t pos) {
    return const_cast<char&>(
        static_cast<const TextBlock&>(*this)[pos]
    );
}
```

- **A.** const 版を本体にし、non-const 版はそれを呼び出して再利用する  
- **B.** non-const 版のみを本体にし、const 版は別実装でよい  
- **C.** どちらを本体にしても同じである  
- **D.** 両方に同じロジックを複製して書くのが最も安全である  

---

#### Q7 次の **Before** と **After** を比べたとき、**After 側が安全とされる主な理由**はどれか。

#### Before

```cpp
const char& operator[](std::size_t pos) const {
    return const_cast<TextBlock&>(*this)[pos];
}
```

#### After

```cpp
const char& operator[](std::size_t pos) const {
    return text[pos];
}
```

- **A.** `const_cast` が使えなくなるため、実行時オーバーヘッドが消える  
- **B.** `text[pos]` が常に例外を投げないため  
- **C.** 暗黙の型変換が起きないため  
- **D.** const メンバ関数の契約（オブジェクトを変えないという期待）を壊しにくい  
