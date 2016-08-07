//-------------------------------------------------
//�����L�A�R�R�p�T�C��
//-------------------------------------------------
#include "HoundsEnemy.h"

gxSprite SprSignal[]=
{
	{enTexPageEffect3,96,0,32,32,16,16},
};

static Sint8 sSignal[][32]={
	{1,1,0,0,1,0,1,0,1,1,1,0,0,0,0,0,0,0,-1,},
	{1,1,1,1,0,0,0,0,-1,},
};

gxSprite SprSpark[]=
{
	{enTexPageEffect3,80*0,200,80,24,0,12},
	{enTexPageEffect3,80*1,200,80,24,0,12},
	{enTexPageEffect3,80*2,200,80,24,0,12},
};


CEneCmnSignal::CEneCmnSignal()
{
	m_Pos.x = 0;
	
	m_sTimer = 0;
	m_sAlpha = 0;
	m_sScale = 100;
	m_bSignal = gxFalse;
	m_sType   = enSignalTypeSOS;
}


CEneCmnSignal::~CEneCmnSignal()
{
	
	
	
}


void CEneCmnSignal::SetSignal(Sint32 x, Sint32 y,Sint32 sType)
{
	//�V�O�i���̃Z�b�g
	m_sType = sType;

	m_Pos.x = x;
	m_Pos.y = y;


	Sint32 sSeq = sSignal[m_sType][m_sTimer/enSignalInterval];

	if( sSeq == -1)
	{
		m_sTimer = 0;
		sSeq = sSignal[m_sType][m_sTimer/enSignalInterval];
	}
	if(m_sTimer%enSignalInterval==0)
	{
		if(sSeq)
		{
			m_sAlpha = 100;
			m_sScale = 150;
		}
	}

	if(m_sAlpha > 0) m_sAlpha-=12;
	if(m_sScale > 0) m_sScale-=12;

	if( m_sAlpha < 0)
	{
		m_sAlpha = 0;
	}
	if( m_sScale < 0)
	{
		m_sScale = 0;
	}

	m_sTimer ++;

}


void CEneCmnSignal::Draw()
{
	if( m_bSignal )
	{
		viiDraw::Sprite( &SprSignal[ 0 ] , m_Pos.x , m_Pos.y , PRIO_EFFECT , ATR_ALPHA_PLUS , ARGB((0xFF*m_sAlpha/100),0xFF,0xFF,0xFF) , m_sScale/100.0f );
	}
	
}


//-------------------------------------------------
//�A�j���c�[���̃f�[�^�擾�p
//-------------------------------------------------

CEnemyAnime::CEnemyAnime()
{
	m_sPatrtsMax = 0;
	m_sPrio = PRIO_ENEMY;

	for(Sint32 ii=0; ii<enPartsMax; ii++)
	{
		m_StPartsInfo[ii] = new StPartsInfo;
	}
}


CEnemyAnime::~CEnemyAnime()
{

	for(Sint32 ii=0; ii<enPartsMax; ii++)
	{
		delete m_StPartsInfo[ii];
	}

}


void CEnemyAnime::SetParts( Sint32 id , Sint32 sMax , Sint32 sParent , Sint32 *pPatrs )
{
	StPartsInfo *p;

	m_sPatrtsMax = sMax;

	p = m_StPartsInfo[id];

	p->sParent = sParent;
	p->sSprite = pPatrs[9];
	p->sDispSprite = p->sSprite;

	p->x  = pPatrs[0];
	p->y  = pPatrs[1];
	p->z  = pPatrs[2];
	p->rx = pPatrs[3];
	p->ry = pPatrs[4];
	p->rz = pPatrs[5]*-1;
	p->a  = pPatrs[6];
	p->sx = pPatrs[7];
	p->sy = pPatrs[8];

	if(id==0)
	{
		//�V�[�����͖�������
		p->x= 0;
		p->y= 0;
	}

}


