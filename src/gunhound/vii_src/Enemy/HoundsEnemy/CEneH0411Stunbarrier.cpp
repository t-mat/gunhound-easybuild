//--------------------------------------------------------------------------------
//
// Stage00:スタンバリヤ
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "EnemyTbl/CEneH0411StunbarrierTBL.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -30,
	enKuraiTop   = -16,
	enKuraiRight =  30,
	enKuraiBottom=  0,

	enScore = 300,
};

/*
gxSprite SprCEneH0411Stunbarrier[]=
{
	{TPG_LEYNOS01,0,192,64,64,32,64},//ボルゾイ
};
*/

CEneH0411Stunbarrier::CEneH0411Stunbarrier( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_HitAtari.set_hp( enAtkPointBazooka );
	m_HitAtari.set_ap( enAtkPointBazooka );

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;

	m_sDirection = DIR_LEFT;
	m_sAtribute = ATR_DFLT;

	m_bDamage = gxFalse;

 	m_pCEneCmnSpark = new CEneCmnSpark( m_Pos.x -40*100, m_Pos.y , m_Pos.x+40*100 , m_Pos.y );
}


CEneH0411Stunbarrier::~CEneH0411Stunbarrier()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

	if(m_pCEneCmnSpark) delete m_pCEneCmnSpark;

}


void CEneH0411Stunbarrier::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

//	if( m_HitAtari.is_dead() )
//	{
//		SetActionSeq( enActionSeqCrash );
//		return;
//	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}

	LogicAI();

	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	if( m_HitAtari.IsImpacted() )
	{
		viiMus::PlaySound(enSoundEneShotBiriBiri);
		m_HitAtari.SetOffImpact();
	}

	//くらい判定登録
	m_bDamage = gxFalse;

	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
	m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );

}


void CEneH0411Stunbarrier::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	m_pCEneCmnSpark->Set( m_Pos.x-40*100 , m_Pos.y-1200 , m_Pos.x+40*100 , m_Pos.y-1200 );
	m_pCEneCmnSpark->SetRotation(0.f);

}


void CEneH0411Stunbarrier::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------


}


void CEneH0411Stunbarrier::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	viiMus::PlaySound(enSoundExplosionMiddle);
	ScorePlus( enScore );
	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

	SetActionSeq( enActionSeqEnd );

}

void CEneH0411Stunbarrier::Draw()
{

	viiDraw::Sprdmg( m_bDamage , &SprCEneH0411Stunbarrier[ enSTUNOBJ1 ] , m_Pos.x-80*100 , m_Pos.y , PRIO_ENEMY , ATR_DFLT , ARGB_DFLT );
	viiDraw::Sprdmg( m_bDamage , &SprCEneH0411Stunbarrier[ enSTUNOBJ2 ] , m_Pos.x+80*100 , m_Pos.y , PRIO_ENEMY , ATR_DFLT , ARGB_DFLT );

	if( m_pCEneCmnSpark )
	{
		m_pCEneCmnSpark->Draw();
		m_pCEneCmnSpark->Draw();
	}

}

