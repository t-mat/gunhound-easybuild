//--------------------------------------------------------------------------------
//
// Stage����:�G�̒e
//
//--------------------------------------------------------------------------------

#include <gunvalken.h>
#include "enemy.h"
#include "../effect/EffectBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -4,
	enKuraiTop   = -4,
	enKuraiRight =  4,
	enKuraiBottom=  4,

	enScore = 300,
};

gxSprite SprCEne0001EneMissile[]=
{
	{TPG_EFFECT, 128,48,16,8,14,4},
};

gxSprite SprCEne0001EneRyudan[]=
{
	{TPG_EFFECT,144,160,32,16,8,8},		//17:�֒e�~�T�C���i��j
};

gxSprite SprCEne0001EneCannon[]=
{
	{enTexPageEffectCommon03,96,0 ,32,32,16,16},		//01:�L���m���e��
	{enTexPageEffectCommon03,96-32 ,0,32,32,16,16},		//01:�L���m���e��
	{enTexPageEffectCommon03,96,0 ,32,32,16,16},		//01:�L���m���e��


	{TPG_EFFECT,96,16,16,16,8,8},		//01:�L���m���e��
	{TPG_EFFECT,96,16,16,16,8,8},		//01:�L���m���e��
	{TPG_EFFECT,48,0,16,16,8,8},		//01:�L���m���e��
	{TPG_EFFECT,96, 0,16,16,8,8},		//01:�L���m���e��
};

gxSprite SprCEne0001EneCrasher[]=
{
	{enTexPageEffectCommon02,0+16*0,144,16,16,8,8},
	{TPG_CLOUD,64,32,32,16,16,8},

	{TPG_EFFECT,16,128,16,16,8,8},		//01:�N���b�V���[�e��
};

gxSprite SprNewDanmaku[] = {
	//�e���p�e��
	{ enTexPageEffectCommon02,32+16,136-8  ,16,16,8,8 },	//��
	{ enTexPageEffectCommon02,32,136-8,14,8,7,5 },			//���i
	{ enTexPageEffectCommon02,32,136  ,10,10,5,5 },			//�ʏ�
};

gxSprite SprCEne0001EneSuperBall[]=
{
	{TPG_EFFECT,160,0,32,32,16,16},		//0:�X�[�p�[�{�[��
};

gxSprite SprCEne0001EneHyperBall[]=
{
	{ enTexPageEffect4,0,0,128,128,64,64},			//0:�n�C�p�[�{�[��
	{ enTexPageEffect4,128,0,128,128,64,64},		//0:�n�C�p�[�{�[��
	{ enTexPageEffect4,0,128,128,128,64,64},		//0:�n�C�p�[�{�[��
	{ enTexPageEffect4,128,128,128,128,64,64},		//0:�n�C�p�[�{�[��
};


gxBool CEneBulletBase::IsClearBullet()
{
	//���̎�ނ̃o���b�g�̓N���A���ꂽ�̂��H

	//�e���������Ă���Ώ�����Ȃ��悤��false��Ԃ�(���]���Ă��邱�Ƃɒ���)
	return !CCockpit::GetInstance()->IsDanmakuEnable( m_sBulletIndex );
}


CEne0001EneBullet::CEne0001EneBullet( Sint32 x, Sint32 y ,Sint32 r ,Sint32 spd ,Sint32 sSpr)
{
	//-----------------------------------------------------
	//�R���X�g���N�^
	//-----------------------------------------------------
	m_Add.x = (viiMath::Cos100(r)*spd)/100;
	m_Add.y = (viiMath::Sin100(r)*spd)/100;

	m_Pos.x = x;
	m_Pos.y = y;

	m_bDrop = gxFalse;
	m_Drop.x = 0;
	m_Drop.y = 0;
	m_pSprite = &SprNewDanmaku[sSpr];
	m_sRotation = viiSub::Rand()%360;
	m_sType = sSpr;
	if( m_sType == 4 )
	{
		m_pSprite = &SprNewDanmaku[1];
		m_sRotation = r;
	}

	m_HitAtari.set_hp( HP_ARMORCLASS_B );

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;

}


CEne0001EneBullet::~CEne0001EneBullet()
{

	//-----------------------------------------------------
	//�f�X�g���N�^
	//-----------------------------------------------------

}


void CEne0001EneBullet::SeqMain()
{
	//-----------------------------------------------------
	//���C��
	//-----------------------------------------------------

	if( m_HitAtari.IsImpacted() || viiSub::GetBoundBlock_Enemy( m_Pos.x , m_Pos.y ) )
	{
		viiEff::Set(EFF_HITFLASH_ENE , m_Pos.x , m_Pos.y , 0 );
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( IsClearBullet() )
	{
		viiEff::Set(EFF_HITFLASH_ENE , m_Pos.x , m_Pos.y , 0 );
		SetActionSeq( enActionSeqCrash );
		return;
	}

	Sint32 sAdd = 1;

	if(m_bDrop)
	{
		sAdd = 3;
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight*sAdd ) )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	LogicAI();

	//���炢����o�^
	m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );

}

void CEne0001EneBullet::SetDrop()
{
	//�h���b�v�e�ɕύX

	m_bDrop = gxTrue;
	m_pSprite = &SprNewDanmaku[0];

}


void CEne0001EneBullet::LogicAI()
{
	//-----------------------------------------------------
	//�ړ����W�b�N
	//-----------------------------------------------------

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	if( m_bDrop )
	{
		if( m_Drop.y < 800 ) m_Drop.y += 2;
		m_Pos.y += m_Drop.y;
	}
	if( m_sType == 4 )
	{
	}
	else
	{
		m_sRotation += 8;
	}
}


void CEne0001EneBullet::Atack()
{
	//-----------------------------------------------------
	//�U���\�Ȃ�U������
	//-----------------------------------------------------

}


void CEne0001EneBullet::SeqCrash()
{
	//-----------------------------------------------------
	//�j�󂳂ꂽ
	//-----------------------------------------------------

	SetActionSeq( enActionSeqEnd );

}

void CEne0001EneBullet::Draw()
{

	viiDraw::Sprite( m_pSprite , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET , ATR_DEFAULT    , ARGB_DFLT , 1.5f , m_sRotation );
}


//--------------------------------------------------------------------------------
//
// Stage����:�G���ʂ̑��˃~�T�C���i�������O���j
//
//--------------------------------------------------------------------------------


