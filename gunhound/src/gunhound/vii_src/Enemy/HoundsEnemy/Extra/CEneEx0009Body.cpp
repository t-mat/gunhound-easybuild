//--------------------------------------------------------------------------------
//
// Stage00:ボディパネル
//
//--------------------------------------------------------------------------------

#include "../HoundsEnemy.h"
#include "../../../Effect/EffectBase.h"
//#include "../EnemyTbl/CEneEx0009BodyTBL.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -128,
	enKuraiTop   = -48,
	enKuraiRight =  128,
	enKuraiBottom=  16,

	enScore = 300,
};

gxSprite SprCEneEx0009Body[]=
{
	{ enTexCEneEx0009Body , 0,0,512,256,256,128 },
};

CEneEx0009Body::CEneEx0009Body( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_HitKurai.set_hp( enHitPointShipPanel );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_RIGHT;
	m_sAtribute = ATR_DFLT;
	m_sRotation = 180;

	m_bDisp = gxTrue;

	m_bAtari  = gxFalse;
	m_bDamage = gxFalse;

}


CEneEx0009Body::~CEneEx0009Body()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneEx0009Body::SeqMain()
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
	if( m_bAtari )
	{
		m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );
	}

}


void CEneEx0009Body::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	m_sRotation = get_valken_rot( m_Pos.x , m_Pos.y );

	Sint32 sTime = (viiSub::GameCounter()*8)%360;

	m_sRotation = 180 + 120*viiMath::Cos100( sTime ) /360;

	if( viiSub::GameCounter()%4 == 0)
	{
//		new CEne0001EneCrasher( m_Pos.x , m_Pos.y , m_sRotation , 400 );
	}


}


void CEneEx0009Body::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CEneEx0009Body::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	if( !m_bDead )
	{
		viiMus::PlaySound(enSoundExplosionMiddle);
		viiMus::PlaySound(enSoundExplosionLarge);
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

//		new CEffectFinalExplosion( m_Pos.x , m_Pos.y ,8 );

		m_bDead = gxTrue;
		m_bDisp = gxFalse;

		CEffectDmgprt *p;

		p = new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, 180+15 , 800 );
		p=new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, 180+65 , 600 );
		p=new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, -60    , 800 );
		p=new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, -20    , 700 );

		ScorePlus( enScore );
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

void CEneEx0009Body::Draw()
{
	if( !m_bDisp ) return;

	viiDraw::Sprdmg( m_bDamage , &SprCEneEx0009Body[ 0 ] , m_Pos.x , m_Pos.y,PRIO_PLAYER-1 , m_sAtribute|m_sAtributeDir , ARGB_DFLT ,1.0f );

}

