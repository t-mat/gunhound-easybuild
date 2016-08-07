//--------------------------------------------------------------------------------
//
// Stage00:ニュートロンポリマー砲(ver.2)
//
//--------------------------------------------------------------------------------

#include "../HoundsEnemy.h"
#include "../../../effect/EffectBase.h"

enum {
	enScrollOutWidth  = 256*100,
	enScrollOutHeight = 256*100,

	enKuraiLeft  = -128,
	enKuraiTop   = -48,
	enKuraiRight = -24,
	enKuraiBottom=  16,

	enScore = 3000,
};

gxSprite SprCEneEx0001Cannon[]=
{
	{enTexCEneEx0001Cannon , 0,0,512,256,396,146 },
};


CEneEx0001Cannon::CEneEx0001Cannon( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;
#ifdef _VII_DEBUG_
	m_HitKurai.set_hp( 1 );
#else
	m_HitKurai.set_hp( enHitPointShipPolymerCannonMain );
#endif

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_RIGHT;
	m_sAtribute  = ATR_DFLT;

	m_bDamage = gxFalse;

	m_sCrashTimer = 0;
	m_pFinalExplosion = NULL;
	m_bDisp = gxTrue;

}


CEneEx0001Cannon::~CEneEx0001Cannon()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneEx0001Cannon::SeqMain()
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


void CEneEx0001Cannon::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

//	Sint32 sTime = viiSub::GameCounter()%120;
//
//	if( sTime ==70 || sTime == 90 || sTime == 110)
//	{
//		Sint32 skaku;
//		m_Pos.rot = 195+5;//180+20;
//
//		skaku = -90;//
//
//		Sint32 ax,ay;
//		ax = m_Pos.x+viiMath::Cos100( skaku )*68;
//		ay = m_Pos.y+viiMath::Sin100( skaku )*68;
//		new CDanmakuRoot( CDanmakuRoot::enDanmakuDonutsShot , ax , ay, m_Pos.rot, 600 );
//
//		m_Pos.rot = 195-5;//180+20;
//		skaku = 120;//
//		ax = m_Pos.x+viiMath::Cos100( skaku )*68;
//		ay = m_Pos.y+viiMath::Sin100( skaku )*68;
//		new CDanmakuRoot( CDanmakuRoot::enDanmakuDonutsShot , ax , ay, m_Pos.rot, 600 );
//	}


}


void CEneEx0001Cannon::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CEneEx0001Cannon::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	if( m_pFinalExplosion == NULL && !m_bDead)
	{
		viiEff::Set(EFF_BIGBOMB , m_Pos.x, m_Pos.y , argset(5));
		viiMus::PlaySound(enSoundExplosionMiddle);
		ScorePlus( enScore );
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

		m_pFinalExplosion = new CEffectFinalExplosionDeluxe( m_Pos.x , m_Pos.y  );
	}
	else
	{
		Sint32 ax,ay,r;
		r = 180+10;

		if( m_sCrashTimer > 200 && m_sCrashTimer < 260 )
		{
			Sint32 sDist = (m_sCrashTimer-200)*5;
			if( (m_sCrashTimer)%8 == 0)
			{
				ax = (viiMath::Cos100(r)*(sDist));
				ay = (viiMath::Sin100(r)*(sDist));
				viiEff::SetBombDeluxe( m_Pos.x+ax , m_Pos.y+ay , 1 , 1+viiSub::Rand()%2 );
			}
		}

		if( m_pFinalExplosion && m_pFinalExplosion->IsFinish() )
		{
			m_bDisp = gxFalse;
		}

		if( m_pFinalExplosion && m_pFinalExplosion->IsDead() )
		{
			m_pFinalExplosion->Clear();
			m_pFinalExplosion = NULL;
			m_bDead = gxTrue;
		}
	}

	m_sCrashTimer ++;

	//-------------------------------------------
	//煙
	//-------------------------------------------
	if( m_sCrashTimer%3 == 0 )
	{
		CEffectBlackFire *pBlack;
		Sint32 ax,ay;
		ax = m_Pos.x+(-32+viiSub::Rand()%80)*100;
		ay = m_Pos.y+(-64+viiSub::Rand()%64)*100;

		pBlack = new CEffectBlackFire( ax , ay , NULL );
		pBlack->m_Pos.z = PRIO_EFFECT;
	}

	//--------------------------------------------

	if( m_bClear )
	{
		SetActionSeq( enActionSeqEnd );
	}

}

void CEneEx0001Cannon::Draw()
{

	if( m_bDead ) return;
	if( !m_bDisp ) return;

	viiDraw::Sprdmg( m_bDamage , &SprCEneEx0001Cannon[ 0 ] , m_Pos.x , m_Pos.y,PRIO_ENEMY , m_sAtribute|m_sAtributeDir , ARGB_DFLT );

}