CEne0001EneMissile::CEne0001EneMissile( Sint32 x, Sint32 y ,Sint32 r ,Sint32 spd )
{
	//-----------------------------------------------------
	//�R���X�g���N�^
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_sSpeed = spd;
	m_sSpeedAdd = spd*2;
	m_sRotation = r;
	m_sGravitAdd = 0;

	m_Add.x = (viiMath::Cos100(r)*m_sSpeedAdd)/100;
	m_Add.y = (viiMath::Sin100(r)*m_sSpeedAdd)/100;

	m_HitAtari.set_ap( enAtkPointMissile );
	m_HitAtari.set_hp( enHitPointDefault );

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;

	m_HitKurai.set_hp( enDefPointGun );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;
	m_HitKurai.DisableAiming();

	m_sTimer = viiSub::Rand()%360;
}


CEne0001EneMissile::~CEne0001EneMissile()
{

	//-----------------------------------------------------
	//�f�X�g���N�^
	//-----------------------------------------------------

}


void CEne0001EneMissile::SeqMain()
{
	//-----------------------------------------------------
	//���C��
	//-----------------------------------------------------
	Sint32 ox,oy;
	ox = m_Pos.x;
	oy = m_Pos.y;
	if( m_HitKurai.is_dead() )
	{
		viiEff::SetBombEffect( m_Pos.x, m_Pos.y , 2 , 2 );
		SetActionSeq( enActionSeqCrash );
	}
	else if( m_HitAtari.IsImpacted() )
	{
		viiEff::SetBombDeluxe( m_Pos.x, m_Pos.y , 1 , 5 );
		SetActionSeq( enActionSeqCrash );
		return;
	}
	else if( viiSub::GetBoundBlock_Enemy( m_Pos.x , m_Pos.y ) )
	{
		viiEff::Set(EFF_HITFLASH_ENE , m_Pos.x , m_Pos.y , 0 );
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	LogicAI();

//	if( viiSub::GameCounter()%4==0)
	{
		for(Sint32 ii=0;ii<2;ii++)
		{
			Sint32 x,y;
			x = ox + (ii+1)*(m_Pos.x-ox)/2;
			y = oy + (ii+1)*(m_Pos.y-oy)/2;
			CAtkH0001MslSmoke *p = new CAtkH0001MslSmoke( x , y );
			p->SetDirection( (m_sTimer*8)%360 , 80 );
			p->SetClearSpeed(3);
		}
	}

	if( m_Add.y >= 0 )
	{
		//�����蔻��o�^
		m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );

		//���炢����o�^
		m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );
	}


}


void CEne0001EneMissile::LogicAI()
{
	//-----------------------------------------------------
	//�ړ����W�b�N
	//-----------------------------------------------------

	if( m_sSpeedAdd > m_sSpeed ) m_sSpeedAdd -= 8;

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;


	if( m_Add.y <= 300 )
	{
		m_Add.y += 6;	
	}

	m_sRotation	= (Sint32)viiMath::Atan2d( m_Add.y , m_Add.x );
/*
	m_Add.x = (viiMath::Cos100(m_sRotation)*m_sSpeedAdd)/100;
	m_Add.y = (viiMath::Sin100(m_sRotation)*m_sSpeedAdd)/100;

	if( m_sRotation > 90 )
	{
		m_sRotation -= 3+viiSub::Rand()%2;
		if(m_sRotation <= 90 ) m_sRotation = 90;
	}
	else if( m_sRotation < 90+45 )
	{
		m_sRotation += 3+viiSub::Rand()%2;
		if(m_sRotation >= 90 ) m_sRotation = 90;
	}
*/

}


void CEne0001EneMissile::Atack()
{
	//-----------------------------------------------------
	//�U���\�Ȃ�U������
	//-----------------------------------------------------

}


void CEne0001EneMissile::SeqCrash()
{
	//-----------------------------------------------------
	//�j�󂳂ꂽ
	//-----------------------------------------------------

	SetActionSeq( enActionSeqEnd );

}

void CEne0001EneMissile::Draw()
{

	viiDraw::Sprite( &spr_ene_bullet[16] , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET , ATR_DEFAULT , ARGB_DFLT , 1.0f , m_sRotation );

}



//--------------------------------------------------------------------------------
//
// Stage����:�������{��
//
//--------------------------------------------------------------------------------


CEne0001EneBomb::CEne0001EneBomb( Sint32 x, Sint32 y ,Sint32 r ,Sint32 spd )
{
	//-----------------------------------------------------
	//�R���X�g���N�^
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_sRotation = viiSub::Rand()%360;

	m_Add.x = (viiMath::Cos100(r)*spd)/100;
	m_Add.y = (viiMath::Sin100(r)*spd)/100;

	m_HitAtari.set_hp( HP_ARMORCLASS_B );

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;

}


CEne0001EneBomb::~CEne0001EneBomb()
{

	//-----------------------------------------------------
	//�f�X�g���N�^
	//-----------------------------------------------------

}


void CEne0001EneBomb::SeqMain()
{
	//-----------------------------------------------------
	//���C��
	//-----------------------------------------------------

	if( m_HitAtari.IsImpacted() )
	{
		viiMus::PlaySound(SE_BOMB_SMALL);
		viiEff::SetBombEffect( m_Pos.x , m_Pos.y , 3 , 3 );
		viiEff::Set(EFF_BOMBFLASH_ENE,m_Pos.x , m_Pos.y );
		SetActionSeq( enActionSeqCrash );
		return;
	}
	if( viiSub::GetBoundBlock_Enemy( m_Pos.x , m_Pos.y ) )
	{
		viiMus::PlaySound(SE_BOMB_SMALL);
		viiEff::Set(EFF_BOMBFLASH_ENE , m_Pos.x , m_Pos.y );
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	LogicAI();

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	//���炢����o�^
	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
	m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );

}


void CEne0001EneBomb::LogicAI()
{
	//-----------------------------------------------------
	//�ړ����W�b�N
	//-----------------------------------------------------
	m_sRotation += 8;

	m_Add.x = -m_Add.x/100;
	m_Add.y += 8;

	if( m_Add.y >= 700 )
	{
		m_Add.y = 700;
	}

}


void CEne0001EneBomb::Atack()
{
	//-----------------------------------------------------
	//�U���\�Ȃ�U������
	//-----------------------------------------------------

}


void CEne0001EneBomb::SeqCrash()
{
	//-----------------------------------------------------
	//�j�󂳂ꂽ
	//-----------------------------------------------------

	SetActionSeq( enActionSeqEnd );

}

void CEne0001EneBomb::Draw()
{

	viiDraw::Sprite(&spr_ene_bullet[1] , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET , ATR_DEFAULT , ARGB_DFLT , 1.0f , m_sRotation );

}


