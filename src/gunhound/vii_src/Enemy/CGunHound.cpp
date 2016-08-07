//--------------------------------------------------------------------------------
//@
//@
// HOUNDプロトタイプ
//@
//@
//--------------------------------------------------------------------------------

#include <gunvalken.h>
#include <gxLib/util/command/CWazaCommand.h>
#include "enemy.h"
#include "../CPenduram.h"
#include "../Effect/EffectBase.h"
#include "CGunHoundTbl.h"
#include "../GunHound/menu/CCustomHound.h"
#include "../Gunhound/Cmenu.h"
#include "../GunHound/GhStringTbl.h"

//#define _WRITE_KEYLOG_

enum {
	enJumpSeqJumpNone = 0,
	enJumpSeqJumpNow,
	enJumpSeqHoveringReady,
	enJumpSeqHoveringNow,
};

enum {
	enSoundHoundMainEngineBroken,
	enSoundHoundRightSideBroken,
	enSoundHoundShieldDestroyed,
};

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -16,
	enKuraiTop   = -34,
	enKuraiRight =  16,
	enKuraiBottom=  0,

	enScore = 300,

};

enum {
	enMutekiTimeMax = 24,		//無敵時間
	enHitPoint    = 10000,		//耐えられるダメージ量
	enDamagePoint = 600,		//1250,		//１度に受けるダメージ量(2016 半分にしてみる)
	enRakkaSpeed = 16,			//落下速度
	enRakkaSpeedGake = 80,
	enRakkaMax   = 600,
	enJumpSpeed  = 280,
	enWalkSpeed  = 32,
	enWalkGakeSpeed  = 80,
	enWalkJumpSpeed  = 15,
	enWalkJumpCntMax = 20,
	enWalkMax    = 120,

	enForceJumpCnt = 4,

	enDashAddSpeed    = 45,
	enDashSpeedMax    = 350,

	enHoverEnergyMax = 80,	//ホバー持続時間
	enHoverSpeedAdd  = 11,	//フルパワーまでの時間(100/12 frm)
	enHoverPowerMax  = 22,	//反重力値(1/60)限界

	enSpaceWalkMax    = 180,
	enGakeWalkMax     = 360,
	enWalkCycles=8,

	enPunchTimeLag = 24,
	enBazookaMax   = 2,
	enBazookaOne   = 1,
	enMachingunOne = 4,
	enThreeWay     = 6,
	enHoverLag = 20,
	enFreezeWait = 14,
	enJumpWaitNormal = 8,
	enJumpWaitLight  = 6,
};


enum {
	enStatusArmorBone = 0,
	enStatusArmorFrame,
	enStatusArmorShield,
	enStatusArmorMax,
};

CGunHound::CGunHound( Sint32 x, Sint32 y ,gxBool bPlayer )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_bPlayer = bPlayer;
	m_bLightWeightHound = gxFalse;
	m_sJumpOmosi = enJumpWaitNormal;

	m_bOffenceControll = gxTrue;
	m_bOutofScreen     = gxFalse;

	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_PLAYER;

	m_Add.x = 0;
	m_Add.y = 0;

	m_Force.x = 0;
	m_Force.y = 0;

	m_sColor = 0xFFFFFFFF;
	m_bGameOver = gxFalse;
	m_sFreezeTimeMax = enFreezeWait;

	CCockpit::GetInstance();

	for(Sint32 ii=0;ii<enDamageMax;ii++)
	{
		m_HitKurai[ii].set_hp( 5 );
		m_sArmor[ii] = enStatusArmorShield;
		m_sHitEffectCnt[ii] = 0;
	}

	m_pMech = new CMechBone();
	m_pObj  = new CObj();

	SetPrio(enActionPrio70);

	m_bRideOn = gxFalse;
	m_pRidePos = NULL;

	m_pMech->SetPos( 160*100 , 240*100 );

	m_eMode        = enHoundModeNormal;	//enHoundModeSpace;
	m_sControlPush     = 0;
	m_sControlTrig     = 0;
	m_sArmRotation = 0;
	m_sDirection   = DIR_RIGHT;
	m_sGravitation = 0;
	m_sRollerCnt   = 0;
	m_sJumpCnt     = 0;
	m_sSandStormCnt = 0;
	m_bSignal       = gxFalse;
	m_sJumpRate = 0;
	m_bBoost        = gxFalse;
	m_bBurnia = gxFalse;

	//自動操作用
	m_bCpuControl = gxFalse;
	m_sForcePadControlPush = 0;
	m_sForcePadControlTrig = 0;

	//崖滑りよう
	m_sPadRotation = 0;
	m_sPadSpeed = 0;

	m_bWaterEffect = gxFalse;

	m_stAtack[enShotN1].sRotation = 0;

	m_sHitPoint = enHitPoint;
	if( m_bPlayer ) CCockpit::GetInstance()->SetHitPoint( m_sHitPoint );

	m_sMutekiTime = 0;

	m_bForceRollerDash = gxFalse;
	m_bForceShift      = gxFalse;
	m_bForceBurnia     = gxFalse;

	m_pObj->pos.x = m_Pos.x;
	m_pObj->pos.y = m_Pos.y;
	m_pObj->mov.x = 0;
	m_pObj->mov.y = 0;
	m_pObj->atari1.x =  -100;
	m_pObj->atari1.y =  -100;
	m_pObj->atari2.x =  100;
	m_pObj->atari2.y =  100;

	m_sBoosterRot = 0;
	m_bSubWeaponReady = gxFalse;

	m_bLeader  = gxFalse;
	m_bBooster = gxFalse;

	m_bAutoPunch = gxFalse;

	//武器のセッティング

	if( m_bPlayer )
	{
		SetWeapon( enShotN1, g_CHoundSaveData.m_StConfig.sMasterMainShot );	//enWeaponMachineGun );
		SetWeapon( enShotN2, g_CHoundSaveData.m_StConfig.sMasterSubShot  );	//enWeaponArmHardBrow );
		SetWeapon( enShotS1, g_CHoundSaveData.m_StConfig.sSlaveMainShot  );	//enWeaponBazooka  );
		SetWeapon( enShotS2, g_CHoundSaveData.m_StConfig.sSlaveSubShot   );	//enWeaponPodMissile  );
		if( g_CHoundSaveData.IsHiddenOpen( enHiddenItemAddWeaponDiverAntenna ) )
		{
			SetLeader( gxTrue );
		}

		//ノーアーマー用のライト設定
		if( g_CHoundSaveData.m_StConfig.sNoArmor )
		{
			m_bLightWeightHound = gxTrue;
			m_sFreezeTimeMax = 0;			//ライトハウンドは着地硬直なし
			m_sJumpOmosi = enJumpWaitLight;	//ライトハウンドは高くジャンプできる

			for(Sint32 ii=0;ii<enDamageMax;ii++)
			{
				m_sArmor[ii] = enStatusArmorBone;
			}
		}
		if( m_stAtack[enShotN2].sType == enWeaponArmZgokCrow )
		{
			//ズゴッククローはオートパンチ
			m_bAutoPunch = gxTrue;
		}
	}
	else
	{
	//テスト用
		SetWeapon(enShotN1 , enWeaponMachineGun  );
		SetWeapon(enShotS1 , enWeaponBazooka  );
		SetWeapon(enShotN2 , enWeaponArmHardBrow );
		SetWeapon(enShotS2 , enWeaponPodMissile  );
	}

//	m_stAtack[enShotN1].sMaxRemain = 100;
//	m_stAtack[enShotN2].sMaxRemain = 1;
//	m_stAtack[enShotS1].sMaxRemain = enBazookaMax;
//	m_stAtack[enShotS2].sMaxRemain = 4;
//	m_stAtack[enShotN1].Relord();
//	m_stAtack[enShotN2].Relord();
//	m_stAtack[enShotS1].Relord();
//	m_stAtack[enShotS2].Relord();

	m_sFreezeWait = 0;

	m_bChakuti = gxFalse;
	m_sJumpSeq = 0;
	m_sHoverEnergy = 0;
	m_sHoverSpd = 0;
	m_sHoverLag = 0;
	m_bHoverAble = gxTrue;

	m_bSearchLight   = gxFalse;
	m_bForceTobidasi = gxFalse;
	m_bSwitchNow     = gxFalse;
	m_bInfinityHover = gxFalse;

//	LoadTexture(enTexPageGunHound,"HoundData\\tex\\AsNew.bmp",0xff00ff00);
//	UploadTexture();

	m_sGravitSpeed = 0;

	m_pMech->SetDefault();

	//-----------------------------------
	//ハウンドの色設定
	//-----------------------------------

	if( m_bPlayer )
	{
		CCustomHound *pCCustomHound;
		pCCustomHound = new CCustomHound();
		pCCustomHound->SetBodyColor( m_pMech );
		delete pCCustomHound;
	}

	//-----------------------------------
	//振り子設定
	//-----------------------------------
	m_pPenduram = new CPenduram();
	m_pPenduram->SetWeight(10);

	//-----------------------------------
	//技コマンド
	//-----------------------------------

#if defined _WRITE_KEYLOG_
	m_pWazaCommand = new CWazaCommand( 60*180 );
	m_pWazaCommand->Rec();
#else

	m_sHitPointStatus = enHitPointStausNormal;

	if( g_StGameInfo.m_bReplay )
	{
		if( m_bPlayer )
		{
			m_pWazaCommand = new CWazaCommand( 60*180 );
			char *pBuf = (char*)malloc( sizeof(Uint32)*60*180 );
			
			switch( g_StGameInfo.PlayStage ){
			case enScenarioJungle:
				{
					Uint32 uSize = 0;
					Uint8 *pData;
					pData = gxLib::LoadFile("asset/gh/GXI/Stage1.bin", &uSize);
					if (pData)
					{
						memcpy( pBuf, pData , sizeof(Uint32) * 60 * 180);
						delete[] pData;
					}
				}
				break;

			default:
			case enScenarioRiver:
				Uint32 uSize = 0;
				Uint8 *pData;
				pData = gxLib::LoadFile("asset/gh/GXI/Stage2.bin", &uSize);
				if (pData)
				{
					memcpy(pBuf, pData, sizeof(Uint32) * 60 * 180);
					delete[] pData;
				}
				break;
			}
			m_pWazaCommand->Play(pBuf);
			free(pBuf);

			//リプレイ時は武装をデフォルト設定にする
			SetWeapon( enShotN1, enWeaponMachineGun );
			SetWeapon( enShotN2, enWeaponArmHardBrow );
			SetWeapon( enShotS1, enWeaponBazooka  );
			SetWeapon( enShotS2, enWeaponPodMissile  );
		}
		else
		{
			m_pWazaCommand = new CWazaCommand();
		}
	}
	else
	{
		m_pWazaCommand = new CWazaCommand();
	}

#endif

//	SetBooster(gxTrue);
//	SetLeader(gxTrue);

	m_sNPCLogicPattern = 0;
	m_sNPCLogicCnt = 0;
	m_NpcTgt.x = m_Pos.x;
	m_NpcTgt.y = m_Pos.y;
	m_bNoDamage = gxFalse;
	m_bDead     = gxFalse;

	if( !m_bPlayer ) m_bNoDamage = gxTrue;

}


CGunHound::~CGunHound()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------
	m_pWazaCommand->Stop();
	delete m_pWazaCommand;
	delete m_pMech;
	delete m_pObj;
	delete m_pPenduram;
}


void CGunHound::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------
	gxBool bOldDash = (m_sRollerCnt==0)? gxFalse : gxTrue;

	//画面外か？
	m_bOutofScreen = viiSub::IsScrollOut( m_Pos.x , m_Pos.y , 32*100 , 32*100 );

//	m_pMech->SetDefault();
/*
if(Joy[0].psh&BTN_R1 )
	{
		CCockpit::GetInstance()->SetSplash(gxTrue);
	}
	else
	{
		CCockpit::GetInstance()->SetSplash(gxFalse);
	}
*/


	//-------------------------------
	//コントロール制御
	//-------------------------------
	if( g_StGameInfo.m_bReplay )
	{
		if( m_bPlayer )
		{
			//リプレイモード時
			Sint32 sOldKey = m_sControlPush;
			m_sControlPush = m_pWazaCommand->GetLog();
			m_sControlTrig = m_sControlPush&(~sOldKey);
		}
	}
	else 
	{
		if( m_bPlayer )
		{
			m_sControlPush = 0;
			m_sControlTrig = 0;
			m_sControlPush = Joy[0].psh;
			m_sControlTrig = Joy[0].trg;
		}

	}

	if( !m_bOffenceControll )
	{
		//パンチ中などはオフェンスコントロール無効の為、追加入力しか受け付けない
		Sint32 sButton = BTN_Y|BTN_A;
		if( m_bAutoPunch )
		{
			sButton |= BTN_X;
		}

		m_sControlPush &= sButton;
		m_sControlTrig &= sButton;
	}

	if( m_bGameOver )
	{
		m_sControlPush = 0;
		m_sControlTrig = 0;
	}

	m_pWazaCommand->SetLog( m_sControlPush );
	//--------------------------------

	m_Old = m_Pos;

	if( m_sFreezeWait > 0)
	{
		m_sFreezeWait --;
	}

	gxBool bChakutiOld = m_bChakuti;
	m_bChakuti = gxFalse;

	if( m_pObj->is_landnow() )
	{
		m_bChakuti = gxTrue;
	}

