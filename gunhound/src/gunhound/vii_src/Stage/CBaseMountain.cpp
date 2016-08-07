//----------------------------------------------------------------------------
//
//ハウンドステージ ３（マウンテン）
//
//----------------------------------------------------------------------------

#include <gunvalken.h>
#include <gxLib/util/gxImage/CGXImage.h>
#include "CStageBase.h"
//---------------------------------------------------------------------------
#include "../effect/EffectBase.h"
#include "../enemy/enemy.h"

extern gxSprite StSprCloud[];

enum {
	enHoundOpeStart,
	enHoundOpeBatsuSyuGou,
	enHoundOpeBatsuEscape,
	enHoundOpeBatsuArrive,
	enHoundOpeBatsuCrossFormation,
	enHoundOpeBatsuHomingEliza,
};

enum
{
	enAreaSeq01,
	enAreaSeq02,
	enAreaSeq03,
	enAreaSeq04,
	enAreaSeq05,
	enAreaSeqTest1,
	enAreaSeqTest2,
	enAreaSeqMax,
};


enum {
	enGroundScrY = 695,
	enScrollSpdX = 333,
	enScrollSpdY = 500,
	enOffsetLeftScreen = 96,
	enWaraWaraMax = 12,

	enRescuePosX  = 1664*100,	//エンジニア
	enRescuePosY  = 3870*100,
	enRescueheliX = 1210*100,
	enRescueDoor  = 1500,	//場所の調整が必要なので１００かけない


	enRescuePointA = 2200,
	enRescuePointB = (enRescueheliX)-12*100,
};

void CBaseMountain::Init()
{
	//---------------------------------------------------------------------------
	//初期化
	//---------------------------------------------------------------------------
//	Sint32 col1 =  0xF0408080;
//	Sint32 col2 =  0xF0808040;
//	pGame->pBg->SetSkyColor(50,col1,col2);

	m_sScrollYMin = 0;
	m_sScrollYMax = 10000*100;

	m_TransPorterPos.x = 0;
	m_TransPorterPos.y = 0;

	while( !CGXImage::GetInstance()->IsLoadFinish() )
	{
		//ロード待ち
	}

	switch(GetArea( )){
	case enAreaSeq01:		//潜入
		if( m_bFirst )
		{
			if( g_StGameInfo.m_bDebugRoot )
			{
				CGXImage::GetInstance()->Load( "asset/gh/GXI/MOUNTA.GXI" );
			}
		}
		CCockpit::GetInstance()->SetMissionSec(  60*5 );
		SetDRQStage( "ScenMountA.txt" );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		viiMus::PlayBGM(enSoundBgm1,enMusicBgmBossDeluxe);
		break;

	case enAreaSeq02:	//到着
		CGXImage::GetInstance()->Load( "asset/gh/GXI/MOUNTB.GXI" );
		SetDRQStage( "ScenMountB.txt" );
		//g_pActManager->Clear(enActionPrioDefault);
		CActManager::GetInstance()->Clear(enActionPrioDefault);
		m_pHound[enHoundSlave] = NULL;
		m_pHound[enHoundMaster] = NULL;
		m_pYusouHeli = NULL;
		m_pGunpelly = NULL;
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		break;

	case enAreaSeq03:	//救出
		if( m_bFirst )
		{
			CGXImage::GetInstance()->Load( "asset/gh/GXI/MOUNTB.GXI" );
			SetDRQStage( "ScenMountB.txt" );
			CCockpit::GetInstance()->SetMissionSec(  60*5 );
			SetFadeIn();
		}
		m_pYusouHeli = NULL;
		//g_pActManager->Clear(enActionPrioDefault);
		CActManager::GetInstance()->Clear(enActionPrioDefault);
		CCockpit::GetInstance()->SetMissionSec(  60*3 );
		SetTargetPlayerPosX( 100*100 );
		SetTargetPlayerPosY( 4200*100 );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		CCockpit::GetInstance()->SetMissionSec(  60*5 );
		break;


	//「ボス」
	case enAreaSeq04:
		if( m_bFirst )
		{
			CCockpit::GetInstance()->SetMissionSec(  60*3 );
		}
		m_pYusouHeli = NULL;
		//g_pActManager->Clear(enActionPrioDefault);
		CActManager::GetInstance()->Clear(enActionPrioDefault);
		CGXImage::GetInstance()->Load( "asset/gh/GXI/MOUNTC.GXI" );
		SetDRQStage( "ScenMountC.txt" );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		ChangeArea( enAreaSeq04 );
		break;

	case enAreaSeq05:
		break;

	//--------------------------------------
	//チェック用
	//--------------------------------------
	case enAreaSeqTest1:
		CGXImage::GetInstance()->Load( "asset/gh/GXI/MOUNTA.GXI" );
		SetDRQStage( "ScenMountA.txt" );
		m_Pos.x = 0;
		m_Pos.y = 0;
		break;

	case enAreaSeqTest2:
		CGXImage::GetInstance()->Load( "asset/gh/GXI/MOUNTB.GXI" );
		SetDRQStage( "ScenMountB.txt" );
		m_Pos.x = 0;
		m_Pos.y = 5466*100;
		break;

	default:
		break;
	}

	InitEnemies();

	SetGamePlay( gxTrue );

	m_bFirst = gxFalse;
}


void CBaseMountain::End()
{
	//---------------------------------------------------------------------------
	//終了処理
	//---------------------------------------------------------------------------


}


void CBaseMountain::InitEnemies()
{
	//---------------------------------------------------------------------------
	//敵の初期化
	//---------------------------------------------------------------------------

	SetEnemy();

}


void CBaseMountain::Action()
{
	//---------------------------------------------------------------------------
	//毎フレームのアクション
	//---------------------------------------------------------------------------
	//エリア外判定を出さない
	CCockpit::GetInstance()->SetOperationRange(0*100,16000*100);

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

	case enAreaSeqTest1:
	case enAreaSeqTest2:
		AreaTest();
		break;
	}

