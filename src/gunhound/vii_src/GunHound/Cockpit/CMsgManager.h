//-----------------------------------------------------------
//
// ���b�Z�[�W�t�H���g��ǂݍ��ݗp
//
//-----------------------------------------------------------
enum{
	enCodeNone,
	enCodeStop,
	enCodeFaceChange,
	enCodeMessageDisp,
	enCodeWaitButton,
	enCodeWaitTime,
	enCodeFinish,
};

typedef struct StMsgFmt {
	Sint32 sOpCode;	//���߃R�[�h
	Sint32 sData;	//����(���b�Z�[�W�A�h���X��)
	Sint32 sDummy1;	//
	Sint32 sDummy2;	//

} StMsgFmt;

//-----------------------------------------------------------

typedef struct StMsgJumpTbl {
	Sint32 sJumpID;
	Sint32 sJumpPos;

} StMsgJumpTbl;


class CMsgManager
{
public:
	enum {
		enWaitTime = 120,
		enKaigyoKode = -1,
	};


	CMsgManager()
	{
		m_pStMsgFmt  = NULL;
		m_pStJumpTbl = NULL;
		m_pKanjiTbl  = NULL;
		m_pString    = NULL;

		m_sScrNum   = 0;
		m_sJmpNum   = 0;
		m_sKanjiNum = 0;
		m_sFaceNum  = 0;
		m_sMsgNum = 0;
		Reset();
//		m_sCurrent = 0;
//		m_bFreeze = gxFalse;
//		m_bButtonWait = gxFalse;
//		m_sWaitTime = 0;
//		m_bRun = gxFalse;
//		m_sLastOpeCode = enCodeFinish;
//		RetStr[0] = 0;
	}

	~CMsgManager()
	{
		if( m_pStMsgFmt ) free(m_pStMsgFmt);
		if( m_pStJumpTbl) free(m_pStJumpTbl);
		if( m_pKanjiTbl ) free(m_pKanjiTbl);
		if( m_pString   ) free(m_pString);
	}

	void Load(char* pFileName);
	void LoadMemory(char* pFileName);
	gxBool Run(Sint32 num);
	gxBool Execute();
	char*  GetMessage();

	gxBool IsWaitTime();
	gxBool IsWaitButton();
	gxBool IsFreeze();
	void   FreezeCancel()
	{
		m_bFreeze = gxFalse;
	}

	Sint32 GetMessageMax()
	{
		return m_sJmpNum;
	}
	Sint32 GetMessageIndex(Sint32 n)
	{
		if( n >= m_sJmpNum ) n = 0;
		if( n <= 0         ) n = 0;
		return m_pStJumpTbl[n].sJumpID;
	}

	void Reset()
	{
		m_sWaitTime = 0;
		m_bRun = gxFalse;
		m_sLastOpeCode = enCodeFinish;
		m_bButtonWait = gxFalse;
		RetStr[0] = 0;
		m_sCurrent = 0;
		m_bFreeze = gxFalse;
	}

	//���b�Z�[�W��摗�肷��
	gxBool Prev();

	Sint32 GetKanjiIndex( Sint32 sMojiNum );
	Sint32 GetLen();

	Sint32 GetFaceIndex()
	{
		return m_sFaceNum;
	}

	Sint32 GetStatus()
	{
		return m_sLastOpeCode;
	}

	void Printf(Sint32 sx, Sint32 y , Sint32 z , Sint32 sCol , char* pMsg );
	Sint32 GetKanjiPos( Sint32 sIndex );
	Sint32 GetKanjiWidth( Sint32 sIndex );

private:

	//-------------------------------
	//��������p
	//-------------------------------
	gxBool m_bFreeze;
	gxBool m_bButtonWait;
	Sint32 m_sWaitTime;

	//�X�N���v�g�|�C���^
	Sint32 m_sCurrent;

	//��ԍ�
	Sint32 m_sFaceNum;

	//���b�Z�[�W�|�C���^
	Sint32 m_sMsgPos[2];
	Sint32 m_sMsgNum;

	//�X�N���v�g�f�[�^
	Sint32    m_sScrNum;
	StMsgFmt *m_pStMsgFmt;

	//�W�����v�e�[�u��
	Sint32        m_sJmpNum;
	StMsgJumpTbl *m_pStJumpTbl;

	//�����e�[�u��
	Sint32  m_sKanjiNum;
	Sint16 *m_pKanjiTbl;

	//������e�[�u��
	char* m_pString;
	char  RetStr[1024];

	//-------------------------------

	gxBool m_bRun;
	Sint32 m_sLastOpeCode;

	Sint32 GetKanjiIndex(char *p);

};


