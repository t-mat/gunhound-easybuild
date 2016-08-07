//--------------------------------------------------
//
// gxPadManager.h
//
//--------------------------------------------------

class CPadManager
{
public:
	enum {
		enJoypadButtonNum = BTN_MAX,
		enTouchMax        = 0x10,	//16点までタッチ対応
	};

	enum {
		enIDKeyboard   = 0x0000,
		enIDMouse      = 0x0100,
		enIDTouch      = 0x0110,
		enIDJoypad1    = enIDTouch + enTouchMax,

		enIDJoypad2    = enIDJoypad1 + enJoypadButtonNum,//0x0140,
		enIDJoypad3    = enIDJoypad2 + enJoypadButtonNum,//0x0140,
		enIDJoypad4    = enIDJoypad3 + enJoypadButtonNum,//0x0140,
		enIDJoypad5    = enIDJoypad4 + enJoypadButtonNum,//0x0140,

		enIDMax        = enIDJoypad5 + enJoypadButtonNum,//0x01C0,
	};

	CPadManager();
	~CPadManager();

	void Action();

	void SetKeyDown(Uint32 uKey);
	void SetKeyUp(Uint32 uKey);
	void SetMouseButtonUp( Uint32 uLR );
	void SetMouseButtonDown( Uint32 uLR );
	void SetMousePosition( Sint32 x , Sint32 y );
	void SetMouseWheel(Sint32 n);

	StJoyStat* GetJoyStatus(Sint32 n=0)
	{
		return &m_stStatus[n];
	}


	void SetTouchInfo( Uint32 uId , gxBool bPush , Sint32 x , Sint32 y );	//ユニークID,タッチ位置x , タッチ位置y
	void SetPadInfo( Sint32 sPlayer , Uint32 button );
	void SetAnalogInfo( Float32 *pAnalogArry );

	Uint8 GetKeyBoardStatus( Uint32 n );

	Sint32 GetDeviceNum()
	{
		return m_sDeviceNum;
	}

	void SetDeviceNum(Sint32 sNum)
	{
		//使用できるジョイパッドデバイスの数を記録しておく（念のため）
		m_sDeviceNum = sNum;
	}

	StTouch* GetTouchStatus( Sint32 n );

	//振動の設定を返す

	gxBool GetMotorStat( Sint32 id , Sint32 motorID , Float32 *fRatio )
	{
		if( m_Motor[ id ][ motorID ].frm == 0 )
		{
			*fRatio = 0.0f;
			return gxFalse;
		}

		*fRatio =  m_Motor[ id ][ motorID ].fRatio;

		return gxTrue;
	}

	void SetRumble(Sint32 playerID , Sint32 motorID, Float32 fRatio , Sint32 frm);

	SINGLETON_DECLARE( CPadManager );

private:

	void MixingKeyboardStat();
	void SetDirectAccessBit();
	void SetCurrentStatus();

	//指定IDのタッチデバイスインデックスを返す
	Sint32 getTouchDeviceIndex( Uint32 n );

	Uint8 m_uButton[enIDMax];
	Uint8 m_uCurrent[enIDMax];
	Uint8 m_uRepeatCnt[enIDMax];
	Uint8 m_uCurStatus[enIDMax];
	Uint8 m_uDClickCnt[enIDMax];

	StJoyStat m_stStatus[ PLAYER_MAX ];
	Uint32 m_JoyButton[ PLAYER_MAX ];

	Sint32  temp_mx,temp_my,temp_mz;
	Float32 temp_axis[32];


	Sint32 m_sDeviceNum;

	typedef struct StTouchStat
	{
		Sint32 x,y;		//現在座標
		Uint32 uStat;	//入力状態( repeat , dcl , trg push )

		Uint32 uId;		//デバイスID
		gxBool bPush;	//デバイス入力状態( ON / OFF )

		void Reset()
		{
			uId = 0xffffffff;
			bPush = gxFalse;
			x = y = 0;
			uStat = 0;
		}

	} StTouchStat;

	StTouchStat m_stTouchStat[ enTouchMax ];
	StTouch m_stTouch[ enTouchMax ];