//	viiDbg::printf(100,0,"ZoneTime= %d",m_sZoneTimer);
//	viiDbg::printf(100,16,"Area    = %d",GetArea( ) );

}


void CBaseMountain::Area01()
{
	//---------------------------------------------------------------------------
	//エリア１　入り口
	//---------------------------------------------------------------------------

	Sint32 col1 =  0xF0404040;
	Sint32 col2 =  0xF0202020;

	pGame->pBg->SetSkyColor(50,col1,col2);
	m_sScrollYMin = 0;
	if( m_pGrazzi )
	{
		HoveredWaraWara();
	}

	if( IsZoneSeq( 0 ) )
	{
		//-------------------------------------
		//初期化
		//-------------------------------------
		m_sScrollMin = 120*100;
//		m_sScrollYMin = 270*100;
		m_sScrollYMin = 0;

		m_sScrollMax = 1500*100;
		SetFadeIn();

		m_pRainEffect = new CEffScene0A02(0,0,PRIO_SYSTEM , -1 );
		m_pRainEffect->SetRain();

		m_pGrazzi = new CEneH0204Grazzi( 1600*100 , 500*100 );
		//m_pGrazzi->SetMoveArea( 3720*100 , 4000*100 );

		CCockpit::GetInstance()->SetMissionSec( 60*10 );

		SetFireEffect();

		ControlHound(enHoundOpeStart);
		ResetZoneTimer();
		SetZoneSeq( 5 );

	}
	else if( IsZoneSeq( 5 ) )
	{
		if( m_sZoneTimer == 120 )
		{
			CCockpit::GetInstance()->SetMessage(3000);	//全機散会！
			SetZoneSeq( 10 );
		}
	}
	else if( IsZoneSeq( 10 ) )
	{
		if( m_pGrazzi && m_pGrazzi->IsDead() )
		{
			m_pGrazzi->Clear();
			m_pGrazzi = NULL;
			m_sScrollMax = 3165*100;

			//輸送ヘリ発進
			m_pYusouHeli = new CEneH0007YusouHeli( 0*100 , 348*100 );
			m_pYusouHeli->SetTargetPos( (enRescuePointA)*100 );

			CCockpit::GetInstance()->SetMessage(3100);	//救助班、まもなく到着します。
			SetZoneSeq( 20 );
		}


	}
	else if( IsZoneSeq( 20 ) )
	{
		//-----------------------------------------
		//人民作成
		//-----------------------------------------
		Sint32 sx,sy;
		viiSub::GetScroll_s( sx,sy );
		if( sx >= 3100 )
		{
			//画面端まで来た
			CCockpit::GetInstance()->SetMessage(3200);	//全員退避しろーーっ
			ResetZoneTimer();
			SetZoneSeq( 100 );

		}
	}
	else if( IsZoneSeq( 100 ) )
	{
		//-----------------------------------------
		//人民撤退
		//-----------------------------------------
		m_sScrollMin = 2000*100;

		switch( m_sZoneTimer ){
		case 25*1:
		case 25*2:
		case 25*3:
		case 25*4:
		case 25*5:
		case 25*6:
		case 25*7:
		case 25*8:
			{
				for(Sint32 ii=0;ii<2;ii++)
				{
					CSoldierBase *p;
					p = new CSoldierBase( );
					if( ii%2 == 0)
					{
						//右の人
						p->SetPosition( 0 , 3564*100, 479*100 , NULL);
					}else{
						//左の人
						p->SetPosition( 0 , 436*100, 479*100 , NULL);
					}
					p->ChangeMoveType( enMoveTypeRideOn );
					p->SetTargetPosition( (enRescuePointA-12)*100 );

					if( viiSub::Rand()%2 == 0)
					{
						//男
						p->SetSpeed(85);
						p->SetCivilian(1);
					}
					else
					{
						//女
						p->SetSpeed(80);
						p->SetCivilian(2);
					}
				}
			}
			break;
		case 60*9:
			ResetZoneTimer();
			SetZoneSeq( 200 );
			break;
		}
	}
	else if( IsZoneSeq( 200 ) )
	{
		if( m_sZoneTimer == 60 )
		{
			m_pGunpelly = new CEneH0301Gunpelly(4000*100 , 370*100 );
		}

		if( !m_pYusouHeli->IsLanding() )
		{
			//強制着陸
			m_pYusouHeli->SetLanding();
		}

		if( m_pYusouHeli->GetSoldierLeader() )
		{
//			m_pYusouHeli->GetSoldierLeader()->CommandMember( enMoveTypeLeading );
//			m_pYusouHeli->GetSoldierLeader()->ChangeMoveType( enMoveTypeLeading );
			if( m_pGunpelly )
			{
				ControlHound( enHoundOpeBatsuSyuGou );
				ResetZoneTimer();
				SetZoneSeq( 300 );
			}
		}
	}
	else if( IsZoneSeq( 300 ) )
	{
		if( m_pGunpelly && m_pGunpelly->m_Pos.x <= 2000*100 )
		{
			m_bForceGameOver = gxTrue;
		}

		if( m_sZoneTimer == 300 )
		{
			Sint32 sx;
			sx = m_pYusouHeli->GetSoldierLeader()->m_Pos.x;
			if( m_pYusouHeli->GetSoldierLeader()->GetMember( 0 ) )
			{
				m_pYusouHeli->GetSoldierLeader()->GetMember( 0 )->SetTargetPosition( sx - 120*100 );
			}
			if( m_pYusouHeli->GetSoldierLeader()->GetMember( 1 ) )
			{
				m_pYusouHeli->GetSoldierLeader()->GetMember( 1 )->SetTargetPosition( sx + 120*100 );
			}
			if( m_pYusouHeli->GetSoldierLeader()->GetMember( 2 ) )
			{
				m_pYusouHeli->GetSoldierLeader()->GetMember( 2 )->SetTargetPosition( sx+240*100 );
			}
		}
		if( m_sZoneTimer == 600 )
		{
			m_pYusouHeli->GetSoldierLeader()->GetMember( 0 )->ChangeMoveType(enMoveTypeLeading);
			m_pYusouHeli->GetSoldierLeader()->GetMember( 2 )->ChangeMoveType(enMoveTypeLeading);
		}
		if( m_sZoneTimer == 1900 )
		{
			m_pYusouHeli->GetSoldierLeader()->SetTargetPosition( (enRescuePointA-16)*100 );
			m_pYusouHeli->GetSoldierLeader()->CommandMember( enMoveTypeRideOn );
			m_pYusouHeli->GetSoldierLeader()->ChangeMoveType( enMoveTypeLeading );
		}
		else if( m_sZoneTimer == 2000 )
		{
			m_pYusouHeli->GetSoldierLeader()->ChangeMoveType( enMoveTypeRideOn );
		}
		else if( m_sZoneTimer == 2100 )
		{
			CCockpit::GetInstance()->SetMessage(3300);	//住民の避難完了！
			m_pYusouHeli->SettakeOff();
			ControlHound( enHoundOpeBatsuEscape );
			SetZoneSeq( 1000 );
		}
	}
	else if( IsZoneSeq( 1000 ) )
	{
		//---------------------------------
		//地下に降りるまで
		//---------------------------------

		if( m_pHound[enHoundSlave]->m_Pos.x >= pGame->GetHound()->m_Pos.x +128*100 )
		{
			SetZoneSeq( 1100 );
		}
	}
	else if( IsZoneSeq( 1100 ) )
	{
		m_sScrollMax = 4200*100;

		if( pGame->GetHound()->m_Pos.x < 3500*100 )
		{
			CCockpit::GetInstance()->SetArrow( 0 );
		}

		CCockpit::GetInstance()->SetMessage(3400);	//隊長！！私はバツを連れ戻します

		m_sScrollYMin = 0;
		if( IsAreaArrive(4650, 200, 64, 400) )
		{
			//エリアクリアへ(→ルート)
			SetZoneSeq( 8500 );
		}
		else if( IsAreaArrive(3600, 660, 480, 32) )
		{
			//下ルート
			m_pRainEffect->SetRainPrio(PRIO_BG2-1);
			SetZoneSeq( 2000 );
		}

	}
	else if( IsZoneSeq( 2000 ) )
	{
		//------------e---------------------
		//地下に降りた
		//---------------------------------
		m_sScrollMax = 3700*100;
		m_sScrollYMin = 520*100;
		m_sScrollYMax = 600*100;
		pGame->SetDetectUnderLine(gxFalse);

		SetZoneSeq( 8000 );

	}
	else if( IsZoneSeq( 8000 ) )
	{
		//-----------------------------------------
		//エリアクリア待ち
		//-----------------------------------------
		m_sScrollMax = 4500*100;

		if( pGame->GetHound()->m_Pos.x < 4000*100 )
		{
			CCockpit::GetInstance()->SetArrow( 0 );
		}

		if( IsAreaArrive(4942, 560, 32, 312) )
		{
			SetZoneSeq( 8500 );
		}
	}
	else if( IsZoneSeq( 8500 ) )
	{
		new CEntranceControl( CEntranceControl::enExit , DIR_RIGHT );
		viiMus::StopBGM( enSoundBgm1 , 90 );
		SetGamePlay( gxFalse );
		SetFadeOut();
		SetZoneSeq( 9000 );
	}
	else if( IsZoneSeq( 9000 ) )
	{
		//-----------------------------------------
		//フェード待ち
		//-----------------------------------------

		if( IsFadeEnd() )
		{
			ResetZoneTimer();
			CCockpit::GetInstance()->ResetMessageWindow();
			SetZoneSeq( 9100 );
			return;
		}
	}
	else if( IsZoneSeq( 9100 ) )
	{
		if( !viiMus::IsPlayBGM( enSoundBgm1 ) )
		{
			CCockpit::GetInstance()->SetSplash( gxTrue );
			viiMus::PlaySound( enSoundEyeCatch );
			ResetZoneTimer();
			SetZoneSeq( 9200 );
			return;
		}
	}
	else if( IsZoneSeq( 9200 ) )
	{
		CCockpit::GetInstance()->SetSplash( gxTrue );
		if( m_sZoneTimer >= 30 )
		{
			ChangeArea( enAreaSeq02 );
			Init();
			return;
		}
	}
	if( m_pGunpelly && m_pGunpelly->IsDead())
	{
		m_pGunpelly->Clear();
		m_pGunpelly = NULL;
	}

	BgCommon();
	viiSub::LimitMoveVH();

}


