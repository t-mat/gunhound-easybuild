//----------------------------------------------------------------------------
//
// スタートのステージ（ジャングル）
//
//----------------------------------------------------------------------------

#include <gunvalken.h>
#include <gxLib/util/gxImage/CGXImage.h>
#include "CStageBase.h"
//---------------------------------------------------------------------------
#include "../effect/EffectBase.h"
#include "../enemy/enemy.h"

/*
gxSprite SprStageH02River[]=
{
	{enTexPageEffectStage01,0,0  ,256,96 , 0 , 0},
	{enTexPageEffectStage01,0,104,256,96 , 0 , 0},
};
*/

gxSprite StSprCloud[]={
	{enTexPageBgObj3,0,0,480,256,0,0},
};

enum {
	enSetHoundArea1 = 0,
//	enSetHoundArea2,
//	enSetSankai,
//	enSetHougekichu,
//	enSetGrazziAtack,
//	enSetGrazziDestroy,
//	enSetInthePlant,
	enSetCarryOn,
	enSetTraceGriffon,
	enSetFreeActionStart,
	enSetFreeActionStop,

};

enum {
	enWaraWaraMax = 32,
	enBgColor01 = 0xF0408080,
	enBgColor02 = 0xF0808040,
	enBgColor03 = 0xF0A0A080,
	enBgColor04 = 0xF0802020,
	enGriffonStartPoint = 0,//812,
	enBlaclColor = 0xffa0a0a0,
};


enum
{
	enAreaSeq01,
	enAreaSeq02,
	enAreaSeq03,
	enAreaSeq04,
	enAreaSeq05,
	enAreaTest,
	enAreaSeqMax,
};

enum {
	//エリア２のシーケンス
//	enZoneSeqMitsurinNukeru=0,
//	enZoneSeqChukeitenArrive,
	enZoneSeqVsGrazziInit=0,
	enZoneSeqVsGrazziBattle,
	enZoneSeqVsGrazziEnd,
	enZoneSeqCannonStart,
	enZoneSeqYusouHeliCome,
	enZoneSeqBleachClear,
	enZoneSeqExplosion,
	enZoneSeqWallWait,
	enZoneSeqAssault,
	enZoneSeqNextArea,
};




CStageStart::CStageStart()
{
	Sint32 ii;

//いらない	m_pTextName = NULL;

	m_DummyPos.x = 0;
	m_DummyPos.y = 0;

	m_sScrollSeq = 0;
	m_sScrollPos = 0;
	m_sBombPosition = 0;

	for(ii=0;ii<enHeavyTankmax;ii++)
	{
		m_pTank[ii] = NULL;
	}

	m_pNightSpider = NULL;

	for(ii=0;ii<enHoundMax;ii++)
	{
		m_pHound[ii] = NULL;
		m_pHoundFly[ii] = NULL;
	}

	for(ii=0;ii<enTachikomaMax;ii++)
	{
		m_pTachikoma[ii] = NULL;
	}

	for(ii=0;ii<enApacheMax;ii++)
	{
		m_pTransHeli[ii] = NULL;
	}
	m_bGrazzi = gxFalse;

	m_pYusouHeli = NULL;
	m_pGrazzi    = NULL;
	m_pWall      = NULL;
	m_pYagura = NULL;

	m_sFlashAlpha = 0x00;
	m_sWaraWaraCnt = 0;

	m_sCoveredSeq = 0;
}


CStageStart::~CStageStart()
{

}


