//--------------------------------------------------
//
// gxSoundManager.h
//
//--------------------------------------------------

#ifndef _GXSOUND_MANAGER_H_
#define _GXSOUND_MANAGER_H_

enum {
	enSoundReqNone       = 0x00,
	enSoundReqLoad       = 0x01,
	enSoundReqPlay       = 0x02,
	enSoundReqStop       = 0x04,
	enSoundReqVolume     = 0x08,
	enSoundReqChangeFreq = 0x10,
};

enum {
	enMasterVolumeLow,
	enMasterVolumeMiddle,
};

typedef struct StPlayInfo
{
	gxBool  bOverWrap;
	gxBool  bReq;
	gxBool  bLoop;
	gxBool  bUse;
	gxBool  bPlayNow;
	Uint32  uStatus;
	Float32 fVolume;
	Float32 fVolumeAdd;
	//Sint32  frequency;
	Float32 fFreqRatio;
	void*  m_pWave;
	void*  m_pTempBuffer;
	Uint32 m_uTempSize;
	gxChar fileName[256];
} StPlayInfo;

class CSoundManager
{
public:
	CSoundManager();
	~CSoundManager();

	void Action();
	void Play();

	StPlayInfo* GetPlayInfo(Uint32 uIndex)
	{
		return &m_Info[ uIndex ];
	}

	gxBool Play( Uint32 index , Float32 fVolume , gxBool bOverWrap=gxFalse , gxBool bLoop=gxFalse );

	gxBool ReadAudioFile( Uint32 uIndex , const Uint8* pMemory ,Uint32 uSize );
	gxBool LoadAudioFile( Uint32 uIndex , const gxChar* pFileName );
	gxBool PlayAudio( Uint32 uIndex , Float32 fVolume = 0.8f , gxBool bOverWrap=gxFalse , gxBool bLoop=gxFalse );

	gxBool SetForceVolumeLevel( Sint32 type );
	gxBool SetMasterVolumeLevel( Float32 fVol );

	gxBool SetVolume(Uint32 index, Float32 fVol);
	gxBool SetFreq( Uint32 index , Float32 );

	gxBool StopAudio( Uint32 uIndex );
	gxBool SetFade( Sint32 uIndex , Float32 fVolume , Uint32 frm );
	gxBool IsPlay( Uint32 uIndex );

	void PlayNow( Uint32 uIndex , gxBool bPlay )
	{
		m_Info[ uIndex ].bPlayNow = bPlay;
	}

	Float32 GetForceVolume()
	{
		return m_fForceVolume;
	}

	Float32 GetMasterVolume()
	{
		return m_fMasterVolume;
	}

	SINGLETON_DECLARE( CSoundManager );

private:

	StPlayInfo m_Info[MAX_SOUND_NUM];
	Float32 m_fMasterVolume;
	Float32 m_fForceVolume;
	Float32 m_fForceTgtVolume;


};

#endif

