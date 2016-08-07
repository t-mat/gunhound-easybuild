//-------------------------------------------------------------------
//
// gxLib.cpp
//
// このラッピング関数群では絶対に機種依存のコードを書かないこと!!!!!
//
//-------------------------------------------------------------------
#include <gxLib.h>
#include "gx.h"
#include "gxOrderManager.h"
#include "gxTexManager.h"
#include "gxPadManager.h"
#include "gxRender.h"
#include "gxSoundManager.h"
#include "gxMovieManager.h"
#include "gxWebManager.h"
#include "gxDebug.h"
/*
【裏ルール】

　PutSpriteのテクスチャページに負の値を設定すると特殊な効果がある。
　enNoneTexturePage・・・ノンテクスチャポリゴン
　enNoneWiredPage　・・・外周部位を取り囲むライン

　通常のボックス処理と似ているが、回転/拡大/縮小が可能。

*/
#define INVALID_PAD_ID (0xff)
#define TOUCH_MAXIMUM  (16)

gxLib::StSaveData gxLib::SaveData;
static StOrder s_Order;

enum {
	enStringMaxSize   = 0xff,
	enNoneTexturePage = -1,
	enNoneWiredPage   = -2,
};

Uint32 gxLib::PutSprite(
		gxSprite *pSpr,
		Sint32 x,		Sint32 y,	Sint32 prio,
		Uint32 atr,	Uint32 col,	Float32 r,
		Float32 sx,		Float32 sy )
{
	return PutSprite(x,y,prio,pSpr->page,pSpr->u,pSpr->v,pSpr->w,pSpr->h,pSpr->cx,pSpr->cy,atr,col,r,sx,sy);
}


Uint32 gxLib::PutSprite(
		Sint32 x,		Sint32 y,	Sint32 prio,
		Sint32 page,	Sint32 u, 	Sint32 v,	Sint32 w,	Sint32 h,
		Sint32 cx,		Sint32 cy,	Uint32 atr,	Uint32 col,	Float32 r,
		Float32 sx,		Float32 sy )
{
	s_Order.sType = enOrderTypeTexturePolygon;

	if( page <= enNoneWiredPage )
	{
		s_Order.sType = enOrderTypWired;
	}
	else if( page <= enNoneTexturePage )
	{
		s_Order.sType = enOrderTypeNoneTexPolygon;
	}

	s_Order.sAtribute = atr;
	s_Order.x  = x;
	s_Order.y  = y;
	s_Order.pr = prio;

	s_Order.sx = sx;
	s_Order.sy = sy;
	s_Order.sz = 0.f;

	s_Order.rx = 0.f;
	s_Order.ry = 0.f;
	s_Order.rz = r;

	s_Order.pg = page;
	s_Order.u = u;
	s_Order.v = v;
	s_Order.w = w;
	s_Order.h = h;
	s_Order.cx = cx;
	s_Order.cy = cy;

	s_Order.color[0] = col;
	s_Order.color[1] = col;
	s_Order.color[2] = col;
	s_Order.color[3] = col;

	s_Order.pString = NULL;

	COrderManager::GetInstance()->set( &s_Order );

	return 0;
}

Uint32 gxLib::DrawPoint( Sint32 x1,	Sint32 y1, Sint32 prio, Uint32 atr, Uint32 col )
{
	//ポイント

	s_Order.sType = enOrderTypePoint;

	s_Order.sAtribute = atr;
	s_Order.x  = 0;
	s_Order.y  = 0;
	s_Order.pr = prio;

	s_Order.x1[0]  = (Float32)x1;
	s_Order.y1[0]  = (Float32)y1;
	s_Order.color[0] = col;

	s_Order.pString = NULL;

	COrderManager::GetInstance()->set( &s_Order );

	return 0;
}


