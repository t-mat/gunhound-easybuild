//----------------------------------------------------------------------------
//
//ハウンドステージ １（ジャングル）
//
//----------------------------------------------------------------------------

#include <gunvalken.h>
#include <gxLib/util/gxImage/CGXImage.h>
#include "CStageBase.h"
//---------------------------------------------------------------------------
#include "../effect/EffectBase.h"
#include "../enemy/enemy.h"
#include "../GunHound/GhStringTbl.h"
#define STAGE_PATH "HoundData\\stageH01\\"

enum
{
	enAreaSeq01a,
	enAreaSeq01b,
	enAreaSeq02Init,
	enAreaSeq02Main,
	enAreaSeq03,
	enAreaSeq04,
	enAreaSeq05,

	enAreaSeqTestA,
	enAreaSeqTestB,
	enAreaSeqMax,
};

enum
{
	enGroundScrY = 695,
	enSubMarineOffset = 980,
	enSlideCannonY = 650*100,
	wnWaraWaraMax = 12,	//タランチュラ数
	enWaraWaraVMax = 18,
};


gxSprite SprBridge[]=
{
	{enTexPageEnemyCommon08,0,16,32,80,16,40},	//橋のかけら
};

gxSprite SprMarineLight[]={
	{enTexPageEnemyCommon08,32,32,224,16,16,16},
};


void CBaseKichi::Init()
{
	//---------------------------------------------------------------------------
	//初期化
	//---------------------------------------------------------------------------

	Sint32 col1 =  0xF0102060;
	Sint32 col2 =  0xF0102030;
	pGame->pBg->SetSkyColor(50,col1,col2);

	while( !CGXImage::GetInstance()->IsLoadFinish() )
	{
		//ロード待ち
	}

	switch(GetArea( )){
	case enAreaSeq01a:
		if( m_bFirst )
		{
			if( g_StGameInfo.m_bDebugRoot )
			{
				CGXImage::GetInstance()->Load( "asset/gh/GXI/BASEA.GXI" );
			}
		}
		SetDRQStage( "ScenBASEA.txt" );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		viiMus::PlayBGM( enSoundBgm1 , enMusicBgmStageEx );
		break;

	case enAreaSeq01b:
		if( m_bFirst )
		{
			SetFadeIn();
			if( g_StGameInfo.m_bDebugRoot )
			{
				CGXImage::GetInstance()->Load( "asset/gh/GXI/BASEA.GXI" );
			}
		}
		SetDRQStage( "ScenBASEA.txt" );
		viiMus::PlayBGM( enSoundBgm1 , enMusicBgmStageEx );
		CCockpit::GetInstance()->SetMissionSec(60);
		SetTargetPlayerPosX( 6450*100 );
		SetTargetPlayerPosY(  800*100 );
		viiSub::SetScroll_l( 6529*100 , 585*100 );
		new CEntranceControl( CEntranceControl::enEntrance , DIR_RIGHT );
		break;

	case enAreaSeq02Init:
	case enAreaSeq02Main:
		CCockpit::GetInstance()->SetMissionSec(60*10);
		CGXImage::GetInstance()->Load( "asset/gh/GXI/BASEB.GXI" );
		SetDRQStage( "ScenBASEB.txt" );
		m_sPlasmaDestroyCnt = 1;
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		ChangeArea( enAreaSeq02Main );
		break;

	case enAreaSeq03:
		if( m_bFirst )
		{
			CGXImage::GetInstance()->Load( "asset/gh/GXI/BASEB.GXI" );
			SetDRQStage( "ScenBASEB.txt" );
			SetTargetPlayerPosX( 7643*100 );
			SetTargetPlayerPosY( 700*100 );
			viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		}

		SetFadeIn();
		ChangeArea( enAreaSeq03 );
		break;

	case enAreaSeq04:
		if( m_bFirst )
		{
			CGXImage::GetInstance()->Load( "asset/gh/GXI/BASEB.GXI" );
			SetDRQStage( "ScenBASEB.txt" );
			CCockpit::GetInstance()->SetMissionSec(  5*10 );
		}
		pGame->SetMarineLine(980*100);
		pGame->GetHound()->SetWaterProof();
		ChangeArea( enAreaSeq04 );
		break;

	case enAreaSeq05:
	case enAreaSeqTestA:
		CGXImage::GetInstance()->Load( "asset/gh/GXI/BASEA.GXI" );
		SetDRQStage( "ScenBASEA.txt" );
		ChangeArea( enAreaSeqTestA );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		return;

	case enAreaSeqTestB:
		CGXImage::GetInstance()->Load( "asset/gh/GXI/BASEB.GXI" );
		SetDRQStage( "ScenBASEB.txt" );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		ClearMapTip(enBgTypeFrnt,7724,980,7724+320,980+294);
		ChangeArea( enAreaSeqTestB );
		return;
	default:
		break;
	}

	InitEnemies();

	SetGamePlay( gxTrue );

	if( m_bFirst )
	{
		m_bFirst = gxFalse;
	}

}


void CBaseKichi::End()
{
	//---------------------------------------------------------------------------
	//終了処理
	//---------------------------------------------------------------------------


}



void CBaseKichi::InitEnemies()
{
	//---------------------------------------------------------------------------
	//敵の初期化
	//---------------------------------------------------------------------------

	SetEnemy();

}


void CBaseKichi::Action()
{
	//---------------------------------------------------------------------------
	//毎フレームのアクション
	//---------------------------------------------------------------------------
	//エリア外判定を出さない
	CCockpit::GetInstance()->SetOperationRange(0*100,16000*100);

	StageCommon();

	//---------------------------------------------------------------------------------
	//これは必ず先にやること
	AreaCommon();
	//※g_pActManager->Clear()によって消されたEnergyUnitの実体が消された後、
	//　ポインタへのアクセスを行ってしまうため。先にアクセス、後でClearの手順が必須
	//---------------------------------------------------------------------------------

	switch(GetArea( )){

	case enAreaSeq01a:
		Area01a();
		break;

	case enAreaSeq01b:
		Area01b();
		break;

	case enAreaSeq02Init:
		//g_pActManager->Clear(enActionPrioDefault);
		CActManager::GetInstance()->Clear(enActionPrioDefault);
		//-----------------------------------------
		//アクションマネージャに実態は消させる
		//ポインタはここで削除して以降アクセスさせない
		//-----------------------------------------
		for(Sint32 ii=0;ii<enPlasmaMax;ii++)
		{
			m_pEnergyUnitZako[ii] = NULL;
		}
		m_sPlasmaCnt = 0;
		ChangeArea( enAreaSeq02Main );
		Init();
		break;
	case enAreaSeq02Main:
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
	case enAreaSeqTestA:
	case enAreaSeqTestB:
		AreaTest();
		break;
	}

}


