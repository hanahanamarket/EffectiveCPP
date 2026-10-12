# Effective C++ Item 3

---
## 回答

#### Answer 1

**正答:** **C**

**回答:** 問題があるのは、引数の型が `char*` になっている部分です。  
この関数は文字列を**読むだけ**なのに、`char*` だと「呼び出し先で文字列を書き換えるかもしれない」宣言に見えます。  
`const char*` に変えることで、**この関数は入力文字列を変更しない**ことを型で示せます。

#### 悪いコード

```cpp
void printName(char* name) {
    std::cout << name << '\n';
}
```

#### 良いコード

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

---

#### Answer 2
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

---

#### Answer 3
**正答:** **A**

**回答:** 防ごうとしているのは、**比較のつもりで代入を書いてしまう事故**です。  

たとえば:

```cpp
if (a * b = c) {}
```

これは本来 `==` と書くつもりだったのに、誤って `=` と書いてしまった例です。  
`operator*` の戻り値が非`const`だと、設計によっては **`a * b` の結果に代入できる形** になってしまいます。  
そこで戻り値を `const Rational` にして、**「積の結果に代入する」という不自然な操作を禁止しやすくする**わけです。


**誤答の要点:**
- **B:** ゼロ除算の有無は演算子の実装次第であり、戻り値を `const Rational` にしたからといってコンパイル時に除算エラーを検出できるわけではない。
- **C:** 演算子の優先順位は言語規則で決まる。戻り値に `const` を付けても、式の結合の仕方（優先順位）そのものは変わらない。
- **D:** `const` 付き戻り値は「一時への代入をしづらくする」などの意味合いがあり、コピーコンストラクタの呼び出しを省略するための指定ではない。

### **重要**  
ただし、**現代C++では値返しに `const` を付けるのは通常推奨されません。**  
**C++11** でムーブ構築・ムーブ代入が標準化されました。（右辺値参照と `std::move` を中心としたモデルで、オブジェクトのコピーを避け、所有権を移動させることで計算コストを削減する手法。)
このため、戻り値を `const T` にすると一時オブジェクトから `T&&` へのムーブが働きにくくなり、値返しの `T` が戻り値最適化（RVO: Return Value Optimization）やムーブと噛み合う利点を活かしづらくなります。その結果、C++11 以降では「値返しにトップレベル `const` を付ける」スタイルは避けるのが主流です。

**それ以前（C++03 時代）**
ムーブがないので、const Rational 返しのデメリットは今ほど目立ちませんでした。
Scott Meyers の Effective 第3版（C++98/03 文脈）では、一時への変な代入を防ぐ意味で const 付き値返しが紹介されることもありましたが、言語仕様として「C++11 から Before に切り替えよ」と決まっているわけではなく、ムーブが入ったあとの実務・ガイドラインの合意が変わった、と理解するのが正確です。

