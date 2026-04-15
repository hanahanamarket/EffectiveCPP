# Effective C++ Item 3

---

## 1. 読み取り専用引数に `const` を付ける

<a id="item3-q1"></a>
### Q1
この関数は文字列を読むだけです。どこが嘘になっていますか？

[回答へ移動](#item3-q1-answer)
### Before

```cpp
void printName(char* name) {
    std::cout << name << '\n';
}
```

### After

```cpp
void printName(const char* name) {
    std::cout << name << '\n';
}
```

### ポイント
- `char*` は「変更するかもしれない」印象になる
- `const char*` は「読み取り専用」を型で表現する

---

## 2. ポインタの `const`（どこを守るか）

### 2-1. 指す先データを守る

<a id="item3-q2-1"></a>
### Q2-1
何が変更不能になりますか？

[回答へ移動](#item3-q2-1-answer)

### Before

```cpp
char greeting[] = "Hello";
char* p = greeting;
```

### After

```cpp
char greeting[] = "Hello";
const char* p = greeting;
```

---

### 2-2. ポインタ自身を守る

<a id="item3-q2-2"></a>
### Q2-2
何が変更不能になりますか？

[回答へ移動](#item3-q2-2-answer)

### Before

```cpp
char greeting[] = "Hello";
char* p = greeting;
```

### After

```cpp
char greeting[] = "Hello";
char* const p = greeting;
```

---

### 2-3. 両方を守る

<a id="item3-q2-3"></a>
### Q2-3
どの2つが変更不能になりますか？

[回答へ移動](#item3-q2-3-answer)

### Before

```cpp
char greeting[] = "Hello";
char* p = greeting;
```

### After

```cpp
char greeting[] = "Hello";
const char* const p = greeting;
```

---

## 3. 戻り値 `const`（歴史的文脈）

<a id="item3-q3"></a>
### Q3
この変更は、どんな代入ミスを防ごうとしていますか？

[回答へ移動](#item3-q3-answer)

### Before

```cpp
Rational operator*(const Rational& lhs,
                   const Rational& rhs);
```

### After

```cpp
const Rational operator*(const Rational& lhs,
                         const Rational& rhs);
```

参考コード:

```cpp
Rational a, b, c;
if (a * b = c) {}  // == のつもりで =
```

### ポイント
- Item 3の文脈では有名な教材
- 現代C++では、値返しに `const` は通常推奨されない

---

## 4. `operator[]` の const穴を塞ぐ

<a id="item3-q4"></a>
### Q4
`const` オブジェクトからの書き換えを防ぐには、どこまで `const` が必要ですか？

[回答へ移動](#item3-q4-answer)

### Before

```cpp
class TextBlock {
public:
    char& operator[](std::size_t pos) {
        return text[pos];
    }

private:
    std::string text;
};
```

### After

```cpp
class TextBlock {
public:
    const char& operator[](std::size_t pos) const {
        return text[pos];
    }

    char& operator[](std::size_t pos) {
        return text[pos];
    }

private:
    std::string text;
};
```

---

## 5. `const` メンバ関数で内部更新する (`mutable`)

<a id="item3-q5"></a>
### Q5
外から見て不変を保ちながら、内部キャッシュだけ更新するには？

[回答へ移動](#item3-q5-answer)

### Before

```cpp
class CTextBlock {
private:
    char* pText;
    std::size_t textLength;
    bool lengthIsValid;
};
```

### After

```cpp
class CTextBlock {
private:
    char* pText;
    mutable std::size_t textLength;
    mutable bool lengthIsValid;
};
```

### ポイント
- `mutable` は logical constness を実現する道具

---

## 6. const版/non-const版の重複を消す

<a id="item3-q6"></a>
### Q6
同じロジックを2回書かずに、どちらを本体にすべきですか？

[回答へ移動](#item3-q6-answer)

### Before

```cpp
char& operator[](std::size_t pos) {
    // 境界チェック
    // ログ
    // 整合性チェック
    return text[pos];
}
```

### After

```cpp
char& operator[](std::size_t pos) {
    return const_cast<char&>(
        static_cast<const TextBlock&>(*this)[pos]
    );
}
```

### ポイント
- const版を single source of truth にする
- `non-const -> const` の呼び出しは有効な整理法

---

## 7. 逆方向は危険（const -> non-const）

<a id="item3-q7"></a>
### Q7
なぜAfter側のほうが安全ですか？

[回答へ移動](#item3-q7-answer)

### Before

```cpp
const char& operator[](std::size_t pos) const {
    return const_cast<TextBlock&>(*this)[pos];
}
```

### After

```cpp
const char& operator[](std::size_t pos) const {
    return text[pos];
}
```

---

## 8. ローカル変数にも `const`

<a id="item3-q8"></a>
### Q8
ローカルに `const` を付ける価値は何ですか？

[回答へ移動](#item3-q8-answer)

### Before

```cpp
void process() {
    int retries = 3;
    double rate = computeRate();
}
```

### After

```cpp
void process() {
    const int retries = 3;
    const double rate = computeRate();
}
```

---

## 9. `iterator` と `const_iterator` の違い

### 9-1. iterator 自体を `const` にする

<a id="item3-q9-1"></a>
[回答へ移動](#item3-q9-1-answer)

### Before

```cpp
std::vector<int>::iterator it = v.begin();
```

### After

```cpp
const std::vector<int>::iterator it = v.begin();
```

---

### 9-2. 指す先を `const` にする

<a id="item3-q9-2"></a>
[回答へ移動](#item3-q9-2-answer)

### Before

```cpp
std::vector<int>::iterator it = v.begin();
```

### After

```cpp
std::vector<int>::const_iterator it = v.begin();
```

---

## 10. 締めの一言

- const はコメントではなく契約
- 人間は忘れる。型は忘れない
- 変更しないなら、そう宣言する

---

## 回答集

### Answer 1
<a id="item3-q1-answer"></a>
※ 回答は後で作成します。  
[問いに戻る](#item3-q1)

### Answer 2-1
<a id="item3-q2-1-answer"></a>
※ 回答は後で作成します。  
[問いに戻る](#item3-q2-1)

### Answer 2-2
<a id="item3-q2-2-answer"></a>
※ 回答は後で作成します。  
[問いに戻る](#item3-q2-2)

### Answer 2-3
<a id="item3-q2-3-answer"></a>
※ 回答は後で作成します。  
[問いに戻る](#item3-q2-3)

### Answer 3
<a id="item3-q3-answer"></a>
※ 回答は後で作成します。  
[問いに戻る](#item3-q3)

### Answer 4
<a id="item3-q4-answer"></a>
※ 回答は後で作成します。  
[問いに戻る](#item3-q4)

### Answer 5
<a id="item3-q5-answer"></a>
※ 回答は後で作成します。  
[問いに戻る](#item3-q5)

### Answer 6
<a id="item3-q6-answer"></a>
※ 回答は後で作成します。  
[問いに戻る](#item3-q6)

### Answer 7
<a id="item3-q7-answer"></a>
※ 回答は後で作成します。  
[問いに戻る](#item3-q7)

### Answer 8
<a id="item3-q8-answer"></a>
※ 回答は後で作成します。  
[問いに戻る](#item3-q8)

### Answer 9-1
<a id="item3-q9-1-answer"></a>
※ 回答は後で作成します。  
[問いに戻る](#item3-q9-1)

### Answer 9-2
<a id="item3-q9-2-answer"></a>
※ 回答は後で作成します。  
[問いに戻る](#item3-q9-2)