//	if( m_Add.y < 0 ) m_bRideOn = gxFalse;
	if( m_bRideOn )
	{
		m_bChakuti = gxTrue;
		m_Pos.x += m_pRidePos->x;
		m_Pos.y += m_pRidePos->y;
	}
	m_bRideOn = gxFalse;

	LogicAI();
	Controls();

	m_pObj->mov.x = m_Add.x;
	m_pObj->mov.y = m_Add.y;

	m_pObj->mov.y += m_sGravitation;

	m_pObj->pos.x = m_Pos.x;
	m_pObj->pos.y = m_Pos.y;

	if( m_bGameOver )
	{
		m_Add.x = 0;
		m_Add.y = 0;
		m_pObj->mov.x = m_Force.x = 0;
		m_pObj->mov.y = m_Force.y = 0;
	}
	else
	{
		m_pObj->adjust_move(0);
	}

	m_Add.x = m_pObj->mov.x;
	m_Add.y = m_pObj->mov.y;

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	m_pObj->pos.x = m_Pos.x;
	m_pObj->pos.y = m_Pos.y;

	//---------------------------
	//強制移動
	//---------------------------

	m_pObj->mov.x = m_Force.x;
	m_pObj->mov.y = m_Force.y;

	m_pObj->pos.x = m_Pos.x;
	m_pObj->pos.y = m_Pos.y;

	m_pObj->m_bNoCheckHalfWall = gxTrue;
	m_pObj->adjust_move(0);

	m_pObj->pos.x += m_pObj->mov.x;
	m_pObj->pos.y += m_pObj->mov.y;

	m_Pos.x = m_pObj->pos.x;
	m_Pos.y = m_pObj->pos.y;

	m_Force.x = 0;
	m_Force.y = 0;
	//---------------------------

	m_pObj->mov.y = m_Add.y;

	if( !bChakutiOld && m_pObj->is_landnow() )
	{
		//---------------------------------------
		//着地の判定
		//---------------------------------------
		if(m_sGravitation >= 300)
		{
			if( pGame->GetMarineLine() && m_Pos.y > pGame->GetMarineLine() )
			{
			}
			else
			{
				viiMus::PlaySound( enSoundLanding , m_bOutofScreen );
			}
		}
		if(m_sGravitation >= 450)
		{
			gxBool bQuake = gxTrue;
			if( m_bOutofScreen ) bQuake = gxFalse;		//画面外では振動させない
			if( !m_bPlayer )	 bQuake = gxFalse;		//プレイヤー機以外は振動させない
			if( pGame->GetMarineLine() )
			{
				if( m_Pos.y > pGame->GetMarineLine() )
				{
					bQuake = gxFalse;	//水中では振動させない
				}
			}

			if( bQuake )
			{
				pGame->QuakeOn(16);
				m_sFreezeWait = m_sFreezeTimeMax;
			}
		}
		m_sJumpSeq = enJumpSeqJumpNone;
		GimmickLanding();
	}
	else if( !m_bChakuti && m_pObj->is_landnow() )
	{
		m_pObj->mov.y -= 800;

		m_pObj->pos.x = m_Pos.x;
		m_pObj->pos.y = m_Pos.y;

		m_pObj->adjust_move(0);

//		m_Add.x = m_pObj->mov.x;
//		m_Add.y = m_pObj->mov.y;

		m_Pos.x += m_Add.x;
		m_Pos.y += m_Add.y;

		m_pObj->pos.x = m_Pos.x;
		m_pObj->pos.y = m_Pos.y;

	}

	if( !m_pObj->is_landnow() )
	{
		m_bChakuti = gxFalse;
	}


	//くらい判定登録
	//m_HitKurai.SetHantei(ID_VERSIS_DEF , &m_Pos );
	SetDamagePosition();

	//--------------------------------------------
	//コックピットへ状態の反映
	//--------------------------------------------

	//バーニアエネルギー
	if( m_bPlayer ) CCockpit::GetInstance()->SetBurniaEnergy( 100*(enHoverEnergyMax - m_sHoverEnergy) /  enHoverEnergyMax );

	//武器残量ゲージ
	if( m_bSubWeaponReady )
	{
		Sint32 n = enShotS1;
		if( m_bPlayer ) CCockpit::GetInstance()->SetWeaponIcon( 0 , m_stAtack[n].sIcon , m_stAtack[n].sGaugeType , m_stAtack[n].sRemainBullet );

		n = enShotS2;
		if( m_bPlayer ) CCockpit::GetInstance()->SetWeaponIcon( 1 , m_stAtack[n].sIcon , m_stAtack[n].sGaugeType , m_stAtack[n].sRemainBullet );
	}
	else
	{
		Sint32 n = enShotN1;
		if( m_bPlayer ) CCockpit::GetInstance()->SetWeaponIcon( 0 , m_stAtack[n].sIcon , m_stAtack[n].sGaugeType , m_stAtack[n].sRemainBullet );

		n = enShotN2;
		if( m_bPlayer ) CCockpit::GetInstance()->SetWeaponIcon( 1 , m_stAtack[n].sIcon , m_stAtack[n].sGaugeType , m_stAtack[n].sRemainBullet );
	}
	//サブウェポンランプ処理
	gxBool bLamp = gxTrue;
	if( m_stAtack[enShotS2].sRelordTime )
	{
		bLamp = gxFalse;
	}

	if( m_bPlayer ) 
	{
		CCockpit::GetInstance()->SetSubWeaponLamp( bLamp );
		CRaderControl::GetInstance()->SetMarker( enTypeMarkerPlayer , m_Pos.x , m_Pos.y );
		CCockpit::GetInstance()->IsOperationArea( m_Pos.x );
	}

	//---------------------------------
	//HP回復制御
	//---------------------------------
	if( m_bPlayer )
	{
		RecoveryHitPoint();
		CCockpit::GetInstance()->SetHitPoint( m_sHitPoint );
	}

	//--------------------------------------------
	//各種エフェクトをコントロール
	//--------------------------------------------
	ControlGimmick();

	if( !bOldDash && IsRollerDash() )
	{
		viiMus::PlaySound( enSoundDash );
	}

	m_Force.x = 0;
	m_Force.y = 0;

	m_bCpuControl = gxFalse;

}


void CGunHound::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

	for(Sint32 ii=0;ii<enShotMax;ii++)
	{
		if( ii == enShotS1 )
		{
			//バズーカの時
		}

		if( m_stAtack[ii].sRelordTime > 0 )
		{
			m_stAtack[ii].sRelordTime --;
			if( m_stAtack[ii].sRelordTime == 0 )
			{
				m_stAtack[ii].Relord();
			}
		}
		if( m_stAtack[ii].sTimeLag > 0 )
		{
			m_stAtack[ii].sTimeLag --;
		}

//		if( ii == enShotS1 )
//		{
//			if(m_stAtack[enShotS1].sRemain < m_stAtack[enShotS1].sMaxRemain )
//			{
//				m_stAtack[enShotS1].sRemain ++;
//			};
//		}
	}

	if( m_stPunch.timelag )
	{
		//AtackMasterSub();
		ControlList( m_stAtack[enShotN2].sType , enShotN2 );
	}
	else
	{
		ControlAtack();
	}

}


void CGunHound::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	//viiMus::PlaySound(SE_ENEMY_OUT);
	//ScorePlus( enScore );
	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

	SetActionSeq( enActionSeqEnd );

}


void CGunHound::Draw()
{

	m_pMech->Draw();
	//viiDraw::Sprdmg( m_HitKurai.is_damage() , &SprCGunHound[ 0 ] , m_Pos.x , m_Pos.y,PRIO_ENEMY , ATR_DEFAULT , ARGB_DFLT );
	m_CEneCmnSignal.Draw();

	gxSprite SprSearchLight[]=
	{
		{enTexPageSearchLight,0,0,320,240,163,120},
	};

	if( m_bSearchLight )
	{
//		viiDraw::Box(0,0,WINDOW_W,WINDOW_H,PRIO_ENE_BULLET-1,gxTrue,ARGB(0xF0,0x00,0xFF,0x00) , ATR_ALPHA_REVERSE );//ATR_ALPHA_CROSS);
		viiDraw::Sprite( &SprSearchLight[0] , m_Pos.x , m_Pos.y-3000 , PRIO_ENE_BULLET-1 , ATR_ALPHA_MINUS , 0xC0010101 , 4.2f , viiSub::SetKakudo( m_sDirection,m_sArmRotation ) );
	}

}

void CGunHound::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	if( m_bCpuControl )
	{
		//自動コントロール
		m_sControlPush = m_sForcePadControlPush;
		m_sControlTrig = m_sForcePadControlTrig;
	}
}


void CGunHound::RecoveryHitPoint()
{
	//--------------------------------------------
	//ヒットポイント回復処理
	//--------------------------------------------

	if( m_sHitPoint <= enDamagePoint )
	{
//		if( m_sTimer%100 == 0 )
		{
			//やっぱうるさいから消すことにします(2016)
			//CCockpit::GetInstance()->SetWarningSound(100);//viiMus::PlaySound( enSoundOutofRange );
		}
	}

	if( m_sHitPoint > 0 )	//破壊されていなければ
	{
		if( m_sHitPoint < enHitPoint )//マックスでなければ
		{
			if( m_sMutekiTime == 0)		//無敵でなければ（無敵の間は回復までのラグ）
			{
//				Sint32 sAdd = (enHitPoint-m_sHitPoint)/10;
//				if(sAdd == 0) sAdd = 1;
//
//				m_sHitPoint += sAdd;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------


void CGunHound::ControlGimmick()
{
	//--------------------------------------------
	//各種エフェクト制御
	//--------------------------------------------

	//肩揺らしエフェクト
	m_pPenduram->Action();
	m_pMech->SetShoulderRotation( (Sint32)m_pPenduram->GetRotation() );

	m_pMech->SetPos( m_Pos.x , m_Pos.y , m_Pos.z);
	m_pMech->Action();

	//水面エフェクト
	if( m_bWaterEffect ) GimmickWaterEffect();

	//シグナルエフェクト
	ghPos stPos;

	m_pMech->getPosition( enPartsBurniaOption , &stPos );
	m_CEneCmnSignal.SetSignal( stPos.x,stPos.y);
	if( m_bSignal )
	{
		m_CEneCmnSignal.On();
		m_bSignal = gxFalse;
	}
	else
	{
		m_CEneCmnSignal.Off();
	}

}

void CGunHound::SetMode(Sint32 eMode)
{
	//-----------------------------------------------------
	//コントロール方法を切り替える
	//-----------------------------------------------------

	m_eMode = eMode;
	
}



void CGunHound::Controls()
{
	//-----------------------------------------------------
	//コントロール
	//-----------------------------------------------------
	m_bBoost = gxFalse;

	if( m_sRollerCnt > 0 )
	{
		m_sRollerCnt -= 6;
		if( m_sRollerCnt < 0 )
		{
			m_sRollerCnt = 0;
		}
	}


	//--------------------------------------------
	//サブウェポンセット
	//--------------------------------------------
	Sint32 sRotSpd = 4;
	m_bSubWeaponReady = gxFalse;

	m_bSwitchNow = gxTrue;

	if( m_sControlPush&BTN_R1 || m_bForceShift )
	{
		if( m_sControlTrig&BTN_R1)
		{
			viiMus::PlaySound(enSoundRelord , m_bOutofScreen );
		}

		m_bSubWeaponReady = gxTrue;
		Sint32 sAdd = (65-m_stAtack[enShotN1].sRotation)/sRotSpd;
		if(gxAbs(sAdd/sRotSpd) > sRotSpd )
		{
			sAdd = sAdd/sRotSpd;
		}
		else
		{
			if(sAdd == 0 )
			{
				if( m_stAtack[enShotN1].sRotation > 65+2 )
				{
					sAdd = -3;
				}else if( m_stAtack[enShotN1].sRotation < 65+-2 )
				{
					sAdd = 3;
				}
				else
				{
					m_stAtack[enShotN1].sRotation = 65;
					m_bSwitchNow = gxFalse;
				}
			}
		}
		m_stAtack[enShotN1].sRotation += sAdd;
		m_pMech->SetWeaponRotaion( 0, m_stAtack[enShotN1].sRotation );
		m_pMech->SetMainWeaponRotation(  -180*m_stAtack[enShotN1].sRotation/65 );
	}
	else
	{
		if( !(m_sControlPush&BTN_R1) && m_stAtack[enShotN1].sRotation == 65)
		{
			viiMus::PlaySound(enSoundRelord , m_bOutofScreen );
		}

		Sint32 sAdd = (0-m_stAtack[enShotN1].sRotation)/sRotSpd;
		if(gxAbs(sAdd/5) > 5 )
		{
			sAdd = sAdd/5;
		}
		else
		{
			if(sAdd == 0 )
			{
				if( m_stAtack[enShotN1].sRotation > 2 )
				{
					sAdd = -3;
				}else if( m_stAtack[enShotN1].sRotation < -2 )
				{
					sAdd = 3;
				}
				else
				{
					m_stAtack[enShotN1].sRotation = 0;
					m_bSwitchNow = gxFalse;
				}
			}
		}

		m_stAtack[enShotN1].sRotation += sAdd;
		m_pMech->SetWeaponRotaion( 0, m_stAtack[enShotN1].sRotation );
		m_pMech->SetMainWeaponRotation(  -180*m_stAtack[enShotN1].sRotation/65 );
	}

	switch( m_eMode ){
	case enHoundModeNormal:
		m_sGravitSpeed = enRakkaSpeed;
		ControlTypeNormal();
		break;

	case enHoundModeSpace:
		m_sGravitSpeed = 0;
		ControlTypeSpace();
		break;

	case enHoundModeGake:
		m_sGravitSpeed =  0;//enRakkaSpeedGake;
		ControlTypeGake();
		break;

	case enHoundModePodStart:
		//何もしない
		m_sGravitation = 0;
		m_pMech->SetDash( gxTrue );	//ダッシュモーション
		//シフト
		return;

	case enHoundModeNone:
		//動かさない
		m_sGravitation = 0;
		return;
	case enHoundModeCarreer:
		//キャリアモード
		m_sGravitation = 0;
		ControlCarry();
		return;
	case enHoundModeNPC:
		m_sGravitSpeed = enRakkaSpeed;
		ControlNPC();
		break;
	}

	Atack();

	GimmickSandStorm();
	GimmickGattling();

	if( m_bBoost || m_bForceBurnia )
	{
		GimmickBoost();
		Sint32 sAdd = (0-m_sBoosterRot)/5;
		if( sAdd == 0 ) sAdd = -1;
		if( m_sBoosterRot > 0 ) m_sBoosterRot += sAdd;
	}
	else
	{
		m_bBurnia = gxFalse;
		Sint32 sAdd = (100-m_sBoosterRot)/20;
		if( sAdd == 0 ) sAdd = 1;
		if( m_sBoosterRot < 100 ) m_sBoosterRot += sAdd;
	}

	m_pMech->SetBoosterRotation( m_sBoosterRot );

}


void CGunHound::GimmickGattling()
{
	//-----------------------------------------------------
	//ガトリングガン用回転エフェクト
	//-----------------------------------------------------
	Sint32 add;

	add = m_stAtack[enShotN1].sRotSpd/20;

	if( m_stAtack[enShotN1].sRotSpd > 0 && add == 0)
	{
		m_stAtack[enShotN1].sRotSpd -= 1;
	}
	else
	{
		m_stAtack[enShotN1].sRotSpd -= add;
	}

	m_stAtack[enShotN1].sRotCnt += m_stAtack[enShotN1].sRotSpd;
	m_stAtack[enShotN1].sRotCnt = m_stAtack[enShotN1].sRotCnt%300;

	Sint32 sSpr = m_stAtack[enShotN1].sRotCnt/100;

	if( m_stAtack[enShotN1].sType == enWeaponMachineGun )
	{
		m_pMech->SetPartsSpr( enPartsWeapon,	enPartsLayerMech , &stSprMainWeapon[sSpr] );
	}

}


void CGunHound::GimmickSandStorm()
{
	//-----------------------------------------------------
	//砂埃エフェクト
	//-----------------------------------------------------
	if( !m_bPlayer ) return;

	//水中でなければ出す
	if( pGame->GetMarineLine() )	return;

	if( m_sSandStormCnt > 0)
	{
		m_sSandStormCnt --;
		new CEffDashStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , m_Pos.y, 0 , 0 );
		new CEffDashStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , m_Pos.y, 0 , 0 );
		new CEffDashStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , m_Pos.y, 0 , 0 );
	}

}


void CGunHound::GimmickRollerEffect()
{
	//-----------------------------------------------------
	//ローラーダッシュエフェクト
	//-----------------------------------------------------
	if( !m_bPlayer ) return;

	//水中でなければ出す
	if( pGame->GetMarineLine() )	return;

	new CEffDashStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , m_Pos.y, 0 , 0 );

	viiEff::Set(EFF_FIREFLOWER , m_Pos.x , m_Pos.y , argset( viiSub::Rand()%360) );
	viiEff::Set(EFF_FIREFLOWER , m_Pos.x , m_Pos.y , argset( viiSub::Rand()%360) );
	viiEff::Set(EFF_FIREFLOWER , m_Pos.x , m_Pos.y , argset( viiSub::Rand()%360) );

}


