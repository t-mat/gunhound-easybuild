class CContinue
{
public:

	CContinue();
	~CContinue();

	void Action();
	void Draw();

	gxBool IsEnd()
	{
		return m_bEnd;
	}

	Sint32 GetNext()
	{
		return m_sReturnSeq;
	}

	void SetScore( Uint32 uAllScore , Uint32 uStageScore );

private:

	Sint32 m_sSeq;
	Sint32 m_sContTimer;
	Sint32 m_sCount;
	Sint32 m_sReturnSeq;

	gxBool m_bGameOver;
	gxBool m_bEnd;

	Sint32 m_sAllScore;
	Sint32 m_sStageScore;

	Sint32 m_sFlashAlpha;
	Sint32 m_sHalfScore;
};


