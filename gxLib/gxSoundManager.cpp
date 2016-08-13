//--------------------------------------------------
//
// gxSoundManager.cpp
// サウンドの管理を行います、フェード処理などもこちらで
// 管理するため、ハードウェア側で持つフェード処理などを
// 独自に使うことを推奨しません
//
//--------------------------------------------------

#include <gxLib.h>
#include "gx.h"
#include "gxSoundManager.h"

#ifdef GX_DEBUG
//#define _NO_SOUND_
#endif

#define DEFAULT_MASTER_VOLUME (1.f)
#define DEFAULT_ORIGINAL_VOLUME (0.9f)

SINGLETON_DECLARE_INSTANCE( CSoundManager )

CSoundManager::CSoundManager()
{
	m_fMasterVolume   = DEFAULT_MASTER_VOLUME;
	m_fForceVolume    = 1.0f;
	m_fForceTgtVolume = m_fForceVolume;

	for(Sint32 ii=0; ii<MAX_SOUND_NUM; ii++)
	{
		m_Info[ ii ].m_pWave = NULL;
		m_Info[ ii ].bOverWrap  = gxFalse;
		m_Info[ ii ].bReq       = gxFalse;
		m_Info[ ii ].bLoop      = gxFalse;
		m_Info[ ii ].bUse       = gxFalse;
		m_Info[ ii ].uStatus    = 0;
		m_Info[ ii ].fVolume    = DEFAULT_ORIGINAL_VOLUME;
		m_Info[ ii ].fVolumeAdd = 0.f;
		m_Info[ ii ].bPlayNow   = gxFalse;
		m_Info[ ii ].m_pTempBuffer = NULL;
		m_Info[ ii ].m_uTempSize = 0;
//		m_Info[ ii ].frequency  = 44800;
		m_Info[ ii ].fFreqRatio = 100.0f;
		m_Info[ ii ].fileName[0] = 0x00;
	}

}


CSoundManager::~CSoundManager()
{
	for(Sint32 ii=0; ii<MAX_SOUND_NUM; ii++)
	{
		if( m_Info[ ii ].m_pWave )
		{
			if( m_Info[ ii ].m_pTempBuffer ) free( m_Info[ ii ].m_pTempBuffer );
			delete m_Info[ ii ].m_pWave;
			m_Info[ ii ].m_pWave = NULL;
		}
	}

}


void CSoundManager::Action()
{
	m_fForceVolume += ( m_fForceTgtVolume - m_fForceVolume )/10.f;

	for(Sint32 ii=0; ii<MAX_SOUND_NUM; ii++)
	{
		StPlayInfo *p;

		p = &m_Info[ ii ];
		if( !p->bUse ) continue;
		p->fVolume += p->fVolumeAdd;

		if( p->bPlayNow && p->fVolume <= 0.f )
		{
			p->fVolume    = 0.f;
			p->fVolumeAdd = 0.f;

			//Stopのリクエスト
			p->bReq = gxTrue;
			p->uStatus |= enSoundReqStop;
			p->bPlayNow = gxFalse;
			p->bLoop = gxFalse;
		}
		else if( p->fVolume > 1.f )
		{
			p->fVolume    = 1.f;
			p->fVolumeAdd = 0.f;
			p->uStatus |= enSoundReqVolume;
		}
		else
		{
			if( p->fVolumeAdd )
			{
				p->bReq = gxTrue;
				p->uStatus |= enSoundReqVolume;
			}
		}

	}


}


gxBool CSoundManager::LoadAudioFile( Uint32 uIndex , const gxChar* pFileName )
{
	//Audioファイルのロード

	if( uIndex < 0 || uIndex >= MAX_SOUND_NUM ) return gxFalse;

	if( m_Info[ uIndex ].m_pWave )
	{
		delete m_Info[ uIndex ].m_pWave;
	}

	Uint32 uSize = 0;

	Uint8* pMemory;

	pMemory = ::LoadFile( pFileName , &uSize );

	if( pMemory == NULL )
	{
		//ファイルがなかった
		gxLib::DebugLog("[%d:%s]の音楽ファイルが存在しません", uIndex , pFileName );
		delete m_Info[ uIndex ].m_pWave;
		m_Info[ uIndex ].m_pWave = NULL;
		m_Info[ uIndex ].bReq    = gxFalse;
		m_Info[ uIndex ].bUse    = gxFalse;
		m_Info[ uIndex ].uStatus = enSoundReqNone;
		m_Info[ uIndex ].fileName[0] = 0x00;
		return gxFalse;
	}

	gxLib::DebugLog("[LoadAudioFile] %d , \"%s\"", uIndex , pFileName );

	SAFE_DELETE(m_Info[uIndex].m_pTempBuffer);

	m_Info[ uIndex ].m_pTempBuffer =  new Uint8[ uSize ];

	memcpy( m_Info[ uIndex ].m_pTempBuffer , pMemory , uSize );

	delete[] pMemory;

	//スレッドで動いているローディング処理が途中で走ってしまうので最後にリクエストを設定することにする

	m_Info[ uIndex ].m_uTempSize = uSize;
	m_Info[ uIndex ].bUse = gxTrue;
	m_Info[ uIndex ].bReq = gxTrue;
	m_Info[ uIndex ].m_pWave = NULL;
	sprintf( m_Info[ uIndex ].fileName , "%s" , pFileName );

	m_Info[ uIndex ].uStatus |= enSoundReqLoad;

	return gxTrue;

}


