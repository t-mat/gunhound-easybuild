//----------------------------------------------------------------------------
//
//ハウンドステージ １
//
//----------------------------------------------------------------------------

class CStageLabo : public CStageBase
{
public:

	enum 
	{
		enForceMax = 8,
	};

	CStageLabo()
	{
		for(Sint32 ii=0;ii<10;ii++)
		{
			EnemyIndex[ii] = 0;
		}

		m_sLoop = 0;

	}

	virtual ~CStageLabo()
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

	virtual void InitEnemies();
	virtual void SetEnemy();

	Sint32 m_sLoop;
};


