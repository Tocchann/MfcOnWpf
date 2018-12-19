
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

IMPLEMENT_DYNCREATE(CMfcClockView, CView)

BEGIN_MESSAGE_MAP(CMfcClockView, CView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
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
	CView::AssertValid();
}

void CMfcClockView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
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
	if( CView::OnCreate( lpCreateStruct ) == -1 )
		return -1;
	auto source = gcnew System::Windows::Interop::HwndSource( 0, WS_CHILD | WS_VISIBLE, 0,
															  0, 0, 0, 0,	//	サイズを0,0にしておくと、リサイズに追従する
															  "WpfClock.Clock", System::IntPtr( m_hWnd ) );
	//source->RootVisual = gcnew WpfClock::Clock();
	source->RootVisual = gcnew WpfClock::PhoneButtonsPage();
	//source->RootVisual = gcnew WpfClock::PhotoListPage();
	System::Windows::Controls::Page^ page = dynamic_cast<System::Windows::Controls::Page^>(source->RootVisual);
	if( page != nullptr && page->Background == nullptr )
	{
		page->Background = System::Windows::SystemColors::WindowBrush;	//	Windowsの標準背景を強制的に配置(WPFのデフォルトと同じ処理)
	}
	WpfClock::PhoneButtonsPage^ buttonsPage = dynamic_cast<WpfClock::PhoneButtonsPage^>(page);
	if( buttonsPage != nullptr )
	{
		buttonsPage->RaiseEnterEvent += MAKE_DELEGATE( System::EventHandler, OnEnter );
	}
	SetHwndSource( source );
	return 0;
}
void CMfcClockView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	auto page = dynamic_cast<WpfClock::PhoneButtonsPage^>(GetHwndSource()->RootVisual);
	if( page != nullptr )
	{
		page->Text = L"";
	}
}
#include <msclr\marshal_atl.h>
void CMfcClockView::OnEnter( System::Object^ sender, System::EventArgs^ e )
{
	auto page = dynamic_cast<WpfClock::PhoneButtonsPage^>(sender);
	auto text = msclr::interop::marshal_as<CString>( page->Text );
	MessageBox( text );
	page->Text = L"";
	page->SetDefFocus();
}
