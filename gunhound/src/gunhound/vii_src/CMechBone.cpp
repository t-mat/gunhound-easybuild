//--------------------------------------------------------------------------------
//
// モーフィング用アニメーション制御
//
//--------------------------------------------------------------------------------

#include <gunvalken.h>
#include "CMechBone.h"

//#define DEBUG_DRAW_KANSETSU_LINE
extern gxSprite stSprSubWeapon[];
extern gxSprite stSprMainWeapon[];
extern gxSprite stSprMech[];
extern gxSprite stSprBody[];
extern gxSprite stSprOption[];
extern gxSprite stSprArmorArm[];
extern gxSprite stSprArmorBody[];
extern gxSprite stSprArmorLeg1[];
extern gxSprite stSprArmorLeg2[];
extern gxSprite stSprArmorShoulder[];
extern gxSprite stSprShot[];
extern gxSprite stSprPunch[];
extern gxSprite stSprApogy[];
extern gxSprite stSprBurn[];
extern gxSprite stSprRedLight[];



CMofingMan::~CMofingMan()
{
	
}


void CMofingMan::SetMofing( StMotionMan *pMotion , StMotionMan *m_pStMotionSrc )
{
	m_pMotionData = pMotion;
	m_StMotionSrc = *m_pStMotionSrc;
	m_StMotionNow = *m_pStMotionSrc;

	m_sMaxFrm = pMotion->sFrm;
	m_sRemainFrm = 0;
	m_bMofing     = gxTrue;

}


StMotion* CMofingMan::GetMotionData(Sint32 sPos )
{
	return &m_pMotionData->Motion[ sPos ];
}

StMotion* CMofingMan::GetMofingData(Sint32 sPos )
{
	return &m_StMotionNow.Motion[ sPos ];
}

gxBool CMofingMan::IsMofing()
{
	return m_bMofing;
}
void CMofingMan::Reset()
{
	m_bMofing = gxFalse;
	m_sRemainFrm = 0;
}


void CMofingMan::Action()
{
	if( !m_bMofing ) return;

	if( m_sRemainFrm < m_sMaxFrm )
	{
		SetNowMotion();

		m_sRemainFrm ++;
	}

	if( m_sRemainFrm >= m_sMaxFrm )
	{
		//モーフィング終了した
		StMotionMan *pOld = m_pMotionData;
		m_pMotionData ++;
		m_sRemainFrm = 0;

		if( m_pMotionData->sFrm == -1 )
		{
			//次のモーションデータがなかった
			m_bMofing = gxFalse;
		}
		else
		{
			SetMofing( m_pMotionData , pOld );
		}
	}
}




//--------------------------------------------------------------------------------
//
// アニメーション用ボーン制御
//
//--------------------------------------------------------------------------------

CMechBone::CMechBone()
{
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_Pos.z = PRIO_ENEMY;

	m_sColor = 0xFFFFFFFF;

	//デフォルト設定
	SetPartsPos( enPartsBase 		,+ 0 , -32 );
	SetPartsPos( enPartsBody 		,+16 , -32 );
	SetPartsPos( enPartsArmL 		,-32 , -32 );
	SetPartsPos( enPartsArmR 		,+32 , -32 );
	SetPartsPos( enPartsFootL 		,  0 ,  +0 );
	SetPartsPos( enPartsFootR 		,  0 ,  +0 );
	SetPartsPos( enPartsLegL 		,-32 , +32 );
	SetPartsPos( enPartsLegR 		,+32 , +32 );
	SetPartsPos( enPartsShoulder 	,-64 , -32 );
	SetPartsPos( enPartsWeapon		,+64 , -32 );

	SetPartsPos( enPartsBurnia 		, -32 , 0 );
	SetPartsPos( enPartsBurniaOption, -64 , 0 );
	SetPartsPos( enPartsNozzle 		, -96 , 0 );
	SetPartsPos( enPartsSubWeapon1 	,  16 , 0 );
	SetPartsPos( enPartsSubWeapon2 	,  16 , 0 );
	SetPartsPos( enPartsSubWeapon3 	,  16 , 0 );

	SetPartsPos( enPartsMainShot 	,  0 , 0 );
	SetPartsPos( enPartsSubWeapon1 	,  0 , 0 );
	SetPartsPos( enPartsSubWeapon2 	,  0 , 0 );
	SetPartsPos( enPartsSubWeapon3 	,  0 , 0 );

	m_bDamage       = gxFalse;
	m_sDirection    = DIR_RIGHT;
	m_sArmRotation  = 0;
	m_sLegLRotation = 0;
	m_sLegRRotation = 0;
	m_sWeapon1Rotation = 0;
	m_sWeapon2Rotation = 0;
	m_sWeapon3Rotation = 0;

	m_sShoulderRotation = 0;
//	m_bBooster = gxFalse;

	m_sBalance = 0;

	m_bDash        = gxFalse;
	m_sMotionWalk  = 0;
	m_sMotionArmRebound = 0;
	m_sBodyRebound      = 0;
	m_sAbsorberRebound  = 0;
	m_bMofingNow        = gxFalse;

	m_sWalkWidth      = 64/8;
	m_sWalkHeight     = 32/4;
	m_sWalkBodyWidth  = 2;
	m_sWalkBodyHeight = 4;
	m_sAbsorbHeight   = 4;
	m_sWalkArmWidth   = 2;
	m_sWalkArmHeight  = 3;

	m_StFormGake.m_bEnable = gxFalse;
	m_StFormGake.m_sSeq     = 0;
	m_StFormGake.m_sBalance = 0;
	m_sMainWeaponRotation   = 0;
	m_sBoosterRotation      = 0;
//	m_bLeader = gxFalse;

	m_sPunchDstAvg   = 100;
	m_sPunchSpeed    = 0;
	m_sPunchRotation = 0;

	m_sGunArmRotation = 0;

//	m_bJump = gxFalse;
//	m_sJumpSeq = 0;
//	m_sJumpBalance = 0;

}


CMechBone::~CMechBone()
{
	
	
}


void CMechBone::Action()
{
	//-----------------------------------------------------------------------------
	//基本アクション制御
	//-----------------------------------------------------------------------------

	//オフセットは必ず毎回初期化

	for ( Sint32 ii=0; ii<enPartsMax; ii++ )
	{
		m_stMech[ ii ].stOffset.x = 0;
		m_stMech[ ii ].stOffset.y = 0;
	}
	m_sLegLRotation = 0;
	m_sLegRRotation = 0;

	MotionMofing();

	BasicAction();

	ShockAction();

	if( m_StFormPunch.m_bEnable )
	{
		MotionPunch();
	}
	else if( m_StFormGake.m_bEnable )
	{
		MotionGake();
	}
	else if( m_StFormJump.m_bEnable )
	{
		MotionJump();
	}
	else
	{
		m_StFormGake.m_sBalance = 0;
		m_StFormJump.m_sBalance = 0;

		if( m_bDash )
		{
			MotionDash();
		}
		else
		{
			if( !m_bMofingNow )
			{
				//注意！キャリアモードの対応の為一時的にテスト
				MotionWalk();
			}
		}
	}

	if( !m_StFormPunch.m_bEnable )
	{
		//パンチしていないときはガンアームローテーションを元に戻す
		Sint32 sAdd = (0-m_sGunArmRotation)/2;
		if( sAdd == 0 && m_sGunArmRotation )
		{
			if( m_sGunArmRotation < 0 ) m_sGunArmRotation = +1;
			if( m_sGunArmRotation > 0 ) m_sGunArmRotation = -1;
		}
		m_sGunArmRotation += sAdd;
	}

	PartsCalc();

	m_bDash    = gxFalse;

	if( m_sBalance > 0 )
	{
		m_sBalance -= 2;
		if( m_sBalance < 0 )
		{
			m_sBalance = 0;
		}
	}
	else if( m_sBalance < 0 )
	{
		m_sBalance += 2;
		if( m_sBalance > 0 )
		{
			m_sBalance = 0;
		}
	}


	//--------------------------------------------
	//ダメージ表現
	//--------------------------------------------

	m_bDamage = gxFalse;

	for ( Sint32 ii=0; ii<enPartsMax; ii++ )
	{
		for(Sint32 jj=0; jj<enPartsLayerMax; jj++ )
		{
			if( m_stMech[ ii ].sDamage[ jj ] > 0 )
			{
				m_stMech[ ii ].sDamage[ jj ] --;
				m_bDamage = gxTrue;
			}
		}
	}

}


