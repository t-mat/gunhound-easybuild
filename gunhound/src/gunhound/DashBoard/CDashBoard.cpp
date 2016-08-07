//-----------------------------------------------------------
//
// Dracue ダッシュボード プログラム
//
//-----------------------------------------------------------

#include <gunhound/gga2.h>
#include <gunhound/gga2x.h>
#include "CDashBoard.h"
#include "CDashEnvironment.h"
#include "CDashPadConfig.h"

#define KEYBOARD_DEC (KEYSIGN12)	//VK_RETURN)
#define KEYBOARD_CAN (KEYSIGN11)	//VK_SPACE)
#define KEYBOARD_DASHOPEN (BTN_START)

//#define KEYBOARD_DEC 'X'
//#define KEYBOARD_CAN 'Z'
//#define KEYBOARD_DASHOPEN (BTN_ST)

enum {
	enMenuBarOpenSpeed = 8,
	enAnnounceFrame    = 120,
//	enTpgDashBoard = 0,
//	enTpgDashControl = 1,
};


enum {
	enDashBoardMenuEnviroment,
	enDashBoardMenuPadConfig,
	enDashBoardMenuManual,
	enDashBoardMenuExit,
	enDashBoardMenuMax,

	enDashBoardMenuMain,
};


gxSprite SprIconTbl[]={
	{ enTpgDashBoard,96,176+16*0,40,16,0,8},
	{ enTpgDashBoard,96,176+16*1,72,16,0,8},
};

gxSprite StSprDashBoard[]={
	//ダッシュボード中のカーソル
//	{enTpgDashBoard,0,192,96,32,0,0},	//ボード
//	{enTpgDashBoard,0,224,96,32,0,0},	//選択中

	{enTpgDashBoard,0,192,96,32,0,0},	//ボード
	{enTpgDashBoard,0,224,96,32,0,0},	//選択中
};

static char *MainMenuStr[]={
	"起動時の画面サイズや、環境の設定を行います。",
	"コントローラーの設定を行います。",
	"取扱説明書を開きます。",
	"リセットしてタイトル画面に戻ります。",
};

enum {
	enIconPosition = 480,
};

CDashBoard* CDashBoard::s_pInstance = NULL;

void PadConfig();

CDashBoard::CDashBoard()
{
	m_sAlpha     = 0;
	m_sAlphaWall = 0;
	m_bDispLogo = gxFalse;

	m_sMenuBarCnt  = 0;
	m_bMenuDisable = gxTrue;

	m_sSelect = 0;
	m_sSeq    = enDashBoardMenuMain;

	m_bReset = gxFalse;

	//子ウインドウの初期化
	m_pPadConfig   = NULL;
	m_pEnvironment = NULL;

	m_bDisp     = gxFalse;
	m_bDispWall = gxFalse;

	m_sTimer = 0;
	m_fScroll = 0.f;
	m_sScrollWait = enAnnounceFrame;
	m_pAnnounce = NULL;

	m_sIconFlag = 0;
	m_sIconX    = 600*100;//enIconPosition;

	m_sForceConfig = 0;
	m_bForceMenuDisp = gxFalse;
	m_bLock = gxFalse;
	m_sVolume = 0;
	m_sVolumeWait = 0;

	m_fScaleL = 1.f;
	m_fScaleR = 1.f;
}


CDashBoard::~CDashBoard()
{
	//子ウインドウの始末
	
	if( m_pPadConfig )   delete m_pPadConfig;
	if( m_pEnvironment ) delete m_pEnvironment;
	
	m_sVolume     = 0;
	m_sVolumeWait = 0;
}


