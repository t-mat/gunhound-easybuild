//------------------------------------------------------------
//
//�G�t�F�N�g�F����
//�@�X�e�[�W�S�F�X�e�[�W�V�Ŏg�p
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

enum {
	enScrollOutWidth  = 64*100,
	enScrollOutHeight = 64*100,
};

gxSprite stSprGakeSmoke[]={
	{ enTexPageEffectCommon02,256-32,0 ,32,32,16,16},
	{ enTexPageEffectCommon02,256-64,32,32,32,16,16},
	{ enTexPageEffectCommon02,256-32,32,32,32,16,16},
};


extern gxSprite spr_cloud[];

CEffSandStorm::CEffSandStorm(Sint32 x , Sint32 y, Sint32 rot , Sint32 Speed )
{
	//----------------------------------------------
	//�R���X�g���N�^
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	if( Speed <= 0) Speed = 2;	//���S�\�t�g
	Sint32 spd = Speed/2+viiSub::Rand()%(Speed/2);

	m_sAdd_X = ( viiMath::Cos100(rot)*spd )/100;
	m_sAdd_Y = ( viiMath::Sin100(rot)*spd )/100;

	m_sRotation = viiSub::Rand()%360;
	m_sScale    = 100;

}


CEffSandStorm::~CEffSandStorm()
{
	//----------------------------------------------
	//�f�X�g���N�^
	//----------------------------------------------


}


void CEffSandStorm::SeqInit()
{
	//----------------------------------------------
	//������
	//----------------------------------------------

	m_sAlpha = 158- viiSub::Rand()%30;
	m_sScale = 50 + viiSub::Rand()%50;

	SetActionSeq( enActionSeqMain );
}


void CEffSandStorm::SeqMain()
{
	//----------------------------------------------
	//���C��
	//----------------------------------------------


	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	m_sAlpha -= 2;

	if(m_sAlpha <= 0)
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	m_Pos.x += m_sAdd_X;
	m_Pos.y += m_sAdd_Y;

	m_sScale    += 1;
	m_sRotation += 2;

	viiSub::SetGravitMove( m_Pos.x , m_Pos.y );

}


void CEffSandStorm::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffSandStorm::SeqEnd()
{
	
	
}


void CEffSandStorm::Draw()
{
	//------------------------------------------------
	//�`��
	//------------------------------------------------

	viiDraw::Sprite(&spr_cloud[ 0 ],m_Pos.x,m_Pos.y,PRIO_EFFECT,ATR_DFLT,ARGB(m_sAlpha,0xff,0xff,0xff),m_sScale/100.0f,m_sRotation);


}


//------------------------------------------------------------
//
//�G�t�F�N�g�F�R����
//�n�E���h�X�e�[�W�R�Ŏg�p
//------------------------------------------------------------

CEffGakeSmoke::CEffGakeSmoke( enSmokeType eType , Sint32 x,Sint32 y, Sint32 rot , Sint32 speed )
{
	//----------------------------------------------
	//�R���X�g���N�^
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	if( speed <= 0) speed = 2;	//���S�\�t�g
	Sint32 spd = speed/2+viiSub::Rand()%(speed/2);

	m_sAdd_X = ( viiMath::Cos100(rot)*spd )/100;
	m_sAdd_Y = ( viiMath::Sin100(rot)*spd )/100;

	m_sRotation = 0;//viiSub::Rand()%360;
	m_sScale    = 100;
	m_enType    = eType;

	m_sAlpha = 158- viiSub::Rand()%30;
	m_sScale = 50 + viiSub::Rand()%50;

	switch(m_enType){
	case enSmokeTypeSlash:
		m_sScale    = 100;
		break;
	case enSmokeTypeWhite:
		m_sScale    = 140-viiSub::Rand()%30;
		break;
	case enSmokeTypeBrown:
		m_sScale    = 50+viiSub::Rand()%80;
		break;
	}

}


CEffGakeSmoke::~CEffGakeSmoke()
{
	//----------------------------------------------
	//�f�X�g���N�^
	//----------------------------------------------


}


