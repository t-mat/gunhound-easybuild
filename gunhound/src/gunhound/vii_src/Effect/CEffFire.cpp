//------------------------------------------------------------
//
//�G�t�F�N�g�F�΍�
//
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

enum {
	enScrollOutWidth  = 192*100,
	enScrollOutHeight = 32*100,
};

extern gxSprite StSprFireEffect[]=
{
	{ enTexPageEffectCommon02,0,160,64,32,32,32},
	{ enTexPageEffectCommon02,128,160,64,32,32,32},
	{ enTexPageEffectCommon02,64,160,32,32,16,16},
	{ enTexPageEffectCommon02,96,160,32,32,16,16},
};

CEffFire::CEffFire(Sint32 x , Sint32 y, Sint32 prio ,Sint32 sAutoCleanWait )
{
	//----------------------------------------------
	//�R���X�g���N�^
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = prio;

	m_sAlpha = 0xF0;
	m_sScale = 100;

	m_sSprite = 0;
	m_sOffsetX = 0;
	m_sAutoCleanWait = sAutoCleanWait;
}


CEffFire::~CEffFire()
{
	//----------------------------------------------
	//�f�X�g���N�^
	//----------------------------------------------


}


void CEffFire::SeqInit()
{
	//----------------------------------------------
	//������
	//----------------------------------------------

//	m_sAlpha = 158- viiSub::Rand()%30;
//	m_sScale = 50 + viiSub::Rand()%50;

	SetActionSeq( enActionSeqMain );
}


void CEffFire::SeqMain()
{
	//----------------------------------------------
	//���C��
	//----------------------------------------------
	Sint32 sTiming = 1;

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
//		SetActionSeq( enActionSeqEnd );
		return;
	}
	if( m_sAutoCleanWait > 0 )
	{
		m_sAutoCleanWait --;
		if(m_sAutoCleanWait==0)
		{
			SetActionSeq( enActionSeqEnd );
			return;
		}
	}

	if( viiSub::GameCounter()%4<2)
	{
		m_sSprite  = viiSub::Rand()%2;
		m_sOffsetX = (-2+viiSub::Rand()%4)*100;
	}

	if( m_sScale > 200 )
	{
		sTiming  = 32;
		if(viiSub::Rand()%3)
		{
			//return;
		}
	}

	if(viiSub::Rand()%sTiming==0)
	{
		new CEffFireParts( m_Pos.x +(-24+viiSub::Rand()%48)*m_sScale , m_Pos.y-(viiSub::Rand()%8)*m_sScale , m_Pos.z);
		new CEffFireSmoke( m_Pos.x +(-24+viiSub::Rand()%48)*m_sScale , m_Pos.y-(viiSub::Rand()%8)*m_sScale , m_Pos.z);
	}

	CEffFireParts *p;
	p = new CEffFireParts( m_Pos.x +(-24+viiSub::Rand()%48)*(m_sScale*2) , m_Pos.y-(viiSub::Rand()%8)*m_sScale , m_Pos.z);
	p->SetScale(m_sScale/100.f);

	p = new CEffFireParts( m_Pos.x +(-24+viiSub::Rand()%48)*(m_sScale*2) , m_Pos.y-(viiSub::Rand()%8)*m_sScale , m_Pos.z);
	p->SetScale(m_sScale/100.f);

}


void CEffFire::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffFire::SeqEnd()
{
	
	
}


void CEffFire::Draw()
{
	//------------------------------------------------
	//�`��
	//------------------------------------------------

/*
	viiDraw::Sprite(&StSprFireEffect[ m_sSprite ] ,
		 m_Pos.x + m_sOffsetX,
		 m_Pos.y,
		 m_Pos.z,
		 ATR_DEFAULT,
		 ARGB(m_sAlpha,0x80,0x80,0xff),
		 m_sScale/100.0f,0);
*/

}


