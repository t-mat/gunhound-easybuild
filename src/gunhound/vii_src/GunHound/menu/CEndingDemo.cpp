//--------------------------------------------
//オープニングデモ制御用
//--------------------------------------------

#include <gunvalken.h>
#include "CEndingDemo.h"
#include "../GhStringTbl.h"

char* StrBonusTbl[]={
	"SCORE",				//最終スコア
	"CONTINUE BONUS",		//コンティニュー分割
	"SPECIAL BONUS",		//TRUEエンド
	"NO CONTINUE BONUS",	//ノーコンティニュー
	"ALL OVER BONUS",		//最終ステージクリア
	"TOTAL",
};

enum {
	enEndingSeqStart,
	enEndingSeqBefore,
	enEndingSeqStaffRoll,
	enEndingSeqAfter,
};
enum {
	enStarPrio,
	enKanjiPrio,
	enEndingScreenPrio  = 100,
	enEndingScreenPrio2 = 100,
	enBlacFadePrio      = 110,
};

gxSprite SprStar[]={
	{enTexPageEffectCommon02,64,128,16,16,8,8},
};
gxSprite SprRough[]={
	{ enTexPageBackGround+24+0,0,0,256,256,128,128},
	{ enTexPageBackGround+24+1,0,0,256,256,128,128},
	{ enTexPageBackGround+24+2,0,0,256,256,128,128},
	{ enTexPageBackGround+24+3,0,0,256,256,128,128},
	{ enTexPageBackGround+24+4,0,0,256,256,128,128},
	{ enTexPageBackGround+24+5,0,0,256,256,128,128},
};

CEndingDemo::CEndingDemo()
{
	m_sTimer  = 0;
	m_fScroll = WINDOW_H*1.f;//SCR_H*1.f;
	m_bEnd = gxFalse;

	m_Pos.x = 
	m_Pos.y = 
	m_Pos.z = 0;

	m_Rot = m_Pos;

	for(Sint32 ii=0;ii<enStarMax;ii++)
	{
		StStar[ii].x = (-100+viiSub::Rand()%200)/50.f;
		StStar[ii].y = (-100+viiSub::Rand()%200)/50.f;
		StStar[ii].z = (0+viiSub::Rand()%100)/10.f;
	}

	m_sSeq = 0;

	m_pStar = new StEndStar[enStarMax];

	m_bAfterEpisode = gxFalse;
	m_bEndingSkip   = gxTrue;

	m_sBeforeSeq = 0;
	m_sStaffrollSeq = 0;
	m_sAfterSeq = 0;
	m_sEndTimer = 0;

	m_sBeforeAlpha = 0xff;
	m_sStaffRollAlpha = 0;

	m_sScroll1 = 0;
	m_sScroll2 = 0;

	m_sRoughCnt = 0;

	CCockpit::GetInstance();

	m_bDisp            = gxFalse;
	m_uAllScore        = 0;
	m_uContinueBonus   = 0;
	m_uTrueEndBonus    = 0;
	m_uNoContinueBonus = 0;
	m_uTotalScore      = 0;

	m_sScoreAlpha = 0;

}



CEndingDemo::~CEndingDemo()
{
	delete[] m_pStar;
}


void CEndingDemo::Action()
{
	switch(m_sSeq){
	case enEndingSeqStart:
		CCockpit::GetInstance()->SetHidden();
		m_sSeq = enEndingSeqBefore;
		break;

	case enEndingSeqBefore:
		CCockpit::GetInstance()->SetHidden();
		SeqBefore();
		break;
	case enEndingSeqStaffRoll:
		CCockpit::GetInstance()->SetHidden();
		SeqStaffRoll();
		break;
	case enEndingSeqAfter:
		SeqAfter();
		break;
	}

	CCockpit::GetInstance()->Action();

}