void CDashBoard::Action()
{
	//------------------------------------------
	//Logoのコントロール
	//------------------------------------------
	if( ghLib::GetMouseWheelNum() != m_sVolume )
	{
		m_sVolumeWait = 60;

		Sint32 sCursor = ghLib::GetMouseWheelNum();
		Sint32 sVolume = sCursor*-1;

		m_sVolume = sCursor;

		if( sCursor < 0 )
		{
			sVolume  = 80+80*sCursor/WHEEL_MAX;
		}
		else
		{
			sVolume  = 80+20*sCursor/WHEEL_MAX;
		}

		//ghLib::SetMasterVolume( sVolume , 0 );
	}

	//sysMenu::Printf( 320,160+16*2,ARGB_DFLT, "%d" ,  );

#ifdef _TRIAL_VERSION_
	//販売元ロゴなど

	//イーフロ
	sysMenu::Sprite( 16,WINDOW_H + (SCR_H-WINDOW_H)/4+14,enPrioHidefBoard+1, enTexPageSysLogo,0,112,128,48,0,32 ,ATR_DFLT , 0xf0ffffff );

//	//デジハ
//	sysMenu::Sprite( 16,WINDOW_H + (SCR_H-WINDOW_H)/4+2,enPrioHidefBoard+1, enTexPageSysLogo,0,160,128,32,0,16 ,ATR_DFLT , 0xf0ffffff );
#endif

	LogoControl();
	if( m_sAlpha ) DrawLogo();

	//メニューバーのコントロール

	if( m_bMenuDisable ) m_sMenuBarCnt = 0;

	ControlMenuBar();
	m_sTimer++;
}


void CDashBoard::SetLogo(gxBool bFlag)
{
	//------------------------------------------
	//ロゴの表示切替え
	//------------------------------------------

	m_bDispLogo = bFlag;

}


void CDashBoard::SetWallPaper(gxBool bFlag)
{
	//------------------------------------------
	//壁紙の表示切替え
	//------------------------------------------

	m_bDispWall = bFlag;

}


void CDashBoard::LogoControl()
{
	//------------------------------------------
	//ロゴのコントロール
	//------------------------------------------

	if( m_bDispLogo )
	{
		m_sAlpha += enMenuBarOpenSpeed;

		if( m_sAlpha >= 0xff )
		{
			m_sAlpha = 0xff;
		}
	}
	else
	{
		m_sAlpha -= 8;
		if( m_sAlpha <= 0x00 )
		{
			m_sAlpha = 0x00;
		}
	}


	if( m_bDispWall )
	{
		m_sAlphaWall += enMenuBarOpenSpeed;

		if( m_sAlphaWall >= 0xff )
		{
			m_sAlphaWall = 0xff;
		}
	}
	else
	{
		m_sAlphaWall -= 8;
		if( m_sAlphaWall <= 0x00 )
		{
			m_sAlphaWall = 0x00;
		}
	}
}


void CDashBoard::DrawLogo()
{
	//------------------------------------------
	//ロゴの描画
	//------------------------------------------

	GGX_DrawBox(0,0,WINDOW_W,WINDOW_H,250,gxTrue,ARGB( m_sAlphaWall , 0xF0 , 0xF0 , 0xF0 ) );
	sysMenu::Sprite( 0,84,250, enTexPageSysLogo,0,0,WINDOW_W,96+8,0,0 ,ATR_DFLT , ARGB( m_sAlpha , 0xFF , 0xFF , 0xFF ) );

}


