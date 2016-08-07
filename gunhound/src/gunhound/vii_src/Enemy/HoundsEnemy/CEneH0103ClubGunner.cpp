//--------------------------------------------------------------------------------
//
// Stage01:クラブガンナー
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "EnemyTbl/CEneH0103ClubGunnerTbl.h"
#include "../../Effect/EffectBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enOffsetHeight = 48,
	enKuraiLeft  = -64,
	enKuraiTop   = -128+enOffsetHeight,
	enKuraiRight =  64,
	enKuraiBottom=  -128+34+enOffsetHeight,

	enScore = 3000,
	enSpeed = 80,
};

enum {
 enMaeL   = 0,
 enUsiroL = 1,
 enMaeR   = 2,
 enUsiroR = 3,
 enLegMax = 4,

 enHigh = 0,
 enMid  = 1,
 enLow  = 2,
 enLegPosMax = 3,

 enOpenLeg = 25,

};

enum {
	enLogicNone = 0,
	//enLogicTransport,
	enLogicBattle,
};

static Sint32 sLegTbl[enLegMax][enLegPosMax] = {
	{ enMAEASIUE      , enMAEASINAKA      , enMAEASISITA      },
	{ enUSIROASIUE    , enUSIROASINAKA    , enUSIROASISITA    },
	{ enMAEASIOKUUE   , enMAEASIOKUNAKA   , enMAEASIOKUSITA   },
	{ enUSIROASIOKUUE , enUSIROASIOKUNAKA , enUSIROASIOKUSITA },
};


CEneH0103ClubGunner::CEneH0103ClubGunner( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y+1200;
	m_Pos.z = PRIO_ENEMY;

	m_HitKurai.set_hp( enAtkPointBomb*2 );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_RIGHT;
	m_sAtribute = ATR_DFLT;

	m_sLogicSeq     = enLogicBattle;
	m_sHigherSpeed  = 30;
	m_bDoubleDamage = gxFalse;

	m_sWalkCnt     = 0;
	m_sMotionTimer = viiSub::Rand()%80;
	m_sControl     = 0;

	m_sHeight = 0;
	m_bDamage = gxFalse;

	m_pTgt    = NULL;
	m_sOffset = 0;

	m_pEnemyAnime = new CEnemyAnime();

	for(Sint32 ii=0; ii<enMax ;ii++)
	{
		m_pEnemyAnime->SetParts( ii , enMax , m_sParentCEneH0103ClubGunner[ii] , sPosCEneH0103ClubGunner[ii] );
	}

	m_pEnemyAnime->PosCalc();

	Float32 dx,dy;

	//------------------------------------------------------------------------

	dx = getLeg( enMaeL , enMid )->dx - getLeg( enMaeL , enHigh )->dx*1.0f;
	dy = getLeg( enMaeL , enMid )->dy - getLeg( enMaeL , enHigh )->dy*1.0f;

	getLegInfo( enMaeL , enMid )->dist = (Sint32)viiMath::Dist(dx,dy);
	getLegInfo( enMaeL , enMid )->rot  = (Sint32)viiMath::Atan2d( dy , dx );

	//------------------------------------------------------------------------

	dx = getLeg( enMaeL , enLow )->dx - getLeg( enMaeL , enMid )->dx*1.0f;
	dy = getLeg( enMaeL , enLow )->dy - getLeg( enMaeL , enMid )->dy*1.0f;

	getLegInfo( enMaeL , enLow )->dist = (Sint32)viiMath::Dist(dx,dy);
	getLegInfo( enMaeL , enLow )->rot  = (Sint32)viiMath::Atan2d( dy , dx );

	//------------------------------------------------------------------------

	dx = getLeg( enUsiroL , enMid )->dx - getLeg( enUsiroL , enHigh )->dx*1.0f;
	dy = getLeg( enUsiroL , enMid )->dy - getLeg( enUsiroL , enHigh )->dy*1.0f;

	getLegInfo( enUsiroL , enMid )->dist = (Sint32)viiMath::Dist(dx,dy);
	getLegInfo( enUsiroL , enMid )->rot  = (Sint32)viiMath::Atan2d( dy , dx );

	//------------------------------------------------------------------------

	dx = getLeg( enUsiroL , enLow )->dx - getLeg( enUsiroL , enMid )->dx*1.0f;
	dy = getLeg( enUsiroL , enLow )->dy - getLeg( enUsiroL , enMid )->dy*1.0f;

	getLegInfo( enUsiroL , enLow )->dist = (Sint32)viiMath::Dist(dx,dy);
	getLegInfo( enUsiroL , enLow )->rot  = (Sint32)viiMath::Atan2d( dy , dx );

	//------------------------------------------------------------------------
	//------------------------------------------------------------------------

	dx = getLeg( enMaeR , enMid )->dx - getLeg( enMaeR , enHigh )->dx*1.0f;
	dy = getLeg( enMaeR , enMid )->dy - getLeg( enMaeR , enHigh )->dy*1.0f;

	getLegInfo( enMaeR , enMid )->dist = (Sint32)viiMath::Dist(dx,dy);
	getLegInfo( enMaeR , enMid )->rot  = (Sint32)viiMath::Atan2d( dy , dx );

	//------------------------------------------------------------------------

	dx = getLeg( enMaeR , enLow )->dx - getLeg( enMaeR , enMid )->dx*1.0f;
	dy = getLeg( enMaeR , enLow )->dy - getLeg( enMaeR , enMid )->dy*1.0f;

	getLegInfo( enMaeR , enLow )->dist = (Sint32)viiMath::Dist(dx,dy);
	getLegInfo( enMaeR , enLow )->rot  = (Sint32)viiMath::Atan2d( dy , dx );

	//------------------------------------------------------------------------

	dx = getLeg( enUsiroR , enMid )->dx - getLeg( enUsiroR , enHigh )->dx*1.0f;
	dy = getLeg( enUsiroR , enMid )->dy - getLeg( enUsiroR , enHigh )->dy*1.0f;

	getLegInfo( enUsiroR , enMid )->dist = (Sint32)viiMath::Dist(dx,dy);
	getLegInfo( enUsiroR , enMid )->rot  = (Sint32)viiMath::Atan2d( dy , dx );

	//------------------------------------------------------------------------

	dx = getLeg( enUsiroR , enLow )->dx - getLeg( enUsiroR , enMid )->dx*1.0f;
	dy = getLeg( enUsiroR , enLow )->dy - getLeg( enUsiroR , enMid )->dy*1.0f;

	getLegInfo( enUsiroR , enLow )->dist = (Sint32)viiMath::Dist(dx,dy);
	getLegInfo( enUsiroR , enLow )->rot  = (Sint32)viiMath::Atan2d( dy , dx );

}