void CMechBone::BasicAction()
{
	//-----------------------------------------------------------------------------
	//基本動作制御
	//-----------------------------------------------------------------------------
	Sint32 sArmRot = m_sArmRotation+m_sGunArmRotation;

	if( !m_bMofingNow )
	{
		if( m_sArmRotation <= -90) m_sArmRotation = -90;
		if( m_sArmRotation >=  90) m_sArmRotation =  90;

		m_stMech[ enPartsArmR ].sActRot = sArmRot;

		//-------------------------------
		//腕にあわせた胴体の制御
		//-------------------------------
		m_stMech[ enPartsBody ].sActRot = sArmRot/4;

		//-------------------------------
		//脚の制御
		//-------------------------------
		m_stMech[ enPartsLegL ].sActRot = 0;
		m_stMech[ enPartsLegR ].sActRot = 0;
	}

	//-------------------------------
	//腕にあわせた銃の制御
	//-------------------------------
	m_stMech[ enPartsWeapon ].sActRot = m_stMech[ enPartsArmR ].sActRot;

	m_stMech[ enPartsShoulder ].sActRot = m_sShoulderRotation;

	//-------------------------------
	//胴体にあわせたバーニアの制御
	//-------------------------------
	m_stMech[ enPartsBurnia ].sActRot = m_stMech[ enPartsBody ].sActRot;
	m_stMech[ enPartsNozzle ].sActRot = m_stMech[ enPartsBody ].sActRot;
	m_stMech[ enPartsBurniaOption ].sActRot = m_stMech[ enPartsBody ].sActRot;

	//-------------------------------
	//胴体にあわせたサブウエポンの制御
	//-------------------------------
	m_stMech[ enPartsSubWeapon1 ].sActRot = m_stMech[ enPartsBody ].sActRot+m_sWeapon1Rotation;
	m_stMech[ enPartsSubWeapon2 ].sActRot = m_stMech[ enPartsBody ].sActRot+m_sWeapon2Rotation;
	m_stMech[ enPartsSubWeapon3 ].sActRot = m_stMech[ enPartsLegR ].sActRot+m_sWeapon3Rotation;

	//-------------------------------
	//脚にあわせた太ももの制御
	//-------------------------------
	Sint32 sx,sy,sRot;

	//左もも
	sx = m_stMech[ enPartsBody ].stPosDst.x - m_stMech[ enPartsLegL ].stPosDst.x;
	sy = m_stMech[ enPartsBody ].stPosDst.y - m_stMech[ enPartsLegL ].stPosDst.y;
	sRot = (Sint32)viiMath::Atan2d( (Float32)-sy , (Float32)-sx );
	m_stMech[ enPartsFootL ].sActRot = sRot;

	//右もも
	sx = m_stMech[ enPartsBody ].stPosDst.x - m_stMech[ enPartsLegR ].stPosDst.x;
	sy = m_stMech[ enPartsBody ].stPosDst.y - m_stMech[ enPartsLegR ].stPosDst.y;
	sRot = (Sint32)viiMath::Atan2d( (Float32)-sy , (Float32)-sx );
	m_stMech[ enPartsFootR ].sActRot = sRot;

}




void CMechBone::SetAnimation( StMotionMan * pMotionMan )
{
	//アニメーションのセット

	StMotionMan stKariMotion;

	GetMotionDefault( &stKariMotion );

//	m_CMofingMan.SetMofing( &stMotion[0] , &stKariMotion );
	m_CMofingMan.SetMofing( pMotionMan , &stKariMotion );

	//m_sMotionWalk = 0;
	m_sMotionArmRebound = 0;
	m_sBodyRebound = 0;
}


void CMechBone::MotionMofing()
{
	//-----------------------------------------------------
	//モーフィング制御
	//-----------------------------------------------------

	Sint32 n = 0;
	Sint32 sIndex = 0;

	m_bMofingNow = gxFalse;

	if( m_CMofingMan.IsMofing() )
	{
		m_bMofingNow = gxTrue;
		m_sMotionWalk = 0;

		for(Sint32 jj=0; jj<8; jj++)
		{
			if( m_CMofingMan.GetMofingData(jj)->sOption == -1 )
			{
				continue;
			}

			SetMotion( 
				m_CMofingMan.GetMofingData(jj)->sIndex ,
				m_CMofingMan.GetMofingData(jj)->sRotation ,
				m_CMofingMan.GetMofingData(jj)->sOffsetX ,
				m_CMofingMan.GetMofingData(jj)->sOffsetY );
		}

		m_CMofingMan.Action();

		if( !m_CMofingMan.IsMofing() )
		{
			//アニメーション終了
			m_sArmRotation = m_CMofingMan.GetMofingData(3)->sRotation;	//腕のポジションをアニメにあわせる
		}
	}

}


void CMechBone::GetMotionDefault( StMotionMan * pMotion )
{
	//-------------------------------------------------------------------
	//デフォルト（現状の間接の状態をモーフィングデータにコピーする）
	//-------------------------------------------------------------------
	Sint32 n = 0;
	Sint32 sPos[] =
	{
		enPartsBase,enPartsBody,enPartsArmL,enPartsArmR , enPartsFootL,enPartsFootR,enPartsLegL,enPartsLegR,-1
	};

	pMotion->sFrm = 0;

	pMotion->Motion[0].sIndex = enPartsBase;
	pMotion->Motion[1].sIndex = enPartsBody;
	pMotion->Motion[2].sIndex = enPartsArmL;
	pMotion->Motion[3].sIndex = enPartsArmR;
	pMotion->Motion[4].sIndex = enPartsFootL;
	pMotion->Motion[5].sIndex = enPartsFootR;
	pMotion->Motion[6].sIndex = enPartsLegL;
	pMotion->Motion[7].sIndex = enPartsLegR;

	while( gxTrue )
	{
 		if( sPos[n] == -1 ) break;

 		pMotion->Motion[ n ].sRotation = m_stMech[  sPos[n]  ].sActRot   ;
		pMotion->Motion[ n ].sOffsetX  = m_stMech[  sPos[n]  ].stOffset.x;
		pMotion->Motion[ n ].sOffsetY  = m_stMech[  sPos[n]  ].stOffset.y;
		pMotion->Motion[ n ].sOption = 0;//    = m_stMech[  sPos[n]  ].stOffset.y;

		n ++;
	}

	pMotion->Motion[ 4 ].sRotation = m_sLegLRotation;
	pMotion->Motion[ 5 ].sRotation = m_sLegRRotation;

}