Uint32 gxLib::DrawLine(
		Sint32 x1,	Sint32 y1,
		Sint32 x2 , Sint32 y2 , 
		Sint32 prio,
		Uint32 atr, Uint32 col , Float32 width )
{
	//ライン

	s_Order.sType = enOrderTypeLine;

	s_Order.sAtribute = atr;
	s_Order.x  = 0;
	s_Order.y  = 0;
	s_Order.pr = prio;

	{
		s_Order.opt  = 0;

		s_Order.x1[0]  = (Float32)x1;
		s_Order.y1[0]  = (Float32)y1;

		s_Order.x1[1]  = (Float32)x2;
		s_Order.y1[1]  = (Float32)y2;
	}

	s_Order.color[0] = col;
	s_Order.color[1] = col;
	s_Order.color[2] = col;
	s_Order.color[3] = col;

	//太さに対応
	s_Order.sx = width;

	s_Order.pString = NULL;

	COrderManager::GetInstance()->set( &s_Order );

	return 0;

}


Uint32 gxLib::DrawTriangle(
		Sint32 x1 ,	Sint32 y1 ,
		Sint32 x2 , Sint32 y2 ,
		Sint32 x3 , Sint32 y3 ,
		Sint32 prio,
		gxBool bFill,
		Uint32 atr,	Uint32 argb )
{

	if( bFill )
	{
		//トライアングルフィル

		s_Order.sType = enOrderTypeTriangle;

		s_Order.sAtribute = atr;
		s_Order.x  = 0;
		s_Order.y  = 0;
		s_Order.pr = prio;

		s_Order.opt  = 1;

		s_Order.x1[0]  = (Float32)x1;
		s_Order.y1[0]  = (Float32)y1;

		s_Order.x1[1]  = (Float32)x2;
		s_Order.y1[1]  = (Float32)y2;

		s_Order.x1[2]  = (Float32)x3;
		s_Order.y1[2]  = (Float32)y3;

		s_Order.color[0] = argb;
		s_Order.color[1] = argb;
		s_Order.color[2] = argb;

		s_Order.pString = NULL;

		COrderManager::GetInstance()->set( &s_Order );
	}
	else
	{
		DrawLine( x1 , y1 , x2 , y2 , prio , atr , argb );
		DrawLine( x2 , y2 , x3 , y3 , prio , atr , argb );
		DrawLine( x3 , y3 , x1 , y1 , prio , atr , argb );
	}

	return 0;

}


Uint32 gxLib::DrawColorBox(
		Sint32 x1 ,	Sint32 y1 ,Uint32 argb1,
		Sint32 x2 , Sint32 y2 ,Uint32 argb2,
		Sint32 x3 , Sint32 y3 ,Uint32 argb3,
		Sint32 x4 , Sint32 y4 ,Uint32 argb4,
		Sint32 prio,
		Uint32 atr )
{
	
	Uint32 _argb[3];
	
	_argb[0] = argb1;
	_argb[1] = argb2;
	_argb[2] = argb4;
	
	DrawColorTriangle(
		x1,y1, _argb[0],
		x2,y2, _argb[1],
		x4,y4, _argb[2],
		prio,atr );

	_argb[0] = argb2;
	_argb[1] = argb4;
	_argb[2] = argb3;

	DrawColorTriangle(
		x2,y2, _argb[0],
		x4,y4, _argb[1],
		x3,y3, _argb[2],
		prio,atr );

	return 0;
}


Uint32 gxLib::DrawColorTriangle(
		Sint32 x1 ,	Sint32 y1 ,Uint32 argb1,
		Sint32 x2 , Sint32 y2 ,Uint32 argb2,
		Sint32 x3 , Sint32 y3 ,Uint32 argb3,
		Sint32 prio,
		Uint32 atr )
{
	//トライアングルフィル

	s_Order.sType = enOrderTypeTriangle;

	s_Order.sAtribute = atr;
	s_Order.x  = 0;
	s_Order.y  = 0;
	s_Order.pr = prio;

	s_Order.opt  = 1;

	s_Order.x1[0]  = (Float32)x1;
	s_Order.y1[0]  = (Float32)y1;

	s_Order.x1[1]  = (Float32)x2;
	s_Order.y1[1]  = (Float32)y2;

	s_Order.x1[2]  = (Float32)x3;
	s_Order.y1[2]  = (Float32)y3;

	s_Order.color[0] = argb1;
	s_Order.color[1] = argb2;
	s_Order.color[2] = argb3;

	s_Order.pString = NULL;

	COrderManager::GetInstance()->set( &s_Order );

	return 0;
}


