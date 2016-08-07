//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneH0003Vorzoy : public CEnemyBase , public CHoundEnemyBase
{
public:
	enum {
		enLogicNormal   = 0,
		enLogicWaraWara = 10,
		enLogicCarry    = 20,
		enLogicFlying   = 30,
	};
	CEneH0003Vorzoy( Sint32 x, Sint32 y ,gxBool bDeadWait=gxFalse );
	~CEneH0003Vorzoy();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

	CEasyLeynos* GetLeynos()
	{
		return &m_Leynos;
	}

	void SetDashMode(gxBool bFlag = gxTrue)
	{
		m_bDashMode = bFlag;
	}
	void SetLogic(Sint32 sLogicPattern )
	{
		m_sLogicPattern = sLogicPattern;
	}

	void SeqCustomize( Sint32 sBody ,Sint32 sArm , Sint32 sLeg);
	void SetNoneScrollOut()
	{
		m_bNoneScrollOut = gxTrue;
	}

private:

	void Atack();
	void Atack2();
	void LogicAI();
	void LogicNormal();
	void LogicWaraWara();
	void LogicCarryAtack();
	void LogicFlyingAtack();

//	void SetDamageMotion();
//	void ActDamageMotion();

	CHitCollision m_HitKurai;
	CEasyLeynos m_Leynos;

	Sint32 m_sAtribute;

	//-----------------------------
	ghPos m_Add;
	ghPos m_Base;

	Sint32 m_sJumpWait;
	Sint32 m_sBackWait;
	Sint32 m_sLogicSeq;
	Sint32 m_sPadControl;

	Sint32 m_sAtackWait;
	Sint32 m_sAtackLag;
	Sint32 m_sAtackCnt;

	Sint32 m_sCrashTimer;

	OBJ_POS_T m_Src;

	Sint32 m_sLogicPattern;
	Sint32 m_sLogicTimer;

//	Sint32 m_sDamgeWait;

	gxBool m_bDashMode;
	gxBool m_bDeadWait;
	gxBool m_bNoneScrollOut;

};

