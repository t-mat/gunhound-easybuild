class CStageStart : public CBaseJungle
{
public:

	CStageStart();
	~CStageStart();

	void Init();
	void Action();
	void Draw();
private:

	enum {
		enHeavyTankMaster,
		enHeavyTankSlave,
		enHeavyTankmax,
	};
	enum {
		enHoundMaster,
		enHoundSlave,
		enHoundPlayer,
		enHoundMax,
	};
	enum {
		enTachikomaMax = 3,
		enTankGuardMax = 4,
		enApacheMax    = 3,
	};

	void Area01();
	void Area02();
	void Area03();
	void Area04();
	void Area05();
	void AreaCommon();
	gxBool WaraWaraSpider();
	void SetCrashDoorEffect();
	void SetFirePlantEffect();
	void ControlHound(Sint32 sOperation );
	OBJ_POS_T m_DummyPos;
	ghPos m_TargetPos;
	CEneH0204Grazzi    *m_pGrazzi;						//ÉOÉâÅ[ÉW
	CEneH0101HeavyTank  *m_pTank[enHeavyTankmax];		//ñ°ï˚êÌé‘
	CEneH0102NightSpider *m_pNightSpider;	//ëŒATêÌé‘
//	CEneH0408Powerdsuit *m_pHound[enHoundMax];
	CGunHound *m_pHound[enHoundMax];
	CEneH0004Tachikoma  *m_pTachikoma[enTachikomaMax];
	CEneH0007YusouHeli  *m_pYusouHeli;
	CEneH0410Wall       *m_pWall;
	CEneH0105TankGuard  *m_pTankGuard[enTankGuardMax];
	CEneH0503HoundFly   *m_pHoundFly[enHoundMax];
	CEneH0503HoundFly   *m_pPlayerFly[enHoundMax];
	CEneH0106Yagura     *m_pYagura;
	CEneH0005TransHeli  *m_pTransHeli[enApacheMax];

	Sint32 m_sFlashAlpha;
	gxBool m_bGrazzi;
	Sint32 m_sWaraWaraCnt;
	Sint32 m_sScrollSeq;
	Sint32 m_sScrollPos;
	Sint32 m_sBombPosition;
	Sint32 m_sCoveredSeq;
};