//--------------------------------------------------------------------------------
//
// Stage����:�֒e
//
//--------------------------------------------------------------------------------


CEne0001Ryudan::CEne0001Ryudan( Sint32 x, Sint32 y ,Sint32 r ,Sint32 spd )
{
	//-----------------------------------------------------
	//�R���X�g���N�^
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_sRotation = r;

	m_Add.x = (viiMath::Cos100(m_sRotation)*spd)/100;
	m_Add.y = (viiMath::Sin100(m_sRotation)*spd)/100;

	m_HitAtari.set_hp( HP_ARMORCLASS_B );

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;

	m_HitKurai.set_hp( HP_ARMORCLASS_C );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;
	m_HitKurai.DisableAiming();

}


CEne0001Ryudan::~CEne0001Ryudan()
{

	//-----------------------------------------------------
	//�f�X�g���N�^
	//-----------------------------------------------------

}


void CEne0001Ryudan::SeqMain()
{
	//-----------------------------------------------------
	//���C��
	//-----------------------------------------------------

	if( m_HitAtari.IsImpacted() || m_HitKurai.is_dead())
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( viiSub::GetBoundBlock_Enemy( m_Pos.x , m_Pos.y ) )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	LogicAI();

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	//�����蔻��o�^
	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
	m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );

	//���炢����o�^
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

}


void CEne0001Ryudan::LogicAI()
{
	//-----------------------------------------------------
	//�ړ����W�b�N
	//-----------------------------------------------------

	m_Add.x = (viiMath::Cos100(m_sRotation)*400)/100;
	m_Add.y = (viiMath::Sin100(m_sRotation)*400)/100;

	if( m_sRotation > 90+25 ) m_sRotation -= 1;
	if( m_sRotation < 90+25 ) m_sRotation += 1;

}


void CEne0001Ryudan::Atack()
{
	//-----------------------------------------------------
	//�U���\�Ȃ�U������
	//-----------------------------------------------------

}


void CEne0001Ryudan::SeqCrash()
{
	//-----------------------------------------------------
	//�j�󂳂ꂽ
	//-----------------------------------------------------

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	viiMus::PlaySound(SE_BOMB_SMALL);
	viiEff::SetBombEffect( m_Pos.x , m_Pos.y , 3 , 3 );
	viiEff::Set(EFF_BOMBFLASH_ENE,m_Pos.x , m_Pos.y );

	SetActionSeq( enActionSeqEnd );

}

void CEne0001Ryudan::Draw()
{

	viiDraw::Sprite(&SprCEne0001EneRyudan[0] , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET , ATR_DEFAULT , ARGB_DFLT , 1.0f , m_sRotation );

}


//--------------------------------------------------------------------------------
//
// Stage����:�����C�e
//
//--------------------------------------------------------------------------------

CEne0001EneCannon::CEne0001EneCannon( Sint32 x, Sint32 y ,Sint32 r ,Sint32 spd )
{
	//-----------------------------------------------------
	//�R���X�g���N�^
	//-----------------------------------------------------
	m_Add.x = (viiMath::Cos100(r)*spd)/100;
	m_Add.y = (viiMath::Sin100(r)*spd)/100;

	m_Pos.x = x;
	m_Pos.y = y;

	m_sDispRotation = viiSub::Rand()%360;
	m_sRotation     = r;
	m_sSpeed = spd;
	m_bDrop = gxFalse;

	while(m_sRotation<0)
	{
		m_sRotation += 360;
	}

	m_HitAtari.set_hp( HP_ARMORCLASS_B );

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;

	m_fScale = 2.6f;
	m_bSound = gxTrue;
}


CEne0001EneCannon::~CEne0001EneCannon()
{

	//-----------------------------------------------------
	//�f�X�g���N�^
	//-----------------------------------------------------

}


