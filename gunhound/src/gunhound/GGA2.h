//============================================================================
//
//ＧＧＡコンフィグ
//written by ragi.
//
//============================================================================

#ifndef _GGA2_H_
#define _GGA2_H_

#include <gxLib.h>

#pragma pack (push , 4)

//#ifdef NDEBUG
//#define _MASTER_VERSION_
//#endif

//スナップショットネーム
#define SNAPSHOT_FILENAME "gunhound"

//環境設定ファイル名
#define _FILENAME_ENVIRONMENT_ "gunhound.sav"

#define _URL_DOCUMENT_ "http://www.garuru.co.jp/manual/index.html"
#define _URL_TRIAL_    "http://www.garuru.co.jp/"

//ダッシュボードの使用可否
#define	USE_DASHBOARD

//---------------------------------------------------
//Windowサイズ設定（通常320x240 : 480x360 -> HIDEF時 480x264[PSP]）	iPhone 480x320
//---------------------------------------------------
#define	SCR_W	(480)
#define	SCR_H	(360)

#define PRIO_MAX	(300)		//プライオリティ（-n ～ +n）
//---------------------------------------------------

//認識可能なパッドの数
#define JOYMAX (16)

//コントローラーを割り当てるプレイヤー数
enum {
	enPlayer01,
	enPlayer02,
	enPlayerMax,
};

//---------------------------------------------------
//自動設定
//---------------------------------------------------

//ダッシュボードのプライオリティ
enum {
	enPrioDashBoardMain   = PRIO_MAX+5,
	enPrioDashBoardWindow = PRIO_MAX+6,
	enPrioDashBoardText   = PRIO_MAX+7,
	enPrioHidefBoard      = PRIO_MAX+8,
	enMaxPriority         = PRIO_MAX+10,
};

enum {
	//ダッシュボード用グラフィックバッファ
	enTpgDashBoard    = 16*0,	//1ページ目
	enTexPageSysLogo  = 16*3,	//4ページ目

	//ダッシュボード用サウンドバッファ
	enSoundMenuOpen=0,
	enSoundMenuSel,
	enSoundMenuDec,
	enSoundMenuCan,
	enSoundMenuMax,
//	enTexPageDashBoard    = 16*0,//enTexturePage00+0,
};

//ホイールの回転限界(-n ～ +n)
#define WHEEL_MAX (12)

// 頂点フォーマット
typedef struct VECTOR
{
	float	x,y,z;
} VECTOR;

// 行列フォーマット
typedef struct MATRIX {
	float	_11,_12,_13,_14;
	float	_21,_22,_23,_24;
	float	_31,_32,_33,_34;
	float	_41,_42,_43,_44;
}MATRIX;

//ポリゴングラフィックス
enum {
	GRA_PIXEL=0,	//点
	GRA_BOXF,		//塗りつぶし四角形
	GRA_BOX,		//四角形
	GRA_LINE,		//線
	GRA_TRIANGLE,	//三角形
	GRA_SPRITE,		//変形四角形
	GRA_SPRITE3D,	//変形四角形２
};

//グラフィック用頂点フォーマット
typedef struct GRA_POS_T
{
	float x;
	float y;
	float z;
	unsigned int col;
}GRA_POS_T;


//3D計算順序

enum{
	ROTATION_XY=0,
	ROTATION_YX,

	ROTATION_YZ,
	ROTATION_ZY,

	ROTATION_ZX,
	ROTATION_XZ,

	ROTATION_END
};

//---------------------------------------------------------------------------
//コントローラー
//---------------------------------------------------------------------------

//GetJoypadDirで返す方向取得モード
#define JOY_PUSH (0)
#define JOY_TRIG (1)

typedef struct StJoyVector {
	//ジョイパッド関連のスクリーン座標保存用
	int x,y;
}StJoyVector;

typedef struct JOYSTICK_T{
	Uint16 psh;
	Uint16 trg;
	Uint16 rep;
	Sint32 analog_x;
	Sint32 analog_y;
	Sint32 analog_z;
	Sint32 analog_rx;
	Sint32 analog_ry;
	Sint32 analog_rz;
	Sint32 mx,my;
	bool dragnow;
	StJoyVector drag1;	//ドラッグ開始位置
	StJoyVector drag2;	//ドラッグ終了位置
	StJoyVector scr;	//スクロールポジション

	Sint16	x, y;		/* 方向スティックの変量. x,y は -0x8000～0～0x7fff になる ※mouseのときは座標*/
	Sint16	cx, cy;		/* mm系でx,yを求めるための変数. ※mouseのときは現在処理中の座標 */
	long	dw,dh;		/* 〃						*/
	char name[0xff];
	Sint32 stick_num;
	Sint32 repeat[20];
} JOYSTICK_T;

typedef struct ButtonConfig_t {
	//-----------------------------------------------
	//セーブデータに保存するパッド情報
	//-----------------------------------------------
	char bUseController;			//そもそもパッドを使用するか？
	char enable[16][16];			//ボタンの使用可否
	char useDevice[enPlayerMax];		//使用するデバイスＩＤ
	char assign[enPlayerMax][32];	//アサインされたボタン番号(GAMINGDEVICE_BTNID)
} ButtonConfig_t;

extern JOYSTICK_T Joy[enPlayerMax];

//-------------------------------------------------------
//２Ｄ関連
//-------------------------------------------------------
//テクスチャファイル読み込み（BMP or TGA）
gxBool LoadTexture(int pg,gxChar *pBmpfilename,int col=0xff00ff00);
gxBool ReadTexture(int pg, Uint8 *pMemory,Uint32 sz,int col=0xff00ff00);
gxBool UploadTexture(bool force_reading=false);