void CGunHound::GimmickWaterEffect()
{
	//-----------------------------------------------------
	//川面
	//-----------------------------------------------------

	if( m_Old.y < pGame->GetMarineLine() )
	{
		//水に浸かった時
		if( m_Pos.y > pGame->GetMarineLine() )
		{
			viiMus::PlaySound( enSoundSplashS , m_bOutofScreen );
			new CEffWater( m_Pos.x , m_Pos.y , enTypeSplashLarge , 0);

			for(Sint32 ii=0;ii<6;ii++)
			{
				CEffWaterSpark *p;
				p = new CEffWaterSpark( m_Pos.x+(-8+viiSub::Rand()%16)*700 , m_Pos.y-400 ,-90+(-15+viiSub::Rand()%30) , (50+viiSub::Rand()%100) );
				p->SetType(enSparkTypeFog);
			}

		}
	}
	else if( m_Old.y > pGame->GetMarineLine() )
	{
		//水から上がったとき
		if( m_Pos.y < pGame->GetMarineLine() )
		{
			viiMus::PlaySound( enSoundSplashS , m_bOutofScreen );
			new CEffWater( m_Pos.x     , m_Pos.y, enTypeSplashPillar , 0);
			new CEffWater( m_Pos.x-800 , m_Pos.y, enTypeSplashPillar , 0);
			new CEffWater( m_Pos.x+800 , m_Pos.y, enTypeSplashPillar , 0);
			for(Sint32 ii=0;ii<5;ii++)
			{
				CEffWaterSpark *p;
				p = new CEffWaterSpark( m_Pos.x+(-8+viiSub::Rand()%16)*100 , m_Pos.y-2400 ,-90+(-15+viiSub::Rand()%30) , (50+viiSub::Rand()%100) );
				p->SetType(enSparkTypeFog);
			}
		}
	}

	if( m_Pos.y >= pGame->GetMarineLine() )
	{
		if( IsRollerDash() && viiSub::GameCounter()%4==0)
		{
			if( m_Add.x < 0)
			{
				new CEffWater( m_Pos.x , m_Pos.y-400 , enTypeSplashWave , 0, 1);
			}
			else
			{
				new CEffWater( m_Pos.x , m_Pos.y-400 , enTypeSplashWave , 0, -1);
			}

		}
		else
		{
			//水流
			new CEffWaterStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , pGame->GetMarineLine(), 180 , 180 );
			new CEffWaterStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , pGame->GetMarineLine(), 180 , 180 );
			new CEffWaterStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , pGame->GetMarineLine(), 180 , 180 );
		}
	}

}


void CGunHound::ControlTypeNormal()
{
	//-----------------------------------------------------
	//ノーマルモード
	//-----------------------------------------------------

	gxBool bArmRotable      = gxTrue;
	gxBool bBodyDirectional = gxTrue;
	gxBool bWalkable        = gxTrue;
	gxBool bJumpable        = gxTrue;

	if( m_sGravitation <= enRakkaMax )
	{
		m_sGravitation += m_sGravitSpeed;
		m_Add.y = 0; 
	}
	else
	{
		m_sGravitation = enRakkaMax;
		m_Add.y = 0; 
	}

	if( m_bChakuti )
	{
		m_sGravitation = 0;
	}

	if( m_sControlPush&BTN_L1 )
	{
		bArmRotable      = gxFalse;
		bBodyDirectional = gxFalse;
	}

	if( !m_bBurnia )
	{
		RecoverHoverEnergy();
	}

	if( bWalkable && m_sFreezeWait==0)
	{

		if( m_bChakuti )
		{
			m_pMech->SetJumpForm( gxFalse , 0 );
			ControlMoveNormal();		//移動可能であれば移動する
//

		}
		else
		{
			ControlMoveJump();
		}
	}
	else
	{
		SpeedDown();		//速度の減衰を行う
	}

	if( bArmRotable )
	{
		ControlArm();		//アームのコントロール
	}

	//--------------------------------------------

	m_pMech->SetArmRotation( m_sArmRotation );
	m_pMech->SetDirection( m_sDirection );

}


void CGunHound::ControlTypeSpace()
{
	//-----------------------------------------------------
	//スペースモード （自由移動：シューティングモード）
	//-----------------------------------------------------

	gxBool bArmRotable      = gxTrue;
	gxBool bBodyDirectional = gxTrue;
	gxBool bWalkable        = gxTrue;
	gxBool bJumpable        = gxTrue;

	if( m_sGravitation <= enRakkaMax )
	{
		m_sGravitation += m_sGravitSpeed;
	}

	if( m_bChakuti )
	{
		m_sGravitation = 0;
		m_Add.y = 0; 
	}

	if( m_sControlPush&BTN_L1 )
	{
		bArmRotable      = gxFalse;
		bBodyDirectional = gxFalse;
	}

	if( bWalkable )
	{
		ControlMoveSpace();	//移動可能であれば移動する
	}
	else
	{
		SpeedDown();		//速度の減衰を行う
	}

	if( bArmRotable )
	{
		ControlArm();		//アームのコントロール
	}

	//--------------------------------------------

	m_pMech->SetArmRotation( m_sArmRotation );
	m_pMech->SetDirection( m_sDirection );

}

#ifdef _VII_DEBUG_
#define ANMTEST
#endif

void CGunHound::ControlCarry()
{
	//-----------------------------------------------------
	//キャリアモード
	//-----------------------------------------------------
#ifdef ANMTEST
	static StMotionMan* pMotionTestBuf = NULL;
	StMotionMan stMotionKick[]={
		//キャリアモード
		{
			5,	//かがみこみ
			{
				{ enPartsBase,	0,0,0,	},
				{ enPartsBody,	-10,0,0,	},
				{ enPartsArmL,	30,0,0,	},
				{ enPartsArmR,	50,0,0,	},
				{ enPartsFootL,	0,0,0,	},
				{ enPartsFootR,	-30,10,0,},
				{ enPartsLegL,	20,0,0,	},
				{ enPartsLegR,	20,0,0,	},
			}
		},
		{
			600,	//かがみこみ
			{
				{ enPartsBase,	0,0,0,	},
				{ enPartsBody,	-10,0,0,	},
				{ enPartsArmL,	30,0,0,	},
				{ enPartsArmR,	50,0,0,	},
				{ enPartsFootL,	0,0,0,	},
				{ enPartsFootR,	-30,10,0,},
				{ enPartsLegL,	20,0,0,	},
				{ enPartsLegR,	20,0,0,	},
			}
		},
		{
			-1,
		}
	//		{ enPartsEnd, },
	};

	if( pMotionTestBuf == NULL )
	{
		pMotionTestBuf = new StMotionMan[128];
	}
	memcpy(pMotionTestBuf,stMotionKick,sizeof(StMotionMan)*32);

	if( m_sControlPush&BTN_L1 )
	{
		m_pMech->SetAnimation( pMotionTestBuf );
	}
#endif

	if( m_sTimer%20 == 0 )
	{
		m_pMech->SetAnimation( &stMotionCarreer[0] );
	}

}


