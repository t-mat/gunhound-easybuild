//--------------------------------------------------------------------------------
//
// Stage00:グラージ
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "EnemyTbl/CEneH0204GrazziTBL.h"
#include "../../Effect/EffectBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -32,
	enKuraiTop   = -132,
	enKuraiRight =  48,
	enKuraiBottom=  -88,

	enScore = 10000,
	enDanmakuIndex = 0,
};


CEneH0204Grazzi::CEneH0204Grazzi( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_Base.x = x;
	m_Base.y = y;

	m_Right.x = 0;
	m_Right.y = 0;
	m_Left.x = 0;
	m_Left.y = 0;

	m_sWalkCycle = 90;

	m_Add.x = m_sWalkCycle;
	m_Add.y = 0;

	m_Tgt.x = m_sWalkCycle;
	m_Tgt.y = 0;

	m_HitKurai.set_hp( 900 );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sAtribute = ATR_DFLT;
	m_sDirection = DIR_LEFT;
	m_sAtackType = 0;
	m_sAtackSeq   = 0;
	m_sAtackTimer = 0;
	m_sTargetRot  = 0;
	m_bMoveable   = gxTrue;

	m_bDamage = gxFalse;

	m_pEnemyAnime = new CEnemyAnime();

	for(Sint32 ii=0; ii<enMax ;ii++)
	{
		m_pEnemyAnime->SetParts( ii , enMax , m_sParentCEneH0204Grazzi[ii] , sPosCEneH0204Grazzi[ii] );
	}

	Init();

	m_sControl = 0;
	m_sLogicTime = 0;
	m_bMoveLimit = gxFalse;
	m_sMoveX1 = 0;
	m_sMoveX2 = 0;

	m_sRebound = 0;
	m_bWalkEnd = gxTrue;
	m_sCrashTimer = 0;

	CCockpit::GetInstance()->SetDanmakuIndex( enDanmakuIndex , gxTrue );

}


CEneH0204Grazzi::~CEneH0204Grazzi()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

	delete m_pEnemyAnime;

}


void CEneH0204Grazzi::Init()
{
	//-----------------------------------------------------
	//初期化
	//-----------------------------------------------------
	
	m_pEnemyAnime->PosCalc();

	Float32 dx,dy;
	//右腕表示位置
	dx = m_pEnemyAnime->GetParts( enBARUKANMIGI )->dx - m_pEnemyAnime->GetParts( enMIGIKATA )->dx*1.0f;
	dy = m_pEnemyAnime->GetParts( enBARUKANMIGI )->dy - m_pEnemyAnime->GetParts( enMIGIKATA )->dy*1.0f;
	m_stArm[0].dist = (Sint32)viiMath::Dist(dx,dy);
	m_stArm[0].rot  = (Sint32)viiMath::Atan2d( dy , dx );

	//左腕表示位置
	dx = m_pEnemyAnime->GetParts( enBARUKANHIDARI )->dx - m_pEnemyAnime->GetParts( enHIDARIKATA )->dx*1.0f;
	dy = m_pEnemyAnime->GetParts( enBARUKANHIDARI )->dy - m_pEnemyAnime->GetParts( enHIDARIKATA )->dy*1.0f;
	m_stArm[1].dist = (Sint32)viiMath::Dist(dx,dy);
	m_stArm[1].rot  = (Sint32)viiMath::Atan2d( dy , dx );

	//右脚部表示位置
	dx = m_pEnemyAnime->GetParts( enMIGISUNE )->dx - m_pEnemyAnime->GetParts( enMIGIHUTOMOMO )->dx*1.0f;
	dy = m_pEnemyAnime->GetParts( enMIGISUNE )->dy - m_pEnemyAnime->GetParts( enMIGIHUTOMOMO )->dy*1.0f;

	m_stFoot[0].dist = (Sint32)viiMath::Dist(dx,dy);
	m_stFoot[0].rot  = (Sint32)viiMath::Atan2d( dy , dx );

	dx = m_pEnemyAnime->GetParts( enMIGIKAKATO )->dx - m_pEnemyAnime->GetParts( enMIGISUNE )->dx*1.0f;
	dy = m_pEnemyAnime->GetParts( enMIGIKAKATO )->dy - m_pEnemyAnime->GetParts( enMIGISUNE )->dy*1.0f;

	m_stLeg[0].dist = (Sint32)viiMath::Dist(dx,dy);
	m_stLeg[0].rot  = (Sint32)viiMath::Atan2d( dy , dx );

	m_stKakato[0].dist = 0;
	m_stKakato[0].rot  = 0;

	//左脚部表示位置
	dx = m_pEnemyAnime->GetParts( enHIDARISUNE )->dx - m_pEnemyAnime->GetParts( enHIDARIHUTOMOMO )->dx*1.0f;
	dy = m_pEnemyAnime->GetParts( enHIDARISUNE )->dy - m_pEnemyAnime->GetParts( enHIDARIHUTOMOMO )->dy*1.0f;

	m_stFoot[1].dist = (Sint32)viiMath::Dist(dx,dy);
	m_stFoot[1].rot  = (Sint32)viiMath::Atan2d( dy , dx );

	dx = m_pEnemyAnime->GetParts( enHIDARIKAKATO )->dx - m_pEnemyAnime->GetParts( enHIDARISUNE )->dx*1.0f;
	dy = m_pEnemyAnime->GetParts( enHIDARIKAKATO )->dy - m_pEnemyAnime->GetParts( enHIDARISUNE )->dy*1.0f;

	m_stLeg[1].dist = (Sint32)viiMath::Dist(dx,dy);
	m_stLeg[1].rot  = (Sint32)viiMath::Atan2d( dy , dx );

	m_stKakato[1].dist = 0;
	m_stKakato[1].rot  = 0;
}


