//--------------------------------------------------------
//
// dxSound.cpp
//
// サウンドのループ再生を別スレッドで監視しています
// 最初からの再生はプチノイズが混入するため、4ミリ秒の頭出しを行なっています
//
//--------------------------------------------------------

#include <gxLib.h>
#include <gxLib/gx.h>
#include <gxLib/hard/machine.h>
#include <gxLib/gxOrderManager.h>
#include <gxLib/gxTexManager.h>
#include <gxLib/gxSoundManager.h>
#include <gxLib/gxRender.h>
#include "../win32cmn/CFileWave.h"
#include "../win32cmn/win32cmn.h"
#include "dxSound.h"

#pragma comment(lib, "dxguid.lib")

SINGLETON_DECLARE_INSTANCE( CDxSound )

//#define SOUND_THREAD

gxBool g_bThreadEnable = gxFalse;
void ThreadFunc( void* n );

CDxSound::CDxSound()
{
	m_pPrimaryBuffer = NULL;
	m_sMasterVolume = 80;

	for(int i=0;i<enSoundMax;i++)
	{
		m_pSoundBuffer[i] = NULL;
	}

	CoInitialize( NULL );

	g_bThreadEnable = gxFalse;
}


CDxSound::~CDxSound()
{
	for(int i=0;i<enSoundMax;i++)
	{
		SAFE_RELEASE( m_pSoundBuffer[i] );
	}

	if( m_pPrimaryBuffer ) m_pPrimaryBuffer->Release();
	if( m_pDirectSound )   m_pDirectSound->Release();

	CoUninitialize();

	g_bThreadEnable = gxFalse;
}


gxBool CDxSound::Init()
{
	HRESULT hr;
	hr = CoCreateInstance(
				CLSID_DirectSound8,
				NULL, 
				CLSCTX_ALL,
				IID_IDirectSound8,
				(void **)&m_pDirectSound );

	if FAILED( hr )
	{
		return gxFalse;
	}

	// 初期化

	hr = m_pDirectSound->Initialize( NULL );

	if FAILED( hr )
	{
		return gxFalse;
	}

	// 協調レベルの設定

	if( FAILED( m_pDirectSound->SetCooperativeLevel( g_pWindows->m_hWindow, DSSCL_PRIORITY ) ) )
	{
		if( FAILED( m_pDirectSound->SetCooperativeLevel( g_pWindows->m_hWindow, DSSCL_NORMAL ) ) )
		{
			return gxFalse;
		}
	}

	hr = m_pDirectSound->Compact();

	if( FAILED( hr ) )
	{
		return gxFalse;
	}

	// プライマリサウンドバッファ生成

	DSBUFFERDESC desc = { sizeof(DSBUFFERDESC) };
	desc.dwFlags = DSBCAPS_PRIMARYBUFFER;

	if( FAILED( m_pDirectSound->CreateSoundBuffer( &desc, &m_pPrimaryBuffer, NULL ) ) )
	{
		return gxFalse;
	}

	// プライマリバッファフォーマット

	WAVEFORMATEX wfx = { 0 };
	wfx.wFormatTag		= 1;
	wfx.nChannels		= 2;
	wfx.nSamplesPerSec	= 44100;
	wfx.nAvgBytesPerSec	= 44100 * 4;
	wfx.nBlockAlign		= 4;
	wfx.wBitsPerSample	= 16;
	wfx.cbSize			= 0;

	hr = m_pPrimaryBuffer->SetFormat( &wfx );

	return gxTrue;
}


