//----------------------------------------------------------------------------
//
//チュートリアルステージ 
//
//----------------------------------------------------------------------------

#include <gunvalken.h>
#include <gunhound/DashBoard/CDashBoard.h>
#include <gxLib/util/gxImage/CGXImage.h>
#include "CStageBase.h"
//---------------------------------------------------------------------------
#include "../effect/EffectBase.h"
#include "../enemy/enemy.h"
#include "../GunHound/GhStringTbl.h"

enum
{
	enAreaSeq01,
	enAreaSeq02,
	enAreaSeq03,
	enAreaSeq04,
	enAreaSeq05,
	enAreaSeqMax,
};

enum {
	enRollerDashHeight = 850*100,
};

void CStageTutorial::Init()
{
	//---------------------------------------------------------------------------
	//初期化
	//---------------------------------------------------------------------------
	//SetFadeIn();

	Sint32 col1 =  0xF0004080;
	Sint32 col2 =  0xF00000F0;
	pGame->pBg->SetSkyColor(50,col1,col2);

	CDashBoard::GetInstance()->SetIcon( enIconNone );

	switch(GetArea( )){
	case enAreaSeq01:
	case enAreaSeq02:
	case enAreaSeq03:
		break;
	default:
		pGame->pBg->DebugTipDraw();
		CGXImage::GetInstance()->Load( "asset/gh/GXI/Tutorial.GXI" );
		if( CGXImage::GetInstance()->IsLoadFinish() )
		{
			SetDRQStage( "Stage.txt" );
			viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		}
		CGXImage::DeleteInstance();
		InitEnemies();
		break;
	}

}


void CStageTutorial::End()
{
	//---------------------------------------------------------------------------
	//終了処理
	//---------------------------------------------------------------------------


}

void CStageTutorial::InitEnemies()
{
	//---------------------------------------------------------------------------
	//敵の初期化
	//---------------------------------------------------------------------------

	SetEnemy();

}

void CStageTutorial::Action()
{
	//---------------------------------------------------------------------------
	//毎フレームのアクション
	//---------------------------------------------------------------------------

	StageCommon();
	AreaCommon();

	switch(GetArea( )){
	case enAreaSeq01:
		Area01();
		break;

	case enAreaSeq02:
		Area02();
		break;

	case enAreaSeq03:
		Area03();
		break;

	case enAreaSeq04:
		Area04();
		break;

	case enAreaSeq05:
		Area05();
		break;
	}

}