void CBaseMountain::Area02()
{
	//-----------------------------------------------------
	//エリア２:山岳地帯
	//-----------------------------------------------------
	pGame->SetDetectUnderLine(gxTrue);

	if( IsZoneSeq( 0 ) )
	{
		//初期化
		SetGamePlay( gxFalse );
		m_sScrollMin  = 0*100;
		m_sScrollMax  = 1894*100;

		SetFadeIn();

		CCockpit::GetInstance()->AddMissionSec( 60*10 );
		viiSub::SetScroll_l( 0*100, 5479*100 );
		pGame->GetHound()->SetMode( CGunHound::enHoundModeNone );
		pGame->GetHound()->SetPadButton(0);

		if( !viiMus::IsPlayBGM( enSoundEyeCatch ) )
		{
			CCockpit::GetInstance()->SetSplash( gxFalse );
			m_bBgm1Play  = gxFalse;
			m_bGakeShita = gxFalse;

			SetTargetPlayerPosX( -128*100 );
			SetTargetPlayerPosY( 5700*100 );

			new CEntranceControl( CEntranceControl::enEntrance , DIR_RIGHT );

			if( g_StGameInfo.m_sGameMode == StGameInfo::enGameModeMission )
			{
				new CItemBox( 165*100, 5236*100 , 0 );
			}
			ResetZoneTimer();
			SetZoneSeq( 5 );
		}
		BgCommon();

		return;
	}
	else if( IsZoneSeq( 5 ) )
	{
		if( m_sZoneTimer >= 10 )
		{
			viiMus::PlayBGM(enSoundBgm1,enMusicBgmStageA);
			pGame->GetHound()->SetMode( CGunHound::enHoundModeNormal );
			CCockpit::GetInstance()->SetMissionSec( 60*10 );
			SetGamePlay( gxTrue );
			SetZoneSeq( 10 );
		}

		BgCommon();
		return;
	}
	else if( IsZoneSeq( 10 ) )
	{
		//-------------------------------
		//１段目
		//-------------------------------


		CCockpit::GetInstance()->SetMessage(3500);	//ニュートロンポリマー採掘所に潜入。


//		m_sScrollMax = 1886*100;
		m_sScrollYMin = 5080*100;
//		m_sScrollYMax = 12000*100;

		if( IsAreaArrive(1080,5000,64,300) )
		{
			m_sScrollYMin = 0*100;
			m_sScrollYMax = 5120*100;

			SetZoneSeq( 20 );
		}

	}
	else if( IsZoneSeq( 20 ) )
	{
		//-------------------------------
		//２段目の踊り場
		//-------------------------------
		m_sScrollMin  = 1077*100;

		if( IsAreaArrive(1980,5032,480,32) )
		{
			SetZoneSeq( 30 );
		}

	}
	else if( IsZoneSeq( 30 ) )
	{
		//-------------------------------
		//２段目の踊り場
		//-------------------------------
		m_sScrollMin  = 0*100;

		if( IsAreaArrive(1980,5032,480,32) )
		{
			//上段へ
			m_sScrollYMax = 5120*100;
		}

		if( IsAreaArrive(1980,4500,48,500) )
		{
			//下段へ
			CCockpit::GetInstance()->SetMessage(3600);	//こっちの防衛施設は
			m_sScrollYMax = 4766*100;
		}

		if( IsAreaArrive(1080,5000,64,300) )
		{
			m_sScrollYMin = 0*100;
			m_sScrollYMax = 12000*100;
		}
		if( IsAreaArrive(450,4550,64,300) )
		{
			m_sVorZoyCnt = 0;
			SetZoneSeq( 520 );
		}

	}
	else if( IsZoneSeq( 520 ) )
	{
		//ワラワラ第１回
		m_sScrollMax  = 0*100;
		m_sScrollYMin = 4746*100;
		m_sScrollYMax = 4900*100;
		CCockpit::GetInstance()->SetMessage(3700);	//待ち伏せ！

		if( VorzoyWaraWara() )
		{
			m_sVorZoyCnt = 0;
			SetZoneSeq( 525 );
		}
		m_bGakeShita = gxTrue;

	}
	else if( IsZoneSeq( 525 ) )
	{
		//ワラワラ第２回
		if( VorzoyWaraWara() )
		{
			ResetZoneTimer();
			SetZoneSeq( 530 );
		}

		m_bGakeShita = gxTrue;
	}
	else if( IsZoneSeq( 530 ) )
	{
		m_bGakeShita = gxTrue;

		m_sScrollMax = 50*100;//3200*100;
		m_sScrollYMin = 0*100;
		m_sScrollYMax = 4900*100;

		CCockpit::GetInstance()->SetArrow(-90);

//		if( m_sZoneTimer == 120 )
//		{
//		}

		if( IsAreaArrive(0,4612,146,132) )
		{
			CCockpit::GetInstance()->SetMessage(3800);	//こちら司令室。パープルツー
		}
		if( IsAreaArrive(200,4264,80,64) )
		{
			SetZoneSeq( 540 );
		}
	}
	else if( IsZoneSeq( 540 ) )
	{
		ChangeArea( enAreaSeq03 );
	}

	//ワラワラ中のがけ下判定

	gxBool bMiss = gxFalse;
	if ( m_bGakeShita )
	{
		if( IsAreaArrive(0,5128,960*2,48) )
		{
			bMiss = gxTrue;
			m_bForceGameOver = gxTrue;
			//pGame->GetHound()->SetGameOver();
		}
	}

	BgCommon();
	viiSub::LimitMoveOnlyV();

}


