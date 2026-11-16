# 自動的に生成される特殊メンバ関数を理解する

<!-- %%{init: {'theme':'base','securityLevel':'loose','htmlLabels':true,'themeCSS':'strong, b, tspan[font-weight=bold] { color: #ff7070 !important; fill: #ff7070 !important; } svg, foreignObject, foreignObject div, .nodeLabel { color-scheme: light dark; } svg { background-color: transparent; } .nodeLabel { display: block !important; } .node rect, .node polygon, .node circle, .classGroup rect, rect.actor { fill: var(--vscode-editorWidget-background, light-dark(#fbfbf8, #2a2a27)) !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; stroke-width: 1.5px !important; } .nodeLabel, .nodeLabel p, .nodeLabel span, .nodeLabel div, .label, .edgeLabel, .edgeLabel span, .edgeLabel p, .edgeLabel tspan, .edgeLabel text, .cluster-label, .cluster-label span, .cluster-label div, foreignObject, foreignObject div, foreignObject p, foreignObject span, .classGroup text, .classTitleText, .classText, .stereotypeText, text.actor, .messageText, .labelText, .loopText, .noteText, tspan { color: var(--vscode-editor-foreground, light-dark(#1a1a1a, #f3f1ea)) !important; fill: var(--vscode-editor-foreground, light-dark(#1a1a1a, #f3f1ea)) !important; } .edgeLabel { background-color: var(--vscode-editor-background, light-dark(#f4f4f1, #1c1c1a)) !important; } .edgeLabel rect, .labelBkg { fill: var(--vscode-editor-background, light-dark(#f4f4f1, #1c1c1a)) !important; } .cluster rect { fill: transparent !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; stroke-width: 1.5px !important; } .edgePath .path, .flowchart-link, .relationshipLine, path.relation, .messageLine0, .messageLine1, .actor-line, .classGroup line { stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; fill: none !important; } .arrowheadPath, marker path, marker polygon { fill: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; } .nodeLabel pre { display: block; margin: 0; text-align: left; background: transparent; border: 0; } .nodeLabel code { color: var(--vscode-editor-foreground, light-dark(#1a1a1a, #f3f1ea)) !important; background-color: var(--vscode-textCodeBlock-background, light-dark(#e0ded8, #3d3d38)) !important; display: block; margin: 0; text-align: left; white-space: pre-wrap; font-family: Consolas, monospace; } .note { fill: var(--vscode-editorWidget-background, light-dark(#f3efe6, #34342f)) !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; }','flowchart':{'htmlLabels':true,'wrappingWidth':900,'padding':12,'nodeSpacing':32,'rankSpacing':40},'themeVariables':{'background':'transparent','primaryColor':'#fbfbf8','primaryTextColor':'#1a1a1a','primaryBorderColor':'#1a1a1a','lineColor':'#1a1a1a','secondaryColor':'#f4f4f1','tertiaryColor':'#f4f4f1','textColor':'#1a1a1a','mainBkg':'#fbfbf8','nodeBorder':'#1a1a1a','edgeLabelBackground':'#f4f4f1','nodeTextColor':'#1a1a1a','clusterBkg':'transparent','clusterBorder':'#1a1a1a','titleColor':'#1a1a1a','fontSize':'16px','actorBkg':'#fbfbf8','actorBorder':'#1a1a1a','actorTextColor':'#1a1a1a','actorLineColor':'#1a1a1a','signalColor':'#1a1a1a','signalTextColor':'#1a1a1a'}}}%% -->