void CGunHound::ControlNPC()
{
	//-----------------------------------------------------
	//NPCモード
	//-----------------------------------------------------

	m_sControlPush = 0;
	m_sControlTrig = 0; 

	Sint32 vx = pGame->GetHound()->m_Pos.x;
	Sint32 vy = pGame->GetHound()->m_Pos.y;

	if( m_sNPCLogicPattern == 0 )
	{
		//大戦艦：ライザ機追従型(時々場所を変える)
		m_Pos.z = PRIO_BG1 + 1;

		if(viiSub::GameCounter()%60==0 || viiSub::Rand()%90 == 0 )
		{
			m_NpcTgt.x = vx+(-30+viiSub::Rand()%60)*9000;
			m_NpcTgt.y = vy;
		}

		vx = m_NpcTgt.x;
		vy = m_NpcTgt.y;

		if( m_Pos.x < vx-100*100 ) m_sControlPush |= JOY_R;
		if( m_Pos.x > vx+100*100 ) m_sControlPush |= JOY_L;
	}

	if( m_sNPCLogicPattern == 1 )
	{
		//大戦艦：メインコアを攻撃
		//強制的にそこから「絶対に」動かさない
		m_Pos.z = PRIO_PLAYER-1;
		m_NpcTgt.x = 5900*100;
		if( m_sNPCLogicCnt%200 > 180 )
		{
			if( m_Pos.x < m_NpcTgt.x-300 )
			{
				m_sControlPush = BTN_L1|JOY_R;
			}
			else if( m_Pos.x > m_NpcTgt.x+300 )
			{
				m_sControlPush = BTN_L1|JOY_L;
			}
			else
			{
//				m_sControlPush |= BTN_X;
			}
		}
//		if( m_sNPCLogicCnt%200> 250)
		{
			//32ドット以上は絶対にずれさせない
			if( gxAbs(m_Pos.x - m_NpcTgt.x ) >= 3200 )
			{
				if( m_Pos.x < m_NpcTgt.x ) m_Pos.x = m_NpcTgt.x-32*100;
				if( m_Pos.x > m_NpcTgt.x ) m_Pos.x = m_NpcTgt.x+32*100;
			}
		}
		m_sRollerCnt = 0;
		m_sDirection = DIR_LEFT;
		m_sArmRotation += (0-m_sArmRotation)/5;

		//-----------------

		switch( m_sNPCLogicCnt%200 ){
		case 0:
			m_pMech->SetAnimation( &stMotion[0] );
			break;
		case 123:	//punch
		case 172:	//punch
		case 72:
			viiEff::SetBombSimple( m_Pos.x-32*100 , m_Pos.y-3200 , 1 , 1 );
		case 70:
		case 74:
			pGame->pBg->SetLayerFilter( enBgTypeRear , ARGB( 0xff ,0xff , 0xff , 0xff ) , ATR_ALPHA_PLUS );
			viiEff::Set(EFF_RING,m_Pos.x-32*100 , m_Pos.y-3200 , argset(250) );
			break;
		case 120:
		case 135:
		case 150:
		case 165:
			m_sControlPush = BTN_Y;
			m_sControlTrig = BTN_Y;
			break;
		}

	}

	if( m_sNPCLogicPattern == 2 )
	{
		//大戦艦：ニュートロンポリマー砲を攻撃
		m_Pos.z = PRIO_PLAYER-1;

		if( m_Pos.x <= 5540*100  ) m_sControlPush = JOY_R;//m_Pos.x += 10;
		if( m_Pos.x >= 5550*100  ) m_sControlPush = BTN_L1|JOY_L;//m_Pos.x -= 10;
		m_sHitPoint = 100*100;

		switch( m_sNPCLogicCnt%250 ){
		case 20:
		case 40:
		case 60:
		case 80:
		case 100:
		case 120:
		case 135:
		case 150:
		case 165:
			m_sControlPush = BTN_Y;
			m_sControlTrig = BTN_Y;
			break;
		}

		if( m_sNPCLogicCnt%250 > 220 )
		{
			m_sControlPush = JOY_R;//|BTN_X;
		}

	}

	if( m_sNPCLogicPattern == 3 )
	{
		//大戦艦：ニュートロンポリマー砲を攻撃
		m_Pos.z = PRIO_PLAYER-1;

		m_sControlPush = 0;
		m_sControlTrig = 0;

		m_sGravitSpeed = 0;
		if( m_sNPCLogicCnt < 10 )
		{
			m_NpcTgt.x = 0;
			m_NpcTgt.y = 300;

			m_bForceRollerDash = gxTrue;
			m_pObj->m_bAtariOff = gxTrue;
			m_pObj->mov.x = 0;
			m_pObj->mov.y = 0;
		}
		else
		{
			m_bForceBurnia  =gxTrue;
			if( m_NpcTgt.x < 800 ) m_NpcTgt.x += 12;
			if( m_NpcTgt.y > -100 ) m_NpcTgt.y -= 2;

			m_Pos.x += m_NpcTgt.x;
			m_Pos.y += m_NpcTgt.y;

			m_pObj->mov.x = 0;
			m_pObj->mov.y = 0;
		}
	}

	if( m_sNPCLogicPattern == 101 )
	{
		//グリフォン護衛
		Sint32 sDist = gxAbs( m_Pos.x - m_NpcTgt.x )/100;

		m_Pos.z = PRIO_BG2-1;

		if( m_Pos.x < m_NpcTgt.x-32*100 )
		{
			m_sControlPush = JOY_R;
			if( sDist >= 300 )
			{
				//離れすぎていたらダッシュする
				m_sControlPush |= BTN_B;
			}
		}
		else if( m_Pos.x > m_NpcTgt.x+32*100 )
		{
			m_sControlPush = JOY_L;
			if( sDist >= 300 )
			{
				//離れすぎていたらダッシュする
				m_sControlPush |= BTN_B;
			}
		}

	}


	if( m_sNPCLogicPattern == 110 )
	{
		//VSグラージ
		Sint32 sPos = m_NpcTgt.x - pGame->GetHound()->m_Pos.x;
		Sint32 sTgtX = m_NpcTgt.x + sPos;
		Sint32 sDist = gxAbs( m_Pos.x - sTgtX )/100;
		m_Pos.z = PRIO_BG2-1;

		if( m_Pos.x < sTgtX-8*100 )
		{
			m_sControlPush = BTN_L1|JOY_R;
		}
		else if( m_Pos.x > sTgtX+8*100 )
		{
			m_sControlPush = BTN_L1|JOY_L;
		}
		if( sDist >= 32 )
		{
			//離れすぎていたらダッシュする
			m_sControlPush |= BTN_B;
		}

		m_sArmRotation += (-45-m_sArmRotation)/10;

		if( m_Pos.x <m_NpcTgt.x )
		{
			m_sDirection = DIR_RIGHT;
		}
		else if( m_Pos.x > m_NpcTgt.x )
		{
			m_sDirection = DIR_LEFT;
		}
		if( Joy[0].psh&BTN_X)
		{
			m_sControlPush |= BTN_X;
		}
	}

	if( m_sNPCLogicPattern == 120 )
	{
		//VSグラージAfter
		Sint32 sDist = gxAbs( m_Pos.x - m_NpcTgt.x )/100;
		m_Pos.z = PRIO_BG2-1;
		if( m_Pos.x < m_NpcTgt.x-32*100 )
		{
			m_sControlPush = BTN_L1|JOY_R;
		}
		else if( m_Pos.x > m_NpcTgt.x+32*100 )
		{
			m_sControlPush = BTN_L1|JOY_L;
		}
//		if( sDist >= 300 )
		{
			//離れすぎていたらダッシュする
			m_sControlPush |= BTN_B;
		}

		m_sArmRotation += (0-m_sArmRotation)/10;

		if( m_Pos.x < m_NpcTgt.x-800 )
		{
			m_sDirection = DIR_RIGHT;
		}
		else if( m_Pos.x > m_NpcTgt.x+800 )
		{
			m_sDirection = DIR_LEFT;
		}
		if( Joy[0].psh&BTN_X)
		{
			m_sControlPush |= BTN_X;
		}
	}

	if( m_sNPCLogicPattern == 150 )
	{
		//ライザをホーミング（方向一緒）
		m_NpcTgt.x = pGame->GetHound()->m_Pos.x;
		Sint32 sDist = gxAbs( m_Pos.x - m_NpcTgt.x )/100;

		if( m_Pos.x < m_NpcTgt.x-64*100 )
		{
			m_sControlPush = BTN_L1|JOY_R;
		}
		else if( m_Pos.x > m_NpcTgt.x+64*100 )
		{
			m_sControlPush = BTN_L1|JOY_L;
		}

		if (sDist >= 120)
		{
			m_sControlPush |= BTN_B;
		}

		m_sArmRotation += (-25-m_sArmRotation)/10;

		m_sArmRotation += (pGame->GetHound()->m_sArmRotation-m_sArmRotation)/5;
		m_sDirection = pGame->GetHound()->m_sDirection;

		if( Joy[0].psh&BTN_X)
		{
			m_sControlPush |= BTN_X;
		}
	}

	if( m_sNPCLogicPattern == 200 )
	{
		//輸送船オープニング
		m_sDirection = DIR_RIGHT;

		if( m_Pos.x < m_NpcTgt.x+32*100 )
		{
			m_sControlPush = BTN_L1|JOY_R;
		}
	}

	if( m_sNPCLogicPattern == 210 )
	{
		//輸送船ディフェンス
		Sint32 sPos = m_NpcTgt.x - pGame->GetHound()->m_Pos.x;
		Sint32 sTgtX = m_NpcTgt.x + sPos;
		Sint32 sDist = gxAbs( m_Pos.x - sTgtX )/100;

		if( m_Pos.x < sTgtX-8*100 )
		{
			m_sControlPush = BTN_L1|JOY_R;
		}
		else if( m_Pos.x > sTgtX+8*100 )
		{
			m_sControlPush = BTN_L1|JOY_L;
		}
		if( sDist >= 32 )
		{
			//離れすぎていたらダッシュする
			m_sControlPush |= BTN_B;
		}

		m_sArmRotation += (-45-m_sArmRotation)/10;

		if( m_Pos.x <m_NpcTgt.x )
		{
			m_sDirection = DIR_RIGHT;
		}
		else if( m_Pos.x > m_NpcTgt.x )
		{
			m_sDirection = DIR_RIGHT;
		}
		if( Joy[0].psh&BTN_X)
		{
			m_sControlPush |= BTN_X;
		}
	}

	if( m_sNPCLogicPattern == 220 )
	{
		//輸送船移動

		if( m_Pos.x < m_NpcTgt.x )
		{
			m_sControlPush = BTN_L1|JOY_R;
		}
		else if( m_Pos.x > m_NpcTgt.x )
		{
			m_sControlPush = BTN_L1|JOY_L;
		}
		m_sControlPush |= BTN_B;

		m_sArmRotation += (-25-m_sArmRotation)/10;

		m_sDirection = DIR_RIGHT;
	}

	if( m_sNPCLogicPattern == 310 )
	{
		//高速移動
		m_Pos.z = PRIO_BG2-1;

		if( m_Pos.x < m_NpcTgt.x-300 )
		{
			m_sControlPush = BTN_L1|JOY_R;
		}
		else if( m_Pos.x > m_NpcTgt.x+300 )
		{
			m_sControlPush = BTN_L1|JOY_L;
		}

		Sint32 sDst = gxAbs(m_Pos.x - m_NpcTgt.x);

		if(sDst >= 100*100 )
		{
			m_sControlPush |= BTN_B;
		}
		else
		{
			sDst = gxAbs( pGame->GetHound()->m_Pos.x -m_Pos.x);
			if( sDst < WINDOW_W*100)
			{
				m_sArmRotation = pGame->GetHound()->m_sArmRotation;
				if( Joy[0].psh&BTN_X)
				{
					m_sControlPush |= BTN_X;
				}
			}
		}

		m_sArmRotation += (-25-m_sArmRotation)/10;

		m_sDirection = DIR_RIGHT;
	}

	if( m_sNPCLogicPattern == 320 )
	{
		//低速移動(村、山頂)
		//m_Pos.z = PRIO_BG2-1;

		if( m_Pos.x < m_NpcTgt.x-300 )
		{
			m_sControlPush = JOY_R;
		}
		else if( m_Pos.x > m_NpcTgt.x+300 )
		{
			m_sControlPush = BTN_L1|JOY_L;
		}
		else
		{
			Sint32 sDst = gxAbs( pGame->GetHound()->m_Pos.x -m_Pos.x);
			if( sDst < WINDOW_W*100)
			{
				m_sArmRotation = pGame->GetHound()->m_sArmRotation;
				if( Joy[0].psh&BTN_X)
				{
					m_sControlPush |= BTN_X;
				}
			}
		}

		m_sRollerCnt = 0;
		m_sDirection = DIR_RIGHT;
	}

	if( m_sNPCLogicPattern == 330 )
	{
		//バツエスケープ
		m_Pos.z = PRIO_BG2-1;

		if( m_Pos.x < m_NpcTgt.x-300 )
		{
			m_sControlPush = JOY_R;
		}
		else if( m_Pos.x > m_NpcTgt.x+300 )
		{
			m_sControlPush = BTN_L1|JOY_L;
		}

		m_sControlPush |= BTN_B;

		m_sArmRotation += (-25-m_sArmRotation)/10;
		m_sRollerCnt = 0;

		m_sDirection = DIR_RIGHT;
	}

	if( m_sNPCLogicPattern == 340 )
	{
		//山頂のクロスフォーメーション
		Sint32 sPos = m_NpcTgt.x - pGame->GetHound()->m_Pos.x;
		Sint32 sTgtX = m_NpcTgt.x + sPos;
		Sint32 sDist = gxAbs( m_Pos.x - sTgtX )/100;

		if( m_Pos.x < sTgtX-8*100 )
		{
			m_sControlPush = BTN_L1|JOY_R;
		}
		else if( m_Pos.x > sTgtX+8*100 )
		{
			m_sControlPush = BTN_L1|JOY_L;
		}
		if( sDist >= 32 )
		{
			//離れすぎていたらダッシュする
			m_sControlPush |= BTN_B;
		}

		m_sArmRotation = pGame->GetHound()->m_sArmRotation;
		m_sDirection = pGame->GetHound()->m_sDirection*-1;

		if( Joy[0].psh&BTN_X)
		{
			m_sControlPush |= BTN_X;
		}
	}

	if( m_sNPCLogicPattern == 350 )
	{
		//ライザをホーミング（方向逆）
		m_NpcTgt.x = pGame->GetHound()->m_Pos.x;
		Sint32 sDist = gxAbs( m_Pos.x - m_NpcTgt.x )/100;

		if( m_Pos.x < m_NpcTgt.x-64*100 )
		{
			m_sControlPush = BTN_L1|JOY_R;
		}
		else if( m_Pos.x > m_NpcTgt.x+64*100 )
		{
			m_sControlPush = BTN_L1|JOY_L;
		}

		if (sDist >= 120)
		{
			m_sControlPush |= BTN_B;
		}

		m_sArmRotation += (-25-m_sArmRotation)/10;

		m_sArmRotation += (pGame->GetHound()->m_sArmRotation-m_sArmRotation)/5;
		m_sDirection = pGame->GetHound()->m_sDirection*-1;

		if( Joy[0].psh&BTN_X)
		{
			m_sControlPush |= BTN_X;
		}
	}

	if( m_sNPCLogicPattern == 360 )
	{
		//ライザをホーミング（方向一緒）
		m_NpcTgt.x = pGame->GetHound()->m_Pos.x;
		Sint32 sDist = gxAbs( m_Pos.x - m_NpcTgt.x )/100;

		if( m_Pos.x < m_NpcTgt.x-64*100 )
		{
			m_sControlPush = BTN_L1|JOY_R;
		}
		else if( m_Pos.x > m_NpcTgt.x+64*100 )
		{
			m_sControlPush = BTN_L1|JOY_L;
		}

		if (sDist >= 120)
		{
			m_sControlPush |= BTN_B;
		}

		m_sArmRotation += (-25-m_sArmRotation)/10;

		m_sArmRotation += (pGame->GetHound()->m_sArmRotation-m_sArmRotation)/5;
		m_sDirection = pGame->GetHound()->m_sDirection;

		if( Joy[0].psh&BTN_X)
		{
			m_sControlPush |= BTN_X;
		}
	}

	if( m_sNPCLogicPattern == 450 )
	{
		//強制着地

		if( m_Pos.y >= m_NpcTgt.y )
		{
			m_bChakuti = gxTrue;
			m_Pos.y = m_NpcTgt.y;
		}
		else
		{
			CEffectBubble *p;
			Sint32 ax,ay;
			if( viiSub::Rand()%2==0)
			{
				ax = m_Pos.x;
				ay = m_Pos.y;

				ax += (-16+viiSub::Rand()%32)*100;
				ay += (-16+viiSub::Rand()%32)*100;

				p = new CEffectBubble(	ax , 	ay ,-90, 50+viiSub::Rand()%100 );
				p->SetWide( m_sTimer%360 , 90 );
				p->SetScale( 1.f );
				p->m_Pos.z = PRIO_PLAYER-1;
			}
		}

//		m_sDirection = pGame->GetHound()->m_sDirection;
		m_sArmRotation += (-45-m_sArmRotation)/5;
	}

	if( m_sNPCLogicPattern == 550 )
	{
		//低速移動(村、山頂)
		//m_Pos.z = PRIO_BG2-1;

		if( m_Pos.x < m_NpcTgt.x-300 )
		{
			m_sControlPush = BTN_L1|JOY_R;
		}
		else if( m_Pos.x > m_NpcTgt.x+300 )
		{
			m_sControlPush = BTN_L1|JOY_L;
		}
		else
		{
			m_sControlPush |= BTN_X;
		}

		m_sRollerCnt = 0;
		m_sDirection = DIR_LEFT;
		m_sArmRotation += (30-m_sArmRotation)/5;
	}
	m_sNPCLogicCnt ++;

	ControlTypeNormal();


}


void CGunHound::ControlTypeGake()
{
	//-----------------------------------------------------
	//崖くだりモード
	//-----------------------------------------------------

	gxBool bArmRotable      = gxTrue;
	gxBool bBodyDirectional = gxTrue;
	gxBool bWalkable        = gxTrue;
	gxBool bJumpable        = gxTrue;

	if( m_sGravitation <= enRakkaMax )
	{
		m_sGravitation += m_sGravitSpeed;
	}

	if( m_bChakuti )
	{
		m_sGravitation = 0;
		m_Add.y = 0; 
	}

	if( m_sControlPush&BTN_L1 )
	{
		bArmRotable      = gxFalse;
		bBodyDirectional = gxFalse;
	}

	if( bWalkable )
	{
		ControlMoveGake();		//移動可能であれば移動する
	}
	else
	{
		SpeedDown();		//速度の減衰を行う
	}

	if( bArmRotable )
	{
		ControlArm();		//アームのコントロール
	}

	//--------------------------------------------

	m_pMech->SetArmRotation( m_sArmRotation );
	m_pMech->SetDirection( m_sDirection );

}


void CGunHound::ControlAtack()
{
	//------------------------------------
	//攻撃のコントロール
	//------------------------------------

	if( m_sControlPush&BTN_X )	
	{
		if(m_bSubWeaponReady)
		{
			if( m_stAtack[enShotS1].sType == enWeaponNapalm )
			{
				ControlList( m_stAtack[enShotS1].sType , enShotS1);
			}
			else if( m_sControlTrig&BTN_X )
			{
//				AtackBazooka(enShotS1);
				ControlList( m_stAtack[enShotS1].sType , enShotS1);
			}
		}
		else
		{
			gxBool bShot = gxTrue;

			if( m_bAutoPunch )
			{
				//スゴッククローの時は至近距離なら自動でパンチを出す
				if( IsNeedPunch() )
				{
					if( m_stPunch.timelag == 0)
					{
						//m_pMech->SetAnimation( &stMotionPunch[0] );
						m_stPunch.timelag   = 1;//m_stPunch.timelag = enPunchTimeLag;
						m_stPunch.sAtackCnt = 1;
						bShot = gxFalse;
					}
				}
			}
			if( bShot )
			{
	//			AtackMachineGun(enShotN1);
				ControlList( m_stAtack[enShotN1].sType , enShotN1 );
			}
		}
	}

	if( m_sControlPush&BTN_Y)
	{
		if(m_bSubWeaponReady)
		{
//			AtackMissile(enShotS2);
			ControlList( m_stAtack[enShotS2].sType , enShotS2 );
		}
		else
		{
			if( m_stPunch.timelag == 0)
			{
				//m_pMech->SetAnimation( &stMotionPunch[0] );
				m_stPunch.timelag   = 1;//m_stPunch.timelag = enPunchTimeLag;
				m_stPunch.sAtackCnt = 1;
			}
		}
	}

}


void CGunHound::ControlDirection(Sint32 sDirection )
{
	//-------------------------------
	//体の方向を制御
	//-------------------------------

	if( m_stPunch.timelag ) return;
	if( m_sControlPush&BTN_L1 )
	{
		return;
	}
	if( m_sControlPush&BTN_X ) return;

	m_sDirection = sDirection;

}


void CGunHound::ControlArm()
{
	//-------------------------------
	//腕の動きの制御
	//-------------------------------

	Sint32 sTargetRot = -1;
	Sint32 sRotSpd    =  4;

	if( m_sControlPush&JOY_U && (m_sControlPush&JOY_L || m_sControlPush&JOY_R) )
	{
		sTargetRot = -45;
	}
	else if( m_sControlPush&JOY_D && (m_sControlPush&JOY_L || m_sControlPush&JOY_R) )
	{
		sTargetRot = 45;
	}
	else if( m_sControlPush&JOY_L || m_sControlPush&JOY_R )
	{
		sTargetRot = 0;
	}
	else  if( m_sControlPush&JOY_U )
	{
		sTargetRot = -90;
	}
	else if( m_sControlPush&JOY_D )
	{
		sTargetRot = 90;
	}

	if( sTargetRot != -1 )
	{
		if( m_sArmRotation > sTargetRot )
		{
			m_sArmRotation -= sRotSpd;
			if( m_sArmRotation < sTargetRot )
			{
				m_sArmRotation = sTargetRot;
			}
		}
		else if( m_sArmRotation < sTargetRot )
		{
			m_sArmRotation += sRotSpd;
			if( m_sArmRotation > sTargetRot )
			{
				m_sArmRotation = sTargetRot;
			}
		}
	}

}

