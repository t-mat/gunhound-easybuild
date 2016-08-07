#include <gxLib.h>
#include "zlib/zlib.h"
#include "CFileTarga.h"


Uint8* decode( Uint8* pData , Uint32 uSize , Uint32 *uSize2 );


CFilePNG::CFilePNG( Uint8* pBytes , Uint32 uSize )
{
//	_paletteTable = [];
//	_transparentTable = [];


	_bitRed   = 0;
	_bitGreen = 0;
	_bitBlue  = 0;
	_bitAlpha = 0;
	_bitGray  = 0;

	_hasBackground = gxFalse;


	Uint8 Header[32];

	memcpy( Header , &pBytes[0] , 8 );

	m_uPos = 8;

	if (!checkHeader( &Header[0] ) )
	{
		gxLib::DebugLog( "invalid PNG ByteArray");
	}

	Uint32 length = uSize;

	typedef struct StChunk
	{
		Uint32 length;
		gxChar type[4];
		//data
		//crc
	} StChunk;

	Uint8 *pChunkData = NULL;

	Uint8* m_pARGBData = NULL;

	Uint8* pARGBData = NULL;

	while ( gxTrue )
	{

		//チャンクのサイズを取得

		Uint32 uLen = (pBytes[ m_uPos+0 ]<<24)|(pBytes[ m_uPos+1 ]<<16)|(pBytes[ m_uPos+2 ]<<8)|(pBytes[ m_uPos+3 ]<<0);

		m_uPos += 4;

		//チャンクタイプを取得

		gxChar chunkType[8];
		memcpy(  chunkType , &pBytes[ m_uPos ] , 4 );
		chunkType[4] = 0x00;

		m_uPos += 4;

		//チャンクデータを取得

		if( pChunkData ) delete[] pChunkData;

		pChunkData = new Uint8[ uLen ];

		memcpy( pChunkData , &pBytes[ m_uPos ] , uLen );

		m_uPos += uLen;

		//CRCを取得

		Uint32 uCRC = (pBytes[ m_uPos+0 ]<<24)|(pBytes[ m_uPos+1 ]<<16)|(pBytes[ m_uPos+2 ]<<8)|(pBytes[ m_uPos+3 ]<<0);

		m_uPos += 4;

		gxLib::DebugLog("[PNG]CHUNK[%s]" , chunkType );

		if( strcmp(chunkType , "IHDR" ) == 0 )
		{
			readHeader( pChunkData , uLen );

			if( pARGBData == NULL )
			{
				m_pARGBData = new Uint8[ (m_Header._width * m_Header._height)*4*10 ];
				pARGBData = m_pARGBData;
			}
		}

		if( strcmp(chunkType , "IDAT" ) == 0 )
		{
			readDat( pChunkData , pARGBData , uLen );
		}

		if( strcmp(chunkType , "bKGD" ) == 0 )
		{
			readBackground( pChunkData );
		}
/*
		if( strcmp(chunkType , "sBIT" )
		{
			m_uPos += readBitDepth( &pDat[ m_uPos ] );
		}
		if( strcmp(chunkType , "PLTE" )
		{
			m_uPos += readPalette( &pDat[ m_uPos ] , length - m_uPos );
		}
		if( strcmp(chunkType , "tRNS" )
		{
			m_uPos += readTransparent( &pDat[ m_uPos ] );
		}
*/
		if( strcmp(chunkType , "IEND" ) == 0 )
		{
			if( pChunkData ) delete[] pChunkData;
			break;
		}
	}

	CFileTarga tga;

	tga.Create( m_Header._width , m_Header._height , 32 );

	//tga.setTexelImage( m_pARGBData );
	createImageRGB( m_pARGBData , &tga );

	tga.save("test.tga");

	if( m_pARGBData ) delete[] m_pARGBData;

}


