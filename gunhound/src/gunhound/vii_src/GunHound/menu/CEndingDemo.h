class CEndingDemo
{
public:
	enum {
		enStarMax=128,
		enRoughMax = 32,
	};

	typedef struct StEndStar
	{
		Sint32 x,y;
		Uint32 sCol;
	}StEndStar;


	CEndingDemo();
	~CEndingDemo();

	void Command(Sint32 sCmd);
	void Action();
	void Draw();

	gxBool IsEnd()
	{
		return m_bEnd;
	}

	void SetAfterEpisode()
	{
		m_bAfterEpisode = gxTrue;
	}
	void SetEndingSkip()
	{
		m_bEndingSkip = gxTrue;
	}

	void SetScoreDisp( gxBool bFlag )
	{
		m_bDisp = bFlag;
	}
	void SetAllScore(Uint32 uScore)
	{
		m_uAllScore = uScore;
	}
	void SetContinueBonus(Uint32 uScore)
	{
		m_uContinueBonus = uScore;
	}
	void SetTrueEndBonus(Uint32 uScore)
	{
		m_uTrueEndBonus = uScore;
	}
	void SetNoContinueBonus(Uint32 uScore)
	{
		m_uNoContinueBonus = uScore;
	}
	void SetTotalScore(Uint32 uScore)
	{
		m_uTotalScore = uScore;
	}

private:

	void StarScreen();
	void StarControl();

	void SeqBefore();
	void SeqStaffRoll();
	void SeqAfter();
	void RoughPicture();

	void DrawBefore();
	void DrawStaffRoll();
	void DrawAfter();

	typedef struct StRoughPicture{

		Sint32 m_sIndex;
		Sint32 m_sRoughSeq;
		Sint32 m_sRoughWait;
		Float32 m_fRoughAlpha;
		Float32	m_fScale;

		StRoughPicture()
		{
			m_sIndex = 0;
			m_sRoughSeq = 0;
			m_sRoughWait = 0;
			m_fRoughAlpha = ZERO;
			m_fScale = 1.0f;
		}

	}StRoughPicture;

	Sint32    m_sTimer;
	gxBool    m_bEnd;
	Float32   m_fScroll;

	VECTOR    StStar[enStarMax];
	StEndStar *m_pStar;

	VECTOR m_Pos;
	VECTOR m_Rot;

	Sint32 m_sSeq;

	Sint32 m_sBeforeSeq;
	Sint32 m_sBeforeAlpha;
	Sint32 m_sStaffRollAlpha;

	Sint32 m_sStaffrollSeq;
	Sint32 m_sAfterSeq;
	Sint32 m_sEndTimer;

	Sint32 m_sRoughCnt;
	StRoughPicture m_stRough[enRoughMax];

	Sint32 m_sScroll1;
	Sint32 m_sScroll2;

	gxBool m_bAfterEpisode;
	gxBool m_bEndingSkip;

	//スコア関連
	gxBool m_bDisp;
	Uint32 m_uAllScore;
	Uint32 m_uContinueBonus;
	Uint32 m_uTrueEndBonus;
	Uint32 m_uNoContinueBonus;
	Uint32 m_uTotalScore;
	//-------------------------
	Sint32 m_sScoreAlpha;

	gxBool m_bDummy;
};


