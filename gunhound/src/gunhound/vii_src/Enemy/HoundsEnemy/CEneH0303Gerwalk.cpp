//--------------------------------------------------------------------------------
//
// Stage00:ガウォーク
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "EnemyTbl/TexEneStg0302Tbl.h"
#include "../../Effect/EffectBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -64,
	enKuraiTop   = -16,
	enKuraiRight =  48,
	enKuraiBottom=  32,

	enScore = 80000,
	enDanmakuIndex = 1,
};


Sint16 sMissileLeft1[] = {
//  60,   0,
   3,   3,   4,   4,   3,   4,   4,   4,   3,   4,   3,   5,   4,   4,   3,   5,
   3,   6,   3,   6,   2,   6,   3,   6,   2,   6,   2,   6,   2,   6,   1,   6,
   1,   6,   1,   5,   1,   5,   0,   4,   0,   4,   0,   3,   0,   4,  -1,   2,
  -1,   3,  -2,   2,  -2,   2,  -2,   2,  -3,   1,  -2,   1,  -3,   0,  -3,   0,
  -4,   0,  -3,   0,  -4,  -1,  -4,  -2,  -4,  -1,  -4,  -2,  -4,  -2,  -5,  -3,
  -5,  -3,  -4,  -3,  -5,  -3,  -5,  -3,  -6,  -4,  -5,  -4,  -6,  -4,  -6,  -4,
  -6,  -4,  -7,  -5,  -7,  -5,  -8,  -6,  -8,  -5,  -8,  -6,  -9,  -6,  -9,  -6,
  -9,  -7,  -9,  -6,  -9,  -7,  -9,  -6,
};

Sint16 sMissileLeft2[] = {
   2,   4,   3,   4,   2,   5,   3,   4,   2,   5,   3,   5,   2,   5,   2,   5,
   2,   6,   2,   6,   2,   6,   2,   7,   1,   6,   1,   7,   2,   7,   1,   7,
   1,   7,   2,   6,   1,   7,   1,   6,   0,   6,   1,   5,   0,   5,   0,   5,
  -1,   4,  -1,   3,  -2,   3,  -2,   3,  -3,   1,  -3,   2,  -4,   0,  -4,   1,
  -4,   0,  -5,   0,  -5,  -1,  -5,  -1,  -6,  -1,  -6,  -1,  -6,  -1,  -6,  -2,
  -6,  -2,  -7,  -2,  -6,  -3,  -7,  -3,  -7,  -3,  -7,  -3,  -8,  -4,  -7,  -4,
  -9,  -4,  -8,  -5,  -9,  -5, -10,  -6, -10,  -6, -11,  -6, -11,  -7, -12,  -7,
 -11,  -7, -12,  -7, -12,  -7, -12,  -7,
};

Sint16 sMissileLeft3[] = {
   2,   5,   2,   6,   2,   6,   2,   6,   2,   6,   2,   6,   1,   6,   2,   7,
   1,   6,   1,   7,   1,   7,   1,   8,   0,   7,   1,   7,   1,   8,   0,   7,
   0,   7,   1,   8,   0,   7,   1,   7,   0,   6,   0,   7,   0,   6,   0,   5,
   0,   5,  -1,   4,  -1,   3,  -1,   3,  -1,   2,  -2,   1,  -3,   2,  -2,   0,
  -4,   1,  -3,   0,  -5,   1,  -4,   0,  -6,   0,  -6,   0,  -7,  -1,  -7,   0,
  -8,  -1,  -9,  -1, -10,  -1, -11,  -1, -11,  -2, -12,  -1, -13,  -3, -12,  -2,
 -14,  -4, -13,  -3, -14,  -5, -13,  -4, -14,  -6, -13,  -6, -13,  -6, -14,  -7,
 -13,  -7, -13,  -7, -13,  -7, -13,  -7,
};



