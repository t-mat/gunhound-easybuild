//--------------------------------------------
//オープニングデモ制御用
//--------------------------------------------

#include <gunvalken.h>
#include <gunhound/DashBoard/CDashBoard.h>
#include "CContinue.h"
#include "../../GunHound/GhStringTbl.h"
gxSprite SprTilePattern[]={
//	{enTexPageMainMenu,0,0,64,64,0,0},
	{enTexPageSystemUI,416,48,16,16,8,8},
};
extern gxSprite SprTitleScreen[];

static char sTblOne[][64]={
	{
		0,0,1,1,1,1,0,0,
		0,1,1,0,0,1,1,0,
		0,1,1,0,0,1,1,0,
		0,1,1,0,0,1,1,0,
		0,1,1,0,0,1,1,0,
		0,1,1,0,0,1,1,0,
		0,1,1,0,0,1,1,0,
		0,0,1,1,1,1,0,0,
	},
	{
		0,0,0,1,1,0,0,0,
		0,0,1,1,1,0,0,0,
		0,0,0,1,1,0,0,0,
		0,0,0,1,1,0,0,0,
		0,0,0,1,1,0,0,0,
		0,0,0,1,1,0,0,0,
		0,0,0,1,1,0,0,0,
		0,0,0,1,1,0,0,0,
	},
	{
		0,0,1,1,1,1,0,0,
		0,1,1,0,0,1,1,0,
		0,1,1,0,0,1,1,0,
		0,0,0,0,0,1,1,0,
		0,0,1,1,1,1,0,0,
		0,1,1,0,0,0,0,0,
		0,1,1,0,0,0,0,0,
		0,1,1,1,1,1,1,0,
	},
	{
		0,0,1,1,1,1,0,0,
		0,1,1,0,0,1,1,0,
		0,0,0,0,0,1,1,0,
		0,0,0,1,1,1,0,0,
		0,0,0,0,0,1,1,0,
		0,1,1,0,0,1,1,0,
		0,1,1,0,0,1,1,0,
		0,0,1,1,1,1,0,0,
	},
	{
		0,0,0,0,1,1,0,0,
		0,0,0,1,1,1,0,0,
		0,0,1,0,1,1,0,0,
		0,1,0,0,1,1,0,0,
		0,1,0,0,1,1,0,0,
		0,1,1,1,1,1,1,0,
		0,0,0,0,1,1,0,0,
		0,0,0,0,1,1,0,0,
	},
	{
		0,1,1,1,1,1,1,0,
		0,1,1,0,0,0,0,0,
		0,1,1,0,0,0,0,0,
		0,1,1,1,1,1,0,0,
		0,0,0,0,0,1,1,0,
		0,0,0,0,0,1,1,0,
		0,1,1,0,0,1,1,0,
		0,0,1,1,1,1,0,0,
	},
	{
		0,0,1,1,1,1,0,0,
		0,1,1,0,0,1,1,0,
		0,1,1,0,0,0,0,0,
		0,1,1,1,1,1,0,0,
		0,1,1,0,0,1,1,0,
		0,1,1,0,0,1,1,0,
		0,1,1,0,0,1,1,0,
		0,0,1,1,1,1,0,0,
	},
	{
		0,1,1,1,1,1,1,0,
		0,1,1,0,0,1,1,0,
		0,0,0,0,0,1,1,0,
		0,0,0,0,1,1,0,0,
		0,0,0,0,1,1,0,0,
		0,0,0,1,1,0,0,0,
		0,0,0,1,1,0,0,0,
		0,0,0,1,1,0,0,0,
	},
	{
		0,0,1,1,1,1,0,0,
		0,1,1,0,0,1,1,0,
		0,1,1,0,0,1,1,0,
		0,0,1,1,1,1,0,0,
		0,1,1,0,0,1,1,0,
		0,1,1,0,0,1,1,0,
		0,1,1,0,0,1,1,0,
		0,0,1,1,1,1,0,0,
	},
	{
		0,0,1,1,1,1,0,0,
		0,1,1,0,0,1,1,0,
		0,1,1,0,0,1,1,0,
		0,1,1,0,0,1,1,0,
		0,0,1,1,1,1,1,0,
		0,0,0,0,0,1,1,0,
		0,1,1,0,0,1,1,0,
		0,0,1,1,1,1,0,0,
	},
};


CContinue::CContinue()
{
	m_sHalfScore = 0;
	m_sContTimer = 0;
	m_sCount     = 9;

	m_sReturnSeq = 0;

	m_sSeq      = 0;
	m_bGameOver = gxFalse;
	m_bEnd      = gxFalse;

	m_sAllScore    = 0;
	m_sStageScore  = 0;

	m_sFlashAlpha = 0xff;

	CFadeManager::GetInstance()->set_fadein( 8,0xffffffff);

}



CContinue::~CContinue()
{
	
}