void CEne0001EneCannon::SeqMain()
{
	//-----------------------------------------------------
	//���C��
	//-----------------------------------------------------

	if( m_HitAtari.IsImpacted() || viiSub::GetBoundBlock_Enemy( m_Pos.x , m_Pos.y ) )
	{
		viiEff::SetBombEffect( m_Pos.x , m_Pos.y , 3 , 4 );
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( IsScrollOut( enScrollOutWidth*8 , enScrollOutHeight*8 ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	LogicAI();

	if( 1 )//viiSub::Rand()%3 )
	{
		CEffExPerticle *p;
		p = new CEffExPerticle( m_Pos.x , m_Pos.y , PRIO_ENE_BULLET);
		p->SetAlphaAdd( -(6+viiSub::Rand()%24 ) );
		p->SetSprite( &SprCEne0001EneCannon[0] );
		p->m_sAtribute = ATR_ALPHA_PLUS;
		p->m_sRotation = m_sDispRotation;
		p->m_fScale = 2.f;//(50+viiSub::Rand()%25)/100.f;
		p->SetEnd();
	}

	//���炢����o�^
	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
	m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );

}


void CEne0001EneCannon::LogicAI()
{
	//-----------------------------------------------------
	//�ړ����W�b�N
	//-----------------------------------------------------

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	if(m_bDrop)
	{
		m_Add.y += 24;
/*
		m_Add.x = (viiMath::Cos100(m_sRotation)*m_sSpeed)/100;
		m_Add.y = (viiMath::Sin100(m_sRotation)*m_sSpeed)/100;

		if( m_sRotation > 90 )
		{
			m_sRotation --;
		}
		if( m_sRotation < 90 )
		{
			m_sRotation ++;
		}
*/

	}

	m_sDispRotation += 8;
}


void CEne0001EneCannon::Atack()
{
	//-----------------------------------------------------
	//�U���\�Ȃ�U������
	//-----------------------------------------------------

}


void CEne0001EneCannon::SeqCrash()
{
	//-----------------------------------------------------
	//�j�󂳂ꂽ
	//-----------------------------------------------------

	if( m_bSound )
	{
		viiMus::GetInstance()->PlayLoopSound( enSoundExplosionMiddle , 120 );
	}
//	viiMus::PlaySound(enSoundExplosionMiddle);
	new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x , m_Pos.y , PRIO_ENEMY , 180+45 ,80 );
	SetActionSeq( enActionSeqEnd );

}

void CEne0001EneCannon::Draw()
{

	viiDraw::Sprite( &SprCEne0001EneCannon[1] , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET , ATR_ALPHA_PLUS , 0x80FFFFFF , m_fScale , -m_sDispRotation*4 );
	viiDraw::Sprite( &SprCEne0001EneCannon[0] , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET , ATR_ALPHA_PLUS , 0x80FFFFFF , m_fScale*1.1f ,  m_sDispRotation*4 );

}


//--------------------------------------------------------------------------------
//
// Stage����:�����e�i�j��\�j
//
//--------------------------------------------------------------------------------

CEne0001EneCrasher::CEne0001EneCrasher( Sint32 x, Sint32 y ,Sint32 r ,Sint32 spd )
{
	//-----------------------------------------------------
	//�R���X�g���N�^
	//-----------------------------------------------------
	m_sSpeed = spd;
	m_sDirection = r;

	m_Add.x = (viiMath::Cos100(m_sDirection)*m_sSpeed)/100;
	m_Add.y = (viiMath::Sin100(m_sDirection)*m_sSpeed)/100;

	m_Pos.x = x;
	m_Pos.y = y;

	m_sRotation = viiSub::Rand()%360;

	m_HitAtari.set_hp( HP_ARMORCLASS_B );
	m_HitKurai.set_hp( 1 );

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;

	m_HitKurai.ax1 = -8;
	m_HitKurai.ay1 = -8;
	m_HitKurai.ax2 =  8;
	m_HitKurai.ay2 =  8;
	m_HitKurai.DisableAiming();

	m_sSpeedAdd  = 0;
	m_sDelayTime = 0;
}


CEne0001EneCrasher::~CEne0001EneCrasher()
{

	//-----------------------------------------------------
	//�f�X�g���N�^
	//-----------------------------------------------------

}


void CEne0001EneCrasher::SetDelay( Sint32 minSpd , Sint32 sFrm )
{
	//-----------------------------------------------------
	//�e���̃f�B���C�ݒ�
	//-----------------------------------------------------
	if( sFrm < 0 ) sFrm = 1;

	m_sSpeedAdd = minSpd;//(minSpd - m_sSpeed)/sFrm;
	m_sDelayTime = sFrm;


}

void CEne0001EneCrasher::SeqMain()
{
	//-----------------------------------------------------
	//���C��
	//-----------------------------------------------------

	if( pGame->GetMarineLine() )
	{
		if( m_Pos.y > pGame->GetMarineLine() )
		{
			new CEffWater( m_Pos.x , m_Pos.y, enTypeSplashSmall , 0);
			SetActionSeq( enActionSeqEnd );
			return;
		}
	}

	if( m_HitKurai.is_damage() )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}
	else if( m_HitAtari.IsImpacted() || viiSub::GetBoundBlock_Enemy( m_Pos.x , m_Pos.y ) )
	{
		viiEff::Set(EFF_HITFLASH_ENE , m_Pos.x , m_Pos.y , 0 );
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( m_sDelayTime > 0 )
	{
		m_sDelayTime --;

		if( m_sDelayTime > 0 )
		{
			m_sSpeed += (m_sSpeedAdd-m_sSpeed)/m_sDelayTime;

			if( m_sSpeed <= m_sSpeedAdd )
			{
				m_sSpeed = m_sSpeedAdd;
			}
		}
		m_Add.x = (viiMath::Cos100(m_sDirection)*m_sSpeed)/100;
		m_Add.y = (viiMath::Sin100(m_sDirection)*m_sSpeed)/100;
	}

	LogicAI();

	//���炢����o�^
	m_HitKurai.SetHantei( ID_ENEMY_DEF , &m_Pos );

	//���炢����o�^
	m_HitAtari.SetHantei( ID_ENEMY_ATK , &m_Pos );

}


void CEne0001EneCrasher::LogicAI()
{
	//-----------------------------------------------------
	//�ړ����W�b�N
	//-----------------------------------------------------

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	m_sRotation += 12;
}


void CEne0001EneCrasher::Atack()
{
	//-----------------------------------------------------
	//�U���\�Ȃ�U������
	//-----------------------------------------------------

}


void CEne0001EneCrasher::SeqCrash()
{
	//-----------------------------------------------------
	//�j�󂳂ꂽ
	//-----------------------------------------------------

	SetActionSeq( enActionSeqEnd );

}

void CEne0001EneCrasher::Draw()
{

	viiDraw::Sprite( &SprCEne0001EneCrasher[1] , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET , ATR_ALPHA_PLUS , 0xf08080f0 , 1.25f , m_sRotation );
	viiDraw::Sprite( &SprCEne0001EneCrasher[0] , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET , ATR_ALPHA_PLUS , 0xf08080f0 , 1.25f , m_sRotation );

}


//--------------------------------------------------------------------------------
//
// Stage����:�y���e
//
//--------------------------------------------------------------------------------

CEne0001EneSakretu::CEne0001EneSakretu( Sint32 x, Sint32 y ,Sint32 r ,Sint32 spd )
{
	//-----------------------------------------------------
	//�R���X�g���N�^
	//-----------------------------------------------------
	m_Add.x = 0;//(viiMath::Cos100(r)*spd)/100;
	m_Add.y = 0;//(viiMath::Sin100(r)*spd)/100;

	m_Pos.x = x;
	m_Pos.y = y;

	m_sSpeed    = spd;
	m_pSprite   = &SprNewDanmaku[0];
	m_sRotation = r;
	m_sBombTimer = 0;

	m_HitAtari.set_hp( HP_ARMORCLASS_B );

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;

}


CEne0001EneSakretu::~CEne0001EneSakretu()
{

	//-----------------------------------------------------
	//�f�X�g���N�^
	//-----------------------------------------------------

}


void CEne0001EneSakretu::SeqMain()
{
	//-----------------------------------------------------
	//���C��
	//-----------------------------------------------------

	if( IsClearBullet() )
	{
		viiEff::Set(EFF_HITFLASH_ENE , m_Pos.x , m_Pos.y , 0 );
		SetActionSeq( enActionSeqEnd );
		return;
	}

	if( m_HitAtari.IsImpacted() || viiSub::GetBoundBlock_Enemy( m_Pos.x , m_Pos.y ) )
	{
		viiEff::Set(EFF_HITFLASH_ENE , m_Pos.x , m_Pos.y , 0 );
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	if( m_sBombTimer >= 60 )
	{
		m_sBombTimer = 120;
		viiEff::Set(EFF_HITFLASH_ENE , m_Pos.x , m_Pos.y , 0 );
		SetActionSeq( enActionSeqCrash );
		return;
	}

	LogicAI();

	//���炢����o�^
	m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );

}


void CEne0001EneSakretu::LogicAI()
{
	//-----------------------------------------------------
	//�ړ����W�b�N
	//-----------------------------------------------------

	m_Add.x = (viiMath::Cos100(m_sRotation)*m_sSpeed)/100;
	m_Add.y = (viiMath::Sin100(m_sRotation)*m_sSpeed)/100;

	if( m_sSpeed >= 0 )
	{
		m_sSpeed -= 8;
		if( m_sSpeed <= 0 )
		{
			m_sSpeed = 0;
			m_sBombTimer ++;
		}
	}

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

//	m_sRotation += 8;
}


void CEne0001EneSakretu::Atack()
{
	//-----------------------------------------------------
	//�U���\�Ȃ�U������
	//-----------------------------------------------------

}


void CEne0001EneSakretu::SeqCrash()
{
	//-----------------------------------------------------
	//�j�󂳂ꂽ
	//-----------------------------------------------------

	if( m_sBombTimer > 0 )
	{
		if( viiSub::GameCounter()%32 == 0)
		{
			Sint32 r;
			r = get_valken_rot( m_Pos.x , m_Pos.y+1600 );
			CDanmakuRoot *p;
			p = new CDanmakuRoot( CDanmakuRoot::enDanmakuGattling , m_Pos.x , m_Pos.y, r , 320*2 );
			p->SetBulletIndex( m_sBulletIndex );
	//		new CDanmakuRoot( CDanmakuRoot::enDanmakuGattling , m_Pos.x , m_Pos.y, r+5 , 320*1 );
	//		new CDanmakuRoot( CDanmakuRoot::enDanmakuGattling , m_Pos.x , m_Pos.y, r-5 , 320*3 );
			SetActionSeq( enActionSeqEnd );
		}
	}
	else
	{
		SetActionSeq( enActionSeqEnd );
	}
}

void CEne0001EneSakretu::Draw()
{

	viiDraw::Sprite( m_pSprite , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET , ATR_DEFAULT , ARGB_DFLT , 1.5f , (m_sTimer*8)%360 );

}


//--------------------------------------------------------------------------------
//
// Stage����:�v���Y�}�{�[��
//
//--------------------------------------------------------------------------------

CEne0001PlasmaBall::CEne0001PlasmaBall( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//�R���X�g���N�^
	//-----------------------------------------------------
	m_Add.x = 0;
	m_Add.y = 0;

	m_Pos.x = x;
	m_Pos.y = y;

	m_HitAtari.set_hp( enHitPointDefault );
	m_HitAtari.set_ap( HP_ARMORCLASS_B );

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;

	m_sSpeed = 0;
	m_sRotation = 0;
}


CEne0001PlasmaBall::~CEne0001PlasmaBall()
{

	//-----------------------------------------------------
	//�f�X�g���N�^
	//-----------------------------------------------------

}


void CEne0001PlasmaBall::SeqMain()
{
	//-----------------------------------------------------
	//���C��
	//-----------------------------------------------------

	if( m_HitAtari.IsImpacted() || viiSub::GetBoundBlock_Enemy( m_Pos.x , m_Pos.y ) )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( pGame->GetMarineLine() && m_Pos.y > pGame->GetMarineLine() )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	LogicAI();

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	//���炢����o�^
	m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );

}


