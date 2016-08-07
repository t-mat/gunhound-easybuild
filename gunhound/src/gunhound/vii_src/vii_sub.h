//-----------------------------------------
//汎用サブルーチン
//-----------------------------------------


const ghPos* GetTargetPlayer();
void  SetTargetPlayerPosX( Sint32 x );
void  SetTargetPlayerPosY( Sint32 y );
void* GetTargetObjPosAddr();

void OpenSecretWeapon();
void PutKanjiMessage(Sint32 x,Sint32 y,Sint32 z,char *msg,Sint32 y1=0,Sint32 y2=240);
Sint32* argset(Sint32 a1,Sint32 a2=0,Sint32 a3=0,Sint32 a4=0,Sint32 a5=0,Sint32 a6=0,Sint32 a7=0,Sint32 a8=0,Sint32 a9=0,Sint32 a10=0);
void PutSystemWord(Sint32 x,Sint32 y,char *p,...);
void set_item_stat();
void get_item(Sint32 type);
Sint32 get_valken_dist(Sint32 x,Sint32 y);
Sint32 get_valken_rot(Sint32 x,Sint32 y);


class viiDraw
{
public:

	static void Sprdmg(gxBool damage,gxSprite *p,Sint32 x,Sint32 y,Sint32 prio,Sint32 atr=ATR_DEFAULT,unsigned long col=ARGB_DFLT,float scl=1.0f,Sint32 rot=0);
	static void Spr_dmg_NoScrl(gxBool damage,gxSprite *p,Sint32 x,Sint32 y,Sint32 prio,Sint32 atr=ATR_DEFAULT,unsigned long col=ARGB_DFLT,float scl=1.0f,Sint32 rot=0);
	static void Sprite(gxSprite *p,Sint32 x,Sint32 y,Sint32 prio,Sint32 atr=ATR_DEFAULT,unsigned long col=ARGB_DFLT,float scl=1.0f,Sint32 rot=0);
	static void Spr_NoScrl(gxSprite *p,Sint32 x,Sint32 y,Sint32 prio,Sint32 atr=ATR_DEFAULT,unsigned long col=ARGB_DFLT,float scl=1.0f,Sint32 rot=0);
	static void Box(Sint32 x1,Sint32 y1,Sint32 x2,Sint32 y2,Sint32 z,gxBool flag,Sint32 col=ARGB_DFLT,Sint32 atr=ATR_DEFAULT);
	static void BoxScrl_s(Sint32 x1,Sint32 y1,Sint32 x2,Sint32 y2,Sint32 z,gxBool flag,Sint32 col,Sint32 atr);
	static void Line(int x1,int y1,int x2,int y2,int z,int atr=ATR_DFLT,int col=ARGB_DFLT);
	static void Line_s(int x1,int y1,int x2,int y2,int z,int atr=ATR_DFLT,int col=ARGB_DFLT);

	static void Pixel(Sint32 x1,Sint32 y1,Sint32 z,Sint32 col,Sint32 atr);
	static void Grad(Sint32 x1,Sint32 y1,Sint32 x2,Sint32 y2,Sint32 z,Sint32 atr,Sint32 col1,Sint32 co2,Sint32 co3,Sint32 col4);
	static void printf(Sint32 x,Sint32 y , Sint32 sCol , char *p,...);
	static void sml_printf(Sint32 x,Sint32 y , Sint32 sCol , char *p,...);
	static void big_printf(Sint32 x,Sint32 y , Sint32 sCol , char *p,...);
	static void big_printf2(Sint32 x,Sint32 y , Sint32 sPrio , Sint32 sCol , char *p,...);

private:
	static void GetBigFont( Sint32 n ,ghPos *p );

};


class viiMath
{
public:
	viiMath();
	~viiMath();

	static Float32 Cos(Sint32 deg);
	static Float32 Sin(Sint32 deg);
	static Sint32 Cos100(Sint32 n);
	static Sint32 Sin100(Sint32 n);
	static Sint32 Atan2_100(Sint32 y,Sint32 x);
	static Float32 Atan2d(Sint32 y,Sint32 x);
	static Sint32 GetSyougen(Sint32 x1,Sint32 y1,Sint32 x2,Sint32 y2);
	static Sint32 GetTargetAngle(Sint32 angle,Sint32 target_angle);
	static Sint32 LineLogic(Sint32 x0,Sint32 y0,Sint32 x1,Sint32 y1,Sint32 n,Sint32 &xx,Sint32 &yy);
	static Sint32 Dist(Sint32 x , Sint32 y);
	static Sint32 Dist(Sint32 x1 , Sint32 y1 ,Sint32 x2, Sint32 y2);
	static Float32 Sqrt( Float32 val );
	
private:

};


class viiMus
{
public:
	viiMus()
	{
		for(Sint32 ii=0;ii<enSoundMax;ii++)
		{
			m_sLoopSoundCnt[ii] = 0;
			m_fFadeSoundAdd[ii] = ZERO;
			m_fFadeSoundAvg[ii] = ZERO;
		}
		s_pInstance = this;
	}

