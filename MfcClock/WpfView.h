//	WPF コントロールをホストするための CView ベースクラス

#pragma once

//	.NET のイベントを受け取るためのマクロ
#include <msclr/event.h>

class CWpfView : public CView
{
protected:
	CWpfView();
	DECLARE_DYNAMIC(CWpfView)

// 実装
public:
	CWpfView( const CWpfView& ) = delete;
	CWpfView& operator=( const CWpfView& ) = delete;

	virtual ~CWpfView();

private:
	gcroot<System::Windows::Interop::HwndSource^>	m_source;
	bool		m_fitWPF;	//	WPF制御ウィンドウをViewの大きさに連動させる(デフォルト連動)
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
// オーバーライド
public:
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

	virtual void OnDraw( CDC* );
// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnSetFocus( CWnd* pOldWnd );
};

