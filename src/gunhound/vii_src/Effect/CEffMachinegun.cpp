//------------------------------------------------------------
//
//�G�t�F�N�g�F�}�V���K��
//
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"
#include "../enemy/enemy.h"

gxSprite stSprGatLight[]={
	{TPG_CLOUD,64,32,32,16,16,8},
};

gxSprite stSprGattling[]={
	{TPG_EFFECT,0+16*0,176,16,16,8,8},
	{TPG_EFFECT,0+16*1,176,16,16,8,8},
	{TPG_EFFECT,0+16*2,176,16,16,8,8},
	{TPG_EFFECT,0+16*3,176,16,16,8,8},
	{TPG_EFFECT,0+16*4,176,16,16,8,8},
	{TPG_EFFECT,0+16*5,176,16,16,8,8},
	{TPG_EFFECT,0+16*6,176,16,16,8,8},
	{TPG_EFFECT,0+16*7,176,16,16,8,8},
	{TPG_EFFECT,0+16*8,176,16,16,8,8},

};


gxSprite stSprNapalm[] = {
	{TPG_CLOUD,64,0,64,64,32,32},	//��
	{TPG_CLOUD,64,0,32,32,16,16},	//��
};


CEffMachinegun::CEffMachinegun(Sint32 x , Sint32 y, Sint32 rot , Sint32 lv , Sint32 reboundCnt )
{
	//----------------------------------------------
	//�R���X�g���N�^
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;

	m_sAdd_X = viiMath::Cos100(rot)*10;
	m_sAdd_Y = viiMath::Sin100(rot)*10;

	m_sLevel = lv;
	m_sCount = reboundCnt;

	m_HitAtari.set_hp(1);
	m_HitAtari.set_ap(2);
}


CEffMachinegun::~CEffMachinegun()
{
	//----------------------------------------------
	//�f�X�g���N�^
	//----------------------------------------------
	
	
}


void CEffMachinegun::SeqInit()
{

	m_HitAtari.ax1 = -4+(-m_sLevel*2);
	m_HitAtari.ay1 = -4+(-m_sLevel*2);
	m_HitAtari.ax2 =  4+(+m_sLevel*2);
	m_HitAtari.ay2 =  4+(+m_sLevel*2);

	SetActionSeq( enActionSeqMain );

}


void CEffMachinegun::SeqMain()
{
	//----------------------------------------------
	//�}�V���K�����C��
	//----------------------------------------------

	if(m_HitAtari.IsImpacted() || viiSub::GetBoundBlock_Player( m_Pos.x , m_Pos.y ) )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	//���݃`�F�b�N
	if(IsScrollOut( 0,0) )
	{
		SetActionSeq( enActionSeqEnd );
	}

	if( Reflect( ) )
	{
		//----------------------------------------------
		//���˂���
		//----------------------------------------------

		viiEff::Set(EFF_HITFLASH , m_Pos.x , m_Pos.y , argset(0) );

		m_sCount --;

		if( m_sCount == 0 )
		{
			//���ˉ񐔌��E�܂ł�����
			SetActionSeq( enActionSeqCrash );
			return;
		}
	}

	m_HitAtari.SetHantei( ID_VERSIS_ATK , &m_Pos );
	m_HitAtari.SetDirectAromorPosition( 150+m_sLevel*20 );

}


void CEffMachinegun::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	viiEff::Set( EFF_HITFLASH , m_Pos.x , m_Pos.y , argset(1) );

	SetActionSeq( enActionSeqEnd );

}


void CEffMachinegun::SeqEnd()
{
	
	
}


