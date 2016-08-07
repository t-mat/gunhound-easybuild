//----------------------------------------------------------------------------
//
//　エクストラステージ １（アイアンギア戦）
//
//----------------------------------------------------------------------------

#include <gunvalken.h>
#include <gxLib/util/gxImage/CGXImage.h>
#include "CStageBase.h"
//---------------------------------------------------------------------------
#include "../effect/EffectBase.h"
#include "../enemy/enemy.h"

extern gxSprite StSprCloud[];

enum
{
	enAreaSeq01,
	enAreaSeq02,
	enAreaSeq03,
	enAreaSeq04,
	enAreaSeq05,
	enAreaSeq06,
	enAreaSeqBoss,	//6
	enAreaDebug,
	enAreaSeqMax,
};

enum {
	enRollerDashHeight = 950*100,
	enScroiiMinY = 180*100,
	enGravitPowerX = 300,
	enMissionTime = 60*20,
};

gxSprite StSprCannion[]={
	{enTexPageEffectStage01,0,0   ,512,128 , 0 , 0},	//岩
	{enTexPageEffectStage01,0,128 ,512,128 , 0 , 0},	//地面

	{enTexPageEffectStage01,512,0 ,512,128 , 0 , 0},	//山
};

Sint32 CBaseExtra::g_sOverLayerHeight = 0;

void CBaseExtra::Init()
{
	//---------------------------------------------------------------------------
	//初期化
	//---------------------------------------------------------------------------
	SetFadeIn();

	//---------------------------------------------------------------------------
	//エリア初期化
	//---------------------------------------------------------------------------

	if( m_bFirst )
	{
		//通常処理
		CGXImage::GetInstance()->Load( "asset/gh/GXI/extra.GXI" );
	}

	new CEffScene0A02(320*100,128*100,PRIO_BG1-1,0);

	pGame->GetHound()->SetHyperBooster();

	switch(GetArea( )){
	case enAreaSeq01:
	case enAreaSeq02:
	case enAreaSeq03:
	case enAreaSeq04:
	case enAreaSeq05:
		if( g_StGameInfo.m_sGameMode == StGameInfo::enGameModeMission )
		{
			new CItemBox( 5471*100, 200*100 , 0 );
		}
///		LoadConfig( "HoundData\\ScenExtra@01.txt" );
///		viiMus::ReadBGM( 0,"HoundData\\bgm\\A\\A5_extra.ogg");
		SetDRQStage( "ScenExtra.txt" );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		AppendBgm();
		ChangeArea( enAreaSeq01 );
		break;
	case enAreaSeq06:
		SetDRQStage( "ScenExtra.txt" );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		AppendBgm();
		ChangeArea( enAreaSeq06 );
		break;
	case enAreaSeqBoss:
		SetDRQStage( "ScenExtra.txt" );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		SetGamePlay( gxTrue );
		AppendBgm();

		m_bEscape = !g_StGameInfo.m_bTrueEnd;

		ChangeArea( enAreaSeqBoss );
		return;

	case enAreaDebug:
		SetDRQStage( "ScenExtra.txt" );
		AppendBgm();
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		break;
	}

	SetGamePlay( gxTrue );

	InitEnemies();

}

void CBaseExtra::AppendBgm()
{
	//---------------------------------------------------------------------------
	//追加BGM
	//---------------------------------------------------------------------------
	//特別処理
	CGXImage::GetInstance()->Load( "asset/gh/GXI/BASEA.GXI" );

	Uint32 sSize;

	Uint8 *p;

	p = CGXImage::GetInstance()->GetFile( "bgm\\extra.wav" , &sSize );

	viiMus::ReadBGM( enSoundBgm2 , p , sSize );

}


void CBaseExtra::End()
{
	//---------------------------------------------------------------------------
	//終了処理
	//---------------------------------------------------------------------------


}

void CBaseExtra::InitEnemies()
{
	//---------------------------------------------------------------------------
	//敵の初期化
	//---------------------------------------------------------------------------

	SetEnemy();

}


void CBaseExtra::Action()
{
	//---------------------------------------------------------------------------
	//毎フレームのアクション
	//---------------------------------------------------------------------------

//	pGame->pBg->SetFilter( ARGB( 0xff ,0xa0 , 0x80 , 0x80 ) );

	StageCommon();
	AreaCommon();
//	pGame->pBg->DebugTipDraw(gxFalse);

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

	case enAreaSeq06:
		Area06();
		break;
	case enAreaSeqBoss:
		Area07();
		break;

	case enAreaDebug:
		AreaTest();
		break;
	}

//	viiDbg::printf(100,0,"ZoneTime= %d",m_sZoneTimer);
//	viiDbg::printf(100,16,"Area    = %d",GetArea( ) );

}