	//振動
	//Float32 m_sMotor1[PLAYER_MAX],m_sMotor2[PLAYER_MAX];
	typedef struct StRumble {
		StRumble()
		{
			frm    = 0;
			fRatio = 0;
		}

		Sint32  frm;
		Float32 fRatio;
	} StRumble;

	StRumble m_Motor[PLAYER_MAX][2];

};


enum {
	//androidOS & iOS
	TOUCH_iCBK1  = 1,
	TOUCH_iCBK2  = 2,
	TOUCH_iCBK3  = 3,
	TOUCH_iCBK4  = 4,
	TOUCH_iCBK5  = 5,
	TOUCH_iCBK6  = 8,
	TOUCH_iCBK7  = 7,
	TOUCH_iCBK8  = 9,
	TOUCH_iCBK9  = 13,
	TOUCH_iCBK10 = 15,

	TOUCH_aCBK1  = 0,
	TOUCH_aCBK2  = 2,
	TOUCH_aCBK3  = 3,
	TOUCH_aCBK4  = 4,
	TOUCH_aCBK5  = 5,
	TOUCH_aCBK6  = 9,
	TOUCH_aCBK7  = 13,
	TOUCH_aCBK8  = 15,
	TOUCH_aCBK9  = 17,
	TOUCH_aCBK10 = 19,
};






class CGXPadConfig
{
public:
	CGXPadConfig()
	{
		m_Pos.x = WINDOW_W/2;
		m_Pos.x = WINDOW_H/2;

		m_sBtnIndex = 0;
		m_sConfigSeq = 0;
		m_sSelectPad = 0;

		m_bConfigFinish = gxFalse;
	}

	~CGXPadConfig()
	{

	}

	typedef struct StJoyConfig {
		Uint32 uButton;
		Uint32 uPlayer;
		gxChar Name[128];
	} StJoyConfig;

	void Init()
	{
		m_sConfigSeq = 0;
		m_sSelectPad = 0;
		m_bConfigFinish = gxFalse;
	}

	void Action();

	void Draw();

	gxBool IsConfigFinish()
	{
		return m_bConfigFinish;
	}

	SINGLETON_DECLARE( CGXPadConfig );

private:

	void drawController( Sint32 wx , Sint32 wy , Uint32 button , Sint32 padDraw )
	{
		Uint32 argb[16]={
			0xff808080,0xff808080,0xff808080,0xff808080,0xff808080,0xff808080,0xff808080,0xff808080,
			0xff808080,0xff808080,0xff808080,0xff808080,0xff808080,0xff808080,0xff808080,0xff808080,
		};

		for( Sint32 ii=0; ii<16; ii++ )
		{
			if( button&(0x01<<ii) )
			{
				argb[ ii ] = 0xffffff00;
			}
		}

		Sint32 ax,ay,az;
		Sint32 jx,jy;

		ax = 0;
		ay = 0;
		az = 255;

		jx = wx -96+ax;
		jy = wy +ay;

		Sint32 bx,by;

		//Base
		if( padDraw )
		{
			gxLib::DrawBox( jx - 32 , jy -48 , jx+224 , jy+32 , az , gxTrue  , ATR_DFLT , 0xffC08080 );
		}
		gxLib::DrawBox( jx - 32 , jy -48 , jx+224 , jy+32 , az , gxFalse  , ATR_DFLT , 0xffffffff );

		//lever

		bx = jx + 0;	by = jy - 16;
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxTrue   , ATR_DFLT , argb[0] );
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxFalse  , ATR_DFLT , 0xffffffff );

		bx = jx + 16;	by = jy + 0;
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxTrue   , ATR_DFLT , argb[1] );
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxFalse  , ATR_DFLT , 0xffffffff );

		bx = jx + 0;	by = jy + 16;
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxTrue   , ATR_DFLT , argb[2] );
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxFalse  , ATR_DFLT , 0xffffffff );

		bx = jx - 16;	by = jy + 0;
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxTrue   , ATR_DFLT , argb[3] );
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxFalse  , ATR_DFLT , 0xffffffff );

		//6btn

		bx = jx + 112 + 0*32;
		by = jy + 0  + 1*16;
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxTrue   , ATR_DFLT , argb[4] );
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxFalse  , ATR_DFLT , 0xffffffff );

		bx = jx + 112 + 1*32;
		by = jy + 0  + 1*16;
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxTrue   , ATR_DFLT , argb[5] );
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxFalse  , ATR_DFLT , 0xffffffff );

		bx = jx + 112 + 2*32;
		by = jy + 0  + 1*16;
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxTrue   , ATR_DFLT , argb[6] );
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxFalse  , ATR_DFLT , 0xffffffff );

		bx = jx + 116 + 0*32;
		by = jy + 0  - 1*16;
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxTrue   , ATR_DFLT , argb[7] );
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxFalse  , ATR_DFLT , 0xffffffff );

		bx = jx + 116 + 1*32;
		by = jy + 0  - 1*16;
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxTrue   , ATR_DFLT , argb[8] );
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxFalse  , ATR_DFLT , 0xffffffff );

		bx = jx + 116 + 2*32;
		by = jy + 0  - 1*16;
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxTrue   , ATR_DFLT , argb[9] );
		gxLib::DrawBox( bx - 8 , by -8 , bx+8 , by+8 , az , gxFalse  , ATR_DFLT , 0xffffffff );

		//L1 / R1

		bx = jx - 64 + 1*32;
		by = jy + 16  - 80;
		gxLib::DrawBox( bx - 12 , by -6 , bx+16 , by+6 , az , gxTrue   , ATR_DFLT , argb[10] );
		gxLib::DrawBox( bx - 12 , by -6 , bx+16 , by+6 , az , gxFalse  , ATR_DFLT , 0xffffffff );

		bx = jx + 248 - 1*32;
		by = jy + 16  + -80;
		gxLib::DrawBox( bx - 12 , by -6 , bx+16 , by+6 , az , gxTrue   , ATR_DFLT , argb[11] );
		gxLib::DrawBox( bx - 12 , by -6 , bx+16 , by+6 , az , gxFalse  , ATR_DFLT , 0xffffffff );

		//L2 / R2

		bx = jx - 32 + 1*8;
		by = jy + 0  - 2*16;
		gxLib::DrawBox( bx - 6 , by -16 , bx+10 , by+16 , az , gxTrue   , ATR_DFLT , argb[12] );
		gxLib::DrawBox( bx - 6 , by -16 , bx+10 , by+16 , az , gxFalse  , ATR_DFLT , 0xffffffff );

		bx = jx + 220 - 1*8;
		by = jy + 0  + -2*16;
		gxLib::DrawBox( bx - 6 , by -16 , bx+10 , by+16 , az , gxTrue   , ATR_DFLT , argb[13] );
		gxLib::DrawBox( bx - 6 , by -16 , bx+10 , by+16 , az , gxFalse  , ATR_DFLT , 0xffffffff );

		//START / SELECT

		bx = jx + 48 + 0*32;
		by = jy + 0  + 0*16;
		gxLib::DrawBox( bx - 8 , by -4 , bx+8 , by+4 , az , gxTrue   , ATR_DFLT , argb[14] );
		gxLib::DrawBox( bx - 8 , by -4 , bx+8 , by+4 , az , gxFalse  , ATR_DFLT , 0xffffffff );

		bx = jx + 80 + 0*32;
		by = jy + 0  + 0*16;
		gxLib::DrawBox( bx - 8 , by -4 , bx+8 , by+4 , az , gxTrue   , ATR_DFLT , argb[15] );
		gxLib::DrawBox( bx - 8 , by -4 , bx+8 , by+4 , az , gxFalse  , ATR_DFLT , 0xffffffff );

	}

	static StJoyConfig CGXPadConfig::btnTbl[];

	Sint32 m_sBtnIndex;
	Sint32 m_sConfigSeq;
	Sint32 m_sSelectPad;

	gxPos m_Pos;

	gxBool m_bConfigFinish;
};