void CBaseMountain::Area03()
{
	//-----------------------------------------------------
	//エリア３：救出
	//-----------------------------------------------------
	Sint32 col2 =  0xF0F04040;
	Sint32 col1 =  0xF0A0A020;
	Sint32 sRescueScrollX = 900;
	Sint32 sRescuePos = 1273;//enRescueheliX/100;

	pGame->pBg->SetSkyColor(80,col1,col2);

	if( pGame->GetHound()->m_Pos.y >= 4600*100 )
	{
		m_sScrollMax = 80*100;
	}
	else
	{
		m_sScrollMax = 1800*100;
	}

	if( IsZoneSeq( 0 ) )
	{
		m_sScrollMax = 3200*100;
		m_sScrollYMin = 0*100;
		m_sScrollYMax = 4900*100;

		Sint32 sHeliX = -1600;

#ifdef _VII_DEBUG_
		sHeliX = 1000;
#endif
		m_pYusouHeli = new CEneH0007YusouHeli(sHeliX*100 , 3720*100 );
		m_pYusouHeli->SetTargetPos(enRescueheliX);

		m_sRescueSeq = 0;

		CCockpit::GetInstance()->AddMissionSec( 5*10 );

		ResetZoneTimer();
		SetZoneSeq( 10 );
	}
	else if( IsZoneSeq( 10 ) )
	{
		//扉前に集合
		if( m_sZoneTimer == 20 )
		{
			if( !viiMus::IsPlayBGM( enSoundBgm1 ) )
			{
				viiMus::PlayBGM(enSoundBgm1,enMusicBgmStageA);
			}
		}

		CCockpit::GetInstance()->SetMessage(3900);	//敵、航空部隊を確認！！

		ControlHound( enHoundOpeBatsuArrive );

		if( m_pYusouHeli->GetSoldierLeader() )
		{
			m_sScrollMin = sRescueScrollX*100;
			m_sScrollMax = (m_sScrollMin+200*100);//(sRescueScrollX+200)*100;
			m_sRescueSeq ++;
			//if (m_sRescueSeq > 180 )
			{
				m_pYusouHeli->GetSoldierLeader()->SetTargetPosition( (enRescueDoor+50)*100 );
				m_pYusouHeli->GetSoldierLeader()->CommandMember( );
				m_pYusouHeli->GetSoldierLeader()->SetTargetPosition( (enRescueDoor)*100 );
				m_sRescueSeq = 0;
				SetZoneSeq( 20 );
			}
		}
	}
	else if( IsZoneSeq( 20 ) )
	{
		//爆弾セット
			m_sScrollMin = sRescueScrollX*100;
			m_sScrollMax = (m_sScrollMin+200*100);//(sRescueScrollX+200)*100;
		m_sRescueSeq ++;
		if( m_sRescueSeq > 420 )
		{
			m_bWaraWaraCommanch = gxTrue;
			CCockpit::GetInstance()->SetMessage(3850);			//無事だったようね。
			m_stTargetPos = m_pYusouHeli->m_Pos;
			m_pYusouHeli->GetSoldierLeader()->CommandMember( enMoveTypeBombSet );
			SetZoneSeq( 30 );
		}
	}
	else if( IsZoneSeq( 30 ) )
	{
		//ブリーチクリア
			m_sScrollMin = sRescueScrollX*100;
			m_sScrollMax = (m_sScrollMin+200*100);//(sRescueScrollX+200)*100;

		if( m_pYusouHeli->GetSoldierLeader()->GetMember( 0 )->GetSeq()==40 )
		{
			m_pYusouHeli->GetSoldierLeader()->GetMember( 1 )->SetSeq( 10 );
			m_pYusouHeli->GetSoldierLeader()->ChangeMoveType( enMoveTypeLeading );
			m_sRescueSeq = 0;
			m_sRescueCnt = 0;
			SetZoneSeq( 50 );
		}

	}
	else if( IsZoneSeq( 50 ) )
	{
		//エンジニア救出(戦闘ヘリを護れ)
		m_sScrollMin = sRescueScrollX*100;
		m_sScrollMax = (m_sScrollMin+200*100);//(sRescueScrollX+200)*100;
		m_sRescueSeq ++;

		if( m_sRescueSeq%150 == 0)
		{
			if( m_pYusouHeli && m_pYusouHeli->IsDead() )
			{
				CCockpit::GetInstance()->SetMessage(4100);	//「ダメだ、救助ヘリを放棄する！！」
			}

			if( m_sRescueCnt >= 12 )
			{
				m_sZoneTimer = 0;
				SetZoneSeq( 60 );
			}
			else
			{
				//エンジニア作成
				CSoldierBase *p;
				p = new CSoldierBase( );
				p->SetPosition( 0 , enRescuePosX, enRescuePosY , NULL);
				p->ChangeMoveType( enMoveTypeRideOn );
				p->SetTargetPosition( enRescuePointB );
				Sint32 sCiviType = 1+viiSub::Rand()%2;
				p->SetCivilian( sCiviType );
				if( sCiviType == 1)
				{
					p->SetSpeed(60);
				}
				else 
				{
					p->SetSpeed(80);
				}

				m_sRescueCnt ++;
			}
		}
	}
	else if( IsZoneSeq( 60 ) )
	{
		//脱出開始
//		CCockpit::GetInstance()->SetMessage(4000);	//「救助ヘリ、エンジン部に被弾！」
			m_sScrollMin = sRescueScrollX*100;
			m_sScrollMax = (m_sScrollMin+200*100);//(sRescueScrollX+200)*100;

		if( m_sZoneTimer == 320 )
		{
			m_pYusouHeli->GetSoldierLeader()->SetTargetPosition( enRescuePointB );
			m_pYusouHeli->GetSoldierLeader()->CommandMember( enMoveTypeRideOn );
		}
		else if( m_sZoneTimer == 640 )
		{
			m_pYusouHeli->GetSoldierLeader()->ChangeMoveType( enMoveTypeRideOn );
		}
		else if( m_sZoneTimer == 1240 )
		{
			CCockpit::GetInstance()->SetMessage(4200);	//全員救助完了、輸送ヘリ
			m_pYusouHeli->SettakeOff();
			SetZoneSeq( 100 );
		}
	}
	else if( IsZoneSeq( 100 ) )
	{
		if( !m_bWaraWaraCommanch )
		{
			CCockpit::GetInstance()->SetArrow( 0 );

			ControlHound(enHoundOpeBatsuHomingEliza);

			if( IsAreaArrive(2250,2000,50,2050) )
			{
				new CEntranceControl( CEntranceControl::enExit , DIR_RIGHT );
				SetFadeOut();
				viiMus::StopBGM( enSoundBgm1 , 180 );
				SetGamePlay( gxFalse );
				SetZoneSeq( 900 );
			}
		}
	}
	else if( IsZoneSeq( 900 ) )
	{
		//フェード待ち
		if( IsFadeEnd() )
		{
			if( !viiMus::IsPlayBGM( enSoundBgm1 ) )
			{
				CCockpit::GetInstance()->ResetMessageWindow();
				ChangeArea( enAreaSeq04 );
				Init();
			}
		}
	}

	if( m_bWaraWaraCommanch )
	{
		ControlHound( enHoundOpeBatsuCrossFormation );
		WaraWaraCommanch();
	}

	if( IsAreaArrive(0,5150,800,64) )
	{
		//pGame->GetHound()->SetGameOver();
		m_bForceGameOver = gxTrue;
	}

	BgCommon();
	viiSub::LimitMoveOnlyV();

}