void CMechBone::SetMotion( Sint32 sIndex , Sint32 sRotation , Sint32 sOffsetX , Sint32 sOffsetY )
{
	//--------------------------------------------------------------------
	//関節ごとのモーフィング制御
	//--------------------------------------------------------------------

	//指定場所以外のモーフィングは認めない

	switch( sIndex ) {
	case enPartsBase:
	case enPartsBody:
	case enPartsArmL:
	case enPartsArmR:
	case enPartsFootL:
	case enPartsFootR:
	case enPartsLegL:
	case enPartsLegR:
		break;
	case enPartsEnd:
	default:
		return;
	}

	m_stMech[ sIndex ].sActRot = sRotation;

	if( sIndex == enPartsFootL )
	{
		m_sLegLRotation = sRotation;
	}
	else if( sIndex == enPartsFootR )
	{
		m_sLegRRotation = sRotation;
	}

	m_stMech[ sIndex ].stOffset.x = sOffsetX;
	m_stMech[ sIndex ].stOffset.y = sOffsetY;

}


void CMechBone::ShockAction()
{
	//-----------------------------------------------------------------------------
	//ショック制御
	//-----------------------------------------------------------------------------

	//-------------------------------
	//腕の制御
	//-------------------------------
	if( m_sMotionArmRebound > 0)
	{
		m_sMotionArmRebound --;
		m_stMech[ enPartsArmR ].stOffset.x = (viiMath::Cos100(m_sArmRotation)*m_sMotionArmRebound)/-100;
		m_stMech[ enPartsArmR ].stOffset.y = (viiMath::Sin100(m_sArmRotation)*m_sMotionArmRebound)/-100;
	}

	//-------------------------------
	//ボディの制御
	//-------------------------------
	if( m_sBodyRebound > 0)
	{
		m_sBodyRebound --;
		m_stMech[ enPartsBody ].stOffset.x = (viiMath::Cos100(m_stMech[ enPartsBody ].sActRot)*m_sBodyRebound)/-100;
		m_stMech[ enPartsBody ].stOffset.y = (viiMath::Sin100(m_stMech[ enPartsBody ].sActRot)*m_sBodyRebound)/-100;
	}

	//-------------------------------------
	//アブソーバー制御
	//-------------------------------------
	if( m_sAbsorberRebound > 0)
	{
		m_sAbsorberRebound -= 8;

		if( m_sAbsorberRebound <= 0 )
		{
			m_sAbsorberRebound = 0;
		}

		m_stMech[ enPartsBody ].stOffset.x += 0;
//		m_stMech[ enPartsBody ].stOffset.y += viiMath::Sin100( m_sAbsorberRebound )*m_sAbsorbHeight/100;
		m_stMech[ enPartsBody ].stOffset.y = m_sAbsorbHeight*m_sAbsorberRebound/100;
	}

}


void CMechBone::MotionPunch()
{
	//-----------------------------------------------------------------------------
	//パンチモーション
	//-----------------------------------------------------------------------------
	Sint32 sSeq = m_StFormPunch.m_sSeq;
	Sint32 r=0;
	Sint32 sOffset = 0;

	//---------------------------------------------
	//ガンアームの角度を調整する
	//---------------------------------------------
	Sint32 nKaku = m_sArmRotation + m_sGunArmRotation;
	if( nKaku < 45 ) nKaku += (45-nKaku)/3;
	m_sGunArmRotation = nKaku-m_sArmRotation;
	//---------------------------------------------

	if( sSeq == 0 )
	{
		r = 90;
		m_sPunchSpeed    = 40;
		m_sPunchRotation = 0;
	}
	else if( sSeq < 90 )
	{
		r = 90 - sSeq;
		r = gxAbs(r);
	}
	else if( sSeq < 150 )
	{
		m_sPunchSpeed    = 2;
		m_sPunchRotation = 0;
		sOffset = ((sSeq%8)<4 )? 2 : 0;
		r = 0;
	}
	else if( sSeq < 240 )
	{
		m_sPunchSpeed    = 12;
		m_sPunchRotation = (30-m_sPunchRotation)/3;
		r = 150 - sSeq;
		r = gxAbs(r);
	}
	else
	{
		m_sPunchSpeed    = 0;
		m_sPunchRotation = 30;
		r = 90;
		m_StFormPunch.m_bEnable = gxFalse;
	}
	MotionWalk();
	
	m_stMech[ enPartsArmL ].stOffset.x = (viiMath::Cos100(r)*(28*m_sPunchDstAvg/100))/100+sOffset;
	m_stMech[ enPartsArmL ].stOffset.y = (viiMath::Sin100(r)*8)/100-4;
	m_stMech[ enPartsArmL ].sActRot    = m_sPunchRotation;

	m_stMech[ enPartsBody ].stOffset.x =  (90-r)/10;
	m_stMech[ enPartsArmR ].stOffset.x = -(90-r)/7;

	m_StFormPunch.m_sSeq += m_sPunchSpeed;
}


void CMechBone::MotionWalk()
{
	//-----------------------------------------------------------------------------
	//歩きモーション
	//-----------------------------------------------------------------------------

	//脚の制御

	m_stMech[ enPartsLegR ].stOffset.x = (viiMath::Cos100(m_sMotionWalk)*m_sWalkWidth)/100;
	m_stMech[ enPartsLegR ].stOffset.y = (viiMath::Sin100(m_sMotionWalk)*m_sWalkHeight)/100;

	m_stMech[ enPartsLegL ].stOffset.x = (viiMath::Cos100(m_sMotionWalk+180)*m_sWalkWidth)/100;
	m_stMech[ enPartsLegL ].stOffset.y = (viiMath::Sin100(m_sMotionWalk+180)*m_sWalkHeight)/100;

	if( m_stMech[ enPartsLegL ].stOffset.y > 0 ) m_stMech[ enPartsLegL ].stOffset.y = 0;
	if( m_stMech[ enPartsLegR ].stOffset.y > 0 ) m_stMech[ enPartsLegR ].stOffset.y = 0;

	//ボディの制御

	m_stMech[ enPartsBody ].stOffset.x +=       (viiMath::Cos100(m_sMotionWalk)*m_sWalkBodyWidth)  / 100;
	m_stMech[ enPartsBody ].stOffset.y += gxAbs((viiMath::Sin100(m_sMotionWalk)*m_sWalkBodyHeight))/-100;

	//腕の制御

	m_stMech[ enPartsArmR ].stOffset.x += (viiMath::Cos100(m_sMotionWalk+180)*m_sWalkArmWidth)/100;
	m_stMech[ enPartsArmR ].stOffset.y += gxAbs( (viiMath::Sin100(m_sMotionWalk+180)*m_sWalkArmHeight)/100 );

}


void CMechBone::MotionJump()
{
	//-----------------------------------------------------------------------------
	//ジャンプモーション
	//-----------------------------------------------------------------------------

	//脚の制御

	if( m_StFormJump.m_sSeq == 1 )
	{
		m_sMotionWalk += (116-m_sMotionWalk)/10;
		m_StFormGake.m_sBalance += (45-m_StFormGake.m_sBalance)/10;
		MotionWalk();
		m_stMech[ enPartsLegR ].sActRot = m_StFormGake.m_sBalance;
		m_stMech[ enPartsBody ].sActRot = -5;
	}
	else
	{
		m_sMotionWalk += (300-m_sMotionWalk)/10;
		m_StFormGake.m_sBalance += (-45-m_StFormGake.m_sBalance)/10;
		MotionWalk();
		m_stMech[ enPartsLegR ].sActRot = m_StFormGake.m_sBalance;
		m_stMech[ enPartsLegL ].sActRot = m_StFormGake.m_sBalance;
		m_stMech[ enPartsBody ].sActRot =  5;
	}

}


