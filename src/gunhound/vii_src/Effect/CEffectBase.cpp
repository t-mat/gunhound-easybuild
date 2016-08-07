//@
//@
//@
//@
//@

#include <gunvalken.h>
#include "EffectBase.h"

CEffectBase::CEffectBase()
{
	
	
}


CEffectBase::~CEffectBase()
{
	
	
}

gxBool CEffectBase::IsScrollOut( Sint32 range_w , Sint32 range_h )
{
	//-----------------------------------------------------
	//スクロールアウトしているか？
	//-----------------------------------------------------

	return viiSub::IsScrollOut( m_Pos.x , m_Pos.y , range_w , range_h );

}