void CBaseMountain::Area04()
{
	//-----------------------------------------------------
	//エリア４：ボス戦
	//-----------------------------------------------------
	m_sScrollMax = 9600*100;

	Sint32 col2 =  0xF0A040C0;
	Sint32 col1 =  0xF0A08040;
	pGame->pBg->SetSkyColor(80,col1,col2);

	if( IsZoneSeq( 0 ) )
	{
		//初期化
		pGame->GetHound()->Reset();
		viiMus::StopBGM( enSoundBgm1,90 );
		SetFadeIn();
		if( m_pBoss == NULL )
		{
			m_pBoss = new CEneH0303Valkylie(63800,28000);
		}

		SetGamePlay( gxTrue );
		CCockpit::GetInstance()->AddMissionSec( 3*10 );
		ResetZoneTimer();
		SetZoneSeq( 10 );

	}
	else if( IsZoneSeq( 10 ) )
	{
		if( m_sZoneTimer == 10 )
		{
			viiMus::PlayBGM( enSoundBgm2,enMusicBgmBossSimple);
		}
		if( m_sZoneTimer == 120 )
		{
			CCockpit::GetInstance()->SetMessage(4300);	//やった！あのやろう、こっちに
		}

		if( m_pBoss && m_pBoss->IsDead() )
		{
			m_pBoss->Clear();
			m_pBoss = NULL;
			ChangeArea( enAreaSeq05 );
		}

	}

	GakeKudari();


}


