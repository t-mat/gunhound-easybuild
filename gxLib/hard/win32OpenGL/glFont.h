#if !defined( __GXLIB_HARD_GLFONT_H__ )
#	define __GXLIB_HARD_GLFONT_H__

/**
 *	@brief
 *	フォント描画のヘルプ
 */
class GLFont
{
public:
	/**
	 *	フォントの生成
	 */
	GLFont( void );

	/** */
	~GLFont( void );

	/**
	 *	フォントの生成
	 *	@param width 文字幅
	 *	@param family フォント名
	 */
	void Initialize( Uint32 width, const char* family );

	/**
	 *	解放
	 */
	void Finalize( void );

	/**
	 *	文字列描画
	 */
	void Draw( const char* text, Sint32 x, Sint32 y, Uint32 color );

	/**
	 *	文字列描画
	 */
	void Draw( const char* text, Sint32 x, Sint32 y, Uint8 a, Uint8 r, Uint8 g, Uint8 b );

private:
	HDC hdc_;				///< デバイスコンテキスト
	HFONT font_;			///< フォント
	HBITMAP bmp_;			///< 描画対象
	DWORD* pixel_;			///< 中身
	int texWidth_;			///< 内部テクスチャ幅
	int texHeight_;			///< 内部テクスチャ高さ
	float width_;			///< 文字幅
	int bmpSize_;			///< ビットマップのサイズ

	GLuint texture_;		///< テクスチャ
};

#endif	//	ifndef __GXLIB_HARD_GLFONT_H__
