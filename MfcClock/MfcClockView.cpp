
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
	ON_WM_CREATE()
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
int CMfcClockView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if( CWpfView::OnCreate( lpCreateStruct ) == -1 )
		return -1;
	//	RootVisual(WPF上のトップレベルのUIElement)をセットアップする。プログラムごとにユニークな値になるのでここでは定義しない。

	//	時計
	//auto rootVisual = SetRootVisual( gcnew WpfClock::Clock() );

	//	プッシュダイヤルっぽい何か
	//auto rootVisual = SetRootVisual( gcnew WpfClock::PhoneButtonsPage() );
	//rootVisual->RaiseEnterEvent += MAKE_DELEGATE( System::EventHandler, OnEnter );
	
	//	某書籍のリスト的なもの
	auto rootVisual = SetRootVisual( gcnew WpfClock::PhotoListPage() );
	return 0;
}
void CMfcClockView::OnInitialUpdate()
{
	CWpfView::OnInitialUpdate();
	auto phonePage = GetRootVisual<WpfClock::PhoneButtonsPage>();
	if( phonePage != nullptr )
	{
		phonePage->Text = L"";
	}
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