Uint32 gxLib::PutTriangle(
		Sint32 x1 ,	Sint32 y1 , Sint32 u1, Sint32 v1 ,
		Sint32 x2 , Sint32 y2 , Sint32 u2, Sint32 v2 ,
		Sint32 x3 , Sint32 y3 , Sint32 u3, Sint32 v3 ,
		Sint32 tpg,
		Sint32 prio,
		Uint32 atr,	Uint32 argb )
{
	//トライアングルフィル

	if( tpg == enNoneTexturePage )
	{
		Uint32 n =gxLib::DrawTriangle(	x1 ,y1 ,	x2 ,y2 ,	x3 ,y3 ,	prio,	gxTrue,	atr , argb );

		return n;
	}

	s_Order.sType = enOrderTypeTextureTriangle;

	s_Order.sAtribute = atr;
	s_Order.x  = 0;
	s_Order.y  = 0;
	s_Order.pr = prio;

	s_Order.opt  = 0;

	s_Order.x1[0]  = (Float32)x1;
	s_Order.y1[0]  = (Float32)y1;
	s_Order.u1[0]   = (Float32)u1;
	s_Order.v1[0]   = (Float32)v1;

	s_Order.x1[1]  = (Float32)x2;
	s_Order.y1[1]  = (Float32)y2;
	s_Order.u1[1]   = (Float32)u2;
	s_Order.v1[1]   = (Float32)v2;

	s_Order.x1[2]  = (Float32)x3;
	s_Order.y1[2]  = (Float32)y3;
	s_Order.u1[2]   = (Float32)u3;
	s_Order.v1[2]   = (Float32)v3;

	s_Order.color[0] = argb;
	s_Order.color[1] = argb;
	s_Order.color[2] = argb;

	s_Order.pg = tpg;

	s_Order.pString = NULL;

	COrderManager::GetInstance()->set( &s_Order );

	return 0;

}


Uint32 gxLib::DrawBox(
		Sint32 x1,		Sint32 y1,	Sint32 x2 , Sint32 y2 , Sint32 prio,
		gxBool bFill,
		Uint32 atr,	Uint32 argb,
		Sint32 cx , Sint32 cy,
		Float32 r,	Float32 sx,	Float32 sy )
{

	//ボックス

	if( x1 > x2 )	SWAP( x1 , x2 );
	if( y1 > y2 )	SWAP( y1 , y2 );

	if( bFill )
	{
		PutSprite(	x1,	y1,	prio,	enNoneTexturePage,0,0,x2-x1,y2-y1,	cx,cy,atr,argb,r,sx,sy );
		return 0;
	}
	else
	{
		PutSprite(	x1,	y1,	prio,	enNoneWiredPage,0,0,x2-x1,y2-y1,	cx,cy,atr,argb,r,sx,sy );
	}

	return 0;
}


gxBool gxLib::LoadTexture( Uint32 texPage , const gxChar* fileName , Uint32 colorKey , Uint32 ox , Uint32 oy , Sint32 *w , Sint32 *h )
{
	CTexManager::GetInstance()->LoadTexture( texPage , fileName , colorKey , ox , oy , w , h );

	return gxTrue;
}

gxBool gxLib::ReadTexture( Uint32 texPage , const Uint8* pBuffer , Uint32 pSize , Uint32 colorKey , Uint32 ox , Uint32 oy , Sint32 *w , Sint32 *h )
{
	CFileTarga tga;

	tga.ReadFile( pBuffer , pSize , colorKey );

	CTexManager::GetInstance()->addTexture( texPage , &tga ,colorKey , ox , oy , w , h );

	return gxTrue;
}