gxBool CDxSound::Action()
{
	// 

#ifdef SOUND_THREAD
	if( !g_bThreadEnable )
	{
		g_bThreadEnable = gxTrue;

		Sint32 arg = 0;
		MakeThread( ThreadFunc , (void*)&arg );
	}
#endif

	Float32 fMasterVolume = CSoundManager::GetInstance()->GetMasterVolume();
	Float32 fForceVolume  = CSoundManager::GetInstance()->GetForceVolume();

	for(int i=0;i<enSoundMax;i++)
	{
		unsigned long pdwStatus;

		if( !CSoundManager::GetInstance()->GetPlayInfo( i )->bUse ) continue;

		if( m_pSoundBuffer[i] == NULL ) continue;
		m_pSoundBuffer[i]->GetStatus( &pdwStatus );

		if( pdwStatus & DSBSTATUS_BUFFERLOST )
		{
			//	バッファは失われており、再生やロックを行う前に復元しなければならない。
		}
		if( pdwStatus & DSBSTATUS_LOOPING )
		{
			//	バッファはループしている。この値が設定されていない場合、バッファはサウンド データの最後に達すると停止する。この値は、DSBSTATUS_PLAYING との組み合わせでのみ返される。
			CSoundManager::GetInstance()->PlayNow( i , gxTrue );
		}
		if( pdwStatus & DSBSTATUS_PLAYING )
		{
			//	バッファは再生中である。この値が設定されていない場合、バッファは停止している。
			CSoundManager::GetInstance()->PlayNow( i , gxTrue );
		}
		else
		{
			CSoundManager::GetInstance()->PlayNow( i , gxFalse );

			if( CSoundManager::GetInstance()->GetPlayInfo( i )->bLoop )
			{
				CSoundManager::GetInstance()->PlayNow( i , gxTrue );
				CSoundManager::GetInstance()->GetPlayInfo( i )->bReq    = gxTrue;
				CSoundManager::GetInstance()->GetPlayInfo( i )->uStatus |= enSoundReqPlay;
			}
		}
		if( pdwStatus & DSBSTATUS_LOCSOFTWARE)
		{
			//	バッファはソフトウェアで再生中である。DSBCAPS_LOCDEFER フラグで作成したバッファにのみ設定される。
		}

		if( pdwStatus & DSBSTATUS_LOCHARDWARE)
		{
			//	バッファはハードウェアで再生中である。DSBCAPS_LOCDEFER フラグで作成したバッファにのみ設定される。
		}

		if( pdwStatus & DSBSTATUS_TERMINATED)
		{
			//	ボイス管理がバッファを早めに終了し、バッファは停止している。DSBCAPS_LOCDEFER フラグで作成したバッファにのみ設定される。
		}

		Float32 fVol = CSoundManager::GetInstance()->GetPlayInfo( i )->fVolume;
		Sint32 vol = (Sint32)(DSBVOLUME_MIN* (1.f - (fVol*fMasterVolume*fForceVolume)));

		if ( fVol*100.f > 100.f )
		{
			fVol = 0.f;
		}
		else if ( fVol*100.f < 0.f )
		{
			fVol = -10000.f;
		}
		else
		{
			fVol = -(Float32)pow( 100.0f, (Float32)(log10(100-fVol)) );
		}
		m_pSoundBuffer[ i ]->SetVolume( vol );

	}

	Play();

	return gxTrue;
}


void CDxSound::Play()
{
	for(Sint32 ii=0; ii<MAX_SOUND_NUM; ii++)
	{
		StPlayInfo *pInfo;
		pInfo = CSoundManager::GetInstance()->GetPlayInfo( ii );
		if( pInfo->bReq )
		{
			if( pInfo->uStatus & enSoundReqStop )
			{
				StopSound( ii );
			}
			if( pInfo->uStatus & enSoundReqLoad )
			{
				read( ii );
			}
			if( pInfo->uStatus & enSoundReqPlay )
			{
				PlaySound( ii , pInfo->bLoop , pInfo->bOverWrap , pInfo->fVolume );
			}
			if( pInfo->uStatus & enSoundReqVolume )
			{
				SetVolume( ii );
			}
			if (pInfo->uStatus & enSoundReqChangeFreq)
			{
				SetFrequency(ii);
			}

			pInfo->bReq = gxFalse;
			pInfo->uStatus = enSoundReqNone;
		}

	}
}

