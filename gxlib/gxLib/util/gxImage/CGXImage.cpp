//---------------------------------------------------------------
// CGaruruImage.cpp
// 合体ファイルを扱うクラス
//
//---------------------------------------------------------------
#include <gxLib.h>
#include "CGXImage.h"

//#define _NO_GXI_IMAGE_

#ifdef GX_DEBUG
	#define GXI_READ_SIZE (8192*32)	//32ではT4でガクガクだった
#else
	#define GXI_READ_SIZE (8192*16)	//32ではT4でガクガクだった
#endif

#define GXI_DECODE_KEY (0xFF)
#define MAX_PATH_LENGTH (256)

gxChar g_NameBuf[MAX_PATH_LENGTH];
gxChar g_NameBuf2[MAX_PATH_LENGTH];
gxChar g_NameBuf3[MAX_PATH_LENGTH];
gxChar g_NameBuf4[MAX_PATH_LENGTH];

//static CGXImage *pCGXImage= NULL;

SINGLETON_DECLARE_INSTANCE( CGXImage );

CGXImage::CGXImage()
{
	//---------------------------
	//---------------------------

	m_pFileImage = NULL;
	m_sFileNum   = 0;
	m_sVersion   = 0;
	m_ZettaiPath[0] = 0x00;
	m_sImageSize = 0;

	for(Sint32 ii=0;ii<enFileNumMax;ii++)
	{
		memset( &m_stFile[ii] ,0x00, sizeof(StUnitFile) );
		m_pStFileInfo[ ii ] = NULL;
	}

	//---------------------------
	m_sAddFileNum = 0;

	m_fh   = -999;
	m_sPos = 0;
	m_LoadSeq = 0;
	m_bLoading = gxFalse;
	m_bLoadComplete = gxTrue;

	m_sErrorCode = 0;
	m_sAllWork = 0;
	m_NowWork  = 0;

	m_root[0] = 0x00;

	m_sFileListNum = 0;
	m_pFileList = new StFileList[ enFileNumMax ];

}


CGXImage::~CGXImage()
{
	//---------------------------
	//---------------------------

	if( m_pFileImage ) delete[] m_pFileImage;

	//SAFE_DELETES( m_pFileList );

	clearAll();


}


void CGXImage::Set(Uint8* pFileImage)
{
	//---------------------------------------------
	//外部からのイメージセット
	//---------------------------------------------

	if( m_pFileImage )
	{
		delete[] m_pFileImage;
	}

	m_pFileImage = pFileImage;

	clearAll();

	m_pFileList = new StFileList[enFileNumMax];
}


Sint32 CGXImage::getIndex( gxChar* pFileName )
{
	//---------------------------------------------
	//ファイルのインデックスを返す
	//---------------------------------------------

	Sint32 l1,l2,len;
	gxChar buf1[255];
	gxChar buf2[255];

	l1 = strlen(pFileName);

	sprintf(buf1,"%s",pFileName );

    gxUtil::StrUpr(buf1);

	for(Sint32 ii=0;ii<m_sFileNum;ii++)
	{
		l2 = strlen( m_stFile[ii].pFileName );
		len = ( l2 > l1 )? l2 : l1;

		sprintf(buf2,"%s",m_stFile[ii].pFileName);
		gxUtil::StrUpr( buf2 );

		if( strncmp( buf2 , buf1 , len) == 0 )
		{
			return ii;
		}
	}

	return -1;

}


gxBool CGXImage::Add (gxChar* pUnitFileName)
{
	//-----------------------------------------------
	//イメージにファイルを追加する
	//-----------------------------------------------

	StUnitFileInfo *p;
	p = m_pStFileInfo[m_sAddFileNum] = new StUnitFileInfo;

	Sint32 len = strlen(m_ZettaiPath)+1;
	sprintf(p->FileName,"%s",&pUnitFileName[len] );
	p->NameSize   = strlen(p->FileName)+1;
	p->NameSize   += (4-p->NameSize%4);	//奇数アドレスにならないように

	p->FileSize   = 0;
	p->pFileImage = NULL;
	m_sAddFileNum ++;

	return gxTrue;
}


