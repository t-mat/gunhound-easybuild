//--------------------------------------------
// 
// ���C�����j���[
// written by ragi
//
//--------------------------------------------

#include <gunvalken.h>
#include "CMainMenu.h"
#include <gunhound/DashBoard/CDashBoard.h>
#include <gxLib/util/command/CWazaCommand.h>
#include "CCustomHound.h"
#include "../GhStringTbl.h"

#define NOUSE_OPTION

Uint32 uMutekiCommand[]={
	CWazaCommand::enKeyLeverNone,
	CWazaCommand::enKeyIncD,
	CWazaCommand::enKeyLeverNone,
	CWazaCommand::enKeyIncD,
	CWazaCommand::enKeyLeverNone,
	CWazaCommand::enKeyIncU,
	CWazaCommand::enKeyLeverNone,
	CWazaCommand::enKeyIncU,
	CWazaCommand::enKeyLeverNone,
	CWazaCommand::enKeyIncL,
	CWazaCommand::enKeyLeverNone,
	CWazaCommand::enKeyIncR,
	CWazaCommand::enKeyLeverNone,
	CWazaCommand::enKeyIncL,
	CWazaCommand::enKeyLeverNone,
	CWazaCommand::enKeyIncR,
	CWazaCommand::enKeyLeverNone,
	CWazaCommand::enKeyBtnA,
	CWazaCommand::enKeyLeverNone,
	CWazaCommand::enKeyBtnB,
	CWazaCommand::enKeyEnd,
};

static Sint32 sHiddenOpenTbl[]=
{
	//---------------------------
	//�B������I�[�v������
	//---------------------------
	enHiddenItemAddWeaponAirGun,
	enHiddenItemAddWeapon3Way,
	enHiddenItemAddWeaponGattling,
	enHiddenItemAddWeaponLinearCannon,
	enHiddenItemAddWeaponIronCrow,
	enHiddenItemStageSelect,
	enHiddenItemAddWeaponShotGun,
//	enHiddenItemAddWeaponZgokCrow,
	enHiddenItemAddWeaponGrenade,
	enHiddenItemAddWeaponShieldArm,
	enHiddenItemAddWeaponDiverAntenna,

	enHiddenItemAddWeaponNapalmGun,	//�I�[�v�����Ȃ�
};

enum {
	enTrainingMax = 2,
	enMissionMax  = 5,
	enAdTimer = 60*8,
	enScore1000Man = 10000000,
};

enum {
	enMenuStringTotalScore = 0,

	enMainMenuStringGameStart =0,
	enMainMenuStringTutorial,
	enMainMenuStringMissionMode,
	enMainMenuStringOption,
	enMainMenuStringDebug,
	enMainMenuStringExit,

	enLessonString1 =0,
	enLessonString2,
	enLessonString3,

	enMenuStringOptionConfig =0,
	enMenuStringOptionController,
	enMenuStringOptionHoundFactory,
	enMenuStringOptionStaffRoll,
	enMenuStringOptionNazo,

	enMenuStringCaptionTutorial = 0,
	enMenuStringCaptionMission,
	enMenuStringCaptionOption,
	enMenuStringCaptionFactory,

	enMenuStringMission1=0,
	enMenuStringMission2,
	enMenuStringMission3,
	enMenuStringMission4,
	enMenuStringMission5,
	enMenuStringMissionNazo,

	//�P������
	enHelpMissionMessage = 5,
	enHelpOptionMessage = 4,
};


gxSprite SprTitleScreen[]={
	{enTexPageTitle ,0,0,480,320,0,0},			//�w�i�P
	{enTexPageMainMenu,0,0,480,320,0,0},		//�w�i�Q
	{enTexPageSystemUI,0,0,128,24,64,10},		//PushStart

	{enTexPageMainMenu,0,0,64,64,0,0},			//�����ǎ�
	{enTexPageLineHound ,0,0,255,255,0,0},		//�t�@�N�g���[�w�i

	{enTexPageString,0,192,480,64,0,0 },		//�L���v�V����

	{enTexPageSystemUI,256+64*0,96+64*0,64,64,0,0 },		//�ǎ�
	{enTexPageSystemUI,256+64*1,96+64*0,64,64,0,0 },		//�ǎ�
	{enTexPageSystemUI,256+64*0,96+64*1,64,64,0,0 },		//�ǎ�
	{enTexPageSystemUI,256+64*1,96+64*1,64,64,0,0 },		//�ǎ�
};

gxSprite StSprClass[]={
	//�M�͂R���R�@���@�񓙕��A�ꓙ���A�R���A���сA���сA��сA�����A�����A�卲
	{ enTexPageSystemUI,432,32+24*0,80,24,40,12 },
	{ enTexPageSystemUI,432,32+24*1,80,24,40,12 },
	{ enTexPageSystemUI,432,32+24*2,80,24,40,12 },
	{ enTexPageSystemUI,432,32+24*3,80,24,40,12 },
	{ enTexPageSystemUI,432,32+24*4,80,24,40,12 },
	{ enTexPageSystemUI,432,32+24*5,80,24,40,12 },
	{ enTexPageSystemUI,432,32+24*6,80,24,40,12 },
	{ enTexPageSystemUI,432,32+24*7,80,24,40,12 },
	{ enTexPageSystemUI,432,32+24*8,80,24,40,12 },
};

gxSprite StSprScreenShot[]={
	//�X�N���[���V���b�g
	{ enTexPageScreenShot,160*0,16+120*1,160,120,0,0 },//1
	{ enTexPageScreenShot,160*1,16+120*1,160,120,0,0 },//2
	{ enTexPageScreenShot,160*0,16+120*0,160,120,0,0 },//3
	{ enTexPageScreenShot,160*1,16+120*0,160,120,0,0 },//4
	{ enTexPageScreenShot,160*2,16+120*0,160,120,0,0 },//5
	{ enTexPageScreenShot,160*2,16+120*1,160,120,0,0 },//??
};

extern gxSprite StSprStageClear[];

CMainMenu::CMainMenu(Sint32 sSelect)
{
	m_bEnd       = gxFalse;
	m_sNextScene = 0;
	m_sSelect    = sSelect;

	m_sTitleAlpha     = 100;
	m_sMainmenueAlpha = 100;

	m_sSequence = enMainSeqTitle;
	m_sTimer = 0;
	m_sColorTimer = 0;
	m_sCursorColor = 0;

	m_sTrainingSel = 0;
	m_sOptiongSel  = 0;
	m_sMissionSel  = 0;

	m_pAnnounce = NULL;
	m_bTitleThrough = gxFalse;
	m_sMenuForceSeq = 0;

	m_sCaption[0] = 1*100;
	m_sCaption[1] = 1*100;
	m_sCaption[2] = 1*100;
	m_sCaption[3] = 1*100;

	m_sMissionPosX = 128;

	m_pCustomHound = NULL;
	m_sFactoryAlpha = 0x00;
	m_fScreenShot = 480.f;

	m_sJoyTrig = 0;
	m_sJoyRep  = 0;

 	m_sAdTimer = enAdTimer;
	m_bSetDemoReady = gxFalse;

	for(Sint32 ii=0;ii<32 ; ii++)
	{
		m_Update[ii] = gxFalse;
	}
	m_sHiddenOpenCnt = 0;
	m_sHiddenOpenWait = 0;

	m_sCaptionBoard = 0;
	m_bCaption = gxFalse;

	m_bMutekiOn = gxFalse;

	m_pWazaCommand = new CWazaCommand( 60*180 );

	m_sDefaultStage = -1;
}