void CDashBoard::ControlMenuBar()
{
	//------------------------------------------
	//メニューバーのコントロール
	//------------------------------------------

	DrawIcon();
	DrawAnnounce();
	if( m_sVolumeWait > 0 )
	{
		m_sVolumeWait --;
		DrawSoundVolume();
	}

	switch(m_sForceConfig){
	case 1:
		if( ConfigEnvironment() )
		{
			m_sForceConfig = 0;
		}
		return;
	case 2:
		if( ConfigController() )
		{
			m_sForceConfig = 0;
		}
		return;
	default:
		break;
	}

	if( !m_bMenuDisable )
	{
		//メニュー有効のとき
		if( Joy[0].trg&KEYBOARD_DASHOPEN && !m_bLock )
		{
			//スタートボタン待ち
			sysMenu::Play( enSoundMenuOpen );
			m_bDisp = !m_bDisp;
		}
	}
	else
	{
		m_bDisp = gxFalse;
	}

	if( m_bDisp )
	{
		m_sMenuBarCnt += enMenuBarOpenSpeed;
	}
	else
	{
		m_sMenuBarCnt -= enMenuBarOpenSpeed;
		m_pAnnounce = NULL;
	}

	if( m_sMenuBarCnt >= 100 || m_bForceMenuDisp )
	{
		m_sMenuBarCnt = 100;
		ManuSeqControl();
	}
	else if( m_sMenuBarCnt <= 0 )
	{
		m_sSelect = enDashBoardMenuEnviroment;
		m_sSeq = enDashBoardMenuMain;
		m_bDisp = gxFalse;
		m_sMenuBarCnt = 0;

		//各種設定をキャンセルしてリセット
		if( m_pPadConfig )
		{
			delete m_pPadConfig;
			m_pPadConfig = NULL;
		}
		if( m_pEnvironment )
		{
			delete m_pEnvironment;
			m_pEnvironment = NULL;
		}

		return;
	}

	DrawMenuBar();

//	if( m_sMenuBarCnt == 100 )
//	{
//	}

}


void CDashBoard::ManuSeqControl()
{
	//---------------------------------------------
	//メニュー内部
	//---------------------------------------------

	switch( m_sSeq ){
	case enDashBoardMenuMain:
		//---------------------------------------------
		//メインメニュー
		//---------------------------------------------
		CDashBoard::GetInstance()->SetIcon( enIconNone );
		m_sSeq = MainMenu();
		break;

	case enDashBoardMenuEnviroment:
		//---------------------------------------------
		//環境設定を行う
		//---------------------------------------------
		CDashBoard::GetInstance()->SetIcon( enIconDecision|enIconCancel );
		if( ConfigEnvironment() )
		{
			m_sSeq = enDashBoardMenuMain;
			CDashBoard::GetInstance()->SetLock( gxFalse );
		}
		break;

	case enDashBoardMenuPadConfig:
		//---------------------------------------------
		//パッド設定を行う
		//---------------------------------------------
		CDashBoard::GetInstance()->SetIcon( enIconDecision|enIconCancel );
		if( ConfigController() )
		{
			m_sSeq = enDashBoardMenuMain;
			CDashBoard::GetInstance()->SetLock( gxFalse );
		}

		break;

	case enDashBoardMenuManual:
		//---------------------------------------------
		//マニュアルを起動
		//---------------------------------------------

		if( ghLib::IsFullScreen() )
		{
			ghLib::ChangeWindowMode( gxTrue );
		}

#ifdef PLATFORM_WINDOWS
		ExecuteApp( _URL_DOCUMENT_ );
#endif
		m_sSeq = enDashBoardMenuMain;
		break;

	case enDashBoardMenuExit:
		//---------------------------------------------
		//ゲームを終了する
		//---------------------------------------------
		m_bReset = gxTrue;
		m_sSeq = enDashBoardMenuMain;
		break;
	}
}


gxBool CDashBoard::ConfigEnvironment()
{
	//-------------------------------------------------------------
	//環境設定
	//-------------------------------------------------------------

	if( m_pEnvironment == NULL )
	{
		m_pEnvironment = new CDashEnvironment();
		CDashBoard::GetInstance()->SetLock(gxTrue);
	}
	m_pEnvironment->Action();
	m_pEnvironment->Draw();
	if(m_pEnvironment->IsEnd())
	{
		delete m_pEnvironment;
		m_pEnvironment = NULL;
		CDashBoard::GetInstance()->SetLock( gxFalse );
		return gxTrue;
	}
	return gxFalse;
}