gxBool CGXImage::Load( gxChar* pFileName )
{
	//-----------------------------------------------
	//イメージファイルを読み込む(ブロック読み込み)
	//-----------------------------------------------
	Sint32 sRet = 1;
	Uint32 sPos = 0;

#ifdef _NO_GXI_IMAGE_
	clearAll();
	m_pFileList = new StFileList[enFileNumMax];

	gxChar temp[256];
	gxUtil::GetFileNameWithoutExt(pFileName, temp);
	SetPath(temp);
	return gxTrue;
#endif

	//-----------------------------------------------------------

	m_bLoadComplete = gxFalse;

	if( m_pFileImage )
	{
		delete[] m_pFileImage;
	}

	{
		// Blowfish復号化
		Uint32 uSize = 0;
		Uint8* pTemp = gxLib::LoadFile( pFileName, &uSize );
		m_pFileImage = pTemp;//毒 gxUtil::FileDecryption(pTemp, uSize, &uReadSize);

		Decode( m_pFileImage, uSize);

		//delete[] pTemp;
	}

	Analyse();

	m_bLoadComplete = gxTrue;

	return gxTrue;

}



gxBool CGXImage::Analyse()
{
	//-----------------------------------------------------------
	//情報解析
	//-----------------------------------------------------------

	Sint32 sRet = 1;
	Uint32 sPos = 0;
	Uint32 sImgPos = 0;

	//ヘッダー情報を登録
	m_pStHeader = (StPackHeader*)&m_pFileImage[0];

	//ファイル数、バージョンを記録
	m_sFileNum = m_pStHeader->filenum;
	m_sVersion = m_pStHeader->version;

	//ディスクイメージの先頭アドレス
	Uint8 *pImage = &m_pFileImage[m_pStHeader->Offset];

	//ファイルのアドレスを登録
	sPos = 16; 
	sImgPos = 0;

	for(Sint32 ii=0;ii<m_sFileNum;ii++)
	{
		m_stFile[ii].pFileAddr = &pImage[sImgPos];
		m_stFile[ii].pFileName = (gxChar*)&m_pFileImage[sPos+16];
		m_stFile[ii].Size      = *((Uint32*)(&m_pFileImage[sPos+4]));

		sImgPos += *((Uint32*)&m_pFileImage[sPos+0]);

		sPos    += (16+   (*(Uint32*)&m_pFileImage[sPos+8]));	//リスト位置を更新
	}

	return gxTrue;
}


