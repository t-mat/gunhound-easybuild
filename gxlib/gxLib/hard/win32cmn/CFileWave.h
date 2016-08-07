typedef struct guid {
    Uint32 Data1;
    Uint16 Data2;
    Uint16 Data3;
    Uint8  Data4[8];
} guid;

typedef struct StWAVEFORMATEX { 
  Uint16 wFormatTag; 
  Uint16 nChannels; 
  Uint32 nSamplesPerSec; 
  Uint32 nAvgBytesPerSec; 
  Uint16 nBlockAlign; 
  Uint16 wBitsPerSample; 
  Uint16 cbSize; 
} StWAVEFORMATEX; 

typedef struct StWaveInfo
{
    StWAVEFORMATEX Format;
    union {
        Uint16 wValidBitsPerSample;
        Uint16 wSamplesPerBlock;
        Uint16 wReserved;
    } Samples;
    Uint32 dwChannelMask;
    guid SubFormat;
} StWaveInfo;


class CFileWave
{
public:

	CFileWave();
	~CFileWave();

	gxBool Load( gxChar* pFileName );
	gxBool Read( Uint8* pMemory );

	Uint32 GetFileSize()
	{
		return m_DataSize;
	}

	Uint8* GetWaveImage()
	{
		return m_pWaveImage;
	}

	StWaveInfo* GetFormat()
	{
		return &m_tWaveInfo;
	}

private:
	Uint32 getWaveData( Uint8 *pFileImage, Uint8 **ppWaveImage, StWaveInfo *pWaveInfo );
	Uint32 getChunk( Uint8 *pSrcData, Uint8 **ppDstData, Uint32 *pSize );
	Uint32 makeWaveInfo( Uint8 *pHeader, StWaveInfo *pWaveInfo );
	Uint32 makeWaveHeader( Uint8 *pHeader, StWaveInfo *pWaveInfo );

	StWaveInfo m_tWaveInfo;		//ヘッダー情報
	Uint8     *m_pWaveImage;	//WAVEデータイメージ
	Uint16	   m_uFormat;		//フォーマット( -1:未知のWAVE / 0:ＰＣＭ / 1:マルチチャンネル ウェーブ フォーマット / 2:圧縮WAVE )
	Uint32	   m_DataSize;		//WAVEデータイメージのサイズ

};