void CGunHound::ControlMoveJump()
{
	//-----------------------------------------------------
	//移動制御
	//-----------------------------------------------------
	Sint32 sAddSpeed = enWalkJumpSpeed;
	Sint32 sMaxSpeed = enWalkMax;
	
	if( m_bInfinityHover )
	{
		sAddSpeed = enWalkJumpSpeed*3;
		sMaxSpeed = enWalkMax*3;
	}

	if( m_sGravitation < 0 )
	{
		m_pMech->SetJumpForm( gxTrue , 1 );
	}
	else
	{
		m_pMech->SetJumpForm( gxTrue , 2 );
	}
	//左右移動制御
	if( m_sControlPush&JOY_L )
	{
		if( m_Add.x > -sMaxSpeed ) m_Add.x -= sAddSpeed;
		ControlDirection( DIR_LEFT );
	}
	else if( m_sControlPush&JOY_R )
	{
		if( m_Add.x < sMaxSpeed ) m_Add.x += sAddSpeed;
		ControlDirection( DIR_RIGHT );
	}


	if( m_sJumpSeq == enJumpSeqJumpNone )
	{
		//オートジャンプ
		m_sJumpCnt = enWalkJumpCntMax;
		m_sGravitation = -enJumpSpeed/2;
		m_sJumpSeq = enJumpSeqHoveringReady;
		m_pMech->SetJumpForm( gxTrue , 2 );
	}
	else if( m_sJumpSeq == enJumpSeqJumpNow )
	{
		//初期ジャンプ
		gxBool bJumpOk = gxTrue;

//		m_sHoverEnergy = 0;
		m_sHoverSpd = 0;

		if( m_sJumpRate > 0 )
		{
			m_sJumpRate -= m_sJumpOmosi;
			if( m_sJumpRate <= 0 )
			{
				m_sJumpRate = 0;
			}
		}

//		m_sGravitation += -(enJumpSpeed*m_sJumpRate)/100;

		if( m_sJumpCnt >= enWalkJumpCntMax ) bJumpOk = gxFalse;
		if( m_sControlPush&BTN_A )
		{
			m_sGravitation += -(80*m_sJumpRate)/100;
		}
		else
		{
			bJumpOk = gxFalse;
		}
		if( m_sJumpCnt < enForceJumpCnt )    bJumpOk = gxTrue;

		if(bJumpOk)
		{
//			m_sGravitation = -(enJumpSpeed*m_sJumpRate)/100;
		}
		else
		{
			m_sJumpSeq = enJumpSeqHoveringReady;
		}

		m_sJumpCnt++;
	}
	else if( m_sJumpSeq == enJumpSeqHoveringReady )
	{
		if( m_sGravitation > 0 )
		{
			m_sJumpSeq = enJumpSeqHoveringNow;
		}
	}
	else if( m_sJumpSeq == enJumpSeqHoveringNow )
	{
		//ホバリング

		if( m_sHoverEnergy < enHoverEnergyMax && m_sControlPush&BTN_A && m_bHoverAble )
		{
			m_pMech->SetJumpForm( gxTrue , 1 );
			if( m_bInfinityHover )
			{
				m_sHoverEnergy += 0;
			}
			else
			{
				m_sHoverEnergy ++;
			}
			if( m_sHoverEnergy >= enHoverEnergyMax )
			{
				m_sHoverLag = enHoverLag;
			}

			if( m_sHoverSpd <= 100 )
			{
				GimmickApogyMotor( m_sControlPush );
				m_bBoost = gxTrue;

				m_sHoverSpd += enHoverSpeedAdd;
				if( m_sHoverSpd >= 100 )
				{
					m_sHoverSpd = 100;
				}
			}
			if( m_sGravitation > -280 )
			{
				m_sGravitation -= 20*m_sHoverSpd/100;
			}

			ghPos stPos;
			m_pMech->getPosition( enPartsNozzle , &stPos );
			viiEff::Set(EFF_BURNIA , stPos.x , stPos.y,argset( m_sDirection) );
		}
		else
		{
			m_sHoverSpd = 0;
		}
	}
	
}


void CGunHound::ControlMoveNormal()
{
	//-----------------------------------------------------
	//移動制御
	//-----------------------------------------------------
	Sint32 sSpeed = enWalkSpeed;

//	if( m_pObj->is_landnow() && 
	m_sJumpCnt = 0;
	sSpeed = enWalkSpeed;

	if( m_sControlTrig&BTN_A )
	{
		SetSandStorm(2);

		viiMus::PlaySound( enSoundJump , m_bOutofScreen );
		m_sJumpSeq = enJumpSeqJumpNow;
		m_sJumpRate = 80;
		m_sGravitation = -(enJumpSpeed*m_sJumpRate)/100;
		m_sJumpCnt = 0;

		m_bChakuti = gxFalse;

		return;
	}


	if( IsDashControl()  || m_bForceRollerDash )
	{
		m_sRollerCnt = 64;
	}

	if( m_bChakuti && IsRollerDash() )
	{
		//火花エフェクト
		GimmickRollerEffect();

		m_pMech->SetDash( gxTrue );
		if( m_sControlPush&JOY_L )
		{
			if( m_sTimer%16 ) m_pMech->SetBalance(-8);
			if( m_sTimer%16 )
			{
				m_pPenduram->Add( 40*m_sDirection );
			}
			if( m_Add.x > -enDashSpeedMax ) m_Add.x -= enDashAddSpeed;
		}
		else if( m_sControlPush&JOY_R )
		{
			if( m_sTimer%16 ) m_pMech->SetBalance(8);
			if( m_sTimer%16 )
			{
				m_pPenduram->Add( -40*m_sDirection );
			}
			if( m_Add.x < enDashSpeedMax ) m_Add.x += enDashAddSpeed;
		}
		else
		{
			SpeedDown();
		}
	}
	else if( m_sControlPush&JOY_L )
	{
		gxBool m_bWalkMotion = gxTrue;
		if( m_Add.x > -enWalkMax )
		{
			m_Add.x -= sSpeed;
			if(m_Add.x <= -enWalkMax)
			{
				m_Add.x = -enWalkMax;
			}
		}
		else if( m_Add.x < -enWalkMax )
		{
			m_bWalkMotion = gxFalse;
			m_sRollerCnt = 64;
			m_pMech->SetDash( gxTrue );
//			SpeedDown();
		}

		if( m_bWalkMotion )
		{
			if( m_sDirection == DIR_RIGHT )
			{
				m_pMech->SetWalk(-enWalkCycles);
			}
			else
			{
				m_pMech->SetWalk(enWalkCycles);
			}
		}

		if(m_Add.x < 0 )//|| !m_bChakuti )
		{
			ControlDirection( DIR_LEFT );
		}
	}
	else if( m_sControlPush&JOY_R )
	{
		gxBool m_bWalkMotion = gxTrue;
		if( m_Add.x < enWalkMax )
		{
			m_Add.x += sSpeed;
			if(m_Add.x >= enWalkMax)
			{
				m_Add.x = enWalkMax;
			}
		}
		else if( m_Add.x > enWalkMax )
		{
			m_bWalkMotion = gxFalse;
			m_sRollerCnt = 64;
			m_pMech->SetDash( gxTrue );
//			SpeedDown();
		}

		if( m_bWalkMotion )
		{
			if( m_sDirection == DIR_RIGHT )
			{
				m_pMech->SetWalk(enWalkCycles);
			}
			else
			{
				m_pMech->SetWalk(-enWalkCycles);
			}
		}

		if(m_Add.x > 0  )//|| !m_bChakuti )
		{
			ControlDirection( DIR_RIGHT );
		}
	}
	else
	{
		SpeedDown();
	}

}


void CGunHound::ControlMoveSpace()
{
	//-----------------------------------------------------
	//移動制御
	//-----------------------------------------------------
	Sint32 sSpeed    = enWalkSpeed*2;
	Sint32 sSpeedMax = enGakeWalkMax;
	Sint32 sKaku     = viiSub::GetPadInputAngle(m_sControlPush);

	m_sGravitation = 0;
	m_sJumpSeq = 0;

	if( sKaku != -1 )
	{
		if( sKaku > -45 && sKaku < 45 )
		{
			ControlDirection( DIR_RIGHT );
		}
		else if( sKaku > 180-45 && sKaku < 180+45 )
		{
			ControlDirection( DIR_LEFT );
		}
	}

	if( m_sControlPush&BTN_A )
	{
		sSpeedMax += 40;
		m_bBoost = gxTrue;
	}

	if( sKaku != -1 )
	{
		m_sPadRotation = sKaku;
		m_sPadSpeed    = sSpeedMax;
		m_Add.x = (viiMath::Cos100(m_sPadRotation)*m_sPadSpeed)/60;
		m_Add.y = (viiMath::Sin100(m_sPadRotation)*m_sPadSpeed)/60;
	}
	else
	{
		m_sPadSpeed += ( 0 - m_sPadSpeed ) / 5;
		m_Add.x = (viiMath::Cos100(m_sPadRotation)*m_sPadSpeed)/100;
		m_Add.y = (viiMath::Sin100(m_sPadRotation)*m_sPadSpeed)/100;

//		m_Add.y = enRakkaSpeedGake;

	}

	if(m_Add.x < 0 )
	{
		m_pMech->SetGakeForm( gxTrue , 2 );
	}
	else if(m_Add.x > 0 )
	{
		m_pMech->SetGakeForm( gxTrue , 1 );
	}

	if( !m_bChakuti )
	{
		GimmickApogyMotor( m_sControlPush );
	}


}


void CGunHound::ControlMoveGake()
{
	//-----------------------------------------------------
	//移動制御
	//-----------------------------------------------------
	Sint32 sSpeed = enWalkSpeed*2;
	Sint32 sSpeedMax = enGakeWalkMax;

	Sint32 sKaku = viiSub::GetPadInputAngle(m_sControlPush);

	m_sGravitation = 0;
	m_sJumpSeq = 0;
/*
	m_sJumpCnt = enWalkJumpCntMax;
	m_sGravitation = -enJumpSpeed/2;
	m_sJumpSeq = enJumpSeqHoveringReady;
*/

	ControlDirection( DIR_RIGHT );

	if( (m_sHoverEnergy < enHoverEnergyMax) && m_sControlPush&BTN_A )
	{
		m_sHoverEnergy ++;
		if( m_sHoverEnergy >= enHoverEnergyMax )
		{
			m_sHoverLag = enHoverLag;
		}
		sSpeedMax += 120;
		m_bBoost = gxTrue;
	}
	else
	{
		m_bBoost = gxFalse;
		if( pGame->pBg->get_kabetype(m_Pos.x/100 , m_Pos.y/100) )
		{
			//---------------------------------
			//ホバーエネルギー回復
			//---------------------------------
			RecoverHoverEnergy();
		}
	}

	if( sKaku != -1 )
	{
		m_sPadRotation = sKaku;
		m_sPadSpeed    = sSpeedMax;
		m_Add.x = (viiMath::Cos100(m_sPadRotation)*m_sPadSpeed)/100;
		m_Add.y = (viiMath::Sin100(m_sPadRotation)*m_sPadSpeed)/100;
	}
	else
	{
		m_sPadSpeed += ( 0 - m_sPadSpeed ) / 10;
		m_Add.x = (viiMath::Cos100(m_sPadRotation)*m_sPadSpeed)/100;
		m_Add.y = (viiMath::Sin100(m_sPadRotation)*m_sPadSpeed)/100;

		m_Add.y = enRakkaSpeedGake;
	}

	if( viiSub::Rand()%8 < 2 )
	{
//		if( viiSub::Rand()%4 < 2 )
//		{
//			m_Pos.y += 200;
//		}
//		else
		{
			m_Pos.y -= 200;
			if( viiSub::Rand()%5 == 0 ) m_pMech->SetShockAbsorber(200);
		}
	}

	if(m_Add.x < 0 )
	{
		m_pMech->SetGakeForm( gxTrue , 2 );
	}
	else if(m_Add.x > 0 )
	{
		m_pMech->SetGakeForm( gxTrue , 1 );
	}

	if( !m_bChakuti )
	{
		GimmickApogyMotor( m_sControlPush );
	}

}


void CGunHound::GimmickApogyMotor( Sint32 sControl )
{
	//-----------------------------------------------------
	//アポジモーターギミック
	//-----------------------------------------------------

	Sint32 sTenka = 0;
	Sint32 sJoyU,sJoyD,sJoyL,sJoyR;

	sJoyU = JOY_U;
	sJoyD = JOY_D;
	sJoyL = JOY_L;
	sJoyR = JOY_R;

	if( m_sDirection==DIR_LEFT )
	{
		//RU  UR
		//DL  LD
		sJoyU = JOY_R;
		sJoyR = JOY_U;
		sJoyD = JOY_L;
		sJoyL = JOY_D;
	}

	switch( sControl&(JOY_U|JOY_D|JOY_R|JOY_L) )
	{
	case JOY_U:
		sTenka = JOY_D|JOY_L;
		break;
	case JOY_U|JOY_R:
		sTenka = JOY_L;
		break;
	case JOY_R:
		sTenka = JOY_U|JOY_L;
		break;
	case JOY_R|JOY_D:
		sTenka = JOY_U;
		break;
	case JOY_D:
		sTenka = JOY_U|JOY_R;
		break;
	case JOY_D|JOY_L:
		sTenka = JOY_R;
		break;
	case JOY_L:
		sTenka = JOY_R|JOY_D;
		break;
	case JOY_L|JOY_U:
		sTenka = JOY_D;
		break;
	default:
		break;
	}

	if( sTenka )
	{
		ghPos stPos;
		Sint32 sDirection = 0;
		if(sTenka&sJoyU)
		{
			m_pMech->getPosition( enPartsApogyUL , &stPos );
			sDirection = 180+45;
			sDirection = (m_sDirection==DIR_RIGHT)? sDirection : (sDirection*-1+180);
			new CEffApogyMotor( stPos.x , stPos.y , PRIO_PLAYER+1 , sDirection , 50 );
		}
		if(sTenka&sJoyR)
		{
			m_pMech->getPosition( enPartsApogyUR , &stPos );
			sDirection = -45;
			sDirection = (m_sDirection==DIR_RIGHT)? sDirection : (sDirection*-1+180);
			new CEffApogyMotor( stPos.x , stPos.y , PRIO_PLAYER+1 , sDirection , 50 );
		}
		if(sTenka&sJoyD)
		{
			m_pMech->getPosition( enPartsApogyDR , &stPos );
			sDirection = 45;
			sDirection = (m_sDirection==DIR_RIGHT)? sDirection : (sDirection*-1+180);
			new CEffApogyMotor( stPos.x , stPos.y , PRIO_PLAYER+1 , sDirection , 50 );
		}
		if(sTenka&sJoyL)
		{
			m_pMech->getPosition( enPartsApogyDL , &stPos );
			sDirection = 135;
			sDirection = (m_sDirection==DIR_RIGHT)? sDirection : (sDirection*-1+180);
			new CEffApogyMotor( stPos.x , stPos.y , PRIO_PLAYER+1 , sDirection , 50 );
		}
	}

}


void CGunHound::GimmickLanding()
{
	//--------------------------------------------
	//着地エフェクト
	//--------------------------------------------

	m_pMech->SetJumpForm( gxFalse , 0 );
	SetSandStorm(4);
	m_pMech->ReSetWalk();
	m_pMech->SetShockAbsorber(200);
	m_pPenduram->Set( -20 );

}


void CGunHound::GimmickBoost()
{
	//--------------------------------------------
	//ブースターエフェクト
	//--------------------------------------------

	CEffectBurnia *p;
	Sint32 sPos = 800;

	if( m_bInfinityHover ) sPos = 1600;

	p = new CEffectBurnia( -sPos, -3200 , -30+viiSub::Rand()%60+155 ,120);
	p->SetParent( &pGame->GetHound()->m_Pos ,&m_sDirection );

	if(viiSub::Rand()%4==0)
	{
		p = new CEffectBurnia( m_Pos.x-sPos, m_Pos.y-3200, 90+45*m_sDirection ,180);

		if( m_sDirection ==  DIR_LEFT )
		{
			new CEffectGroundSmoke( m_Pos.x+viiSub::Rand()%1600 , m_Pos.y , DIR_RIGHT );
		}
		else
		{
			new CEffectGroundSmoke( m_Pos.x+viiSub::Rand()%1600 , m_Pos.y, DIR_LEFT );
		}

	}

	if( !m_bBurnia )
	{
		viiMus::PlaySound( enSoundBoost , m_bOutofScreen );
		m_bBurnia = gxTrue;
	}
}