void CEneH0204Grazzi::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------


	if( m_HitKurai.is_dead())
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	LogicAI();

	SetRader( enTypeMarkerMiddleBoss );
	DirectionAdjust();

	m_bDamage = m_HitKurai.is_damage();

	//くらい判定登録
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

}



void CEneH0204Grazzi::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------


	if( m_bMoveable ) LogicWalk();

	if( m_sLogicTime == 0 )
	{
		Atack();
	}

	ArmControl();

	//------------------------------------
	//ギミック制御
	//------------------------------------

	m_sWalkCycle = (360+m_sWalkCycle)%360;

	Sint32 sRot = m_sWalkCycle;

	m_stFoot[0].rot = 90+55+(45*viiMath::Cos100(sRot))/100;
	m_stLeg[0].rot  = 100-10+(45*viiMath::Sin100(sRot))/100;;
	m_stKakato[0].rot = 0;

	sRot = (m_sWalkCycle+180)%360;

	m_stFoot[1].rot = 90+55+(45*viiMath::Cos100(sRot))/100;
	m_stLeg[1].rot  = 100-10+(45*viiMath::Sin100(sRot))/100;;
	m_stKakato[1].rot = 0;


	m_pEnemyAnime->PosCalc();

	if( m_sRebound > 0 )
	{
		m_sRebound += (0-m_sRebound)/10;
		m_pEnemyAnime->GetParts( enBODY )->dx = -8*m_sRebound/100;
	}

	GimmickArm();
	GimmickLeg();

	m_pEnemyAnime->GetParts( enATAMAROKETTO )->bDisp = gxFalse;
}


void CEneH0204Grazzi::LogicWalk()
{
	//-----------------------------------------------------
	//歩きロジック
	//-----------------------------------------------------

	Sint32 tx = pGame->GetHound()->m_Pos.x+18000;

	if( m_bMoveLimit )
	{
		if( m_Pos.x >= m_sMoveX2 )
		{
			m_sControl = JOY_L;
			m_sLogicTime = 64;
		}
		else if( m_Pos.x <= m_sMoveX1 )
		{
			m_sControl = JOY_R;
			m_sLogicTime = 64;
		}
	}

	if(m_sLogicTime > 0)
	{
		//強制移動時間
		m_sLogicTime --;
	}
	else if(gxAbs(tx-m_Pos.x) >= 128*100)
	{
		if( tx < m_Pos.x ) m_sControl = JOY_L;
		if( tx > m_Pos.x ) m_sControl = JOY_R;
	}


	if( m_Tgt.x == m_Add.x )
	{
		if(!m_bWalkEnd)
		{
			pGame->QuakeOn(32);
			viiMus::PlaySound( enSoundLanding );
			m_bWalkEnd = gxTrue;
		}

		if( m_sDirection == DIR_LEFT )
		{
			if( m_sControl&JOY_L )
			{
				m_Tgt.x -= 180;
			}
			else if( m_sControl&JOY_R )
			{
				m_Tgt.x += 180;
			}
		}
		else
		{
			if( m_sControl&JOY_L )
			{
				m_Tgt.x += 180;
			}
			else if( m_sControl&JOY_R )
			{
				m_Tgt.x -= 180;
			}
		}
	}
	else
	{
		m_bWalkEnd = gxFalse;
	}

	if( m_Tgt.x > m_Add.x )
	{
		m_Add.x += 4;
		if( m_Tgt.x < m_Add.x )
		{
			m_Add.x = m_Tgt.x;
		}
	}
	else if( m_Tgt.x < m_Add.x )
	{
		m_Add.x -= 4;
		if( m_Tgt.x > m_Add.x )
		{
			m_Add.x = m_Tgt.x;
		}
	}

	m_sWalkCycle = (360+m_Add.x)%360;
}


