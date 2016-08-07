//---------------------------------------------------
//コクピット画面
//---------------------------------------------------
#include <gunvalken.h>
#include "../../enemy/enemy.h"
#include "CStageClear.h"
#include "CMsgWindow.h"
#include "../../GunHound/GhStringTbl.h"

gxSprite StSprStageClear[]={
	{enTexPageString,0,128,488,64,240,32},
};

CStageClear::CStageClear()
{
	m_sScrollw = 100;

	m_sSeq = 0;
	m_bEnd = gxFalse;

}


CStageClear::~CStageClear()
{
	
	
}


void CStageClear::Action()
{
	//----------------------------------------
	//ステージクリア演出
	//----------------------------------------

	Sint32 sAdd = (0-m_sScrollw)/5;

	if( sAdd == 0 ) sAdd = -1;
	if( m_sScrollw > 0) m_sScrollw += sAdd;

	switch(m_sSeq){
	case 0:		//
		m_sSeq = 10;
		viiMus::StopBGM( enSoundBgm1 );
		viiMus::StopBGM( enSoundBgm2 );
		viiMus::PlaySound( enSoundStageClear );
		m_sScrollw = 100;
		pGame->set_the_world();
		break;

	case 10:	//
		if( !CCockpit::GetInstance()->IsMessageNow() )
		{
			m_sSeq = 100;
		}
		break;

	case 100:	//得点計算
		pGame->set_the_world();
//		if( Joy[0].trg&enBtnDecision )
		{
			m_sSeq = 999;
		}
		break;

	case 900:
		pGame->cancel_the_world();
		m_sSeq = 999;
		break;

	case 999:
		pGame->cancel_the_world();
		m_bEnd = gxTrue;
		break;
	}

}


void CStageClear::Draw()
{
	//------------------------------------------
	//描画
	//------------------------------------------
	Sint32 mx = 0,my = WINDOW_H/2+32;

	viiDraw::Box( 0, 0, WINDOW_W ,WINDOW_H , PRIO_BLACKTONE , gxTrue ,0x80202020 , ATR_ALPHA_MINUS);

	//格子の表示
	for(Sint32 y=0;y<WINDOW_H;y+=32)
	{
		for(Sint32 x=0;x<WINDOW_W;x+=32)
		{
			viiDraw::Box( x, 0, x+2 ,WINDOW_H , PRIO_BLACKTONE , gxTrue, 0x10108030  , ATR_ALPHA_PLUS );
			viiDraw::Box( 0, y, WINDOW_W ,y+2 , PRIO_BLACKTONE , gxTrue, 0x10108030  , ATR_ALPHA_PLUS );
		}
	}

	viiSub::MenuSprite( &StSprStageClear[0]  , WINDOW_W/2,my-54 , PRIO_BLACKTONE ,ATR_DFLT,0xf0ffffff);

	viiDraw::big_printf2( 62-WINDOW_W*m_sScrollw/100   ,my-64  , PRIO_BLACKTONE+1, 0xffffffff , StrMissionClear[0] );

}


