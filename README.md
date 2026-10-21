# EffectiveCPP

このリポジトリは、Effective C++ / Effective Modern C++ の学習用ノートと問題・解答をまとめたものです。

## Effective C++

| 項目 | 内容 | 問題 | 解答 |
|------|------|------|------|
| Item 3 | 可能ならいつでも `const` を使おう | [item3_Question.md](EffectiveCPP/item3_Question.md) | [item3_Answer.md](EffectiveCPP/item3_Answer.md) |
| Item 9 | コンストラクタやデストラクタ内では決して仮想関数を呼び出さないようにしよう | [Item09_Question.md](EffectiveCPP/Item09_Question.md) | [Item09_Answer.md](EffectiveCPP/Item09_Answer.md) |
| Item 10 | 代入演算子は `*this` への参照を戻す | [Item10_Question.md](EffectiveCPP/Item10_Question.md) | [Item10_Answer.md](EffectiveCPP/Item10_Answer.md) |
| Item 15 | リソース管理クラスでは、リソースへのアクセス手段を提供しよう | [Item15_Question.md](EffectiveCPP/Item15_Question.md) | [Item15_Answer.md](EffectiveCPP/Item15_Answer.md) |
| Item 18 | インタフェースは正しく使うことが容易で、誤って使うことが困難になるように設計しよう | [Item18_Question.md](EffectiveCPP/Item18_Question.md) | [Item18_Answer.md](EffectiveCPP/Item18_Answer.md) |
| Item 25 | 例外を送出しない `swap` のサポートも検討しよう | [Item25_Question.md](EffectiveCPP/Item25_Question.md) | [Item25_Answer.md](EffectiveCPP/Item25_Answer.md) |
| Item 26 | 変数定義は可能な限り先延ばししよう | [Item26_Question.md](EffectiveCPP/Item26_Question.md) | [Item26_Answer.md](EffectiveCPP/Item26_Answer.md) |
| Item 30 | インライン化の長所と短所を理解しよう | [Item30_Question.md](EffectiveCPP/Item30_Question.md) | [Item30_Answer.md](EffectiveCPP/Item30_Answer.md) |
| Item 42 | `typename` の二つの意味を理解しよう | [Item42_Question.md](EffectiveCPP/Item42_Question.md) | [Item42_Answer.md](EffectiveCPP/Item42_Answer.md) |
| Item 43 | テンプレート化された基底クラス内の名前へのアクセス方法を知っておこう | [Item43_Question.md](EffectiveCPP/Item43_Question.md) | [Item43_Answer.md](EffectiveCPP/Item43_Answer.md) |

Item 3 の追加解説: [item3.md](EffectiveCPP/item3.md)

## Effective Modern C++

| 項目 | 内容 | 問題 | 解答 | 概要 |
|------|------|------|------|------|
| Item 5 | 型を明示する理由がなければ `auto` を使う | [Item05_Question.md](EffectiveModernCPP/Item05_Question.md) | [Item05_Answer.md](EffectiveModernCPP/Item05_Answer.md) | [Item05_Overview.md](EffectiveModernCPP/Item05_Overview.md) |
| Item 17 | 自動的に生成される特殊メンバ関数を理解する | [Item17_Question.md](EffectiveModernCPP/Item17_Question.md) | [Item17_Answer.md](EffectiveModernCPP/Item17_Answer.md) | [Item17_Overview.md](EffectiveModernCPP/Item17_Overview.md) |
| Item 26 | ユニヴァーサル参照をとるオーバーロードは避ける | [Item26_Question.md](EffectiveModernCPP/Item26_Question.md) | [Item26_Answer.md](EffectiveModernCPP/Item26_Answer.md) | [Item26_Overview.md](EffectiveModernCPP/Item26_Overview.md) |
| Item 41 | コピー可能仮引数のムーブコストが安く、かつ常にコピーする場面では値渡しを検討する | [Item41_Question.md](EffectiveModernCPP/Item41_Question.md) | [Item41_Answer.md](EffectiveModernCPP/Item41_Answer.md) | [Item41_Overview.md](EffectiveModernCPP/Item41_Overview.md) |

## 使い方

- まずは問題ファイルで自分で考える
- 必要なら解答ファイルを確認する
- Effective Modern C++ の概要ファイルは、その項の要点を短くまとめたもの

## 備考

- このリポジトリは Markdown ベースの学習用資料です
- 実行用のコードプロジェクトではなく、理解確認用のノート集です