CEneH0303Valkylie::CEneH0303Valkylie( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------

	m_stTarget.x = x;
	m_stTarget.y = y;

	m_Pos.x = x+640*100;
	m_Pos.y = y;

	m_Base.x = x;
	m_Base.y = y;

	m_Foot[0] = 0;
	m_Foot[1] = 0;

	m_Leg[0] = 0;
	m_Leg[1] = 0;

	m_sWalkCycle = 90;
	m_sArmCycle  = 0;
	m_sHandCycle = 0;

	m_sBuraBura[0] = 0;
	m_sBuraBura[1] = 0;

	m_sBuraBuraArm = 0;
	m_sBuraTime[0] = viiSub::Rand()%360;
	m_sBuraTime[1] = viiSub::Rand()%360;

	m_sCrashSeq   = 0;
	m_sCrashTimer = 0;

	m_Add.x = 0;
	m_Add.y = 0;

	m_Tgt.x = m_sWalkCycle;
	m_Tgt.y = 0;

	m_Drop.x = 0;
	m_Drop.y = 0;

	m_sAtackCnt = 0;
	m_sCrashCnt = 0;

	m_pEffFire = NULL;
	m_bDisp = gxTrue;

#ifdef _VII_DEBUG_
	m_HitKurai.set_hp( 1 );
#else
	m_HitKurai.set_hp( enHitPointBossChicken );
#endif

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sAtribute = ATR_DFLT;

	m_sDirection = DIR_LEFT;

	m_bDamage = gxFalse;

	m_pEnemyAnime = new CEnemyAnime();

	for(Sint32 ii=0; ii<enMax ;ii++)
	{
		m_pEnemyAnime->SetParts( ii , enMax , m_sParentCEneH0303Valkylie[ii] , sPosCEneH0303Valkylie[ii] );
	}

	Init();

	m_sAtackSeq = 0;
	m_sLogicSeq = 0;
	m_sControl  = 0;

	CCockpit::GetInstance()->SetDanmakuIndex( enDanmakuIndex , gxTrue );

}


CEneH0303Valkylie::~CEneH0303Valkylie()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

	delete m_pEnemyAnime;

}