void CEneH0204Grazzi::GimmickArm()
{
	//-----------------------------------------------------
	//腕部関節ギミック
	//-----------------------------------------------------
	Sint32 sRot,sDist,sDst,sSrc,dx,dy;
	Sint32 sLeft,sRight;
	Sint32 sHandLeft,sHandRight;
	sLeft  = m_StAutoArm[0].GetRot();
	sRight = m_StAutoArm[1].GetRot();
	sHandLeft  = m_StAutoHand[0].GetRot();
	sHandRight = m_StAutoHand[1].GetRot();

	sRot  = m_stArm[0].rot+sRight;
	sDist = m_stArm[0].dist;
	sSrc = enBODY;
	sDst = enMIGIKATA;

	m_pEnemyAnime->GetParts( sDst )->dr = sRight;
	dx = m_pEnemyAnime->GetParts( sDst )->dx+( viiMath::Cos100( sRot )*sDist )/100;
	dy = m_pEnemyAnime->GetParts( sDst )->dy+( viiMath::Sin100( sRot )*sDist )/100;

	sSrc = enMIGIKATA;
	sDst = enBARUKANMIGI;
	m_pEnemyAnime->GetParts( sDst )->dr = sHandRight;//-32+(32*viiMath::Sin100(sRight))/100;//m_sArmCycle;
	m_pEnemyAnime->GetParts( sDst )->dx = dx;
	m_pEnemyAnime->GetParts( sDst )->dy = dy;

	//---------------------------------------

	sRot  = m_stArm[1].rot+sLeft;
	sDist = m_stArm[1].dist;
	sSrc = enBODY;
	sDst = enHIDARIKATA;

	m_pEnemyAnime->GetParts( sDst )->dr = sLeft;
	dx = m_pEnemyAnime->GetParts( sDst )->dx+( viiMath::Cos100( sRot )*sDist )/100;
	dy = m_pEnemyAnime->GetParts( sDst )->dy+( viiMath::Sin100( sRot )*sDist )/100;

	sSrc = enHIDARIKATA;
	sDst = enBARUKANHIDARI;
	m_pEnemyAnime->GetParts( sDst )->dr = sHandLeft;	//-32+(32*viiMath::Sin100(sRight))/100;//
	m_pEnemyAnime->GetParts( sDst )->dx = dx;
	m_pEnemyAnime->GetParts( sDst )->dy = dy;


}

