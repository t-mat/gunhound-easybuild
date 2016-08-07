//--------------------------------------------------
//
// gxMovieManager.cpp
// ムービーの管理を行います、フェード処理などもこちらで
// 管理するため、ハードウェア側で持つエフェクト処理などを
// 独自に使うことを推奨しません
//
//--------------------------------------------------

#include <gxLib.h>
#include <gxLib/util/CFiletarga.h>
#include "gx.h"
#include "gxTexManager.h"
#include "gxMovieManager.h"

SINGLETON_DECLARE_INSTANCE( CMovieManager )

CMovieFile::CMovieFile()
{
	m_pTga = NULL;
	m_FileName[0] = 0x00;
	m_Stat = 0x00;
	m_uMusicBindIndex   = enMusicNone;
	m_uTextureBindIndex = 0;

	pData = NULL;

	width  = 1;
	height = 1;
	fps = 0;
	max = 0;
	now = 0;
	depth  = 1;

	bUpdate = gxFalse;
}

CMovieFile::~CMovieFile()
{
	if( m_pTga ) delete m_pTga;
	m_pTga = NULL;
}

CMovieManager::CMovieManager()
{
	m_pMovie = new CMovieFile[enMovieFileMax];

	m_bUpdate = gxFalse;
}


CMovieManager::~CMovieManager()
{
	SAFE_DELETES( m_pMovie );
}


void CMovieManager::Action()
{
	m_bUpdate = gxFalse;

	CFileTarga *pTga;

	::Movie();

	for( Sint32 ii=0; ii<enMovieFileMax; ii++ )
	{
		if( !m_pMovie[ii].IsExist() ) continue;

		if( m_pMovie[ii].m_Stat&enMovieReqLoad )
		{
			m_pMovie[ii].m_Stat = BIT_OFF( m_pMovie[ii].m_Stat , enMovieReqLoad );

			if( m_pMovie[ii].m_pTga )
			{
				delete m_pMovie[ii].m_pTga;
				m_pMovie[ii].m_pTga = NULL;
			}

			if( m_pMovie[ii].m_pTga == NULL )
			{
				m_pMovie[ii].m_pTga = new CFileTarga();
				m_pMovie[ii].m_pTga->Create( m_pMovie[ii].width , m_pMovie[ii].width , 24 );
			}
		}

		if( !(m_pMovie[ii].m_Stat&enMovieReqPlay) ) continue;

		if( !(m_pMovie[ii].bUpdate) ) continue;	//更新がなければ書かない

		Sint32 w,h;
		Sint32 n = 0;
		Uint32 argb = 0xffffffff;
		Uint8 *pData;

		pTga = m_pMovie[ii].m_pTga;

		w = m_pMovie[ii].width;
		h = m_pMovie[ii].height;
		pData = m_pMovie[ii].pData;

		if (pData == NULL) continue;

		m_bUpdate = gxTrue;
		Sint32 masterTextureX=4, masterTextureY = 4;

		Sint32 texPage = m_pMovie[ii].m_uTextureBindIndex / (masterTextureX*masterTextureY) ;
		Sint32 offsetX = (m_pMovie[ii].m_uTextureBindIndex%masterTextureX)*CTexManager::enPageWidth;
		Sint32 offsetY = ((m_pMovie[ii].m_uTextureBindIndex/ masterTextureX)% masterTextureX)*CTexManager::enPageHeight;

		pTga = CTexManager::GetInstance()->GetAtlasTexture( texPage );

		for (Sint32 y = 0; y < h; y++)
		{
			for (Sint32 x = 0; x < w; x++)
			{
#ifdef USE_OPENGL
				argb = ARGB(0xff, pData[n * 3 + 0], pData[n * 3 + 1], pData[n * 3 + 2]);
#else
				argb = ARGB(0xff, pData[n * 3 + 2], pData[n * 3 + 1], pData[n * 3 + 0]);
#endif
				pTga->SetARGB( offsetX + x, offsetY + y, argb );
//				pTga->SetRGB( offsetX + x, offsetY + y, argb );

				n++;
			}
		}

		CTexManager::GetInstance()->SetForceUpdate(texPage);

//		pTga->SaveFile( "test.tga" );

		//出来あがったテクスチャを読み込む

		//gxLib::ReadTexture( m_pMovie[ii].m_uTextureBindIndex, pTga->getImage(), pTga->getFileSize(), 0xff00ff00);

		//マスターテクスチャに直接書いた方が速いのではないか？

	}

}


void CMovieManager::Draw()
{
	if( m_bUpdate )
	{
		gxLib::UploadTexture();
	}
}



void CMovieManager::LoadMovie( Sint32 sIndex , gxChar *pFileName )
{
	m_pMovie[sIndex].m_Stat = BIT_ON( m_pMovie[sIndex].m_Stat , enMovieReqLoad );

	sprintf( m_pMovie[sIndex].m_FileName , pFileName );
}

void CMovieManager::PlayMovie( Uint32 uIndex )
{
	m_pMovie[uIndex].m_Stat = BIT_ON( m_pMovie[uIndex].m_Stat , enMovieReqPlay );
	m_pMovie[uIndex].m_BeganTime = gxLib::GetTime();
}

void CMovieManager::StopMovie( Uint32 uIndex )
{
	m_pMovie[uIndex].m_Stat = BIT_ON( m_pMovie[uIndex].m_Stat , enMovieReqStop );
}

void CMovieManager::BindMusic( Uint32 uIndex , Sint32 sMusicIndex)
{
	m_pMovie[uIndex].m_uMusicBindIndex = sMusicIndex;
}


CMovieFile* CMovieManager::GetMovie( Sint32 index )
{
	return &m_pMovie[index];
}
void CMovieManager::BindTexturePage( Uint32 uIndex , Sint32 sTexPage )
{
	m_pMovie[uIndex].m_uTextureBindIndex = sTexPage;
}


void CMovieManager::SetLoop( Uint32 uIndex , gxBool bLoop )
{
	if( bLoop )
	{
		m_pMovie[uIndex].m_Stat = BIT_ON( m_pMovie[uIndex].m_Stat , enMovieReqLoop );
	}
	else
	{
		m_pMovie[uIndex].m_Stat = BIT_OFF( m_pMovie[uIndex].m_Stat , enMovieReqLoop );
	}
}

void CMovieManager::SetFrame( Uint32 uIndex , Sint32 frm )
{
	m_pMovie[uIndex].m_Stat  = BIT_ON( m_pMovie[uIndex].m_Stat , enMovieReqPosition );
	m_pMovie[uIndex].JumpFrm = frm;
}



void CMovieManager::SetAutoFrame( Uint32 uIndex , gxBool bAuto )
{
	if( bAuto )
	{
		m_pMovie[uIndex].m_Stat  = BIT_ON( m_pMovie[uIndex].m_Stat , enMovieReqAutoFrm );
	}
	else
	{
		m_pMovie[uIndex].m_Stat  = BIT_OFF( m_pMovie[uIndex].m_Stat , enMovieReqAutoFrm );
	}
}