void CBaseKichi::Area01a()
{
	//---------------------------------------------------------------------------
	//エリア１　高速スクロール
	//---------------------------------------------------------------------------
	gxBool bLimitMove = gxFalse;

	if( IsZoneSeq(0) )
	{
		SetGamePlay( gxFalse );
		CCockpit::GetInstance()->SetBlackMask( gxTrue );
		pGame->GetHound()->SetMode( CGunHound::enHoundModeNone );
		SetFadeIn();

		m_Pos.x = 0;//3200*100;
		m_Pos.y = 580*100;

		m_stScrAdd.x = 0;
		m_stScrAdd.y = 0;
		m_stScroll.sBg1x = 0;
		m_pScene = new CEffScene02( (6300)*100 , 680*100  ,PRIO_BG2 );

		m_stScroll.bLoopEnd  = gxFalse;

		SetTargetPlayerPosX( 5550*100 );
		SetTargetPlayerPosY( 700*100 );
		pGame->GetHound()->SetPadButton(0);

		ResetZoneTimer();
		SetZoneSeq(100);
	}
	else if( IsZoneSeq(100) )
	{
		StRaster stRaster;
		stRaster.bEnable = gxTrue;
		stRaster.sDetail = 24;
		stRaster.sSpeed  = 2;
		stRaster.sWidth  = 8;

		KaiteiGimmick();
		pGame->pBg->SetRaster( enBgTypeBack , &stRaster );
		//動きを止めておく
		m_stScroll.sBg1x -= 30;
		pGame->GetHound()->SetPadButton(0);

		m_pScene->SetControl( JOY_L );
		if( m_sZoneTimer ==  320 )
		{
			CCockpit::GetInstance()->SetMessage(4000);	//まもなく作戦エリアに到達します。
		}
		if( m_sZoneTimer == 360 )
		{
			m_pScene->Set(1);
		}
		if( m_sZoneTimer > 760 )
		{
			pGame->QuakeOn();	//ガクガク
			if( m_stScrAdd.x >= 100 ) m_stScrAdd.x -= 16;
		}
		else if( m_sZoneTimer > 360 )
		{
			if( m_stScrAdd.x <= 800 ) m_stScrAdd.x += 8;
		}


/*
		if(Joy[0].psh&JOY_L)
		{
			m_Pos.x -= 800;
		}
		if(Joy[0].psh&JOY_R)
		{
			m_Pos.x += 800;
		}
*/
		if( m_Pos.x < 3200*100 )
		{
			m_Pos.x += m_stScrAdd.x;
//			m_Pos.x += 120;
		}
		else
		{
			ResetZoneTimer();
			SetZoneSeq(200);
		}
	}
	else if( IsZoneSeq(200) )
	{
		//--------------------------------
		//出撃
		//--------------------------------
		m_stScroll.sBg1x -= 30;
		pGame->GetHound()->SetPadButton(0);

		SetTargetPlayerPosX( 5500*100 );
		SetTargetPlayerPosY( 750*100 );

		pGame->GetHound()->SetMode( CGunHound::enHoundModeNormal );
		new CEntranceControl( CEntranceControl::enEntrance , DIR_RIGHT );

		SetGamePlay( gxTrue );
		CCockpit::GetInstance()->SetMissionSec( 60*10 );
		CCockpit::GetInstance()->SetBlackMask( gxFalse );
		pGame->GetHound()->SetForceRollerDash(gxTrue);
		pGame->pBg->SetRaster( enBgTypeBack , NULL );
		ResetZoneTimer();
		SetZoneSeq(1000);
	}
	else if( IsZoneSeq(1000) )
	{
		//--------------------------------
		//無限スクロール
		//--------------------------------
		if( m_stScrAdd.x <= 800 ) m_stScrAdd.x += 8;

		m_Pos.x += m_stScrAdd.x;

		//--------------------------------
		//通路導入部
		//--------------------------------
		if( m_sZoneTimer < 60 )
		{
			pGame->GetHound()->SetPadButton(0);
			if( m_sZoneTimer > 20 )
			{
				pGame->GetHound()->SetPadButton(BTN_A);
			}
		}
		else
		{
			//移動制限をかける
			bLimitMove = gxTrue;
		}

		//--------------------------------
		//ステージ開始
		//--------------------------------
		if( m_sZoneTimer == 120 )
		{
			CCockpit::GetInstance()->SetMessage(4100);	//研究施設内部のダクトに進入成功！
		}

		if( m_sZoneTimer >= 300 )
		{
			//-------------------------------------
			//スパイダーを撃破した
			//-------------------------------------
			if( AreaInfinityScroll() )
			{
				ResetZoneTimer();
				SetZoneSeq(2000);
			}
		}
	}
	else if( IsZoneSeq(2000) )
	{
		//-------------------------------------
		//柱演出までの調整時間
		//-------------------------------------
		m_stScrAdd.x = 800;
		m_Pos.x += m_stScrAdd.x;

		CCockpit::GetInstance()->SetArrow(0);

		if( m_sZoneTimer >= 120 )
		{
			m_stScroll.sBg1x = 0;
			m_stScroll.bLoopEnd = gxTrue;
			ResetZoneTimer();
			SetZoneSeq(2050);
		}
	}
	else if( IsZoneSeq(2050) )
	{
		bLimitMove = gxTrue;
		m_stScroll.bLoopEnd = gxTrue;

		m_Pos.x += m_stScrAdd.x;

		if( !m_stScroll.bLoopNow )
		{
			ResetZoneTimer();
			SetZoneSeq(2100);
		}
	}
	else if( IsZoneSeq(2100) )
	{
		if( m_Pos.x >= 5600*100 )
		{
			//逃げるソルジャーを生成
			for(Sint32 ii=0;ii<4;ii++)
			{
				CSoldierBase *q;
				q = new CSoldierBase( );
				q->SetPosition( 0 , (6600+ii*24)*100, 800*100 , NULL);
				q->SetForce(enForceEnemy);
				q->SetTargetPosition( 9600*100 );
				q->SetWaitScreenIn();
				q->ChangeMoveType( enMoveTypeEscape );
		//		q->SetEnd();
			}
			m_pNightSpider->SetLogicSec( 200 );
			ResetZoneTimer();
			SetZoneSeq(3000);
		}

		m_Pos.x += m_stScrAdd.x;
	}
	else if( IsZoneSeq(3000) )
	{
		LayerScroll[enBgTypeBack].x = 30;
		LayerScroll[enBgTypeBack].y = 30;
		LayerScroll[enBgTypeBack].ox = 500;
		LayerScroll[enBgTypeBack].oy = 0;

		bLimitMove = gxTrue;
		AreaBombAndWall();

		if( m_sZoneTimer >= 60 )
		{
			if( m_stScrAdd.x >= 200 )
			{
				m_stScrAdd.x -= 8;
				if( m_stScrAdd.x <= 200 )
				{
					m_stScrAdd.x = 200;
					if( m_sZoneTimer >= 200 )
					{
						ResetZoneTimer();
						SetZoneSeq(4000);
					}
				}
			}
		}

		m_Pos.x += m_stScrAdd.x;

		{
			pGame->GetHound()->GetKasoku()->x = m_stScrAdd.x;
		}

	}
	else if( IsZoneSeq(4000) )
	{
		LayerScroll[enBgTypeBack].x = 30;
		LayerScroll[enBgTypeBack].y = 30;
		LayerScroll[enBgTypeBack].ox = 500;
		LayerScroll[enBgTypeBack].oy = 0;

		if( m_sZoneTimer >= 120 )
		{
			pGame->GetHound()->SetForceRollerDash(gxFalse);
			ChangeArea( enAreaSeq01b );
		}

		LayerScroll[enBgTypeRear].x = 100;
		LayerScroll[enBgTypeRear].y = 10;
		LayerScroll[enBgTypeRear].ox = 0;
		LayerScroll[enBgTypeRear].oy = 0;

		LayerScroll[enBgTypeFrnt].x = 100;
		LayerScroll[enBgTypeFrnt].y = 100;
		LayerScroll[enBgTypeFrnt].ox = 0;
		LayerScroll[enBgTypeFrnt].oy = 0;

		m_sScrollMin = 6520*100;
		m_sScrollMax = 6520*100;//6980*100;
		pGame->pBg->SetFilter( ARGB( 0xff ,0xff , 0xff , 0xff ) );
		pGame->pBg->SetLayerFilter( enBgTypeBack , ARGB( 0xff ,0x80 , 0x80 , 0x80 ) , ATR_DFLT );

		BgCommon();
		viiSub::LimitMoveVH();
		return;
	}


	//-----------------------------------------------
	//背景制御
	//-----------------------------------------------

	ghPos m_Add;

	m_Add.x = m_Pos.x - 3200*100;
	m_Add.y = 0;
	if( m_Add.x > 0 ) m_Add.x = 0;

	if( !m_stScroll.bLoopEnd )
	{
		m_stScroll.bLoopNow = gxFalse;
		if( m_Pos.x > 5604*100)
		{
			m_Pos.x = (5600-480)*100;
			m_stScroll.bLoopNow = gxTrue;
		};
	}

	if( m_Pos.x < 0*100) m_Pos.x = 0;

	LayerScroll[enBgTypeBack].x = m_Pos.x /500-m_stScroll.sBg1x/20;
	LayerScroll[enBgTypeBack].y = m_Pos.y /300;

	LayerScroll[enBgTypeRear].x = m_Pos.x /100;
	LayerScroll[enBgTypeRear].y = m_Pos.y /100;

	LayerScroll[enBgTypeFrnt].x = m_Pos.x /100+m_Add.x/100;
	LayerScroll[enBgTypeFrnt].y = m_Pos.y /100+m_Add.y/100;

	viiSub::SetBgLayer( enBgTypeBack , LayerScroll[enBgTypeBack].x , LayerScroll[enBgTypeBack].y );
	viiSub::SetBgLayer( enBgTypeRear , LayerScroll[enBgTypeRear].x , LayerScroll[enBgTypeRear].y );
	viiSub::SetBgLayer( enBgTypeFrnt , LayerScroll[enBgTypeFrnt].x , LayerScroll[enBgTypeFrnt].y );

	Sint32 sx,sy;

	sx = m_Pos.x;///100;
	sy = m_Pos.y;///100;

	if( sx < 5600*100 ) sx = 5600*100;

	pGame->pBg->setScroll100(sx,sy);

	if( bLimitMove )
	{
		viiSub::LimitMoveVH();
	}

	pGame->power_x = -m_stScrAdd.x;
}

