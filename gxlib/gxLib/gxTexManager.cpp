//--------------------------------------------------
//
// gxTexdManager.cpp
// テクスチャの管理を行います、1024x1024のマスターテクスチャを
// 管理し、テクスチャ切り替えのオーバーヘッドを吸収します。
//
//--------------------------------------------------

#include <gxLib.h>
#include "gx.h"
#include "gxTexManager.h"

SINGLETON_DECLARE_INSTANCE( CTexManager );

CTexManager::CTexManager(void)
{
	for(Sint32 ii=0;ii<enMasterPageNum; ii++)
	{
		Uint32 sz = enMasterWidth*enMasterHeight;

		m_Targa[ii].Create( enMasterWidth , enMasterHeight , 32 );
		m_bUpdate[ ii ] = gxFalse;
		m_bUsed[ ii ] = gxFalse;
	}

	m_sTexMax = (enMasterWidth/enPageWidth)*(enMasterHeight/enPageHeight);
}


CTexManager::~CTexManager(void)
{

}


gxBool CTexManager::save( Sint32 bankNo )
{
	//画像ファイルとして書き出す

	gxChar buf[255];

	sprintf( buf , "vram%03d.tga" , bankNo ); 
	m_Targa[bankNo].SaveFile( buf );

	return gxTrue;
}


gxBool CTexManager::LoadTexture( Sint32 tpg , const gxChar* fileName , Uint32 colorKey , Uint32 ox , Uint32 oy ,Sint32 *w , Sint32 *h )
{
	//画像ファイルの読み込み
	//BMP,TGAに対応

	CFileTarga tga;

	gxLib::DebugLog("[テクスチャ%3d(%d)]%s",tpg,tpg/16,fileName );

	if( !tga.LoadFile( fileName , colorKey ) )
	{
		gxLib::DebugLog("ファイルが見つからなかった" );
		return gxFalse;
	}

	addTexture( tpg , &tga ,colorKey , ox , oy , w , h );

	return gxTrue;
}


gxBool CTexManager::addTexture( Sint32 tpg ,CFileTarga* pTga , Uint32 colorKey ,Uint32 ox , Uint32 oy , Sint32 *pTexW , Sint32 *pTexH )
{
	//テクスチャをVRAMバッファへ更新する

	Uint32 pg = tpg/m_sTexMax;	//テクスチャページ番号
	Uint32 w  = (enMasterWidth  / enPageWidth);
	Uint32 h  = (enMasterHeight / enPageWidth);
	Uint32 u  = enPageWidth  * (tpg%w);
	Uint32 v  = enPageHeight * ((tpg/w)%h);

	if( pTexW ) *pTexW = w;
	if( pTexH ) *pTexH = h;

	for(Uint32 x=0;x<pTga->GetWidth(); x++)
	{
		for(Uint32 y=0;y<pTga->GetHeight(); y++)
		{
			Uint32 argb = pTga->GetARGB( x , y );

#ifdef USE_OPENGL
			//OpenGL用の変換
			Uint32 r,g,b,a;
			a = ((argb&0xff000000)>>24)&0x000000ff;
			r = ((argb&0x00ff0000)>>16)&0x000000ff;
			g = ((argb&0x0000ff00)>>8 )&0x000000ff;
			b = ((argb&0x000000ff)>>0 )&0x000000ff;
			argb = ((a<<24)|(b<<16)|(g<<8)|(r<<0));
#endif
			if( (argb&0x00ffffff) == (colorKey&0x00ffffff) )
			{
				argb &= 0x00ffffff;
			}
			m_Targa[pg].SetARGB( ox + u+x , oy + v+y , argb ); 
		}
	}

	m_bUpdate[ pg ] = gxTrue;
	m_bUsed[ pg ]   = gxTrue;

#ifdef GX_DEBUG
	save( pg );
#endif

	return gxTrue;

}


gxBool CTexManager::UploadTexture( gxBool bForce ,Sint32 sPage )
{
	//テクスチャの更新

	for(Sint32 ii=0;ii<enMasterPageNum; ii++)
	{
		if( m_bUpdate[ii] || bForce )
		{
			//if( bForce && sPage == ii && !m_bUsed[ ii ] ) m_bUsed[ ii ] = gxTrue;
			if( bForce ) m_bUsed[ ii ] = gxTrue;
			if( m_bUsed[ ii ] )	::Upload( ii );
		}
		m_bUpdate[ii] = gxFalse;
	}

	return gxTrue;
}


void CTexManager::changeABGR2RGBA ( Sint32 x , Sint32 y , Uint8* pGraphicsBuffer )
{
	//RGBA画像をARGBに変換

	Uint32 *p = (Uint32*)pGraphicsBuffer;
	for(Uint32 ii=0; ii<(Uint32)(x*y); ii++)
	{
		Uint32 r,g,b,a;
		a = ((p[ii]&0xff000000)>>24)&0x000000ff;
		b = ((p[ii]&0x00ff0000)>>16)&0x000000ff;
		g = ((p[ii]&0x0000ff00)>>8 )&0x000000ff;
		r = ((p[ii]&0x000000ff)>>0 )&0x000000ff;

		p[ii] = ((a<<24)|(r<<16)|(g<<8)|(b<<0));
	}
}

// ------------------------------------------------------------------------------------

void CTexManager::changeTop2Bottom( Sint32 w , Sint32 h , Uint8* pGraphicsBuffer )
{
	//天地逆転

	Uint32 *p = new Uint32[w*h];
	Uint32 *q = (Uint32*)pGraphicsBuffer;
	Uint32 line=0;

	for(Uint32 y=0; y<(Uint32)h; y++)
	{
		line = h-y;
		for(Uint32 x=0; x<(Uint32)w; x++)
		{
			p[y*w+x] = q[line*w+x];
		}
	}

	memcpy(q,p,w*h*4);

	delete[] p;

}


gxBool CTexManager::rectCopy( Uint32 tpg , Uint32 u , Uint32 v , Uint32 w , Uint32 h , Uint8* pBuf )
{
/*
	Uint32 col = 0;
	Uint32  n;
	Uint32  m;

	if( w >= enMasterWidth  ) w = enMasterWidth;
	if( h >= enMasterHeight ) h = enMasterHeight;

	for(Uint32 y=0;y<h;y++)
	{
		for(Uint32 x=0;x<w;x++)
		{
			n = (v+y)*enMasterWidth + (u+x);
			m = (y*w+x)*4;
			//ABGR - OpenGL
//			col = (pBuf[m+3]<<24) | (pBuf[m+0]<<16) | (pBuf[m+1]<<8) | (pBuf[m+2]<<0);
//			m_VRAM[tpg][n] = col;

			//ARGB
			col = (pBuf[m+3]<<24) | (pBuf[m+2]<<16) | (pBuf[m+1]<<8) | (pBuf[m+0]<<0);
//			m_VRAM[tpg][n] = col;
		}
	}
*/
	return gxTrue;

}

