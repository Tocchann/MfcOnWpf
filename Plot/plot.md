# セッションタイトル

CView にも UIElement を！

今日話すこと。

* ざっくりと Forms との違い
* MFC アプリの CView 部分に、WPF の Visual オブジェクトを貼り付けてみる
* 多分唯一の問題点(というか実装仕様として定着してるとみるべきもの。。。)
* まとめ

## Windows Forms との主な違い

* .NET Framework 側で完結する形でフォローがあるかどうか。
  * WPFはフォローあり
  * WinForms はフォローなし。
    * Win32 からは、ActiveXコントロールチックに扱う(実際内部処理はそっくり)

## Windows Forms をビューとして使う

* MFC のサポートがある
  * アプリケーションクラスのコンストラクタで System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException); を呼び出す
* class CWinFormsView を利用。
  * コンストラクタにビューに張り付けるFormクラスの型をセットするだけ。
* mfcmifc80.dll を参照する必要がある。
  * MFCのライブラリフォルダにある
    * ローカルに参照
    * ローカルコピー
    * ローカルインストール
      * .NET Framework には含まれない
      * VCランタイムには含まれない

## WPF をビューとして使う

* System::Windows::Interop::HwndSource を全面貼り付け
  * 子ウィンドウとして設定する
    * リサイズは親側で対応
    * フォーカスは全面譲渡

## 実際に使ってみる!

MFC のサポートする Windows Forms のクラス群。  
.NET Framework 2.0 の時に C++/CLI の機能として追加。  
Windows Forms 側には、Native HWND への貼り付けは双方で連動しないと支えられるようになっていない。

* `class CWinFormsControl<T> : public CWnd`
  * コントロールとして直接貼り付け
* `class CWinFormsDialog<T> : public CDialog`
  * ダイアログとして貼り付け(CDhtmlDialogのように利用する)
  * ポップアップで使うなら、Form をそのまま表示すればいい(ShowDialog/Show)。
* `class CWinFormsView : public CView{}`
  * View として貼り付け(CFormView のように利用する)

## Windows Presentation Foundation(WPF) の貼り付け

WPF 側に、Native HWND の子ウィンドウとして張り付ける機能が標準搭載されている。  
ホスト側に複雑な構築を行う必要がない。

`System::Windows::Interop::HwndSource` を子ウィンドウとして任意位置に張り付け、RootVisual に表示したいVisual オブジェクトを配置することであとはほぼ全自動で配置される。

> Popup するウィンドウは、直接Windowオブジェクトを構築すればよい。  
> オーナーウィンドウは適切に処理される。

## ダイアログ内で、子コントロールとして張り付ける場合

HwndSource のコンストラクタで、親ウィンドウ上の位置を指定することで固定的な位置に配置が可能。
ただし、固定されるので可変ウィンドウでは通用しない。

実行時再配置する場合は、コンストラクタでは、座標を 0,0,0,0 で渡して、WM_SIZE でほかのコントロールと一緒に再配置する。


## Native HWND と WPF とのフォーカス連動

ダイアログ内ダイアログと同様、タブ移動で、兄弟ウィンドウを超えて移動する場合は、



---
WinForms/WPF の違いをしゃべるか？  
サポートの有無はあってもいいと思うが。。。

デフォルトから作った場合に追記する部分とカットする部分。

アプリケーションクラスのコンストラクタにある `_MANAGED` の中身は WinForms 専用。


MFC には、WPF連携サポートは用意されていない。

WPF 側に、Native HWND との連携用クラス( `System::Windows::Interop::HwndSource` )が用意されている。

今回作るものは。。。

MFC の CView レイヤー部分を丸ごと置き換える形で張り付ける。

ウィンドウレイヤー(SDI)

```Text
CMainFrame
+CWpfView(CView派生)
 +System::Windows::Interop::HwndSource
  +RootVisual(UIElement)
```