void CStageStart::Init()
{
	//---------------------------------------------------------------------------
	//初期化
	//---------------------------------------------------------------------------
	SetFadeIn();

	Sint32 col1 =  0xF0404060;
	Sint32 col2 =  0xF0406080;
//	Sint32 col1 =  0x00000000;
//	Sint32 col2 =  0x00000000;
	pGame->pBg->SetSkyColor(80,col1,col2);

	//---------------------------------------------------------------------------
	//エリア初期化
	//---------------------------------------------------------------------------
//	CEneH0105TankGuard *pTankGuard;

//	if( m_bFirst )
//	{
//		if( g_StGameInfo.m_bDebugRoot )
//		{
//			CGXImage::GetInstance()->Load( "asset/gh/GXI/JUNGLE.GXI" );
//		}
//	}

	while( !CGXImage::GetInstance()->IsLoadFinish() )
	{
		//ロード待ち
	}

	switch(GetArea( )){
	case enAreaSeq01:
	case enAreaSeq02:
		if( g_StGameInfo.m_sGameMode == StGameInfo::enGameModeMission )
		{
			new CItemBox( 9245*100, 128*100 , 0 );
		}
		CGXImage::GetInstance()->Load( "asset/gh/GXI/JUNGLE.GXI" );
		SetDRQStage( "ScenJungle.txt" );

		//味方タンクの生成
		m_pTank[enHeavyTankMaster] = new CEneH0101HeavyTank( enGriffonStartPoint*100,650*100);
		m_pTank[enHeavyTankMaster]->SetOperation(CHoundEnemyBase::enForceOpMovePos,(5500+240)*100);
		m_pTank[enHeavyTankMaster]->SetDummy();

		m_pTank[enHeavyTankSlave]  = new CEneH0101HeavyTank((enGriffonStartPoint+400)*100,650*100);
		m_pTank[enHeavyTankSlave]->SetOperation(CHoundEnemyBase::enForceOpMovePos,(6000+240)*100);
		m_pTank[enHeavyTankSlave]->SetDummy();

		//--------------------

//		m_pTankGuard[0] = new CEneH0105TankGuard( (enGriffonStartPoint+242)*100,(650+40)*100 ,gxTrue);
//		m_pTankGuard[0]->SetTarget( &m_pTank[enHeavyTankMaster]->m_Pos ,248*100 );

//		m_pTankGuard[1] = new CEneH0105TankGuard( (enGriffonStartPoint+242)*100,(650+40)*100 ,gxTrue);
//		m_pTankGuard[1]->SetTarget( &m_pTank[enHeavyTankMaster]->m_Pos ,-248*100 );

		//--------------------

//		m_pTankGuard[2] = new CEneH0105TankGuard( (enGriffonStartPoint+650)*100,(650+40)*100 , gxTrue );
//		m_pTankGuard[2]->SetTarget( &m_pTank[enHeavyTankSlave]->m_Pos , 248*100);

//		m_pTankGuard[3] = new CEneH0105TankGuard( (enGriffonStartPoint+650)*100,(650+40)*100 , gxTrue );
//		m_pTankGuard[3]->SetTarget( &m_pTank[enHeavyTankSlave]->m_Pos , -248*100);


		if(GetArea( ) == enAreaSeq01)
		{
			ControlHound( enSetHoundArea1 );
			InitEnemies();
			viiSub::SetScroll_l( GetTargetPlayer()->x+256*100 , GetTargetPlayer()->y );
		}
		else
		{
			viiMus::PlayBGM(enSoundBgm1,enMusicBgmStageA);
			ControlHound( enSetFreeActionStart );
			SetTargetPlayerPosX(5500*100);
			SetTargetPlayerPosY(500*100);
			viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
			InitEnemies(5800);
			CCockpit::GetInstance()->SetMissionSec( 5*60 );
		}
		break;

	case enAreaSeq03:
		CGXImage::GetInstance()->Load( "asset/gh/GXI/JUNGLE.GXI" );
		SetDRQStage( "ScenJungle.txt" );
		viiMus::PlayBGM(enSoundBgm1,enMusicBgmStageA);
		SetTargetPlayerPosX(7350*100);
		SetTargetPlayerPosY(500*100);
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		InitEnemies(7000);
		new CEffScene0A02(320*100,160*100,PRIO_BG1-1,0);
		CCockpit::GetInstance()->SetMissionSec( 5*60 );
		break;

	case enAreaSeq04:
		//ボスエリア
		CGXImage::GetInstance()->Load( "asset/gh/GXI/JUNGLE.GXI" );
		SetDRQStage( "ScenJungle.txt" );

		m_pHound[enHoundMaster] = NULL;
		m_pHound[enHoundSlave ] = NULL;
		m_pTank[enHeavyTankMaster] = NULL;
		m_pTank[enHeavyTankSlave ] = NULL;

		CCockpit::GetInstance()->SetMissionSec( 60*5 );
		//g_pActManager->Clear(enActionPrioDefault);
		CActManager::GetInstance()->Clear(enActionPrioDefault);
		viiMus::StopBGM( enSoundBgm1 );
		//viiMus::PlayBGM( enSoundBgm2,enMusicBgmBossSimple);
		CCockpit::GetInstance()->SetMissionSec( 5*60 );
		break;

	case enAreaSeq05:
	case enAreaTest:
		CGXImage::GetInstance()->Load( "asset/gh/GXI/JUNGLE.GXI" );
		SetDRQStage( "ScenJungle.txt" );
		return;
	}

	SetGamePlay( gxTrue );

	m_bFirst = gxFalse;

}


void CStageStart::Action()
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
	case enAreaTest:
		AreaTest();
		break;
	}

}

