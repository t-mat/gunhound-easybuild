//------------------------------------------------------------
//
// OggVorbisファイルの読み込み
// ※メモリ読み込みに対応
//
// written by tomi.
//------------------------------------------------------------

//#define _TEST_WAV_OUTPUT_DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <vorbis/vorbisfile.h>
#include <vorbis/vorbisenc.h>
#include <gxLib.h>

#ifdef GX_BUILD_OPTIONx86
	#ifdef GX_DEBUG
		#pragma comment(lib,"oggVorbis/debug/x86/d_libogg_static_x86.lib")
		#pragma comment(lib,"oggVorbis/debug/x86/d_libvorbis_static_x86.lib")
		#pragma comment(lib,"oggVorbis/debug/x86/d_libvorbisfile_static_x86.lib")

	#else

		#pragma comment(lib,"oggVorbis/r_libogg_static_x86.lib")
		#pragma comment(lib,"oggVorbis/r_libvorbis_static_x86.lib")
		#pragma comment(lib,"oggVorbis/r_libvorbisfile_static_x86.lib")

	#endif

#elif defined GX_BUILD_OPTIONx64

	#ifdef GX_DEBUG
		#pragma comment(lib,"oggVorbis/debug/x64/d_libogg_static_x64.lib")
		#pragma comment(lib,"oggVorbis/debug/x64/d_libvorbis_static_x64.lib")
		#pragma comment(lib,"oggVorbis/debug/x64/d_libvorbisfile_static_x64.lib")

	#else

		#pragma comment(lib,"oggVorbis/r_libogg_static_x64.lib")
		#pragma comment(lib,"oggVorbis/r_libvorbis_static_x64.lib")
		#pragma comment(lib,"oggVorbis/r_libvorbisfile_static_x64.lib")

	#endif

#endif

#pragma pack (push, 1)
typedef struct{
	char ckidRIFF[4];
	int ckSizeRIFF;
	char fccType[4];
	char ckidFmt[4];
	int ckSizeFmt;
	WAVEFORMATEX WaveFmt;
	char ckidData[4];
	int ckSizeData;
} WAVEFILEHEADER;

typedef struct {
    char *buf;//メモリ上のogg vorbis
    int siz;//サイズ
    int pos;//現在位置
} OVMEM;


Uint8* readOggData(Uint8 *pBuff, Uint32 uSize , Uint32 *pSize );
long makeWaveHeader( vorbis_info* vi , Uint32 uPcmSize , Uint8* pData );



Uint8* ReadOgg( Uint8* pData , Uint32 uSize , Uint32 *pSize)
{
	//メモリからのogg読み込み

	pData = readOggData( pData , uSize , pSize );	

#ifdef _TEST_WAV_OUTPUT_DEBUG
	::SaveFile((Uint8*)"Otamesi.wav",(Uint8*)pData , *pSize );
#endif

	return pData;
}


#if 0
Uint8* loadOggFile(char  *pName, Uint32 *pSize );
//##############################################################################
// long LoadOggVorbis(char *file_name, int word, char **ogg)
//
// 概要
//	 ファイルからOggVorbis形式のデータを読み込み、メモリ上へWAVE形式でデコー
//	 ドする。
//	 データが不要になったら ReleaseOggVorbis を用いて領域を開放しなくてはなら
//	 ない。
//
// 引数
//		 *file_name : 読み込むファイル名
//		 word		: 量子化バイト数	1 か 2 を指定する。
//		 **ogg		: データを格納するchar型のポインタへのポインタ
//
// 戻り値
//		 0 より小さい : エラー
//		 0 より大きい : サイズ
//##############################################################################

Uint8* LoadOgg( char* pFileName , Uint32 *pSize)
{
	//ファイルからの読み込み
	Uint8* pData;

	pData = loadOggFile( pFileName , pSize );

	::SaveFile((Uint8*)"Otamesi.wav",(Uint8*)pData , *pSize );

	return pData;
}


