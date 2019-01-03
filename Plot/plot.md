# セッションタイトル

CView にも UIElementを！

今日話すこと。

* CView の派生クラスを作って、WPF の Visual オブジェクトを張り付けるようビュークラスを作る時に考えておくこと。

今日話さないこと。

* TAB移動やニーモニック処理などのキーボード処理回り



## ポイントをどこに置く？

MFC でサポートしている .NET Framework のGUIライブラリは Windows Forms。

WPF については直接サポートしていない。

## Windows Forms の貼り付け

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
 +CToolBar
 +CWpfView(CView派生)
  +System::Windows::Interop::HwndSource
   +RootVisual(UIElement)
 +CStatusBar
```