gxBool CGXImage::Save(gxChar* pSaveName)
{
	//-----------------------------------------------
	//イメージファイルを作成する
	//-----------------------------------------------

	//---------------------------------------
	//ヘッダ情報作成
	//---------------------------------------

	StPackHeader stHead;
	stHead.name[0]  = 'D';
	stHead.name[1]  = 'R';
	stHead.name[2]  = 'Q';
	stHead.name[3]  = 0;
	stHead.version  = enVersionSaisin;
	stHead.filenum  = m_sAddFileNum;
	stHead.imagesize = 0;

	//---------------------------------------
	//ファイル情報書き込み
	//---------------------------------------
#if 0
 -- poison
	for(ii=0;ii<m_sAddFileNum;ii++)
	{
		int fh;
		struct stat filestat;

		sprintf( g_NameBuf,"%s\\%s",m_ZettaiPath,m_pStFileInfo[ii]->FileName );
		fh = open( g_NameBuf , O_RDONLY|O_BINARY );

		if( fh < 0 )
		{
			//---------------------------------------
			//ファイル読み込みの失敗
			//---------------------------------------

			return gxFalse;
		}
		else
		{
			//---------------------------------------
			//ファイルイメージの読み込み
			//---------------------------------------
			Sint32 ret = 1;
			Sint32 pos = 0;

			fstat( fh , &filestat );

			m_pStFileInfo[ii]->FileSize   = filestat.st_size;
			m_pStFileInfo[ii]->DiskSize   = filestat.st_size+( 4-(m_pStFileInfo[ii]->FileSize%4) );
			m_pStFileInfo[ii]->pFileImage = (gxChar*)malloc( m_pStFileInfo[ii]->DiskSize );	//�m�A

			ret = read( fh , &m_pStFileInfo[ii]->pFileImage[ pos ] , filestat.st_size );
			close( fh );
		}
		stHead.imagesize += m_pStFileInfo[ii]->DiskSize;
	}

    //-------------------------------------------
	//実際のファイル書き込み
	//-------------------------------------------
	int fh;

	//-------------------------------------------
	//リストファイルの作成
	//-------------------------------------------
	Uint32 sOffset= 0;
	Sint32 sDummy = 0;

	sprintf(g_NameBuf4,"%s\\..\\%s",m_ZettaiPath,"TempList.bin" );
	fh = open( g_NameBuf4,O_WRONLY|O_BINARY|O_TRUNC|O_CREAT,S_IREAD|S_IWRITE);

	for( ii=0; ii<m_sAddFileNum; ii++ )
	{
		write( fh , &m_pStFileInfo[ii]->DiskSize , 4);
		write( fh , &m_pStFileInfo[ii]->FileSize , 4);
		write( fh , &m_pStFileInfo[ii]->NameSize , 4);
		write( fh , &sDummy , 4);
		write( fh , &m_pStFileInfo[ii]->FileName , m_pStFileInfo[ii]->NameSize );

		sOffset += ( 4+4+4+4+m_pStFileInfo[ii]->NameSize );
	}

	close(fh);

	//-------------------------------------------
	//データファイルの作成
	//-------------------------------------------

	sprintf(g_NameBuf4,"%s\\..\\%s",m_ZettaiPath,"TempData.bin" );
	fh = open( g_NameBuf4 ,O_WRONLY|O_BINARY|O_TRUNC|O_CREAT,S_IREAD|S_IWRITE);

	for( ii=0; ii<m_sAddFileNum; ii++ )
	{
		write( fh , m_pStFileInfo[ii]->pFileImage , m_pStFileInfo[ii]->DiskSize );

		delete[] m_pStFileInfo[ii]->pFileImage;	//解放A
	}

	close(fh);

	//-------------------------------------------
	//ヘッダファイルの作成
	//-------------------------------------------

	stHead.Offset = sizeof(stHead)+sOffset;

	sprintf(g_NameBuf4,"%s\\..\\%s",m_ZettaiPath,"TempHead.bin" );
	fh = open( g_NameBuf4 , O_WRONLY|O_BINARY|O_TRUNC|O_CREAT,S_IREAD|S_IWRITE );

	write( fh , &stHead , sizeof(stHead) );

	close(fh);


	Uint8 *pBuf;

	//-----------------------------------------------------
	//結合ファイル作成
	//-----------------------------------------------------
	Sint32 fh2;
	struct stat FileStat2;
	fh2 = open( pSaveName , O_WRONLY|O_BINARY|O_TRUNC|O_CREAT,S_IREAD|S_IWRITE );

	Uint32 sPos = 0;
	Uint32 sRet = 1;

	sprintf(g_NameBuf4,"%s\\..\\%s",m_ZettaiPath,"TempHead.bin" );
	fh = open( g_NameBuf4 , O_RDONLY|O_BINARY );
	fstat( fh , &FileStat2 );

	pBuf = new Uint8[ FileStat2.st_size ];

	sRet = read( fh , &pBuf[ sPos ] , FileStat2.st_size );
	close( fh );

	Decode(pBuf,FileStat2.st_size);
	write( fh2 , pBuf , FileStat2.st_size );

	delete[] pBuf;

	//---------------------------------------------------

	sPos = 0;
	sRet = 1;

	sprintf(g_NameBuf4,"%s\\..\\%s",m_ZettaiPath,"TempList.bin" );
	fh = open( g_NameBuf4 , O_RDONLY|O_BINARY );
	fstat( fh , &FileStat2 );

	pBuf = new Uint8[ FileStat2.st_size ];

	sRet = read( fh , &pBuf[ sPos ] , FileStat2.st_size );
	close( fh );

	Decode(pBuf,FileStat2.st_size);
	write( fh2 , pBuf , FileStat2.st_size );

	delete[] pBuf;

	//---------------------------------------------------

	sPos = 0;
	sRet = 1;

	sprintf(g_NameBuf4,"%s\\..\\%s",m_ZettaiPath,"TempData.bin" );
	fh = open( g_NameBuf4 , O_RDONLY|O_BINARY );
	fstat( fh , &FileStat2 );

	pBuf = new Uint8[ FileStat2.st_size ];//(gxChar*)malloc( FileStat2.st_size );

	sRet = read( fh , &pBuf[ sPos ] , FileStat2.st_size );
	close( fh );

	Decode(pBuf,FileStat2.st_size);
	write( fh2 , pBuf , FileStat2.st_size );

	delete[] pBuf;

	close( fh2 );

	{
		// Blowfish暗号化
		Uint8* pTemp = NULL;
		Uint32 uSize = 0;
		Uint8* pWrite = NULL;
		Uint32 uWriteSize = 0;

		pTemp = gxLib::LoadFile( pSaveName, &uSize );
		pWrite = pTemp;//毒 gxUtil::FileEncryption(pTemp, uSize, &uWriteSize);

		gxLib::SaveFile( pSaveName, pWrite, uWriteSize );

		delete[] pTemp;
		delete[] pWrite;

	}
#endif
    

	return gxTrue;

}


