#include "stdafx.h"

#include "WpfView.h"

IMPLEMENT_DYNAMIC(CWpfView, CView)

BEGIN_MESSAGE_MAP(CWpfView, CView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CWpfView::CWpfView()
{
}
CWpfView::~CWpfView()
{
}
#ifdef _DEBUG
#include <msclr\marshal_atl.h>
void CWpfView::AssertValid() const
{
	CView::AssertValid();
	//	HwndSource を構築していないまたは、ウィンドウハンドルが同じ。
	System::Windows::Interop::HwndSource^ src = m_source;
	ASSERT( src == nullptr || src->Handle.ToPointer() != nullptr );
}
void CWpfView::Dump( CDumpContext& dc ) const
{
	CView::Dump( dc );
	System::Windows::Interop::HwndSource^ src = m_source;
	if( src != nullptr )
	{
		CString str = _T( "RootVisual=" );
		//	RootVisual に張り付けているクラス名を表示する
		if( src->RootVisual != nullptr )
		{
			str += msclr::interop::marshal_as<CString>( src->RootVisual->GetType()->FullName ) + _T( "\n" );
		}
		else
		{
			str += _T( "\tnullptr\n" );
		}
		dc << str;
	}
}
#endif
BOOL CWpfView::PreCreateWindow( CREATESTRUCT& cs )
{
	//cs.style &= ~WS_BORDER;			//	ボーダーがついていると WS_EX_CLIENTEDGE に変更される(互換機能)
	auto result = CView::PreCreateWindow( cs );
	cs.style |= WS_CLIPCHILDREN;
	//cs.dwExStyle &= WS_EX_CLIENTEDGE;	//	アプリケーションのスタイルなので標準という概念はとらない
	return result;
}
System::Windows::Interop::HwndSource^ CWpfView::CreateHwndSource()
{
	_ASSERTE( m_hWnd != nullptr );	//	ウィンドウを作る前に呼び出しても処理できないのでアサーションする
	if( m_hWnd == nullptr ) return nullptr;
	//	複数回呼ばれても問題ないようにしておく(実際複数回呼び出される可能性があるため)
	System::Windows::Interop::HwndSource^ src = m_source;
	if( src != nullptr ) return src;

	m_source = gcnew System::Windows::Interop::HwndSource(
		0, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,	0,	//	ClassStyle, WindowStyle, ExStyle
		0, 0, 0, 0,									//	WPFコンテンツ領域(サイズを0にするとHWNDのリサイズで自動追従する)
		L"CWpfView::Client",						//	SPY++ 向けウィンドウタイトル名(コンパイラが自動的に良しなにしてくれる)
		System::IntPtr( m_hWnd ) );					//	親ウィンドウ

	//	HwndSource が作成されると連動コントロール用のHWNDが作成される(WPFコントロール本体はウィンドウレスコントロール)
	auto hwnd = GetHwndSourceWindow();
	_ASSERT( hwnd != nullptr );
	if( hwnd == nullptr ) return nullptr;
	//	クライアント領域全体に再配置する
	CRect rc;
	GetClientRect( &rc );
	::SetWindowPos( hwnd, nullptr, 0, 0, rc.right, rc.bottom, SWP_NOZORDER );
	return m_source;
}
void CWpfView::OnDraw( CDC* )
{
	//	OnDrawでの描画は必要ない
}
BOOL CWpfView::OnEraseBkgnd( CDC* pDC )
{
	//	WPFコントロールを張り付けていない場合は、ワークスペースカラーで塗りつぶす
	if( GetHwndSource() == nullptr )
	{
		CBrush* pbr = CBrush::FromHandle( GetSysColorBrush( COLOR_APPWORKSPACE ) );
		if( pbr != nullptr )
		{
			CRect rc;
			pDC->GetClipBox( &rc );
			pDC->FillRect( &rc, pbr );
			return TRUE;	//	描画したのでデフォルト処理は不要
		}
	}
	return CView::OnEraseBkgnd( pDC );
}
void CWpfView::OnDestroy()
{
	// gcroot<T> には reset メソッドがないので、参照を解除するため null をセットする(Viewは自動的に削除されるので必要はないが...)
	DestroyHwndSource();
	CView::OnDestroy();
}
void CWpfView::OnSize( UINT nType, int cx, int cy )
{
	ASSERT_VALID( this );

	CView::OnSize( nType, cx, cy );

	//	クライアント領域全域にウィンドウをリサイズする(どこにいるかわからないので常に全面再配置)
	auto hwnd = GetHwndSourceWindow();
	if( hwnd != nullptr )
	{
		::SetWindowPos( hwnd, nullptr, 0, 0, cx, cy, SWP_NOZORDER|SWP_FRAMECHANGED );
	}
}
void CWpfView::OnSetFocus( CWnd* pOldWnd )
{
	ASSERT_VALID( this );

	CView::OnSetFocus( pOldWnd );

	//	子ウィンドウにフォーカスを受け渡す(フォーカスを受け取る==ウィンドウが表示されている)
	auto hwnd = GetHwndSourceWindow();
	if( hwnd != nullptr )
	{
		::SetFocus( hwnd );
	}
}
