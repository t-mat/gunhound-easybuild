//--------------------------------------------------
//
// gxRender.h
// レンダリングする為の情報を管理します
//
//--------------------------------------------------

#include <gxLib.h>
#include "gx.h"
#include "gxOrderManager.h"
#include "gxTexmanager.h"
#include "gxRender.h"

SINGLETON_DECLARE_INSTANCE( CRender )

#define INVALID_TEXTURE    (-99)
#define NONE_TEXTURE (-1)
#define INVALID_ATTR    (0xffffffff)
#define INVALID_ORDER   (0xffffffff)

CRender::CRender()
{

	m_pCommand     = new CCommandList[enCommandMax];
	m_pCutomVertex = new StCustomVertex[enVertexMax];

#if INDEXBUFFER_BIT == 32
	m_pIndexBuffer = new Uint32[enIndexMax];
#else
	m_pIndexBuffer = new Uint16[enIndexMax];
#endif

	m_sOldBankNum  = NONE_TEXTURE;
	m_sStartVertex = 0;
	m_sLastVertex  = 0;
	m_sStartIndex  = 0;
	m_sLastIndex   = 0;

	m_sOldAttribute = INVALID_ATTR;
	m_sLastOdrType      = INVALID_ORDER;

	m_sIBufferNum = 0;
	m_sIndexCnt   = 0;

	m_fTexturePageWidth  = CTexManager::enMasterWidth;
	m_fTexturePageHeight = CTexManager::enMasterHeight;
	m_sPageX = CTexManager::enMasterWidth / CTexManager::enPageWidth;

	m_uBackGroundColor = 0xff010101;
	SetClearColor( m_uBackGroundColor );

}


CRender::~CRender()
{
	delete[] m_pCommand;
	delete[] m_pCutomVertex;
	delete[] m_pIndexBuffer;
}


void CRender::Action()
{
	//レンダリング用のコマンドを生成する

	Sint32 sMax = COrderManager::GetInstance()->GetOrderNum();
	COrderManager *pOdrMgr = COrderManager::GetInstance();

	//コマンド初期化

	m_sCommandNum = 0;
	m_sVBufferNum = 0;
	m_sIBufferNum = 0;

	m_sOldBankNum   = INVALID_TEXTURE;
	m_sStartVertex  = 0;
	m_sLastVertex   = 0;
	m_sStartIndex   = 0;
	m_sLastIndex	= 0;
	m_sIndexCnt     = 0;

	m_sOldAttribute = INVALID_ATTR;
	m_sLastOdrType      = INVALID_ORDER;

	//zソート
	COrderManager::GetInstance()->ZSort();

	Uint32* pZsortBuf = COrderManager::GetInstance()->getZsortBuf();

	for(Uint16 ii=0; ii<sMax; ii++)
	{
		makeCommand( (Uint32)pZsortBuf[ii] );
	}

/*
	if( m_bPolygonReq )
	{
		addCommand( INVALID_TYPE , 0,0,0,0, 0,0 );
	}
*/

}


INLINE void CRender::makeCommand( Uint32 index )
{
	//コマンドを生成する

	StOrder* pOdr     = COrderManager::GetInstance()->get( index );
	Sint32 sBankIndex = CTexManager::GetInstance()->getBankIndex(pOdr->pg);

	if( index == 70 )
	{
		index = 70;
	}

	m_sIndexCnt = m_sVBufferNum;

	switch( pOdr->sType ){
	case enOrderTypePoint:
		atacheTexture( NONE_TEXTURE );
		setAttribute( pOdr->sAtribute );
		drawPoint( pOdr );

		while( pOdr->m_pNextOrder )
		{
			drawPoint( pOdr->m_pNextOrder );
			pOdr = pOdr->m_pNextOrder;
		}
		break;

	case enOrderTypeTriangle:
		atacheTexture( NONE_TEXTURE );
		setAttribute( pOdr->sAtribute );
		drawTriangle( pOdr );
		while( pOdr->m_pNextOrder )
		{
			drawTriangle( pOdr->m_pNextOrder );
			pOdr = pOdr->m_pNextOrder;
		}
		break;

	case enOrderTypeLine:
		atacheTexture( NONE_TEXTURE );
		setAttribute( pOdr->sAtribute );
		drawLine( pOdr );
		while( pOdr->m_pNextOrder )
		{
			drawLine( pOdr->m_pNextOrder );
			pOdr = pOdr->m_pNextOrder;
		}
		break;

	case enOrderTypWired:
		atacheTexture( NONE_TEXTURE );
		setAttribute( pOdr->sAtribute );
		drawLineStrip( pOdr );
		while( pOdr->m_pNextOrder )
		{
			drawLineStrip( pOdr->m_pNextOrder );
			pOdr = pOdr->m_pNextOrder;
		}
		break;

	case enOrderTypeNoneTexPolygon:
		atacheTexture( NONE_TEXTURE );
		setAttribute( pOdr->sAtribute );
		drawSprite( pOdr );
		while( pOdr->m_pNextOrder )
		{
			drawSprite( pOdr->m_pNextOrder );
			pOdr = pOdr->m_pNextOrder;
		}
		break;

	case enOrderTypeTexturePolygon:
		atacheTexture( sBankIndex );
		setAttribute( pOdr->sAtribute );
		drawSprite( pOdr );
		while( pOdr->m_pNextOrder )
		{
			drawSprite( pOdr->m_pNextOrder );
			pOdr = pOdr->m_pNextOrder;
		}
		break;

	case enOrderTypeTextureTriangle:
		atacheTexture( sBankIndex );
		setAttribute( pOdr->sAtribute );
		drawTriangleSprite( pOdr );
		while( pOdr->m_pNextOrder )
		{
			drawTriangleSprite( pOdr->m_pNextOrder );
			pOdr = pOdr->m_pNextOrder;
		}
		break;

	case enOrderTypeFont:
		drawFont( pOdr );
		atacheTexture( NONE_TEXTURE );
		break;
	default:
		break;
	}

	return;

}


