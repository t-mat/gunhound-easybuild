// -----------------------------------------------------------------
//
//
//
// -----------------------------------------------------------------

// 以前の Windows プラットフォーム用にアプリケーションをビルドする場合は、WinSDKVer.h をインクルードし、
// SDKDDKVer.h をインクルードする前に、サポート対象とするプラットフォームを示すように _WIN32_WINNT マクロを設定します。

#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <fcntl.h>
#include <time.h> 

#include <gxLib.h>
#include "../win32cmn/win32cmn.h"

SINGLETON_DECLARE_INSTANCE(CAppManager);


int main( int arg , char *argv[] )
{
	for( int ii=0; ii<arg; ii++ )
	{
		printf("argv=%s" , argv[ii]);
	}

	GameMain();

	GameEnd();

    return 0;
}


void InitAction()
{

}

void Render()
{

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
}


void Resume()
{
}


void Play()
{
}

void Upload( Sint32 sBank )
{
	updateMemoryStatus();
}


void Movie()
{

}


gxBool PadConfig( Sint32 padNo , Uint32 button )
{
	
}