void CEneH0303Valkylie::Init()
{
	//-----------------------------------------------------
	//初期化
	//-----------------------------------------------------
	
	m_pEnemyAnime->PosCalc();
	m_pEnemyAnime->GetParts( enRAIHUL )->bDisp = gxFalse;

	Float32 dx,dy;
	//右腕表示位置
	dx = m_pEnemyAnime->GetParts( enUDE )->dx - m_pEnemyAnime->GetParts( enKATA )->dx*1.0f;
	dy = m_pEnemyAnime->GetParts( enUDE )->dy - m_pEnemyAnime->GetParts( enKATA )->dy*1.0f;
	m_stArm[0].dist = (Sint32)viiMath::Dist(dx,dy);
	m_stArm[0].rot  = (Sint32)viiMath::Atan2d( dy , dx );

	//ライフル表示位置
	dx = m_pEnemyAnime->GetParts( enRAIHUL )->dx - m_pEnemyAnime->GetParts( enUDE )->dx*1.0f;
	dy = m_pEnemyAnime->GetParts( enRAIHUL )->dy - m_pEnemyAnime->GetParts( enUDE )->dy*1.0f;
	m_stArm[1].dist = (Sint32)viiMath::Dist(dx,dy);
	m_stArm[1].rot  = (Sint32)viiMath::Atan2d( dy , dx );


	//右脚部表示位置
	dx = m_pEnemyAnime->GetParts( enMIGIHUKUROHAGI )->dx - m_pEnemyAnime->GetParts( enMIGIHUTOMOMO )->dx*1.0f;
	dy = m_pEnemyAnime->GetParts( enMIGIHUKUROHAGI )->dy - m_pEnemyAnime->GetParts( enMIGIHUTOMOMO )->dy*1.0f;

	m_stFoot[0].dist = (Sint32)viiMath::Dist(dx,dy);
	m_stFoot[0].rot  = (Sint32)viiMath::Atan2d( dy , dx );

	dx = m_pEnemyAnime->GetParts( enMIGIKAKATO )->dx - m_pEnemyAnime->GetParts( enMIGIHUKUROHAGI )->dx*1.0f;
	dy = m_pEnemyAnime->GetParts( enMIGIKAKATO )->dy - m_pEnemyAnime->GetParts( enMIGIHUKUROHAGI )->dy*1.0f;

	m_stLeg[0].dist = (Sint32)viiMath::Dist(dx,dy);
	m_stLeg[0].rot  = (Sint32)viiMath::Atan2d( dy , dx );

	m_stKakato[0].dist = 0;
	m_stKakato[0].rot  = 0;

	//左脚部表示位置
	dx = m_pEnemyAnime->GetParts( enHIDARIHUKUROHAGI )->dx - m_pEnemyAnime->GetParts( enHIDARIHUTOMOMO )->dx*1.0f;
	dy = m_pEnemyAnime->GetParts( enHIDARIHUKUROHAGI )->dy - m_pEnemyAnime->GetParts( enHIDARIHUTOMOMO )->dy*1.0f;

	m_stFoot[1].dist = (Sint32)viiMath::Dist(dx,dy);
	m_stFoot[1].rot  = (Sint32)viiMath::Atan2d( dy , dx );

	dx = m_pEnemyAnime->GetParts( enHIDARIKAKATO )->dx - m_pEnemyAnime->GetParts( enHIDARIHUKUROHAGI )->dx*1.0f;
	dy = m_pEnemyAnime->GetParts( enHIDARIKAKATO )->dy - m_pEnemyAnime->GetParts( enHIDARIHUKUROHAGI )->dy*1.0f;

	m_stLeg[1].dist = (Sint32)viiMath::Dist(dx,dy);
	m_stLeg[1].rot  = (Sint32)viiMath::Atan2d( dy , dx );

	m_stKakato[1].dist = 0;
	m_stKakato[1].rot  = 0;

}


void CEneH0303Valkylie::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( m_HitKurai.is_dead() )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	LogicAI();

	m_Add.y = (30*viiMath::Cos100((m_sTimer*8)%360))/100;
	m_Pos.y+=m_Add.y;

//	if( m_sAtackSeq )
//	{
//		Atack();
//	}

//	m_pEnemyAnime->GetParts( enRAIHUL )->bDisp = gxFalse;

	SetRader( enTypeMarkerMiddleBoss );
	DirectionAdjust();

	m_bDamage = m_HitKurai.is_damage();

	//くらい判定登録
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

}