gxBool CDxSound::read( Uint32 no )
{
	DSBUFFERDESC			desc			= { sizeof(DSBUFFERDESC) };
	IDirectSoundBuffer		*pTempBuff		= NULL;
	unsigned char			*pData			= NULL;
	void					*pSoundData1	= NULL;
	void					*pSoundData2	= NULL;
	unsigned long			size1			= 0;
	unsigned long			size2			= 0;
	unsigned long			DataSize		= 0;
	BOOL					IsLoadResource	= gxFalse;
	HRSRC					hFound			= NULL;
	HGLOBAL					hRes			= NULL;
	HRESULT					hr				= S_OK;


	//=====================================================
	// Waveファイルフォーマット情報とデータ取得
	//=====================================================

	if(m_pSoundBuffer[no])
	{
		SAFE_RELEASE( m_pSoundBuffer[no] );
	}

	if( CSoundManager::GetInstance()->GetPlayInfo( no )->m_pWave )
	{
		delete CSoundManager::GetInstance()->GetPlayInfo( no )->m_pWave;
		CSoundManager::GetInstance()->GetPlayInfo( no )->m_pWave = NULL;
	}

	CSoundManager::GetInstance()->GetPlayInfo( no )->m_pWave = new CFileWave();		//ちゃんと削除すること（CDxSound::read( Uint32 no )で処理）

	Sint32 sType = getFormat( (Uint8*)CSoundManager::GetInstance()->GetPlayInfo( no )->m_pTempBuffer );

	Uint8 *pWaveBuffer = NULL;

	if( sType == enSoundFormatOGG )
	{
		pWaveBuffer = readOgg(
								(Uint8*)CSoundManager::GetInstance()->GetPlayInfo( no )->m_pTempBuffer , 
								CSoundManager::GetInstance()->GetPlayInfo( no )->m_uTempSize );

		if( pWaveBuffer == NULL )
		{
			gxLib::DebugLog("gxLib::OGGのデコード、もしくはメモリ確保に失敗した。");
			goto EXIT_LOAD;
		}
	}
	else if ( sType == enSoundFormatWAVE ) 
	{
		gxLib::DebugLog("gxLib::WAVEファイルを読み込み");
		gxLib::DebugLog("ファイルサイズは%d",CSoundManager::GetInstance()->GetPlayInfo( no )->m_uTempSize);

		//WAVEファイルとして読み込む
		//pWave->Load( CSoundManager::GetInstance()->GetPlayInfo( no )->cFileName );
		pWaveBuffer = (Uint8*)malloc( CSoundManager::GetInstance()->GetPlayInfo( no )->m_uTempSize );

		memcpy(
			pWaveBuffer ,
			CSoundManager::GetInstance()->GetPlayInfo( no )->m_pTempBuffer ,
			CSoundManager::GetInstance()->GetPlayInfo( no )->m_uTempSize );
	}

	if( pWaveBuffer == NULL )
	{
		SAFE_DELETE( CSoundManager::GetInstance()->GetPlayInfo( no )->m_pWave );
		gxLib::DebugLog("gxLib::pWaveBufferがNULL。ファイルないかも。[%s]", CSoundManager::GetInstance()->GetPlayInfo(no)->fileName);
		goto EXIT_LOAD;
	}

	//WAVEを読み込み
	CFileWave *pWave = (CFileWave*)CSoundManager::GetInstance()->GetPlayInfo( no )->m_pWave;
	pWave->Read( pWaveBuffer );

	//-------------------------------------------
	//テンポラリを破棄
	SAFE_DELETE( pWaveBuffer );
	SAFE_DELETE( CSoundManager::GetInstance()->GetPlayInfo( no )->m_pTempBuffer );
	CSoundManager::GetInstance()->GetPlayInfo( no )->m_pTempBuffer = NULL;
	CSoundManager::GetInstance()->GetPlayInfo( no )->m_uTempSize = 0;
	//-------------------------------------------

	//
	pWave = (CFileWave*)CSoundManager::GetInstance()->GetPlayInfo( no )->m_pWave;
	DataSize = pWave->GetFileSize();
	pData    = pWave->GetWaveImage();

	if ( DataSize == 0 )
	{
		goto EXIT_LOAD;
	}

	//=====================================================
	// DirectSoundBuffer設定
	//=====================================================
	//DSBCAPS_LOCDEFER | 
	pWave = (CFileWave*)CSoundManager::GetInstance()->GetPlayInfo( no )->m_pWave;

	desc.dwFlags = DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFX | DSBCAPS_GLOBALFOCUS| DSBCAPS_CTRLFREQUENCY;
	desc.dwBufferBytes = DataSize;
	desc.lpwfxFormat = (WAVEFORMATEX*)(&pWave->GetFormat()->Format);

	//=====================================================
	// DirectSoundBuffer生成
	//=====================================================
	hr = m_pDirectSound->CreateSoundBuffer( &desc, &pTempBuff, NULL );
	if FAILED( hr )
	{
		goto EXIT_LOAD;
	}

	//=====================================================
	// DirectSoundBuffer8取得
	//=====================================================
	hr = pTempBuff->QueryInterface( IID_IDirectSoundBuffer8, (void **)&m_pSoundBuffer[no] );
	// DirectSoundBuffer開放
	pTempBuff->Release();

	if FAILED( hr )
	{
		goto EXIT_LOAD;
	}

	//=====================================================
	// バッファ書き込み
	//=====================================================
	hr = m_pSoundBuffer[no]->Lock(
					0, DataSize,
					&pSoundData1, &size1,
					&pSoundData2, &size2,
					DSBLOCK_ENTIREBUFFER
				);
	if FAILED( hr )
	{
		goto EXIT_LOAD;
	}

	// サウンドデータコピー
	CopyMemory( pSoundData1, pData, DataSize );

	// ロック解除
	m_pSoundBuffer[no]->Unlock( pSoundData1, size1, pSoundData2, size2 );

	// デフォルトの設定
	m_pSoundBuffer[no]->SetCurrentPosition( 0 );
	m_pSoundBuffer[no]->SetVolume( 0 );
	m_pSoundBuffer[no]->SetPan( 0 );

	//=====================================================
	// 終了
	//=====================================================

EXIT_LOAD:

	if ( pData != NULL )
	{
		CFileWave* pWave = (CFileWave*)CSoundManager::GetInstance()->GetPlayInfo(no)->m_pWave;	//void*にしているとデストラクタに来ない
		delete pWave;	// CSoundManager::GetInstance()->GetPlayInfo(no)->m_pWave;
		CSoundManager::GetInstance()->GetPlayInfo( no )->m_pWave = NULL;
	}


	return gxTrue;

}

