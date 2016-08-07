//----------------------------------------------------------------------------
//
//ハウンドステージ １
//
//----------------------------------------------------------------------------
class CEneH0408Powerdsuit;
class CBaseExtra : public CStageBase
{
public:

	enum 
	{
		enForceMax = 8,
		enNozzleMax = 2,
		enSubCannonMax = 3,
		enHoundMax = 2,
	};

	CBaseExtra()
	{
		Sint32 ii;

		for(ii=0;ii<10;ii++)
		{
			EnemyIndex[ii] = 0;
		}

		m_sOutofRangeTime=0;
		m_sLoop = 0;

		m_PosFlagShip.x = 0;
		m_PosFlagShip.y = 0;

		m_bStageClearStep = gxFalse;

		m_sAtackTimer = 0;
		m_sAtackSeq   = 0;

		m_Pos.x = 0;
		m_Pos.y = 0;

		m_sBgScroll = 0;
		m_Scroll[0].x = 0;
		m_Scroll[1].x = 0;
		m_Scroll[2].x = 0;

		for(Sint32 jj=0; jj<32;jj++)
		{
			if( jj < enNozzleMax )    m_pNozzle[jj]    = NULL;
			if( jj < enSubCannonMax ) m_pSubCannon[jj] = NULL;
			if( jj < enHoundMax )     m_pGunHound[jj]  = NULL;
		}

		m_bFrontTankDead  = gxFalse;
		m_bMiddleTankDead = gxFalse;
		m_bRearTankDead   = gxFalse;

		m_bNozzleEnd            = gxFalse;
		m_bEngineBlockCompleted = gxFalse;

		m_pMainCannon = NULL;
		m_bMainCannonEnd = gxFalse;

		m_pBodyTank = NULL;
		m_bBodyTankEnd = gxFalse;

		m_pCannonCore    = NULL;
		m_bCannonCoreEnd = gxFalse;

		m_pEnergyRear   = NULL;
		m_pEnergyFrnt   = NULL;
		m_pEnergyMiddle = NULL;

		m_pBodyPanel  = NULL;

		m_pBoss = NULL;

		m_sFinalAtackWait = 0;

		g_sOverLayerHeight = 0;

		m_bZakiDead = gxFalse;
		m_bEscape   = gxFalse;

		m_bLastBoss = gxFalse;
		m_ScrAdd.x = 0;
		m_ScrAdd.y = 0;
		m_ScrPos = m_ScrAdd;
		m_sHighSpeed = 0;
	}

	virtual ~CBaseExtra()
	{

	}

	virtual void Init();
	virtual void Action();
	virtual void End();
	virtual void Draw();

	static Sint32 GetOverLayerY()
	{
		//カバー部の高さ
		return g_sOverLayerHeight ;
	}

	static Sint32 g_sOverLayerHeight;

protected:

	virtual void Area01();
	virtual void Area02();
	virtual void Area03();
	virtual void Area04();
	virtual void Area05();
	virtual void Area06();
	virtual void Area07();
	virtual void Area08();
	virtual void AreaCommon();

	virtual void BgCommon(Sint32 x=0,Sint32 y=0);

	virtual void InitEnemies();
	virtual void SetEnemy();

	void Gimmick();
	gxBool EngineAtack();
	void FinalAtack();
	void ResetEnemy();

	void AppendBgm();
	//---------------------------

	Sint32 m_sAtackSeq;
	Sint32 m_sAtackTimer;

	Sint32 m_sLoop;
	Sint32 m_sOutofRangeTime;

	//------------------------------------
	//キャノン砲台（大）
	//------------------------------------
	gxBool m_bFrontTankDead;
	gxBool m_bMiddleTankDead;
	gxBool m_bRearTankDead;

	CEneEx0002Laser     *m_pSubCannon[enSubCannonMax];


	//---------------------------
	//エンジン周り
	//---------------------------
	CEneEx0007Nozzle *m_pNozzle[enNozzleMax];
	gxBool m_bNozzleEnd;
	gxBool m_bEngineBlockCompleted;

	//---------------------------
	//ニュートロンポリマー砲
	//---------------------------
	CEneEx0001Cannon *m_pMainCannon;
	gxBool m_bMainCannonEnd;

	Sint32 m_sHighSpeed;

	//---------------------------
	CEneEx0005Wii *m_pCannonCore;
	gxBool m_bCannonCoreEnd;

	OBJ_POS_T m_PosFlagShip;

	gxBool m_bStageClearStep;

	CEneEx0006Tank* m_pEnergyFrnt;
	CEneEx0006Tank* m_pEnergyMiddle;
	CEneEx0006Tank* m_pEnergyRear;

	//---------------------------
	//コア
	//---------------------------
	CEneEx0006Tank *m_pBodyTank;
	gxBool m_bBodyTankEnd;

	CEneEx0009Body * m_pBodyPanel;
	CGunHound *m_pGunHound[enHoundMax];

	CEneH0504Boss *m_pBoss;
	Sint32 m_sFinalAtackWait;
	Sint32 m_sBgScroll;
	ghPos  m_Scroll[3];
	gxBool m_bZakiDead;
	gxBool m_bLastBoss;
	gxBool m_bEscape;

	ghPos m_ScrAdd;	
	ghPos m_ScrPos;
};


