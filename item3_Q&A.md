# Effective C++ Item 3

---

<a id="item3-q1"></a>
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

[回答へ移動](#item3-q1-answer)

---

<a id="item3-q2"></a>
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

[回答へ移動](#item3-q2-answer)

---

<a id="item3-q3"></a>
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

[回答へ移動](#item3-q3-answer)

---

<a id="item3-q4"></a>
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

[回答へ移動](#item3-q4-answer)

---

<a id="item3-q5"></a>
#### Q5 `length() const` 内でキャッシュ `textLength` / `lengthIsValid` を更新したい。
次のコードでは、1行目のメンバに付ける指定を **X**、2行目のメンバに付ける指定を **Y** とする（`X` と `Y` には `static` / `mutable` などのキーワードが入る）。**X と Y の組み合わせ**として適切なのは次のうちどれか。

```cpp
class CTextBlock {
    private:
        char* pText;
        __X__ std::size_t textLength;
        __Y__ bool lengthIsValid;
};
```

- **A.** __X__: `mutable`, __Y__: `mutable`  
- **B.** __X__: `static`, __Y__: `static`  
- **C.** __X__: `mutable`, __Y__: `static`  
- **D.** __X__: `constexpr`,__Y__: `volatile`  

[回答へ移動](#item3-q5-answer)

---

<a id="item3-q6"></a>
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

[回答へ移動](#item3-q6-answer)

---

<a id="item3-q7"></a>
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

[回答へ移動](#item3-q7-answer)

---

<a id="item3-q8"></a>
#### Q8 次のようにローカル変数に `const` を付ける価値として、最も適切なのはどれか。

#### Before

```cpp
void process() {
    int retries = 3;
    double rate = computeRate();
}
```

#### After

```cpp
void process() {
    const int retries = 3;
    const double rate = computeRate();
}
```

- **A.** 変数のスコープがブロックから関数全体に広がる  
- **B.** 必ず `constexpr` と同じ最適化が行われる  
- **C.** 「途中で変えない」意図を示し、誤った再代入をコンパイル時に防ぎやすくする  
- **D.** ローカル変数が必ずレジスタにだけ置かれる  

[回答へ移動](#item3-q8-answer)

---

<a id="item3-q9"></a>
#### Q9 次の **After** において、`const` が付いているのは何か。

#### Before

```cpp
std::vector<int>::iterator it = v.begin();
```

#### After

```cpp
const std::vector<int>::iterator it = v.begin();
```

- **A.** 反復子 `it` 自体（ポインタでいう `T* const` に相当する振る舞い）  
- **B.** `vector` の要素型 `int`  
- **C.** 反復子が指す先の要素（`*it` の内容）  
- **D.** `vector` コンテナオブジェクト全体  

[回答へ移動](#item3-q9-answer)

---

<a id="item3-q10"></a>
#### Q10 次の **After** の `const_iterator` において、`const` とみなされるのは主にどれか。

#### Before

```cpp
std::vector<int>::iterator it = v.begin();
```

#### After

```cpp
std::vector<int>::const_iterator it = v.begin();
```

- **A.** 反復子変数 `it` 自体だけ（`++it` も禁止される）  
- **B.** 反復子が指す要素（`*it` 経由の書き換えが禁止される；ポインタでいう `const T*` に相当）  
- **C.** `vector` のサイズ変更だけ  
- **D.** `it` も `*it` もどちらも変更不可  

[回答へ移動](#item3-q10-answer)

---

## 回答集

#### Answer 1
<a id="item3-q1-answer"></a>

**正答:** **C**

**回答:** 問題があるのは、引数の型が `char*` になっている部分です。  
この関数は文字列を**読むだけ**なのに、`char*` だと「呼び出し先で文字列を書き換えるかもしれない」宣言に見えます。  
`const char*` に変えることで、**この関数は入力文字列を変更しない**ことを型で示せます。

#### 悪いコード（Before）

```cpp
void printName(char* name) {
    std::cout << name << '\n';
}
```

#### 良いコード（After）

```cpp
void printName(const char* name) {
    std::cout << name << '\n';
}
```


**意図:**
- APIの意図を明確にする
- 誤って書き換えるコードをコンパイラに止めてもらう
- 呼び出し側に値が更新されないことが伝わり、安心感を与える

**記載根拠（日本語版）:**  
- **p.8**: 「オブジェクトの値を変更するつもりがないのであれば、`const` を付けるべきです。それにより、コンパイラも助けてくれるからです。」  
- **p.8**: `const` は「クラスの外では、グローバルな定数…関数…にも付けることができます」とあり、関数引数にも広く使う方針が示されている。  
- **p.8**: 「ポインタに使って、ポインタそのものが変更されないのか、ポインタの指す値が変更されないのか、あるいは両方なのかを指定することもできます。」

[問いに戻る](#item3-q1)

#### Answer 2
<a id="item3-q2-answer"></a>
**正答:** **D**

**回答:** 変更不能になるのは、**`p` が指している先のデータ**です。  
`const char* p` は、ポインタ `p` 自体は別の場所を指せますが、`*p` 経由で文字を書き換えることはできません。

**整理:**
- `const` が `*` の左側
- 守られるのは **データ**
- ポインタ自体は変更可能

**対応イメージ:**  
`const char* p` は **「constなデータへのポインタ」** です。

**記載根拠（日本語版）:**  
- **p.8**: `const char * p = greeting;` に対して「// ポインタは非const」「// データはconst」と明記されている。  
- **p.9**: 「`const` がアスタリスク（*）の左にあるときは、『ポインタが指し示すデータ』が不変とされ…」と説明されている。  
- **p.9**: `void f1(const Widget *pw);` と `void f2(Widget const *pw);` が同じ意味だと示され、`*` の左側の `const` が「指す先」を守ることが確認できる。

[問いに戻る](#item3-q2)

#### Answer 3
<a id="item3-q3-answer"></a>
**正答:** **A**

**回答:** 防ごうとしているのは、**比較のつもりで代入を書いてしまう事故**です。  

たとえば:

```cpp
if (a * b = c) {}
```

これは本来 `==` と書くつもりだったのに、誤って `=` と書いてしまった例です。  
`operator*` の戻り値が非`const`だと、設計によっては **`a * b` の結果に代入できる形** になってしまいます。  
そこで戻り値を `const Rational` にして、**「積の結果に代入する」という不自然な操作を禁止しやすくする**わけです。

**意図:**
- 一時オブジェクトへの不自然な代入を防ぐ
- `==` と `=` の打ち間違い事故を減らす
- 型でおかしな操作を拒否する

**重要:**  
ただし、**現代C++では値返しに `const` を付けるのは通常推奨されません。**  
**C++11** でムーブ構築・ムーブ代入（右辺値参照と std::move を中心としたモデル）が標準になったことで、戻り値を const T にすると、その一時オブジェクトから T&& へのムーブがしづらい値で返す T が、戻り値最適化に加えて ムーブともきちんと噛み合うという理由で、「値返しにトップレベル const を付ける」スタイルは C++11 以降では避けるのが主流になりました。

**それ以前（C++03 時代）**
ムーブがないので、const Rational 返しのデメリットは今ほど目立ちませんでした。
Scott Meyers の Effective 第3版（C++98/03 文脈）では、一時への変な代入を防ぐ意味で const 付き値返しが紹介されることもありましたが、言語仕様として「C++11 から Before に切り替えよ」と決まっているわけではなく、ムーブが入ったあとの実務・ガイドラインの合意が変わった、と理解するのが正確です。

**記載根拠（日本語版）:**  
- **p.10**: `const Rational operator*(const Rational& lhs, const Rational& rhs);` の例が掲載されている。  
- **p.10**: `(a * b) = c` の例に対して、「『2つの数の積に別の値を代入する』というおかしな操作」と説明されている。  
- **p.10**: `if (a * b = c) ...` の例に「おっと、比較のつもりだったのに！」とあり、`=` と `==` の誤用防止が意図として示されている。  
- **p.10**: 「*演算子の戻り値を const にしておけば、そのようなことが避けられるのです」と説明されている。  
- [C++ Core Guidelines F.49: Don't return const T](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines.html#f49-dont-return-const-t)



[問いに戻る](#item3-q3)

#### Answer 4
<a id="item3-q4-answer"></a>
**正答:** **D**

**A: ×**
末尾に `const` を付けても戻り値が `char&` なら、呼び出し側は返した参照経由で中身を書き換えられます。**「const を付けたのに、返り値で穴が空く」** 状態です。

**B: ×**
末尾に `const` がない `operator[]` は **`const TextBlock` から呼べません**。条件1 を満たせません。

**C: ×**
non-const 版だけでは **`const TextBlock` に対して `operator[]` 自体が使えず**、条件1 を満たしません。

**D: ○**
```cpp
const char& operator[](std::size_t pos) const
```
- 関数末尾の `const` で **const オブジェクトから呼べる**（条件1）  
- 戻り値の `const char&` で **参照経由の代入を拒否できる**（条件2）


**記載根拠（日本語版）:**  
- **p.11**: `TextBlock` に対して `const char& operator[](std::size_t position) const` と `char& operator[](std::size_t position)` の2つの `operator[]` が掲載されている。  
- **p.11**: 「const でない TextBlock に対しても、const な TextBlock に対しても、[]演算子を…使うことができます」とあり、両方のオーバーロードが必要であることが示されている。  
- **p.12**: `ctb[0] = 'x';` が「// エラー！」になる理由として、「const な [] 演算子の戻り値を const にしたからです」と明記されている。  
- **p.12**: 「エラーは、『const char&』である [] 演算子の戻り値（ctb[0]）に値を代入しようとしたことで起こるのです」と説明されている。

[問いに戻る](#item3-q4)

#### Answer 5
<a id="item3-q5-answer"></a>
**正答:** **A**

**回答:** **X も Y も `mutable`** が適切である（選択肢 **A**）。キャッシュ用の両メンバに **`mutable`** を付ける。

`const` メンバ内では通常メンバを変更できないが、`mutable` 付きメンバは **bitwise const** の制約から外れ、`const` メンバからも書き換えられる。キャッシュのように **logical constness**（外から見た不変）だけ保てばよい場合に使う。

```cpp
mutable std::size_t textLength;
mutable bool lengthIsValid;
```

**記載根拠（日本語版）:**  
- **p.12**: 「メンバ関数の const には、2つの考え方があります。①『ビットレベルでの不変性』… ②『論理的な不変性』です」と説明されている。  
- **p.14**: `std::size_t length() const;` と、`textLength` / `lengthIsValid` を更新しようとしてエラーになる `CTextBlock::length()` の例が掲載されている。  
- **p.14**: 「このクラスのメンバ関数 length は、textLength と lengthIsValid の値を変更するので、静的に『ビットレベルの不変性』を持っていません。にもかかわらず、外部から変更は見えない」と説明されている。  
- **p.14**: 「答は簡単です。mutable を使うのです。mutable は static でないデータメンバをビットレベルの不変性の制約から解き放ちます」と明記されている。  
- **p.15**: `mutable std::size_t textLength;` と `mutable bool lengthIsValid;` を使った解決版が掲載されている。

[問いに戻る](#item3-q5)

#### Answer 6
<a id="item3-q6-answer"></a>
**正答:** **A**

**回答:** 本体にすべきなのは **const版** です。  

non-const版でロジックを二重に持つと、
- 境界チェック
- ログ
- 整合性チェック

などが重複し、片方だけ修正漏れする危険があります。  
そこで **const版を single source of truth（唯一の本体）** にして、  
non-const版はそれを呼び出して再利用します。

```cpp
char& operator[](std::size_t pos) {
    return const_cast<char&>(
        static_cast<const TextBlock&>(*this)[pos]
    );
}
```

**なぜこの向きがよいか:**
- const版は「読み取り専用」の安全な本体
- non-const版はそこから `const` を外して使うだけ
- 重複が減り、バグ混入箇所が減る

**結論:**  
**「non-const版を本体にするのではなく、const版を本体にして再利用する」**

**記載根拠（日本語版）:**  
- **p.15**: 「const なメンバ関数と非 const なメンバ関数の重複を取り除く」という見出しで、重複削減が主題として扱われている。  
- **p.15**: `const` 版の `operator[]` に「境界超えのチェック」「アクセスログを取る」「データの整合性をチェック」などをまとめて書く例が出ている。  
- **p.16**: 「ここで [] 演算子の機能をどこか一箇所で実装できればよい… これは、const をキャストで取り除けば、できます」と説明されている。  
- **p.16**: `return const_cast<char&>( static_cast<const TextBlock&>(*this)[position] );` という non-const版から const版を呼ぶ実装例がそのまま掲載されている。  
- **p.17**: 「非 const な関数の中で const な関数を呼び出しています… const な [] 演算子の実装に const な [] 演算子を使う方法で、コードの重複を避けることができた」と説明されている。

[問いに戻る](#item3-q6)

#### Answer 7
<a id="item3-q7-answer"></a>
**正答:** **D**

**回答:** After側のほうが安全なのは、**const関数の契約を壊しにくい**からです。  

Before側では:

```cpp
const char& operator[](std::size_t pos) const {
    return const_cast<TextBlock&>(*this)[pos];
}
```

のように、`const` メンバ関数の中から non-const版へ逃がしています。  
これは「この関数はオブジェクトを変えません」という期待に反する設計になりやすく、  
**const契約の破れ目** になります。

After側では、const版は素直に constな処理を行い、  
必要なら **non-const版から const版を呼ぶ** 形にします。  
この向きなら、const版が危険な操作をしないので安全です。

**原則:**
- **non-const → const** は再利用として有効
- **const → non-const** は契約違反を招きやすい

**一言でいうと:**  
**「const関数は、constらしく振る舞わせる」**

**記載根拠（日本語版）:**  
- **p.17**: 「この少し奇妙なコードでは、非 const な関数の中で const な関数を呼び出しています」とあり、この方向の再利用が説明されている。  
- **p.17**: 「非 const なメンバ関数の実装に const なメンバ関数を使いましたが、この逆は、決してよい方法ではありません」と明記されている。  
- **p.17**: 「const なメンバ関数は、そのオブジェクトの状態を変えないと保証するものです。非 const なメンバ関数はそのような保証をしません」と説明されている。  
- **p.17**: 「これから、const なメンバ関数の実装に非 const なメンバ関数を使うのは誤り、と言えます」と結論づけている。

[問いに戻る](#item3-q7)

#### Answer 8
<a id="item3-q8-answer"></a>
**正答:** **C**

**回答:** ローカル変数に `const` を付ける価値は、**意図の固定** と **誤代入防止** にあります。  

```cpp
const int retries = 3;
const double rate = computeRate();
```

としておけば、「この値は途中で変えない前提だ」とコード上で明示できます。  
あとから自分や他人が読んだときにも安心ですし、  
誤って再代入しようとしたらコンパイラが止めてくれます。

**意図:**
- 変えない値を明示する
- うっかり再代入を防ぐ
- 読み手の認知負荷を下げる

**Item 3 の本質:**  
`const` はメンバ関数だけの話ではなく、  
**ローカル変数・引数・戻り値・ポインタ・反復子** にも広く使うものです。

**記載根拠（日本語版）:**  
- **p.8**: 「const は驚くほど広い範囲で使われます」とあり、クラス外・クラス内・ポインタなど幅広い適用先が列挙されている。  
- **p.10**: 「仮引数は、普通のオブジェクトと違いはありません。const な仮引数は、ローカルな const オブジェクトとして振舞います」と説明されている。  
- **p.10**: 「仮引数に限らず、一般のローカルなオブジェクトも、その内容を変更する必要がなければ、const を付けて宣言するべきです」と明記されている。  
- **p.10**: 「たった6文字のタイプを加えるだけで、今見たような『== のつもりだったのに = としてしまった』という…エラーから身を守ることになる」と説明されている。

[問いに戻る](#item3-q8)

#### Answer 9
<a id="item3-q9-answer"></a>
**正答:** **A**

**回答:** `const std::vector<int>::iterator it` で `const` になっているのは、**iterator 自体**です。  

これはポインタでいうと **`T* const`** に対応します。  
つまり:
- `it` 自体は別の要素へ進めない
- しかし `*it` 経由で要素を書き換えることはできる

たとえば:

```cpp
*it = 10; // OK
// ++it;  // NG
```

**ポイント:**  
`const iterator` と `const_iterator` はまったく別物です。  
前者は **「動かせない iterator」** です。

**記載根拠（日本語版）:**  
- **p.9**: 「STLの反復子 iterator はポインタをモデルにしています」と説明されている。  
- **p.9**: `const std::vector<int>::iterator iter = vec.begin();` の例に対して、「`T* const` のように振舞う反復子」と書かれている。  
- **p.9**: 同じ例で `*iter = 10;` は「iter が指すものの内容を変更しても問題ない」、`++iter;` は「エラー！ iter は const だから」と説明されている。

[問いに戻る](#item3-q9)

#### Answer 10
<a id="item3-q10-answer"></a>
**正答:** **B**

**回答:** `std::vector<int>::const_iterator it` で `const` になっているのは、**iterator が指す先**です。  

これはポインタでいうと **`const T*`** に対応します。  
つまり:
- `it` 自体は進められる
- しかし `*it` 経由で要素は書き換えられない

たとえば:

```cpp
// *it = 10; // NG
++it;        // OK
```

**ポイント:**  
- `const iterator` = iterator自体がconst
- `const_iterator` = 指す先がconst

**記載根拠（日本語版）:**  
- **p.9**: `std::vector<int>::const_iterator cIter = vec.begin();` の例が掲載されている。  
- **p.9**: これに対して「cIter は `const T*` のように振舞う」と説明されている。  
- **p.9**: `*cIter = 10;` は「エラー！ *cIter は const だから」、`++cIter;` は「cIter を変えても問題ない」と明記されている。  
- **p.9**: 「『指し示しているものの内容を変更しない反復子』… は `const_iterator` で表されます」と説明されている。

[問いに戻る](#item3-q10)