gxBool CDxSound::PlaySound( Uint32 uIndex , gxBool bLoop , gxBool bOverWrap , Float32 fVolume )
{
	if( m_pSoundBuffer[ uIndex ] == NULL ) return gxFalse;

	if( !bOverWrap )
	{
		StopSound( uIndex );
	}

	m_pSoundBuffer[ uIndex ]->SetCurrentPosition( 4 );

	if( bLoop )
	{
#ifdef SOUND_THREAD
		m_pSoundBuffer[ uIndex ]->Play( 0, 0, 0 );
#else
		m_pSoundBuffer[ uIndex ]->Play( 0, 0, DSBPLAY_LOOPING );
#endif
		CSoundManager::GetInstance()->PlayNow( uIndex , gxTrue );
	}
	else
	{
		m_pSoundBuffer[ uIndex ]->Play( 0, 0, 0 );
		CSoundManager::GetInstance()->PlayNow( uIndex , gxTrue );
	}

	return gxTrue;
}


gxBool CDxSound::StopSound( Uint32 uIndex )
{
	if( m_pSoundBuffer[ uIndex ] == NULL ) return gxFalse;
	m_pSoundBuffer[ uIndex ]->Stop();
	return gxTrue;
}


gxBool CDxSound::SetVolume( Uint32 uIndex )
{
	if( m_pSoundBuffer[ uIndex ] == NULL ) return gxFalse;

	Sint32 i = uIndex;
	Float32 fMasterVolume = CSoundManager::GetInstance()->GetMasterVolume();
	Float32 fForceVolume  = CSoundManager::GetInstance()->GetForceVolume();

	Float32 fVol = CSoundManager::GetInstance()->GetPlayInfo( i )->fVolume;
	Sint32 vol = (Sint32)(DSBVOLUME_MIN* (1.f - (fVol*fMasterVolume*fForceVolume)));

	if ( fVol*100.f > 100.f )
	{
		fVol = 0.f;
	}
	else if ( fVol*100.f < 0.f )
	{
		fVol = -10000.f;
	}
	else
	{
		fVol = -(Float32)pow( 100.0f, (Float32)(log10(100-fVol)) );
	}

	m_pSoundBuffer[ i ]->SetVolume( vol );

	return gxTrue;
}

