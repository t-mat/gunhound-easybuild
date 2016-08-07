#ifndef _CMOVIEMANAGER_H_
#define _CMOVIEMANAGER_H_

#define BIT_ON(a,b) (a|b)
#define BIT_OFF(a,b) (a&(~b))

class CMovieFile;

class CMovieManager
{
public:
	enum {
		enMovieFileMax = 2,
	};
	enum {
		enMovieReqLoad     = 0x01,
		enMovieReqPlay     = 0x02,
		enMovieReqLoop     = 0x04,
		enMovieReqPosition = 0x08,
		enMovieReqStop     = 0x10,
		enMovieReqAutoFrm  = 0x20,
		enMovieExist       = 0x2000,
	};

	CMovieManager();
	~CMovieManager();
	void Action();
	void Draw();

	void LoadMovie( Sint32 sIndex , gxChar *pFileName );
	void PlayMovie( Uint32 uIndex );
	void StopMovie( Uint32 uIndex );
	void SetLoop( Uint32 uIndex , gxBool bLoop );
	void SetFrame( Uint32 uIndex , Sint32 frm );
	void SetAutoFrame( Uint32 uIndex , gxBool bAuto );


	void BindTexturePage( Uint32 uIndex , Sint32 sTexPage );
	void BindMusic( Uint32 uIndex , Sint32 sMusicIndex);

	CMovieFile* GetMovie( Sint32 index );

	SINGLETON_DECLARE( CMovieManager )

private:

	gxBool m_bUpdate;

	CMovieFile *m_pMovie;
};

class CFileTarga;

class CMovieFile
{
public:
	enum {
		enMusicNone = -1,
	};

	CMovieFile();
	~CMovieFile();

	gxBool IsMusic()
	{
		if( m_uMusicBindIndex != enMusicNone ) return gxTrue;

		return gxFalse;
	}

	Sint32 GetMusicIndex()
	{
		return m_uMusicBindIndex;
	}

	gxBool IsExist()
	{
		return ( m_Stat&CMovieManager::enMovieExist )? gxTrue : gxFalse;
	}

	gxChar m_FileName[256];
	Uint32 m_Stat;
	CFileTarga *m_pTga;
	Sint32 m_uMusicBindIndex;
	Sint32 m_uTextureBindIndex;
	Sint32 width;
	Sint32 height;
	Float32 fps;
	Sint32 max;
	Sint32 now;

	//頭出し用のジャンプ先フレーム
	Sint32 JumpFrm;

	//再生開始時刻
	Uint32 m_BeganTime;

	Sint32 depth;
	Uint8 *pData;

	gxBool bUpdate;

private:


};

#endif
