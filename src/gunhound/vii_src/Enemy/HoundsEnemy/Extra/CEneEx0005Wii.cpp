//--------------------------------------------------------------------------------
//
// Stage00:Wii
//
//--------------------------------------------------------------------------------

#include "../HoundsEnemy.h"
#include "../../../Effect/EffectBase.h"
//#include "../EnemyTbl/CEneEx0005WiiTBL.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -64,
	enKuraiTop   = -64,
	enKuraiRight =  48,
	enKuraiBottom=  48,

	enScore = 300,
};


gxSprite SprCEneEx0005Wii[]=
{
	{enTexPageEnemyGear01 , 32,112,80,64,40,32 	},	//コア
	{ enTexCEneEx0003ShotRight , 144,64,112,96,80,48},
};


CEneEx0005Wii::CEneEx0005Wii( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_HitKurai.set_hp( enHitPointShipPolymerCannonCore );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_RIGHT;
	m_sAtribute = ATR_DFLT;
	m_sRotation = 180;

	m_bAtari = gxFalse;

	m_bDamage = gxFalse;
	m_sDamageWait = 0;
	m_bhakkyouMode = gxFalse;

}


CEneEx0005Wii::~CEneEx0005Wii()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneEx0005Wii::SeqMain()
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
	if( m_bDamage )
	{
		m_sDamageWait = 10;
	}
	if( m_sDamageWait > 0)
	{
		m_sDamageWait --;
	}
	//くらい判定登録
	m_HitKurai.ax1 = -140;
	m_HitKurai.ay1 = -64;
	m_HitKurai.ax2 = 140;
	m_HitKurai.ay2 = 64;

	m_Pos.x = 555000;
	m_Pos.y = 76000;
	if( m_bAtari )
	{
		m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );
	}

}


void CEneEx0005Wii::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	SetRader( enTypeMarkerGroundObject );
//	DirectionAdjust();
//	m_sRotation = get_valken_rot( m_Pos.x , m_Pos.y );
//	Sint32 sTime = (viiSub::GameCounter()*8)%360;
//	m_sRotation = 180 + 120*viiMath::Cos100( sTime ) /360;

	if( m_bhakkyouMode )
	{
		if( (viiSub::GameCounter())%6)
		{
			Sint32 ax = 5650*100 + (-50+viiSub::Rand()%100)*800;
			Sint32 ay = 770*100  + (-50+viiSub::Rand()%100)*100;
			new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , ax,ay , PRIO_BG2-1 , -5 ,180 );
		}

		CEffectBlackFire *pBlack;
		Sint32 ax,ay;
		ax = m_Pos.x+(-32+viiSub::Rand()%80)*100;
		ay = m_Pos.y+(-64+viiSub::Rand()%64)*100;

		pBlack = new CEffectBlackFire( ax , ay , NULL );
		pBlack->m_Pos.z = PRIO_EFFECT;
	}
}


void CEneEx0005Wii::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CEneEx0005Wii::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	m_bDamage = gxFalse;

	if( m_sDamageWait > 0)
	{
		m_sDamageWait --;
	}

	if( !m_bDead )
	{
		viiMus::PlaySound( enSoundCrashB );
		viiMus::PlaySound( enSoundExplosionMaximum );
		ScorePlus( enScore );
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

		new CEffectFinalExplosion( m_Pos.x , m_Pos.y ,8 );
		m_bDead = gxTrue;
	}

	if( viiSub::Rand()%32 == 0 )
	{
		viiEff::Set( EFF_BREAK_THUNDER , m_Pos.x+(-48+viiSub::Rand()%96)*100 , m_Pos.y+(-64+viiSub::Rand()%128)*100 );
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
		viiMus::PlaySound(enSoundExplosionLarge);
		SetActionSeq( enActionSeqEnd );
	}

}

void CEneEx0005Wii::SetHakkyouMode()
{
	m_bDead = gxFalse;
	m_HitKurai.set_hp( enHitPointShipPolymerCannonCore*2 );
	SetActionSeq( enActionSeqMain );
}


void CEneEx0005Wii::Draw()
{

	viiDraw::Sprdmg( m_bDamage , &SprCEneEx0005Wii[ 0 ] , m_Pos.x , m_Pos.y,PRIO_ENEMY , ATR_DFLT , ARGB_DFLT ,1.0f , m_sRotation);

	viiDbg::printf(100,50,"WII POINT(%d)",m_HitKurai.GetTotalDamage() );
}