gxBool CFilePNG::checkHeader( Uint8* pData)
{
	//ヘッダーチェック

	Uint32 *p1 = (Uint32*)&pData[0];
	Uint32 *p2 = (Uint32*)&pData[4];

//	return header.readUnsignedInt() == 0x89504e47 && header.readUnsignedInt() == 0x0D0A1A0A;

//	if( *p1 == 0x89504e47 && *p2 == 0x0D0A1A0A )
	if( *p1 == 0x474e5089 && *p2 == 0x0A1A0A0D )
	{
		return gxTrue;
	}

	return gxFalse;


/*
137	0x89	非 ASCII 文字。先頭が非 ASCII 値になることで、テキストファイルと明確に区別ができるらしい。
80	0x50	P
78	0x4e	N
71	0x47	G
13	0x0d	CR (Ctrl-M or ^M)
10	v0a	LF (Ctrl-J or ^J)
26	0x1a	Ctrl-Z or ^Z
10	0x0a	LF (Ctrl-J or ^J)
*/

}

void CFilePNG::readHeader( Uint8 *pData , Uint32 uSize )
{
	memcpy( &m_Header , pData , sizeof( m_Header ) );

/*
	_width				 = data.readUnsignedInt();
	_height				 = data.readUnsignedInt();
	_bitDepth			 = data.readUnsignedByte();
	_colorType			 = data.readUnsignedByte();
	_compressionMethod	 = data.readUnsignedByte();
	_filterMethod		 = data.readUnsignedByte();
	_interlaceMethod	 = data.readUnsignedByte();
*/

	m_Header._width  = (pData[ 0+0 ]<<24)|(pData[ 0+1 ]<<16)|(pData[ 0+2 ]<<8)|(pData[ 0+3 ]<<0);
	m_Header._height = (pData[ 4+0 ]<<24)|(pData[ 4+1 ]<<16)|(pData[ 4+2 ]<<8)|(pData[ 4+3 ]<<0);


}

void CFilePNG::readBackground( Uint8 *pData )
{
	_background = pData[0];//data.readUnsignedByte();
	_hasBackground = gxTrue;
}

void CFilePNG::readDat( Uint8 *pData, Uint8* pARGBBuf , Uint32 uSize )
{
	Uint32 uSize2;
	Uint8 *pDecData = decode( pData , uSize , &uSize2 );

	memcpy( pARGBBuf , pDecData , uSize2 );

	delete[] pDecData;

/*
	for( Uint32 ii=0;ii<uSize; ii+=4 )
	{
		pARGBBuf[ ii+0 ] = pData[ ii+0 ];
		pARGBBuf[ ii+1 ] = pData[ ii+1 ];
		pARGBBuf[ ii+2 ] = pData[ ii+2 ];
		pARGBBuf[ ii+3 ] = pData[ ii+3 ];
	}
*/
}


