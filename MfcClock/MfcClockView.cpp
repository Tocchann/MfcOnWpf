
// MfcClockView.cpp : CMfcClockView クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "MfcClock.h"
#endif

#include "MfcClockDoc.h"
#include "MfcClockView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMfcClockView

IMPLEMENT_DYNCREATE(CMfcClockView, CWpfView )

BEGIN_MESSAGE_MAP(CMfcClockView, CWpfView )
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND( ID_POPUP, &CMfcClockView::OnPopup )
	ON_UPDATE_COMMAND_UI( ID_NONE, &CMfcClockView::OnUpdateNone )
	ON_UPDATE_COMMAND_UI( ID_CLOCK, &CMfcClockView::OnUpdateClock )
	ON_UPDATE_COMMAND_UI( ID_PHONE, &CMfcClockView::OnUpdatePhone )
	ON_UPDATE_COMMAND_UI( ID_LISTBOX, &CMfcClockView::OnUpdateListbox )
	ON_UPDATE_COMMAND_UI( ID_ADDR, &CMfcClockView::OnUpdateAddr )
	ON_COMMAND( ID_NONE, &CMfcClockView::OnNone )
	ON_COMMAND( ID_CLOCK, &CMfcClockView::OnClock )
	ON_COMMAND( ID_PHONE, &CMfcClockView::OnPhone )
	ON_COMMAND( ID_LISTBOX, &CMfcClockView::OnListbox )
	ON_COMMAND( ID_ADDR, &CMfcClockView::OnAddr )
	ON_COMMAND( ID_NON_OWNER_POPUP, &CMfcClockView::OnNonOwnerPopup )
END_MESSAGE_MAP()

// CMfcClockView コンストラクション/デストラクション

CMfcClockView::CMfcClockView() noexcept
{
	// TODO: 構築コードをここに追加します。

}

CMfcClockView::~CMfcClockView()
{
}

// CMfcClockView 描画

// CMfcClockView の印刷
BOOL CMfcClockView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CMfcClockView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CMfcClockView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}


// CMfcClockView の診断

#ifdef _DEBUG
void CMfcClockView::AssertValid() const
{
	CWpfView::AssertValid();
}

void CMfcClockView::Dump(CDumpContext& dc) const
{
	CWpfView::Dump(dc);
}

CMfcClockDoc* CMfcClockView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMfcClockDoc)));
	return (CMfcClockDoc*)m_pDocument;
}
#endif //_DEBUG
// CMfcClockView メッセージ ハンドラー
void CMfcClockView::OnInitialUpdate()
{
	CWpfView::OnInitialUpdate();

	//	通常はここで一度だけ初期化する
	//if( GetHwndSource() == nullptr )
	//{
	//}

	auto phonePage = GetRootVisual<WpfClock::PhoneButtonsPage>();
	if( phonePage != nullptr )
	{
		phonePage->Text = L"";
	}
}
void CMfcClockView::OnNonOwnerPopup()
{
	auto window = gcnew WpfClock::PopupClock();
	window->ShowDialog();
}
void CMfcClockView::OnPopup()
{
	//	ホストオーナーをセットしたうえで、ポップアップウィンドウをダイアログ表示する(ここでは戻り値は受け取らない)
	auto window = gcnew WpfClock::PopupClock();
	auto helper = gcnew System::Windows::Interop::WindowInteropHelper( window );
	helper->Owner = System::IntPtr( AfxGetMainWnd()->GetSafeHwnd() );
	window->ShowDialog();
}
//	何もない状態
void CMfcClockView::OnUpdateNone( CCmdUI *pCmdUI )
{
	auto src = GetHwndSource();
	pCmdUI->SetCheck( (src == nullptr || src->RootVisual == nullptr) ? TRUE : FALSE );
}
void CMfcClockView::OnNone()
{
	DestroyHwndSource();
}
//	アナログ時計
void CMfcClockView::OnUpdateClock( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck( (GetRootVisual<WpfClock::Clock>() != nullptr) ? TRUE : FALSE );
}
void CMfcClockView::OnClock()
{
	SetRootVisual( gcnew WpfClock::Clock() );
}
//	電話っぽい何か
void CMfcClockView::OnUpdatePhone( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck( (GetRootVisual<WpfClock::PhoneButtonsPage>() != nullptr) ? TRUE : FALSE );
}
void CMfcClockView::OnPhone()
{
	auto rootVisual = SetRootVisual( gcnew WpfClock::PhoneButtonsPage() );
	//	サンプルなのでイベントハンドラのデリゲートを解除していない
	rootVisual->RaiseEnterEvent += MAKE_DELEGATE( System::EventHandler, OnEnter );
}
#include <msclr\marshal_atl.h>
void CMfcClockView::OnEnter( System::Object^ sender, System::EventArgs^ )
{
	//	送信元はルートのページなので、そっちをキャストして直接参照する
	auto page = dynamic_cast<WpfClock::PhoneButtonsPage^>(sender);
	auto text = msclr::interop::marshal_as<CString>( page->Text );
	if( text.IsEmpty() )
	{
		AfxMessageBox( _T( "番号を入力してください。" ) );
	}
	else
	{
		AfxMessageBox( _T( "電話っぽい感じでしょ？\n\n" ) + text, MB_ICONINFORMATION|MB_OK );
		page->Text = L"";
	}
	page->SetDefFocus();
}
//	マウスオーバーでアイテムサイズが変わるリストボックス
void CMfcClockView::OnUpdateListbox( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck( (GetRootVisual<WpfClock::PhotoListPage>() != nullptr) ? TRUE : FALSE );
}
void CMfcClockView::OnListbox()
{
	SetRootVisual( gcnew WpfClock::PhotoListPage() );
}
//	住所録っぽい何か
void CMfcClockView::OnUpdateAddr( CCmdUI *pCmdUI )
{
	pCmdUI->SetCheck( (GetRootVisual<WpfClock::AddrDetailPage>() != nullptr) ? TRUE : FALSE );
}
void CMfcClockView::OnAddr()
{
	SetRootVisual( gcnew WpfClock::AddrDetailPage() );
}
