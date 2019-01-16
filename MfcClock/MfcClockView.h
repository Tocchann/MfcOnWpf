
// MfcClockView.h : CMfcClockView クラスのインターフェイス
//

#pragma once
#include "WpfView.h"

#include <msclr/event.h>	//	イベントハンドラマクロ

class CMfcClockView : public CWpfView
{
protected: // シリアル化からのみ作成します。
	CMfcClockView() noexcept;
	DECLARE_DYNCREATE(CMfcClockView)

// 属性
public:
	CMfcClockDoc* GetDocument() const;
// 操作
public:

// オーバーライド
public:
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 実装
public:
	virtual ~CMfcClockView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
public:
	BEGIN_DELEGATE_MAP( CMfcClockView )
		EVENT_DELEGATE_ENTRY( OnEnter, System::Object^, System::EventArgs^ )
	END_DELEGATE_MAP()
	void OnEnter( System::Object^ sender, System::EventArgs^ e );

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	virtual void OnInitialUpdate();
	afx_msg void OnPopup();
	afx_msg void OnUpdateNone( CCmdUI *pCmdUI );
	afx_msg void OnUpdateClock( CCmdUI *pCmdUI );
	afx_msg void OnUpdatePhone( CCmdUI *pCmdUI );
	afx_msg void OnUpdateListbox( CCmdUI *pCmdUI );
	afx_msg void OnUpdateAddr( CCmdUI *pCmdUI );
	afx_msg void OnNone();
	afx_msg void OnClock();
	afx_msg void OnPhone();
	afx_msg void OnListbox();
	afx_msg void OnAddr();
};

#ifndef _DEBUG  // MfcClockView.cpp のデバッグ バージョン
inline CMfcClockDoc* CMfcClockView::GetDocument() const
   { return reinterpret_cast<CMfcClockDoc*>(m_pDocument); }
#endif

