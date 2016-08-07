//------------------------------------------------------------
//
//�G�t�F�N�g�F���Ԃ�����
//
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

gxSprite sprFloor[] = {
	{ TPG_EFFECT,64,208,48,16,24,16 },	//�t���A
	{ TPG_EFFECT,160,144,16,16,8,8 },	//�u���b�N
	{ TPG_EFFECT,176,144,16,16,8,8 },	//�u���b�N
};

enum {
	enScrollOutWidth  = 32*100,
	enScrollOutHeight = 32*100,
};

CEffBrokenFloor::CEffBrokenFloor(Sint32 x , Sint32 y )
{
	//----------------------------------------------
	//�R���X�g���N�^
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;

}


CEffBrokenFloor::~CEffBrokenFloor()
{
	//----------------------------------------------
	//�f�X�g���N�^
	//----------------------------------------------

}


void CEffBrokenFloor::SeqInit()
{

//	if( Joy[0].trg&BTN_Z)
	{
		new CEffBrokenFloorBlock( m_Pos.x , m_Pos.y );
		new CEffBrokenFloorBlock( m_Pos.x , m_Pos.y );
		new CEffBrokenFloorBlock( m_Pos.x , m_Pos.y );
		new CEffBrokenFloorBlock( m_Pos.x , m_Pos.y );
	}

	SetActionSeq( enActionSeqMain );
}


void CEffBrokenFloor::SeqMain()
{
	//----------------------------------------------
	//���C��
	//----------------------------------------------

}


void CEffBrokenFloor::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffBrokenFloor::SeqEnd()
{


}


void CEffBrokenFloor::Draw()
{
	//------------------------------------------------
	//�`��
	//------------------------------------------------

	viiDraw::Sprite( &sprFloor[0] , m_Pos.x , m_Pos.y , PRIO_EFFECT , ATR_DEFAULT , ARGB_DFLT );

}


//------------------------------------------------------------
//
//�G�t�F�N�g�F���Ԃ��������̃u���b�N
//
//------------------------------------------------------------


CEffBrokenFloorBlock::CEffBrokenFloorBlock(Sint32 x , Sint32 y )
{
	//----------------------------------------------
	//�R���X�g���N�^
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;

	m_sRotation = viiSub::Rand()%360;
	m_fScale    = (50+viiSub::Rand()%50)/100.0f;

	m_sAddX = -300+(viiSub::Rand()%7)*100;
	m_sAddY = -(500+(viiSub::Rand()%3)*100);
}


CEffBrokenFloorBlock::~CEffBrokenFloorBlock()
{
	//----------------------------------------------
	//�f�X�g���N�^
	//----------------------------------------------

}


void CEffBrokenFloorBlock::SeqInit()
{

	SetActionSeq( enActionSeqMain );
}


void CEffBrokenFloorBlock::SeqMain()
{
	//----------------------------------------------
	//���C��
	//----------------------------------------------
	if(IsScrollOut( enScrollOutWidth,enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqEnd );
	}

	m_Pos.x += m_sAddX;
	m_Pos.y += m_sAddY;

	m_sAddY += 50;

	m_sRotation += 4;

}


void CEffBrokenFloorBlock::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffBrokenFloorBlock::SeqEnd()
{


}


void CEffBrokenFloorBlock::Draw()
{
	//------------------------------------------------
	//�`��
	//------------------------------------------------

	viiDraw::Sprite( &sprFloor[1] , m_Pos.x , m_Pos.y , PRIO_EFFECT , ATR_DEFAULT , ARGB_DFLT ,m_fScale, m_sRotation );

}




