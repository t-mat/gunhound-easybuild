// --------------------------------------------------------------------
//
// つじつまあわせ用共通ヘッダ
//
// --------------------------------------------------------------------
#ifndef _MACHINE_H_
#define _MACHINE_H_


#ifdef PLATFORM_WIN32_CONSOLE
	#include "win32console/machine.h"

#elif defined PLATFORM_WIN32_APP
	#include "win32app/machine.h"

#elif defined PLATFORM_WIN32_DIRECTX9
	#include "win32DirectX9/machine.h"

#elif defined PLATFORM_WIN32_OPENGL
	#include "win32OpenGL/machine.h"

#elif defined PLATFORM_WIN64_DIRECTX11

#elif defined PLATFORM_ANDROID

#elif defined PLATFORM_IOS

#elif defined PLATFORM_COCOS2DX
	#include "cocos2dx/machine.h"

#endif


//各プラットフォーム共通


gxBool GameMain();
gxBool GameSleep();
gxBool GameResume();
gxBool GameEnd();
gxBool GameReset();

void   InitAction();
void   Render();
gxBool vSync( gxBool bNoWait = gxFalse );
void   Flip();

void   Clock();
void   Movie();

void   Resume();

void   Upload(Sint32 sBank);
void   LogDisp(char* pString);
void   Play();


Uint8* LoadFile( const gxChar* pFileName , Uint32* pLength );
gxBool SaveFile( const gxChar* pFileName , Uint8* pReadBuf , Uint32 uSize );

//ストレージへのファイルアクセス

gxBool SaveStorageFile( const gxChar* pFileName , Uint8* pReadBuf , Uint32 uSize );
Uint8* LoadStorageFile( const gxChar* pFileName , Uint32* pLength );

//特殊

void   MakeThread( void (*pFunc)(void*) , void * pArg );
gxBool PadConfig( Sint32 padNo , Uint32 button );

#endif
