//--------------------------------------------------------------------------------
//
// Stage00:ノズル
//
//--------------------------------------------------------------------------------

#include "../HoundsEnemy.h"
#include "../../../Effect/EffectBase.h"

enum {
	enScrollOutWidth  = 256*100,
	enScrollOutHeight = 256*100,

	enKuraiLeft  = 0,
	enKuraiTop   = -32,
	enKuraiRight = 128,
	enKuraiBottom= 32,

	enScore = 3000,
};

gxSprite SprCEneEx0007Nozzle[]=
{
	{enTexCEneEx0007Nozzle , 0,0,256,128,0,64 },
};


CEneEx0007Nozzle::CEneEx0007Nozzle( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_Add.x = m_Add.y = 0;

	m_HitKurai.set_hp( enHitPointHard*2 );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_RIGHT;
	m_sAtribute = ATR_DFLT;
	m_sRotation = 0;

	m_bDamage = gxFalse;

}


CEneEx0007Nozzle::~CEneEx0007Nozzle()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneEx0007Nozzle::SeqMain()
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
		return;
	}

	LogicAI();

	m_bDamage = m_HitKurai.is_damage();

	//くらい判定登録
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

}


void CEneEx0007Nozzle::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

}


void CEneEx0007Nozzle::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CEneEx0007Nozzle::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	if( !m_bDead )
	{
		viiMus::PlaySound(enSoundExplosionMiddle);
		ScorePlus( enScore );
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
		viiEff::Set(EFF_BIGBOMB , m_Pos.x, m_Pos.y , argset(2) );
		viiEff::Set(EFF_BIGBOMB , m_Pos.x, m_Pos.y , argset(3) );

//		new CEffectFinalExplosion( m_Pos.x , m_Pos.y ,8 );
		m_bDead = gxTrue;
	}


	//-------------------------------------------
	//煙
	//-------------------------------------------
	CEffectBlackFire *pBlack;
	Sint32 ax,ay;
	ax = m_Pos.x+(-32+viiSub::Rand()%80)*100;
	ay = m_Pos.y+(-64+viiSub::Rand()%64)*100;

	pBlack = new CEffectBlackFire( ax , ay , NULL );
	pBlack->m_Pos.z = PRIO_EFFECT;

	//--------------------------------------------

	if( m_bClear )
	{
		SetActionSeq( enActionSeqEnd );
	}
}

void CEneEx0007Nozzle::Draw()
{

	if( m_bDead ) return;

	viiDraw::Sprdmg( m_bDamage , &SprCEneEx0007Nozzle[ 0 ] , m_Pos.x , m_Pos.y , PRIO_ENEMY-1 , ATR_DFLT , ARGB_DFLT );

}

