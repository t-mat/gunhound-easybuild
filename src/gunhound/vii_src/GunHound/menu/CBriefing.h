class CGrow;
class CBriefing
{
public:
	CBriefing(gxBool bExtra = gxFalse);
	~CBriefing();

	void Action();

	void Draw();

	void SetGold(Sint32 sGold)
	{
		m_sGold = sGold;
	}
	void SetClearStage(Sint32 *pClearStage )
	{
		m_pClearStage = pClearStage;
	}

	gxBool IsEnd();

	gxBool IsPreviewOK();

	Sint32 GetStage();

private:

	typedef struct StStageInfo
	{
		Sint32 sCost;
		Sint32 sGyara;
		char   *pGyaraMsg;
		char   *pCostMsg;
		char   *pStageName;
		char   **pStageDoc;
		gxBool bClear;
		gxBool bSelectable;
	}StStageInfo;

//	void Grow();
//	void GrowEnd();
	void SelectMission();
	void MissionDocument();

	void DrawBackGround();
	void DrawUI();
	void DrawMission();
	void DrawCursor();

	void GetStageInfo();

	Sint32 m_sSequence;

	Sint32 m_sGold;
	Sint32 m_sStage;
	Sint32 m_sSelectEvent;

	gxBool m_bEnd;

	//---------------------------------
	Sint32 m_sColorBoard;
	Sint32 m_sCursor;
	Sint32 m_sWaitTimer;
	Sint32 m_sWaitY;
	Sint32 m_sWaitYMax;

	Sint32 AlphaBoad[3];

	ghPos m_CursorPos;
	Sint32 m_sCursorTimer;
	Sint32 m_sCursorUpDown;
	Sint32 m_sCursorFlashU;
	Sint32 m_sCursorFlashD;
	Sint32 *m_pClearStage;
	Sint32 m_sDistance;
	Sint32 m_sMaximumItem;
	StStageInfo m_stStageInfo[enScenarioMax];
};


