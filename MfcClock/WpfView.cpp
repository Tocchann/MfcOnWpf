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
	//	HwndSource ���\�z���Ă��Ȃ��܂��́A�E�B���h�E�n���h���������B
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
		//	RootVisual �ɒ���t���Ă���N���X����\������
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
	//cs.style &= ~WS_BORDER;
	//	�S�ʎq�E�B���h�E�Ȃ̂ŁA�N���b�v�������Ė��ʂ��Ȃ�(�����d�v�I)
	auto result = CView::PreCreateWindow( cs );
	cs.style |= WS_CLIPCHILDREN;
	return result;
}
System::Windows::Interop::HwndSource^ CWpfView::CreateHwndSource()
{
	_ASSERTE( m_hWnd != nullptr );	//	�E�B���h�E�����O�ɌĂяo���Ă������ł��Ȃ��̂ŃA�T�[�V��������
	if( m_hWnd == nullptr )
	{
		return nullptr;
	}
	System::Windows::Interop::HwndSource^ val = m_source;
	if( val == nullptr )
	{
		//	�N���C�A���g�̈�S�̂�WPF�E�B���h�E�Ő�߂�`�ŗp�ӂ���B
		//	��N���C�A���g������ CView ���܂��Ȃ��̂Ń{�[�_�[�̓����̂ݍl������΂悢
		//	HwndSource ���̂��̂� gcroot �ŕێ����邱�ƂŊm���ɃL�[�v����(HWND�������Ă邾������Ӗ����Ȃ�)
		m_source = gcnew System::Windows::Interop::HwndSource(
			0,										//	�q�E�B���h�E��ClassStyle
			WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,	//	�q�E�B���h�E�X�^�C��
			0,										//	�q�E�B���h�E�̊g���X�^�C��
			0, 0, 0, 0,								//	WPF�R���g���[���z�u����̈�(x,y, cx, cy) cx,cy ��0�̏ꍇ���̃E�B���h�E�̗̈�ɒǏ]����
			"CWpfView::HwndSource",					//	�q�E�B���h�E�Ȃ̂ŃE�B���h�E�^�C�g���͌����Ȃ��B�킩��₷�����邽�߂̌��������̃e�L�X�g
			System::IntPtr( m_hWnd ) );				//	�e�E�B���h�E�̃E�B���h�E�n���h��

		//	�z�u�Ȃǂ�HWND���g���Đ��䂷��
		auto hwnd = GetHwndSourceWindow();
		if( hwnd == nullptr )
		{
			return nullptr;
		}
		CRect rc;
		GetClientRect( &rc );
		//	�S�ʓ\��t��
		::SetWindowPos( hwnd, nullptr, 0, 0, rc.right, rc.bottom, SWP_NOZORDER|SWP_FRAMECHANGED );
	}
	return GetHwndSource();
}
void CWpfView::OnDraw( CDC* )
{
	//	�S�ʓ\��t���Ȃ̂ŁA�����B�؂������ł���
}
BOOL CWpfView::OnEraseBkgnd( CDC* pDC )
{
	//	WPF�R���g���[���𒣂�t���Ă��Ȃ��ꍇ�́A�O���[�A�E�g������Ԃŕ\������(�Ȃ�ƂȂ��������߁H)
	if( GetHwndSource() == nullptr )
	{
		CBrush* pbr = CBrush::FromHandle( GetSysColorBrush( COLOR_APPWORKSPACE ) );
		if( pbr != nullptr )
		{
			CRect rc;
			pDC->GetClipBox( &rc );
			pDC->FillRect( &rc, pbr );
			return TRUE;
		}
	}
	return CView::OnEraseBkgnd( pDC );
}
void CWpfView::OnDestroy()
{
	System::Windows::Interop::HwndSource^ nulobj = nullptr;
	m_source = nulobj;
	CView::OnDestroy();
}
void CWpfView::OnSize( UINT nType, int cx, int cy )
{
	CView::OnSize( nType, cx, cy );
	ASSERT_VALID( this );
	//	�N���C�A���g�̈�S��ɃE�B���h�E�����T�C�Y����(���_�͏����ݒ�ł��낦�Ă���̂ŁA�T�C�Y�����ύX)
	auto hwnd = GetHwndSourceWindow();
	if( hwnd != nullptr )
	{
		::SetWindowPos( hwnd, nullptr, 0, 0, cx, cy, SWP_NOZORDER|SWP_NOMOVE|SWP_FRAMECHANGED );
	}
}
void CWpfView::OnSetFocus( CWnd* pOldWnd )
{
	ASSERT_VALID( this );
	CView::OnSetFocus( pOldWnd );
	//	�q�E�B���h�E�Ƀt�H�[�J�X���󂯓n��(�t�H�[�J�X���󂯎��==�E�B���h�E���\������Ă���)
	auto hwnd = GetHwndSourceWindow();
	if( hwnd != nullptr )
	{
		::SetFocus( hwnd );
	}
}
