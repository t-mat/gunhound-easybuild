//--------------------------------------------------------------------------
//
// CFontManager.cpp
// written bt tomi.
// 2013.01.21　更新
// 
//--------------------------------------------------------------------------

#include <gxLib.h>
#include <gxLib/Util/CFileTarga.h>
#include "CFontManager.h"
#include "CKanjiTbl.h"

#define KANJI_NONE (0xffff)

SINGLETON_DECLARE_INSTANCE( CFontManager );

//汎用の文字列バッファ
gxChar CFontManager::s_StrTemp[enTempLength];

CFontManager::CFontManager()
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------
	m_sStartTpg = 0;
	m_uFontSize = enFontSize32;

	setDefault();

	m_pKanjiDB = NULL;

	for(Uint32 ii=0x0000; ii<0xffff; ii++ )
	{
		m_uKanjiList[ ii ] = KANJI_NONE;
	}

}

CFontManager::~CFontManager()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------

	if( m_pKanjiDB == NULL ) delete[] m_pKanjiDB;
}


gxBool CFontManager::LoadKNJ( Sint32 sPage , gxChar* pFileName , gxBool bDebugNoneLoadTexture  )
{
	//----------------------------------------------
	//漢字ファイルをロードする
	//----------------------------------------------
	Uint8* pKNJ   = 0;
	Uint32  uSize = 0;

	for(Uint32 ii=0x0000; ii<0xffff; ii++ )
	{
		m_uKanjiList[ ii ] = KANJI_NONE;
	}


	{
		//ファイルからセットする
		m_sStartTpg = sPage;

		pKNJ = gxLib::LoadFile( pFileName , &uSize );

		gxChar tempPath[256];

		gxUtil::GetPath(pFileName, tempPath);

		setPath(tempPath);

		if( pKNJ )
		{
			readKNJ( pKNJ , uSize , bDebugNoneLoadTexture );
			delete[] pKNJ;	//free( pKNJ );
		}
		else
		{
			//ファイルが見つからなかった
			return gxFalse;
		}
	}

	return gxTrue;
}



void CFontManager::setDefault( StFontStat* pStat )
{
	//----------------------------------------------
	//フォントの設定をデフォルトに戻す（＆設定する）
	//----------------------------------------------

	if( pStat )
	{
		//引数の設定をデフォルトにする
		m_stDefault = *pStat;
	}
	else
	{
		//これがデファクトスタンダード
		m_stDefault.argb        = ARGB( 0xff,0xff,0xff,0xff );
		m_stDefault.bProp       = gxFalse;
		m_stDefault.bRectEnable = gxFalse;
		m_stDefault.sPitch      = 0;
		m_stDefault.u = 0;
		m_stDefault.v = 0;
		m_stDefault.w = 0;
		m_stDefault.h = 0;
		m_stDefault.fScaleX = 1.f;
		m_stDefault.fScaleY = 1.f;
	}
}