void CEne0001PlasmaBall::LogicAI()
{
	//-----------------------------------------------------
	//�ړ����W�b�N
	//-----------------------------------------------------
	if( m_sTimer <= 60 )
	{
		m_sRotation = get_valken_rot( m_Pos.x , m_Pos.y );
	}

	if( m_sSpeed <= 320 )
	{
		m_sSpeed += 4;
	}

	m_Add.x = (viiMath::Cos100(m_sRotation)*m_sSpeed)/100;
	m_Add.y = (viiMath::Sin100(m_sRotation)*m_sSpeed)/100;

}


void CEne0001PlasmaBall::Atack()
{
	//-----------------------------------------------------
	//�U���\�Ȃ�U������
	//-----------------------------------------------------

}


void CEne0001PlasmaBall::SeqCrash()
{
	//-----------------------------------------------------
	//�j�󂳂ꂽ
	//-----------------------------------------------------

	if( pGame->GetMarineLine() )
	{
		if( m_Pos.y > pGame->GetMarineLine() )
		{
			MakeSplash( m_Pos.x, pGame->GetMarineLine() );
		}
	}
	else
	{
		new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x+(-4+viiSub::Rand()%8)*100,m_Pos.y+(-4+viiSub::Rand()%8)*100 , PRIO_ENEMY , 270+45 ,40 );
		new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x+(-4+viiSub::Rand()%8)*100,m_Pos.y+(-4+viiSub::Rand()%8)*100 , PRIO_ENEMY , 270+45 ,120);
		new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x+(-4+viiSub::Rand()%8)*100,m_Pos.y+(-4+viiSub::Rand()%8)*100 , PRIO_ENEMY , 270+45 ,80 );
	}

	SetActionSeq( enActionSeqEnd );

}

void CEne0001PlasmaBall::Draw()
{

	viiDraw::Sprite( &SprCEne0001EneRyudan[0] , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET , ATR_DEFAULT , ARGB_DFLT );

}


//--------------------------------------------------------------------------------
//
// Stage����:�X�[�p�[�{�[��
//
//--------------------------------------------------------------------------------

CEne0001EneSuperBall::CEne0001EneSuperBall(  Sint32 x, Sint32 y ,Sint32 r ,Sint32 spd )
{
	//-----------------------------------------------------
	//�R���X�g���N�^
	//-----------------------------------------------------
	m_Add.x = 0;
	m_Add.y = 0;
	m_Add.z = 0;

	m_Pos.x = x;
	m_Pos.y = y;

	m_Tgt.x = 0;
	m_Tgt.y = 0;
	m_Tgt.z = 0;

	m_Drop.x = 0;
	m_Drop.y = 0;

	m_HitAtari.set_hp( enHitPointDefault );
	m_HitAtari.set_ap( enAtkPointRyudan );

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;

	for(Sint32 ii=0;ii<enOptionMax;ii++)
	{
		m_StOption[ii].x = m_Pos.x;
		m_StOption[ii].y = m_Pos.y;
		m_StOption[ii].z = 0;
	}

	m_bDrop = gxFalse;
	m_sOptionCnt = 0;

	m_sSpeed    = spd;
	m_sRotation = r;
	m_pSprite = NULL;

	m_sWait = 0;
	m_sPattern = 0;

	m_bEndless = gxFalse;

	m_sMaxSpeed = 640;
	m_sDropMax = 200;
	m_sLevel = 0;
	m_sScrollOutAdd = enScrollOutWidth*2;
	m_bLeft4Dead = gxFalse;
	m_fScale = 1.0f;
	m_bGensui = gxFalse;
}


