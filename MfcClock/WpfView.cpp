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

	//	Viewエリア全域をそのままWPFエリアとして割り当てる
	m_source = gcnew System::Windows::Interop::HwndSource( 0, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN, 0,
															0, 0, 0, 0,
															"CWpfView.Client", System::IntPtr( m_hWnd ) );

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
	auto hwnd = GetHwndSourceWindow();
	::SetWindowPos( hwnd, nullptr, 0, 0, cx, cy, SWP_NOZORDER|SWP_NOMOVE|SWP_FRAMECHANGED );
}
void CWpfView::OnSetFocus( CWnd* pOldWnd )
{
	CView::OnSetFocus( pOldWnd );
	auto hwnd = GetHwndSourceWindow();
	::SetFocus( hwnd );
}