Uint8* loadOggFile(char  *pName, Uint32 *pSize )
{
	OggVorbis_File vf;
	vorbis_info *vi;
	int eof=0;
	int current_section;
	FILE *fp;
	WAVEFILEHEADER wh;
	long whsize;
	long size = 0;
	long data_size;
	long ret;
	Sint32 word = 2;
	Uint8* pOgg;

	//量子化バイト数が正しい値かどうか調べる
	if(!(word == 1 || word == 2))
	{
		gxLib::DebugLog( "word is error");
		return NULL;
	}

	/* ファイルを開く */
	fp = fopen(pName, "rb");
	if(fp == NULL)
	{
		gxLib::DebugLog("Open Error : %s", pName);
		return NULL;
	}

	/* OggVorbis初期化 */
	if(ov_open(fp, &vf, NULL, 0) < 0)
	{
		gxLib::DebugLog("Input does not appear to be an Ogg bitstream.");
		fclose(fp);
		return NULL;
	}

	/* 詳細収得 */
	vi = ov_info(&vf,-1);
	if(vi == NULL)
	{
		gxLib::DebugLog("Get Info Error");
		return NULL;
	}

	/* ヘッダサイズの収得 */
	/* sizeof(wh) だと全てのメンバのサイズの合計より大きくなってしまう */
	whsize = sizeof(wh.ckidRIFF) + sizeof(wh.ckSizeRIFF) + sizeof(wh.fccType) +
					 sizeof(wh.ckidFmt)	+ sizeof(wh.ckSizeFmt)	+ sizeof(wh.WaveFmt) +
					 sizeof(wh.ckidData) + sizeof(wh.ckSizeData);

	/* デコード後のデータサイズを求め、メモリ確保 */
	data_size = (long)ceil(vi->channels * vi->rate * ov_time_total(&vf,-1) * word);
	pOgg = (Uint8*)malloc(data_size + whsize);

	if(pOgg == NULL)
	{
		free(pOgg);
		ov_clear(&vf);
		fclose(fp);
		gxLib::DebugLog("malloc Error");
		return NULL;
	}


	/* デコード */
	/* 一回でバッファ全てにデコードされるとは限らない */
	/* 一回につき 256～4096Byte デコードされるようだ	*/
	while(!eof){
		ret = ov_read(&vf, (char*)(pOgg + size + whsize) , data_size - size, 0, word, 1, &current_section);
		if (ret == 0) {
			eof=1;
		}
		else if (ret < 0)
		{
			//デコード完了
//			free(ogg);
//			ov_clear(&vf);
//			fclose(fp);
//			fprintf(stderr, "Decode Error\n");
//			return -1;
			eof=1;
		}
		else
		{
			size += ret;	//rawデータサイズにov_readから返されるデコードバッファサイズを足しこむ
		}
	}

	//long makeWaveHeader( vorbis_info* vi , Uint32 uPcmSize , Uint8* pData )

	/* cleanup */
	ov_clear(&vf);

	return pOgg;
}
#endif


size_t mread(void *vptr, size_t size, size_t nmemb, void *vpom)
{
	char *ptr = (char*)vptr;
	OVMEM *pom = (OVMEM*)vpom;

	if (pom == NULL) return -1;

    if ((pom->pos >= pom->siz) || (pom->pos == -1)) return 0;

    if (pom->pos + size*nmemb >= (unsigned int)pom->siz)
    {
        int ret;
        memcpy(ptr, &pom->buf[pom->pos], pom->siz - pom->pos);
        ret = (pom->siz - pom->pos)/size;
        pom->pos = pom->siz;
        return ret;
    }

    memcpy(ptr, &pom->buf[pom->pos], nmemb * size);

    pom->pos += (nmemb * size);

    return nmemb;
}

int mseek(void* vpom, ogg_int64_t offset, int whence)
{
	OVMEM* pom = (OVMEM*)vpom;

	int newpos;
   
    if (pom == NULL || pom->pos < 0) return -1;

    if (offset < 0)
    {
        pom->pos = -1;
        return -1;
    }

    switch (whence) {
    case SEEK_SET:
        newpos = (int)(offset);
        break;
    case SEEK_CUR:
        newpos = (int)(pom->pos + offset);
        break;
    case SEEK_END:
        newpos = (int)(pom->siz + offset);
        break;
    default:
        return -1;
    }

    if (newpos < 0) return -1;

    pom->pos = newpos;

    return 0;
}