```mermaid

flowchart TD
    subgraph canvas ["項目17の論理展開"]
        direction TB

        open --> Prerequisite --> SpecialMemberFunc
                 Prerequisite--> RuleOfThree
                 Prerequisite--> EqualDefault
        open -.-> Requirements

        %% 特殊関数とは
        Prerequisite["前提知識"]
        SpecialMemberFunc["特殊関数とは<br/>C++自ら生成するメンバ関数を意味する"]
        SpecialMemberFunc98["<div style='text-align:center !important'>C++98</div><div style='text-align:left !important;white-space:pre'>Widget();    // デフォルトコンストラクタ<br/>~Widget();    // デストラクタ<br/>Widget(const Widget&amp; rhs);    // コピーコンストラクタ<br/>Widget&amp; operator=(const Widget&amp; rhs);    // コピー代入演算子<br/><br/><br/>"]
        SpecialMemberFunc11["<div style='text-align:center !important'>C++11</div><div style='text-align:left !important;white-space:pre'>Widget();    // デフォルトコンストラクタ<br/>~Widget();    // デストラクタ<br/>Widget(const Widget&amp; rhs);    // コピーコンストラクタ<br/>Widget&amp; operator=(const Widget&amp; rhs);    // コピー代入演算子<br/><span style='color:#ff7070 !important'>Widget(Widget&amp;&amp; rhs);    // ムーブコンストラクタ<br/>Widget&amp; operator=(Widget&amp;&amp; rhs);    // ムーブ代入演算子</span></div>"]
        MoveByMember["ムーブコンストラクタとムーブ代入の挙動"]


        %% 特殊関数の生成ルールと成立背景
        RuleOfThree[三関数同時ルール</br>以下のいづれか1つでも宣言する場合は、<br/>この３つを全てしなさいというもの</br>・コピーコンストラクタ</br>・コピー代入演算子</br>・デストラクタ]
        RuleOfMoveCpp11["C++11のコンパイラのルール<br/>ムーブ演算を生成しない"]
        MoveConstructorCondition["ムーブ演算の自動生成条件<br/>・クラスがコピー演算を宣言していない。<br/>・クラスがムーブ演算を宣言していない。<br/>・クラスがデストラクタを宣言していない。"]
        EqualDefault[" =defaultの付与<br/>コンパイラで自動生成されるムーブ演算やコピー演算と<br/>同様のものを明示的に宣言するときに=defaultを付ける"]
        GoodExampleDefault["=defaultと定義する方式"]


        %% 実装したい要件
        Requirements["実装したい要件"]
        Polymorphism["多態な基底クラス<br>派生クラスオブジェクトを操作するインターフェースを定義したクラス"]
        VirtualDestructor["仮想デストラクタを持つのが通例"]
        ExplicitVirtualDestructor["仮想デストラクタの明示的な宣言が必要"]
        WantMove["ムーブを可能にしたい"]
        NeedMoveDeclare["ムーブを明示的に宣言"]
        AddDefaultToCopy["コピー可能するために=defaultを追加"]
        

        %% メンバ単位のムーブと、通常のコピー演算
        MemberwiseAsCopy["メンバ単位のムーブ<br/>ムーブ非対応な型はコピー演算により「ムーブ」"]
        CopyInappropriate["通常のコピー演算ではそのクラスにふさわしくないことを示唆する"]


        %% 特殊関数とは のつながり
        SpecialMemberFunc --> SpecialMemberFunc98 --> MoveByMember
        SpecialMemberFunc --> SpecialMemberFunc11 --> MoveByMember
        MoveByMember --> MemberwiseAsCopy --> CopyInappropriate


        %% 特殊関数の生成ルールと成立背景 のつながり
        CopyInappropriate -- "C++98" --> RuleIgnored["重要視されず"]
        CopyInappropriate -- "C++11" --> RuleEnforced["重要視される"] --> RuleOfMoveCpp11 --> MoveConstructorCondition
        RuleOfThree --> MoveConstructorCondition

        %% 実装したい要件 のつながり
        Requirements -.-> Polymorphism -. "スライス問題を避けるため" .-> VirtualDestructor
        MoveConstructorCondition -. "ムーブ演算の生成が抑制" .-> ExplicitVirtualDestructor -.-> GoodExampleDefault
        VirtualDestructor -.-> MoveConstructorCondition
        Requirements -.-> WantMove -. "ムーブ演算の生成が抑制" .-> NeedMoveDeclare
        NeedMoveDeclare -. "クラス内でムーブ演算を宣言すればコピー演算を無効にする" .-> AddDefaultToCopy
        EqualDefault -.-> AddDefaultToCopy -.-> GoodExampleDefault
    end
```