void CGunHound::SetDamagePosition()
{
	//---------------------------------------------------------
	//ダメージの設定
	//---------------------------------------------------------
	gxBool bDamage = gxFalse;
	gxBool bBigDamage = gxFalse;
	gxBool bFuttobi = gxFalse;
	Sint32 sFuttobiX = 0;

	if( m_sMutekiTime > 0 )
	{
		m_sMutekiTime --;
	}
	else
	{

	}

	if( IsNoDamage() )
	{
		return;
	}

	Sint32 sShagamiY = 6*m_pMech->GetShockAbsorber()/100;

	for(Sint32 ii=0;ii<enDamageMax;ii++)
	{
		ghPos stPos;

		m_HitKurai[ii].ax1 = stBodyParts[ii].x1;
		m_HitKurai[ii].ay1 = stBodyParts[ii].y1+sShagamiY;
		m_HitKurai[ii].ax2 = stBodyParts[ii].x2;
		m_HitKurai[ii].ay2 = stBodyParts[ii].y2+sShagamiY;

		if( m_sDirection == DIR_LEFT )
		{
			m_HitKurai[ii].ax1 = -stBodyParts[ii].x2;
			m_HitKurai[ii].ay1 = stBodyParts[ii].y1+sShagamiY;
			m_HitKurai[ii].ax2 = -stBodyParts[ii].x1;
			m_HitKurai[ii].ay2 = stBodyParts[ii].y2+sShagamiY;
		}

		switch( ii ) {
		case enDamageBody:
			m_pMech->getPosition( enPartsBody , &stPos );
			break;

		case enDamageArmR:
			m_pMech->getPosition( enPartsArmR , &stPos );
			break;

		case enDamageLegL:
			m_pMech->getPosition( enPartsLegL , &stPos );
			break;

		case enDamageLegR:
			m_pMech->getPosition( enPartsLegR , &stPos );
			break;

		default:
			break;
		}

		stPos.x -= m_Pos.x;
		stPos.y -= m_Pos.y;

		stPos.x = stPos.x/100;
		stPos.y = stPos.y/100;

		m_HitKurai[ii].ax1 += stPos.x;
		m_HitKurai[ii].ay1 += stPos.y;
		m_HitKurai[ii].ax2 += stPos.x;
		m_HitKurai[ii].ay2 += stPos.y;;

		if( m_HitKurai[ii].ay2 >= m_Pos.y/100 )
		{
			m_HitKurai[ii].ay2 = m_Pos.y/100;
		}

		gxBool bDamageNow = gxFalse;

		if( m_HitKurai[ii].is_damage() )
		{
			if( m_HitKurai[ii].IsAtackType( ATK_TYPE_CANNON ) )
			{
				bFuttobi = gxTrue;
				sFuttobiX = ( m_HitKurai[ii].dx > 0 )? -3 : 3;
			}
			else
			{
				if( sFuttobiX == 0 )
				{
					sFuttobiX = ( m_HitKurai[ii].dx > 0 )? -1 : 1;
				}
			}
			bDamageNow = gxTrue;
			m_sHitEffectCnt[ii] = 60;
		}

		//-------------------------------------------
		//裸パーツのダメージ
		//-------------------------------------------
		ghPos stBreakPos;

		switch( ii ) {
		case enDamageBody:
			m_pMech->getPosition( enPartsBody , &stBreakPos );
			break;

		case enDamageArmR:
			m_pMech->getPosition( enPartsArmR , &stBreakPos );
			break;

		case enDamageLegL:
			m_pMech->getPosition( enPartsLegL , &stBreakPos );
			break;

		case enDamageLegR:
			m_pMech->getPosition( enPartsLegR , &stBreakPos );
			break;
		default:
			break;
		}

		if( m_sHitEffectCnt[ii] > 0 )
		{
			m_sHitEffectCnt[ii] --;

			if( bDamageNow )//m_sArmor[ii]== 0 )
			{
				if( m_sHitPoint > 0 )
				{
					if(m_sMutekiTime == 0)
					{
						if( m_bPlayer )
						{
							CCockpit::GetInstance()->SetHitStop(1);
						}
						if( bFuttobi )
						{
							//キャノンダメージの場合
							m_sHitPoint -= ((enDamagePoint*4)/(m_sArmor[ii]+1));
							bBigDamage = gxTrue;
						}
						else
						{
							m_sHitPoint -= ( (enDamagePoint*2) / (m_sArmor[ii]+1));
						}

						m_sMutekiTime = enMutekiTimeMax;
					}
					if(m_sHitPoint <= 0)
					{
						m_sHitPoint = 0;
#ifdef MUTEKI_ON
						//体験版では無敵
						m_sHitPoint = 1;//enHitPoint;
#else
						Crash();
#endif
					}
				}
			}

			if( m_bPlayer )
			{
				CAtkH0001MslSmoke *p = new CAtkH0001MslSmoke( stBreakPos.x , stBreakPos.y );
				p->SetDirection( (viiMath::Cos100( (m_sTimer*8)%360)*360)/100 , 60 );
			}
			else
			{
				if( viiSub::Rand()%16 == 0 )
				{
					CAtkH0001MslSmoke *p = new CAtkH0001MslSmoke( stBreakPos.x , stBreakPos.y );
					p->SetDirection( (viiMath::Cos100( (m_sTimer*8)%360)*360)/100 , 60 );
				}
			}
		}

		if( m_sHitEffectCnt[ii] || m_sArmor[ii] == enStatusArmorBone )
		{
			if( m_bLightWeightHound )
			{
				//ライト級ハウンドはパチパチさせない
			}
			else
			{
				if( viiSub::Rand()%8 == 0 )
				{
					new CEffBreakStorm( stBreakPos.x , stBreakPos.y, 0 , 0 );
					viiEff::Set(EFF_FIREFLOWER , stBreakPos.x , stBreakPos.y , argset( viiSub::Rand()%360) );
				}
			}
		}

		if( m_sArmor[ii] > enStatusArmorBone && m_HitKurai[ii].is_dead() )
		{
			//-------------------------------------------
			//パーツが破壊された
			//-------------------------------------------
			Sint32 ax,ay;

			if( m_sArmor[ii] > enStatusArmorBone ) m_sArmor[ii] --;

			ax = m_Pos.x + 100*(m_HitKurai[ii].ax1 + m_HitKurai[ii].ax2)/2;
			ay = m_Pos.y + 100*(m_HitKurai[ii].ay1 + m_HitKurai[ii].ay2)/2;

			ghPos stBreakPos;
			m_pMech->getPosition( enPartsMainShot , &stBreakPos );

			switch( ii ) {
			case enDamageBody:
				if( m_sArmor[ii] == enStatusArmorFrame )
				{
					m_pMech->SetPartsBroken( enPartsBody , enPartsLayerPrtc );
					m_HitKurai[ii].set_hp( HP_ARMORCLASS_C );
					if( m_bPlayer ) CCockpit::GetInstance()->SetShortMessage( StrShortMessage[0] );	//"WARNING!!BODY SHIELD IS BROKEN");
				}
				else
				{
					if( m_bPlayer ) CCockpit::GetInstance()->SetShortMessage( StrShortMessage[1] );//"ALERT!!BODY ARMOR IS BROKEN");
					if( m_bPlayer ) CCockpit::GetInstance()->GetRader()->SetCrash( enDamageBody );
					m_pMech->SetPartsBroken( enPartsBody , enPartsLayerBody );
				}
				break;

			case enDamageArmR:
				if( m_sArmor[ii] == enStatusArmorFrame )
				{
					m_pMech->SetPartsBroken( enPartsArmR     , enPartsLayerPrtc );
					m_pMech->SetPartsBroken( enPartsShoulder , enPartsLayerPrtc );
					m_HitKurai[ii].set_hp( HP_ARMORCLASS_C );
				}
				else
				{
					if( m_bPlayer ) CCockpit::GetInstance()->SetShortMessage( StrShortMessage[2] );//"ALERT!!ARM ARMOR IS BROKEN");
					if( m_bPlayer ) CCockpit::GetInstance()->GetRader()->SetCrash( enDamageArmR );
					m_pMech->SetPartsBroken( enPartsArmR     , enPartsLayerBody );
					m_pMech->SetPartsBroken( enPartsShoulder , enPartsLayerBody );
				}
				break;

			case enDamageLegL:
				if( m_sArmor[ii] == enStatusArmorFrame )
				{
					if( m_bPlayer ) CCockpit::GetInstance()->SetShortMessage( StrShortMessage[3] );//"WARNING!!LEG SHIELD IS BROKEN");
					m_pMech->SetPartsBroken( enPartsLegL , enPartsLayerPrtc );
					m_HitKurai[ii].set_hp( HP_ARMORCLASS_C );
				}
				else
				{
					if( m_bPlayer ) CCockpit::GetInstance()->SetShortMessage( StrShortMessage[4] );//"ALERT!!LEG ARMOR IS BROKEN");
					if( m_bPlayer ) CCockpit::GetInstance()->GetRader()->SetCrash( enDamageLegL );
					m_pMech->SetPartsBroken( enPartsLegL , enPartsLayerBody );
				}
				break;

			case enDamageLegR:
				if( m_sArmor[ii] == enStatusArmorFrame )
				{
					if( m_bPlayer ) CCockpit::GetInstance()->SetShortMessage( StrShortMessage[5] );//"WARNING!!LEG SHIELD IS BROKEN");
					m_pMech->SetPartsBroken( enPartsLegR , enPartsLayerPrtc );
					m_HitKurai[ii].set_hp( HP_ARMORCLASS_C );
				}
				else
				{
					if( m_bPlayer ) CCockpit::GetInstance()->SetShortMessage( StrShortMessage[6] );	//"ALERT!!LEG ARMOR IS BROKEN");
					if( m_bPlayer ) CCockpit::GetInstance()->GetRader()->SetCrash( enDamageLegR );
					m_pMech->SetPartsBroken( enPartsLegR , enPartsLayerBody );
				}
//				viiEff::Set(EFF_BOMB_SMALL,ax,ay,argset(0,50));
//				SoundPlay( enSoundHoundShieldDestroyed , gxFalse , 80 , 1 );	//毒
				break;

			default:
				break;
			}
		}

		if( bDamageNow )
		{
			//-------------------------------------------
			//ダメージを通達する
			//-------------------------------------------

			if( m_bPlayer )
			{
				CCockpit::GetInstance()->GetRader()->SetDamage( ii );
				CCockpit::GetInstance()->SetHitStop(1);
			}

			m_pMech->SetDamage( ii , m_sArmor[ii] );

			bDamage = gxTrue;
		}

		m_HitKurai[ii].SetHantei( ID_VERSIS_DEF , &m_Pos );
	}

	if( bDamage && !IsMuteki() )
	{
		//ダメージ用のフェード処理
		//（無敵の時はスルーする※演出用のホワイトフェードが解けるため）

		if( m_bPlayer )
		{
			if( bBigDamage )
			{
				CFadeManager::GetInstance()->set_fadein(4,0xA0F04020);
			}
			else
			{
				if(m_sMutekiTime )
				{
					//CFadeManager::GetInstance()->set_fadein(16,0xA0F04020);
				}
			}
		}
		if(sFuttobiX)
		{
			m_Add.x = sFuttobiX*100;
		}
	}

}

void CGunHound::Crash()
{
	//---------------------------------------------------
	//
	// GAME OVER処理
	//
	//---------------------------------------------------

	CEffectDmgprt *p;

	m_bDead = gxTrue;

	//ステージクリア時など無敵なら無効にする
	if( IsMuteki() ) return;
	if( !m_bPlayer ) return;

	if( g_StGameInfo.m_bDebugRoot )
	{
		return;
	}

	if( m_bGameOver ) return;

	m_bGameOver = gxTrue;

	if( m_bPlayer ) CCockpit::GetInstance()->SetDead();

	viiMus::PlaySound( enSoundExplosionLarge , gxFalse );
	EffectMakeBigBomb( m_Pos.x+(-16+viiSub::Rand()%32)*100 , m_Pos.y-2600+(-16+viiSub::Rand()%32)*100 );

	for(Sint32 ii=0;ii<enPartsWeapon;ii++)
	{
		m_pMech->SetPartsSpr( ii,	enPartsLayerMech , NULL );
		m_pMech->SetPartsSpr( ii,	enPartsLayerBody , NULL );
		m_pMech->SetPartsSpr( ii,	enPartsLayerPrtc , NULL );

		p = new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, 0-30*ii , 420 );
		p->SetSprite( (gxSprite*)&stSprMech[ii] );
	}

	m_pMech->SetPartsBroken( enPartsBody , enPartsLayerPrtc );
	m_pMech->SetPartsBroken( enPartsBody , enPartsLayerBody );
	m_pMech->SetPartsBroken( enPartsArmR     , enPartsLayerPrtc );
	m_pMech->SetPartsBroken( enPartsArmR     , enPartsLayerBody );
	m_pMech->SetPartsBroken( enPartsShoulder , enPartsLayerBody );
	m_pMech->SetPartsBroken( enPartsLegL , enPartsLayerPrtc );
	m_pMech->SetPartsBroken( enPartsLegL , enPartsLayerBody );
	m_pMech->SetPartsBroken( enPartsLegR , enPartsLayerPrtc );
	m_pMech->SetPartsBroken( enPartsLegR , enPartsLayerBody );


}


gxBool CGunHound::IsDashControl()
{
	gxBool bFlag = gxFalse;


	if( m_sControlPush&BTN_B )
	{
		return gxTrue;
	}

	Uint32 uDashCommand[]={
		CWazaCommand::enKeyLeverNone,
		CWazaCommand::enKeyRight,
		CWazaCommand::enKeyLeverNone,
		CWazaCommand::enKeyRight,
		CWazaCommand::enKeyEnd,
	};

	if( m_bCpuControl ) return gxFalse;

	bFlag = m_pWazaCommand->Check( uDashCommand , sizeof(uDashCommand)/sizeof(uDashCommand[0]) ,gxFalse );
	if( bFlag ) return gxTrue;

	bFlag = m_pWazaCommand->Check( uDashCommand , sizeof(uDashCommand)/sizeof(uDashCommand[0]) ,gxTrue );
	if( bFlag ) return gxTrue;

	return gxFalse;
}


gxBool CGunHound::AddHitPoint(Sint32 sPoint)
{
	//---------------------------------------------
	//ヒットポイントを回復
	//---------------------------------------------

	m_sHitPoint += sPoint*100;

	if( m_sHitPoint >= enHitPoint )
	{
		m_sHitPoint = enHitPoint;
	}

	return gxTrue;
}


void CGunHound::SetCarryOff()
{

	m_sGravitation = 0;
	m_Add.y = -100; 
	m_Add.x = 0; 

	SetMode( CGunHound::enHoundModeNormal );

	m_pMech->ResetAnime();
}

