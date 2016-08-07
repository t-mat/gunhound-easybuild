class CFaceScreen;
class CMsgManager;

#pragma pack (push,1)
class CMsgWindow
{

public:
	CMsgWindow();
	~CMsgWindow();
	
	void Action();
	void Draw(Sint32 bx,Sint32 by);

	CMsgManager* GetFont()
	{
		return m_pFontScript;
	}

	void   Run(Sint32 sIndex);
	gxBool IsMessageNow();
	void   Reset();

private:

	Float32 fCount;

	Sint32  m_sButtonWaitTimer;
	CFaceScreen *m_pFaceScreen;
	CMsgManager *m_pFontScript;
	Sint32 m_sWindowOpenSeq;

	gxBool m_bSpeedUp;
	Sint32 m_sWindowHeight;
	Sint32 m_sCursorAnimCnt;

	gxBool m_bFastButtonOn;
	gxBool m_bCursorDraw;
};

#pragma pack (pop)
