#include "stdafx.h"

#include "WpfView.h"

IMPLEMENT_DYNAMIC(CWpfView, CView)

BEGIN_MESSAGE_MAP(CWpfView, CView)
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()
CWpfView::CWpfView()
	: m_fitWPF( true )
{
}
CWpfView::~CWpfView()
{
}
BOOL CWpfView::PreCreateWindow( CREATESTRUCT& cs )
{
	cs.style |= WS_CLIPCHILDREN;
	return CView::PreCreateWindow( cs );
}
void CWpfView::OnDraw( CDC* )
{
	//	Fit Ç∑ÇÈèÍçáÇÕOnDrawÇé¿ëïÇ∑ÇÈïKóvÇÕÇ»Ç¢
	ASSERT( m_fitWPF );
}
void CWpfView::OnSize( UINT nType, int cx, int cy )
{
	CView::OnSize( nType, cx, cy );
	if( IsFitWPF() )
	{
		auto hwnd = static_cast<HWND>(GetHwndSource()->Handle.ToPointer());
		::SetWindowPos( hwnd, nullptr, 0, 0, cx, cy, SWP_NOZORDER|SWP_NOMOVE|SWP_FRAMECHANGED );
	}
}
void CWpfView::OnSetFocus( CWnd* pOldWnd )
{
	CView::OnSetFocus( pOldWnd );
	if( IsFitWPF() )
	{
		auto hwnd = static_cast<HWND>(GetHwndSource()->Handle.ToPointer());
		::SetFocus( hwnd );
	}
}