void CFilePNG::createImageRGB( Uint8* pData , CFileTarga* pTga )
{
	Sint32 x,y;
	Uint32 c,filterType, prevColor, currentColor;
	Uint32 p1 = 0;
	x = y = 0;
	c = filterType = prevColor = currentColor = 0;

	Uint32 *aboveColors    = new Uint32[ m_Header._width ];
	Uint32 *currentColors  = new Uint32[ m_Header._width ];

/*
	if (_hasBackground)
	{
		for (y = 0; y < _height; y++) {
			filterType = pData[p1];

			if(filterType == 0)
			{
				for (x = 0; x < _width; x++ )
				{
					c = readRGB(dat);
					c != _background ? _image.setPixel(x, y, c) : _image.setPixel32(x, y, c);
					currentColors[x] = c;
				}
			}
			else if (filterType == 1)
			{
				c = readRGB(dat);
				c != _background ? _image.setPixel(x, y, c) : _image.setPixel32(x, y, c);
				prevColor = c;
				for (x = 1; x < _width; x++ ) {
					c = readRGB(dat);
					currentColor = c + prevColor;
					currentColor != _background ? _image.setPixel(x, y, currentColor) : _image.setPixel32(x, y, currentColor);
					currentColors[x] = currentColor;
					prevColor = c;
				}
			}
			else if (filterType == 2)
			{
				for (x = 0; x < _width; x++ ) {
					c = readRGB(dat);
					currentColor = c + aboveColors[x];
					currentColor != _background ? _image.setPixel(x, y, currentColor) : _image.setPixel32(x, y, currentColor);
					currentColors[x] = currentColor;
				}
			}
			else if (filterType == 3)
			{
				c = readRGB(dat);
				c != _background ? _image.setPixel(x, y, c) : _image.setPixel32(x, y, c);
				prevColor = c;
				for (x = 1; x < _width; x++ ) {
					c = readRGB(dat);
					currentColor = c + (prevColor + aboveColors[x]) /2;
					currentColor != _background ? _image.setPixel(x, y, currentColor) : _image.setPixel32(x, y, currentColor);
					prevColor = c;
					currentColors[x] = currentColor;
				}
			}
			else if (filterType == 4)
			{
				c = readRGB(dat);
				c != _background ? _image.setPixel(x, y, c) : _image.setPixel32(x, y, c);
				prevColor = c;
				for (x = 1; x < _width; x++ ) {
					c = readRGB(dat);
					currentColor = c + getPaethPredictor(prevColor, aboveColors[x], aboveColors[x-1]);
					currentColor != _background ? _image.setPixel(x, y, currentColor) : _image.setPixel32(x, y, currentColor);
					prevColor = c;
					currentColors[x] = currentColor;
				}
			}

			memcpy( aboveColors   , currentColors , m_Header._width*4 );
			//memset( currentColors , 0x00          , m_Header._width*4 );
		}
	}
	else
*/
	{
		Uint32 pixelSize = 4;

		for (y = 0; y < m_Header._height; y++)
		{
			filterType = pData[p1];
			p1 += 1;

			if(filterType == 0)
			{
				for (x = 0; x < m_Header._width; x++ )
				{
					c = readRGB( &pData[p1] );
					p1 += pixelSize;

					pTga->setARGB( x, y, c );
					currentColors[x] = c;
				}
			}
			else if (filterType == 1)
			{
				c = readRGB( &pData[p1] );
				p1 += pixelSize;

				pTga->setARGB( x, y, c );
				prevColor = c;
				for (x = 1; x < m_Header._width; x++ )
				{
					c = readRGB( &pData[p1] );
					p1 += pixelSize;

					currentColor = c + prevColor;
					pTga->setARGB( x, y, currentColor );
					currentColors[x] = currentColor;
					prevColor = c;
				}
			}
			else if (filterType == 2)
			{
				for (x = 0; x < m_Header._width; x++ )
				{
					c = readRGB( &pData[p1] );
					p1 += pixelSize;

					currentColor = c + aboveColors[x];
					pTga->setARGB( x, y, currentColor );
					currentColors[x] = currentColor;
				}
			}
			else if (filterType == 3)
			{
				c = readRGB( &pData[p1] );
				p1 += pixelSize;

				pTga->setARGB( x, y, c );
				prevColor = c;

				for (x = 1; x < m_Header._width; x++ )
				{
					c = readRGB( &pData[p1] );
					p1 += pixelSize;

					currentColor = c + (prevColor + aboveColors[x]) /2;
					pTga->setARGB( x, y, currentColor );
					prevColor = c;
					currentColors[x] = currentColor;
				}
			}
			else if (filterType == 4)
			{
				c = readRGB( &pData[p1] );
				p1 += pixelSize;

				pTga->setARGB( x, y, c );
				prevColor = c;
				for (x = 1; x < m_Header._width; x++ )
				{
					c = readRGB( &pData[p1] );
					p1 += pixelSize;

					currentColor = c + getPaethPredictor(prevColor, aboveColors[x], aboveColors[x-1]);
					pTga->setARGB( x, y, currentColor );
					prevColor = c;
					currentColors[x] = currentColor;
				}
			}
			else
			{
				gxLib::DebugLog("見たことのないフィルタ番号が来た、展開失敗？");
			}

			memcpy( aboveColors   , currentColors , m_Header._width*4 );
			//memset( currentColors , 0x00          , m_Header._width*4 );

//			aboveColors = currentColors;
//			currentColors = [];

		}
	}

	delete[] aboveColors;
	delete[] currentColors;

}


