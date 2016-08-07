//-------------------------------------------------------------------
//
// 以下、DirectX / OpenGLで使用する共通コード
//
// win32cmn.h
//-------------------------------------------------------------------
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

#include "resource.h"

#include <gxLib/gx.h>
#include <gxLib/gxOrderManager.h>
#include <gxLib/gxTexManager.h>
#include <gxLib/gxRender.h>
#include <gxLib/gxPadManager.h>
#include <gxLib/gxDebug.h>
#include "win32cmn.h"

LRESULT	CALLBACK CGameGirlProc(HWND hw, UINT iMsg, WPARAM wParam, LPARAM lParam);
void ChangeScreenMode( gxBool bFullScreen );
void updateMemoryStatus();
void InputShortCutCheck( Uint32 uId );
void InputTouchCheck( UINT iMsg , WPARAM wParam ,LPARAM lParam);
void InputKeyCheck( UINT iMsg , WPARAM wParam ,LPARAM lParam );


CWindows* g_pWindows = NULL;
StTouchDevice *m_pTouchDevice;//[ CPadManager::enTouchMax ];


#define NAME_APRICATION APPLICATION_NAME
#define NAME_APRICLASS  "gxLib9"


void makeWindow( )
{

	WNDCLASSEX	wndClass;

	wndClass.cbSize        = sizeof(wndClass);
	wndClass.style         = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc   = CGameGirlProc;
	wndClass.cbClsExtra    = 0;
	wndClass.cbWndExtra    = 0;
	wndClass.hInstance     = g_pWindows->m_hInstance;
	wndClass.hIcon         = LoadIcon( g_pWindows->m_hInstance , MAKEINTRESOURCE(IDI_APP));
	wndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wndClass.lpszMenuName  = NULL;//MAKEINTRESOURCE(CGameGirl::GetInstance()->MENU_BAR);
	wndClass.lpszClassName = TEXT( NAME_APRICLASS );
	wndClass.hIconSm       = LoadIcon (NULL, _T( "MAIN" ));

	RegisterClassEx(&wndClass);

	//画面の中央にセット
	RECT		desktop;
	GetWindowRect(GetDesktopWindow(), (LPRECT)&desktop);

	/*-- フレームなどのクライアント領域以外のサイズを考慮 --*/
	Sint32 w,h;
	RECT rect = { 0, 0, SWINDOW_W, SWINDOW_H };
	AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW|WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, FALSE );

	w = rect.right - rect.left;
	h = rect.bottom - rect.top;

	Sint32 ax,ay;

	ax = (desktop.right  - desktop.left)/2 - w/2;
	ay = (desktop.bottom - desktop.top)/2  - h/2;

	if( ax < 0) ax = 0;
	if( ay < 0) ay = 0;

	g_pWindows->m_hWindow = CreateWindow(
				TEXT( NAME_APRICLASS ),
				TEXT( NAME_APRICATION ),
	            WS_OVERLAPPEDWINDOW|WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
				ax,
				ay,
				w,
				h,
				NULL,
				NULL,
				g_pWindows->m_hInstance,
			    NULL);

	g_pWindows->m_WinDC = GetDC( g_pWindows->m_hWindow );

	g_pWindows->m_AppStyle = GetWindowLong( g_pWindows->m_hWindow , GWL_STYLE);
	GetWindowRect( g_pWindows->m_hWindow , &g_pWindows->m_WinRect );

	/*-- 念のためウインドウサイズ補正 --*/
	RECT client;
	GetClientRect( g_pWindows->m_hWindow, &client );
	int diffx = (client.right - client.left) - SWINDOW_W;
	int diffy = (client.bottom - client.top) - SWINDOW_H;
	if ( diffx != 0 || diffy != 0 ) {
		rect.right -= diffx;
		rect.bottom -= diffy;

		MoveWindow( g_pWindows->m_hWindow, g_pWindows->m_WinRect.left, g_pWindows->m_WinRect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE );
	}

	// ハードがマルチタッチをサポートしているかどうか
	int  value= ~GetSystemMetrics( SM_DIGITIZER );

	if( !(value & 0xc0) )
	{
		RegisterTouchWindow( g_pWindows->m_hWindow, 0 );
	}

	QueryPerformanceFrequency((LARGE_INTEGER*)&g_pWindows->Vsyncrate);		//秒間のカウント

}



