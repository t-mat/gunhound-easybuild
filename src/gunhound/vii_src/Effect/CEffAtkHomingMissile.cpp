//------------------------------------------------------------
//
//�p���`�i�G�l���M�[���j (�p�x,���x,�傫��(%))
//
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

CEffAtkHomingMissile::CEffAtkHomingMissile( Sint32 x,Sint32 y , Sint32 sRot , Sint32 sLevel )
{
	//----------------------------------------------
	//�R���X�g���N�^
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;

	m_sRotation        = sRot;				//��������
	m_sRotationAdd     = (3+viiSub::Rand()%3)*(viiSub::Rand()%2)? 4 : -4;
	m_sRotationMaster  = sRot;
	m_sHomingLevel     = sLevel;			//�z�[�~���O���x��
	m_sHomingWait      = 30+viiSub::Rand()%20;	//�z�[�~���O�J�n����
	m_sHomingAdjustSpd = -3+viiSub::Rand()%7;	//�O���C���i���炵�j���x��
	m_sSpeed = 300;							//�X�s�[�h

	m_HitAtari.set_hp(1);

	m_Add.x = (viiMath::Cos100(m_sRotation)*m_sSpeed)/100;
	m_Add.y = (viiMath::Sin100(m_sRotation)*m_sSpeed)/100;

	m_HitAtari.ax1 = -6;
	m_HitAtari.ay1 = -6;
	m_HitAtari.ax2 =  6;
	m_HitAtari.ay2 =  6;

}


CEffAtkHomingMissile::~CEffAtkHomingMissile()
{
	//----------------------------------------------
	//�f�X�g���N�^
	//----------------------------------------------
	
	
}


void CEffAtkHomingMissile::SeqMain()
{
	//----------------------------------------------
	//���C��
	//----------------------------------------------

	//�z�[�~���O
	gxBool bHoming = gxFalse;

	if(m_sHomingWait > 0)
	{
		//�z�[�~���O�J�n�܂ł̎��Ԑ���
		m_sHomingWait--;
	}

	Sint32 sRangeScreenOut = 0;	//��ʊO���e�͈�

	switch(m_sHomingLevel){
	case 0:
		//�z�[�~���O���\�i�V
		m_HitAtari.set_ap(5);
		missileLv1();
		sRangeScreenOut = 0*100;
		break;
	case 1:
		//�k�u�Q�ȈՃz�[�~���O�i�ǔ���Ȃ��j
		m_HitAtari.set_ap(8);
		sRangeScreenOut = 32*100;
		missileLv2();
		break;
	default:
		//�k���R�z�[�~���O�i�ǔ���Ȃ��j
		m_HitAtari.set_ap(10);
		sRangeScreenOut = 64*100;
		missileLv3();
		break;
	}

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	viiEff::Set(EFF_MISSILE_SMOKE,m_Pos.x           , m_Pos.y           , NULL );
	viiEff::Set(EFF_MISSILE_SMOKE,m_Pos.x+m_Add.x/2 , m_Pos.y+m_Add.y/2 , NULL );

	//���݃`�F�b�N
	if(viiSub::IsScrollOut( m_Pos.x , m_Pos.y , sRangeScreenOut , sRangeScreenOut ) )
	{
		SetActionSeq( enActionSeqCrash );
	}

	if( m_HitAtari.IsImpacted() )
	{
		viiEff::SetBombEffect( m_Pos.x , m_Pos.y , 3 , 5 );
		SetActionSeq( enActionSeqCrash );
		return;
	}

	m_HitAtari.SetHantei( ID_VERSIS_ATK,&m_Pos );

}


