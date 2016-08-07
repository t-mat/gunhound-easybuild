//-------------------------------------------------------------------------------------------------
//
// gxLib App Main
// OpenGL
// written by tomi.
//-------------------------------------------------------------------------------------------------

#pragma once

#include <gxLib.h>

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーから使用されていない部分を除外します。
#include <windows.h>

// C ランタイム ヘッダー ファイル
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <fcntl.h>
#include <time.h> 


#include <gxLib/gx.h>
#include <gxLib/gxOrderManager.h>
#include <gxLib/gxTexManager.h>
#include <gxLib/gxRender.h>
#include <gxLib/gxPadManager.h>

#include "../win32cmn/win32cmn.h"
//#include "../win32DirectX9/dxPad.h"
#include "../win32DirectX9/dxSound.h"
#include "xiPad.h"
#include "glDraw.h"


//#define MAX_LOADSTRING 100
// グローバル変数:
//HINSTANCE hInst;                                // 現在のインターフェイス
//WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
//WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
//ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);
//LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//#ifdef GX_BUILD_OPTIONx86
//	#ifdef GX_DEBUG
//		#pragma comment(lib,"freeglut/d_freeGlutx86.lib")
//	#else
//		#pragma comment(lib,"freeglut/r_freeGlutx86.lib")
//	#endif
//#elif defined GX_BUILD_OPTIONx64
//	#ifdef GX_DEBUG
//		#pragma comment(lib,"freeglut/d_freeGlutx64.lib")
//	#else
//		#pragma comment(lib,"freeglut/r_freeGlutx64.lib")
//	#endif
//#endif

int APIENTRY wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int nCmdShow)
{
	TCHAR* pStr = GetCommandLine();
	SetCurrentDirectory( pStr );

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

//    // グローバル文字列を初期化しています。
//    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//    LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
//    MyRegisterClass(hInstance);

//    // アプリケーションの初期化を実行します:
//    if (!InitInstance (hInstance, nCmdShow))
//    {
//        return FALSE;
//    }
//
//    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	g_pWindows = new CWindows();

	g_pWindows->m_hInstance  = hInstance;

	// ---------------------------------------------------------------
	//アクセラレータキーの設定
	// ---------------------------------------------------------------

	ACCEL wAccel[enAccelMax];

	wAccel[0].cmd   = enID_ChangeFullScreenMode;
	wAccel[0].key   = VK_RETURN;
	wAccel[0].fVirt = FVIRTKEY|FALT;

	wAccel[1].cmd   = enID_AppExit;
	wAccel[1].key   = VK_ESCAPE;
	wAccel[1].fVirt = 0;

	wAccel[2].cmd   = enID_GamePause;
	wAccel[2].key   = VK_F1;
	wAccel[2].fVirt = FVIRTKEY;

	wAccel[3].cmd   = enID_GameStep;
	wAccel[3].key   = VK_F3;
	wAccel[3].fVirt = FVIRTKEY;

	wAccel[4].cmd   = enID_PadConfig;
	wAccel[4].key   = VK_F4;
	wAccel[4].fVirt = FVIRTKEY;

	wAccel[5].cmd   = enID_DebugMode;
	wAccel[5].key   = VK_F8;
	wAccel[5].fVirt = FVIRTKEY;

	g_pWindows->m_hAccel = CreateAcceleratorTable(wAccel, enAccelMax );

	// ---------------------------------------------------------------
	//タッチ情報の初期化
	// ---------------------------------------------------------------
	m_pTouchDevice = new StTouchDevice[ CPadManager::enTouchMax ];

	for(Sint32 ii=0; ii<CPadManager::enTouchMax; ii++)
	{
		StTouchDevice *pDev = &m_pTouchDevice[ ii ];
		pDev->Reset();
	}

	//------------------------------------------------------------
	//ウインドウの初期化
	//------------------------------------------------------------

	makeWindow();

	gxLib::LoadConfig();	//移植時注意！！！！


	//サウンドの初期化

	CDxSound::GetInstance()->Init();

	CXiPad::GetInstance()->Init();

	CGLDraw::GetInstance()->Init();

	CGameGirl::GetInstance()->Init();

	while( CGameGirl::GetInstance()->IsExist() )
	{
		CGameGirl::GetInstance()->Main();
	}

	CGameGirl::GetInstance()->End();

	CGLDraw::DeleteInstance();

	CDxSound::DeleteInstance();

	CXiPad::DeleteInstance();

	delete[] m_pTouchDevice;

	delete g_pWindows;

	gxLib::SaveConfig();	//移植時注意！！！！

	g_pWindows = NULL;


	return 0;// (int)msg.wParam;
}

