// -----------------------------------------------------------------
//
//
//
// -----------------------------------------------------------------

// �ȑO�� Windows �v���b�g�t�H�[���p�ɃA�v���P�[�V�������r���h����ꍇ�́AWinSDKVer.h ���C���N���[�h���A
// SDKDDKVer.h ���C���N���[�h����O�ɁA�T�|�[�g�ΏۂƂ���v���b�g�t�H�[���������悤�� _WIN32_WINNT �}�N����ݒ肵�܂��B

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
	//���r�������҂�(1/60�b�o�����珟��ɋA��)
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