void CEneH0303Valkylie::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	switch( m_sLogicSeq ){
	case 0:	//登場
		m_sLogicWait = 0;
		m_sControl = JOY_D;
		m_sLogicSeq = 5;
		m_stTarget.x = m_Base.x;
		m_stTarget.y = m_Base.y;
		m_Add.x = 0;
		m_Add.y = 0;
		break;
	case 5:	//登場
		m_sControl = JOY_D;
		if( m_sLogicWait >= 30 )
		{
			m_sLogicSeq = 10;
			m_sLogicWait = 0;
		}
		break;

	case 10:	//接近
		m_sControl = JOY_D;
		if( m_Pos.x > m_Base.x )
		{
			if( m_Add.x > -800 ) m_Add.x -= 10;
			m_Pos.x += m_Add.x;//(m_stTarget.x-m_Pos.x)/20;
			m_Pos.y += (m_stTarget.y-m_Pos.y)/20;
		}
		else
		{
			m_sLogicSeq = 20;
			m_sLogicWait = 0;
		}
		break;

	case 20:	//逆噴射
		m_sControl = JOY_U;
//		if( m_Pos.x > m_Base.x )
//		{
//			if( m_Add.x > -300 ) m_Add.x -= 100;
//			m_Pos.x += m_Add.x;
//		}
//		else
		if( m_sLogicWait < 20 )
		{
			m_Add.x += (0-m_Add.x)/20;
			m_Pos.x += m_Add.x;
		}
		else
		{
			if( m_Add.x <  800 ) m_Add.x += 10;
			m_Pos.x += m_Add.x;
			if( m_Pos.x > m_Base.x )
			{
				m_sLogicSeq  = 100;
				m_sLogicWait = 0;
			}
		}
		break;

	case 30:	//逆噴射
	//-----------------------------------
	//ポイントＡで攻撃
	//-----------------------------------
	case 100:	//ポイントAを設定
		m_stTarget.x = m_Base.x+96*100;
		m_stTarget.y = m_Base.y+96*100;
		m_sLogicSeq = 120;
		m_sLogicWait = 0;
		break;

	case 120:	//ポイントAに移動
		m_Pos.x += (m_stTarget.x-m_Pos.x)/20;
		m_Pos.y += (m_stTarget.y-m_Pos.y)/20;
		if( m_sLogicWait >= 60 )
		{
			m_sLogicSeq = 140;
			m_sLogicWait = 0;
		}
		break;
	case 140:	//ポイントAで攻撃
		m_Pos.x += (m_stTarget.x-m_Pos.x)/20;
		m_Pos.y += (m_stTarget.y-m_Pos.y)/20;
		Atack();
		if( m_sLogicWait >= 60 )
		{
			m_sLogicSeq = 160;
			m_sLogicWait = 0;
		}
		break;
	case 160:	//ポイントAで待機
		m_sLogicSeq = 200;
		m_sLogicWait = 0;
		break;

	//-----------------------------------
	//ポイントＢで攻撃
	//-----------------------------------

	case 200:	//ポイントBを設定
		m_stTarget.x = m_Base.x;
		m_stTarget.y = m_Base.y;
		m_sLogicSeq = 220;
		m_sLogicWait = 0;
		break;

	case 220:	//ポイントBに移動
		m_Pos.x += (m_stTarget.x-m_Pos.x)/20;
		m_Pos.y += (m_stTarget.y-m_Pos.y)/20;
		if( m_sLogicWait >= 60 )
		{
			m_sLogicSeq = 240;
			m_sLogicWait = 0;
		}
		break;
	case 240:	//ポイントBで攻撃
		m_Pos.x += (m_stTarget.x-m_Pos.x)/20;
		m_Pos.y += (m_stTarget.y-m_Pos.y)/20;
		Atack();
		if( m_sLogicWait >= 120 )
		{
			m_sLogicSeq = 260;
			m_sLogicWait = 0;
		}
		break;
	case 260:	//ポイントBで待機
		m_sLogicSeq = 1000;
		m_sLogicWait = 0;
		break;

	case 1000:
		//-----------------------------------
		//遠隔攻撃
		//-----------------------------------
		m_stTarget.x = m_Base.x+480*100;
		m_stTarget.y = m_Base.y;
		m_Add.x = 0;
		m_sLogicSeq = 1050;
		m_sLogicWait = 0;
		break;

	case 1050:
		if( m_sLogicWait%45 == 0 )
		{
			Sint32 sTbl[]={
				-32,-32,
				 32, 32,
				-32, 0,
				 32, 0,
				 32, -32,
				-32, 32,
			};

//			AtackValcun( sTbl[m_sAtackCnt*2+0]*100 , sTbl[m_sAtackCnt*2+1]*100 );
			AtackValcun( viiMath::Cos100(m_sAtackCnt*30)*32 , viiMath::Sin100(m_sAtackCnt*30)*32 );
			m_sAtackCnt ++;
			m_sAtackCnt = m_sAtackCnt%6;
		}

		if( m_sLogicWait >= 50*5 )
		{
			m_sLogicSeq = 1100;
			m_sLogicWait = 0;
		}
		break;
	case 1100:
		if( m_Add.x < 800 ) m_Add.x += 10;
		m_Pos.x += m_Add.x;
		m_Pos.y += (m_stTarget.y-m_Pos.y)/20;
		if( m_sLogicWait >= 120 )
		{
			m_sLogicSeq = 1120;
			m_sLogicWait = 0;
		}
		break;

	case 1120:
		if( m_Add.x > 0 )
		{
			m_Add.x -= 10;
		}
		else
		{
			m_Add.x = 0;
		}
		m_Pos.x += m_Add.x;//(m_stTarget.x-m_Pos.x)/20;
		if( m_sLogicWait >= 60 )
		{
			m_sLogicSeq  = 0;
			m_sLogicWait = 0;
		}
		break;
	}

	m_sLogicWait ++;

	LogicGimmick();

}


