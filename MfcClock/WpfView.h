//	CView �̈�S�ʂ� WPF �� UIElement ��z�u���� CWpfView

#pragma once

#ifndef __cplusplus_cli
	#error WPF support requires /clr (does not support oldSyntax)
#endif

//	�Q�Ɛݒ肵�Ă��Ȃ��ꍇ�ł��R���p�C������荞�݂���悤�ɂ��Ă���
#using <mscorlib.dll>
#using <PresentationCore.dll>
#using <WindowsBase.dll>
#using <PresentationFramework.dll>
#using <System.dll>
#using <System.Xaml.dll>


class CWpfView : public CView
{
protected:
	CWpfView();
	DECLARE_DYNAMIC(CWpfView)

// ����
public:
	CWpfView( const CWpfView& ) = delete;
	CWpfView& operator=( const CWpfView& ) = delete;

	virtual ~CWpfView();

public:
	template<class TYPE>
	TYPE^ SetRootVisual( _In_ TYPE^ rootVisual )
	{
		m_source->RootVisual = rootVisual;
		//	HwndSource �͎q�E�B���h�E�N���b�v�X�^�C���������ł��Ă��܂��̂Ŕw�i�h��Ԃ����K�{�B
		//	�{���́AHwndSource ���g�ɂ��������ݒ肪����΂����񂾂낤���ǁA�Ȃ��̂Ŏ��ۂɃZ�b�g�����^��Background �v���p�e�B�������Ă��邩�Ŕ���
		__if_exists(TYPE::Background)
		{
			if( rootVisual != nullptr )
			{
				if( rootVisual->Background == nullptr )
				{
					rootVisual->Background = System::Windows::SystemColors::WindowBrush;	//	Windows�̕W���w�i�������I�ɔz�u
				}
			}
		}
		return rootVisual;
	}
	template<class TYPE>
	TYPE^ GetRootVisual() const
	{
		auto src = GetHwndSource();
		_ASSERTE( src != nullptr );
		return dynamic_cast<TYPE^>((src != nullptr) ? src->RootVisual : nullptr);
	}
	//	��{�I�ɐG��K�v�͂Ȃ��͂������ǂ����ĉB�����Ȃ��`�ɂ��Ă���
	System::Windows::Interop::HwndSource^ GetHwndSource() const
	{
		return m_source;
	}
	HWND GetHwndSourceWindow() const
	{
		auto src = GetHwndSource();
		_ASSERTE( src != nullptr );	//	OnCreate �ō\�z���Ă���̂ő��݂��Ă��Ȃ��Ƃ������Ƃ͂Ȃ��͂�
		return static_cast<HWND>(src->Handle.ToPointer());
	}
private:
	gcroot<System::Windows::Interop::HwndSource^>	m_source;
// �I�[�o�[���C�h
public:
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

	virtual void OnDraw( CDC* );
// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnSetFocus( CWnd* pOldWnd );
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
};
