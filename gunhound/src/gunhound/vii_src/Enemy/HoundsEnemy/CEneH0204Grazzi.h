//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneH0204Grazzi : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0204Grazzi( Sint32 x, Sint32 y );
	~CEneH0204Grazzi();

	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetMoveArea(Sint32 x1,Sint32 x2)
	{
		m_bMoveLimit = gxTrue;
		m_sMoveX1 = x1;
		m_sMoveX2 = x2;
	}

private:

	typedef struct StAutoArm
	{
		Float32 fRot;
		Float32 fAdd;
		Sint32 sFrm;
		gxBool bAuto;

		StAutoArm()
		{
			fRot = ZERO;
			fAdd = ZERO;
			sFrm = 0;
			bAuto = gxFalse;
		}
		Sint32 GetRot()
		{
			return (Sint32)fRot;
		}

		gxBool IsRotEnd()
		{
			return !bAuto;
		}
	}StAutoArm;


	void Init();
	void Atack();
	void LogicAI();
	void LogicWalk();
	void GimmickArm();
	void GimmickLeg();

	void SetAutoArm( Sint32 sSide , Sint32 sRot1, Sint32 sRot2, Sint32 sFrm );
	void ArmControl();

	gxBool AtackChest();
	gxBool AtackArm();
	gxBool AtackBazooka();

	CHitCollision m_HitKurai;
	CEnemyAnime *m_pEnemyAnime;
	gxBool m_bDamage;

	ghPos m_Add;
	ghPos m_Tgt;
	ghPos m_Base;
	ghPos m_Right,m_Left;

	Sint32 m_sAtackType;
	StPatrsOffset m_stFoot[2];
	StPatrsOffset m_stLeg[2];
	StPatrsOffset m_stKakato[2];
	StPatrsOffset m_stArm[2];

	//-------------------------------
	//自動腕制御
	//-------------------------------
	StAutoArm m_StAutoArm[2];
	StAutoArm m_StAutoHand[2];

	Sint32 m_sWalkCycle;

	Sint32 m_sAtribute;

	Sint32 m_sMoveX1;
	Sint32 m_sMoveX2;
	Sint32 m_sLogicTime;
	Sint32 m_sControl;

	//------------------------------
	Sint32 m_sAtackSeq;
	Sint32 m_sAtackTimer;
	Sint32 m_sTargetRot;
	gxBool m_bMoveLimit;
	gxBool m_bMoveable;
	gxBool m_bWalkEnd;
	Sint32 m_sRebound;
	Sint32 m_sCrashTimer;

};


