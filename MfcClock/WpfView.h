//	CView 領域全面に WPF の Visual を配置する CWpfView

#pragma once

//	WPF の貼り付けが前提なので、/clr は必須扱いとする(少なくともこのヘッダーを参照するところは...)
#ifndef __cplusplus_cli
	#error WPF support requires /clr (does not support oldSyntax)
#endif
#include <msclr\gcroot.h>

//	HwndSource を保持するために最低限必要なもののみ定義する
#using <mscorlib.dll>
#using <PresentationCore.dll>
#using <PresentationFramework.dll>
#using <WindowsBase.dll>

class CWpfView : public CView
{
	DECLARE_DYNAMIC( CWpfView )
protected:
	CWpfView();

	//	CView はコピー不可なので明示的に止める
	CWpfView( const CWpfView& ) = delete;
	CWpfView& operator=( const CWpfView& ) = delete;

// 実装
public:
	virtual ~CWpfView() = 0;	//	直接利用は認めない！
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext& dc ) const;
#endif
public:
	//	ビューの物理的内容にアクセスする部分については、外部参照も可能にする
	template<class TYPE>
	TYPE^ GetRootVisual() const
	{
		auto src = GetHwndSource();
		return dynamic_cast<TYPE^>((src != nullptr) ? src->RootVisual : nullptr);
	}
protected:
	//	初期化処理に当たる部分なので、派生クラス側で面倒を見るものとする。
	template<class TYPE>
	TYPE^ SetRootVisual( _In_ TYPE^ rootVisual )
	{
		auto src = CreateHwndSource();
		_ASSERTE( src != nullptr );
		src->RootVisual = rootVisual;
		//	HwndSource は子ウィンドウクリップスタイルが自動でついてしまうので背景塗りつぶしが必須。
		//	本当は、HwndSource 自身にそういう設定があればいいんだろうけど、ないので実際にセットされる型がBackground プロパティを持っているかで判定
		__if_exists(TYPE::Background)
		{
			if( rootVisual != nullptr )
			{
				if( rootVisual->Background == nullptr )
				{
					rootVisual->Background = System::Windows::SystemColors::WindowBrush;	//	Windowsの標準背景を強制的に配置
				}
			}
		}
		return rootVisual;
	}
	//	基本的に触る必要はないはずなので、protected で定義する
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
	//	WPFオブジェクトをホストするための HwndSource の構築(自動で構築するので通常参照しない)
	System::Windows::Interop::HwndSource^ CreateHwndSource();
private:
	msclr::gcroot<System::Windows::Interop::HwndSource^>	m_source;
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
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
};
