//--------------------------------------------
//
// メッセージウインドウ
//
//--------------------------------------------
//#include <windows.h>
#include <gunvalken.h>
#include "CMsgManager.h"
#include "CMsgWindow.h"
#include "CFaceScreen.h"

enum {
	enWindowOpenSeqNone,
	enWindowOpenSeqStart,
	enWindowOpenSeqMessageInit,
	enWindowOpenSeqMessageMain,
	enWindowOpenSeqWaitButton,
	enWindowOpenSeqWaitTime,
	enWindowOpenSeqNext,
	enWindowOpenSeqClose,
};

gxSprite StSprMessageBoard[]=		//メッセージボード
{
//	{ enTexPageSystemUI ,0,208,256,48,0,0},		//BASE
//	{ enTexPageSystemUI ,240,48,16,16,8,8},		//カーソルアイコン

	{ enTexPageSystemUI ,0,192 ,256,64,0,0},	//BASE
	{ enTexPageSystemUI ,416,32,11,11,5,5},		//カーソルアイコン
};

CMsgWindow::CMsgWindow()
{

	//	m_pFontScript->Load( "HoundData\\Font\\Scen001\\DatMsg.bmp" );
	//"HoundData\\Font\\Scen001\\DatMsg.bmp" );
	//	m_pFontScript->Run(1000);

	m_pFontScript = NULL;
	m_pFaceScreen = NULL;
	m_pFaceScreen = new CFaceScreen();
	m_pFontScript = new CMsgManager();

	fCount = ZERO;
	m_sButtonWaitTimer = 0;
	m_sWindowOpenSeq = enWindowOpenSeqNone;
	m_bSpeedUp = gxFalse;
	m_sWindowHeight = 0;
	m_bFastButtonOn = gxFalse;

	m_sCursorAnimCnt = 0;
	m_bCursorDraw    = gxFalse;
}


CMsgWindow::~CMsgWindow()
{

	if(m_pFontScript ) delete m_pFontScript;
	if(m_pFaceScreen)  delete m_pFaceScreen;

}


void CMsgWindow::Run(Sint32 sIndex)
{

	m_sWindowOpenSeq = enWindowOpenSeqStart;
	m_pFontScript->Run( sIndex ); 

}


void CMsgWindow::Reset()
{
	//------------------------------
	//Windowのリセット
	//------------------------------

	if(pGame)
	{
		pGame->cancel_the_world();
	}

	m_sButtonWaitTimer = 0;
	m_sWindowOpenSeq   = enWindowOpenSeqNone;
	m_pFontScript->Reset();
}