gxBool CEffMachinegun::Reflect()
{
	//------------------------------
	//���e����
	//x_,y_,xx_,yy_��ύX���܂�
	//------------------------------

	Sint32 x1,y1,x2,y2;

	Sint32 ax  = m_sAdd_X;
	Sint32 ay = m_sAdd_Y;

	gxBool bHitFlag = gxFalse;

	x1 = m_Pos.x;
	y1 = m_Pos.y;

	x2 = x1 + ax;
	y2 = y1 + ay;

	//-----------------------------------------------------
	//���B�_���ǂ��ǂ����𒲂ׂ�
	//-----------------------------------------------------
	Sint32 sType = viiSub::GetBoundBlock_Player( x2,y2 );

	if(sType)
	{
		//-----------------------------------------------------
		//�ǂɂ�������
		//-----------------------------------------------------
		Sint32 px,py,sLength;
		Sint32 qx=x1/100,qy=y1/100;
		bHitFlag = gxTrue;

		//-----------------------------------------------------
		//�ړ����C������ړ��������Z�o
		//-----------------------------------------------------
		sLength = viiMath::LineLogic( x1/100 , y1/100 , x2/100 , y2/100 , -1 , px , py );

		for( Sint32 i=0 ; i<=sLength ; i++)
		{
			//-----------------------------------------------------
			//�ŏ��ɂ��������ꏊ����������
			//-----------------------------------------------------
			viiMath::LineLogic( x1/100 , y1/100 , x2/100 , y2/100 , i , px , py );

			sType = viiSub::GetBoundBlock_Player( px*100,py*100 );
			if( sType )
			{
				break;
			}
			qx = px;
			qy = py;
		}

		//-----------------------------------------------------
		//�o�E���f�B���O���������肷��
		//-----------------------------------------------------
		Sint32 zz;

		switch( sType ){
		case enKabeTypeBlock:
			if( viiSub::GetBoundBlock_Player( x2 , y1 ) )
			{
				//���ł�������
				ax *= -1;
			}
			else if( viiSub::GetBoundBlock_Player( x1 , y2 ) )
			{
				//�c�ł�������
				ay *= -1;
			}
			else
			{
				//�΂߂ł�������
				ax *= -1;
				ay *= -1;
			}
			x2 = qx*100;
			y2 = qy*100;
			break;

		case enKabeTypeTriangleUL:
		case enKabeTypeTriangleDR:
			x2 = qx*100;
			y2 = qy*100;
			zz  =  ax;
			ax =  -ay;
			ay = -zz;
			break;

		case enKabeTypeTriangleUR:
		case enKabeTypeTriangleDL:
			x2 = qx*100;
			y2 = qy*100;
			zz  = ax;
			ax = ay;
			ay = zz;
			break;
		}
	}

	m_Pos.x = x2;
	m_Pos.y = y2;

	m_sAdd_X = ax;
	m_sAdd_Y = ay;

	return bHitFlag;

}


void CEffMachinegun::Draw()
{
	//------------------------------------------------
	//�`��
	//------------------------------------------------

	float fScl = ( 100+(m_sLevel*50) )/100.0f;

	viiDraw::Sprite( &spr_custom[SPR_BULLET] , m_Pos.x , m_Pos.y , PRIO_ATACK , ATR_DEFAULT , ARGB_DFLT , fScl , 0 );
}




//------------------------------------------------------------
//
//�G�t�F�N�g�F�K�g�����O�K��
//
//------------------------------------------------------------

CEffGattlingGun::CEffGattlingGun(Sint32 x , Sint32 y, Sint32 sRot , Sint32 sSpd )
{
	//----------------------------------------------
	//�R���X�g���N�^
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;

	m_Add.x = (viiMath::Cos100(sRot)*sSpd)/100;
	m_Add.y = (viiMath::Sin100(sRot)*sSpd)/100;

	m_HitAtari.ax1 = -4+(-1*2);
	m_HitAtari.ay1 = -4+(-1*2);
	m_HitAtari.ax2 =  4+(+1*2);
	m_HitAtari.ay2 =  4+(+1*2);

	m_sAtackPower = enAtkPointMachineGun;

	m_HitAtari.set_hp(1);
	m_HitAtari.set_ap( m_sAtackPower );
	SetActionSeq( enActionSeqMain );

	m_sRotation = sRot;

	m_bKantsu = gxFalse;
}


CEffGattlingGun::~CEffGattlingGun()
{
	//----------------------------------------------
	//�f�X�g���N�^
	//----------------------------------------------
	
	
}


void CEffGattlingGun::SeqInit()
{


	SetActionSeq( enActionSeqMain );

}


