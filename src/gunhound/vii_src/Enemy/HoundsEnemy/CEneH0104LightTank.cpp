//--------------------------------------------------------------------------------
//
// Stage00:軽戦車
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"

#include "../../effect/EffectBase.h"
//#include "EnemyTbl/CEneH0104LightTankTbl.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -30,
	enKuraiTop   = -34,
	enKuraiRight =  30,
	enKuraiBottom=  0,

	enScore = 100,
};

gxSprite SprCEneH0104LightTank[]={
	{enTexCEneH0104LightTank,0,208,96,48,48,40},
};

CEneH0104LightTank::CEneH0104LightTank( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_HitKurai.set_hp( enAtkPointMissile );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_LEFT;
	m_sAtribute = ATR_DFLT;

	m_bDamage = gxFalse;
	m_sLogicTimer = 0;


}


CEneH0104LightTank::~CEneH0104LightTank()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------


}


void CEneH0104LightTank::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( m_HitKurai.is_dead() )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

/*
	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}
*/

	LogicAI();

	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	m_bDamage = m_HitKurai.is_damage();

	//くらい判定登録
	m_HitKurai.SetHantei( ID_ENEMY_DEF , &m_Pos );

}


void CEneH0104LightTank::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	if( m_sTimer%180 == 42)
	{
		m_sLogicTimer = 1;
	}
	switch( m_sLogicTimer ) {
	case 5:
	case 18:
	case 30:
		Atack();
		break;
	case 35:
		m_sLogicTimer = 0;
		break;
	default:
		break;
	}

	if( m_sLogicTimer > 0 )
	{
		viiEff::Set(EFF_MAZZLE,m_Pos.x-900 , m_Pos.y-3000,NULL);
		m_sLogicTimer ++;
	}


}


void CEneH0104LightTank::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

	Sint32 sRot = 270;

	new CEffBombSmoke ( m_Pos.x , m_Pos.y-2600 , 180+20 , 280 );
	new CEffEneTwinGun( m_Pos.x , m_Pos.y-2600 , 180+20 , 680 );


}


void CEneH0104LightTank::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	viiMus::PlaySound(enSoundExplosionMiddle);

	ScorePlus( enScore );

	EffectMakeBigBomb(m_Pos.x , m_Pos.y);

	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

	SetActionSeq( enActionSeqEnd );

}

void CEneH0104LightTank::Draw()
{

	viiDraw::Sprdmg( m_HitKurai.is_damage() , &SprCEneH0104LightTank[ 0 ] , m_Pos.x , m_Pos.y,PRIO_ENEMY , m_sAtribute|m_sAtributeDir , ARGB_DFLT );

}