gxBool CFontManager::MakeKNJ( Sint32 sFontPixel , gxChar* pTxtName , gxChar* pTexName )
{
	//--------------------------------------------------
	//KNJファイルを作成します
	//--------------------------------------------------
	StKNJformat KanjiHeader;
	Uint32 uSize = 0;
	Uint8 *pFile = NULL;

	pFile = gxLib::LoadFile( pTxtName , &uSize );

	if( pFile == NULL ) 
	{
		return gxFalse;
	}

	gxChar word[32];
	Uint32 uKanjiCnt = 0;

	//フォントヘッダー

	KanjiHeader.wordNum  = 0;
	KanjiHeader.texNum   = 0;
	KanjiHeader.pixel = sFontPixel;
	memcpy( KanjiHeader.filename , pTexName ,enTexFileNameLength );
	KanjiHeader.filename[enTexFileNameLength-1] = 0x00;

	memcpy( KanjiHeader.dummyLine , "---------------" ,0x10 );

	//漢字データ

//	Uint8* pKanjiBuffer = (Uint8*)malloc( sizeof(StKNJformat) + sizeof(StKanji)*(1024*10) );
	Uint8* pKanjiBuffer = new Uint8[ sizeof(StKNJformat) + sizeof(StKanji)*(1024*10) ];

	StKanji *pKanjiData = (StKanji *)&pKanjiBuffer[ sizeof(StKNJformat) ];

	Sint32 sPageMax = 0;

	for(Uint32 ii=0; ii<uSize; ii+=2 )
	{
		switch( pFile[ii+0] ){
		case '\r':
		case '\n':
			continue;
		}

		word[0] = pFile[ii+0];
		word[1] = pFile[ii+1];
		word[2] = pFile[ii+2];
		word[3] = pFile[ii+3];
		word[4] = 0x00;

		//-----------------------------------------------
		//漢字DB作成
		//-----------------------------------------------

		if( IsZenkaku( word ) )
		{
			//全角文字だった
			Uint16 *pWord = (Uint16*)word;
			pKanjiData->uIndex = uKanjiCnt;
			pKanjiData->uCode  = *pWord;
		}
		else
		{
			//半角文字だった
			Uint16 *pWord = (Uint16*)word;
			pKanjiData->uIndex = uKanjiCnt;
			pKanjiData->uCode  = *pWord;
		}

		pKanjiData->pg = uKanjiCnt/(16*16);

		pKanjiData->x = uKanjiCnt%16;
		pKanjiData->y = (uKanjiCnt/16)%16;

		pKanjiData->u = 0;
		pKanjiData->v = 0;
		pKanjiData->w = sFontPixel;
		pKanjiData->h = sFontPixel;

		if( pKanjiData->pg >= sPageMax ) sPageMax = pKanjiData->pg;

		uKanjiCnt ++;

		pKanjiData ++;
	}

	KanjiHeader.wordNum = uKanjiCnt;
	KanjiHeader.texNum  = sPageMax + 1;

	makeProportionData( KanjiHeader.texNum , sFontPixel , pTexName , (StKanji*)&pKanjiBuffer[ sizeof(StKNJformat) ] );

	delete[] pFile;//free( pFile );

	//-----------------------------------------------
	//ヘッダをコピー
	//-----------------------------------------------
	memcpy( pKanjiBuffer , &KanjiHeader , sizeof(KanjiHeader) );

	//-----------------------------------------------
	//漢字データをコピー
	//-----------------------------------------------
	gxChar fileName[256];
	sprintf( fileName , "%s.KNJ" , pTxtName );

	//-----------------------------------------------
	//ファイルを保存
	//-----------------------------------------------

	gxLib::SaveFile( fileName, pKanjiBuffer , sizeof(StKNJformat) + sizeof(StKanji)*(uKanjiCnt) );

	//-----------------------------------------------
	//使ったメモリを開放する
	//-----------------------------------------------
	delete[] pKanjiBuffer;//free( pKanjiBuffer );

	return gxTrue;
}


gxBool CFontManager::readKNJ( Uint8* pKNJ , Uint32 uSize , gxBool bNoneLoadTextue )
{
	//--------------------------------------------------
	//KNJファイルを分析します
	//--------------------------------------------------

	Uint32 w = 16;

	StKNJformat *pHead = (StKNJformat*)&pKNJ[0];
	StKanji     *pData = (StKanji*)&pKNJ[sizeof(StKNJformat)];

	m_uKanjiNum = pHead->wordNum;
	m_uFontSize = pHead->pixel/enFontSizeStandard-1;

	if( m_pKanjiDB ) delete[] m_pKanjiDB;

	if( m_uKanjiNum <= 0 ) return gxFalse;

	if( m_uFontSize == enFontSize32 )
	{
		w = 32;
	}

	m_pKanjiDB = new StKanji[ m_uKanjiNum ];

	Sint32 sPageMax = 0;

	for( Uint32 ii=0; ii<m_uKanjiNum; ii++ )
	{
		StKanji *pTbl;
		pTbl = &m_pKanjiDB[ ii ];

		m_uKanjiList[ pData->uCode ] = pData->uIndex;

		//gxLib::DebugLog( "0x%04x" , pData->uCode );

		pTbl->uCode  = 	pData->uCode;
		pTbl->uIndex = 	pData->uIndex;

		//テクスチャの設定
		pTbl->pg = pData->pg;

		if( m_uFontSize == enFontSize32 )
		{
			pTbl->pg = convertTpg512( pTbl->pg );
		}

		pTbl->x  = pData->x;
		pTbl->y  = pData->y;

		pTbl->u  = pData->u;
		pTbl->v  = pData->v;
		pTbl->w  = pData->w;
		pTbl->h  = pData->h;

		if( pData->pg >= sPageMax ) sPageMax = pData->pg;

		pData ++;
	}

	if( sPageMax+1 != pHead->texNum )
	{
		//テクスチャ数が合わない
		gxLib::DebugLog("[Error]テクスチャの数が合いません");
	}

	for(Sint32 ii=0;ii<=pHead->texNum;ii++)
	{
		gxChar fileName[64];
		sprintf( fileName , "%s/%s%02d.tga", KNJFilePath , pHead->filename ,ii+1 );//

		Sint32 n = ii;
		if( m_uFontSize == enFontSize32 )
		{
			n = convertTpg512( n );
		}

		if( bNoneLoadTextue ) continue;

		gxLib::LoadTexture( m_sStartTpg+n , fileName );
	}

	gxLib::UploadTexture();

	return gxTrue;

}