void CStageStart::Area01()
{
	//---------------------------------------------------------------------------
	//エリア１　～登場シーン
	//---------------------------------------------------------------------------

//	CCockpit::GetInstance()->SetOperationRange( m_pTank[enHeavyTankSlave]->m_Pos.x - 600*100 , m_pTank[enHeavyTankSlave]->m_Pos.x + 1200*100);

	if( IsZoneSeq(0) )
	{
		//----------------------------------
		//初期化
		//----------------------------------
		SetGamePlay( gxFalse );
		m_sScrollPos = 0;

		m_sScrollYMin = 280*100;
		m_sScrollYMax = m_sScrollYMin;

		m_sScrollMin = (180)*100+m_sScrollPos;
		m_sScrollMax = m_sScrollMin;

		viiSub::SetScroll_l( m_sScrollMin , m_sScrollYMin );

		CCockpit::GetInstance()->SetBlackMask( gxTrue );

		m_sBombPosition = 0;
		pGame->GetHound()->SetPadButton(0);

//		m_pPlayerFly[enHoundSlave] = new CEneH0503HoundFly(  -100*100 , 340*100 );
		m_pPlayerFly[enHoundSlave] = new CEneH0503HoundFly(   80*100 , 200*100 );
		m_pPlayerFly[enHoundSlave]->SetCarryTarget( &m_pHound[enHoundSlave]->m_Pos.x , &m_pHound[enHoundSlave]->m_Pos.y );
		m_pPlayerFly[enHoundSlave]->SetColor(enBlaclColor);
//		m_pHound[enHoundSlave]->GetLeynos()->CarryOn();
//		m_pHound[enHoundSlave]->GetLeynos()->SetARGB(enBlaclColor);


//		m_pPlayerFly[enHoundMaster] = new CEneH0503HoundFly( -450*100 , 380*100 );
		m_pPlayerFly[enHoundMaster] = new CEneH0503HoundFly( -232*100 , 200*100 );
		m_pPlayerFly[enHoundMaster]->SetCarryTarget( &m_pHound[enHoundMaster]->m_Pos.x , &m_pHound[enHoundMaster]->m_Pos.y );
		m_pPlayerFly[enHoundMaster]->SetColor(enBlaclColor);

//		m_pHound[enHoundMaster]->GetLeynos()->CarryOn();
//		m_pHound[enHoundMaster]->GetLeynos()->SetARGB(enBlaclColor);

//		m_pPlayerFly[enHoundPlayer] = new CEneH0503HoundFly(  -800*100 , 400*100 );
		m_pPlayerFly[enHoundPlayer] = new CEneH0503HoundFly(  -578*100 , 120*100 );
		m_pPlayerFly[enHoundPlayer]->SetCarryTarget( &pGame->GetHound()->m_Pos.x , &pGame->GetHound()->m_Pos.y );
//		m_pPlayerFly[enHoundPlayer]->SetColor(enBlaclColor);

		ControlHound(enSetCarryOn);

		m_pYagura  =new CEneH0106Yagura( 2966*100 , 600*100 );

		m_pTransHeli[0] = new CEneH0005TransHeli(   0*100  , 458*100 );
		m_pTransHeli[0]->SetMaxSpeed(420);

		m_pTransHeli[1] = new CEneH0005TransHeli(  100*100  , 408*100 );
		m_pTransHeli[1] ->SetMaxSpeed(280);

		m_pTransHeli[2] = new CEneH0005TransHeli(  -300*100 , 428*100 );
		m_pTransHeli[2] ->SetMaxSpeed(320);

		BgCommon();
		ResetZoneTimer();
		SetZoneSeq(100);
		return;
	}
	else if( IsZoneSeq(100) )
	{
		//----------------------------------
		//ハウンドフライ更新
		//----------------------------------
		pGame->GetHound()->SetPadButton(0);
		m_sScrollPos += 35;
		m_pPlayerFly[enHoundSlave]->m_Pos.x   += 80*120/100;
		m_pPlayerFly[enHoundSlave]->m_Pos.y   += (340*100 - m_pPlayerFly[enHoundSlave]->m_Pos.y) /120;

		m_pPlayerFly[enHoundMaster ]->m_Pos.x += 80*160/100;
		m_pPlayerFly[enHoundMaster]->m_Pos.y   += (380*100 - m_pPlayerFly[enHoundMaster]->m_Pos.y) /120;

		m_pPlayerFly[enHoundPlayer]->m_Pos.x  += 80*195/100;
		m_pPlayerFly[enHoundPlayer]->m_Pos.y   += (400*100 - m_pPlayerFly[enHoundPlayer]->m_Pos.y) /120;

		m_sScrollMin = (180)*100+m_sScrollPos;
		m_sScrollMax = m_sScrollMin;

		switch( m_sZoneTimer ){
		case 120:
			CCockpit::GetInstance()->SetMessage(1050);	//本隊は予定通り突入ポイントへ到着。
			break;
		case 30:
			viiMus::PlayBGM(enSoundBgm1,enMusicBgmStageA);
			break;
		case 230:
		case 255:
		case 320:
			pGame->QuakeOn(64);
			viiMus::PlaySound( enSoundExplosionLarge );
			break;

		case 260:
		case 290:
			viiMus::PlaySound(enSoundExplosionMiddle);
			break;
		case 330:
			viiMus::PlaySound( enSoundCrashA );
			break;

		case 140:
			m_pTransHeli[0] ->SetBomb();
			viiMus::PlaySound( enSoundMissile );
			break;
		case 165:
			m_pTransHeli[1] ->SetBomb();
			viiMus::PlaySound( enSoundMissile );
			break;
		case 240:
			m_pTransHeli[2] ->SetBomb();
			viiMus::PlaySound( enSoundMissile );
			break;
		}

		if( viiSub::Rand()%6 == 0  && m_sZoneTimer <= 350 )
		{
			CEffEneTwinGun *pTwinGun;
			Sint32 ax,ay;
			ax = m_sScrollMin+(WINDOW_W/2+viiSub::Rand()%WINDOW_W)*100;
			ay = m_sScrollYMin+WINDOW_H*100;
			pTwinGun = new CEffEneTwinGun( ax , ay , 180+45 , 920 );
			pTwinGun->SetAtariOff();
		}

		if( m_sZoneTimer == 460 )
		{
			CEffFire *p;
			p = new CEffFire( 726*100,638*100 , PRIO_BG2+1 );
			p->SetScale(2.f);

			p = new CEffFire( 1200*100,655*100 , PRIO_BG2-1 );
			p->SetScale(2.5f);

			p = new CEffFire( 900*100,655*100 , PRIO_BG2-1 );
			p->SetScale(3.f);

			p = new CEffFire( 1500*100,655*100 , PRIO_BG2-1 );
			p->SetScale(2.f);

			ResetZoneTimer();
			SetZoneSeq(200);
		}

		viiMus::GetInstance()->PlayLoopSound( enSoundHeliRoter , 108 );
		BgCommon();
		return;
	}
	else if( IsZoneSeq(200) )
	{
		//----------------------------------
		//スクロール位置下げる
		//----------------------------------
		pGame->GetHound()->SetPadButton(0);
		m_sScrollPos += 80;
		m_sScrollMin = (145)*100+m_sScrollPos;
		m_sScrollMax = (145)*100+m_sScrollPos;

		m_pPlayerFly[enHoundSlave]->m_Pos.x   += (120);
		m_pPlayerFly[enHoundMaster ]->m_Pos.x += (180);
		m_pPlayerFly[enHoundPlayer]->m_Pos.x  += (195);

		if( m_sZoneTimer == 180 )
		{
/*
			for(Sint32 ii=0;ii<4;ii++)
			{
				CSoldierBase *q;
				q = new CSoldierBase( );
				q->SetPosition( 0 , (720+ii*60)*100, 650*100 , NULL);
				q->SetForce(enForceEnemy);
				q->SetTargetPosition( 9600*100 );
				q->ChangeMoveType( enMoveTypeEscape );
				q->SetEnd();
			}
*/
		}

		if( m_sScrollYMin < 450*100 )
		{
			m_sScrollYMin += 50;
			m_sScrollYMax = m_sScrollYMin;
		}
		else
		{
			ResetZoneTimer();
			SetZoneSeq(300);
		}

		viiMus::GetInstance()->PlayLoopSound( enSoundHeliRoter , 108 );
		BgCommon();
		return;
	}
	else if( IsZoneSeq(300) )
	{
		//----------------------------------
		//ハウンド投下
		//----------------------------------
		pGame->GetHound()->SetPadButton(0);
		switch( m_sZoneTimer){
		case 20:
			m_pPlayerFly[enHoundMaster]->SetCarryOff();
//			m_pHound[enHoundMaster]->GetLeynos()->SetARGB(ARGB_DFLT);
//			m_pHound[enHoundMaster]->GetLeynos()->CarryOff();
			m_pHound[enHoundMaster]->SetCarryOff();
			m_pHound[enHoundMaster]->SetColor(ARGB_DFLT);
			break;

		case 60:
			m_pPlayerFly[enHoundPlayer]->SetCarryOff();
			pGame->GetHound()->SetCarryOff();
			pGame->GetHound()->SetColor(ARGB_DFLT);
			break;

		case 110:
			m_pPlayerFly[enHoundSlave]->SetCarryOff();
//			m_pHound[enHoundSlave]->GetLeynos()->SetARGB(ARGB_DFLT);
//			m_pHound[enHoundSlave]->GetLeynos()->CarryOff();
			m_pHound[enHoundSlave]->SetCarryOff();
			m_pHound[enHoundSlave]->SetColor(ARGB_DFLT);
			break;
		case 120:
			SetZoneSeq(400);
			break;
		}

		viiMus::GetInstance()->PlayLoopSound( enSoundHeliRoter , 108 );
		BgCommon();
		return;
	}
	else if( IsZoneSeq(400) )
	{
		//----------------------------------
		//演出終了
		//----------------------------------
		//味方ヘリ終了
		m_pTransHeli[0] ->Clear();
		m_pTransHeli[1] ->Clear();
		m_pTransHeli[2] ->Clear();

		m_pTransHeli[0] = NULL;
		m_pTransHeli[1] = NULL;
		m_pTransHeli[2] = NULL;

		CCockpit::GetInstance()->SetBlackMask( gxFalse );
		ResetZoneTimer();
		SetZoneSeq( 1000 );

		viiMus::GetInstance()->PlayLoopSound( enSoundHeliRoter , 108 );
		BgCommon();
		return;
	}
	else if( IsZoneSeq(1000) )
	{
		SetGamePlay( gxTrue );
		m_sScrollMax = 2550*100;
		CCockpit::GetInstance()->SetMissionSec( 60*10 );

		viiMus::GetInstance()->PlayLoopSound( enSoundHeliRoter , 108 );

		if( m_sZoneTimer == 60 )
		{
			CCockpit::GetInstance()->SetMessage(1100);	//こちらライザ、目標地点に到達！
		}

		if( m_sZoneTimer == 120 )
		{
			SetZoneSeq(1200);
		}
	}
	else if( IsZoneSeq(1200) )
	{
		SetZoneSeq( 1300 );
	}
	else if( IsZoneSeq( 1300 ) )
	{
		//----------------------------------------
		//目標地点Ａ（密林内部）
		//----------------------------------------
		ControlHound( enSetTraceGriffon );

		m_sScrollMax = 2550*100;

		if( viiSub::Rand()%128==0) m_sFlashAlpha = 0xa0;

		gxBool bFlag = gxFalse;

		if( m_pYagura && m_pYagura->IsDead() )
		{
			bFlag = gxTrue;
		}

		if( bFlag )
		{
			//CCockpit::GetInstance()->SetMessage(1100);	//密林を抜けたらこっちは丸見えよ。

			//--------------------------------------
			//タンクガード攻撃開始
			//--------------------------------------
//			for(Sint32 ii=0;ii<enTankGuardMax;ii++)
//			{
//				m_pTankGuard[ii]->SetRealAtack();
//			}

			m_sScrollMax = 5800*100;
			SetZoneSeq(1400);
		}
	}
	else if( IsZoneSeq( 1400 ) )
	{
		//----------------------------------------
		//目標地点Ｂ（中継点）に到達
		//----------------------------------------
		ControlHound( enSetFreeActionStart );

		m_sScrollYMin = 150*100;
		m_sScrollYMax = 450*100;
		if(GetTargetPlayer()->x < 3200*100)
		{
			CCockpit::GetInstance()->SetArrow( 0 );
			//AddMissionSec( 1, 60*3 );
		}

		Sint32 sx,sy;
		viiSub::GetScroll_s( sx,sy );

		if( sx >= 5700 )
		{
			ChangeArea( enAreaSeq02 );
		}
	}

	BgCommon();
	viiSub::LimitMoveOnlyV();

}

