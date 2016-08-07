class CWazaCommand;
class CCustomHound;
class CMainMenu {

public:
	enum{

		enMainMenuStart,
		enMainMenuTutorial,
		enMainMenuMission,
		enMainMenuOption,

	#ifdef _VII_DEBUG_
			enMainMenuExit,
			enMainMenuDebug,
			enMainMenuMax,
	#else
			enMainMenuExit,
			enMainMenuMax,
			enMainMenuDebug,
	#endif

		enMainMenuEnding,
		enMainMenuOpening,
	};

	enum {
		enMainSeqTitle=0,
		enMainSeqMenuInit,
		enMainSeqMenuHiddenOpen,
		enMainSeqMenuMain,
		enMainSeqTraining,
		enMainSeqMission,
		enMainSeqOption,
		enMainSeqOptionEnvironment,
		enMainSeqOptionController,
		enMainSeqOptionCustomize,
		enMainSeqFadeWaitGoNext,

		enCaptionMax =4,

	};

	CMainMenu(Sint32 sSelect = 0);
	~CMainMenu();
	void Action();
	void Draw();

	Sint32 GetSelect()
	{
		return m_sSelect;
	}

	Sint32 GetNext()
	{
		return m_sNextScene;
	}

	gxBool IsEnd()
	{
		return m_bEnd;
	}

	void SetMessage( char *pAnnounce )
	{
		m_pAnnounce = pAnnounce;
	}

	void SetMainMenu(Sint32 sLayer=0 , Sint32 sCommand = 0)
	{
		//タイトルをスルーする
		m_bTitleThrough = gxTrue;
		m_sMenuForceSeq = sLayer;
		m_sDefaultStage = sCommand;
	}

	Sint32 GetSelectedItem()
	{
		return m_sMissionSel;
	}
	void SetDemoReady( gxBool bReady )
	{
		m_bSetDemoReady = bReady;
	}
	gxBool IsMutekiCommandOn()
	{
		return m_bMutekiOn;
	}
private:

	void MainMenu();
	void MainTraining();
	void MainMission();
	void MainOption();
	void EditMode();

	//------------------------------
	gxBool HiddenOpen();
	gxBool CheckScore();
	void DrawHiddenOpen();
	void DrawMainMenu();
	void DrawTraining();
	void DrawMission();
	void DrawOption();
	void DrawOptionController();
	void DrawOptionEnvironment();
	void DrawSprSaveData();
	char* GetWeaponNameByHiddenItem( Sint32 n );


	gxBool m_bEnd;
	gxBool m_bTitleThrough;
	Sint32 m_sMenuForceSeq;
	Sint32 m_sSelect;
	Sint32 m_sMenuLayer;
	Sint32 m_sNextScene;

	Sint32 m_sSequence;
	Sint32 m_sCursorColor;
	Sint32 m_sTitleAlpha;
	Sint32 m_sMainmenueAlpha;
	Sint32 m_sFactoryAlpha;
	Sint32 m_sTimer;
	Sint32 m_sColorTimer;

	Sint32 m_sTrainingSel;
	Sint32 m_sMissionSel;
	Sint32 m_sOptiongSel;

	char* m_pAnnounce;
	CWazaCommand *m_pWazaCommand;

	//キャプションの移動制御用
	Sint32 m_sCaption[enCaptionMax];
	Sint32 m_sMissionPosX;
	Sint32 m_sCaptionBoard;

	CCustomHound *m_pCustomHound;

	Float32 m_fScreenShot;
	Sint32 m_sAdTimer;
	Sint32 m_sJoyTrig;
	Sint32 m_sJoyRep;
	Sint32 m_sHiddenOpenCnt;
	Sint32 m_sHiddenOpenWait;
	Sint32 m_sDefaultStage;

	gxBool m_bSetDemoReady;
	gxBool m_Update[32];
	gxBool m_bCaption;
	gxBool m_bJoyDecision;
	gxBool m_bMutekiOn;
	gxBool m_bDummy;
};

