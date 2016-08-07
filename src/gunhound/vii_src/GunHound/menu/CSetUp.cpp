//------------------------------------------------------------
//
//　セットアップ
//　ファイルのノンブロック読み込み待ち
//　割と豪華なナウローディング
//------------------------------------------------------------
#include <gunvalken.h>
#include <gunhound/DashBoard/CDashBoard.h>
#include <gxLib/util/gxImage/CGXImage.h>
#include "../Cmenu.h"
#include "CSetUp.h"
#include "../../GunHound/GhStringTbl.h"

gxSprite SprBgWindow[]={
	{ enTexPageGrowObj,256,32,160,128,0,0},	//Window
};

gxSprite SprBg[]={
	{ enTexPageScreenShot,160*0,16+120*1,160,120,0,0 },//1
	{ enTexPageScreenShot,160*1,16+120*1,160,120,0,0 },//2
	{ enTexPageScreenShot,160*0,16+120*0,160,120,0,0 },//3
	{ enTexPageScreenShot,160*1,16+120*0,160,120,0,0 },//4
	{ enTexPageScreenShot,160*2,16+120*0,160,120,0,0 },//5
	{ enTexPageScreenShot,160*2,16+120*1,160,120,0,0 },//??
};


CSetUp::CSetUp(Sint32 sStage)
{
	m_Base.x = 32;
	m_Base.y = 32;
	m_Base.z = PRIO_FADE-5;

	m_NowLoading.x = WINDOW_W-18*8;
	m_NowLoading.y = WINDOW_H-16;

	m_bEnd = gxFalse;

	m_sFadeOut = 100;
	m_sWait = 8;
	m_sSeq = enSeqView;

	m_Add.x = 0;
	m_Add.y = 0;

	m_sStage = sStage;

//	//コックピット画面を生成する
//	CCockpit::GetInstance();


	m_bNowLoading = gxFalse;
	m_sFadeTimer = 0;

	m_sViewStage = 0;

	m_MissionPos.x = WINDOW_W;
	m_MissionPos.y = 0;
	m_MissionPos.z = PRIO_FADE+1;
}


CSetUp::~CSetUp()
{
	
}


void CSetUp::ReadScenario()
{

	switch( m_sStage ){
	case enScenarioJungle:
		CCockpit::GetInstance()->SetMessage(1000);
		break;

	case enScenarioRiver:
		CCockpit::GetInstance()->SetMessage(2000);
		break;

	case enScenarioMountain:
		CCockpit::GetInstance()->SetMessage(3000);
		break;

	case enScenarioBase:
		CCockpit::GetInstance()->SetMessage(4000);
		break;

	case enScenarioIronGear:
		CCockpit::GetInstance()->SetMessage(5000);
//		CCockpit::GetInstance()->SetMessage(5100);
//		CCockpit::GetInstance()->SetMessage(5200);
		break;
	case enScenarioExtra:
	case enScenarioExtra2:
	case enScenarioExtra3:
	default:
		CCockpit::GetInstance()->SetMessage(5000);
		break;
	}

}


void CSetUp::SetViewStage(Sint32 n)
{
	m_sViewStage = n;
	m_sStage     = n;
}


void CSetUp::SetSequence(Sint32 sSeq)
{
	m_sSeq = enSeqMain;
	
}


void CSetUp::Action()
{
	//---------------------------------------------
	//毎フレームの動作
	//---------------------------------------------

	if( CDashBoard::GetInstance()->IsMenuBar() )
	{
		return;
	}
	if( m_sSeq != enSeqView )
	{
		m_MissionPos.x += -m_MissionPos.x/10;
	}

	switch( m_sSeq ){
	case enSeqView:
		break;

	case enSeqMain:
		CDashBoard::GetInstance()->SetIcon(enIconNone);
		if( m_sFadeTimer >= 30 )
		{
			CFadeManager::GetInstance()->set_fadein(300);
			m_sFadeTimer = 0;
			m_sSeq = 50;
		}
		break;

	case 50:
		if( m_sFadeTimer >= 30 )
		{
			ReadScenario();
			m_sSeq = 100;
		}
		//MissionDisp();
		break;

	case 100:
		//ロード待ち
		
		if( CGXImage::GetInstance()->IsLoadFinish() )
		{
			m_MissionPos.z = PRIO_FADE-1;
			CDashBoard::GetInstance()->SetIcon(enIconDecision);
			m_sSeq = 150;
		}
		break;

	case 150:
		if( Joy[0].trg&enBtnDecision )
		{
			CFadeManager::GetInstance()->set_fadeout();
			viiMus::PlaySound( enSoundMenuDec );
			m_bNowLoading = gxTrue;
			m_sSeq = 200;
		}
		break;

	case 200:
		//フェード待ち
		if( !CFadeManager::GetInstance()->is_fade() )
		{
			m_sSeq = 999;
		}
		break;

	case 999:
		m_bEnd = gxTrue;
		break;
	}

	if( m_bNowLoading )
	{
		NowLoading();
	}

	m_sFadeTimer ++;
}


