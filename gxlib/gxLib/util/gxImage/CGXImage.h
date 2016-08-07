//----------------------------------
//*gx
//----------------------------------
//ヘッダ
//ファイル数情報、
//1024,2048,...ファイル数分
//----------------------------------
//ファイル名リスト（暗号化）
//----------------------------------
//ファイルアドレスリスト
//----------------------------------
#pragma pack ( push , 1 )
enum {
	enVersion1_0_0_=0,
	enVersion1_0_1,
	enVersion1_0_2,
	enVersionSaisin,
};


class CGXImage
{
public:

	//--------------------------------------
	//イメージファイル情報
	//--------------------------------------
	typedef struct StPackHeader
	{
		//enVersion1_0_0_
#if 1
		gxChar name[4];				//4
		gxChar version;				//1
		Uint8  filenum;		//1
		Uint32 imagesize;			//4
		Uint32 Offset;				//4
		gxChar dummy2[2];				//2
#else
		//enVersion1_0_1_
		gxChar  name[4];		//4
		Uint16 version;			//2
		Uint16 filenum;			//2
		Uint32 imagesize;		//4
		Uint32 Offset;			//4
#endif
	} StPackHeader;

	//--------------------------------------
	//単一ファイル情報
	//--------------------------------------
	typedef struct StUnitFile
	{
		Uint8  *pFileAddr;
		gxChar *pFileName;
		Uint32 Size;

	}StPackFile;

	enum {
		enFileNumMax = 1024,//256,
	};


	CGXImage();
	~CGXImage();

	void Set(Uint8* pFileImage);

	//---------------------------------------

	Sint32 m_sFileNum;
	Sint32 m_sVersion;

	StPackHeader*  m_pStHeader;
	StUnitFile     m_stFile[enFileNumMax];

	//---------------------------------------

	gxBool Load    (gxChar* pFileName);

	gxBool IsLoadFinish()
	{
		return m_bLoadComplete;
	}

	Uint8* GetFile(gxChar* pFileName,Uint32 *pSize=NULL );

	Uint32 GetFileSize( gxChar* pFileName )
	{
		Sint32 n;
		n = getIndex(pFileName);
		return m_stFile[n].Size;
	}

	void SetPath  ( gxChar* pFileName )
	{
		sprintf( m_ZettaiPath , "%s" , pFileName );
	}

	Sint32 GetError()
	{
		return m_sErrorCode;
	}

	gxChar *GetFullPathFileName(gxChar *fileName);

	gxBool SetTexture( Sint32 pg , char* pName );

	gxBool DeleteFile( gxChar *pFileName );

	//make用

	void InfoMake ( gxChar *path );
	gxBool Add     (gxChar* pUnitFileName);
	gxBool Save    (gxChar* pSaveName);
	void Directory( gxChar* pFilename);

	SINGLETON_DECLARE(CGXImage);

private:

	Sint32 getIndex( gxChar* pFileName );

	gxBool Analyse();
	void Decode( Uint8 *p ,Uint32 sz);
	Uint32 DecodeAsync( Uint8 *p ,Uint32 sz,Uint32 sPos);

	Uint8 *m_pFileImage;

	//void AddImage();

	//---------------------------------------
	//GXIを使わない時用
	//---------------------------------------

	void clearAll()
	{
		m_sFileListNum = 0;
		SAFE_DELETES(m_pFileList);
	}

	typedef struct StFileList
	{
		StFileList()
		{
			pData = NULL;
			Clear();
		}

		~StFileList()
		{
			SAFE_DELETES( pData );
		}


		gxBool Clear()
		{
			gxBool ret = gxFalse;

			if( pData )
			{
				ret = gxTrue;
			}

			Name[0] = 0x00;
			uSize = 0;
			SAFE_DELETES( pData );

			return ret;
		}

		Uint32 uSize;
		gxChar Name[ 256 ];
		Uint8  *pData;

	} StFileList;

	Sint32 m_sFileListNum;

	StFileList *m_pFileList;
	
	
	gxBool isFileExist( gxChar *pFileName )
	{
		Sint32 id = getFileID( pFileName );

		if( id < 0 ) return gxFalse;

		return gxTrue;
	}

	Sint32 getFileID( gxChar *pFileName );
	

	//---------------------------------------
	//イメージファイル作成用
	//---------------------------------------

	typedef struct StUnitFileInfo
	{
		//イメージファイル作成用構造体
		Uint32 NameSize;	 //8
		Uint32 FileSize;	 //8
		Uint32 DiskSize;	 //8
		gxChar   FileName[255];	//8
		gxChar*  pFileImage;
	} StUnitFileInfo;
	Sint32 m_sAddFileNum;
	StUnitFileInfo *m_pStFileInfo[ enFileNumMax ];


	gxChar m_ZettaiPath[256];
	Uint32 m_sImageSize;
	Sint32 m_fh;
	Uint32 m_sPos;
	Sint32 m_LoadSeq;
	gxBool m_bLoading;
	gxBool m_bLoadComplete;
	Sint32 m_sErrorCode;

	Uint32 m_sAllWork;
	Uint32 m_NowWork;

	//追加
	gxChar tempBuf[280];
	gxChar m_root[32];
	Uint32 m_lastLength;

};





































#pragma pack ( pop )
