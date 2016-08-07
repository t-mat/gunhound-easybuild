//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneH0408Powerdsuit : public CEnemyBase , public CHoundEnemyBase
{
public:
	enum{
		enLogicPrev,
		enLogicAssault,
		enLogicBattleShip,
		enLogicDefence,
		enLogicAtackTarget,
	};

	CEneH0408Powerdsuit( Sint32 x, Sint32 y ,gxBool bDeadWait=gxFalse );
	~CEneH0408Powerdsuit();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

	CEasyLeynos* GetLeynos()
	{
		return &m_Leynos;
	}

	void SetTarget(OBJ_POS_T *pPos , Sint32 OffsetX)
	{
		m_pTgt = pPos;
		m_Offset.x = OffsetX;
	}

	void SetLogic(Sint32 sLogicPattern )
	{
		m_sLogicPattern = sLogicPattern;
	}

	void SetDashMode(gxBool bFlag = gxTrue)
	{
		m_bDashMode = bFlag;
	}

	void SetPrio(Sint32 n)
	{
		m_Pos.z = n;
		m_Leynos.SetPrio(m_Pos.z);
	}

	void SetRealAtack()
	{
		m_bRealAtack = gxTrue;
	}

	void SetLeader()
	{
		m_bLeader = gxTrue;
	}

	void SetArmRange(Sint32 sArmRange )
	{
		m_sArmRotRange = sArmRange;
	}

	void AtackTarget(ghPos *pPos ,Sint32 sOffset)
	{
		//攻撃対象を設定する
		m_pAtkTgt = pPos;
		m_Offset.x = sOffset;
		m_Offset.y = 0;
	}

private:

	void Atack();
	void Atack2();
	void LogicAI();
	void LogicNormal();
	void LogicAssault();
	void LogicBattleShip();
	void LogicDefence();
	void LogicAtack();
	void GimmickWaterEffect();

	void GattlingAtack();
	void SeqCustomize( Sint32 sBody ,Sint32 sArm , Sint32 sLeg);

	void AdjustArmAngle( Sint32 ax , Sint32 ay );

	CHitCollision m_HitKurai;
	CEasyLeynos m_Leynos;

	Sint32 m_sAtribute;

	//-----------------------------

	Sint32 m_sJumpWait;
	Sint32 m_sBackWait;
	Sint32 m_sLogicSeq;
	Sint32 m_sPadControl;

	Sint32 m_sAtackWait;
	Sint32 m_sAtackLag;
	Sint32 m_sAtackCnt;

	OBJ_POS_T* m_pTgt;
	ghPos*     m_pAtkTgt;
	ghPos      m_Offset;
	ghPos      m_Offset2;
	ghPos      m_Old;

	gxBool m_bDeadWait;
	gxBool m_bDashMode;
	Sint32 m_sLogicPattern;

	gxBool m_bRealAtack;
	gxBool m_bLeader;

	//-----------------------------
	Sint32 m_sAtackSeq;
	Sint32 m_sAtackTimer;
	Sint32 m_sArmRotRange;
};

