#include <gxlib.h>
#include "glut.h"
#include "./glFont.h"

enum {
	enTexWidth = 512,//256,
};

/**
 *	フォントの生成
 */
GLFont::GLFont( void )
	: hdc_( NULL )
	, font_( NULL )
	, bmp_( NULL )
	, pixel_( NULL )
	, texWidth_( enTexWidth )
	, texHeight_( 32 )
	, width_( 0.f )
	, texture_( 0 )
{
}

/** */
GLFont::~GLFont( void )
{
	Finalize();
}

/**
 *	フォントの生成
 */
void GLFont::Initialize( Uint32 width, const char* family )
{
	Finalize();

	//	フォントの属性を設定
	LOGFONTA logFont = { 16, 8, 0, 0, 0, 0, 0, 0, SHIFTJIS_CHARSET, 0, 0, 0, FIXED_PITCH | FF_DONTCARE, { '\0' } };

	logFont.lfHeight = DEBUG_FONT_SIZE;
	logFont.lfWidth  = 0;

	if ( family != NULL ) {
		strcpy( logFont.lfFaceName, family );
	}
	else
	{
		strcpy( logFont.lfFaceName, DEBUG_FONT_NAME );
	}

	//	フォント生成
	font_ = CreateFontIndirectA( &logFont );
	ASSERT( font_ != NULL );

	//	ビットマップの属性を設定
	BITMAPINFO bmpInfo = { {
		sizeof( BITMAPINFOHEADER ),
		texWidth_, -texHeight_, 1, 32, 0,
		32 * texWidth_ * texHeight_ / 8,
		0, 0, 0, 0
	}, };
	//	ビットマップ生成
	bmp_ = CreateDIBSection( NULL, &bmpInfo, DIB_RGB_COLORS, (void**) &pixel_, NULL, 0 );
	ASSERT( bmp_ != NULL );

	//	HDC生成
	hdc_ = CreateCompatibleDC( NULL );
	ASSERT( hdc_ != NULL );

	//	ビットマップを関連付け
	HGDIOBJ old = SelectObject( hdc_, bmp_ );
	ASSERT( old != NULL );

	//	背景色設定
	const RECT rect = { 0, 0, texWidth_, texHeight_ };
	FillRect( hdc_, &rect, (HBRUSH) GetStockObject( GRAY_BRUSH ) );
	//for ( unsigned i = 0; i < bmpInfo.bmiHeader.biSizeImage / 4; ++i ) {
	//	pixel_[i] = 0xFFFFFFFF;
	//}
	memset( pixel_, 0x00, bmpInfo.bmiHeader.biSizeImage );
	bmpSize_ = bmpInfo.bmiHeader.biSizeImage;

	//	フォント関連付け
	SelectObject( hdc_, font_ );
	SetBkMode( hdc_, TRANSPARENT );

	//	描画幅
	width_ = (float) width;

	//	GLテクスチャ生成
	glGenTextures( 1, &texture_ );
	glBindTexture( GL_TEXTURE_2D, texture_ );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texWidth_, texHeight_, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel_ );
}

/**
 *	解放
 */
void GLFont::Finalize( void )
{
	//	GLテクスチャ破棄
	glDeleteTextures( 1, &texture_ );

	//	HDC破棄
	if ( hdc_ != NULL ) { DeleteDC( hdc_ ); }

	//	ビットマップ破棄
	if ( bmp_ != NULL ) { DeleteObject( bmp_ ); }

	//	フォント破棄
	if ( font_ != NULL ) { DeleteFont( font_ ); }

	hdc_ = NULL;
	bmp_ = NULL;
	font_ = NULL;
	texture_ = 0;
	pixel_ = NULL;
}

/**
 *	文字列描画
 */
void GLFont::Draw( const char* text, Sint32 x, Sint32 y, Uint32 color )
{
	Uint8 r, g, b, a;
	r = (Uint8) ((color >> 0x10) & 0xff);
	g = (Uint8) ((color >> 0x08) & 0xff);
	b = (Uint8) ((color >> 0x00) & 0xff);
	a = (Uint8) ((color >> 0x18) & 0xff);

	Draw( text, x, y, a, r, g, b );
}

/**
 *	文字列描画
 */