void updateMemoryStatus()
{
	//メモリの使用状況を更新する
	static Uint32 m_uMemoryUse    = 1;
	static Uint32 m_uMemoryMaxUse = 1;
	static Uint32 m_uMemoryTotal  = 0;

    MEMORYSTATUS memory;

    memory.dwLength = sizeof( MEMORYSTATUS );
    GlobalMemoryStatus( &memory );

	if( m_uMemoryTotal == 0 )
	{
		//ゲームで使用可能なメモリ量を計測
		m_uMemoryTotal = memory.dwAvailPhys;
		m_uMemoryTotal = memory.dwAvailVirtual;
	}

	//現在ゲームが使用しているメモリ量
	m_uMemoryUse   = m_uMemoryTotal - memory.dwAvailPhys;
	m_uMemoryUse   = m_uMemoryTotal - memory.dwAvailVirtual;

	//最大風速
	if( m_uMemoryUse > m_uMemoryMaxUse ) m_uMemoryMaxUse = m_uMemoryUse;

	CGameGirl::GetInstance()->ReportMemoryStatus( m_uMemoryUse , m_uMemoryTotal , m_uMemoryMaxUse );
}


void InputKeyCheck( UINT iMsg , WPARAM wParam ,LPARAM lParam )
{
	//キー入力されたら記録する

	switch(iMsg) {
	case WM_KEYDOWN:
		//キーボード押した
		//CPadManager::GetInstance()->SetKeyDown(wParam);
		g_pWindows->m_KeyBoard[ wParam ] = 0x01;
		return;

	case WM_KEYUP:
		//キーボード離した
		//CPadManager::GetInstance()->SetKeyUp(wParam);
		{
			g_pWindows->m_KeyBoard[wParam] = 0x02;
		}
		return;

	case WM_LBUTTONDOWN:
		//マウスボタン押した
		CPadManager::GetInstance()->SetMouseButtonDown( 0 );	//←
		break;
	case WM_LBUTTONUP:
		//マウスボタン離した
		CPadManager::GetInstance()->SetMouseButtonUp( 0 );
		break;
	case WM_RBUTTONDOWN:
		//マウスボタン押した
		CPadManager::GetInstance()->SetMouseButtonDown( 1 );	//→
		break;
	case WM_RBUTTONUP:
		//マウスボタン離した
		CPadManager::GetInstance()->SetMouseButtonUp( 1 );
		break;

	case WM_MOUSEWHEEL:
		CPadManager::GetInstance()->SetMouseWheel( (Sint16)HIWORD(wParam)/WHEEL_DELTA );
		break;

	case WM_TOUCH:
		//タッチ対応
		InputTouchCheck( iMsg , wParam , lParam );
		break;
	}


}


void InputTouchCheck( UINT iMsg , WPARAM wParam ,LPARAM lParam )
{
	//タッチ情報の更新

	TOUCHINPUT	touch[CPadManager::enTouchMax ];
	int	num= LOWORD( wParam );
	HTOUCHINPUT	hinput= reinterpret_cast<HTOUCHINPUT>( lParam );

	if( GetTouchInputInfo( hinput, num, &touch[0], sizeof(TOUCHINPUT) ) )
	{
		for( int ii= 0 ; ii< num ; ii++ )
		{
			TOUCHINPUT*	p= &touch[ii];
			gxBool bFind = gxFalse;


			//スクリーン座標に変換する
			POINT pt;
			pt.x = p->x/100;
			pt.y = p->y/100;
			ScreenToClient( g_pWindows->m_hWindow , &pt );

			//どれを更新するか？

			for( Sint32 jj=0; jj<CPadManager::enTouchMax; jj++)
			{
				if( p->dwID == m_pTouchDevice[jj].id )
				{
					//既にタッチされているものが更新された
					Sint32 n = jj;
					bFind = gxTrue;
//					m_pTouchDevice[jj].nx   = pt.x;
//					m_pTouchDevice[jj].ny   = pt.y;
					m_pTouchDevice[jj].x   = pt.x;
					m_pTouchDevice[jj].y   = pt.y;
					m_pTouchDevice[jj].psh  = gxTrue;
					m_pTouchDevice[jj].bUse = gxTrue;

					if( p->dwFlags&TOUCHEVENTF_UP )
					{
//						m_pTouchDevice[jj].ex   = pt.x;
//						m_pTouchDevice[jj].ey   = pt.y;
						m_pTouchDevice[jj].x   = pt.x;
						m_pTouchDevice[jj].y   = pt.y;
						m_pTouchDevice[jj].psh = gxFalse;
					}
					break;
				}
			}

			if( bFind )
			{
				continue;
			}

			//新規IDがきた

			for( Sint32 jj=0; jj<CPadManager::enTouchMax; jj++)
			{
				if( !m_pTouchDevice[jj].bUse )
				{
					//使っていないものを発見した
					m_pTouchDevice[jj].id   = p->dwID;
					m_pTouchDevice[jj].bUse = gxTrue;
//					m_pTouchDevice[jj].sx   = pt.x;
//					m_pTouchDevice[jj].sy   = pt.y;
//					m_pTouchDevice[jj].nx   = m_pTouchDevice[ii].sx;
//					m_pTouchDevice[jj].ny   = m_pTouchDevice[ii].sy;
					m_pTouchDevice[jj].x   = pt.x;
					m_pTouchDevice[jj].y   = pt.y;
					m_pTouchDevice[jj].psh  = gxTrue;
					break;
				}
			}
			
			//開いてるのがないから無視する

			// ...


		}
	}
}


