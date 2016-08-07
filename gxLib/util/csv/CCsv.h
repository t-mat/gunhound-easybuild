//------------------------------------------------------------------
//CSV操作クラス
//CCsv.h
//------------------------------------------------------------------
//written by ragi
//------------------------------------------------------------------
#ifndef _CCSV_H_
#define _CCSV_H_

#define TEMP_SIZE_MAX (1024*32) //１セルあたり持てるデータの上限値(bytes)

class CCsv {

//---------------------------------------------------------------------------------------------------------------
private:
//---------------------------------------------------------------------------------------------------------------
	enum{
		CSV_MAX_X=128,						//読み込める列のＭＡＸ
		CSV_MAX_Y=65535,					//読み込める行のＭＡＸ
		CSV_MAX_FILE_SIZE = 1024*1024*10,	//外部ＣＳＶを読み込めるファイルサイズＭＡＸ
		CSV_DATA_NONE = 0x00,				//データが無いことを示す
	};

//---------------------------------------------------------------------------------------------------------------
public:
//---------------------------------------------------------------------------------------------------------------

	CCsv();
	~CCsv();

	gxBool LoadFile( gxChar *filename , gxBool cmtActive = gxFalse );
	gxBool ReadFile( Uint8 *pData , Uint32 sz , gxBool cmtActive = gxFalse );
	gxBool SaveFile( gxChar *filename);

	Sint32 GetWidth()
	{
		return rangeX;
	}

	Sint32 GetHeight()
	{
		return rangeY;
	}

	gxBool DelCell(Sint32 x,Sint32 y);

	gxChar* GetCell(Sint32 x,Sint32 y);

	gxBool SetCell(Sint32 x,Sint32 y,gxChar *msg,...);

	gxBool CleanTab();

	gxBool SearchWord(gxChar *p,Sint32 &x,Sint32 &y);

	Sint32 COUNTA( Sint32 x1 , Sint32 y1 , Sint32 x2 , Sint32 y2 );

	gxChar* VLOOKUP( gxChar *name,Sint32 x1,Sint32 x2)
	{
		//------------------------------------------------------------------------
		//X1列目から指定された名前のセルを探して、X2列目のデータを取る。
		//------------------------------------------------------------------------

		for(Sint32 i=0;i<GetHeight();i++)
		{
			if(strcmp(name,GetCell(x1,i)) == 0)
			{
				return GetCell(x2,i);
			}
		}

		return NULL;
	}

private:

	Sint32 rangeX;							//セルの列（Ｘ）
	Sint32 rangeY;							//セルの行（Ｙ）

	gxChar* pCell[ CSV_MAX_Y ][ CSV_MAX_X ];

	gxChar* temporaryBuf;

	gxBool m_bIgnoreComment;				//セル取得時にコメントを考慮する
	gxBool m_bCommentActive;				//ロード時にコメント以降は相手にしない

	gxChar *setNewCell(Sint32 x,Sint32 y,Sint32 len)
	{
		//新しいセルバッファの作成
		pCell[y][x] = new gxChar[ len + 32 ];
		return pCell[y][x];
	}

	gxBool checkRange(Sint32 x,Sint32 y)
	{
		//セルの指定範囲が限界値を超えていないかチェック

		if( x >= CSV_MAX_X || x<0) return gxFalse;
		if( y >= CSV_MAX_Y || y<0) return gxFalse;

		//現在の範囲を超えていたら範囲を更新する
		if(x+1 >= rangeX ) rangeX = x+1;
		if(y+1 >= rangeY ) rangeY = y+1;

		return true;
	}

	void  GetCell(Sint32 x,Sint32 y,gxChar *buf,Uint32 sz);

	gxBool analysingCsv( gxChar *p,Uint32 sz);

	void SetIgnoreFlag( gxBool n)
	{
		m_bIgnoreComment = n;
	}

	gxBool IsIgnore(gxChar *p);

};

#endif
