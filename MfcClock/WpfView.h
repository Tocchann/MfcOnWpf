//	CView 領域全面に WPF の UIElement を配置する CWpfView

#pragma once

#ifndef __cplusplus_cli
	#error WPF support requires /clr (does not support oldSyntax)
#endif

//	参照設定していない場合でもコンパイル時取り込みするようにしておく
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

// 実装
public:
	CWpfView( const CWpfView& ) = delete;
	CWpfView& operator=( const CWpfView& ) = delete;

	virtual ~CWpfView();

public:
	template<class TYPE>
	TYPE^ SetRootVisual( _In_ TYPE^ rootVisual )
	{
		m_source->RootVisual = rootVisual;
		//	HwndSource は子ウィンドウクリップスタイルが自動でついてしまうので背景塗りつぶしが必須。
		//	本当は、HwndSource 自身にそういう設定があればいいんだろうけど、ないので実際にセットされる型がBackground プロパティを持っているかで判定
		__if_exists(TYPE::Background)
		{
			if( rootVisual != nullptr )
			{
				if( rootVisual->Background == nullptr )
				{
					rootVisual->Background = System::Windows::SystemColors::WindowBrush;	//	Windowsの標準背景を強制的に配置(WPFのデフォルトと同じ処理)
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
	//	基本的に触る必要はないはずだけどあえて隠蔽しない形にしておく
	System::Windows::Interop::HwndSource^ GetHwndSource() const
	{
		return m_source;
	}
	HWND GetHwndSourceWindow() const
	{
		auto src = GetHwndSource();
		_ASSERTE( src != nullptr );	//	OnCreate で構築しているので存在していないということはないはず
		return static_cast<HWND>(src->Handle.ToPointer());
	}
private:
	gcroot<System::Windows::Interop::HwndSource^>	m_source;
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
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
};