void CEffGakeSmoke::SeqInit()
{
	//----------------------------------------------
	//������
	//----------------------------------------------

	SetActionSeq( enActionSeqMain );
}


void CEffGakeSmoke::SeqMain()
{
	//----------------------------------------------
	//���C��
	//----------------------------------------------


	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	m_Pos.x += m_sAdd_X;
	m_Pos.y += m_sAdd_Y;

	switch(m_enType){
	case enSmokeTypeSlash:
		m_sAlpha -= 8;
		m_sScale += 1;
		m_sRotation += 2;
		break;

	case enSmokeTypeWhite:
		m_sScale -= 1;
		m_sAlpha -= 3;
		break;
	case enSmokeTypeBrown:
		m_sScale += 4;
		m_sAlpha -= 1;
		break;
	}

	m_sScale    += 1;

	if(m_sAlpha <= 0)
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	viiSub::SetGravitMove( m_Pos.x , m_Pos.y );

}


void CEffGakeSmoke::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffGakeSmoke::SeqEnd()
{
	
	
}


void CEffGakeSmoke::Draw()
{
	//------------------------------------------------
	//�`��
	//------------------------------------------------

	switch(m_enType){
	case enSmokeTypeSlash:
		viiDraw::Sprite(&stSprGakeSmoke[ 1 ],m_Pos.x,m_Pos.y,PRIO_EFFECT,ATR_DFLT,ARGB(m_sAlpha/2,0xff,0xff,0xff),m_sScale/100.0f,m_sRotation);
		break;
	case enSmokeTypeWhite:
//		viiDraw::Sprite(&stSprGakeSmoke[ 1 ],m_Pos.x,m_Pos.y,PRIO_EFFECT,ATR_ALPHA_PLUS,ARGB(m_sAlpha,0xff,0xff,0xff),m_sScale/100.0f,m_sRotation);
		viiDraw::Sprite(&stSprGakeSmoke[ 2 ],m_Pos.x,m_Pos.y,PRIO_BG2,ATR_DFLT,ARGB(m_sAlpha/2,0xff,0xff,0xff),m_sScale/100.0f,m_sRotation);
		break;
	case enSmokeTypeBrown:
		viiDraw::Sprite(&stSprGakeSmoke[ 2 ],m_Pos.x,m_Pos.y,PRIO_BG2,ATR_DFLT,ARGB(m_sAlpha/2,0xff,0xff,0xff),m_sScale/100.0f,m_sRotation);
		break;
	}

}



//------------------------------------------------------------
//
//�G�t�F�N�g�F�_�b�V���p�X���[�N
//
//------------------------------------------------------------

CEffDashStorm::CEffDashStorm(Sint32 x , Sint32 y, Sint32 rot , Sint32 Speed )
{
	//----------------------------------------------
	//�R���X�g���N�^
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	if( Speed <= 0) Speed = 2;	//���S�\�t�g
	Sint32 spd = Speed/2+viiSub::Rand()%(Speed/2);

	m_sAdd_X = ( viiMath::Cos100(rot)*spd )/100;
	m_sAdd_Y = ( viiMath::Sin100(rot)*spd )/100;

	m_sRotation = viiSub::Rand()%360;
	m_sScale    = 40;

}


CEffDashStorm::~CEffDashStorm()
{
	//----------------------------------------------
	//�f�X�g���N�^
	//----------------------------------------------


}


void CEffDashStorm::SeqInit()
{
	//----------------------------------------------
	//������
	//----------------------------------------------

	m_sAlpha = (158- viiSub::Rand()%60);
	m_sScale = 10 + viiSub::Rand()%20;

	SetActionSeq( enActionSeqMain );
}


void CEffDashStorm::SeqMain()
{
	//----------------------------------------------
	//���C��
	//----------------------------------------------


	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	m_sAlpha -= 4;

	if(m_sAlpha <= 0)
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	m_Pos.x += m_sAdd_X;
	m_Pos.y += m_sAdd_Y;

	m_sScale    += 1;
	m_sRotation += 2;

	viiSub::SetGravitMove( m_Pos.x , m_Pos.y );

}


void CEffDashStorm::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffDashStorm::SeqEnd()
{
	
	
}


