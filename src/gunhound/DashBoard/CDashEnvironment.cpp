//--------------------------------------------
//
// 環境
//
//--------------------------------------------

#include <gunhound/gga2.h>
#include <gunhound/gga2x.h>
#include "CDashBoard.h"
#include "CDashEnvironment.h"

static char *StrDashEnvScreen[]={
	"１倍",
	"２倍",
	"フルスクリーン",
};

static char *StrDashEnvWide[]={
	"4:3(ノーマル)",
	"16:9(ワイド)",
};

static char *StrDashEnvFrame[]={
	"高性能PC向け（60fps）",
	"普通のPC向け（30fps）",
	"古いPC向け（15fps）",
	"決定",
};

static char *StrDashEnvMenu[]={
	"画面サイズ",
	"ワイド設定",
	"動作環境設定",
	"決定",
};

static Sint32 EnvSelectMax[]={
	3,2,3,1,
};

static Sint32 EnvSelectScreenSize[]={
	1,2,4,
};

static Sint32 EnvSelectScreenSizeR[]={
	0,0,1,0,2
};

static Sint32 EnvSelectFrameSkipTbl[]={
	0,1,3
};

static Sint32 EnvSelectFrameSkipRTbl[]={
	0,//	0,
	1,//	1,
	0,//	2,
	2,//	3
};

CDashEnvironment::CDashEnvironment()
{
	//--------------------------------------------
	// 環境設定
	//--------------------------------------------
	for(Sint32 ii=0;ii<enEnvSelectMax;ii++)
	{
		m_sSelect[ii] = 0;
	}
	m_sCursor = enEnvSelectSize;

	m_sSelect[0] = EnvSelectScreenSizeR[ g_StSaveData.windowSize ];
	m_sSelect[1] = g_StSaveData.wideScreen;
	m_sSelect[2] = EnvSelectFrameSkipRTbl[g_StSaveData.FrameSkip];

	m_bEnd = gxFalse;
	m_bDecision = gxFalse;

}


CDashEnvironment::~CDashEnvironment()
{
	if( m_bDecision )
	{
		g_StSaveData.windowSize = EnvSelectScreenSize[ m_sSelect[0] ];
		g_StSaveData.wideScreen = m_sSelect[1];
		g_StSaveData.FrameSkip  = EnvSelectFrameSkipTbl[ m_sSelect[2] ];
		ghLib::SaveConfig();
	}

}


gxBool CDashEnvironment::IsEnd()
{
	return m_bEnd;
}


void CDashEnvironment::Action()
{
	//--------------------------------------------
	// 
	//--------------------------------------------
	if(Joy[0].rep&JOY_U)
	{
		sysMenu::Play( enSoundMenuSel );
		m_sCursor --;
		m_sCursor = (m_sCursor+enEnvSelectMax)%enEnvSelectMax;
	}
	else if(Joy[0].rep&JOY_D)
	{
		sysMenu::Play( enSoundMenuSel );
		m_sCursor ++;
		m_sCursor = (m_sCursor+enEnvSelectMax)%enEnvSelectMax;
	}
	else if(Joy[0].rep&JOY_L)
	{
		Sint32 sMax = EnvSelectMax[m_sCursor];
		m_sSelect[m_sCursor] --;
		m_sSelect[m_sCursor] = ( m_sSelect[m_sCursor]+sMax )%sMax;
		sysMenu::Play( enSoundMenuSel );
	}
	else if(Joy[0].rep&JOY_R)
	{
		Sint32 sMax = EnvSelectMax[m_sCursor];
		m_sSelect[m_sCursor] ++;
		m_sSelect[m_sCursor] = ( m_sSelect[m_sCursor]+sMax )%sMax;
		sysMenu::Play( enSoundMenuSel );
	}
	if(Joy[0].trg&enBtnMenuDecision)
	{
		if( enEnvSelectDec == m_sCursor )
		{
			m_bDecision = gxTrue;
			m_bEnd = gxTrue;
			sysMenu::Play( enSoundMenuDec );
		}
	}
	else if(Joy[0].trg&enBtnMenuCancel)
	{
		m_bEnd = gxTrue;
		sysMenu::Play( enSoundMenuCan );
	}


	switch(m_sCursor){
	case enEnvSelectSize:
		CDashBoard::GetInstance()->SetAnnounce( "起動時の画面サイズを設定します" );
		break;
	case enEnvSelectWide:
		CDashBoard::GetInstance()->SetAnnounce( "ディスプレイにあわせた表示方法を選択してください" );
		break;
	case enEnvSelectFrame:
		CDashBoard::GetInstance()->SetAnnounce( "ゲームの速度が遅い場合に調整してください" );
		break;
	case enEnvSelectDec:
		CDashBoard::GetInstance()->SetAnnounce( "設定を保存します。" );
		break;
	}
}


void CDashEnvironment::Draw()
{
	//--------------------------------------------
	// 環境設定
	//--------------------------------------------

	Sint32 x,y,z,sAdd;
	Sint32 sHaba = 128;
	x = 32;
	y = 112;
	z = 250;

	sAdd = 0;

	sysMenu::Printf( 296,212,ARGB_DFLT, "設定は再起動後有効になります");

	sysMenu::Selectf( (m_sCursor == sAdd)? 1 : 0, x,y+sAdd*16    , StrDashEnvMenu[0]);
	sysMenu::Selectf( (m_sCursor == sAdd)? 1 : 0, x+sHaba,y+sAdd*16 , StrDashEnvScreen[ m_sSelect[sAdd] ]);
	sAdd ++;

	sysMenu::Selectf( (m_sCursor == sAdd)? 1 : 0,x,y+sAdd*16    , StrDashEnvMenu[1] );
	sysMenu::Selectf( (m_sCursor == sAdd)? 1 : 0,x+sHaba,y+sAdd*16 , StrDashEnvWide[ m_sSelect[sAdd] ]);
	sAdd ++;

	sysMenu::Selectf( (m_sCursor == sAdd)? 1 : 0,x,y+sAdd*16    , StrDashEnvMenu[2] );
	sysMenu::Selectf( (m_sCursor == sAdd)? 1 : 0,x+sHaba,y+sAdd*16 , StrDashEnvFrame[ m_sSelect[sAdd] ]);
	sAdd ++;

	sysMenu::Selectf( (m_sCursor == sAdd)? 1 : 0,x,y+sAdd*16    , StrDashEnvMenu[3] );
	sAdd ++;

}


