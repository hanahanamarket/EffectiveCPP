# 設問

テンプレートパラメータの宣言では `class` と `typename` は同じ意味で使える。しかし、テンプレート本体の中で「テンプレートパラメータに依存するネストされた名前」を型として使う場面では、`typename` が**必須**になることがある。逆に、基底クラスの指定などでは `typename` を**書いてはいけない**場合もある。

各問は独立している。

---

## (1)

次の2つの宣言を考える。

```cpp
template<class T> class Widget;
template<typename T> class Widget;
```

これらの違いとして最も適切なものはどれか。

A. `class` はユーザ定義型専用で、`typename` は組み込み型も受け付ける

B. `typename` の方が新しいので、`class` は非推奨である

C. 言語規則上、この文脈では両者に違いはない

D. `class` を使うと `T` は必ずクラス型に制限される

---

## (2)

次の関数テンプレートを考える（設問用であり、このままでは標準に照らして正しくない）。

```cpp
template<typename C>
void print2nd(const C& container)
{
    if (container.size() >= 2) {
        C::const_iterator iter(container.begin());
        ++iter;
        int value = *iter;
        std::cout << value;
    }
}
```

`C::const_iterator iter(...);` が問題になる**主な理由**として最も適切なものはどれか。

A. `C::const_iterator` はネストされた依存名であり、特に指示がなければ型名と解釈されないから

B. `const_iterator` は必ず静的メンバ変数だから

C. テンプレート内ではイテレータを宣言できないから

D. `container.begin()` の戻り値型が `int` だから

---

## (3)

次の宣言を考える。

```cpp
template<typename C>
void f(const C& container, typename C::iterator iter);
```

この宣言における `typename` の扱いとして最も適切なものはどれか。

A. `C` にも `typename` を付ける必要がある

B. パラメータ宣言では `typename` は常に禁止される

C. `container` にも `typename` を付ける必要がある

D. `C::iterator` はネストされた依存型名なので `typename` が必要で、テンプレートパラメータ `C` 自体には不要である

---

## (4)

次のクラステンプレートを考える。

```cpp
template<typename T>
class Derived : public Base<T>::Nested {
public:
    explicit Derived(int x)
    : Base<T>::Nested(x)
    {
        typename Base<T>::Nested temp;
        // ...
    }
};
```

`typename` の使い方として最も適切な説明はどれか。

A. 基底クラス指定・初期化リスト・関数本体のいずれでも `typename` が必須である

B. 関数本体内では `typename` を付けてはいけない

C. 基底クラス指定と初期化リストでは `typename` を使えず、関数本体内のネストされた依存型名では `typename` が必要である

D. `Base<T>::Nested` は依存名ではないので、どこでも `typename` は不要である