void CBaseMountain::Area05()
{
	//-----------------------------------------------------
	//エリア５：ボス戦終了
	//-----------------------------------------------------

	if( IsZoneSeq( 0 ) )
	{
		//クリア
		CCockpit::GetInstance()->SetMessage(4400);
		SetGamePlay( gxFalse );
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

	GakeKudari();

}


void CBaseMountain::AreaCommon()
{
	//-----------------------------------------------------
	//エリア：共通
	//-----------------------------------------------------

	//--------------------------------------------
	//輸送ヘリやられた
	//--------------------------------------------
	if( m_pYusouHeli && m_pYusouHeli->IsDamage() )
	{
		if( !CCockpit::GetInstance()->IsMessageNow() )
		{
			CCockpit::GetInstance()->SetMessage(4000);	//救助ヘリ、エンジン部に被弾！
		}
	}

	if( m_pYusouHeli && m_pYusouHeli->IsDead() ) 
	{
		CCockpit::GetInstance()->SetMessage(4100);			//ダメだ、救助ヘリを放棄する！！
		CCockpit::GetInstance()->SetMessageEnable(gxFalse);	//以降のメッセージを禁止する
		m_bForceGameOver = gxTrue;
	}

	if( m_bForceGameOver )
	{
		if( !CCockpit::GetInstance()->IsMessageNow() )
		{

			m_sGameOverWait ++;

			if( m_sGameOverWait == 60 )
			{
				if( m_pYusouHeli )
				{
					//m_pYusouHeli->Clear();
					//m_pYusouHeli = NULL;
				}
				pGame->GetHound()->SetGameOver();
			}
		}
	}

	//輸送ヘリやられた

}

void CBaseMountain::Draw()
{
	//-------------------------------------
	//空
	//-------------------------------------

/*
	Sint32 col1 =  0xF0F08020;
	Sint32 col2 =  0xF0208040;

	pGame->pBg->SetSkyColor( 80 , col1 , col2 );
*/



	//-------------------------------------
	//雲
	//-------------------------------------

	viiDraw::Spr_NoScrl( &StSprCloud[ 0 ] , 0,0,PRIO_BG1-1 , ATR_DFLT , 0x80808080 );

}


void CBaseMountain::GakeKudari()
{
	//-----------------------------------------------------
	//崖スクロール
	//-----------------------------------------------------
	Sint32 sx,sy;

	BossBattle();

	viiSub::LimitMoveOnlyV();

	pGame->power_x = -enScrollSpdX;
	pGame->power_y = -enScrollSpdY;

	m_stScroll.x += enScrollSpdX;
	m_stScroll.y += enScrollSpdY;

	if( m_stScroll.y >= (480+240*2)*100 )
	{
		m_stScroll.y -= 240*2*100;
		m_stScroll.x -= 320*100;
	}

	if( m_sBackGroundScroll <= 100*100 )
	{
		m_sBackGroundScroll += 4;
	}

	sx = enOffsetLeftScreen+m_stScroll.x/100;
	sy = m_stScroll.y/100;

	pGame->pBg->offset_x = enOffsetLeftScreen-sx+200;
	pGame->pBg->offset_y = 212-sy;
	//-----------------------------------------------------

	pGame->pBg->set_disp( 0 , gxFalse );
	viiSub::SetBgLayer( enBgTypeBack ,0,m_sBackGroundScroll/100);
	viiSub::SetBgLayer( enBgTypeRear ,sx*LayerScroll[enBgTypeRear].x/100,sy+0*sy/100 );
	viiSub::SetBgLayer( enBgTypeFrnt ,sx*LayerScroll[enBgTypeFrnt].x/100,sy+0*sy/100 );

	gxBool bMiss = gxFalse;

	if( pGame->GetHound()->IsOutofScreen( 128*100 , 64*100 ) )
	{
		bMiss = gxTrue;
		pGame->GetHound()->SetGameOver();
	}

}


void CBaseMountain::BossBattle()
{
	//---------------------------------------------
	//崖すべりメイン
	//---------------------------------------------

	Sint32 x = GetTargetPlayer()->x;
	Sint32 y = GetTargetPlayer()->y;

	Sint32 num = pGame->pBg->get_kabetype(x/100 , y/100) ;//== 4)
	viiDbg::printf(32,WINDOW_H,"KABE=%d",num );

	if( num == 4 )
	{
		pGame->GetHound()->SetMode( CGunHound::enHoundModeGake );

		switch(viiSub::Rand()%3){
		case 0:
			new CEffGakeSmoke( CEffGakeSmoke::enSmokeTypeSlash,x+(-4+viiSub::Rand()%8)*100,y+(-4+viiSub::Rand()%8)*100,45,600);
			break;
		case 1:
			new CEffGakeSmoke( CEffGakeSmoke::enSmokeTypeWhite,x+(-4+viiSub::Rand()%8)*100,y+(-4+viiSub::Rand()%8)*100);
			break;
		case 2:
			new CEffGakeSmoke( CEffGakeSmoke::enSmokeTypeBrown,x+(-4+viiSub::Rand()%8)*100,y+(-4+viiSub::Rand()%8)*100 , 45 , 300);
			break;
		}
	}
	else
	{
		pGame->GetHound()->SetMode( CGunHound::enHoundModeNormal );
	}

}


//---------------------------------------------------------------------------
//ＶＭＰから敵の生成
//---------------------------------------------------------------------------


void CBaseMountain::SetEnemy()
{
	for(Sint32 y=0; y<pGame->pBg->getHeight()/8; y++)
	{
		for(Sint32 x=0; x<pGame->pBg->getWidth()/8; x++)
		{
			Sint32 sIndex = pGame->pBg->get_vmpdat(x,y);
			sIndex = EnemyIndex[sIndex];

			switch(sIndex){

			case enCEneH0101HeavyTank:	//味方重戦車
				break;

			case enCEneH0102NightSpider:	//BOSS
//				m_pCEneH0102NightSpider = new CEneH0102NightSpider(x*800, y*800);
				break;

			case enCEneH0108Hovered:
				new CEneH0108Hovered( x*800 , y*800 , gxTrue);
				break;

			case enCEneH0006UGTortika:
				{
					//プライオリティをBG2の上に設定した特別版
					CEneH0006UGTortika *p;
					p = new CEneH0006UGTortika(x*800,y*800);
					p->m_Pos.z = PRIO_BG2 + 1;
				}
				break;

			case enCEneH0006UGTortika2:
				{
					//プライオリティをBG2の上に設定した特別版
					CEneH0006UGTortika *p;
					p = new CEneH0006UGTortika(x*800,y*800,gxTrue);
					p->m_Pos.z = PRIO_BG2 + 1;
				}
				break;

			case enCEneH0204Grazzi:
				{
				m_pGrazzi = new CEneH0204Grazzi( x*800 , y*800 );
				m_pGrazzi->SetMoveArea( 3720*100 , 4000*100 );
				}
				break;
			default:
				MakeHoundsEnemy(x*800,y*800,sIndex);
				break;
			}

		}
	}

}


void CBaseMountain::BgCommon(Sint32 x,Sint32 y)
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
//	viiSub::SetBgLayer( enBgTypeRear ,pScr->ox+sx*pScr->x/100+stRiver.m_sInfinityScroll/100 , pScr->oy+sy*pScr->y/100 );

}