void CMechBone::MotionGake()
{
	//-----------------------------------------------------------------------------
	//崖モーション
	//-----------------------------------------------------------------------------

	if( m_StFormGake.m_sSeq == 2 )
	{
		m_sMotionWalk += (30-m_sMotionWalk)/10;
		m_StFormGake.m_sBalance += (10-m_StFormGake.m_sBalance)/10;
		MotionWalk();
		m_stMech[ enPartsLegR ].sActRot = m_StFormGake.m_sBalance+18;
		m_stMech[ enPartsLegL ].sActRot = -m_StFormGake.m_sBalance+18;
		m_stMech[ enPartsBody ].sActRot = 5;

		m_stMech[ enPartsLegR ].stOffset.x = 0;
		m_stMech[ enPartsLegL ].stOffset.x = 16;
	}
	else
	{
		m_sMotionWalk += (60-m_sMotionWalk)/10;
		m_StFormGake.m_sBalance += (-35-m_StFormGake.m_sBalance)/10;
		MotionWalk();
		m_stMech[ enPartsLegR ].sActRot = m_StFormGake.m_sBalance;
		m_stMech[ enPartsLegL ].sActRot = -m_StFormGake.m_sBalance;
		m_stMech[ enPartsBody ].sActRot =  5;

		m_stMech[ enPartsLegR ].stOffset.x = 0;
		m_stMech[ enPartsLegL ].stOffset.x = 16;
	}
}


void CMechBone::MotionDash()
{
	//-----------------------------------------------------------------------------
	//ダッシュモーション
	//-----------------------------------------------------------------------------
	if( m_sMotionWalk != 0)
	{
		m_sMotionWalk += 16;
		if( m_sMotionWalk >= 360 )
		{
			m_sMotionWalk = 0;
		}
		MotionWalk();
		return;
	}

	m_sMotionWalk = (360+m_sMotionWalk)%360;

	//脚の制御

	m_stMech[ enPartsLegR ].stOffset.x = (viiMath::Cos100(m_sMotionWalk)*m_sWalkWidth)/100;
	m_stMech[ enPartsLegR ].stOffset.y = (viiMath::Sin100(m_sMotionWalk)*m_sWalkHeight)/100;

	m_stMech[ enPartsLegL ].stOffset.x = (viiMath::Cos100(m_sMotionWalk+180)*m_sWalkWidth)/100;
	m_stMech[ enPartsLegL ].stOffset.y = (viiMath::Sin100(m_sMotionWalk+180)*m_sWalkHeight)/100;

	if( m_stMech[ enPartsLegL ].stOffset.y > 0 ) m_stMech[ enPartsLegL ].stOffset.y = 0;
	if( m_stMech[ enPartsLegR ].stOffset.y > 0 ) m_stMech[ enPartsLegR ].stOffset.y = 0;

	//ボディの制御
	Sint32 sBalance  = m_sBalance;
	Sint32 sBalanceX = sBalance*6/100;
	Sint32 sBalanceY = sBalance*3/100;

	if( m_sDirection == DIR_LEFT )
	{
		sBalanceX = -sBalance*6/100;
		sBalanceY = -sBalance*3/100;
	}

	m_stMech[ enPartsBody ].stOffset.x += sBalanceX;
	m_stMech[ enPartsBody ].stOffset.y += sBalanceY;

	if( m_sAbsorberRebound <= 100 ) SetShockAbsorber(m_sAbsorberRebound+16);
	m_stMech[ enPartsBody ].stOffset.y += 4*m_sAbsorberRebound/100;
	

	m_stMech[ enPartsLegL ].stOffset.x += sBalanceX/3;
	m_stMech[ enPartsLegR ].stOffset.x += sBalanceX/6;

	//腕の制御

	m_stMech[ enPartsArmR ].stOffset.x = (viiMath::Cos100(m_sMotionWalk+180)*m_sWalkArmWidth)/100;
	m_stMech[ enPartsArmR ].stOffset.y = gxAbs( (viiMath::Sin100(m_sMotionWalk+180)*m_sWalkArmHeight)/100 );

}


void CMechBone::PartsCalc()
{
	//-----------------------------------------------------------------------------------
	//各パーツの挙動制御
	//-----------------------------------------------------------------------------------

	Sint32 actPrio[][2]={
		//計算順序
		{ enParentNone	, enPartsBase },
		{ enPartsBase	, enPartsBody },
		{ enPartsBody	, enPartsArmL },
		{ enPartsBody	, enPartsArmR },
		{ enPartsArmR	, enPartsWeapon },
		{ enPartsArmR	, enPartsShoulder},

		{ enPartsBase	, enPartsLegR },
		{ enPartsBase	, enPartsLegL },

		{ enPartsLegL	, enPartsFootL },
		{ enPartsLegR	, enPartsFootR },

		{ enPartsBody	, enPartsBurnia },
		{ enPartsBody	, enPartsBurniaOption },
		{ enPartsBurnia	, enPartsNozzle },
		{ enPartsBody	, enPartsSubWeapon1 },
		{ enPartsBody	, enPartsSubWeapon2 },
		{ enPartsLegR	, enPartsSubWeapon3 },

		{ enPartsWeapon	, enPartsMainShot },
		{ enPartsSubWeapon1	, enPartsShot1 },
		{ enPartsSubWeapon2	, enPartsShot2 },
		{ enPartsSubWeapon3	, enPartsShot3 },

		{ enPartsBody	, enPartsApogyUL },
		{ enPartsBody	, enPartsApogyUR },
		{ enPartsBody	, enPartsApogyDL },
		{ enPartsBody	, enPartsApogyDR },

		{ enParentNone	, enPartsMax },
	};

	Sint32 nItem=0;

	while( gxTrue )
	{
		if ( actPrio[nItem][0] == enParentNone && actPrio[nItem][1] == enPartsMax )
		{
			break;
		}

		PartsPosition( actPrio[nItem][0] , actPrio[nItem][1] );

		nItem ++;
	}

}


void CMechBone::PartsPosition( Sint32 sParent , Sint32 sIndex)
{
	Sint32 px=0,py=0;

	StMech *pParent = NULL , *pMine = &m_stMech[ sIndex ];


	if( sParent == enParentNone )
	{
		//BASEのみの特別処理
		pParent = NULL;
		pMine->stPosSrc.x = 0;
		pMine->stPosSrc.y = 0;

		pMine->stPosSrc.x += pMine->stOffset.x;
		pMine->stPosSrc.y += pMine->stOffset.y;

		pMine->stPosDst.x = pMine->stPosSrc.x+( viiMath::Cos100(pMine->sBoneRot)*pMine->sBoneDist ) / 100;
		pMine->stPosDst.y = pMine->stPosSrc.y+( viiMath::Sin100(pMine->sBoneRot)*pMine->sBoneDist ) / 100;
	}
	else
	{
		//BASE以外の処理
		Sint32 sOffRot = 0;

		pParent = &m_stMech[ sParent ];
		pMine->stPosSrc.x = pParent->stPosDst.x;
		pMine->stPosSrc.y = pParent->stPosDst.y;

		pMine->stPosSrc.x += pMine->stOffset.x;
		pMine->stPosSrc.y += pMine->stOffset.y;

		if( sIndex == enPartsLegL || sIndex == enPartsLegR )
		{
			if( sIndex == enPartsLegL )
			{
				sOffRot = m_sLegLRotation;
			}
			else if( sIndex == enPartsLegR )
			{
				sOffRot = m_sLegRRotation;
			}
		}

		pMine->stPosDst.x = pMine->stPosSrc.x+( viiMath::Cos100(pMine->sBoneRot+pParent->sActRot+sOffRot)*pMine->sBoneDist ) / 100;
		pMine->stPosDst.y = pMine->stPosSrc.y+( viiMath::Sin100(pMine->sBoneRot+pParent->sActRot+sOffRot)*pMine->sBoneDist ) / 100;

	}

}