void CGXImage::InfoMake(gxChar *filename)
{
	//-------------------------------------
	//ディスクイメージ情報を書き出す
	//-------------------------------------
	Load( filename );

//	sprintf(g_NameBuf,"%s", filename );
	gxUtil::GetFileNameWithoutExt( filename , g_NameBuf);
	sprintf(g_NameBuf2,"%s.txt", g_NameBuf );

	Analyse();

	FILE *fp = fopen( g_NameBuf2,"wt");

	fprintf(fp,"ファイル名[%s]\n",filename);
	fprintf(fp,"ファイル数[%d]\n",m_sFileNum);

	for(Sint32 ii=0;ii<m_sFileNum;ii++)
	{
		fprintf(fp,"ID%03d : [%8d(%5dKB)] / [%s]\n",ii,m_stFile[ii].Size,m_stFile[ii].Size/1024,m_stFile[ii].pFileName);
	}

	fprintf(fp,"[EOF]");

	fclose(fp);
}

void CGXImage::Decode( Uint8 *p ,Uint32 sz)
{
	//-------------------------------------
	//XOR暗号化
	//-------------------------------------
	
	for(Uint32 ii=0;ii<sz;ii++)
	{
		p[ii] ^= GXI_DECODE_KEY;
	}

}

Uint32 CGXImage::DecodeAsync( Uint8 *p ,Uint32 sz,Uint32 sPos)
{
	//-------------------------------------
	//XOR暗号化
	//-------------------------------------
	
	for(Uint32 ii=0;ii<GXI_READ_SIZE*8;ii++)
	{
		if(sPos >= sz)
		{
			return sz;
		}

		p[sPos] ^= GXI_DECODE_KEY;
		sPos ++;
		m_NowWork ++;
	}

	return sPos;
}



