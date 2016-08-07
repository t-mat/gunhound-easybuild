#ifndef _ENEMY_H_
#define _ENEMY_H_

class CEnemyBase : public CAction
{
public:
	CEnemyBase();
	virtual ~CEnemyBase();

	OBJ_POS_T m_Pos;

private:

protected:
	gxBool IsScrollOut( Sint32 range_w , Sint32 range_h );
	void SetRader( enTypeMarker eType );
	void ScorePlus(Sint32 score , Sint32 x=0,Sint32 y=0);
};


class CHoundEnemyBase
{
public:
	enum {
		enForceOpNone=0,
		enForceOpGo,
		enForceOpStop,
		enForceOpMovePos,
		enForceOpMoveAdd,
	};


	CHoundEnemyBase();
	virtual ~CHoundEnemyBase();

	void ActionCommon();

	//戦闘コマンド発令用
	virtual void SetOperation( Sint32 n , Sint32 opt );

	void   Clear()
	{
		//if( m_bDead ) 
		m_bClear = gxTrue;
	}

	gxBool IsClear()
	{
		return m_bClear;
	}

	gxBool IsDead()
	{
		return m_bDead;
	}

private:

protected:

	Sint32 m_sDirection;
	Sint32 m_sAtributeDir;

	void DirectionAdjust();

	gxBool m_bDead;
	gxBool m_bClear;

	//オペレーションコード
	Sint32 m_sOpeCode;
	Sint32 m_sOpeData;
};

//---------------------------------------------
//ハウンドファイル
//---------------------------------------------
#include "CGunHound.h"
#include "CEne0000Dummy.h"
#include "CEne0001EneBullet.h"
#include "CSoldierBase.h"
#include "HoundsEnemy/CEneH0000PunchBall.h"
#include "HoundsEnemy/CEneCommon.h"
#include "HoundsEnemy/CEneH0413Fannel.h"
#include "HoundsEnemy/CEneH0002Commanch.h"
#include "HoundsEnemy/CEneH0003Vorzoy.h"
#include "HoundsEnemy/CEneH0004Tachikoma.h"
#include "HoundsEnemy/CEneH0005TransHeli.h"
#include "HoundsEnemy/CEneH0006UGTortika.h"
#include "HoundsEnemy/CEneH0007YusouHeli.h"
#include "HoundsEnemy/CEneH0008BloodSocker.h"
#include "HoundsEnemy/CEneH0101HeavyTank.h"
#include "HoundsEnemy/CEneH0102NightSpider.h"
#include "HoundsEnemy/CEneH0103ClubGunner.h"
#include "HoundsEnemy/CEneH0104LightTank.h"
#include "HoundsEnemy/CEneH0105TankGuard.h"
#include "HoundsEnemy/CEneH0106Yagura.h"
#include "HoundsEnemy/CEneH0107Walker.h"
#include "HoundsEnemy/CEneH0108Hovered.h"
#include "HoundsEnemy/CEneH0201VorzoyFly.h"
#include "HoundsEnemy/CEneH0202VorzoyWP.h"
#include "HoundsEnemy/CEneH0203Serpent.h"
#include "HoundsEnemy/CEneH0204Grazzi.h"
#include "HoundsEnemy/CEneH0205Danmaku.h"
#include "HoundsEnemy/CEneH0206Bee.h"
#include "HoundsEnemy/CEneH0207TransShips.h"
#include "HoundsEnemy/CEneH0301Gunpelly.h"
#include "HoundsEnemy/CEneH0008GunTank.h"
#include "HoundsEnemy/CEneH0303GerWalk.h"
#include "HoundsEnemy/CEneH0304Bee.h"
#include "HoundsEnemy/CEneH0305Missile.h"
#include "HoundsEnemy/CEneH0401MovingCannon.h"
#include "HoundsEnemy/CEneH0403Tarantura.h"
#include "HoundsEnemy/CEneH0404GravityStar.h"
#include "HoundsEnemy/CEneH0405Nancy.h"
#include "HoundsEnemy/CEneH0406Carrymissile.h"
#include "HoundsEnemy/CEneH0407Nautilus.h"
#include "HoundsEnemy/CEneH0408Powerdsuit.h"
#include "HoundsEnemy/CEneH0409SlideCannon.h"
#include "HoundsEnemy/CEneH0410Wall.h"
#include "HoundsEnemy/CEneH0411Stunbarrier.h"
#include "HoundsEnemy/CEneH0501PlasmaMachine.h"
#include "HoundsEnemy/CEneH0412Stunbarrier.h"
#include "HoundsEnemy/CEneH0503HoundFly.h"
#include "HoundsEnemy/CEneH0203Snake.h"
#include "HoundsEnemy/CEneH0504Boss.h"

#include "HoundsEnemy/CAtkH0001Missile.h"
#include "HoundsEnemy/CAtkH0002GMissile.h"
#include "HoundsEnemy/CAtkH0003Effect.h"

#include "HoundsEnemy/Extra/CEneEx0001Cannon.h"
#include "HoundsEnemy/Extra/CEneEx0002Laser.h"
#include "HoundsEnemy/Extra/CEneEx0003Shot.h"
#include "HoundsEnemy/Extra/CEneEx0004Missile.h"
#include "HoundsEnemy/Extra/CEneEx0005Wii.h"
#include "HoundsEnemy/Extra/CEneEx0006Tank.h"
#include "HoundsEnemy/Extra/CEneEx0007Nozzle.h"
#include "HoundsEnemy/Extra/CEneEx0008Bomb.h"
#include "HoundsEnemy/Extra/CEneEx0009Body.h"
#include "HoundsEnemy/Extra/CEneEx0010Suihei.h"


#endif