void CEnemyAnime::PosCalc()
{
	StPartsInfo *p,*q;

	for(Sint32 ii=0; ii<m_sPatrtsMax ;ii++)
	{
		p = m_StPartsInfo[ii];

		if( p->sParent == -1 )
		{
			p->ox = p->x;
			p->oy = p->y;
			p->oz = p->z;
 		}
		else
		{
			q = m_StPartsInfo[ p->sParent ];
			p->ox = q->ox + p->x;
			p->oy = q->oy + p->y;
			p->oz = q->oz + p->z;
		}

		p->dx = p->ox;
		p->dy = p->oy;
		p->dz = p->oz;
		p->dr = p->rz;
	}

}

gxBool CEnemyAnime::GetPartsDist( Sint32 sParts , StPatrsOffset* pStPatrsOffset )
{
	//--------------------------------------------------
	// �p�[�c�̋��������Z�b�g����
	//--------------------------------------------------

	Float32 dx,dy;
	Sint32 sParent;

	sParent = GetParts( sParts )->sParent;

	if( sParent == -1 )
	{
//		return;
	}

	dx = GetParts( sParts )->dx - GetParts( sParent )->dx*1.0f;
	dy = GetParts( sParts )->dy - GetParts( sParent )->dy*1.0f;

	pStPatrsOffset->dist = (Sint32)viiMath::Dist(dx,dy);
	pStPatrsOffset->rot  = (Sint32)viiMath::Atan2d( dy , dx );

	return gxTrue;

}


void CEnemyAnime::GetPos( ghPos *pPos , Sint32 num , Sint32 sDirection )
{
	pPos->x += GetParts( num )->dx*100*sDirection;
	pPos->y += GetParts( num )->dy*100;
}


void CEnemyAnime::Draw( gxBool bDamage , Sint32 x , Sint32 y , gxBool bReverse ,gxSprite *pSpr)
{
	//--------------------------------------------
	//�\��
	//--------------------------------------------

	Sint32 sReverse = 1;
	Sint32 sAtr = ATR_DFLT;
	StPartsInfo *p;
	gxBool bDmg;
	for(Sint32 ii=0; ii<m_sPatrtsMax ;ii++)
	{
		p = GetParts( ii );

		sReverse = 1;
		sAtr = ATR_DFLT;

		if( bReverse )
		{
			sReverse = -1;
			sAtr = ATR_FLIP_X;
		}

		if( !p->bDisp )
		{
			continue;
		}
		bDmg = bDamage;

		if( p->bNoDamage )
		{
			bDmg = gxFalse;
		}

		viiDraw::Sprdmg(
			bDmg ,
			&pSpr[ p->sDispSprite ] ,
			x + p->dx*100*sReverse,
			y + p->dy*100,
			/*PRIO_ENEMY*/m_sPrio + p->dz,
			sAtr,
			ARGB_DFLT,
			p->sx/100.f,
			p->dr);
	}
}


//-------------------------------------------------
//
// �L���^�s���ғ����[�`��
//
//-------------------------------------------------

void CCatapiller::SetSpeed(Sint32 sSpeed)
{
	//-------------------------------------------------
	//�L���^�s���̉�]���x��ݒ�
	//-------------------------------------------------

	m_sSpeed = sSpeed;

}

void CCatapiller::SetSpriteIndex( Sint32 sMin , Sint32 sMax )
{
	//-------------------------------------------------
	//�L���^�s���̍ŏ��ƍŌ�̃X�v���C�g��o�^
	//-------------------------------------------------

	m_sSprMin = sMin;
	m_sSprMax = sMax;
	m_sDist   = m_sSprMax - m_sSprMin + 1;
}

void CCatapiller::MoveLeft()
{
	//-------------------------------------------------
	//����]
	//-------------------------------------------------
	if( m_sAdd > -100 )
	{
		 m_sAdd -= m_sSpeed;
		if( m_sAdd <= -100 )
		{
			m_sAdd = -100;
		}
	}

}

void CCatapiller::MoveRight()
{
	//-------------------------------------------------
	//�E��]
	//-------------------------------------------------

	if( m_sAdd < 100 )
	{
		 m_sAdd += m_sSpeed;
		if( m_sAdd >= 100 )
		{
			m_sAdd = 100;
		}
	}
}