CMainMenu::~CMainMenu()
{
	if( m_pCustomHound )
	{
		delete m_pCustomHound;
		m_pCustomHound = NULL;
	}
	if( m_pWazaCommand )
	{
		m_pWazaCommand->Stop();
		delete m_pWazaCommand;
	}
}


void CMainMenu::Action()
{
	//-------------------------------------------
	//���C�����j���[�I��
	//-------------------------------------------
	Sint32 sTgt = -1;

	m_sJoyTrig = Joy[0].trg;
	m_sJoyRep  = Joy[0].rep;

	m_bJoyDecision = gxFalse;

	m_pWazaCommand->SetLog( Joy[0].psh );

	if( m_pWazaCommand->Check( uMutekiCommand , sizeof(uMutekiCommand)/sizeof(uMutekiCommand[0]) ,gxFalse ) )
	{
		m_bMutekiOn = gxTrue;
		viiMus::PlaySound( enSoundCrashB );
	}

	if( m_sJoyTrig&BTN_MENU_DECISION ) m_bJoyDecision = gxTrue;;

	if( CDashBoard::GetInstance()->IsMenuBar() )
	{
		return;
	}

	if( m_sAdTimer > 0 )
	{
		if( m_sJoyTrig || m_sJoyRep )
		{
			m_sAdTimer = enAdTimer;
		}
	}
	else
	{
		m_sAdTimer = 0;
		if( !m_bSetDemoReady )
		{
			m_sAdTimer = 60*2;	//�܂��ǂݍ��߂Ă��Ȃ���΂Q�b����
		}
		else
		{
			m_sJoyTrig = 0;
			m_sJoyRep  = 0;
		}
	}

	m_sTimer ++;
	m_sColorTimer ++;
	m_sCursorColor ++;

	//---------------------------------------
	//�ǎ��̃R���g���[��
	//---------------------------------------
	if( m_sSequence == enMainSeqTitle )
	{
		if( m_sTitleAlpha < 100 ) m_sTitleAlpha += 3;
	}
	else
	{
		if( m_sTitleAlpha > 0 ) m_sTitleAlpha -= 3;
	}
	if( m_sTitleAlpha <=0 )
	{
		m_sTitleAlpha = 0;
	}
	if( m_sTitleAlpha >=100 )
	{
		m_sTitleAlpha = 100;
	}


	if( m_sSequence <= enMainSeqMenuMain )
	{
		if( m_sMainmenueAlpha < 100 ) m_sMainmenueAlpha += 3;
	}
	else
	{
		if( m_sMainmenueAlpha > 0 ) m_sMainmenueAlpha -= 3;
	}
	if( m_sMainmenueAlpha <=0 )
	{
		m_sMainmenueAlpha = 0;
	}
	if( m_sMainmenueAlpha >=100 )
	{
		m_sMainmenueAlpha = 100;
	}

	//�H��p�A���t�@
	if( m_sSequence == enMainSeqOptionCustomize )
	{
		if( m_sFactoryAlpha < 100 ) m_sFactoryAlpha += 3;
	}
	else
	{
		if( m_sFactoryAlpha > 0 ) m_sFactoryAlpha -= 3;
	}
	if( m_sFactoryAlpha <=0 )
	{
		m_sFactoryAlpha = 0;
	}
	if( m_sFactoryAlpha >=100 )
	{
		m_sFactoryAlpha = 100;
	}


	//---------------------------------------

	switch( m_sSequence ){

	case enMainSeqTitle:
		m_bCaption = gxFalse;
		CDashBoard::GetInstance()->SetIcon( enIconNone );
//		m_sCaption[0] += (-480*100-m_sCaption[0])/5;
//		m_sCaption[1] += (-480*100-m_sCaption[1])/5;
//		m_sCaption[2] += (-480*100-m_sCaption[2])/5;
//		m_sCaption[3] += (-480*100-m_sCaption[3])/5;

		if( (m_sJoyTrig&(enBtnCancel|BTN_X|BTN_Y|BTN_L1|BTN_R1)) || m_bJoyDecision || m_bTitleThrough )
		{
			if( m_bTitleThrough ) 
			{
				//�^�C�g�����X���[����
				m_sTitleAlpha = 0;
				m_sMainmenueAlpha = 0;
				m_bTitleThrough = gxFalse;
				m_sSequence = m_sMenuForceSeq;
				if( m_sDefaultStage != -1 )
				{
					switch( m_sMenuForceSeq ){
					case enMainSeqMission:
						m_sMissionSel  = m_sDefaultStage;
						break;
					case enMainMenuTutorial:
						m_sTrainingSel = m_sDefaultStage;
						break;
					}
				}
			}
			else
			{
				m_sSelect = 0;
				viiMus::PlaySound( enSoundMenuDec );
				m_sSequence = enMainSeqMenuInit;
			}
		}
		else
		{
			if( m_sAdTimer > 0 ) m_sAdTimer --;	else m_sAdTimer = 0;
			if( m_sAdTimer == 0 && m_bSetDemoReady )
			{
				m_sSequence = enMainSeqFadeWaitGoNext;
				CFadeManager::GetInstance()->set_fadeout();
				break;
			}
		}
		break;

 	case enMainSeqMenuInit:
		m_bCaption = gxFalse;
		if( CheckScore() )
		{
			m_sSequence = enMainSeqMenuHiddenOpen;
		}
		else
		{
			m_sSequence = enMainSeqMenuMain;
		}
		break;

 	case enMainSeqMenuHiddenOpen:
		//--------------------------------------------------
		//�B���v�f�I�[�v���󋵂����m�点
		//--------------------------------------------------
		m_bCaption = gxFalse;
		if( HiddenOpen() )
		{
			m_sSequence = enMainSeqMenuMain;
		}
 		break;

 	case enMainSeqMenuMain:
		//--------------------------------------------------
		//���C���Z���N�^�[
		//--------------------------------------------------
		m_bCaption = gxFalse;
		CDashBoard::GetInstance()->SetIcon( enIconNone );
		m_sTrainingSel = 0;
		m_sOptiongSel  = 0;
		m_sMissionSel  = 0;
		MainMenu();
		break;

	case enMainSeqTraining:
		//--------------------------------------------------
		//�g���[�j���O
		//--------------------------------------------------
		m_bCaption = gxTrue;
		CDashBoard::GetInstance()->SetIcon( enIconDecision|enIconCancel );
		sTgt = 0;
		MainTraining();
		break;

	case enMainSeqMission:
		//--------------------------------------------------
		//�~�b�V�����Z���N�g
		//--------------------------------------------------
		m_bCaption = gxTrue;
		CDashBoard::GetInstance()->SetIcon( enIconDecision|enIconCancel );
		sTgt = 1;
		MainMission();
		break;

	case enMainSeqOption:
		//--------------------------------------------------
		//�I�v�V����
		//--------------------------------------------------
		m_bCaption = gxTrue;
		CDashBoard::GetInstance()->SetIcon( enIconDecision|enIconCancel );
		sTgt = 2;
		MainOption();
		break;

	case enMainSeqOptionEnvironment:
		sTgt = 2;
		DrawOptionEnvironment();
		break;

	case enMainSeqOptionController:
		sTgt = 2;
		DrawOptionController();
		break;

	case enMainSeqOptionCustomize:
		sTgt = 3;
		EditMode();
		break;

	case enMainSeqFadeWaitGoNext:
		//------------------------------------------------
		//�Q�[���֐i�s����
		//------------------------------------------------
		if( !CFadeManager::GetInstance()->is_fade() )
		{
			m_bEnd = gxTrue;
			if( m_sAdTimer == 0 && m_bSetDemoReady )
			{
				//�f���ֈڍs����
				m_sSelect = enMainMenuOpening;
			}
			m_sNextScene = enMenuSeqNext;
		}
		break;
	}

	//----------------------------------------------------
	//�L���v�V�����̐���
	//----------------------------------------------------
	if( !m_bCaption )
	{
		Sint32 sAdd = (0-m_sCaptionBoard)/5;
		if( sAdd > -2 ) sAdd = -2;
		m_sCaptionBoard += sAdd;
		if( m_sCaptionBoard < 0 ) m_sCaptionBoard =  0;
	}
	else
	{
		Sint32 sAdd = (100-m_sCaptionBoard)/5;
		if( sAdd < 2 ) sAdd = 2;
		m_sCaptionBoard += sAdd;
		if( m_sCaptionBoard > 100 ) m_sCaptionBoard =  100;
	}

	for(Sint32 ii=0;ii<enCaptionMax;ii++)
	{
		if( ii == sTgt && m_bCaption )
		{
			Sint32 sAdd = (0-m_sCaption[ii])/5;
			if( sAdd > -2 ) sAdd = -2;

			m_sCaption[ii] += (0-m_sCaption[ii])/5;
			if( m_sCaption[ii] < 0 ) m_sCaption[ii] =  0;

//			m_sCaption[ii] += (0-m_sCaption[ii])/5;
		}
		else
		{
			Sint32 sAdd = (100-m_sCaption[ii])/5;
			if( sAdd < 2 ) sAdd = 2;
			m_sCaption[ii] += sAdd;
			if( m_sCaption[ii] > 100 ) m_sCaption[ii] =  100;
//			m_sCaption[ii] += (-480*100-m_sCaption[ii])/5;
		}
	}

	if( sTgt == -1 )
	{
		m_sMissionPosX = 128;
	}

}


