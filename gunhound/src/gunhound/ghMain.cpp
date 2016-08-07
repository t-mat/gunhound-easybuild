//------------------------------------------------------------
//
// ゲームメインループ
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
	//ダッシュボードのアクション

	CDashBoard::GetInstance()->SetLock(gxTrue);
	//現在のライブラリと整合が取りにくいので
	//このバージョンでは一部のダッシュボード機能を
	//ロックして使用不可にします

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
	//メインシーケンス
	//------------------------------------------
	static Sint32 g_sMainGameSeq = 0;
	static Sint32 g_sMainLogoTime = 120;

	ghLib::AdjustControl();

	switch( g_sMainGameSeq ){
	case 0:
		//１度しか通らない
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
		//体験版では読み込まない
		ReadTexture( enTexPageEnemyCommon08	, AppendTexture ,sizeof( AppendTexture  ));
		ReadTexture( enTexPageLastBoss		, AppendTexture2,sizeof( AppendTexture2 ));
		ReadTexture( 5						, AppendTexture3,sizeof( AppendTexture3 ));
#endif

		CGXImage::DeleteInstance();

		UploadTexture();

		viiDbg::log("ダッシュボードを読みこみ。");
		CDashBoard::GetInstance()->SetLogo(gxTrue);
		CDashBoard::GetInstance()->SetWallPaper(gxTrue);

		viiDbg::log("初期ファイルの読み込み。");
		g_sMainGameSeq = 10;
		return gxTrue;

	case 10:
		//ロゴ表示
		if( CDashBoard::GetInstance()->IsLogoComplete() )
		{
			//フェードイン終了したら常駐ファイルを読む

			gxBool HoundStartUpLoading();
			if( HoundStartUpLoading() )
			{
				viiDbg::log("初期ファイルの読み込み終了。");
				g_sMainGameSeq = 20;
			}
		}
		CDashBoard::GetInstance()->Action();
		return gxTrue;

	case 20:
		//ロゴ表示終了まで待つ
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


	//ダッシュボードのアクション
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
//			//ゲーム終了
//			return gxFalse;
			CDashBoard::GetInstance()->SetResetOff();
			sCmd = -1;	//Cmd::リセット
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
	//体験版用のスプラッシュ画面
	//-----------------------------------------

	if( Joy[0].trg)
	{
		return gxTrue;
	}
	GGX_DrawBox( 0,0,WINDOW_W,WINDOW_H,enPrioHidefBoard-1,gxTrue,0xff010101);
	PutSpriteDX( 0,4,enPrioHidefBoard , enTexPageSearchLight,0,0,WINDOW_W,WINDOW_H-8,0,0 );

	return gxFalse;
}

