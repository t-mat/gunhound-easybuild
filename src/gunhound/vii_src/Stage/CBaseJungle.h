//----------------------------------------------------------------------------
//
//ハウンドステージ １
//
//----------------------------------------------------------------------------

class CBaseJungle : public CStageBase
{
public:

	enum 
	{
		enForceMax = 8,
	};

	CBaseJungle()
	{
		for(Sint32 ii=0;ii<10;ii++)
		{
			EnemyIndex[ii] = 0;
		}

		m_sMorphColorCnt = 0;
		m_bBgColorChange = gxFalse;

		m_BgLayerPos.x = 0;
		m_BgLayerPos.y = 0;
		m_sRiverScroll = 0;
	}

	virtual ~CBaseJungle()
	{

	}

	virtual void Init();
	virtual void Action();
	virtual void End();

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

	gxBool m_bBgColorChange;
	Sint32 m_sMorphColorCnt;
	Sint32 m_sRiverScroll;

	ghPos m_BgLayerPos;
	ghPos m_FirePos;
};