void CBaseExtra::Area01()
{
	//---------------------------------------------------------------------------
	//エリア１　～戦艦～前部砲台破壊まで
	//---------------------------------------------------------------------------
	m_sScrollYMin = enScroiiMinY;
	m_sScrollYMax = m_Pos.y;
	m_sScrollMin  = 2000*100;
	m_sScrollMax  = 6300*100;

	m_sBgScroll  += 400;
	m_sBgScroll = m_sBgScroll%200000;

//	m_sScrollYMax = m_Pos.y;
//	m_sScrollYMin = enScroiiMinY;

	if( IsZoneSeq(0) )
	{
		SetFadeIn();
#ifdef _VII_DEBUG_
		CCockpit::GetInstance()->SetMissionSec( 60*4 );
#else
		CCockpit::GetInstance()->SetMissionSec( enMissionTime );
#endif

		m_sScrollMin = 2000*100;
		m_sScrollMin = 4000*100;
		m_sScrollMax = 4900*100;

		SetTargetPlayerPosX( 4175*100 );
		SetTargetPlayerPosY( 800*100 );

		pGame->pBg->SetPriority(enBgTypeRear , PRIO_BG2);
		pGame->pBg->SetPriority(enBgTypeFrnt , PRIO_BG3);

		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );	//4058*100,723*100 );//

		m_Pos.x = 5550*100;
		m_Pos.y = 732*100;

		m_pBodyPanel = new CEneEx0009Body( 6010*100 , 795*100 );
		m_pSubCannon[0] = new CEneEx0002Laser( 5153*100 , 800*100 );

		m_pMainCannon = new CEneEx0001Cannon( 5478*100 , 524*100 );
		m_pCannonCore = new CEneEx0005Wii(5635*100 , 647*100 );

		pGame->GetHound()->m_Pos.x = 3500*100;

		m_pGunHound[0] = new CGunHound( 4680*100 , 900*100 , gxFalse );
		m_pGunHound[0]->SetLeader(gxTrue);
		m_pGunHound[0]->SetHyperBooster();
		m_pGunHound[0]->SetForceRollerDash(gxFalse);
		m_pGunHound[0]->SetMode( CGunHound::enHoundModeNPC );
		m_pGunHound[0]->m_Pos.x = 5929*100;
		m_pGunHound[0]->m_Pos.y = 700*100;
		m_pGunHound[0]->SetLogicPattern( 1 );
		m_pGunHound[0]->SetNoDamage( gxFalse );

		m_pGunHound[1] = new CGunHound( 4480*100 , 900*100 , gxFalse );
		m_pGunHound[1]->SetHyperBooster();
		m_pGunHound[1]->SetForceRollerDash(gxTrue);
		m_pGunHound[1]->SetMode( CGunHound::enHoundModeNPC );
		m_pGunHound[1]->SetHyperBooster();
		m_pGunHound[1]->m_Pos.x = 3000*100;
		m_pGunHound[1]->m_Pos.y = 900*100;
		m_pGunHound[1]->m_Pos.z = PRIO_PLAYER-2;
		m_pGunHound[1]->SetLogicPattern( 360 );

		SetGamePlay( gxTrue );
		ResetZoneTimer();
		SetZoneSeq(100);
	}
	else if( IsZoneSeq(100) )
	{
		//前部砲台を破壊

		if( m_sZoneTimer == 20 )
		{
			viiMus::PlayBGM( enSoundBgm1,enMusicBgmBossDeluxe);
		}

		if( pGame->GetHound()->m_Pos.x < 4500*100)
		{
			if( viiSub::Rand()%32 == 0 )
			{
				new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , pGame->GetHound()->m_Pos.x+(-128+viiSub::Rand()%512)*100,960*100 , PRIO_ENEMY , -45 ,180 );
			}
			if( viiSub::Rand()%32 == 0 )
			{
				new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , pGame->GetHound()->m_Pos.x+(-128+viiSub::Rand()%512)*100,960*100 , PRIO_EFFECT , -45 ,180 );
			}
		}

		m_sScrollMax = 4900*100;

		if( m_sZoneTimer == 240 )
		{
			CCockpit::GetInstance()->SetMessage( 5000 );	//陸上戦闘艦がエリアＤに進行
		}

		if( m_pSubCannon[0] == NULL )
		{
			//フロントキャノンを破壊した
			CCockpit::GetInstance()->SetMessage( 5100 );	//戦闘艦の熱源反応が拡大中。
			ChangeArea( enAreaSeq02 );
		}
	}

	LayerScroll[enBgTypeBack].ox = -m_sBgScroll/100;
	LayerScroll[enBgTypeFrnt].oy = (viiMath::Cos100( (viiSub::GameCounter()*8)%360 )*3)/100;
	LayerScroll[enBgTypeRear].oy = (viiMath::Cos100( (viiSub::GameCounter()*8)%360 )*2)/100;

	BgCommon();
	viiSub::LimitMoveVH();

}



void CBaseExtra::Area02()
{
	//---------------------------------------------------------------------------
	//エリア２　～ミドルキャノン～エンジンブロック破壊まで
	//---------------------------------------------------------------------------
	m_sScrollYMin = enScroiiMinY;
	m_sScrollYMax = m_Pos.y;
	m_sScrollMin  = 2000*100;
	m_sScrollMax  = 6500*100;

	m_sBgScroll  += 400;
	m_sBgScroll = m_sBgScroll%200000;

	if( IsZoneSeq(0) )
	{
		m_sScrollMax  = 5400*100;
//		m_sScrollYMax = m_Pos.y;
//		m_sScrollYMin = enScroiiMinY;

		pGame->pBg->SetPriority(enBgTypeRear , PRIO_BG2);
		pGame->pBg->SetPriority(enBgTypeFrnt , PRIO_BG3);

		m_pSubCannon[1] = new CEneEx0002Laser( 5650*100 , 750*100 );

		ResetZoneTimer();
		SetZoneSeq(200);
	}
	else if( IsZoneSeq(200) )
	{
		//-------------------------------------
		//ミドルサブキャノン破壊待ち
		//-------------------------------------
		m_sScrollMax  = 5400*100;

		if( m_pSubCannon[1] == NULL )
		{
			//---------------------------------
			//ノズルを作成
			//---------------------------------
			m_pNozzle[0] = new CEneEx0007Nozzle( 6470*100, (738)*100);
			m_pNozzle[1] = new CEneEx0007Nozzle( 6470*100, (832)*100);

			//---------------------------------
			//リアキャノンを作成
			//---------------------------------
			m_pSubCannon[2] = new CEneEx0002Laser( 6400*100 , 770*100 );
			m_pSubCannon[2]->SetType( 1 );	//＋－45度でしか動かない。上空砲弾を打ち上げる

			ResetZoneTimer();
			SetZoneSeq(300);
		}
	}
	else if( IsZoneSeq(300) )
	{
		//-------------------------------------
		//エンジンブロック破壊待ち
		//-------------------------------------

		if( m_bEngineBlockCompleted )
		{
			CCockpit::GetInstance()->SetMessage( 5300 );	//戦闘艦のエンジン出力が減少。
			ResetZoneTimer();
			SetZoneSeq(500);
		}

	}
	else if( IsZoneSeq(500) )
	{
		//-------------------------------------
		//全ての砲台、エンジン部を破壊してクリア
		//-------------------------------------

		if( !CCockpit::GetInstance()->IsMessageNow() )
		{
			if( m_sZoneTimer >= 120 )
			{
				ChangeArea( enAreaSeq03 );
			}
		}
	}

	LayerScroll[enBgTypeBack].ox = -m_sBgScroll/100;
	LayerScroll[enBgTypeFrnt].oy = (viiMath::Cos100( (viiSub::GameCounter()*8)%360 )*3)/100;
	LayerScroll[enBgTypeRear].oy = (viiMath::Cos100( (viiSub::GameCounter()*8)%360 )*2)/100;

	BgCommon();
	viiSub::LimitMoveVH();

}