void CEneH0204Grazzi::GimmickLeg()
{
	//-----------------------------------------------------
	//脚部関節ギミック
	//-----------------------------------------------------
	Sint32 sRot,sDist,sDst,sSrc,dx,dy;
	ghPos Right,Left;

	sRot  = m_stFoot[0].rot;
	sDist = m_stFoot[0].dist;
	sSrc = enBODY;
	sDst = enMIGIHUTOMOMO;
	m_pEnemyAnime->GetParts( sDst )->dr = sRot;

	dx = m_pEnemyAnime->GetParts( sDst )->dx+( viiMath::Cos100( sRot )*sDist )/100;
	dy = m_pEnemyAnime->GetParts( sDst )->dy+( viiMath::Sin100( sRot )*sDist )/100;

	sRot  = m_stLeg[0].rot;
	sDist = m_stLeg[0].dist;
	sSrc = enMIGIHUTOMOMO;
	sDst = enMIGISUNE;
	m_pEnemyAnime->GetParts( sDst )->dr = sRot;
	m_pEnemyAnime->GetParts( sDst )->dx = dx;
	m_pEnemyAnime->GetParts( sDst )->dy = dy;

	dx = m_pEnemyAnime->GetParts( sDst )->dx+( viiMath::Cos100( sRot )*sDist )/100;
	dy = m_pEnemyAnime->GetParts( sDst )->dy+( viiMath::Sin100( sRot )*sDist )/100;

	sRot  = m_stKakato[0].rot;
	sDist = m_stKakato[0].dist;
	sSrc = enMIGISUNE;
	sDst = enMIGIKAKATO;
	m_pEnemyAnime->GetParts( sDst )->dr = sRot;
	m_pEnemyAnime->GetParts( sDst )->dx = dx;
	m_pEnemyAnime->GetParts( sDst )->dy = dy;
	
	Right.x = dx;
	Right.y = dy;
	//-------------------------------------------------------------

	sRot  = m_stFoot[1].rot;
	sDist = m_stFoot[1].dist;
	sSrc = enBODY;
	sDst = enHIDARIHUTOMOMO;
	m_pEnemyAnime->GetParts( sDst )->dr = sRot;

	dx = m_pEnemyAnime->GetParts( sDst )->dx+( viiMath::Cos100( sRot )*sDist )/100;
	dy = m_pEnemyAnime->GetParts( sDst )->dy+( viiMath::Sin100( sRot )*sDist )/100;

	sRot  = m_stLeg[1].rot;
	sDist = m_stLeg[1].dist;
	sSrc = enHIDARIHUTOMOMO;
	sDst = enHIDARISUNE;
	m_pEnemyAnime->GetParts( sDst )->dr = sRot;
	m_pEnemyAnime->GetParts( sDst )->dx = dx;
	m_pEnemyAnime->GetParts( sDst )->dy = dy;

	dx = m_pEnemyAnime->GetParts( sDst )->dx+( viiMath::Cos100( sRot )*sDist )/100;
	dy = m_pEnemyAnime->GetParts( sDst )->dy+( viiMath::Sin100( sRot )*sDist )/100;


	sRot  = m_stKakato[1].rot;
	sDist = m_stKakato[1].dist;
	sSrc = enHIDARISUNE;
	sDst = enHIDARIKAKATO;
	m_pEnemyAnime->GetParts( sDst )->dr = sRot;
	m_pEnemyAnime->GetParts( sDst )->dx = dx;
	m_pEnemyAnime->GetParts( sDst )->dy = dy;

	Left.x = dx;
	Left.y = dy;

	Sint32 sAdd=0,sAddY;

	if(Left.y > Right.y)
	{
		//左足がついていた
		sAdd = (m_Left.x-Left.x);
		sAddY = Left.y;
	}
	else
	{
		//右足がついていた
		sAdd = (m_Right.x-Right.x);
		sAddY = Right.y;
	}

	sAdd *= m_sDirection;
	m_Pos.y = m_Base.y - sAddY*100;
	m_Pos.x += sAdd*100;//(100-viiSub::Rand()%2)*sAdd/100*100;

	m_Right = Right;
	m_Left  = Left;

//	viiDbg::printf(100,50,"CYCLE %d",m_sWalkCycle );
//	viiDbg::printf(100,50+16*1,"Right %03d / %03d",Right.x ,Right.y);
//	viiDbg::printf(100,50+16*2,"Left  %03d / %03d",Left.x ,Left.y );

}


void CEneH0204Grazzi::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

	switch( m_sAtackType ){
	case 0:
		AtackArm();
		break;
	case 1:
		AtackChest();
		break;
	}

	if(m_sAtackSeq == 999)
	{
		m_sAtackSeq = 0;
		m_sAtackType ++;
		m_sAtackType = m_sAtackType%2;
	}
}

enum {
	enChestAtackInit,
	enChestAtackYobiDousaStartR,
	enChestAtackYobiDousaStartL,
	enChestAtackYobiDousaWait,
	enChestAtackYobiDousaEnd,
	enChestAtackAtack,
	enChestAtackEnd=999,
};