void CEneH0303Valkylie::LogicGimmick()
{
	//-----------------------------------------------------
	//移動ギミック
	//-----------------------------------------------------

//	m_sArmCycle = -90;

	if( m_sTimer%960 <= 480 && m_sTimer%64 == 0 ) //m_sControl&BTN_X )
	{
		m_sAtackSeq = 1;
	}

/*
	if( m_sControl&JOY_U )
	{
		m_sArmCycle --;
	}
	else if( m_sControl&JOY_D )
	{
		m_sArmCycle ++;
	}
	if( m_sControl&JOY_L )
	{
		if( m_sHandCycle > -45) m_sHandCycle --;
	}
	else if( m_sControl&JOY_R )
	{
		if( m_sHandCycle < 90 ) m_sHandCycle ++;
	}
*/

	if( m_sControl&JOY_U )
	{
		m_Tgt.x = 220;//+viiSub::Rand()%24;
	}
	else if( m_sControl&JOY_D )
	{
		m_Tgt.x = 300;//+viiSub::Rand()%24;;
	}

	{
		Sint32 tx=pGame->GetHound()->m_Pos.x;
		Sint32 ty=pGame->GetHound()->m_Pos.y-64*100;
		Sint32 r = viiMath::Atan2_100(ty-m_Pos.y , m_Pos.x-tx)/100;
		r = viiMath::GetTargetAngle(m_sArmCycle,r);
//		r = (m_sDirection == DIR_RIGHT)? r : (r*-1+180);
		m_sArmCycle += (r-m_sArmCycle)/10;
		m_sHandCycle = -45+68+(4*viiMath::Cos100((m_sBuraBuraArm*8)%360))/100;
	}

	m_sBuraBura[0] = (5*viiMath::Cos100((m_sBuraTime[0]*8)%360))/100;
	m_sBuraBura[1] = (5*viiMath::Cos100((m_sBuraTime[1]*8)%360))/100;
	m_sBuraTime[0] ++;
	m_sBuraTime[1] ++;
	m_sBuraBuraArm ++;

	m_sWalkCycle += (m_Tgt.x - m_sWalkCycle )/10;

	m_sWalkCycle = (360+m_sWalkCycle)%360;
	m_sArmCycle  = (360+m_sArmCycle)%360;
	m_sHandCycle = (360+m_sHandCycle)%360;

	Sint32 sRot = (m_sWalkCycle+150)%360;

	m_Foot[0] = sRot;
	m_Leg[0]  = sRot+(-60*viiMath::Cos100(sRot))/100;

//	sRot -= 25;
	m_Foot[1] = sRot;
	m_Leg[1]  = sRot+(-60*viiMath::Cos100(sRot))/100;

	m_pEnemyAnime->PosCalc();

	GimmickArm();
	GimmickLeg();

}