gxBool CDashBoard::ConfigController()
{
	//-------------------------------------------------------------
	//コントローラー
	//-------------------------------------------------------------

	if( m_pPadConfig == NULL )
	{
		m_pPadConfig = new CDashPadConfig();
		CDashBoard::GetInstance()->SetLock(gxTrue);
	}
	m_pPadConfig->Action();
	m_pPadConfig->Draw();

	if(m_pPadConfig->IsEnd())
	{
		delete m_pPadConfig;
		m_pPadConfig = NULL;
		m_sSeq = enDashBoardMenuMain;
		CDashBoard::GetInstance()->SetLock( gxFalse );
		return gxTrue;
	}

	return gxFalse;
}


Sint32 CDashBoard::MainMenu()
{

	//-------------------------------------------------------------
	//ダッシュボードメニュー選択
	//-------------------------------------------------------------
	if( Joy[0].rep&JOY_L )
	{
		sysMenu::Play( enSoundMenuSel );
		m_sTimer = 0;
		m_sSelect --;
		m_fScaleL = 0.5f;
	}
	else if( Joy[0].rep&JOY_R )
	{
		sysMenu::Play( enSoundMenuSel );
		m_sTimer = 0;
		m_sSelect ++;
		m_fScaleR = 0.5f;
	}
	else if( Joy[0].trg&enBtnDecision || gxLib::KeyBoard( KEYBOARD_DEC )&enStatTrig )
	{
		sysMenu::Play( enSoundMenuDec );
		return m_sSelect;

	}

	m_sSelect = (enDashBoardMenuMax+m_sSelect)%enDashBoardMenuMax;

	CDashBoard::GetInstance()->SetAnnounce( MainMenuStr[m_sSelect] );

	m_fScaleL += (1.0f-m_fScaleL)/5;
	m_fScaleR += (1.0f-m_fScaleR)/5;

	return enDashBoardMenuMain;
}


void CDashBoard::DrawMenuBar()
{
	//-------------------------------------------------------------
	//メニューバーの表示
	//-------------------------------------------------------------
	gxPos _BasePos;
	Sint32 x,y,z;

	_BasePos.x = 4;
	_BasePos.y = -128+128*m_sMenuBarCnt/100;
	_BasePos.z = enPrioDashBoardMain;

	x = _BasePos.x;
	y = _BasePos.y;
	z = _BasePos.z;

	x = _BasePos.x;
	y = _BasePos.y;
	z = _BasePos.z;

	Sint32 n=0;
	Sint32 sCOL; 

	Sint32 PosTbl[]={
		44,12,
		140,12,
		236,12,
		332,12
	};

	//カーソル
	for(Sint32 ii=0;ii<4;ii++)
	{
		n = 0;

		sCOL = 0xC0;
		if( m_sSelect == ii )
		{
			Float32 fKaku = s_cast<Float32>((m_sTimer*4)%360);
			sCOL = s_cast<Sint32>( gxAbs( 0xFF* gxLib::Cos( fKaku ) ) );
			if( m_sSeq != enDashBoardMenuMain )
			{
				sCOL = 0xF0;
			}
			n = 1;
		}
		sysMenu::Sprite( &StSprDashBoard[n], x+PosTbl[ii*2+0] ,y+PosTbl[ii*2+1] ,z-1  ,ATR_DFLT       , 0xff010101  );
		sysMenu::Sprite( &StSprDashBoard[n], x+PosTbl[ii*2+0] ,y+PosTbl[ii*2+1] ,z    ,ATR_ALPHA_PLUS , ARGB(sCOL,0xFF,0xFF,0xFF)  );
	}

	//ダッシュボード
	sysMenu::Sprite( x , y ,z, enTpgDashBoard,0,0 ,480,56,0,0 );

	//バックボード
	z = _BasePos.z;
	GGX_DrawBox(0,0,WINDOW_W,WINDOW_H,z-1,gxTrue,ARGB( (m_sMenuBarCnt*0x80/100) , 0x10 , 0x10 , 0x10 ) ,ATR_ALPHA_MINUS );

	sysMenu::Sprite( 32    , y+26    ,z, enTpgDashBoard,0,176 ,32,16,13,4 ,ATR_DFLT, ARGB_DFLT,90 , m_fScaleL,m_fScaleL);
	sysMenu::Sprite( 480-32, y+26 ,z, enTpgDashBoard,0,176 ,32,16,13,4 ,ATR_DFLT, ARGB_DFLT,-90, m_fScaleR,m_fScaleR);
}


