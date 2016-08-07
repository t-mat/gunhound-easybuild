#ifndef _EFFECT_H_
#define _EFFECT_H_

class CEffectBase : public CAction
{
public:
	CEffectBase();
	virtual ~CEffectBase();

	OBJ_POS_T m_Pos;

private:

protected:
	gxBool IsScrollOut( Sint32 range_w , Sint32 range_h );
};


#include "ItemBox.h"
#include "CEffMachinegun.h"
#include "CEffPerticle.h"
#include "CEffEnegun.h"
#include "CEffSandStorm.h"
#include "CEffShipBurnia.h"
#include "CEffEagis.h"
#include "CEffBrokenFloor.h"
#include "CEffScene01.h"
#include "CEffScene02.h"
#include "CEffScene03.h"
#include "CEffScene0A01.h"
#include "CEffScene0A02.h"
#include "CEffAtkLaser.h"
#include "CEffeneBitLaser.h"
#include "CEffBomb.h"
#include "CEffWater.h"
#include "CEffAtkPunch.h"
#include "CEffAtkHomingMissile.h"

#include "CEffZanzo.h"
#include "CEffScn04Tree.h"
#include "CEffHound.h"
#include "CEffFire.h"

#endif