void GLFont::Draw( const char* text, Sint32 x, Sint32 y, Uint8 a, Uint8 r, Uint8 g, Uint8 b )
{
	//	ビットマップに書き出す
	RECT rect = { 0, 0, texWidth_, texHeight_ };
	SetTextColor( hdc_, RGB( b, g, r ) );		//	文字色
	memset( pixel_, 0x00, bmpSize_ );			//	透明で塗りつぶす

	//	テクスチャからはみ出す範囲は改行する
	static const int kMaxLen = enTexWidth / 8;
	int length = (int) strlen( text );
	int off = 0;
	int rest = 0;
	while ( length > 0 ) {
		int len = length;
		if ( len > kMaxLen ) {
			len = kMaxLen;
		}
		TextOutA( hdc_, 0, off * 16, text + (off * kMaxLen), len );
		++off;
		ASSERT( off < 16 );
		length -= kMaxLen;
	}
	--off;
	length += kMaxLen;

	//	文字部分に不透明度を付ける
	const int loop = (bmpSize_ >> 2);
	for ( int i = 0; i < loop; ++i ) {
		if ( pixel_[i] != 0 ) {
			pixel_[i] |= 0xFF000000;
		}
	}

	//	テクスチャをバインドして書き換え
	glEnable( GL_TEXTURE_2D );								//	テクスチャオン

	glBindTexture( GL_TEXTURE_2D, texture_ );
	glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, texWidth_, 16 * (off + 1) /*texHeight_*/, GL_RGBA, GL_UNSIGNED_BYTE, pixel_ );

	//
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );	//	αオン

	//	--頂点座標計算
	GLfloat cx, cy;
	GLfloat cw, ch;

	//	UV
	static const float invw = 1.f / (WINDOW_W / 2);
	static const float invh = -1.f / (WINDOW_H / 2);
	GLfloat u1, v1, u2, v2;
	u1 = 0.f;
	v1 = 0.f;
	v2 = 16.f / texHeight_;

	//	頂点
	cx = x * invw - 1.0f;
	cy = y * invh + 1.0f;
	ch = width_ * invh;

	//	α計算
	static const float invc = 1.f / 0xff;
	GLfloat fa = a * invc;

	//	描画開始
	//	テクスチャからはみ出してる分
	cw = (width_ / 16) * enTexWidth * invw;
	u2 = 1.f;
	for ( int i = 0; i < off; ++i ) {
		glBegin( GL_TRIANGLE_STRIP );
		{
			//	左上
			glTexCoord2f( u1, v1 );
			glColor4f( 1.f, 1.f, 1.f, fa );
			glVertex2f( cx, cy );
			//	左下
			glTexCoord2f( u1, v1 + v2 );
			glColor4f( 1.f, 1.f, 1.f, fa );
			glVertex2f( cx, cy + ch );
			//	右上
			glTexCoord2f( u1 + u2, v1 );
			glColor4f( 1.f, 1.f, 1.f, fa );
			glVertex2f( cx + cw, cy );
			//	右下
			glTexCoord2f( u1 + u2, v1 + v2 );
			glColor4f( 1.f, 1.f, 1.f, fa );
			glVertex2f( cx + cw, cy + ch );
		}
		glEnd();
		cx += cw;
		v1 += v2;
	}
	//	最後の行
	cw = (length * (width_ * 0.5f)) * invw;
	u2 = (float) (length * 8) / texWidth_;
	glBegin( GL_TRIANGLE_STRIP );
	{
		//	左上
		glTexCoord2f( u1, v1 );
		glColor4f( 1.f, 1.f, 1.f, fa );
		glVertex2f( cx, cy );
		//	左下
		glTexCoord2f( u1, v1 + v2 );
		glColor4f( 1.f, 1.f, 1.f, fa );
		glVertex2f( cx, cy + ch );
		//	右上
		glTexCoord2f( u1 + u2, v1 );
		glColor4f( 1.f, 1.f, 1.f, fa );
		glVertex2f( cx + cw, cy );
		//	右下
		glTexCoord2f( u1 + u2, v1 + v2 );
		glColor4f( 1.f, 1.f, 1.f, fa );
		glVertex2f( cx + cw, cy + ch );
	}
	glEnd();

	//テクスチャの割り当てを切っておく

	glDisable(GL_TEXTURE_2D);

}
