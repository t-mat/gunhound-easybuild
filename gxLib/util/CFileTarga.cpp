//-----------------------------------------------------------------------------------
//targaファイル１枚を管理する
//-----------------------------------------------------------------------------------
//---------------------------------------------------------------------------
//型宣言
//---------------------------------------------------------------------------

#include <gxLib.h>
#include "CFileTarga.h"
//#include "CFilePNG.h"

#pragma pack(push,1)
static const gxChar tgaHutter[] = {
//	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x54,0x52,0x55,0x45,0x56,0x49,0x53,0x49,0x4f,0x4e,0x2d,0x58,0x46,0x49,0x4c,0x45,0x2e,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,'C' ,'r' ,'e' ,'a' ,'t' ,'e' ,'d' ,'B' ,'y' ,'R' ,'a' ,'g' ,'i' ,'.' ,'T' ,'G' ,'A' ,' ' ,
};

CFileTarga::CFileTarga()
{
	m_Filename[0] = 0x00;
	m_pFileImage = NULL;		//ファイルイメージ
	m_TexelImage = NULL;		//テクスチャデータの開始位置
	m_FileSize 	= 0;			//ファイルサイズ
	m_BgColor 	= 0x00000000;	//クリアする背景色
	m_Pallet 	= NULL;			//パレットデータへのポインタ
	m_pTgaInfo 	= NULL;			//ヘッダー情報へのポインタ

}


CFileTarga::~CFileTarga()
{
	releaseImage();
}


gxBool CFileTarga::Create(int width,int height,int colbit,gxChar *pal)
{
	int palsize = 0;
	int palletNum=0;

	releaseImage();

	if(pal)
	{
		palletNum = 256;
		palsize = palletNum*4;
	}

	if(pal)
	{
		m_FileSize = width*height*(colbit/8) + 256*4;
	}
	else
	{
		m_FileSize = width*height*(colbit/8);
	}

	m_FileSize += sizeof(stTgaInfo) + sizeof(tgaHutter);

	m_pFileImage = (Uint8*)malloc(m_FileSize);
	memset((void*)m_pFileImage,m_BgColor,m_FileSize);	//背景色でクリア
	m_pTgaInfo   = (stTgaInfo*)m_pFileImage;

	//-----------------------------------------------------------
	//ヘッダ情報の構築
	//-----------------------------------------------------------
	m_pTgaInfo->FieldLength   = 0;					/* palletNum*4 ヘッダ後のピクセルデータまでのバイト数? */
	m_pTgaInfo->ColorMapType  = palletNum? 1 : 0;	/* フォーマットの種類. 0,1,2,3,9,10,11 */
	m_pTgaInfo->ImageType     = palletNum? 1 : 2;	/* フォーマットの種類. 0,1,2,3,9,10,11 */

	m_pTgaInfo->ColorMapIndex = 0;				/* 色パレットの開始番号 */
	m_pTgaInfo->clutNum       = palletNum;		/* 色パレットの個数 */
	m_pTgaInfo->ColorMapSize  = 32;				/* 色パレットのビット数 15,16,24,32 */

	m_pTgaInfo->x0 = 0;					/* 始点x */
	m_pTgaInfo->y0 = 0;					/* 始点y 画面左下が原点(0,0)なので注意！ */
	m_pTgaInfo->w = width;				/* 横幅 */
	m_pTgaInfo->h = height;				/* 縦幅 */
	m_pTgaInfo->bpp = colbit;			/* ピクセルのビット数  8,16,24,32 */
	m_pTgaInfo->mode = 8;				//不明/* bit 3..0(4bits) : 属性. 0:αなし. 1:未定義のため無視可能. 2:未定義だが保存すべき */

	if( m_pTgaInfo->ColorMapType )
	{
		//パレット付の場合
		m_Pallet     = (Uint32*)&m_pFileImage[sizeof(stTgaInfo)];
		m_TexelImage = (Uint8*)&m_pFileImage[ sizeof(stTgaInfo)+m_pTgaInfo->ColorMapSize/8 * m_pTgaInfo->clutNum ];//m_pTgaInfo->ColorMapType];
		memcpy( (Uint8*)m_Pallet , pal ,  m_pTgaInfo->ColorMapSize/8 * m_pTgaInfo->clutNum );
	}
	else
	{
		//パレット無の場合
		m_TexelImage = &m_pFileImage[sizeof(stTgaInfo)];
		m_Pallet 	= NULL;
	}

	//-----------------------------------------------------------
	//フッタ情報の追加
	//-----------------------------------------------------------
	memcpy(&m_pFileImage[m_FileSize-sizeof(tgaHutter)/*HUTTER_SIZE*/],tgaHutter,sizeof(tgaHutter));

	return true;
}