gxBool CBaseMountain::VorzoyWaraWara()
{
	//-------------------------------------------
	//ワラワラボルゾイスナイパーゲーム
	//-------------------------------------------
	static Sint32 sVorzotTbl[]={
		//ボルゾイ出現位置
		40*100,5071*100,
		40*100,4950*100,
		40*100,4830*100,
	};

	Sint32 sEndCnt  = 0;
	gxBool bEndFlag = gxFalse;

	for(Sint32 ii=0; ii<3; ii++)
	{
		if( m_pVorzoy[ii] == NULL )
		{
			if(m_sVorZoyCnt < enWaraWaraMax )
			{
				m_pVorzoy[ii] = new CEneH0003Vorzoy( sVorzotTbl[ii*2+0] , sVorzotTbl[ii*2+1] , gxTrue);
				m_pVorzoy[ii]->SetLogic(10);
				m_sVorZoyCnt ++;
			}
			else
			{
				bEndFlag = gxTrue;
			}
		}
		else
		{
			if(m_pVorzoy[ii]->IsDead())
			{
				m_pVorzoy[ii]->Clear();
				m_pVorzoy[ii] = NULL;
			}
		}

		if( m_pVorzoy[ii] == NULL )
		{
			//生成されていなければ終了カウントを増やす
			sEndCnt ++;
		}
	}

	if(sEndCnt==3 && bEndFlag )
	{
		return gxTrue;
	}

	return gxFalse;
}

gxBool CBaseMountain::HoveredWaraWara()
{
	//-----------------------------------------
	//ワラワラジオング
	//-----------------------------------------
	Sint32 x,y;

//	return gxFalse;
	if(m_sZoneTimer%48) return gxFalse;

	x = 4792*100;
	y = 500*100;

	for(Sint32 ii=0;ii<enHoverMax; ii++)
	{
		if( m_pHovered[ii] == NULL )
		{
			m_pHovered[ii] = new CEneH0108Hovered( x , y , gxTrue );
			return gxFalse;
		}
		if( m_pHovered[ii]->IsDead() )
		{
			m_pHovered[ii]->Clear();
			m_pHovered[ii] = NULL;
		}
	}

	return gxFalse;
}


void CBaseMountain::SetFireEffect()
{
	//-----------------------------------
	//ファイヤーエフェクト
	//-----------------------------------
	CEffFire *p;

	p = new CEffFire( 600*100,450*100 , PRIO_BG2-1 );
	p->SetScale(5.f);

	p = new CEffFire( 1861*100,450*100 , PRIO_BG2-1 );
	p->SetScale(5.f);

	p = new CEffFire( 3021*100,450*100 , PRIO_BG2-1 );
	p->SetScale(5.f);

	p = new CEffFire( 3444*100,492*100 , PRIO_BG2-1 );
	p->SetScale(5.f);

	new CEffectBlackExplosion( 240*100,490*100  , NULL );

	new CEffectBlackExplosion( 700*100,490*100  , NULL );
	new CEffectBlackExplosion( 1900*100,490*100 , NULL );
	new CEffectBlackExplosion( 3000*100,490*100 , NULL );
	new CEffectBlackExplosion( 3500*100,490*100 , NULL );
}