Uint8* LoadFile( const gxChar* pFileName , Uint32* pLength )
{
	//------------------------------------------
	//ファイルの読み込み
	//------------------------------------------
	TCHAR* pStr = GetCommandLine();
	SetCurrentDirectory( pStr );

	Uint8* pBuffer=NULL;
	int fh;
	long sz_zero;
	long sz,readsz;
	unsigned long pos=0;
	int ret=1;
	struct stat filestat;

	fh = open((char*)pFileName,O_RDONLY|O_BINARY);

	updateMemoryStatus();

	if(fh<0)
	{
		//読み込みミス
		//close(fh);
		return NULL;
	}
	else
	{
		fstat(fh,&filestat);
		readsz = sz = sz_zero = filestat.st_size;

		*pLength = filestat.st_size;
		pBuffer = new Uint8[ readsz ];

		if( pBuffer == NULL ) return NULL;

		while(ret > 0)
		{
			if( readsz > 1024 ) readsz = 1024; 

			ret = read(fh,&pBuffer[pos],readsz);
			pos += ret;
			sz -= ret;
			readsz = sz;
		}
	}

	close(fh);

	return pBuffer;
}


gxBool SaveFile( const gxChar* pFileName , Uint8* pReadBuf , Uint32 uSize )
{
	//ファイルの書き込み
	TCHAR* pStr = GetCommandLine();
	SetCurrentDirectory( pStr );

	int fh;

	fh = open((char*)pFileName,O_WRONLY|O_BINARY|O_TRUNC|O_CREAT,S_IREAD|S_IWRITE);

	updateMemoryStatus();

	if(fh<0)
	{
		//書き込みミス
		return gxFalse;
	}
	else
	{
		write(fh,pReadBuf,uSize);
	}

	close(fh);

	return gxTrue;
}


gxBool SaveStorageFile( const gxChar* pFileName , Uint8* pReadBuf , Uint32 uSize )
{
	// とりあえずSaveFileと同じ扱い
	return SaveFile( pFileName, pReadBuf, uSize );

}
Uint8* LoadStorageFile( const gxChar* pFileName , Uint32* pLength )
{
	// とりあえずLoadFileと同じ扱い
	return LoadFile( pFileName, pLength );

}

void LogDisp( char* pString )
{
	//デバッグログの表示
	OutputDebugStringA( pString );
	OutputDebugStringA( "\n" );
}


Uint32 GetVsyncRate()
{
	return g_pWindows->Vsyncrate;
}

Uint32 GetDebugTime()
{
	//時刻を返す
	LONGLONG timer;

	QueryPerformanceCounter((LARGE_INTEGER*)&timer);			//現在の時間を取得

	return timer;
}