gxBool gxLib::UploadTexture(gxBool bForce)
{
	//テクスチャをアップロード

	CTexManager::GetInstance()->UploadTexture(bForce);

	return gxTrue;
}

StJoyStat* gxLib::Joy(Uint32 n)
{
	//コントローラーの取得

	return CPadManager::GetInstance()->GetJoyStatus( n );
}

Uint8 gxLib::KeyBoard( Uint32 n )
{
	return CPadManager::GetInstance()->GetKeyBoardStatus( n );
}


void gxLib::Printf( Sint32 x , Sint32 y , Sint32 prio , Uint32 atr , Uint32 argb , gxChar* pFormat , ... )
{
	//文字列の格納

	gxChar _buf[ enStringMaxSize ];
	va_list app;

	va_start( app, pFormat );
	vsprintf( _buf , pFormat, app );
	va_end( app );

	s_Order.sType = enOrderTypeFont;

	if( atr&ATR_STR_CENTER )
	{
		Sint32 len = strlen(_buf);
		x -= (len*6)/2;
	}
	else if( atr&ATR_STR_RIGHT )
	{
		Sint32 len = strlen(_buf);
		x -= (len*6);
	}

	s_Order.sAtribute = atr;
	s_Order.x  = x;
	s_Order.y  = y;
	s_Order.pr = prio;


	s_Order.color[0] = argb;
	s_Order.color[1] = argb;
	s_Order.color[2] = argb;
	s_Order.color[3] = argb;

	s_Order.pString = _buf;

	COrderManager::GetInstance()->set( &s_Order );

}


gxBool gxLib::LoadAudio( Uint32 uIndex , const gxChar* pFileName )
{

	CSoundManager::GetInstance()->LoadAudioFile( uIndex , pFileName );

	return gxTrue;
}


gxBool gxLib::ReadAudio( Uint32 uIndex , const Uint8* pMemory ,Uint32 uSize )
{

	CSoundManager::GetInstance()->ReadAudioFile( uIndex , pMemory , uSize );

	return gxTrue;
}


gxBool gxLib::PlayAudio( Uint32 uIndex , gxBool bLoop , gxBool bOverWrap , Uint32 uFrm )
{

	if( uFrm )
	{
		CSoundManager::GetInstance()->PlayAudio( uIndex , 1.0f , bOverWrap, bLoop );
		CSoundManager::GetInstance()->SetFade( uIndex , 1.0f , uFrm );
	}
	else
	{
		CSoundManager::GetInstance()->PlayAudio( uIndex , 1.0f , bOverWrap, bLoop );
		CSoundManager::GetInstance()->SetFade( uIndex , 1.0f , 0 );
	}

	return gxTrue;
}


/*
gxBool gxLib::SetAudioFadeIn( Uint32 uIndex , gxBool bLoop , Float32 fTgtVolume ,Uint32 uFrm )
{
	CSoundManager::GetInstance()->PlayAudio( uIndex , 0.f , gxFalse , bLoop );
	CSoundManager::GetInstance()->SetFade( uIndex , fTgtVolume , uFrm );

	return gxTrue;
}


gxBool gxLib::SetAudioFadeOut( Uint32 uIndex , Uint32 uFrm )
{

	CSoundManager::GetInstance()->SetFade( uIndex , 0.f , uFrm );

	return gxTrue;
}
*/


gxBool gxLib::SetAudioVolume( Uint32 uIndex , Float32 fVol )
{
	CSoundManager::GetInstance()->SetVolume( uIndex , fVol );
	return gxTrue;
}


gxBool gxLib::ChangeAudioPitch(Uint32 uIndex, Float32 fRatio)
{
	CSoundManager::GetInstance()->SetFreq(uIndex, fRatio);

	return gxTrue;
}


gxBool gxLib::StopAudio( Uint32 uIndex , Uint32 uFrm )
{
	if( uFrm )
	{
		CSoundManager::GetInstance()->SetFade( uIndex , 0.f , uFrm );
	}
	else
	{
		CSoundManager::GetInstance()->StopAudio( uIndex );
	}

	return gxTrue;
}