void CEffGattlingGun::SeqMain()
{
	//----------------------------------------------
	//�}�V���K�����C��
	//----------------------------------------------

	if(m_HitAtari.IsImpacted() )
	{
		viiEff::Set( EFF_HITFLASH , m_Pos.x , m_Pos.y , argset(1) );
		if( !m_bKantsu )
		{
			SetActionSeq( enActionSeqCrash );
			return;
		}
		else
		{
			//�ђʒe�̎��͏����Ȃ�
			m_HitAtari.m_bImpact = gxFalse;
			m_HitAtari.set_hp(1);
			m_HitAtari.set_ap( m_sAtackPower );
		}
	}
	else if( viiSub::GetBoundBlock_Player( m_Pos.x , m_Pos.y ) )
	{
		viiEff::Set( EFF_HITFLASH , m_Pos.x , m_Pos.y , argset(0) );
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( pGame->GetMarineLine() )
	{
		if( m_Pos.y > pGame->GetMarineLine() )
		{
			new CEffWater( m_Pos.x , m_Pos.y, enTypeSplashSmall , 0);
			SetActionSeq( enActionSeqEnd );
			return;
		}
	}

	//���݃`�F�b�N
	if(IsScrollOut( 0,0) )
	{
		SetActionSeq( enActionSeqEnd );
	}

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	m_HitAtari.set_ap( m_sAtackPower );
	m_HitAtari.SetHantei( ID_VERSIS_ATK , &m_Pos );

}


void CEffGattlingGun::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffGattlingGun::SeqEnd()
{
	
	
}



void CEffGattlingGun::Draw()
{
	//------------------------------------------------
	//�`��
	//------------------------------------------------

	float fScl = 0.7f+m_sTimer/150.f;
	Sint32 sSpr = (m_sTimer/2)%9;
	gxSprite *p = &stSprGattling[sSpr];
	Sint32 rot = (m_sTimer*16)%360;

	viiDraw::Sprite( p , m_Pos.x , m_Pos.y , PRIO_ATACK , ATR_DFLT);
	viiDraw::Sprite( &stSprGatLight[0] , m_Pos.x , m_Pos.y , PRIO_ATACK , ATR_ALPHA_PLUS , 0xF0808080 , 1.0f,rot);

//	viiDraw::Sprite( p , m_Pos.x , m_Pos.y , PRIO_ATACK , ATR_DFLT ,0xffffffff,0.25f);
//	viiDraw::Sprite( &stSprGatLight[0] , m_Pos.x , m_Pos.y , PRIO_ATACK , ATR_ALPHA_PLUS , 0xff808080 , 0.25f,rot);
}




//------------------------------------------------------------
//
//�G�t�F�N�g�F�K�g�����O�K��
//
//------------------------------------------------------------

CEffNapalmGun::CEffNapalmGun(Sint32 x , Sint32 y, Sint32 sRot , Sint32 sSpd )
{
	//----------------------------------------------
	//�R���X�g���N�^
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;

	m_sSpeed = sSpd;

	m_Add.x = viiMath::Cos100(sRot);
	m_Add.y = viiMath::Sin100(sRot);

	m_HitAtari.ax1 = -12;
	m_HitAtari.ay1 = -12;
	m_HitAtari.ax2 =  12;
	m_HitAtari.ay2 =  12;

	m_sAtackPower = 1;

	m_HitAtari.set_hp(1);
	m_HitAtari.set_ap( m_sAtackPower );
	SetActionSeq( enActionSeqMain );

	m_sRotation = viiSub::Rand()%360;

	for(Sint32 ii=0;ii<enOldMax;ii++)
	{
		m_Old[ii].x = x;
		m_Old[ii].y = y;
	}

	m_sOldCnt = 0;
	m_sAlpha = 200+viiSub::Rand()%20;
	m_fScale = (50+(viiSub::Rand()%20))/100.f;

}


CEffNapalmGun::~CEffNapalmGun()
{
	//----------------------------------------------
	//�f�X�g���N�^
	//----------------------------------------------
	
	
}


void CEffNapalmGun::SeqInit()
{


	SetActionSeq( enActionSeqMain );

}


void CEffNapalmGun::SeqMain()
{
	//----------------------------------------------
	//�}�V���K�����C��
	//----------------------------------------------

	if(m_HitAtari.IsImpacted() )
	{
		//viiEff::Set( EFF_HITFLASH , m_Pos.x , m_Pos.y , argset(1) );
		//SetActionSeq( enActionSeqCrash );
		//return;
	}
//	else if( viiSub::GetBoundBlock_Player( m_Pos.x , m_Pos.y ) )
//	{
//		//viiEff::Set( EFF_HITFLASH , m_Pos.x , m_Pos.y , argset(0) );
//		SetActionSeq( enActionSeqCrash );
//		return;
//	}

	if( pGame->GetMarineLine() )
	{
		if( m_Pos.y > pGame->GetMarineLine() )
		{
			new CEffWater( m_Pos.x , m_Pos.y, enTypeSplashSmall , 0);
			SetActionSeq( enActionSeqEnd );
			return;
		}
	}

	//���݃`�F�b�N
	if(IsScrollOut( 0,0) )
	{
		SetActionSeq( enActionSeqEnd );
	}

	//�������Ă���
	m_sAlpha -= 0x04;
	if( m_sAlpha <= 0 )
	{
		m_sAlpha = 0x00;
		SetActionSeq( enActionSeqEnd );
		return;
	}

	//�ߋ����W�Ƃ��ĕۑ�����
	m_Old[m_sOldCnt].x = m_Pos.x;
	m_Old[m_sOldCnt].y = m_Pos.y;
	m_sOldCnt ++;
	m_sOldCnt = m_sOldCnt%enOldMax;

	//���W�X�V
	Sint32 ax,ay;
	ax = ( m_Add.x *m_sSpeed)/100;
	ay = ( m_Add.y *m_sSpeed)/100;

	m_Pos.x += ax;
	m_Pos.y += ay;

	//����
//	if( m_Add.y < 800 ) m_Add.y += 2;
//	if( m_Add.y > -800 ) m_Add.y -= 2;
	if( m_sSpeed > 0 )
	{
		m_sSpeed -= 20;
		if( m_sSpeed < 0 )
		{
			m_sSpeed = 0;
		}
	}

	m_HitAtari.set_hp(1);
	m_HitAtari.SetDirectAromorPosition(35);	//.set_ap( m_sAtackPower );
	m_HitAtari.SetHantei( ID_VERSIS_ATK , &m_Pos );

	m_sRotation += 8;
	m_fScale += 0.015f;
}


void CEffNapalmGun::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffNapalmGun::SeqEnd()
{
	
	
}



void CEffNapalmGun::Draw()
{
	//------------------------------------------------
	//�`��
	//------------------------------------------------

//	float fScl = 0.7f+m_sTimer/150.f;
//	Sint32 sSpr = (m_sTimer/2)%9;
//	gxSprite *p = &stSprGattling[sSpr];
//	Sint32 rot = (m_sTimer*16)%360;
//	viiDraw::Sprite( p , m_Pos.x , m_Pos.y , PRIO_ATACK , ATR_DFLT);

	viiDraw::Sprite( &stSprNapalm[1] , m_Pos.x , m_Pos.y , PRIO_ATACK , ATR_ALPHA_PLUS , ARGB(m_sAlpha, 0xff,0xff,0xff)  );//, 1.0f,rot);

	for(Sint32 ii=0;ii<enOldMax;ii++)
	{
		Sint32 ax,ay,n;

		n = (enOldMax+m_sOldCnt-ii)%enOldMax;

		ax = m_Old[n].x;
		ay = m_Old[n].y;

		//viiDraw::Sprite( &stSprGatLight[0] , ax , ay , PRIO_ATACK , ATR_ALPHA_PLUS , 0xF0808080 );//, 1.0f,rot);
		viiDraw::Sprite( &stSprNapalm[1] , ax , ay , PRIO_ATACK , ATR_ALPHA_PLUS , ARGB(m_sAlpha, 0xff,0xff,0xff) , m_fScale , m_sRotation );
	}

}


//------------------------------------------------------------
//
//�G�t�F�N�g�F�V���b�g�K��
//
//------------------------------------------------------------

CEffShotGun::CEffShotGun(Sint32 x , Sint32 y, Sint32 sRot , Sint32 sSpd )
{
	//----------------------------------------------
	//�R���X�g���N�^
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;

	m_Add.x = (viiMath::Cos100(sRot)*sSpd)/100;
	m_Add.y = (viiMath::Sin100(sRot)*sSpd)/100;

	m_HitAtari.ax1 = -4+(-1*2);
	m_HitAtari.ay1 = -4+(-1*2);
	m_HitAtari.ax2 =  4+(+1*2);
	m_HitAtari.ay2 =  4+(+1*2);

	m_HitKurai.ax1 = -6;
	m_HitKurai.ay1 = -6;
	m_HitKurai.ax2 =  12;
	m_HitKurai.ay2 =  12;

	m_sAtackPower = enAtkPointMachineGun;

	m_HitAtari.set_hp(1);
	m_HitAtari.set_ap( m_sAtackPower );

	m_HitKurai.set_hp(1);
	m_HitKurai.set_ap( m_sAtackPower );

	SetActionSeq( enActionSeqMain );

	m_sRotation = sRot;

	m_bKantsu = gxFalse;
}


CEffShotGun::~CEffShotGun()
{
	//----------------------------------------------
	//�f�X�g���N�^
	//----------------------------------------------
	
	
}


void CEffShotGun::SeqInit()
{


	SetActionSeq( enActionSeqMain );

}


void CEffShotGun::SeqMain()
{
	//----------------------------------------------
	//�V���b�g�K�����C��
	//----------------------------------------------

	if(m_HitAtari.IsImpacted() )
	{
		viiEff::Set( EFF_HITFLASH , m_Pos.x , m_Pos.y , argset(1) );
		if( !m_bKantsu )
		{
			SetActionSeq( enActionSeqCrash );
			return;
		}
		else
		{
			//�ђʒe�̎��͏����Ȃ�
			m_HitAtari.m_bImpact = gxFalse;
			m_HitAtari.set_hp(1);
			m_HitAtari.set_ap( m_sAtackPower );
		}
	}
	else if( viiSub::GetBoundBlock_Player( m_Pos.x , m_Pos.y ) )
	{
		viiEff::Set( EFF_HITFLASH , m_Pos.x , m_Pos.y , argset(0) );
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( pGame->GetMarineLine() )
	{
		if( m_Pos.y > pGame->GetMarineLine() )
		{
			new CEffWater( m_Pos.x , m_Pos.y, enTypeSplashSmall , 0);
			SetActionSeq( enActionSeqEnd );
			return;
		}
	}

	//���݃`�F�b�N
	if(IsScrollOut( 0,0) )
	{
		SetActionSeq( enActionSeqEnd );
	}

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	m_HitAtari.set_ap( m_sAtackPower );
	m_HitAtari.SetHantei( ID_VERSIS_ATK , &m_Pos );

	m_HitKurai.set_hp(1);
	m_HitKurai.SetHantei( ID_VERSIS_DEF , &m_Pos );

}


void CEffShotGun::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffShotGun::SeqEnd()
{
	
	
}



void CEffShotGun::Draw()
{
	//------------------------------------------------
	//�`��
	//------------------------------------------------

	float fScl = 0.7f+m_sTimer/150.f;
	Sint32 sSpr = (m_sTimer/2)%9;
	gxSprite *p = &stSprGattling[sSpr];
	Sint32 rot = (m_sTimer*16)%360;

	viiDraw::Sprite( p , m_Pos.x , m_Pos.y , PRIO_ATACK , ATR_DFLT , 0xffffffff , 1.5f , rot );
	viiDraw::Sprite( &stSprGatLight[0] , m_Pos.x , m_Pos.y , PRIO_ATACK , ATR_ALPHA_PLUS , 0xF0f0a0f0 , 1.5f,rot);

}