gxBool CBaseKichi::AreaInfinityScroll()
{
	//-----------------------------------------
	//
	//無限スクロール処理
	//
	//-----------------------------------------
//	if( Joy[0].trg&BTN_L1) return gxTrue;

	if( WaraWaraTarantura() )
	{
		if( m_pNightSpider == NULL )
		{
			m_pNightSpider = new CEneH0102NightSpider( 4600*100, 800*100 );
			m_pNightSpider->SetDirection( DIR_RIGHT );
			m_pNightSpider->SetLogicMode( CEneH0102NightSpider::enLogicModeKousoku );
			m_bSpiderDead = gxFalse; 
			CCockpit::GetInstance()->SetShortMessage( StrShortMessage[8] );//"WARNING.....HIGH-SPEED ARMS APROACHES.");
			//CCockpit::GetInstance()->SetWarningSound( 240 );
		}
		else
		{
			if( m_pNightSpider->IsDead() )
			{
//				CCockpit::GetInstance()->SetArrow(4000);
				CCockpit::GetInstance()->SetArrow(0);

				if( !m_bSpiderDead )
				{
					m_bSpiderDead = gxTrue; 
					m_pNightSpider->SetLogicMode( CEneH0102NightSpider::enLogicModeCrash );
					m_pNightSpider->SetTargetPosition( 5900*100 );
				}
				else if( m_pNightSpider->IsLogicSec(150) )
				{
					return gxTrue;
				}
			}
		}
	}

	if( viiSub::GameCounter()%16 == 0 && viiSub::Rand()%2 == 0 )
	{
		//------------------------------------------------
		//ピラー制御
		//------------------------------------------------

		static gxSprite StSprPiller[]={
			{ enTexPageEnemyStage01,128,0,64,256,32,0 },
		};

		CEffExPerticle *p;
		p = new CEffExPerticle( WINDOW_W*100 , 0 , PRIO_ENE_BULLET+1 );//PRIO_BG3+1 );
		p->SetSprite( &StSprPiller[ 0 ] );
		p->m_fScale = 1.f;
		p->m_sAtribute = ATR_DFLT;
		p->m_sARGB = 0x80FFFFFF;
		p->m_sRotation = 0;
		p->SetDirection(180,1400);
		p->SetTimer( 320 );
		p->SetNoneScroll();
	}

	return gxFalse;
}