void CMechBone::Draw()
{
	Sint32 drawPrio[]={
		//描画順序
		enPartsBase,
		enPartsArmL,
		enPartsFootL,
		enPartsLegL,
		enPartsSubWeapon1,
		enPartsBody,
		enPartsBurniaOption,
		enPartsNozzle,
		enPartsBurnia,
		enPartsSubWeapon2,
		enPartsFootR,
		enPartsLegR,
		enPartsSubWeapon3,
		enPartsWeapon,
		enPartsArmR,
		enPartsShoulder,

		enPartsMainShot,
		enPartsShot1,

	enPartsApogyUL,		//アポジモーター左上
	enPartsApogyUR,		//アポジモーター右上
	enPartsApogyDL,		//アポジモーター左下
	enPartsApogyDR,		//アポジモーター右下

		enPartsMax,
	};

	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );

	for ( Sint32 ii=0; ii<enPartsMax; ii++ )
	{
		Sint32 x1,y1,x2,y2;
		Sint32 n = drawPrio[ii];

		if( n == enPartsMax ) break;

		x1 = m_stMech[ n ].stPosSrc.x*m_sDirection + m_Pos.x/100  - sx; 
		y1 = m_stMech[ n ].stPosSrc.y + m_Pos.y/100  - sy; 
		x2 = m_stMech[ n ].stPosDst.x*m_sDirection + m_Pos.x/100  - sx; 
		y2 = m_stMech[ n ].stPosDst.y + m_Pos.y/100  - sy; 

#ifdef DEBUG_DRAW_KANSETSU_LINE
		GGX_DrawLine( x1 , y1 , x2 , y2 , m_Pos.z+8 , ARGB_DFLT,ATR_DEFAULT);
#endif

		for(Sint32 jj=0; jj<enPartsLayerMax; jj++ )
		{
			//レイヤー単位で表示を行う
			Sint32 sColor = m_sColor;
			Sint32 sWeaponRot = 0;
			Sint32 sBurniaRot = 0;
			Sint32 sGunArmRot = 0;
			gxSprite* pSpr;
			gxBool bDamage = gxFalse;

			pSpr = m_stMech[ n ].pSprLayer[jj];

			if( n == enPartsWeapon )
			{
				if( jj == enPartsLayerMech )	sWeaponRot = m_sMainWeaponRotation;
				if( jj == enPartsLayerPrtc )	sWeaponRot = m_sMainWeaponRotation+180;
			}

			if( jj == enPartsLayerMech )
			{
				if( n == enPartsBurnia )	sBurniaRot = -90*m_sBoosterRotation/100;
				if( n == enPartsNozzle )	sBurniaRot = -45+-45*m_sBoosterRotation/100;
			}

			m_stMech[ n ].stDisp.x = (x2+sx)*100;
			m_stMech[ n ].stDisp.y = (y2+sy)*100;

			if( pSpr == NULL) continue;

			//ダメージ表示
			if( m_bDamage )
			{
				bDamage = gxTrue;
			}

			if( m_stMech[ ii ].sDamage[ jj ] > 0 )
			{
				bDamage = gxTrue;
				sColor = 0xFFFF0000;
			}

			if( m_sDirection == DIR_RIGHT )
			{
				viiDraw::Sprdmg(
					bDamage ,
					(gxSprite*)pSpr,
					m_stMech[ n ].stDisp.x,
					m_stMech[ n ].stDisp.y,
					m_Pos.z ,
					ATR_DEFAULT ,
					sColor ,1.0f , m_stMech[ n ].sActRot+sWeaponRot+sBurniaRot+sGunArmRot );

					if( jj == enPartsLayerPrtc )
					{
						Uint32 redARGB = ARGB((abs(-100+(viiSub::GameCounter()*4)%200)*255/100),0x01,0x01,0x01);
						Sint32 lightSpr = 0;
						switch(n){
						case enPartsArmR:		lightSpr = 0;	break;
						case enPartsBody:		lightSpr = 1;	break;
						case enPartsLegR:		lightSpr = 2;	break;
						case enPartsLegL:		lightSpr = 3;	break;
						case enPartsShoulder:	lightSpr = 4;	break;
						default:
							continue;
						}
						viiDraw::Sprdmg(
							bDamage ,
							(gxSprite*)&stSprRedLight[lightSpr],
							m_stMech[ n ].stDisp.x,
							m_stMech[ n ].stDisp.y,
							m_Pos.z ,
							ATR_DEFAULT,
							redARGB ,1.0f , m_stMech[ n ].sActRot+sWeaponRot+sBurniaRot+sGunArmRot );
					}


			}
			else
			{
				viiDraw::Sprdmg(
					bDamage ,
					(gxSprite*)pSpr,
					m_stMech[ n ].stDisp.x,
					m_stMech[ n ].stDisp.y,
					m_Pos.z ,
					ATR_FLIP_X ,
					sColor ,1.0f , m_stMech[ n ].sActRot+sWeaponRot+sBurniaRot );

					if( jj == enPartsLayerPrtc )
					{
						Uint32 redARGB = ARGB((abs(-100+(viiSub::GameCounter()*4)%200)*255/100),0x01,0x01,0x01);
						Sint32 lightSpr = 0;
						switch(n){
						case enPartsArmR:		lightSpr = 0;	break;
						case enPartsBody:		lightSpr = 1;	break;
						case enPartsLegR:		lightSpr = 2;	break;
						case enPartsLegL:		lightSpr = 3;	break;
						case enPartsShoulder:	lightSpr = 4;	break;
						default:
							continue;
						}
						viiDraw::Sprdmg(
							bDamage ,
							(gxSprite*)&stSprRedLight[lightSpr],
							m_stMech[ n ].stDisp.x,
							m_stMech[ n ].stDisp.y,
							m_Pos.z ,
							ATR_FLIP_X ,
							redARGB ,1.0f , m_stMech[ n ].sActRot+sWeaponRot+sBurniaRot );
					}
			}
		}

	}

}

void CMechBone::SetDamage( Sint32 sIndex , Sint32 sLayer )
{
	//-------------------------------------------------------
	//パーツにダメージを受けた
	//-------------------------------------------------------

	m_stMech[ sIndex ].sDamage[sLayer] = 3;

}


void CMechBone::SetPartsBroken( Sint32 sIndex , Sint32 sLayer )
{
	//-------------------------------------------------------
	//パーツが破壊された
	//-------------------------------------------------------
	ghPos stPos;

	getPosition( sIndex , &stPos );

	viiEff::Set( EFF_DAMAGE_PARTS,stPos.x , stPos.y ,
			argset(
				0,	//(Sint32)m_stMech[ sIndex ].pSprLayer[ sLayer ],	//ポインタが64bitになって辻褄が合わなくなった
				(m_sDirection == DIR_LEFT)? ATR_FLIP_X : ATR_DFLT,
				ARGB_DFLT,
				m_stMech[ sIndex ].sActRot,
				100,
				0 ),
				m_stMech[ sIndex ].pSprLayer[ sLayer ]	);

	SetPartsNone( sIndex , sLayer );

}