gxBool gxLib::IsAudioPlay( Uint32 uIndex )
{
	return CSoundManager::GetInstance()->IsPlay( uIndex );
}


gxBool gxLib::SetAudioMasterVolume( Float32 fVolume )
{
	if( fVolume < 0.f ) fVolume = 0.f;
	if( fVolume > 1.f ) fVolume = 1.f;

	CSoundManager::GetInstance()->SetMasterVolumeLevel( fVolume );

	return gxTrue;
}


void gxLib::DebugLog( gxChar* pFormat , ... )
{
#if _MASTERVERSION_

#else
	gxChar _buf[ enStringMaxSize ];
	va_list app;

	if( pFormat == NULL ) return;

	va_start( app, pFormat );

	if( vsprintf( _buf , pFormat, app ) >= enStringMaxSize )
	{
		va_end( app );
		return;
	}

	va_end( app );

	LogDisp( _buf );
#endif
}


Float32 gxLib::Cos( Float32 deg )
{
	Float32 l;

	l = cos( DEG2RAD(deg) );

	return l;
}


Float32 gxLib::Sin( Float32 deg )
{
	Float32 l;

	l = sin( DEG2RAD(deg));

	return l;
}


Float32 gxLib::Atan( Float32 x ,Float32 y )
{
	Float32 r = atan2( y , x );

	return RAD2DEG(r);

}


Float32 gxLib::Distance( Float32 x ,Float32 y )
{
	x = x/100.f;
	y = y/100.f;

	Float32 dist = (Float32)sqrt(x*x+y*y);
	return dist*100.f;

	return (Float32)sqrt(x*x+y*y);
}

Float32 gxLib::Distance( Float32 x1 ,Float32 y1 , Float32 x2 ,Float32 y2 )
{
	x1 = (x1-x2)/100.f;
	y1 = (y1-y2)/100.f;

	Float32 dist = (Float32)sqrt(x1*x1+y1*y1);

	return dist*100.f;
}


gxBool gxLib::SaveConfig()
{
	//コンフィグファイルをセーブする

	gxBool bSuccess = gxFalse;

	bSuccess = gxLib::SaveFile( FILENAME_CONFIG , (Uint8*)&SaveData , sizeof(StSaveData) );

	return bSuccess;
}

gxBool gxLib::LoadConfig()
{
	//コンフィグファイルをロードする

	Uint32 uSize=0;
	Uint8 *p = NULL;
	p = gxLib::LoadFile( FILENAME_CONFIG , &uSize );

	if( sizeof(StSaveData) != uSize )
	{
		//バージョンが変わっている場合も無いとみなす
		free( p );
		p = NULL;
	}

	if( p == NULL )
	{
		//デフォルト設定を作る
		memset(&SaveData, 0x00, sizeof(StSaveData) );

//		for(Uint32 ii=0;ii<64;ii++)
//		{
//			//コントローラー関連はINVALID_PAD_IDで初期化する
//			//そうしないとID=0 : BTN = 0の奴が認識できなくなるため
//			SaveData.m_sPadConfig[ii][0] = INVALID_PAD_ID;
//			SaveData.m_sPadConfig[ii][1] = INVALID_PAD_ID;
//		}
		for(Sint32 jj=0;jj<BTN_MAX;jj++)
		{
			for(Sint32 id=0;id<PLAYER_MAX;id++)
			{
				SaveData.Assign[ id ][ jj ].uDevice = INVALID_PAD_ID;
				SaveData.Assign[ id ][ jj ].uButton = INVALID_PAD_ID;
			}
		}
		return gxFalse;
	}
	else
	{
		memcpy(&SaveData, p , sizeof(StSaveData) );
	}

	if( p )
	{
		free( p );
	}

	return gxTrue;
}

Uint8* gxLib::LoadFile( const gxChar* pFileName , Uint32* pLength )
{
	return ::LoadFile( pFileName , pLength );
}