void CStageTutorial::Area01()
{
	//---------------------------------------------------------------------------
	//エリア１
	//---------------------------------------------------------------------------
	gxBool bMessage = gxFalse;

	if( IsZoneSeq( 0 ) )
	{
		SetFadeIn();
		bMessage = gxTrue;
		pGame->GetHound()->SetMode( CGunHound::enHoundModeNone );
		CCockpit::GetInstance()->SetSplash( gxTrue );
		CGXImage::GetInstance()->Load( "asset/gh/GXI/Tutorial.GXI" );
		SetZoneSeq( 10 );
	}
	else if( IsZoneSeq( 10 ) )
	{
		bMessage = gxTrue;

		if( CGXImage::GetInstance()->IsLoadFinish() )
		{
			SetDRQStage( "tutorial1.txt" );
			viiSub::SetScroll_l( 0,450*100 );
			viiMus::PlayBGM(enSoundBgm1,enMusicBgmBriefing);
			InitEnemies();
			SetZoneSeq( 20 );
		}
	}
	else if( IsZoneSeq( 20 ) )
	{
		bMessage = gxTrue;

		CDashBoard::GetInstance()->SetIcon( enIconDecision );

		if(Joy[0].trg&enBtnDecision)
		{
			pGame->pBg->DebugTipDraw();
			CCockpit::GetInstance()->SetSplash( gxFalse );
			CDashBoard::GetInstance()->SetIcon(enIconNone);
			pGame->GetHound()->SetMode( CGunHound::enHoundModeNormal );
			SetZoneSeq( 100 );
		}
	}
	else if( IsZoneSeq( 100 ) )
	{
		//最初のメッセージ

		if( IsFadeEnd() )
		{
			CCockpit::GetInstance()->SetMessage(1000);
			SetZoneSeq( 120 );
		}
	}
	else if( IsZoneSeq( 120 ) )
	{
		if( IsAreaArrive(151*8,74*8,16*8,8*8) )
		{
			//ジャンプ練習終わり
			CCockpit::GetInstance()->SetMessage(1200);
			SetZoneSeq( 130 );
		}
	}
	else if( IsZoneSeq( 130 ) )
	{
		//ダッシュ練習開始
		m_sScrollMin = 1187*100;

		if( IsAreaArrive(308*8,71*8,17*8,8*8) )
		{
			//ダッシュ練習終わり
			CCockpit::GetInstance()->SetMessage(1300);
			SetZoneSeq( 140 );
		}
	}
	else if( IsZoneSeq( 140 ) )
	{
		//ダッシュ＋ジャンプ終わり(失敗終わり)
		if( IsAreaArrive(436*8,75*8,3*8,63*8) )
		{
			CCockpit::GetInstance()->SetMessage(3000);
		}
	}

	//-------------------------------------------------------

	if( bMessage )
	{
		char **pMsg = &TutorialCommand01[0];
		Sint32 x=8,y=80,ii=0;
		Sint32 sAlp = 255-CFadeManager::GetInstance()->GetFadeAlpha();
		Sint32 yy = 0;

		viiDraw::Box(x,y,x+280,y+128,PRIO_FADEOVER+1,gxTrue,0x80010101,ATR_DFLT);

		while( pMsg[ii] != NULL )
		{
			if( pMsg[ii] == "" )
			{
				yy += 8;
			}
			else
			{
				viiSub::MenuPrintf( x,y+12+yy,ARGB(sAlp,0xff,0xff,0xff) , pMsg[ii] );
				yy += 12;
			}

			ii++;
		}
		return;
	}

	//-------------------------------------------------------

	if( IsAreaArrive(71*8,58*8,12*8,16*8) )
	{
		//そこはブーストを使え！
		CCockpit::GetInstance()->SetMessage(1050);
	}

	if( IsAreaArrive(115*8,71*8,18*8,11*8) )
	{
		//落ちても大丈夫だ
		CCockpit::GetInstance()->SetMessage(1070);
	}

	if( IsAreaArrive(144*8,28*8,6*8,10*8) )
	{
		//軟着陸開始
		CCockpit::GetInstance()->SetMessage(1100);
	}

	if( IsAreaArrive(241*8,103*8,43*8,8*8) )
	{
		//第１次：奈落いき
		CCockpit::GetInstance()->SetMessage(1230);
	}

	if( IsAreaArrive(337*8,99*8,64*8,7*8) )
	{
		//第２次：奈落いき
		CCockpit::GetInstance()->SetMessage(1330);
	}

	if( IsAreaArrive(408*8,56*8,9*8,7*8) )
	{
		//上級者コース入りぐち
		CCockpit::GetInstance()->SetMessage(1400);
	}


	//ステージクリア
	if( IsAreaArrive(4100,559,WINDOW_W,WINDOW_H) )
	{
		CCockpit::GetInstance()->SetMessage(3200);
		ChangeArea( enAreaSeq05 );
	}

	BgCommon();
	viiSub::LimitMoveVH();

}



