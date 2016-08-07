//--------------------------------------------
//
//  オープニングデモ制御用
//
//--------------------------------------------

#include <gunvalken.h>
#include "COpeningDemo.h"

enum {
	enSeqOpeningNone,
	enSeqLoadingFinish,
	enSeqWaitGame,
	enSeqFadeFinish,

	enAdvertiseWaitMax = (60*120)/2,
};

COpeningDemo::COpeningDemo()
{
	m_sTimer = 0;
	m_bEnd = gxFalse;

	m_sSequence       = 0;
	m_sOperationIndex = enOperationNone;

	m_sAlpha = 0x00;
}



COpeningDemo::~COpeningDemo()
{
	
}


void COpeningDemo::Action()
{

	switch(m_sSequence){
	case 0:
		m_sSequence = 100;
		m_sOperationIndex = enOperationInit;
		break;

	case 100:
		m_sOperationIndex = enOperationMain;
		m_sTimer ++;
		if( m_sTimer >= enAdvertiseWaitMax || Joy[0].psh )
		{
			m_sSequence = 200;
		}
		break;

	case 200:
		m_sAlpha += 3;
		if( m_sAlpha > 255 )
		{
			m_sAlpha    = 255;
			m_sSequence = 300;
		}
		break;

	case 300:
		m_sOperationIndex = enOperationEnd;
		m_bEnd = gxTrue;
		break;
	}


}

void COpeningDemo::SetStatusGameOver()
{
	if( m_sSequence == 100 )
	{
		m_sSequence = 200;
	}

}

void COpeningDemo::Draw()
{
/*
Sint32 sBairitsu = m_sTimer;
	Float32 m_fScale;
	if( sBairitsu < 0) 
	{
		sBairitsu = 0;
	}

	if( sBairitsu > 300) 
	{
		sBairitsu = 300;
	}

	Sint32 sCol = 0xFF*sBairitsu/300;

	m_fScale = 1.f + 6.f*(300-sBairitsu)/300;

	viiSub::MenuSprite( WINDOW_W/2,WINDOW_H/2,252, TPG_ANM+8,0,0,WINDOW_W,WINDOW_H,WINDOW_W/2,WINDOW_H/2,ATR_DFLT,ARGB(sCol,0xff,0xff,0xff),0,m_fScale,m_fScale );
*/
//	viiSub::MenuSprite( WINDOW_W/2,WINDOW_H/2,PRIO_FADEOVER-1, enTexPageSysLogo,272,176,208,80,104,40,ATR_DFLT,ARGB(0xf0,0xff,0xff,0xff) );
	viiSub::MenuSprite( 380,WINDOW_H-48/*224*/,PRIO_FADEOVER, enTexPageSysLogo,272,176,208,80,104,40,ATR_DFLT,ARGB(0xf0,0xff,0xff,0xff) );
	viiDraw::Box( 0, 0, WINDOW_W , WINDOW_H , PRIO_FADEOVER+1  , gxTrue , ARGB(m_sAlpha,0x01,0x01,0x01) );

}