Uint32 CFilePNG::readRGB( Uint8 *pData)
{
	Uint32 r,g,b,a;
	
	r=g=b=a=0x00000000;

	a = 0xff;//pData[3];
	r = pData[0];
	g = pData[1];
	b = pData[2];

//	Uint32 rgb = ( r << 16) + (g << 8) + b;
	Uint32 rgb = (a<<24) | ( b << 16) | (g << 8) | r;

	return rgb;
}


Uint32 distance( Uint32 a, Uint32 b )
{
	return (a < b)? b - a : a - b;
}


Uint32 CFilePNG::getPaethPredictor( Uint32 left, Uint32 above, Uint32 upperLeft )
{
	Uint32 p, pLeft, pAbove, pUpperLeft;

	p = left + above - upperLeft;

	pLeft = distance(p, left);

	pAbove = distance(p, above);

	pUpperLeft = distance(p, upperLeft);

	if (pLeft <= pAbove && pLeft <= pUpperLeft)
	{
		return left;
	}
	else if (pAbove <= pUpperLeft)
	{
		return above;
	}
	else
	{
		return upperLeft;
	}


	return 0;
}



#if 0

Uint32 readPalette( Uint8 *pData , Uint32 uAvailable )
{
	Uint32 sz = 0;

	for ( Sint32 i=0; uAvailable >= 3; i++)
	{
		_paletteTable[i] = readRGB( data );

		uAvailable -= 3;
		sz += 3;
	}

	return sz;
}


Uint32 readTransparent(data:ByteArray)
{
	for (var i:int = 0; data.bytesAvailable > 0; i++) {
		_transparentTable[i] = data.readUnsignedByte();
	}
}

Uint32 readBitDepth( Uint8 *pData )
{
	Uint32 sz = 0;

	switch(m_Header._colorType) {
	case 0:
		_bitGray = pData[0];
		sz ++;
		break;
	case 2:
		_bitRed   = pData[0];
		_bitGreen = pData[1];
		_bitBlue  = pData[2];
		sz += 3;
		break;
	case 3:
		_bitRed   = pData[0];
		_bitGreen = pData[1];
		_bitBlue  = pData[2];
		sz += 3;
		break;
	case 4:
		_bitGray  = pData[0];
		_bitAlpha = pData[1];
		sz += 3;
		break;
	case 6:
		_bitRed   = pData[0];
		_bitGreen = pData[1];
		_bitBlue  = pData[2];
		_bitAlpha = pData[3];
		break;
	default:
		break;
	}

	return sz;
}


void readBackground(data:ByteArray)
{
	if(_colorType == 3) {
		_background = data.readUnsignedByte();
	}
	else if (_colorType == 0 || _colorType == 4) {
		_background = data.readUnsignedShort();
	}
	else if (_colorType == 2 || _colorType == 6) {
		var red:uint = data.readUnsignedShort();
		var green:uint = data.readUnsignedShort();
		var blue:uint = data.readUnsignedShort();
		
		_background = (red << 16) + (green << 8) + blue;
	}
	_hasBackground = true;
}



void createImage(dat:ByteArray)
{
/*
	if (_width == 0 || _height == 0)
		return;
	
	dat.uncompress();
	
	_image = new BitmapData(_width, _height, true);
	
	switch(_colorType) {
		case 2:
			createImageRGB(dat);
			break;
		case 3:
			createImagePalette(dat);
			break;
		case 6:
			createImageRGBA(dat);
		default:
			break;
	}
*/
}