void CMsgWindow::Action()
{
	gxBool bSpeedUp = gxFalse;
	Sint32 sSpeed = 4;

	m_pFontScript->Execute();

	switch(m_sWindowOpenSeq){
	case enWindowOpenSeqNone:
		//WindowInit
		m_bFastButtonOn = gxFalse;
		m_sWindowHeight = 0;
		break;

	case enWindowOpenSeqStart:
		//WindowOpen
		fCount = ZERO;
		m_pFaceScreen->SetFace( m_pFontScript->GetFaceIndex() );
		if( m_pFontScript->IsFreeze() )
		{
			if( pGame ) pGame->set_the_world();
		}
		if( m_sWindowHeight == 0 )
		{
			viiMus::PlaySound( enSoundSwitch03 );
		}

		m_sWindowHeight += 8;
		if(m_sWindowHeight >= 100)
		{
			m_sWindowHeight = 100;
			m_sWindowOpenSeq = enWindowOpenSeqMessageInit;
			//char *pStr = m_pFontScript->GetMessage();
		}
		break;

	case enWindowOpenSeqMessageInit:
		m_bCursorDraw = gxFalse;
		m_sCursorAnimCnt = 0;
		m_pFaceScreen->SetFace( m_pFontScript->GetFaceIndex() );
		m_bSpeedUp = gxFalse;
		if( pGame )
		{
			pGame->cancel_the_world();
		}
		fCount = ZERO;
		if( m_pFontScript->IsFreeze() )
		{
			pGame->set_the_world();
		}
		if( m_pFontScript->GetStatus() == enCodeFinish )
		{
			//終了していた場合はCLOSE処理
			m_sWindowOpenSeq = enWindowOpenSeqClose;
		}
		else
		{
			m_sWindowOpenSeq = enWindowOpenSeqMessageMain;
		}
		break;

	case enWindowOpenSeqMessageMain:
		//Message Start
		{
			Sint32 sLen = m_pFontScript->GetLen();
			Sint32 sOld = (Sint32)(fCount/2);
			m_pFaceScreen->SetKuchiPaku(gxFalse);
			fCount += 0.32f;
			if(sOld < (Sint32)(fCount/2))
			{
				viiMus::PlaySound( enSoundTypeWriter );
			}
			if( Joy[0].trg&enBtnCancel )
			{
				m_bFastButtonOn = gxTrue;
			}
			else if( Joy[0].trg&enBtnDecision )
			{
				m_bFastButtonOn = gxFalse;
				m_bSpeedUp = gxTrue;
			}

			if( m_bFastButtonOn )
			{
//				fCount = sLen*1.f;
				sSpeed = 16;
				m_bSpeedUp = bSpeedUp = gxTrue;
			}
			if(m_bSpeedUp && Joy[0].psh&enBtnDecision )
			{
				bSpeedUp = gxTrue;
			}
			if( m_pFontScript->IsFreeze() &&  bSpeedUp )
			{
				//Freeze時には文字送りを行う
				fCount += 0.32f*sSpeed;
			}

			m_pFaceScreen->SetKuchiPaku(gxTrue);

			if(fCount >= sLen)
			{
				gxBool bEnd = gxFalse;
				fCount = sLen*1.f;

				m_pFaceScreen->SetKuchiPaku(gxFalse);

				if( m_pFontScript->IsWaitButton() )	bEnd = gxTrue;//|| m_pFontScript->IsWaitTime())
				if( m_pFontScript->GetStatus() == enCodeFinish )
				{
					bEnd = gxTrue;
				}

				if( bEnd )
				{
					//ボタン待ちなら
					if( m_pFontScript->IsFreeze() )
					{
						//フリーズ時はボタン待ち
						m_sWindowOpenSeq = enWindowOpenSeqWaitButton;
					}
					else
					{
						//自然消滅なら時間待ち
						m_sButtonWaitTimer = 0;
						m_sWindowOpenSeq = enWindowOpenSeqWaitTime;
					}
				}
			}
		}
		break;

	case enWindowOpenSeqWaitButton:
		//ボタン待ち
		m_bCursorDraw = gxTrue;
		m_sCursorAnimCnt ++;

		if(Joy[0].trg&enBtnDecision )
		{
			viiMus::PlaySound( enSoundSwitch02 );
			m_bFastButtonOn = gxFalse;
			m_sWindowOpenSeq = enWindowOpenSeqNext;
		}
		else if( Joy[0].rep&enBtnCancel )
		{
			viiMus::PlaySound( enSoundSwitch02 );
			m_bFastButtonOn = gxTrue;
			m_sWindowOpenSeq = enWindowOpenSeqNext;
		}
		break;

	case enWindowOpenSeqWaitTime:
		//時間待ち
		m_sButtonWaitTimer ++;
		if( m_sButtonWaitTimer >= 120 )
		{
			m_sWindowOpenSeq = enWindowOpenSeqNext;
		}
		break;

	case enWindowOpenSeqNext:
//			m_sWindowOpenSeq = enWindowOpenSeqClose;
		m_pFontScript->Prev();
		m_sWindowOpenSeq = enWindowOpenSeqMessageInit;
		break;

	case enWindowOpenSeqClose:

		m_sWindowHeight -= 8;
		if(m_sWindowHeight <= 0)
		{
			if(pGame) pGame->cancel_the_world();
			m_sWindowHeight = 0;
			m_sWindowOpenSeq = enWindowOpenSeqNone;
		}
		break;
	default:
		break;
	}


/*
	if( Joy[0].trg&BTN_L1 )
	{
		m_pFontScript->Run(1000); 
		fCount = ZERO;
	}
	else if( Joy[0].trg&BTN_R1 )
	{
		m_pFontScript->Run(1100); 
		fCount = ZERO;
	}
*/

/*
	if( Joy[0].trg&BTN_A )
	{
		if( m_pFontScript->IsWaitButton() )
		{
			m_pFontScript->Prev();
			fCount = ZERO;
		}
	}
*/

/*
	if( m_pFontScript->GetStatus() == enCodeFinish )
	{
		//メッセージウインドウ終了
		if(pGame)
		{
			pGame->cancel_the_world();
		}
	//	if( pStr == NULL )
		fCount = ZERO;
		return;
	}
	else
	{
		Sint32 sLen = m_pFontScript->GetLen();

		if( m_pFontScript->IsFreeze() && Joy[0].trg&enBtnDecision )
		{
			//Freeze時には文字送りを行う
			fCount = sLen;
		}

		if( fCount >= sLen )
		{
			if( m_pFontScript->IsWaitButton() )
			{
				//２秒待つ
				m_sButtonWaitTimer ++;
				if( m_sButtonWaitTimer >= 120 )
				{
					m_pFontScript->Prev();
					fCount = ZERO;
					m_pFontScript->FreezeCancel();
					m_sButtonWaitTimer = 0;
				}
			}
		}
	}
*/
	//IsWaitButton

	m_pFaceScreen->Action();

}

