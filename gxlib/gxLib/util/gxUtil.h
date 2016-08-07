//--------------------------------------------------
//
// gxUtil.cpp
// 便利な関数群
//
//--------------------------------------------------

//#include <gxLib/Util/CCollisionManager.h>
//#include <gxLib/Util/CActManager.h>

class gxUtil
{
public:
	gxUtil()
	{
	}

	~gxUtil()
	{
	}

	static Uint32 atox( gxChar *p );
	static Uint32 CalcCrc32( void *dat, int siz );
	static void han2zen( gxChar *pSrc , gxChar *pDst );

	static gxBool GetFileNameWithoutPath(gxChar *in,gxChar *out);
	static gxBool GetPath( gxChar *in , gxChar *out );
	static gxBool GetExt ( gxChar *in , gxChar* out );
	static gxBool GetFileNameWithoutExt(gxChar *in , gxChar *out);

	static void RotationPoint( gxPoint *pPt, Float32 fRot );
	static void StrUpr( gxChar* pStr );
	static void MemSet( void* pMem , Uint8 val, Uint32 sz );
	static void MemCpy( void* pDst , void *pSrc , Uint32 sz );

	//自由変形できる分割スプライト

	static Uint32 PutFreeSprite(
		Sint32 x,Sint32 y,
		Sint32 prio,
		gxVector2D *pUV,
		Sint32 tpg,
		Sint32 u,Sint32 v , Sint32 w , Sint32 h,
		Sint32 divNum = 3,
		Uint32  atr = ATR_DFLT,
		Uint32  argb = ARGB_DFLT,
		Float32 fRot = 0.0f,
		Float32 fx = 1.0f,
		Float32 fy = 1.0f
		);

	//多角形を描画します
	static Uint32 DrawPolygon(
			Sint32 num,
			gxVector2D *pXY,
			gxVector2D *pUV,
			Uint32 *argb,
			Sint32 prio,
			Uint32 atr ,
			Sint32 page,
			gxBool bFill );

	//メモリー残量を返します
	static Uint32 GetMemoryRemain( Uint32* uUse = 0x00, Uint32* uTotal = 0x00 , Uint32* uMax = 0x00 );

	//処理落ちを検出します
	static gxBool IsOverProc();

/*
	// ファイルを暗号化する
	static Uint8* FileEncryption( Uint8* pSrc, Uint32 uSize, Uint32* encodeSize );
	// ファイルを復号化する
	static Uint8* FileDecryption( Uint8* pSrc, Uint32 uSize, Uint32* decodeSize );

	// メモリ上のLzma圧縮されたデータを展開する
	static Uint8* DecodeArchiveFile( Uint8* pSrc, Uint32 uSize, Uint32* outLen );
*/


private:

};

int     gxGetHomingDirection(Sint32 my_angle , Sint32 target_angle);
Float32 gxGetHomingDirection(Float32 my_angle,Float32 target_angle);

#define ISKANJI(c)			((Uint8)(c) >= 0x81 && ((Uint8)(c) <= 0x9F || ((Uint8)(c) >= 0xE0 && (Uint8)(c) <= 0xFC)))
#define ISKANJI2(c) 		((Uint8)(c) >= 0x40 && (Uint8)(c) <= 0xfc && (c) != 0x7f)

class CircleCircle
{
public:
	typedef struct StCircle {
		//円の定義用
		Float32 x,y,r;
		StCircle()
		{
			x = y = 0;
			r = 1;
		}
	} StCircle;

	CircleCircle();
	~CircleCircle();

	void SetCircle( Sint32 id , Sint32 x , Sint32 y , Sint32 r )
	{
		//円を定義する
		m_Circle[id].x = (Float32)x;
		m_Circle[id].y = (Float32)y;
		m_Circle[id].r = (Float32)r;
	}

	void SetNear( Sint32 x , Sint32 y )
	{
		//近い座標を設定する
		m_Near.x = (Sint32)(x);
		m_Near.y = (Sint32)(y);
	}

	gxPos* GetKouten( Sint32 sIndex = 0 )
	{
		//交点座標を返す
		return &m_Kouten[sIndex];
	}

	Sint32 Calc();
	Sint32 GetNearest()
	{
		return m_sNearest;
	}
private:

	gxPos    m_Near;		//交点が２つある場合に近い点を探す際の参照する座標
	Sint32   m_sNearest;	//交点１、２の最も近い点のインデックス
	StCircle m_Circle[2];	//円１、２
	gxPos    m_Kouten[2];	//交点１，２

};
