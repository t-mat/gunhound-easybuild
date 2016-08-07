//============================================================================
//
//�f�f�`�R���t�B�O
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

//�X�i�b�v�V���b�g�l�[��
#define SNAPSHOT_FILENAME "gunhound"

//���ݒ�t�@�C����
#define _FILENAME_ENVIRONMENT_ "gunhound.sav"

#define _URL_DOCUMENT_ "http://www.garuru.co.jp/manual/index.html"
#define _URL_TRIAL_    "http://www.garuru.co.jp/"

//�_�b�V���{�[�h�̎g�p��
#define	USE_DASHBOARD

//---------------------------------------------------
//Window�T�C�Y�ݒ�i�ʏ�320x240 : 480x360 -> HIDEF�� 480x264[PSP]�j	iPhone 480x320
//---------------------------------------------------
#define	SCR_W	(480)
#define	SCR_H	(360)

#define PRIO_MAX	(300)		//�v���C�I���e�B�i-n �` +n�j
//---------------------------------------------------

//�F���\�ȃp�b�h�̐�
#define JOYMAX (16)

//�R���g���[���[�����蓖�Ă�v���C���[��
enum {
	enPlayer01,
	enPlayer02,
	enPlayerMax,
};

//---------------------------------------------------
//�����ݒ�
//---------------------------------------------------

//�_�b�V���{�[�h�̃v���C�I���e�B
enum {
	enPrioDashBoardMain   = PRIO_MAX+5,
	enPrioDashBoardWindow = PRIO_MAX+6,
	enPrioDashBoardText   = PRIO_MAX+7,
	enPrioHidefBoard      = PRIO_MAX+8,
	enMaxPriority         = PRIO_MAX+10,
};

enum {
	//�_�b�V���{�[�h�p�O���t�B�b�N�o�b�t�@
	enTpgDashBoard    = 16*0,	//1�y�[�W��
	enTexPageSysLogo  = 16*3,	//4�y�[�W��

	//�_�b�V���{�[�h�p�T�E���h�o�b�t�@
	enSoundMenuOpen=0,
	enSoundMenuSel,
	enSoundMenuDec,
	enSoundMenuCan,
	enSoundMenuMax,
//	enTexPageDashBoard    = 16*0,//enTexturePage00+0,
};

//�z�C�[���̉�]���E(-n �` +n)
#define WHEEL_MAX (12)

// ���_�t�H�[�}�b�g
typedef struct VECTOR
{
	float	x,y,z;
} VECTOR;

// �s��t�H�[�}�b�g
typedef struct MATRIX {
	float	_11,_12,_13,_14;
	float	_21,_22,_23,_24;
	float	_31,_32,_33,_34;
	float	_41,_42,_43,_44;
}MATRIX;

//�|���S���O���t�B�b�N�X
enum {
	GRA_PIXEL=0,	//�_
	GRA_BOXF,		//�h��Ԃ��l�p�`
	GRA_BOX,		//�l�p�`
	GRA_LINE,		//��
	GRA_TRIANGLE,	//�O�p�`
	GRA_SPRITE,		//�ό`�l�p�`
	GRA_SPRITE3D,	//�ό`�l�p�`�Q
};

//�O���t�B�b�N�p���_�t�H�[�}�b�g
typedef struct GRA_POS_T
{
	float x;
	float y;
	float z;
	unsigned int col;
}GRA_POS_T;


//3D�v�Z����

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
//�R���g���[���[
//---------------------------------------------------------------------------

//GetJoypadDir�ŕԂ������擾���[�h
#define JOY_PUSH (0)
#define JOY_TRIG (1)

typedef struct StJoyVector {
	//�W���C�p�b�h�֘A�̃X�N���[�����W�ۑ��p
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
	StJoyVector drag1;	//�h���b�O�J�n�ʒu
	StJoyVector drag2;	//�h���b�O�I���ʒu
	StJoyVector scr;	//�X�N���[���|�W�V����

	Sint16	x, y;		/* �����X�e�B�b�N�̕ϗ�. x,y �� -0x8000�`0�`0x7fff �ɂȂ� ��mouse�̂Ƃ��͍��W*/
	Sint16	cx, cy;		/* mm�n��x,y�����߂邽�߂̕ϐ�. ��mouse�̂Ƃ��͌��ݏ������̍��W */
	long	dw,dh;		/* �V						*/
	char name[0xff];
	Sint32 stick_num;
	Sint32 repeat[20];
} JOYSTICK_T;