void CMainMenu::MainMenu()
{
	//---------------------------------------
	//���C�����j���|
	//---------------------------------------

	m_sColorTimer = 0;

	if( m_sTitleAlpha <=0 )
	{
		m_sTitleAlpha = 0;
		if( m_sJoyRep&JOY_U )
		{
			viiMus::PlaySound( enSoundMenuSel );
			m_sSelect --;
			m_sSelect = (m_sSelect+enMainMenuMax)%enMainMenuMax;
			m_sCursorColor = 0;
		}
		else if( m_sJoyRep&JOY_D )
		{
			viiMus::PlaySound( enSoundMenuSel );
			m_sSelect ++;
			m_sSelect = (m_sSelect+enMainMenuMax)%enMainMenuMax;
			m_sCursorColor = 0;
		}
		else if( m_bJoyDecision ) 
		{
			viiMus::PlaySound( enSoundMenuDec );
			switch( m_sSelect ){
			case enMainMenuMission:
				m_fScreenShot = 480.f;
				m_sSequence = enMainSeqMission;
				break;
			case enMainMenuOption:
				m_sSequence = enMainSeqOption;
				break;
			case enMainMenuTutorial:
				m_sSequence = enMainSeqTraining;
				break;
			//case enMainMenuStart:
			default:
				m_sSequence = enMainSeqFadeWaitGoNext;
				CFadeManager::GetInstance()->set_fadeout();
				break;
			}
		}
		else if(m_sJoyTrig&enBtnCancel )
		{
			viiMus::PlaySound( enSoundMenuCan );
			m_sSequence = enMainSeqTitle;
		}
	}

	m_pAnnounce = HelpMessage[m_sSelect];

	if( m_pAnnounce )
	{
		CDashBoard::GetInstance()->SetAnnounce(m_pAnnounce);
	}
}


void CMainMenu::MainTraining()
{
	//---------------------------------------
	//���C���g���[�j���O
	//---------------------------------------

	Sint32 Max = enTrainingMax;

/*
#ifdef _TRIAL_VERSION_
	Max = 1;
#endif
*/

	if( m_sJoyRep&JOY_U )
	{
		viiMus::PlaySound( enSoundMenuSel );
		m_sTrainingSel --;
		m_sTrainingSel = (m_sTrainingSel+Max)%Max;
	}
	else if( m_sJoyRep&JOY_D )
	{
		viiMus::PlaySound( enSoundMenuSel );
		m_sTrainingSel ++;
		m_sTrainingSel = (m_sTrainingSel+Max)%Max;
	}
	else if( m_bJoyDecision ) 
	{
		viiMus::PlaySound( enSoundMenuDec );

		m_sMissionSel = m_sTrainingSel;
		m_sSequence = enMainSeqFadeWaitGoNext;
		CFadeManager::GetInstance()->set_fadeout();

	}
	else if(m_sJoyTrig&enBtnCancel )
	{
		viiMus::PlaySound( enSoundMenuCan );
		m_sSequence = enMainSeqMenuInit;
	}

	m_pAnnounce = HelpTutorialMessage[m_sTrainingSel];
	if( m_pAnnounce )
	{
		CDashBoard::GetInstance()->SetAnnounce(m_pAnnounce);
	}
}