long mtell(void *vpom)
{
	OVMEM *pom = (OVMEM*)vpom;

    if (pom == NULL) return -1;

    return pom->pos;
}

int mclose(void *vpom)
{
	OVMEM *pom = (OVMEM*)vpom;

	if (pom == NULL) return -1;

    return 0;
}

Uint8* readOggData(Uint8 *pOggFile, Uint32 uOggFileSize ,Uint32 *pSize )
{
    OggVorbis_File  m_VorbisFile;
    vorbis_info    *m_pVorbisInfo;
    ov_callbacks    oggCallbacks; 
	Sint32 sResult = 0;

    //コールバック関数を指定
    oggCallbacks.read_func  = mread;
    oggCallbacks.seek_func  = mseek;
    oggCallbacks.close_func = mclose;
    oggCallbacks.tell_func  = mtell;

	OVMEM m_stOvMem;
	m_stOvMem.pos = 0;
	m_stOvMem.siz = uOggFileSize;
	m_stOvMem.buf = (char*)pOggFile;

	sResult = ov_open_callbacks( &m_stOvMem, &m_VorbisFile, NULL, 0, oggCallbacks );

    switch ( sResult ) {
	case OV_EREAD:		{	gxLib::DebugLog("A read from media returned an error.");	return NULL;	} 
	case OV_ENOTVORBIS:	{	gxLib::DebugLog("Bitstream is not Vorbis data. ");			return NULL;	} 
	case OV_EVERSION :	{	gxLib::DebugLog("Vorbis version mismatch. ");				return NULL;	} 
	case OV_EBADHEADER:	{	gxLib::DebugLog("Invalid Vorbis bitstream header.");		return NULL;	} 
	case OV_EFAULT:		{	gxLib::DebugLog("Internal logic fault; indicates a bug or heap/stack corruption. ");	return NULL;	} 
    }

	m_pVorbisInfo = ov_info( &m_VorbisFile , -1 );

	Sint32 word = 2;
	int current_section;

	Uint32 whsize;
	Uint32 pcm_size;
	WAVEFILEHEADER wh;

	/* ヘッダサイズの収得 */
	/* sizeof(wh) だと全てのメンバのサイズの合計より大きくなってしまう */
	whsize = sizeof(wh.ckidRIFF) + sizeof(wh.ckSizeRIFF) + sizeof(wh.fccType) +
					 sizeof(wh.ckidFmt)	+ sizeof(wh.ckSizeFmt)	+ sizeof(wh.WaveFmt) +
					 sizeof(wh.ckidData) + sizeof(wh.ckSizeData);

	/* デコード後のデータサイズを求め、メモリ確保 */
	double wavesize = ov_time_total( &m_VorbisFile , -1 );

	pcm_size = (Uint32)ceil(m_pVorbisInfo->channels * m_pVorbisInfo->rate * wavesize * word);

	Uint8* pPcmData;
	pPcmData = (Uint8*)malloc( whsize + pcm_size );

	if( pPcmData == NULL)
	{
		free( pPcmData );
		ov_clear( &m_VorbisFile );
		gxLib::DebugLog( "malloc Error" );
		return NULL;
	}

	/* デコード */
	/* 一回でバッファ全てにデコードされるとは限らない */
	/* 一回につき 256～4096Byte デコードされるようだ	*/
	int eof=0;
	Uint32 ReadSize = 0;

	while(!eof)
	{
		Sint32 result;
		result = ov_read(&m_VorbisFile, (char*)(pPcmData + ReadSize + whsize), pcm_size - ReadSize, 0, word, 1, &current_section);
		if (result == 0)
		{
			eof=1;
		}
		else if (result < 0)
		{
			//デコード完了
			gxLib::DebugLog("gxLib::oggデコードでサイズが合わない？");
//			free(ogg);
//			ov_clear(&pVorbisFile);
//			fclose(fp);
//			fprintf(stderr, "Decode Error\n");
//			return -1;
			eof=1;
		}
		else
		{
			ReadSize += result;	//rawデータサイズにov_readから返されるデコードバッファサイズを足しこむ
		}
	}

	*pSize = makeWaveHeader( m_pVorbisInfo , ReadSize , pPcmData );

	ov_clear( &m_VorbisFile );

	return pPcmData;
}