CEne0001EneSuperBall::~CEne0001EneSuperBall()
{
	//-----------------------------------------------------
	//�f�X�g���N�^
	//-----------------------------------------------------


}


void CEne0001EneSuperBall::SeqMain()
{
	//-----------------------------------------------------
	//���C��
	//-----------------------------------------------------

	if( m_HitAtari.IsImpacted() || viiSub::GetBoundBlock_Enemy( m_Pos.x , m_Pos.y ) )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( pGame->GetMarineLine() && m_Pos.y > pGame->GetMarineLine()+800 )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( !m_bEndless && IsScrollOut( m_sScrollOutAdd , m_sScrollOutAdd ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	if( IsClearBullet() )
	{
		viiEff::Set(EFF_HITFLASH_ENE , m_Pos.x , m_Pos.y , 0 );
		SetActionSeq( enActionSeqEnd );
		return;
	}

	LogicAI();

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	m_StOption[ m_sOptionCnt ].x = m_Pos.x;
	m_StOption[ m_sOptionCnt ].y = m_Pos.y;

	m_sOptionCnt ++;
	m_sOptionCnt = m_sOptionCnt%enOptionMax;

	//���炢����o�^
	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
	m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );

}


void CEne0001EneSuperBall::LogicAI()
{
	//-----------------------------------------------------
	//�ړ����W�b�N
	//-----------------------------------------------------

	switch(m_sPattern){
	case enPatternDefault:
		if( m_sSpeed < m_sMaxSpeed ) m_sSpeed += 10;
		m_Add.x = (viiMath::Cos100( m_sRotation )*m_sSpeed)/100;
		m_Add.y = (viiMath::Sin100( m_sRotation )*m_sSpeed)/100;
		break;

	case enPatternTargetting:
		LogicTargetting();
		break;
	case enPatternDrop:
		LogicDrop();
		break;

	case enPatternHoming:
		LogicHoming();
		break;
	case enPatternMicroMissile:
		LogicMicroMissile();
		break;
	}

}


void CEne0001EneSuperBall::LogicDrop()
{
	//-----------------------------------------------------
	//�h���b�v�V���b�g
	//-----------------------------------------------------

	m_Tgt.x = (viiMath::Cos100( m_sRotation )*m_sSpeed)/100;
	m_Tgt.y = (viiMath::Sin100( m_sRotation )*m_sSpeed)/100;

	if( m_bGensui )
	{
		if( m_sSpeed > 0 ) m_sSpeed -=8;
		if( m_Drop.y < m_sDropMax ) m_Drop.y += 8;
	}
	else
	{
		if( m_Drop.y < 600 ) m_Drop.y += 8;
	}
	m_Add.x = m_Tgt.x;
	m_Add.y = m_Tgt.y+m_Drop.y;

}


void CEne0001EneSuperBall::LogicHoming()
{
	//-----------------------------------------------------
	//�z�[�~���O
	//-----------------------------------------------------
	m_sRotation = get_valken_rot( m_Pos.x , m_Pos.y );
	ghPos Spd;

	Spd.x = viiMath::Cos100( m_sRotation )/6;
	Spd.y = viiMath::Sin100( m_sRotation )/6;

	m_Tgt.x += Spd.x;
	m_Tgt.y += Spd.y;

	if( viiMath::Dist( m_Tgt.y , m_Tgt.x ) >= m_sSpeed )
	{
		Float32 fRot = viiMath::Atan2d( m_Tgt.y , m_Tgt.x );

		m_Tgt.x = ( viiMath::Cos100( fRot )*m_sSpeed )/100;
		m_Tgt.y = ( viiMath::Sin100( fRot )*m_sSpeed )/100;

		m_Add.x = m_Tgt.x;
		m_Add.y = m_Tgt.y;
	}
	else
	{
		m_Add.x = m_Tgt.x;
		m_Add.y = m_Tgt.y;
	}

	if( m_sSpeed <= 640 )
	{
		m_sSpeed += 4;
	}

}


void CEne0001EneSuperBall::LogicTargetting()
{
	//-----------------------------------------------------
	//�^�[�Q�b�g�����߂ēːi����
	//-----------------------------------------------------
	if( m_sWait < 60 )
	{
		m_sWait ++;
		m_sRotation = get_valken_rot( m_Pos.x , m_Pos.y );
		m_Tgt.x = 0;
		m_Tgt.y = 0;
	}

	if( m_sSpeed <= 480 )
	{
		m_sSpeed += 6;
	}

	m_sRotation = get_valken_rot( m_Pos.x , m_Pos.y );

	m_Tgt.x += (viiMath::Cos100(m_sRotation)*m_sSpeed)/100;
	m_Tgt.y += (viiMath::Sin100(m_sRotation)*m_sSpeed)/100;

	if( m_bDrop )
	{
		if( m_Drop.y < 800 ) m_Drop.y += 8;
	}

	m_Add.x = m_Tgt.x/100;
	m_Add.y = m_Tgt.y/100+m_Drop.y;
}


void CEne0001EneSuperBall::LogicMicroMissile()
{
	//-----------------------------------------------------
	//�}�C�N���~�T�C��
	//-----------------------------------------------------
	ghPos Spd;
	Sint32 sRotSpd = 5;

	m_sScrollOutAdd = 256*100;
	m_sWait ++;

	if( m_sWait < 30 )
	{
		m_sSpeed = 300;
		m_Tgt.z = 0;
	}
	else
	{
		m_sRotation = get_valken_rot( m_Pos.x , m_Pos.y+2400 );
		m_Tgt.z += 1;
	}

	if( m_bLeft4Dead )
	{
		//�n�E���h��荶�ɍs���ΏI���
		if( pGame->GetHound()->m_Pos.x > m_Pos.x+32*100 )
		{
			SetActionSeq( enActionSeqCrash );
			return;
		}
	}
	else
	{
		//�傫����ʊO�ɏo��ΏI��
		if( IsScrollOut( WINDOW_W*100 , WINDOW_H*100 ) )
		{
			SetActionSeq( enActionSeqEnd );
		}
	}

	if( m_sSpeed < 800 ) m_sSpeed += 3;

	//�z�[�~���O�R���g���[���[

	Spd.x = viiMath::Cos100( m_sRotation )*10;
	Spd.y = viiMath::Sin100( m_sRotation )*10;

	m_Tgt.x += Spd.x/(100+m_Tgt.z/100);
	m_Tgt.y += Spd.y/(100+m_Tgt.z/100);



	if( viiMath::Dist( m_Tgt.y , m_Tgt.x ) >= m_sSpeed )
	{
		Float32 fRot = viiMath::Atan2d( m_Tgt.y , m_Tgt.x );

		m_Tgt.x = ( viiMath::Cos100( fRot )*m_sSpeed )/100;
		m_Tgt.y = ( viiMath::Sin100( fRot )*m_sSpeed )/100;
	
	}

	m_Add.x = m_Tgt.x;
	m_Add.y = m_Tgt.y;

}


void CEne0001EneSuperBall::Atack()
{
	//-----------------------------------------------------
	//�U���\�Ȃ�U������
	//-----------------------------------------------------

}


void CEne0001EneSuperBall::SeqCrash()
{
	//-----------------------------------------------------
	//�j�󂳂ꂽ
	//-----------------------------------------------------

	if( pGame->GetMarineLine() )
	{
		if( m_Pos.y > pGame->GetMarineLine() )
		{
			viiMus::PlaySound(enSoundSplashS);
			MakeSplash( m_Pos.x , pGame->GetMarineLine() );
		}
		else
		{
			new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x+(-4+viiSub::Rand()%8)*100,m_Pos.y+(-4+viiSub::Rand()%8)*100 , PRIO_ENEMY , 270+45 ,80 );
		}
	}
	else
	{
//		new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x+(-4+viiSub::Rand()%8)*100,m_Pos.y+(-4+viiSub::Rand()%8)*100 , PRIO_ENEMY , 270+45 ,40 );
//		new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x+(-4+viiSub::Rand()%8)*100,m_Pos.y+(-4+viiSub::Rand()%8)*100 , PRIO_ENEMY , 270+45 ,120);
		Sint32 r = viiMath::Atan2_100(m_Add.y , m_Add.x )/100+180;
		new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x+(-4+viiSub::Rand()%8)*100,m_Pos.y+(-4+viiSub::Rand()%8)*100 , PRIO_ENEMY , r ,180 );
	}

	SetActionSeq( enActionSeqEnd );

}


