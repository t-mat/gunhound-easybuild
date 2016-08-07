//-----------------------------------------------------------------------------------
//targaファイル１枚を管理する
//-----------------------------------------------------------------------------------
#ifdef _CFILETARGA_H_
#else
#define _CFILETARGA_H_


#define HUTTER_SIZE (26)

#pragma pack(push , 1)

class CFileTarga
{
public:

	//	typedef struct stTgaInfo {
	//		/* tgaヘッダ情報 */
	//		Uint16			exHdrSz;		/* ヘッダ後のピクセルデータまでのバイト数? */
	//		gxChar			id;				/* ?t?H?[?}?b?g?̎嵭ﾞ. 0,1,2,3,9,10,11 */
	//		Uint16			clutNo;			/* 色パレットの開始番号 */
	//		Uint16			clutNum;		/* 色パレットの個数 */
	//		gxChar			clutBit;		/* ?F?p???b?g?̃r?b?g?? 15,16,24,32 */
	//		short			x0;				/* 始点x */
	//		short			y0;				/* 始点y 画面左下が原点(0,0)なので注意！ */
	//		short			w;				/* 横幅 */
	//		short			h;				/* 縦幅 */
	//		gxChar			bpp;			/* ?s?N?Z???̃r?b?g??  8,16,24,32 */
	//		gxChar			mode;			/* bit 3..0(4bits) : ????. 0:???Ȃ?. 1:???闍`?̂??ߖ????\. 2:???闍`?????ۑ????ׂ? */
	//										/* 3:通常のα 4:予め乗算されたα. 8:詳細不明. */
	//										/*  (bpp=32のとき:0x08, 24のとき0x00, 16のとき0x01 8のとき0x08,だった) */
	//										/* bit 5,4         : ピクセルの格納順は  0:左下から 1:左上 2:右下  3:右上 */
	//	} stTgaInfo;


	typedef struct stTgaInfo
	{
		/* tgaヘッダ情報 ver 2.0 */
		gxChar	FieldLength;
		gxChar	ColorMapType;			//パレット　なし(0) / あり(1)
		gxChar	ImageType;				//0:なし　1:8bit: 2:24bit 3:Gray 9:index(RLE) 10:24bit(RLE) 11:gray(RLE)
		short	ColorMapIndex;			//// type of image 0=none,1=indexed,2=rgb,3=grey,+8=rle packed;//Low | High	(00)
		short	clutNum;				//Low | High	(0100)
		gxChar	ColorMapSize;			//0x18

		short			x0;				/* 始点x (00)*/
		short			y0;				/* 始点y (00) 画面左下が原点(0,0)なので注意！ */
		short			w;				/* 横幅 */
		short			h;				/* 縦幅 */

		gxChar			bpp;			/* ピクセルのビット数  8,16,24,32 */
		gxChar			mode;			/* discripter bit 3..0(4bits) :*/
										/* 属性.*/
										/* 0:αなし.*/
										/* 1:未定義のため無視可能.*/
										/* 2:未定義だが保存すべき */
										/* 3:通常のα
										/* 4:予め乗算されたα. 8:詳細不明. */
										/*  (bpp=32のとき:0x08, 24のとき0x00, 16のとき0x01 8のとき0x08,だった) */
										/* bit 5,4         : ピクセルの格納順は  0:左下から 1:左上 2:右下  3:右上 */

	} stTgaInfo;

	typedef struct stTgaPixel
	{
		Uint8	blue;
		Uint8	green;
		Uint8	red;
		Uint8	alpha;

	} stTgaPixel;

	CFileTarga(void);
	~CFileTarga(void);

	//新規に画像を作成する

	gxBool Create(int width,int height,int colbit,gxChar *pal=NULL);

	//画像のファイルデータそのものを取得する
	Uint8* GetFileImage(){	return m_pFileImage;	}