void ExecuteApp( char *appname )
{
	//アプリケーションを実行する

	if( g_pWindows )
	{
		wchar_t wStr[0xff];
		Uint32 len = strlen(appname);

		len = MultiByteToWideChar( CP_ACP , 0 ,(char*)appname , strlen((char*)appname) , wStr , 0xff );

		wStr[len] = 0x0000;
		ShellExecute( g_pWindows->m_hWindow, _T("open"),  wStr, NULL, NULL, SW_SHOWNORMAL);

		//ShellExecute( g_pWindows->m_hWindow, _T("open"),  appname, NULL, NULL, SW_SHOWNORMAL);
	}

}


gxBool IsFullScreen()
{
	//スクリーンモードの問い合わせ

	if( g_pWindows )
	{
		return g_pWindows->m_bFullScreen;
	}

	return gxFalse;
}


void ChangeScreenMode( gxBool bFullScreen )
{
	//フルスクリーン / ウインドウモード切り替え

	RECT rc;
	DEVMODE devmode;

	EnumDisplaySettings( NULL, 0, &devmode );

	if( bFullScreen )
	{
		//フルスクリーンモード
		if( g_pWindows->m_bFullScreen ) return;

		Sint32 w,h;
		w = 640;
		h = 480;
		g_pWindows->m_AppStyle = GetWindowLong( g_pWindows->m_hWindow , GWL_STYLE);
		GetWindowRect( g_pWindows->m_hWindow , &g_pWindows->m_WinRect );

		devmode.dmSize       = sizeof(DEVMODE);
		devmode.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT;
		devmode.dmPelsWidth  = w;//SWINDOW_W;
		devmode.dmPelsHeight = h;//SWINDOW_H;

		Sint32 bw,bh;
		bw =  GetSystemMetrics(SM_CXSIZEFRAME)*2;
		bh =  GetSystemMetrics(SM_CYSIZEFRAME)*2;
		SetMenu           ( g_pWindows->m_hWindow , NULL );
		SetWindowLong     ( g_pWindows->m_hWindow , GWL_STYLE, WS_POPUP|WS_VISIBLE|WS_EX_TOPMOST );
		MoveWindow        (	g_pWindows->m_hWindow , -bw/2,-bh/2, w+bw , h+bh , true	);

		Sint32 err = ChangeDisplaySettings( &devmode, CDS_FULLSCREEN );
		if( DISP_CHANGE_SUCCESSFUL )
		{
		}

		//マルチディスプレイ時のカーソルの移動制限を行う
		g_pWindows->m_uScreenWidth  = w;
		g_pWindows->m_uScreenHeight = h;

		SetRect(&rc, 0, 0, g_pWindows->m_uScreenWidth, g_pWindows->m_uScreenHeight );
		ClipCursor( &rc );

		UpdateWindow( g_pWindows->m_hWindow );

//		::Resume();

		g_pWindows->m_bFullScreen = gxTrue;

		while( ShowCursor(FALSE)>0 );
	}
	else
	{
		//Windowモード

		if( !g_pWindows->m_bFullScreen ) return;
/*
		devmode.dmSize       = sizeof(DEVMODE);
		devmode.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT;
		devmode.dmPelsWidth  = SWINDOW_W;
		devmode.dmPelsHeight = SWINDOW_H;
*/
		Sint32 bw,bh;
#ifdef NDEBUG
		bw =  GetSystemMetrics(SM_CXSIZEFRAME)*2;
		bh =  GetSystemMetrics(SM_CYSIZEFRAME)*2+GetSystemMetrics(SM_CYCAPTION);
//		SetMenu( ggx.m_Windows.g_hWnd , NULL );
#else
		bw =  GetSystemMetrics(SM_CXFIXEDFRAME)*2;
		bh =  GetSystemMetrics(SM_CYFIXEDFRAME)*2;
		bh += GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYMENU);
//		SetMenu( ggx.m_Windows.g_hWnd , LoadMenu( ggx.m_Windows.hInst ,	MAKEINTRESOURCE(CGameGirl::GetInstance()->MENU_BAR) ) );
#endif

		//Windowの大きさを元に戻す
		SetWindowLong( g_pWindows->m_hWindow , GWL_STYLE  , g_pWindows->m_AppStyle );
		SetWindowPos ( g_pWindows->m_hWindow,
		               HWND_NOTOPMOST,
		               g_pWindows->m_WinRect.left , g_pWindows->m_WinRect.top,
					   g_pWindows->m_WinRect.right-g_pWindows->m_WinRect.left, g_pWindows->m_WinRect.bottom-g_pWindows->m_WinRect.top,
					   SWP_SHOWWINDOW );

		ChangeDisplaySettings(NULL, 0);

		//マルチディスプレイ時のカーソルの移動制限を解除
		ClipCursor( NULL );

//		g_pWindows->m_uScreenWidth  = SWINDOW_W;
//		g_pWindows->m_uScreenHeight = SWINDOW_H;

		UpdateWindow( g_pWindows->m_hWindow );

//		::Resume();

		g_pWindows->m_bFullScreen = gxFalse;

		ShowCursor(TRUE);
	}

}