void CEndingDemo::SeqBefore()
{

	switch( m_sBeforeSeq ){
	case 0:
		CFadeManager::GetInstance()->set_fadein(1,0xffffffff);
		m_sEndTimer  = 0;
		m_sBeforeSeq = 50;
		break;
	case 50:
		if( m_sEndTimer >= 30 )
		{
			viiMus::PlayBGM( enSoundBgm1,enMusicBgmEpilogue );
			m_sBeforeSeq = 100;
		}
		break;
	case 100:

		m_sScroll1 += 30;
		if( m_sScroll1 > (512-WINDOW_H)*100 )
		{
			m_sScroll1 = (512-WINDOW_H)*100;
		}

		if( m_sEndTimer > 60*5 )
		{
			m_sScoreAlpha += 2;
			if( m_sScoreAlpha >= 255 ) m_sScoreAlpha = 255;
		}
		if( m_sEndTimer == 2600 )
		{
			viiMus::StopBGM( enSoundBgm1,600 );
		}

#ifdef _VII_DEBUG_
		if( m_sEndTimer >= 3200 || (m_sEndTimer >= 0 && Joy[0].trg&enBtnDecision ) )
#else
		if( m_sEndTimer >= 3200 || (m_sEndTimer >= 1200 && Joy[0].trg&enBtnDecision ) )
#endif
		{
			viiMus::StopBGM( enSoundBgm1,180 );
			viiMus::PlayBGM( enSoundBgm2,enMusicBgmEnding );
			m_sBeforeSeq = 200;
		}
		break;
	case 200:
		m_sBeforeAlpha -= 2;

		if( m_sBeforeAlpha < 0 )
		{
			m_sBeforeAlpha = 0;
			m_sBeforeSeq = 300;
		}
		m_sScoreAlpha = m_sBeforeAlpha;
		break;

	case 300:
		m_sSeq = enEndingSeqStaffRoll;
		break;
	}

	StarScreen();
	m_sEndTimer ++;
}



void CEndingDemo::SeqStaffRoll()
{
	switch( m_sStaffrollSeq ){
	case 0:
		m_sEndTimer  = 0;
		m_sStaffrollSeq = 100;
		break;
	case 100:

		if( m_sEndTimer >= 120 && Joy[0].trg&enBtnDecision)
		{
			if( m_fScroll == -1200.f ||  m_bEndingSkip)
			{
				m_sStaffrollSeq = 200;
				viiMus::StopBGM( enSoundBgm2 , 300 );
			}
		}
		break;
	case 200:
		m_sStaffRollAlpha += 1;
		if( m_sStaffRollAlpha >= 255 )
		{
			m_sStaffRollAlpha = 255;
			if( !m_bAfterEpisode )
			{
				m_sStaffrollSeq = 900;
			}
			else
			{
				m_sStaffrollSeq = 300;
			}
		}
		break;

	case 300:
		m_sSeq = enEndingSeqAfter;
		break;

	case 900:
		CFadeManager::GetInstance()->set_fadeout();
		m_sStaffrollSeq = 950;
		break;
	case 950:
		if( !CFadeManager::GetInstance()->is_fade() )
		{
			m_sSeq = enEndingSeqAfter;
		}
		break;
	}

	Sint32 scr = m_fScroll;
	enum {
		bs=-500+WINDOW_H/2,
		wd=-80,
	};
	switch( scr ){ //Joy[0].trg&BTN_C )
	case bs+wd*0:
		m_stRough[0].m_sRoughSeq = 10;
		break;
	case bs+wd*1:
		m_stRough[1].m_sRoughSeq = 10;
		break;
	case bs+wd*2:
		m_stRough[2].m_sRoughSeq = 10;
		break;
	case bs+wd*3:
		m_stRough[3].m_sRoughSeq = 10;
		break;
	case bs+wd*4:
		m_stRough[4].m_sRoughSeq = 10;
		break;
	case bs+wd*5:
		m_stRough[5].m_sRoughSeq = 10;
		break;
	}

	m_sEndTimer++;

	StarControl();
	StarScreen();
	RoughPicture();

	//------------------------------------

	m_fScroll -= 0.3f;
	if( m_fScroll <= -1580) m_fScroll = -1580;

	m_sTimer ++;

}


