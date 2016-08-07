//------------------------------------------------------------
//
//�G�t�F�N�g�F�p�[�e�B�N��
//
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"
gxSprite sprPixel = { TPG_EFFECT,16,24,3,3,0,0 };

CEffPerticle::CEffPerticle(Sint32 x , Sint32 y, Sint32 rot , Sint32 Speed )
{
	//----------------------------------------------
	//�R���X�g���N�^
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;

	m_sAdd_X = viiMath::Cos100(rot)*Speed;
	m_sAdd_Y = viiMath::Sin100(rot)*Speed;

	m_sTimer = 5+viiSub::Rand()%5;
	m_sGravityAddY = 0;
}


CEffPerticle::~CEffPerticle()
{
	//----------------------------------------------
	//�f�X�g���N�^
	//----------------------------------------------
	
	
}


void CEffPerticle::SeqInit()
{

	SetActionSeq( enActionSeqMain );
}


void CEffPerticle::SeqMain()
{
	//----------------------------------------------
	//���C��
	//----------------------------------------------

	if( m_sTimer > 0 ) m_sTimer --;

	if( IsScrollOut( 0,0 ) || m_sTimer == 0)
	{
		SetActionSeq( enActionSeqCrash );
	}

	m_sGravityAddY += 30;

	m_Pos.x += m_sAdd_X;
	m_Pos.y += m_sAdd_Y;
	m_Pos.y += m_sGravityAddY;
}


void CEffPerticle::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffPerticle::SeqEnd()
{
	
	
}


void CEffPerticle::Draw()
{
	//------------------------------------------------
	//�`��
	//------------------------------------------------

	viiDraw::Sprite( &sprPixel , m_Pos.x , m_Pos.y , PRIO_EFFECT , ATR_DEFAULT , ARGB_DFLT , 1.0f , 0 );

}


//------------------------------------------------------------
//
//�G�t�F�N�g�F�g���p�[�e�B�N��
//
//------------------------------------------------------------

CEffExPerticle::CEffExPerticle( Sint32 x , Sint32 y , Sint32 prio )
{
	//----------------------------------------------
	//�R���X�g���N�^
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = prio;
	m_sAdd_X = 0;
	m_sAdd_Y = 0;
	m_sGravit = 0;
	m_bEndFlag = gxFalse;
	m_fScale    = 1.0f;
	m_sARGB     = ARGB_DFLT;
	m_sPrio     = prio;
	m_sRotation = 0;
	m_sAtribute = ATR_DFLT;
	m_sAlphaAdd = 0x00;
	m_bNoScroll = gxFalse;

	m_sDeadTimer = 0;
	m_pSprite = NULL;
}


CEffExPerticle::~CEffExPerticle()
{
	//----------------------------------------------
	//�f�X�g���N�^
	//----------------------------------------------


}


void CEffExPerticle::SeqInit()
{

	SetActionSeq( enActionSeqMain );
}


void CEffExPerticle::SeqMain()
{
	//----------------------------------------------
	//���C��
	//----------------------------------------------
	if( m_sDeadTimer > 0 )
	{
		m_sDeadTimer --;
		if(m_sDeadTimer==0)
		{
			SetActionSeq( enActionSeqCrash );
			return;
		}
	}

	if(m_bEndFlag)
	{
		if( IsScrollOut( 0,0 ) )
		{
			SetActionSeq( enActionSeqCrash );
		}
	}

	//------------------------------------------------
	//�����������Ă����i������������j
	//------------------------------------------------
	Sint32 sAlpha = ((m_sARGB&0xff000000)>>24);

	sAlpha += m_sAlphaAdd;
	if( sAlpha >= 255) sAlpha = 0xFF;

	if( sAlpha <= 0)
	{
		sAlpha = 0x00;
//		if(m_bEndFlag)
		{
			SetActionSeq( enActionSeqCrash );
		}
	}

	m_sARGB = ( (m_sARGB&0x00ffffff) | ((sAlpha<<24)&0xFF000000));

	m_sAdd_Y += m_sGravit;

	m_Pos.x += m_sAdd_X;
	m_Pos.y += m_sAdd_Y;

}


void CEffExPerticle::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffExPerticle::SeqEnd()
{
	
	
}


void CEffExPerticle::Draw()
{
	//------------------------------------------------
	//�`��
	//------------------------------------------------
	if( m_pSprite == NULL ) return;

	if( m_bNoScroll )
	{
		viiDraw::Spr_NoScrl( m_pSprite , m_Pos.x , m_Pos.y , m_sPrio , m_sAtribute , m_sARGB , m_fScale , m_sRotation );
	}
	else
	{
		viiDraw::Sprite( m_pSprite , m_Pos.x , m_Pos.y , m_sPrio , m_sAtribute , m_sARGB , m_fScale , m_sRotation );
	}

}


void CEffExPerticle::SetDirection( Sint32 rot , Sint32 Speed )
{

	m_sAdd_X = (viiMath::Cos100(rot)*Speed)/100;
	m_sAdd_Y = (viiMath::Sin100(rot)*Speed)/100;

}


void CEffExPerticle::SetSprite(gxSprite *pSpr)
{

	m_pSprite = pSpr;

}


void CEffExPerticle::SetEnd()
{

	m_bEndFlag = gxTrue;

}