void CBaseKichi::Area01b()
{
	//-----------------------------------------------------
	//エリア１－Ｂ（橋の上）
	//-----------------------------------------------------
	gxBool bBakuhaNow = gxFalse;

	pGame->power_x = 0;
	pGame->pBg->SetLayerFilter( enBgTypeBack , ARGB( 0xff ,0x80 , 0x80 , 0x80 ) , ATR_DFLT );
	LayerScroll[enBgTypeBack].x = 30;
	LayerScroll[enBgTypeBack].y = 30;
	LayerScroll[enBgTypeBack].ox = 500;
	LayerScroll[enBgTypeBack].oy = 0;
	m_sScrollYMax = 870*100;

	if( m_bWarning )
	{
		Sint32 sCol = 0xb0+viiMath::Cos100((viiSub::GameCounter()*8)%360)*0x4f/100;
		pGame->pBg->SetLayerFilter( enBgTypeBack , ARGB( 0xff ,sCol , 0x01 , 0x01 ) );
	}

	BridgeGimmick();

	if( IsZoneSeq(0) )
	{
		//--------------------------------------
		//次エリアへの進行
		//--------------------------------------
		m_sBridgePos = 0;
		m_sScrollMin = 6520*100;
		m_sScrollMax = 8700*100;//6980*100;
		CCockpit::GetInstance()->AddMissionSec(  3*10 );

		for(Sint32 ii=0;ii<4;ii++)
		{
			CSoldierBase *q;
			q = new CSoldierBase( );
			q->SetPosition( 0 , (7200+ii*24)*100, 800*100 , NULL);
			q->SetForce(enForceEnemy);
			q->SetTargetPosition( 9600*100 );
			q->SetWaitScreenIn();
	//		q->ChangeMoveType( enMoveTypeEscape );
	//		q->SetEnd();
		}
		SetZoneSeq( 100 );
	}
	else if( IsZoneSeq( 100 ) )
	{
		if(pGame->GetHound()->m_Pos.x >= 7100*100 )
		{
			m_bWarning = gxTrue;
		}
		if(pGame->GetHound()->m_Pos.x >= 7200*100 )
		{
			//m_bBackView = gxTrue;
			ResetZoneTimer();
			SetZoneSeq( 110 );
		}
	}
	else if( IsZoneSeq( 110 ) )
	{
		//第１弾爆破
		bBakuhaNow = gxTrue;
		CCockpit::GetInstance()->SetArrow(2000);

		if( m_sZoneTimer >= 120 )
		{
			SetBridgeBomb( 1 );
			ResetZoneTimer();
			SetZoneSeq( 120 );
		}
	}
	else if( IsZoneSeq( 120 ) )
	{
		//第２弾爆破
		bBakuhaNow = gxTrue;
		CCockpit::GetInstance()->SetArrow(4000);

		if( m_sZoneTimer >= 120 )
		{
			SetBridgeBomb( 0 );
			ResetZoneTimer();
			SetZoneSeq( 150 );
		}
	}
	else if( IsZoneSeq( 150 ) )
	{
		//6700-8100
		CCockpit::GetInstance()->SetArrow(4000);

		if(pGame->GetHound()->m_Pos.x >= 8100*100 )
		{
			bBakuhaNow = gxFalse;
			ResetZoneTimer();
			SetZoneSeq( 200 );
		}
	}
	else if( IsZoneSeq( 200 ) )
	{
		CCockpit::GetInstance()->SetArrow(0);

		//m_bBackView = gxFalse;
		m_sScrollMax = 8340*100;

		if( IsAreaArrive(8800,400,32,800) )
		{
			SetGamePlay( gxFalse );
			new CEntranceControl( CEntranceControl::enExit , DIR_RIGHT );
			viiMus::StopBGM( enSoundBgm1 , 90 );
			SetFadeOut();
			SetZoneSeq(300);
		}
	}
	else if( IsZoneSeq( 300 ) )
	{
		//フェード待ち
		if( !viiMus::IsPlayBGM( enSoundBgm1 ) )
		{
			if( IsFadeEnd() )
			{
				CCockpit::GetInstance()->ResetMessageWindow();
				ChangeArea( enAreaSeq02Init );
				return;
			}
		}
	}

	if( !pGame->GetHound()->GetTobidashi() )
	{
		viiSub::LimitMoveOnlyV();
	}

	if( IsAreaArrive(6500,1200,2000,100) )
	{
		pGame->GetHound()->SetGameOver();
	}

	if( bBakuhaNow )
	{
		BgCommon();
		viiSub::LimitMoveOnlyV();
		return;

	}

	BgCommon();

}


void CBaseKichi::Area02()
{
	Sint32 sZoneTbl[][6]={
		//１段目
		{0,15,160,42 ,0,190},
		{160,0,315,49,0,90},
		{475,5,223,40,0,90},
		{730,0,157,43,0,90},//特別室

		//２段目
		{487,54,211,31,0,430},
		{419,53,69 ,40,0,520},
		{198,53,221,32,0,480},

		//３段目
		{196,97,381,42,0,820},
		{578,97,142,48,0,780},
		{759,69,155,34,0,560},//ボス前
	};

	pGame->GetHound()->SetForceRollerDash(gxFalse);

	if( IsZoneSeq( 0 ) )
	{
		//初期化
		SetTargetPlayerPosX( -96*100 );
		SetTargetPlayerPosY( 380*100 );
		new CEntranceControl( CEntranceControl::enEntrance , DIR_RIGHT );
		viiSub::SetScroll_l( 0*100 , 225*100 );
		CCockpit::GetInstance()->AddMissionSec(  60*10 );
		SetGamePlay( gxTrue );

		pGame->pBg->SetFilter( ARGB( 0xff ,0xff , 0xff , 0xff ) );

		viiMus::PlayBGM( enSoundBgm1 , enMusicBgmStageB);

		if( g_StGameInfo.m_sGameMode == StGameInfo::enGameModeMission )
		{
			new CItemBox( 6962*100, 64*100 , 0 );
		}

		SetFadeIn();
		SetZoneSeq( 10 );
	}
	else if( IsZoneSeq( 10 ) )
	{
		if( IsAreaArrive(200,100,32,800) )
		{
			CCockpit::GetInstance()->SetMessage(4300);	//	ニュートロンポリマー研究施設に
		}
		if( IsAreaArrive(2400,450,32,300) )
		{
			CCockpit::GetInstance()->SetMessage(4400);	//	ちくしょう、脚に一発食らっちまった
		}
		if( IsAreaArrive(5616,800,32,300) )
		{
			CCockpit::GetInstance()->SetMessage(4500);	//	まもなく脱出ポイントへ到達
		}

		gxBool bZoneHosei = gxFalse;
		for(Sint32 ii=0;ii<10; ii++)
		{
			if( IsAreaArrive( sZoneTbl[ii][0]*8,sZoneTbl[ii][1]*8,sZoneTbl[ii][2]*8,sZoneTbl[ii][3]*8) )
			{
				m_sScrollYMin = 0;//sZoneTbl[ii][4];
				m_sScrollYMax = sZoneTbl[ii][5]*100;
				bZoneHosei = gxTrue;
			}
		}

		if( !bZoneHosei )
		{
			m_sScrollYMin = 0;//sZoneTbl[ii][4];
			m_sScrollYMax = 9600*100;
		}
		//画面切り替え待ち
		if( IsAreaArrive(7280,512,64,432) )
		{
			//ボス戦へ
			SetZoneSeq( 20 );
		}
	}
	else if( IsZoneSeq( 20 ) )
	{
		ChangeArea( enAreaSeq03 );
		return;
	}

	BgCommon();
	viiSub::LimitMoveVH();

}


