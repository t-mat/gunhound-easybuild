#ifndef _CMECHBONE_H_
#define _CMECHBONE_H_
enum {
	enPartsBase=0,
	enPartsBody,
	enPartsArmL,
	enPartsArmR,
	enPartsLegL,
	enPartsLegR,
	enPartsFootL,
	enPartsFootR,
	enPartsShoulder,
	enPartsWeapon,

	enPartsBurnia,
	enPartsNozzle,
	enPartsBurniaOption,
	enPartsSubWeapon1,	//ボディに装着
	enPartsSubWeapon2,	//バーニアに装着
	enPartsSubWeapon3,	//脚に装着

	enPartsMainShot,	//メインショット位置
	enPartsShot1,		//ショルダーショット位置
	enPartsShot2,		//ボディショット位置
	enPartsShot3,		//レッグショット位置

	enPartsApogyUL,		//アポジモーター左上
	enPartsApogyUR,		//アポジモーター右上
	enPartsApogyDL,		//アポジモーター左下
	enPartsApogyDR,		//アポジモーター右下

	enPartsMax,
	enPartsEnd = -1,
};

enum {
	enParentNone = -1 ,
};

enum
{
	//レイヤー構造
	enPartsLayerMech,
	enPartsLayerBody,
	enPartsLayerPrtc,
	enPartsLayerMax,
};


//-------------------------------
//データ精製用
//-------------------------------
typedef struct StSprConfig {

	StSprConfig()
	{
		for(Sint32 ii=0;ii<enPartsMax;ii++)
		{
			sIndex[ii] = -1;
		}
	}

	Sint32 sIndex[enPartsMax];

}StSprConfig;

typedef struct StSprPos {

	StSprPos()
	{
		for(Sint32 ii=0;ii<enPartsMax;ii++)
		{
			sIndex[ii].x = 0;
			sIndex[ii].y = 0;
			sIndex[ii].z = 0;
		}
	}

	ghPos sIndex[enPartsMax];

} StSprPos;
//-------------------------------


typedef struct StMotion {
	Sint32 sIndex;
	Sint32 sRotation;
	Sint32 sOffsetX;
	Sint32 sOffsetY;
	Sint32 sOption;
} StMotion;

typedef struct StMotionMan {
	Sint32 sFrm;
	StMotion Motion[8];//Base;
/*
	StMotion Body;
	StMotion ArmL;
	StMotion ArmR;
	StMotion FootL;
	StMotion FootR;
	StMotion LegL;
	StMotion LegR;
*/

} StMotionMan;

typedef struct StForm
{
	//フォーム制御用
public:

	StForm()
	{
		m_bEnable = gxFalse;
		m_sSeq = 0;
		m_sBalance = 0;
	}
	gxBool m_bEnable;
	Sint32 m_sSeq;
	Sint32 m_sBalance;

}StForm;

//------------------------------------------------------------------------
//モーフィング用クラス
//------------------------------------------------------------------------
class CMofingMan {

public:
	CMofingMan()
	{
		m_sRemainFrm = 0;
		m_sMaxFrm = 0;
		m_sMotionId  = 0;
		m_bMofing    = gxFalse;
		m_pMotionData = NULL;
	}

	~CMofingMan();

	void SetMofing( StMotionMan *pMotion , StMotionMan *m_pStMotionSrc );
	StMotion* GetMotionData(Sint32 sPos );
	StMotion* GetMofingData(Sint32 sPos );
	gxBool IsMofing();
	void Action();
	void Reset();
private:


	Sint32 m_sRemainFrm,m_sMaxFrm;
	Sint32 m_sMotionId;
	gxBool m_bMofing;
	StMotionMan *m_pMotionData;	//現在再生中のモーションデータ（終端）
	StMotionMan m_StMotionSrc;	//現在再生中のモーションの最初の形
	StMotionMan m_StMotionNow;	//現在のモーション

	void SetNowMotion()
	{

		for(Sint32 ii=0;ii<8;ii++)
		{
			if( m_StMotionNow.Motion[ii].sOption == -1 ) continue;
			if( m_StMotionSrc.Motion[ii].sOption == -1 ) continue;
			m_StMotionNow.Motion[ii].sRotation = m_StMotionSrc.Motion[ii].sRotation + m_sRemainFrm*(GetMotionData( ii )->sRotation - m_StMotionSrc.Motion[ii].sRotation )/m_sMaxFrm;
			m_StMotionNow.Motion[ii].sOffsetX  = m_StMotionSrc.Motion[ii].sOffsetX  + m_sRemainFrm*(GetMotionData( ii )->sOffsetX  - m_StMotionSrc.Motion[ii].sOffsetX )/m_sMaxFrm;
			m_StMotionNow.Motion[ii].sOffsetY  = m_StMotionSrc.Motion[ii].sOffsetY  + m_sRemainFrm*(GetMotionData( ii )->sOffsetY  - m_StMotionSrc.Motion[ii].sOffsetY )/m_sMaxFrm;

		}


	}
};