void MakeThread( void (*pFunc)(void*) , void * pArg )
{
	DWORD threadId; 
	HANDLE hThread;

	// スレッドの作成
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pFunc, (LPVOID)pArg, CREATE_SUSPENDED, &threadId); 

	// スレッドの起動 
	ResumeThread(hThread); 

	//HANDLE myMutex = CreateMutex(NULL, TRUE, NULL);
	//TerminateThread()
	//ExitThread() 
	//スレッドの終了を待つ

//	WaitForSingleObject(hThread, INFINITE);
//	//スレッドのハンドルを閉じる
//	CloseHandle(hThread);

}

LRESULT	CALLBACK CGameGirlProc(HWND hw, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//コールバック受付け

	switch(iMsg) {
    case WM_CREATE:
	    DragAcceptFiles(hw, gxTrue);
		break;

	case WM_DROPFILES:
		{
			HDROP hDrop = (HDROP) wParam;
			Sint32 sNum = DragQueryFile(hDrop , 0xffffffff , NULL , NULL );

			gxChar str[512];

			for(Sint32 ii=0;ii<sNum;ii++)
			{
				wchar_t wStr[512];

				DragQueryFile(hDrop , ii , (LPWSTR)wStr , sizeof(wStr) );

				Uint32 uLen = wcslen(wStr);
				WideCharToMultiByte( CP_ACP , 0 ,(LPCWSTR)wStr , uLen+1 , str, uLen*2 ,NULL,NULL);

				//DragQueryFile(hDrop , ii , str , sizeof(str) );

				gxBool DragAndDrop(char* szFileName);
				DragAndDrop( str );
			}
		}
		break;

	case WM_CLOSE:
		DragAcceptFiles(hw, gxFalse);
		CGameGirl::GetInstance()->SetExitWindow();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SYSCOMMAND:
		//スクリーンセーバー抑制
		if ( wParam == SC_SCREENSAVE )
	    {
	        return 1;
	    }
		if ( wParam == SC_MONITORPOWER )
	    {
	        return 1;
	    }
	    return (DefWindowProc(hw, iMsg, wParam, lParam));

	case WM_COMMAND:
		InputShortCutCheck( LOWORD(wParam) );
		return 0;

	case WM_SIZE:
		g_pWindows->m_uScreenWidth  = LOWORD(lParam);
		g_pWindows->m_uScreenHeight = HIWORD(lParam);
		::Resume();
		break;

	case WM_QUERYENDSESSION:
		ShutdownBlockReasonCreate( hw ,L"gxLib running" );
		return false;

 	case WM_ENDSESSION:
		CGameGirl::GetInstance()->SetExitWindow();
		ShutdownBlockReasonDestroy( hw );
		break;

	default:
		InputKeyCheck( iMsg , wParam , lParam );
		break;
	}

	return DefWindowProc( hw, iMsg, wParam, lParam);
}