CEneH0103ClubGunner::~CEneH0103ClubGunner()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

	delete m_pEnemyAnime;

}

void CEneH0103ClubGunner::SeqInit()
{

	SetActionSeq( enActionSeqMain );

}


void CEneH0103ClubGunner::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( m_HitKurai.is_dead() )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	m_pEnemyAnime->PosCalc();

	LogicAI();

	SetRader( enTypeMarkerGroundPartner );
	DirectionAdjust();

	m_bDamage = m_HitKurai.is_damage();

	m_HitKurai.SetHantei(ID_VERSIS_DEF , &m_Pos );

	if( !pGame->GetMarineLine() )	return;

	new CEffWaterStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , pGame->GetMarineLine(), 180 , 180 );
	new CEffWaterStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , pGame->GetMarineLine(), 180 , 180 );
	new CEffWaterStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , pGame->GetMarineLine(), 180 , 180 );

}


void CEneH0103ClubGunner::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	Sint32 tx = GetTargetPlayer()->x;

	if( m_pTgt )
	{
		tx = m_pTgt->x + m_sOffset;
	}

	m_sControl = 0;

	if (tx < m_Pos.x -18600 )
	{
		m_sControl = JOY_L;
	}
	else if (tx > m_Pos.x -18000 )
	{
		m_sControl = JOY_R;
	}