void CEffDashStorm::Draw()
{
	//------------------------------------------------
	//�`��
	//------------------------------------------------

	viiDraw::Sprite(&spr_cloud[ 0 ],m_Pos.x,m_Pos.y,PRIO_EFFECT,ATR_DFLT,ARGB(m_sAlpha,0xff,0xff,0xff),m_sScale/100.0f,m_sRotation);

}


//------------------------------------------------------------
//
//�G�t�F�N�g�F�_�b�V���p�X���[�N
//
//------------------------------------------------------------

CEffBreakStorm::CEffBreakStorm(Sint32 x , Sint32 y, Sint32 rot , Sint32 Speed )
{
	//----------------------------------------------
	//�R���X�g���N�^
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	if( Speed <= 0) Speed = 2;	//���S�\�t�g
	Sint32 spd = Speed/2+viiSub::Rand()%(Speed/2);

	m_sAdd_X = ( viiMath::Cos100(rot)*spd )/100;
	m_sAdd_Y = ( viiMath::Sin100(rot)*spd )/100;

	m_sRotation = viiSub::Rand()%360;
	m_sScale    = 40;

}


CEffBreakStorm::~CEffBreakStorm()
{
	//----------------------------------------------
	//�f�X�g���N�^
	//----------------------------------------------


}


void CEffBreakStorm::SeqInit()
{
	//----------------------------------------------
	//������
	//----------------------------------------------

	m_sAlpha = (158- viiSub::Rand()%60);
	m_sScale = 10 + viiSub::Rand()%20;

	SetActionSeq( enActionSeqMain );
}


void CEffBreakStorm::SeqMain()
{
	//----------------------------------------------
	//���C��
	//----------------------------------------------


	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	m_sAlpha -= 4;

	if(m_sAlpha <= 0)
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	m_Pos.x += m_sAdd_X;
	m_Pos.y += m_sAdd_Y;

	m_sScale    += 1;
	m_sRotation += 2;

	viiSub::SetGravitMove( m_Pos.x , m_Pos.y );

}


void CEffBreakStorm::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffBreakStorm::SeqEnd()
{
	
	
}


void CEffBreakStorm::Draw()
{
	//------------------------------------------------
	//�`��
	//------------------------------------------------

	viiDraw::Sprite(&spr_cloud[ 0 ],m_Pos.x,m_Pos.y,PRIO_EFFECT,ATR_ALPHA_MINUS,ARGB( (m_sAlpha/2),0xff,0xff,0xff),m_sScale/100.0f,m_sRotation);


}


//------------------------------------------------------------
//
//�X�R�A�\��
//
//------------------------------------------------------------

CEffScore::CEffScore( Sint32 x,Sint32 y, Sint32 score )
{
	m_Pos.x = x;
	m_Pos.y = y;
	
	m_Add.x = 0;
	m_Add.y = 160;

	m_sScore = score;

	Sint32 keta = s_cast<Sint32>(log10(score*1.f));
	keta ++;
	m_Pos.x -= keta*400;

	if( CCockpit::GetInstance()->GetScoreBairitsu() >= 2 )
	{
		m_Pos.x -= 2*400;
	}

	m_WaitTimer = 0;
}


CEffScore::~CEffScore()
{
	
	
}

void CEffScore::SeqMain()
{
	m_Add.y += -m_Add.y/10;
	m_Pos.y -= m_Add.y;

	m_WaitTimer ++;

	if( m_WaitTimer >= 120)
	{
		SetActionSeq( enActionSeqEnd );
	}
}

void CEffScore::Draw()
{
	Sint32 sx,sy,x,y;
	viiSub::GetScroll_s( sx,sy );

	x = m_Pos.x;
	y = m_Pos.y;

	x = x/100 - sx;
	y = y/100 - sy;

	if( CCockpit::GetInstance()->GetScoreBairitsu() >=2 )
	{
		viiDraw::sml_printf(x,y,0xffffffff,"%dx2",m_sScore);
	}
	else
	{
		viiDraw::sml_printf(x,y,0xffffffff,"%d",m_sScore);
	}
}


