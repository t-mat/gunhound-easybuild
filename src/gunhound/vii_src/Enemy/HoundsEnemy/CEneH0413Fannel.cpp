//--------------------------------------------------------------------------------
//
// 輸送トラック
//
//--------------------------------------------------------------------------------
#include "HoundsEnemy.h"
#include "../../Effect/EffectBase.h"

enum {
	enScrollOutWidth  = 256*100,
	enScrollOutHeight = 256*100,

	enKuraiLeft  = -16,
	enKuraiTop   = -16,
	enKuraiRight =  16,
	enKuraiBottom=  16,

};

gxSprite SprCEneH0413Fannel[]=
{
//	{enTexCEneH0413Fannel,0   ,224,32,32,16,16},
	{enTexCEneH0413Fannel,32  ,224,32,32,16,16},
	{enTexCEneH0413Fannel,64  ,224,32,32,16,16},
};

CEneH0413Fannel::CEneH0413Fannel( Sint32 x, Sint32 y , Sint32 sRotation )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_sSprite = 0;

	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_ENEMY;

	m_sRotation = sRotation;

	m_sWaitMax = 128+sRotation;

}


CEneH0413Fannel::~CEneH0413Fannel()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------


}


void CEneH0413Fannel::SeqInit()
{
	//-----------------------------------------------------
	//初期化
	//-----------------------------------------------------

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_HitKurai.set_hp( enAtkPointBazooka );

	m_Add.x = 0;
	m_Add.y = 0;

	m_sWaitCnt = m_sWaitMax;
	m_sAtackRot = 0;

	m_pTgt = NULL;

	m_bDamage   = gxFalse;

	SetActionSeq( enActionSeqMain );

}


void CEneH0413Fannel::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if (m_HitKurai.is_dead() || m_bClear )
	{
		viiEff::Set( EFF_RING , m_Pos.x , m_Pos.y , argset(500) );
		SetActionSeq( enActionSeqCrash );
		return;
	}

	LogicAI();

	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	m_bDamage = m_HitKurai.is_damage();

	m_HitKurai.SetHantei( ID_ENEMY_DEF , &m_Pos );

}


void CEneH0413Fannel::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	Sint32 tx,ty;

	if( m_pTgt )
	{
		tx = m_pTgt->x + viiMath::Cos100(m_sRotation)*160;
		ty = m_pTgt->y + viiMath::Sin100(m_sRotation)*160;

		m_Add.x = (tx-m_Pos.x)/50;
		m_Add.y = (ty-m_Pos.y)/50;

		m_Pos.x += m_Add.x;
		m_Pos.y += m_Add.y;

	}

	//---------------------------
	//角度補正
	//---------------------------

	Sint32 sR = get_valken_rot( m_Pos.x , m_Pos.y );

	m_sAtackRot = sR;

	if( m_sWaitCnt > 0 )
	{
		m_sWaitCnt --;
		if( m_sWaitCnt == 0 )
		{
			m_sWaitCnt = m_sWaitMax;
			Atack();
		}
	}

	Sint32 sFrm = 8;
	m_sSprite = (viiSub::GameCounter()%(2*sFrm))/sFrm;

}


gxBool CEneH0413Fannel::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

	Sint32 ax,ay;

	ax = viiMath::Cos100( (m_sRotation)%360 )*24;
	ay = viiMath::Sin100( (m_sRotation)%360 )*24;

	CEne0001EneSuperBall *p;
	p = new CEne0001EneSuperBall( m_Pos.x+ax , m_Pos.y+ay , 680*100 , 0 );
	p->SetPattern( CEne0001EneSuperBall::enPatternTargetting );

	return gxFalse;
}


void CEneH0413Fannel::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	EffectMakeBigBomb(m_Pos.x , m_Pos.y);
	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 5 , 3 );

	if( m_bClear )
	{
		SetActionSeq( enActionSeqEnd );
	}
	else
	{
		if( m_pTgt )
		{
			m_Pos.x = m_pTgt->x;
			m_Pos.y = m_pTgt->y;
		}
		viiMus::PlaySound(enSoundExplosionMiddle);
		m_sWaitCnt = m_sWaitMax;
		SetActionSeq( enActionSeqInit );
	}

}


void CEneH0413Fannel::Draw()
{
	//---------------------------------------------
	//表示
	//---------------------------------------------

	viiDraw::Sprdmg(
		m_bDamage ,
		&SprCEneH0413Fannel[m_sSprite],
		m_Pos.x,
		m_Pos.y,
		m_Pos.z,
		ATR_DFLT,
		ARGB_DFLT,1.f , m_sAtackRot );
}