//スプライト表示
int PutSpriteDX(
	int x,		int y,	int prio,
	int page,	int u, 	int v,	int w,	int h,
	 //オプション
	 int cx=0,	int cy=0,
	 int atr=ATR_ALPHA_NML,	 unsigned long col=ARGB_DFLT,
	 float rot=0, float scl_x=1.0f,float scl_y=1.0f
	);

//ポリゴングラフィック表示
int PutGraphics(
	 int type,
	 int x,	int y,	int prio,
	 GRA_POS_T *pos,
	 //オプション
	  int atr=ATR_DEFAULT,
	  float rot=0.0f,
	  float scl_x=1.0f,float scl_y=1.0f
	 );

//-------------------------------------------------------
//サウンド（ＷＡＶ、ＭＩＤＩ）
//-------------------------------------------------------
typedef struct StSoundInfo {
	gxBool bPlay;
	gxBool bLoop;
	gxBool bLoad;
	Uint64 uStart;
	Uint64 uLength;
	Sint32 sVol;

	Uint32 nAvgBytesPerSec;
	Uint32 nSamplesPerSec;
	Uint32 nChannels;
	Uint32 wBitsPerSample;

	StSoundInfo()
	{
		bPlay = gxFalse;
		bLoop = gxFalse;
		bLoad = gxFalse;
		uStart = 0;
		sVol = 65;
		nAvgBytesPerSec = 0;
		nSamplesPerSec = 0;
		nChannels = 0;
		wBitsPerSample = 0;
	}
} StSoundInfo;


//-------------------------------------------------------
//行列操作
//-------------------------------------------------------
void mtxSetUnit(void);
void mtxSetUnit2(MATRIX *m);
void mtxTrans(VECTOR *v);
void mtxRotZ(float r);
void mtxRotX(float r);
void mtxRotY(float r);
void mtxScale(float x, float y, float z);
void mtxAffin2(VECTOR *d, MATRIX *m, VECTOR *s);
void mtxAffin(VECTOR *d, VECTOR *s);
void mtxAffinRot(VECTOR *d, VECTOR *s);

void vctAdd(VECTOR *d, VECTOR *v1, VECTOR *v2);
void vctSub(VECTOR *d, VECTOR *v1, VECTOR *v2);
void vctMul(VECTOR *d, VECTOR *v1, VECTOR *v2);
void vctDiv(VECTOR *d, VECTOR *v1, VECTOR *v2);
void mtxMul(MATRIX *m2,MATRIX *m1);
void TransPers(VECTOR *v);

//-------------------------------------------------------
//システムフォント表示
//-------------------------------------------------------
void msg_printf(int x,int y,int pr,char *msg,...);
void msg_printfwithCol(int x,int y,int pr,unsigned int col,char *msg,...);
int PutStringDX(int x,int y,int prio,char *msg,unsigned long col=ARGB_DFLT);


//-------------------------------------------------------
//デバッグWindow用
//-------------------------------------------------------
void dbg_printf(char *msg,...);

class ghLib
{
public:
	enum {
		enSaveDataFreeArea = 32,
	};

	typedef struct SAVEDATA_T {

		Sint32 windowSize;
		Sint32 version_no;
		Sint8  wideScreen;
		Sint8  FrameSkip;
		Sint32 free[enSaveDataFreeArea];		//汎用
		ButtonConfig_t BtnConfig;

	} SAVEDATA_T;

	ghLib()
	{

	}
	~ghLib()
	{

	}

	//コンフィグ関連
	static gxBool SaveConfig();
	static gxBool LoadConfig();
	static gxBool SetConfigDefault();

	static gxBool IsFullScreen();
	static gxBool ChangeWindowMode( gxBool bFlag );

	//パッド関連
	static Sint32 GetJoypadDir(int pl,int mode);
	static void   SetRumble(int player,int time,int x,int y);
	static Sint32 GetMouseWheelNum(Sint32 sMax = WHEEL_MAX );

	static Uint32 GetTextureARGB(int pg, int x,int y);
	static Uint32 GetTexturePalletIndex(int pg, int x,int y);

	static char*  GetLocalFilePath();
	static void   SetLocalFilePath(char *p);
	static Sint32& Value(Sint32 n);
	static Uint32 SetTextureARGB(int pg, int x,int y , Uint32 argb );

	//サウンド関連
	static int    LoadWaveFile		(long no, char *file);
	static int    ReadWaveData(long no, Uint8* pWaveBuf , Uint32 uSize );
	static void   PlayWave		(Sint32 no,gxBool bLoop=gxFalse,Sint32 vol=65,Uint64 current=0 );
	static void   StopWave		(Sint32 , Sint32 frm=0 );
//	static void   SetMasterVolume	(int vol,int pan);
//	static Uint32 GetPlayPosition(Sint32 no );
//	static Uint32 GetSoundLength(Sint32 no );
//	static StSoundInfo* GetWaveInfo(Sint32 no);
//	static void SetSoundLoopPosition(Uint32 no , Float32 uAvg);
	static void SetSoundVolume(int no , int vol );
	static gxBool IsSoundPlay(int n);

	//コントローラー設定のつじつま合わせ用

	static void AdjustControl();

private:

	static void SetDefaultPadCongig();
};

extern ghLib::SAVEDATA_T g_StSaveData;

#ifndef ZERO
#define ZERO (0.0f)
#endif


#ifdef	USE_DASHBOARD
		//ダッシュボード使う
	enum {
		enTextureStartPage = enTpgDashBoard+4,
		enSoundStartPage   = enSoundMenuMax,
	};

#else	//USE_DASHBOARD
		//ダッシュボード使わない
	enum {
		enTextureStartPage = 0,
		enSoundStartPage   = 0,
	};

#endif	//USE_DASHBOARD


#pragma pack (pop)

#endif	//_GGA2_H_