typedef struct ButtonConfig_t {
	//-----------------------------------------------
	//�Z�[�u�f�[�^�ɕۑ�����p�b�h���
	//-----------------------------------------------
	char bUseController;			//���������p�b�h���g�p���邩�H
	char enable[16][16];			//�{�^���̎g�p��
	char useDevice[enPlayerMax];		//�g�p����f�o�C�X�h�c
	char assign[enPlayerMax][32];	//�A�T�C�����ꂽ�{�^���ԍ�(GAMINGDEVICE_BTNID)
} ButtonConfig_t;

extern JOYSTICK_T Joy[enPlayerMax];

//-------------------------------------------------------
//�Q�c�֘A
//-------------------------------------------------------
//�e�N�X�`���t�@�C���ǂݍ��݁iBMP or TGA�j
gxBool LoadTexture(int pg,gxChar *pBmpfilename,int col=0xff00ff00);
gxBool ReadTexture(int pg, Uint8 *pMemory,Uint32 sz,int col=0xff00ff00);
gxBool UploadTexture(bool force_reading=false);

//�X�v���C�g�\��
int PutSpriteDX(
	int x,		int y,	int prio,
	int page,	int u, 	int v,	int w,	int h,
	 //�I�v�V����
	 int cx=0,	int cy=0,
	 int atr=ATR_ALPHA_NML,	 unsigned long col=ARGB_DFLT,
	 float rot=0, float scl_x=1.0f,float scl_y=1.0f
	);

//�|���S���O���t�B�b�N�\��
int PutGraphics(
	 int type,
	 int x,	int y,	int prio,
	 GRA_POS_T *pos,
	 //�I�v�V����
	  int atr=ATR_DEFAULT,
	  float rot=0.0f,
	  float scl_x=1.0f,float scl_y=1.0f
	 );

//-------------------------------------------------------
//�T�E���h�i�v�`�u�A�l�h�c�h�j
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
//�s�񑀍�
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
//�V�X�e���t�H���g�\��
//-------------------------------------------------------
void msg_printf(int x,int y,int pr,char *msg,...);
void msg_printfwithCol(int x,int y,int pr,unsigned int col,char *msg,...);
int PutStringDX(int x,int y,int prio,char *msg,unsigned long col=ARGB_DFLT);


//-------------------------------------------------------
//�f�o�b�OWindow�p
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
		Sint32 free[enSaveDataFreeArea];		//�ėp
		ButtonConfig_t BtnConfig;

	} SAVEDATA_T;

	ghLib()
	{

	}
	~ghLib()
	{

	}

	//�R���t�B�O�֘A
	static gxBool SaveConfig();
	static gxBool LoadConfig();
	static gxBool SetConfigDefault();

	static gxBool IsFullScreen();
	static gxBool ChangeWindowMode( gxBool bFlag );

	//�p�b�h�֘A
	static Sint32 GetJoypadDir(int pl,int mode);
	static void   SetRumble(int player,int time,int x,int y);
	static Sint32 GetMouseWheelNum(Sint32 sMax = WHEEL_MAX );

	static Uint32 GetTextureARGB(int pg, int x,int y);
	static Uint32 GetTexturePalletIndex(int pg, int x,int y);

	static char*  GetLocalFilePath();
	static void   SetLocalFilePath(char *p);
	static Sint32& Value(Sint32 n);
	static Uint32 SetTextureARGB(int pg, int x,int y , Uint32 argb );

	//�T�E���h�֘A
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

	//�R���g���[���[�ݒ�̂��܍��킹�p

	static void AdjustControl();

private:

	static void SetDefaultPadCongig();
};

extern ghLib::SAVEDATA_T g_StSaveData;

#ifndef ZERO
#define ZERO (0.0f)
#endif


#ifdef	USE_DASHBOARD
		//�_�b�V���{�[�h�g��
	enum {
		enTextureStartPage = enTpgDashBoard+4,
		enSoundStartPage   = enSoundMenuMax,
	};

#else	//USE_DASHBOARD
		//�_�b�V���{�[�h�g��Ȃ�
	enum {
		enTextureStartPage = 0,
		enSoundStartPage   = 0,
	};

#endif	//USE_DASHBOARD


#pragma pack (pop)

#endif	//_GGA2_H_