void CBaseExtra::Area03()
{
	//-----------------------------------------------------
	//エリア３:コア破壊
	//-----------------------------------------------------

	m_sScrollYMin = enScroiiMinY;
	m_sScrollYMax = m_Pos.y;
	m_sScrollMin  = 2000*100;
	m_sScrollMax  = 6300*100;

	m_sBgScroll  += 250;
	m_sBgScroll = m_sBgScroll%200000;

	if( IsZoneSeq(0) )
	{
		pGame->pBg->SetPriority(enBgTypeRear , PRIO_BG2);
		pGame->pBg->SetPriority(enBgTypeFrnt , PRIO_BG3);

		ResetZoneTimer();
		SetZoneSeq(100);

	}
	else if( IsZoneSeq(100) )
	{
		//-------------------------------
		//全コアを破壊した
		//-------------------------------

		if( m_bMiddleTankDead && m_bFrontTankDead && m_bRearTankDead)
		{
			if( m_pMainCannon == NULL )
			{
				m_pBodyPanel->SetAtariHantei();					//パネルの判定発生
				SetZoneSeq(200);
			}
		}
	}
	else if( IsZoneSeq(200) )
	{
		//-----------------------------------------------
		//ボディパネルを破壊
		//-----------------------------------------------
		if( m_bBodyTankEnd )
		{
			//パネル裏のボディタンクを破壊した
			ResetZoneTimer();
			SetZoneSeq(300);
		}

	}
	else if( IsZoneSeq(300) )
	{
		//-----------------------------------------------
		//破壊演出
		//-----------------------------------------------
		if( m_sZoneTimer >= 120 )
		{
			ChangeArea( enAreaSeq04 );
		}
	}

	m_sScrollYMax = m_Pos.y;
	m_sScrollYMin = enScroiiMinY;

	LayerScroll[enBgTypeBack].ox = -m_sBgScroll/100;
	LayerScroll[enBgTypeFrnt].oy = (viiMath::Cos100( (viiSub::GameCounter()*8)%360 )*3)/100;
	LayerScroll[enBgTypeRear].oy = (viiMath::Cos100( (viiSub::GameCounter()*8)%360 )*2)/100;

	BgCommon();
	viiSub::LimitMoveVH();

}


void CBaseExtra::Area04()
{
	//-----------------------------------------------------
	//エリア４：ラスボス（ここまでくれば逃げられればとりあえずクリア可能）
	//-----------------------------------------------------
	m_sScrollYMin = enScroiiMinY;
	m_sScrollYMax = m_Pos.y;
	m_sScrollMin  = 2000*100;
	m_sScrollMax  = 6300*100;

	m_sBgScroll  += 250;
	m_sBgScroll = m_sBgScroll%200000;

	m_sScrollMax = 8000*100;

	if( m_pGunHound[0] && m_pGunHound[0]->IsDead() )
	{
		//ザキ死亡確認
		m_bZakiDead = gxTrue;
		CCockpit::GetInstance()->SetMessage( 5400 );	//！
	}

	if( IsZoneSeq(0) )
	{
		CCockpit::GetInstance()->SetMessage( 5350 );	//隊長、なかからワラワラと敵が
		m_pCannonCore->SetAtariHantei();				//最終コアをセット

		m_Pos.y = 732*100;

		if( m_pGunHound[0] == NULL )
		{
			m_pGunHound[0] = new CGunHound( 4680*100 , 900*100 , gxFalse );
			m_pGunHound[0]->SetLeader(gxTrue);
			m_pGunHound[0]->SetHyperBooster();
			m_pGunHound[0]->SetForceRollerDash(gxFalse);
			m_pGunHound[0]->SetMode( CGunHound::enHoundModeNPC );
			m_pGunHound[0]->m_Pos.x = 5900*100;
			m_pGunHound[0]->m_Pos.y = 760*100;
		}

		m_pGunHound[0]->m_Pos.z = PRIO_PLAYER -1 ;
		m_pGunHound[0]->SetLogicPattern( 550 );
		m_pGunHound[0]->SetNoDamage( gxFalse );
		m_pGunHound[0]->SetTarget( 5850*100, 0 );

		SetZoneSeq(100);
	}
	else if( IsZoneSeq(100) )
	{
		if( !CCockpit::GetInstance()->IsMessageNow() )
		{
			new CItemChip( m_pGunHound[0]->m_Pos.x, m_pGunHound[0]->m_Pos.y, enItemEnergyChip);
			SetZoneSeq(200);
		}
	}
	else if( IsZoneSeq(200) )
	{
		//----------------------------------------
		//ファイナルコアを破壊したか？
		//----------------------------------------

		CCockpit::GetInstance()->SetArrow( 0 );

		if( m_pCannonCore && m_pCannonCore->IsDead() )
		{
			//最終コアを破壊
			CCockpit::GetInstance()->SetMessage( 9100 );	//エネルギーが臨界点を突破します！
			m_pCannonCore->SetHakkyouMode();
			ResetZoneTimer();
			SetZoneSeq(500);
		}
		else if( pGame->GetHound()->m_Pos.x <= 2000*100 || pGame->GetHound()->m_Pos.x >= 8000*100 )
		{
			//ESCAPEへ
			ResetZoneTimer();
			SetZoneSeq(1000);
		}
	}
	else if( IsZoneSeq(500) )
	{
		//----------------------------------
		//リボーンファイナルコアを破壊した
		//----------------------------------
		CCockpit::GetInstance()->SetArrow( 0 );

		FinalAtack();

		if( m_sZoneTimer == 60*10 )
		{
			if( m_bZakiDead )
			{
				CCockpit::GetInstance()->SetMessage( 5410 );	//バツを・・・頼むぜ
			}
			else
			{
				CCockpit::GetInstance()->SetMessage( 5360 );	//ここは任せて、バツをつれて逃げろ！
			}
		}

		if( m_sZoneTimer == 60*20 )
		{
			if( m_bZakiDead )
			{
				CCockpit::GetInstance()->SetMessage( 5420 );	//・ライ・・・・・・・・・
			}
			else
			{
				CCockpit::GetInstance()->SetMessage( 5370 );	//ライザ、撤退しろっ！
			}
		}

		if( m_pCannonCore && m_pCannonCore->IsDead() )
		{
			//コアを破壊した
			m_pCannonCore->Clear();
			m_pCannonCore = NULL;
			ResetZoneTimer();
			SetZoneSeq(2000);
		}
		else if( pGame->GetHound()->m_Pos.x <= 2000*100 || pGame->GetHound()->m_Pos.x >= 8000*100 )
		{
			//ESCAPEへ
			ResetZoneTimer();
			SetZoneSeq(1000);
		}

	}
	else if( IsZoneSeq(1000) )
	{
		//----------------------------------
		//ESCAPEエンドへ
		//----------------------------------
		m_bEscape = gxTrue;
		viiMus::StopBGM( enSoundBgm1 , 60*4 );
		CCockpit::GetInstance()->SetMessage( 6000 );	//あれ・・・？

		//BADエンディングを設定
		g_StGameInfo.m_bTrueEnd = gxFalse;

		if( !CCockpit::GetInstance()->IsMessageNow() )
		{
			SetFadeOut();
			SetZoneSeq(3000);
		}
	}
	else if( IsZoneSeq(2000) )
	{
		//----------------------------------
		//TRUEエンディングへSeq１
		//----------------------------------
		CCockpit::GetInstance()->SetArrow( 0 );

		m_bEscape = gxFalse;

		//TRUEエンディングを設定
		g_StGameInfo.m_bTrueEnd = gxTrue;

		//GOOD
		if( m_sZoneTimer >= 60*4 )
		{
			viiMus::StopBGM( enSoundBgm1 , 60*4 );
			CCockpit::GetInstance()->SetMessage( 6100 );
			SetZoneSeq(2100);
		}
	}
	else if( IsZoneSeq(2100) )
	{
		//----------------------------------
		//TRUEエンディングへSeq２
		//----------------------------------
		if( !CCockpit::GetInstance()->IsMessageNow() )
		{
			SetFadeOut();
			SetZoneSeq(3000);
		}
	}
	else if( IsZoneSeq(3000) )
	{
		//----------------------------------
		//合流地点
		//----------------------------------

		SetZoneSeq(4000);
	}
	else if( IsZoneSeq(4000) )
	{
		//----------------------------------
		//ラスボス戦へ
		//----------------------------------
		if( !viiMus::IsPlayBGM( enSoundBgm1 ) )
		{
			if( IsFadeEnd() )
			{
				ChangeArea( enAreaSeqBoss );
			}
		}
	}

	m_sBgScroll  += 250;
	m_sBgScroll = m_sBgScroll%200000;

	m_sScrollYMax = m_Pos.y;
	m_sScrollYMin = enScroiiMinY;

	LayerScroll[enBgTypeBack].ox = -m_sBgScroll/100;
	LayerScroll[enBgTypeFrnt].oy = (viiMath::Cos100( (viiSub::GameCounter()*8)%360 )*3)/100;
	LayerScroll[enBgTypeRear].oy = (viiMath::Cos100( (viiSub::GameCounter()*8)%360 )*2)/100;

	BgCommon();
	viiSub::LimitMoveVH();

}


