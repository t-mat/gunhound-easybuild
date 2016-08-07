class CSetUp
{
public:
	enum {
		enSeqInit,
		enSeqView,
		enSeqMain=49,
	};

	CSetUp(Sint32 sStage);
	~CSetUp();

	void Action();
	void NowLoading();
	void Draw();

	gxBool IsEnd();

	gxBool IsWait()
	{
		m_sWait --;
		if( m_sWait <= 0)
		{
			m_sWait = 0;
			return gxTrue;
		}
		return gxFalse;
	}

	gxBool GoDestroy()
	{
		//Ià‚ÉŒü‚©‚Á‚Äis‚³‚¹‚é
		m_Add.x += 1;
		m_NowLoading.x += m_Add.x;
		m_sFadeOut -= 3;

		if(m_sFadeOut<=0)
		{
			m_sFadeOut = 0;
			return gxTrue;
		}
		return gxFalse;
	}

	void SetViewStage(Sint32 n);
	void SetSequence(Sint32 sSeq);


private:


	Sint32 m_sSeq;
	Sint32 m_sWait;
	Sint32 m_sFadeOut;
	Sint32 m_sViewStage;

	ghPos  m_Add;
	ghPos  m_Base;
	ghPos  m_NowLoading;

	void ReadScenario();
	void NowLoadingDisp();
	void MissionDisp();
	void SetUpDisp();

	Sint32 m_sStage;
	Sint32 m_sFadeTimer;

	ghPos m_MissionPos;

	gxBool m_bNowLoading;
	gxBool m_bEnd;

};


