// --------------------------------------------------------------------
//
// つじつまあわせ用共通ヘッダ
//
// --------------------------------------------------------------------

#define PLATFORM_WINDOWS

#ifdef _DEBUG
	#define GX_DEBUG
#else
	#define GX_RELEASE
#endif

#ifdef WIN32
	#define GX_BUILD_OPTIONx86
#else
	#define GX_BUILD_OPTIONx64
#endif

#pragma warning(disable : 4996)
#pragma warning(disable : 4819)

#include <d3d9.h>
#include <d3dx9.h>

//#define _WIN32_WINNT _WIN32_WINNT_WINXP
//#define _WIN32_WINNT _WIN32_WINNT_WIN2K
//#define _WIN32_WINNT _WIN32_WINNT_WINXP
//#define _WIN32_WINNT _WIN32_WINNT_VISTA
//#define _WIN32_WINNT _WIN32_WINNT_WIN7
//#define _WIN32_WINNT _WIN32_WINNT_WIN8

//#define _WIN32_WINNT _WIN32_WINNT_WIN8(d3dx9,hで定義済み)


#define DEBUG_FONT_SIZE (12)					//デバッグ用のフォントサイズ 12がデフォ
#define DEBUG_FONT_NAME "M+2VM+IPAG circle"		//デバッグ用のフォント種類
#define INDEXBUFFER_BIT (32)

#pragma warning(disable : 4996)
#pragma warning(disable : 4819)

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <assert.h>
#include <string.h>
#include <math.h>

//#include <d3d9.h>
//#include <d3dx9.h>
//#include <dmusici.h>
//#include <d3d9types.h>
//#include "directx9/CHttpAccess.h"


//----------------------------------------------------
//プラットフォーム専用関数
//----------------------------------------------------
void Movie();
Uint32 GetDebugTime();
Uint32 GetVsyncRate();
void ExecuteApp( char *appname );
gxBool IsFullScreen();
void ChangeWindowMode( gxBool bWindow );


enum {
	eCmdChgTexture,
	eCmdNoneTexture,
	eCmdChgAttributeAlphaAdd,
	eCmdChgAttributeAlphaSub,
	eCmdChgAttributeAlphaCrs,
	eCmdChgAttributeAlphaRvs,
	eCmdChgAttributeAlphaNml,
	eCmdChgAttributeAlphaXor,
	eCmdChgAttributeAlphaScr,
	eCmdRenderSquare,
	eCmdRenderPoint,
	eCmdRenderLineStrip,
	eCmdRenderLineNormal,
	eCmdRenderTriangle,
	eCmdRenderFont,
};

typedef struct StCustomVertex
{
	Float32 x,y,z,rhw;
	Uint32  col;
	Float32 u,v;
} StCustomVertex;


class CCommandList
{
public:
	Uint32 eCommand;
	Uint32 arg[4];
	Sint32 x,y;
	void*  pString;
	Float32 opt;
private:

};


class CAppManager
{
public:

	enum {
		enArgMax = 32,
	};

	CAppManager()
	{
		for( Sint32 ii=0; ii<enArgMax; ii++)
		{
			m_pArg[ii] = NULL;
		}

		m_sArgNum = 0;
	}

	~CAppManager()
	{
		for( Sint32 ii=0; ii<enArgMax; ii++)
		{
			if( m_pArg[ii] ) SAFE_DELETES( m_pArg[ii] );
		}
	}

	void SetArg( int n , gxChar *pStr )
	{
		Uint32 len = static_cast<Uint32>( strlen( pStr ) );

		if( m_pArg[n] ) SAFE_DELETES( m_pArg[n] );

		m_pArg[n] = new gxChar[len];

		sprintf( &m_pArg[n][0] , "%s" , pStr );

		if( m_sArgNum < n+1 ) m_sArgNum = n+1;
	}

	gxChar* GetArg( int n )
	{
		if( n >= m_sArgNum ) return "None Arg";

		return &m_pArg[n][0];
	}

	SINGLETON_DECLARE( CAppManager );

	gxChar *m_pArg[enArgMax];
	Sint32 m_sArgNum;

};