gxBool CBaseMountain::WaraWaraCommanch()
{
	Sint32 sCommanchCnt = 0;

	if( m_sZoneTimer%60 == 0)
	{
		//コマンチ作成

		for(Sint32 ii=0;ii<3;ii++)
		{
			if( m_pCommanch[ii] == NULL)
			{
				if( m_sWaraWaraCommanchCnt < enWaraWaraCommanchMax )
				{
					if( viiSub::Rand()%2==0)
					{
						m_pCommanch[ii] = new CEneH0002Commanch( enRescuePosX-1000*100 , enRescuePosY-200*100 ,gxTrue);
					}
					else
					{
						m_pCommanch[ii] = new CEneH0002Commanch( enRescuePosX -3600*100 - 1000*100, enRescuePosY-200*100 ,gxTrue);
					}
					m_pCommanch[ii]->SetTarget( &m_stTargetPos );
					m_sWaraWaraCommanchCnt ++;
				}
				else
				{
					sCommanchCnt ++;
				}
			}
			else
			{
				if( m_pCommanch[ii]->IsDead() )
				{
					m_pCommanch[ii]->Clear();
					m_pCommanch[ii] = NULL;
				}
			}
		}
	}

	if( m_sWaraWaraCommanchCnt >= enWaraWaraCommanchMax )
	{
		//全滅させた
		if( sCommanchCnt == 3 )
		{
			m_bWaraWaraCommanch = gxFalse;
			return gxTrue;
		}
	}

	return gxFalse;
}



void CBaseMountain::ControlHound(Sint32 sOperation )
{
	//-----------------------------------------
	//ハウンドのコントロール
	//-----------------------------------------

	switch(sOperation){
	case enHoundOpeStart:
		//-------------------------------
		//新規ハウンドをセット
		//-------------------------------
		m_pHound[enHoundMaster] = new CGunHound( 460*100,100*100 ,gxFalse);
		m_pHound[enHoundMaster]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundMaster]->SetLeader(gxTrue);
		m_pHound[enHoundMaster]->m_Pos.z = PRIO_ENEMY-4;
		m_pHound[enHoundMaster]->SetLogicPattern( 310 );
		m_pHound[enHoundMaster]->SetTarget( 2365*100 , 519*100 );
		m_pHound[enHoundMaster]->m_Pos.z = PRIO_BG2-1;


		m_pHound[enHoundSlave] = new CGunHound( 200*100,100*100 ,gxFalse);
		m_pHound[enHoundSlave]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundSlave ]->m_Pos.z = PRIO_PLAYER-1;
		m_pHound[enHoundSlave]->SetLogicPattern( 310 );
		m_pHound[enHoundSlave]->SetTarget( 2970*100 , 519*100 );
		m_pHound[enHoundSlave]->m_Pos.z = PRIO_BG2-1;

		pGame->GetHound()->m_Pos.y = 200*100;
		break;

	case enHoundOpeBatsuSyuGou:
		//-------------------------------
		//集合
		//-------------------------------
		m_pHound[enHoundMaster]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundMaster]->SetLogicPattern( 320 );
		m_pHound[enHoundMaster]->SetTarget( 2365*100 , 0*100 );
		m_pHound[enHoundSlave ]->m_Pos.z = PRIO_BG2-1;

		m_pHound[enHoundSlave]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundSlave]->SetLogicPattern( 320 );
		m_pHound[enHoundSlave]->SetTarget( 2138*100 , 0*100 );
			m_pHound[enHoundSlave ]->m_Pos.z = PRIO_BG2-1;
		break;

	case enHoundOpeBatsuEscape:
//		m_pHound[enHoundMaster]->SetMode( CGunHound::enHoundModeNPC );
//		m_pHound[enHoundMaster]->SetLogicPattern( 310 );
//		m_pHound[enHoundMaster]->SetTarget( 1600*100 , 0*100 );

		m_pHound[enHoundSlave]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundSlave]->SetLogicPattern( 330 );
		m_pHound[enHoundSlave]->SetTarget( 4650*100 , 0*100 );
		break;

	case enHoundOpeBatsuArrive:
		if( m_pHound[enHoundSlave] == NULL )
		{
			m_pHound[enHoundSlave] = new CGunHound( 2264*100,3840*100 ,gxFalse);
			m_pHound[enHoundSlave]->SetMode( CGunHound::enHoundModeNPC );
			m_pHound[enHoundSlave ]->m_Pos.z = PRIO_BG2+1;
			m_pHound[enHoundSlave]->SetLogicPattern( 320 );
			m_pHound[enHoundSlave]->SetTarget( 1535*100 , 0*100 );
		}
		break;

	case enHoundOpeBatsuCrossFormation:
		if( m_pHound[enHoundSlave] == NULL )
		{
			m_pHound[enHoundSlave] = new CGunHound( 2264*100,3840*100 ,gxFalse);
		}
		m_pHound[enHoundSlave]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundSlave ]->m_Pos.z = PRIO_BG2+1;
		m_pHound[enHoundSlave]->SetLogicPattern( 340 );
		m_pHound[enHoundSlave]->SetTarget( 1350*100 , 0*100 );
		break;

	case enHoundOpeBatsuHomingEliza:
		if( m_pHound[enHoundSlave] == NULL )
		{
			m_pHound[enHoundSlave] = new CGunHound( 2264*100,3840*100 ,gxFalse);
		}
		m_pHound[enHoundSlave]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundSlave ]->m_Pos.z = PRIO_BG2+1;
		m_pHound[enHoundSlave]->SetLogicPattern( 360 );
		m_pHound[enHoundSlave]->SetTarget( pGame->GetHound()->m_Pos.x , 0*100 );
		break;
	}
}