#if 0
	static Sint32 m_sAtackPattern = 0;

	if( m_sTimer%64 == 0)
	{
		if( m_sAtackPattern == 0)
		{
			//ロケットポッド
			new CEne0001EneMissile( m_Pos.x , m_Pos.y-8400 , -4+viiSub::Rand()%8+180 , 220 );
			new CEne0001EneMissile( m_Pos.x , m_Pos.y-8400 , -4+viiSub::Rand()%8+180 , 220 );
			new CEne0001EneMissile( m_Pos.x , m_Pos.y-8400 , -4+viiSub::Rand()%8+180 , 220 );
		}
		else if( m_sAtackPattern == 1)
		{
			//主砲
			new CEne0001EneMissile( m_Pos.x-8000 , m_Pos.y-15200 , 4+180 , 360 );
		}
		else if( m_sAtackPattern == 2)
		{
			//主砲
/*
			new CEne0001EneMissile( m_Pos.x , m_Pos.y-11200 , 180+15*1 , 160 );
			new CEne0001EneMissile( m_Pos.x , m_Pos.y-11200 , 180+15*2 , 160 );
			new CEne0001EneMissile( m_Pos.x , m_Pos.y-11200 , 180+15*3 , 160 );
			new CEne0001EneMissile( m_Pos.x , m_Pos.y-11200 , 180+15*4 , 160 );
			new CEne0001EneMissile( m_Pos.x , m_Pos.y-11200 , 180+15*5 , 160 );
			new CEne0001EneMissile( m_Pos.x , m_Pos.y-11200 , 180+15*6 , 160 );
			new CEne0001EneMissile( m_Pos.x , m_Pos.y-11200 , 180+15*7 , 160 );
			new CEne0001EneMissile( m_Pos.x , m_Pos.y-11200 , 180+15*8 , 160 );
*/
		}

		m_sAtackPattern ++;
		m_sAtackPattern = m_sAtackPattern%3;
	}
#endif


	switch( m_sLogicSeq ) {
	case enLogicNone:
		break;

//	case enLogicTransport:
//		//LogicTransport();
		break;

	case enLogicBattle:
		LogicBattle();
		break;
	}

}


void CEneH0103ClubGunner::LogicBattle()
{
	Sint32 sControl = Joy[0].psh;

	Sint32 sWalkTbl1[][3]={
		{-40,0,15},
		{-30,-20,0},
		{-20 ,-10,0},
		{ 0,0,0},
		{ 10, 10,0},
		{ 10 ,25,0},
	};

	Sint32 sWalkTbl2[][3]={
		{ 15 ,25,0},
		{ 15 ,45,25},
		{ 5 ,15,0},
		{-15 ,10,0},
		{-20 ,15,0},
		{-5  ,25,0},
	};

	Sint32 now = 0,tgt = 0;

	m_sMotionTimer += 8;

	if(m_sControl&JOY_L)
	{
		m_Pos.x -= enSpeed;
	}
	else if(m_sControl&JOY_R)
	{
//		m_sMotionTimer += 8*-1;
		m_Pos.x += enSpeed;
	}

	if( m_sMotionTimer >= 100 )
	{
		m_sWalkCnt ++;
		m_sWalkCnt = m_sWalkCnt%6;
		m_sMotionTimer = 0;
	}
	else if( m_sMotionTimer <= 0 )
	{
		m_sWalkCnt --;
		m_sWalkCnt = (6+m_sWalkCnt)%6;
		m_sMotionTimer = 100;
	}

	now = m_sWalkCnt;
	tgt = (m_sWalkCnt+1)%6;

	getLeg( enMaeL , enHigh )->dr   = sWalkTbl1[now][0] + m_sMotionTimer*(sWalkTbl1[tgt][0] - sWalkTbl1[now][0])/100;
	getLeg( enMaeL , enMid  )->dr   = sWalkTbl1[now][1] + m_sMotionTimer*(sWalkTbl1[tgt][1] - sWalkTbl1[now][1])/100;
	getLeg( enMaeL , enLow  )->dr   = sWalkTbl1[now][2] + m_sMotionTimer*(sWalkTbl1[tgt][2] - sWalkTbl1[now][2])/100;

	getLeg( enUsiroL , enHigh )->dr   = sWalkTbl2[now][0] + m_sMotionTimer*(sWalkTbl2[tgt][0] - sWalkTbl2[now][0])/100;
	getLeg( enUsiroL , enMid  )->dr   = sWalkTbl2[now][1] + m_sMotionTimer*(sWalkTbl2[tgt][1] - sWalkTbl2[now][1])/100;
	getLeg( enUsiroL , enLow  )->dr   = sWalkTbl2[now][2] + m_sMotionTimer*(sWalkTbl2[tgt][2] - sWalkTbl2[now][2])/100;

	Sint32 m_sWalkCntOku = (m_sWalkCnt+3)%6;
	now = m_sWalkCntOku;
	tgt = (m_sWalkCntOku+1)%6;

	getLeg( enMaeR , enHigh )->dr   = sWalkTbl1[now][0] + m_sMotionTimer*(sWalkTbl1[tgt][0] - sWalkTbl1[now][0])/100;
	getLeg( enMaeR , enMid  )->dr   = sWalkTbl1[now][1] + m_sMotionTimer*(sWalkTbl1[tgt][1] - sWalkTbl1[now][1])/100;
	getLeg( enMaeR , enLow  )->dr   = sWalkTbl1[now][2] + m_sMotionTimer*(sWalkTbl1[tgt][2] - sWalkTbl1[now][2])/100;

	getLeg( enUsiroR , enHigh )->dr   = sWalkTbl2[now][0] + m_sMotionTimer*(sWalkTbl2[tgt][0] - sWalkTbl2[now][0])/100;
	getLeg( enUsiroR , enMid  )->dr   = sWalkTbl2[now][1] + m_sMotionTimer*(sWalkTbl2[tgt][1] - sWalkTbl2[now][1])/100;
	getLeg( enUsiroR , enLow  )->dr   = sWalkTbl2[now][2] + m_sMotionTimer*(sWalkTbl2[tgt][2] - sWalkTbl2[now][2])/100;

	LegCalc();

}