gxBool CFileTarga::SaveFile( const gxChar *filename)
{
	//---------------------------------------
	//ファイルを保存する
	//---------------------------------------

	if( !gxLib::SaveFile( filename , m_pFileImage , m_FileSize ) )
	{
		return gxFalse;
	}


	return true;
}

gxBool CFileTarga::LoadFile( const gxChar *filename , Uint32 col )
{
	gxChar Buf[256];
	sprintf(Buf,"%s",filename);
	Uint32 len = strlen(Buf);

	gxUtil::StrUpr( Buf );

	for(int ii=len;ii>0;ii--)
	{
		if( filename[ii] == '.' )
		{
			if( strcmp( (gxChar*)&Buf[ii],".BMP" ) == 0 )
			{
				return loadBitmap( (gxChar*)filename , col );
			}
			else if( strcmp( (gxChar*)&Buf[ii],".PNG" ) == 0 )
			{
				return loadPNG( (gxChar*)filename , col );
			}
			else
			{
				return LoadTarga( filename , col );
			}
		}
	}

	return gxFalse;	
}


gxBool CFileTarga::LoadTarga( const gxChar *filename , Uint32 col)
{
	//---------------------------------------
	//ファイルを読み込む
	//---------------------------------------

	Uint8* pBuffer=NULL;

	pBuffer = gxLib::LoadFile( filename ,  &m_FileSize );

	if( pBuffer == NULL  )
	{
		return gxFalse;
	}

	ReadFile( pBuffer , m_FileSize , col );

	delete[] pBuffer;//free( pBuffer );

	return true;

}


gxBool CFileTarga::ReadFile( const Uint8 *pMem,Uint32 filesz,Uint32 col)
{
	//---------------------------------------------
	//メモリからテクスチャデータを構築する
	//---------------------------------------------

	releaseImage();

	if(pMem[0] == 'B' && pMem[1] == 'M' )//&& pMem[2] == '6')
	{
		return convertBMPtoTGA((gxChar*)pMem , filesz , col);
	}

	m_pFileImage = (Uint8*)malloc(filesz);

	memcpy(m_pFileImage,pMem,filesz);

	m_pTgaInfo 	= (stTgaInfo*)m_pFileImage;	//ヘッダー情報へのポインタ

	if( m_pTgaInfo->ImageType >= 0x08 )
	{
		//RLE圧縮タイプ
		Sint32 sRLEunPack = 0;	//RLE圧縮展開ルーチンが必要
		RLEunpack( NULL/*m_TexelImage*/ , 0 );
	}

	if(m_pTgaInfo->ColorMapType)
	{
		//パレット付の場合
		m_Pallet     = (Uint32*)&m_pFileImage[sizeof(stTgaInfo)];
		m_TexelImage = (Uint8*)&m_pFileImage[sizeof(stTgaInfo)+m_pTgaInfo->clutNum*(m_pTgaInfo->ColorMapSize/8)];//ColorMapType];
	}
	else
	{
		//パレット無の場合
		m_TexelImage = &m_pFileImage[sizeof(stTgaInfo)];
		m_Pallet 	= NULL;
	}

	convToPlatformPixels();

	return true;
}

struct _RGB {
	Uint8 r;
	Uint8 g;
	Uint8 b;
};

struct _ABGR {
	Uint8 a;
	Uint8 b;
	Uint8 g;
	Uint8 r;
};