gxBool CSoundManager::ReadAudioFile( Uint32 uIndex , const Uint8* pMemory ,Uint32 uSize )
{
	//Audioファイルをメモリから読み込む

	if( uIndex < 0 || uIndex >= MAX_SOUND_NUM ) return gxFalse;

	if( m_Info[ uIndex ].m_pWave )
	{
		delete m_Info[ uIndex ].m_pWave;
	}

	if( m_Info[ uIndex ].m_pTempBuffer )
	{
		free( m_Info[ uIndex ].m_pTempBuffer );
	}

	if( pMemory == NULL )
	{
		gxLib::DebugLog("【gxLib::Error】サウンドデータバッファがNULLですよ");
		return gxFalse;
	}
	m_Info[ uIndex ].bUse = gxTrue;
	m_Info[ uIndex ].bReq = gxTrue;
	m_Info[ uIndex ].uStatus |= enSoundReqLoad;	//LoadでOK
	m_Info[ uIndex ].m_pTempBuffer = new Uint8[ uSize ];	//malloc( uSize );
	gxUtil::MemCpy( m_Info[ uIndex ].m_pTempBuffer , (void*)pMemory , uSize );
	m_Info[ uIndex ].m_uTempSize = uSize;

	return gxTrue;

}



gxBool CSoundManager::PlayAudio( Uint32 uIndex , Float32 fVolume , gxBool bOverWrap , gxBool bLoop )
{
	if( uIndex < 0 || uIndex >= MAX_SOUND_NUM ) return gxFalse;

	m_Info[ uIndex ].bReq      = gxTrue;
	m_Info[ uIndex ].uStatus   |= enSoundReqPlay;
	m_Info[ uIndex ].bOverWrap = bOverWrap;
	m_Info[ uIndex ].bLoop     = bLoop;
	m_Info[ uIndex ].bPlayNow  = gxTrue;
	m_Info[ uIndex ].fVolume   = fVolume;
	return gxTrue;

}


gxBool CSoundManager::StopAudio( Uint32 uIndex )
{
	if( uIndex < 0 || uIndex >= MAX_SOUND_NUM ) return gxFalse;

	m_Info[ uIndex ].bReq = gxTrue;
	m_Info[ uIndex ].uStatus |= enSoundReqStop;
	m_Info[ uIndex ].bPlayNow  = gxFalse;
	m_Info[ uIndex ].bLoop     = gxFalse;

	return gxTrue;

}


gxBool CSoundManager::SetVolume( Uint32 uIndex , Float32 fVol )
{
	if( uIndex < 0 || uIndex >= MAX_SOUND_NUM ) return gxFalse;

	m_Info[ uIndex ].bReq = gxTrue;
	m_Info[ uIndex ].fVolume = fVol;
	m_Info[ uIndex ].uStatus |= enSoundReqVolume;

	return gxTrue;
}

gxBool CSoundManager::SetFreq(Uint32 uIndex, Float32 fRatio)
{
	if (uIndex < 0 || uIndex >= MAX_SOUND_NUM) return gxFalse;

	m_Info[uIndex].bReq = gxTrue;
	m_Info[uIndex].fFreqRatio = fRatio;
	m_Info[uIndex].uStatus |= enSoundReqChangeFreq;

	return gxTrue;
}

gxBool CSoundManager::SetFade( Sint32 uIndex , Float32 fVolume , Uint32 frm )
{
	if( uIndex < 0 || uIndex >= MAX_SOUND_NUM ) return gxFalse;

	if( frm <= 0 )
	{
		frm = 1;
		m_Info[ uIndex ].bReq       = gxTrue;
		m_Info[ uIndex ].fVolumeAdd = 1.0f;
		m_Info[ uIndex ].uStatus |= enSoundReqVolume;
		return gxTrue;
	}
	else
	{
		m_Info[ uIndex ].bReq = gxTrue;
		m_Info[ uIndex ].fVolumeAdd = (fVolume-m_Info[ uIndex ].fVolume)/frm;
		m_Info[ uIndex ].uStatus |= enSoundReqVolume;
	}

	return gxTrue;
}

gxBool CSoundManager::IsPlay( Uint32 uIndex )
{
//	if( m_Info[ uIndex ].m_pWave == NULL ) m_Info[ uIndex ].bPlayNow = gxFalse;
	if( uIndex < 0 || uIndex >= MAX_SOUND_NUM ) return gxFalse;

	if( !m_Info[ uIndex ].bUse ) m_Info[ uIndex ].bPlayNow = gxFalse;

	return m_Info[ uIndex ].bPlayNow;

}

gxBool CSoundManager::SetForceVolumeLevel( Sint32 type )
{
	switch( type ){
	case enMasterVolumeLow:
		m_fForceTgtVolume = 0.5f;
		break;
	case enMasterVolumeMiddle:
		m_fForceTgtVolume = 1.0f;
		break;
	}

//	m_Info[ uIndex ].uStatus |= enSoundReqVolume;
	return gxTrue;
}


gxBool CSoundManager::SetMasterVolumeLevel( Float32 fLevel )
{
	if(fLevel < 0.f ) fLevel = 0.f;
	if(fLevel > 1.f ) fLevel = 1.f;
	m_fMasterVolume = fLevel;

//	m_Info[ uIndex ].uStatus |= enSoundReqVolume;
	return gxTrue;
}


void CSoundManager::Play()
{
#ifdef _NO_SOUND_
	return;
#endif
	::Play();
}