void CEneH0103ClubGunner::LegCalc()
{
	//---------------------------------------------
	//脚の動き制御
	//---------------------------------------------

	Sint32 sRot;
	Sint32 sLeg,sSrc,sDst;

	//前脚（左）

	sSrc = enHigh;
	sDst = enMid;
	sLeg = enMaeL;
	sRot = getLeg(sLeg,sSrc )->dr+90;
	getLeg(sLeg,sDst )->dx  = getLeg(sLeg,sSrc )->ox + ( viiMath::Cos100( sRot )*getLegInfo( sLeg , sDst )->dist )/100;
	getLeg(sLeg,sDst )->dy  = getLeg(sLeg,sSrc )->oy + ( viiMath::Sin100( sRot )*getLegInfo( sLeg , sDst )->dist )/100;
//	getLeg(sLeg,sDst )->dr  = 0;

	sSrc = enMid;
	sDst = enLow;
	sLeg = enMaeL;
	sRot = getLeg(sLeg,sSrc )->dr+90;
	getLeg(sLeg,sDst )->dx  = getLeg(sLeg,sSrc )->dx + ( viiMath::Cos100( sRot )*getLegInfo( sLeg , sDst )->dist )/100;
	getLeg(sLeg,sDst )->dy  = getLeg(sLeg,sSrc )->dy + ( viiMath::Sin100( sRot )*getLegInfo( sLeg , sDst )->dist )/100;
//	getLeg(sLeg,sDst )->dr  = 0;


	//前脚（右）

	sSrc = enHigh;
	sDst = enMid;
	sLeg = enMaeR;
	sRot = getLeg(sLeg,sSrc )->dr+90;
	getLeg(sLeg,sDst )->dx  = getLeg(sLeg,sSrc )->ox + ( viiMath::Cos100( sRot )*getLegInfo( sLeg , sDst )->dist )/100;
	getLeg(sLeg,sDst )->dy  = getLeg(sLeg,sSrc )->oy + ( viiMath::Sin100( sRot )*getLegInfo( sLeg , sDst )->dist )/100;
//	getLeg(sLeg,sDst )->dr  = 0;

	sSrc = enMid;
	sDst = enLow;
	sLeg = enMaeR;
	sRot = getLeg(sLeg,sSrc )->dr+90;
	getLeg(sLeg,sDst )->dx  = getLeg(sLeg,sSrc )->dx + ( viiMath::Cos100( sRot )*getLegInfo( sLeg , sDst )->dist )/100;
	getLeg(sLeg,sDst )->dy  = getLeg(sLeg,sSrc )->dy + ( viiMath::Sin100( sRot )*getLegInfo( sLeg , sDst )->dist )/100;
//	getLeg(sLeg,sDst )->dr  = 0;


	m_sHeight = getLeg(sLeg,sDst )->dx*15;

	//後脚（左）

	sSrc = enHigh;
	sDst = enMid;
	sLeg = enUsiroL;
	sRot = getLeg(sLeg,sSrc )->dr+90;
	getLeg(sLeg,sDst )->dx  = getLeg(sLeg,sSrc )->ox + ( viiMath::Cos100( sRot )*getLegInfo( sLeg , sDst )->dist )/100;
	getLeg(sLeg,sDst )->dy  = getLeg(sLeg,sSrc )->oy + ( viiMath::Sin100( sRot )*getLegInfo( sLeg , sDst )->dist )/100;
//	getLeg(sLeg,sDst )->dr  = 0;

	sSrc = enMid;
	sDst = enLow;
	sLeg = enUsiroL;
	sRot = getLeg(sLeg,sSrc )->dr+90;
	getLeg(sLeg,sDst )->dx  = getLeg(sLeg,sSrc )->dx + ( viiMath::Cos100( sRot )*getLegInfo( sLeg , sDst )->dist )/100;
	getLeg(sLeg,sDst )->dy  = getLeg(sLeg,sSrc )->dy + ( viiMath::Sin100( sRot )*getLegInfo( sLeg , sDst )->dist )/100;
//	getLeg(sLeg,sDst )->dr  = 0;


	//後脚（右）

	sSrc = enHigh;
	sDst = enMid;
	sLeg = enUsiroR;
	sRot = getLeg(sLeg,sSrc )->dr+90;
	getLeg(sLeg,sDst )->dx  = getLeg(sLeg,sSrc )->ox + ( viiMath::Cos100( sRot )*getLegInfo( sLeg , sDst )->dist )/100;
	getLeg(sLeg,sDst )->dy  = getLeg(sLeg,sSrc )->oy + ( viiMath::Sin100( sRot )*getLegInfo( sLeg , sDst )->dist )/100;
//	getLeg(sLeg,sDst )->dr  = 0;

	sSrc = enMid;
	sDst = enLow;
	sLeg = enUsiroR;
	sRot = getLeg(sLeg,sSrc )->dr+90;
	getLeg(sLeg,sDst )->dx  = getLeg(sLeg,sSrc )->dx + ( viiMath::Cos100( sRot )*getLegInfo( sLeg , sDst )->dist )/100;
	getLeg(sLeg,sDst )->dy  = getLeg(sLeg,sSrc )->dy + ( viiMath::Sin100( sRot )*getLegInfo( sLeg , sDst )->dist )/100;
//	getLeg(sLeg,sDst )->dr  = 0;

}


StPartsInfo* CEneH0103ClubGunner::getLeg( Sint32 sType , Sint32 sPos )
{
	//脚アドレスを得る

	return m_pEnemyAnime->GetParts( sLegTbl[sType][sPos] );

}


StPatrsOffset* CEneH0103ClubGunner::getLegInfo( Sint32 sType , Sint32 sPos )
{
	//脚情報を得る

	return &stLegs[ sType*3 + sPos ];

}


void CEneH0103ClubGunner::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CEneH0103ClubGunner::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

//	if( !m_bDead )
	{
		m_bDead = gxTrue;
		viiMus::PlaySound(enSoundExplosionMiddle);
		ScorePlus( enScore );
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
	}

//	if( m_bClear )
	{
		SetActionSeq( enActionSeqEnd );
	}

}

void CEneH0103ClubGunner::Draw()
{

	m_pEnemyAnime->SetPrio( m_Pos.z );
	m_pEnemyAnime->Draw( m_bDamage , m_Pos.x , m_sHeight+m_Pos.y+enOffsetHeight*100 , ( m_sDirection == DIR_LEFT )? gxTrue : gxFalse , &SprCEneH0103ClubGunner[0] );

}

