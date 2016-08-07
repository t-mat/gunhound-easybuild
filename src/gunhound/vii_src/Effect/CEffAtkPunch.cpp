//------------------------------------------------------------
//
//�p���`�i�G�l���M�[���j (�p�x,���x,�傫��(%))
//
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

CEffAtkPunch::CEffAtkPunch( Sint32 x,Sint32 y , Sint32 sRot , Sint32 sDist , Sint32 sScale )
{
	//----------------------------------------------
	//�R���X�g���N�^
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;

	m_Pos.x -= GetTargetPlayer()->x;
	m_Pos.y -= GetTargetPlayer()->y;

	m_Disp = m_Pos;

	m_Add.x = (viiMath::Cos100(sRot)*sDist)/100;
	m_Add.y = (viiMath::Sin100(sRot)*sDist)/100;

	m_sScale = sScale;	//�傫��
	m_sAlpha = 255;		//�F����
	m_bHit   = 0;		//��x�������������t���O


}


CEffAtkPunch::~CEffAtkPunch()
{
	//----------------------------------------------
	//�f�X�g���N�^
	//----------------------------------------------
	
	
}


void CEffAtkPunch::SeqMain()
{
	//----------------------------------------------
	//���C��
	//----------------------------------------------

	//----------------------------------------
	//�ړ�
	//----------------------------------------
	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	//----------------------------------------
	//GV�ɂ��킹�ē���
	//----------------------------------------
//	viiSub::GetScroll_s( m_sScroll.x , m_sScroll.y );
//	x  = m_sScroll.x*100 + m_Pos.x;
//	y  = m_sScroll.y*100 + m_Pos.y;

	m_Disp.x  = GetTargetPlayer()->x + m_Pos.x;
	m_Disp.y  = GetTargetPlayer()->y + m_Pos.y;

	//----------------------------------------
	//�ǂւ̓����蔻��
	//----------------------------------------
	if( IsHitKabe() )
	{
		SetActionSeq( enActionSeqCrash );
	}

	//----------------------------------------
	//�F
	//----------------------------------------

	SetColor();

	if(m_sAlpha > 0)
	{
		m_Add.x = 95*m_Add.x/100;
		m_sAlpha -= 16;

		if(m_sAlpha <= 0)
		{
			m_sAlpha = 0;
			SetActionSeq( enActionSeqCrash );
			return;
		}
	}

	//----------------------------------------
	//�����蔻��o�^
	//----------------------------------------
	m_HitAtari.ax1 = -10*m_sScale/100;	//���x���ɉ����Ă����蔻���傫������
	m_HitAtari.ax2 =  10*m_sScale/100;
	m_HitAtari.ay1 =  -8-10*m_sScale/100;
	m_HitAtari.ay2 =   8+10*m_sScale/100;

	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
	m_HitAtari.SetDirectAromorPosition( 1+m_sScale );
	m_HitAtari.SetHantei( ID_VERSIS_ATK,&m_Disp );

	if(m_HitAtari.IsImpacted() && !m_bHit )
	{
		//----------------------------------------
		//�������Ă���Έ�x�����j�Ђ��o��
		//----------------------------------------
		Sint32 rot = 0;
		if(m_Add.x > 0) rot += 180;

		viiEff::Set( EFF_BROKENPARTS , m_Disp.x , m_Disp.y , argset(-45+90*0+rot,300,1) );
		viiEff::Set( EFF_BROKENPARTS , m_Disp.x , m_Disp.y , argset( 45+90*0+rot,300,1) );

		m_bHit = gxTrue;
		viiMus::PlaySound(SE_PUNCHHIT);
		viiSub::SetRumble(20,100);
	}

}


void CEffAtkPunch::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffAtkPunch::SetColor()
{
	//---------------------------------------------------------------
	//�p���`�F�ݒ�
	//---------------------------------------------------------------

	m_sCol_Red = (viiSub::Rand()%2)*240;
	m_sCol_Grn = (viiSub::Rand()%2)*240;
	m_sCol_Blu = (viiSub::Rand()%2)*240;

	if( m_sCol_Red + m_sCol_Grn + m_sCol_Blu ==0)
	{
		m_sCol_Red = m_sCol_Grn = m_sCol_Blu = 0x80;
	}
}

gxBool CEffAtkPunch::IsHitKabe()
{
	//---------------------------------------------------------------
	//�ǃq�b�g
	//---------------------------------------------------------------

	switch(pGame->pBg->get_kabetype( m_Disp.x/100 , m_Disp.y/100)){
	case 1:
	case 12:
	case 13:
	case 14:
	case 15:
		return gxTrue;
	}

	return gxFalse;
}


void CEffAtkPunch::Draw()
{
	//---------------------------------------------------------------
	//�`��
	//---------------------------------------------------------------

	Float32 fScale = m_sScale / 150.0f;

	viiDraw::Sprite( &spr_custom[4] , m_Disp.x , m_Disp.y , PRIO_ATACK , ATR_DFLT      , ARGB(0xff,m_sCol_Red,m_sCol_Grn,m_sCol_Blu )      , fScale , 0 );
	viiDraw::Sprite( &spr_custom[4] , m_Disp.x , m_Disp.y , PRIO_ATACK , ATR_ALPHA_PLUS, ARGB(m_sAlpha*2,m_sCol_Red,m_sCol_Grn,m_sCol_Blu) , fScale ,  m_sTimer*4 );
	viiDraw::Sprite( &spr_custom[4] , m_Disp.x , m_Disp.y , PRIO_ATACK , ATR_ALPHA_PLUS, ARGB(m_sAlpha  ,m_sCol_Red,m_sCol_Grn,m_sCol_Blu) , fScale , -m_sTimer*8 );

}