void CFileTarga::convToPlatformPixels()
{
#if 0   //defined BIG_ENDIAN
	//assert(m_pTgaInfo->bpp == 32 || m_pTgaInfo->bpp == 24);

	unsigned size = m_pTgaInfo->w * m_pTgaInfo->h;

	switch (m_pTgaInfo->bpp) {
	case 32:
		for (unsigned i = 0; i < size; ++i) {
			unsigned c = ((unsigned*)m_TexelImage)[i];
			unsigned a = TARGA_GET_A(c);
			unsigned r = TARGA_GET_R(c);
			unsigned g = TARGA_GET_G(c);
			unsigned b = TARGA_GET_B(c);
			c		   = TEX_ABGR(a, r, g, b);
			((unsigned*)m_TexelImage)[i] = c;
		}
		break;

	case 24:
		for (unsigned i = 0; i < size; ++i) {
			_RGB c = ((_RGB*)m_TexelImage)[i];
			//unsigned a = 0xff;
			unsigned r = c.r;
			unsigned g = c.g;
			unsigned b = c.b;
			c.r = r;
			c.g = g;
			c.b = b;
			((RGB*)m_TexelImage)[i] = c;
		}
		break;
	case 8:
		{
	        if( false ) {
	            if( m_pTgaInfo->ColorMapSize == 32 ) {
	                for (unsigned i = 0; i < size; ++i) {
	                    int index = ((Uint8*)m_TexelImage)[i];
	                    unsigned c = 
						_ABGR c = (_ABGR*)((unsigned*)m_Pallet)[index];
	                    unsigned a = c.a;
	                    unsigned r = c.r;
	                    unsigned g = c.g;
	                    unsigned b = c.b;
	                    ((unsigned*)m_TexelImage)[i] = ARGB(a,b,g,r);
	                }
	            } else {
	                for (unsigned i = 0; i < size; ++i) {
	                    int index = ((Uint8*)m_TexelImage)[i];
	                    RGB c = ((RGB*)m_Pallet)[index];
	                    unsigned a = 0xff;
	                    unsigned r = c.b;
	                    unsigned g = c.g;
	                    unsigned b = c.r;
	                    ((unsigned*)m_TexelImage)[i] = ARGB(a,b,g,r);
	                }
	            }
	        } else {
	            unsigned clutNum = m_pTgaInfo->clutNum[0] | (m_pTgaInfo->clutNum[1] << 8);
	            Uint8* clut = (Uint8*)m_Pallet;
	            if (m_pTgaInfo->ColorMapSize == 32) {
	                for (int i = 0; i < clutNum; i++) {
	                    m_Pallet[i] = ARGB(*(clut + 3), *(clut + 0), *(clut + 1), *(clut + 2));
	                    clut += 4;
	                }
	            }
	        }
		}
	    break;

	default:
		break;
	}
#endif
}


Sint32 CFileTarga::SetRGB(int x,int y,Uint32 col)
{
	//------------------------------------------------------
	//ピクセルの合成(新設RGBのみアルファチャンネルを残す版)
	//------------------------------------------------------

	if( m_pTgaInfo && m_pTgaInfo->ColorMapType )
	{
		return 0x00;
	}

	Uint32 *pos2 = (Uint32*)getPos(x,y);

	*pos2  = (*pos2)&0xff000000;
	*pos2 |= (col&0x00ffffff);

	return *pos2;	
}


Sint32 CFileTarga::SetARGB(int x,int y,Uint32 col)
{
	//--------------------------------------------
	//ピクセルの合成
	//--------------------------------------------

///*毒*/	Uint8 *pos = getPos(x,y);

	if(m_pTgaInfo && m_pTgaInfo->ColorMapType)
	{
		return 0x00;
	}

	Uint32 *pos2 = (Uint32*)getPos(x,y);

	*pos2 = col;

	return *pos2;	
}

Sint32 CFileTarga::SetARGBadd(int x,int y,Uint32 col)
{
	//--------------------------------------------
	//ピクセルの加算合成
	//--------------------------------------------
	Uint32 *pos = (Uint32*)getPos(x,y);

	stTgaPixel dstcolor;
	stTgaPixel *srccolor = (stTgaPixel*)pos;

	int a,r,g,b;
	a = dstcolor.alpha = (col>>24)&0x000000ff;
	r = dstcolor.red   = (col>>16)&0x000000ff;
	g = dstcolor.green = (col>>8)&0x000000ff;
	b = dstcolor.blue  = (col>>0)&0x000000ff;

	a += srccolor->alpha;
	r += srccolor->red;
	g += srccolor->green;
	b += srccolor->blue;

	if(a>= 0xff) a = 0xff;
	if(r>= 0xff) r = 0xff;
	if(g>= 0xff) g = 0xff;
	if(b>= 0xff) b = 0xff;

	*pos = ((a<<24)|(r<<16)|(g<<8)|(b<<0));

	return *pos;

}