**記載根拠（日本語版）:**  
- **p.10**: `const Rational operator*(const Rational& lhs, const Rational& rhs);` の例が掲載されている。  
- **p.10**: `(a * b) = c` の例に対して、「『2つの数の積に別の値を代入する』というおかしな操作」と説明されている。  
- **p.10**: `if (a * b = c) ...` の例に「おっと、比較のつもりだったのに！」とあり、`=` と `==` の誤用防止が意図として示されている。  
- **p.10**: 「*演算子の戻り値を const にしておけば、そのようなことが避けられるのです」と説明されている。  
- [C++ Core Guidelines F.49: Don't return const T](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines.html#f49-dont-return-const-t)

---

#### Answer 4
**正答:** **D**

**A: ×**
末尾に `const` を付けても戻り値が `char&` なら、呼び出し側は返した参照経由で中身を書き換えられます。**「const を付けたのに、返り値で穴が空く」** 状態で、**条件2**（`ctb[0] = 'x'` をコンパイルエラーにしたい）を満たしません。

**B: ×**
末尾に `const` がない `operator[]` は **`const TextBlock` から呼べません**。**条件1**（`std::cout << ctb[0]`）を満たせません。

**C: ×**
`operator[]` に **`const TextBlock` 向け（末尾 `const`）のオーバーロードがない** と、`ctb[0]` 自体が使えません。**条件1** を満たせません。

**D: ○**
```cpp
const char& operator[](std::size_t position) const {...}
char& operator[](std::size_t position) {...}
```
- **`const TextBlock ctb` 向け:** `const` 付きの `operator[]` が選ばれ、`std::cout << ctb[0]` で読める（条件1）。戻り値が `const char&` なので `ctb[0] = 'x'` はコンパイルエラー（条件2）。
- **`TextBlock tb` 向け:** 非 `const` の `operator[]`（`char&` 返し）が選ばれ、`std::cout << tb[0]` で読める（条件3）。`tb[0] = 'x'` で書き換え可能（条件4）。
- **`const` 版だけ**では `tb` に対しても `const` オーバーロードしか使えず、条件4 を満たせない。よって設問 D のとおり **2つの `operator[]` を併用**するのが適切である。


**記載根拠（日本語版）:**  
- **p.11**: `TextBlock` に対して `const char& operator[](std::size_t position) const` と `char& operator[](std::size_t position)` の2つの `operator[]` が掲載されている。  
- **p.11**: 「const でない TextBlock に対しても、const な TextBlock に対しても、[]演算子を…使うことができます」とあり、両方のオーバーロードが必要であることが示されている。  
- **p.12**: `ctb[0] = 'x';` が「// エラー！」になる理由として、「const な [] 演算子の戻り値を const にしたからです」と明記されている。  
- **p.12**: 「エラーは、『const char&』である [] 演算子の戻り値（ctb[0]）に値を代入しようとしたことで起こるのです」と説明されている。

---

#### Answer 5
**正答:** **A**

- **A** `const` メンバ内では通常メンバを変更できないが、`mutable` 付きメンバは **bitwise const** の制約から外れ、`const` メンバからも書き換えられる。キャッシュのように **logical constness**（外から見た不変）だけ保てばよい場合に使う。
```cpp
   mutable std::size_t textLength;
   mutable bool lengthIsValid;
```
- **B:** `static` データメンバは**クラス全体で1つ**であり、各オブジェクトが自分の文字列に対応する長さ・有効フラグを持つ「インスタンスごとのキャッシュ」にはならない。また Item 3 が扱う **`const` メンバから通常の非 `static` メンバを書き換える問題**とも主題がずれる。
- **C:** `Y` だけ `static` にすると、`lengthIsValid` が**全インスタンスで共有**され、あるオブジェクトのキャッシュ状態が別オブジェクトに影響する。片方だけ `mutable` でも、もう片方が `static` なら意図したキャッシュにならない。
- **D:** `constexpr` は主に**コンパイル時定数**や定数式の文脈向けであり、実行中に `length() const` 内で書き換えるキャッシュ用途の第一選択ではない。`volatile` は**最適化を抑止する**ための指定で、論理的 const を保ちつつメンバを更新するための仕組みではない（Item 3 の `mutable` とは別物）。

**記載根拠（日本語版）:**  
- **p.12**: 「メンバ関数の const には、2つの考え方があります。①『ビットレベルでの不変性』… ②『論理的な不変性』です」と説明されている。  
- **p.14**: `std::size_t length() const;` と、`textLength` / `lengthIsValid` を更新しようとしてエラーになる `CTextBlock::length()` の例が掲載されている。  
- **p.14**: 「このクラスのメンバ関数 length は、textLength と lengthIsValid の値を変更するので、静的に『ビットレベルの不変性』を持っていません。にもかかわらず、外部から変更は見えない」と説明されている。  
- **p.14**: 「答は簡単です。mutable を使うのです。mutable は static でないデータメンバをビットレベルの不変性の制約から解き放ちます」と明記されている。  
- **p.15**: `mutable std::size_t textLength;` と `mutable bool lengthIsValid;` を使った解決版が掲載されている。

---

#### Answer 6
**正答:** **コード2**

**回答:** 適切なのは **コード2** である。本体にすべきなのは **`const char& operator[](std::size_t position) const`** の実装であり、設問の **コード2** が示す考え方に合う。**コード1** の方針は、`const` 版と `char&` 版でロジックが二重になりやすい。

**コード1** のように **`char& operator[](std::size_t position)`** 側だけに境界チェック・ログ・整合性チェックなどを書くと、
- 境界チェック
- ログ
- 整合性チェック

などが **`const char& operator[](std::size_t position) const`** 側にも必要になり、重複し、片方だけ修正漏れする危険があります。  
そこで **`const char& operator[](std::size_t position) const` を single source of truth（唯一の本体）** にまとめ、  
**`char& operator[](std::size_t position)`** はそれを呼び出して再利用します（**コード2**）。

```cpp
char& operator[](std::size_t position) {
    return const_cast<char&>(
        static_cast<const TextBlock&>(*this)[position]
    );
}
```

**なぜこの向きがよいか:**
- **`const char& operator[](std::size_t position) const`** は「読み取り専用」の安全な本体になりやすい
- **`char& operator[](std::size_t position)`** はそこから `const` を外して使うだけ
- 重複が減り、バグ混入箇所が減る
- 逆に **`const` 側から非 `const` 側を呼ぶ** と、`const` オブジェクトに対して `const_cast` で書き込み可能参照へ変換する流れになりやすく、`const` 性を壊す設計になりがちである。
- つまり「どちらでも重複は減る」ではなく、**`const` な実装を正として、非 `const` はその薄いラッパーにする** ほうが安全性と意図の明確さで有利

**結論:**  
**「コード1 のように `char& operator[](std::size_t position)` だけを本体にするのではなく、コード2 のように `const char& operator[](std::size_t position) const` を本体にして再利用する」**

**記載根拠（日本語版）:**  
- **p.15**: 「const なメンバ関数と非 const なメンバ関数の重複を取り除く」という見出しで、重複削減が主題として扱われている。  
- **p.15**: 末尾が `const` の `operator[]` に「境界超えのチェック」「アクセスログを取る」「データの整合性をチェック」などをまとめて書く例が出ている。  
- **p.16**: 「ここで [] 演算子の機能をどこか一箇所で実装できればよい… これは、const をキャストで取り除けば、できます」と説明されている。  
- **p.16**: `return const_cast<char&>( static_cast<const TextBlock&>(*this)[position] );` という、`char&` 返しから `const` 付きの `operator[]` を呼ぶ実装例がそのまま掲載されている。  
- **p.17**: 「非 const な関数の中で const な関数を呼び出しています… const な [] 演算子の実装に const な [] 演算子を使う方法で、コードの重複を避けることができた」と説明されている。

---

#### Answer 7
**正答:** **D**

**回答:** コード2のほうが安全なのは、**`const` メンバとしての `operator[]` が、「オブジェクトを変えない」という読み手の期待と、実際の処理内容が食い違いにくい**からです。（設計の説明では、この「期待と実装の約束」を**契約**と呼ぶこともあります。）

コード1では:

```cpp
const char& operator[](std::size_t position) const {  // ← const 版 operator[]
    return const_cast<TextBlock&>(*this)[position];   // ← 非const 版 operator[] へ
}
```

のように、**宣言末尾が `const` の `operator[]`（const 版）の実装の中で、非const 版の `operator[]` に回す**書き方をしています。  
`const` 版は「オブジェクトを変えない」ように読めるのに、内部では変更しうるオーバーロードにつながり、**読み手の期待を損ねやすい**です。

コード2では、**const 版**は `text[position]` のように読み取りだけにし、  
必要なら **`char& operator[](std::size_t position)`（非const 版）から `const char& operator[](std::size_t position) const`（const 版）を呼ぶ**形にします。  
この向きなら、const 版が危険な経路を持たないので安全です。

**向きの整理（どちらも `TextBlock::operator[]` のオーバーロードの話です）:**
- **非const 版** `char& operator[](std::size_t position)` **から** **const 版** `const char& operator[](std::size_t position) const` **を呼ぶ**のは、ロジックを1か所にまとめるのに有効です。
- **const 版** `const char& operator[](std::size_t position) const` **の中から** **非const 版** `char& operator[](std::size_t position)` **を呼ぶ**（`const_cast` で 非const の `*this` に戻してから呼ぶ）は、`const` と書いた見え方と裏の動きがずれやすく、避けた方が無難です。


**記載根拠（日本語版）:**  
- **p.17**: 「この少し奇妙なコードでは、非 const な関数の中で const な関数を呼び出しています」とあり、この方向の再利用が説明されている。  
- **p.17**: 「非 const なメンバ関数の実装に const なメンバ関数を使いましたが、この逆は、決してよい方法ではありません」と明記されている。  
- **p.17**: 「const なメンバ関数は、そのオブジェクトの状態を変えないと保証するものです。非 const なメンバ関数はそのような保証をしません」と説明されている。  
- **p.17**: 「これから、const なメンバ関数の実装に非 const なメンバ関数を使うのは誤り、と言えます」と結論づけている。
