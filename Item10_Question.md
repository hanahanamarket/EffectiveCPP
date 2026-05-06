# Effective C++ Item 10 理解度テスト（問題編）

## Q1
次のコードの実行結果として、**正しい解釈**はどれですか。

```cpp
int x, y, z;
x = y = z = 15;
```

### 選択肢

A. 左から順に評価されるので、`((x = y) = z) = 15`

B. 右から左へ結びつくので、`x = (y = (z = 15))`

C. 複数の `=` を連続で結びつけられなず、コンパイルエラーとなる。

D. `z = 15` の結果は `bool` になるので `x` と `y` に `true` が代入される。

---

## Q2
```cpp
#include <iostream>
using namespace std;

class Yen {
public:
    int money;  // ・・・ 選択肢A

    Yen(int m = 0) : money(m) {}

    Yen operator+=(const Yen& rhs)  // ・・・ 選択肢B・C・D
    {
        money += rhs.money;
        return *this;
    }

    void print(const char* name) const {
        cout << name << ": " << money << "円" << endl;
    }
};

int main() {
    Yen wallet(150000);
    Yen salary(250000);
    Yen bonus(1000000);

    (wallet += salary) += bonus;

    wallet.print("wallet");
}
```

上記コードでは、C++の組み込みの加算代入演算と同様に演算がされることを期待して、Yenクラスに加算代入演算を定義し、それを呼び出す処理を実行しています。
しかしながら、期待値と実測値は以下の通りで、C++の組み込みの加算代入演算とは異なる結果となりました。

期待される出力結果:  `wallet: 1400000円`
実際に出力された結果:  `wallet: 400000円`

期待値と実測値に差異が生まれた原因を修正するには、選択肢のどれを適用すれば良いですか。
ただし、main の式は変更せず、Yen クラス定義内のみ修正可とする。

### 選択肢

A. money を int から long long に変更する。
B. operator+= の戻り値型を Yen から Yen& に変更し、return *this; で参照を返す。
C. operator+= の引数を const Yen& rhs から Yen rhs（値渡し）に変更する。
D. operator+= の戻り値を const Yen& に変更する。