#define KEYBOARD_ESCAPE      (VK_ESCAPE)
#define KEYBOARD_BACKSPACE   (VK_BACK)
#define KEYBOARD_TAB         (VK_TAB)
#define KEYBOARD_RETURN      (VK_RETURN)
#define KEYBOARD_SHIFT       (VK_LSHIFT)
#define KEYBOARD_RSHIFT      (VK_RSHIFT)
#define KEYBOARD_CTRL        (VK_LCONTROL)
#define KEYBOARD_RCTRL       (VK_RCONTROL)
#define KEYBOARD_ALT         (VK_LMENU)
#define KEYBOARD_RALT        (VK_RMENU)
#define KEYBOARD_ARROW_UP    (VK_UP)
#define KEYBOARD_ARROW_DOWN  (VK_DOWN)
#define KEYBOARD_ARROW_LEFT  (VK_LEFT)
#define KEYBOARD_ARROW_RIGHT (VK_RIGHT)
#define KEYBOARD_SPACE       (VK_SPACE)
#define KEYBOARD_ENTER       (VK_RETURN)
#define KEYBOARD_F1          (VK_F1)
#define KEYBOARD_F2          (VK_F2)
#define KEYBOARD_F3          (VK_F3)
#define KEYBOARD_F4          (VK_F4)
#define KEYBOARD_F5          (VK_F5)
#define KEYBOARD_F6          (VK_F6)
#define KEYBOARD_F7          (VK_F7)
#define KEYBOARD_F8          (VK_F8)
#define KEYBOARD_F9          (VK_F9)
#define KEYBOARD_F10         (VK_F10)
#define KEYBOARD_F11         (VK_F11)
#define KEYBOARD_F12         (VK_F12)
#define KEYBOARD_0           ('0')
#define KEYBOARD_1           ('1')
#define KEYBOARD_2           ('2')
#define KEYBOARD_3           ('3')
#define KEYBOARD_4           ('4')
#define KEYBOARD_5           ('5')
#define KEYBOARD_6           ('6')
#define KEYBOARD_7           ('7')
#define KEYBOARD_8           ('8')
#define KEYBOARD_9           ('9')
#define KEYBOARD_A           ('A')
#define KEYBOARD_B           ('B')
#define KEYBOARD_C           ('C')
#define KEYBOARD_D           ('D')
#define KEYBOARD_E           ('E')
#define KEYBOARD_F           ('F')
#define KEYBOARD_G           ('G')
#define KEYBOARD_H           ('H')
#define KEYBOARD_I           ('I')
#define KEYBOARD_J           ('J')
#define KEYBOARD_K           ('K')
#define KEYBOARD_L           ('L')
#define KEYBOARD_M           ('M')
#define KEYBOARD_N           ('N')
#define KEYBOARD_O           ('O')
#define KEYBOARD_P           ('P')
#define KEYBOARD_Q           ('Q')
#define KEYBOARD_R           ('R')
#define KEYBOARD_S           ('S')
#define KEYBOARD_T           ('T')
#define KEYBOARD_U           ('U')
#define KEYBOARD_V           ('V')
#define KEYBOARD_W           ('W')
#define KEYBOARD_X           ('X')
#define KEYBOARD_Y           ('Y')
#define KEYBOARD_Z           ('Z')

#define KEYSIGN_U KEYBOARD_ARROW_UP
#define KEYSIGN_D KEYBOARD_ARROW_DOWN
#define KEYSIGN_L KEYBOARD_ARROW_LEFT
#define KEYSIGN_R KEYBOARD_ARROW_RIGHT

#define KEYSIGN01 KEYBOARD_Z		//ボタン１
#define KEYSIGN02 KEYBOARD_X		//ボタン２
#define KEYSIGN03 KEYBOARD_C		//ボタン３
#define KEYSIGN04 KEYBOARD_A		//ボタン４
#define KEYSIGN05 KEYBOARD_S		//ボタン５
#define KEYSIGN06 KEYBOARD_D		//ボタン６
#define KEYSIGN07 KEYBOARD_SHIFT	//ボタンＬ
#define KEYSIGN08 KEYBOARD_RSHIFT	//ボタンＲ
#define KEYSIGN09 KEYBOARD_CTRL		//ボタンL2
#define KEYSIGN10 KEYBOARD_RCTRL	//ボタンR2
#define KEYSIGN11 KEYBOARD_SPACE	//ＳＥＬＥＣＴ
#define KEYSIGN12 KEYBOARD_ENTER	//ＳＴＡＲＴ

gxBool GameDragAndDrop();