INLINE void CRender::drawSprite( StOrder* pOdr )
{
	//４頂点を確定する

	pOdr->x1[0] = (Float32)pOdr->x;
	pOdr->y1[0] = (Float32)pOdr->y;

	pOdr->x1[1] = (Float32)pOdr->x+pOdr->w;
	pOdr->y1[1] = (Float32)pOdr->y;

	pOdr->x1[2] = (Float32)pOdr->x+pOdr->w;
	pOdr->y1[2] = (Float32)pOdr->y+pOdr->h;

	pOdr->x1[3] = (Float32)pOdr->x;
	pOdr->y1[3] = (Float32)pOdr->y+pOdr->h;

//*
	pOdr->x1[0] -= 0.5f;
	pOdr->y1[0] -= 0.5f;

	pOdr->x1[1] -= 0.5f;
	pOdr->y1[1] -= 0.5f;

	pOdr->x1[2] -= 0.5f;
	pOdr->y1[2] -= 0.5f;

	pOdr->x1[3] -= 0.5f;
	pOdr->y1[3] -= 0.5f;

//*/

	if( pOdr->sAtribute&ATR_FLIP_X) pOdr->sx *= -1.f;
	if( pOdr->sAtribute&ATR_FLIP_Y) pOdr->sy *= -1.f;

	//スプライトの描画
	if( pOdr->sx == 0.f ||  pOdr->sy == 0.f )
	{
		//そもそも描画しない（一番速い）
		return;
	}
	else if( pOdr->rz )
	{
		//回転がある（低速）
		spriteRotationScale( pOdr );
	}
	else if( pOdr->sx!=1.f || pOdr->sy!=1.f )
	{
		//スケール値が１以外（中速）
		spriteScale( pOdr );
	}
	else
	{
		//特に何も加工しない（高速）
		spriteNormal( pOdr );
	}

/*
	if( pOdr->m_pNextOrder )
	{
		drawSprite( pOdr->m_pNextOrder );
	}
*/

}


INLINE void CRender::setVertex( StCustomVertex *pVtx1 , StOrder* pOdr )
{
	//頂点情報を与える
	Sint32 sVtxCnt = m_sVBufferNum;
	Sint32 sIdxCnt = m_sIBufferNum;

	pVtx1->x   = pOdr->x1[0];
	pVtx1->y   = pOdr->y1[0];
	pVtx1->z   = 0.f;
	pVtx1->rhw = 1.0f;
	pVtx1->col = pOdr->color[0];

	m_sVBufferNum ++;
	pVtx1 ++;

	pVtx1->x   = pOdr->x1[1];
	pVtx1->y   = pOdr->y1[1];
	pVtx1->z   = 0.f;
	pVtx1->rhw = 1.0f;
	pVtx1->col = pOdr->color[1];

	m_sVBufferNum ++;
	pVtx1 ++;

	pVtx1->x   = pOdr->x1[3];
	pVtx1->y   = pOdr->y1[3];
	pVtx1->z   = 0.f;
	pVtx1->rhw = 1.0f;
	pVtx1->col = pOdr->color[3];

	m_sVBufferNum ++;
	pVtx1 ++;

	pVtx1->x   = pOdr->x1[2];
	pVtx1->y   = pOdr->y1[2];
	pVtx1->z   = 0.f;
	pVtx1->rhw = 1.0f;
	pVtx1->col = pOdr->color[2];

	m_sVBufferNum ++;
	pVtx1 ++;

	m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt+0;	m_sIBufferNum ++;
	m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt+1;	m_sIBufferNum ++;
	m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt+2;	m_sIBufferNum ++;

	m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt+2;	m_sIBufferNum ++;
	m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt+1;	m_sIBufferNum ++;
	m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt+3;	m_sIBufferNum ++;
	m_sIndexCnt += (m_sVBufferNum - sVtxCnt);
}



