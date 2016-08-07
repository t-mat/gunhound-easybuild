#ifndef _CWIN32_H_
#define _CWIN32_H_

class CWindows
{
public:

	CWindows()
	{
		m_uScreenWidth  = WINDOW_W;
		m_uScreenHeight = WINDOW_H;
		m_bFullScreen   = gxFalse;

		for( Sint32 ii=0; ii<256; ii++ )
		{
			m_KeyBoard[ii] = 0x00;
		}

	}

	~CWindows()
	{
	}

	HINSTANCE m_hInstance;
	HWND      m_hWindow;
	WPARAM    m_wParam;
	HACCEL    m_hAccel;
	HDC		  m_WinDC;

	Uint32 m_AppStyle;
	RECT   m_WinRect;

	Uint32 m_uScreenWidth;
	Uint32 m_uScreenHeight;
	gxBool m_bFullScreen;

	LONGLONG Vsyncrate;

	Sint32 m_KeyBoard[256];

private:


};


//タッチデバイス情報
typedef struct StTouchDevice
{
	//タッチデバイス情報管理用

//	Sint32 sx,sy;
//	Sint32 nx,ny;
//	Sint32 ex,ey;
	Sint32 x,y;
	Sint32 id;
	gxBool bUse;
	gxBool psh;

	void Reset()
	{
		id = 0xffffffff;
		bUse = gxFalse;
//		sx = sy = 0;
//		nx = ny = 0;
//		ex = ey = 0;
		x = y = 0;
		psh = 0;
	}

} StTouchDevice;

enum {
	enID_ChangeFullScreenMode = 1001,	//フルスクリーン切り替え
	enID_AppExit              = 1002,	//アプリ終了
	enID_GamePause			  = 1003,	//ゲームのポーズ
	enID_GameStep			  = 1004,	//ゲームのステップ
	enID_PadConfig			  = 1005,	//コントローラー設定
	enID_DebugMode			  = 1006,	//デバッグモードのON/OFF
	enID_Reset				  = 1007,	//リセット
	enAccelMax			  	  = 7,

};

#define SWINDOW_W (WINDOW_W)
#define SWINDOW_H (WINDOW_H)

extern CWindows* g_pWindows;
extern StTouchDevice *m_pTouchDevice;

void makeWindow();
void updateMemoryStatus();
gxBool vSyncWithTimer( gxBool bNoWaitvSync );

#endif
