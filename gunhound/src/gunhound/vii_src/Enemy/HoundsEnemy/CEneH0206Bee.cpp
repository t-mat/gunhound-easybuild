//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
#include "HoundsEnemy.h"

//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../Effect/EffectBase.h"

enum {
	enScrollOutWidth  = 512*100,
	enScrollOutHeight = 256*100,

	enKuraiLeft  = -32,
	enKuraiTop   = -16,
	enKuraiRight =  32,
	enKuraiBottom=  16,

	enScore = 300,
};

/*
gxSprite SprCEneH0206Bee[]=
{
	{enTexPageEnemyStage02,0,88,232,96,116,96},//味方重戦車
	{TPG_LEYNOS02,0,88,232,96,116,96},	//味方重戦車
	{TPG_LEYNOS02,0,88,232,96,116,96},
	{TPG_LEYNOS02, 0,104,128,56,64,56},
};
*/


gxSprite SprCEneH0206Bee[]=
{
	{ enTexPageEnemyStage02,416,176   ,96,32,24,16},
	{ enTexPageEnemyStage02,416,176+32,96,32,24,16},
};


CEneH0206Bee::CEneH0206Bee( Sint32 x, Sint32 y ,Sint32 sType )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_ENEMY;

	m_Add.x = 0;
	m_Add.y = 0;

	m_bDisp = gxFalse;

	m_HitKurai.set_hp( enDefPointZakoS );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_HitAtari.ax1 = -4;
	m_HitAtari.ay1 = -4;
	m_HitAtari.ax2 =  4;
	m_HitAtari.ay2 =  4;
	m_HitAtari.set_ap( enAtkPointBazooka );

	m_sDirection = DIR_LEFT;

	m_sLogicWait = 0;
	m_sLogicSeq = 0;
	m_fSpeed    = 320;
	m_fRotation = 175.f + (viiSub::Rand()%10)*1.f;

	m_bLeader = gxFalse;

	if( sType )
	{
		m_bLeader = gxTrue;
	}
}


CEneH0206Bee::~CEneH0206Bee()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------


}

void CEneH0206Bee::SeqInit()
{
	//-----------------------------------------------------
	//初期化
	//-----------------------------------------------------

	SetRader( enTypeMarkerFlyingObject );

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}

	m_bDisp = gxTrue;

	SetActionSeq( enActionSeqMain );

}


void CEneH0206Bee::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( m_HitKurai.is_dead() || m_HitAtari.IsImpacted() )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( pGame->GetMarineLine() && m_Pos.y >= pGame->GetMarineLine() )
	{
		MakeSplash( m_Pos.x , m_Pos.y );
		viiMus::PlaySound( enSoundSplashM );

		SetActionSeq( enActionSeqEnd );
		return;
	}


	LogicAI();

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	DirectionAdjust();

	SetRader( enTypeMarkerFlyingObject );

	m_bDamage = m_HitKurai.is_damage();
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
	m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );

}


void CEneH0206Bee::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	switch( m_sLogicSeq ){
	case 0:
		m_sLogicSeq = 10;
		break;

	case 10:
		if( m_fSpeed < 40.f ) m_fSpeed += 0.01f;
		m_Add.x = (viiMath::Cos100(m_fRotation)*m_fSpeed)/100;
		m_Add.y = (viiMath::Sin100(m_fRotation)*m_fSpeed)/100;

		if( !IsScrollOut( 512 , enScrollOutHeight ) )
		{
			m_fRotation -= 0.2f;
		}
		break;

	case 20:
		break;
	}

	m_sLogicWait ++;

}


void CEneH0206Bee::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

	Sint32 r = get_valken_rot( m_Pos.x , m_Pos.y);

	new CEne0001EneBullet( m_Pos.x , m_Pos.y , r    , 220 ,1);
	new CEne0001EneBullet( m_Pos.x , m_Pos.y , r-15 , 220 ,1);

}


void CEneH0206Bee::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	if( m_bLeader )
	{
		EffectMakeBigBomb( m_Pos.x , m_Pos.y );
	}

/*
	m_bDamage = m_HitKurai.is_damage();
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

	if( pGame->GetMarineLine() )
	{
		if( !m_bDead )
		{
			m_bDead = gxTrue;
			viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
			viiMus::PlaySound(enSoundExplosionMiddle);
			ScorePlus( enScore );
		}

		if( m_Pos.y >= pGame->GetMarineLine() )
		{
			MakeSplash( m_Pos.x , m_Pos.y );
			viiMus::PlaySound( enSoundSplashM );
			SetActionSeq( enActionSeqEnd );
			return;
		}
		m_fRotation -= 6.f;
		m_Add.y += 6.f;
		m_Pos.y += m_Add.y;
		return;
	}
*/

	viiMus::PlaySound(enSoundExplosionMiddle);
	ScorePlus( enScore );
	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

	SetActionSeq( enActionSeqEnd );

}


void CEneH0206Bee::Draw()
{
	//-----------------------------------------------------
	//描画
	//-----------------------------------------------------

	if( !m_bDisp ) return;

	if( m_bLeader )
	{
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0206Bee[ 1 ] , m_Pos.x , m_Pos.y , m_Pos.z , ATR_DFLT , ARGB_DFLT , 1.f , m_fRotation );
	}
	else
	{
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0206Bee[ 0 ] , m_Pos.x , m_Pos.y , m_Pos.z , ATR_DFLT , ARGB_DFLT , 1.f , m_fRotation );
	}

}