void CMechBone::SetPartsRelease( Sint32 sIndex , Sint32 sLayer ,Sint32 sRot )
{
	//-------------------------------------------------------
	//パーツが破壊された
	//-------------------------------------------------------
	ghPos stPos;

	getPosition( sIndex , &stPos );

	viiEff::Set( EFF_RELEASE_PARTS , stPos.x , stPos.y ,
			argset(
				0,//(Sint32)m_stMech[ sIndex ].pSprLayer[ sLayer ],//ポインタが64bitになって辻褄が合わなくなった
				(m_sDirection == DIR_LEFT)? ATR_FLIP_X : ATR_DFLT,
				ARGB_DFLT,
				sRot,//m_stMech[ sIndex ].sActRot,
				800 ),
				m_stMech[ sIndex ].pSprLayer[ sLayer ]
				);

	SetPartsNone( sIndex , sLayer );
}

void CMechBone::SetPartsNone( Sint32 sIndex , Sint32 sLayer )
{
	//-------------------------------------------------------
	//パーツを外す
	//-------------------------------------------------------

	m_stMech[ sIndex ].pSprLayer[ sLayer ] = NULL;
}


void CMechBone::SetPartsSpr( Sint32 sIndex , Sint32 sLayer , gxSprite *pSpr)
{
	//-------------------------------------------------------
	//スプライトの設定
	//-------------------------------------------------------
	ASSERT( sLayer >= 0 && sLayer<enPartsLayerMax );
	m_stMech[ sIndex ].pSprLayer[ sLayer ] = pSpr;
}


void CMechBone::SetPartsPos( Sint32 sIndex , Sint32 x , Sint32 y )
{
	//-------------------------------------------------------
	//場所の設定
	//-------------------------------------------------------
	Sint32 sRot = (Sint32)viiMath::Atan2d( (Float32)y , (Float32)x );

	m_stMech[ sIndex ].sBoneRot  = sRot;
	m_stMech[ sIndex ].sBoneDist = viiMath::Dist( 0, 0, x, y )/1;

}


void CMechBone::getPosition(Sint32 sIndex , ghPos *p_ghPos )
{
	//-------------------------------------------------------
	//最終的な表示位置を返す（１フレーム前の情報）
	//-------------------------------------------------------
	p_ghPos->x = m_stMech[ sIndex ].stDisp.x;
	p_ghPos->y = m_stMech[ sIndex ].stDisp.y;

}



void CMechBone::SetDefault()
{
	//-----------------------------------------
	//デフォルトのポジション
	//-----------------------------------------
	StSprPos g_StSprPos;
	StSprConfig m_Mecha,m_Body,m_Armor;

	//-----------------------------

	g_StSprPos.sIndex[enPartsBase].x =  0;
	g_StSprPos.sIndex[enPartsBase].y =  -23;

	g_StSprPos.sIndex[enPartsBody].x =  0;
	g_StSprPos.sIndex[enPartsBody].y =  -4;

	g_StSprPos.sIndex[enPartsArmL].x =  -8;
	g_StSprPos.sIndex[enPartsArmL].y =  -6;

	g_StSprPos.sIndex[enPartsArmR].x =  0;
	g_StSprPos.sIndex[enPartsArmR].y =  -12;

	g_StSprPos.sIndex[enPartsLegL].x =  0;
	g_StSprPos.sIndex[enPartsLegL].y =  5;

	g_StSprPos.sIndex[enPartsLegR].x =  0;
	g_StSprPos.sIndex[enPartsLegR].y =  5;

	g_StSprPos.sIndex[enPartsShoulder].x = -6;
	g_StSprPos.sIndex[enPartsShoulder].y = -4;

	g_StSprPos.sIndex[enPartsWeapon].x = 8;
	g_StSprPos.sIndex[enPartsWeapon].y = 8;

	g_StSprPos.sIndex[enPartsMainShot].x = 36;
	g_StSprPos.sIndex[enPartsMainShot].y = -6;

	g_StSprPos.sIndex[enPartsShot1].x =   -8;
	g_StSprPos.sIndex[enPartsShot1].y =   -16;

	//---------------------------
	//背中セット
	g_StSprPos.sIndex[enPartsBurniaOption].x = -4;
	g_StSprPos.sIndex[enPartsBurniaOption].y = -16;

	g_StSprPos.sIndex[enPartsBurnia].x = 	-10;
	g_StSprPos.sIndex[enPartsBurnia].y = 	-18;

	g_StSprPos.sIndex[enPartsNozzle].x = 	-6;
	g_StSprPos.sIndex[enPartsNozzle].y = 	2;

	//---------------------------

	g_StSprPos.sIndex[enPartsSubWeapon1].x = -4;
	g_StSprPos.sIndex[enPartsSubWeapon1].y = -18;

	g_StSprPos.sIndex[enPartsSubWeapon2].x = 16;
	g_StSprPos.sIndex[enPartsSubWeapon2].y = 0;

	g_StSprPos.sIndex[enPartsSubWeapon3].x = 16;
	g_StSprPos.sIndex[enPartsSubWeapon3].y = 0;

	g_StSprPos.sIndex[enPartsApogyUL].x =   -8;
	g_StSprPos.sIndex[enPartsApogyUL].y =   -8;

	g_StSprPos.sIndex[enPartsApogyUR].x =   8;
	g_StSprPos.sIndex[enPartsApogyUR].y =   -8;

	g_StSprPos.sIndex[enPartsApogyDL].x =   -8;
	g_StSprPos.sIndex[enPartsApogyDL].y =   8;

	g_StSprPos.sIndex[enPartsApogyDR].x =   8;
	g_StSprPos.sIndex[enPartsApogyDR].y =   8;

	SetPosition( &g_StSprPos );

	//-----------------------------

	//メカ設定
	m_Mecha.sIndex[enPartsBase]     = -1;
	m_Mecha.sIndex[enPartsBody]     =  1;
	m_Mecha.sIndex[enPartsArmL]     =  0;
	m_Mecha.sIndex[enPartsArmR]     =  0;
	m_Mecha.sIndex[enPartsLegL]     =  3;
	m_Mecha.sIndex[enPartsLegR]     =  2;
	m_Mecha.sIndex[enPartsFootL]    =  7;
	m_Mecha.sIndex[enPartsFootR]    =  6;
	m_Mecha.sIndex[enPartsShoulder] =  4;
	m_Mecha.sIndex[enPartsWeapon]   =  0;
	m_Mecha.sIndex[enPartsMainShot] = -1;
	m_Mecha.sIndex[enPartsShot1]    = -1;
	m_Mecha.sIndex[enPartsNozzle]   = 0;
	m_Mecha.sIndex[ enPartsSubWeapon1  ] = 0;

	m_Mecha.sIndex[enPartsBurniaOption]   =  -1;
	m_Mecha.sIndex[enPartsNozzle]         =  -1;
	m_Mecha.sIndex[enPartsBurnia]         =  -1;

	//ボディ設定
	m_Body.sIndex[enPartsBase]     = -1;
	m_Body.sIndex[enPartsBody]     =  1;
	m_Body.sIndex[enPartsArmL]     = -1;
	m_Body.sIndex[enPartsArmR]     =  0;
	m_Body.sIndex[enPartsLegL]     =  3;
	m_Body.sIndex[enPartsLegR]     =  2;
	m_Body.sIndex[enPartsFootL]    = -1;
	m_Body.sIndex[enPartsFootR]    = -1;
	m_Body.sIndex[enPartsShoulder] =  4;
	m_Body.sIndex[enPartsWeapon]   =  1;
	m_Body.sIndex[enPartsNozzle]   =  -1;


	//アーマー設定
	m_Armor.sIndex[ enPartsBase ]     = -1;
	m_Armor.sIndex[ enPartsBody ]     =  0;
	m_Armor.sIndex[ enPartsArmL ]     = -1;
	m_Armor.sIndex[ enPartsArmR ]     =  0;
	m_Armor.sIndex[ enPartsLegL ]     =  0;
	m_Armor.sIndex[ enPartsLegR ]     =  0;
	m_Armor.sIndex[ enPartsFootL ]    = -1;
	m_Armor.sIndex[ enPartsFootR ]    = -1;
	m_Armor.sIndex[ enPartsShoulder ] =  0;
	m_Armor.sIndex[ enPartsNozzle ]     = -1;

	//オプションパーツ
	m_Armor.sIndex[ enPartsBurnia      ] = -1;
	m_Armor.sIndex[ enPartsBurniaOption] = -1;
	m_Armor.sIndex[ enPartsSubWeapon1  ] = -1;
	m_Armor.sIndex[ enPartsSubWeapon2  ] = -1;
	m_Armor.sIndex[ enPartsSubWeapon3  ] = -1;

	SetSprite( &m_Mecha , &m_Body , &m_Armor );
}