gxBool CMsgWindow::IsMessageNow()
{

	switch(m_sWindowOpenSeq){
	case enWindowOpenSeqClose:
		return gxFalse;

	case enWindowOpenSeqNone:
		return gxFalse;
	}

	return gxTrue;

}


void CMsgWindow::Draw(Sint32 bx,Sint32 by)
{
	switch(m_sWindowOpenSeq){
	case enWindowOpenSeqNone:
		return;
	default:
		break;
	}

	ghPos base;
	base.x = bx+52;//68;//80;//152;
	base.y = by+7;//WINDOW_H-64;//8;//216;//8;

	Sint32 xx=base.x,yy=base.y;
	Sint32 iCount = (Sint32)fCount;
	Sint32 s,w;

	viiSub::MenuSprite( &StSprMessageBoard[ 0 ]  ,  base.x, base.y-8 , PRIO_SYSTEM ,ATR_DFLT , 0x80FFFFFF/*c0a0a0a0*/ ,m_sWindowHeight/100.f );

	//顔の表示	
	m_pFaceScreen->Draw(bx,by,m_sWindowHeight);

	if( !IsMessageNow() ) return;

	//メッセージの表示

	Sint32 sLen = m_pFontScript->GetLen();

	xx = base.x +12;
	yy = base.y +18;

	for(Sint32 ii=0; ii<sLen ; ii++)
	{
		if( ii >= iCount ) continue;

		Sint32 num,u,v;
		num = m_pFontScript->GetKanjiIndex( ii );

		s = m_pFontScript->GetKanjiPos( num );
		w = m_pFontScript->GetKanjiWidth( num );

		if( num == m_pFontScript->enKaigyoKode )
		{
			xx = base.x+12;
			yy += 16;
			continue;
		}

		u = (num%32)*16;
		v = (num/32)*16;

		xx -= s;

		PutSpriteDX( xx,yy,PRIO_SYSTEM,	enTexPageFont,u,v,16,16,0,0	);

		xx += (s+w);
		xx -= 2;
		//xx += 12;
	}

	if( m_bCursorDraw )
	{
		Float32 w = (-100+(m_sCursorAnimCnt*8)%200)/100.f;
	 	viiSub::MenuSprite( xx+12, yy+9 , PRIO_SYSTEM ,
//	 	viiSub::MenuSprite( base.x+246, base.y+32 , PRIO_SYSTEM ,
	 		StSprMessageBoard[ 1 ].page,
	 		StSprMessageBoard[ 1 ].u,
	 		StSprMessageBoard[ 1 ].v,
	 		StSprMessageBoard[ 1 ].w,
	 		StSprMessageBoard[ 1 ].h,
	 		StSprMessageBoard[ 1 ].cx,
	 		StSprMessageBoard[ 1 ].cy,
	 		ATR_DFLT , 0xa0ffffff ,0,w );
	}

}