Sint32 CFileTarga::SetARGBsub(int x,int y,Uint32 col)
{
	//--------------------------------------------
	//ピクセルの減算合成
	//--------------------------------------------
	Uint32 *pos = (Uint32*)getPos(x,y);

	stTgaPixel dstcolor;
	stTgaPixel *srccolor = (stTgaPixel*)pos;

	int a,r,g,b;
	a = dstcolor.alpha = (col>>24)&0x000000ff;
	r = dstcolor.red   = (col>>16)&0x000000ff;
	g = dstcolor.green = (col>>8)&0x000000ff;
	b = dstcolor.blue  = (col>>0)&0x000000ff;

	a -= srccolor->alpha;
	r -= srccolor->red;
	g -= srccolor->green;
	b -= srccolor->blue;

	if(a<= 0x00) a = 0xff;
	if(r<= 0x00) r = 0xff;
	if(g<= 0x00) g = 0xff;
	if(b<= 0x00) b = 0xff;

	*pos = ((a<<24)|(r<<16)|(g<<8)|(b<<0));

	return *pos;

}


Sint32 CFileTarga::SetARGBmul(int x,int y,Uint32 col)
{
	//--------------------------------------------
	//EsENEZEaCAa?eZcae¨
	//--------------------------------------------
	Uint32 *pos = (Uint32*)getPos(x,y);
    
	stTgaPixel dstcolor;
	stTgaPixel *srccolor = (stTgaPixel*)pos;
    
	Uint8 a,r,g,b;
	dstcolor.alpha = a = (col>>24)&0x000000ff;
	dstcolor.red   = r = (col>>16)&0x000000ff;
	dstcolor.green = g = (col>>8)&0x000000ff;
	dstcolor.blue  = b = (col>>0)&0x000000ff;
    
	a = srccolor->alpha * dstcolor.alpha / 255.0f;
	r = srccolor->red   * dstcolor.red   / 255.0f;
	g = srccolor->green * dstcolor.green / 255.0f;
	b = srccolor->blue  * dstcolor.blue  / 255.0f;
    
	*pos = ((a<<24)|(r<<16)|(g<<8)|(b<<0));
    
	return *pos;
    
}


Sint32 CFileTarga::GetPalletIndex(int x,int y)
{
	Uint8 *pos = getPos(x,y);

	if(m_pTgaInfo->ColorMapType)
	{
		return *pos;
	}

	return 0x00;
}


Sint32 CFileTarga::GetARGB(int x,int y)
{
	Uint8 *pos = getPos(x,y);

	if(m_pTgaInfo->ColorMapType)
	{
		if(m_pTgaInfo->ColorMapSize == 24 )
		{
			Uint8 *pPal = (Uint8*)&m_Pallet[0];//(*pos)*3];
			pPal += (*pos)*3;
			int a,r,g,b;
			a = 0xff;
			b = pPal[0];
			g = pPal[1];
			r = pPal[2];
			
			return (a<<24) | (r<<16) | (g<<8) | (b<<0);
		}
		if(m_pTgaInfo->ColorMapSize == 32 )
		{
			return m_Pallet[*pos];
		}
	}

	Uint32 argb = *(Uint32*)pos;

	if(m_pTgaInfo->bpp == 24 )
	{
		argb = 0xff000000;
		argb = (pos[2]<<16) | (pos[1]<<8 ) | (pos[0]<<0 ) ;
		argb |= 0xff000000;
	}

	return argb;
}


Uint8 CFileTarga::setID(Sint8 _palletNum)
{
	//--------------------------------------------
	//ヘッダー情報のIDを生成する
	//--------------------------------------------

	if(_palletNum) return 1;	//INDEX
	return 0;					//RGB

/*
	case 1:	//INDEX
	case 2:	//RGB
		break;
	case 0:	//データなし
	case 3:	//白黒
	case 9:	//INDEX:RLE圧縮
	case 10://RGB  :RLE圧縮
	case 11://白黒 :RLE圧縮
	default:
		break;
	}
*/
}