INLINE void CRender::setTextureUV( Sint32 tpg , StCustomVertex *pVtx0 , StOrder* pOdr )
{
	//テクスチャにUV座標を与える
	if( tpg < 0 )
	{
		return;
	}

	pVtx0->u = getTetureOffsetX( tpg , pOdr->u );
	pVtx0->v = getTetureOffsetY( tpg , pOdr->v );
	pVtx0 ++;

	pVtx0->u   = getTetureOffsetX( tpg , pOdr->u+pOdr->w );
	pVtx0->v   = getTetureOffsetY( tpg , pOdr->v );
	pVtx0 ++;

	pVtx0->u   = getTetureOffsetX( tpg , pOdr->u );
	pVtx0->v   = getTetureOffsetY( tpg , pOdr->v+pOdr->h );
	pVtx0 ++;

	pVtx0->u   = getTetureOffsetX( tpg , pOdr->u+pOdr->w );
	pVtx0->v   = getTetureOffsetY( tpg , pOdr->v+pOdr->h );
	pVtx0 ++;

}


INLINE Float32 CRender::getTetureOffsetX( Sint32 page , Sint32 x )
{
	Sint32 ox = page%m_sPageX;

	ox *= CTexManager::enPageWidth;
	x += ox;

	return x/m_fTexturePageWidth;
}

INLINE Float32 CRender::getTetureOffsetY( Sint32 page , Sint32 y )
{
	Sint32 oy = page/m_sPageX;

	oy *= CTexManager::enPageHeight;
	y += oy;

	return y/m_fTexturePageHeight;
}

INLINE void CRender::setAttribute( Uint32 sAtr )
{
	//アトリビュートの設定

	sAtr &= 0x0f;

	if( sAtr == m_sOldAttribute )
	{
		return;
	}

	m_sOldAttribute = sAtr;

	if( sAtr&ATR_ALPHA_ADD )
	{
		stCmd.sType = eCmdChgAttributeAlphaAdd;
		addCommand( &stCmd );
	}
	else if( sAtr&ATR_ALPHA_SUB )
	{
		stCmd.sType = eCmdChgAttributeAlphaSub;
		addCommand( &stCmd );
	}
	else if( sAtr&ATR_ALPHA_CRS )
	{
		stCmd.sType = eCmdChgAttributeAlphaAdd;
		addCommand( &stCmd );
	}
	else if( sAtr&ATR_ALPHA_RVS )
	{
		stCmd.sType = eCmdChgAttributeAlphaSub;
		addCommand( &stCmd );
	}
	else
	{
		stCmd.sType = eCmdChgAttributeAlphaNml;
		addCommand( &stCmd );
	}

}

// -----------------------------------------------------------------------------------
// 描画コマンド発行
// -----------------------------------------------------------------------------------

INLINE void CRender::atacheTexture( Sint32 sBankIndex )
{
	//テクスチャのアタッチ

	if( sBankIndex == m_sOldBankNum )
	{
		//前回と同じテクスチャページならアタッチしない
		return;
	}

	m_sOldBankNum = sBankIndex;

	if( sBankIndex == NONE_TEXTURE )
	{
		//ノンテクスチャ
		stCmd.sType = eCmdNoneTexture;
		addCommand( &stCmd );
	}
	else
	{
		//テクスチャ
		stCmd.sType = eCmdChgTexture;
		stCmd.arg1  = sBankIndex;
		addCommand( &stCmd );
	}

}


INLINE void CRender::drawPoint( StOrder* pOdr )
{
	//ポイント描画

	StCustomVertex *pVtx0 = &m_pCutomVertex[ m_sVBufferNum ];
	StCustomVertex *pVtx1 = pVtx0;

	Sint32 sVtxCnt = m_sVBufferNum;
	Sint32 sIdxCnt = m_sIBufferNum;

	pVtx1->x   = pOdr->x1[0];
	pVtx1->y   = pOdr->y1[0];
	pVtx1->z   = 0.f;
	pVtx1->rhw = 1.0f;
	pVtx1->col = pOdr->color[0];

	m_sVBufferNum ++;
	pVtx1 ++;

	m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt;	m_sIBufferNum ++;
	m_sIndexCnt ++;

	stCmd.sType        = eCmdRenderPoint;
	stCmd.sStartVertex = sVtxCnt;
	stCmd.sLastVertex  = m_sVBufferNum;
	stCmd.sStartIndex  = sIdxCnt;
	stCmd.sLastIndex   = m_sIBufferNum;

	addCommand( &stCmd );

/*
	if( pOdr->m_pNextOrder )
	{
		drawPoint( pOdr->m_pNextOrder );
	}
*/

}