void CMainMenu::MainMission()
{
	//--------------------------------------
	//�~�b�V�������[�h
	//--------------------------------------
	Sint32 Max = enMissionMax;

#ifdef _TRIAL_VERSION_
	Max = 1;
#endif

	if( m_sJoyRep&JOY_U )
	{
		viiMus::PlaySound( enSoundMenuSel );
		m_sMissionSel --;
		m_sMissionSel = (m_sMissionSel+Max)%Max;
		m_fScreenShot = 480.f;
	}
	else if( m_sJoyRep&JOY_D )
	{
		viiMus::PlaySound( enSoundMenuSel );
		m_sMissionSel ++;
		m_sMissionSel = (m_sMissionSel+Max)%Max;
		m_fScreenShot = 480.f;
	}
	else if( m_bJoyDecision ) 
	{
		if( g_CHoundSaveData.IsHiddenOpen( enHiddenItemMission1 + m_sMissionSel ) )
		{
			viiMus::PlaySound( enSoundMenuDec );
			m_sSequence = enMainSeqFadeWaitGoNext;
			CFadeManager::GetInstance()->set_fadeout();
		}
		else
		{
			viiMus::PlaySound( enSoundMenuCan );
		}
	}
	else if(m_sJoyTrig&enBtnCancel )
	{
		viiMus::PlaySound( enSoundMenuCan );
		m_sSequence = enMainSeqMenuInit;
	}

	m_fScreenShot += -m_fScreenShot/5.f;

	if( g_CHoundSaveData.IsHiddenOpen( enHiddenItemMission1 + m_sMissionSel ) )
	{
		m_pAnnounce = HelpMissionMessage[m_sMissionSel];
	}
	else
	{
		m_pAnnounce = HelpMissionMessage[enHelpMissionMessage];
	}

	if( m_pAnnounce )
	{
		CDashBoard::GetInstance()->SetAnnounce(m_pAnnounce);
	}
}


void CMainMenu::MainOption()
{
	//--------------------------------------
	//�I�v�V����
	//--------------------------------------
	Sint32 Max = 4;

#ifdef _TRIAL_VERSION_
	Max = 3;
#endif

	if( m_sJoyRep&JOY_U )
	{
		viiMus::PlaySound( enSoundMenuSel );
		m_sOptiongSel --;
		m_sOptiongSel = (m_sOptiongSel+Max)%Max;
	}
	else if( m_sJoyRep&JOY_D )
	{
		viiMus::PlaySound( enSoundMenuSel );
		m_sOptiongSel ++;
		m_sOptiongSel = (m_sOptiongSel+Max)%Max;
	}
	else if( m_bJoyDecision ) 
	{
		switch( m_sOptiongSel ){
		case enMenuStringOptionConfig:
#ifdef NOUSE_OPTION
				viiMus::PlaySound( enSoundMenuCan );
#else
			CDashBoard::GetInstance()->SetForceMenu(1);
			m_sSequence = enMainSeqOptionEnvironment;
#endif
			break;
		case enMenuStringOptionController:
#ifdef NOUSE_OPTION
				viiMus::PlaySound( enSoundMenuCan );
#else
			CDashBoard::GetInstance()->SetForceMenu(2);
			m_sSequence = enMainSeqOptionController;
#endif
			break;
		case enMenuStringOptionHoundFactory:
			m_sSequence = enMainSeqOptionCustomize;
			break;

		case enMenuStringOptionStaffRoll:	//�G���f�B���O��
			if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemViewEnding ) )
			{
				viiMus::PlaySound( enSoundMenuCan );
				return;
			}
			m_sSequence = enMainSeqFadeWaitGoNext;
			CFadeManager::GetInstance()->set_fadeout();
			m_sSelect = enMainMenuEnding;
			break;

		default:
			m_sSequence = enMainSeqFadeWaitGoNext;
			CFadeManager::GetInstance()->set_fadeout();
			break;
		}
		viiMus::PlaySound( enSoundMenuDec );
	}
	else if(m_sJoyTrig&enBtnCancel )
	{
		viiMus::PlaySound( enSoundMenuCan );
		m_sSequence = enMainSeqMenuInit;
	}

	m_pAnnounce = HelpOptionMessage[m_sOptiongSel];

#ifdef NOUSE_OPTION
	//���̃��C�u�����Ƃ��܂�����Ȃ��̂�

	if( m_sOptiongSel == 0 || m_sOptiongSel == 1 )
	{
		m_pAnnounce = HelpOptionMessage[ 5 ];	//"���̃o�[�W�����ł͂��g�p�ɂȂ�܂���",
	}
#endif

	if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemViewEnding ) && m_sOptiongSel == enMenuStringOptionStaffRoll )
	{
		//�ŏI�X�e�[�W���N���A����ƃI�[�v�����܂�
		m_pAnnounce = HelpOptionMessage[enHelpOptionMessage];
	}

	if( m_pAnnounce )
	{
		CDashBoard::GetInstance()->SetAnnounce(m_pAnnounce);
	}

}


void CMainMenu::EditMode()
{
	//--------------------------------------
	//�G�f�B�b�g���[�h
	//--------------------------------------
	Sint32 Max = 4;

	if( m_pCustomHound == NULL )
	{
		m_pCustomHound = new CCustomHound();
	}

	if( m_pCustomHound == NULL )
	{
		return;
	}

	m_pCustomHound->Action();

	if( m_pCustomHound->IsDead() )
	{
		m_pCustomHound->Clear();
		delete m_pCustomHound;
		m_pCustomHound = NULL;

		m_sSequence = enMainSeqOption;
	}
	else
	{
		
	}


}



void CMainMenu::DrawOptionController()
{
	if( !CDashBoard::GetInstance()->IsForceMenu() )
	{
		m_sSequence = enMainSeqOption;
	}
}


void CMainMenu::DrawOptionEnvironment()
{

	if( !CDashBoard::GetInstance()->IsForceMenu() )
	{
		m_sSequence = enMainSeqOption;
	}
}