void CStageStart::Area02()
{
	//---------------------------------------------------------------------------
	//エリア２　～砲台まで（戦車護衛）
	//---------------------------------------------------------------------------
	m_sScrollMin = 5500*100;

	if( IsZoneSeq( enZoneSeqVsGrazziInit ) )
	{
		//----------------------------------------
		//グラージ登場
		//----------------------------------------
		m_sScrollMax = 5800*100;

		if( m_pGrazzi == NULL )
		{
			m_pGrazzi = new CEneH0204Grazzi(6500*100,654*100);
			m_pGrazzi->SetMoveArea( 5800*100 , 6200*100 );

			//シャッター作成
			m_pWall   = new CEneH0410Wall( 7132*100 , 0*100 );
			SetZoneSeq( enZoneSeqVsGrazziBattle );
		}
	}
	else if( IsZoneSeq( enZoneSeqVsGrazziBattle ) )
	{
		//-------------------------------------
		//グラージ破壊待ち
		//-------------------------------------

//		m_sScrollMax = m_pGrazzi->m_Pos.x-420*100;
//		m_sScrollMin = m_pGrazzi->m_Pos.x-420*100;

		if( m_pGrazzi->m_Pos.x < 6300*100 )
		{
			if( !CCockpit::GetInstance()->IsMessageNow() )
			{
				CCockpit::GetInstance()->SetMessage(1300);	//奴らをこれ以上、侵入させるな！！
			}
		}

		if( m_pGrazzi->IsDead())
		{
			new CEffScene0A02(320*100,160*100,PRIO_BG1-1,0);
			m_bBgColorChange = gxTrue;

			m_pGrazzi->Clear();
			m_pGrazzi = NULL;

			ResetZoneTimer();
			SetZoneSeq( enZoneSeqVsGrazziEnd );
		}
		else
		{
			if( m_pTank[enHeavyTankSlave]->IsMokutekichi() )
			{
				if( !CCockpit::GetInstance()->IsMessageNow() )
				{
					CCockpit::GetInstance()->SetMessage(1350);	//こちら輸送ヘリ。合流地点に敵影、
				}
			}
		}
	}
	else if( IsZoneSeq( enZoneSeqVsGrazziEnd ) )
	{
		//-------------------------------------
		//グラージ破壊直後
		//-------------------------------------
		m_sScrollMax = 6760*100;
		m_sScrollYMin = 0*100;
		m_sScrollYMax = 9600*100;

		if( 100 == m_sZoneTimer )
		{
			//まだ輸送ヘリが来ていない場合
			if( !m_pTank[enHeavyTankSlave]->IsMokutekichi() )
			{
				CCockpit::GetInstance()->SetMessage(1400);	//合流地点を確保したぞ。輸送ヘリはまだか？
			}
		}

		if( m_pTank[enHeavyTankSlave]->IsMokutekichi() )
		{
			if( !m_pYusouHeli->IsLanding() )
			{
				//強制着陸
				m_pYusouHeli->SetLanding();

				//カバーを閉じて爆破待ち
				m_sCoveredSeq = 1;
				m_pWall->SetCover();
			}

			ResetZoneTimer();
			SetZoneSeq(enZoneSeqCannonStart);
		}

		if(m_pWall->IsDead())
		{
			//---------------------------------------------------
			//上級者コース
			//ヘリ到着までにシャッターが破壊されていれば次へ進行する
			//---------------------------------------------------
			CCockpit::GetInstance()->SetMessage(1850);	//全機、プラント内のニュートロンポリマー
			SetZoneSeq(enZoneSeqWallWait);
		}

	}
	else if( IsZoneSeq(enZoneSeqCannonStart) )
	{
		//----------------------------------------
		//グリフォン砲撃開始
		//----------------------------------------

		m_pTank[enHeavyTankMaster]->SetHougeki();
		m_pTank[enHeavyTankSlave ]->SetHougeki();

		ResetZoneTimer();

		SetZoneSeq(enZoneSeqYusouHeliCome);
	}
	else if( IsZoneSeq(enZoneSeqYusouHeliCome) )
	{
		//----------------------------------------
		//ヘリ到着待ち
		//----------------------------------------

		if(m_pYusouHeli->IsSoldierRideOff())
		{
			//--------------------------------------
			//ソルジャー集合待ち
			//--------------------------------------

			if( m_sZoneTimer >= 300 )
			{
				if( m_pYusouHeli->GetSoldierLeader() )
				{
					m_pYusouHeli->GetSoldierLeader()->SetTargetPosition( 6916*100 );
					m_pYusouHeli->GetSoldierLeader()->CommandMember( );
					m_pYusouHeli->GetSoldierLeader()->SetTargetPosition( 6948*100 );

					CCockpit::GetInstance()->SetMessage( 1600 );	//くそ！　対爆用の特殊隔壁か。

					ResetZoneTimer();
					SetZoneSeq(enZoneSeqBleachClear);
				}
			}
		}
	}
	else if( IsZoneSeq(enZoneSeqBleachClear) )
	{
		//--------------------------------------
		//ブリーチクリア
		//--------------------------------------

		if( 1000 == m_sZoneTimer )
		{
			m_pYusouHeli->GetSoldierLeader()->CommandMember( enMoveTypeBombSet );
		}
		else if( 1600 <= m_sZoneTimer )
		{
			if( m_pYusouHeli->GetSoldierLeader()->GetMember( 0 )->GetSeq()==40 )
			{
				m_pYusouHeli->GetSoldierLeader()->SetTargetPosition( 6000*100 );
				m_pYusouHeli->GetSoldierLeader()->CommandMember( );
				m_pYusouHeli->GetSoldierLeader()->SetTargetPosition( 6000*100 );
				ResetZoneTimer();
				CCockpit::GetInstance()->SetMessage(1700);	//Ｃ４０爆薬をセット、
				SetZoneSeq(enZoneSeqExplosion);
			}
		}
	}
	else if( IsZoneSeq(enZoneSeqExplosion) )
	{
		//--------------------------------------
		//シャッター破壊
		//--------------------------------------

		if( 300 == m_sZoneTimer )
		{
			//反応弾爆発
			viiMus::PlaySound( enSoundCrashB );
			new CEffectFinalExplosion(7150*100, 530*100 ,3);
		}
		else if( 400 == m_sZoneTimer )
		{
			//破壊エフェクトを出す
			m_pWall->ForceCrash();

			SetCrashDoorEffect();

			//プラント内に火を放つ
			//SetFirePlantEffect();
		}
		else if( 500 == m_sZoneTimer )
		{
			CCockpit::GetInstance()->SetMessage(1800);	//電磁シャッターの消失を確認。
			SetZoneSeq(enZoneSeqWallWait);
		}
	}
	else if( IsZoneSeq(enZoneSeqWallWait) )
	{
		if( m_pWall->IsDead() )
		{
			SetZoneSeq(enZoneSeqAssault);
		}

	}
	else if( IsZoneSeq(enZoneSeqAssault) )
	{
		m_sScrollMax = 9300*100;

		CCockpit::GetInstance()->SetArrow( 0 );
		//AddMissionSec( 2, 60*3 );

		Sint32 sx,sy;
		viiSub::GetScroll_s( sx,sy );
		if(sx>7250)
		{
			ChangeArea( enAreaSeq03 );
		}
	}


	//-----------------------------------------------
	//シャッター処理
	//-----------------------------------------------

	BgCommon();
	viiSub::LimitMoveVH();
}


