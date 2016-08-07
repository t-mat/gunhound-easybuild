// --------------------------------------------------------------------
//
// つじつまあわせ用共通ヘッダ
//
// --------------------------------------------------------------------
#define PLATFORM_WINDOWS

#ifdef WIN32
	#define GX_BUILD_OPTIONx86
#else
	#define GX_BUILD_OPTIONx64
#endif

#ifdef _DEBUG
	#define GX_DEBUG
#endif

//#define _WIN32_WINNT _WIN32_WINNT_WINXP
//#define _WIN32_WINNT _WIN32_WINNT_WIN2K
//#define _WIN32_WINNT _WIN32_WINNT_WINXP
//#define _WIN32_WINNT _WIN32_WINNT_VISTA
//#define _WIN32_WINNT _WIN32_WINNT_WIN7
//#define _WIN32_WINNT _WIN32_WINNT_WIN8
#define _WIN32_WINNT _WIN32_WINNT_WIN8


#define DEBUG_FONT_SIZE (12)					//デバッグ用のフォントサイズ 12がデフォ
#define DEBUG_FONT_NAME "M+2VM+IPAG circle"		//デバッグ用のフォント種類

#define KEYSIGN_U VK_UP
#define KEYSIGN_D VK_DOWN
#define KEYSIGN_L VK_LEFT
#define KEYSIGN_R VK_RIGHT

#define KEYSIGN01 'Z'			//ボタン１
#define KEYSIGN02 'X'			//ボタン２
#define KEYSIGN03 'C'			//ボタン３
#define KEYSIGN04 'A'			//ボタン４
#define KEYSIGN05 'S'			//ボタン５
#define KEYSIGN06 'D'			//ボタン６
#define KEYSIGN07 VK_CONTROL	//ボタンＬ
#define KEYSIGN08 VK_SHIFT		//ボタンＲ
#define KEYSIGN09 VK_LCONTROL	//ボタンL2
#define KEYSIGN10 VK_RCONTROL	//ボタンR2
#define KEYSIGN11 VK_SPACE		//ＳＥＬＥＣＴ
#define KEYSIGN12 VK_RETURN		//ＳＴＡＲＴ


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
//gxLib的に必須
//----------------------------------------------------
void InitAction();
void Render();
gxBool vSync( gxBool bNoWait = gxFalse );
void Flip();
void Resume();

void Upload(Sint32 sBank);
void LogDisp(char* pString);
void Play();

Uint32 GetDebugTime();
Uint32 GetVsyncRate();

Uint8* LoadFile( const gxChar* pFileName , Uint32* pLength );
gxBool SaveFile( const gxChar* pFileName , Uint8* pReadBuf , Uint32 uSize );

void MakeThread( void (*pFunc)(void*) , void * pArg );

//----------------------------------------------------
//Win32固有
//----------------------------------------------------
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
	Uint16 eCommand;
	Uint16 arg[4];
	Sint16 x,y;
	void*  pString;
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
		Uint32 len = strlen( pStr );

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


gxBool GameDragAndDrop();


