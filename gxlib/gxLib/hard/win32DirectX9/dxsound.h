#pragma once
//#include <dmusici.h>
#include <dsound.h>

/*
typedef struct StSoundInfo {

	gxBool bPlay;
	gxBool bLoop;
	gxBool bLoad;
	Uint64 uStart;
	Uint64 uLength;
	Sint32 sVol;

	Uint32 nAvgBytesPerSec;
	Uint32 nSamplesPerSec;
	Uint32 nChannels;
	Uint32 wBitsPerSample;

	StSoundInfo()
	{
		bPlay = gxFalse;
		bLoop = gxFalse;
		bLoad = gxFalse;
		uStart = 0;
		sVol = 65;
		nAvgBytesPerSec = 0;
		nSamplesPerSec = 0;
		nChannels = 0;
		wBitsPerSample = 0;
	}

} StSoundInfo;
*/

class CDxSound
{
public:
	enum {
		enSoundMax = MAX_SOUND_NUM,
	};

	enum {
		enSoundFormatUNKNOWN = -1,
		enSoundFormatWAVE,
		enSoundFormatOGG,
	};

	CDxSound();
	~CDxSound();

	gxBool Init();
	gxBool Action();

	IDirectSoundBuffer8* GetSoundBuffer( Sint32 n);

	SINGLETON_DECLARE( CDxSound );

private:

	void Play();
	gxBool SetVolume( Uint32 uIndex );
	gxBool SetFrequency(Uint32 uIndex);
	gxBool read( Uint32 n );
	Sint32 getFormat( Uint8* pData );
	Uint8* readOgg( Uint8* pFileName , Uint32 uSize );

	IDirectSound8*		 m_pDirectSound;
	IDirectSoundBuffer*	 m_pPrimaryBuffer;
	IDirectSoundBuffer8* m_pSoundBuffer[ enSoundMax ];

	// ---------------------------
	gxBool PlaySound( Uint32 uIndex , gxBool bLoop , gxBool bOverWrap , Float32 fVolume );
	gxBool StopSound( Uint32 uIndex );
	// ---------------------------

	StSoundInfo m_stSoundInfo[ enSoundMax ];
	Sint32 m_sMasterVolume;

};

