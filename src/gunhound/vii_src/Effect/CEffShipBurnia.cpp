//------------------------------------------------------------
//
//エフェクト：戦艦のバーニア
//　バーシス艦尾の炎、ICBMの炎など
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

enum {
	enScrollOutWidth  = 128*100,
	enScrollOutHeight = 32*100,
};

extern gxSprite sprShipBurnia[]=
{
	{TPG_EFFECT,0,208,64,16,56,8},
};

CEffShipBurnia::CEffShipBurnia(Sint32 x , Sint32 y, Sint32 prio,Sint32 rot , Sint32 Speed )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_sPrio = prio;

	Sint32 spd = Speed/2+viiSub::Rand()%(Speed/2);

	m_sAdd_X = ( viiMath::Cos100(rot)*spd )/100;
	m_sAdd_Y = ( viiMath::Sin100(rot)*spd )/100;

	m_sRotation = rot-180;

}


CEffShipBurnia::~CEffShipBurnia()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------


}


void CEffShipBurnia::SeqInit()
{
	//----------------------------------------------
	//初期化
	//----------------------------------------------

	m_sAlpha = 158- viiSub::Rand()%30;
	m_sScale = 50 + viiSub::Rand()%50;

	SetActionSeq( enActionSeqMain );
}


void CEffShipBurnia::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------


	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	m_sAlpha -= 12;

	if(m_sAlpha <= 0)
	{
		m_sAlpha = 0;
		SetActionSeq( enActionSeqEnd );
		return;
	}

	m_Pos.x += m_sAdd_X;
	m_Pos.y += m_sAdd_Y;

	m_sScale    += 1;

	viiSub::SetGravitMove( m_Pos.x , m_Pos.y );

}


void CEffShipBurnia::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffShipBurnia::SeqEnd()
{
	
	
}


void CEffShipBurnia::Draw()
{
	//------------------------------------------------
	//描画
	//------------------------------------------------

	viiDraw::Sprite(&sprShipBurnia[ 0 ] ,
		 m_Pos.x,
		 m_Pos.y,
		 m_sPrio,
		 ATR_ALPHA_PLUS,
		 ARGB(m_sAlpha,0x80,0x80,0xff),
		 m_sScale/50.0f,m_sRotation);


}