//------------------------------------------------------------------------
//ボーン制御用クラス
//------------------------------------------------------------------------
class CMechBone
{
public:

	typedef struct StMech
	{
		//-----------------------------------------------------------------
		//パーツポジション
		//-----------------------------------------------------------------
//		gxSprite* pSpr;	//スプライト
		gxSprite* pSprLayer[enPartsLayerMax];	//スプライト
		Sint32 sBoneRot;
		Sint32 sBoneDist;
		Sint32 sActRot;	
		ghPos stPosSrc;		//パーツポジション付け根
		ghPos stPosDst;		//パーツポジション接合先
		ghPos stOffset;		//パーツポジションの表示オフセット
		ghPos stDisp;		//最終的な表示位置
		Sint32 sDamage[enPartsLayerMax];		//ダメージ表示用

		StMech()
		{
//			pSpr  = NULL;
			sBoneRot  = 0;
			sBoneDist = 0;
			sActRot = 0;
			stPosSrc.x = stPosSrc.y = stPosSrc.z = stPosSrc.r = 0;
			stPosDst.x = stPosDst.y = stPosDst.z = stPosDst.r = 0;
			stOffset.x = stOffset.y = stOffset.z = stOffset.r = 0;
			stDisp.x = stDisp.y = stDisp.z = stDisp.r = 0;

			for(Sint32 ii=0;ii<enPartsLayerMax; ii++)
			{
				pSprLayer[ ii ] = NULL;
				sDamage[ii] = 0;
			}

		}

	} StMech;

	CMechBone();
	~CMechBone();

	void Action();
	void Draw();

	//------------------------------------------------------------
	//外部からのパーツ設定（簡単）
	//------------------------------------------------------------
	void SetDefault();
	void SetPosition( StSprPos *pPos );
	void SetSprite( StSprConfig* pMecha , StSprConfig* pBody , StSprConfig* pArmor );

	//------------------------------------------------------------
	//外部からのパーツ設定
	//------------------------------------------------------------
	void SetPartsSpr( Sint32 sIndex , Sint32 sLayer , gxSprite *pSpr);
	void SetPartsPos  ( Sint32 sIndex , Sint32 x , Sint32 y );
	void SetPos( Sint32 x , Sint32 y ,Sint32 z=PRIO_ENEMY )
	{
		m_Pos.x = x;
		m_Pos.y = y;
		m_Pos.z = z;
	}

	void GetMotionDefault( StMotionMan * pMotion );
	void SetAnimation( StMotionMan * pMotionMan );

	void SetPartsBroken( Sint32 sIndex , Sint32 sLayer );
	void SetPartsRelease( Sint32 sIndex , Sint32 sLayer ,Sint32 sRot);
	void SetPartsNone( Sint32 sIndex , Sint32 sLayer );


	void SetShockAbsorber( Sint32 n)
	{
		m_sAbsorberRebound = n;
	}

	Sint32 GetShockAbsorber()
	{
		return m_sAbsorberRebound;
	}

	void SetBodyAbsorber( Sint32 n)
	{
		m_sBodyRebound = n;
	}

	void SetArmRebound( Sint32 n )
	{
		m_sMotionArmRebound = n;
	}

	void SetDash( gxBool bDash )
	{
		m_bDash = bDash;
	}

	void SetPunchMotion( gxBool bPunch ,Sint32 sSeq ,Sint32 sAvg=100)
	{
		m_StFormPunch.m_bEnable = bPunch;
		m_StFormPunch.m_sSeq    = sSeq;
		m_sPunchDstAvg = sAvg;
	}

	void SetJumpForm( gxBool bJump ,Sint32 sSeq )
	{
		m_StFormJump.m_bEnable = bJump;
		m_StFormJump.m_sSeq    = sSeq;
	}

	void SetGakeForm( gxBool bJump ,Sint32 sSeq)
	{
		m_StFormGake.m_bEnable = bJump;
		m_StFormGake.m_sSeq    = sSeq;
	}