INLINE void CRender::drawLine( StOrder* pOdr )
{
	//ライン描画

	StCustomVertex *pVtx0 = &m_pCutomVertex[ m_sVBufferNum ];
	StCustomVertex *pVtx1 = pVtx0;

	Sint32 sVtxCnt = m_sVBufferNum;
	Sint32 sIdxCnt  = m_sIBufferNum;

	if( pOdr->opt )
	{
		//ラインストリップ（ボックスを描画 ）

		pVtx1->x   = pOdr->x1[0];
		pVtx1->y   = pOdr->y1[0];
		pVtx1->z   = 0.f;
		pVtx1->rhw = 1.0f;
		pVtx1->col = pOdr->color[0];

		pVtx1 ++;
		m_sVBufferNum ++;

		pVtx1->x   = pOdr->x1[1];
		pVtx1->y   = pOdr->y1[1];
		pVtx1->z   = 0.f;
		pVtx1->rhw = 1.0f;
		pVtx1->col = pOdr->color[1];

		pVtx1 ++;
		m_sVBufferNum ++;

		pVtx1->x   = pOdr->x1[2];
		pVtx1->y   = pOdr->y1[2];
		pVtx1->z   = 0.f;
		pVtx1->rhw = 1.0f;
		pVtx1->col = pOdr->color[2];

		pVtx1 ++;
		m_sVBufferNum ++;

		pVtx1->x   = pOdr->x1[3];
		pVtx1->y   = pOdr->y1[3];
		pVtx1->z   = 0.f;
		pVtx1->rhw = 1.0f;
		pVtx1->col = pOdr->color[3];

		pVtx1 ++;
		m_sVBufferNum ++;

		pVtx1->x   = pOdr->x1[0];
		pVtx1->y   = pOdr->y1[0];
		pVtx1->z   = 0.f;
		pVtx1->rhw = 1.0f;
		pVtx1->col = pOdr->color[0];

		pVtx1 ++;
		m_sVBufferNum ++;

		m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt+0;	m_sIBufferNum ++;
		m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt+1;	m_sIBufferNum ++;
		m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt+2;	m_sIBufferNum ++;
		m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt+3;	m_sIBufferNum ++;
		m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt+4;	m_sIBufferNum ++;
		m_sIndexCnt += (m_sVBufferNum - sVtxCnt);

		stCmd.sType        = eCmdRenderLineStrip;
		stCmd.sStartVertex = sVtxCnt;
		stCmd.sLastVertex  = m_sVBufferNum;
		stCmd.sStartIndex  = sIdxCnt;
		stCmd.sLastIndex   = m_sIBufferNum;
		stCmd.arg1         = pOdr->sx*100;
		addCommand( &stCmd );

	}
	else
	{
		//ライン
		pVtx1->x   = pOdr->x1[0];
		pVtx1->y   = pOdr->y1[0];
		pVtx1->z   = 0.f;
		pVtx1->rhw = 1.0f;
		pVtx1->col = pOdr->color[0];

		m_sVBufferNum ++;
		pVtx1 ++;

		pVtx1->x   = pOdr->x1[1];
		pVtx1->y   = pOdr->y1[1];
		pVtx1->z   = 0.f;
		pVtx1->rhw = 1.0f;
		pVtx1->col = pOdr->color[1];

		m_sVBufferNum ++;
		pVtx1 ++;

		m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt+0;	m_sIBufferNum ++;
		m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt+1;	m_sIBufferNum ++;
		m_sIndexCnt += (m_sVBufferNum - sVtxCnt);

		stCmd.sType        = eCmdRenderLineNormal;
		stCmd.sStartVertex = sVtxCnt;
		stCmd.sLastVertex  = m_sVBufferNum;
		stCmd.sStartIndex  = sIdxCnt;
		stCmd.sLastIndex   = m_sIBufferNum;
		stCmd.arg1         = pOdr->sx*100;
		addCommand( &stCmd );

/*
		if( pOdr->m_pNextOrder )
		{
			drawLine( pOdr->m_pNextOrder );
		}
*/

	}

}


INLINE void CRender::drawTriangle( StOrder* pOdr )
{
	//トライアングル描画

	StCustomVertex *pVtx0 = &m_pCutomVertex[ m_sVBufferNum ];
	StCustomVertex *pVtx1 = pVtx0;

	Sint32 sVtxCnt = m_sVBufferNum;
	Sint32 sIdxCnt  = m_sIBufferNum;

	pVtx1->x   = pOdr->x1[0];
	pVtx1->y   = pOdr->y1[0];
	pVtx1->z   = 0.f;
	pVtx1->rhw = 1.0f;
	pVtx1->col = pOdr->color[0];

	m_sVBufferNum ++;
	pVtx1 ++;

	pVtx1->x   = pOdr->x1[1];
	pVtx1->y   = pOdr->y1[1];
	pVtx1->z   = 0.f;
	pVtx1->rhw = 1.0f;
	pVtx1->col = pOdr->color[1];

	m_sVBufferNum ++;
	pVtx1 ++;

	pVtx1->x   = pOdr->x1[2];
	pVtx1->y   = pOdr->y1[2];
	pVtx1->z   = 0.f;
	pVtx1->rhw = 1.0f;
	pVtx1->col = pOdr->color[2];

	m_sVBufferNum ++;
	pVtx1 ++;

	m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt+0;	m_sIBufferNum ++;
	m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt+1;	m_sIBufferNum ++;
	m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt+2;	m_sIBufferNum ++;
	m_sIndexCnt += (m_sVBufferNum - sVtxCnt);

	stCmd.sType        = eCmdRenderTriangle;
	stCmd.sStartVertex = sVtxCnt;
	stCmd.sLastVertex  = m_sVBufferNum;
	stCmd.sStartIndex  = sIdxCnt;
	stCmd.sLastIndex   = m_sIBufferNum;

	addCommand( &stCmd );

/*
	if( pOdr->m_pNextOrder )
	{
		drawTriangle( pOdr->m_pNextOrder );
	}
*/


}


