//---------------------------------------------------------
//
// 蛇腹アニメ
//
//---------------------------------------------------------
#include <gunvalken.h>
#include "CPenduram.h"
#include "CBellowsSprite.h"

CBellowsSprite::CBellowsSprite()
{
	//------------------------------
	//初期設定
	//------------------------------

	for(Sint32 ii=0;ii<enBellowMax;ii++)
	{
		m_PosSrc[ii].x = 0;
		m_PosSrc[ii].y = 0;
		m_PosSrc[ii].z = 0;

		m_PosMid[ii] = m_PosSrc[ii];

		m_PosDst[ii].x = ZERO;
		m_PosDst[ii].y = ZERO;
		m_PosDst[ii].z = ZERO;
		m_PosDst[ii].col = ARGB_DFLT;
	}

	m_Pos.x = 0;
	m_Pos.y = 0;
	m_Pos.z = 0;

	uColor[0] = ARGB_DFLT;
	uColor[1] = ARGB_DFLT;

	m_fMaxLength[0] = 1.f;
	m_fMaxLength[1] = 1.f;

	m_fLength = 100.f;

	m_sMax = enBellowMax;
	m_sMax = 0;
}


CBellowsSprite::~CBellowsSprite()
{
	
	
}


void CBellowsSprite::SetColor( Uint32 uSrc , Uint32 uDst )
{
	//------------------------------
	//グラデーションカラーを設定
	//------------------------------

	uColor[0] = uSrc;
	uColor[1] = uDst;
}


void CBellowsSprite::AddPosition(Sint32 x1, Sint32 y1 ,Sint32 x2, Sint32 y2 )
{
	//------------------------------
	//蛇腹を追加
	//------------------------------

	//外側
	m_PosSrc[m_sMax*2+0].x = x1;
	m_PosSrc[m_sMax*2+0].y = y1;

	//内側
	m_PosSrc[m_sMax*2+1].x = x2;
	m_PosSrc[m_sMax*2+1].y = y2;

	m_sMax ++;

	m_fMaxLength[0] = ZERO;
	m_fMaxLength[1] = ZERO;

	//距離を測る

	for(Sint32 ii=1;ii<m_sMax;ii++)
	{
		Sint32 ax,ay;
		Float32 fDist;

		m_PosMid[ii-0].x = (m_PosSrc[ii*2+0].x+m_PosSrc[ii*2+1].x)/2.f;
		m_PosMid[ii-0].y = (m_PosSrc[ii*2+0].y+m_PosSrc[ii*2+1].y)/2.f;

		m_PosMid[ii-1].x = (m_PosSrc[(ii-1)*2+0].x + m_PosSrc[(ii-1)*2+1].x)/2.f;
		m_PosMid[ii-1].y = (m_PosSrc[(ii-1)*2+0].y + m_PosSrc[(ii-1)*2+1].y)/2.f;

		ax = m_PosMid[ii-0].x - m_PosMid[ii-1].x;
		ay = m_PosMid[ii-0].y - m_PosMid[ii-1].y;
		fDist = viiMath::Dist(ax,ay);

		m_PosDst[(ii-1)*4+0].z = fDist;
		m_fMaxLength[0]   += fDist;

		//グラフィックデータポジションを確定する

		m_PosDst[(ii-1)*4+0].x = m_PosSrc[(ii-1)*2+0].x;
		m_PosDst[(ii-1)*4+0].y = m_PosSrc[(ii-1)*2+0].y;
//		m_PosDst[(ii-1)*4+0].z = 0;
		m_PosDst[(ii-1)*4+0].col = 0;

		m_PosDst[(ii-1)*4+1].x = m_PosSrc[(ii-1)*2+1].x;
		m_PosDst[(ii-1)*4+1].y = m_PosSrc[(ii-1)*2+1].y;
//		m_PosDst[(ii-1)*4+1].z = 0;
		m_PosDst[(ii-1)*4+1].col = 0;

		m_PosDst[(ii-1)*4+2].x = m_PosSrc[(ii-0)*2+0].x;
		m_PosDst[(ii-1)*4+2].y = m_PosSrc[(ii-0)*2+0].y;
//		m_PosDst[(ii-1)*4+2].z = 0;
		m_PosDst[(ii-1)*4+2].col = 0;

		m_PosDst[(ii-1)*4+3].x = m_PosSrc[(ii-0)*2+1].x;
		m_PosDst[(ii-1)*4+3].y = m_PosSrc[(ii-0)*2+1].y;
//		m_PosDst[(ii-1)*4+3].z = 0;
		m_PosDst[(ii-1)*4+3].col = 0;
	}

}


