//--------------------------------------------------------------------------------
//
// Stage00:�_�~�[�G�l�~�[
//
//--------------------------------------------------------------------------------

#include <gunvalken.h>
#include "enemy.h"

enum {
	enLogicWaitTime   = 120,
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -40,
	enKuraiTop   = -40,
	enKuraiRight =  40,
	enKuraiBottom=  -8,
	enScore = 300,
};

//�_�~�[�̓G�ݒ�
StDummyEnemyConfig   g_stDummyEnemyConfig[enDummyEnemyMax];

//�_�~�[�̓G�o���҂��o�b�t�@
StDummyEnemyWaitList g_StDummyEnemyWaitList[ enDummyEnemyWaitListMax ];

void DummyEnemyWaitInit()
{
	//----------------------------------------------
	//�o���҂�������������
	//----------------------------------------------
	Sint32 ii;

	for(ii=0;ii<enDummyEnemyMax;ii++)
	{
		g_stDummyEnemyConfig[ii].Init();
	}


	for(Sint32 ii=0;ii<enDummyEnemyWaitListMax;ii++)
	{
		g_StDummyEnemyWaitList[ii].Init();
	}

}

void DummyEnemyWaitCheck(Sint32 sTime)
{
	//----------------------------------------------
	//���Ԃ�����܂ŏo����҂�(���t���[���`�F�b�N)
	//----------------------------------------------

	//�S���o���̊m�F���s��
	for(Sint32 ii=0;ii<enDummyEnemyWaitListMax;ii++)
	{
		//�܂��o�����Ă��Ȃ����
		if( g_StDummyEnemyWaitList[ii].bStart == gxFalse && g_StDummyEnemyWaitList[ii].bExist)
		{
			//������������
			if( g_StDummyEnemyWaitList[ii].sWaitTime == sTime )
			{
				//�o���m��
				Sint32 sType = g_StDummyEnemyWaitList[ii].sEneIndex;
				if( sType >= 1000 )
				{
					Sint32 x,y,no;
					no = 					sType;
					x = g_StDummyEnemyWaitList[ii].sX;
					y = g_StDummyEnemyWaitList[ii].sY;

					CEne0000Dummy *p;
					g_StDummyEnemyWaitList[ii].bStart = gxTrue;
					p = new CEne0000Dummy( sType-1000 , g_StDummyEnemyWaitList[ii].sX , g_StDummyEnemyWaitList[ii].sY );
					p->SetTargetPos(g_StDummyEnemyWaitList[ii].sTx);

					viiDbg::log("�y�G(%d)�����zX=%d,Y=%d",no,x,y);
				}
				else
				{
					Sint32 x,y,no;
					no = sType;
					x = g_StDummyEnemyWaitList[ii].sX;
					y = g_StDummyEnemyWaitList[ii].sY;

					viiDbg::log("�y�G(%d)�����zX=%d,Y=%d",no,x,y);

					viiSub::MakeEnemy( sType , x , y );
				}
			}
		}
	}

}


CEne0000Dummy::CEne0000Dummy( Sint32 sEneIndex , Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//�R���X�g���N�^
	//-----------------------------------------------------
	m_sEnemyIndex = sEneIndex;

	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_ENEMY;

	m_bDamage = gxFalse;

	m_Obj.pos.x = m_Pos.x = x;
	m_Obj.pos.y = m_Pos.y = y;

	m_Pos.mx = 60;
	m_Pos.my = 0;

	m_bDisp = gxTrue;

	m_sAtackTimer = -60;
}


CEne0000Dummy::~CEne0000Dummy()
{

	//-----------------------------------------------------
	//�f�X�g���N�^
	//-----------------------------------------------------

}