void CEne0001EneSuperBall::Draw()
{
	//-----------------------------------------------------
	//�`��
	//-----------------------------------------------------

	Sint32 sCnt = 0;
	Sint32 sCol = 0xffffffff;

	switch( (m_sTimer/2)%3 ){
	case 0:
		sCol = ARGB( 0xff , 0xa0,0xa0,0x00 );
		break;
	case 1:
		sCol = ARGB( 0xff, 0xa0,0x00,0x00 );
		break;
	case 2:
		sCol = ARGB( 0xff , 0x00,0x00,0x00 );
		break;
	}

	for(Sint32 ii=0;ii<6;ii++)
	{
		Sint32 n,x,y;

		n = m_sOptionCnt - 1 - sCnt*4;
		n = (n+enOptionMax)%enOptionMax;

		x = m_StOption[ n ].x;
		y = m_StOption[ n ].y;

		viiDraw::Sprite( &SprCEne0001EneSuperBall[ 0 ] , x , y , PRIO_ENE_BULLET , ATR_ALPHA_PLUS , sCol ,m_fScale-(m_fScale*0.15f*sCnt) );

		sCnt += 1;
	}

	viiDraw::Sprite( &SprCEne0001EneSuperBall[ 0 ] , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET , ATR_ALPHA_PLUS , 0xA0ff0000 ,m_fScale);
	viiDraw::Sprite( &SprCEne0001EneSuperBall[ 0 ] , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET , ATR_ALPHA_PLUS , 0x30ffffff ,m_fScale*1.2f);

//	viiDraw::Sprite( &SprCEne0001EneSuperBall[ 0 ] , m_Pos.x+m_Tgt.x*80 , m_Pos.y+m_Tgt.y*80 , PRIO_ENE_BULLET , ATR_DFLT , 0xF00000ff );

}


//--------------------------------------------------------------------------------
//
// Stage����:�n�C�p�[�{�[���i����e�ہj
//
//--------------------------------------------------------------------------------

CEne0001EneHyperBall::CEne0001EneHyperBall(  Sint32 x, Sint32 y ,Sint32 r ,Sint32 spd )
{
	//-----------------------------------------------------
	//�R���X�g���N�^
	//-----------------------------------------------------
	m_Add.x = 0;
	m_Add.y = 0;
	m_Add.z = 0;

	m_Pos.x = x;
	m_Pos.y = y;

	m_Tgt.x = 0;
	m_Tgt.y = 0;
	m_Tgt.z = 0;

	m_Drop.x = 0;
	m_Drop.y = 0;

	m_HitAtari.set_hp( HP_ARMORCLASS_B );
	m_HitAtari.set_ap( HP_ARMORCLASS_B );

	m_HitAtari.ax1 = -46;//enKuraiLeft;
	m_HitAtari.ay1 = -46;//enKuraiTop;
	m_HitAtari.ax2 =  46;//enKuraiRight;
	m_HitAtari.ay2 =  46;//enKuraiBottom;

	for(Sint32 ii=0;ii<enOptionMax;ii++)
	{
		m_StOption[ii].x = m_Pos.x;
		m_StOption[ii].y = m_Pos.y;
		m_StOption[ii].z = 0;
	}

	m_bDrop = gxFalse;
	m_sOptionCnt = 0;

	m_sSpeed    = spd;
	m_sRotation = r;
	m_pSprite = NULL;
	m_sChakutiCnt = 0;

	m_sWait = 0;
	m_sPattern = 0;

	m_bEndless = gxFalse;
	m_bChakuti = gxFalse;

	m_sMaxSpeed = 640;

	m_sScrollOutAdd = enScrollOutWidth*2;
}


CEne0001EneHyperBall::~CEne0001EneHyperBall()
{
	//-----------------------------------------------------
	//�f�X�g���N�^
	//-----------------------------------------------------


}