void CDashBoard::SetAnnounce( char *pStr )
{
	if(pStr != m_pAnnounce)
	{
		//違うメッセージならスクロール位置をリセット
		m_fScroll = 0.f;
		m_sScrollWait = enAnnounceFrame;
	}
	m_pAnnounce = pStr;
}

void CDashBoard::DrawAnnounce()
{
	//---------------------------------------
	//アナウンス描画
	//---------------------------------------

	if( m_pAnnounce )
	{
		Sint32 sLength = strlen( m_pAnnounce );

/*
		if( (sLength*12)/2 >= WINDOW_W)
		{
			if( m_sScrollWait > 0 ) m_sScrollWait --;
			if( m_sScrollWait == 0)
			{
				m_fScroll -= 1.f;
				if(m_fScroll*-1.f >= WINDOW_W+120)
				{
					m_fScroll = WINDOW_W+32;
				}
			}
		}
*/
//		sysMenu::Printf( WINDOW_W/2-sLength*3+m_fScroll,WINDOW_H-32,ARGB_DFLT, "%s" , m_pAnnounce );
//		GGX_DrawBox(0,WINDOW_H-24-12,WINDOW_W,WINDOW_H-4-12,enPrioDashBoardMain,gxTrue,ARGB( (100*0x60/100) , 0x80 , 0x80 , 0x10 ) ,ATR_ALPHA_MINUS );

		Sint32 xx = s_cast<Sint32>(WINDOW_W/2-sLength*3+m_fScroll);
		sysMenu::Printf( xx ,WINDOW_H-32,ARGB_DFLT, "%s" , m_pAnnounce );
		GGX_DrawBox(0,WINDOW_H-36,WINDOW_W,WINDOW_H-16,enPrioDashBoardMain,gxTrue,ARGB( (100*0x60/100) , 0x80 , 0x80 , 0x10 ) ,ATR_ALPHA_MINUS );
	}

}


void CDashBoard::DrawIcon()
{
	//---------------------------------------
	//アイコン表示
	//---------------------------------------
	Sint32 x,y,x2;
	Sint32 n=0;
	Sint32 w = 0,ii;

	x2 = x = enIconPosition;
	y = 256;

	if( m_sIconFlag == 0 )
	{
		m_sIconX += (60000-m_sIconX)/10;
		return;
	}

	for(ii=0;ii<enIconMax; ii++)
	{
		if( m_sIconFlag&(0x01<<n) )
		{
			x -= 8;
			x -= SprIconTbl[n].w;
		}
		n ++;
	}

	m_sIconX += (x*100-m_sIconX)/5;

	n = 0;

	x2 = m_sIconX/100;

	for(ii=0;ii<enIconMax; ii++)
	{
		if( m_sIconFlag&(0x01<<n) )
		{
			sysMenu::Sprite( &SprIconTbl[n] , x2 , y , enPrioDashBoardMain  );
			x2 += SprIconTbl[n].w;
			x2 += 8;
		}
		n ++;
	}

}

void CDashBoard::DrawSoundVolume()
{
	//音声ボリューム表示

	Sint32 x = 32;
	Sint32 sCol = 0xa010f010;

	if( m_sVolume == WHEEL_MAX )
	{
		sCol = 0xa0ff0000;
	}
	if( m_sVolume == 0 )
	{
		sCol = 0xa0ffff00;
	}

	for(Sint32 ii=0;ii<(WHEEL_MAX+m_sVolume)+1;ii++)
	{
		GGX_DrawBox( x,WINDOW_H-36 , x+12,WINDOW_H-8 , enPrioDashBoardMain , gxTrue , sCol  );
		x += 16;
	}

}