void CBaseKichi::Area03()
{
	//-----------------------------------------------------
	//エリア３：ボス戦
	//-----------------------------------------------------

	if( IsZoneSeq(0) )
	{
 		m_sScrollMin = 7300*100;
		m_sScrollMax = 8000*100;
		m_sScrollYMax = 780*100;
		pGame->SetMarineLine(980*100);
		pGame->GetHound()->SetWaterProof();
		pGame->GetHound()->SetForceRollerDash(gxFalse);
		viiMus::StopBGM(enSoundBgm1,90);
		viiMus::PlayBGM(enSoundBgm2,enMusicBgmBossSimple);
		CCockpit::GetInstance()->AddMissionSec(  3*10 );
		ResetZoneTimer();
		SetZoneSeq(10);
	}
	else if( IsZoneSeq(10) )
	{
		if( m_sZoneTimer == 60 )
		{
			CCockpit::GetInstance()->SetMessage(4600);	//メイン融合炉を確認
		}

		if( m_pGravitStar && m_pGravitStar->IsDead() )
		{
			SetGamePlay( gxFalse );
			pGame->GetHound()->SetMuteki();
			CCockpit::GetInstance()->SetMessage(4700);	//融合炉を破壊した、脱出口が開くぞ！
			ResetZoneTimer();
			SetZoneSeq(100);
		}
	}
	else if( IsZoneSeq(100) )
	{
		//ステージクリアへ
		pGame->GetHound()->SetMuteki();
		m_sScrollMin = m_pGravitStar->m_Pos.x-240*100;
		m_sScrollMax = m_sScrollMin;
		m_sScrollYMin = m_pGravitStar->m_Pos.y -120*100;
		m_sScrollYMax = m_sScrollYMin;

		if( m_sZoneTimer >= 400)
		{
			Sint32 sx=7945+240,sy=809+240;
			for(Sint32 y=-64;y<480*4;y+=8)
			{
				for(Sint32 x=-164;x<164;x+=8)
				{
					pGame->pBg->del_vmptip((sx+x)/8,(sy+y)/8);
					//pGame->pBg->del_maptip( enBgTypeFrnt, (sx+x)/MAP_BLOCK_SIZE, (sy+y)/MAP_BLOCK_SIZE);
				}
			}

			ClearMapTip(enBgTypeFrnt,7724,980,7724+320,980+294);
			m_pGravitStar->Clear();
			m_pGravitStar = NULL;
			ResetZoneTimer();
			SetZoneSeq(200);
		}
		BgCommon();
		return;
	}
	else if( IsZoneSeq(200) )
	{
		//脱出へ
		pGame->GetHound()->SetMuteki();
		m_sScrollYMin += (1050*100-m_sScrollYMin)/10;
		m_sScrollYMax = m_sScrollYMin;

		CEffectBubble *p;
		Sint32 ax,ay;

		for(Sint32 ii=0;ii<1;ii++)
		{
			ax = 7649*100;
			ay = (900+WINDOW_H)*100;

			ax += (viiSub::Rand()%WINDOW_W)*100;
			ay += (viiSub::Rand()%WINDOW_H*2)*100;

				p = new CEffectBubble(	ax , 	ay ,-90, 50+viiSub::Rand()%100 );
				p->SetWide( 0 , 90 );
				p->SetScale( 1.f );
				p->m_Pos.z = PRIO_EFFECT;
		}

		if( m_sZoneTimer >= 60)
		{
			SetZoneSeq(300);
		}
		BgCommon();
		return;
	}
	else if( IsZoneSeq(300) )
	{
		//穴が開いた
		pGame->GetHound()->SetMuteki();

//		pGame->pBg->del_vmptip(sx/8,sy/8);
//		pGame->pBg->del_maptip( enBgTypeFrnt, sx/MAP_BLOCK_SIZE, sy/MAP_BLOCK_SIZE);

		SetZoneSeq(999);
		BgCommon();
		return;
	}
	else if( IsZoneSeq(999) )
	{
		pGame->GetHound()->SetMuteki();
		m_pGravitStar = NULL;

		ChangeArea( enAreaSeq04 );
	}

	BgCommon();
	viiSub::LimitMoveVH();
}

