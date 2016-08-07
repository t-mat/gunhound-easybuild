//-------------------------------------------
//
//機種依存型HTTP通信
//
//-------------------------------------------
#include <gxLib.h>
#include "CHttpAccess.h"
#include <WININET.H>

/*
	enum {
		enMaxFileSize = 1024*1024,	//1MBまで
		enURLLength   = 3*512,
		enPassLength      = 3*512,
		enAccountLength   = 3*512,
	};
*/

#define NAME_USERAGENT	"WININET Sample Program"

typedef struct StInterNet {

	HINTERNET hInternet;
	HINTERNET hFile;

} StInterNet;

extern HINSTANCE	app_hinst;

#pragma comment(lib,"wininet.lib")


CHTTPAccess::CHTTPAccess()
{
	m_pInterNet = new StInterNet;

	StInterNet *pInet = (StInterNet*)m_pInterNet;

	m_pFileImage = NULL;
	m_pTemp      = NULL;
	m_uFileSize  = 0;

	m_sStatus    = 0;
	m_sError     = 0;

	m_uOffset    = 0;

	m_bError     = gxFalse;
	m_bReadComplete = gxFalse;

	memset( m_sUrl      ,0x00 , enURLLength );
	memset( m_sAcount   ,0x00 , enAccountLength );
	memset( m_sPassword ,0x00 , enPassLength );

	Uint32 sz = strlen( NAME_USERAGENT )+1;
	//wchar_t* wURL = new wchar_t[ sz ];
	//MultiByteToWideChar( CP_ACP, 0, NAME_USERAGENT, -1, &wURL[0], sz );

	pInet->hInternet = InternetOpen(
		NAME_USERAGENT,
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		0);

	if( pInet->hInternet == NULL )
	{
		m_bError = gxTrue;
	}

	m_pTemp = (Uint8*)malloc( enMaxFileSize );
}


CHTTPAccess::~CHTTPAccess()
{
	if( m_pTemp ) free( m_pTemp );

	StInterNet *pInet = (StInterNet*)m_pInterNet;

	InternetCloseHandle( pInet->hFile );
	InternetCloseHandle( pInet->hInternet );

	delete m_pInterNet;
}

void CHTTPAccess::Open( gxChar* pURL , gxChar* pUser , gxChar* pPassword )
{
	if( m_bError ) return;

	StInterNet *pInet = (StInterNet*)m_pInterNet;

	Uint32 sz = strlen(pURL)+1;
//	wchar_t* wURL = new wchar_t[ sz ];
//	MultiByteToWideChar( CP_ACP, 0, pURL, -1, &wURL[0], sz );

	pInet->hFile = InternetOpenUrl(
		pInet->hInternet,
		pURL,
		NULL,
		0,
		INTERNET_FLAG_RELOAD,
		0);

//	delete wURL;

	if( pInet->hFile == NULL )
	{
		m_bError = gxTrue;
		return;
	}

}


void CHTTPAccess::Read()
{
	if( m_bError ) return;
	if( m_bReadComplete ) return;

	StInterNet *pInet = (StInterNet*)m_pInterNet;

	DWORD readSize;
	BOOL bResult = gxFalse;

	readSize = enReadSize;

	bResult = InternetReadFile(
		pInet->hFile,
		&m_pTemp[ m_uOffset ],
		enReadSize,
		&readSize);

	m_uOffset += readSize;

	m_pTemp[ m_uOffset ] = 0x00;

	/* 全て読み込んだらループを抜ける */
	if( bResult && ( readSize == 0 ) )
	{
		m_pFileImage = (Uint8*)malloc( m_uOffset );
		memcpy( m_pFileImage , m_pTemp , m_uOffset );
		//m_pFileImage[ m_uOffset ] = 0x00;

		free( m_pTemp );
		m_pTemp = NULL;
		m_bReadComplete = gxTrue;

		m_uFileSize = m_uOffset;
		return;
	}

}


gxBool CHTTPAccess::IsReadEnd()
{
	if( m_bError ) return gxTrue;

	return m_bReadComplete;
}

gxBool CHTTPAccess::IsError()
{
	if( m_bError ) return gxTrue;

	return gxFalse;
}


Uint8* CHTTPAccess::GetDataImage()
{
	if( m_bError ) return NULL;

	return m_pFileImage;

}