INLINE Uint8* CFileTarga::getPos(int x,int y)
{
	//-----------------------------------------------
	//イメージ上の座標位置アドレスを得る
	//-----------------------------------------------

	int mode = m_pTgaInfo->mode&0x30;

	if((mode&0x10) != 0 ) x = (m_pTgaInfo->w-1)-x;	//右から
	if((mode&0x20) == 0 ) y = (m_pTgaInfo->h-1)-y;	//下から

	Sint32 n = ((x) + (y*m_pTgaInfo->w)) *(m_pTgaInfo->bpp>>3);
	if( n >= (m_pTgaInfo->w * m_pTgaInfo->h)*(m_pTgaInfo->bpp>>3) || n < 0 )
	{
		ASSERT( n == 0 );
		n = 0;
	}
	return &m_TexelImage[ n ];

}

gxBool CFileTarga::Line(int x0,int y0,int x1,int y1,int atr,int col)
{
	int E,x,y;
	int dx,dy,sx,sy,i;

	sx = ( x1 > x0 ) ? 1 : -1;
	dx = ( x1 > x0 ) ? x1 - x0 : x0 - x1;
	sy = ( y1 > y0 ) ? 1 : -1;
	dy = ( y1 > y0 ) ? y1 - y0 : y0 - y1;

	x = x0;
	y = y0;

	if( dx >= dy )
	{
		// 傾きが1以下の場合

		E = -dx;

		for( i = 0; i <= dx; i++ )
		{
			Point( x, y, atr,col );

			x += sx;
			E += 2 * dy;

			if( E >= 0 )
			{
				y += sy;
				E -= 2 * dx;
			}
		}
	}
	else
	{
		// 傾きが1より大きい場合
		E = -dy;
		for( i = 0; i <= dy; i++ )
		{
			Point( x, y, atr,col );
			y += sy;
			E += 2 * dx;
			if( E >= 0 )
			{
				x += sx;
				E -= 2 * dy;
			}
		}
	}

	return true;
}

gxBool CFileTarga::Point(int x1,int y1,int atr,int col)
{
	//点を打つ

	if(atr&0x04){
		SetARGBadd(x1,y1,col);
	}else if(atr&0x08){
		SetARGBsub(x1,y1,col);
	}else{
		SetARGB(x1,y1,col);
	}
	return gxTrue;
}

gxBool CFileTarga::Box(int x1,int y1,int x2,int y2,int atr,int col)
{
	//箱を書く

	int tmp;

	if(x2<x1){
		tmp = x1;
		x2 = x1;
		x1 = tmp;
	}

	if(y2<y1){
		tmp = y1;
		y2 = y1;
		y1 = tmp;
	}

	for(int y=y1;y<=y2;y++){
		for(int x=x1;x<=x2;x++){
			if(y==y1 || x==x1 || y==y2 || x==x2) {
				Point(x,y,atr,col);
				continue;
			}
		}
	}

	return true;
}

gxBool CFileTarga::BoxFill(int x1,int y1,int x2,int y2,int atr,int col)
{
	int tmp;

	if(x2<x1)
	{
		tmp = x1;
		x2 = x1;
		x1 = tmp;
	}

	if(y2<y1)
	{
		tmp = y1;
		y2 = y1;
		y1 = tmp;
	}

	for(int y=y1;y<y2;y++)
	{
		for(int x=x1;x<x2;x++)
		{
			Point(x,y,atr,col);
		}
	}

	return true;
}

gxBool CFileTarga::Copy(int x1,int y1,int x2,int y2,int x3,int y3,int atr,int col)
{
	//-----------------------------------------
	//自分のテクスチャの中でコピーを行う
	//x1,y1,x2,y2::コピー元の矩形
	//x3,y3::コピー先の座標
	//-----------------------------------------

	//x1,y1=u1,v1 x2,y2 = u2,v2, x3,y3=x,y;
	CFileTarga *rect = new CFileTarga();
	int xx,yy,x,y,flip=atr&(0x01|0x02);

	rect->Create(x2-x1,y2-y1,GetBPP(),(gxChar*)m_Pallet);

	xx = 0;	yy = 0;
	for(y=y1;y<y2;y++)
	{
		xx = 0;
		for(x=x1;x<x2;x++)
		{
			rect->SetARGB(xx,yy,GetARGB(x,y));
			xx++;
		}
		yy++;
	}

	Copy(rect,0,0,x2-x1,y2-y1,x3,y3);

	delete rect;

	return true;

}