gxBool gxLib::SaveFile( const gxChar* pFileName , Uint8* pData ,Uint32 uSize )
{
	return ::SaveFile( pFileName , pData , uSize );
}


Uint8* gxLib::LoadStorage( const gxChar* pFileName , Uint32* pLength )
{
	return ::LoadStorageFile( pFileName , pLength );
}


gxBool gxLib::SaveStorage( const gxChar* pFileName , Uint8* pData ,Uint32 uSize )
{
	return ::SaveStorageFile( pFileName , pData , uSize );
}


Uint32 gxLib::Rand( Uint32 n )
{
	//ランダム生成：：xorshift法

    static Uint32 x=123456789,y=362436069,z=521288629,w=88675123;
	Uint32 t;
	Uint32 r;

	if( n )
	{
		n = abs((int)n);
		//ランダムを初期化
		 x = 123456789 + n;
		 y = 362436069 + n;
		 z = 521288629 + n;
		 w = 88675123  + n;
	}

    t=(x^(x<<11));
    x=y;
    y=z;
    z=w;

	r = ( w=(w^(w>>19))^(t^(t>>8)) );

	Sint32 rnd = r;

	return abs(rnd);

}

void gxLib::SetBgColor(Uint32 argb)
{
	CRender::GetInstance()->SetClearColor( argb );
}


Float32 gxLib::Sqrt( Float32 n )
{
	return (Float32)sqrt(n);
}


Uint32 gxLib::GetGameCounter()
{
	//メインループを通るたびに加算されるゲームカウンターの値を得る
	return CGameGirl::GetInstance()->GetCounter();
}


Uint8* gxLib::LoadWebFile( gxChar* pURL , Uint32* pLength , gxChar* pUser , gxChar* pPassword)
{
	//--------------------------------------------------
	//WEBのデータを取得する
	//取得したデータは「必ず」freeすること！！！！
	//--------------------------------------------------

	gxWebManager *pWebManager = NULL;
	pWebManager = new gxWebManager();
	pWebManager->Open( pURL , pUser , pPassword );

	do{
		pWebManager->Read();
	}
	while( !pWebManager->IsReadEnd() );

	if( pWebManager->IsError() )
	{
		return NULL;
	}

	Uint8 *pData = pWebManager->GetDataImage();

	*pLength = pWebManager->GetDataSize();

    if( pData == NULL || *pLength == 0 ) return NULL;
    
    pData[ *pLength-1 ] = 0x00;

	delete pWebManager;

	return pData;
}


void gxLib::CreateThread( void (*pFunc)(void*) , void * pArg )
{
	//--------------------------------------------------
	//スレッドを作成します
	//作成できるスレッド開始関数は
	//void func(Sint32)
	//です
	//--------------------------------------------------

	MakeThread( pFunc , pArg );

}

StTouch* gxLib::Touch( Sint32 n )
{
	if( n < 0 || n >= TOUCH_MAXIMUM )
	{
		return NULL;
	}

	return CPadManager::GetInstance()->GetTouchStatus( n );
}



gxBool gxLib::LoadMovie( Sint32 uIndex , gxChar *pFileName , Sint32 texPage , Sint32 uAudioIndex , gxChar *AudioFileName )
{
	if( AudioFileName )
	{
		gxLib::LoadAudio( uAudioIndex , AudioFileName );
		CMovieManager::GetInstance()->BindMusic( uIndex , uAudioIndex );
	}

	CMovieManager::GetInstance()->LoadMovie( uIndex , pFileName );
	CMovieManager::GetInstance()->BindTexturePage( uIndex , texPage );

	return gxTrue;
}


gxBool gxLib::PlayMovie( Sint32 uIndex , gxBool bLoop )
{
	Sint32 index = 0;

	CMovieFile *pMovie;

	pMovie = CMovieManager::GetInstance()->GetMovie( uIndex );

	if( pMovie )
	{
		index = pMovie->m_uMusicBindIndex;

		if ( index >= 0 )	gxLib::PlayAudio( index );

		CMovieManager::GetInstance()->PlayMovie( uIndex );

		if( bLoop )
		{
			CMovieManager::GetInstance()->SetLoop( uIndex , bLoop );
		}

		CMovieManager::GetInstance()->SetFrame(uIndex , 0 );

		CMovieManager::GetInstance()->SetAutoFrame(uIndex , gxTrue );

	}
	else
	{
		return gxFalse;
	}
	return gxTrue;

}