void CEffAtkHomingMissile::SeqCrash()
{
	//----------------------------------------------
	//�I��
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffAtkHomingMissile::missileLv1()
{
	//-------------------------------------------
	//�~�T�C�����W�b�NLV1�i�z�[�~���O���\�i�V�j
	//-------------------------------------------
	if(m_sSpeed <= 800)
	{
		m_sSpeed += 8;
	}

	if( m_sRotation < m_sRotationMaster ) m_sRotationAdd += 1;
	if( m_sRotation > m_sRotationMaster ) m_sRotationAdd -= 1;
	m_sRotation += m_sRotationAdd;

	m_Add.x = (viiMath::Cos100(m_sRotation)*m_sSpeed)/100;
	m_Add.y = (viiMath::Sin100(m_sRotation)*m_sSpeed)/100;

}

void CEffAtkHomingMissile::missileLv2()
{
	//-------------------------------------
	//�~�T�C�����W�b�NLV2
	//-------------------------------------
	Sint32 px,py;

	if(m_sHomingWait > 15)
	{
		//�r���܂ł�Lv1�Ɠ���
		missileLv1();
		return;
	}

	if( m_sHomingAdjustSpd < 32 ) m_sHomingAdjustSpd ++;

	if( getTargetEnemyPosition(m_Pos.x/100 , m_Pos.y/100 , px , py) )
	{
		//�z�[�~���O���\���啝�ɃA�b�v
		px *= 100;
		py *= 100;

		if(px < m_Pos.x) m_Add.x -= m_sHomingAdjustSpd;
		if(px > m_Pos.x) m_Add.x += m_sHomingAdjustSpd;
		if(py < m_Pos.y) m_Add.y -= m_sHomingAdjustSpd;
		if(py > m_Pos.y) m_Add.y += m_sHomingAdjustSpd;
	}

	m_sRotation = (Sint32)viiMath::Atan2d( m_Add.y , m_Add.x );//(RAD2DEG(gxAtan(m_Add.y,m_Add.x)));

	if( viiSub::GetBoundBlock_Player( m_Pos.x , m_Pos.y ) )
	{
		//�ǂɂ�������
		new CEffBomb( m_Pos.x , m_Pos.y , 1 , 0 );
		SetActionSeq( enActionSeqCrash );
		return;
	}

}


void CEffAtkHomingMissile::missileLv3()
{
	//-------------------------------------
	//�~�T�C�����W�b�NLV3
	//-------------------------------------
	Sint32 px,py;

	if(m_sHomingWait > 0)
	{
		//�r���܂ł�Lv1�Ɠ���
		missileLv1();
		return;
	}

	if( getTargetEnemyPosition(m_Pos.x/100 , m_Pos.y/100 , px , py) )
	{
		//�z�[�~���O���\���啝�ɃA�b�v
		px *= 100;
		py *= 100;

		if(px < m_Pos.x) m_Add.x -= 48;
		if(px > m_Pos.x) m_Add.x += 48;
		if(py < m_Pos.y) m_Add.y -= 48;
		if(py > m_Pos.y) m_Add.y += 48;
	}

	m_sRotation = (Sint32)viiMath::Atan2d( m_Add.y , m_Add.x );//(RAD2DEG(gxAtan(m_Add.y,m_Add.x)));

	if( viiSub::GetBoundBlock_Player( m_Pos.x , m_Pos.y ) )
	{
		//�ǂɂ�������
		new CEffBomb( m_Pos.x , m_Pos.y , 1 , 0 );
		SetActionSeq( enActionSeqCrash );
		return;
	}

}


gxBool CEffAtkHomingMissile::getTargetEnemyPosition(Sint32 x,Sint32 y,Sint32 &px,Sint32 &py)
{
	//-------------------------------------
	//���b�N�I�����W�b�N
	//�ł��߂��G�̍��W�𓾂�
	//�Y���̓G�����Ȃ��ꍇ�ɂ�gxFalse��Ԃ�
	//-------------------------------------
	double kyori,min_dist = SCR_W*SCR_H;
	Sint32 min=-1;
	Sint32 x1,y1;

	//���W��1/100�T�C�Y

	for(Sint32 i=0;i<pGame->pHitManager->GetOldMax();i++)
	{

		if( pGame->pHitManager->GetOldID(i) == ID_ENEMY_DEF)
		{
			x1 = pGame->pHitManager->GetOldX(i);
			y1 = pGame->pHitManager->GetOldY(i);

			kyori = viiMath::Dist( x1-x , y1-y );

			if(kyori < min_dist)
			{
				if(!viiSub::IsScrollOut(x1*100,y1*100,0,0))
				{
					//��ʔ͈͊O�̃��c�͑���ɂ��Ȃ�
					min_dist = kyori;
					min = i;
				}
			}
		}
	}

	if(min==-1) return gxFalse;	//������Ȃ�����

	px = pGame->pHitManager->GetOldX(min);
	py = pGame->pHitManager->GetOldY(min);

	return gxTrue;
}


void CEffAtkHomingMissile::Draw()
{
	//---------------------------------------------------------------
	//�`��
	//---------------------------------------------------------------

	float scl = 1.0f;
	viiDraw::Sprite(&spr_custom[SPR_MISSILE]        , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_DEFAULT,ARGB_DFLT,scl,m_sRotation );
	viiDraw::Sprite(&spr_custom[SPR_MISSILE_BURNIA] , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_DEFAULT,ARGB_DFLT,scl,m_sRotation );

}