void CBellowsSprite::SetLength( Float32 fLength )
{
	//------------------------------
	//長さを設定（０～１００％）
	//------------------------------

	if( fLength <   0.f ) fLength = ZERO;
	if( fLength > 100.f ) fLength = 100.f;

	m_fLength = fLength/100.f;
}


void CBellowsSprite::Action()
{
	//------------------------------
	//毎フレームのアクション
	//------------------------------

	return;

	static Sint32 sCursor = 0;

	if(Joy[0].trg&BTN_L1) sCursor --;
	if(Joy[0].trg&BTN_R1) sCursor ++;
	sCursor = (m_sMax+sCursor)%m_sMax;

	for(Sint32 ii=0;ii<m_sMax; ii++)
	{
		Sint32 sCol = ARGB_DFLT;
		if(sCursor == ii)
		{
			Sint32 n = -1;
			sCol = 0xffff0000;
			if(Joy[0].psh&BTN_X) n = 1;;
			if(Joy[0].psh&BTN_Y) n = 0;;
			if( n != -1 )
			{
				if(Joy[0].trg&JOY_U) m_PosDst[(ii)*4+n].y-=1.f;
				if(Joy[0].trg&JOY_D) m_PosDst[(ii)*4+n].y+=1.f; 
				if(Joy[0].trg&JOY_L) m_PosDst[(ii)*4+n].x-=1.f;
				if(Joy[0].trg&JOY_R) m_PosDst[(ii)*4+n].x+=1.f; 
			}
		}


		GGX_DrawPixel(	m_PosDst[(ii)*4+0].x+m_Pos.x,	m_PosDst[(ii)*4+0].y+m_Pos.y,	280 ,sCol);
		GGX_DrawPixel(	m_PosDst[(ii)*4+1].x+m_Pos.x,	m_PosDst[(ii)*4+1].y+m_Pos.y,	280 ,sCol );
		GGX_DrawPixel(	m_PosDst[(ii)*4+2].x+m_Pos.x,	m_PosDst[(ii)*4+2].y+m_Pos.y,	280 ,sCol );
		GGX_DrawPixel(	m_PosDst[(ii)*4+3].x+m_Pos.x,	m_PosDst[(ii)*4+3].y+m_Pos.y,	280 ,sCol );

		GGX_DrawPixel(	m_PosMid[ii].x+m_Pos.x,	m_PosMid[ii].y+m_Pos.y,	0 );
	}


}