### クラス図は、ムーブの対象になる部分を並べたものです。
```mermaid
%%{init: {'theme':'base','securityLevel':'loose','htmlLabels':false,'themeCSS':'strong, b, tspan[font-weight=bold] { color: #ff7070 !important; fill: #ff7070 !important; } svg, foreignObject, foreignObject div, .nodeLabel { color-scheme: light dark; } svg { background-color: transparent; } .node rect, .node polygon, .node circle, .classGroup rect, rect.actor { fill: var(--vscode-editorWidget-background, light-dark(#fbfbf8, #2a2a27)) !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; stroke-width: 1.5px !important; } .nodeLabel, .nodeLabel p, .nodeLabel span, .nodeLabel div, .label, .edgeLabel, .edgeLabel span, .edgeLabel p, .edgeLabel tspan, .edgeLabel text, .cluster-label, .cluster-label span, .cluster-label div, foreignObject, foreignObject div, foreignObject p, foreignObject span, .classGroup text, .classTitleText, .classText, .stereotypeText, text.actor, .messageText, .labelText, .loopText, .noteText, tspan { color: var(--vscode-editor-foreground, light-dark(#1a1a1a, #f3f1ea)) !important; fill: var(--vscode-editor-foreground, light-dark(#1a1a1a, #f3f1ea)) !important; } .edgeLabel { background-color: var(--vscode-editor-background, light-dark(#f4f4f1, #1c1c1a)) !important; } .edgeLabel rect, .labelBkg { fill: var(--vscode-editor-background, light-dark(#f4f4f1, #1c1c1a)) !important; } .cluster rect { fill: transparent !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; stroke-width: 1.5px !important; } .edgePath .path, .flowchart-link, .relationshipLine, path.relation, .messageLine0, .messageLine1, .actor-line, .classGroup line { stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; fill: none !important; } .arrowheadPath, marker path, marker polygon { fill: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; } .note { fill: var(--vscode-editorWidget-background, light-dark(#f3efe6, #34342f)) !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; } g.edgeLabel foreignObject div, g.edgeLabel span.edgeLabel, span.edgeLabel, div.labelBkg { background-color: var(--vscode-editorWidget-background, light-dark(#fbfbf8, #2a2a27)) !important; color: var(--vscode-editor-foreground, light-dark(#1a1a1a, #f3f1ea)) !important; opacity: 1 !important; } g.edgeLabel rect, rect.background { fill: var(--vscode-editorWidget-background, light-dark(#fbfbf8, #2a2a27)) !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; opacity: 1 !important; } g.edgeLabel text, g.edgeLabel tspan, .edgeLabel tspan { fill: var(--vscode-editor-foreground, light-dark(#1a1a1a, #f3f1ea)) !important; stroke: var(--vscode-editorWidget-background, light-dark(#fbfbf8, #2a2a27)) !important; stroke-width: 4px !important; paint-order: stroke fill !important; } path.outer-path, path.label-container, path.basic, .outer-path path, .label-container path, g.basic path, .node .outer-path path { fill: var(--vscode-editorWidget-background, light-dark(#fbfbf8, #2a2a27)) !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; } .divider path, .neo-line path { fill: none !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; stroke-width: 1.5px !important; } .annotation-group span, .label-group span, .members-group span, .methods-group span, .annotation-group div, .label-group div, .members-group div, .methods-group div { color: var(--vscode-editor-foreground, light-dark(#1a1a1a, #f3f1ea)) !important; fill: var(--vscode-editor-foreground, light-dark(#1a1a1a, #f3f1ea)) !important; opacity: 1 !important; } #compositionStart path, #compositionEnd path, #extensionStart path, #extensionEnd path, #aggregationStart path, #aggregationEnd path, #dependencyStart path, #dependencyEnd path { fill: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; } svg g.methods-group.text g.label foreignObject div span.nodeLabel.markdown-node-label p strong, svg g.methods-group.text g.label foreignObject div span.nodeLabel.markdown-node-label strong, svg g.methods-group.text g.label foreignObject div span.nodeLabel.markdown-node-label p b, svg g.methods-group.text g.label text tspan.text-inner-tspan[font-weight=bold] { color: #ff7070 !important; fill: #ff7070 !important; }','themeVariables':{'background':'transparent','primaryColor':'#fbfbf8','primaryTextColor':'#1a1a1a','primaryBorderColor':'#1a1a1a','lineColor':'#1a1a1a','textColor':'#1a1a1a','mainBkg':'#fbfbf8','nodeBorder':'#1a1a1a','edgeLabelBackground':'#f4f4f1','nodeTextColor':'#1a1a1a','titleColor':'#1a1a1a','fontSize':'16px'}}}%%
classDiagram
    class Base {
        <<ムーブあり>>
        +Base(Base&& rhs)
        +operator=(Base&& rhs) Base&
    }
    class String {
        <<ムーブあり>>
        +String(**String&& rhs**)
        +operator=(**String&& rhs**) __String&__
    }
    class Legacy {
        <<コピーのみ>>
        +Legacy(**const Legacy& rhs**)
        +operator=(**const Legacy& rhs**) __Legacy&__
    }
    class Widget {
        +Widget(Widget&& rhs)
        +operator=(Widget&& rhs) Widget&
        -String name
        -Legacy tag
    }
    Base <|-- Widget : 基底部分
    Widget *-- String : name
    Widget *-- Legacy : tag
```