	void SetWalk( Sint32 n )
	{
		m_sMotionWalk += n;
		m_sMotionWalk = (360+m_sMotionWalk)%360;
	}
	void ReSetWalk( )
	{
		m_sMotionWalk = 0;
	}

	void SetBalance( Sint32 add )
	{
		//加重計算用（左右バランス値）
		m_sBalance += add;

		if( m_sBalance < -100) m_sBalance = -100;
		if( m_sBalance >  100) m_sBalance =  100;
	}

/*
	void SetPadControl(Sint32 sPush )
	{
		m_sControl = sPush;
	}
*/

	void getPosition(Sint32 sIndex , ghPos *p_ghPos );


	//方向の設定

	void SetDirection(Sint32 sDir)
	{
		m_sDirection = sDir;
	}

	Sint32 GetDirection()
	{
		return m_sDirection;
	}

	void SetArmRotation(Sint32 sRot)
	{
		m_sArmRotation = sRot;
	}

	void SetShoulderRotation(Sint32 sRot)
	{
		m_sShoulderRotation = sRot;
	}

	void SetWeaponRotaion(Sint32 sIndex , Sint32 sRot)
	{
		switch(sIndex){
		case 0:
			m_sWeapon1Rotation = sRot;
			break;
		case 1:
			m_sWeapon2Rotation = sRot;
			break;
		case 2:
			m_sWeapon3Rotation = sRot;
			break;
		default:
			break;
		}
	}
	void SetColor(Sint32 sCol)
	{
		m_sColor = sCol;
	}
	void SetMainWeaponRotation(Sint32 n)
	{
		m_sMainWeaponRotation = n;
	}
	
/*
	void SetLeader(gxBool bFlag)
	{
		m_bLeader = bFlag;
	}

	void SetBooster(gxBool bFlag)
	{
		m_bBooster = bFlag;
	}
*/
	void SetBoosterRotation(Sint32 n)
	{
		m_sBoosterRotation = n;
	}

	void SetDamage( Sint32 sIndex , Sint32 sLayer );
	void ResetAnime();

private:

	//パンチモーションを制御
	void MotionPunch();

	//歩きモーションを制御
	void MotionWalk();

	//ダッシュモーションを制御
	void MotionDash();

	//ジャンプモーションを制御
	void MotionJump();

	//崖モーションを制御
	void MotionGake();

	void BasicAction();
	void ShockAction();
	void MotionMofing();

	//各パーツの計算処理を行う
	void PartsCalc();
	void PartsPosition( Sint32 sParent , Sint32 sIndex);
	void SetMotion( Sint32 sIndex , Sint32 sRotation , Sint32 sOffsetX , Sint32 sOffsetY );

	ghPos  m_Pos;
	gxBool m_bDamage;
//	gxBool m_bBooster;

	gxBool m_bDash;
	Sint32 m_sDirection;
	Sint32 m_sArmRotation;
	Sint32 m_sLegLRotation;
	Sint32 m_sLegRRotation;
	Sint32 m_sMotionWalk;
	Sint32 m_sMotionArmRebound;
	Sint32 m_sBodyRebound;
	Sint32 m_sAbsorberRebound;
//	Sint32 m_sControl;
	Sint32 m_sWeapon1Rotation;
	Sint32 m_sWeapon2Rotation;
	Sint32 m_sWeapon3Rotation;

	Sint32 m_sShoulderRotation;
	Sint32 m_sPunchDstAvg;
	Sint32 m_sPunchSpeed;
	Sint32 m_sPunchRotation;
	Sint32 m_sGunArmRotation;

	StForm m_StFormGake;
	StForm m_StFormJump;
	StForm m_StFormPunch;

//	gxBool m_bJump;
//	Sint32 m_sJumpSeq;
//	Sint32 m_sJumpBalance;

//	gxBool m_bGake;
//	Sint32 m_sGakeSeq;
//	Sint32 m_sGakeBalance;

	//左右のバランス調整用
	Sint32 m_sBalance;

	//モーフィング用
	CMofingMan m_CMofingMan;
	gxBool m_bMofingNow;

	Sint32 m_sColor;

	//ＥＤＩＴ項目
	Sint32 m_sWalkWidth;
	Sint32 m_sWalkHeight;
	Sint32 m_sWalkBodyWidth;
	Sint32 m_sWalkBodyHeight;
	Sint32 m_sAbsorbHeight;
	Sint32 m_sWalkArmWidth;
	Sint32 m_sWalkArmHeight;
	Sint32 m_sMainWeaponRotation;
	Sint32 m_sBoosterRotation;

	StMech m_stMech[enPartsMax];

};


#endif