gxBool CEneH0204Grazzi::AtackChest()
{
	//-----------------------------------------------------
	//胸部バルカン攻撃
	//-----------------------------------------------------
	CEne0001EneBullet *pEneBullet;

	switch(m_sAtackSeq){
	case enChestAtackInit:
		m_sAtackSeq   = 0;
		m_sAtackTimer = 0;
		SetAutoArm( DIR_LEFT  , -90 , -120  , 30 );
		SetAutoArm( DIR_RIGHT , -80 , -70 , 30 );

		m_bMoveable = gxFalse;
		if( pGame->GetHound()->m_Pos.x < m_Pos.x )
		{
			m_sAtackSeq = enChestAtackYobiDousaStartL;
		}
		else
		{
			m_sAtackSeq = enChestAtackYobiDousaStartR;
		}
		break;

	case enChestAtackYobiDousaStartR:
		if(m_sAtackTimer <= 180)
		{
			if( m_sAtackTimer%8 == 0 )
			{
				Sint32 ax,ay,r = 40;
				ax = m_Pos.x-3200;
				ay = m_Pos.y-16000;
				viiEff::Set(EFF_MAZZLE,ax-800*2 , ay+800*0,NULL);
				pEneBullet = new CEne0001EneBullet( ax-800*2 , ay+800*0, r-20 , 140+m_sAtackTimer*3 );
				pEneBullet->SetBulletIndex(enDanmakuIndex);

				viiEff::Set(EFF_MAZZLE,ax-800*2 , ay+800*2,NULL);
				pEneBullet = new CEne0001EneBullet( ax-800*2 , ay+800*2, r-40 , 140+m_sAtackTimer*3 );
				pEneBullet->SetBulletIndex(enDanmakuIndex);

				viiEff::Set(EFF_MAZZLE,ax-800*2 , ay+800*4,NULL);
				pEneBullet = new CEne0001EneBullet( ax-800*2 , ay+800*4, r-60 , 140+m_sAtackTimer*3 );
				pEneBullet->SetBulletIndex(enDanmakuIndex);

				viiEff::Set(EFF_MAZZLE,ax+800*2 , ay+800*0,NULL);
				pEneBullet = new CEne0001EneBullet( ax+800*2 , ay+800*0, r+20 , 140+m_sAtackTimer*3 );
				pEneBullet->SetBulletIndex(enDanmakuIndex);

				viiEff::Set(EFF_MAZZLE,ax+800*2 , ay+800*2,NULL);
				pEneBullet = new CEne0001EneBullet( ax+800*2 , ay+800*2, r+40 , 140+m_sAtackTimer*3 );
				pEneBullet->SetBulletIndex(enDanmakuIndex);

				viiEff::Set(EFF_MAZZLE,ax+800*2 , ay+800*4,NULL);
				pEneBullet = new CEne0001EneBullet( ax+800*2 , ay+800*4, r+60 , 140+m_sAtackTimer*3 );
				pEneBullet->SetBulletIndex(enDanmakuIndex);
			}
			break;
		}
		if(m_sAtackTimer <= 320)
		{
			//いないいいない
			break;
		}
		m_bMoveable = gxFalse;
		if( m_StAutoArm[0].IsRotEnd() )
		{
			if(m_sAtackTimer >= 30 )
			{
				m_sAtackTimer = 0;
				m_sAtackSeq = enChestAtackYobiDousaEnd;
			}
		}
		else
		{
			m_sAtackTimer = 0;
		}
		break;

	case enChestAtackYobiDousaStartL:
		if(m_sAtackTimer <= 180)
		{
			if( m_sAtackTimer%8 == 0 )
			{
				Sint32 ax,ay,r = 45+90;
				ax = m_Pos.x-3200;
				ay = m_Pos.y-16000;

				viiEff::Set(EFF_MAZZLE,ax-800*2 , ay+800*0,NULL);
				pEneBullet = new CEne0001EneBullet( ax-800*2 , ay+800*0, r-20 , 140+m_sAtackTimer*3 );
				pEneBullet->SetBulletIndex(enDanmakuIndex);

				viiEff::Set(EFF_MAZZLE,ax-800*2 , ay+800*2,NULL);
				pEneBullet = new CEne0001EneBullet( ax-800*2 , ay+800*2, r-40 , 140+m_sAtackTimer*3 );
				pEneBullet->SetBulletIndex(enDanmakuIndex);

				viiEff::Set(EFF_MAZZLE,ax-800*2 , ay+800*4,NULL);
				pEneBullet = new CEne0001EneBullet( ax-800*2 , ay+800*4, r-60 , 140+m_sAtackTimer*3 );
				pEneBullet->SetBulletIndex(enDanmakuIndex);

				viiEff::Set(EFF_MAZZLE,ax+800*2 , ay+800*0,NULL);
				pEneBullet = new CEne0001EneBullet( ax+800*2 , ay+800*0, r+20 , 140+m_sAtackTimer*3 );
				pEneBullet->SetBulletIndex(enDanmakuIndex);

				viiEff::Set(EFF_MAZZLE,ax+800*2 , ay+800*2,NULL);
				pEneBullet = new CEne0001EneBullet( ax+800*2 , ay+800*2, r+40 , 140+m_sAtackTimer*3 );
				pEneBullet->SetBulletIndex(enDanmakuIndex);

				viiEff::Set(EFF_MAZZLE,ax+800*2 , ay+800*4,NULL);
				pEneBullet = new CEne0001EneBullet( ax+800*2 , ay+800*4, r+60 , 140+m_sAtackTimer*3 );
				pEneBullet->SetBulletIndex(enDanmakuIndex);

			}
			break;
		}
		if(m_sAtackTimer <= 320)
		{
			//いないいいない
			break;
		}
		m_bMoveable = gxFalse;
		if( m_StAutoArm[0].IsRotEnd() )
		{
			if(m_sAtackTimer >= 30 )
			{
				m_sAtackTimer = 0;
				m_sAtackSeq = enChestAtackYobiDousaEnd;
			}
		}
		else
		{
			m_sAtackTimer = 0;
		}
		break;

	case enChestAtackYobiDousaEnd:
		//ばあーー
		SetAutoArm( DIR_LEFT  , 15+15 , 15 , 15 );
		SetAutoArm( DIR_RIGHT , 45+15 , 25 , 15 );
		m_sTargetRot = 180+15;//get_valken_rot( m_Pos.x , m_Pos.y+(-112+32)*100);
		m_sAtackTimer = 0;
		m_sAtackSeq = enChestAtackAtack;
		break;

	case enChestAtackAtack:
		m_sAtackSeq = enChestAtackEnd;
		break;

	case enChestAtackEnd:
		m_bMoveable = gxTrue;
		m_sAtackSeq = 0;//enChestAtackEnd;
		break;
	}

	m_sAtackTimer ++;

	return gxFalse;
}