### シーケンス図は、Widget dst(std::move(src)) でその部分を宣言順に初期化する流れです。
```mermaid
%%{init: {'theme':'base','securityLevel':'loose','htmlLabels':false,'themeCSS':'strong, b, tspan[font-weight=bold] { color: #ff7070 !important; fill: #ff7070 !important; } svg, foreignObject, foreignObject div, .nodeLabel { color-scheme: light dark; } svg { background-color: transparent; } .node rect, .node polygon, .node circle, .classGroup rect, rect.actor { fill: var(--vscode-editorWidget-background, light-dark(#fbfbf8, #2a2a27)) !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; stroke-width: 1.5px !important; } .nodeLabel, .nodeLabel p, .nodeLabel span, .nodeLabel div, .label, .edgeLabel, .edgeLabel span, .edgeLabel p, .edgeLabel tspan, .edgeLabel text, .cluster-label, .cluster-label span, .cluster-label div, foreignObject, foreignObject div, foreignObject p, foreignObject span, .classGroup text, .classTitleText, .classText, .stereotypeText, text.actor, .messageText, .labelText, .loopText, .noteText, tspan { color: var(--vscode-editor-foreground, light-dark(#1a1a1a, #f3f1ea)) !important; fill: var(--vscode-editor-foreground, light-dark(#1a1a1a, #f3f1ea)) !important; } .edgeLabel { background-color: var(--vscode-editor-background, light-dark(#f4f4f1, #1c1c1a)) !important; } .edgeLabel rect, .labelBkg { fill: var(--vscode-editor-background, light-dark(#f4f4f1, #1c1c1a)) !important; } .cluster rect { fill: transparent !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; stroke-width: 1.5px !important; } .edgePath .path, .flowchart-link, .relationshipLine, path.relation, .messageLine0, .messageLine1, .actor-line, .classGroup line { stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; fill: none !important; } .arrowheadPath, marker path, marker polygon { fill: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; } .note { fill: var(--vscode-editorWidget-background, light-dark(#f3efe6, #34342f)) !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; }','themeVariables':{'background':'transparent','primaryColor':'#fbfbf8','primaryTextColor':'#1a1a1a','primaryBorderColor':'#1a1a1a','lineColor':'#1a1a1a','textColor':'#1a1a1a','mainBkg':'#fbfbf8','actorBkg':'#fbfbf8','actorBorder':'#1a1a1a','actorTextColor':'#1a1a1a','actorLineColor':'#1a1a1a','signalColor':'#1a1a1a','signalTextColor':'#1a1a1a','noteBkgColor':'#f3efe6','noteTextColor':'#1a1a1a','noteBorderColor':'#1a1a1a','fontSize':'16px'}}}%%
sequenceDiagram
    participant C as 呼出し
    participant W as Widget(Widget&&)
    participant B as Base
    participant N as name
    participant T as tag

    C->>W: dst(std::move(src))
    W->>B: Base(std::move(src))
    B-->>W: ムーブコンストラクタ
    W->>N: name(std::move(src.name))
    N-->>W: ムーブコンストラクタ
    W->>T: tag(std::move(src.tag))
    T-->>W: コピーコンストラクタ
```

