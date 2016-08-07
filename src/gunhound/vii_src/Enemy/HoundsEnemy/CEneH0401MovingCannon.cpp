//--------------------------------------------------------------------------------
//
// Stage00:�T���v���G�l�~�[
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "EnemyTbl/CEneH0401MovingCannonTBL.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -30,
	enKuraiTop   = -34,
	enKuraiRight =  30,
	enKuraiBottom=  0,

	enScore = 300,
};


enum {
	enSlide_UP,
	enSlide_DOWN,
	enSlide_LEFT,
	enSlide_RIGHT,
};

static Sint32 m_sControl = 0;
static Sint32 m_sStatus  = enSlide_UP;
static Sint32 m_sWait    = 0;
static Sint32 m_sKakudo  = 0;

/*
gxSprite SprCEneH0401MovingCannon[]=
{
	{TPG_LEYNOS01,0,192,64,64,32,64},//�{���]�C
};
*/

CEneH0401MovingCannon::CEneH0401MovingCannon( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//�R���X�g���N�^
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_HitKurai.set_hp( HP_ARMORCLASS_B );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_RIGHT;
	m_sAtribute = ATR_DFLT;

	m_bDamage = gxFalse;
	m_pEnemyAnime = new CEnemyAnime();

	for(Sint32 ii=0; ii<enMax ;ii++)
	{
		m_pEnemyAnime->SetParts( ii , enMax , m_sParentCEneH0401MovingCannon[ii] , sPosCEneH0401MovingCannon[ii] );
	}

	m_pEnemyAnime->PosCalc();

	while( !viiSub::IsGroundBlock(m_Pos.x,m_Pos.y) )
	{
		m_Pos.y += 100;
	}

}


CEneH0401MovingCannon::~CEneH0401MovingCannon()
{

	//-----------------------------------------------------
	//�f�X�g���N�^
	//-----------------------------------------------------

	delete m_pEnemyAnime;
}


void CEneH0401MovingCannon::SeqMain()
{
	//-----------------------------------------------------
	//���C��
	//-----------------------------------------------------

	if( m_HitKurai.is_dead() )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}

	LogicAI();

	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	//���炢����o�^
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

}