void CEndingDemo::RoughPicture()
{
	for(Sint32 ii=0;ii<enRoughMax; ii++)
	{
		StRoughPicture *p = &m_stRough[ii];

		if( p->m_sRoughSeq  )
		{
			if( p->m_fScale > 0 )
			{
				p->m_fScale -= 0.00025f;
			}
			else
			{
				p->m_fScale = 0.f;
			}
		}
		if( p->m_sRoughSeq == 10 )
		{
			p->m_fRoughAlpha = ZERO;
			p->m_sRoughWait  = 0;
			p->m_sRoughSeq   = 20;
		}
		else if( p->m_sRoughSeq == 20 )
		{
			p->m_sRoughSeq   = 100;
		}
		else if( p->m_sRoughSeq == 100 )
		{
			//フェードイン
			if( p->m_fRoughAlpha < 100)
			{
				p->m_fRoughAlpha += 0.5f;
				if( p->m_fRoughAlpha >= 100.f ) p->m_fRoughAlpha = 100.f;
			}
			else
			{
				p->m_fRoughAlpha = 100.f;
				p->m_sRoughWait = 300;
				p->m_sRoughSeq   = 200;
			}
		}
		else if( p->m_sRoughSeq == 200 )
		{
			//待ち
			if( p->m_sRoughWait > 0 )
			{
				p->m_sRoughWait --;
			}
			else
			{
				p->m_sRoughSeq   = 300;
			}
		}
		else if( p->m_sRoughSeq == 300 )
		{
			//フェードアウト
			if( p->m_fRoughAlpha > 0)
			{
				p->m_fRoughAlpha -= 0.25f;
				if( p->m_fRoughAlpha <= 0.f ) p->m_fRoughAlpha = 0.f;
			}
			else
			{
				p->m_fRoughAlpha = 0.f;
				p->m_sRoughWait  = 999;
				p->m_sRoughSeq   = 999;
			}

		}
	}
}


void CEndingDemo::SeqAfter()
{
	if( !m_bAfterEpisode && m_sAfterSeq == 0)
	{
		m_sAfterSeq = 600;
	}

	switch( m_sAfterSeq ){
	case 0:
		m_sEndTimer  = 0;
		m_sAfterSeq = 50;
		break;
	case 50:
		m_sScroll2 += 30;

		if( m_sEndTimer >= 60*5 )
		{
			CCockpit::GetInstance()->SetMessage(9000);
			m_sEndTimer = 0;
			m_sAfterSeq = 100;
		}
		break;

	case 100:
		m_sScroll2 += 50;
		m_sStaffRollAlpha -= 1;
		if( m_sStaffRollAlpha <= 0 ) m_sStaffRollAlpha = 0;
		if( m_sEndTimer >= 1080 )
		{
			viiMus::PlaySound( enSoundStageClear );
			m_sEndTimer = 0;
			m_sAfterSeq = 200;
		}
		break;

	case 200:
		if( m_sEndTimer >= 200 )
		{
			m_sEndTimer = 0;
			m_sAfterSeq = 300;
		}
		break;

	case 300:
		m_sStaffRollAlpha += 16;
		if( m_sStaffRollAlpha >= 255 ) m_sStaffRollAlpha = 255;
//		m_sSeq = enEndingSeqStaffRoll;
		if( m_sEndTimer >= 340 )
		{
			m_sAfterSeq = 500;
		}
		break;
	case 500:
		CFadeManager::GetInstance()->set_fadeout();
		m_sAfterSeq = 600;
		break;
	case 600:
		if( !CFadeManager::GetInstance()->is_fade() )
		{
			m_sAfterSeq = 700;
		}
		break;
	case 700:
		m_bEnd = gxTrue;
		return;
	}

	if( m_sScroll2 > (1024-WINDOW_W)*100 )
	{
		m_sScroll2 = (1024-WINDOW_W)*100;
	}

	m_sEndTimer ++;

}



void CEndingDemo::StarControl()
{

/*	if( Joy[0].psh&JOY_D)
	{
		m_Pos.z += 0.01f*10;
	}
	else if( Joy[0].psh&JOY_U)
	{
		m_Pos.z -= 0.01f*10;
	}

	if( Joy[0].psh&JOY_L)
	{
		m_Pos.x += 0.1f;
	}
	else if( Joy[0].psh&JOY_R)
	{
		m_Pos.x -= 0.1f;
	}

	if( Joy[0].psh&BTN_A)
	{
		m_Rot.z += 0.1f*10;
	}
	else if( Joy[0].psh&BTN_B)
	{
		m_Rot.z -= 0.1f*10;
	}

	if( Joy[0].psh&BTN_X)
	{
		m_Rot.y += 0.1f;
	}
	else if( Joy[0].psh&BTN_Y)
	{
		m_Rot.y -= 0.1f;
	}
*/

}