void CContinue::Action()
{
	//----------------------------------------
	//コンティニューカウント
	//----------------------------------------

	if( m_sContTimer >= 90 || Joy[0].trg&enBtnCancel )
	{
		m_sContTimer = 0;
		if( m_sCount > -1 && m_sSeq <= 10 )
		{
			m_sCount --;
			m_sFlashAlpha = 255;
			if( m_sCount < 9 && !m_bGameOver )
			{
				viiMus::PlaySound( enSoundMenuSel );
			}
		}
	}

	if( m_sCount == -1)
	{
		m_sCount = 0;
		if( !m_bGameOver )
		{
			m_bGameOver = gxTrue;
			viiMus::PlaySound( enSoundGameOver );
		}
	}

	switch( m_sSeq ){
	case 0:
		if( !CFadeManager::GetInstance()->is_fade() )
		{
		CDashBoard::GetInstance()->SetIcon( enIconDecision );
			m_sSeq = 10;
		}
		break;

	case 10:
		if( Joy[0].trg&enBtnDecision )
		{
			if( m_bGameOver )
			{
				//ゲームオーバー
				CFadeManager::GetInstance()->set_fadeout();
				m_sReturnSeq = enMenuSeqNext;
				m_sSeq = 30;
			}
			else
			{
				//コンティニュー
				m_sFlashAlpha = 255;
				viiMus::PlaySound( enSoundMenuDec );
				m_sReturnSeq = enMenuSeqBack;
				m_sSeq = 20;
			}
		}
		break;

	case 20:
		//コンティニューする場合はスコアをゼロに
		CDashBoard::GetInstance()->SetIcon( enIconNone );
		CFadeManager::GetInstance()->set_fadeout();
		m_sAllScore += (m_sHalfScore-m_sAllScore)/10;
		if( gxAbs(m_sHalfScore-m_sAllScore) < 10 )
		{
			m_sAllScore = m_sHalfScore;
			m_sSeq = 30;
		}
		break;

	case 30:
		CDashBoard::GetInstance()->SetIcon( enIconNone );
		if( !CFadeManager::GetInstance()->is_fade() )
		{
			m_bEnd = gxTrue;
			m_sSeq = 999;
		}
		break;

	case 999:
		break;
	}

	if( m_sFlashAlpha > 0 )
	{
		m_sFlashAlpha -= 16;
	}
	else
	{
		m_sFlashAlpha = 0;
	}
	m_sContTimer ++;

}


void CContinue::SetScore( Uint32 uAllScore , Uint32 uStageScore )
{

	m_sAllScore    = (Sint32)uAllScore;
	m_sHalfScore   = 0;//(Sint32)(m_sAllScore/2);
	m_sStageScore  = (Sint32)uStageScore;

	//端数を切り捨てる
	m_sHalfScore -= (m_sHalfScore%10);

}


void CContinue::Draw()
{
	//----------------------------------------
	//コンティニュー描画
	//----------------------------------------

	if( m_bGameOver )
	{
		viiDraw::big_printf(116,WINDOW_H/2-16,0xffff2040,"%s",GameOverString[0] );//GAME OVER");

		return;
	}
	else
	{
		viiDraw::big_printf(136+5,32+5,0xF0101010,"%s",GameOverString[1] );//CONTINUE");
		viiDraw::big_printf(136  ,32  ,0xa0F0F0F0,"%s",GameOverString[1] );//CONTINUE");
		//viiDraw::printf(WINDOW_W/2,WINDOW_H/2+32,0xffff00ff,"%d",m_sCount);
	}

	Sint32 x,y;

	for(Sint32 ii=0;ii<64;ii++)
	{
		if( sTblOne[m_sCount][ii] )
		{
			x = ii%8;
			y = ii/8;

//			viiDraw::printf( -48+WINDOW_W/2+x*12   , -16+WINDOW_H/2+y*12   , 0xff4080ff , "%s",GameOverString[2] );//"C" );
//			viiDraw::printf( -48+WINDOW_W/2+x*12+4 , -16+WINDOW_H/2+y*12+4 , 0x40202020 , "%s",GameOverString[2] );//"C" );
			viiSub::MenuSprite( &SprTilePattern[0] , -48+WINDOW_W/2+x*12   , -16+WINDOW_H/2+y*12   , PRIO_SYSTEM,ATR_DFLT, ARGB(0xff,0xff,0xff,0xff) );
			viiSub::MenuSprite( &SprTilePattern[0] , -48+WINDOW_W/2+x*12+4 , -16+WINDOW_H/2+y*12+4 , PRIO_SYSTEM,ATR_DFLT, 0x40202020 );

			if( m_sFlashAlpha > 0 )
			{
//				viiDraw::printf( -48+WINDOW_W/2+x*12 , -16+WINDOW_H/2+y*12 , ARGB( m_sFlashAlpha , 0xff , 0xff ,0xff ) , "%s",GameOverString[2] );//"C" );
				viiSub::MenuSprite( &SprTilePattern[0] , -48+WINDOW_W/2+x*12+4,-16+WINDOW_H/2+y*12+4  ,PRIO_SYSTEM+1,ATR_ALPHA_PLUS, ARGB( m_sFlashAlpha , 0xff , 0xff ,0xff ) );
			}
		}
	}

	viiDraw::printf( -80+WINDOW_W/2 , 100+WINDOW_H/2+16*1 , 0xFFFFFFFF , "%s %d"       , GameOverString[3] , m_sAllScore );
//	viiDraw::printf( -196+WINDOW_W/2 , 100+WINDOW_H/2+16*1 , 0xFFFFFFff , "STAGE SCORE %d" , m_sStageScore);

	//設定用背景
	Sint32 xx = viiSub::GameCounter()%64;

	for(Sint32 y=-64;y<WINDOW_H+64;y+=64)
	{
		for(Sint32 x=-64;x<WINDOW_W+64;x+=64)
		{
			viiSub::MenuSprite( &SprTitleScreen[8] , x+xx,y+xx,0  ,ATR_DFLT, ARGB(0xff,0xff,0xff,0xff) );
		}
	}


}