void CEneH0303Valkylie::GimmickArm()
{
	//-----------------------------------------------------
	//腕部関節ギミック
	//-----------------------------------------------------
	Sint32 sRot,sDist,sDst,sSrc,dx,dy;
	Sint32 sArmRot = m_sArmCycle;
	Sint32 sHandRot = m_sHandCycle+sArmRot;

	sRot  = m_stArm[0].rot+sArmRot;
	sDist = m_stArm[0].dist;

	sSrc = enDOUTAI;
	sDst = enKATA;

	m_pEnemyAnime->GetParts( sDst )->dr = sRot;
	dx = m_pEnemyAnime->GetParts( sDst )->dx+( viiMath::Cos100( sRot )*sDist )/100;
	dy = m_pEnemyAnime->GetParts( sDst )->dy+( viiMath::Sin100( sRot )*sDist )/100;

	sRot  = m_stArm[1].rot+sHandRot;
	sDist = m_stArm[1].dist;
	sSrc = enKATA;
	sDst = enUDE;
	m_pEnemyAnime->GetParts( sDst )->dr += sRot;
	m_pEnemyAnime->GetParts( sDst )->dx = dx;
	m_pEnemyAnime->GetParts( sDst )->dy = dy;

	sSrc = enUDE;
	sDst = enRAIHUL;
	m_pEnemyAnime->GetParts( sDst )->dr += sRot;
	m_pEnemyAnime->GetParts( sDst )->dx = dx;
	m_pEnemyAnime->GetParts( sDst )->dy = dy;

}


void CEneH0303Valkylie::GimmickLeg()
{
	//-----------------------------------------------------
	//脚部関節ギミック
	//-----------------------------------------------------

	Sint32 sRot,sDist,sDst,sSrc,dx,dy;

	sRot  = m_stFoot[0].rot+m_Foot[0];
	sDist = m_stFoot[0].dist;
	sSrc = enDOUTAI;
	sDst = enMIGIHUTOMOMO;
	m_pEnemyAnime->GetParts( sDst )->dr = sRot;
	dx = m_pEnemyAnime->GetParts( sDst )->dx+( viiMath::Cos100( sRot )*sDist )/100;
	dy = m_pEnemyAnime->GetParts( sDst )->dy+( viiMath::Sin100( sRot )*sDist )/100;

	sRot  = m_stLeg[0].rot+m_Leg[0]+m_sBuraBura[0]/4;
	sDist = m_stLeg[0].dist;
	sSrc = enMIGIHUTOMOMO;
	sDst = enMIGIHUKUROHAGI;
	m_pEnemyAnime->GetParts( sDst )->dr = sRot;
	m_pEnemyAnime->GetParts( sDst )->dx = dx;
	m_pEnemyAnime->GetParts( sDst )->dy = dy;

	dx = m_pEnemyAnime->GetParts( sDst )->dx+( viiMath::Cos100( sRot )*sDist )/100;
	dy = m_pEnemyAnime->GetParts( sDst )->dy+( viiMath::Sin100( sRot )*sDist )/100;

	sRot  = m_stKakato[0].rot+m_Leg[0]+m_sBuraBura[0];
	sDist = m_stKakato[0].dist;
	sSrc = enMIGIHUKUROHAGI;
	sDst = enMIGIKAKATO;
	m_pEnemyAnime->GetParts( sDst )->dr = sRot;
	m_pEnemyAnime->GetParts( sDst )->dx = dx;
	m_pEnemyAnime->GetParts( sDst )->dy = dy;

	//-------------------------------------------------------------

	sRot  = m_stFoot[1].rot+m_Foot[1];
	sDist = m_stFoot[1].dist;
	sSrc = enDOUTAI;
	sDst = enHIDARIHUTOMOMO;
	m_pEnemyAnime->GetParts( sDst )->dr = sRot;
	dx = m_pEnemyAnime->GetParts( sDst )->dx+( viiMath::Cos100( sRot )*sDist )/100;
	dy = m_pEnemyAnime->GetParts( sDst )->dy+( viiMath::Sin100( sRot )*sDist )/100;

	sRot  = m_stLeg[1].rot+m_Leg[1]-m_sBuraBura[1]/4;
	sDist = m_stLeg[1].dist;
	sSrc = enHIDARIHUTOMOMO;
	sDst = enHIDARIHUKUROHAGI;
	m_pEnemyAnime->GetParts( sDst )->dr = sRot;
	m_pEnemyAnime->GetParts( sDst )->dx = dx;
	m_pEnemyAnime->GetParts( sDst )->dy = dy;

	dx = m_pEnemyAnime->GetParts( sDst )->dx+( viiMath::Cos100( sRot )*sDist )/100;
	dy = m_pEnemyAnime->GetParts( sDst )->dy+( viiMath::Sin100( sRot )*sDist )/100;

	sRot  = m_stKakato[1].rot+150*m_Leg[1]/100+m_sBuraBura[1];
	sDist = m_stKakato[1].dist;
	sSrc = enHIDARIHUKUROHAGI;
	sDst = enHIDARIKAKATO;
	m_pEnemyAnime->GetParts( sDst )->dr = sRot;
	m_pEnemyAnime->GetParts( sDst )->dx = dx;
	m_pEnemyAnime->GetParts( sDst )->dy = dy;

}