INLINE void CRender::drawTriangleSprite( StOrder* pOdr )
{
	//トライアングルースプライト描画

	StCustomVertex *pVtx0 = &m_pCutomVertex[ m_sVBufferNum ];
	StCustomVertex *pVtx1 = pVtx0;

	Sint32 sVtxCnt = m_sVBufferNum;
	Sint32 sIdxCnt  = m_sIBufferNum;

	pVtx1->x   = pOdr->x1[0];
	pVtx1->y   = pOdr->y1[0];
	pVtx1->z   = 0.f;
	pVtx1->rhw = 1.0f;
	pVtx1->col = pOdr->color[0];
	pVtx1->u = getTetureOffsetX( pOdr->pg , (Sint32)pOdr->u1[0] );
	pVtx1->v = getTetureOffsetY( pOdr->pg , (Sint32)pOdr->v1[0] );
	pVtx0 ++;

	m_sVBufferNum ++;
	pVtx1 ++;

	pVtx1->x   = pOdr->x1[1];
	pVtx1->y   = pOdr->y1[1];
	pVtx1->z   = 0.f;
	pVtx1->rhw = 1.0f;
	pVtx1->col = pOdr->color[1];
	pVtx1->u = getTetureOffsetX( pOdr->pg , (Sint32)pOdr->u1[1] );
	pVtx1->v = getTetureOffsetY( pOdr->pg , (Sint32)pOdr->v1[1] );

	m_sVBufferNum ++;
	pVtx1 ++;

	pVtx1->x   = pOdr->x1[2];
	pVtx1->y   = pOdr->y1[2];
	pVtx1->z   = 0.f;
	pVtx1->rhw = 1.0f;
	pVtx1->col = pOdr->color[2];
	pVtx1->u = getTetureOffsetX( pOdr->pg , (Sint32)pOdr->u1[2] );
	pVtx1->v = getTetureOffsetY( pOdr->pg , (Sint32)pOdr->v1[2] );

	m_sVBufferNum ++;
	pVtx1 ++;

	m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt+0;	m_sIBufferNum ++;
	m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt+1;	m_sIBufferNum ++;
	m_pIndexBuffer[m_sIBufferNum] = m_sIndexCnt+2;	m_sIBufferNum ++;
	m_sIndexCnt += (m_sVBufferNum - sVtxCnt);

	stCmd.sType        = eCmdRenderTriangle;
	stCmd.sStartVertex = sVtxCnt;
	stCmd.sLastVertex  = m_sVBufferNum;
	stCmd.sStartIndex  = sIdxCnt;
	stCmd.sLastIndex   = m_sIBufferNum;
	addCommand( &stCmd );

/*
	if( pOdr->m_pNextOrder )
	{
		drawTriangleSprite( pOdr->m_pNextOrder );
	}
*/

}


INLINE void CRender::drawLineStrip( StOrder* pOdr )
{
	//ワイヤーライン描画

	Sint32 tpg = pOdr->pg;	//テクスチャページ
	StCustomVertex *pVtx0 = &m_pCutomVertex[ m_sVBufferNum ];
	StCustomVertex *pVtx1 = pVtx0;

	Sint32 sVtxCnt = m_sVBufferNum;
	Sint32 sIdxCnt = m_sIBufferNum;

	pOdr->x1[0] = (Float32)pOdr->x;
	pOdr->y1[0] = (Float32)pOdr->y;

	pOdr->x1[1] = (Float32)pOdr->x+pOdr->w;
	pOdr->y1[1] = (Float32)pOdr->y;

	pOdr->x1[2] = (Float32)pOdr->x+pOdr->w;
	pOdr->y1[2] = (Float32)pOdr->y+pOdr->h;

	pOdr->x1[3] = (Float32)pOdr->x;
	pOdr->y1[3] = (Float32)pOdr->y+pOdr->h;

	for(int ii=0;ii<4;ii++)
	{
		gxPoint Pt;

		//中心位置を原点にずらす
		Pt.x = pOdr->x1[ii] - pOdr->x;
		Pt.y = pOdr->y1[ii] - pOdr->y;

		Pt.x -= pOdr->cx;
		Pt.y -= pOdr->cy;

		//拡大処理
		Pt.x *= pOdr->sx;
		Pt.y *= pOdr->sy;

		//回転処理
		Float32 rot = pOdr->rz;

		gxUtil::RotationPoint( &Pt , rot );

		//元に戻す
		pOdr->x1[ii] = Pt.x + pOdr->x;
		pOdr->y1[ii] = Pt.y + pOdr->y;
	}

	//lineStrip描画に作業を振る

	StOrder tmp;

	tmp.opt = 1;
	tmp.x1[0]    = pOdr->x1[0];
	tmp.y1[0]    = pOdr->y1[0];
	tmp.color[0] = pOdr->color[0];

	tmp.x1[1]    = pOdr->x1[1];
	tmp.y1[1]    = pOdr->y1[1];
	tmp.color[1] = pOdr->color[1];

	tmp.x1[2]    = pOdr->x1[2];
	tmp.y1[2]    = pOdr->y1[2];
	tmp.color[2] = pOdr->color[2];

	tmp.x1[3]    = pOdr->x1[3];
	tmp.y1[3]    = pOdr->y1[3];
	tmp.color[3] = pOdr->color[3];

	drawLine( &tmp );

/*
	if( pOdr->m_pNextOrder )
	{
		drawLineStrip( pOdr->m_pNextOrder );
	}
*/

}