void CEndingDemo::StarScreen()
{
	//単位ユニットを設定
	VECTOR dst;
	mtxSetUnit();

	m_Pos.z -= 0.01f*10;
	m_Rot.z += 0.3f;

	mtxTrans( &m_Pos );
	mtxRotY( DEG2RAD( m_Rot.y ) );
	mtxRotZ( DEG2RAD( m_Rot.z ) );

	for(Sint32 ii=0;ii<enStarMax;ii++)
	{
		mtxAffin( &dst , &StStar[ii] );
		TransPers(&dst);

		if(dst.z<0)
		{
			StStar[ii].z += 10.f;
		}
		else
		{
			Sint32 sAlp = 0xff*(1.f-dst.z);
			//viiDraw::printf( dst.x,dst.y,,".");
			m_pStar[ii].x = dst.x;
			m_pStar[ii].y = dst.y;
			m_pStar[ii].sCol = ARGB( sAlp,0xff,0xff,0xff);
		}
	}

}

void CEndingDemo::Command(Sint32 sCmd)
{
	
	
	
}


void CEndingDemo::Draw()
{
	//-------------------------------------------------------------------------
	//EndingDemo中
	//-------------------------------------------------------------------------

	switch(m_sSeq){
	case enEndingSeqBefore:
		DrawBefore();
		break;
	case enEndingSeqStaffRoll:
		DrawStaffRoll();
		break;
	case enEndingSeqAfter:
		DrawAfter();
		break;
	}

	for(Sint32 ii=0;ii<enStarMax;ii++)
	{
		//viiDraw::printf( m_pStar[ii].x , m_pStar[ii].y,m_pStar[ii].sCol , "." );
		viiDraw::Spr_NoScrl( &SprStar[0] , m_pStar[ii].x*100 , m_pStar[ii].y*100 , enStarPrio , ATR_DFLT , m_pStar[ii].sCol );
	}

	viiDraw::Box( 0, 0, WINDOW_W , WINDOW_H , enBlacFadePrio , gxTrue , ARGB(m_sStaffRollAlpha,0x01,0x01,0x01) );
}


void CEndingDemo::DrawBefore()
{
	Sint32 y = 0;
	y = -m_sScroll1/100;

	viiSub::MenuSprite( 0,y,enEndingScreenPrio, enTexPageBackGround+0,0,0,WINDOW_W,WINDOW_H*2,0,0,ATR_DFLT ,ARGB(m_sBeforeAlpha,0xff,0xff,0xff));

	Sint32 xx,yy,ww,sArgb;

	xx =16 ;
	yy = 200-10;
	ww = 160;

	sArgb = ARGB( m_sScoreAlpha , 0xf0 , 0xf0 , 0xf0 );

	viiDraw::sml_printf(xx,yy+10*0,sArgb,"%s" , StrBonusTbl[0] );
	viiDraw::sml_printf(xx,yy+10*1,sArgb,"%s" , StrBonusTbl[1] );
	viiDraw::sml_printf(xx,yy+10*2,sArgb,"%s" , StrBonusTbl[2] );
	viiDraw::sml_printf(xx,yy+10*3,sArgb,"%s" , StrBonusTbl[3] );
	viiDraw::sml_printf(xx,yy+10*4,sArgb,"%s" , StrBonusTbl[4] );

	viiDraw::sml_printf(xx,yy+10*6,sArgb,"%s" , StrBonusTbl[5] );

	viiDraw::sml_printf(xx+ww,yy+10*0,sArgb,"%d" , m_uAllScore );
	viiDraw::sml_printf(xx+ww,yy+10*1,sArgb,"%d" , m_uContinueBonus );
	viiDraw::sml_printf(xx+ww,yy+10*2,sArgb,"%d" , m_uTrueEndBonus );
	viiDraw::sml_printf(xx+ww,yy+10*3,sArgb,"%d" , m_uNoContinueBonus );
	viiDraw::sml_printf(xx+ww,yy+10*4,sArgb,"%d" , MISSION_ALLOVER_BONUS );

	viiDraw::sml_printf(xx+ww,yy+10*6,sArgb,"%d" , m_uTotalScore+MISSION_ALLOVER_BONUS );

}