void CGunHound::RecoverHoverEnergy()
{
	//-------------------------------------
	//ホバーエネルギー回復
	//-------------------------------------
	if( m_sHoverLag > 0 )
	{
		m_bHoverAble = gxFalse;
		m_sHoverLag --;
		return;
	}

	if( m_sHoverEnergy > 0 )
	{
		Sint32 sAdd = (0-m_sHoverEnergy)/30;

		if(sAdd == 0)
		{
			sAdd = -1;
		}
		m_sHoverEnergy += sAdd;
	}
	else
	{
		m_bHoverAble = gxTrue;
	}
}


gxBool CGunHound::IsOutofScreen( Sint32 ofx , Sint32 ofy )
{
	//-------------------------------------
	// 画面外
	//-------------------------------------

	Sint32 sx,sy;
	Sint32 x , y;

	x = m_Pos.x / 100;
	y = m_Pos.y / 100;

	ofx = ofx/100;
	ofy = ofy/100;

	viiSub::GetScroll_s( sx,sy );

	if(y >= sy-ofy && y <= sy+WINDOW_H+ofy)
	{
		if(x >= sx-ofx && x <= sx+WINDOW_W+ofx)
		{
			return gxFalse;
		}
	}

	return gxTrue;

}

void CGunHound::SetGameOver()
{
	//-------------------------------------
	// ゲームオーバーをセット
	//-------------------------------------

	//ステージクリア時など無敵なら無効にする
	if( !m_bPlayer ) return;
	if( IsMuteki() ) return;

	//m_sHitPoint = 0;

	Crash();

	if( m_bPlayer ) CCockpit::GetInstance()->SetDead();

}


void CGunHound::SpeedDown()
{
	//------------------------------------
	//加速度の減衰
	//------------------------------------

	if( !m_bChakuti )
	{
		//空中であれば減衰させない
		return;
	}

	if( IsRollerDash() )
	{
		if( m_Add.x > 0 )
		{
			m_Add.x -= enWalkSpeed/4;
			if( m_Add.x < 0 ) m_Add.x = 0;
		}
		else if( m_Add.x < 0 )
		{
			m_Add.x += enWalkSpeed/4;
			if( m_Add.x > 0 ) m_Add.x = 0;
		}
	}
	else
	{
		if( m_Add.x > 0 )
		{
			if( m_Add.x > 200 )	viiEff::Set(EFF_DASH_SMOKE,m_Pos.x,m_Pos.y,argset( DIR_RIGHT ) );
			m_Add.x -= enWalkSpeed;
			if( m_Add.x < 0 ) m_Add.x = 0;
		}
		else if( m_Add.x < 0 )
		{
			if( m_Add.x < -200 )	viiEff::Set(EFF_DASH_SMOKE,m_Pos.x,m_Pos.y,argset( DIR_LEFT ) );
			m_Add.x += enWalkSpeed;
			if( m_Add.x > 0 ) m_Add.x = 0;
		}


	}

}

void CGunHound::SetLeader(gxBool bflag)
{
#ifdef _TRIAL_VERSION_
	bflag = gxFalse;
#endif

	m_bLeader = bflag;
	
	if( m_bLeader )
	{
		if( !m_bPlayer )
		{
			//リーダー機は白
			CCustomHound *pCCustomHound;
			pCCustomHound = new CCustomHound();
			pCCustomHound->SetBodyColor( m_pMech ,1,3);
			delete pCCustomHound;
		}
		m_pMech->SetPartsSpr( enPartsBurniaOption,	enPartsLayerMech , &stSprBurn[0] );
	}
	else
	{
		m_pMech->SetPartsSpr( enPartsBurniaOption,	enPartsLayerMech , NULL );
	}

}

void CGunHound::SetBooster(gxBool bflag)
{
	m_bBooster = bflag;

	if( m_bBooster )
	{
		m_pMech->SetPartsSpr( enPartsNozzle,	enPartsLayerMech,&stSprBurn[1] );
		m_pMech->SetPartsSpr( enPartsBurnia,	enPartsLayerMech,&stSprBurn[2] );
	}
	else
	{
		m_pMech->SetPartsSpr( enPartsNozzle,	enPartsLayerMech,NULL );
		m_pMech->SetPartsSpr( enPartsBurnia,	enPartsLayerMech,NULL );
	}

}

Sint32 CGunHound::GetHitPointStatus()
{
	Sint32 sAvr = 100 * m_sHitPoint / enHitPoint;

	if( sAvr < 30 )
	{
		return enHitPointStausAlert;
	}

	return enHitPointStausNormal;
}


gxBool CGunHound::IsNeedPunch( )
{
	//-------------------------------------
	//ロックオンロジック
	//最も近い敵の座標を得る
	//該当の敵がいない場合にはgxFalseを返す
	//-------------------------------------
	Sint32 x1,y1;
	Sint32 x = m_Pos.x/100;
	Sint32 y = m_Pos.y/100;

	Sint32 bx1,by1,bx2,by2;

	if( m_sDirection == DIR_LEFT )
	{
		bx1 = x - 80;
		bx2 = x -  0;
		by1 = y - 64;
		by2 = y +  16;
	}
	else
	{
		bx1 = x +  0;
		bx2 = x + 80;
		by1 = y - 64;
		by2 = y + 16;
	}

	//座標は1/100サイズ

	for(Sint32 i=0;i<pGame->pHitManager->GetOldMax();i++)
	{
		if( pGame->pHitManager->GetOldID(i) == ID_ENEMY_DEF || pGame->pHitManager->GetOldID(i) == ID_ENEMY_ATK )
		{
			x1 = pGame->pHitManager->GetOldX(i);
			y1 = pGame->pHitManager->GetOldY(i);

			if( viiSub::IsScrollOut(x1*100,y1*100,0,0) ) continue;

			if( ( ( x1 >= bx1 ) && (x1 <= bx2 ) ) && ( ( y1 >= by1 ) && (y1 <= by2 ) ) )
			{
				return gxTrue;
			}
		}
	}

	return gxFalse;	//見つからなかった

}


void CGunHound::Reset()
{

	m_Add.y = 0; 
	m_Add.x = 0; 

	m_sGravitation = 0;

//	m_sGravityMax   = 0;
//	m_sBaseRotation = 0;
	m_sArmRotation  = 0;
	m_bForceTobidasi = gxFalse;
	m_bForceShift    = gxFalse;
	m_bForceBurnia   = gxFalse;

	m_pMech->ResetAnime();
}



#if 0
void CGunHound::SetDefault()
{
	//-----------------------------------------
	//デフォルトのポジション
	//-----------------------------------------
	StSprPos g_StSprPos;
	StSprConfig m_Mecha,m_Body,m_Armor;

	//-----------------------------

	g_StSprPos.sIndex[enPartsBase].x =  0;
	g_StSprPos.sIndex[enPartsBase].y =  -23;

	g_StSprPos.sIndex[enPartsBody].x =  0;
	g_StSprPos.sIndex[enPartsBody].y =  0;

	g_StSprPos.sIndex[enPartsArmL].x =  -8;
	g_StSprPos.sIndex[enPartsArmL].y =  -6;

	g_StSprPos.sIndex[enPartsArmR].x =  0;
	g_StSprPos.sIndex[enPartsArmR].y =  -12;

	g_StSprPos.sIndex[enPartsLegL].x =  0;
	g_StSprPos.sIndex[enPartsLegL].y =  5;

	g_StSprPos.sIndex[enPartsLegR].x =  0;
	g_StSprPos.sIndex[enPartsLegR].y =  5;

	g_StSprPos.sIndex[enPartsShoulder].x = -6;
	g_StSprPos.sIndex[enPartsShoulder].y = -4;

	g_StSprPos.sIndex[enPartsWeapon].x = 8;
	g_StSprPos.sIndex[enPartsWeapon].y = 8;

	g_StSprPos.sIndex[enPartsMainShot].x = 36;
	g_StSprPos.sIndex[enPartsMainShot].y = -6;

	g_StSprPos.sIndex[enPartsShot1].x =   -8;
	g_StSprPos.sIndex[enPartsShot1].y =   -16;

	g_StSprPos.sIndex[enPartsBurnia].x = 	-16;
	g_StSprPos.sIndex[enPartsBurnia].y = 	-16;

	g_StSprPos.sIndex[enPartsBurniaOption].x = 24;
	g_StSprPos.sIndex[enPartsBurniaOption].y = -4;

	g_StSprPos.sIndex[enPartsNozzle].x = 	0;
	g_StSprPos.sIndex[enPartsNozzle].y = 	0;

	g_StSprPos.sIndex[enPartsSubWeapon1].x = 12;
	g_StSprPos.sIndex[enPartsSubWeapon1].y = -4;

	g_StSprPos.sIndex[enPartsSubWeapon2].x = 16;
	g_StSprPos.sIndex[enPartsSubWeapon2].y = 0;

	g_StSprPos.sIndex[enPartsSubWeapon3].x = 16;
	g_StSprPos.sIndex[enPartsSubWeapon3].y = 0;

	g_StSprPos.sIndex[enPartsApogyUL].x =   -8;
	g_StSprPos.sIndex[enPartsApogyUL].y =   -8;

	g_StSprPos.sIndex[enPartsApogyUR].x =   8;
	g_StSprPos.sIndex[enPartsApogyUR].y =   -8;

	g_StSprPos.sIndex[enPartsApogyDL].x =   -8;
	g_StSprPos.sIndex[enPartsApogyDL].y =   8;

	g_StSprPos.sIndex[enPartsApogyDR].x =   8;
	g_StSprPos.sIndex[enPartsApogyDR].y =   8;

	SetPosition( &g_StSprPos );

	//-----------------------------

	//メカ設定
	m_Mecha.sIndex[enPartsBase]     = -1;
	m_Mecha.sIndex[enPartsBody]     =  1;
	m_Mecha.sIndex[enPartsArmL]     =  0;
	m_Mecha.sIndex[enPartsArmR]     =  0;
	m_Mecha.sIndex[enPartsLegL]     =  3;
	m_Mecha.sIndex[enPartsLegR]     =  2;
	m_Mecha.sIndex[enPartsFootL]    =  7;
	m_Mecha.sIndex[enPartsFootR]    =  6;
	m_Mecha.sIndex[enPartsShoulder] =  4;
	m_Mecha.sIndex[enPartsWeapon]   =  0;
	m_Mecha.sIndex[enPartsMainShot] = -1;
	m_Mecha.sIndex[enPartsShot1]    = -1;

	//ボディ設定
	m_Body.sIndex[enPartsBase]     = -1;
	m_Body.sIndex[enPartsBody]     =  1;
	m_Body.sIndex[enPartsArmL]     = -1;
	m_Body.sIndex[enPartsArmR]     =  0;
	m_Body.sIndex[enPartsLegL]     =  3;
	m_Body.sIndex[enPartsLegR]     =  2;
	m_Body.sIndex[enPartsFootL]    = -1;
	m_Body.sIndex[enPartsFootR]    = -1;
	m_Body.sIndex[enPartsShoulder] =  4;

	//アーマー設定
	m_Armor.sIndex[ enPartsBase ]     = -1;
	m_Armor.sIndex[ enPartsBody ]     =  0;
	m_Armor.sIndex[ enPartsArmL ]     = -1;
	m_Armor.sIndex[ enPartsArmR ]     =  0;
	m_Armor.sIndex[ enPartsLegL ]     =  1;
	m_Armor.sIndex[ enPartsLegR ]     =  2;
	m_Armor.sIndex[ enPartsFootL ]    = -1;
	m_Armor.sIndex[ enPartsFootR ]    = -1;
	m_Armor.sIndex[ enPartsShoulder ] =  0;

	//オプションパーツ
	m_Armor.sIndex[ enPartsBurnia      ] = -1;
	m_Armor.sIndex[ enPartsNozzle      ] = -1;
	m_Armor.sIndex[ enPartsBurniaOption] = -1;
	m_Armor.sIndex[ enPartsSubWeapon1  ] =  0;
	m_Armor.sIndex[ enPartsSubWeapon2  ] = -1;
	m_Armor.sIndex[ enPartsSubWeapon3  ] = -1;

	SetSprite( &m_Mecha , &m_Body , &m_Armor );


}


void CGunHound::SetPosition( StSprPos *pPos )
{
	//位置設定

/*
	m_pMech->SetPartsPos( enPartsBase, 	0 , -23 );
	m_pMech->SetPartsPos( enPartsBody, 	0 , 0 );
	m_pMech->SetPartsPos( enPartsArmL, 	-8 , -6 );
	m_pMech->SetPartsPos( enPartsArmR, 	0 , -12 );
	m_pMech->SetPartsPos( enPartsLegL, 	-0 , 5 );
	m_pMech->SetPartsPos( enPartsLegR, 	 0 , 5 );
	m_pMech->SetPartsPos( enPartsShoulder, -6 , -4 );
	m_pMech->SetPartsPos( enPartsWeapon,	8 , 8 );

	m_pMech->SetPartsPos( enPartsMainShot,	36 , -6 );
	m_pMech->SetPartsPos( enPartsShot1   ,	-8 , -16 );


	m_pMech->SetPartsPos( enPartsBurnia 		, -16 , -16 );
	m_pMech->SetPartsPos( enPartsBurniaOption , 24 , -4 );
	m_pMech->SetPartsPos( enPartsNozzle 		,  0 , 0 );
	m_pMech->SetPartsPos( enPartsSubWeapon1 	,  12 , -4 );
	m_pMech->SetPartsPos( enPartsSubWeapon2 	,  16 , 0 );
	m_pMech->SetPartsPos( enPartsSubWeapon3 	,  16 , 0 );

	m_pMech->SetPartsPos( enPartsApogyUL   ,	-8 , -8 );
	m_pMech->SetPartsPos( enPartsApogyUR   ,	 8 , -8 );
	m_pMech->SetPartsPos( enPartsApogyDL   ,	-8 , 8 );
	m_pMech->SetPartsPos( enPartsApogyDR   ,	 8 , 8 );
*/

	m_pMech->SetPartsPos( enPartsBase			, 	pPos->sIndex[enPartsBase].x			,pPos->sIndex[enPartsBase].y	 );
	m_pMech->SetPartsPos( enPartsBody			, 	pPos->sIndex[enPartsBody].x			,pPos->sIndex[enPartsBody].y	 );
	m_pMech->SetPartsPos( enPartsArmL			, 	pPos->sIndex[enPartsArmL].x			,pPos->sIndex[enPartsArmL].y	 );
	m_pMech->SetPartsPos( enPartsArmR			, 	pPos->sIndex[enPartsArmR].x			,pPos->sIndex[enPartsArmR].y	 );
	m_pMech->SetPartsPos( enPartsLegL			, 	pPos->sIndex[enPartsLegL].x			,pPos->sIndex[enPartsLegL].y	 );
	m_pMech->SetPartsPos( enPartsLegR			, 	pPos->sIndex[enPartsLegR].x			,pPos->sIndex[enPartsLegR].y	 );
	m_pMech->SetPartsPos( enPartsShoulder		, 	pPos->sIndex[enPartsShoulder].x		,pPos->sIndex[enPartsShoulder].y );
	m_pMech->SetPartsPos( enPartsWeapon			, 	pPos->sIndex[enPartsWeapon].x		,pPos->sIndex[enPartsWeapon].y	 );

	m_pMech->SetPartsPos( enPartsMainShot		, 	pPos->sIndex[enPartsMainShot].x		,pPos->sIndex[enPartsMainShot].y );
	m_pMech->SetPartsPos( enPartsShot1   		, 	pPos->sIndex[enPartsShot1].x		,pPos->sIndex[enPartsShot1].y	 );

	m_pMech->SetPartsPos( enPartsBurnia 		, 	pPos->sIndex[enPartsBurnia].x		,pPos->sIndex[enPartsBurnia].y	 );
	m_pMech->SetPartsPos( enPartsBurniaOption   , 	pPos->sIndex[enPartsBurniaOption].x	,pPos->sIndex[enPartsBurniaOption].y );
	m_pMech->SetPartsPos( enPartsNozzle 		, 	pPos->sIndex[enPartsNozzle].x		,pPos->sIndex[enPartsNozzle].y	 );
	m_pMech->SetPartsPos( enPartsSubWeapon1 	, 	pPos->sIndex[enPartsSubWeapon1].x	,pPos->sIndex[enPartsSubWeapon1].y );
	m_pMech->SetPartsPos( enPartsSubWeapon2 	, 	pPos->sIndex[enPartsSubWeapon2].x	,pPos->sIndex[enPartsSubWeapon2].y );
	m_pMech->SetPartsPos( enPartsSubWeapon3 	, 	pPos->sIndex[enPartsSubWeapon3].x	,pPos->sIndex[enPartsSubWeapon3].y );

	m_pMech->SetPartsPos( enPartsApogyUL   		, 	pPos->sIndex[enPartsApogyUL].x		,pPos->sIndex[enPartsApogyUL].y );
	m_pMech->SetPartsPos( enPartsApogyUR   		, 	pPos->sIndex[enPartsApogyUR].x		,pPos->sIndex[enPartsApogyUR].y );
	m_pMech->SetPartsPos( enPartsApogyDL   		, 	pPos->sIndex[enPartsApogyDL].x		,pPos->sIndex[enPartsApogyDL].y );
	m_pMech->SetPartsPos( enPartsApogyDR   		, 	pPos->sIndex[enPartsApogyDR].x		,pPos->sIndex[enPartsApogyDR].y );

}


