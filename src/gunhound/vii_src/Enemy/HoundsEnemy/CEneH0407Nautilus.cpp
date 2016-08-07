//--------------------------------------------------------------------------------
//
// Stage00:潜水艦
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"

enum {
	enScrollOutWidth  = 1024*100,
	enScrollOutHeight = 128*100,

	enKuraiLeft  = -30,
	enKuraiTop   = -34,
	enKuraiRight =  30,
	enKuraiBottom=  0,

	enScore = 300,
};

gxSprite SprCEneH0407Nautilus[]=
{
	{ enTexCEneH0407Nautilus,0,0,512,144,256,72},	//本体
	{ enTexCEneH0407Nautilus,0,144,256,112,128,56},	//収納用
};

CEneH0407Nautilus::CEneH0407Nautilus( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_Add.x = 0;
	m_Add.y = 0;

	m_Base.x = x;
	m_Base.y = y;

	m_bStart = gxFalse;

	m_HitKurai.set_hp( HP_ARMORCLASS_B );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_RIGHT;
	m_sAtribute  = ATR_DFLT;

	m_bDamage = gxFalse;

	m_RidePos.x = 0;
	m_RidePos.y = 0;
}


CEneH0407Nautilus::~CEneH0407Nautilus()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------


}


void CEneH0407Nautilus::SeqMain()
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

/*
	gxBool bRide = gxFalse;
	Sint32 vx,vy;

	vx = pGame->GetHound()->m_Pos.x;
	vy = pGame->GetHound()->m_Pos.y;

	if( vx >m_Pos.x-88*100 &&  vx < m_Pos.x+96*100 )
	{
		if( vy >m_Pos.y && vy < m_Pos.y+32*100 )
		{
			bRide = gxTrue;
		}
	}

	if( bRide )
	{
		pGame->GetHound()->SetRide( gxTrue , &m_RidePos );
	}
	else
	{
		pGame->GetHound()->SetRide( gxFalse , NULL );
	}
*/

	LogicAI();

	SetRader( enTypeMarkerFlyingPartner );
	DirectionAdjust();

	//くらい判定登録
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

}


void CEneH0407Nautilus::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	if( m_bStart )
	{
		if( m_Add.x <= 600*100 )
		{
			if( m_Add.x <= 100 ) m_Add.x = 100;
			m_Add.x += 16;
		}

	}

/*
		CEffectBubble *p;
		Sint32 ax,ay;

		for(Sint32 ii=0;ii<3;ii++)
		{
			ax = m_Pos.x-246*100;
			ay = m_Pos.y;

			ax += (-64+viiSub::Rand()%128)*100;
			ay += (-32+viiSub::Rand()%64)*100;

			p = new CEffectBubble(	ax , 	ay ,176, 850+viiSub::Rand()%100 );
			p->SetWide( (m_sTimer*16)%360 , 180 );
			p->SetScale( 0.5f );
			p->m_Pos.z = PRIO_EFFECT;
		}

*/
	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	m_RidePos.x = m_Add.x;
	m_RidePos.y = m_Add.y;

}


void CEneH0407Nautilus::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CEneH0407Nautilus::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	viiMus::PlaySound(enSoundExplosionMiddle);

	ScorePlus( enScore );

	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

	SetActionSeq( enActionSeqEnd );

}

void CEneH0407Nautilus::Draw()
{

	viiDraw::Sprdmg( gxFalse , &SprCEneH0407Nautilus[ 0 ] , m_Pos.x , m_Pos.y , PRIO_PLAYER-1 , m_sAtribute|m_sAtributeDir , ARGB_DFLT );
	viiDraw::Sprdmg( gxFalse , &SprCEneH0407Nautilus[ 1 ] , m_Pos.x , m_Pos.y , PRIO_PLAYER+1 , m_sAtribute|m_sAtributeDir , ARGB_DFLT );

}