void CBaseExtra::Area07()
{
	//-----------------------------------------------------
	//ラスボス戦
	//-----------------------------------------------------
	m_bLastBoss = gxTrue;

	m_sScrollMin  = 5000*100;
	m_sScrollMax  = 6300*100;
	m_sScrollYMin = enScroiiMinY;
	m_sScrollYMax = m_Pos.y;

	//背景のスクロール

	if( m_sHighSpeed < 1250 ) m_sHighSpeed += 8;
	m_sBgScroll  += m_sHighSpeed;
	m_sBgScroll   = m_sBgScroll%200000;

	m_sScrollYMax = m_Pos.y;
	m_sScrollYMin = enScroiiMinY;

	LayerScroll[enBgTypeBack].ox = -m_sBgScroll/100;
	LayerScroll[enBgTypeFrnt].oy = (viiMath::Cos100( (viiSub::GameCounter()*8)%360 )*3)/100;
	LayerScroll[enBgTypeRear].oy = (viiMath::Cos100( (viiSub::GameCounter()*8)%360 )*2)/100;

	m_ScrPos.x = -LayerScroll[enBgTypeFrnt].ox*100;

	FinalAtack();

	if( IsZoneSeq( 0 ) )
	{
		SetFadeIn();
		CCockpit::GetInstance()->SetBlackMask( gxTrue );

		m_Pos.y = 732*100;

		ResetEnemy();

		m_pBoss = new CEneH0504Boss(5133*100, 850*100);

		pGame->GetHound()->m_Pos.x = 5600*100;
		pGame->GetHound()->m_Pos.y = 950*100;
		pGame->GetHound()->SetLeft();
		pGame->GetHound()->SetMode( CGunHound::enHoundModeNone );
		pGame->GetHound()->SetAdd(0,0);

		pGame->pBg->SetPriority(enBgTypeRear , PRIO_BG3+0);
		pGame->pBg->SetPriority(enBgTypeFrnt , PRIO_BG3+1);

		viiMus::PlayBGM( enSoundBgm2 , enMusicBgmStageEx );
/*
		if( m_pGunHound[1] == NULL )
		{
			m_pGunHound[1] = new CGunHound( 4480*100 , 900*100 , gxFalse );
		}

		m_pGunHound[1]->SetHyperBooster();
		m_pGunHound[1]->SetForceRollerDash(gxTrue);
		m_pGunHound[1]->SetMode( CGunHound::enHoundModeNPC );
		m_pGunHound[1]->SetHyperBooster();
		m_pGunHound[1]->m_Pos.x = pGame->GetHound()->m_Pos.x+64*100;
		m_pGunHound[1]->m_Pos.y = pGame->GetHound()->m_Pos.y;
		m_pGunHound[1]->m_Pos.z = PRIO_PLAYER-2;
		m_pGunHound[1]->SetLogicPattern( 360 );
*/
		Sint32 x = m_pBoss->m_Pos.x-256*100;
		Sint32 y = m_pBoss->m_Pos.y-128*100;
		x = 4877*100;
		y = 522*100;
		viiSub::SetScroll_l( x,y );
		ResetZoneTimer();
		SetZoneSeq( 200 );
		return;
	}
	else if( IsZoneSeq( 200 ) )
	{
		//-------------------------
		//ボス登場
		//-------------------------
		Sint32 x = m_pBoss->m_Pos.x-256*100;
		Sint32 y = m_pBoss->m_Pos.y-128*100;
		if( y >= 513*100 ) y = 513*100;

		BgCommon( x,y );

		if( m_pBoss->m_Pos.y < 500*100 )
		{
			m_pBoss->Go();
			ResetZoneTimer();
			SetZoneSeq( 300 );
		}
		return;
	}
	else if( IsZoneSeq( 300 ) )
	{
		//-------------------------
		//位置合わせ
		//-------------------------
		pGame->pBg->SetPriority(enBgTypeRear , PRIO_BG1+0);
		pGame->pBg->SetPriority(enBgTypeFrnt , PRIO_BG1+1);

		Sint32 x = m_pBoss->m_Pos.x-256*100;
		Sint32 y = m_pBoss->m_Pos.y-128*100;
		if( y >= 513*100 ) y = 513*100;

		BgCommon( x,y );

		if( m_sZoneTimer >= 120 )
		{
			CCockpit::GetInstance()->SetBlackMask( gxFalse );
	//		pGame->GetHound()->SetMode( CGunHound::enHoundModeNormal );
			pGame->GetHound()->SetCarryOff();
			ClearVmpTip( 4781,500 , 6587,912 );
			ResetZoneTimer();
			SetZoneSeq( 400 );
		}
		return;
	}
	else if( IsZoneSeq( 400 ) )
	{
		//------------------------------------------
		//自機位置合わせ
		//------------------------------------------
		if( m_ScrAdd.x < 300 )
		{
			m_ScrAdd.x += 1;
		}

		LayerScroll[enBgTypeFrnt].ox -= m_ScrAdd.x/100;
		LayerScroll[enBgTypeRear].ox -= m_ScrAdd.x/100;

		if( m_sZoneTimer >= 200 )
		{
			SetZoneSeq( 900 );
			ResetZoneTimer();
		}
		else
		{
			BgCommon( );
			return;
		}
	}
	else if( IsZoneSeq( 900 ) )
	{
		//------------------------------------------
		//ボス戦
		//------------------------------------------

		if( m_ScrAdd.x < 300 )
		{
			m_ScrAdd.x += 1;
		}

		LayerScroll[enBgTypeFrnt].ox -= m_ScrAdd.x/100;
		LayerScroll[enBgTypeRear].ox -= m_ScrAdd.x/100;
		if( LayerScroll[enBgTypeFrnt].ox >= -2205 )
		{
			if( viiSub::Rand()%24  == 0 )
			{
				switch( viiSub::Rand()%3 ){
				case 0:
					viiMus::PlaySound(enSoundExplosionMiddle);
					break;
				case 1:
					viiMus::PlaySound(enSoundExplosionSmall);
					break;
				case 2:
					viiMus::PlaySound(enSoundExplosionLarge);
					break;
				}
			}
		}

		if( m_pBoss->IsCrash() )
		{
			viiMus::StopBGM( enSoundBgm2 , 300 );
			pGame->GetHound()->SetMuteki();
			SetZoneSeq( 920 );
		}

	}
	else if( IsZoneSeq( 920 ) )
	{
		if( m_ScrAdd.x < 300 )
		{
			m_ScrAdd.x += 1;
		}

		LayerScroll[enBgTypeFrnt].ox -= m_ScrAdd.x/100;
		LayerScroll[enBgTypeRear].ox -= m_ScrAdd.x/100;

		if( m_pBoss->IsDead() )
		{
			m_pBoss->Clear();
			m_pBoss = NULL;
			SetZoneSeq( 999 );
		}

	}
	else if( IsZoneSeq( 999 ) )
	{
		//--------------------------------------
		//ステージクリア
		//--------------------------------------

		if( m_bEscape )
		{
			//逃げていた場合にはBADエンド
			ChangeArea( enAreaSeq05 );
		}
		else
		{
			//生きていた場合にはGOODエンド
			ChangeArea( enAreaSeq06 );
		}
	}

	if( m_pBoss && m_pBoss->IsChumoku() )
	{
		Sint32 x = m_pBoss->m_Pos.x-256*100;
		Sint32 y = m_pBoss->m_Pos.y-128*100;

		BgCommon( x,y );
		return;
	}

	m_bEngineBlockCompleted = gxTrue;

	BgCommon();
	viiSub::LimitMoveVH();
}


