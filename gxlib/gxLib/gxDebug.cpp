// -----------------------------------------------------------
//
// デバッグ用ルーチン色々
// （処理棒など）
// -----------------------------------------------------------

#include <gxLib.h>
#include "gx.h"
#include "gxOrderManager.h"
#include "gxTexManager.h"
#include "gxRender.h"
#include "gxPadManager.h"
#include "gxSoundManager.h"
#include "gxDebug.h"

SINGLETON_DECLARE_INSTANCE( gxDebug );

#ifdef GX_DEBUG
Sint32 gxDebug::m_DebugSwitch[8]={1,1,1,1,1,1,1,1};
#else
Sint32 gxDebug::m_DebugSwitch[8]={0,0,0,0,0,0,0,0};
#endif

gxBool gxDebug::m_bMasterDebugSwitch = gxFalse;

gxDebug::gxDebug()
{
	for(Sint32 ii=0;ii<8;ii++)
	{
		m_uTime[ii]  = 0;
		m_uOldTime[ii] = 0;
		m_fWorks[ii] = 0.f;
	}

	m_stInfo.drawCnt = 0;
	m_stInfo.start = -1;

}


gxDebug::~gxDebug()
{

}


void gxDebug::CheckPoint(Sint32 n)
{

 	m_uTime[ n ] = gxLib::GetTime();
	if( m_stInfo.start == -1 )
	{
		m_stInfo.start = m_uTime[ n ];
	}

}


void gxDebug::CheckEnd()
{

/*
	m_fWorks[0] = 1.0f*(cl[1].LowPart - cl[0].LowPart) / (freq.LowPart / 60);	//Action
	m_fWorks[1] = 1.0f*(cl[2].LowPart - cl[0].LowPart) / (freq.LowPart / 60);	//Action
	m_fWorks[2] = 1.0f*(cl[3].LowPart - cl[0].LowPart) / (freq.LowPart / 60);	//Action
	m_fWorks[3] = 1.0f*(cl[4].LowPart - cl[0].LowPart) / (freq.LowPart / 60);	//Action
	m_fWorks[4] = 1.0f*(cl[5].LowPart - cl[0].LowPart) / (freq.LowPart / 60);	//Action
*/

	m_fWorks[0] = ( m_uTime[1] - m_uTime[0] );// / (freq / 60);	//Movie
	m_fWorks[1] = ( m_uTime[2] - m_uTime[0] );// / (freq / 60);	//Movie
	m_fWorks[2] = ( m_uTime[3] - m_uTime[0] );// / (freq/60);	//Draw
	m_fWorks[3] = ( m_uTime[4] - m_uTime[0] );// / (freq/60);	//Sound
	m_fWorks[4] = ( m_uTime[5] - m_uTime[0] );// / (freq/60);	//vSync

	m_stInfo.OrderNum    = COrderManager::GetInstance()->GetOrderNum();
	m_stInfo.SubOrderNum = COrderManager::GetInstance()->GetSubOrderNum();
	m_stInfo.vtx_max     = CRender::GetInstance()->GetVertexNum();
	m_stInfo.idx_max     = CRender::GetInstance()->GetIndexNum();


	Uint32 now = gxLib::GetTime();

	m_stInfo.drawCnt ++;

	if( now >= m_stInfo.start + 1000 )
	{
		m_stInfo.fps     = m_stInfo.drawCnt;
		m_stInfo.start   = now;
		m_stInfo.drawCnt = 0;
	}


}


void gxDebug::Draw()
{
#ifdef GX_MASTER
	return;
#endif

	if( !m_bMasterDebugSwitch ) return;

	Sint32 ax,ay,az;

	ax = 0;
	ay = 0;
	az = PRIORITY_MAX;

	gxLib::DrawBox( ax , ay , ax + 100 , ay + 100 , az , gxTrue , ATR_ALPHA_MINUS , 0x80ffffff );
	gxLib::Printf( ax+8 , ay + 12*0+8 , az , ATR_DFLT , 0xffffffff , "fps %.2f" , m_stInfo.fps );
	gxLib::Printf( ax+8 , ay + 12*1+8 , az , ATR_DFLT , 0xffffffff , "Order %d" , m_stInfo.OrderNum + m_stInfo.SubOrderNum -6  );
	gxLib::Printf( ax+8 , ay + 12*2+8 , az , ATR_DFLT , 0xffffffff , "Sub   %d (%d%%)" , m_stInfo.SubOrderNum  , 100*m_stInfo.SubOrderNum / (m_stInfo.OrderNum + m_stInfo.SubOrderNum -6) );
	gxLib::Printf( ax+8 , ay + 12*3+8 , az , ATR_DFLT , 0xffffffff , "vtx   %d" , m_stInfo.vtx_max );
	gxLib::Printf( ax+8 , ay + 12*4+8 , az , ATR_DFLT , 0xffffffff , "idx   %d" , m_stInfo.idx_max );

	Sint32 num = 0;
	for( Sint32 ii=0; ii<MAX_SOUND_NUM; ii++ )
	{
		if( CSoundManager::GetInstance()->GetPlayInfo( ii )->bPlayNow )
		{
			gxLib::Printf( ax+8 , ay + 12*(5+num)+8 , az , ATR_DFLT , 0xffffffff , "snd(%03d)   %04x/%d/%.2f" ,
				ii,
				CSoundManager::GetInstance()->GetPlayInfo( ii )->uStatus,
				CSoundManager::GetInstance()->GetPlayInfo( ii )->bPlayNow,
				CSoundManager::GetInstance()->GetPlayInfo( ii )->fVolume );
				num ++;
		}
	}
}


void gxDebug::drawBar( Sint32 n , Float32 ratio )
{
	Sint32 ax,ay,max = 128;

	ax = 0;
	ay = 0;

	Sint32 w = 1;
	w = max * ratio;

	Uint32 argbTbl[]={
		0xff0000ff,
		0xff00ff00,
		0xff00ffff,
		0xffff0000,
		0xffff00ff,
		0xffffff00,
		0xffffffff,
	};

	gxLib::DrawBox( ax,ay, ax+w ,ay+8, PRIORITY_MAX, gxTrue , ATR_DFLT , argbTbl[n] );

//#ifdef DEBUG_ON
//	gxLib::DrawBox( 0,0, WINDOW_W*m_fWorks[3]/100.f ,8, PRIORITY_MAX, gxTrue , ATR_DFLT , 0xf000f0f0 );
//	gxLib::DrawBox( 0,0, WINDOW_W*m_fWorks[2]/100.f ,8, PRIORITY_MAX, gxTrue , ATR_DFLT , 0xf000f0c0 );
//	gxLib::DrawBox( 0,0,  WINDOW_W*m_fWorks[1]/100.f,8, PRIORITY_MAX, gxTrue , ATR_DFLT , 0xf0808000 );
//	gxLib::DrawBox( 0,0,  WINDOW_W*m_fWorks[0]/100.f,8, PRIORITY_MAX, gxTrue , ATR_DFLT , 0xf0f00000 );	//赤げーじ
//
//	Uint32 uUse , uMax , uTotal;
//
//	gxLib::GetMemoryRemain( &uUse , &uTotal, &uMax );
//
////	gxLib::DrawBox( 0,8 , 128,12, PRIORITY_MAX, gxTrue , ATR_DFLT , 0xA0010101 );
//	gxLib::Printf ( 0,0 , PRIORITY_MAX ,ATR_DFLT ,ARGB_DFLT , "%dMB / %dMB (%dMB) %.2f%%" , uUse/1024 , uTotal/1024 , uMax/1024 , 100.f*uUse/uTotal);
//
//#endif
}