void createImagePalette(dat:ByteArray)
{
/*
	var paletteColors:Array = [];
	var n:int = _paletteTable.length;
	for (var i:int = 0; i < n; i++ ) {
		paletteColors[i] = _paletteTable[i];
		paletteColors[i] += (_transparentTable != null && i in _transparentTable) ? (_transparentTable[i] << 24) : (0xFF << 24)
	}
	
	var x:int, y:int, c:uint;
	
	if(_bitDepth == 8){
		for (y = 0; y < _height; y++) {
			dat.readByte()
			for (x = 0; x < _width; x++ ) {
				c = paletteColors[dat.readUnsignedByte()];
				_image.setPixel32(x, y, c);
			}
		}
	}
	else
	{
		var value:uint;
		var index:uint;
		var position:uint;
		var ones:uint = Math.pow(2,_bitDepth) - 1;
		
		n = dat.length;
		x = y = 0;
		dat.readByte();
		
		while (dat.position < n && y < _height) {
			value <<= position;
			value += dat.readUnsignedByte();
			position += 8;
			
			while (position >= _bitDepth) {
				var shift:uint = ones << (position - _bitDepth);
				index = (value & shift) >> (position - _bitDepth);
				value &= ~shift;
				position -= _bitDepth; 
				c = paletteColors[index];
				_image.setPixel32(x, y, c);
				x++;
				
				if (x >= _width) {
					x = 0;
					y++;
					if(dat.position < n && y < _height)
						dat.readByte();
				}
			}
		}
	}
*/

}
void createImageRGBA(dat:ByteArray)
{
/*
	var x:int, y:int, c:uint, filterType:uint, prevColor:uint, currentColor:uint;
	var aboveColors:Array = [];
	var currentColors:Array = [];
	
	if (_hasBackground) {
		for (y = 0; y < _height; y++) {
			filterType = dat.readByte();
			if(filterType == 0){
				for (x = 0; x < _width; x++ ) {
					c = dat.readUnsignedInt();
					c = (c >> 8) + ((c & 0xFF) << 24);
					(c & 0xFFFFFF) != _background ? _image.setPixel32(x, y, c) : _image.setPixel32(x, y, c & 0xFFFFFF);
					currentColors[x] = c;
				}
			}
			else if (filterType == 1) {
				c = dat.readUnsignedInt();
				c = (c >> 8) + ((c & 0xFF) << 24);
				(c & 0xFFFFFF) != _background ? _image.setPixel32(x, y, c) : _image.setPixel32(x, y, c & 0xFFFFFF);
				prevColor = c;
				for (x = 1; x < _width; x++ ) {
					c = dat.readUnsignedInt();
					c = (c >> 8) + ((c & 0xFF) << 24);
					currentColor = c + prevColor;
					(currentColor & 0xFFFFFF) != _background ? _image.setPixel32(x, y, currentColor) : _image.setPixel32(x, y, currentColor & 0xFFFFFF);
					currentColors[x] = currentColor;
					prevColor = c;
				}
			}
			else if (filterType == 2) {
				for (x = 0; x < _width; x++ ) {
					c = dat.readUnsignedInt();
					c = (c >> 8) + ((c & 0xFF) << 24);
					currentColor = c + aboveColors[x];
					(currentColor & 0xFFFFFF) != _background ? _image.setPixel32(x, y, currentColor) : _image.setPixel32(x, y, currentColor & 0xFFFFFF);
					currentColors[x] = currentColor;
				}
			}
			else if (filterType == 3) {
				c = dat.readUnsignedInt();
				c = (c >> 8) + ((c & 0xFF) << 24);
				(c & 0xFFFFFF) != _background ? _image.setPixel32(x, y, c) : _image.setPixel32(x, y, c & 0xFFFFFF);
				prevColor = c;
				for (x = 1; x < _width; x++ ) {
					c = dat.readUnsignedInt();
					c = (c >> 8) + ((c & 0xFF) << 24);
					currentColor = c + (prevColor + aboveColors[x]) /2;
					(currentColor & 0xFFFFFF) != _background ? _image.setPixel32(x, y, currentColor) : _image.setPixel32(x, y, currentColor & 0xFFFFFF);
					prevColor = c;
					currentColors[x] = currentColor;
				}
			}
			else if (filterType == 4) {
				c = dat.readUnsignedInt();
				c = (c >> 8) + ((c & 0xFF) << 24);
				(c & 0xFFFFFF) != _background ? _image.setPixel32(x, y, c) : _image.setPixel32(x, y, c & 0xFFFFFF);
				prevColor = c;
				for (x = 1; x < _width; x++ ) {
					c = dat.readUnsignedInt();
					c = (c >> 8) + ((c & 0xFF) << 24);
					currentColor = c + getPaethPredictor(prevColor, aboveColors[x], aboveColors[x-1]);
					(currentColor & 0xFFFFFF) != _background ? _image.setPixel32(x, y, currentColor) : _image.setPixel32(x, y, currentColor & 0xFFFFFF);
					prevColor = c;
					currentColors[x] = currentColor;
				}
			}
			aboveColors = currentColors;
			currentColors = [];
		}
	}
	else {
		for (y = 0; y < _height; y++) {
			filterType = dat.readByte();
			if(filterType == 0){
				for (x = 0; x < _width; x++ ) {
					c = dat.readUnsignedInt();
					c = (c >> 8) + ((c & 0xFF) << 24);
					_image.setPixel32(x, y, c);
					currentColors[x] = c;
				}
			}
			else if (filterType == 1) {
				c = dat.readUnsignedInt();
				c = (c >> 8) + ((c & 0xFF) << 24);
				_image.setPixel32(x, y, c);
				prevColor = c;
				for (x = 1; x < _width; x++ ) {
					c = dat.readUnsignedInt();
					c = (c >> 8) + ((c & 0xFF) << 24);
					currentColor = c + prevColor;
					_image.setPixel32(x, y, currentColor);
					currentColors[x] = currentColor;
					prevColor = c;
				}
			}
			else if (filterType == 2) {
				for (x = 0; x < _width; x++ ) {
					c = dat.readUnsignedInt();
					c = (c >> 8) + ((c & 0xFF) << 24);
					currentColor = c + aboveColors[x];
					_image.setPixel32(x, y, currentColor);
					currentColors[x] = currentColor;
				}
			}
			else if (filterType == 3) {
				c = dat.readUnsignedInt();
				c = (c >> 8) + ((c & 0xFF) << 24);
				_image.setPixel32(x, y, c);
				prevColor = c;
				for (x = 1; x < _width; x++ ) {
					c = dat.readUnsignedInt();
					c = (c >> 8) + ((c & 0xFF) << 24);
					currentColor = c + (prevColor + aboveColors[x]) /2;
					_image.setPixel32(x, y, currentColor);
					prevColor = c;
					currentColors[x] = currentColor;
				}
			}
			else if (filterType == 4) {
				c = dat.readUnsignedInt();
				c = (c >> 8) + ((c & 0xFF) << 24);
				_image.setPixel32(x, y, c);
				prevColor = c;
				for (x = 1; x < _width; x++ ) {
					c = dat.readUnsignedInt();
					c = (c >> 8) + ((c & 0xFF) << 24);
					currentColor = c + getPaethPredictor(prevColor, aboveColors[x], aboveColors[x-1]);
					_image.setPixel32(x, y, currentColor);
					prevColor = c;
					currentColors[x] = currentColor;
				}
			}
			aboveColors = currentColors;
			currentColors = [];
		}
	}
*/
}



