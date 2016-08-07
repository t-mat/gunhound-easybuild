//----------------------------------------------
//シューティングゲーム
//shooting.h
//----------------------------------------------

//WIN32
typedef signed long Int32;
typedef unsigned long UInt32;
typedef signed int Int16;
typedef unsigned int  UInt16;
typedef signed char Int8;
typedef unsigned char UInt8;
typedef void* FormPtr;
typedef void* EventPtr;
typedef void* BitmapPtr;

//#define MemSet(a,b,c)  memset(a,c,b)

typedef struct StSpriteDef
{
	//元ビットマップの
	//座標位置	ＵＶ(8x8キャラ単位）
	//サイズ　	ＷＨ(8x8キャラ単位）
	//真中位置　ＣＸ，ＣＹ(1ドット単位）
	Sint32 tpg,u,v,w,h,cx,cy;

} StSpriteDef;

extern unsigned char sample[];
extern StSpriteDef SprVisor[];

void Visorizer(void);
void VisorizerInit(void);

class ShootSub
{
public:
	ShootSub(){}
	~ShootSub(){}

	static void SoundLoad(Sint32 n , char *name )
	{
		gxLib::LoadAudio( n , name );
	}

	static void SoundPlay(Sint32 n)
	{
		if( n == 0 )
		{
			gxLib::PlayAudio( n , gxTrue );
		}
		else
		{
			gxLib::PlayAudio( n );
		}
		if( n == 3 )
		{
			gxLib::SetAudioVolume( n , 1.f );
		}
		else
		{
			gxLib::SetAudioVolume( n , 0.95f );
		}
	}

	static void PutSprite(int px, int py, int sprno,int prio,int flip)
	{
		PutSpriteVisor(px,py,sprno,prio,ATR_DFLT,0xffffffff,1);
	}

	static void PutSpriteVisor(int x,int y,int spr,int prio,int atr=ATR_DFLT , unsigned long col=ARGB_DFLT,float ras=0)
	{

		if(ras<1) ras=1;

		if( col&0xff000000 ) col |= 0xff000000;
		else				 col &= 0x00ffffff;

		gxLib::PutSprite(
			//転送先設定
			 x*2,y*2+8,prio,
			//転送元ページ
			 SprVisor[spr].tpg,				
			 SprVisor[spr].u*8, 	SprVisor[spr].v*8,		SprVisor[spr].w*8, SprVisor[spr].h*8,		//UVWH
			//センター位置
			 SprVisor[spr].cx,	SprVisor[spr].cy
			//オプション(デフォルト設定あり)
			 ,atr
			 ,col
			 ,0
			 ,ras*2
			 ,ras*2
			);
		}

		static void SetBgColor(Uint32 argb )
		{
			gxLib::SetBgColor( argb );
		}
private:

};
