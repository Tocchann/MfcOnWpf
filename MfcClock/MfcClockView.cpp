
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
END_MESSAGE_MAP()

// CMfcClockView コンストラクション/デストラクション

CMfcClockView::CMfcClockView() noexcept
{
	// TODO: 構築コードをここに追加します。

}

CMfcClockView::~CMfcClockView()
{
}

BOOL CMfcClockView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。
	cs.style |= WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

// CMfcClockView 描画

void CMfcClockView::OnDraw(CDC* /*pDC*/)
{
	CMfcClockDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}


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

	//	WPFコントロールを子ウィンドウとして張り付ける。
	m_source = gcnew System::Windows::Interop::HwndSource( 0, WS_CHILD | WS_VISIBLE, 0,
										 0, 0, 0, 0,	//	サイズを0,0にしておくと、リサイズに追従する
										 "WpfClock.Clock", System::IntPtr( m_hWnd ) );
	//WpfClock::Clock^ page = gcnew WpfClock::Clock();
	m_source->RootVisual = gcnew WpfClock::Clock();
	m_source->RootVisual = gcnew WpfClock::PhoneButtonsPage();
	m_source->RootVisual = gcnew WpfClock::PhotoListPage();
	System::Windows::Controls::Page^ page = dynamic_cast<System::Windows::Controls::Page^>(m_source->RootVisual);
	if( page != nullptr )
	{
		page->Background = System::Windows::SystemColors::WindowBrush;	//	Windowsの標準背景を強制的に配置(WPFのデフォルトと同じ処理)
	}
	WpfClock::PhoneButtonsPage^ buttonsPage = dynamic_cast<WpfClock::PhoneButtonsPage^>(page);
	if( buttonsPage != nullptr )
	{
		buttonsPage->RaiseEnterEvent += MAKE_DELEGATE( System::EventHandler, OnEnter );
	}
	return 0;
}
void CMfcClockView::OnSize( UINT nType, int cx, int cy )
{
	CView::OnSize( nType, cx, cy );
	//	クライアント領域全面を利用してもらうので、サイズが変わったらリサイズしてやることで対応
	auto hwnd = reinterpret_cast<HWND>(m_source->Handle.ToPointer());
	::SetWindowPos( hwnd, nullptr, 0, 0, cx, cy, SWP_NOZORDER|SWP_FRAMECHANGED );
}

void CMfcClockView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	auto page = dynamic_cast<WpfClock::PhoneButtonsPage^>(m_source->RootVisual);
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
}