//------------------------------------------------------------
//
//�G�t�F�N�g�F�΍Ѓp�[�c
//
//------------------------------------------------------------

CEffFireParts::CEffFireParts(Sint32 x , Sint32 y, Sint32 prio )
{
	//----------------------------------------------
	//�R���X�g���N�^
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = prio;

	m_sAlpha = 228- viiSub::Rand()%80;
	m_sScale = 50 + viiSub::Rand()%50;

	Sint32 rot = -100;
	Sint32 spd = 120;

	m_sAdd_X = ( viiMath::Cos100(rot)*spd )/100;
	m_sAdd_Y = ( viiMath::Sin100(rot)*spd )/100;

	m_sRotation = rot-180;

}


CEffFireParts::~CEffFireParts()
{
	//----------------------------------------------
	//�f�X�g���N�^
	//----------------------------------------------


}


void CEffFireParts::SeqInit()
{
	//----------------------------------------------
	//������
	//----------------------------------------------

	SetActionSeq( enActionSeqMain );
}


void CEffFireParts::SeqMain()
{
	//----------------------------------------------
	//���C��
	//----------------------------------------------

	m_sAlpha -= 6;

	if(m_sAlpha <= 0)
	{
		m_sAlpha = 0;
		SetActionSeq( enActionSeqEnd );
		return;
	}

	m_Pos.x += m_sAdd_X;
	m_Pos.y += m_sAdd_Y;

	m_sScale += 1;

}


void CEffFireParts::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffFireParts::SeqEnd()
{
	
	
}


void CEffFireParts::Draw()
{
	//------------------------------------------------
	//�`��
	//------------------------------------------------


	viiDraw::Sprite(&StSprFireEffect[ 2 ] ,
		 m_Pos.x,
		 m_Pos.y,
		 m_Pos.z,
		 ATR_ALPHA_PLUS,
		 ARGB(m_sAlpha,0x80,0x80,0xff),
		 m_sScale/100.0f,m_sRotation);

}





CEffFireSmoke::CEffFireSmoke(Sint32 x , Sint32 y, Sint32 prio )
{
	//----------------------------------------------
	//�R���X�g���N�^
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = prio;

	m_sAlpha = 228- viiSub::Rand()%80;
	m_sScale = 50 + viiSub::Rand()%50;

	Sint32 rot = -100;
	Sint32 spd = 60;

	m_sAdd_X = ( viiMath::Cos100(rot)*spd )/100;
	m_sAdd_Y = ( viiMath::Sin100(rot)*spd )/100;

	m_sRotation = rot-180;

}


CEffFireSmoke::~CEffFireSmoke()
{
	//----------------------------------------------
	//�f�X�g���N�^
	//----------------------------------------------


}


void CEffFireSmoke::SeqInit()
{
	//----------------------------------------------
	//������
	//----------------------------------------------

	SetActionSeq( enActionSeqMain );
}


void CEffFireSmoke::SeqMain()
{
	//----------------------------------------------
	//���C��
	//----------------------------------------------

	m_sAlpha -= 4;

	if(m_sAlpha <= 0)
	{
		m_sAlpha = 0;
		SetActionSeq( enActionSeqEnd );
		return;
	}

	m_Pos.x += m_sAdd_X;
	m_Pos.y += m_sAdd_Y;

	m_sScale += 2;
	m_sRotation += 2;

}


void CEffFireSmoke::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffFireSmoke::SeqEnd()
{
	
	
}


void CEffFireSmoke::Draw()
{
	//------------------------------------------------
	//�`��
	//------------------------------------------------


	viiDraw::Sprite(&StSprFireEffect[ 2 ] ,
		 m_Pos.x,
		 m_Pos.y,
		 m_Pos.z,
		 ATR_ALPHA_MINUS,
		 ARGB(m_sAlpha,0x80,0x80,0xff),
		 m_sScale/100.0f,m_sRotation);

}