void CMainMenu::Draw()
{
	//-------------------------------------------
	//���C�����j���[�\��
	//-------------------------------------------

	switch( m_sSequence ){
	case enMainSeqTitle:
	case enMainSeqMenuMain:
		DrawMainMenu();
		break;

	case enMainSeqMenuHiddenOpen:
		DrawHiddenOpen();
		break;

	case enMainSeqTraining:
		DrawTraining();
		break;

	case enMainSeqMission:
		DrawMission();
		break;

	case enMainSeqOption:
		DrawOption();
		break;

	case enMainSeqOptionCustomize:
		if( m_pCustomHound )
		{
			m_pCustomHound->Draw();
		}
		break;
	}

	//---------------------
	//�w�i�`��
	//---------------------

	Sint32 col1 = 0xFF*m_sTitleAlpha/100;
	Sint32 col2 = 0xFF*m_sMainmenueAlpha/100;
	Sint32 z = 32;

	//�^�C�g���p�w�i�i��������j
	//viiSub::MenuSprite( &SprTitleScreen[0] , 0,0,z+2 ,ATR_DFLT, ARGB(col1,0xff,0xff,0xff) , 0 , 1.0f , 1.025f );
	viiSub::MenuSprite(0, 0, z + 2, SprTitleScreen[0].page, SprTitleScreen[0].u, SprTitleScreen[0].v, SprTitleScreen[0].w, SprTitleScreen[0].h, SprTitleScreen[0].cx, SprTitleScreen[0].cy,ATR_DFLT, ARGB(col1, 0xff, 0xff, 0xff), 0, 1.0f, 1.025f);

	//���j���[�p�w�i�i�{�P�j
	//viiSub::MenuSprite( &SprTitleScreen[1] , 0,0,z+1 , ATR_DFLT , ARGB(col2,0xff,0xff,0xff) , 0 , 1.0f , 1.025f );
	viiSub::MenuSprite(0, 0, z + 1, SprTitleScreen[1].page, SprTitleScreen[1].u, SprTitleScreen[1].v, SprTitleScreen[1].w, SprTitleScreen[1].h, SprTitleScreen[1].cx, SprTitleScreen[1].cy, ATR_DFLT, ARGB(col2, 0xff, 0xff, 0xff), 0, 1.0f, 1.025f);

	//�L���v�V�����p�̑�
	//if( m_bCaption )
	{
		viiSub::MenuSprite( &SprTitleScreen[5] , 0,-8-100+ 100*m_sCaptionBoard/100 ,z+2  ,ATR_DFLT, ARGB(0xff,0xff,0xff,0xff) );
//		viiSub::MenuSprite( &SprTitleScreen[5] , WINDOW_W,WINDOW_H+108-100*m_sCaptionBoard/100 ,z+2  ,ATR_DFLT, ARGB(0xff,0xff,0xff,0xff) ,1.f , 180.f);
	}

	//�L���v�V����
	viiDraw::big_printf(-512*m_sCaption[0]/100+32,12, ARGB(0xc0, 46,194, 255) , CaptionNameTbl[enMenuStringCaptionTutorial] );	//"TUTORIAL"
	viiDraw::big_printf(-512*m_sCaption[1]/100+32,12, ARGB(0xc0, 46,194, 255) , CaptionNameTbl[enMenuStringCaptionMission]  );	//"MISSION" 
	viiDraw::big_printf(-512*m_sCaption[2]/100+32,12, ARGB(0xc0, 46,194, 255) , CaptionNameTbl[enMenuStringCaptionOption]   );	//"OPTION"  
	viiDraw::big_printf(-512*m_sCaption[3]/100+32,12, ARGB(0xc0, 46,194, 255) , CaptionNameTbl[enMenuStringCaptionFactory]  );	//"FACTORY" 

	

	//�ݒ�p�w�i
	Sint32 xx = viiSub::GameCounter()%64;

	for(Sint32 y=-64;y<WINDOW_H+64;y+=64)
	{
		for(Sint32 x=-64;x<WINDOW_W+64;x+=64)
		{
			viiSub::MenuSprite( &SprTitleScreen[6+2] , x+xx,y+xx,z+0  ,ATR_DFLT, ARGB(0xff,0xff,0xff,0xff) );
		}
	}

	//�t�@�N�g���[�p�摜
	if( m_sFactoryAlpha )
	{
		Sint32 sAlpha = 255*m_sFactoryAlpha/100;
		viiSub::MenuSprite( &SprTitleScreen[4] , 0,0,z+1 , ATR_DFLT , ARGB(sAlpha,0xff,0xff,0xff)  , WINDOW_W/255.f );
	}
}


void CMainMenu::DrawMainMenu()
{
	//-------------------------------------------
	//���C�����j���[�\��
	//-------------------------------------------
	ghPos base;
	Sint32 x,y,z;

	base.x = 32;
	base.y = 32;
	base.z = 32;

	x = base.x;
	y = base.y;
	z = base.z;

	if( m_sTitleAlpha == 0 )
	{
		//�A���t�@�̒������I�������\�����s��

		Sint32 n=0;
		Sint32 sCol;
		Sint32 sColSelect;
		Sint32 sColDefault = ARGB_DFLT;

		x = base.x+128;
		y = base.y+80;

		sColSelect  = viiSub::GetMorphingColor( 50+(45*viiMath::Cos100((m_sCursorColor*6)%360))/100 ,0xf0f0f000,0xf000f000);

		if(enMainMenuStart  < enMainMenuMax)
		{
			sCol = ( m_sSelect == n )? sColSelect : sColDefault;
			viiDraw::printf(x,y+16*n,sCol,MainMenuNameTbl[enMainMenuStringGameStart] );	//"  Game Start"
			n ++;
		}
		if(enMainMenuTutorial < enMainMenuMax)
		{
			sCol = ( m_sSelect == n )? sColSelect : sColDefault;
			viiDraw::printf(x,y+16*n,sCol,MainMenuNameTbl[enMainMenuStringTutorial] );	//"  Tutorial"
			n ++;
		}
		if(enMainMenuMission< enMainMenuMax)
		{
			sCol = ( m_sSelect == n )? sColSelect : sColDefault;
			viiDraw::printf(x,y+16*n,sCol,MainMenuNameTbl[enMainMenuStringMissionMode] );	//"  Mission Mode"
			n ++;
		}
		if(enMainMenuOption < enMainMenuMax)
		{
			sCol = ( m_sSelect == n )? sColSelect : sColDefault;
			viiDraw::printf(x,y+16*n,sCol,MainMenuNameTbl[enMainMenuStringOption] );//"  Option"
			n ++;
		}
		if(enMainMenuExit  < enMainMenuMax)
		{
			sCol = ( m_sSelect == n )? sColSelect : sColDefault;
			viiDraw::printf(x,y+16*n,sCol,MainMenuNameTbl[enMainMenuStringExit] );	//"  Exit"
			n ++;
		}
		if(enMainMenuDebug  < enMainMenuMax)
		{
			sCol = ( m_sSelect == n )? sColSelect : sColDefault;
			viiDraw::printf(x,y+16*n,sCol,MainMenuNameTbl[enMainMenuStringDebug] );	//"  Debug Mode"
			n ++;
		}

		DrawSprSaveData();
	}
	else if( m_sTitleAlpha == 100 )
	{
		//Push Button �\��
		x = base.x+146;
		y = base.y+96;
		Sint32 sCol;

		sCol = (m_sColorTimer*8)%360;
		sCol = 240-64+64*viiMath::Cos100(sCol)/100;

		viiSub::MenuSprite( &SprTitleScreen[2] , WINDOW_W/2,y+16*4,z+10 ,ATR_DFLT, ARGB(sCol,0xff,0xff,0xff) );
	}

	Sint32 sAlpha = (m_sTitleAlpha)*0xff/100;
	viiDraw::sml_printf( 150,WINDOW_H-16 , ARGB( sAlpha, 0xff , 0xff, 0xff ) ,"2016 GARURU CO.LTD. ALL RIGHTS RESERVED." );

}