void CBaseExtra::Area05()
{
	//-----------------------------------------------------
	//エスケープクリア(BAD)
	//-----------------------------------------------------
	m_sScrollYMin = enScroiiMinY;
	m_sScrollYMax = m_Pos.y;
	m_sScrollMin  = 2000*100;
	m_sScrollMax  = 6300*100;

	m_sBgScroll  += 520;
	m_sBgScroll = m_sBgScroll%200000;

	m_sScrollMax = 8000*100;

	CCockpit::GetInstance()->SetSystemSound(gxFalse);

	if( IsZoneSeq( 0 ) )
	{
		//クリア
		SetGamePlay( gxFalse );
		pGame->GetHound()->SetMuteki();

		if( g_StGameInfo.m_sGameMode == StGameInfo::enGameModeMission )
		{
			CCockpit::GetInstance()->SetStageClear();
		}
		else
		{
			if( m_pCannonCore )
			{
				//-------------------------
				//BADエンド認定
				//-------------------------

			}
			else
			{
				//-------------------------
				//-------------------------
			}

		}

		SetZoneSeq( 10 );
	}
	else if( IsZoneSeq( 10 ) )
	{
		if( g_StGameInfo.m_sGameMode == StGameInfo::enGameModeMission )
		{
			if( CCockpit::GetInstance()->IsClearOver() )
			{
				CCockpit::GetInstance()->DestroyStageClear();
				SetFadeOut(2);
				SetZoneSeq( 20 );
			}
		}
		else
		{
			if( !CCockpit::GetInstance()->IsMessageNow() )
			{
//				CFadeManager::GetInstance()->set_fadeout(1,0xffffffff);
				viiMus::StopBGM( enSoundBgm1 , 255 );
				SetZoneSeq( 20 );
			}
		}
	}
	else if( IsZoneSeq( 20 ) )
	{
		//フェード終了待ち
		if( IsFadeEnd() )
		{
			ResetZoneTimer();
			SetZoneSeq( 30 );
		}
	}
	else if( IsZoneSeq( 30 ) )
	{
		SetZoneSeq( 100 );
		switch( m_sZoneTimer ){
		case 60:
//			viiMus::PlaySound( enSoundExplosionMaximum );
			break;
		case 120:
//			viiMus::PlaySound( enSoundCrashB );
			break;
		case 300:
//			viiMus::PlaySound( enSoundCrashA );
			break;
		case 500:
//			SetZoneSeq( 100 );
			break;
		}
	}
	else if( IsZoneSeq( 100 ) )
	{
		pGame->stage_clear(enClearSeqEnding);
//		pGame->stage_clear(enClearSeqStageClear);
		ChangeArea( GAME_NONE );	//ステージ終了
	}

	BgCommon();
	viiSub::LimitMoveVH();
}