INLINE void CRender::spriteRotationScale( StOrder* pOdr )
{
	//スプライト：：回転 / 拡大 / 縮小処理

	Sint32 tpg = pOdr->pg;	//テクスチャページ
	StCustomVertex *pVtx0 = &m_pCutomVertex[ m_sVBufferNum ];
	StCustomVertex *pVtx1 = pVtx0;

	Sint32 sVtxCnt = m_sVBufferNum;
	Sint32 sIdxCnt  = m_sIBufferNum;

	for(int ii=0;ii<4;ii++)
	{
		gxPoint Pt;

		//中心位置を原点にずらす
		Pt.x = pOdr->x1[ii] - pOdr->x;
		Pt.y = pOdr->y1[ii] - pOdr->y;

		Pt.x -= pOdr->cx;
		Pt.y -= pOdr->cy;

		//拡大処理
		Pt.x *= ABS(pOdr->sx);
		Pt.y *= ABS(pOdr->sy);

		//回転処理
		Float32 rot = pOdr->rz;

		gxUtil::RotationPoint( &Pt , rot );

		//反転処理
		if( pOdr->sx< 0 ) Pt.x *= -1.f;
		if( pOdr->sy< 0 ) Pt.y *= -1.f;

		//元に戻す
		pOdr->x1[ii] = Pt.x + pOdr->x;
		pOdr->y1[ii] = Pt.y + pOdr->y;
	}


	pVtx1 = pVtx0;

	//頂点設定
	setVertex( pVtx1, pOdr );

	pVtx1 = pVtx0;

	//テクスチャ設定
	setTextureUV(tpg , pVtx1, pOdr );

	stCmd.sType        = eCmdRenderSquare;
	stCmd.sStartVertex = sVtxCnt;
	stCmd.sLastVertex  = m_sVBufferNum;
	stCmd.sStartIndex  = sIdxCnt;
	stCmd.sLastIndex   = m_sIBufferNum;
	addCommand( &stCmd );

}

INLINE void CRender::spriteScale( StOrder* pOdr )
{
	//スプライト：：拡大 / 縮小処理

	Sint32 tpg = pOdr->pg;	//テクスチャページ

	StCustomVertex *pVtx0 = &m_pCutomVertex[ m_sVBufferNum ];
	StCustomVertex *pVtx1 = pVtx0;

	Sint32 sVtxCnt = m_sVBufferNum;
	Sint32 sIdxCnt = m_sIBufferNum;

	for(int ii=0;ii<4;ii++)
	{
		gxPoint Pt;

		//中心位置を原点にずらす
		Pt.x = pOdr->x1[ii] - pOdr->x;
		Pt.y = pOdr->y1[ii] - pOdr->y;

		Pt.x -= pOdr->cx;
		Pt.y -= pOdr->cy;

		//回転処理

		//拡大処理
		Pt.x *= pOdr->sx;
		Pt.y *= pOdr->sy;

		//元に戻す
		pOdr->x1[ii] = Pt.x + pOdr->x;
		pOdr->y1[ii] = Pt.y + pOdr->y;
	}

	pVtx1 = pVtx0;

	//頂点設定
	setVertex( pVtx1, pOdr );

	pVtx1 = pVtx0;

	//テクスチャ設定
	setTextureUV(tpg , pVtx1, pOdr );

	stCmd.sType        = eCmdRenderSquare;
	stCmd.sStartVertex = sVtxCnt;
	stCmd.sLastVertex  = m_sVBufferNum;
	stCmd.sStartIndex  = sIdxCnt;
	stCmd.sLastIndex   = m_sIBufferNum;
	addCommand( &stCmd );
}


