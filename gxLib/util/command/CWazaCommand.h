/*!
 @file CWazaCommand.h
 @brief コマンド技によるボタン入力判定ルーチン
 @author ragi
 @date 2009.07.26
*/

//--------------------------------------------------------------------
//1.毎フレームの入力情報をSetLog()で渡してやること
//2.Check()でコマンドをチェックする
//3.うまくできないときはコマンドの優先順位（判定順序）を確認すること。
//--------------------------------------------------------------------
class CWazaCommand
{
public:
	enum {
		enKeyBufferMax = 60*3,
		enKeyTameWaitMax = 4,	//１コマンド4フレームとする
		enKeyValidFrm    = 12,	//１コマンドを追いかける時間
	};

	enum {
		enKeyNone,
		//----------------------
		//なんでも許可
		enKeyEvery,
		enKeyEnd,

		//ボタン押しがなければ許可
		enKeyBtnNone,

		//レバー入れがなければ許可
		enKeyLeverNone,

		//特定のキー入力
		enKeyUp,
		enKeyUpRight,
		enKeyRight,
		enKeyRightDown,
		enKeyDown,
		enKeyDownLeft,
		enKeyLeft,
		enKeyLeftUp,
		enKeyBtnA,
		enKeyBtnB,
		enKeyBtnC,
		enKeyBtnX,
		enKeyBtnY,
		enKeyBtnZ,
		enKeyBtnL1,
		enKeyBtnR1,
		enKeyBtnL2,
		enKeyBtnR2,
		//---------------------
		//その方向を含んでいれば許可
		enKeyIncU,
		enKeyIncR,
		enKeyIncD,
		enKeyIncL,

		//------------------------------------
		//未実装
		//------------------------------------

		//タメ判定許容範囲（小）
		enKeyWaitU,
		enKeyWaitR,
		enKeyWaitD,
		enKeyWaitL,

		//タメ判定許容範囲（大）
		enKeyWaitIncU,
		enKeyWaitIncR,
		enKeyWaitIncD,
		enKeyWaitIncL,

	};

	CWazaCommand(Uint32 uKeyLogMax=enKeyBufferMax)
	{
		//--------------------------------------------
		//コンストラクタ
		//--------------------------------------------
		m_uKeyLogMax = uKeyLogMax;

//		m_pKeyLog = (Sint32*)malloc( m_uKeyLogMax*sizeof(Uint32) );

		m_pKeyLog = new Sint32[ m_uKeyLogMax*sizeof(Uint32) ];

		for(Uint32 ii=0; ii<m_uKeyLogMax; ii++)
		{
			m_pKeyLog[ii] = 0;
		}

		m_pReplayKeyLog = NULL;
		m_uKeyCnt = 0;
		m_bRecord = gxFalse;
		m_bReplay = gxFalse; 
	}

	~CWazaCommand()
	{
		//--------------------------------------------
		//デストラクタ
		//--------------------------------------------
		//free(m_pKeyLog);
		delete[] m_pKeyLog;
	}

	void   SetLog(Sint32 sKey);
	Uint32 GetLog( Uint32 uFrm = 0xffffffff );
	gxBool Check(Uint32 *pCommand,Uint32 uSize,gxBool bReverse = gxFalse);
	void Play(gxChar* pBuffer);
	void Rec();
	void Stop();
private:

	gxBool CheckInput( Uint32 uKey , Uint32 uPush );
	Uint32 ConvertReverseCommand( Uint32 uCmd );

	Sint32* m_pKeyLog;
	Uint32  m_uKeyCnt;
	Uint32  m_uKeyLogMax;
	Uint32* m_pReplayKeyLog;

	gxBool m_bRecord;
	gxBool m_bReplay;
};