void CMainMenu::DrawTraining()
{
	//-------------------------------------------
	//�g���[�j���O���j���[�\��
	//-------------------------------------------
	ghPos base;
	Sint32 x,y,z;

	m_sMissionPosX += (32-m_sMissionPosX)/10;
	base.x = m_sMissionPosX;
	base.y = 32;
	base.z = 32;

	x = base.x;
	y = base.y;
	z = base.z;

	Sint32 n=0;
	Sint32 sCol;
	Sint32 sColSelect;

	x = base.x+32;
	y = base.y+80;

	sColSelect = viiSub::GetMorphingColor( 50+(45*viiMath::Cos100((m_sCursorColor*6)%360))/100 ,0xf0f0f000,0xf000f000);

	viiDraw::Box( x-16, y-8, x+128 , y+64-8 , 128 , gxTrue , 0x40108030  , ATR_ALPHA_MINUS );

	sCol = ( m_sTrainingSel == n )? sColSelect : ARGB_DFLT;
	viiDraw::printf(x,y+16*n,sCol, LessonMenuNameTbl[enLessonString1] );	//"Lesson 1",
	n ++;

	sCol = ( m_sTrainingSel == n )? sColSelect : ARGB_DFLT;
	viiDraw::printf(x,y+16*n,sCol,LessonMenuNameTbl[enLessonString2] );	//"Lesson 2",
	n ++;

//	sCol = ( m_sTrainingSel == n )? sColSelect : ARGB_DFLT;
//	viiDraw::printf(x,y+16*n,sCol,LessonMenuNameTbl[enLessonString3] );	//"Lesson 3",
//	n ++;

}

void CMainMenu::DrawMission()
{
	//-------------------------------------------
	//�~�b�V�������j���[�\��
	//-------------------------------------------
	ghPos base;
	Sint32 x,y,z;
	Sint32 sNameIndex = 0;

	m_sMissionPosX += (16-m_sMissionPosX)/10;
	base.x = m_sMissionPosX;
	base.y = 48;
	base.z = 32;

	x = base.x;
	y = base.y;
	z = base.z;

	Sint32 n=0;
	Sint32 sCol;
	Sint32 sColSelect;

	x = base.x+16;
	y = base.y+48;

	sColSelect = viiSub::GetMorphingColor( 50+(45*viiMath::Cos100((m_sCursorColor*6)%360))/100 ,0xf0f0f000,0xf000f000);

	viiDraw::Box( x-16, y-8, x+180 , y+128-8 , 128 , gxTrue , 0x40108030  , ATR_ALPHA_MINUS );

	sCol = ( m_sMissionSel == n )? sColSelect : ARGB_DFLT;
	sNameIndex = ( g_CHoundSaveData.IsHiddenOpen( enHiddenItemMission1 ) )? enMenuStringMission1 : enMenuStringMissionNazo; 
	viiDraw::printf(x,y+24*n,sCol,"%s",MissionNameTbl[sNameIndex]);
	n ++;

	sCol = ( m_sMissionSel == n )? sColSelect : ARGB_DFLT;
	sNameIndex = ( g_CHoundSaveData.IsHiddenOpen( enHiddenItemMission2 ) )? enMenuStringMission2 : enMenuStringMissionNazo; 
	viiDraw::printf(x,y+24*n,sCol,"%s",MissionNameTbl[sNameIndex]);
	n ++;

	sCol = ( m_sMissionSel == n )? sColSelect : ARGB_DFLT;
	sNameIndex = ( g_CHoundSaveData.IsHiddenOpen( enHiddenItemMission3 ) )? enMenuStringMission3 : enMenuStringMissionNazo; 
	viiDraw::printf(x,y+24*n,sCol,"%s",MissionNameTbl[sNameIndex]);
	n ++;

	sCol = ( m_sMissionSel == n )? sColSelect : ARGB_DFLT;
	sNameIndex = ( g_CHoundSaveData.IsHiddenOpen( enHiddenItemMission4 ) )? enMenuStringMission4 : enMenuStringMissionNazo; 
	viiDraw::printf(x,y+24*n,sCol,"%s",MissionNameTbl[sNameIndex]);
	n ++;

	sCol = ( m_sMissionSel == n )? sColSelect : ARGB_DFLT;
	sNameIndex = ( g_CHoundSaveData.IsHiddenOpen( enHiddenItemMission5 ) )? enMenuStringMission5 : enMenuStringMissionNazo; 
	viiDraw::printf(x,y+24*n,sCol,"%s",MissionNameTbl[sNameIndex]);
	n ++;

	//----------------------------------
	//�X�N���[���V���b�g�̕\��
	//----------------------------------

	Sint32 page = m_sMissionSel;

	if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemMission1+m_sMissionSel ) )
	{
		page = enMenuStringMissionNazo;
	}

	viiSub::MenuSprite( &StSprScreenShot[page] , (Sint32)(256+m_fScreenShot) , 78 , 64 ,ATR_DFLT , 0xc0ffffff ,1.2f );

}


void CMainMenu::DrawHiddenOpen()
{
	//-------------------------------------
	//���i���b�Z�[�W
	//-------------------------------------

	Sint32 x,y,z;
	char *p;

	if( m_sHiddenOpenWait > 0 )
	{
		return;
	}

	//---------------------------------------

	x = WINDOW_W/2-(strlen(HiddenOpenMessageTbl[ m_sHiddenOpenCnt])*12)/4;
	y = WINDOW_H/2-4;
	z = 200;

	//viiDraw::Box( 0, y-8, WINDOW_W , y+8+48 , 128 , gxTrue , 0x401080a0  , ATR_DFLT );
	viiSub::MenuSprite( &StSprStageClear[0]  , WINDOW_W/2,y+12 , PRIO_BLACKTONE ,ATR_DFLT,0xf0ffffff);

	//�`�ɏ��i���܂���

	p = HiddenOpenMessageTbl[ m_sHiddenOpenCnt];

	viiSub::MenuPrintf( x-1 , y , 0x7f808000,"%s",p);
	viiSub::MenuPrintf( x+1 , y , 0x7f808000,"%s",p);
	viiSub::MenuPrintf( x , y-1 , 0x7f808000,"%s",p);
	viiSub::MenuPrintf( x , y+1 , 0x7f808000,"%s",p);
	viiSub::MenuPrintf( x , y , 0xfff0f000,"%s",p);


	//����x���R�����g

	y += 22;

	//---------------------------------------

	p = GetWeaponNameByHiddenItem(  sHiddenOpenTbl[m_sHiddenOpenCnt] );

	if( p == NULL )
	{
		//�`���ǉ�����܂���

		//NULL
	}
	else
	{
		//�`���x������܂���
		x = WINDOW_W/2-( (strlen(p)+18)*12)/4;

		viiSub::MenuPrintf( x-1 , y   , 0x7f40a0a0,"�u%s�v%s", p , WeaponSupply[0]);
		viiSub::MenuPrintf( x+1 , y   , 0x7f40a0a0,"�u%s�v%s", p , WeaponSupply[0]);
		viiSub::MenuPrintf( x   , y-1 , 0x7f40a0a0,"�u%s�v%s", p , WeaponSupply[0]);
		viiSub::MenuPrintf( x   , y+1 , 0x7f40a0a0,"�u%s�v%s", p , WeaponSupply[0]);
		viiSub::MenuPrintf( x   , y   , 0x7ff0f0f0,"�u%s�v%s", p , WeaponSupply[0] );
	}

	//-----------------------------------------

	x = 280;
	y += 32;

	switch( sHiddenOpenTbl[m_sHiddenOpenCnt] ){
	case enHiddenItemStageSelect:
		//> �V�i���I���[�h�ŃX�e�[�W�I�����\�ɂȂ�܂��B
		viiSub::MenuPrintf( x-96   , y-28   , 0xF0f0f0f0,"%s",WeaponSupply[2]);
		break;
	case enHiddenItemAddWeaponDiverAntenna:
		//> ���@�ɃA�N�Z�T���Ƃ��đ�������܂��B
		viiSub::MenuPrintf( x-64   , y-28   , 0xF0f0f0f0,"%s",WeaponSupply[3]);
		break;
	default:
		//> HOUND FACTORY�ő����ł��܂��B
		viiSub::MenuPrintf( x   , y-4   , 0xF0f0f0f0,"%s",WeaponSupply[1]);
		break;
	}

}


