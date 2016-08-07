//----------------------------------------------------------------------------
//
//ハウンドステージ ２（リバー）
//
//----------------------------------------------------------------------------

#include <gunvalken.h>
#include <gxLib/util/gxImage/CGXImage.h>
#include "CStageBase.h"
//---------------------------------------------------------------------------
#include "../effect/EffectBase.h"
#include "../enemy/enemy.h"
//#include "kernel/util/win32tool.h"
#define STAGE_PATH "HoundData\\stageH02\\"

gxSprite SprStageH02River[]=
{
	{enTexPageEffectStage01,0,16  ,256,20 , 0 , 0},
	{enTexPageEffectStage01,0,43  ,256,20 , 0 , 0},

	{enTexPageEffectStage01,0,85  ,256,20 , 0 , 0},
	{enTexPageEffectStage01,0,112 ,256,20 , 0 , 0},

	{enTexPageEffectStage01,0,152 ,256,40 , 0 , 0},
	{enTexPageEffectStage01,0,197 ,256,40 , 0 , 0},
};

enum {
	enHoundOpeStart,
	enHoundOpeShipDefence,
	enHoundOpeShipDefence2,
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

enum
{
	enOnTheShip = 471*100,
	enGroundScrY = 695,
	enWaraWaraMax = 8,
};

Sint32 CBaseRiver::m_sTransShipX = 0;

void CBaseRiver::Init()
{
	//---------------------------------------------------------------------------
	//初期化
	//---------------------------------------------------------------------------
	SetFadeIn();

	Sint32 col1 =  0xF0408080;
	Sint32 col2 =  0xF0808040;
	pGame->pBg->SetSkyColor(50,col1,col2);

	m_bLoop = gxTrue;
	stRiver .m_sAdd = 10*280/10;
	stRiver2.m_sAdd = 7*280/10;
	stRiver3.m_sAdd = 5*280/10;
	m_stRiverOffset.x = 0;
	m_stRiverOffset.y = 0;
	m_pTankBoss = NULL;

	m_DummyPos.x = 0;
	m_DummyPos.y = 0;

	m_LeaderyPos.x = 0;
	m_LeaderyPos.y = 0;

	if( m_bFirst )
	{
		pGame->SetMarineLine(500*100);
		pGame->GetHound()->SetWaterProof();

		if( g_StGameInfo.m_bDebugRoot )
		{
			CGXImage::GetInstance()->Load( "asset/gh/GXI/RIVER.GXI" );
		}
	}

	while( !CGXImage::GetInstance()->IsLoadFinish() )
	{
		//ロード待ち
	}

	switch(GetArea( )){
	case enAreaSeq01:
	case enAreaSeq02:
		if( g_StGameInfo.m_sGameMode == StGameInfo::enGameModeMission )
		{
			new CItemBox( 4137*100, 192*100 , 0 );
		}
		SetDRQStage( "ScenRiver.txt" );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		InitEnemies();
		ChangeArea( enAreaSeq01 );
		break;

	case enAreaSeq03:
		SetDRQStage( "ScenRiver.txt" );
		SetTargetPlayerPosX( 4000*100 );
		SetTargetPlayerPosY( 480*100 );
		InitEnemies(4500);
		viiSub::SetScroll_l( GetTargetPlayer()->x , 270*100 );
		CCockpit::GetInstance()->SetMissionSec( 60*5 );
		break;

	case enAreaSeq04:
		SetDRQStage( "ScenRiver.txt" );
		SetTargetPlayerPosX( 6718*100 );
		SetTargetPlayerPosY( 480*100 );
		InitEnemies(6800);
		viiSub::SetScroll_l( GetTargetPlayer()->x , 270*100 );
		CCockpit::GetInstance()->SetMissionSec( 60*3 );
		break;

	case enAreaSeq05:
	case enAreaTest:
		//テスト用
		SetDRQStage( "ScenRiver.txt" );
		SetTargetPlayerPosX( 320*100 );
		SetTargetPlayerPosY( 480*100 );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		CCockpit::GetInstance()->SetMissionSec( 60*3 );
		return;
	}

	SetGamePlay( gxTrue );

}


void CBaseRiver::End()
{
	//---------------------------------------------------------------------------
	//終了処理
	//---------------------------------------------------------------------------


}


void CBaseRiver::InitEnemies(Sint32 sMin)
{
	//---------------------------------------------------------------------------
	//敵の初期化
	//---------------------------------------------------------------------------

	SetEnemy( sMin );

}


void CBaseRiver::Action()
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


void CBaseRiver::Area01()
{
	//---------------------------------------------------------------------------
	//エリア１：オープニング
	//---------------------------------------------------------------------------

	m_sScrollPos += 60;
	m_sScrollMin = (200-400)*100+m_sScrollPos;
	m_sScrollMax = (200+400)*100+m_sScrollPos;

	if( m_sScrollMax > 7500*100 ) m_sScrollMax = 7500*100;
	if( m_sScrollMin > 6300*100 ) m_sScrollMin = 6300*100;
	if( m_sScrollMin < 100*100  ) m_sScrollMin = 100*100;


	if( IsZoneSeq(0) )
	{
		SetGamePlay( gxFalse );
		m_sScrollPos = 0*100;
		viiSub::SetScroll_l( 500*100 , 271*100 );
		CCockpit::GetInstance()->SetBlackMask( gxTrue );
		m_bLoop = gxFalse;

		m_pTransShips->SetMove( 60 );

		new CEffScene0A02(320*100,32*100,PRIO_BG1-1,1);
		ControlHound( enHoundOpeStart );
		ResetZoneTimer();
		SetZoneSeq(10);
	}
	else if( IsZoneSeq(10) )
	{
		m_sScrollPos = 0;
		m_sScrollMin = (500-3*m_sZoneTimer/5)*100;
		m_sScrollMax = m_sScrollMin;//pGame->GetHound()->m_Pos.x - (WINDOW_W*100)+m_sZoneTimer*100;
		Sint32 n = m_sBombPosition;
		Sint32 sTbl[]={
			-100+600 ,479,PRIO_ENEMY,
			-100+750 ,479,PRIO_BG2+5,
			-100+900 ,479,PRIO_ENEMY,
			-100+850 ,479,PRIO_BG2+5,
			-100+1100,479,PRIO_ENEMY,
			-100+950 ,479,PRIO_ENEMY,
			-100+650 ,479,PRIO_BG2+5,
			-100+850 ,479,PRIO_ENEMY,
			-100+950 ,479,PRIO_BG2+5,
			-100+1100,479,PRIO_ENEMY,
		};

		switch( m_sZoneTimer ){//viiSub::Rand()%64==0)
		case 100:
		case 150:
		case 200:
		case 250:
		case 300:
		case 320:
		case 350:
		case 380:
		case 400:
		case 420:
			MakeSplash( sTbl[n*3+0]*100, pGame->GetMarineLine() ,sTbl[n*3+2] );
			viiMus::PlaySound( enSoundSplashL );
			pGame->QuakeOn(16);
			m_sBombPosition ++;
			m_sBombPosition = m_sBombPosition%10;
			break;
		case 30:
			viiMus::PlayBGM(enSoundBgm1,enMusicBgmStageB);
			break;
		}
		if( m_sZoneTimer == 300 )
		{
			CCockpit::GetInstance()->SetMessage(2000);
		}

		if( m_sZoneTimer == 400 )
		{
		}
		if( m_sZoneTimer == 320 )
		{
			CCockpit::GetInstance()->SetBlackMask( gxFalse );
			CCockpit::GetInstance()->SetMissionSec( 60*10 );
			SetGamePlay( gxTrue );
			ResetZoneTimer();
			SetZoneSeq(100);
		}

	}
	else if( IsZoneSeq(100) )
	{
		ControlHound( enHoundOpeShipDefence );

		if( m_sZoneTimer == 60 )
		{
			ChangeArea( enAreaSeq02 );
		}
	}

	pGame->GetHound()->SetPadButton( JOY_R );

	BgCommon();
//	viiSub::LimitMoveOnlyVH();

}



void CBaseRiver::Area02()
{
	//-----------------------------------------------------
	//エリア２:強行突破
	//-----------------------------------------------------

	m_sScrollPos += 60;
	m_sScrollMin = (200-300)*100+m_sScrollPos;
	m_sScrollMax = (200+300)*100+m_sScrollPos;

	if( m_sScrollMax > 6785*100 ) m_sScrollMax = 6785*100;
	if( m_sScrollMin > 6300*100 ) m_sScrollMin = 6300*100;

	if( m_sScrollPos >= 2000*100 )
	{
		ControlHound( enHoundOpeShipDefence2 );
	}
	else
	{
		ControlHound( enHoundOpeShipDefence );
	}

	if( IsZoneSeq(0) )
	{
		ResetZoneTimer();
		SetZoneSeq(10);
	}
	else if( IsZoneSeq(10) )
	{
		if( m_sScrollPos >= 1000*100 )
		{
			if( CCockpit::GetInstance()->SetMessage(2100))//くそっ、空中からの爆雷はなんとか
			{
				if( m_pMiddleBoss[0] == NULL )
				{
					m_pMiddleBoss[0] = new CEneH0203Serpent(  -1800*100,550*100 );
					SetZoneSeq(20);
				}
			}
		}

	}
	else if( IsZoneSeq(20) )
	{
		if( m_sScrollPos >= 1900*100 )
		{
			CCockpit::GetInstance()->SetMessage(2200);	//水中、後方に戦略機動兵器を確認！！
		}

		if( m_sScrollPos >= 2500*100 )
		{
			CCockpit::GetInstance()->SetMessage(2300);	//ライ姉！そっちに１機逃げた

			if( m_sScrollPos >= 2500*100 && m_sScrollPos <= 3600*100 )
			{
				if( m_pMiddleBoss[0] )
				{
					CCockpit::GetInstance()->SetArrow(4000);
				}
			}
		}

		if( m_sScrollPos >= 3400*100 )
		{
			if( m_pMiddleBoss[0] )
			{
				if( m_pMiddleBoss[0]->IsDead() )
				{
					m_pMiddleBoss[0]->Clear();
					m_pMiddleBoss[0] = NULL;
				}
			};
			ChangeArea( enAreaSeq03 );
		}
	}

	if( m_pMiddleBoss[0] )
	{
		if( m_pMiddleBoss[0]->IsDead() )
		{
			m_pMiddleBoss[0]->Clear();
			m_pMiddleBoss[0] = NULL;
		}
	};

	BgCommon();
	viiSub::LimitMoveVH();

}

void CBaseRiver::Area03()
{
	//-----------------------------------------------------
	//エリア３:強行突破２
	//-----------------------------------------------------

	m_sScrollPos += 60;
	m_sScrollMin = (200-300)*100+m_sScrollPos;
	m_sScrollMax = (200+300)*100+m_sScrollPos;

	if( m_sScrollMax > 6785*100 ) m_sScrollMax = 6785*100;
	if( m_sScrollMin > 6300*100 ) m_sScrollMin = 6300*100;

	ControlHound( enHoundOpeShipDefence2 );

	if( IsZoneSeq(0) )
	{
		m_pMiddleBoss[1] = new CEneH0203Serpent(  1500*100,550*100 );

		if( m_pTransShips == NULL )
		{
			m_sScrollPos = 3400*100;
			m_pTransShips = new CEneH0207TransShips( 432430 , 51200 );
			m_pTransShips->SetMove( 60 );
			new CEffScene0A02(320*100,32*100,PRIO_BG1-1,1);
			viiMus::PlayBGM(enSoundBgm1,enMusicBgmStageB);
		}

		ResetZoneTimer();
		SetZoneSeq(10);
	}
	else if( IsZoneSeq(10) )
	{
		if( m_sScrollPos >= 4000*100 && m_sScrollPos <= 4500*100 )
		{
			CCockpit::GetInstance()->SetMessage(2500);	//バツ！逃げろ。こいつはただもん
		}
		if( m_sScrollPos >= 4360*100 && m_sScrollPos <= 5000*100 && m_pMiddleBoss[1] )
		{
			CCockpit::GetInstance()->SetArrow(4000);
		}

		Sint32 sx,sy;
		viiSub::GetScroll_s( sx,sy );
		if( m_sScrollPos >= 5700 && sx >= 6700 )
		{
			CCockpit::GetInstance()->SetMessage(2600);	//輸送船、安全圏に脱出！
			viiMus::StopBGM( enSoundBgm1 , 180 );

			//-------------------------------------------
			//サーペント残骸処理
			//-------------------------------------------
			if( m_pMiddleBoss[0] )
			{
				m_pMiddleBoss[0]->Clear();
				m_pMiddleBoss[0] = NULL;
			}
			if( m_pMiddleBoss[1] )
			{
				m_pMiddleBoss[1]->Clear();
				m_pMiddleBoss[1] = NULL;
			}

			ChangeArea( enAreaSeq04 );
		}
	}

	if( m_pMiddleBoss[0] )
	{
		if( m_pMiddleBoss[0]->IsDead() )
		{
			m_pMiddleBoss[0]->Clear();
			m_pMiddleBoss[0] = NULL;
		}
	};
	if( m_pMiddleBoss[1] )
	{
		if( m_pMiddleBoss[1]->IsDead() )
		{
			m_pMiddleBoss[1]->Clear();
			m_pMiddleBoss[1] = NULL;
		}
	};
	BgCommon();
	viiSub::LimitMoveVH();

}


void CBaseRiver::Area04()
{
	//-----------------------------------------------------
	//エリア４:ボス
	//-----------------------------------------------------

	m_sScrollMax = 6800*100;
	m_sScrollMin = 6300*100;

	if( IsZoneSeq(0) )
	{
//		viiMus::PlayBGM( enSoundBgm1 , enMusicBgmStageB );
//		viiMus::StopBGM( 0 , 90 );
		m_sSplahCnt    = 0;
		m_sSplahHeight = 0;
		CCockpit::GetInstance()->AddMissionSec( 3*10 );
		ResetZoneTimer();
		SetZoneSeq( 100 );
	}
	else if( IsZoneSeq(100) )
	{
		//出現演出

		Sint32 sTbl[]={
			6500+64*0 ,479,PRIO_ENEMY,
			6500+64*1 ,479,PRIO_BG2+5,
			6500+64*2 ,479,PRIO_ENEMY,
			6500+64*3 ,479,PRIO_BG2+5,
			6500+64*4 ,479,PRIO_ENEMY,
			6500+64*5 ,479,PRIO_ENEMY,
			6500+64*6 ,479,PRIO_BG2+5,
			6500+64*7 ,479,PRIO_ENEMY,
			6500+64*8 ,479,PRIO_BG2+5,
			6500+64*9 ,479,PRIO_ENEMY,
		};


		CCockpit::GetInstance()->SetArrow(3000);

		if( m_sZoneTimer%24==0)
		{
			if( viiSub::Rand()%4 )
			{
				viiMus::PlaySound( enSoundSplashS );
			}
		}

		if( m_sZoneTimer%5==0)
		{
			m_sSplahHeight += 1;
			MakeSplash( sTbl[ m_sSplahCnt*3+0]*100, (pGame->GetMarineLine()+6400)-(viiSub::Rand()%(m_sSplahHeight))*100 ,sTbl[m_sSplahCnt*3+2] );
			m_sSplahCnt ++;
			m_sSplahCnt = m_sSplahCnt%8;
		}
		if( m_sZoneTimer == 550)
		{
			SetZoneSeq( 1000 );
		}
	}
	else if( IsZoneSeq(1000) )
	{
		//ヘビ精製
		m_pBoss = new CEneH0203Snake( (6518+64)*100 , 620*100 );
		viiMus::PlaySound( enSoundSplashL );
		SetZoneSeq( 1010 );
	}
	else if( IsZoneSeq(1010) )
	{
		if( !viiMus::IsPlayBGM( enSoundBgm1 ) )
		{
			viiMus::PlayBGM(enSoundBgm2,enMusicBgmBossSimple);
			SetZoneSeq( 1020 );
		}
	}
	else if( IsZoneSeq(1020) )
	{
		if( m_pBoss )
		{
			if( m_pBoss->IsCrash() )
			{
				pGame->GetHound()->SetMuteki();
			}
	
			if( m_pBoss->IsDead() )
			{
				m_pBoss->Destroy();
				m_pBoss = NULL;
				SetZoneSeq( 1030 );
			}
		}
	}
	else if( IsZoneSeq(1030) )
	{
		//次のラウンドへ
		ChangeArea( enAreaSeq05 );
	}

	BgCommon();
	viiSub::LimitMoveVH();

}


void CBaseRiver::Area05()
{
	//-----------------------------------------------------
	//エリア５：ステージクリア
	//-----------------------------------------------------

	if( IsZoneSeq( 0 ) )
	{
		//クリア
		SetGamePlay( gxFalse );
		CCockpit::GetInstance()->SetMessage(3000);//オペレーションレポート。
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


void CBaseRiver::AreaCommon()
{
	//-----------------------------------------------------
	//エリア：共通
	//-----------------------------------------------------
	if( m_pTransShips && m_pTransShips->IsAllItemDead() )
	{
		if( !CCockpit::GetInstance()->IsMessageNow() )
		{
			if( m_sGameOverWait >= 120 )
			{
				CCockpit::GetInstance()->SetMessage(9800);	//貨物が全てやられたのかっ！！
				CCockpit::GetInstance()->SetMessageEnable(gxFalse);	//以降のメッセージを禁止する
			}
			m_sGameOverWait ++;
		}

		if( !CCockpit::GetInstance()->IsMessageNow() )
		{
			if( m_sGameOverWait >= 200 )
			{
				pGame->GetHound()->SetGameOver();
			}
		}
	}

	stRiver.m_sScroll += stRiver.m_sAdd;
	stRiver.m_sScroll =  stRiver.m_sScroll%51200;

	stRiver2.m_sScroll += stRiver2.m_sAdd;
	stRiver2.m_sScroll =  stRiver2.m_sScroll%51200;

	stRiver3.m_sScroll += stRiver3.m_sAdd;
	stRiver3.m_sScroll =  stRiver3.m_sScroll%51200;

/*
	if( m_bLoop )
	{
		stRiver.m_sInfinityScroll += stRiver.m_sAdd;
		stRiver.m_sInfinityScroll = stRiver.m_sInfinityScroll%(480*100);
	}
	else
	{
		if( stRiver.m_sAdd > 0 )
		{
			m_stRiverOffset.x += stRiver.m_sAdd;
			Sint32 sOffset = gxAbs(m_stRiverOffset.x)+stRiver.m_sInfinityScroll;
			if( sOffset >= 480*400 )
			{
				stRiver.m_sAdd --;
			}
		}
	}
*/

}


//---------------------------------------------------------------------------
//ＶＭＰから敵の生成
//---------------------------------------------------------------------------


void CBaseRiver::SetEnemy(Sint32 sMin)
{
	CEneH0201VorzoyFly *pVorzoyFly;

	for(Sint32 y=0; y<pGame->pBg->getHeight()/8; y++)
	{
		for(Sint32 x=sMin/8; x<pGame->pBg->getWidth()/8; x++)
		{
			Sint32 sIndex = pGame->pBg->get_vmpdat(x,y);

			sIndex = EnemyIndex[sIndex];

			switch(sIndex){

			case enCEneH0207TransShips:
				m_pTransShips = new CEneH0207TransShips(x*800,y*800);
				break;

			case enCEneH0101HeavyTank:	//味方重戦車
				break;

			case enCEneH0203Serpent:
				//m_pBoss = new CEneH0203Serpent(  x*800,y*800 );
				break;

			case enCEneH0108Hovered:
				new CEneH0108Hovered( x*800 , y*800 );
				break;

			case 138:
				{
					CEneH0108Hovered * p;
					p = new CEneH0108Hovered( x*800 , y*800 );
					p->SetReverse();
				}
				break;

			case enCEneH0103ClubGunner:
				break;

			case enCEneH0201VorzoyFly:
				pVorzoyFly = new CEneH0201VorzoyFly( x*800 , y*800 );
				pVorzoyFly->SetTarget( &pGame->GetHound()->m_Pos.x , -160 );
				//m_sTransShipX
				break;

			case enCEneH0201VorzoyFlyAfter:
				pVorzoyFly = new CEneH0201VorzoyFly( x*800 , y*800 );
				pVorzoyFly->SetTarget( &pGame->GetHound()->m_Pos.x , -160 );
				break;

			case enCEneH0003Vorzoy:
				new CEneH0202VorzoyWP( x*800 , y*800 );
				break;
			default:
				MakeHoundsEnemy(x*800,y*800,sIndex);
				break;
			}

		}
	}

}


void CBaseRiver::BgCommon(Sint32 x,Sint32 y)
{
	//----------------------------------------------------
	//背景共通
	//----------------------------------------------------
	BgScrollAdjust();

	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );

	StLayerScroll *pScr;

	LayerScroll[enBgTypeBack].oy = 150;
	for(Sint32 ii=enBgTypeBack; ii<=enBgTypeFrnt; ii++)
	{
		pScr = &LayerScroll[ii];
		viiSub::SetBgLayer( ii , pScr->ox+(sx*pScr->x)/100 , pScr->oy+(sy*pScr->y)/100 );
	}

	viiSub::SetBgLayer( enBgTypeRear ,pScr->ox+(sx+(m_stRiverOffset.x/100)+stRiver.m_sInfinityScroll/100) , pScr->oy+sy*pScr->y/100 );

}


void CBaseRiver::Draw()
{
	//-------------------------------------
	//空
	//-------------------------------------

	gxSprite StSprCloud[]={
		{enTexPageBgObj3,0,0,480,256,0,0},
	};
	viiDraw::Spr_NoScrl( &StSprCloud[ 0 ] , 0,0,PRIO_BG1-1 , ATR_DFLT , 0x40FFFFFF );

	Sint32 col1 =  0xF00040A0;
	Sint32 col2 =  0xF00080A0;
	pGame->pBg->SetSkyColor(30,col1,col2);

	//------------------------------------
	//河の処理
	//------------------------------------
	Sint32 sx,sy,ii,kx;
	Sint32 vx = GetTargetPlayer()->x;

	kx =  ( (vx/100)/512 )*51200 - 51200;

	//-------------------------------------
	//手前
	//-------------------------------------

	sx = kx-stRiver.m_sScroll;
	sy = 492;

	for(ii=0; ii<5; ii++)
	{
		viiDraw::Sprite( &SprStageH02River[ 4 ] , sx+(0    )+ii*51200 , sy*100,PRIO_BG3-1 , ATR_DFLT , 0xF0FFFFFF );
		viiDraw::Sprite( &SprStageH02River[ 5 ] , sx+(25600)+ii*51200 , sy*100,PRIO_BG3-1 , ATR_DFLT , 0xF0FFFFFF );
	}

	//-------------------------------------
	//真ん中
	//-------------------------------------

	sx = kx-(stRiver2.m_sScroll);
	sy = 482;

	for(ii=0; ii<5;ii++)
	{
		viiDraw::Sprite( &SprStageH02River[ 2 ] , sx+(0    )+ii*51200 , sy*100,PRIO_BG2+2 , ATR_DFLT , 0xF0ffffff );
		viiDraw::Sprite( &SprStageH02River[ 3 ] , sx+(25600)+ii*51200 , sy*100,PRIO_BG2+2 , ATR_DFLT , 0xF0ffffff );
	}

	//-------------------------------------
	//奥
	//-------------------------------------

	sx = kx-(stRiver3.m_sScroll);
	sy = 472;

	for(ii=0; ii<5;ii++)
	{
		viiDraw::Sprite( &SprStageH02River[ 0 ] , sx+(0    )+ii*51200 , sy*100,PRIO_BG2+1 , ATR_DFLT , 0xF0ffffff );
		viiDraw::Sprite( &SprStageH02River[ 1 ] , sx+(25600)+ii*51200 , sy*100,PRIO_BG2+1 , ATR_DFLT , 0xF0ffffff );
	}


}

gxBool CBaseRiver::WaraWaraCommanch()
{
	//----------------------------------------
	//ワラワラコマンチ
	//----------------------------------------
	Sint32 sCnt = 0;

	for(Sint32 ii=0;ii<enCommanchMax; ii++)
	{
		if( m_pCommanch[ii] == NULL )
		{
			if( m_sWaraWaraCnt < enWaraWaraMax )
			{
				m_pCommanch[ii] = new CEneH0002Commanch( (3000+(ii*300))*100 , (300+ii*20)*100 ,gxTrue );
				m_pCommanch[ii]->SetTarget( &m_pTransShips->m_Pos );
				m_sWaraWaraCnt ++;
			}
			else
			{
				sCnt ++;
			}
		}
		else if( m_pCommanch[ii]->IsDead() )
		{
			m_pCommanch[ii]->Clear();
			m_pCommanch[ii] = NULL;
		}
	}

	if( m_sWaraWaraCnt >= enWaraWaraMax )
	{
		return gxTrue;
	}

	return gxFalse;
}


void CBaseRiver::ControlHound(Sint32 sOperation )
{
	//-----------------------------------------
	//ハウンドのコントロール
	//-----------------------------------------

	switch(sOperation){
	case enHoundOpeStart:
		//-------------------------------
		//新規ハウンドをセット
		//-------------------------------
		m_pTransShips->m_Pos.x = 420*100;
		m_pHound[enHoundMaster] = new CGunHound( 460*100,519*100 ,gxFalse);
		m_pHound[enHoundMaster]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundMaster]->SetLeader(gxTrue);
		m_pHound[enHoundMaster]->m_Pos.z = PRIO_ENEMY-4;
		m_pHound[enHoundMaster]->SetLogicPattern( 200 );
		m_pHound[enHoundMaster]->SetTarget( 1000*100 , 519*100 );
		m_pHound[enHoundMaster]->SetWaterProof();

		m_pHound[enHoundSlave] = new CGunHound( 0*100,519*100 ,gxFalse);
		m_pHound[enHoundSlave]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundSlave ]->m_Pos.z = PRIO_PLAYER-1;
		m_pHound[enHoundSlave]->SetLogicPattern( 200 );
		m_pHound[enHoundSlave]->SetTarget( 1000*100 , 519*100 );
		m_pHound[enHoundSlave]->SetWaterProof();

		pGame->GetHound()->m_Pos.x += 0*100;
		break;

	case enHoundOpeShipDefence:
		//-------------------------------
		//輸送船を護衛
		//-------------------------------
		m_pHound[enHoundMaster]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundMaster]->SetLogicPattern( 210 );
		m_pHound[enHoundMaster]->SetTarget( m_pTransShips->m_Pos.x+80*100 , m_pTransShips->m_Pos.y );