### ムーブ代入 dst = std::move(src) も同じ順です。呼ばれるのは各部分の代入演算子です。
```mermaid
%%{init: {'theme':'base','securityLevel':'loose','htmlLabels':false,'themeCSS':'strong, b, tspan[font-weight=bold] { color: #ff7070 !important; fill: #ff7070 !important; } svg, foreignObject, foreignObject div, .nodeLabel { color-scheme: light dark; } svg { background-color: transparent; } .node rect, .node polygon, .node circle, .classGroup rect, rect.actor { fill: var(--vscode-editorWidget-background, light-dark(#fbfbf8, #2a2a27)) !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; stroke-width: 1.5px !important; } .nodeLabel, .nodeLabel p, .nodeLabel span, .nodeLabel div, .label, .edgeLabel, .edgeLabel span, .edgeLabel p, .edgeLabel tspan, .edgeLabel text, .cluster-label, .cluster-label span, .cluster-label div, foreignObject, foreignObject div, foreignObject p, foreignObject span, .classGroup text, .classTitleText, .classText, .stereotypeText, text.actor, .messageText, .labelText, .loopText, .noteText, tspan { color: var(--vscode-editor-foreground, light-dark(#1a1a1a, #f3f1ea)) !important; fill: var(--vscode-editor-foreground, light-dark(#1a1a1a, #f3f1ea)) !important; } .edgeLabel { background-color: var(--vscode-editor-background, light-dark(#f4f4f1, #1c1c1a)) !important; } .edgeLabel rect, .labelBkg { fill: var(--vscode-editor-background, light-dark(#f4f4f1, #1c1c1a)) !important; } .cluster rect { fill: transparent !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; stroke-width: 1.5px !important; } .edgePath .path, .flowchart-link, .relationshipLine, path.relation, .messageLine0, .messageLine1, .actor-line, .classGroup line { stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; fill: none !important; } .arrowheadPath, marker path, marker polygon { fill: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; } .note { fill: var(--vscode-editorWidget-background, light-dark(#f3efe6, #34342f)) !important; stroke: var(--vscode-editor-foreground, light-dark(#1a1a1a, #e4e0d6)) !important; }','themeVariables':{'background':'transparent','primaryColor':'#fbfbf8','primaryTextColor':'#1a1a1a','primaryBorderColor':'#1a1a1a','lineColor':'#1a1a1a','textColor':'#1a1a1a','mainBkg':'#fbfbf8','actorBkg':'#fbfbf8','actorBorder':'#1a1a1a','actorTextColor':'#1a1a1a','actorLineColor':'#1a1a1a','signalColor':'#1a1a1a','signalTextColor':'#1a1a1a','noteBkgColor':'#f3efe6','noteTextColor':'#1a1a1a','noteBorderColor':'#1a1a1a','fontSize':'16px'}}}%%
sequenceDiagram
    participant C as 呼出し
    participant W as operator=(Widget&&)
    participant B as Base
    participant N as name
    participant T as tag

    C->>W: dst = std::move(src)
    W->>B: Base::operator=(std::move(src))
    B-->>W: ムーブ代入
    W->>N: name = std::move(src.name)
    N-->>W: ムーブ代入
    W->>T: tag = std::move(src.tag)
    T-->>W: コピー代入
    W-->>C: Widget&

```