/*
void makeCRCTable()
{
	m_pCRCTable = [];
	
	for (var n:int = 0; n < 256; n++)
	{
		var c:uint = n;
		for (var k:int = 0; k < 8; k++)
		{
			if (c & 1)
				c = 0xedb88320 ^ (c >>> 1);
			else
				c = c >>> 1;
		}
		m_pCRCTable[n] = c;
	}
	_bCRCTableComputed = true;
}
protected function updateCRC(crc:uint, buf:ByteArray):uint
{
	var c:uint = crc;
	
	if (!_bCRCTableComputed)
		makeCRCTable();
	var n:int = buf.length;
	for (var i:int = buf.position; i < n; i++) {
		c = m_pCRCTable[(c ^ buf.readUnsignedByte()) & 0xFF] ^ (c >>> 8)
	}
	return c;
}
protected function getCRC(buf:ByteArray):uint
{
	return updateCRC(0xFFFFFFFF, buf) ^ 0xFFFFFFFF;
}
*/
#endif

};



Uint8* encode( Uint8* pData , Uint32 uSize , Uint32 *uSize2 )
{
	//pData  RAWデータ
	//uSize  RAWデータサイズ
	//uSize2 圧縮後のデータサイズ
	//戻り値 圧縮後のデータへのポインタ

	Uint8 *pDst;

	Uint32 tempInBufSize  = 1024;
	Uint32 tempOutBufSize = 1024;

	Uint32 p1 = 0;
	Uint32 p2 = 0;

	if( uSize < tempInBufSize )
	{
		tempInBufSize = uSize;
	}

	Bytef* inbuf = new Bytef[ tempInBufSize ];
	Bytef* outbuf= new Bytef[ tempOutBufSize ];


	Sint32 count, status,flush;

	z_stream z;

    z.zalloc = Z_NULL;
    z.zfree = Z_NULL;
    z.opaque = Z_NULL;

    z.avail_in = 0;
	z.next_out = outbuf;
    z.avail_out = tempOutBufSize;

	flush = Z_NO_FLUSH;

	if ( deflateInit( &z , Z_DEFAULT_COMPRESSION ) != Z_OK )
	{
		return NULL;
	}

	z.next_out  = outbuf;
	z.avail_out = tempOutBufSize;
	status = Z_OK;

	pDst = new Uint8[ tempOutBufSize ];

    while (status != Z_STREAM_END)
    {
        if (z.avail_in == 0)
        {
			// 入力残量がゼロになれば入力バッファにデータを入れる
			Uint32 add = tempInBufSize;

            if( (uSize - p1) < tempInBufSize )
            {
				 add = uSize - p1;
			}

            memcpy( inbuf, &pData[p1], add );

            z.next_in  = inbuf;
            z.avail_in = add;

			if (z.avail_in < tempInBufSize ) flush = Z_FINISH;
			p1 += add;
        }

        status = deflate(&z, flush );

        if (status == Z_STREAM_END) break;

        if (status != Z_OK)
        {
			return NULL;
        }

        if (z.avail_out == 0)
        {
			//出力バッファが尽きた場合はとりあえず今の展開済みデータを保存する

			//if (fwrite(outbuf, 1, OUTBUFSIZ, fout) != OUTBUFSIZ)
			//{
			//	return;
			//}

			//memcpy( &pDst[p2] , outbuf , tempOutBufSize );

			//データを追記する

			Uint8* pTemp = new Uint8[p2 + tempOutBufSize];
			memcpy( &pTemp[ 0 ] , pDst ,p2 );
			memcpy( &pTemp[ p2] , outbuf , tempOutBufSize );
			delete[] pDst;
			
			pDst = pTemp;

			p2 += tempOutBufSize;

            z.next_out  = outbuf;
            z.avail_out = tempOutBufSize;
        }
    }

	//残り

	count = tempOutBufSize - z.avail_out;

	if (count != 0)
	{
		//memcpy( &pDst[p2] , outbuf , count );

		//データを追記する

		Uint8* pTemp = new Uint8[p2 + count ];
		memcpy( &pTemp[ 0 ] , pDst , p2 );
		memcpy( &pTemp[ p2] , outbuf , count );
		delete[] pDst;
		
		pDst = pTemp;


		p2 += count;
    }

	if (deflateEnd(&z) != Z_OK)
	{
		return NULL;
	}

	delete[] inbuf;
	delete[] outbuf;

	*uSize2 = p2;

	return pDst;


}