void CCatapiller::MoveNone()
{
	//-------------------------------------------------
	//�����Ȃ��Ƃ��i�������߂��j
	//-------------------------------------------------

	if( m_sAdd > 0 )
	{
		m_sAdd -=2;
		if( m_sAdd < 0 )
		{
			m_sAdd = 0;
		}
	}
	else if( m_sAdd < 0 )
	{
		m_sAdd +=2;
		if( m_sAdd > 0 )
		{
			m_sAdd = 0;
		}
	}
}

void CCatapiller::Action( CEnemyAnime* pAnime )
{
	//-------------------------------------------------
	//���t���[���̏���
	//-------------------------------------------------

	m_sRotation += m_sAdd/10;
	m_sRotation = (100+m_sRotation)%100;

	Sint32 tgt,now;

	for(Sint32 ii=0; ii<m_sDist; ii++)
	{
		//�L���^�s����]����
		Sint32 src = ii;
		Sint32 next = ii+1;
		next = (m_sDist + next)%m_sDist;
		src  = m_sSprMin+ii;
		next = m_sSprMin+next;

		pAnime->GetParts( src )->dx += m_sRotation*(pAnime->GetParts( next )->ox - pAnime->GetParts( src )->ox)/100; 
		pAnime->GetParts( src )->dy += m_sRotation*(pAnime->GetParts( next )->oy - pAnime->GetParts( src )->oy)/100; 


		tgt = pAnime->GetParts( next )->rz;
		now = pAnime->GetParts( src )->rz;
		if ( ( tgt - now ) <= -180 )
		{
			now = now - 360;
		}
		else  if ( ( tgt - now ) >= 180 )
		{
			now = now + 360;
		}

		pAnime->GetParts( src )->dr  = now + m_sRotation*( tgt - now )/100;
	}

}

Sint32 CCatapiller::GetRotation()
{
	//-------------------------------------------------
	//���݂̉�]�x�𓾂�
	//-------------------------------------------------

	return m_sRotation;
}



//-------------------------------------------------
//�d���X�p�[�N
//-------------------------------------------------

CEneCmnSpark::CEneCmnSpark(Sint32 x1, Sint32 y1 ,Sint32 x2,Sint32 y2)
{

	m_sTimer = 0;
	m_sAlpha = 0;

	m_sDist = 1;
	m_sLoop   = 1;
	m_fRot    = ZERO;
	m_fTgtRot = ZERO;
	m_fRotAdd = ZERO;
	m_fScale  = 1.f;
	m_sCos = viiSub::Rand()%360;
	m_bRotSet = gxFalse;
	m_fRotSet = ZERO;

	Set(x1,y1,x2,y2);

	m_bScaleControl = gxFalse;
}


CEneCmnSpark::~CEneCmnSpark()
{
	
	
	
}


void CEneCmnSpark::Set(Sint32 x1, Sint32 y1 ,Sint32 x2,Sint32 y2)
{
	//-------------------------------------------------
	//�Q�_�Ԃ��狗���v�Z
	//-------------------------------------------------

	m_Pos.x = x1;
	m_Pos.y = y1;

	m_Tgt.x = x2;
	m_Tgt.y = y2;

	x1 = x1/100;
	y1 = y1/100;
	x2 = x2/100;
	y2 = y2/100;

	m_fTgtRot = viiMath::Atan2_100(y2-y1,x2-x1)/100;
	m_fRot    = m_fTgtRot-80+viiSub::Rand()%160;
	if( m_bRotSet )
	{
		m_fRot = m_fRotSet;
	}
//	m_fRot    = m_fTgtRot-45+(90*viiMath::Cos100(m_sCos))/100;
//	m_sCos += 2;
//	m_sCos = m_sCos%360;

	m_sDist = viiMath::Dist( (y2-y1) , (x2-x1) );

	if(m_sDist>=80)
	{
		m_sLoop = m_sDist/80+1;
		m_fScale = (1.0f*m_sDist/m_sLoop)/80.f;
		m_fRotAdd = (m_fTgtRot - m_fRot)/m_sLoop;
	}
	else
	{
		m_sLoop = 1;
		m_fScale = (1.0f*m_sDist)/80.f;
		m_fRotAdd = 0;
	}

	if( !m_bScaleControl ) m_fScale = 1.0f;

}

