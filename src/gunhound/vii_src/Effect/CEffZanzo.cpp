//------------------------------------------------------------
//
//�G�t�F�N�g�F�c��
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
	//�R���X�g���N�^
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
	//�f�X�g���N�^
	//----------------------------------------------
	
	
}


void CEffZanzo::SeqMain()
{
	//----------------------------------------------
	//���C��
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
		//���ߓx�[���p�[�Z���g�ŏ���
		SetActionSeq( enActionSeqCrash );
		return;
	}

}


void CEffZanzo::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}



void CEffZanzo::Draw()
{
	//------------------------------------------------
	//�`��
	//------------------------------------------------
	Sint32  col = m_sColor/2;
	Float32 scl = m_sScale/100.0f;

	viiDraw::Spr_NoScrl( &sprZanzo[0] , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET+1  ,ATR_ALPHA_PLUS ,ARGB(col,0xff,0xff,0xff)  ,scl );

}