void CFontManager::makeProportionData( Sint32 sTexNum , Sint32 sWidth , gxChar *pTexName , StKanji *pKanjiData )
{
	//--------------------------------------------------
	// プロポーションデータを作成する
	//--------------------------------------------------
	gxChar fileName[64];

	for(Sint32 ii=0;ii<sTexNum;ii++)
	{
		CFileTarga *pTga = new CFileTarga();

		sprintf( fileName , "%s%02d.tga",pTexName ,ii+1 );
		pTga->LoadFile( fileName );

		for(Sint32 my=0;my<16;my++)
		{
			for(Sint32 mx=0;mx<16;mx++)
			{
				gxRect _rect;
				scanMoji( pTga , mx*sWidth , my*sWidth , sWidth , sWidth ,&_rect );
				pKanjiData->u = _rect.x1;
				pKanjiData->w = _rect.x2 - _rect.x1;
				pKanjiData->v = sWidth;
				pKanjiData->h = sWidth;

				pKanjiData ++;
			}
		}

		sprintf( fileName , "%s%02dx.tga",pTexName ,ii+1 );
		pTga->SaveFile(fileName );
		delete pTga;
	}

}

void CFontManager::scanMoji( CFileTarga* pTga , Sint32 mx , Sint32 my , Sint32 w,Sint32 h ,gxRect *pRect )
{
	//----------------------------------------------
	//文字の幅をスキャンする
	//_DEBUGが定義されていればフォントサイズ
	//検出用ファイルを作成します
	//----------------------------------------------

	Sint32 left  = -1;
	Sint32 right = -1;

	for(Sint32 x=0;x<w;x++)
	{
		Sint32 bTrouble = gxFalse;
		for(Sint32 y=0;y<h;y++)
		{
			Uint32 argb;
#ifdef GX_DEBUG
			argb = pTga->GetARGB( mx+x , my+y );
#endif
			argb = argb&0xff000000;

			if( argb )
			{
				bTrouble = gxTrue;
#ifdef GX_DEBUG
				pTga->SetARGB( mx+x , my+y , 0xff00ffff );
#endif
				break;
			}

#ifdef GX_DEBUG
			pTga->SetARGB( mx+x , my+y , 0xffff0000 );
#endif
		}

		if( bTrouble )
		{
			left = x-1;
			break;
		}
	}

	for(Sint32 x=0;x<w;x++)
	{
		Sint32 bTrouble = gxFalse;
		for(Sint32 y=0;y<h;y++)
		{
			Uint32 argb;
			argb = pTga->GetARGB( mx+w-x-1 , my+y );
			argb = argb&0xff000000;

			if( argb )
			{
				bTrouble = gxTrue;
				pTga->SetARGB( mx+w-x-1 , my+y , 0xff00ffff );
				break;
			}

			pTga->SetARGB( mx+w-x-1 , my+y , 0xff0000ff );
		}

		if( bTrouble )
		{
			right = w-x-1;
			break;
		}
	}


	if( right == -1 )	right = w;
	if( left == -1 )	left = 0;
	if( right < left )
	{
		left  = 4;
		right = w-4;
	}

	pRect->x1 = left;
	pRect->y1 = 0;
	pRect->x2 = right;
	pRect->y2 = h;
}


