//------------------------------------------------------------
//
//�G�t�F�N�g�F�K�g�����O�r�b�g���[�U�[
//
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

extern gxSprite spr_ene_bullet[];

CEffeneBitLaser::CEffeneBitLaser(Sint32 x , Sint32 y, Sint32 rot ,Sint32 spd ,gxBool bBVflag )
{
	//----------------------------------------------
	//�R���X�g���N�^
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
	//�f�X�g���N�^
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
	//���C��
	//----------------------------------------------

	m_Pos.x += m_sAdd_X;
	m_Pos.y += m_sAdd_Y;

	if(m_HitAtari.IsImpacted() || viiSub::GetBoundBlock_Player( m_Pos.x , m_Pos.y ) )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	//���݃`�F�b�N
	if(IsScrollOut( 0,0) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	m_HitAtari.SetHantei( ID_ENEMY_ATK , &m_Pos );
	m_HitAtari.SetDirectAromorPosition( 50 );

	if(!m_bBuildVorg) return;

	//---------------------------------------------------
	//�r���h�{�[�O��p�e�����i���g�ɂ�������j
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
	//�I��
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
	//�`��
	//------------------------------------------------

	viiDraw::Sprite( &spr_ene_bullet[14], m_Pos.x , m_Pos.y , PRIO_ENE_BULLET,ATR_ALPHA_PLUS , viiSub::GetRandColor(0xff) ,1.0f , m_sRotation );

}