gxBool gxLib::StopMovie( Sint32 uIndex )
{
	Sint32 index = 0;

	CMovieFile *pMovie;

	pMovie = CMovieManager::GetInstance()->GetMovie( uIndex );

	if( pMovie )
	{
		index = pMovie->m_uMusicBindIndex;

		if ( index >= 0 )	gxLib::StopAudio( index );

		CMovieManager::GetInstance()->StopMovie( uIndex );
	}
	else
	{
		return gxFalse;
	}

	return gxTrue;
}


gxBool gxLib::StillMovie( Sint32 uIndex , Sint32 frmNo )
{
	//動画の特定フレームを再生する

	Sint32 index = 0;

	CMovieFile *pMovie;

	pMovie = CMovieManager::GetInstance()->GetMovie( uIndex );

	if( pMovie )
	{
		//index = pMovie->m_uMusicBindIndex;

		//if ( index >= 0 )	gxLib::StopAudio( index );

		CMovieManager::GetInstance()->SetLoop( uIndex , gxFalse );

		CMovieManager::GetInstance()->SetFrame( uIndex , frmNo );

		CMovieManager::GetInstance()->SetAutoFrame( uIndex , gxFalse );

	}
	else
	{
		return gxFalse;
	}
	return gxTrue;

}



Uint32 gxLib::GetTime( Sint32 *pYear , Sint32 *pMonth , Sint32 *pDay , Sint32 *pHour , Sint32 *pMin , Sint32 *pSec , Sint32 *pMSec )
{
	::Clock();

	Sint32 Year , Month , Day , Hour , Min , Sec , MSec;

	Uint32 uSec;
	uSec = CGameGirl::GetInstance()->GetTime( &Year , &Month , &Day , &Hour , &Min , &Sec , &MSec );

	if( pYear  ) *pYear  = Year;
	if( pMonth ) *pMonth = Month;
	if( pDay   ) *pDay   = Day;
	if( pHour  ) *pHour  = Hour;
	if( pMin   ) *pMin   = Min;
	if( pSec   ) *pSec   = Sec;
	if( pMSec  ) *pMSec  = MSec;

	
	Uint32 now;

	now = (Hour *60*60*1000 + Min*60*1000 + Sec*1000 + MSec - CGameGirl::GetInstance()->GetStartTime());

	return  now;
}


//コントローラーを振動させる
void gxLib::SetRumble( Sint32 playerID , Sint32 bigMotorFrm , Sint32 smallMotorFrm )
{
	//止めるときは両方ゼロで

	CPadManager::GetInstance()->SetRumble( playerID , 0 , 1.0f , bigMotorFrm );
	CPadManager::GetInstance()->SetRumble( playerID , 1 , 1.0f , smallMotorFrm );
}


gxBool gxLib::IsDebugSwitchOn( Sint32 n )
{
	if( !gxDebug::GetInstance()->m_bMasterDebugSwitch ) return gxFalse;

	return (gxDebug::GetInstance()->m_DebugSwitch[ n ] > 0 )? gxTrue : gxFalse;
}

void gxLib::SetDebugSwitch( Sint32 n , gxBool bOn , gxBool bToggle )
{
	if( bToggle )
	{
		if( gxDebug::GetInstance()->m_DebugSwitch[ n ] > 0 )
		{
			gxDebug::GetInstance()->m_DebugSwitch[ n ] = 0;
		}
		else
		{
			gxDebug::GetInstance()->m_DebugSwitch[ n ] = 1;
		}
		return;
	}

	gxDebug::GetInstance()->m_DebugSwitch[ n ] = (bOn>0)? 1 : 0;
}


