//#include "kernel/CPadControl.h"

enum{
	enPadCfgLayeNone=-1,
	enPadCfgLayerMain=0,
	enPadCfgLayerPad,
	enPadCfgLayerButton,
	enPadCfgLayerDevice,
	enPadCfgLayerMax,
};
class CDashPadConfig
{
public:

	CDashPadConfig();
	~CDashPadConfig();
	void Action();
	void Draw();

	gxBool IsEnd();
	void SetPadButtonOn(Sint32 n);

private:

	void SelectMain();
	void SelectPad();
	void SelectBtn();
	gxBool SelectBtnWait();
	void SelectDvc();

	void DrawMain();
	void DrawPadSel();
	void DrawBtnSel();
	void DrawDvcSel();

	Sint32 GetDiviceNum();

	Sint32 m_sSelect[enPadCfgLayerMax];
	Sint32 m_sScroll[enPadCfgLayerMax];

	Sint32 m_ConfigLayer;
	Sint32 m_ChangeLayer;

	Sint32 m_KeyBoardRepeat;
	Sint32 m_sPadSelectMax;

	gxPos  m_Pos;

	Sint32 m_sSeqWaitMode;
	Sint32 m_sRotation;

	gxBool m_bPadInit;
	gxBool m_bPadNumOver;
	gxBool m_bEnd;
	gxBool m_bButtonOn[32];

	ButtonConfig_t m_stBkButtonConfig;

	gxBool m_bNext;
	gxBool m_bAutoSeq;
	gxBool m_bReleaseCheck;
};