void CEne0001EneHyperBall::SeqMain()
{
	//-----------------------------------------------------
	//���C��
	//-----------------------------------------------------
	m_bFlash = gxFalse;

	if( m_sChakutiCnt >= 180 )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( !m_bChakuti  )
	{
		if( viiSub::GetBoundBlock_Enemy( m_Pos.x , m_Pos.y ) )
		{
			m_Tgt.x = 0;
			m_Tgt.y = 0;
			m_bChakuti = gxTrue;
		}
	}

	if( m_HitAtari.IsImpacted() || m_bChakuti )
	{
		if( viiSub::Rand()%3==0 )
		{
			m_bFlash = gxTrue;
		}
		m_sChakutiCnt ++;

		if( m_sTimer%12 == 0)
		{
			//viiEff::Set(EFF_BIGBOMB,m_Pos.x+(-48+viiSub::Rand()%96)*100, m_Pos.y+(-48+viiSub::Rand()%96)*100,argset(1));
			viiEff::SetBombDeluxe( m_Pos.x, m_Pos.y , 1 , 5 );
		}
	}

	if( pGame->GetMarineLine() && m_Pos.y > pGame->GetMarineLine()+800 )
	{
		m_bChakuti = gxTrue;
	}

	if( viiSub::Rand()%16==0 )
	{
		m_bFlash = gxTrue;
	}

	if( !m_bEndless && IsScrollOut( m_sScrollOutAdd , m_sScrollOutAdd ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	LogicAI();

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	m_StOption[ m_sOptionCnt ].x = m_Pos.x;
	m_StOption[ m_sOptionCnt ].y = m_Pos.y;

	m_sOptionCnt ++;
	m_sOptionCnt = m_sOptionCnt%enOptionMax;

	//���炢����o�^
	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
	m_HitAtari.SetHantei( ID_ENEMY_ATK , &m_Pos );

}


void CEne0001EneHyperBall::LogicAI()
{
	//-----------------------------------------------------
	//�ړ����W�b�N
	//-----------------------------------------------------

	switch( m_sPattern ){
	case enPatternDefault:
		LogicHoming();
		break;
	case enPatternTargetting:
		LogicTargetting();
		break;
	}

}

void CEne0001EneHyperBall::LogicTargetting()
{
	//-----------------------------------------------------
	//�^�[�Q�b�g�����߂ēːi����
	//-----------------------------------------------------
	if( m_sWait < 180 )
	{
		m_sWait ++;
		//m_sRotation = get_valken_rot( m_Pos.x , m_Pos.y );
		m_Tgt.x = 0;
		m_Tgt.y = 0;
	}

	if( m_sSpeed <= 480 )
	{
		m_sSpeed += 6;
	}

	if( m_bChakuti )
	{
		if( m_sSpeed >= 120 )
		{
			m_sSpeed = 120;
		}
	}

	m_Tgt.x += (viiMath::Cos100(m_sRotation)*m_sSpeed)/100;
	m_Tgt.y += (viiMath::Sin100(m_sRotation)*m_sSpeed)/100;

	if( m_bDrop )
	{
		if( m_Drop.y < 800 ) m_Drop.y += 8;
	}

	m_Add.x = m_Tgt.x/100;
	m_Add.y = m_Tgt.y/100+m_Drop.y;

}


void CEne0001EneHyperBall::LogicHoming()
{
	//-----------------------------------------------------
	//�^�[�Q�b�g�����߂ēːi����
	//-----------------------------------------------------
	if( m_sWait < 180 )
	{
		m_sWait ++;
		m_sRotation = get_valken_rot( m_Pos.x , m_Pos.y );
		m_Tgt.x = 0;
		m_Tgt.y = 0;
	}

	if( m_sSpeed <= 480 )
	{
		m_sSpeed += 6;
	}

	if( m_bChakuti )
	{
		if( m_sSpeed >= 120 )
		{
			m_sSpeed = 120;
		}
	}

	m_Tgt.x += (viiMath::Cos100(m_sRotation)*m_sSpeed)/100;
	m_Tgt.y += (viiMath::Sin100(m_sRotation)*m_sSpeed)/100;

	if( m_bDrop )
	{
		if( m_Drop.y < 800 ) m_Drop.y += 8;
	}

	m_Add.x = m_Tgt.x/100;
	m_Add.y = m_Tgt.y/100+m_Drop.y;
}


void CEne0001EneHyperBall::Atack()
{
	//-----------------------------------------------------
	//�U���\�Ȃ�U������
	//-----------------------------------------------------

}


void CEne0001EneHyperBall::SeqCrash()
{
	//-----------------------------------------------------
	//�j�󂳂ꂽ
	//-----------------------------------------------------

	if( pGame->GetMarineLine() )
	{
		if( m_Pos.y > pGame->GetMarineLine() )
		{
			MakeSplash( m_Pos.x , pGame->GetMarineLine() );
		}
	}
	else
	{
//		new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x+(-4+viiSub::Rand()%8)*100,m_Pos.y+(-4+viiSub::Rand()%8)*100 , PRIO_ENEMY , 270+45 ,40 );
//		new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x+(-4+viiSub::Rand()%8)*100,m_Pos.y+(-4+viiSub::Rand()%8)*100 , PRIO_ENEMY , 270+45 ,120);
//		new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x+(-4+viiSub::Rand()%8)*100,m_Pos.y+(-4+viiSub::Rand()%8)*100 , PRIO_ENEMY , 270+45 ,80 );
		viiEff::Set(EFF_BIGBOMB,m_Pos.x+(-48+viiSub::Rand()%96)*100, m_Pos.y+(-48+viiSub::Rand()%96)*100,argset(5));
	}

	SetActionSeq( enActionSeqEnd );

}


void CEne0001EneHyperBall::Draw()
{
	//-----------------------------------------------------
	//�`��
	//-----------------------------------------------------

	Sint32 sCnt = 0;
	Sint32 sCol = 0xffffffff;
	Float32 fScale = 0.8f*m_sWait/120.f;
	Float32 fScale2 = fScale*0.2f*viiMath::Cos100((m_sTimer*1)%360)/100.f;
	
	if( viiSub::Rand()%3 == 0 ) viiDraw::Sprite( &SprCEne0001EneHyperBall[ 0 ] , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET , ATR_DFLT       , viiSub::GetRandColor(0x40) ,fScale2);
	if( viiSub::Rand()%3 == 0) viiDraw::Sprite( &SprCEne0001EneHyperBall[ 1 ] , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET , ATR_ALPHA_PLUS , viiSub::GetRandColor(0x40) ,fScale2);
	if( viiSub::Rand()%3 ) viiDraw::Sprite( &SprCEne0001EneHyperBall[ 2 ] , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET , ATR_ALPHA_PLUS , 0x60ffffff ,fScale+0.6f , (m_sTimer*1)%360);
	if( viiSub::Rand()%3 ) viiDraw::Sprite( &SprCEne0001EneHyperBall[ 3 ] , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET , ATR_ALPHA_PLUS , 0x60ffffff ,fScale+0.65f , ((m_sTimer*1)%360)*-1);

	if( m_bFlash )
	{
		viiDraw::Sprite( &SprCEne0001EneHyperBall[ 0 ] , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET , ATR_ALPHA_PLUS , 0x80ffffff ,fScale+0.1f);
	}
}