void CEneH0401MovingCannon::LogicAI()
{
	//-----------------------------------------------------
	//�ړ����W�b�N
	//-----------------------------------------------------
	Sint32 ax=0,ay=0;

	m_pEnemyAnime->PosCalc();

	if( m_sWait > 0 )
	{
		m_sWait --;
	}
	else
	{
		m_sControl = Joy[0].psh;
	}

	//�R���g���[��

	if( m_sStatus == enSlide_UP )
	{
		if( m_sControl&JOY_L )
		{
			ax --;
		}
		if( m_sControl&JOY_R )
		{
			ax ++;
		}
	}
	else if( m_sStatus == enSlide_DOWN )
	{
		if( m_sControl&JOY_L )
		{
			ax ++;
		}
		if( m_sControl&JOY_R )
		{
			ax --;
		}
	}
	else if( m_sStatus == enSlide_LEFT )
	{
		if( m_sControl&JOY_L )
		{
			ay ++;
		}
		if( m_sControl&JOY_R )
		{
			ay --;
		}
	}
	else if( m_sStatus == enSlide_RIGHT )
	{
		if( m_sControl&JOY_L )
		{
			ay --;
		}
		if( m_sControl&JOY_R )
		{
			ay ++;
		}
	}

	m_Pos.x += ax*100;
	m_Pos.y += ay*100;

	//�ǂ̃`�F�b�N

	if( m_sStatus == enSlide_UP )
	{
		if( !viiSub::IsGroundBlock(m_Pos.x,m_Pos.y+100) )
		{
			m_sWait = 8;

			if( ax < 0 )
			{
				//���Ɉړ����Ă���΍��̕ǂɋz��������
				m_sStatus = enSlide_LEFT;
				m_sWait = 8;

			}else  if( ax > 0 )
			{
				//�E�Ɉړ����Ă���ΉE�̕ǂɋz��������
				m_sStatus = enSlide_RIGHT;
				m_sWait = 8;
			}
		}
	}
	else if( m_sStatus == enSlide_DOWN )
	{
		if( !viiSub::IsGroundBlock(m_Pos.x,m_Pos.y-100) )
		{
			m_sWait = 8;

			if( ax < 0 )
			{
				//���Ɉړ����Ă���΍��̕ǂɋz��������
				m_sStatus = enSlide_LEFT;

			}else  if( ax > 0 )
			{
				//�E�Ɉړ����Ă���ΉE�̕ǂɋz��������
				m_sStatus = enSlide_RIGHT;
			}
		}
	}
	else if( m_sStatus == enSlide_LEFT )
	{
		if( !viiSub::IsGroundBlock(m_Pos.x+100,m_Pos.y) )
		{
			m_sWait = 8;

			if( ay < 0 )
			{
				//��Ɉړ����Ă���Ώ�̕ǂɋz��������
				m_sStatus = enSlide_UP;

			}else  if( ay > 0 )
			{
				//���Ɉړ����Ă���Ή��̕ǂɋz��������
				m_sStatus = enSlide_DOWN;
			}
		}
	}
	else if( m_sStatus == enSlide_RIGHT )
	{
		if( !viiSub::IsGroundBlock(m_Pos.x-100,m_Pos.y) )
		{
			m_sWait = 8;

			if( ay < 0 )
			{
				//��Ɉړ����Ă���Ώ�̕ǂɋz��������
				m_sStatus = enSlide_UP;

			}else  if( ay > 0 )
			{
				//���Ɉړ����Ă���Ή��̕ǂɋz��������
				m_sStatus = enSlide_DOWN;
			}
		}
	}

	Sint32 sKakudo   = 0;
	Sint32 sRotation = 0;

	if( m_sStatus == enSlide_UP )
	{
		sKakudo = 0;
		sRotation = viiMath::GetTargetAngle( m_sKakudo , sKakudo );
		if( gxAbs(sRotation/10) < 1 )
		{
			m_sKakudo += sRotation;
		}
		else
		{
			m_sKakudo += sRotation/10;
		}
	}
	else if( m_sStatus == enSlide_DOWN )
	{
		sKakudo = 180;
		sRotation = viiMath::GetTargetAngle( m_sKakudo , sKakudo );
		if( gxAbs(sRotation/10) < 1 )
		{
			m_sKakudo += sRotation;
		}
		else
		{
			m_sKakudo += sRotation/10;
		}
		m_sKakudo += sRotation/10;

	}
	else if( m_sStatus == enSlide_LEFT )
	{
		sKakudo = 270;
		sRotation = viiMath::GetTargetAngle( m_sKakudo , sKakudo );
		if( gxAbs(sRotation/10) < 1 )
		{
			m_sKakudo += sRotation;
		}
		else
		{
			m_sKakudo += sRotation/10;
		}
		m_sKakudo += sRotation/10;

	}
	else if( m_sStatus == enSlide_RIGHT )
	{
		sKakudo = 90;
		sRotation = viiMath::GetTargetAngle( m_sKakudo , sKakudo );
		if( gxAbs(sRotation/10) < 1 )
		{
			m_sKakudo += sRotation;
		}
		else
		{
			m_sKakudo += sRotation/10;
		}
		m_sKakudo += sRotation/10;

	}

	m_pEnemyAnime->GetParts( enKYATAPIRA1 )->dr = m_sKakudo;

}


void CEneH0401MovingCannon::Atack()
{
	//-----------------------------------------------------
	//�U���\�Ȃ�U������
	//-----------------------------------------------------

}


void CEneH0401MovingCannon::SeqCrash()
{
	//-----------------------------------------------------
	//�j�󂳂ꂽ
	//-----------------------------------------------------

	viiMus::PlaySound(enSoundExplosionMiddle);

	ScorePlus( enScore );

	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

	SetActionSeq( enActionSeqEnd );

}

void CEneH0401MovingCannon::Draw()
{

	viiDraw::Sprdmg( m_HitKurai.is_damage() , &SprCEneH0401MovingCannon[ m_pEnemyAnime->GetParts( enKYATAPIRA1 )->sSprite ] , m_Pos.x , m_Pos.y,PRIO_ENEMY , m_sAtribute|m_sAtributeDir , ARGB_DFLT , 1.0f , m_pEnemyAnime->GetParts( enKYATAPIRA1 )->dr);
//	m_pEnemyAnime->Draw( m_bDamage , m_Pos.x , m_Pos.y , ( m_sDirection == DIR_LEFT )? gxTrue : gxFalse , &SprCEneH0401MovingCannon[0] );

}

