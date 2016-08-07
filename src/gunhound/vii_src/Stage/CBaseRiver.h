//----------------------------------------------------------------------------
//
//ハウンドステージ １
//
//----------------------------------------------------------------------------

typedef struct StRiver {
	StRiver()
	{
		m_sScroll = 0;
		m_sAdd    = 0;
		m_sInfinityScroll = 0;
	}

	Sint32 m_sScroll;
	Sint32 m_sAdd;
	Sint32 m_sInfinityScroll;

}StRiver;

class CBaseRiver : public CStageBase
{
public:

	enum {
		enHoundMaster,
		enHoundSlave,
		enHoundMax,
	};
	enum {
		enCommanchMax=3,
	};

	enum 
	{
		enForceMax = 8,
		enClubMax = 4,
	};

	CBaseRiver()
	{
		Sint32 ii,jj;

		m_sBombPosition = 0;
		m_sTransShipX = 0;

		for(ii=0;ii<10;ii++)
		{
			EnemyIndex[ii] = 0;
		}

		for(jj=0;jj<enClubMax;jj++)
		{
			m_pClubGunner[jj] = NULL;
		}

		for(ii=0;ii<enHoundMax;ii++)
		{
			m_pHound[ii] = NULL;
		}

		for(ii=0;ii<enCommanchMax;ii++)
		{
			m_pCommanch[ii] = NULL;
		}

		m_pTransShips = NULL;

		m_pBoss                = NULL;
		m_pMiddleBoss[0]       = NULL;
		m_pMiddleBoss[1]       = NULL;

		m_sWaraWaraCnt = 0;
		m_sScrollPos   = 0;
		m_sSplahCnt    = 0;
		m_sSplahHeight = 0;
		m_sGameOverWait = 0;
	}

	virtual ~CBaseRiver()
	{

	}

	virtual void Init();
	virtual void Action();
	virtual void End();
	virtual void Draw();

	static Sint32 m_sTransShipX;

protected:

	virtual void Area01();
	virtual void Area02();
	virtual void Area03();
	virtual void Area04();
	virtual void Area05();
	virtual void AreaCommon();

	virtual void BgCommon(Sint32 x=0,Sint32 y=0);

	virtual void InitEnemies(Sint32 sMin=0);
	virtual void SetEnemy(Sint32 sMin=0);

	gxBool WaraWaraCommanch();
	void ControlHound(Sint32 sOperation );

	gxBool m_bLoop;
	StRiver stRiver;
	StRiver stRiver2;
	StRiver stRiver3;

	ghPos   m_stRiverOffset;
	CEneH0203Snake   *m_pBoss;
	CEneH0203Serpent *m_pMiddleBoss[2];
	CEneH0102NightSpider* m_pTankBoss;
	CEneH0207TransShips* m_pTransShips;
	CEneH0103ClubGunner * m_pClubGunner[enClubMax];

	CGunHound *m_pHound[enHoundMax];

	Sint32 m_sWaraWaraCnt;
	CEneH0002Commanch   *m_pCommanch[enCommanchMax];

	OBJ_POS_T m_DummyPos;
	OBJ_POS_T m_LeaderyPos;

	Sint32 m_sScrollPos;
	Sint32 m_sBombPosition;
	Sint32 m_sSplahCnt;
	Sint32 m_sSplahHeight;
	Sint32 m_sGameOverWait;
};
