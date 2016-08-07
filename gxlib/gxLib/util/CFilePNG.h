#ifndef _CFILE_PNG_
#define _CFILE_PNG_

class CFilePNG
{

public:

	Uint32 m_uPos;

	CFilePNG( Uint8* pBytes , Uint32 uSize );
	~CFilePNG()
	{
	}

private:

	gxBool checkHeader( Uint8* pData);
	void readHeader( Uint8 *pData , Uint32 uSize );
	void readBackground( Uint8 *pData );
	void readDat( Uint8 *pData, Uint8* pARGBBuf , Uint32 uSize );
	void createImageRGB( Uint8* pData , CFileTarga* pTga );

	Uint32 readRGB( Uint8 *pData);
	Uint32 distance( Uint32 a, Uint32 b );
	Uint32 getPaethPredictor( Uint32 left, Uint32 above, Uint32 upperLeft );


	// Bit depth [sBIT]

	Uint32 _bitRed;
	Uint32 _bitGreen;
	Uint32 _bitBlue;
	Uint32 _bitAlpha;
	Uint32 _bitGray;
	gxBool _hasBackground;
	Uint8 _background;

	// CRC
	gxBool _bCRCTableComputed;
	Uint32* m_pCRCTable;

	typedef struct StHeader
	{	Uint32 _width				 ;//= data.readUnsignedInt();
		Uint32 _height				 ;//= data.readUnsignedInt();
		Uint8  _bitDepth			 ;//= data.readUnsignedByte();
		Uint8 _colorType			 ;//= data.readUnsignedByte();
		Uint8 _compressionMethod	 ;//= data.readUnsignedByte();
		Uint8 _filterMethod		 	 ;//= data.readUnsignedByte();
		Uint8 _interlaceMethod	 	 ;//= data.readUnsignedByte();

	} StHeader;

	StHeader m_Header;
};
#endif