void CBaseKichi::Area04()
{
	//-----------------------------------------------------
	//エリア４：脱出演出
	//-----------------------------------------------------

	pGame->GetHound()->SetPadButton(0);

	if( IsZoneSeq(0) )
	{
		m_pNautilus = new CEneH0407Nautilus(7860*100 , 1450*100);
		pGame->pBg->cancel_atari_flag();
		m_sScrollMax = m_sScrollMin;
		m_sScrollYMin = 1050*100;
		m_sScrollYMax = m_sScrollYMin;

		pGame->GetHound()->m_Pos.x = 7850*100;
		pGame->GetHound()->m_Pos.y = 600*100;
		pGame->GetHound()->SetAdd(0,0);
		pGame->GetHound()->SetPadButton(0);


		m_pGunHound[enHoundMaster] = new CGunHound( 4680*100 , 900*100 , gxFalse );
		m_pGunHound[enHoundMaster]->SetLeader(gxTrue);
		m_pGunHound[enHoundMaster]->SetMode( CGunHound::enHoundModeNPC );
		m_pGunHound[enHoundMaster]->m_Pos.x = 7900*100;
		m_pGunHound[enHoundMaster]->m_Pos.y = 400*100;
		m_pGunHound[enHoundMaster]->SetLogicPattern( 450 );
		m_pGunHound[enHoundMaster]->SetNoDamage( gxTrue );
		m_pGunHound[enHoundMaster]->SetTarget( 0 , 1450*100 );

		m_pGunHound[enHoundSlave] = new CGunHound( 4680*100 , 900*100 , gxFalse );
		m_pGunHound[enHoundSlave]->SetMode( CGunHound::enHoundModeNPC );
		m_pGunHound[enHoundSlave]->m_Pos.x = 7800*100;
		m_pGunHound[enHoundSlave]->m_Pos.y = 832*100;
		m_pGunHound[enHoundSlave]->SetLogicPattern( 450 );
		m_pGunHound[enHoundSlave]->SetNoDamage( gxTrue );
		m_pGunHound[enHoundSlave]->SetTarget( 0 , 1450*100 );

		m_pGunHound[enHoundPlayer] = pGame->GetHound();
		m_pGunHound[enHoundPlayer]->SetMode( CGunHound::enHoundModeNPC );
		m_pGunHound[enHoundPlayer]->SetLogicPattern( 450 );
		m_pGunHound[enHoundPlayer]->SetNoDamage( gxTrue );
		m_pGunHound[enHoundPlayer]->SetTarget( 0 , 1450*100 );

		ResetZoneTimer();
		SetZoneSeq(10);
	}
	else if( IsZoneSeq(10) )
	{
		CEffectBubble *p;
		Sint32 ax,ay;

		for(Sint32 ii=0;ii<1;ii++)
		{
			ax = 7649*100;
			ay = (900+WINDOW_H)*100;

			ax += (viiSub::Rand()%WINDOW_W)*100;
			ay += (viiSub::Rand()%WINDOW_H*2)*100;

				p = new CEffectBubble(	ax , 	ay ,-90, 50+viiSub::Rand()%100 );
				p->SetWide( 0 , 90 );
				p->SetScale( 1.f );
				p->m_Pos.z = PRIO_PLAYER-1;
		}

		if( m_sScrollYMin < 1300*100 )
		{
			pGame->GetHound()->m_Pos.y -= 120;
			m_sScrollYMin += 120;
		}
		m_sScrollYMax = m_sScrollYMin;
		if( m_sZoneTimer >= 225 )
		{
			m_pNautilus->SetStart();
			SetZoneSeq(100);
		}
	}
	else if( IsZoneSeq(100) )
	{

		ChangeArea( enAreaSeq05 );
	}

	BgCommon();
//	viiSub::LimitMoveVH();

}


void CBaseKichi::Area05()
{
	//-----------------------------------------------------
	//エリア５：ボス戦
	//-----------------------------------------------------

	pGame->GetHound()->SetPadButton(0);

	if( IsZoneSeq( 0 ) )
	{
		CCockpit::GetInstance()->SetMessage(4800);	//オペレーションレポート
		CCockpit::GetInstance()->SetStageClear();
		SetZoneSeq( 10 );
	}
	else if( IsZoneSeq( 10 ) )
	{
		for(Sint32 ii=0;ii<enHoundMax;ii++)
		{
			m_HoundPos[ii].x = m_pGunHound[ii]->m_Pos.x;
			m_HoundPos[ii].y = m_pGunHound[ii]->m_Pos.x;
		}

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
		Sint32 sx,sy;
		m_pNautilus->GetOffset(sx,sy);

		for(Sint32 ii=0;ii<enHoundMax;ii++)
		{
			m_pGunHound[ii] ->m_Pos.x = m_HoundPos[ii].x + sx;
		}

		if( IsFadeEnd() )
		{
			pGame->stage_clear(enClearSeqStageClear);
			ChangeArea( GAME_NONE );	//ステージ終了
		}
	}


}

void CBaseKichi::AreaCommon()
{
	//-----------------------------------------------------
	//エリア：共通
	//-----------------------------------------------------
//	Sint32 sMsgTbl[]={
//		2210,2220,2230,2240,2250,2260,2270,2280,2290,0,0,0,0,0,
//	};

/*
	for(Sint32 ii=0;ii<enPlasmaMax;ii++)
	{
		if( m_pEnergyUnitZako[ii] )
		{
			if( m_pEnergyUnitZako[ii]->IsDead() )
			{
				m_pEnergyUnitZako[ii]->Clear();
				m_pEnergyUnitZako[ii] = NULL;

//				CCockpit::GetInstance()->SetMessage( sMsgTbl[m_sPlasmaDestroyCnt] );
//				m_sPlasmaDestroyCnt ++;
			}
		}
	}
*/


}


//---------------------------------------------------------------------------
//ＶＭＰから敵の生成
//---------------------------------------------------------------------------


void CBaseKichi::SetEnemy()
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

			case enCEneH0501PlasmaMachine:
				if( m_sPlasmaCnt < enPlasmaMax )
				{
					m_pEnergyUnitZako[m_sPlasmaCnt] = new CEneH0501PlasmaMachine(x*800, y*800);
					m_sPlasmaCnt ++;
				}
				break;

			case enCEneH0404GravityStar:
				if( m_pGravitStar == NULL )
				{
					m_pGravitStar = new CEneH0404GravityStar(x*800,y*800);
				}
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
					p = new CEneH0006UGTortika( x*800,y*800,gxTrue);
					p->m_Pos.z = PRIO_BG2 + 1;
				}
				break;

			case enCEneH0422StunBarrierTate:
				{
					//スタンバリヤ縦
					CEneH0412StunbarrierTate *p;
					p = new CEneH0412StunbarrierTate( x*800,y*800 );
					p->SetWaitIndex( m_sStunBarrierIndex );
					m_sStunBarrierIndex ++;
				}
				break;

			default:
				MakeHoundsEnemy(x*800,y*800,sIndex);
				break;
			}

		}
	}

}


