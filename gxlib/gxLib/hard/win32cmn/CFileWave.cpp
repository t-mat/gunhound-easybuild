//-----------------------------------------------------
//
// CFileWave.cpp
//
//-----------------------------------------------------

#include <gxLib.h>
#include "CFileWave.h"

#define WAVE_FORMAT_EXTENSIBLE	0xFFFE
#define WAVE_FORMAT_PCM     1

CFileWave::CFileWave()
{
	m_pWaveImage = NULL;	//WAVEデータイメージ
	m_uFormat    = 0;		//フォーマット( -1:未知のWAVE / 0:ＰＣＭ / 1:マルチチャンネル ウェーブ フォーマット / 2:圧縮WAVE )
	m_DataSize   = 0;		//WAVEデータイメージのサイズ
}


CFileWave::~CFileWave()
{
	if( m_pWaveImage ) delete[] m_pWaveImage;
	
}


gxBool CFileWave::Read( Uint8* pMemory )
{
	Uint8 *pFileImage = NULL;

	getWaveData( pMemory , &m_pWaveImage , &m_tWaveInfo );

	return gxTrue;

}


gxBool CFileWave::Load( gxChar* pFileName )
{

	Uint8 *pFileImage = NULL;
	Uint32 uLength;

	pFileImage = LoadFile( pFileName , &uLength );

	if( pFileImage == NULL ) return gxFalse;

	getWaveData( pFileImage , &m_pWaveImage , &m_tWaveInfo );

	delete[] pFileImage;

	return gxTrue;
}


Uint32 CFileWave::getWaveData( Uint8 *pFileImage, Uint8 **ppWaveImage, StWaveInfo *pWaveInfo )
{
	//WAVEデータの取得

	StWaveInfo* _pWaveInfo = pWaveInfo;

	Uint8 *pHeader	= NULL;
	Uint32 WaveType	= 0;
	Uint32 bResult	= gxTrue;
	gxChar Chunk[5]	= "";

	//--------------------------------------------------------
	// "RIFF"の文字列
	//--------------------------------------------------------
	memcpy( Chunk, pFileImage, 4 );
	pFileImage += 4;
	if ( strcmp( "RIFF", Chunk ) )
	{
		//"RIFF"じゃない
		bResult = gxFalse;
		goto EXIT_DATALOAD;
	}

	//--------------------------------------------------------
	// RIFFデータサイズ
	//--------------------------------------------------------
	pFileImage += 4;

	//--------------------------------------------------------
	// "WAVE"の文字列
	//--------------------------------------------------------
	memcpy( Chunk, pFileImage, 4 );
	pFileImage += 4;
	if ( strcmp( "WAVE", Chunk ) )
	{
		//"WAVE"じゃない
		bResult = gxFalse;
		goto EXIT_DATALOAD;
	}

	//========================================================
	// チャンク情報取得
	//========================================================
	while ( gxTrue )
	{
		Uint32 size = 0;

		memcpy( Chunk, pFileImage, 4 );
		pFileImage += 4;

		// フォーマットのチャンク

		if ( strcmp( "fmt ", Chunk ) == 0 )
		{
			size += getChunk( pFileImage, &pHeader, NULL );
			m_uFormat = (Uint16)(*pHeader);
			pFileImage += size;
		}
		else if ( strcmp( "data", Chunk ) == 0 )
		{
			// データチャンク

			size += getChunk( pFileImage, NULL/*&pFileImage*/, &m_DataSize  );	//メモリリークしてたー！
			break;
		}
		else
		{
			// ？？？
			size = *( (Uint32 *)pFileImage );
			pFileImage += 4 + size;
		}
	}

	// WAVEデータの中身確認

	WaveType = makeWaveInfo( pHeader, _pWaveInfo );

	if ( WaveType < 0 )
	{
		//知らない形式だった
		bResult = gxFalse;
		goto EXIT_DATALOAD;
	}

	//WAVEデータのみを取得する

	(*ppWaveImage) = new Uint8 [ m_DataSize ];	//毒BADALLOC
	memcpy( (*ppWaveImage), pFileImage, m_DataSize );

	// 終了
EXIT_DATALOAD:

	if ( pHeader != NULL ) delete [] pHeader;

	if( m_DataSize == 0 )
	{
		bResult = gxFalse;
	}

	return bResult;
}


Uint32 CFileWave::getChunk( Uint8 *pSrcData, Uint8 **ppDstData, Uint32 *pSize )
{
	//チャンクデータ取得

	Uint32 DataSize = 0;

	// チャンクサイズ取得
	DataSize = *( (Uint32 *)pSrcData );

	pSrcData += 4;

	if( ppDstData )
	{
		// データ受信用バッファ生成
		(*ppDstData) = new Uint8 [ DataSize ];

		// チャンクデータ取得
		memcpy( (*ppDstData), pSrcData, DataSize );
	}

	if ( pSize != NULL) *pSize = DataSize;

	return DataSize + 4;
}

Uint32 CFileWave::makeWaveInfo( Uint8 *pHeader, StWaveInfo *pWaveInfo )
{
	//Waveデータのヘッダーを解析します

	StWaveInfo* _pWaveInfo = pWaveInfo;

	Uint32 result = -1;

	memset( _pWaveInfo, 0x00 ,sizeof(StWaveInfo) );

	//=================================================
	// フォーマットごとに処理わけ
	//=================================================

	switch ( m_uFormat )
	{
	//-------------------------------------------------
	// PCM
	//-------------------------------------------------
	case WAVE_FORMAT_PCM:
		_pWaveInfo->Format.wFormatTag		= *((Uint16 *)pHeader);	pHeader += 2;
		_pWaveInfo->Format.nChannels		= *((Uint16 *)pHeader);	pHeader += 2;
		_pWaveInfo->Format.nSamplesPerSec	= *((Uint32 *)pHeader);	pHeader += 4;
		_pWaveInfo->Format.nAvgBytesPerSec	= *((Uint32 *)pHeader);	pHeader += 4;
		_pWaveInfo->Format.nBlockAlign		= *((Uint16 *)pHeader);	pHeader += 2;
		_pWaveInfo->Format.wBitsPerSample	= *((Uint16 *)pHeader);
		_pWaveInfo->Format.cbSize			= 0;
		result = 0;
		break;

	//-------------------------------------------------
	// マルチチャンネル ウェーブ フォーマット
	//-------------------------------------------------
	case WAVE_FORMAT_EXTENSIBLE:
		*_pWaveInfo = *((StWaveInfo *)pHeader);
		result = 1;
		break;

	// 圧縮WAVE
	default:
		_pWaveInfo->Format.wFormatTag		= *((Uint16 *)pHeader);	pHeader += 2;
		_pWaveInfo->Format.nChannels		= *((Uint16 *)pHeader);	pHeader += 2;
		_pWaveInfo->Format.nSamplesPerSec	= *((Uint32 *)pHeader);	pHeader += 4;
		_pWaveInfo->Format.nAvgBytesPerSec	= *((Uint32 *)pHeader);	pHeader += 4;
		_pWaveInfo->Format.nBlockAlign		= *((Uint16 *)pHeader);	pHeader += 2;
		_pWaveInfo->Format.wBitsPerSample	= *((Uint16 *)pHeader);	pHeader += 2;
		_pWaveInfo->Format.cbSize			= *((Uint16 *)pHeader);
		result = 2;
	}

	return result;
}


Uint32 CFileWave::makeWaveHeader( Uint8 *pHeader, StWaveInfo *pWaveInfo )
{
	//Info情報からWaveデータのヘッダーを作成します

	//

	return 0;
}

