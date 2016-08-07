//--------------------------------------------------
//
// gxTexdManager.h
//
//--------------------------------------------------
#ifndef _TEXMANAGER_H_
#define _TEXMANAGER_H_

#include "util/CFileTarga.h"

class CTexManager
{

public:

	enum {
		enMasterPageNum = MAX_MASTERTEX_NUM,//512,//16,
		enMasterWidth   = 256*4,
		enMasterHeight  = 256*4,
		enPageWidth     = 256,
		enPageHeight    = 256,
	};

	CTexManager(void);
	~CTexManager(void);

	//ページからバンク番号を割り出す

	Sint32 getBankIndex( Sint32 pg )
	{
		return pg / m_sTexMax;
	}

	//テクスチャのロード
	gxBool LoadTexture( Sint32 tpg , const gxChar* fileName , Uint32 colorKey=0xff00ff00 , Uint32 ox = 0, Uint32 oy = 0 , Sint32 *w=NULL , Sint32 *h=NULL );

	//バンクのイメージファイル
	Uint8* GetFileImage(Sint32 n)
	{
		return m_Targa[n].GetFileImage();
	}

	CFileTarga* GetAtlasTexture(Sint32 n)
	{
		return &m_Targa[n];
	}

	//バンクのファイルサイズを返す
	Uint32 GetFileSize( Sint32 n)
	{
		return m_Targa[n].GetFileSize();
	}

	//バンクの更新
	gxBool UploadTexture( gxBool bForce = gxFalse ,Sint32 sPage = -1);

	// ------------------------------------------

	//テクスチャの追加
	gxBool addTexture( Sint32 tpg ,CFileTarga* pTga , Uint32 colorKey , Uint32 ox = 0, Uint32 oy = 0 , Sint32 *w=NULL , Sint32 *h=NULL );

	//テクスチャイメージの保存
	gxBool save( Sint32 tpg );

	void changeABGR2RGBA ( Sint32 x , Sint32 y , Uint8* pGraphicsBuffer );
	void changeTop2Bottom( Sint32 x , Sint32 y , Uint8* pGraphicsBuffer );

	void UpdateBankInfo( Sint32 sBank )
	{
		//バンクデータを外部から更新したことにする
		m_bUpdate[ sBank ] = gxTrue;
		m_bUsed[ sBank ]   = gxTrue;
	}

	void SetForceUpdate( Sint32 page , gxBool bUpdate = gxTrue )
	{
		//強制的にアップデートテクスチャ対象にする
		//(マスターテクスチャを直接書き換えた場合などに使用する)
		//ムービーが使用する

		m_bUpdate[ page ] = bUpdate;
	}

	SINGLETON_DECLARE( CTexManager );

private:

	//バンクに入るページの数
	Sint32 m_sTexMax;

	//テクスチャの更新があればバンクに更新フラグを立てる
	gxBool m_bUpdate[enMasterPageNum];
	gxBool m_bUsed[enMasterPageNum];

	//テクスチャのイメージ格納用
	CFileTarga m_Targa[enMasterPageNum];

	// ------------------------------------------

//	Uint32* m_VRAM[enMasterPageNum];
	gxBool rectCopy( Uint32 tpg , Uint32 u , Uint32 v , Uint32 w , Uint32 h , Uint8* pBuf );

};

#endif