void CEne0000Dummy::SeqInit()
{

/*
	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}
*/

	m_HitKurai.ax1 = g_stDummyEnemyConfig[ m_sEnemyIndex ].sKurai_u;
	m_HitKurai.ay1 = g_stDummyEnemyConfig[ m_sEnemyIndex ].sKurai_v;
	m_HitKurai.ax2 = g_stDummyEnemyConfig[ m_sEnemyIndex ].sKurai_w;
	m_HitKurai.ay2 = g_stDummyEnemyConfig[ m_sEnemyIndex ].sKurai_h;
	m_HitKurai.set_hp( g_stDummyEnemyConfig[ m_sEnemyIndex ].sKurai_dmg );

	m_HitAtari.ax1 = g_stDummyEnemyConfig[ m_sEnemyIndex ].sAtari_u;
	m_HitAtari.ay1 = g_stDummyEnemyConfig[ m_sEnemyIndex ].sAtari_v;
	m_HitAtari.ax2 = g_stDummyEnemyConfig[ m_sEnemyIndex ].sAtari_w;
	m_HitAtari.ay2 = g_stDummyEnemyConfig[ m_sEnemyIndex ].sAtari_h;
	m_HitAtari.set_ap( g_stDummyEnemyConfig[ m_sEnemyIndex ].sAtari_dmg );
	m_HitAtari.set_hp( 100 );

	SetActionSeq( enActionSeqMain );
}



void CEne0000Dummy::SeqMain()
{
	//-----------------------------------------------------
	//���C��
	//-----------------------------------------------------

	if( g_stDummyEnemyConfig[ m_sEnemyIndex ].m_bKurai )
	{
		if( m_HitKurai.is_dead() )
		{
			SetActionSeq( enActionSeqCrash );
		}
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
//		LongRangeAtack();
//		return;
	}

	//-----------------------------------------------------
	//�_���[�W���󂯂�
	//-----------------------------------------------------
	m_bDamage = m_HitKurai.is_damage();

	//-----------------------------------------------------
	//�v�l�p�^�[��
	//-----------------------------------------------------
	LogicAI();

	//-----------------------------------------------------
	//���炢����o�^
	//-----------------------------------------------------
	if( g_stDummyEnemyConfig[ m_sEnemyIndex ].m_bHit )
	{
		m_HitAtari.SetHantei( ID_ENEMY_ATK , &m_Pos );
	}

	if( g_stDummyEnemyConfig[ m_sEnemyIndex ].m_bKurai )
	{
		m_HitKurai.SetHantei( ID_ENEMY_DEF , &m_Pos );
	}

	m_Pos.x = m_Obj.pos.x;
	m_Pos.y = m_Obj.pos.y;

}


void CEne0000Dummy::LogicAI()
{
	//-----------------------------------------------------
	//�ړ����W�b�N
	//-----------------------------------------------------
	if( m_sTargetPosX > m_Pos.x ) m_Pos.mx = +100;
	if( m_sTargetPosX < m_Pos.x ) m_Pos.mx = -100;

	m_Obj.mov.x = m_Pos.mx;
	m_Obj.mov.y = m_Pos.my+120;

	if( m_Obj.easy_collision_check() )
	{
		m_Pos.my = 0;
	}
	else
	{
		m_Pos.my += 30;
	}

	m_Obj.pos.x += (m_Obj.mov.x*g_stDummyEnemyConfig[ m_sEnemyIndex ].sSpeed/100);
	m_Obj.pos.y += (m_Obj.mov.y*g_stDummyEnemyConfig[ m_sEnemyIndex ].sSpeed/100);

	m_sAtackTimer ++;

	if( m_sAtackTimer >= 0 )
	{
		Atack();
	}

}


void CEne0000Dummy::Atack()
{

}


void CEne0000Dummy::SeqCrash()
{
	//-----------------------------------------------------
	//�j�󂳂ꂽ
	//-----------------------------------------------------

	if( m_bDisp )
	{
		m_bDisp = gxFalse;
		viiMus::PlaySound( enSoundExplosionSmall );
		ScorePlus( enScore );
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y-30*100 , 5 , 3 );
	}

/*
	if( viiSub::Rand()%80 == 0)
	{
		viiEff::SetBombSimple( m_Pos.x , m_Pos.y-30*100 , 5 , 3 );
	}
*/

	SetActionSeq( enActionSeqEnd );

}

void CEne0000Dummy::Draw()
{
	if( !m_bDisp ) return;

	viiDraw::Sprdmg( m_bDamage , &g_stDummyEnemyConfig[m_sEnemyIndex].stSprdat[0] , m_Pos.x , m_Pos.y , m_Pos.z , ATR_DEFAULT , ARGB_DFLT );
}


