//------------------------------------------------------------
//
//エフェクト：残像
//
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

static gxSprite sprZanzo[]={
	{ enTexPageEffectCommon02,224,128,8,8,4,4},//LED
};


CEffZanzo::CEffZanzo(Sint32 x , Sint32 y )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------
	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );

	x -=  sx*100;
	y -=  sy*100;

	m_Pos.x = x;
	m_Pos.y = y;

	m_sScale = 100;
	m_sColor = 200;
}


CEffZanzo::~CEffZanzo()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------
	
	
}


void CEffZanzo::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------


	if(m_sColor > 0)
	{
		m_sColor -= 2;
	}
	if( m_sScale > 0)
	{
		m_sScale -= 1;
	}

	if( m_sColor <= 0 || m_sScale <= 0)
	{
		//透過度ゼロパーセントで消滅
		SetActionSeq( enActionSeqCrash );
		return;
	}

}


void CEffZanzo::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}



void CEffZanzo::Draw()
{
	//------------------------------------------------
	//描画
	//------------------------------------------------
	Sint32  col = m_sColor/2;
	Float32 scl = m_sScale/100.0f;

	viiDraw::Spr_NoScrl( &sprZanzo[0] , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET+1  ,ATR_ALPHA_PLUS ,ARGB(col,0xff,0xff,0xff)  ,scl );

}