void CStageStart::Area03()
{
	//-----------------------------------------------------
	//エリア３：プラント内部
	//-----------------------------------------------------
	m_sScrollMin = 7250*100;
	m_sScrollMax = 9300*100;

//	CCockpit::GetInstance()->SetOperationRange( m_sScrollMin,m_sScrollMax);

	if( m_sCoveredSeq == 1 )
	{
		CCockpit::GetInstance()->SetMessage(1900);	//全機、プラント内のニュートロンポリマー
	}

	ControlHound( enSetFreeActionStop );

	if( IsZoneSeq(0) )
	{
		//-------------------------------
		//ミッションタイム更新
		//-------------------------------
		SetFirePlantEffect();
		SetZoneSeq( 100 );
	}
	else if( IsZoneSeq(100) )
	{
		if( IsAreaArrive(8000,0,50,1000) )
		{
			CCockpit::GetInstance()->SetMessage(1900);
		}

		if( IsAreaArrive(9750,680,50,320) )
		{
			new CEntranceControl( CEntranceControl::enExit , DIR_RIGHT );
			viiMus::StopBGM( enSoundBgm1 , 90 );
			SetFadeOut();
			SetZoneSeq( 200 );
		}
	}
	else if( IsZoneSeq(200) )
	{
		//フェード待ち
		if( IsFadeEnd() )
		{
			if( !viiMus::IsPlayBGM( enSoundBgm1 ) )
			{
				CCockpit::GetInstance()->ResetMessageWindow();
				//g_pActManager->Clear(enActionPrioDefault);
				CActManager::GetInstance()->Clear(enActionPrioDefault);
				m_pYusouHeli = NULL;
				m_pGrazzi    = NULL;
				m_pWall      = NULL;
				m_pYagura = NULL;
				m_pHound[enHoundMaster]    = NULL;
				m_pHound[enHoundSlave ]    = NULL;
				m_pTank[enHeavyTankMaster] = NULL;
				m_pTank[enHeavyTankSlave ] = NULL;
				ChangeArea( enAreaSeq04 );
			}
			return;
		}
	}

//	CCockpit::GetInstance()->SetOperationRange( m_sScrollMin,m_sScrollMax);

	BgCommon();
	viiSub::LimitMoveVH();

}