void CBaseExtra::Area06()
{
	//-----------------------------------------------------
	//破壊クリア(TRUE)
	//-----------------------------------------------------
	m_sScrollYMin = enScroiiMinY;
	m_sScrollYMax = m_Pos.y;
	m_sScrollMin  = 2000*100;
	m_sScrollMax  = 6300*100;

	m_sBgScroll  += 520;
	m_sBgScroll  = m_sBgScroll%200000;

	m_sScrollMax = 8000*100;

	CCockpit::GetInstance()->SetSystemSound(gxFalse);

	if( IsZoneSeq( 0 ) )
	{
		//クリア
		SetGamePlay( gxFalse );
		pGame->GetHound()->SetMuteki();

		if( g_StGameInfo.m_sGameMode == StGameInfo::enGameModeMission )
		{
			CCockpit::GetInstance()->SetStageClear();
		}
		else
		{
			//TRUEエンディングを設定
			if( !m_bZakiDead )
			{
				//特に関係なし
			}

			if( m_pCannonCore )
			{
				//-------------------------
				//BADエンド認定
				//-------------------------

			}
			else
			{
				//-------------------------
				//GOODエンド
				//-------------------------
			}
		}

		SetZoneSeq( 10 );
	}
	else if( IsZoneSeq( 10 ) )
	{
		if( g_StGameInfo.m_sGameMode == StGameInfo::enGameModeMission )
		{
			if( CCockpit::GetInstance()->IsClearOver() )
			{
				CCockpit::GetInstance()->DestroyStageClear();
				SetFadeOut(2);
				SetZoneSeq( 20 );
			}
		}
		else
		{
			if( !CCockpit::GetInstance()->IsMessageNow() )
			{
//				CFadeManager::GetInstance()->set_fadeout(1,0xffffffff);
				viiMus::StopBGM( enSoundBgm1 , 255 );
				SetZoneSeq( 20 );
			}
		}
	}
	else if( IsZoneSeq( 20 ) )
	{
		//フェード終了待ち
		if( IsFadeEnd() )
		{
			ResetZoneTimer();
			SetZoneSeq( 30 );
		}
	}
	else if( IsZoneSeq( 30 ) )
	{
		SetZoneSeq( 100 );
		switch( m_sZoneTimer ){
		case 60:
//			viiMus::PlaySound( enSoundExplosionMaximum );
			break;
		case 120:
//			viiMus::PlaySound( enSoundCrashB );
			break;
		case 300:
//			viiMus::PlaySound( enSoundCrashA );
			break;
		case 500:
//			SetZoneSeq( 100 );
			break;
		}
	}
	else if( IsZoneSeq( 100 ) )
	{
		pGame->stage_clear(enClearSeqEnding);
//		pGame->stage_clear(enClearSeqStageClear);
		ChangeArea( GAME_NONE );	//ステージ終了
	}

	BgCommon();
	viiSub::LimitMoveVH();
}


void CBaseExtra::Area08()
{
	
	
	
}


void CBaseExtra::FinalAtack()
{
	//------------------------------------------
	//発狂モード
	//------------------------------------------
	Sint32 sWaitTiming = 60;
	Sint32 ax = 5650*100+m_ScrPos.x;
	Sint32 ay = 770*100;

	if(m_bLastBoss)
	{
		ax = 4900*100 + (viiSub::Rand()%360)*100+m_ScrPos.x;
		ay = 740*100  + (viiSub::Rand()%64)*100;

		if( m_sZoneTimer%14==0 )
		{
			CEffBigBomb *p;

			p = new CEffBigBomb( ax , ay , 0 , 0 );
			p->m_Pos.z = PRIO_BG1;
			p->SetYubaku(gxFalse);

			viiEff::SetBombDeluxe( ax , ay , 1 , 1+viiSub::Rand()%2 );
			viiEff::SetBombDeluxe( ax , 740*100 , 1 , 1+viiSub::Rand()%2 );
		}

		if( viiSub::Rand()%16==0 )
		{
			ax = 5650*100 + (viiSub::Rand()%640)*100+m_ScrPos.x;
			ay = 770*100;

			CEffBigBomb *p;
			p = new CEffBigBomb( ax , ay , 0 , 0 );
			p->m_Pos.z = PRIO_BG2+1;
			p->SetYubaku(gxFalse);
		}
		//return;
	}

	if( m_pGunHound[0] )
	{
		viiDbg::printf( 32 , 64 , "GunHound Master HP = %d" , m_pGunHound[0]->GetHitPoint( ) );
	}

	if( m_sFinalAtackWait > 0 )
	{
		m_sFinalAtackWait --;
	}

	if( m_sFinalAtackWait == 0 )
	{
		m_sFinalAtackWait = sWaitTiming*5;

		//--------------------------------------
		//回転スーパーボール
		//--------------------------------------

		for(Sint32 ii=0;ii<360;ii+=24 )
		{
			if(!m_bLastBoss) new CEne0001EneSuperBall( ax , ay ,  ii ,220 );
		}
	}

	if( m_sFinalAtackWait < sWaitTiming*2 ) return;
	if( m_sFinalAtackWait > sWaitTiming*4 ) return;

	if( m_sFinalAtackWait%4 == 0 )	//(viiSub::GameCounter())%4==0)
	{
		ax = 5650*100 + (viiSub::Rand()%160)*100+m_ScrPos.x;
		ay = 770*100  ;//+ (-50+viiSub::Rand()%100)*100;

		{
			Sint32 sRotation = get_valken_rot( ax , ay+2400 );

			CEffBigBomb *p;
			p = new CEffBigBomb( ax , ay , 0 , 0 );
			p->m_Pos.z = PRIO_BG2+1;
			p->SetYubaku(gxFalse);

//			if (ax < 5750*100 )
			{
				sRotation -= 15;
				for(Sint32 ii=0;ii<360;ii+=30)
				{
					ax += viiMath::Cos100(sRotation+ii)*8;
					ay += viiMath::Sin100(sRotation+ii)*8;
					if(!m_bLastBoss) new CEne0001EneBullet( ax , ay , sRotation+ii     , 380*2 );
				}
			}
		}
	}

}


void CBaseExtra::AreaCommon()
{
	//-----------------------------------------------------
	//エリア：共通
	//-----------------------------------------------------

	m_sLoop += 8;
	if( m_sLoop >= 480)
	{
		m_sLoop -= 480;
	}
}


//---------------------------------------------------------------------------
//ＶＭＰから敵の生成
//---------------------------------------------------------------------------


void CBaseExtra::SetEnemy()
{
	for(Sint32 y=0; y<pGame->pBg->getHeight()/8; y++)
	{
		for(Sint32 x=0; x<pGame->pBg->getWidth()/8; x++)
		{
			Sint32 sIndex = pGame->pBg->get_vmpdat(x,y);

			sIndex = EnemyIndex[sIndex];

			switch(sIndex){
			case enCEneH0501PlasmaMachine:
				//m_pEnergyRear = new CEneH0501PlasmaMachine( x*800 , y*800 );
				break;

			case enCEneH0101HeavyTank:	//味方重戦車
				break;

/*
			case 302:
				new CEneEx0003Shot( x*800 , y*800 , 1 );
				break;

			case 303:
				new CEneEx0003Shot( x*800 , y*800 , 0);
				break;
*/

			default:
				MakeHoundsEnemy(x*800,y*800,sIndex);
				break;
			}

		}
	}

}