		if( m_pHound[enHoundSlave ] )
		{
			m_pHound[enHoundSlave]->SetMode( CGunHound::enHoundModeNPC );
			m_pHound[enHoundSlave]->SetLogicPattern( 220 );
			m_pHound[enHoundSlave]->SetTarget( m_pTransShips->m_Pos.x+640*100 , m_pTransShips->m_Pos.y );
			if( m_pHound[enHoundSlave ]->m_Pos.x > m_pTransShips->m_Pos.x+640*100 )
			{
				delete m_pHound[enHoundSlave ];
				m_pHound[enHoundSlave ] = NULL;
			}
		}
		break;

	case enHoundOpeShipDefence2:
		if( m_pHound[enHoundMaster ] )
		{
			m_pHound[enHoundMaster]->SetMode( CGunHound::enHoundModeNPC );
			m_pHound[enHoundMaster]->SetLogicPattern( 220 );
			m_pHound[enHoundMaster]->SetTarget( m_pTransShips->m_Pos.x-640*100 , m_pTransShips->m_Pos.y );
			if( m_pHound[enHoundMaster ]->m_Pos.x < m_pTransShips->m_Pos.x-640*100 )
			{
				delete m_pHound[enHoundMaster ];
				m_pHound[enHoundMaster ] = NULL;
			}
		}
		break;


	}
}

