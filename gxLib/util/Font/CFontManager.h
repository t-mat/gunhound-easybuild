//--------------------------------------------------------------------------
//
// CFontManager.h
// written bt tomi.
// 2013.01.21　更新
// 
//--------------------------------------------------------------------------

#include <gxLib.h>

#define DEFAULT_FONT_SIZE (1.0f)

class CFileTarga;

class CFontManager
{
public:
	enum {
		enTempLength = 256*3,
		enTexFileNameLength = 64,
		enDummyLength = 5,
		enDummyLineLength = 16,
		enFontSizeStandard = 16,
	};

	enum {
		enFontSize16,
		enFontSize32,
	};

	typedef struct StKNJformat {
		//------------------------------------
		//KNJデータフォーマット
		//------------------------------------

		Sint32 wordNum;								//文字数
		Sint32 texNum;								//テクスチャ数
		Sint32 pixel;								//文字サイズ
		Sint32 dummy[enDummyLength];
		gxChar filename[enTexFileNameLength];		//テクスチャファイルネーム
		gxChar dummyLine[enDummyLineLength];		//----------------------

		//------------------------------------
		//以下　StKanji データがwordNum個
		//------------------------------------

		//StKanji

		//StKanji

		// ...

	} StKNJformat;

	typedef struct StFontStat {

		StFontStat()
		{
			fScaleX = 1.f;
			fScaleY = 1.f;
			argb = 0xffffffff;
			bProp = gxFalse;
			bRectEnable = gxFalse;
			u=v=w=h=0;
			sPitch=0;
		}

		Float32 fScaleX,fScaleY;
		Uint32  argb;
		gxBool  bProp;
		gxBool  bRectEnable;
		Sint32  u,v,w,h;	//描画矩形
		Sint32  sPitch;		//文字間の幅

	} StFontStat;

	CFontManager();
	~CFontManager();

	//-------------------------------------------------
	//漢字表示リクエスト
	//-------------------------------------------------
	Uint32 Print( Sint32 x , Sint32 y , Sint32 prio , Uint32 argb , gxChar *pStr , ... );

	//詳細設定付き
	Uint32 PrintEx( Sint32 x , Sint32 y , Sint32 prio , StFontStat *pStat, gxChar *pStr , ... );

	//表示サイズのみ得る(NULLならデフォルト)
	//Uint32 GetFontSize( gxChar* pString );
	Uint32 GetStringWidth( StFontStat *pStat, gxChar* pStr , ...);

	//-------------------------------------------------
	//漢字ファイル(*.KNJ)をロード
	//-------------------------------------------------
	gxBool LoadKNJ( Sint32 sPage , gxChar* pFileName , gxBool bDebugNoneLoadTexture = gxFalse );

	//-------------------------------------------------
	//フォントの拡大率を設定する
	//-------------------------------------------------
	void SetFontScale( Float32 fScale )
	{
		m_stDefault.fScaleX = DEFAULT_FONT_SIZE*fScale;
		m_stDefault.fScaleY = DEFAULT_FONT_SIZE*fScale;
	}

	//-------------------------------------------------
	//ピッチをセット
	//-------------------------------------------------
	void SetPitch( Sint32 sPixel )
	{
		//文字と文字の幅を設定する
		m_stDefault.sPitch = sPixel;
	}

	//-------------------------------------------------
	//プロポーショナルをセット
	//-------------------------------------------------
	void SetProportional( gxBool bOn = gxTrue )
	{
		m_stDefault.bProp = bOn;
	}

	//-------------------------------------------------
	// 半角、全角を判定
	//-------------------------------------------------
	gxBool IsZenkaku( gxChar *s );

	//-------------------------------------------------
	//KNJ生成ツール
	//-------------------------------------------------

	//漢字ファイル(*.KNJ)を作成する
	gxBool MakeKNJ( Sint32 sFontPixel , gxChar* pTxtName , gxChar* pTexName );

	//-------------------------------------------------
	//-------------------------------------------------

	SINGLETON_DECLARE( CFontManager );

private:

	typedef struct StKanji {
		//--------------------------------------------
		//漢字テーブルの単一データ
		//--------------------------------------------
		Uint16 uIndex;	//(0)管理コード
		Uint16 uCode;	//(2)漢字コード（デバッグ用）
		Uint8 pg;		//(4)テクスチャページ
		Sint8 x,y;		//(5,6)テクスチャ内の漢字の位置(0～16文字目)　※ピクセルではないので注意
		Uint8 u,w, v,h;	//(7,8,9,10)	横UW / 縦VH
		Uint8 dummy[5];
	} StKanji;

	gxBool readKNJ( Uint8 *pKNJ , Uint32 uSize , gxBool bNoneReadTexture );

	Uint32 printf( Sint32 x , Sint32 y , Sint32 prio , StFontStat *pStat, gxChar *pStr );

	//デフォルトの設定に戻す
	void setDefault( StFontStat* pStat = NULL );

	//512サイズのテクスチャページに変換する
	Sint32 convertTpg512(Sint32 page );

	//---------------------------------------------------------------
	//プロポーションデータ作成
	//---------------------------------------------------------------
	void makeProportionData( Sint32 sTexNum , Sint32 width , gxChar *pTexName , StKanji *pKanjiData );
	void scanMoji( CFileTarga* pTga , Sint32 mx , Sint32 my , Sint32 w,Sint32 h ,gxRect *pRect );

	void setPath(gxChar *pPath);

	//フォントサイズ
	Uint32 m_uFontSize;		//0->16pix / 1->32pix

	//テクスチャの読み込み開始ページ
	Sint32 m_sStartTpg;

	//現在の漢字数
	Uint32   m_uKanjiNum;

	//現在の漢字テーブル
	StKanji* m_pKanjiDB;

	//漢字コードからインデックスへ変換するテーブル
	//※漢字検索の高速化の為、変換を16bitのテーブルで管理します
	Uint16   m_uKanjiList[ 0xffff+32 ];

	//何も指定しなかった時のデフォルト
	StFontStat m_stDefault;

	gxChar KNJFilePath[256];

	//文字列格納用のテンポラリバッファ
	static gxChar s_StrTemp[enTempLength];

};