Uint8* decode( Uint8* pData , Uint32 uSize , Uint32 *uSize2 )
{
	//pData  圧縮データ
	//uSize  圧縮データサイズ
	//uSize2 展開後のデータサイズ
	//戻り値 展開後のデータへのポインタ

	Uint8 *pDst;

	Uint32 tempInBufSize  = 1024;
	Uint32 tempOutBufSize = 1024;

	Uint32 p1 = 0;
	Uint32 p2 = 0;

	if( uSize < tempInBufSize )
	{
		tempInBufSize = uSize;
	}

	Bytef* inbuf = new Bytef[ tempInBufSize ];
	Bytef* outbuf= new Bytef[ tempOutBufSize ];

	Sint32 count, status;

	z_stream z;

    z.zalloc = Z_NULL;
    z.zfree = Z_NULL;
    z.opaque = Z_NULL;
    z.next_in = Z_NULL;
    z.avail_in = 0;

	if ( inflateInit( &z ) != Z_OK )
	{
		return NULL;
	}

	z.next_out  = outbuf;
	z.avail_out = tempOutBufSize;
	status = Z_OK;

	pDst = new Uint8[ tempOutBufSize ];

    while (status != Z_STREAM_END)
    {
        if (z.avail_in == 0)
        {
			// 入力残量がゼロになれば入力バッファにデータを入れる
			Uint32 add = tempInBufSize;

            if( (uSize - p1) < tempInBufSize )
            {
				 add = uSize - p1;
			}

            memcpy( inbuf, &pData[p1], add );

            z.next_in = inbuf;
            z.avail_in = add;

			p1 += add;
        }

        status = inflate(&z, Z_NO_FLUSH);

        if (status == Z_STREAM_END) break;

        if (status != Z_OK)
        {
			return NULL;
        }

        if (z.avail_out == 0)
        {
			//出力バッファが尽きた場合はとりあえず今の展開済みデータを保存する

			//if (fwrite(outbuf, 1, OUTBUFSIZ, fout) != OUTBUFSIZ)
			//{
			//	return;
			//}

			//memcpy( &pDst[p2] , outbuf , tempOutBufSize );

			//データを追記する

			Uint8* pTemp = new Uint8[p2 + tempOutBufSize];
			memcpy( &pTemp[ 0 ] , pDst ,p2 );
			memcpy( &pTemp[ p2] , outbuf , tempOutBufSize );
			delete[] pDst;
			
			pDst = pTemp;

			p2 += tempOutBufSize;

            z.next_out  = outbuf;
            z.avail_out = tempOutBufSize;
        }
    }

	//残り

	count = tempOutBufSize - z.avail_out;

	if (count != 0)
	{
		//memcpy( &pDst[p2] , outbuf , count );

		//データを追記する

		Uint8* pTemp = new Uint8[p2 + count ];
		memcpy( &pTemp[ 0 ] , pDst , p2 );
		memcpy( &pTemp[ p2] , outbuf , count );
		delete[] pDst;
		
		pDst = pTemp;


		p2 += count;
    }

	if (inflateEnd(&z) != Z_OK)
	{
		return NULL;
	}

	*uSize2 = p2;

	delete[] inbuf;
	delete[] outbuf;

	return pDst;
}