void CBaseKichi::BgCommon(Sint32 x,Sint32 y)
{
	//----------------------------------------------------
	//背景共通
	//----------------------------------------------------
	Sint32 sx,sy;
//	pGame->pBg->SetFilter(0xff101010);

	if( x==0 && y== 0)
	{
		BgScrollAdjust();
		viiSub::GetScroll_s( sx,sy );
	}
	else
	{
		sx = x/100;
		sy = y/100;
		pGame->pBg->setScroll100(x,y);
	}
	if( m_bBackView )
	{
		viiSub::GetIdealScroll_l(sx,sy);
		sx = pGame->GetHound()->m_Pos.x-320*100;
		viiSub::AdjScroll_l( sx,sy );
		viiSub::GetScroll_s( sx,sy );
	}

	viiSub::SetBgDisp ( enBgTypeStar , gxFalse );
	viiSub::SetBgLayer( enBgTypeStar , sx*LayerScroll[enBgTypeBack].x/10,sy*LayerScroll[enBgTypeBack].y/100 );

	StLayerScroll *pScr;

	for(Sint32 ii=enBgTypeBack; ii<=enBgTypeFrnt; ii++)
	{
		pScr = &LayerScroll[ii];
		viiSub::SetBgLayer( ii , pScr->ox+(sx*pScr->x)/100 , pScr->oy+(sy*pScr->y)/100 );
	}

}


gxBool CBaseKichi::WaraWaraTarantura()
{
	//------------------------------------------
	//ワラワラタランチュラゲーム
	//------------------------------------------
	Sint32 sWaraWaraMax = wnWaraWaraMax;
	Sint32 ii;
	Sint32 yy = WINDOW_H/2+16;

//	return gxTrue;

	switch( m_sWaraWaraSeq ){
	case 0:
		m_pTarantura[0] = new CEneH0403Tarantura( -WINDOW_W/2*100 , yy*100 );
		m_pTarantura[0]->SetTargetPos( (WINDOW_W/2)*100 , (yy-80)*100);

		m_pTarantura[1] = new CEneH0403Tarantura( -WINDOW_W/2*100 , yy*100 );
		m_pTarantura[1]->SetTargetPos( WINDOW_W/2*100 , (yy- 0)*100);

//		m_pTarantura[2] = new CEneH0403Tarantura( -WINDOW_W/2*100 , WINDOW_H/2*100 );
//		m_pTarantura[2]->SetTargetPos( (WINDOW_W/2+80)*100 , (WINDOW_H/2+80)*100);
		m_sWaraWaraSeq = 900;
		m_sWaraWaraCnt = 2;
		break;

	case 900:
		{
			Sint32 cnt=0;
			for(ii=0;ii<enTaranturaMax;ii++)
			{
				if( m_pTarantura[ii] && m_pTarantura[ii]->IsDead() )
				{
					m_pTarantura[ii]->Clear();
					m_pTarantura[ii] = NULL;
					if( m_sWaraWaraCnt < sWaraWaraMax )
					{
						m_pTarantura[ii] = new CEneH0403Tarantura( -WINDOW_W/2*100-ii*100 , yy*100 );
						m_pTarantura[ii]->SetTargetPos( (WINDOW_W/2)*100 , (yy)*100);
						m_sWaraWaraCnt ++;
					}
				}
				if( m_pTarantura[ii] == NULL )
				{
					cnt ++;
				}
			}

			if(cnt == enTaranturaMax && m_sWaraWaraCnt == sWaraWaraMax)
			{
				m_sWaraWaraSeq = 999;
			}
		}
		break;

	case 999:
		return gxTrue;
	}

	return gxFalse;
}


gxBool CBaseKichi::AreaBombAndWall()
{
	//------------------------------------------
	//爆発＆壁
	//------------------------------------------
	Sint32 sStart = 4200;
	Sint32 sOffset = 2050;

	switch( m_sZoneTimer ){
	case 10:
	case 50:
	case 70:
	case 80:
	case 90:
	case 100:
		m_sAlpha = 0xff;
		pGame->GetHound()->SetSearchLight( gxFalse );
		m_pNightSpider->SetLogicSec( 500 );
		break;
	case 130:
		m_pNightSpider->SetLogicSec( 800 );
		break;
	}

	if( m_sZoneTimer == 170 && IsAreaArrive(6593-WINDOW_W*4,800-WINDOW_H,WINDOW_W*4,WINDOW_H+64) )
	{
		//やられた
		pGame->GetHound()->SetGameOver();
	}

	//--------------------------------
	//ホワイトフェード
	//--------------------------------
	if( m_sZoneTimer >= 120 )
	{
		if( m_sWhite < 255 )
		{
			m_sWhite += 0x04;
			if( m_sWhite > 255 )
			{
				m_sWhite = 255;
			}
		}
	}

	if( m_Pos.x > (sStart+100*0)*100 )
	{
		if( m_pCEneH0410Wall[0] == NULL ) m_pCEneH0410Wall[0] = new CEneH0410Wall( (sOffset+3700)*100 , 657*100 );
	}

	if( m_Pos.x > (sStart+100*1)*100 )
	{
		if( m_pCEneH0410Wall[1] == NULL )
		{
			m_pCEneH0410Wall[1] = new CEneH0410Wall( 3900*100 , 657*100 );
		}
	}

	if( m_Pos.x > (sStart+100*2)*100 )
	{
		if( m_pCEneH0410Wall[2] == NULL ) m_pCEneH0410Wall[2] = new CEneH0410Wall( (sOffset+4100)*100 , 657*100 );
	}

	if( m_Pos.x > (sStart+100*3)*100 )
	{
		if( m_pCEneH0410Wall[3] == NULL )
		{
			m_pCEneH0410Wall[3] = new CEneH0410Wall( (4300+sOffset)*100 , 657*100 );
		}
	}


	if( m_Pos.x > (sStart+100*4)*100 )
	{
		if( m_pCEneH0410Wall[4] == NULL )
		{
			m_pCEneH0410Wall[4] = new CEneH0410Wall( (4500+sOffset)*100 , 657*100 );
		}
	}

	if( m_sAlpha > 0 )
	{
		m_sAlpha -= 0x02;
		if( m_sAlpha <= 0 )
		{
			m_sAlpha = 0x00;
		}
	}

	Sint32 sCol = m_sAlpha;
	pGame->pBg->SetFilter( ARGB( 0xff ,sCol , 0x01 , 0x01 ) );

	return gxTrue;
}