void CStageStart::Area04()
{
	//-----------------------------------------------------
	//エリア４：ボスエリア
	//-----------------------------------------------------

	m_sScrollMax = 11000*100;
	m_sScrollMin = 9856*100;

//	CCockpit::GetInstance()->SetOperationRange( m_sScrollMin,m_sScrollMax);
	LayerScroll[enBgTypeRear].ox  = 2000*100;
	LayerScroll[enBgTypeRear].oy  = 0*100;
	LayerScroll[enBgTypeRear].x  = 80;
	LayerScroll[enBgTypeRear].y  = 100;
	LayerScroll[enBgTypeFrnt].x  = 100;
	LayerScroll[enBgTypeFrnt].y  = 100;
	LayerScroll[enBgTypeFrnt].ox  = 1164*100;
	LayerScroll[enBgTypeFrnt].oy  = 0;

	if( IsZoneSeq(0) )
	{
		//g_pActManager->Clear(enActionPrioDefault);
		CActManager::GetInstance()->Clear(enActionPrioDefault);

		SetFadeIn();
		SetTargetPlayerPosX( 9700*100 );
		SetTargetPlayerPosY( (895-32)*100);

		new CEntranceControl( CEntranceControl::enEntrance , DIR_RIGHT );
		viiSub::SetScroll_l( 9847*100,665*100 );//GetTargetPlayer()->x , GetTargetPlayer()->y );

		m_pNightSpider = new CEneH0102NightSpider( 10845*100, 825*100 );

		SetZoneSeq(10);
	}
	else if( IsZoneSeq(10) )
	{
		if( m_sZoneTimer == 20 )
		{
			viiMus::PlayBGM( enSoundBgm2,enMusicBgmBossSimple);
		}

		if( m_sZoneTimer == 120 )
		{
			CCockpit::GetInstance()->SetMessage(2000);	//こちらパープル２、格納庫に戦略機動兵器を発見！！
		}

		if( m_pNightSpider->IsDead() )
		{
			m_pNightSpider->Clear();
			m_pNightSpider = NULL;
			ChangeArea( enAreaSeq05 );
		}
	}
	else if( IsZoneSeq(20) )
	{

	}


	BgCommon();
	viiSub::LimitMoveVH();

}

void CStageStart::Area05()
{
	//-----------------------------------------------------
	//エリア５：ステージクリア
	//-----------------------------------------------------

	if( IsZoneSeq( 0 ) )
	{
		//クリア
		if( m_sZoneTimer >= 60 )
		{
			SetZoneSeq( 100 );
		}
	}
	else if( IsZoneSeq( 100 ) )
	{
		SetGamePlay( gxFalse );

#ifdef _TRIAL_VERSION_
		CCockpit::GetInstance()->SetMessage(3200);	//オペレーション・レポート。予約せよ
#else
		CCockpit::GetInstance()->SetMessage(3000);	//オペレーション・レポート。
#endif
		CCockpit::GetInstance()->SetStageClear();
		SetZoneSeq( 200 );

	}
	else if( IsZoneSeq( 200 ) )
	{
//		if( !CCockpit::GetInstance()->IsMessageNow() )
		{
			if( CCockpit::GetInstance()->IsClearOver() )
			{
				CCockpit::GetInstance()->DestroyStageClear();
				SetFadeOut(2);
				SetZoneSeq( 300 );
			}
		}
	}
	else if( IsZoneSeq( 300 ) )
	{
		//フェード終了待ち
		if( IsFadeEnd() )
		{
			pGame->stage_clear(enClearSeqStageClear);
			ChangeArea( GAME_NONE );	//ステージ終了
		}
	}

}


gxBool CStageStart::WaraWaraSpider()
{
	//----------------------------------------
	//ワラワラスパイダーゲーム
	//----------------------------------------

	return gxFalse;
}


void CStageStart::SetCrashDoorEffect()
{
	//---------------------------------------
	//シャッター破壊エフェクト
	//---------------------------------------

	new CEffectDmgprt( 7150*100, 530*100, 180+15 , 800 );
	new CEffectDmgprt( 7150*100, 530*100, 180+45 , 600 );
	new CEffectDmgprt( 7150*100, 530*100, 180-15 , 500 );
	new CEffectDmgprt( 7150*100, 530*100, 180-32 , 700 );

}

void CStageStart::SetFirePlantEffect()
{
	//---------------------------------------
	//プラント内部に火を放つ
	//---------------------------------------
	new CEffectBlackExplosion( 4200*100,600*100 , &m_BgLayerPos );

	EffectSun((4200-340+viiSub::Rand()%(WINDOW_W*2))*100,500*100 , &m_BgLayerPos );
	EffectSun((4200-340+viiSub::Rand()%(WINDOW_W*2))*100,500*100 , &m_BgLayerPos );

	new CEffFire( 7503*100,582*100 , PRIO_BG2-1 );
	new CEffFire( 7583*100,582*100 , PRIO_BG2-1 );
	new CEffFire( 7420*100,582*100 , PRIO_BG2-1 );

	new CEffFire( 8000*100,582*100 , PRIO_BG2 );
	new CEffFire( 8080*100,582*100 , PRIO_EFFECT );
	new CEffFire( 8170*100,582*100 , PRIO_BG2 );

	new CEffFire( 8484*100,582*100 , PRIO_BG2-1 );
	new CEffFire( 8554*100,582*100 , PRIO_BG2-1 );
	new CEffFire( 8620*100,582*100 , PRIO_BG2-1 );


//	new CEffFire( 9720*100,455*100 , PRIO_ENEMY );

}


