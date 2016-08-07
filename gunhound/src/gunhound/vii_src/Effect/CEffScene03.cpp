//------------------------------------------------------------
//
//�G�t�F�N�g�F�}�[�Y�M�A����
//
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

enum {
	enScrollOutWidth  = 32*100,
	enScrollOutHeight = 32*100,
};

enum {
	enSeqNone,
	enSeqBombFirst,
	enSeqBombSecond,
};


extern gxSprite sprMarsGear[]=
{
	{ TPG_ENEMY04,0  ,160 ,  32,32,16,16},	//�}�[�Y�M�A
	{ TPG_ENEMY04,32 ,160 ,  8,8,4,4},		//�}�[�Y�M�A�j�Ёi��j
	{ TPG_ENEMY04,32 ,168 ,  8,8,4,4},		//�}�[�Y�M�A�j�Ёi���j
	{ TPG_ENEMY04,0  ,192  , 48,48,24,24},	//�}�[�Y�M�A����
	{ TPG_ENEMY04,168,152  , 88,104,44,104},//�}�[�Y�M�A�i���̂��_�j

};


CEffScene03::CEffScene03( Sint32 x , Sint32 y , Sint32 z )
{
	//----------------------------------------------
	//�R���X�g���N�^
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_sPrio = z;
	m_sEnsyutuTimer = 0;

	m_bBombCloudDisp = gxFalse;
	m_pMarsGear      = NULL;
	m_sBombSeq       = enSeqNone;
	m_sColor = 0;
	m_sColorAdd = 0;

}


CEffScene03::~CEffScene03()
{
	//----------------------------------------------
	//�f�X�g���N�^
	//----------------------------------------------

	if( m_pMarsGear ) delete m_pMarsGear;

}

void CEffScene03::SeqInit()
{
	//----------------------------------------------
	//������
	//----------------------------------------------

	m_pMarsGear = new CEffExPerticle( m_Pos.x+ 120*100, m_Pos.y+ 80*100 , m_sPrio );
	m_pMarsGear->SetDirection( 70,10 );
	m_pMarsGear->SetSprite( &sprMarsGear[0] );
	m_pMarsGear->SetNoneScroll();
//	m_pMarsGear->SetEnd();

	SetActionSeq( enActionSeqMain );

}


void CEffScene03::SeqMain()
{
	//----------------------------------------------
	//���C��
	//----------------------------------------------

	m_sEnsyutuTimer ++;

	if( m_sBombSeq == enSeqNone )
	{

		if( viiSub::Rand()%12==0 )
		{
			Sint32 nx,ny;
			nx = (viiSub::Rand()%12)*100;
			ny = (viiSub::Rand()%12)*100;

			CEffExPerticle *p;

			p = new CEffExPerticle( m_pMarsGear->m_Pos.x+nx, m_pMarsGear->m_Pos.y+ny , m_sPrio );
			p->SetDirection( 250,20+viiSub::Rand()%20 );
			p->SetAlphaAdd( -1 );
			p->SetSprite( &sprMarsGear[1+viiSub::Rand()%2] );
//			p->SetEnd();
			p->SetNoneScroll();
		}

		m_pMarsGear->m_sAtribute = ATR_DFLT;

		if( viiSub::Rand()%12 == 0 )
		{
			m_pMarsGear->m_sAtribute = ATR_ALPHA_PLUS;
		}

	}
	else if( m_sBombSeq == enSeqBombFirst )
	{
		CEffExPerticle *p;

		for( Sint32 j=0;j<3;j++)
		{
			Sint32 nx,ny;
			nx = (-32+viiSub::Rand()%64)*100;
			ny = (-32+viiSub::Rand()%64)*100;
			p = new CEffExPerticle( m_pMarsGear->m_Pos.x+nx, m_pMarsGear->m_Pos.y+ny , m_sPrio );
			p->SetAlphaAdd( -8 );
			p->SetSprite( &sprMarsGear[3] );
			p->m_sAtribute = ATR_ALPHA_PLUS;
			p->m_sRotation = viiSub::Rand()%360;
			p->SetNoneScroll();
			//p->SetEnd();
		}

		if ( m_sColor >= 255 )
		{
			m_sColorAdd = -1;
			delete m_pMarsGear;

			m_pMarsGear      = NULL;
			m_bBombCloudDisp = gxTrue;
			m_sBombSeq       = enSeqBombSecond;
		}

	}
	else if( m_sBombSeq == enSeqBombSecond )
	{
		
	}
	
	//-----------------------------------------------------------
	//���O�t�F�[�h�����i�v���C�I���e�B������Ȃ̂Ŏ��O�ł��j
	//-----------------------------------------------------------
	m_sColor += m_sColorAdd;

	if(m_sColor >= 255) m_sColor = 255;
	if(m_sColor <= 0  ) m_sColor = 0;
	viiDraw::Box(0,0,SCR_W,SCR_H,PRIO_BG2+1,gxTrue,ARGB(m_sColor,0xFF,0xFF,0xFF));

//	msg_prSint32f (100,200,PRIO_DEBUG,"���o�^�C�}�[[%d]",m_sEnsyutuTimer);

}




void CEffScene03::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffScene03::SeqEnd()
{
	
	
}


void CEffScene03::Draw()
{
	//----------------------------------------------
	//�`��
	//----------------------------------------------

	if( m_bBombCloudDisp )
	{
		viiDraw::Spr_NoScrl( &sprMarsGear[4], m_Pos.x+220*100 , m_Pos.y+240*100 , m_sPrio );
	}


}


void CEffScene03::SetBombStart()
{
	//----------------------------------------------
	//�����Z�b�g
	//----------------------------------------------

	m_sColorAdd = 2;
	m_sBombSeq = enSeqBombFirst;

}