enum {
	enArmAtackInit,
	enArmAtackYobiDousaInit,
	enArmAtackYobiDousaMain,
	enArmAtackHidariteSet,
	enArmAtackHidariteHassya,
	enArmAtackYobiDousaEnd = 999,
};

gxBool CEneH0204Grazzi::AtackArm()
{
	//-----------------------------------------------------
	//腕部レーザーミサイル発射
	//-----------------------------------------------------
	Sint32 ax,ay,r=180;

	switch(m_sAtackSeq){
	case enArmAtackInit:
		m_sAtackSeq   = 0;
		m_sAtackTimer = 0;
		SetAutoArm( DIR_LEFT  , -90 , -120  , 30 );
		SetAutoArm( DIR_RIGHT , -80 , -70 , 30 );

		m_bMoveable = gxTrue;
		m_sAtackSeq = enArmAtackYobiDousaInit;
		break;
	case enArmAtackYobiDousaInit:
		if(m_sAtackTimer < 60 )
		{
			break;
		}
		if( !m_bWalkEnd )
		{
			break;
		}
		m_bMoveable = gxFalse;
		if( m_StAutoArm[0].IsRotEnd() )
		{
			if( pGame->GetHound()->m_Pos.x > m_Pos.x )
			{
				m_sAtackSeq = enArmAtackYobiDousaEnd;
			}
			else
			{
				SetAutoArm( DIR_LEFT  , 90 , 90 , 10 );
				SetAutoArm( DIR_RIGHT , 80 , 90 , 10 );
				m_sAtackTimer = 0;
				m_sAtackSeq = enArmAtackYobiDousaMain;
			}
		}
		break;
	case enArmAtackYobiDousaMain:
		if( m_StAutoArm[0].IsRotEnd() )
		{
			SetAutoArm( DIR_LEFT  , 55 , 0  , 30 );
			SetAutoArm( DIR_RIGHT , 55  ,0 , 35 );
			m_sAtackTimer = 0;
			m_bMoveable = gxFalse;
			m_sAtackSeq = enArmAtackHidariteSet;
		}
		break;

	case enArmAtackHidariteSet:

		if(  m_sAtackTimer < 60 )
		{
			//左手エネルギー注入
			ax = m_Pos.x-6400;
			ay = m_Pos.y-6400;
			viiEff::Set(EFF_ENERGYBALL , ax , ay , argset(200,200,0,100));
		}

		if( m_sAtackTimer < 40 )
		{
			//右手エネルギー注入
			ax = m_Pos.x-4600;
			ay = m_Pos.y-7200;
			viiEff::Set(EFF_ENERGYBALL , ax , ay , argset(200,200,0,100));
		}

		if( m_sAtackTimer == 60 )
		{
			//左手発射
			ax = m_Pos.x-6400;
			ay = m_Pos.y-6000;
			m_sRebound = 100;
			SetAutoArm( DIR_LEFT  , 55 , -50  , 5 );
			CEne0001EneSuperBall *p;
			p = new CEne0001EneSuperBall( ax , ay-8*100 , 180, 800 );
			p->SetPattern( CEne0001EneSuperBall::enPatternDefault );
			p->SetSpeed( 520 );
			p->SetBulletIndex(enDanmakuIndex);

			p = new CEne0001EneSuperBall( ax , ay+8*100 , 180, 800 );
			p->SetPattern( CEne0001EneSuperBall::enPatternDefault );
			p->SetSpeed( 520 );
			p->SetBulletIndex(enDanmakuIndex);

		}
		else if( m_sAtackTimer == 70 )
		{
			//左手リバウンド
			SetAutoArm( DIR_LEFT  , -25 , 0  , 40 );
		}
		else if( m_sAtackTimer == 100 )
		{
			//右手発射
			ax = m_Pos.x-4600;
			ay = m_Pos.y-6800;
			m_sRebound = 100;
			{
				CEne0001EneSuperBall *p;
				p = new CEne0001EneSuperBall( ax , ay-8*100 , 180, 800 );
				p->SetPattern( CEne0001EneSuperBall::enPatternDefault );
				p->SetSpeed( 520 );
				p->SetBulletIndex(enDanmakuIndex);

				p = new CEne0001EneSuperBall( ax , ay+8*100 , 180, 800 );
				p->SetPattern( CEne0001EneSuperBall::enPatternDefault );
				p->SetSpeed( 520 );
				p->SetBulletIndex(enDanmakuIndex);

			}
			SetAutoArm( DIR_RIGHT , 55  , -50 , 5 );
		}
		else if( m_sAtackTimer == 110 )
		{
			//右手リバウンド
			SetAutoArm( DIR_RIGHT , -25  , 0 , 40 );
			m_sAtackSeq = enArmAtackHidariteHassya;
		}
		break;

	case enArmAtackHidariteHassya:
		if( m_StAutoArm[1].IsRotEnd() )
		{
			if( m_sAtackTimer >= 120 )
			{
				m_sAtackTimer = 0;
				m_sAtackSeq = enArmAtackYobiDousaEnd;
			}
		}
		break;

	case enArmAtackYobiDousaEnd:
		m_bMoveable = gxTrue;
		m_sAtackSeq   = 0;
		m_sAtackTimer = 0;
		break;
	}

	m_sAtackTimer ++;

	return gxFalse;

}