gxBool CBaseKichi::WaraWaraVorzoyFlying()
{
	//------------------------------------------
	//ワラワラボルゾイゲーム
	//------------------------------------------
	Sint32 ii;

	switch( m_sWaraWaraVorzoySeq ){
	case 0:
		m_pVorzoy[0] = new CEneH0003Vorzoy( 5700*100, 1200*100 ,gxTrue );
		m_pVorzoy[0]->SetLogic( CEneH0003Vorzoy::enLogicFlying );
		m_pVorzoy[0]->SetNoneScrollOut();

		m_pVorzoy[1] = new CEneH0003Vorzoy( 5400*100, 1200*100 ,gxTrue );
		m_pVorzoy[1]->SetLogic( CEneH0003Vorzoy::enLogicFlying );
		m_pVorzoy[1]->SetNoneScrollOut();

		m_pVorzoy[2] = new CEneH0003Vorzoy( 5100*100, 1200*100 ,gxTrue );
		m_pVorzoy[2]->SetLogic( CEneH0003Vorzoy::enLogicFlying );
		m_pVorzoy[2]->SetNoneScrollOut();

		m_sWaraWaraVorzoySeq = 100;
		m_sWaraWaraVorzoyCnt  = 3;
		break;

	case 100:
		{
			Sint32 cnt=0;
			for(ii=0;ii<enVorzoyMax;ii++)
			{
				if( m_pVorzoy[ii] && m_pVorzoy[ii]->IsDead() )
				{
					m_pVorzoy[ii]->Clear();
					m_pVorzoy[ii] = NULL;
				}
				if( m_pVorzoy[ii] == NULL )
				{
					if( m_sWaraWaraVorzoyCnt < enWaraWaraVMax && m_sZoneTimer%64==0 )
					{
						m_pVorzoy[ii] = new CEneH0003Vorzoy( (5000+(viiSub::Rand()%10*100))*100, 1200*100 ,gxTrue );
						m_pVorzoy[ii]->SetLogic( CEneH0003Vorzoy::enLogicFlying );
						m_pVorzoy[ii]->SetNoneScrollOut();
						m_sWaraWaraVorzoyCnt ++;
						break;
					}
					cnt ++;
				}
			}

			if(cnt == enVorzoyMax && m_sWaraWaraVorzoyCnt == enWaraWaraVMax)
			{
				m_sWaraWaraVorzoySeq = 999;
			}
		}
		break;

	case 999:
		return gxTrue;
	}

	return gxFalse;
}


void CBaseKichi::SetBrokenSeeling()
{
	//--------------------------------------------
	//天井から破片が落ちてくる
	//--------------------------------------------
	Sint32 sx , sy;
	viiSub::GetScroll_s( sx , sy );

	sx = sx + viiSub::Rand()%WINDOW_W;

	if( sx%8 ) return;
	sy -= 100;
	sx *= 100;
	sy *= 100;


	CEffectDmgprt *pDmg;
	pDmg = new CEffectDmgprt( sx,sy , 90 , 10 );
	pDmg->SetHeavy();
	pDmg->SetSmoke(gxFalse);
}


void CBaseKichi::SetBridgeBomb(Sint32 n )
{
	//--------------------------------------------
	//ブリッジ爆弾始動
	//--------------------------------------------

	if( m_sBridgeBomb[n] == 0 )
	{
		m_sBridgeBomb[n] = 1;

		pGame->QuakeOn(64);
		if( n == 1 )
		{
			viiMus::PlaySound( enSoundExplosionLarge );
			new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , 7500*100 , 800*100 , PRIO_BG3+1 , 45  ,180 );
			new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , 8100*100 , 800*100 , PRIO_BG3+1 , -45 ,180 );
		}
		else
		{
			viiMus::PlaySound( enSoundExplosionLarge );
			new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , 6700*100 , 800*100 , PRIO_BG3+1 , 45 ,180 );
			new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , 7200*100 , 800*100 , PRIO_BG3+1 , -45 ,180 );
		}
	}

}


void CBaseKichi::BridgeGimmick()
{
	//--------------------------------------------
	//ブリッジのギミック
	//--------------------------------------------
	Sint32 sStart;
	Sint32 sCnt;
	for(Sint32 n=0;n<2;n++)
	{
		sStart = 6700;
		if( n == 1 ) sStart = 7500;

		if( m_sBridgeBomb[n] )
		{
			m_sBridgeBomb[n] ++;
		}
		else
		{
			continue;
		}


		//待ち時間設定
		sCnt = m_sBridgeBomb[n]-64;
		if( sCnt < 0 ) continue;

		SetBrokenSeeling();

		Sint32 bx = sStart+sCnt*16;
		Sint32 by = 700;

/*
		if( n == 0 && sCnt == 40 )
		{
			new CEffBigBomb( bx*100 , 800*100 , 0 , 0 );
			viiEff::SetBombDeluxe( bx*100 , 800*100 , 1 , 1+viiSub::Rand()%2 );
		}
*/
		if( sCnt == 120 )
		{
			viiMus::PlaySound( enSoundCrashA );
		}

		if( n == 1 && sCnt == 320 )
		{
			new CItemChip( 7431*100, 500*100, enItemEnergyChip);
		}
		if( n == 0 && bx > 7200 )
		{
			continue;
		}

		if( n == 1 && bx > 8100 ) continue;

		viiEff::SetBombSimple( bx*100 , 800*100 , 1 , 1+viiSub::Rand()%2 );

		CEffectDmgprt *pDmg;
		pDmg = new CEffectDmgprt( bx*100 , 800*100 , -45-viiSub::Rand()%45 , 100 );
		pDmg->SetHeavy();
		pDmg->SetSmoke(gxFalse);
		pDmg->SetSprite( &SprBridge[0] );

		ClearMapTip( enBgTypeFrnt , bx,by,bx+16,by+256 );
		ClearVmpTip(                bx,by,bx+16,by+128 );
	}

}


void CBaseKichi::KaiteiGimmick()
{
	Sint32 sx , sy;

	for(Sint32 ii=0;ii<4;ii++)
	{
		CEffExPerticle *p;

		viiSub::GetScroll_s( sx , sy );
		sx += viiSub::Rand()%WINDOW_W*2;
		sx *= 100;
		sy *= 100;

		p = new CEffExPerticle( sx , sy , PRIO_ENE_BULLET+1 );
		p->SetSprite( &SprMarineLight[ 0 ] );
		p->m_fScale = 0.03f*(50+viiSub::Rand()%50);
		p->m_sAtribute = ATR_ALPHA_PLUS;
		p->m_sARGB = 0x08FFFFFF;
		p->m_sRotation = 90+25+viiSub::Rand()%10;
		p->SetTimer( 80 );
		p->SetAlphaAdd( -1 );//-(1+viiSub::Rand()%2) );
		p->SetEnd();
		p->m_sPrio = ( viiSub::Rand()%2 )? PRIO_BG1+1 : PRIO_EFFECT;
		p->m_sPrio = PRIO_BG1+1;
	}
}


