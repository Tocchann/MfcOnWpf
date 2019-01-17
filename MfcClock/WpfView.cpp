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
	//	HwndSource ���\�z���Ă��Ȃ��܂��́A�E�B���h�E�n���h��������
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
	//cs.style &= ~WS_BORDER;			//	�{�[�_�[�����Ă���� WS_EX_CLIENTEDGE �ɕύX�����(�݊��@�\)
	auto result = CView::PreCreateWindow( cs );
	cs.style |= WS_CLIPCHILDREN;
	//cs.dwExStyle &= WS_EX_CLIENTEDGE;	//	�A�v���P�[�V�����̃X�^�C���Ȃ̂ŕW���Ƃ����T�O�͂Ƃ�Ȃ�
	return result;
}
System::Windows::Interop::HwndSource^ CWpfView::CreateHwndSource()
{
	_ASSERTE( m_hWnd != nullptr );	//	�E�B���h�E�����O�ɌĂяo���Ă������ł��Ȃ��̂ŃA�T�[�V��������
	if( m_hWnd == nullptr ) return nullptr;
	//	������Ă΂�Ă����Ȃ��悤�ɂ��Ă���(���ە�����Ăяo�����\�������邽��)
	System::Windows::Interop::HwndSource^ src = m_source;
	if( src != nullptr ) return src;

	m_source = gcnew System::Windows::Interop::HwndSource(
		0, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,	0,	//	ClassStyle, WindowStyle, ExStyle
		0, 0, 0, 0,									//	WPF�R���e���c�̈�(�T�C�Y��0�ɂ����HWND�̃��T�C�Y�Ŏ����Ǐ]����)
		L"CWpfView::Client",						//	SPY++ �����E�B���h�E�^�C�g����(�R���p�C���������I�ɗǂ��Ȃɂ��Ă����)
		System::IntPtr( m_hWnd ) );					//	�e�E�B���h�E

	//	HwndSource ���쐬�����ƘA���R���g���[���p��HWND���쐬�����(WPF�R���g���[���{�̂̓E�B���h�E���X�R���g���[��)
	auto hwnd = GetHwndSourceWindow();
	_ASSERT( hwnd != nullptr );
	if( hwnd == nullptr ) return nullptr;
	//	�N���C�A���g�̈�S�̂ɍĔz�u����
	CRect rc;
	GetClientRect( &rc );
	::SetWindowPos( hwnd, nullptr, 0, 0, rc.right, rc.bottom, SWP_NOZORDER );
	return m_source;
}
void CWpfView::OnDraw( CDC* )
{
	//	OnDraw�ł̕`��͕K�v�Ȃ�
}
BOOL CWpfView::OnEraseBkgnd( CDC* pDC )
{
	//	WPF�R���g���[���𒣂�t���Ă��Ȃ��ꍇ�́A���[�N�X�y�[�X�J���[�œh��Ԃ�
	if( GetHwndSource() == nullptr )
	{
		CBrush* pbr = CBrush::FromHandle( GetSysColorBrush( COLOR_APPWORKSPACE ) );
		if( pbr != nullptr )
		{
			CRect rc;
			pDC->GetClipBox( &rc );
			pDC->FillRect( &rc, pbr );
			return TRUE;	//	�`�悵���̂Ńf�t�H���g�����͕s�v
		}
	}
	return CView::OnEraseBkgnd( pDC );
}
void CWpfView::OnDestroy()
{
	// ���I�ɍ폜���ł���悤�ɂ����̂ŁAWindow�j���̃^�C�~���O�ō��킹�ăI�u�W�F�N�g�̎Q�Ƃ���������
	DestroyHwndSource();
	CView::OnDestroy();
}
void CWpfView::OnSize( UINT nType, int cx, int cy )
{
	ASSERT_VALID( this );

	CView::OnSize( nType, cx, cy );

	//	�N���C�A���g�̈�S��ɃE�B���h�E�����T�C�Y����(�ǂ��ɂ��邩�킩��Ȃ��̂ŏ�ɑS�ʍĔz�u)
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

	//	�q�E�B���h�E�Ƀt�H�[�J�X���󂯓n��(�t�H�[�J�X���󂯎��==�E�B���h�E���\������Ă���)
	auto hwnd = GetHwndSourceWindow();
	if( hwnd != nullptr )
	{
		::SetFocus( hwnd );
	}
}