void CMechBone::SetPosition( StSprPos *pPos )
{
	//------------------------------------------
	//位置設定
	//------------------------------------------

	SetPartsPos( enPartsBase			, 	pPos->sIndex[enPartsBase].x			,pPos->sIndex[enPartsBase].y	 );
	SetPartsPos( enPartsBody			, 	pPos->sIndex[enPartsBody].x			,pPos->sIndex[enPartsBody].y	 );
	SetPartsPos( enPartsArmL			, 	pPos->sIndex[enPartsArmL].x			,pPos->sIndex[enPartsArmL].y	 );
	SetPartsPos( enPartsArmR			, 	pPos->sIndex[enPartsArmR].x			,pPos->sIndex[enPartsArmR].y	 );
	SetPartsPos( enPartsLegL			, 	pPos->sIndex[enPartsLegL].x			,pPos->sIndex[enPartsLegL].y	 );
	SetPartsPos( enPartsLegR			, 	pPos->sIndex[enPartsLegR].x			,pPos->sIndex[enPartsLegR].y	 );
	SetPartsPos( enPartsShoulder		, 	pPos->sIndex[enPartsShoulder].x		,pPos->sIndex[enPartsShoulder].y );
	SetPartsPos( enPartsWeapon			, 	pPos->sIndex[enPartsWeapon].x		,pPos->sIndex[enPartsWeapon].y	 );

//	SetPartsPos( enPartsBurniaOption	, 	pPos->sIndex[enPartsBurniaOption].x	,pPos->sIndex[enPartsBurniaOption].y );
//	SetPartsPos( enPartsNozzle			, 	pPos->sIndex[enPartsNozzle].x		,pPos->sIndex[enPartsNozzle].y	 );
//	SetPartsPos( enPartsBurnia			, 	pPos->sIndex[enPartsBurnia].x		,pPos->sIndex[enPartsBurnia].y	 );

	SetPartsPos( enPartsMainShot		, 	pPos->sIndex[enPartsMainShot].x		,pPos->sIndex[enPartsMainShot].y );
	SetPartsPos( enPartsShot1   		, 	pPos->sIndex[enPartsShot1].x		,pPos->sIndex[enPartsShot1].y	 );

	SetPartsPos( enPartsBurnia 			, 	pPos->sIndex[enPartsBurnia].x		,pPos->sIndex[enPartsBurnia].y	 );
	SetPartsPos( enPartsBurniaOption	, 	pPos->sIndex[enPartsBurniaOption].x	,pPos->sIndex[enPartsBurniaOption].y );
	SetPartsPos( enPartsNozzle 			, 	pPos->sIndex[enPartsNozzle].x		,pPos->sIndex[enPartsNozzle].y	 );
	SetPartsPos( enPartsSubWeapon1 		, 	pPos->sIndex[enPartsSubWeapon1].x	,pPos->sIndex[enPartsSubWeapon1].y );
	SetPartsPos( enPartsSubWeapon2 		, 	pPos->sIndex[enPartsSubWeapon2].x	,pPos->sIndex[enPartsSubWeapon2].y );
	SetPartsPos( enPartsSubWeapon3 		, 	pPos->sIndex[enPartsSubWeapon3].x	,pPos->sIndex[enPartsSubWeapon3].y );

	SetPartsPos( enPartsApogyUL   		, 	pPos->sIndex[enPartsApogyUL].x		,pPos->sIndex[enPartsApogyUL].y );
	SetPartsPos( enPartsApogyUR   		, 	pPos->sIndex[enPartsApogyUR].x		,pPos->sIndex[enPartsApogyUR].y );
	SetPartsPos( enPartsApogyDL   		, 	pPos->sIndex[enPartsApogyDL].x		,pPos->sIndex[enPartsApogyDL].y );
	SetPartsPos( enPartsApogyDR   		, 	pPos->sIndex[enPartsApogyDR].x		,pPos->sIndex[enPartsApogyDR].y );

}