void CBaseExtra::BgCommon(Sint32 x,Sint32 y)
{
	//----------------------------------------------------
	//背景共通
	//----------------------------------------------------
	Sint32 col1 =  0xFFC02020;
	Sint32 col2 =  0xFFA06020;
//	Sint32 col1 =  0xFF202040;
//	Sint32 col2 =  0xDF205080;
	pGame->pBg->SetSkyColor(80,col1,col2);

	Gimmick();

	if( x==0 && y == 0 )
	{
		BgScrollAdjust();
	}
	else
	{
//		viiSub::AdjScroll_l( sx,sy );
//		viiSub::SetScroll_l( x,y );//  SetScroll_s( sx,sy );
		viiSub::AdjScroll_l( x,y );
//	viiSub::GetScroll_s( sx,sy );

		viiSub::SetBgDisp ( enBgTypeStar , gxFalse );

	}

	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );

	viiSub::SetBgDisp ( enBgTypeStar , gxFalse );
	viiSub::SetBgLayer( enBgTypeStar , sx*LayerScroll[enBgTypeBack].x/10,sy*LayerScroll[enBgTypeBack].y/100 );

	StLayerScroll *pScr;

	for(Sint32 ii=enBgTypeBack; ii<=enBgTypeFrnt; ii++)
	{
		pScr = &LayerScroll[ii];
		viiSub::SetBgLayer( ii , pScr->ox+(sx*pScr->x)/100 , pScr->oy+(sy*pScr->y)/100 );
	}

//	viiSub::SetBgLayer( enBgTypeFrnt , (pScr->ox+(sx*pScr->x)+(-m_sLoop*100))/100 , pScr->oy+(sy*pScr->y)/100 );

	m_Scroll[0].x += 800;
	m_Scroll[0].x = m_Scroll[0].x%51200;

	m_Scroll[1].x += 600;
	m_Scroll[1].x = m_Scroll[1].x%51200;

	m_Scroll[2].x += 10;
	m_Scroll[2].x = m_Scroll[2].x%51200;

}


void CBaseExtra::Gimmick()
{
	//-------------------------------------
	//ギミック類
	//-------------------------------------
	pGame->power_x = enGravitPowerX;

	if( !m_bEngineBlockCompleted )
	{
		//エンジンブロックを制するまでは右にいかせない
		if( IsAreaArrive(5241,180,720,540) )
		{
			m_sScrollYMax = 470*100;
			m_sScrollMin  = 2000*100;
			m_sScrollMax  = 5460*100;
		}

		//エンジンブロックを制するまでは左にいかせない
		if( IsAreaArrive(6200,180,920,700) )
		{
			m_sScrollMin  = 6350*100;
		}

		//エンジンブロックを制するまでは下段から上段にはいかせない
		if( IsAreaArrive(5241,720,1200,300) )
		{
			m_sScrollYMin = 720*100;
			m_sScrollYMax = 720*100;
		}
	}

	g_sOverLayerHeight = LayerScroll[enBgTypeFrnt].oy;

	//-------------------------------------
	//本体コア
	//-------------------------------------

	if( ( m_pCannonCore && m_pCannonCore->IsDamage() ) )
	{
		if(viiSub::GameCounter()%6 < 3 )
		{
			pGame->pBg->SetLayerFilter( enBgTypeRear , ARGB( 0xff ,0xff , 0xff , 0xff ) , ATR_ALPHA_PLUS );
		}
		else
		{
			pGame->pBg->SetLayerFilter( enBgTypeRear , ARGB( 0xff ,0xff , 0xff , 0xff ) , ATR_DFLT );
		}
	}
	else
	{
		pGame->pBg->SetLayerFilter( enBgTypeRear , ARGB( 0xff ,0xff , 0xff , 0xff ) , ATR_DFLT );
	}

	//-------------------------------------
	//ホバーの煙
	//-------------------------------------
	if( !m_bFrontTankDead )
	{
		for(Sint32 jj=0;jj<1;jj++)
		{
			CEffectGroundSmoke *p;
			p = new CEffectGroundSmoke( 4800*100+(viiSub::Rand()%480)*100+m_ScrPos.x,900*100, DIR_RIGHT );
			p->SetScale(3.f);
			p->SetAlpha(128);
		}
	}
/*
	for(Sint32 jj=0;jj<2;jj++)
	{
		CEffectGroundSmoke *p;
		p = new CEffectGroundSmoke( (4850+viiSub::Rand()%1800)*100,900*100, DIR_RIGHT );
		p->SetScale(3.f);
		p->SetAlpha(128);
	}
*/

	//-------------------------------------
	//ニュートロンポリマー砲のエネルギーボール
	//-------------------------------------
	if( !m_bLastBoss ) viiEff::Set(EFF_ENERGYBALL , 5478*100+m_ScrPos.x , 524*100 , argset(500,200,0,300));

	//--------------------------------------
	//フロントサブキャノン
	//--------------------------------------
	if( m_pSubCannon[0] )
	{
		m_pSubCannon[0]->SetOffset( -LayerScroll[enBgTypeFrnt].oy*2 );
		if( m_pSubCannon[0]->IsDead() )
		{
			//エネルギーユニット作成
			m_pEnergyFrnt = new CEneEx0006Tank( m_pSubCannon[0]->m_Pos.x , m_pSubCannon[0]->m_Pos.y );

			m_pSubCannon[0]->Clear();
			m_pSubCannon[0] = NULL;
		}
	}
	//--------------------------------------
	//フロントサブキャノン破壊後のフロントタンク
	//--------------------------------------
	if( m_pEnergyFrnt && m_pEnergyFrnt->IsDead() )
	{
//		m_pEnergyFrnt->Clear();
//		m_pEnergyFrnt = NULL;
		//AddMissionSec( 1, 30*1 );
		m_bFrontTankDead = gxTrue;
		//ClearMapTip( enBgTypeFrnt , 4721,582,4721+599,582+378 );
	}


	//--------------------------------------
	//ミドルサブキャノン
	//--------------------------------------
	if( m_pSubCannon[1] )
	{
		m_pSubCannon[1]->SetOffset( -LayerScroll[enBgTypeRear].oy );
		if( m_pSubCannon[1]->IsDead() )
		{
			m_pSubCannon[1]->Clear();
			m_pSubCannon[1] = NULL;
			//エネルギーユニット作成
			m_pEnergyMiddle  = new CEneEx0006Tank( 5640*100 , 760*100 );
		}
	}

	//--------------------------------------
	//ミドルサブキャノン破壊後のボディタンク
	//--------------------------------------
	if( !m_bMiddleTankDead )
	{
		if( m_pEnergyMiddle && m_pEnergyMiddle->IsDead() )
		{
			Sint32 ax,ay;
			ax = 5500*100;
			ay = 750*100;

//			AddMissionSec( 2, 30*1 );
			m_bMiddleTankDead = gxTrue;

			//m_pEnergyMiddle->Clear();
			//m_pEnergyMiddle = NULL;

			//ClearMapTip( enBgTypeFrnt , 5365,679,5365+393,679+143 );
		}
	}


	//--------------------------------------
	//リアサブキャノン
	//--------------------------------------
	EngineAtack();

	//--------------------------------------
	//パネル裏の隠しコア
	//--------------------------------------
	if( m_pBodyPanel && m_pBodyPanel->IsDead() )
	{
		m_pBodyPanel->Clear();
		m_pBodyPanel = NULL;
		m_pBodyTank = new CEneEx0006Tank( 6010*100 , 770*100 );
	}

	if( m_pBodyTank && m_pBodyTank->IsDead() )
	{
//		AddMissionSec( 4, 30*1 );
		m_bBodyTankEnd = gxTrue;
	}

	//--------------------------------------
	//ファイナルコア破壊された
	//--------------------------------------
//	if( m_pCannonCore && m_pCannonCore->IsDead() )
//	{
//	}

	//--------------------------------------
	//ダッシュ制御
	//--------------------------------------
	if(GetTargetPlayer()->y >= enRollerDashHeight )
	{
		pGame->GetHound()->SetForceRollerDash(gxTrue);
	}
	else
	{
		pGame->GetHound()->SetForceRollerDash(gxFalse);
	}

	//--------------------------------------
	//ニュートロンポリマー砲を破壊
	//--------------------------------------
	if( m_pMainCannon && m_pMainCannon->IsDead() )
	{
		if( !CCockpit::GetInstance()->IsMessageNow() )
		{
			CCockpit::GetInstance()->SetMessage( 5200 );	//主砲のバレルを破壊した
			m_pMainCannon->Clear();
			m_pMainCannon = NULL;
			ClearVmpTip( 638*8 , 48*8 , (638+56)*8 , (48+30)*8 );
		}
	}
}