void CGunHound::SetSprite( StSprConfig* pMecha , StSprConfig* pBody , StSprConfig* pArmor )
{
	//----------------------------------------------------------------------
	//スプライト設定
	//----------------------------------------------------------------------

	if( pMecha )
	{
		//スプライト1設定(メカ)
//		m_pMech->SetPartsSpr( enPartsBase, 		enPartsLayerMech,NULL );
//		m_pMech->SetPartsSpr( enPartsBody, 		enPartsLayerMech,&stSprMech[1] );
//		m_pMech->SetPartsSpr( enPartsArmL, 		enPartsLayerMech,&stSprPunch[0] );//NULL );	//&stSprMech[5] );
//		m_pMech->SetPartsSpr( enPartsArmR, 		enPartsLayerMech,&stSprMech[0] );
//		m_pMech->SetPartsSpr( enPartsLegL, 		enPartsLayerMech,&stSprMech[3] );
//		m_pMech->SetPartsSpr( enPartsLegR, 		enPartsLayerMech,&stSprMech[2] );
//		m_pMech->SetPartsSpr( enPartsFootL, 	enPartsLayerMech,&stSprMech[7] );
//		m_pMech->SetPartsSpr( enPartsFootR, 	enPartsLayerMech,&stSprMech[6] );
//		m_pMech->SetPartsSpr( enPartsShoulder,	enPartsLayerMech,&stSprMech[4] );
//		m_pMech->SetPartsSpr( enPartsWeapon,	enPartsLayerMech,&stSprMainWeapon[0] );
//
//		m_pMech->SetPartsSpr( enPartsMainShot,  enPartsLayerMech , NULL );//&stSprShot[0] );
//		m_pMech->SetPartsSpr( enPartsShot1,     enPartsLayerMech , NULL);//&stSprShot[0] );
//
//	//	m_pMech->SetPartsSpr( enPartsApogyUL,        enPartsLayerMech , &stSprShot[0] );
//	//	m_pMech->SetPartsSpr( enPartsApogyUR,        enPartsLayerMech , &stSprBody[0] );
//	//	m_pMech->SetPartsSpr( enPartsApogyDL,        enPartsLayerMech , &stSprShot[0] );
//	//	m_pMech->SetPartsSpr( enPartsApogyDR,        enPartsLayerMech , &stSprBody[0] );

		if( pMecha->sIndex[enPartsBase]     != -1)  m_pMech->SetPartsSpr( enPartsBase, 		enPartsLayerMech,NULL );	//pMecha->sIndex[enPartsBase]
		if( pMecha->sIndex[enPartsBody]     != -1)  m_pMech->SetPartsSpr( enPartsBody, 		enPartsLayerMech,&stSprMech[ pMecha->sIndex[enPartsBody] ] );
		if( pMecha->sIndex[enPartsArmL]     != -1)  m_pMech->SetPartsSpr( enPartsArmL, 		enPartsLayerMech,&stSprPunch[ pMecha->sIndex[enPartsArmL]] );
		if( pMecha->sIndex[enPartsArmR]     != -1)  m_pMech->SetPartsSpr( enPartsArmR, 		enPartsLayerMech,&stSprMech[ pMecha->sIndex[enPartsArmR] ] );
		if( pMecha->sIndex[enPartsLegL]     != -1)  m_pMech->SetPartsSpr( enPartsLegL, 		enPartsLayerMech,&stSprMech[ pMecha->sIndex[enPartsLegL] ] );
		if( pMecha->sIndex[enPartsLegR]     != -1)  m_pMech->SetPartsSpr( enPartsLegR, 		enPartsLayerMech,&stSprMech[ pMecha->sIndex[enPartsLegR] ] );
		if( pMecha->sIndex[enPartsFootL]    != -1)  m_pMech->SetPartsSpr( enPartsFootL, 	enPartsLayerMech,&stSprMech[ pMecha->sIndex[enPartsFootL]] );
		if( pMecha->sIndex[enPartsFootR]    != -1)  m_pMech->SetPartsSpr( enPartsFootR, 	enPartsLayerMech,&stSprMech[ pMecha->sIndex[enPartsFootR]] );
		if( pMecha->sIndex[enPartsShoulder] != -1)  m_pMech->SetPartsSpr( enPartsShoulder,	enPartsLayerMech,&stSprMech[ pMecha->sIndex[enPartsShoulder]] );
		if( pMecha->sIndex[enPartsWeapon]   != -1)  m_pMech->SetPartsSpr( enPartsWeapon,	enPartsLayerMech,&stSprMainWeapon[ pMecha->sIndex[enPartsWeapon] ] );
		if( pMecha->sIndex[enPartsMainShot] != -1)  m_pMech->SetPartsSpr( enPartsMainShot,  enPartsLayerMech , NULL );	//pMecha->sIndex[enPartsMainShot]		
		if( pMecha->sIndex[enPartsShot1]    != -1)  m_pMech->SetPartsSpr( enPartsShot1,     enPartsLayerMech , NULL);	//pMecha->sIndex[enPartsShot1]		
	}

	if( pBody )
	{
//		//スプライト2設定(フレーム)
//		m_pMech->SetPartsSpr( enPartsBase, 		enPartsLayerBody,NULL );
//		m_pMech->SetPartsSpr( enPartsBody, 		enPartsLayerBody,&stSprBody[1] );
//		m_pMech->SetPartsSpr( enPartsArmL, 		enPartsLayerBody,NULL );
//		m_pMech->SetPartsSpr( enPartsArmR, 		enPartsLayerBody,&stSprBody[0] );
//		m_pMech->SetPartsSpr( enPartsLegL, 		enPartsLayerBody,&stSprBody[3] );
//		m_pMech->SetPartsSpr( enPartsLegR, 		enPartsLayerBody,&stSprBody[2] );
//		m_pMech->SetPartsSpr( enPartsFootL, 	enPartsLayerBody,NULL);//&stSprBody[7] );
//		m_pMech->SetPartsSpr( enPartsFootR, 	enPartsLayerBody,NULL);//&stSprBody[6] );
//		m_pMech->SetPartsSpr( enPartsShoulder,	enPartsLayerBody,&stSprBody[4] );
//	//	m_pMech->SetPartsSpr( enPartsWeapon,	enPartsLayerBody,NULL);//&stSprBody[1] );

		if( pBody->sIndex[enPartsBase]     != -1)  m_pMech->SetPartsSpr( enPartsBase, 	enPartsLayerBody ,NULL );	//pMecha->sIndex[enPartsBase]
		if( pBody->sIndex[enPartsBody]     != -1)  m_pMech->SetPartsSpr( enPartsBody, 	enPartsLayerBody ,&stSprBody [ pBody->sIndex[enPartsBody] ] );
		if( pBody->sIndex[enPartsArmL]     != -1)  m_pMech->SetPartsSpr( enPartsArmL, 	enPartsLayerBody ,NULL );
		if( pBody->sIndex[enPartsArmR]     != -1)  m_pMech->SetPartsSpr( enPartsArmR, 	enPartsLayerBody ,&stSprBody [ pBody->sIndex[enPartsArmR] ] );
		if( pBody->sIndex[enPartsLegL]     != -1)  m_pMech->SetPartsSpr( enPartsLegL, 	enPartsLayerBody ,&stSprBody [ pBody->sIndex[enPartsLegL] ] );
		if( pBody->sIndex[enPartsLegR]     != -1)  m_pMech->SetPartsSpr( enPartsLegR, 	enPartsLayerBody ,&stSprBody [ pBody->sIndex[enPartsLegR] ] );
		if( pBody->sIndex[enPartsFootL]    != -1)  m_pMech->SetPartsSpr( enPartsFootL, 	enPartsLayerBody ,NULL );
		if( pBody->sIndex[enPartsFootR]    != -1)  m_pMech->SetPartsSpr( enPartsFootR, 	enPartsLayerBody ,NULL );
		if( pBody->sIndex[enPartsShoulder] != -1)  m_pMech->SetPartsSpr( enPartsShoulder,	enPartsLayerBody ,&stSprBody [ pBody->sIndex[enPartsShoulder]] );
	}

	if( pArmor )
	{
//		//スプライト3設定(アーマー)
//		m_pMech->SetPartsSpr( enPartsBase, 		enPartsLayerPrtc,NULL );
//		m_pMech->SetPartsSpr( enPartsBody, 		enPartsLayerPrtc,&stSprArmorBody[0] );
//		m_pMech->SetPartsSpr( enPartsArmL, 		enPartsLayerPrtc,NULL );
//		m_pMech->SetPartsSpr( enPartsArmR, 		enPartsLayerPrtc,&stSprArmorArm[0] );
//		m_pMech->SetPartsSpr( enPartsLegL, 		enPartsLayerPrtc,&stSprArmorLeg2[0] );
//		m_pMech->SetPartsSpr( enPartsLegR, 		enPartsLayerPrtc,&stSprArmorLeg1[0] );
//		m_pMech->SetPartsSpr( enPartsFootL, 	enPartsLayerPrtc,NULL);
//		m_pMech->SetPartsSpr( enPartsFootR, 	enPartsLayerPrtc,NULL);
//		m_pMech->SetPartsSpr( enPartsShoulder,	enPartsLayerPrtc,&stSprArmorShoulder[0]  );
//
//		//オプションパーツ
//		m_pMech->SetPartsSpr( enPartsBurnia,		enPartsLayerMech,NULL );//&stSprMech[8] );
//		m_pMech->SetPartsSpr( enPartsNozzle,		enPartsLayerMech,NULL );//&stSprMech[9] );
//		m_pMech->SetPartsSpr( enPartsBurniaOption,	enPartsLayerMech,NULL );//&stSprMech[10] );
//		m_pMech->SetPartsSpr( enPartsSubWeapon1,	enPartsLayerMech,&stSprSubWeapon[0] );
//		m_pMech->SetPartsSpr( enPartsSubWeapon2,	enPartsLayerMech,NULL );//&stSprMech[12] );
//		m_pMech->SetPartsSpr( enPartsSubWeapon3,	enPartsLayerMech,NULL );//&stSprMech[13] );

		//スプライト3設定(アーマー)
		if( pArmor->sIndex[enPartsBase] != -1 )     m_pMech->SetPartsSpr( enPartsBase, 		enPartsLayerPrtc,NULL );
		if( pArmor->sIndex[enPartsBody] != -1 )     m_pMech->SetPartsSpr( enPartsBody, 		enPartsLayerPrtc,&stSprArmorBody[ pArmor->sIndex[enPartsBody] ] );
		if( pArmor->sIndex[enPartsArmL] != -1 )     m_pMech->SetPartsSpr( enPartsArmL, 		enPartsLayerPrtc,NULL );
		if( pArmor->sIndex[enPartsArmR] != -1 )     m_pMech->SetPartsSpr( enPartsArmR, 		enPartsLayerPrtc,&stSprArmorArm [ pArmor->sIndex[enPartsArmR] ] );
		if( pArmor->sIndex[enPartsLegL] != -1 )     m_pMech->SetPartsSpr( enPartsLegL, 		enPartsLayerPrtc,&stSprArmorLeg2[ pArmor->sIndex[enPartsLegL] ] );
		if( pArmor->sIndex[enPartsLegR] != -1 )     m_pMech->SetPartsSpr( enPartsLegR, 		enPartsLayerPrtc,&stSprArmorLeg1[ pArmor->sIndex[enPartsLegR] ] );
		if( pArmor->sIndex[enPartsFootL] != -1 )    m_pMech->SetPartsSpr( enPartsFootL, 	enPartsLayerPrtc,NULL);
		if( pArmor->sIndex[enPartsFootR] != -1 )    m_pMech->SetPartsSpr( enPartsFootR, 	enPartsLayerPrtc,NULL);
		if( pArmor->sIndex[enPartsShoulder] != -1 ) m_pMech->SetPartsSpr( enPartsShoulder,	enPartsLayerPrtc,&stSprArmorShoulder[ pArmor->sIndex[enPartsShoulder]]  );

		//オプションパーツ
		if( pArmor->sIndex[enPartsBurnia] != -1 )       m_pMech->SetPartsSpr( enPartsBurnia,		enPartsLayerMech,NULL );//&stSprMech[8] );
		if( pArmor->sIndex[enPartsNozzle] != -1 )       m_pMech->SetPartsSpr( enPartsNozzle,		enPartsLayerMech,NULL );//&stSprMech[9] );
		if( pArmor->sIndex[enPartsBurniaOption] != -1 ) m_pMech->SetPartsSpr( enPartsBurniaOption,	enPartsLayerMech,NULL );//&stSprMech[10] );
		if( pArmor->sIndex[enPartsSubWeapon1] != -1 )   m_pMech->SetPartsSpr( enPartsSubWeapon1,	enPartsLayerMech,&stSprSubWeapon[ pArmor->sIndex[enPartsSubWeapon1] ] );
		if( pArmor->sIndex[enPartsSubWeapon2] != -1 )   m_pMech->SetPartsSpr( enPartsSubWeapon2,	enPartsLayerMech,NULL );//&stSprMech[12] );
		if( pArmor->sIndex[enPartsSubWeapon3] != -1 )   m_pMech->SetPartsSpr( enPartsSubWeapon3,	enPartsLayerMech,NULL );//&stSprMech[13] );
	}

}
#endif
#include "CGunHoundArms.h"

