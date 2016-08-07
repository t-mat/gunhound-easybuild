//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -8,
	enKuraiTop   = -8,
	enKuraiRight =  8,
	enKuraiBottom=  8,

	enScore = 300,
};

gxSprite SprCEneH0000PunchBall[] =
{
	{ enTexCEneH0107Walker,0,96,40,24,16,16},	//ボディ
	{ enTexCEneH0107Walker,40,96,16,16,8,2},	//レッグ
	{ enTexCEneH0107Walker,64,96,24,16,16,4},	//フット
};

CEneH0000PunchBall::CEneH0000PunchBall( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_LEFT;
	m_sAtribute = ATR_DFLT;

	m_Add.x = 0;
	m_Add.y = 0;
	m_Base.x = m_Pos.x;
	m_Base.y = m_Pos.y;

	m_sWait  = 0;
	m_sCount = 0;

 	m_pPenduram = new CPenduram();
 	m_pPenduram->SetWeight(300);
 	m_pPenduram->Set( 10 );
}


CEneH0000PunchBall::~CEneH0000PunchBall()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

	delete m_pPenduram;

}


void CEneH0000PunchBall::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------
	gxBool bAtack = gxFalse;

	if( m_HitKurai.is_dead() )
	{
//		SetActionSeq( enActionSeqCrash );
//		return;
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}

	LogicAI();

	m_pPenduram->Action();

	Sint32 r;

	r = (Sint32)m_pPenduram->GetRotation()+90;
	m_Add.x = viiMath::Cos100( r )*48;
	m_Add.y = viiMath::Sin100( r )*32;

	m_Pos.x = m_Base.x + m_Add.x;
	m_Pos.y = m_Base.y - m_Add.y;

	if( m_sWait > 0 )
	{
		m_sWait --;
		if( m_sWait == 0 )
		{
			bAtack = gxTrue;
		}
	}
	else
	{
		m_sCount = 0;
	}

	//くらい判定登録
	if( m_HitKurai.IsImpacted() )
	{
		m_HitKurai.SetOffImpact();
		m_pPenduram->Set( 32 );
		m_sCount ++;
		m_sWait  = 24;
		ScorePlus( m_sCount );
	}

	if( bAtack )
	{
//		m_HitKurai.SetAtackType( ATK_TYPE_CANNON );
//		m_HitKurai.set_ap( 10 );
//		m_HitKurai.SetHantei(ID_ENEMY_ATK , &m_Pos );
	}
	else
	{
		m_HitKurai.set_hp( 100 );
		m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );
	}
}


void CEneH0000PunchBall::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

}


void CEneH0000PunchBall::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CEneH0000PunchBall::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	viiMus::PlaySound(enSoundExplosionMiddle);

	ScorePlus( enScore );

	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

	SetActionSeq( enActionSeqEnd );

}

void CEneH0000PunchBall::Draw()
{

	viiDraw::Sprdmg( m_HitKurai.is_damage() , &SprCEneH0000PunchBall[ 0 ] , m_Pos.x , m_Pos.y , PRIO_ENEMY , m_sAtribute|m_sAtributeDir , ARGB_DFLT );

}