void sysMenu::Printf(Sint32 x,Sint32 y,Sint32 col,char *msg,...)
{
	//---------------------
	//メニュー用文字表示
	//---------------------

	char buf[0xff];
	va_list app;

	va_start(app, msg);
	vsprintf(buf, msg, app);
	va_end(app);

	msg_printfwithCol(x,y,enPrioDashBoardText,col,&buf[0]);
}


void sysMenu::Selectf(Sint32 sPattern , Sint32 x,Sint32 y,char *msg,...)
{
	char buf[0xff];
	va_list app;

	va_start(app, msg);
	vsprintf(buf, msg, app);
	va_end(app);

	if(sPattern > 0)
	{
		msg_printfwithCol(x+1,y,enPrioDashBoardText,0xA000BF00,&buf[0]);
		msg_printfwithCol(x-1,y,enPrioDashBoardText,0xA000BF00,&buf[0]);
		msg_printfwithCol(x,y-1,enPrioDashBoardText,0xA000BF00,&buf[0]);
		msg_printfwithCol(x,y+1,enPrioDashBoardText,0xA000BF00,&buf[0]);
		msg_printfwithCol(x,y,enPrioDashBoardText  ,0xF0FFffA0,&buf[0]);
	}
	else if(sPattern < 0)
	{
		msg_printfwithCol(x+1,y,enPrioDashBoardText,0xA0404040,&buf[0]);
		msg_printfwithCol(x-1,y,enPrioDashBoardText,0xA0404040,&buf[0]);
		msg_printfwithCol(x,y-1,enPrioDashBoardText,0xA0404040,&buf[0]);
		msg_printfwithCol(x,y+1,enPrioDashBoardText,0xA0404040,&buf[0]);
		msg_printfwithCol(x,y,enPrioDashBoardText  ,0xA0808080,&buf[0]);
	}
	else
	{
		msg_printfwithCol(x+1,y,enPrioDashBoardText,0xf00000ff,&buf[0]);
		msg_printfwithCol(x-1,y,enPrioDashBoardText,0xf00000ff,&buf[0]);
		msg_printfwithCol(x,y-1,enPrioDashBoardText,0xf00000ff,&buf[0]);
		msg_printfwithCol(x,y+1,enPrioDashBoardText,0xf00000ff,&buf[0]);
		msg_printfwithCol(x,y,enPrioDashBoardText  ,0xF0F0F0F0,&buf[0]);
	}
}

void sysMenu::Sprite(gxSprite *p,Sint32 x,Sint32 y,Sint32 prio,Sint32 atr,unsigned long col,float scl,Sint32 rot)
{
	//--------------------------------------------
	//スクロールを考慮しないスプライトを描画する
	//--------------------------------------------

	PutSpriteDX(
		x,y,prio,
		p->page,p->u,p->v,p->w,p->h,
		//オプション
		p->cx,p->cy,
		atr,
		col,
		(float)rot,scl,scl
		);

}


void sysMenu::Sprite( int x,	int y, int prio,
                 int page, int u, int v, int w, int h, int cx, int cy,
                 int atr, unsigned long col,float rot, float scl_x,float scl_y )
{
	//--------------------------------------------
	//メニュー用のスプライトを描画する
	//--------------------------------------------

	PutSpriteDX(
		x,y,prio,
		page,u,v,w,h,
		//オプション
		cx,cy,
		atr, col, (float)rot,scl_x,scl_y	);

}


void sysMenu::Play( Sint32 n )
{
	//--------------------------------------------
	//メニュー用の効果音再生
	//--------------------------------------------

	ghLib::PlayWave( n , gxFalse , 90 );

}