void CStageTutorial::Area02()
{
	//-----------------------------------------------------
	//攻撃サンプル
	//-----------------------------------------------------
	gxBool bMessage = gxFalse;

	if( IsZoneSeq( 0 ) )
	{
		SetFadeIn();
		bMessage = gxTrue;
		pGame->GetHound()->SetMode( CGunHound::enHoundModeNone );
		CCockpit::GetInstance()->SetSplash( gxTrue );
		CGXImage::GetInstance()->Load( "asset/gh/GXI/Tutorial.GXI" );
		SetZoneSeq( 10 );
	}
	else if( IsZoneSeq( 10 ) )
	{
		bMessage = gxTrue;

		if( CGXImage::GetInstance()->IsLoadFinish() )
		{
			SetDRQStage( "tutorial2.txt" );
			viiSub::SetScroll_l( 0,450*100 );
			viiMus::PlayBGM(enSoundBgm1,enMusicBgmBriefing);
			InitEnemies();
			SetZoneSeq( 20 );
		}
	}
	else if( IsZoneSeq( 20 ) )
	{
		bMessage = gxTrue;

		CDashBoard::GetInstance()->SetIcon( enIconDecision );

		if(Joy[0].trg&enBtnDecision)
		{
			pGame->pBg->DebugTipDraw();
			CCockpit::GetInstance()->SetSplash( gxFalse );
			CDashBoard::GetInstance()->SetIcon(enIconNone);
			pGame->GetHound()->SetMode( CGunHound::enHoundModeNormal );
			SetZoneSeq( 100 );
		}
	}
	else if( IsZoneSeq( 100 ) )
	{
		//最初のメッセージ

		if( IsFadeEnd() )
		{
			CCockpit::GetInstance()->SetMessage(1000);
			SetZoneSeq( 120 );
		}
	}
	else if( IsZoneSeq( 120 ) )
	{
		if( IsAreaArrive(466*8,45*8,31*8,11*8) )
		{
			//次はハードブロウの特殊な使い方を
			CCockpit::GetInstance()->SetMessage(1200);
			SetZoneSeq( 200 );
		}
	}
	else if( IsZoneSeq( 200 ) )
	{
		//敵弾生成
		if( m_sZoneTimer%64==0)
		{
			new CEne0001EneBullet( 512*800 , 44*800 , 90+45 , 240 ,0 );
		}

		if( m_sZoneTimer%12==0 && m_sZoneTimer%(12*5) <12*2 )
		{
			new CEne0001EneBullet( 522*800 + 128*100, 44*800 , 90+25 , 120*3 ,1 );
		}

		if( m_sZoneTimer%8==0 &&  m_sZoneTimer%(12*5) <12*2 )
		{
			new CEne0001EneBullet( 512*800 + 128*200, 44*800 , 90-15 , 120*4 ,2 );
		}

		if( m_sZoneTimer%4==0 &&  m_sZoneTimer%(12*6) <12*4 )
		{
			new CEne0001EneBullet( 512*800 + 128*300+64*100, 44*800 , 90 , 120*5 ,2 );
		}
	}
	
	//-------------------------------------------------------
	if( bMessage )
	{
		char **pMsg = &TutorialCommand02[0];
		Sint32 x=8,y=80,ii=0;
		Sint32 sAlp = 255-CFadeManager::GetInstance()->GetFadeAlpha();
		Sint32 yy = 0;

		viiDraw::Box(x,y,x+280,y+128,PRIO_FADEOVER+1,gxTrue,0x80010101,ATR_DFLT);

		while( pMsg[ii] != NULL )
		{
			if( pMsg[ii] == "" )
			{
				yy += 8;
			}
			else
			{
				viiSub::MenuPrintf( x,y+12+yy,ARGB(sAlp,0xff,0xff,0xff) , pMsg[ii] );
				yy += 12;
			}

			ii++;
		}
		return;
	}

	//-------------------------------------------------------

	if( IsAreaArrive(135*8,79*8,27*8,22*8) )
	{
		//機装猟兵は対地、対空の２種類の兵装を切り替えることができる。
		CCockpit::GetInstance()->SetMessage(1050);
	}

	if( IsAreaArrive(300*8,52*8,16*8,15*8) )
	{
		//次は誘爆システムについて説明する。
		CCockpit::GetInstance()->SetMessage(1070);
		m_sScrollMin = 2410*100;
	}

	if( IsAreaArrive(385*8,45*8,15*8,11*8) )
	{
		//次はハードブロウの使い方をマスターする。
		if( CCockpit::GetInstance()->SetMessage(1100) )
		{
			new CEneH0410Wall( 449*800,37*800 );
		}
	}

	//ステージクリア
	if( IsAreaArrive(595*8,48*8,33*8,25*8) )
	{
		CCockpit::GetInstance()->SetMessage(3100);
		ChangeArea( enAreaSeq05 );
	}

	BgCommon();
	viiSub::LimitMoveVH();
}


