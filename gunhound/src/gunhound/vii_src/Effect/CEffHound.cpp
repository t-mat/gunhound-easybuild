//@
//@
//@
//@
//@
#include <gunvalken.h>
#include "EffectBase.h"

enum {
	enScrollOutWidth  = 128*100,
	enScrollOutHeight = 32*100,
};

extern gxSprite stSprApogy[];

//------------------------------------------------------------
//
//�A�|�W���[�^�[
//
//------------------------------------------------------------

CEffApogyMotor::CEffApogyMotor(Sint32 x , Sint32 y, Sint32 prio,Sint32 rot , Sint32 Speed )
{
	//----------------------------------------------
	//�R���X�g���N�^
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = prio;

	m_Add.x = ( viiMath::Cos100(rot)*Speed )/100;
	m_Add.y = ( viiMath::Sin100(rot)*Speed )/100;

	m_sRotation = rot;
	m_fScale = 1.0f;
	m_sAlpha = 0xff;
	m_fScale = 1.0f;

}


CEffApogyMotor::~CEffApogyMotor()
{
	//----------------------------------------------
	//�f�X�g���N�^
	//----------------------------------------------


}


void CEffApogyMotor::SeqInit()
{
	//----------------------------------------------
	//������
	//----------------------------------------------


	SetActionSeq( enActionSeqMain );
}


void CEffApogyMotor::SeqMain()
{
	//----------------------------------------------
	//���C��
	//----------------------------------------------


	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	m_sAlpha -= 64;

	if(m_sAlpha <= 0)
	{
		m_sAlpha = 0;
		SetActionSeq( enActionSeqEnd );
		return;
	}

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	m_fScale    += -0.01f;

}


void CEffApogyMotor::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffApogyMotor::SeqEnd()
{
	
	
}


void CEffApogyMotor::Draw()
{
	//------------------------------------------------
	//�`��
	//------------------------------------------------

	viiDraw::Sprite((gxSprite*)&stSprApogy[ 0 ] ,
		 m_Pos.x,
		 m_Pos.y,
		 m_Pos.z,
		 ATR_ALPHA_PLUS,
		 ARGB(m_sAlpha,0x80,0x80,0xff),
		 m_fScale,m_sRotation);


}