	//画像のファイルサイズを取得する
	Uint32 GetFileSize()
	{
		return m_FileSize;
	}

	//ファイルに吐き出す
	gxBool SaveFile( const gxChar *filename);

	//ファイルを読み込む
	gxBool LoadFile( const gxChar *filename,Uint32 col=0x00000000 );

	//Targaファイルを読み込む
	gxBool LoadTarga( const gxChar *filename,Uint32 col=0x00000000);

	//メモリからTGAファイルを読み込む
	gxBool ReadFile(const Uint8 *pMem,Uint32 sz,Uint32 col=0x00000000);

	//背景色を設定
	void SetBgColor(Uint32 bgcolor)
	{
		m_BgColor = bgcolor;
	}

	gxBool Line(int x1,int y1,int x2,int y2,int atr,int col=0xffffffff);
	gxBool Point(int x1,int y1,int atr,int col=0xffffffff);
	gxBool Box(int x1,int y1,int x2,int y2,int atr,int col=0xffffffff);
	gxBool BoxFill(int x1,int y1,int x2,int y2,int atr,int col=0xffffffff);
	gxBool Copy(int x1,int y1,int x2,int y2,int x3,int y3,int atr=0,int col=0xffffffff);
	gxBool Copy(CFileTarga *pTga,int x1,int y1,int x2,int y2,int x3,int y3,int atr=0,int col=0xffffffff);

	Uint32 GetPallet(int n)	{	return m_Pallet[n];		}
	Uint32 GetWidth()		{	return m_pTgaInfo->w;	}
	Uint32 GetHeight()		{	return m_pTgaInfo->h;	}
	Sint32 GetBPP()			{	return m_pTgaInfo->bpp;	}

	Sint32 SetRGB(int x,int y,Uint32 col);
	Sint32 SetARGB(int x,int y,Uint32 col);
	Sint32 SetARGBadd(int x,int y,Uint32 col);
	Sint32 SetARGBsub(int x,int y,Uint32 col);
	Sint32 SetARGBmul(int x,int y,Uint32 col);
	Sint32 GetARGB(int x,int y);
	Sint32 GetPalletIndex(int x,int y);

	void GetARGB( Uint32 uCol , stTgaPixel* pCol )
	{
		pCol->blue  = (uCol&0x000000ff)>>0;
		pCol->red   = (uCol&0x00ff0000)>>16;
		pCol->green = (uCol&0x0000ff00)>>8;
		pCol->alpha = (uCol&0xff000000)>>24;
	}

	void ClearImage();

	Uint8* GetTexelImage()
	{
		return m_TexelImage;
	}

private:

	void convToPlatformPixels();

	Uint8  setID(Sint8 palletnum);

	gxBool loadBitmap(gxChar *filename,Uint32 colorKey=0xff00ff00 );
	gxBool loadPNG( gxChar *filename , Uint32 colorKey );

	gxBool convertBMPtoTGA(gxChar *p,Uint32 sz , Uint32 colorKey = 0xff00ff00 );

	void setTexelImage( Uint8* pBuf );

	Uint8* RLEunpack( Uint8* pBuf , Uint32 uSize );

	Uint8   m_Filename[256];
	Uint8*  m_pFileImage;	//ファイルイメージ
	Uint32* m_Pallet;		//パレットデータへのポインタ
	Uint8*  m_TexelImage;	//テクスチャデータの開始位置
	Uint32  m_FileSize;		//ファイルサイズ
	Uint32  m_BgColor;		//クリアする背景色

	stTgaInfo *m_pTgaInfo;	//ヘッダー情報へのポインタ

	INLINE Uint8* getPos(int x,int y);

	void releaseImage()
	{
		if(m_pFileImage)
		{
			free(m_pFileImage);
		}
		m_pFileImage = NULL;
	};
	//----------------------------------
	//BMP関連
	//----------------------------------
};
#pragma pack (pop)

#endif	//_CFILETARGA_H_