void CEneCmnSpark::Action()
{
	
}


void CEneCmnSpark::Draw()
{
	Sint32 x,y,x2,y2;
	Float32 r,tr=0.f;

	x = m_Pos.x;
	y = m_Pos.y;
	r = m_fRot;

	x2 = m_Tgt.x;
	y2 = m_Tgt.y;

	Sint32 rnd= 0;

	for(Sint32 ii=0;ii<32;ii++)
	{
		rnd = viiSub::Rand()%3;
		viiDraw::Sprite( &SprSpark[ rnd ] , x , y , PRIO_ENEMY-1 , ATR_ALPHA_PLUS , 0xFFFFFFFF , m_fScale ,(Sint32)r );
		x += viiMath::Cos100(r)*(80*m_fScale);
		y += viiMath::Sin100(r)*(80*m_fScale);

		m_fTgtRot = viiMath::Atan2_100(y2-y,x2-x)/100;
		Sint32 b = viiMath::GetTargetAngle( r ,m_fTgtRot );
		r += b/(1.5f-tr);
		if( viiMath::Dist( (y2-y) , (x2-x) ) <= 64*100 )  break;
		tr += 0.05f;
	}
}


//-------------------------------------------------
//
// ����R���g���[��
//
//-------------------------------------------------
CEntranceControl::CEntranceControl( Sint32 sMode , Sint32 sDir )
{
	//------------------------------------
	//����R���g���[��
	//------------------------------------

	m_sMode     = sMode;
	m_sSequence = 0;
	m_sSeqWait  = 0;

	m_Pos.x = pGame->GetHound()->m_Pos.x;
	m_Pos.y = pGame->GetHound()->m_Pos.y;

	m_Add.x =  480;
	m_Add.y = -300;

}


CEntranceControl::~CEntranceControl( )
{

}


void CEntranceControl::SeqMain()
{

	switch(m_sMode){
	case enExit:
		//�ړ����đޏo
		OutofScreen();
		break;

	case enEntrance:
		//��яo���X�^�[�g
		InofScreen();
		break;

	default:
		pGame->GetHound()->SetTobidashi(gxFalse);
		SetActionSeq( enActionSeqEnd );
		break;
	}
}


void CEntranceControl::InofScreen()
{
	//------------------------------------
	//�ړ����đޏo
	//------------------------------------

	pGame->GetHound()->SetTobidashi();

	switch( m_sSequence ){
	case 0:
		//�������ςȂ�
		if( m_Add.y < 800 ) m_Add.y += 12;

		m_Pos.x += m_Add.x;
		m_Pos.y += m_Add.y;

		if( m_sSeqWait >= 40 )
		{
			pGame->pBg->set_atari_flag();
			if( pGame->GetHound()->IsChakuti() )
			{
				m_sSequence = 999;
			}
//			else if( viiSub::IsGroundBlock( m_Pos.x , m_Pos.y ) )
			else if( pGame->pBg->get_kabetype(m_Pos.x/100,m_Pos.y/100) )
			{
				m_sSequence = 999;
			}
			else
			{
				pGame->GetHound()->m_Pos.x = m_Pos.x;
		 		pGame->GetHound()->m_Pos.y = m_Pos.y;
			}
		}
		else
		{
/*
			if( pGame )
			{
				pGame->pBg->cancel_atari_flag();
			}
*/
			pGame->GetHound()->m_Pos.x = m_Pos.x;
	 		pGame->GetHound()->m_Pos.y = m_Pos.y;
		}
		break;

	case 999:
		m_sMode = -999;
		break;
	}

	m_sSeqWait ++;
}


void CEntranceControl::OutofScreen()
{
	//--------------------------------------
	//��ʊO�ֈړ�����
	//--------------------------------------

	pGame->GetHound()->SetTobidashi();

	switch( m_sSequence ){
	case 0:
		//�������ςȂ�
		pGame->GetHound()->SetPadButton( JOY_R );
		if( m_sSeqWait == 180 )
		{
			m_sSequence = 999;
		}
		break;

	case 999:
		m_sMode = -999;
		break;
	}


	m_sSeqWait  ++;

}


void CEntranceControl::Draw()
{
	
}