void CGXImage::Directory(gxChar *path)
{
	//-------------------------------------
	//ディレクトリ捜索
	//-------------------------------------
//毒
/*
	gxChar subpath[_MAX_PATH];
    WIN32_FIND_DATA lp;
	gxChar filepath[_MAX_PATH];

	sprintf(filepath,"%s\\*",path);

    HANDLE h=FindFirstFile(filepath,&lp);

	if(h == INVALID_HANDLE_VALUE)
	{
		printf("検索失敗\n");
		return; 
	}

	do{
		if((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			&& strcmp((const gxChar*)lp.cFileName,"..")!=0 && strcmp((const gxChar*)lp.cFileName,".")!=0) {
		    sprintf(subpath,"%s\\%s",path,lp.cFileName);
		    Directory(subpath);
		}

		if((lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=FILE_ATTRIBUTE_DIRECTORY)
		{
			//ファイルだった場合

			sprintf(filepath,"%s\\%s",path,lp.cFileName);
			Add( filepath );
		}
	} while(FindNextFile(h,&lp));
 
    FindClose(h);
*/
}


Uint8* CGXImage::GetFile( gxChar* pFileName , Uint32 *pSize )
{

//#ifdef _NO_GXI_IMAGE_
//	Uint8 *pData;
//	gxChar Name[256];
//	sprintf( Name, "%s/%s",this->m_ZettaiPath , pFileName );
//    gxUtil::StrUpr( Name );
//    pData = gxLib::LoadFile(Name, pSize);
//#endif


#ifdef _NO_GXI_IMAGE_

	if( isFileExist(pFileName) )
	{
		Sint32 id = getFileID(pFileName);

		*pSize = m_pFileList[ id ].uSize;

		return m_pFileList[ id ].pData;
	}
	else
	{
		sprintf( m_pFileList[ m_sFileListNum ].Name , "%s", pFileName );

	    gxUtil::StrUpr( m_pFileList[ m_sFileListNum ].Name );

		Uint32 uSize;
		Uint8 *pData;

		gxChar *pName = GetFullPathFileName(m_pFileList[m_sFileListNum].Name);

		pData = gxLib::LoadFile(pName, &uSize );

		if( pData )
		{
			*pSize = uSize;

			m_pFileList[ m_sFileListNum ].pData = pData;
			m_pFileList[ m_sFileListNum ].uSize = uSize;
			m_sFileListNum ++;

			return pData;
		}
	}

	return NULL;
#endif


	Sint32 n;

	n = getIndex(pFileName);
	if( n < 0 )
	{
		*pSize = 0;
#ifdef GX_DEBUG
		gxLib::DebugLog( "[CGXImage]File not Found 「%s」がないようです",pFileName);
#endif
		return NULL;
	}
	if( pSize)
	{
		*pSize = m_stFile[n].Size;
	}
#ifdef GX_DEBUG
		gxLib::DebugLog( "[CGXImage]File Found「%s」を読み込みました",pFileName);
#endif

	return (Uint8*)(m_stFile[n].pFileAddr);
}
gxChar *CGXImage::GetFullPathFileName(gxChar *fileName)
{
	gxChar temp[256];

	if (m_ZettaiPath[0])
	{
		sprintf(temp, "%s\\%s", m_ZettaiPath, fileName);
	}
	else
	{
		sprintf(temp, "%s", fileName);
	}

	Sint32 len = strlen(temp);
	static gxChar name[256];
	Sint32 num = 0;

	for (Uint32 ii = 0; ii<len; ii++)
	{
		/*
		if( pFileName[num] == 'ｴｴ' )
		{
		fileName[ii] = '/';
		fileName[ii+1] = 0x00;
		continue;
		}
		else
		*/
		if (temp[num] == '\\')
		{
			name[ii] = '/';
			name[ii + 1] = 0x00;
			num += 1;
			continue;
		}
		name[ii] = temp[num];
		name[ii + 1] = 0x00;
		num++;
	}


	return name;
}


