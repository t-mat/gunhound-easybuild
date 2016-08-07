#ifndef _gxWebManager_H_
#define _gxWebManager_H_

class gxWebManager
{
public:
	enum {
		enErrorFileNotFound,
		enErrorAccessDenied,
		enErrorFileSizeOver,
	};
	enum {
		enMaxFileSize = 1024*1024,	//1MBまで
		enReadSize    = 1024,		//1KBずつ読む
		enURLLength   = 3*512,
		enPassLength      = 3*512,
		enAccountLength   = 3*512,
	};

	gxWebManager();
	~gxWebManager();
	void Open( gxChar* pURL , gxChar* pUser , gxChar* pPassword );
	void Read();
	gxBool IsReadEnd();
	gxBool IsError();
	Uint8* GetDataImage();
	Uint32 GetDataSize()
	{
		return m_uFileSize;
	}

private:

	Uint8* m_pFileImage;
	Uint8* m_pTemp;
	Uint32 m_uFileSize;
	Uint32 m_uOffset;

	Sint32 m_sStatus;
	Sint32 m_sError;

	gxChar m_sUrl[enURLLength];
	gxChar m_sAcount[enAccountLength];
	gxChar m_sPassword[enPassLength];

	gxBool m_bError;
	gxBool m_bReadComplete;

	void *m_pInterNet;
};


#endif