void CBellowsSprite::Draw()
{
	//------------------------------
	//描画
	//------------------------------

	GRA_POS_T gPos[6];
	Float32 fTotalDist = ZERO;
	Float32 fAllDist   = m_fMaxLength[0];
	Float32 fNowDist   = ZERO;

	gxBool bChudan  = gxFalse;
	Sint32 sChudanIndex = 0;

	for(Sint32 ii=0;ii<m_sMax; ii++)
	{
		fNowDist    = m_PosDst[ii*4+0].z;

		sChudanIndex = ii;

		if( (fTotalDist+fNowDist) > fAllDist*m_fLength )
		{
			//途中で途切れた
			bChudan      = gxTrue;
			break;
		}

		fTotalDist += fNowDist;

		gPos[0].x = m_PosDst[(ii)*4+0].x;	gPos[0].y = m_PosDst[(ii)*4+0].y;
		gPos[1].x = m_PosDst[(ii)*4+1].x;	gPos[1].y = m_PosDst[(ii)*4+1].y;
		gPos[3].x = m_PosDst[(ii)*4+2].x;	gPos[3].y = m_PosDst[(ii)*4+2].y;
		gPos[2].x = m_PosDst[(ii)*4+3].x;	gPos[2].y = m_PosDst[(ii)*4+3].y;

		gPos[0].z = 0;
		gPos[1].z = 0;
		gPos[2].z = 0;
		gPos[3].z = 0;

		gPos[0].col = uColor[0];
		gPos[1].col = uColor[0];
		gPos[2].col = uColor[0];
		gPos[3].col = uColor[0];

		gPos[4].x = (float)240;
		gPos[4].y = (float)128;
		gPos[4].z = (float)13;

		gPos[5].x = (float)8;
		gPos[5].y = (float)8;
		gPos[5].z = (float)0;

		PutGraphics( GRA_SPRITE, m_Pos.x,	m_Pos.y,	m_Pos.z,  &gPos[0] );
	}

	if( bChudan )
	{
		//最後の１ポリゴンを描画する
		Sint32 ii = sChudanIndex;

		fNowDist    = m_PosDst[ii*4+0].z;					// 次の長さは？ｃｍ
		Float32 fShortLength = fAllDist*m_fLength;			// 今回の全長は？ｃｍ
		Float32 fRestLength  = fShortLength-fTotalDist;;	// 残りの長さはｃｍ？
		Float32 fRestAvg     = fRestLength/fNowDist;		// 残りの長さのうち？％が必要か？

		gPos[0].x = m_PosDst[(ii)*4+0].x;
		gPos[0].y = m_PosDst[(ii)*4+0].y;

		gPos[1].x = m_PosDst[(ii)*4+1].x;
		gPos[1].y = m_PosDst[(ii)*4+1].y;

		gPos[3].x = gPos[0].x+( m_PosDst[(ii)*4+2].x-gPos[0].x )*fRestAvg;
		gPos[3].y = gPos[0].y+( m_PosDst[(ii)*4+2].y-gPos[0].y )*fRestAvg;

		gPos[2].x = gPos[1].x+( m_PosDst[(ii)*4+3].x - gPos[1].x )*fRestAvg ;
		gPos[2].y = gPos[1].y+( m_PosDst[(ii)*4+3].y - gPos[1].y )*fRestAvg;

		gPos[0].z = 0;
		gPos[1].z = 0;
		gPos[2].z = 0;
		gPos[3].z = 0;

		gPos[0].col = uColor[0];
		gPos[1].col = uColor[0];
		gPos[2].col = uColor[0];
		gPos[3].col = uColor[0];

		gPos[4].x = (float)240;
		gPos[4].y = (float)128;
		gPos[4].z = (float)13;

		gPos[5].x = (float)8;
		gPos[5].y = (float)8;
		gPos[5].z = (float)0;

		PutGraphics( GRA_SPRITE, m_Pos.x,	m_Pos.y,	m_Pos.z,  &gPos[0] );

	}

}


void CBellowsTest()
{
	Sint32 m_gaugeLength = 100.f;
	CBellowsSprite *g_pBellowSprite = NULL;

	ghPos sTbl[]={
		{59,151}, {51,151},
		{58,144}, {51,145},
		{56,137}, {49,140},
		{53,133}, {47,137},
		{50,129}, {45,135},
		{46,126}, {43,133},
		{42,124}, {40,131},
		{38,123}, {37,130},
		{34,122}, {34,129},
		{31,122}, {31,129},
	};

	if( g_pBellowSprite == NULL )
	{
		m_gaugeLength = 100.f;

		g_pBellowSprite = new CBellowsSprite();
		g_pBellowSprite->m_Pos.x = 60;
		g_pBellowSprite->m_Pos.y = 120;

		for(Sint32 ii=0;ii<ARRAY_LENGTH(sTbl)/2;ii++)
		{
			g_pBellowSprite->AddPosition( sTbl[ii*2+0].x , sTbl[ii*2+0].y ,sTbl[ii*2+1].x , sTbl[ii*2+1].y );
		}
	}

	if( m_gaugeLength < 100.f && Joy[0].psh&JOY_U)
	{
		m_gaugeLength += 1.f;
		g_pBellowSprite->SetLength( m_gaugeLength );
	}
	else if( m_gaugeLength > 0.f && Joy[0].psh&JOY_D)
	{
		m_gaugeLength -= 1.f;
		g_pBellowSprite->SetLength( m_gaugeLength );
	}

	g_pBellowSprite->Action();

	g_pBellowSprite->Draw();

}

