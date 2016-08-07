//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneH0007YusouHeli : public CEnemyBase , public CHoundEnemyBase
{
	enum {
		enSoldierMax = 4,
		enNoneLeader = -1,
	};
public:

	CEneH0007YusouHeli( Sint32 x, Sint32 y );
	~CEneH0007YusouHeli();

	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetTargetPos(Sint32 x)
	{
		m_sTargetPosX = x;
	}

	void SettakeOff()
	{
		m_bTakeOff = gxTrue;
	}

	void   SetLanding()
	{
		m_bForceChakuriku = gxTrue;
	}

	gxBool IsLanding()
	{
		return m_bLanding;
	}

	CSoldierBase* GetSoldierLeader();

	gxBool IsSoldierRideOff();
	gxBool IsDamage()
	{
		return m_bWarning;
	}

private:


	void Atack();
	void LogicAI();
	void LogicMove();
	void LogicDropItem();
	void LogicEscape();

	void ProperaWind();
	CHitCollision m_HitKurai;
	OBJ_POS_T m_Add;

	CEneCmnSignal m_CEneCmnSignal;
	CEnemyAnime   *m_pEnemyAnime;
	gxBool m_bRideOff;
	Sint32 m_sAtribute;
	Sint32 m_sTargetPosX;
	Sint32 m_sHigh;
	Sint32 m_sHighAdd;
	Sint32 m_sChakurikuSeq;
	Sint32 m_sDropSeq;
	Sint32 m_sDoorOpenX;

	CSoldierBase *m_pSoldier[enSoldierMax];
	Sint32 m_sSoldierNum;
	Sint32 m_sSoldierLeader;
	Sint32 m_sCrashSeq;

	gxBool m_bTakeOff;
	gxBool m_bForceChakuriku;
	gxBool m_bLanding;
	gxBool m_bDamage;
	gxBool m_bWarning;
	gxBool m_bDummy;
};