void CEneH0303Valkylie::AtackValcun(Sint32 x ,Sint32 y)
{
	//-----------------------------------------------------
	//バルカン攻撃
	//-----------------------------------------------------
	Sint32 ax,ay;
	Sint32 r = m_sAtackCnt*45;//viiSub::Rand()%360;

	for(Sint32 ii=0;ii<360;ii+=30)
	{

		ax = m_Pos.x+x;
		ay = m_Pos.y+y;
		ax += viiMath::Cos100(ii+r)*28;
		ay += viiMath::Sin100(ii+r)*28;

		CEne0001EneSakretu *p;
		p = new CEne0001EneSakretu( ax , ay , -ii, 280 );
		p->SetBulletIndex(enDanmakuIndex);
	}
	viiMus::PlaySound(enSoundEneShotSmall);
//	viiMus::PlaySound(enSoundEneShotLaser);

}


void CEneH0303Valkylie::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------
//	CAtkH001SplineMissile *p;
	gxBool bAtack = gxFalse;
	Sint32 ax,ay;

	Sint32 sRotation = get_valken_rot( m_Pos.x , m_Pos.y+2400 )+180;
	Sint32 sKaku = sRotation-30+viiSub::Rand()%60;

	ax = m_Pos.x+viiMath::Cos100( sKaku ) *48;
	ay = m_Pos.y+viiMath::Sin100( sKaku ) *48;

	m_sAtackSeq ++;

	switch( m_sAtackSeq ){
	case 10:
	case 20:
	case 30:
	case 45:
	case 60:
	case 80:
	case 100:
		bAtack = gxTrue;
		m_sAtackSeq = 0;
		break;
	}
	if( bAtack )
	{
		CEne0001EneSuperBall *p;
		p = new CEne0001EneSuperBall( ax , ay , sKaku ,300 );
		p->SetPattern( CEne0001EneSuperBall::enPatternMicroMissile );
		p->SetMaxSpeed( 720 );
		p->SetScrollOutRange( 128 );
		p->SetScale( 0.4f);
		p->SetLevel(1);
		p->SetLeft4Dead();
		p->SetBulletIndex(enDanmakuIndex);
		viiMus::PlaySound(enSoundEneShotLaser);
	}

}