gxBool CFileTarga::Copy(CFileTarga *pTga,int x1,int y1,int x2,int y2,int x3,int y3,int atr,int col)
{
	//-----------------------------------------
	//他のテクスチャとコピーを行う
	//x1,y1,x2,y2::コピー元の矩形
	//x3,y3::コピー先の座標
	//-----------------------------------------

	//x1,y1=u1,v1 x2,y2 = u2,v2, x3,y3=x,y;
	int xx,yy,x,y,flip=atr&(0x01|0x02);
	int w=0,h=0,wx=-1,hy=-1,argb;
	int xmax = GetWidth();
	int ymax = GetHeight();

	int mode = atr&(0x04|0x08);

	yy = 0;
	if(flip&0x01)
	{
		w = (x2-x1)-1;
		wx = 1;
	}

	if(flip&0x02)
	{
		h = (y2-y1)-1;
		hy = 1;
	}

	int px,py;

	for(y=y1;y<y2;y++)
	{
		xx = 0;
		py = y3+(h-yy)*hy;
		if(py > ymax) continue;

		for(x=x1;x<x2;x++)
		{
			px = x3+(w-xx)*wx;
			if(px >= xmax || px < 0)
			{
				xx ++;
				continue;
			}

			argb = pTga->GetARGB(x1+xx,y1+yy);
			if(0)
				{//(m_BgColor&0x00ffffff) == (unsigned int)(argb&0x00ffffff)){
				//抜き色なしにしてしまいます
			}
			else
			{
				if(mode&0x04)
				{
					SetARGBadd(px,py,argb);
				}
				else if(mode&0x08)
				{
					SetARGBsub(px,py,argb);
				}
				else
				{
					SetARGB(px,py,argb);
				}
			}
			xx ++;
		}
		yy ++;
	}

	return true;
}

gxBool CFileTarga::loadBitmap(gxChar *filename,Uint32 colorKey )
{
	//--------------------------------------------------
	//Bmpファイルを読み込んでtgaファイルとして保持する
	//ただし、何が何でも32bitTGAとします。
	//--------------------------------------------------

	Uint32 sLength;
	Uint8 * pBuffer=NULL;

	pBuffer = gxLib::LoadFile( filename , &sLength );

	if( pBuffer == NULL )
	{
		return gxFalse;
	}

	convertBMPtoTGA((gxChar*)pBuffer , sLength , colorKey);

	delete[] pBuffer;

	return true;
}

gxBool CFileTarga::loadPNG( gxChar *filename , Uint32 colorKey )
{
	//--------------------------------------------------
	//Bmpファイルを読み込んでtgaファイルとして保持する
	//ただし、何が何でも32bitTGAとします。
	//--------------------------------------------------

	Uint32 sLength;
	Uint8 * pData = NULL;

	pData = gxLib::LoadFile( filename , &sLength );

	if( pData == NULL )
	{
		return gxFalse;
	}

	//convertBMPtoTGA((gxChar*)pBuffer , sLength , colorKey);

	delete[] pData;

	return gxTrue;
}


//---------------------------------------------------------
//BMP関連
//---------------------------------------------------------

typedef struct stBmpHead {
	Uint16/*short*/	bfType;				// BM 0x4d42
	Uint32/*long*/	bfSize;				// Filesize
	Uint16/*short*/	bfReserved1;		// 0
	Uint16/*short*/	bfReserved2;		// 0
	Uint32/*long*/	bfOffBits;			// ファイルの先頭からデータ部までのオフセット
} stBmpHead;

typedef struct stBmpInfo {
	Uint32/*long*/	biSize;				// BMPINFOのサイズ：40Byte
	Uint32/*long*/	biWidth;			// 画像の横幅
	Uint32/*long*/	biHeight;			// 画像の高さ
	Uint16/*short*/	biPlanes;			// プレーン数(必1)
	Uint16/*short*/	biBitCount;			// PixelあたりのBit数
	Uint32/*long*/	biCompression;		// 圧縮(必：無し：0)
	Uint32/*long*/	biSizeImage;		// ?? 0
	Uint32/*long*/	biXPelsPerMeter;	// 横解像度(7198)
	Uint32/*long*/	niYPelsPerMeter;	// 縦解像度(7198)
	Uint32/*long*/	biClrUsed;			// 使用色数(0)
	Uint32/*long*/	biClrImportant;		// 重要な色数(0)
} stBmpInfo;