void CEndingDemo::DrawStaffRoll()
{
	Sint32 ii=0;
	Sint32 yy = (Sint32)m_fScroll;
	Sint32 x,y;
	Sint32 sAlpha;
	Sint32 sPosTbl[][2]={
		{0,0},
		{-32,-16},
		{16,8},
		{-32,-8},
		{0,0},
		{-32,8},
	};

	for(Sint32 ii=0;ii<6;ii++)
	{
		Sint32 a = 0xF0*m_stRough[ii].m_fRoughAlpha/100.f;
		Float32 fScale = m_stRough[ii].m_fScale*0.8f;
		Sint32 x = sPosTbl[ii][0];
		Sint32 y = sPosTbl[ii][1];

		viiSub::MenuSprite( 232+128+x , 8+128+y , enEndingScreenPrio2, 
			SprRough[ii].page,
			SprRough[ii].u,
			SprRough[ii].v,
			SprRough[ii].w,
			SprRough[ii].h,
			SprRough[ii].cx,
			SprRough[ii].cy,
			ATR_DFLT ,ARGB(a,0xff,0xff,0xff),0, fScale,fScale);
	}

	ii = 0;

	while( gxTrue ) {
		if( EndingName[ii] == NULL ) break;

		if( EndingName[ii][0] == '@' )
		{
			viiDraw::big_printf( 96,yy+24+ii*20, 0xF0FFFFFF , "@"   );
		}
		else
		{
			x = 32;
			y = yy+32+ii*20;
			if( y <= -32 )
			{
				ii ++;
				continue;
			}
			if( y >= WINDOW_H+32 )
			{
				ii ++;
				continue;
			}
			sAlpha = 250;
			if( y < 32 )
			{
				sAlpha = 250*y/32;
			}
			if( y > WINDOW_H-32 )
			{
				sAlpha = 250 - 250*(y-WINDOW_H+32)/32;
			}
			if( sAlpha>250 ) sAlpha = 250;
			if( sAlpha<0 ) sAlpha = 0;
			//			viiSub::MenuPrintf( 32,yy+32+ii*24,0xFF00FF00,"%s",EndingName[ii] );
			CCockpit::GetInstance()->KanjiPrintfZ( x+2,y+0,enKanjiPrio,ARGB(sAlpha/2,0x80,0x00,0xA0),"%s",EndingName[ii] );
			CCockpit::GetInstance()->KanjiPrintfZ( x-2,y+0,enKanjiPrio,ARGB(sAlpha/2,0x80,0x00,0xA0),"%s",EndingName[ii] );
			CCockpit::GetInstance()->KanjiPrintfZ( x+0,y-2,enKanjiPrio,ARGB(sAlpha/2,0x80,0x00,0xA0),"%s",EndingName[ii] );
			CCockpit::GetInstance()->KanjiPrintfZ( x+0,y+2,enKanjiPrio,ARGB(sAlpha/2,0x80,0x00,0xA0),"%s",EndingName[ii] );
			CCockpit::GetInstance()->KanjiPrintfZ( x,y    ,enKanjiPrio,ARGB(sAlpha,0xF0,0xF0,0xF0) ,"%s",EndingName[ii] );
		}
		ii ++;
	}


}



void CEndingDemo::DrawAfter()
{
	CCockpit::GetInstance()->Draw();

	Sint32 x = 0;
	x = -m_sScroll2/100;

	viiSub::MenuSprite( x,0,enEndingScreenPrio2, enTexPageBackGround+16,0,0,1024,WINDOW_H,0,0,ATR_DFLT );
	
	if( m_bAfterEpisode && m_sAfterSeq >= 300)
	{
		viiDraw::big_printf2(222,200, 150,0xff8fC0ff , EndingString[0] );
	}
	
}