void CStageStart::ControlHound(Sint32 sOperation )
{
	//-----------------------------------------
	//ハウンドのコントロール
	//-----------------------------------------

	switch(sOperation){
	case enSetHoundArea1:
		//-------------------------------
		//新規ハウンドをセット
		//-------------------------------
		m_pHound[enHoundMaster] = new CGunHound( 300*100,100*100 ,gxFalse);
		m_pHound[enHoundMaster]->SetLeader(gxTrue);

		m_pHound[enHoundSlave] = new CGunHound( 300*100,100*100 ,gxFalse);
		break;

	case enSetCarryOn:
		//-------------------------------
		//つるされている
		//-------------------------------
		m_pHound[enHoundSlave]->SetMode( CGunHound::enHoundModeCarreer );
		m_pHound[enHoundSlave]->SetColor(enBlaclColor);
		m_pHound[enHoundMaster]->SetMode( CGunHound::enHoundModeCarreer );
		m_pHound[enHoundMaster]->SetColor(enBlaclColor);
		pGame->GetHound()->SetMode( CGunHound::enHoundModeCarreer );
		pGame->GetHound()->SetColor(enBlaclColor);
		m_pHound[enHoundMaster]->m_Pos.z = PRIO_BG2-1;
		m_pHound[enHoundSlave ]->m_Pos.z = PRIO_BG2-1;
		break;

	case enSetTraceGriffon:
		//-------------------------------
		//グリフォンを護衛
		//-------------------------------
		if( m_pHound[enHoundMaster] == NULL ) m_pHound[enHoundMaster] = new CGunHound( 300*100,100*100 ,gxFalse);
		m_pHound[enHoundMaster]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundMaster]->SetLogicPattern( 101 );
		if( viiSub::Rand()%120 == 0 )
		{
			m_pHound[enHoundMaster]->SetTarget( m_pTank[enHeavyTankSlave]->m_Pos.x+320*100 , m_pTank[enHeavyTankSlave]->m_Pos.y );
		}

		m_pHound[enHoundSlave]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundSlave ]->SetLogicPattern( 101 );
		if( viiSub::Rand()%120 == 0 )
		{
			m_pHound[enHoundSlave ]->SetTarget( m_pTank[enHeavyTankMaster ]->m_Pos.x+320*100 , m_pTank[enHeavyTankMaster ]->m_Pos.y );
		}
		break;

	case enSetFreeActionStart:
		//---------------------------------------------------------------
		//バツ機追従
		//---------------------------------------------------------------
		if( m_pHound[enHoundSlave] == NULL ) m_pHound[enHoundSlave] = new CGunHound( 300*100,100*100 ,gxFalse);
		m_pHound[enHoundSlave]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundSlave ]->SetLogicPattern( 150 );
		break;

	case enSetFreeActionStop:
		//---------------------------------------------------------------
		//バツ機追従（停止）
		//---------------------------------------------------------------
		if( m_pHound[enHoundSlave] == NULL ) m_pHound[enHoundSlave] = new CGunHound( 300*100,100*100 ,gxFalse);
		m_pHound[enHoundSlave]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundSlave ]->SetLogicPattern( 101 );
		if( viiSub::Rand()%120 == 0 )
		{
			if( m_pTank[enHeavyTankMaster] == NULL ) m_pTank[enHeavyTankMaster] = new CEneH0101HeavyTank( enGriffonStartPoint*100,650*100);
			m_pHound[enHoundSlave ]->SetTarget( m_pTank[enHeavyTankMaster ]->m_Pos.x+320*100 , m_pTank[enHeavyTankMaster ]->m_Pos.y );
		}
		break;


#if 0

	case enSetHoundArea2:
		m_pHound[enHoundMaster] = new CGunHound( 300*100,100*100 ,gxFalse);
		m_pHound[enHoundMaster]->SetLeader(gxTrue);

		m_pHound[enHoundSlave] = new CGunHound( 300*100,100*100 ,gxFalse);

		m_pHound[enHoundMaster]->m_Pos.x = 5300*100;
		m_pHound[enHoundMaster]->m_Pos.y = 600*100;

/*
		m_pHound[enHoundSlave]->m_Pos.x = 5300*100;
		m_pHound[enHoundSlave]->m_Pos.y = 600*100;

		m_pTank[enHeavyTankMaster]->m_Pos.x = 3200*100;
		m_pTank[enHeavyTankSlave ]->m_Pos.x = (3200+400)*100;
*/
		break;

	case enSetSankai:
		//-------------------------------
		//密林を抜けて散会
		//-------------------------------
		m_pHound[enHoundSlave ]->SetLogicPattern( 150 );
		break;

	case enSetHougekichu:
		//-------------------------------
		//砲撃開始固定砲台化
		//-------------------------------
		m_TargetPos.x = 6360*100;
		m_TargetPos.y = 400*100;
		break;

	case enSetGrazziAtack:
		//-------------------------------
		//グラージ破壊行為
		//-------------------------------
//		m_pHound[enHoundMaster]->SetMode( CGunHound::enHoundModeNPC );
//		m_pHound[enHoundMaster]->SetLogicPattern( 110 );

		m_pHound[enHoundSlave ]->SetLogicPattern( 150 );

		m_pHound[enHoundMaster]->m_Pos.z = PRIO_BG2+1;
		{
			if( m_pGrazzi )
			{
				m_pHound[enHoundMaster]->SetTarget( m_pGrazzi->m_Pos.x+(-16+viiSub::Rand()%32)*2000 , m_pGrazzi->m_Pos.y );
				m_pHound[enHoundMaster]->SetTarget( m_pGrazzi->m_Pos.x, m_pGrazzi->m_Pos.y );
			}
		}

		m_pHound[enHoundMaster]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundMaster ]->SetLogicPattern( 101 );
		if( viiSub::Rand()%120 == 0 )
		{
			m_pHound[enHoundMaster ]->SetTarget( m_pTank[enHeavyTankMaster ]->m_Pos.x+320*100 , m_pTank[enHeavyTankMaster ]->m_Pos.y );
		}
		break;

	case enSetGrazziDestroy:
		//-------------------------------
		//グラージ破壊後シャッターを攻撃
		//-------------------------------