#define GETOFFSET_BMP(x,y,bmpx,bmpy)		(( ( (bmpy-1)-(y) ) *(bmpx) )  +(x))

gxBool CFileTarga::convertBMPtoTGA(gxChar *p,Uint32 sz ,Uint32 colorKey )
{
	//-------------------------------------------------
	//BMPからTGAに変換します
	//-------------------------------------------------
	stBmpHead* pBmpHead;
	stBmpInfo* pBmpInfo;
	int w,h;
	gxChar *pImage;
	gxChar *pPallet;

	pBmpHead = (stBmpHead*)p;
	pBmpInfo = (stBmpInfo*)&p[sizeof(stBmpHead)];

	w = pBmpInfo->biWidth;
	h = pBmpInfo->biHeight;

	if(pBmpInfo->biBitCount==8){
		//パレットの場合
		pPallet = &p[sizeof(stBmpHead)+sizeof(stBmpInfo)];
		pImage  = &p[pBmpHead->bfOffBits];
	}else{
		//RGBの場合
		pPallet = NULL;
		pImage  = &p[sizeof(stBmpHead)+sizeof(stBmpInfo)];
	}

	//BMPサイズの空のtgaファイルを生成
	Create(pBmpInfo->biWidth,pBmpInfo->biHeight,32/*pBmpInfo->biBitCount*/,NULL);

	//ピクセル単位でデータをコピーします
	Uint32 a,r,g,b;

	for(int y=0;y<h;y++) {
		for(int x=0;x<w;x++) {
			int offset = GETOFFSET_BMP(x,y,w,h)*(pBmpInfo->biBitCount>>3);
			Uint8 index = pImage[offset];
			int col=0;
			switch(pBmpInfo->biBitCount){
			case 4:
				//めんどいのでサポートしない
				break;
			case 8:
				a = 0xFF;
				b = pPallet[index*4+0]&0x000000ff;
				g = pPallet[index*4+1]&0x000000ff;
				r = pPallet[index*4+2]&0x000000ff;
				a = ( (colorKey&0x00ffffff) == ((r<<16)|(g<<8)|(b<<0)) )? 0x00 : 0xff;
				break;
			case 24:
				b = pImage[offset+0]&0x000000ff;
				g = pImage[offset+1]&0x000000ff;
				r = pImage[offset+2]&0x000000ff;
				//colorKey = 0xffff00ff;
				a = ( (colorKey&0x00ffffff) == ((r<<16)|(g<<8)|(b<<0)) )? 0x00 : 0xff;
				break;
			case 32:
				b = pImage[offset+0]&0x000000ff;
				g = pImage[offset+1]&0x000000ff;
				r = pImage[offset+2]&0x000000ff;
				a = ( (colorKey&0x00ffffff) == ((r<<16)|(g<<8)|(b<<0)) )? 0x00 : 0xff;
				break;
			}

			col = ((a<<24)|(r<<16)|(g<<8)|(b<<0));

			SetARGB(x,y,col);
		}
	}

	return true;
}

void CFileTarga::setTexelImage( Uint8* pBuf )
{
	memcpy( &m_pFileImage[sizeof(stTgaInfo)] , pBuf , m_pTgaInfo->w*m_pTgaInfo->h*(m_pTgaInfo->bpp>>3) );
	m_pTgaInfo->mode |= 0x20;
}


void CFileTarga::ClearImage()
{
	gxUtil::MemSet(m_TexelImage,0x00,m_pTgaInfo->w*m_pTgaInfo->h*(m_pTgaInfo->bpp/8));
}


