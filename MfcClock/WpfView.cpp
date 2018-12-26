#include "stdafx.h"

#include "WpfView.h"

IMPLEMENT_DYNAMIC(CWpfView, CView)

BEGIN_MESSAGE_MAP(CWpfView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

CWpfView::CWpfView()
{
}
CWpfView::~CWpfView()
{
}
BOOL CWpfView::PreCreateWindow( CREATESTRUCT& cs )
{
	//	全面子ウィンドウなので、クリップをかけて無駄を省く(ここ重要！)
	auto result = CView::PreCreateWindow( cs );
	cs.style |= WS_CLIPCHILDREN;
	return result;
}
void CWpfView::OnDraw( CDC* )
{
	//	全面貼り付けなので、実装隠ぺいだけでいい
}
int CWpfView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if( CView::OnCreate( lpCreateStruct ) == -1 )
		return -1;

	//	クライアント領域全体をWPFウィンドウで占める形で用意する。
	//	非クライアント部分は CView がまかなうのでボーダーの内側のみ考慮すればよい
	//	HwndSource そのものを gcroot で保持することで確実にキープする(HWNDを持ってるだけじゃ意味がない)
	m_source = gcnew System::Windows::Interop::HwndSource( 
		0,										//	子ウィンドウのClassStyle
		WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,	//	子ウィンドウスタイル
		0,										//	子ウィンドウの拡張スタイル
		0, 0, 0, 0,								//	WPFコントロール配置する領域(x,y, cx, cy) cx,cy が0の場合このウィンドウの領域に追従する
		"CWpfView::HwndSource",					//	子ウィンドウなのでウィンドウタイトルは見えない。
		System::IntPtr( m_hWnd ) );				//	親ウィンドウのウィンドウハンドル

	//	配置などはHWNDを使って制御する
	auto hwnd = GetHwndSourceWindow();
	if( hwnd == nullptr )
	{
		return -1;	//	そもそもこの時点でウィンドウは作られているのであとは考慮しない
	}
	::SetWindowPos( hwnd, nullptr, 0, 0, lpCreateStruct->cx, lpCreateStruct->cy, SWP_NOZORDER|SWP_NOMOVE|SWP_FRAMECHANGED );
	return 0;
}
void CWpfView::OnSize( UINT nType, int cx, int cy )
{
	CView::OnSize( nType, cx, cy );
	//	クライアント領域全域にウィンドウをリサイズする
	auto hwnd = GetHwndSourceWindow();
	::SetWindowPos( hwnd, nullptr, 0, 0, cx, cy, SWP_NOZORDER|SWP_NOMOVE|SWP_FRAMECHANGED );
}
void CWpfView::OnSetFocus( CWnd* pOldWnd )
{
	CView::OnSetFocus( pOldWnd );
	//	子ウィンドウにフォーカスを受け渡す
	auto hwnd = GetHwndSourceWindow();
	::SetFocus( hwnd );
}
