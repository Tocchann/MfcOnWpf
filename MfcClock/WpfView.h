//	WPF �R���g���[�����z�X�g���邽�߂� CView �x�[�X�N���X

#pragma once

//	.NET �̃C�x���g���󂯎�邽�߂̃}�N��
#include <msclr/event.h>

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

private:
	gcroot<System::Windows::Interop::HwndSource^>	m_source;
	bool		m_fitWPF;	//	WPF����E�B���h�E��View�̑傫���ɘA��������(�f�t�H���g�A��)
protected:
	void	SetHwndSource( _In_ System::Windows::Interop::HwndSource^ source )
	{
		m_source = source;
	}
	System::Windows::Interop::HwndSource^ GetHwndSource() const
	{
		return m_source;
	}
	void	SetFitWPF( bool value )
	{
		m_fitWPF = value;
	}
	bool	GetFitWPF() const
	{
		return m_fitWPF;
	}
	bool	IsFitWPF() const
	{
		return GetFitWPF() && GetHwndSource() != nullptr ;
	}
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
};