void CStageTutorial::Area03()
{
	//-----------------------------------------------------
	//攻撃サンプル
	//-----------------------------------------------------
	gxBool bMessage = gxFalse;

	if( IsZoneSeq( 0 ) )
	{
		SetFadeIn();
		bMessage = gxTrue;
		pGame->GetHound()->SetMode( CGunHound::enHoundModeNone );
		CCockpit::GetInstance()->SetSplash( gxTrue );
		CGXImage::GetInstance()->Load( "asset/gh/GXI/Tutorial.GXI" );
		SetZoneSeq( 10 );
	}
	else if( IsZoneSeq( 10 ) )
	{
		bMessage = gxTrue;

		if( CGXImage::GetInstance()->IsLoadFinish() )
		{
			SetDRQStage( "tutorial3.txt" );
			viiSub::SetScroll_l( 0,450*100 );
			viiMus::PlayBGM(enSoundBgm1,enMusicBgmBriefing);
			InitEnemies();
			SetZoneSeq( 20 );
		}
	}
	else if( IsZoneSeq( 20 ) )
	{
		bMessage = gxTrue;

		CDashBoard::GetInstance()->SetIcon( enIconDecision );

		if(Joy[0].trg&enBtnDecision)
		{
			pGame->pBg->DebugTipDraw();
			CCockpit::GetInstance()->SetSplash( gxFalse );
			CDashBoard::GetInstance()->SetIcon(enIconNone);
			pGame->GetHound()->SetMode( CGunHound::enHoundModeNormal );
			SetZoneSeq( 100 );
		}
	}
	else if( IsZoneSeq( 100 ) )
	{
		//最初のメッセージ

		if( IsFadeEnd() )
		{
			CCockpit::GetInstance()->SetMessage(1000);
			SetZoneSeq( 120 );
		}
	}
	else if( IsZoneSeq( 120 ) )
	{
		if( IsAreaArrive(151*8,74*8,16*8,8*8) )
		{
			//ジャンプ練習終わり
			CCockpit::GetInstance()->SetMessage(1200);
			SetZoneSeq( 130 );
		}
	}
	else if( IsZoneSeq( 130 ) )
	{
		//ダッシュ練習開始
		m_sScrollMin = 1187*100;

		if( IsAreaArrive(308*8,71*8,17*8,8*8) )
		{
			//ダッシュ練習終わり
			CCockpit::GetInstance()->SetMessage(1300);
			SetZoneSeq( 140 );
		}
	}
	else if( IsZoneSeq( 140 ) )
	{
		//ダッシュ＋ジャンプ終わり(失敗終わり)
		if( IsAreaArrive(436*8,75*8,3*8,63*8) )
		{
			CCockpit::GetInstance()->SetMessage(3000);
		}
	}

	//-------------------------------------------------------

	if( bMessage )
	{
		char **pMsg = &TutorialCommand01[0];
		Sint32 x=164,y=110,ii=0;
		Sint32 sAlp = 255-CFadeManager::GetInstance()->GetFadeAlpha();
		Sint32 yy = 0;
		while( pMsg[ii] != NULL )
		{
			if( pMsg[ii] == "" )
			{
				yy += 8;
			}
			else
			{
				viiSub::MenuPrintf( x,y+12+yy,ARGB(sAlp,0xff,0xff,0xff) , pMsg[ii] );
				yy += 12;
			}

			ii++;
		}
		return;
	}

	//-------------------------------------------------------

	if( IsAreaArrive(71*8,58*8,12*8,16*8) )
	{
		//そこはブーストを使え！
		CCockpit::GetInstance()->SetMessage(1050);
	}

	if( IsAreaArrive(115*8,71*8,18*8,11*8) )
	{
		//落ちても大丈夫だ
		CCockpit::GetInstance()->SetMessage(1070);
	}

	if( IsAreaArrive(144*8,28*8,6*8,10*8) )
	{
		//軟着陸開始
		CCockpit::GetInstance()->SetMessage(1100);
	}

	if( IsAreaArrive(241*8,103*8,43*8,8*8) )
	{
		//第１次：奈落いき
		CCockpit::GetInstance()->SetMessage(1230);
	}

	if( IsAreaArrive(337*8,99*8,64*8,7*8) )
	{
		//第２次：奈落いき
		CCockpit::GetInstance()->SetMessage(1330);
	}

	if( IsAreaArrive(408*8,56*8,9*8,7*8) )
	{
		//上級者コース入りぐち
		CCockpit::GetInstance()->SetMessage(1400);
	}


	//ステージクリア
	if( IsAreaArrive(4100,559,WINDOW_W,WINDOW_H) )
	{
		CCockpit::GetInstance()->SetMessage(3200);
		ChangeArea( enAreaSeq05 );
	}

	BgCommon();
	viiSub::LimitMoveVH();

}