void CEneH0303Valkylie::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	CCockpit::GetInstance()->SetDanmakuIndex( enDanmakuIndex , gxFalse );

	switch( m_sCrashSeq ){
	case 0:
		viiEff::Set( EFF_RING , m_Pos.x , m_Pos.y , argset(500) );
		viiMus::PlaySound(enSoundExplosionMiddle);
		viiMus::PlaySound(enSoundExplosionLarge);
		m_sCrashTimer = 0;
		m_sCrashSeq   = 100;
		m_pEffFire = new CEffFire( m_Pos.x , m_Pos.y-8*100 , PRIO_BG2+1);
		m_Drop.y = 0;
		m_Drop.x = 800;
		m_Drop.y = 300;
		break;

	case 100:
		new CEffectBlackFire ( m_Pos.x + (-64+viiSub::Rand()%128)*100, m_Pos.y  + (-32+viiSub::Rand()%64)*100, NULL );

		if( m_sCrashTimer%40 == 0 )
		{
			viiEff::SetBombDeluxe( m_Pos.x+(-32+viiSub::Rand()%64)*100 , m_Pos.y+(-32+viiSub::Rand()%64)*100 , 4 , 3+viiSub::Rand()%2 );
		}
		if( m_Drop.y > -150 )
		{
			m_Drop.y -= 4;
		}
		else
		{
			viiMus::PlaySound(enSoundExplosionMiddle);
			EffectMakeBigBomb( m_Pos.x+(-32+viiSub::Rand()%64)*100 , m_Pos.y+(-32+viiSub::Rand()%64)*100  );		
			m_Drop.x = -500;
			m_sCrashTimer = 0;
			m_sCrashSeq = 300;
		}
		break;

	case 300:
		if( m_Drop.y < 200 ) m_Drop.y +=2;
		if( m_sCrashTimer%20 == 0 )
		{
			m_sCrashCnt ++;
			m_Drop.x = ( m_sCrashCnt%2 )? 500 : -500;
			viiMus::PlaySound(enSoundExplosionMiddle);
			viiEff::SetBombDeluxe( m_Pos.x+(-32+viiSub::Rand()%64)*100 , m_Pos.y+(-32+viiSub::Rand()%64)*100 , 4 , 3+viiSub::Rand()%2 );
			//EffectMakeBigBomb( m_Pos.x+(-32+viiSub::Rand()%64)*100 , m_Pos.y+(-32+viiSub::Rand()%64)*100  );		
		}
		if( m_sCrashTimer > 60 )
		{
			viiMus::PlaySound( enSoundCrashB );
			new CEffectFinalExplosion( m_Pos.x , m_Pos.y ,8 );
			m_sCrashTimer = 0;
			m_sCrashSeq = 900;
		}
		break;

	case 900:
		m_Drop.y += (0-m_Drop.y)/30;
		if( m_sCrashTimer == 100 )
		{
			m_bDisp = gxFalse;
			viiMus::PlaySound( enSoundCrashA );
			viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
		}
		if( m_sCrashTimer == 120 )
		{
			new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, 180+15 , 800/2 );
			new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, 180+65 , 600/2 );
			new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, -60    , 800/2 );
			new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, -20    , 700/2 );
			m_sCrashTimer = 0;
			m_sCrashSeq = 950;
			delete m_pEffFire;
			m_pEffFire = NULL;
			ScorePlus( enScore );
		}
		break;
	case 950:
		if( m_sCrashTimer > 120 )
		{
			m_bDead = gxTrue;
		}
		break;
	}
	m_Drop.x += (0-m_Drop.x)/10;

	m_Pos.x += m_Drop.x;
	m_Pos.y += m_Drop.y;

	if( m_pEffFire )
	{
		m_pEffFire->m_Pos.x = m_Pos.x;
		m_pEffFire->m_Pos.y = m_Pos.y;
		m_pEffFire->m_Pos.z = PRIO_ENEMY;
	}
	m_sCrashTimer ++;

	if( m_bClear )
	{
		SetActionSeq( enActionSeqEnd );
	}

}

void CEneH0303Valkylie::Draw()
{
	if( !m_bDisp ) return;
//	viiDraw::Sprdmg( m_HitKurai.is_damage() , &SprCEneH0303Valkylie[ 0 ] , m_Pos.x , m_Pos.y , PRIO_ENEMY , m_sAtribute|m_sAtributeDir , ARGB_DFLT );
	m_pEnemyAnime->Draw( m_bDamage , m_Pos.x , m_Pos.y , ( m_sDirection == DIR_LEFT )? gxTrue : gxFalse , &SprCEneH0303Valkylie[0] );

}