void CMainMenu::DrawOption()
{
	//-------------------------------------------
	//�I�v�V�����\��
	//-------------------------------------------
	ghPos base;
	Sint32 x,y,z;

	m_sMissionPosX += (32-m_sMissionPosX)/10;
	base.x = m_sMissionPosX;
	base.y = 32;
	base.z = 32;

	x = base.x;
	y = base.y;
	z = base.z;

	Sint32 n=0;
	Sint32 sCol;
	Sint32 sColSelect;

	x = base.x+32;
	y = base.y+80;

	viiDraw::Box( x-16, y-8, x+192 , y+72 , 128 , gxTrue , 0x40108030  , ATR_ALPHA_MINUS );

	sColSelect = viiSub::GetMorphingColor( 50+(45*viiMath::Cos100((m_sCursorColor*6)%360))/100 ,0xf0f0f000,0xf000f000);

#ifdef NOUSE_OPTION
	sCol = ( m_sOptiongSel == n )? sColSelect : 0xff808080;
#else
	sCol = ( m_sOptiongSel == n )? sColSelect : ARGB_DFLT;
#endif

	viiDraw::printf(x,y+16*n,sCol,OptionMenuNameTbl[enMenuStringOptionConfig] );	//"CONFIG"
	n ++;

#ifdef NOUSE_OPTION
	sCol = ( m_sOptiongSel == n )? sColSelect : 0xff808080;
#else
	sCol = ( m_sOptiongSel == n )? sColSelect : ARGB_DFLT;
#endif

	viiDraw::printf(x,y+16*n,sCol,OptionMenuNameTbl[enMenuStringOptionController]);	//"CONTROLLER"
	n ++;

	sCol = ( m_sOptiongSel == n )? sColSelect : ARGB_DFLT;
	viiDraw::printf(x,y+16*n,sCol,OptionMenuNameTbl[enMenuStringOptionHoundFactory]);	//"HOUND FACTORY",
	n ++;

	sCol = ( m_sOptiongSel == n )? sColSelect : ARGB_DFLT;
	if( g_CHoundSaveData.IsHiddenOpen( enHiddenItemViewEnding ) )
	{
		//�I�[�v�����Ă����ꍇ
		viiDraw::printf(x,y+16*n,sCol,OptionMenuNameTbl[enMenuStringOptionStaffRoll]);	//"STAFF ROLL",
	}
	else
	{
		//�B�����
		viiDraw::printf(x,y+16*n,sCol,OptionMenuNameTbl[enMenuStringOptionNazo]);		//"??????????",
	}
	n ++;

}


void CMainMenu::DrawSprSaveData()
{
	//-------------------------------------------
	//�Z�[�u�f�[�^
	//-------------------------------------------
	Sint32 x=48,y=24;

	//StSprClass
	Uint32 uLv = g_CHoundSaveData.m_uLevel;
	Uint32 uSc = g_CHoundSaveData.m_uTotalScore;

	//�N���X�\��
	viiSub::MenuSprite( &StSprClass[uLv] , x,y,64 ,ATR_DFLT );

	//�g�[�^���X�R�A�\��
	viiDraw::sml_printf(x+64,y-4,0xfff0f0f0,"%s %d" , MainMenuEtcNameTbl[enMenuStringTotalScore] , uSc );

}