gxBool vSyncWithTimer( gxBool bNoWaitvSync )
{
	//------------------------------------------
	//ｖＳｙｎｃ待ち(1/60秒経ったら勝手に帰る)
	//------------------------------------------
	MSG msg;
	static gxBool s_bFirst = gxFalse;
	static LONGLONG before;
	static LONGLONG time,time1sec;

	gxBool bSyoriOchi = gxFalse;

	if( !s_bFirst )
	{
		// 高精度タイマーの初期化
		g_pWindows->Vsyncrate = g_pWindows->Vsyncrate/60;									//１フレームのカウント数を取得
		QueryPerformanceCounter((LARGE_INTEGER*)&before);			//現在の時間を取得
		s_bFirst ++;
	}

	QueryPerformanceCounter((LARGE_INTEGER*)&time);				//現在の時間を取得

	if( time > (before+g_pWindows->Vsyncrate) )
	{
		bSyoriOchi = gxTrue;
	}

	do{ // ループ時間待機（最低一回は通る）
		QueryPerformanceCounter((LARGE_INTEGER*)&time);				//現在の時間を取得

		while(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if(!GetMessage(&msg, NULL, 0, 0))
			{
				 g_pWindows->m_wParam = msg.wParam;
				 return bSyoriOchi; 
			}

			if (!TranslateAccelerator( g_pWindows->m_hWindow , g_pWindows->m_hAccel , &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		if( bNoWaitvSync ) break;
	}
	while( time<(before+g_pWindows->Vsyncrate) );

	before = time;

	return bSyoriOchi;
}


void Clock()
{

#if 1
	struct timeval myTime; 

	LARGE_INTEGER time, freq;

	QueryPerformanceFrequency( &freq );
	QueryPerformanceCounter  ( &time );

	//1秒以上の現在時刻(sec) = 現在時刻(HPET) - 秒間周波数(HPET)
	myTime.tv_sec = (long)(time.QuadPart / freq.QuadPart);

	//1秒未満の現在時刻(mSec) = 現在時刻(mSec) - ((マイクロ)現在秒)
	myTime.tv_usec = (long)( ( time.QuadPart * 1000000.0 / freq.QuadPart ) - (myTime.tv_sec * 1000000.0) );

	SYSTEMTIME st;
	GetSystemTime(&st);


	Sint32 Year = st.wYear;// +1900;
	Sint32 Month = st.wMonth;
	Sint32 Day   = st.wDay;
	//Sint32 time_st->tm_wday;

	Sint32 Hour = st.wHour+9;
	Sint32 Min  = st.wMinute;
	Sint32 Sec  = st.wSecond;
	Sint32 MSec = st.wMilliseconds;
	Sint32 USec = myTime.tv_sec;	//実際の値とは異なる

	CGameGirl::GetInstance()->SetTime( Year , Month , Day , Hour , Min , Sec , MSec , USec);
#endif

/*
	LARGE_INTEGER freq;
	QueryPerformanceFrequency( &freq );

 	LARGE_INTEGER now;
	QueryPerformanceCounter( &now );

	now.QuadPart / freq.QuadPart;
*/

}



void InputShortCutCheck( Uint32 uId )
{
	//ショートカットキー対応

	switch ( uId ){
	case enID_ChangeFullScreenMode:
		ChangeScreenMode( !g_pWindows->m_bFullScreen );
		break;

	case enID_AppExit:
		if( g_pWindows->m_bFullScreen )
		{
			//フルスクリーン時にはウインドウモードに戻す
			ChangeScreenMode( gxFalse );
		}
		else
		{
			CGameGirl::GetInstance()->SetExitWindow();
		}
		break;

	case enID_GamePause:
		//ゲームの一時停止
		if( CGameGirl::GetInstance()->IsPause() )
		{
			CGameGirl::GetInstance()->SetPause(gxFalse);
		}
		else
		{
			CGameGirl::GetInstance()->SetPause(gxTrue);
		}
		break;

	case enID_GameStep:
		//ゲームのコマ送り
		if( CGameGirl::GetInstance()->IsPause() )
		{
			CGameGirl::GetInstance()->SetKomaokuri(gxTrue);
		}
		else
		{
			CGameGirl::GetInstance()->SetPause(gxTrue);
		}
		break;

	case enID_PadConfig:
		/*
		if( CGameGirl::GetInstance()->IsPadConfigMode() )
		{
			CGameGirl::GetInstance()->SetPadConfig( gxFalse );

		}
		else
		*/
		{
			CGameGirl::GetInstance()->SetPadConfig();
	}
		break;

	case enID_DebugMode:
		//デバッグスイッチ
		gxDebug::GetInstance()->m_bMasterDebugSwitch = !gxDebug::GetInstance()->m_bMasterDebugSwitch;
		break;

	case enID_Reset:
		//リセット
		CGameGirl::GetInstance()->SetReset();
		break;

	}
}


