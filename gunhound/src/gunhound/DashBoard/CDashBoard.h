//-----------------------------------------------------------
//
// ダッシュボード
//
//-----------------------------------------------------------
#ifndef _CDASHBOARD_H_
#define _CDASHBOARD_H_

struct gxSprite;
class sysMenu
{
public:
	static void Sprite(gxSprite *p,Sint32 x,Sint32 y,Sint32 prio,Sint32 atr=ATR_DEFAULT,unsigned long col=ARGB_DFLT,float scl=1.0f,Sint32 rot=0);
	static void Printf(Sint32 x,Sint32 y,Sint32 col,char *msg,...);
	static void Selectf(Sint32 bFlag , Sint32 x,Sint32 y,char *msg,...);
	static void Sprite(	int x,		int y,	int prio,	int page,	int u, 	int v,	int w,	int h,	
						 int cx=0,	int cy=0,	 int atr=ATR_ALPHA_NML,	 unsigned long col=ARGB_DFLT,	 float rot=0, float scl_x=1.0f,float scl_y=1.0f	);
	static void Play(Sint32 n);
private:

};

enum {
	enIconNone     = 0x00,
	enIconDecision = 0x01,
	enIconCancel   = 0x02,
	enIconMax,
};

class CDashPadConfig;
class CDashEnvironment;

class CDashBoard
{
public:

	CDashBoard();
	~CDashBoard();

	static CDashBoard* GetInstance()
	{
		//インスタンスを作成する

		if( s_pInstance == NULL )
		{
			s_pInstance = new CDashBoard();
		}

		return s_pInstance;
	}

	static void Destroy()
	{
		//インスタンスを削除する

		if( s_pInstance )
		{
			delete s_pInstance;
			s_pInstance = NULL;
		}
	}

	gxBool IsReset()
	{
		return m_bReset;
	}

	void SetResetOff()
	{
		m_bDisp  = gxFalse;
		m_bReset = gxFalse;
	}

	gxBool IsLogoComplete()
	{

		if( m_sAlpha == 0xFF ) return gxTrue;

		return gxFalse;
	}

	void Action();

	void SetLogo(gxBool bFlag);
	void SetWallPaper(gxBool bFlag);

	gxBool IsMenuBar()
	{
		//メニューバー出てますか？
		if( m_sMenuBarCnt ) return gxTrue;
		return gxFalse;
	}
	void SetMenuDisable(gxBool bFlag)
	{
		m_bMenuDisable = bFlag;
	}

	void SetAnnounce( char *pStr );

	void SetIcon(Sint32 sFlag)
	{
		m_sIconFlag = sFlag;
	}

	void SetLock(gxBool bFlag)
	{
		m_bLock = bFlag;
	}

	void ForceMenuDisp(gxBool bFlag)
	{
		m_bForceMenuDisp = bFlag;
	}

	void SetForceMenu(Sint32 sSeq)
	{
		//-------------------------------
		//強制メニュー
		//-------------------------------
		m_sForceConfig = sSeq;
	}

	gxBool IsForceMenu()
	{
		if( m_sForceConfig )
		{
			return gxTrue;
		}

		return gxFalse;
	}

	Sint32 GetSoundVolume()
	{
		//サウンドのボリュームを０～１００％で返す

		Sint32 sVol = 50+50*m_sVolume/WHEEL_MAX;

		if( sVol < 0   ) sVol = 0;
		if( sVol > 100 ) sVol = 100;

		return sVol;
	}

private:

	gxBool m_bDisp;

	void LogoControl();
	void DrawLogo();
	void ControlMenuBar();

	void ManuSeqControl();
	gxBool ConfigEnvironment();
	gxBool ConfigController();

	Sint32 MainMenu();

	void DrawMenuBar();
	void DrawIcon();
	void DrawAnnounce();
	void DrawSoundVolume();

	static CDashBoard *s_pInstance;


	gxBool m_bMenuDisable;
	Sint32 m_sMenuBarCnt;
	Sint32 m_sSelect;
	Sint32 m_sSeq;
	gxBool m_bReset;

	gxBool m_bDispLogo;
	Sint32 m_sAlpha;

	gxBool m_bDispWall;
	Sint32 m_sAlphaWall;

	CDashPadConfig   *m_pPadConfig;
	CDashEnvironment *m_pEnvironment;
	Sint32 m_sTimer;

	Sint32 m_sIconFlag;
	Sint32 m_sIconX;
	Sint32 m_sForceConfig;

	Sint32 m_sScrollWait;
	Float32 m_fScroll;
	char *m_pAnnounce;

	Sint32 m_sVolume;
	Sint32 m_sVolumeWait;

	Float32 m_fScaleL;
	Float32 m_fScaleR;

	gxBool m_bLock;	//ダッシュボードの開閉をロックする
	gxBool m_bForceMenuDisp;
};

enum {
	enBtnMenuDecision = enBtnDecision|BTN_START|BTN_SELECT,
	enBtnMenuCancel   = enBtnCancel,
};

#endif	//_CDASHBOARD_H_
