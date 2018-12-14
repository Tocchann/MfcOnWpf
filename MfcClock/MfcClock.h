
// MfcClock.h : MfcClock アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CMfcClockApp:
// このクラスの実装については、MfcClock.cpp を参照してください
//

class CMfcClockApp : public CWinApp
{
public:
	CMfcClockApp() noexcept;


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMfcClockApp theApp;
