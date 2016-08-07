//------------------------------------------------------------
//
// �Q�[�����C�����[�v
//
//------------------------------------------------------------

#include <gunvalken.h>
#include <gxLib/Util/gxImage/CGXImage.h>
#include "DashBoard/CDashBoard.h"
#include "appendTexture.h"


gxBool GunHoundMain(gxBool bReset);

gxBool GunHound( gxBool bReset )
{
#ifdef USE_DASHBOARD
	//�_�b�V���{�[�h�̃A�N�V����

	CDashBoard::GetInstance()->SetLock(gxTrue);
	//���݂̃��C�u�����Ɛ��������ɂ����̂�
	//���̃o�[�W�����ł͈ꕔ�̃_�b�V���{�[�h�@�\��
	//���b�N���Ďg�p�s�ɂ��܂�

	CDashBoard::GetInstance()->Action();

	if( CDashBoard::GetInstance()->IsMenuBar() )
	{
		if( CDashBoard::GetInstance()->IsReset() )
		{
			CDashBoard::GetInstance()->SetResetOff();
			bReset = gxTrue;
		}
	}
#endif

	GunHoundMain( bReset );

	return gxTrue;
}


gxBool GunHoundMain(gxBool bReset)
{
	//------------------------------------------
	//���C���V�[�P���X
	//------------------------------------------
	static Sint32 g_sMainGameSeq = 0;
	static Sint32 g_sMainLogoTime = 120;

	ghLib::AdjustControl();

	switch( g_sMainGameSeq ){
	case 0:
		//�P�x�����ʂ�Ȃ�
		{
			Uint32 uSize;
			Uint8 *pData;
			CGXImage::GetInstance()->Load("asset/gh/GXI/first.GXI");

			pData = CGXImage::GetInstance()->GetFile( "DashBoard.tga" , &uSize );
			ReadTexture( enTexPageDashBoard	, pData ,uSize );

			pData = CGXImage::GetInstance()->GetFile( "dqLogo.tga"    , &uSize );
			ReadTexture(enTexPageSysLogo, pData ,uSize );
		}

#ifndef _TRIAL_VERSION_
		//�̌��łł͓ǂݍ��܂Ȃ�
		ReadTexture( enTexPageEnemyCommon08	, AppendTexture ,sizeof( AppendTexture  ));
		ReadTexture( enTexPageLastBoss		, AppendTexture2,sizeof( AppendTexture2 ));
		ReadTexture( 5						, AppendTexture3,sizeof( AppendTexture3 ));
#endif

		CGXImage::DeleteInstance();

		UploadTexture();

		viiDbg::log("�_�b�V���{�[�h��ǂ݂��݁B");
		CDashBoard::GetInstance()->SetLogo(gxTrue);
		CDashBoard::GetInstance()->SetWallPaper(gxTrue);

		viiDbg::log("�����t�@�C���̓ǂݍ��݁B");
		g_sMainGameSeq = 10;
		return gxTrue;

	case 10:
		//���S�\��
		if( CDashBoard::GetInstance()->IsLogoComplete() )
		{
			//�t�F�[�h�C���I��������풓�t�@�C����ǂ�

			gxBool HoundStartUpLoading();
			if( HoundStartUpLoading() )
			{
				viiDbg::log("�����t�@�C���̓ǂݍ��ݏI���B");
				g_sMainGameSeq = 20;
			}
		}
		CDashBoard::GetInstance()->Action();
		return gxTrue;

	case 20:
		//���S�\���I���܂ő҂�
		CDashBoard::GetInstance()->Action();
		if( g_sMainLogoTime > 0 )
		{
			g_sMainLogoTime --;
		}
		else
		{
			g_sMainGameSeq = 999;
//			g_sMainGameSeq = 900;
			return gxTrue;
		}
		return gxTrue;

	case 30:
		g_sMainGameSeq = 999;
		break;

	case 900:
		return gxTrue;

	case 910:
		return gxTrue;

	case 920:
		break;

	default:
		break;
	}


	//�_�b�V���{�[�h�̃A�N�V����
/*
	CDashBoard::GetInstance()->Action();
*/
	gxBool GunHoundGameMain(Sint32 cmd=0);

/*
	if( CDashBoard::GetInstance()->IsMenuBar() )
	{
		Sint32 sCmd = 0;
		if( CDashBoard::GetInstance()->IsReset() )
		{
//			//�Q�[���I��
//			return gxFalse;
			CDashBoard::GetInstance()->SetResetOff();
			sCmd = -1;	//Cmd::���Z�b�g
		}
		GunHoundGameMain(sCmd);
	}
	else
	{
		if( GunHoundGameMain() == gxFalse ) return 0;
	}
*/
	if( GunHoundGameMain( (bReset)? -1 : 0) == gxFalse ) return 0;
	
	return gxTrue;
}


gxBool DispTrialSplash()
{
	//-----------------------------------------
	//�̌��ŗp�̃X�v���b�V�����
	//-----------------------------------------

	if( Joy[0].trg)
	{
		return gxTrue;
	}
	GGX_DrawBox( 0,0,WINDOW_W,WINDOW_H,enPrioHidefBoard-1,gxTrue,0xff010101);
	PutSpriteDX( 0,4,enPrioHidefBoard , enTexPageSearchLight,0,0,WINDOW_W,WINDOW_H-8,0,0 );

	return gxFalse;
}

