class CSoldierBase;
class CEffScene0A01 : public CEffectBase
{
public:

	CEffScene0A01(Sint32 x,Sint32 y ,Sint32 z );
	~CEffScene0A01();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

	Sint32 GetSoldierNum()
	{
		return m_sSoldierCnt;
	}

	CSoldierBase *GetSoldierAddr(Sint32 n)
	{
		return m_pSoldier[n];
	}

	gxBool IsEnd()
	{
		return m_bFinish;
	}

	enum {
		enSoldierMax = 6,
	};

private:


	Sint32 m_sPrio;
	Sint32 m_sEnsyutuTimer;

	Sint32 m_sColor;
	Sint32 m_sColorAdd;

	CEffExPerticle *m_pHeli;
	CSoldierBase   *m_pSoldier[enSoldierMax];	//‚±‚±‚Å‚ÍŠJ•ú‚µ‚È‚¢
	Sint32 m_sSoldierCnt;
	gxBool m_bFinish;
};