Uint8* CFileTarga::RLEunpack( Uint8* pBuf , Uint32 uSize )
{
	//-------------------------------------------
	// RLE圧縮されたTGAを展開します
	//-------------------------------------------

	Uint32 w = m_pTgaInfo->w;
	Uint32 h = m_pTgaInfo->h;
	Uint32 pixelSize = m_pTgaInfo->bpp/8;
	Uint32 uBufSize = w*h*pixelSize;
	Uint32 uBufPos1 = 0;	//圧縮データのポインタ
	Uint32 uBufPos2 = 0;	//展開後データのポインタ

	Sint32 bpp = m_pTgaInfo->bpp;
	Uint8* pPalletBuffer = NULL;
	Uint32 uPalletBufSize = m_pTgaInfo->clutNum*(m_pTgaInfo->ColorMapSize/8);

	//画像フォーマットに応じたバッファサイズを確保
	Uint8* pFileImage = (Uint8*)malloc( uBufSize );

	pBuf = &m_pFileImage[ sizeof(stTgaInfo) + uPalletBufSize ];

	if( uPalletBufSize > 0 )
	{
		m_Pallet = (Uint32*)&m_pFileImage[ sizeof(stTgaInfo) ];
	}


	//イメージタイプを非圧縮タイプに変更
//	if( m_pTgaInfo->ImageType >= 0x08 ) m_pTgaInfo->ImageType -= 0x08;


	for(Uint32 yy=0; yy<h; yy++)
	{
		//１ライン毎の対応
		for(Uint32 xx=0; xx<w; xx++)
		{
			Uint32 sCnt = 0;
			gxBool bSingle = gxFalse;	//リテラルなグループか？

			if( (pBuf[uBufPos1+0]&0x80) )
			{
				//連続したデータ領域がある(反復)
				sCnt = pBuf[ uBufPos1+0 ]&0x7f;
			}
			else
			{
				//連続してないピクセルがsCnt+1続く
				sCnt = pBuf[ uBufPos1+0 ]&0x7f;
				bSingle = gxTrue;
			}

			sCnt ++;

			//制御データ分を１つすすめる
			uBufPos1 ++;

			for(Uint32 ii=0;ii<sCnt;ii++)
			{
				if( uBufPos2 > uBufSize )
				{
					//展開後のバッファサイズを超えてしまった
					gxLib::DebugLog("ERROR::展開後のバッファサイズを超えてしまった(%d / %d )",uBufPos2 , uBufSize);
					break;
				}

				if( uBufPos1 >= uSize )
				{
					//圧縮データの範囲を超えてデコードしにいってしまった
					//gxLib::DebugLog("ERROR::展開後のバッファサイズを超えてしまった(%d / %d )",uBufPos1 , uSize );
					//break;
				}

				switch(pixelSize){
				case 1:	//8bit
					pFileImage[ uBufPos2+0 ] = pBuf[uBufPos1+0];
					break;

				case 3:	//24bit
					pFileImage[ uBufPos2+0 ] = pBuf[uBufPos1+0];
					pFileImage[ uBufPos2+1 ] = pBuf[uBufPos1+1];
					pFileImage[ uBufPos2+2 ] = pBuf[uBufPos1+2];
					break;

				case 4:	//32bit
					pFileImage[ uBufPos2+0 ] = pBuf[uBufPos1+0];
					pFileImage[ uBufPos2+1 ] = pBuf[uBufPos1+1];
					pFileImage[ uBufPos2+2 ] = pBuf[uBufPos1+2];
					pFileImage[ uBufPos2+3 ] = pBuf[uBufPos1+3];
					break;
				default:
					gxLib::DebugLog("ERROR::よくわからない謎のTGA画像(%dbit)",pixelSize*8);
					break;
				}

				uBufPos2 += pixelSize;

				if( bSingle ) uBufPos1 += pixelSize;
			}

			xx += sCnt;
			xx --;
			if( !bSingle ) uBufPos1 += pixelSize;
		}
	}

	//--------------------------------------------------
	//パレット関連の制御
	//--------------------------------------------------

	if( m_Pallet )
	{
		//パレットがある場合はパレットデータをコピーしておく

		pPalletBuffer = (Uint8*)malloc( uPalletBufSize );
		memcpy( pPalletBuffer , m_Pallet , uPalletBufSize );
	}

	//新規にテクスチャを生成
	Create( w , h , bpp , (gxChar*)pPalletBuffer );

	//テクスチャイメージをコピー
	memcpy( m_TexelImage , pFileImage , uBufSize );

	//パレットバッファを解放
	if( pPalletBuffer )	free( pPalletBuffer );

	free( pFileImage );

	return NULL;
}




#pragma pack(pop)


