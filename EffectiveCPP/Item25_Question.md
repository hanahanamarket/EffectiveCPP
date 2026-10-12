# 設問

`swap` は「2つのオブジェクトの値を互いに入れ替える」操作で、標準ライブラリの `std::swap` は概ね次のように、**一時オブジェクトへのコピーを3回**行う実装になっている。

```cpp
namespace std {
    template<typename T>
    void swap(T& a, T& b) {
        T temp(a);   // a を temp にコピー
        a = b;       // b を a にコピー
        b = temp;    // temp を b にコピー
    }
}
```

ここで、実体を別オブジェクトへのポインタ1本だけで保持する「pimplイディオム」のクラス `Widget` を考える。

```cpp
class WidgetImpl { /* 大きなデータを多数持つ。コピーは高コスト */ };

class Widget {
public:
    Widget(const Widget& rhs);
    Widget& operator=(const Widget& rhs) { /* *pImpl = *(rhs.pImpl); など */ }
private:
    WidgetImpl* pImpl;   // 実装本体へのポインタ
};
```

各問は独立している。

---

## (1)

2つの `Widget` を `std::swap(w1, w2)` で入れ替えると、上の既定実装がそのまま使われる。この場合に生じる**主な問題**として、最も適切なものを一つ選べ。

- **A** `pImpl` が `private` なので、`std::swap` のテンプレートがコンパイルエラーになる。
- **B** 値を入れ替えるには `pImpl` という**ポインタ1本を交換すれば十分**なのに、既定実装は `Widget` と `WidgetImpl` を**まるごと3回コピー**してしまい、非常に非効率である。
- **C** `std::swap` は内部で例外を投げるため、`w1` と `w2` の値が壊れる。
- **D** ポインタを交換するだけなので、既定実装でも常に最速であり、問題はない。

---

## (2)

`Widget` と `WidgetImpl` が**クラステンプレート** `Widget<T>` / `WidgetImpl<T>` だとする。`pImpl` だけを入れ替える効率的な処理は、`Widget<T>` の **public メンバ関数 `swap`**（例外を投げない）として用意済みとする。

このとき、**利用側のコードが「`T` 用に最適化された `swap` があればそれを、なければ `std::swap` を」呼べるようにする**ための方法として、最も適切なものを一つ選べ。

```cpp
template<typename T>
void doSomething(T& a, T& b) {
    // ここで a と b を入れ替えたい
}
```

- **A** `std` 名前空間の中に、`Widget<T>` 用の `swap` を**関数テンプレートの部分特化**として追加し、`doSomething` では `std::swap(a, b);` と書く。
- **B** `Widget` と同じ名前空間に、メンバ `swap` を呼び出す**非メンバ `swap` を置き**、`doSomething` では `using std::swap;` を書いたうえで **`swap(a, b);` と修飾なしで呼ぶ**。
- **C** `doSomething` の中で必ず `std::swap(a, b);` と**`std::` を付けて**呼ぶ。こうすれば常に最適な `swap` が選ばれる。
- **D** `std` 名前空間に、`Widget<T>` 用の新しい `swap` 関数テンプレートを**追加で定義**し、それを呼ぶ。