void InitAction()
{
	//ゲームに入る直前処理
	Float32 fAnalog[4];

	//ジョイパッド情報を更新
	CXiPad::GetInstance()->Action();
	CPadManager::GetInstance()->SetPadInfo( 0 , CXiPad::GetInstance()->GetPressButton( 0 ) );
	CPadManager::GetInstance()->SetPadInfo( 1 , CXiPad::GetInstance()->GetPressButton( 1 ) );
/*
	3P,4Pは？
*/

	//タッチ情報の更新

	for(Sint32 ii=0; ii<CPadManager::enTouchMax; ii++)
	{
		StTouchDevice *pDev = &m_pTouchDevice[ ii ];
		if( pDev->bUse )
		{
			CPadManager::GetInstance()->SetTouchInfo( pDev->id , pDev->psh , pDev->x , pDev->y );

			if( !pDev->psh )
			{
				//押されていなければクリア
				pDev->Reset();
			}
		}
	}

	Float32 *pAnalog;

	pAnalog = CXiPad::GetInstance()->GetAnalogInfo( 0 );

	fAnalog[0] = pAnalog[0];
	fAnalog[1] = pAnalog[1];

	pAnalog = CXiPad::GetInstance()->GetAnalogInfo( 1 );
	fAnalog[2] = pAnalog[0];
	fAnalog[3] = pAnalog[1];

	CPadManager::GetInstance()->SetAnalogInfo( fAnalog );

	//マウスカーソル位置を更新
	POINT point={0,0};

	GetCursorPos( &point );
	if( ScreenToClient( g_pWindows->m_hWindow , &point ) )
	{
		Sint32 w = (Sint32)g_pWindows->m_uScreenWidth;
		Sint32 h = (Sint32)g_pWindows->m_uScreenHeight;

		if( w == 0 || h == 0 ) return;

		CPadManager::GetInstance()->SetMousePosition( SWINDOW_W*point.x/w , SWINDOW_H*point.y/h );
	}

	CPadManager::GetInstance()->SetDeviceNum( CXiPad::GetInstance()->GetUseableDeviceNum() );


	//キーボードの状態を伝える

	g_pWindows->m_KeyBoard[ VK_LSHIFT   ] = (GetKeyState(VK_LSHIFT  ) < 0)? 0x01 : 0x02;
	g_pWindows->m_KeyBoard[ VK_LCONTROL ] = (GetKeyState(VK_LCONTROL) < 0)? 0x01 : 0x02;
	g_pWindows->m_KeyBoard[ VK_LMENU ]	  = (GetKeyState(VK_LMENU   ) < 0)? 0x01 : 0x02;

	g_pWindows->m_KeyBoard[ VK_RSHIFT   ] = (GetKeyState(VK_RSHIFT  ) < 0)? 0x01 : 0x02;
	g_pWindows->m_KeyBoard[ VK_RCONTROL ] = (GetKeyState(VK_RCONTROL) < 0)? 0x01 : 0x02;
	g_pWindows->m_KeyBoard[ VK_RMENU ]	  = (GetKeyState(VK_RMENU   ) < 0)? 0x01 : 0x02;

	for( Sint32 ii=0; ii<256; ii++ )
	{
		if(g_pWindows->m_KeyBoard[ii] == 0x01 )
		{
			CPadManager::GetInstance()->SetKeyDown( ii );
		}
		else if(g_pWindows->m_KeyBoard[ii] == 0x02 )
		{
			CPadManager::GetInstance()->SetKeyUp( ii );
		}

		g_pWindows->m_KeyBoard[ii] = 0x00;
	}


	//メモリ使用量を更新
	updateMemoryStatus();

}

void Render()
{
	CGLDraw::GetInstance()->Action();
	CGLDraw::GetInstance()->Draw();
}

gxBool vSync( gxBool bNoWaitvSync )
{
	//------------------------------------------
	//ｖＳｙｎｃ待ち(1/60秒経ったら勝手に帰る)
	//------------------------------------------

	return vSyncWithTimer( bNoWaitvSync );

}

void Flip()
{
	CGLDraw::GetInstance()->FlipScreen();
}


void Resume()
{
	CGLDraw::GetInstance()->Resume();
}


void Play()
{
	CDxSound::GetInstance()->Action();
}

void Upload( Sint32 sBank )
{
	CGLDraw::GetInstance()->ReadTexture( sBank );

	updateMemoryStatus();
}



