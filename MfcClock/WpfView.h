//	CView �̈�S�ʂ� WPF �� Visual ��z�u���� CWpfView

#pragma once

//	WPF �̓\��t�����O��Ȃ̂ŁA/clr �͕K�{�����Ƃ���(���Ȃ��Ƃ����̃w�b�_�[���Q�Ƃ���Ƃ����...)
#ifndef __cplusplus_cli
	#error WPF support requires /clr (does not support oldSyntax)
#endif
#include <msclr\gcroot.h>

//	HwndSource ��ێ����邽�߂ɍŒ���K�v�Ȃ��̂̂ݒ�`����
#using <mscorlib.dll>
#using <PresentationCore.dll>
#using <PresentationFramework.dll>
#using <WindowsBase.dll>

class CWpfView : public CView
{
	DECLARE_DYNAMIC( CWpfView )
protected:
	CWpfView();

	//	CView �̓R�s�[�s�Ȃ̂Ŗ����I�Ɏ~�߂�
	CWpfView( const CWpfView& ) = delete;
	CWpfView& operator=( const CWpfView& ) = delete;

// ����
public:
	virtual ~CWpfView() = 0;	//	���ڗ��p�͔F�߂Ȃ��I
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext& dc ) const;
#endif
public:
	//	�r���[�̕����I���e�ɃA�N�Z�X���镔���ɂ��ẮA�O���Q�Ƃ��\�ɂ���
	template<class TYPE>
	TYPE^ GetRootVisual() const
	{
		auto src = GetHwndSource();
		return dynamic_cast<TYPE^>((src != nullptr) ? src->RootVisual : nullptr);
	}
protected:
	//	�����������ɓ����镔���Ȃ̂ŁA�h���N���X���Ŗʓ|��������̂Ƃ���B
	template<class TYPE>
	TYPE^ SetRootVisual( _In_ TYPE^ rootVisual )
	{
		auto src = CreateHwndSource();
		_ASSERTE( src != nullptr );
		src->RootVisual = rootVisual;
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
	//	��{�I�ɐG��K�v�͂Ȃ��͂��Ȃ̂ŁAprotected �Œ�`����
	System::Windows::Interop::HwndSource^ GetHwndSource() const
	{
		System::Windows::Interop::HwndSource^ val = m_source;
		return val;
	}
	HWND GetHwndSourceWindow() const
	{
		HWND result = nullptr;
		auto src = GetHwndSource();
		if( src != nullptr )
		{
			result = static_cast<HWND>(m_source->Handle.ToPointer());
		}
		return result;
	}
	//	WPF�I�u�W�F�N�g���z�X�g���邽�߂� HwndSource �̍\�z(�����ō\�z����̂Œʏ�Q�Ƃ��Ȃ�)
	System::Windows::Interop::HwndSource^ CreateHwndSource();
private:
	msclr::gcroot<System::Windows::Interop::HwndSource^>	m_source;
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
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
};
