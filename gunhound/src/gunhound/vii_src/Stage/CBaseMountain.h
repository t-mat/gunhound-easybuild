//----------------------------------------------------------------------------
//
//ハウンドステージ １
//
//----------------------------------------------------------------------------

class CBaseMountain : public CStageBase
{
public:
	enum {
		enHoundMaster,
		enHoundSlave,
		enHoundMax,
	};

	enum 
	{
		enForceMax = 8,
		enHoverMax = 3,
		enCommanchMax = 3,
		enWaraWaraCommanchMax = 22,
	};


	CBaseMountain()
	{
		Sint32 ii;

		m_pBoss = NULL;
		m_pYusouHeli = NULL;

		for(ii=0;ii<10;ii++)
		{
			EnemyIndex[ii] = 0;
		}

		for(ii=0;ii<enHoverMax;ii++)
		{
			m_pHovered[ii] = NULL;
		}
		for(ii=0;ii<enCommanchMax;ii++)
		{
			m_pCommanch[ii] = NULL;
		}

		m_sWaraWaraCommanchCnt = 0;
		m_bWaraWaraCommanch = gxFalse;

		m_sVorZoyCnt = 0;
		m_pVorzoy[0] = NULL;
		m_pVorzoy[1] = NULL;
		m_pVorzoy[2] = NULL;

		m_sRescueSeq = 0;
		m_sRescueCnt = 0;

		m_stScroll.x = 0;
		m_stScroll.y = 0;

		m_pKillerTank  = NULL;
		m_pHound[0]    = NULL;
		m_pHound[1]    = NULL;

		m_pGrazzi = NULL;
		m_pRainEffect = NULL;

		m_HoundPos.x = 0;
		m_HoundPos.y = 0;

		m_Pos.x = 0;
		m_Pos.y = 0;

		m_sBackGroundScroll = 0;
		m_bGakeShita = gxFalse;
		m_bBgm1Play  = gxFalse;

		m_stTargetPos.x = 0;
		m_stTargetPos.y = 0;
		m_sGameOverWait = 0;

		m_pGunpelly = NULL;
		m_bForceGameOver = gxFalse;

	}

	virtual ~CBaseMountain()
	{

	}

	virtual void Init();
	virtual void Action();
	virtual void End();
	void Draw();

protected:

	virtual void Area01();
	virtual void Area02();
	virtual void Area03();
	virtual void Area04();
	virtual void Area05();
	virtual void AreaCommon();

	gxBool VorzoyWaraWara();
	gxBool HoveredWaraWara();
	void SetFireEffect();

	gxBool WaraWaraCommanch();

	void BossBattle();
	void GakeKudari();

	virtual void BgCommon(Sint32 x=0,Sint32 y=0);

	virtual void InitEnemies();
	virtual void SetEnemy();
	void ControlHound(Sint32 sOperation );

	ghPos m_stScroll;
	CEneH0303Valkylie *m_pBoss;

	Sint32 m_sVorZoyCnt;
	CEneH0003Vorzoy *m_pVorzoy[3];

	OBJ_POS_T m_TransPorterPos;
	OBJ_POS_T m_HoundPos;
	CEneH0007YusouHeli   *m_pYusouHeli;
	CEneH0102NightSpider *m_pKillerTank;

	CEneH0204Grazzi *m_pGrazzi;

	Sint32 m_sRescueSeq;
	Sint32 m_sRescueCnt;

	CEneH0108Hovered *m_pHovered[enHoverMax];
	CEffScene0A02 *m_pRainEffect;

	Sint32 m_sBackGroundScroll;
	gxBool m_bGakeShita;
	gxBool m_bBgm1Play;

	gxBool m_bWaraWaraCommanch;
	OBJ_POS_T m_stTargetPos;
	CEneH0002Commanch *m_pCommanch[enCommanchMax];
	Sint32 m_sWaraWaraCommanchCnt;
	Sint32 m_sGameOverWait;

	CGunHound *m_pHound[enHoundMax];
	CEneH0301Gunpelly *m_pGunpelly;
	gxBool m_bForceGameOver;
};