void CMechBone::SetSprite( StSprConfig* pMecha , StSprConfig* pBody , StSprConfig* pArmor )
{
	//----------------------------------------------------------------------
	//スプライト設定
	//----------------------------------------------------------------------

	if( pMecha )
	{
		//スプライト1設定(メカ)

		if( pMecha->sIndex[enPartsBase]     != -1)  SetPartsSpr( enPartsBase, 		enPartsLayerMech,NULL );	//pMecha->sIndex[enPartsBase]
		if( pMecha->sIndex[enPartsBody]     != -1)  SetPartsSpr( enPartsBody, 		enPartsLayerMech,&stSprMech[ pMecha->sIndex[enPartsBody] ] );
		if( pMecha->sIndex[enPartsArmL]     != -1)  SetPartsSpr( enPartsArmL, 		enPartsLayerMech,&stSprPunch[ pMecha->sIndex[enPartsArmL]] );
		if( pMecha->sIndex[enPartsArmR]     != -1)  SetPartsSpr( enPartsArmR, 		enPartsLayerMech,&stSprMech[ pMecha->sIndex[enPartsArmR] ] );
		if( pMecha->sIndex[enPartsLegL]     != -1)  SetPartsSpr( enPartsLegL, 		enPartsLayerMech,&stSprMech[ pMecha->sIndex[enPartsLegL] ] );
		if( pMecha->sIndex[enPartsLegR]     != -1)  SetPartsSpr( enPartsLegR, 		enPartsLayerMech,&stSprMech[ pMecha->sIndex[enPartsLegR] ] );
		if( pMecha->sIndex[enPartsFootL]    != -1)  SetPartsSpr( enPartsFootL, 		enPartsLayerMech,&stSprMech[ pMecha->sIndex[enPartsFootL]] );
		if( pMecha->sIndex[enPartsFootR]    != -1)  SetPartsSpr( enPartsFootR, 		enPartsLayerMech,&stSprMech[ pMecha->sIndex[enPartsFootR]] );
		if( pMecha->sIndex[enPartsShoulder] != -1)  SetPartsSpr( enPartsShoulder,	enPartsLayerMech,&stSprMech[ pMecha->sIndex[enPartsShoulder]] );
		if( pMecha->sIndex[enPartsWeapon]   != -1)  SetPartsSpr( enPartsWeapon,		enPartsLayerMech,&stSprMainWeapon[ pMecha->sIndex[enPartsWeapon] ] );
		if( pMecha->sIndex[enPartsMainShot] != -1)  SetPartsSpr( enPartsMainShot,	enPartsLayerMech , NULL );	//pMecha->sIndex[enPartsMainShot]		
		if( pMecha->sIndex[enPartsShot1]    != -1)  SetPartsSpr( enPartsShot1,    	enPartsLayerMech , NULL);	//pMecha->sIndex[enPartsShot1]		
		if( pMecha->sIndex[enPartsSubWeapon1] != -1 )   SetPartsSpr( enPartsSubWeapon1,		enPartsLayerMech,&stSprSubWeapon[ pMecha->sIndex[enPartsSubWeapon1] ] );

		if( pMecha->sIndex[enPartsNozzle]   != -1 ) 	 SetPartsSpr( enPartsNozzle       ,	enPartsLayerMech , &stSprBurn[ pMecha->sIndex[enPartsNozzle]] );
		if( pMecha->sIndex[enPartsBurniaOption] != -1 )  SetPartsSpr( enPartsBurniaOption ,	enPartsLayerMech , &stSprBurn[ pMecha->sIndex[enPartsBurniaOption] ] );
		if( pMecha->sIndex[enPartsBurnia] != -1 ) 		 SetPartsSpr( enPartsBurnia       , enPartsLayerMech , &stSprBurn[ pMecha->sIndex[enPartsBurnia] ] );
	}

	if( pBody )
	{
		if( pBody->sIndex[enPartsBase]     != -1)  SetPartsSpr( enPartsBase, 		enPartsLayerBody ,NULL );	//pMecha->sIndex[enPartsBase]
		if( pBody->sIndex[enPartsBody]     != -1)  SetPartsSpr( enPartsBody, 		enPartsLayerBody ,&stSprBody [ pBody->sIndex[enPartsBody] ] );
		if( pBody->sIndex[enPartsArmL]     != -1)  SetPartsSpr( enPartsArmL, 		enPartsLayerBody ,NULL );
		if( pBody->sIndex[enPartsArmR]     != -1)  SetPartsSpr( enPartsArmR, 		enPartsLayerBody ,&stSprBody [ pBody->sIndex[enPartsArmR] ] );
		if( pBody->sIndex[enPartsLegL]     != -1)  SetPartsSpr( enPartsLegL, 		enPartsLayerBody ,&stSprBody [ pBody->sIndex[enPartsLegL] ] );
		if( pBody->sIndex[enPartsLegR]     != -1)  SetPartsSpr( enPartsLegR, 		enPartsLayerBody ,&stSprBody [ pBody->sIndex[enPartsLegR] ] );
		if( pBody->sIndex[enPartsFootL]    != -1)  SetPartsSpr( enPartsFootL, 		enPartsLayerBody ,NULL );
		if( pBody->sIndex[enPartsFootR]    != -1)  SetPartsSpr( enPartsFootR, 		enPartsLayerBody ,NULL );
		if( pBody->sIndex[enPartsShoulder] != -1)  SetPartsSpr( enPartsShoulder,	enPartsLayerBody ,&stSprBody [ pBody->sIndex[enPartsShoulder]] );
//		if( pBody->sIndex[enPartsWeapon]   != -1)  SetPartsSpr( enPartsWeapon,		enPartsLayerBody ,&stSprMainWeapon[ pBody->sIndex[enPartsWeapon] ] );
		if( pBody->sIndex[enPartsWeapon]   != -1)  SetPartsSpr( enPartsWeapon,		enPartsLayerBody ,NULL );
		if( pBody->sIndex[enPartsNozzle]   != -1 ) SetPartsSpr( enPartsNozzle,		enPartsLayerBody ,NULL );//&stSprBurn[ pBody->sIndex[enPartsNozzle]] );
	}

	if( pArmor )
	{
		//スプライト3設定(アーマー)

		if( pArmor->sIndex[enPartsBase] != -1 )     SetPartsSpr( enPartsBase, 		enPartsLayerPrtc,NULL );
		if( pArmor->sIndex[enPartsBody] != -1 )     SetPartsSpr( enPartsBody, 		enPartsLayerPrtc,&stSprArmorBody[ pArmor->sIndex[enPartsBody] ] );
		if( pArmor->sIndex[enPartsArmL] != -1 )     SetPartsSpr( enPartsArmL, 		enPartsLayerPrtc,NULL );
		if( pArmor->sIndex[enPartsArmR] != -1 )     SetPartsSpr( enPartsArmR, 		enPartsLayerPrtc,&stSprArmorArm [ pArmor->sIndex[enPartsArmR] ] );
		if( pArmor->sIndex[enPartsLegL] != -1 )     SetPartsSpr( enPartsLegL, 		enPartsLayerPrtc,&stSprArmorLeg2[ pArmor->sIndex[enPartsLegL] ] );
		if( pArmor->sIndex[enPartsLegR] != -1 )     SetPartsSpr( enPartsLegR, 		enPartsLayerPrtc,&stSprArmorLeg1[ pArmor->sIndex[enPartsLegR] ] );
		if( pArmor->sIndex[enPartsFootL] != -1 )    SetPartsSpr( enPartsFootL, 		enPartsLayerPrtc,NULL);
		if( pArmor->sIndex[enPartsFootR] != -1 )    SetPartsSpr( enPartsFootR, 		enPartsLayerPrtc,NULL);
		if( pArmor->sIndex[enPartsShoulder] != -1 ) SetPartsSpr( enPartsShoulder,	enPartsLayerPrtc,&stSprArmorShoulder[ pArmor->sIndex[enPartsShoulder]]  );
		if( pArmor->sIndex[enPartsWeapon]   != -1)  SetPartsSpr( enPartsWeapon,		enPartsLayerPrtc,&stSprMainWeapon[ pArmor->sIndex[enPartsWeapon] ] );

		//オプションパーツ
		if( pArmor->sIndex[enPartsBurnia] != -1 )       SetPartsSpr( enPartsBurnia,			enPartsLayerPrtc,NULL );
		if( pArmor->sIndex[enPartsNozzle] != -1 )       SetPartsSpr( enPartsNozzle,			enPartsLayerPrtc,NULL );//&stSprBurn[ pArmor->sIndex[enPartsNozzle]] );
		if( pArmor->sIndex[enPartsBurniaOption] != -1 ) SetPartsSpr( enPartsBurniaOption,	enPartsLayerPrtc,NULL );
		if( pArmor->sIndex[enPartsSubWeapon1] != -1 )   SetPartsSpr( enPartsSubWeapon1,		enPartsLayerPrtc,NULL );//&stSprSubWeapon[ pArmor->sIndex[enPartsSubWeapon1] ] );
		if( pArmor->sIndex[enPartsSubWeapon2] != -1 )   SetPartsSpr( enPartsSubWeapon2,		enPartsLayerPrtc,NULL );
		if( pArmor->sIndex[enPartsSubWeapon3] != -1 )   SetPartsSpr( enPartsSubWeapon3,		enPartsLayerPrtc,NULL );
	}

}


void CMechBone::ResetAnime()
{
	m_CMofingMan.Reset();
	m_bMofingNow = gxFalse;


}