INLINE void CRender::spriteNormal( StOrder* pOdr )
{
	//スプライト

	Sint32 tpg = pOdr->pg;	//テクスチャページ

	StCustomVertex *pVtx0 = &m_pCutomVertex[ m_sVBufferNum ];
	StCustomVertex *pVtx1 = pVtx0;

	Sint32 sVtxCnt = m_sVBufferNum;
	Sint32 sIdxCnt = m_sIBufferNum;

	for(int ii=0;ii<4;ii++)
	{
		gxPoint Pt;

		//中心位置を原点にずらす
		Pt.x = pOdr->x1[ii] - pOdr->x;
		Pt.y = pOdr->y1[ii] - pOdr->y;

		Pt.x -= pOdr->cx;
		Pt.y -= pOdr->cy;

		//元に戻す
		pOdr->x1[ii] = Pt.x + pOdr->x;
		pOdr->y1[ii] = Pt.y + pOdr->y;
	}

	pVtx1 = pVtx0;

	//頂点設定
	setVertex( pVtx1, pOdr );

	pVtx1 = pVtx0;

	//テクスチャ設定
	setTextureUV(tpg , pVtx1, pOdr );

	stCmd.sType        = eCmdRenderSquare;
	stCmd.sStartVertex = sVtxCnt;
	stCmd.sLastVertex  = m_sVBufferNum;
	stCmd.sStartIndex  = sIdxCnt;
	stCmd.sLastIndex   = m_sIBufferNum;
	addCommand( &stCmd );

}


INLINE void CRender::drawFont( StOrder* pOdr )
{
	//フォントの表示

	stCmd.sType = eCmdRenderFont;
	stCmd.x     = pOdr->x;
	stCmd.y     = pOdr->y;
	stCmd.arg1  = (Uint32)((pOdr->color[0]>>24)&0x000000ff);
	stCmd.arg2  = (Uint32)((pOdr->color[0]>>16)&0x000000ff);
	stCmd.arg3  = (Uint32)((pOdr->color[0]>>8)&0x000000ff);
	stCmd.arg4  = (Uint32)((pOdr->color[0]>>0)&0x000000ff);
	stCmd.pString = (void*)pOdr->pString;

	addCommand( &stCmd );

/*
	addCommand( 
		eCmdRenderFont ,
		(Uint32)((pOdr->color[0]>>24)&0x000000ff),
		(Uint32)((pOdr->color[0]>>16)&0x000000ff),
		(Uint32)((pOdr->color[0]>>8)&0x000000ff),
		(Uint32)((pOdr->color[0]>>0)&0x000000ff),
		(Sint16)pOdr->x,
		(Sint16)pOdr->y,
		(void*)pOdr->pString );
*/

}

/*
INLINE void CRender::addCommand(Uint32 sType , Uint16 a1,Uint16 a2,Uint16 a3,Uint16 a4, Sint16 x , Sint16 y , void* pString )
{
	if( m_bPolygonReq )
	{
		//ポリゴンのリクエストがたまっていればここで生成する
		m_bPolygonReq = gxFalse;
		m_pCommand[m_sCommandNum].eCommand = eCmdRenderSquare;
		m_pCommand[m_sCommandNum].arg[0]   = m_sStartVertex;
		m_pCommand[m_sCommandNum].arg[1]   = (m_sLastVertex-m_sStartVertex);
		m_pCommand[m_sCommandNum].arg[2]   = m_sStartIndex;
		m_pCommand[m_sCommandNum].arg[3]   = (m_sLastIndex-m_sStartIndex);
		m_sStartVertex = INVALID_VERTEX;
		m_sStartIndex  = 0;
		m_sCommandNum ++;
	}
	if( sType == INVALID_TYPE ) return;

	m_pCommand[m_sCommandNum].eCommand = sType;
	m_pCommand[m_sCommandNum].arg[0]   = a1;
	m_pCommand[m_sCommandNum].arg[1]   = a2;
	m_pCommand[m_sCommandNum].arg[2]   = a3;
	m_pCommand[m_sCommandNum].arg[3]   = a4;
	m_pCommand[m_sCommandNum].x        = x;
	m_pCommand[m_sCommandNum].y        = y;
	m_pCommand[m_sCommandNum].pString  = pString;

	m_sLastOdrType = sType;

	m_sCommandNum ++;

}
*/