Sint32 CFontManager::convertTpg512(Sint32 page )
{
	//------------------------------------------------------------
	//256ページ番号を512サイズのテクスチャページに変換する
	//------------------------------------------------------------

	Sint32 tbl[]={0,2,8,10};
	Sint32 num = tbl[ page%4 ];
	num += (page/4)*16;

	return num;
}


Uint32 CFontManager::Print( Sint32 x , Sint32 y , Sint32 prio , Uint32 _argb ,gxChar *pStr , ... )
{
	//------------------------------------------------------------
	//詳細設定なし表示リクエスト
	//------------------------------------------------------------

	Uint32 argb = _argb;

	va_list app;

	va_start( app, pStr );
	vsprintf( s_StrTemp, pStr, app );
	va_end( app );

	StFontStat _config = m_stDefault;
	_config.argb = _argb;

	Uint32 n = printf( x , y , prio , &_config, s_StrTemp );

	return n;
}

Uint32 CFontManager::PrintEx( Sint32 x , Sint32 y , Sint32 prio , StFontStat *pStat, gxChar *pStr , ... )
{
	//------------------------------------------------------------
	//詳細設定付き表示リクエスト
	//------------------------------------------------------------

	va_list app;
	va_start( app, pStr );
	vsprintf( s_StrTemp, pStr, app );
	va_end( app );

	if( pStat == NULL ) pStat = &m_stDefault;
	Uint32 n = printf( x , y , prio , pStat, s_StrTemp );

	return n;
}

Uint32 CFontManager::GetStringWidth( StFontStat *pStat, gxChar* pStr , ...)
{
	//------------------------------------------------------------
	//表示せずに文字サイズのみ返す
	//------------------------------------------------------------

	va_list app;

	va_start( app, pStr );
	vsprintf( s_StrTemp, pStr, app );
	va_end( app );

	Uint32 n = printf( 0 ,0 , -1 , pStat, s_StrTemp );

	return n;
}

