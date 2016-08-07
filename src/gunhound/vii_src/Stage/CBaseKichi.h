//----------------------------------------------------------------------------
//
//ハウンドステージ １
//
//----------------------------------------------------------------------------

class CBaseKichi : public CStageBase
{
public:

	enum 
	{
		enForceMax = 8,
		enTaranturaMax = 2,
		enCannonMax = 4,
		enPlasmaMax = 16,
		enWallMax = 8,
		enVorzoyMax = 5,
	};
	enum {
		enHoundMaster,
		enHoundSlave,
		enHoundPlayer,
		enHoundMax,
	};

	CBaseKichi()
	{
		Sint32 ii;

		for(ii=0;ii<10;ii++)
		{
			EnemyIndex[ii] = 0;
		}

		for(ii=0;ii<enTaranturaMax;ii++)
		{
			m_pTarantura[ii] = NULL;
		}

		for(ii=0;ii<enPlasmaMax;ii++)
		{
			m_pEnergyUnitZako[ii] = NULL;
			m_pEnergyUnitMain = NULL;
		}

		for(ii=0;ii<enWallMax;ii++)
		{
			m_pCEneH0410Wall[ii] = NULL;
		}

		for(ii=0;ii<enVorzoyMax;ii++)
		{
			m_pVorzoy[ii] = NULL;
		}
		for(ii=0;ii<enHoundMax;ii++)
		{
			m_pGunHound[ii] = NULL;
		}

		m_pGravitStar = NULL;

		m_Pos.x = 0;
		m_Pos.y = 0;

		m_stScrAdd.x = 0;
		m_stScrAdd.y = 0;
		m_stScrAdd.z = 0;

		m_sWaraWaraCnt = 0;
		m_sWaraWaraSeq = 0;

		m_sWaraWaraVorzoySeq = 0;
		m_sWaraWaraVorzoyCnt = 0;

		m_sPlasmaCnt = 0;
		m_sPlasmaDestroyCnt = 0;

		m_pNightSpider = NULL;
		m_pGrazzi = NULL;

		m_pTorpedo = NULL;
		m_sPodTimer = 0;
		m_bSpiderDead = gxFalse;

		m_pNautilus = NULL;

		m_pScene    = NULL;

		m_sAlpha = 0x00;
		m_sWhite = 0x00;
		m_sStunBarrierIndex = 0;

		m_bWarning   = gxFalse;
		m_bBackView  = gxFalse;
		m_sBridgePos = 0;
		m_sBridgeBomb[0] = 0;
		m_sBridgeBomb[1] = 0;

	}

	virtual ~CBaseKichi()
	{

	}

	virtual void Init();
	virtual void Action();
	virtual void End();

protected:

	virtual void Area01a();
	virtual void Area01b();
	virtual void Area02();
	virtual void Area03();
	virtual void Area04();
	virtual void Area05();

	virtual void AreaCommon();

	virtual void BgCommon(Sint32 x=0,Sint32 y=0);

	virtual void InitEnemies();
	virtual void SetEnemy();

	gxBool AreaInfinityScroll();
	gxBool AreaBombAndWall();

	void SetBrokenSeeling();
	void SetBridgeBomb(Sint32 n );
	void BridgeGimmick();
	void KaiteiGimmick();

	CEneH0204Grazzi        *m_pGrazzi;						//グラージ
	CEneH0102NightSpider   *m_pNightSpider;					//対AT戦車
	CEneH0406Carrymissile  *m_pTorpedo;						//魚雷
	CEneH0501PlasmaMachine *m_pEnergyUnitMain;				//エネルギーユニット（ボス）
	CEneH0501PlasmaMachine *m_pEnergyUnitZako[enPlasmaMax];

	//----------------------------------------
	//高速スクロール処理
	//----------------------------------------
	typedef struct StHighSpeedScrol
	{
		Sint32 sOffset;
		Sint32 sSpeed;
		Sint32 sLoopCnt;
		Sint32 sBg1x;
		gxBool bLoopEnd;
		gxBool bLoopNow;

		StHighSpeedScrol()
		{
			sOffset  = 0;
			sSpeed   = 0;
			sLoopCnt = 0;
			bLoopEnd = 0;
			bLoopNow = 0;
			sBg1x = 0;
		}
	} StHighSpeedScrol;

	ghPos m_stScrAdd;
	StHighSpeedScrol m_stScroll;
	Sint32 m_sPodTimer;

	CEneH0410Wall *m_pCEneH0410Wall[enWallMax];
	Sint32 m_sAlpha;
	Sint32 m_sWhite;
	Sint32 m_sStunBarrierIndex;

	//ワラワラゲーム
	gxBool WaraWaraTarantura();
	Sint32 m_sWaraWaraSeq;
	Sint32 m_sWaraWaraCnt;
	gxBool m_bSpiderDead;
	CEneH0403Tarantura* m_pTarantura[enTaranturaMax];

	//ワラワラボルゾイ（フライング）
	gxBool WaraWaraVorzoyFlying();
	Sint32 m_sWaraWaraVorzoySeq;
	Sint32 m_sWaraWaraVorzoyCnt;
	CEneH0003Vorzoy* m_pVorzoy[enVorzoyMax];

	//プラズマゲーム
	Sint32 m_sPlasmaCnt;
	Sint32 m_sPlasmaDestroyCnt;

	CEneH0404GravityStar *m_pGravitStar;

	CEffScene02 *m_pScene;

	CEneH0407Nautilus *m_pNautilus;

	CGunHound * m_pGunHound[enHoundMax];
	ghPos       m_HoundPos[enHoundMax];

	//ブリッジ制御

	Sint32 m_sBridgePos;
	gxBool m_bBackView;
	gxBool m_bWarning;

	Sint32 m_sBridgeBomb[2];

};