INLINE void CRender::addCommand( StCmdMake *pCmd )
{


	if( m_sLastOdrType != pCmd->sType )
	{
/*
		m_sIndexCnt = m_sIBufferNum;

		switch( pCmd->sType ){
		case eCmdRenderPoint:
			m_sCommandNum ++;
			break;
		}
*/
	}
	switch( pCmd->sType ){

	//テクスチャ

	case eCmdChgTexture:
		m_pCommand[m_sCommandNum].eCommand = pCmd->sType;
		m_pCommand[m_sCommandNum].arg[0]   = pCmd->arg1;
		m_sCommandNum ++;
		break;

	case eCmdNoneTexture:
		m_pCommand[m_sCommandNum].eCommand = pCmd->sType;
		m_pCommand[m_sCommandNum].arg[0]   = NONE_TEXTURE;
		m_sCommandNum ++;
		break;

	//アトリビュート

	case eCmdChgAttributeAlphaAdd:
	case eCmdChgAttributeAlphaSub:
	case eCmdChgAttributeAlphaCrs:
	case eCmdChgAttributeAlphaRvs:
	case eCmdChgAttributeAlphaNml:
	case eCmdChgAttributeAlphaXor:
	case eCmdChgAttributeAlphaScr:
		m_pCommand[m_sCommandNum].eCommand = pCmd->sType;
		m_sCommandNum ++;
		break;

	//図形

	case eCmdRenderPoint:
		if( m_sLastOdrType != pCmd->sType )
		{
			m_pCommand[m_sCommandNum].eCommand = pCmd->sType;
			m_pCommand[m_sCommandNum].arg[0]   = pCmd->sStartVertex;
			m_pCommand[m_sCommandNum].arg[1]   = (pCmd->sLastVertex-pCmd->sStartVertex);
			m_pCommand[m_sCommandNum].arg[2]   = pCmd->sStartIndex;
			m_pCommand[m_sCommandNum].arg[3]   = (pCmd->sLastIndex-pCmd->sStartIndex);
			m_sCommandNum ++;
		}
		else
		{
			m_pCommand[m_sCommandNum-1].arg[1]   += (pCmd->sLastVertex-pCmd->sStartVertex);
			m_pCommand[m_sCommandNum-1].arg[3]   += (pCmd->sLastIndex-pCmd->sStartIndex);
		}
		break;

	case eCmdRenderLineNormal:
		if( m_sLastOdrType != pCmd->sType )
		{
			m_pCommand[m_sCommandNum].eCommand = pCmd->sType;
			m_pCommand[m_sCommandNum].arg[0]   = pCmd->sStartVertex;
			m_pCommand[m_sCommandNum].arg[1]   = (pCmd->sLastVertex-pCmd->sStartVertex);
			m_pCommand[m_sCommandNum].arg[2]   = pCmd->sStartIndex;
			m_pCommand[m_sCommandNum].arg[3]   = (pCmd->sLastIndex-pCmd->sStartIndex);
			m_pCommand[m_sCommandNum].opt	   = pCmd->arg1/100.0f;
			m_sCommandNum ++;
		}
		else
		{
			m_pCommand[m_sCommandNum-1].arg[1]   += (pCmd->sLastVertex-pCmd->sStartVertex);
			m_pCommand[m_sCommandNum-1].arg[3]   += (pCmd->sLastIndex-pCmd->sStartIndex);
		}
		break;
	case eCmdRenderLineStrip:
		m_pCommand[m_sCommandNum].eCommand = pCmd->sType;
		m_pCommand[m_sCommandNum].arg[0]   = pCmd->sStartVertex;
		m_pCommand[m_sCommandNum].arg[1]   = (pCmd->sLastVertex-pCmd->sStartVertex);
		m_pCommand[m_sCommandNum].arg[2]   = pCmd->sStartIndex;
		m_pCommand[m_sCommandNum].arg[3]   = (pCmd->sLastIndex-pCmd->sStartIndex);
		m_pCommand[m_sCommandNum].opt	   = pCmd->arg1/100.0f;
		m_sCommandNum ++;
		break;

	case eCmdRenderTriangle:
		m_pCommand[m_sCommandNum].eCommand = pCmd->sType;
		m_pCommand[m_sCommandNum].arg[0]   = pCmd->sStartVertex;
		m_pCommand[m_sCommandNum].arg[1]   = (pCmd->sLastVertex-pCmd->sStartVertex);
		m_pCommand[m_sCommandNum].arg[2]   = pCmd->sStartIndex;
		m_pCommand[m_sCommandNum].arg[3]   = (pCmd->sLastIndex-pCmd->sStartIndex);
		m_sCommandNum ++;
		break;

	case eCmdRenderSquare:
		if( m_sLastOdrType != pCmd->sType )
		{
			m_pCommand[m_sCommandNum].eCommand = pCmd->sType;
			m_pCommand[m_sCommandNum].arg[0]   = pCmd->sStartVertex;
			m_pCommand[m_sCommandNum].arg[1]   = (pCmd->sLastVertex-pCmd->sStartVertex);
			m_pCommand[m_sCommandNum].arg[2]   = pCmd->sStartIndex;
			m_pCommand[m_sCommandNum].arg[3]   = (pCmd->sLastIndex-pCmd->sStartIndex);
			m_sCommandNum ++;
		}
		else
		{
			m_pCommand[m_sCommandNum-1].arg[1]   += (pCmd->sLastVertex-pCmd->sStartVertex);
			m_pCommand[m_sCommandNum-1].arg[3]   += (pCmd->sLastIndex-pCmd->sStartIndex);
		}
		break;

	case eCmdRenderFont:
		m_pCommand[m_sCommandNum].eCommand = pCmd->sType;
		m_pCommand[m_sCommandNum].arg[0]   = pCmd->arg1;
		m_pCommand[m_sCommandNum].arg[1]   = pCmd->arg2;
		m_pCommand[m_sCommandNum].arg[2]   = pCmd->arg3;
		m_pCommand[m_sCommandNum].arg[3]   = pCmd->arg4;
		m_pCommand[m_sCommandNum].x        = pCmd->x;
		m_pCommand[m_sCommandNum].y        = pCmd->y;
		m_pCommand[m_sCommandNum].pString  = pCmd->pString;
		m_sCommandNum ++;
		break;
	}

	m_sLastOdrType = pCmd->sType;

}