gxBool CEneH0204Grazzi::AtackBazooka()
{
	//-----------------------------------------------------
	//メインバルカンでの攻撃
	//-----------------------------------------------------


	return gxFalse;

}

void CEneH0204Grazzi::ArmControl( )
{
	for(Sint32 ii=0;ii<2;ii++)
	{
		if( m_StAutoArm[ii].sFrm > 0 )
		{
			m_StAutoArm[ii].sFrm --;
			m_StAutoArm[ii].fRot += m_StAutoArm[ii].fAdd;
		}
		else
		{
			m_StAutoArm[ii].bAuto = gxFalse;
		}

		if( m_StAutoHand[ii].sFrm > 0 )
		{
			m_StAutoHand[ii].sFrm --;
			m_StAutoHand[ii].fRot += m_StAutoHand[ii].fAdd;
		}
		else
		{
			m_StAutoHand[ii].bAuto = gxFalse;
		}
	}

}


void CEneH0204Grazzi::SetAutoArm( Sint32 sSide , Sint32 sRot1, Sint32 sRot2, Sint32 sFrm )
{
	//-------------------------------------
	//オートアーム
	//-------------------------------------
	Float32 fAdd = ZERO;

	if(sSide==DIR_LEFT)
	{
		m_StAutoArm[0].sFrm  = sFrm;
		m_StAutoArm[0].bAuto = gxTrue;
		m_StAutoArm[0].fAdd  = (sRot1 - m_StAutoArm[0].fRot)/sFrm;
		m_StAutoArm[0].bAuto  = gxTrue;

		m_StAutoHand[0].sFrm  = sFrm;
		m_StAutoHand[0].bAuto = gxTrue;
		m_StAutoHand[0].fAdd  = (sRot2 - m_StAutoHand[0].fRot)/sFrm;
		m_StAutoHand[0].bAuto = gxTrue;
	}
	else if(sSide==DIR_RIGHT)
	{
		m_StAutoArm[1].sFrm  = sFrm;
		m_StAutoArm[1].bAuto = gxTrue;
		m_StAutoArm[1].fAdd  = (sRot1 - m_StAutoArm[1].fRot)/sFrm;
		m_StAutoArm[1].bAuto  = gxTrue;

		m_StAutoHand[1].sFrm  = sFrm;
		m_StAutoHand[1].bAuto = gxTrue;
		m_StAutoHand[1].fAdd  = (sRot2 - m_StAutoHand[1].fRot)/sFrm;
		m_StAutoHand[1].bAuto = gxTrue;
	}

}



