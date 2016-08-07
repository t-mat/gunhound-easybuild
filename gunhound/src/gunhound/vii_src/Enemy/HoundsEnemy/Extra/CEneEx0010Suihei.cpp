//--------------------------------------------------------------------------------
//
// Stage00:水平撃ち戦車
//
//--------------------------------------------------------------------------------

#include "../HoundsEnemy.h"
#include "../../../Effect/EffectBase.h"
//#include "../EnemyTbl/CEneEx0010SuiheiTBL.h"
#include "../../../Stage/CStageBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -30,
	enKuraiTop   = -34,
	enKuraiRight =  30,
	enKuraiBottom=  32,

	enScore = 300,
};

gxSprite SprCEneEx0010Suihei[]=
{
	{ enTexCEneEx0010Suihei , 128,208,128,48,64,40 },
	{ enTexCEneEx0010Suihei , 128,208-48,128,48,64,40 },
};

CEneEx0010Suihei::CEneEx0010Suihei( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y+800;

	m_HitKurai.set_hp( enHitPointHard*2 );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_LEFT;
	m_sAtribute = ATR_DFLT;
	m_sRotation = 180;

	m_bDisp = gxTrue;

	m_bDamage = gxFalse;
	m_sBlackSmokeWait = 0;

	m_sAtackCnt = 0;
	m_sRebound = 0;
}


CEneEx0010Suihei::~CEneEx0010Suihei()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneEx0010Suihei::SeqMain()
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


void CEneEx0010Suihei::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();


	if( m_sRebound > 0 )
	{
		m_sRebound -= 32;
		if( m_sRebound < 0 )
		{
			m_sRebound = 0;
		}
	}


	Sint32 r = get_valken_rot( m_Pos.x , m_Pos.y );

	r = ( 360 + r )%360;

	if( m_sDirection == DIR_LEFT )
	{
		//左向き
		if( gxAbs( r-180 ) <= 20 )
		{
			if( m_sAtackCnt == 0 )
			{
				m_sAtackCnt = 30;
			}
		}
	}
	else
	{
		//右向き
		if( gxAbs( r-0 ) <= 20 )
		{
			if( m_sAtackCnt == 0 )
			{
				m_sAtackCnt = 30;
			}
		}
	}

	if( m_sAtackCnt > 0 )
	{
		m_sAtackCnt --;
		if( m_sAtackCnt == 0 )
		{
			m_sAtackCnt = -60;
		}
		Atack();
	}
	else if( m_sAtackCnt < 0 )
	{
		m_sAtackCnt ++;
	}

}


void CEneEx0010Suihei::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

	if( viiSub::GameCounter()%4 == 0)
	{
		Sint32 ax,ay;
		ax = 6400*m_sDirection;
		ay = m_Pos.y-1200;
		viiEff::Set( EFF_MAZZLE , m_Pos.x+ax , ay , NULL );
		new CEne0001EneBullet( m_Pos.x+ax , ay , viiSub::SetKakudo(m_sDirection,0) , 1024 , 1 );
		m_sRebound = 100;
	}
}


void CEneEx0010Suihei::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	m_bDamage = gxFalse;
	if( !m_bDead )
	{
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
		viiEff::Set(EFF_BIGBOMB , m_Pos.x, m_Pos.y , argset(1) );

		viiMus::PlaySound(enSoundExplosionMiddle);

		ScorePlus( enScore );

		m_bDead = gxTrue;
		m_bDisp = gxFalse;
	}


	//-------------------------------------------
	//煙
	//-------------------------------------------

	if( viiSub::Rand()%128 == 0 )
	{
		m_sBlackSmokeWait = 32;
		viiEff::Set( EFF_BREAK_THUNDER , m_Pos.x+(-8+viiSub::Rand()%16)*100 , m_Pos.y+(-24+viiSub::Rand()%12)*100 );
	}
	if( m_sBlackSmokeWait > 0 )
	{
		if( m_sBlackSmokeWait%4==0)
		{
			CEffectBlackFire *pBlack;
			Sint32 ax,ay;
			ax = m_Pos.x+(-32+viiSub::Rand()%64)*100;
			ay = m_Pos.y+(-16+viiSub::Rand()%32)*100;

			pBlack = new CEffectBlackFire( ax , ay , NULL );
			pBlack->m_Pos.z = PRIO_BG2-1;
		}
		m_sBlackSmokeWait --;
	}

	//--------------------------------------------

	if( m_bClear )
	{
		SetActionSeq( enActionSeqEnd );
	}

}

void CEneEx0010Suihei::Draw()
{
	Sint32 sSpr = 0;
	if( !m_bDisp )
	{
		sSpr = 1;
	}

	Sint32 yy = -CBaseExtra::GetOverLayerY()*50;
	Sint32 ax = (4*m_sRebound)*m_sDirection*-1;

	viiDraw::Sprdmg( m_bDamage , &SprCEneEx0010Suihei[ sSpr ] , m_Pos.x + ax, m_Pos.y+yy , PRIO_PLAYER-1 , m_sAtribute|m_sAtributeDir , ARGB_DFLT ,1.0f );

}

void CEneEx0010Suihei::SetRight()
{
	//右向きにする

	m_sDirection = DIR_RIGHT;
}