void testZip()
{
	Uint8 data[]={
		0,1,1,2,2,3,3,4,
		0,1,1,2,2,3,3,4,
		0,1,1,2,2,3,3,4,
		0,1,1,2,2,3,3,4,
		0,1,1,2,2,3,3,4,
	};

	Uint32 uSize1;
	Uint8* pData1;

	Uint32 uSize2;
	Uint8* pData2;

	pData1 = encode( data , sizeof(data) , &uSize1 );

	if( pData1 )
	{
		gxLib::DebugLog( "圧縮率%d%%(%d->%d)", 100*uSize1 / sizeof(data) , sizeof(data) , uSize1 );

		pData2 = decode( pData1 , uSize1 , &uSize2 );

		if( memcmp( data , pData2 , uSize2 ) == 0x00 )
		{
			gxLib::DebugLog("圧縮展開成功");
		}
		else
		{
			gxLib::DebugLog("展開失敗");
		}

	}
	else
	{
		gxLib::DebugLog("圧縮展開失敗");
	}


	delete[] pData1;
	delete[] pData2;

}



void test(gxChar *pFileName )
{
//	testZip();
//	return;

	CFilePNG *pPNG;

	Uint8 *pData;
	Uint32 uSize;

	pData = gxLib::LoadFile( pFileName , &uSize );

	pPNG = new CFilePNG( pData , uSize );

	delete pPNG;
}