	~viiMus()
	{

	}

	static viiMus* GetInstance()
	{
		if( s_pInstance == NULL )
		{
			s_pInstance = new viiMus();
		}

		return s_pInstance;
	}

	static void Destroy()
	{
		if(s_pInstance) delete s_pInstance;
		s_pInstance = NULL;
	}

	void Action();

	static	gxBool ReadBGM(long no, Uint8 *pData , Uint32 uSize );
	static	void   PlayBGM(Sint32 no ,Sint32 bgm_no );//,Sint32 loop,Sint32 vol=0,Sint32 stop=1);
	static	void   StopBGM(Sint32 no , Sint32 sFrm=0);
	static	gxBool IsPlayBGM(Sint32 page);
	static	void   PlaySound(Sint32 n , gxBool bSilence = gxFalse);
	static  void   PlayLoopSound(Sint32 n,Sint32 sFrm=32);
	static	void   SoundLoad(Sint32 n , char* filename);
//	static	void   SoundLoadFromMemory(Sint32 n , Uint8 *pBuf );

private:
	static viiMus* s_pInstance;
	static Sint32 m_sLoopSoundCnt[enSoundMax];
	static Float32 m_fFadeSoundAdd[enSoundMax];
	static Float32 m_fFadeSoundAvg[enSoundMax];
};


class viiEff
{
public:
	viiEff();
	~viiEff();

	static CEffect* Set(Sint32 type,Sint32 x,Sint32 y,Sint32 *opt=NULL , gxSprite *pSpr = NULL );
	static void SetBombSimple(Sint32 x,Sint32 y,Sint32 num,Sint32 dist);
	static void SetBombEffect(Sint32 x,Sint32 y,Sint32 num,Sint32 dist);
	static void SetBombDeluxe(Sint32 x,Sint32 y,Sint32 num,Sint32 dist);
	static void SetBrokenParts(Sint32 x,Sint32 y,Sint32 n,Sint32 spd=700);

private:


};


class CGXImage;
class viiSub
{
public:
	viiSub();
	~viiSub();

	static Sint32 LoadTexture(Sint32 sPg , CGXImage *p,char* pName , Sint32 sCol=0xff00ff00);
	static Sint32 LoadSound  (Sint32 sPg , CGXImage *p,char* pName );

	static Sint32 GameCounter();
	static Sint32 vSyncTimer();
	static void   SetRumble(Sint32 time,Sint32 mag);
	static Sint32 Rand();
	static Sint32 GetBoundBlock_Player(Sint32 x,Sint32 y);
	static Sint32 GetBoundBlock_Enemy(Sint32 x,Sint32 y);
	static gxBool IsGroundBlock(Sint32 x , Sint32 y);
	static void   OpenSecretWeapon();
	static Sint32 GetRandColor(Sint32 alpha);
	static void   AddScore(Sint32 n);
	static void   SetGravitMove(Sint32 &x,Sint32 &y);
	static void   GetScreenPosition(Sint32 &x,Sint32 &y);
	static gxBool IsScrollOut(Sint32 x,Sint32 y,Sint32 ofx,Sint32 ofy);
	static void   GetIdealScroll_l(Sint32 &sx,Sint32 &sy);
	static void   AdjScroll_l(Sint32 sx , Sint32 sy);
	static void   GetScroll_s( Sint32 &x,Sint32 &y );
	static void   SetScroll_l( Sint32 x,Sint32 y );
	static void   SetBgLayer( Sint32 n,Sint32 x,Sint32 y );
	static void   SetBgDisp(Sint32 n,gxBool flag);
	static void   LimitMoveVH();
	static void   LimitMoveOnlyV();
	static Sint32 SetKakudo(Sint32 sDirection ,Sint32 sKakudo);
	static Sint32 GetMorphingColor( Sint32 per , Sint32 col1 , Sint32 col2 );

	//メニュ－表示専用
	static void   MenuSprite(gxSprite *p,Sint32 x,Sint32 y,Sint32 prio,Sint32 atr=ATR_DEFAULT,unsigned long col=ARGB_DFLT,float scl=1.0f,Sint32 rot=0);
	static void   MenuPrintf(Sint32 x,Sint32 y,Sint32 col,char *msg,...);
	static void   MenuSprite(	int x,		int y,	int prio,	int page,	int u, 	int v,	int w,	int h,	
						 int cx=0,	int cy=0,	 int atr=ATR_ALPHA_NML,	 unsigned long col=ARGB_DFLT,	 float rot=0, float scl_x=1.0f,float scl_y=1.0f	);
	static Sint32 GetPadInputAngle( Sint32 sPad );
	static gxBool MakeEnemy(Sint32 sID , Sint32 x , Sint32 y );


private:

};


class viiDbg
{
public:
	viiDbg();
	~viiDbg();

	static void printf(Sint32 x,Sint32 y,char *msg,...);
	static void printc(Sint32 x,Sint32 y,Uint32 col,char *msg,...);
	static void log(char *msg,...);

private:

};


