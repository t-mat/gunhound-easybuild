//--------------------------------------------------------------------------------
//
// Stage00:砲台（小）
//
//--------------------------------------------------------------------------------

#include "../HoundsEnemy.h"
#include "../../../Effect/EffectBase.h"
//#include "../EnemyTbl/CEneEx0004MissileTBL.h"
#include "../../../Stage/CStageBase.h"

enum {
	enScrollOutWidth  = 128*100,
	enScrollOutHeight = 128*100,

	enKuraiLeft  = -30,
	enKuraiTop   = -34,
	enKuraiRight =  30,
	enKuraiBottom=  0,

	enScore = 500,
};


gxSprite SprCEneEx0004Missile[]=
{
	{enTexPageEnemyGear01 , 0 ,224,48,32 ,16,16	},	//src
	{enTexPageEnemyGear01 , 48,224,32,32 ,0,16	},	//dst

	{enTexPageEnemyGear01 , 80 ,224,48,32 ,16,16	},	//src
};


CEneEx0004Missile::CEneEx0004Missile( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_HitKurai.set_hp( enHitPointShipSmallCannon );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_RIGHT;
	m_sAtribute = ATR_DFLT;
	m_sRotation = 180+45;

	m_sRebound = 100;

	m_Saki.x = 0;
	m_Saki.y = 0;

	m_sAtackWait = 0;

	m_bDamage = gxFalse;

}


CEneEx0004Missile::~CEneEx0004Missile()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneEx0004Missile::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( m_HitKurai.is_dead() )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
//		return;
	}

	LogicAI();

	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	m_bDamage = m_HitKurai.is_damage();
	//くらい判定登録
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

}


void CEneEx0004Missile::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	if( m_sRebound < 100 )
	{
		Sint32 sAdd = (100-m_sRebound)/10;
		if( sAdd == 0 )	sAdd = 1;
		m_sRebound += sAdd;
	}
	if( m_sAtackWait > 0 )
	{
		m_sAtackWait --;
	}

	Sint32 r = get_valken_rot( m_Pos.x , m_Pos.y+3200 );
	Sint32 tgt_r = r;
	r = r/30;
	r = r*30;

	Sint32 dir = viiMath::GetTargetAngle( m_sRotation , r  );

	if( m_sAtackWait == 0 )
	{
		if( dir < 0 ) m_sRotation --;
		if( dir > 0 ) m_sRotation ++;
	}

	m_Saki.x = m_Pos.x+viiMath::Cos100( m_sRotation )*(16 + 8*m_sRebound/100 );
	m_Saki.y = m_Pos.y+viiMath::Sin100( m_sRotation )*(16 + 8*m_sRebound/100 );

	NORMALIZE(tgt_r);
	NORMALIZE(m_sRotation);

	if( gxAbs(tgt_r - m_sRotation ) < 15 && m_sAtackWait == 0)
	{
		m_sAtackWait = 150;
	}

	switch( m_sAtackWait ){
	case 140:
	case 120:
	case 110:
		Atack();
		break;
	}
}


void CEneEx0004Missile::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------
	Sint32 ax,ay;

	m_sRebound = 0;

	ax = m_Pos.x+viiMath::Cos100( m_sRotation )*32;
	ay = m_Pos.y+viiMath::Sin100( m_sRotation )*32;

	new CEffBombSmoke ( ax , ay , m_sRotation , 120 );
	new CEffEneTwinGun( ax , ay , m_sRotation , 240 );

}


void CEneEx0004Missile::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	m_bDamage = gxFalse;
	if( !m_bDead )
	{
		ScorePlus( enScore );
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
		viiMus::PlaySound(enSoundExplosionMiddle);
		m_bDead = gxTrue;
	}

	if( m_sTimer%80 == 0)
	{
		viiEff::Set( EFF_BREAK_THUNDER , m_Pos.x+(-8+viiSub::Rand()%16)*100 , m_Pos.y+(-6+viiSub::Rand()%12)*100 );
	}

//	new CEffFire( m_Pos.x , m_Pos.y , PRIO_ENEMY-1);
	if( m_bClear )
	{
		SetActionSeq( enActionSeqEnd );
	}

}

void CEneEx0004Missile::Draw()
{
	Sint32 yy = -CBaseExtra::GetOverLayerY()*100;

	if( m_bDead )
	{
		viiDraw::Sprdmg( m_bDamage , &SprCEneEx0004Missile[ 2 ] , m_Pos.x  , m_Pos.y+yy  , PRIO_BG3+2 , ATR_DFLT ,ARGB_DFLT , 1.f , m_sRotation );
		return;
	}
	else
	{
		viiDraw::Sprdmg( m_bDamage , &SprCEneEx0004Missile[ 0 ] , m_Pos.x  , m_Pos.y+yy  , PRIO_BG3+2 , ATR_DFLT ,ARGB_DFLT , 1.f , m_sRotation );
	}

	viiDraw::Sprdmg( m_bDamage , &SprCEneEx0004Missile[ 1 ] , m_Saki.x , m_Saki.y+yy , PRIO_BG3+1 , ATR_DFLT ,ARGB_DFLT , 1.f , m_sRotation );

}

