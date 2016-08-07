

#include "HoundsEnemy.h"


CHoundEnemyBase::CHoundEnemyBase()
{
	m_sDirection  = DIR_RIGHT;
	m_sAtributeDir = 0;

	m_bDead  = gxFalse;
	m_bClear = gxFalse;

	m_sOpeCode = 0;
	m_sOpeData = 0;

}


CHoundEnemyBase::~CHoundEnemyBase()
{
	
	
	
}


void CHoundEnemyBase::ActionCommon()
{
	
	
	
}


void CHoundEnemyBase::DirectionAdjust()
{

	if( m_sDirection == DIR_LEFT  ) m_sAtributeDir = ATR_FLIP_X;
	if( m_sDirection == DIR_RIGHT ) m_sAtributeDir = ATR_DFLT;

}


void CHoundEnemyBase::SetOperation( Sint32 n , Sint32 opt )
{
	
	
	
}