long makeWaveHeader( vorbis_info* vi , Uint32 uPcmSize , Uint8* pData )
{
	//----------------------------------------------
	//vorbis InfoからWaveファイルのヘッダをつくる
	//----------------------------------------------

	WAVEFILEHEADER wh;
	Uint32 whsize;
	Sint32 word = 2;

	//ヘッダのサイズ
	whsize = sizeof(wh.ckidRIFF) + sizeof(wh.ckSizeRIFF) + sizeof(wh.fccType) +
					 sizeof(wh.ckidFmt)	+ sizeof(wh.ckSizeFmt)	+ sizeof(wh.WaveFmt) +
					 sizeof(wh.ckidData) + sizeof(wh.ckSizeData);


	/* ヘッダの初期化 */
	memcpy(wh.ckidRIFF, "RIFF", 4);

	wh.ckSizeRIFF = whsize + uPcmSize - 8;

	memcpy(wh.fccType, "WAVE", 4);
	memcpy(wh.ckidFmt, "fmt ", 4);
	wh.ckSizeFmt = sizeof(WAVEFORMATEX);

	wh.WaveFmt.cbSize			= sizeof(WAVEFORMATEX);
	wh.WaveFmt.wFormatTag		= WAVE_FORMAT_PCM;
	wh.WaveFmt.nChannels		= vi->channels;
	wh.WaveFmt.nSamplesPerSec	= vi->rate;
	wh.WaveFmt.nAvgBytesPerSec	= vi->rate * vi->channels * word;
	wh.WaveFmt.nBlockAlign		= vi->channels * word;
	wh.WaveFmt.wBitsPerSample	= word * 8;

	memcpy(wh.ckidData, "data", 4);
	wh.ckSizeData = uPcmSize;


	//WAVEデータを構築
	int s = 0;
	memcpy(pData 	 , &wh.ckidRIFF,   sizeof(wh.ckidRIFF));	s += sizeof(wh.ckidRIFF);
	memcpy(pData  + s, &wh.ckSizeRIFF, sizeof(wh.ckSizeRIFF));	s += sizeof(wh.ckSizeRIFF);
	memcpy(pData  + s, &wh.fccType,    sizeof(wh.fccType));		s += sizeof(wh.fccType);
	memcpy(pData  + s, &wh.ckidFmt,    sizeof(wh.ckidFmt));		s += sizeof(wh.ckidFmt);
	memcpy(pData  + s, &wh.ckSizeFmt,  sizeof(wh.ckSizeFmt));	s += sizeof(wh.ckSizeFmt);
	memcpy(pData  + s, &wh.WaveFmt,    sizeof(wh.WaveFmt));		s += sizeof(wh.WaveFmt);
	memcpy(pData  + s, &wh.ckidData,   sizeof(wh.ckidData));	s += sizeof(wh.ckidData);
	memcpy(pData  + s, &wh.ckSizeData, sizeof(wh.ckSizeData));

	return uPcmSize + whsize;
}


////wavファイルのヘッダ
//typedef struct{
//    char RIFF[4];//"RIFF"
//    DWORD bytes;//(ファイルのバイト数)-8
//    char WAVE[4];// "WAVE" 
//    char fmt[4];//"fmt "
//    DWORD siz_wf;//PCMWAVEFORMAT構造体のバイト数=常に16
//
//    //PCMWAVEFORMAT構造
//    WORD wFormatTag; 
//    WORD nChannels; 
//    DWORD nSamplesPerSec;
//    DWORD nAvgBytesPerSec; 
//    WORD nBlockAlign;
//    WORD wBitsPerSample;
//    char data[4];//"data"
//    DWORD pcmbytes;//波形データのバイト数
//} WAVHEADER;


#pragma pack (pop)
