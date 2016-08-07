//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -30,
	enKuraiTop   = -34,
	enKuraiRight =  30,
	enKuraiBottom=  0,

	enScore = 300,
};

gxSprite SprCEneH0503HoundFly[]=
{
	{enTexCEneH0201VorzoyFly,128,96 ,96,32,48,8},	//ボルゾイ輸送ヘリ
	{enTexCEneH0201VorzoyFly,128,64 ,96,32,48,32},	//ボルゾイ輸送ヘリ(Prop)
	{enTexCEneH0201VorzoyFly,128,224,96,32,48,32},	//ボルゾイ輸送ヘリ(Prop)
};


CEneH0503HoundFly::CEneH0503HoundFly( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_PLAYER+1;

	m_pTgtX = NULL;
	m_pTgtY = NULL;
	m_HitKurai.set_hp( HP_ARMORCLASS_B );

	m_sHigh = 0;
	m_sRotation = 10;

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_RIGHT;

	m_pPenduram = new CPenduram();

	m_pPenduram->SetWeight(100);
	m_pPenduram->Set( 20 );
	m_pPenduram->SetInfinity();

	m_Add.x = 0;
	m_Add.y = 0;

	m_bDamage      = gxFalse;
	m_bCarryOn     = gxFalse;
	m_bCarryOffNow = gxFalse;

	m_uARGB = ARGB_DFLT;

	m_sPropTimer = 0;
}


CEneH0503HoundFly::~CEneH0503HoundFly()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

	if(m_pPenduram)
	{
		delete m_pPenduram;
	}
	else
	{
		ASSERT(m_pPenduram==NULL);
	}

}


void CEneH0503HoundFly::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( m_bClear )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}


	if( m_HitKurai.is_dead() )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	m_pPenduram->Action();

	LogicAI();

	if( m_bCarryOffNow )
	{
		//切り離した
		m_bCarryOffNow = gxFalse;
		m_bCarryOn     = gxFalse;
	}

	if( m_bCarryOn )
	{
		//吊り下げている
		Sint32 x,y,r;

		r = (Sint32)m_pPenduram->GetRotation()/2+90;
		x = viiMath::Cos100( r )*16;
		y = viiMath::Sin100( r )*56;

		*m_pTgtX = m_Pos.x +x;
		*m_pTgtY = m_Pos.y +y;
	}
	else
	{
		if( m_Add.y >= -320 ) m_Add.y -= 1;
		if( m_Add.x >= -320 ) m_Add.x -= 1;
		if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
		{
			SetActionSeq( enActionSeqEnd );
			return;
		}
	}

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;


	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	//くらい判定登録
//	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

}


void CEneH0503HoundFly::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

/*
	if(Joy[0].psh&JOY_U)
	{
		m_Pos.y -= 100;
	}
	else if(Joy[0].psh&JOY_D)
	{
		m_Pos.y += 100;
	}

	if(Joy[0].psh&JOY_L)
	{
		m_Pos.x -= 100;
	}
	else if(Joy[0].psh&JOY_R)
	{
		m_Pos.x += 100;
	}
*/

	if( m_sTimer%4 == 0 )
	{
		new CEffectGroundSmoke( m_Pos.x-1600+viiSub::Rand()%1600 , m_Pos.y , DIR_LEFT );
		new CEffectGroundSmoke( m_Pos.x+1600-viiSub::Rand()%1600 , m_Pos.y , DIR_RIGHT );
	}

	m_sHigh = (viiSub::Rand()%2)*100;

	m_sPropTimer ++;
}


void CEneH0503HoundFly::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CEneH0503HoundFly::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	viiMus::PlaySound(enSoundExplosionMiddle);
	ScorePlus( enScore );
	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

	SetActionSeq( enActionSeqEnd );

}

void CEneH0503HoundFly::Draw()
{
	Sint32 xx = 0;


	if( m_sPropTimer%4 < 2 )
	{
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0503HoundFly[ 1 ] , m_Pos.x+xx , m_Pos.y , m_Pos.z-1 , m_sAtributeDir , m_uARGB ,1.0f,m_sRotation);
		if( m_sPropTimer%2 ) viiDraw::Sprdmg( m_bDamage , &SprCEneH0503HoundFly[ 2 ] , m_Pos.x+xx , m_Pos.y+100 , m_Pos.z-1 , ATR_DFLT , m_uARGB|0x30ffffff ,1.0f,m_sRotation);
	}
	else
	{
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0503HoundFly[ 2 ] , m_Pos.x+xx , m_Pos.y , m_Pos.z-1 , m_sAtributeDir , m_uARGB ,1.0f , m_sRotation);
		if( m_sPropTimer%2 ) viiDraw::Sprdmg( m_bDamage , &SprCEneH0503HoundFly[ 1 ] , m_Pos.x+xx , m_Pos.y+100 , m_Pos.z-1 , ATR_DFLT , m_uARGB|0x30ffffff ,1.0f,m_sRotation);
	}

	viiDraw::Sprdmg( m_bDamage , &SprCEneH0503HoundFly[ 0 ] , m_Pos.x , m_Pos.y+m_sHigh , m_Pos.z , m_sAtributeDir , m_uARGB , 1.0f , m_sRotation );
}

