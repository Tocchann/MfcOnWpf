#include "stdafx.h"

#include "WpfView.h"

IMPLEMENT_DYNAMIC(CWpfView, CView)

BEGIN_MESSAGE_MAP(CWpfView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

CWpfView::CWpfView()
	:m_hwndSource( nullptr )
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
	//	HwndSource ���\�z���Ă��Ȃ��܂��́A�E�B���h�E�n���h���������B
	System::Windows::Interop::HwndSource^ src = m_source;
	ASSERT( src == nullptr || src->Handle.ToPointer() == m_hwndSource );
}
void CWpfView::Dump( CDumpContext& dc ) const
{
	CView::Dump( dc );
	System::Windows::Interop::HwndSource^ src = m_source;
	if( src != nullptr )
	{
		dc << _T( "RootVisual=" );
		if( src->RootVisual != nullptr )
		{
			dc << msclr::interop::marshal_as<CString>( src->RootVisual->GetType()->FullName ) << _T( "\n" );
		}
		else
		{
			dc << _T( "nullptr\n" );
		}
	}
}
#endif
BOOL CWpfView::PreCreateWindow( CREATESTRUCT& cs )
{
	//	�S�ʎq�E�B���h�E�Ȃ̂ŁA�N���b�v�������Ė��ʂ��Ȃ�(�����d�v�I)
	auto result = CView::PreCreateWindow( cs );
	cs.style |= WS_CLIPCHILDREN;
	return result;
}
void CWpfView::OnDraw( CDC* )
{
	//	�S�ʓ\��t���Ȃ̂ŁA�����B�؂������ł���
}
int CWpfView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if( CView::OnCreate( lpCreateStruct ) == -1 )
		return -1;

	//	�N���C�A���g�̈�S�̂�WPF�E�B���h�E�Ő�߂�`�ŗp�ӂ���B
	//	��N���C�A���g������ CView ���܂��Ȃ��̂Ń{�[�_�[�̓����̂ݍl������΂悢
	//	HwndSource ���̂��̂� gcroot �ŕێ����邱�ƂŊm���ɃL�[�v����(HWND�������Ă邾������Ӗ����Ȃ�)
	m_source = gcnew System::Windows::Interop::HwndSource( 
		0,										//	�q�E�B���h�E��ClassStyle
		WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,	//	�q�E�B���h�E�X�^�C��
		0,										//	�q�E�B���h�E�̊g���X�^�C��
		0, 0, 0, 0,								//	WPF�R���g���[���z�u����̈�(x,y, cx, cy) cx,cy ��0�̏ꍇ���̃E�B���h�E�̗̈�ɒǏ]����
		"CWpfView::HwndSource",					//	�q�E�B���h�E�Ȃ̂ŃE�B���h�E�^�C�g���͌����Ȃ��B
		System::IntPtr( m_hWnd ) );				//	�e�E�B���h�E�̃E�B���h�E�n���h��

	//	�z�u�Ȃǂ�HWND���g���Đ��䂷��
	m_hwndSource = static_cast<HWND>(m_source->Handle.ToPointer());
	if( m_hwndSource == nullptr )
	{
		return -1;	//	�����������̎��_�ŃE�B���h�E�͍���Ă���̂ł��Ƃ͍l�����Ȃ�
	}
	::SetWindowPos( m_hwndSource, nullptr, 0, 0, lpCreateStruct->cx, lpCreateStruct->cy, SWP_NOZORDER|SWP_NOMOVE|SWP_FRAMECHANGED );
	return 0;
}
void CWpfView::OnDestroy()
{
	m_hwndSource = nullptr;
	System::Windows::Interop::HwndSource^ nulobj = nullptr;
	m_source = nulobj;
	CView::OnDestroy();
}
void CWpfView::OnSize( UINT nType, int cx, int cy )
{
	CView::OnSize( nType, cx, cy );
	//	�N���C�A���g�̈�S��ɃE�B���h�E�����T�C�Y����
	auto hwnd = GetHwndSourceWindow();
	if( hwnd != nullptr )
	{
		::SetWindowPos( hwnd, nullptr, 0, 0, cx, cy, SWP_NOZORDER|SWP_NOMOVE|SWP_FRAMECHANGED );
	}
}
void CWpfView::OnSetFocus( CWnd* pOldWnd )
{
	CView::OnSetFocus( pOldWnd );
	//	�q�E�B���h�E�Ƀt�H�[�J�X���󂯓n��
	auto hwnd = GetHwndSourceWindow();
	::SetFocus( hwnd );
}