Uint32 CFontManager::printf( Sint32 x , Sint32 y , Sint32 prio , StFontStat *pStat, gxChar *pStr )
{
	//------------------------------------------------------------
	//フォントを表示する
	//prioが-1なら、表示せずにサイズのみ返す
	//------------------------------------------------------------
	StFontStat _Config;
	Sint32 sLen = strlen( pStr );
	gxChar *p8  = s_StrTemp;
	Uint32 ax,ay;
	gxChar word[16];
	Uint16 uIndex;
	StKanji *pData;
	Sint32 sOffset = 0;

	if( pStat )
	{
		_Config = *pStat;
	}
	else
	{
		_Config = m_stDefault;
	}

	ax = x;
	ay = y;

	Sint32 x1=0,y1=0,x2=64,y2=0;

	if( _Config.bRectEnable )
	{
		x1 =_Config.u;
		y1 =_Config.v;
		x2 = x1+_Config.w;
		y2 = y1+_Config.h;
	}


	Sint32 width  = enFontSizeStandard*(m_uFontSize+1);
	Sint32 height = enFontSizeStandard*(m_uFontSize+1);

	for(Sint32 ii=0;ii<sLen;ii++)
	{
		if( IsZenkaku(p8) )
		{
			sOffset = 2;
			word[0] = p8[0];
			word[1] = p8[1]&0x000000ff;
			word[2] = 0x00;

			uIndex = ((word[1]<<8)&0x0000ff00) | ((word[0]<<0)&0x000000ff);
		}
		else
		{
			sOffset = 1;
			word[0] = p8[0]&0x000000ff;
			word[1] = 0x20;
			word[2] = 0x00;

			uIndex = (word[1]<<8 )|(word[0]<<0 );
		}

		Uint32 n = 	m_uKanjiList[ uIndex ];

/*
//		_Config.argb        = ARGB( 0xff,0xff,0xff,0xff );
		_Config.bRectEnable = gxTrue;
//		_Config.sPitch      = 0;
		_Config.u = 0;
		_Config.v = 0;
		_Config.w = 0;
		_Config.h = 0;
//		_Config.fScaleX = 1.f;
//		_Config.fScaleY = 1.f;
*/
		if( n == KANJI_NONE )
		{
			//文字がなかった
			n = 0;

			if( p8[0] == '\t' )
			{
				//TABに関してだけは許してやる
				p8 +=sOffset;
				ii += (sOffset-1);
				continue;
			
			}
			//----------------------------

			if( prio != -1 ) gxLib::DrawBox( ax , ay , ax+16,ay+16 , PRIORITY_MAX , gxTrue,  ATR_DFLT , gxLib::Rand()%0xffffffff );
			p8 +=sOffset;
			ii += (sOffset-1);
			ax += 16;
			continue;
		}

		pData =	&m_pKanjiDB[ n ];

		Uint8* pMoji = (Uint8*)&pData->uCode;
		word[0] = pMoji[0];
		word[1] = pMoji[1];
		word[2] = 0x00;

		Sint32 u,v,w,h;
		u = pData->x*width;
		v = pData->y*height;
		w = width;
		h = height;

/*
		if( prio != -1 )
		{
			gxLib::PutSprite( ax-pData->u , ay , prio ,
				  m_sStartTpg+pData->pg, u, v, w,h,0,0,
				  ATR_DFLT , _Config.argb , 0, _Config.fScaleX , _Config.fScaleY );
		}
*/
		if( prio != -1 )
		{
			gxBool bDisp = gxTrue;
			Sint32 dx = ax-pData->u;
			Sint32 dy = ay;

			if( _Config.bRectEnable )
			{
				if( dy < y1 )
				{
					v -= (dy - y1)/_Config.fScaleY;
					h += (dy - y1)/_Config.fScaleY;
					dy = y1;
				}

				if( dy+h*_Config.fScaleY > y2 )
				{
					//v -= dy - y1;
					h -= ((dy+h*_Config.fScaleY) - y2)/_Config.fScaleY;
				}

				if( dx < x1 )
				{
					u -= (dx - x1)/_Config.fScaleX;
					w += (dx - x1)/_Config.fScaleX;
					dx = x1;
				}

				if( dx+w*_Config.fScaleX > x2 )
				{
					w -= ((dx+w*_Config.fScaleX) - x2)/_Config.fScaleX;
				}

				if( h < 0 || dy+h < y1 ) bDisp = gxFalse;
				if( w < 0 || dx+w < x1 ) bDisp = gxFalse;
			}

			if( bDisp )
				gxLib::PutSprite( dx , dy , prio ,
				  m_sStartTpg+pData->pg, u, v, w,h,0,0,
				  ATR_DFLT , _Config.argb , 0, _Config.fScaleX , _Config.fScaleY );
		}

		//----------------------------------------------
		//文字の幅分を詰める
		//----------------------------------------------

		Sint32 ww = 0;

		if( _Config.bProp )
		{
			//プロポーショナルフォントの場合
			ww += (Sint32)( pData->w*_Config.fScaleX );
			ww += (Sint32)( 1*_Config.fScaleX );
		}
		else
		{
			//等幅フォントの場合
			ww += (Sint32)( ( ( m_uFontSize+1 ) * enFontSizeStandard ) * _Config.fScaleX );
		}

		ww += (Sint32)( _Config.sPitch * _Config.fScaleX );	//文字間の隙間（ピクセル単位）

		ax += ww;

		p8 +=sOffset;

		ii += (sOffset-1);
	}

	return ax-x;
}


gxBool CFontManager::IsZenkaku( gxChar *s )
{
	Uint8 c = s[0];
	if ( (c>=0x81 && c<=0x9f) || (c>=0xe0 && c<=0xfc))
	{
		// 全角の第１バイト
		c = s[1];
		if (c >= 0x40 && c <= 0xfc) return gxTrue;    // 全角の第２バイト
	}
    return gxFalse;
}


void CFontManager::setPath(gxChar *pPath)
{
	sprintf(KNJFilePath, "%s", pPath);
}

