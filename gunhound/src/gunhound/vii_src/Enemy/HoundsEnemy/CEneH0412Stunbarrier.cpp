//--------------------------------------------------------------------------------
//
// Stage00:スタンバリヤ
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../effect/EffectBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 256*100,

	enKuraiLeft  = -24,
	enKuraiTop   =  0,
	enKuraiRight =  24,
	enKuraiBottom=  0,

	enScore = 300,
	enWaitTime = 60,
	enWaitCycle = 3,
};



CEneH0412StunbarrierTate::CEneH0412StunbarrierTate( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_Tgt.x = x;
	m_Tgt.y = y+128*100;

	m_HitAtari.set_hp( enAtkPointBazooka );
	m_HitAtari.set_ap( enAtkPointBazooka );

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = 0;//enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = 0;//enKuraiBottom;

	m_sDirection = DIR_LEFT;
	m_sAtribute = ATR_DFLT;

	m_bDamage = gxFalse;

	//-----------------------------------------------------
	//グラウンドブロック
	//-----------------------------------------------------
	Sint32 ii = 0;
	while( ii < 128*2 )
	{
		if( viiSub::IsGroundBlock( m_Tgt.x , m_Tgt.y ) )
		{
			break;
		}
		m_Tgt.y += 800;
		ii += 1;
	}

	m_HitAtari.ay2 = (m_Tgt.y-m_Pos.y)/100;

	m_bThunder   = gxFalse;
	m_sWaitTime  = 0;
	m_sWaitIndex = 0;

 	m_pCEneCmnSpark = new CEneCmnSpark( m_Pos.x -40*100, m_Pos.y , m_Pos.x+40*100 , m_Pos.y );
	m_pCEneCmnSpark->SetLengthControl();
}


CEneH0412StunbarrierTate::~CEneH0412StunbarrierTate()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

	if(m_pCEneCmnSpark) delete m_pCEneCmnSpark;

}


void CEneH0412StunbarrierTate::SeqMain()
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

	//くらい判定登録
	m_bDamage = gxFalse;

	if( m_HitAtari.IsImpacted() )
	{
		viiMus::PlaySound(enSoundEneShotBiriBiri);
		m_HitAtari.SetOffImpact();
	}

	if( m_bThunder )
	{
		m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
		m_HitAtari.SetHantei   ( ID_ENEMY_ATK , &m_Pos );
	}

}


void CEneH0412StunbarrierTate::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	m_sWaitTime --;

	Sint32 sTime = (viiSub::GameCounter()*2)%(enWaitTime*(enWaitCycle*2));
	Sint32 sBefore = (enWaitCycle+m_sWaitIndex-1)%enWaitCycle;

	if( (sTime > (m_sWaitIndex*enWaitTime ))  && (sTime < ((m_sWaitIndex+enWaitCycle)*enWaitTime )) )
	{
		if( sTime > ((m_sWaitIndex+2)*enWaitTime) )
		viiEff::Set(EFF_FIREFLOWER , m_Pos.x , m_Pos.y+800 , argset( viiSub::Rand()%360) );
		m_bThunder = gxFalse;
	}
	else
	{
		m_bThunder = gxTrue;
		m_pCEneCmnSpark->Set( m_Pos.x , m_Pos.y , m_Tgt.x , m_Tgt.y );
		m_pCEneCmnSpark->SetRotation( 90.f-10+viiSub::Rand()%20 );

		if( viiSub::GameCounter()%2==0)
		{
			new CEffBreakStorm( m_Tgt.x , m_Tgt.y-800, 0 , 0 );
			viiEff::Set(EFF_FIREFLOWER , m_Tgt.x , m_Tgt.y-800 , argset( viiSub::Rand()%360) );
		}
	}

	if( m_sWaitTime <= 0 )
	{
		m_sWaitTime = enWaitTime*(enWaitCycle*2);
	}

}


void CEneH0412StunbarrierTate::SetWaitIndex( Sint32 n )
{
	m_sWaitIndex = n%enWaitCycle;
}


void CEneH0412StunbarrierTate::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------


}


void CEneH0412StunbarrierTate::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	viiMus::PlaySound(enSoundExplosionMiddle);

	ScorePlus( enScore );

	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

	SetActionSeq( enActionSeqEnd );

}

void CEneH0412StunbarrierTate::Draw()
{
	//	viiDraw::Sprdmg( m_bDamage , &SprCEneH0412StunbarrierTate[ enSTUNOBJ1 ] , m_Pos.x-80*100 , m_Pos.y , PRIO_ENEMY , ATR_DFLT , ARGB_DFLT );
	//	viiDraw::Sprdmg( m_bDamage , &SprCEneH0412StunbarrierTate[ enSTUNOBJ2 ] , m_Pos.x+80*100 , m_Pos.y , PRIO_ENEMY , ATR_DFLT , ARGB_DFLT );

	if( m_bThunder )
	{
		if( m_pCEneCmnSpark )
		{
			m_pCEneCmnSpark->Draw();
			m_pCEneCmnSpark->Draw();
		}
	}

}