gxBool CMainMenu::CheckScore()
{
	//---------------------------------
	//�A�b�v�f�[�g���`�F�b�N
	//---------------------------------

	gxBool bUpdate = gxFalse;

	//TEST
/*
	g_CHoundSaveData.m_uTotalScore = HIGH_SCORE_MAX;
	g_CHoundSaveData.m_sHidden[0] = 0x00000000;
	g_CHoundSaveData.m_sHidden[1] = 0x00000000;
	g_CHoundSaveData.m_sHidden[2] = 0x00000000;
	g_CHoundSaveData.m_sHidden[3] = 0x00000000;
	g_CHoundSaveData.m_uLevel = 0;
*/
	for(Sint32 ii=0;ii<32 ; ii++)
	{
		m_Update[ii] = gxFalse;
	}
	m_sHiddenOpenCnt = 0;

	//�T�O�O���_
	if( g_CHoundSaveData.m_uTotalScore >= enScore1000Man/2 )
	{
		//���Q
		if( g_CHoundSaveData.m_uLevel < enLevelNumGunso )
		{
			g_CHoundSaveData.m_uLevel = enLevelNumGunso;
			m_Update[0] = gxTrue;
			bUpdate = gxTrue;
		}
	}

	//�P�O�O�O���_
	if( g_CHoundSaveData.m_uTotalScore >= enScore1000Man*1 )
	{
		//���R
		if( g_CHoundSaveData.m_uLevel < enLevelNumJokyuGunso )
		{
			g_CHoundSaveData.m_uLevel = enLevelNumJokyuGunso;
			m_Update[1] = gxTrue;
			bUpdate = gxTrue;
		}
	}

	//�R�O�O�O���_
	if( g_CHoundSaveData.m_uTotalScore >= enScore1000Man*3 )
	{
		//��P
		if( g_CHoundSaveData.m_uLevel < enLevelNumSyoui )
		{
			g_CHoundSaveData.m_uLevel = enLevelNumSyoui;
			m_Update[2] = gxTrue;
			bUpdate = gxTrue;
		}
	}

	//�U�O�O�O���_
	if( g_CHoundSaveData.m_uTotalScore >= enScore1000Man*6 )
	{
		//��Q
		if( g_CHoundSaveData.m_uLevel < enLevelNumChui )
		{
			g_CHoundSaveData.m_uLevel = enLevelNumChui;
			m_Update[3] = gxTrue;
			bUpdate = gxTrue;
		}

	}

	//�P���_
	if( g_CHoundSaveData.m_uTotalScore >= enScore1000Man*10 )
	{
		if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemAddWeaponNapalmGun ) )
		{
			m_Update[10] = gxTrue;
			bUpdate = gxTrue;
		}
	}

	//�P���Q�O�O�O���_
	if( g_CHoundSaveData.m_uTotalScore >= enScore1000Man*12 )
	{
		//��R
		if( g_CHoundSaveData.m_uLevel < enLevelNumTaii )
		{
			g_CHoundSaveData.m_uLevel = enLevelNumTaii;
			m_Update[4] = gxTrue;
			bUpdate = gxTrue;
		}
	}

	//�P���W�O�O�O���_
	if( g_CHoundSaveData.m_uTotalScore >= enScore1000Man*18 )
	{
		if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemStageSelect ) )
		{
			m_Update[5] = gxTrue;
			bUpdate = gxTrue;
		}
		if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemAddWeaponNapalmGun ) )
		{
			m_Update[10] = gxTrue;
			bUpdate = gxTrue;
		}
	}

	//�Q���T�O�O�O���_
	if( g_CHoundSaveData.m_uTotalScore >= enScore1000Man*25 )
	{
		//���P
		if( g_CHoundSaveData.m_uLevel < enLevelNumSyosa )
		{
			g_CHoundSaveData.m_uLevel = enLevelNumSyosa;
			m_Update[6] = gxTrue;
			bUpdate = gxTrue;
		}

	}

	//�R���T�O�O�O���_
	if( g_CHoundSaveData.m_uTotalScore >= enScore1000Man*35 )
	{
		//���Q
		if( g_CHoundSaveData.m_uLevel < enLevelNumChusa )
		{
			g_CHoundSaveData.m_uLevel = enLevelNumChusa;
			m_Update[7] = gxTrue;
			bUpdate = gxTrue;
		}
		if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemAddWeaponGrenade ) )
		{
			m_Update[7] = gxTrue;
			bUpdate = gxTrue;
		}
	}

	//�T���O�O�O�O�_
	if( g_CHoundSaveData.m_uTotalScore >= enScore1000Man*50 )
	{
		//���R
		if( g_CHoundSaveData.m_uLevel < enLevelNumTaisa )
		{
			g_CHoundSaveData.m_uLevel = enLevelNumTaisa;
			m_Update[8] = gxTrue;
			bUpdate = gxTrue;
		}

	}

	if( g_CHoundSaveData.m_uTotalScore >= HIGH_SCORE_MAX )
	{
		//�J���X�g
		if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemAddWeaponDiverAntenna ) )
		{
			m_Update[9] = gxTrue;
			bUpdate = gxTrue;
		}
	}

#ifdef _TRIAL_VERSION_
	//--------------------------------------
	//�̌��ł����̓��ʃI�[�v������
	//--------------------------------------
	if( g_CHoundSaveData.m_uTotalScore >= 100000 )
	{
		if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemAddWeaponNapalmGun ) )
		{
			m_Update[10] = gxTrue;
			bUpdate = gxTrue;
		}
	}
	if( g_CHoundSaveData.m_uTotalScore >= 10000 )
	{
		if( !g_CHoundSaveData.IsHiddenOpen( enHiddenItemAddWeaponGattling ) )
		{
			m_Update[2] = gxTrue;
			bUpdate = gxTrue;
		}
	}



#endif


	m_sHiddenOpenWait = 8;

//	g_CHoundSaveData.m_uLevel = 0;
	if(bUpdate)
	{
		viiMus::PlaySound( enSoundMenuOpen );
	}

	return bUpdate;
}


gxBool CMainMenu::HiddenOpen()
{
	//---------------------------------
	//�B���󋵂��`�F�b�N
	//---------------------------------
	if( m_sHiddenOpenWait > 0 )
	{
		m_sHiddenOpenWait --;
	}

	if( m_sHiddenOpenWait == 0 && (m_sJoyTrig&enBtnDecision || m_sJoyTrig&enBtnCancel ) )
	{
		m_sHiddenOpenCnt ++;
		if( m_sHiddenOpenCnt >= 32 )
		{
			viiMus::PlaySound( enSoundMenuDec );
			return gxTrue;
		}
	}

	if( m_Update[m_sHiddenOpenCnt] == gxFalse)
	{
		while( m_sHiddenOpenCnt < 32 )
		{
			if( m_Update[m_sHiddenOpenCnt] )
			{
				viiMus::PlaySound( enSoundMenuOpen );
				return gxFalse;
			}
			m_sHiddenOpenCnt ++;
		}

		return gxTrue;
	}
	else
	{
		if( m_sHiddenOpenCnt < ARRAY_LENGTH(sHiddenOpenTbl) )
		{
			Sint32 n = sHiddenOpenTbl[m_sHiddenOpenCnt];

			if( !g_CHoundSaveData.IsHiddenOpen( n ) )
			{
				g_CHoundSaveData.SetHiddenOpen( n );
			}
		}
	}

	return gxFalse;

}


char* CMainMenu::GetWeaponNameByHiddenItem( Sint32 n )
{
	//-------------------------------------------------
	//���햼���擾
	//-------------------------------------------------

	switch(n){
	case enHiddenItemAddWeaponAirGun:
		return WeaponNameTbl[enWeaponCannonGun];

	case enHiddenItemAddWeapon3Way:
		return WeaponNameTbl[enWeapon3Way];

	case enHiddenItemAddWeaponGattling:
		return WeaponNameTbl[enWeaponArmGattling];

	case enHiddenItemAddWeaponLinearCannon:
		return WeaponNameTbl[enWeaponCannonLinar];

	case enHiddenItemAddWeaponIronCrow:
		return WeaponNameTbl[enWeaponArmStrikeCrow];

	case enHiddenItemAddWeaponShotGun:
		return WeaponNameTbl[enWeaponShotGun];

	case enHiddenItemAddWeaponZgokCrow:
		return WeaponNameTbl[enWeaponArmZgokCrow];

	case enHiddenItemAddWeaponNapalmGun:
		return WeaponNameTbl[enWeaponNapalm];

	case enHiddenItemAddWeaponGrenade:
		return WeaponNameTbl[enWeaponGrenade];

	case enHiddenItemStageSelect:
		return NULL;

	case enHiddenItemAddWeaponDiverAntenna:
		return NULL;

	default:
	case enHiddenItemAddWeaponShieldArm:
		return WeaponNameTbl[enWeaponArmShieldArm];
	}
}


