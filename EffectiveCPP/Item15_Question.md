# 設問

カウンター用の関数`hand_ticket_to_counter`は所与のものとして決まっており、伝票番号の値（`int` 型）しか受け取れない。その実装もシグネチャも変えられない。

なお、次のコードでは、`patron` の実行の際の処理の順に **ⅰ〜ⅶ** の番号を付番している。

```cpp
void hand_ticket_to_counter(int ticketNumber);                  // ⅶ カウンター

class LibraryTicket {                                           // ⅲ
    public:
        explicit LibraryTicket(int number) : number_(number) {} // ⅳ
        ~LibraryTicket() {}
        int ticketNumber() const { return number_; }
        
    private:
        int number_;                                            // ⅴ
};

void patron() {                                                 // ⅰ
    LibraryTicket mine(555);                                    // ⅱ
    hand_ticket_to_counter(mine);                               // ⅵ
}
```

**(1)** 上のコードでは、ⅵ は `hand_ticket_to_counter(mine)` がカウンター用の関数呼び出し、ⅶ は関数宣言に相当します。<br>
`LibraryTicket`のインスタンス`mine`をカウンター用の関数`hand_ticket_to_counter`に対し、**そのままでは渡せない主な理由**として、**正しいものを一つ**選べ。

- **A** `mine` は値渡しで渡されるため `LibraryTicket` オブジェクト全体がコピーされ、カウンターが要求する `int` 型の引数としては渡せないから。
- **B** `LibraryTicket` のコンストラクタに `explicit` が付いているから、関数の引数として `mine` を渡せない。
- **C** `hand_ticket_to_counter` が受け取るのは `int` であり、渡している `mine` は `LibraryTicket` という**別の型**だから、自動では `number_` に格納された `int` 値に置き換わらない。
- **D** `mine` が保持する伝票番号（`number_`）は `private` だから、コンパイラが読み取れない。

**(2)** `mine` が保持する伝票番号（`number_` の値）をカウンター関数へ渡したい。

- **対策1（明示的な生リソースへのアクセス）**

```cpp
class LibraryTicket {
    public:
        explicit LibraryTicket(int number) : number_(number) {}
        ~LibraryTicket() {}
        int ticketNumber() const { return number_; }
    private:
        int number_;
};
```

- **対策2（暗黙の型変換）** … 対策1 に **`operator int()`** を追加した例。

```cpp
class LibraryTicket {
    public:
        explicit LibraryTicket(int number) : number_(number) {}
        ~LibraryTicket() {}
        int ticketNumber() const { return number_; }
        operator int() const { return number_; }
    private:
        int number_;
};
```

次のうち、**対策1・対策2 の違いと、`LibraryTicket` が伝票番号（生のリソースに相当する `int`）へのアクセスをどう与えるか**について、**適切な説明を一つ**選べ。

- **A** 対策2 の方がコードが短い。**インターフェースを短く書ける方が**常に望ましいから、対策1 より対策2 が優れている。
- **B** 対策1 も対策2 も、カウンターに渡る値としての `int` は同じであり、かつ **`number_` はどちらも `private` のまま**だから、**型の外から `number_` を直接読めない**という点に限れば、二つの対策に本質的な違いはない。
- **C** 対策2 を加えても `ticketNumber()` は残せる。**`int` を外へ渡すだけなら暗黙も明示的なアクセスも同じ**で、差は **`mine` と書くか `mine.ticketNumber()` と書くか**にすぎない。
- **D** 対策2（暗黙変換）では、**`int` の引数や `int` へ代入する式など「`int` が要求される所」に、`LibraryTicket` をそのまま渡せる**ようになる。**ソースを読んだだけでは、いつ `LibraryTicket` から伝票番号の `int` に置き換わったか気づきにくい**。