gxBool CDxSound::SetFrequency(Uint32 uIndex)
{
	if (m_pSoundBuffer[uIndex] == NULL) return gxFalse;

	Sint32 i = uIndex;
	Float32 fMasterVolume = CSoundManager::GetInstance()->GetMasterVolume();
	Float32 fForceVolume = CSoundManager::GetInstance()->GetForceVolume();

	Float32 fRatio = CSoundManager::GetInstance()->GetPlayInfo(i)->fFreqRatio;

	DWORD original;

	 
	if (m_pSoundBuffer[uIndex]->SetFrequency(DSBFREQUENCY_ORIGINAL) == DS_OK)
	{
		if (m_pSoundBuffer[uIndex]->GetFrequency(&original) != DS_OK)
		{
			return gxFalse;
		}
	}

	DWORD freq = original *  fRatio/100.0f;

	//DSBFREQUENCY_ORIGINAL

	if (freq <= DSBFREQUENCY_MIN) freq = DSBFREQUENCY_MIN;
	if (freq >= DSBFREQUENCY_MAX) freq = DSBFREQUENCY_MAX;

	if (m_pSoundBuffer[uIndex]->SetFrequency(freq) == DS_OK)
	{
		return gxTrue;
	}

	return gxTrue;
}


Sint32 CDxSound::getFormat( Uint8* pData )
{
	//ファイルフォーマットからOggかRiffの判断をする
	char str[32];

	if ( pData == NULL )
	{
		return enSoundFormatUNKNOWN;
	}

	memcpy( str , pData , 0x04 );
	str[4] = 0x00;

	strupr( str );

	if ( strncmp( "RIFF", str ,4 ) == 0 ) return enSoundFormatWAVE;
	if ( strncmp( "OGG" , str ,3 ) == 0 ) return enSoundFormatOGG;

	return enSoundFormatUNKNOWN;
}


Uint8* CDxSound::readOgg( Uint8* pData ,Uint32 uSize)
{
	//oggファイル読み込み処理

	Uint8* ReadOgg( Uint8* pData , Uint32 uSize , Uint32 *pSize);

	Uint32 uWaveSize;

	return ReadOgg( pData , uSize , &uWaveSize );
}

IDirectSoundBuffer8* CDxSound::GetSoundBuffer( Sint32 n)
{
	return m_pSoundBuffer[ n ];
}


void ThreadFunc( void* n )
{
	//ループ再生時の処理

	while( g_bThreadEnable )
	{
		for(int i=0;i<CDxSound::enSoundMax;i++)
		{
			unsigned long pdwStatus;

			if( !CSoundManager::GetInstance()->GetPlayInfo( i ) == NULL  ) continue;
			if( !CSoundManager::GetInstance()->GetPlayInfo( i )->bUse ) continue;

			CDxSound::GetInstance()->GetSoundBuffer(i)->GetStatus( &pdwStatus );

			if( pdwStatus & DSBSTATUS_PLAYING )
			{
				//再生中
			}
			else
			{
				//再生が止まった

				CSoundManager::GetInstance()->PlayNow( i , gxFalse );

				if( CSoundManager::GetInstance()->GetPlayInfo( i )->bLoop )
				{
					CSoundManager::GetInstance()->PlayNow( i , gxTrue );
					CSoundManager::GetInstance()->GetPlayInfo( i )->bReq    = gxTrue;
					CSoundManager::GetInstance()->GetPlayInfo( i )->uStatus |= enSoundReqPlay;
				}
			}
		}

		Sleep(8);
	}
}