void CBaseExtra::Draw()
{
	//-------------------------------------
	//雲
	//-------------------------------------
	Sint32 ax,ay,ii;
	Sint32 sx,sy;

	viiSub::GetScroll_s( sx,sy );
	sx = sx*100;
	sy = sy*100;

	viiDraw::Spr_NoScrl( &StSprCloud[ 0 ] , 0,0,PRIO_BG1-1 , ATR_DFLT , 0x80808080 );


	for(ii=-1;ii<2;ii++)
	{
		ax = sx+m_Scroll[0].x;
		ay = 932*100;
		viiDraw::Sprite( &StSprCannion[ 0 ] , ax+ii*51200,ay,PRIO_BG3+1 , ATR_DFLT , 0xC0FFFFFF );
	}

	for(ii=-1;ii<2;ii++)
	{
		ax = sx+m_Scroll[1].x;
		ay = 916*100;
		viiDraw::Sprite( &StSprCannion[ 1 ] , ax+ii*51200,ay,PRIO_BG2-1 , ATR_DFLT , 0xFFFFFFFF );
	}

	for(ii=-1;ii<2;ii++)
	{
		ax = sx+m_Scroll[2].x;;
		ay = 776*100;
		viiDraw::Sprite( &StSprCannion[ 2 ] , ax+ii*51200,ay,PRIO_BG1-1 , ATR_DFLT , 0xFFFFFFFF );
	}

}

gxBool CBaseExtra::EngineAtack()
{
	//--------------------------------------
	//メインエンジンノズルｘ２の制御
	//--------------------------------------
	if( m_pSubCannon[2] )
	{
		m_pSubCannon[2]->SetOffset( -LayerScroll[enBgTypeRear].oy );
		if( m_pSubCannon[2]->IsDead() )
		{

			if( m_pEnergyRear == NULL )
			{
				//エネルギーユニット作成
				Sint32 ax,ay;
				ax = 6400*100;
				ay = 770*100;
				CCockpit::GetInstance()->SetMessage( 5300 );	//戦闘艦のエンジン出力が減少。
				m_pEnergyRear = new CEneEx0006Tank( ax , ay );
			}
		}
	}

	if( !m_bNozzleEnd )
	{
		if( m_pNozzle[0] == NULL || m_pNozzle[1]== NULL ) return gxFalse;

		if( m_pNozzle[0]->IsDead() && m_pNozzle[1]->IsDead() )
		{
			Sint32 ax,ay;
			ax = 6380*100;
			ay = 800*100;

			m_pNozzle[0]->Clear();
			m_pNozzle[1]->Clear();
			new CEffectFinalExplosion( ax , ay ,8 );
			m_bNozzleEnd = gxTrue;

			CEffectBlackExplosion *pBlack;
			pBlack = new CEffectBlackExplosion( ax , ay , NULL );
			pBlack->SetLayerPrio(PRIO_EFFECT);

			m_pNozzle[0] = NULL;
			m_pNozzle[1] = NULL;

		}
	}

	if( m_pEnergyRear && !m_bEngineBlockCompleted)
	{
		if( m_pEnergyRear->IsDead() )
		{
			//ClearMapTip( enBgTypeFrnt , 6240,628,6240+353,628+258 );
			//m_pEnergyRear->Clear();
			//m_pEnergyRear = NULL;
			m_bRearTankDead = gxTrue;
//			AddMissionSec( 3, 30*1 );
			m_bEngineBlockCompleted = gxTrue;

			//xここでやっと破棄
			m_pSubCannon[2]->Clear();
			m_pSubCannon[2] = NULL;
		}
	}
	return gxTrue;
}


void CBaseExtra::ResetEnemy()
{
	//g_pActManager->Clear(enActionPrioDefault);
	CActManager::GetInstance()->Clear(enActionPrioDefault);

	for(Sint32 jj=0; jj<32;jj++)
	{
		if( jj < enNozzleMax )    m_pNozzle[jj]    = NULL;
		if( jj < enSubCannonMax ) m_pSubCannon[jj] = NULL;
	}
	m_pBodyPanel    = NULL;
	m_pBodyTank     = NULL;
	m_pEnergyFrnt   = NULL;
	m_pEnergyMiddle = NULL;
	m_pEnergyRear   = NULL;
	m_pCannonCore   = NULL;
	m_pMainCannon   = NULL;

	if( m_pGunHound[1] )
	{
		delete m_pGunHound[1];
		m_pGunHound[1] = NULL;
	}

	if( m_pGunHound[0] )
	{
		delete m_pGunHound[0];
		m_pGunHound[0] = NULL;
	}
}


