//@
//@
// HOUNDプロトタイプ
//@
//@

#include <gunvalken.h>
#include "enemy.h"
#include "../effect/effectBase.h"

CEnemyBase::CEnemyBase()
{
	
	
}


CEnemyBase::~CEnemyBase()
{
	
	
}


void CEnemyBase::ScorePlus(Sint32 score , Sint32 x,Sint32 y)
{
	
	new CEffScore( m_Pos.x+x , m_Pos.y+y , score );
	viiSub::AddScore(score);
	
}

gxBool CEnemyBase::IsScrollOut( Sint32 range_w , Sint32 range_h )
{
	//-----------------------------------------------------
	//スクロールアウトしているか？
	//-----------------------------------------------------

	return viiSub::IsScrollOut( m_Pos.x , m_Pos.y , range_w , range_h );

}
void CEnemyBase::SetRader(enTypeMarker eType )
{
	if( CRaderControl::GetInstance() )
	{
		CRaderControl::GetInstance()->SetMarker( eType , m_Pos.x , m_Pos.y );
	}

}