void CSetUp::NowLoading()
{


}

void CSetUp::MissionDisp()
{
	//-------------------------------------------
	//ミッション詳細
	//-------------------------------------------
	char **pMissionTbl;

	switch( m_sStage ){
	case enScenarioJungle:
		pMissionTbl = MissionInfoJungle;
		break;
	case enScenarioRiver:
		pMissionTbl = MissionInfoRiver;
		break;
	case enScenarioMountain:
		pMissionTbl = MissionInfoMountain;
		break;
	case enScenarioBase:
		pMissionTbl = MissionInfoBase;
		break;
	default:
	case enScenarioIronGear:
		pMissionTbl = MissionInfoExtra;
		break;
	}

	Sint32 x,y;

	x = 280+m_MissionPos.x;
	y = 32;

	viiDraw::sml_printf(x,y+16*0,0xfff0f0f0,"%s" , pMissionTbl[0] );
	viiDraw::sml_printf(x,y+16*1,0xfff0f0f0,"%s" , pMissionTbl[1] );
	viiDraw::sml_printf(x,y+16*2,0xfff0f0f0,"%s" , pMissionTbl[2] );
	viiDraw::sml_printf(x,y+16*3,0xfff0f0f0,"%s" , pMissionTbl[3] );
	viiDraw::sml_printf(x,y+16*4,0xfff0f0f0,"%s" , pMissionTbl[4] );
	viiDraw::sml_printf(x,y+16*5,0xfff0f0f0,"%s" , pMissionTbl[5] );
	viiDraw::sml_printf(x,y+16*6,0xfff0f0f0,"%s" , pMissionTbl[6] );
	viiDraw::sml_printf(x,y+16*7,0xfff0f0f0,"%s" , pMissionTbl[7] );
}



void CSetUp::Draw()
{
	//-------------------------------------------
	//NOW LOADING
	//-------------------------------------------
	if( m_bNowLoading )
	{
		NowLoadingDisp();
	}

	if( m_bEnd ) return;

	SetUpDisp();

	switch( m_sSeq ){
	case enSeqView:
		break;
	default:
		MissionDisp();
		break;
	}

}


void CSetUp::SetUpDisp()
{
	//-------------------------------------------
	//セットアップ
	//-------------------------------------------
	Sint32 x,y,z;

	x = m_Base.x;
	y = m_Base.y;
	z = m_Base.z+1;

	//viiSub::MenuSprite( &SprTitleScreen[2] , WINDOW_W/2,y+16*4,z+2 ,ATR_DFLT, ARGB(sCol,0xff,0xff,0xff) );

	if( m_sViewStage >= 0 )
	{
//		viiDraw::Sprite( &SprBg[m_sViewStage] , (WINDOW_W/2)*100,(WINDOW_H/2)*100, 0 ,ATR_DFLT,ARGB_DFLT );
		viiDraw::Sprite( &SprBg[m_sViewStage] , 16*100,16*100, m_MissionPos.z ,ATR_DFLT,ARGB_DFLT ,1.35f);
		viiDraw::Sprite( &SprBgWindow[0]      , 16*100,16*100, m_MissionPos.z ,ATR_DFLT,ARGB_DFLT ,1.35f);
		
	}

}


void CSetUp::NowLoadingDisp()
{
	//-------------------------------------------
	//NOW LOADING
	//-------------------------------------------
	gxSprite SprNowLoading[]={
		{ enTpgDashBoard,96,240,128,16,0,8},
	};

	CDashBoard::GetInstance()->SetIcon( enIconNone );

	Sint32 x,y,z;

	x = m_NowLoading.x;//m_Base.x;
	y = m_NowLoading.y;//m_Base.y;
	z = m_Base.z;

	sysMenu::Sprite( &SprNowLoading[0] , x , y , PRIO_FADEOVER );//enPrioDashBoardMain  );

}

gxBool CSetUp::IsEnd()
{
	return m_bEnd;
}