/*
		{
			Sint32 sPos = 6650*100;
			Sint32 sDist = sPos - pGame->GetHound()->m_Pos.x;
			m_pHound[enHoundMaster]->SetMode( CGunHound::enHoundModeNPC );
			m_pHound[enHoundMaster]->SetLogicPattern( 120 );
			m_pHound[enHoundMaster]->SetTarget( sPos+sDist, pGame->GetHound()->m_Pos.y );

			m_pHound[enHoundSlave]->SetMode( CGunHound::enHoundModeNPC );
			m_pHound[enHoundSlave ]->SetLogicPattern( 101 );
			if( viiSub::Rand()%120 == 0 )
			{
				m_pHound[enHoundSlave ]->SetTarget( m_pTank[enHeavyTankMaster ]->m_Pos.x+320*100 , m_pTank[enHeavyTankMaster ]->m_Pos.y );
			}
		}
*/
		m_pHound[enHoundSlave ]->SetLogicPattern( 150 );
		break;

	case enSetInthePlant:
		if( m_pHound[enHoundSlave] )
		{
			delete m_pHound[enHoundSlave];
			m_pHound[enHoundSlave] = NULL;
		}
		if( m_pHound[enHoundMaster] )
		{
			delete m_pHound[enHoundMaster];
			m_pHound[enHoundMaster] = NULL;
		}
		if( m_pTank[enHeavyTankMaster] )
		{
			delete m_pTank[enHeavyTankMaster];
			m_pTank[enHeavyTankMaster] = NULL;
		}
		if( m_pTank[enHeavyTankSlave] )
		{
			delete m_pTank[enHeavyTankSlave];
			m_pTank[enHeavyTankSlave] = NULL;
		}
		break;
#endif

	}
}

void CStageStart::AreaCommon()
{
	//-----------------------------------------------------
	//エリア：共通
	//-----------------------------------------------------

	if( m_sFlashAlpha > 0 )
	{
		//------------------------------------------
		//フラッシュ
		//------------------------------------------
		Sint32 sAdd = -m_sFlashAlpha/10;
		if( sAdd == 0 ) sAdd = -1;
		m_sFlashAlpha += sAdd;
	}

	if( m_bBgColorChange )
	{
		m_sMorphColorCnt += 8;
		if( m_sMorphColorCnt >= 100*100 )
		{
			m_sMorphColorCnt = 100*100;
		}

		Sint32 col1,col2;

		col1 = viiSub::GetMorphingColor( m_sMorphColorCnt/100 , enBgColor01 , enBgColor02 );
		col2 = viiSub::GetMorphingColor( m_sMorphColorCnt/100 , enBgColor03 , enBgColor04 );

		pGame->pBg->SetSkyColor( 50 , col1 , col2 );
	}

	if( m_pTank[enHeavyTankSlave] )
	{
		if( m_pTank[enHeavyTankSlave]->m_Pos.x >= 3200*100 )
		{
			if( !CCockpit::GetInstance()->IsMessageNow() )
			{
				CCockpit::GetInstance()->SetMessage(1200);	//まったくトロくせえ戦車だ、
			}

			if( m_pYusouHeli == NULL )
			{
				m_pYusouHeli = new CEneH0007YusouHeli( -1100*100 , 446*100 );
				m_pYusouHeli->SetTargetPos( 6200*100 );
			}
		}
	}

}


void CStageStart::Draw()
{

	//-------------------------------------
	//空
	//-------------------------------------

	viiDraw::Spr_NoScrl( &StSprCloud[ 0 ] , 0,0,PRIO_BG1-1 , ATR_DFLT , 0x40808080 );


	//-------------------------------------
	//フラッシュ
	//-------------------------------------

	if( m_sFlashAlpha > 0 )
	{
		viiDraw::BoxScrl_s( 0 , 500 , 3000 , 1000 , PRIO_BG1+1 , gxTrue , ARGB(m_sFlashAlpha,0xa0,0xa0,0xa0) , ATR_ALPHA_PLUS );
	}

/*
	Sint32 yy1 = 60*WINDOW_H/100;
	Sint32 yy2 = 80*WINDOW_H/100;

	Uint32 sColor[4] = 
	{
		ARGB( 0xff, 14,  18, 21 ),
		ARGB( 0xff, 45,  77, 135 ),
		ARGB( 0xff, 129, 158,193 ),
		ARGB( 0xff, 255, 255,233 ),
	};


//	if( !m_bBgColorChange )
	{
		viiDraw::Grad(0 ,0   ,WINDOW_W , yy1	  , PRIO_BG0+0  ,ATR_DFLT,sColor[0],sColor[0],sColor[1],sColor[1] );
		viiDraw::Grad(0 ,yy1 ,WINDOW_W , yy2	  , PRIO_BG0+0  ,ATR_DFLT,sColor[1],sColor[1],sColor[2],sColor[2] );
		viiDraw::Grad(0 ,yy2 ,WINDOW_W , WINDOW_H , PRIO_BG0+0  ,ATR_DFLT,sColor[2],sColor[2],sColor[3],sColor[3] );
	}
*/

	//-------------------------------------
	//河
	//-------------------------------------
/*

	Sint32 sx,sy;

	sx = 0-(m_sRiverScroll/100)*100 + ( ( GetTargetPlayer()->x/100)/512)*51200 - 51200;
	sy = 600;

	for(Sint32 ii=0; ii<5; ii++)
	{
		viiDraw::Sprite( &SprStageH02River[ 0 ] , sx+(0    )+ii*51200 , sy*100,PRIO_BG1+1 , ATR_DFLT , 0xF0FFFFFF );
		viiDraw::Sprite( &SprStageH02River[ 1 ] , sx+(25600)+ii*51200 , sy*100,PRIO_BG1+1 , ATR_DFLT , 0xF0FFFFFF );
	}
*/

}


