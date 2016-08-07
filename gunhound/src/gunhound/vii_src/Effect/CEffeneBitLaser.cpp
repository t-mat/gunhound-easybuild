//------------------------------------------------------------
//
//エフェクト：ガトリングビットレーザー
//
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

extern gxSprite spr_ene_bullet[];

CEffeneBitLaser::CEffeneBitLaser(Sint32 x , Sint32 y, Sint32 rot ,Sint32 spd ,gxBool bBVflag )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_sRotation = rot;
	m_sAdd_X = (viiMath::Cos100(rot)*spd)/100;
	m_sAdd_Y = (viiMath::Sin100(rot)*spd)/100;

	m_HitAtari.set_hp(1);
	m_bBuildVorg = bBVflag;
}


CEffeneBitLaser::~CEffeneBitLaser()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------
	
	
}


void CEffeneBitLaser::SeqInit()
{

	m_HitAtari.ax1 = -1;
	m_HitAtari.ay1 = -1;
	m_HitAtari.ax2 =  1;
	m_HitAtari.ay2 =  1;

	m_HitAtariMe.ax1 = -1;
	m_HitAtariMe.ay1 = -1;
	m_HitAtariMe.ax2 =  1;
	m_HitAtariMe.ay2 =  1;

	SetActionSeq( enActionSeqMain );

}


void CEffeneBitLaser::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------

	m_Pos.x += m_sAdd_X;
	m_Pos.y += m_sAdd_Y;

	if(m_HitAtari.IsImpacted() || viiSub::GetBoundBlock_Player( m_Pos.x , m_Pos.y ) )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	//存在チェック
	if(IsScrollOut( 0,0) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	m_HitAtari.SetHantei( ID_ENEMY_ATK , &m_Pos );
	m_HitAtari.SetDirectAromorPosition( 50 );

	if(!m_bBuildVorg) return;

	//---------------------------------------------------
	//ビルドボーグ専用弾処理（自身にも当たる）
	//---------------------------------------------------

	m_HitAtariMe.SetHantei( ID_VERSIS_ATK , &m_Pos );

	if(m_Pos.x <= 10*100 || m_Pos.x >= 350*100 || m_HitAtariMe.IsImpacted() )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

}


void CEffeneBitLaser::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	viiEff::Set( EFF_HITFLASH , m_Pos.x , m_Pos.y , argset(1) );

	SetActionSeq( enActionSeqEnd );

}


void CEffeneBitLaser::SeqEnd()
{
	
	
}


void CEffeneBitLaser::Draw()
{
	//------------------------------------------------
	//描画
	//------------------------------------------------

	viiDraw::Sprite( &spr_ene_bullet[14], m_Pos.x , m_Pos.y , PRIO_ENE_BULLET,ATR_ALPHA_PLUS , viiSub::GetRandColor(0xff) ,1.0f , m_sRotation );

}