Sint32 CGXImage::getFileID( gxChar *pFileName )
{
	gxChar temp[256];

	sprintf( temp , "%s" , pFileName );
	gxUtil::StrUpr( temp );

	for( Sint32 ii=0; ii<m_sFileListNum; ii++ )
	{
		if( m_pFileList[ ii ].Name[0] == 0x00 ) continue;

		if( strcmp( m_pFileList[ ii ].Name , temp ) == 0 )
		{
			return ii;
		}
	}

	return -1;
}

gxBool CGXImage::SetTexture( Sint32 pg , char* pName )
{
	Uint32 uSize;

	Uint8 *p = GetFile( pName , &uSize );

	gxLib::ReadTexture( pg , p , uSize , 0xff00ff00 );

	return gxTrue;
}


gxBool CGXImage::DeleteFile( gxChar *pFileName )
{
	Sint32 id = getFileID( pFileName );

	if( m_pFileList[ id ].Clear() )
	{
//		if( m_sFileListNum > 0 ) m_sFileListNum --;
		return gxTrue;
	}
	return gxFalse;
}

//-----------------------------------------------------------
//サンプルコード
//-----------------------------------------------------------
#if 0
void LoadFile();
void SaveFile();

void CGXImageFile()
{
	if( pCGXImage == NULL)
	{
		pCGXImage = new CGXImage();
//		pCGXImage->Add("test/test.txt");
//		pCGXImage->Add("test/haikei.bmp");
//		pCGXImage->Save("DiskImage.drq");

//		LoadTextureFromMemory( 0 , p , sSize , 0xff00ff00 );
//		pCGXImage->GetSize("test/test.txt");
//		LoadTexture( 0,"test/haikei.bmp",0xff00ff00 );
//		UploadTexture();
//		pCGXImage->Load( "DiskImage.drq" );
	}

//	SaveFile();
//	LoadFile();

//	if( Joy[0].trg&BTN_C )
//	{
//		UploadTexture();
//	}

	//----------------------------------
	//非同期確認用
	//----------------------------------

	static int x=0;

//	x+=2;
//	x=x%320;
//	PutSpriteDX( x,0,100, 0,0,0,128,128,0,0, ATR_DFLT,ARGB_DFLT );

}


void DragAndDrop(gxChar *filename)
{
	if( pCGXImage )
	{
		delete pCGXImage;
	}

	pCGXImage = new CGXImage();

	if( pCGXImage )
	{
		gxChar out1[255];
		sprintf(g_NameBuf3,"%s",filename );
		gxUtil::GetExt(g_NameBuf3,out1);
        gxUtil::StrUpr(out1);

		if( strcmp(out1,".GXI")==0 )
		{
			//情報モード
			pCGXImage->InfoMake( filename );
		}
		else
		{
			//作成モード
			pCGXImage->SetPath( filename );
			pCGXImage->Directory( filename );

			gxChar out2[255];
			sprintf(g_NameBuf,"%s", filename );
			gxUtil::GetFileNameWithoutExt(g_NameBuf , out2);

			sprintf(g_NameBuf2,"%s.drq", out2 );
			pCGXImage->Save( g_NameBuf2 );
		}
	}

	if( pCGXImage )
	{
		delete pCGXImage;
	}

	pCGXImage = NULL;
}


void LoadFile()
{
//	if( pCGXImage )
//	{
//		bool LoadTextureFromMemory(int pg,gxChar *pBmpfilename,Uint32 sz , int col);
//		if( pCGXImage->IsLoadFinish() )
//		{
//			if( Joy[0].trg&BTN_B )
//			{
//				gxChar *p;
//				Uint32 sSize=0;
//				p = pCGXImage->GetFile( "haikei.bmp" , &sSize );
//				LoadTextureFromMemory( 0 , p , sSize , 0xff00ff00 );
//			}
//		}
//		else
//		{
//			if( Joy[0].psh&BTN_A )
//			{
//				pCGXImage->LoadReq("DiskImage.drq");
//			}
//		}
//	}
}

void SaveFile()
{
	
	
	
}

#endif