void CStageTutorial::Area04()
{
	//-----------------------------------------------------
	//エリア４
	//-----------------------------------------------------

	BgCommon();
	viiSub::LimitMoveVH();

}


void CStageTutorial::Area05()
{
	if( IsZoneSeq( 0 ) )
	{
		CCockpit::GetInstance()->SetStageClear();
		SetZoneSeq( 10 );
	}
	else if( IsZoneSeq( 10 ) )
	{
		if( CCockpit::GetInstance()->IsClearOver() )
		{
			CCockpit::GetInstance()->DestroyStageClear();
			SetFadeOut(2);
			SetZoneSeq( 20 );
		}
	}
	else if( IsZoneSeq( 20 ) )
	{
		//フェード終了待ち
		if( IsFadeEnd() )
		{
			pGame->stage_clear(enClearSeqStageClear);
			ChangeArea( GAME_NONE );	//ステージ終了
		}
	}

	BgCommon();
	viiSub::LimitMoveVH();

}


void CStageTutorial::AreaCommon()
{
	//-----------------------------------------------------
	//エリア：共通
	//-----------------------------------------------------


}


//---------------------------------------------------------------------------
//ＶＭＰから敵の生成
//---------------------------------------------------------------------------


void CStageTutorial::SetEnemy()
{

	for(Sint32 y=0; y<pGame->pBg->getHeight()/8; y++)
	{
		for(Sint32 x=0; x<pGame->pBg->getWidth()/8; x++)
		{
			Sint32 sIndex = pGame->pBg->get_vmpdat(x,y);
			sIndex = EnemyIndex[sIndex];
			switch(sIndex){
			case enCEneH0410Wall:
				new CEneH0410Wall2(x*800,y*800);
				break;

			case enCEneH0101HeavyTank:		//味方重戦車
				break;

			case enCEneH0102NightSpider:	//BOSS
				break;

			default:
				MakeHoundsEnemy(x*800,y*800,sIndex);
				break;
			}

		}
	}

}


void CStageTutorial::BgCommon(Sint32 x,Sint32 y)
{
	//----------------------------------------------------
	//背景共通
	//----------------------------------------------------
	BgScrollAdjust();

	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );

	StLayerScroll *pScr;

	for(Sint32 ii=enBgTypeBack; ii<=enBgTypeFrnt; ii++)
	{
		pScr = &LayerScroll[ii];
		viiSub::SetBgLayer( ii , pScr->ox+(sx*pScr->x)/100 , pScr->oy+(sy*pScr->y)/100 );
	}

}