void CEneH0204Grazzi::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	CEffectDmgprt *p;
	Sint32 ax,ay;

	ax = m_Pos.x;
	ay = m_Pos.y-128*100;

	if( m_sCrashTimer%4==0 )
	{
		viiEff::SetBombDeluxe( ax , ay , 1 , 1+viiSub::Rand()%2 );
	}

	if(!m_bDead)
	{
		switch( m_sCrashTimer ){
		case 0:
			viiMus::PlaySound(enSoundExplosionMiddle);
			break;
		case 20:
			viiMus::PlaySound(enSoundExplosionMiddle);
			EffectMakeBigBomb( m_Pos.x+0*100 , m_Pos.y-164*100 );
			break;
		case 40:
			viiMus::PlaySound(enSoundExplosionMiddle);
			viiMus::PlaySound(enSoundExplosionLarge);
			p = new CEffectDmgprt( ax, ay -1600, 180+15 , 600 );
			p->SetSprite( &SprCEneH0204Grazzi[ m_pEnemyAnime->GetParts( enBARUKANMIGI )->sSprite ] );

			p=new CEffectDmgprt( ax, ay -1600, 180+65 , 400 );
			p->SetSprite( &SprCEneH0204Grazzi[ m_pEnemyAnime->GetParts( enBARUKANHIDARI )->sSprite ] );

			p=new CEffectDmgprt( ax, ay -1600, -60    , 500 );
			p->SetSprite( &SprCEneH0204Grazzi[ m_pEnemyAnime->GetParts( enHIDARIKATA )->sSprite ] );

			p=new CEffectDmgprt( ax, ay -1600, -20    , 500 );
			p->SetSprite( &SprCEneH0204Grazzi[ m_pEnemyAnime->GetParts( enMIGIKATA )->sSprite ] );
			EffectMakeBigBomb( m_Pos.x-64*100 , m_Pos.y-96*100 );
			pGame->QuakeOn(64);
			break;
		case 60:
			viiEff::Set( EFF_RING , m_Pos.x+64*100 , m_Pos.y-96*100 , argset(300) );
			viiMus::PlaySound(enSoundExplosionMiddle);
			EffectMakeBigBomb( m_Pos.x+64*100 , m_Pos.y-96*100 );
			break;
		case 70:
			ScorePlus( enScore ,0 , -128*100);
			m_bDead = gxTrue;
			break;
		}
	}

	m_sCrashTimer ++;

	if( m_bClear )
	{
		CCockpit::GetInstance()->SetDanmakuIndex( enDanmakuIndex , gxFalse );
		CSoldierBase *p = NULL;
		p = new CSoldierBase( );
		p->SetPosition( 0 , m_Pos.x  , m_Pos.y-96*100 , NULL);
		p->SetForce(enForceEnemy);
		p->Command( enSoldierCommandFuttobi );

		SetActionSeq( enActionSeqEnd );
	}

}

void CEneH0204Grazzi::Draw()
{

//	viiDraw::Sprdmg( m_HitKurai.is_damage() , &SprCEneH0204Grazzi[ 0 ] , m_Pos.x , m_Pos.y , PRIO_ENEMY , m_sAtribute|m_sAtributeDir , ARGB_DFLT );
	m_pEnemyAnime->Draw( m_bDamage , m_Pos.x , m_Pos.y , ( m_sDirection == DIR_LEFT )? gxTrue : gxFalse , &SprCEneH0204Grazzi[0] );

}

