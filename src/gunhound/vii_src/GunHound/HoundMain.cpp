//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//
//�@�n�E���h�F�F�Q�[�����C�����[�v
//�@written by ragi.
//
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
#include <gunvalken.h>
#include <gxLib/Util/gxImage/CGXImage.h>
#include <gunhound/DashBoard/CDashBoard.h>
#include "Cmenu.h"
#include "Menu/CMainMenu.h"
#include "Menu/COpeningDemo.h"
#include "Menu/CBriefing.h"
#include "Menu/CSetUp.h"
#include "Menu/CEndingDemo.h"
#include "Menu/CContinue.h"

#define TPG_LOGO (64)

CHoundSaveData g_CHoundSaveData;

class CGameGunHound
{
	//-----------------------------------------------
	//�V���O���g���̃Q�[���}�l�[�W��
	//-----------------------------------------------

	enum {
		enMainSeqInit,

		enMainSeqStart,
		enMainSeqRestart,
		enMainSeqDebugMenu,

		//�I�[�v�j���O�f��
		enMainSeqGameOpeningDemo,

		//�I�[�v�j���O�^�C�g��
		enMainSeqGameTitle,

		//���C�����j���[
		enMainSeqGameMainMenu,
		enMainSeqGameBriefing,
		enMainSeqGameControlPanel,

		enMainSeqGameSetUp,
		enMainSeqGameBefore,
		enMainSeqGameInit,
		enMainSeqGameMain,
		enMainSeqGameEnd,

		enMainSeqGameContinue,

		enMainSeqResetInGame,

		enMainSeqGameEndingDemo,

		enMainSeqEnd,
	};

	typedef struct StEndingBonus
	{
		Uint32 uAllScore;
		Uint32 uContinueBonus;
		Uint32 uTrueEndBonus;
		Uint32 uNoContinueBonus;
		Uint32 uTotalScore;

		StEndingBonus()
		{
			Reset();
		}

		void Reset()
		{
			uAllScore = 0;
			uContinueBonus   = 0;
			uTrueEndBonus    = 0;
			uNoContinueBonus = 0;
			uTotalScore      = 0;
		}

	}StEndingBonus;

public:

	CGameGunHound();
	~CGameGunHound();

	void Loop();
	void GameInit();
	void GameMain();
	void GameEnd();

	gxBool IsPause()
	{
		//�|�[�Y���Ă܂����H
		return  m_bPauseFlag;
	}

	void SetReset()
	{
		m_bResetPlease = gxTrue;
	}
	gxBool IsExit()
	{
		return m_bExitMainSequence;
	}

private:

	void ChangeGameSeq( Sint32 n )
	{
		m_GameSeq = n;
	}

	void StageSet();

	Sint32 DebugMenu();
	Sint32 MainMenu();

	gxBool BriefingMain();
	gxBool GameSetUp();
	Sint32 AdvertiseDemo();
	gxBool EndingDemo();
	Sint32 ContinueScreen();

	void CockpitControl();
	void SlowMode(Sint32 n=2);
	void ResetClass();

	void ExitGame()
	{
		m_bExitMainSequence = gxTrue;
	}

	Sint32 m_GameSeq;

	//�I�[�v�j���O�p

	Sint32 GetDemoStage();
	Sint32 m_sOpeningSeq;
	Sint32 m_sOpenTimer;

	//�e�q���j���[�p

	CMainMenu    *m_pMainMenu;
	CBriefing    *m_pBriefing;
	CSetUp       *m_pSetUp;
	COpeningDemo *m_pOpeningDemo;
	CEndingDemo  *m_pEndingDemo;
	CContinue    *m_pContinue;
	Sint32 m_sMainMenuSelected;
	Sint32 m_sSelectedStage;		//�I�����ꂽ�X�e�[�W��Ԃ�
	Sint32 m_sEndSeq;
	Sint32 m_sDemoCount;
	Sint32 m_sResetCount;
	StEndingBonus m_stEndingBonus;
	gxBool m_bPauseFlag;
	gxBool m_bResetOK;
	gxBool m_bResetPlease;
	gxBool m_bBriefingRoot;
	gxBool m_bGameStart;
	gxBool m_bSuperMutekiMode;
	gxBool m_bFirstSeq;	//�������Ԃ��Ες��Ă͂����Ȃ��̂��Ǘ�����t���O
	gxBool m_bExitMainSequence;
};


static CGameGunHound *g_pCGameGunHound = NULL;
StGameInfo g_StGameInfo;

extern Sint32 gtimer_ex;


gxBool GunHoundGameMain(Sint32 cmd)
{
	//------------------------------------------------
	//�K���n�E���h���C�����[�v
	//------------------------------------------------

	if( g_pCGameGunHound == NULL )
	{
		g_pCGameGunHound = new CGameGunHound();
	}

	if( cmd == -1 )
	{
		//���Z�b�g�R�}���h
		g_pCGameGunHound->SetReset();
	}

	g_pCGameGunHound->Loop();

	if( g_pCGameGunHound->IsExit() )
	{
		delete g_pCGameGunHound;
		g_pCGameGunHound = NULL;
		return gxFalse;
	}

	gtimer_ex ++;

	return gxTrue;
}


//------------------------------------------------------


CGameGunHound::CGameGunHound()
{
	//-----------------------------------------
	//�n�E���h������
	//-----------------------------------------
	g_CHoundSaveData.Load();

	g_CHoundSaveData.SetHiddenOpen( enHiddenItemMission1 );

	//�f�t�H�����Ƃ��ăI�[�v�����Ă���
//	g_CHoundSaveData.SetHiddenOpen( enHiddenItemAddWeaponGattling );
	g_CHoundSaveData.SetHiddenOpen( enHiddenItemAddWeaponZgokCrow );

//#ifdef _VII_DEBUG_
//	g_CHoundSaveData.AllHiddenOpen();
//#endif

	m_GameSeq = enMainSeqInit;
	m_sEndSeq = 0;

	m_pMainMenu    = NULL;
	m_pBriefing    = NULL;
	m_pSetUp       = NULL;
	m_pOpeningDemo = NULL;
	m_pEndingDemo  = NULL;
	m_pContinue    = NULL;
	m_bSuperMutekiMode = gxFalse;

	m_bPauseFlag = gxFalse;

	m_sMainMenuSelected = 0;
	m_sSelectedStage    = 0;

	m_bResetOK     = gxFalse;
	m_bResetPlease = gxFalse;

	m_sOpenTimer = 0;

	m_bBriefingRoot = gxFalse;

	m_bGameStart = gxFalse;

	CCockpit::GetInstance();

	m_sDemoCount = 0;

	m_sResetCount = 0;
	m_bFirstSeq = gxFalse;
	m_bExitMainSequence = gxFalse;
}


CGameGunHound::~CGameGunHound()
{
	//-----------------------------------------
	//�n�E���h�I��
	//-----------------------------------------

	g_CHoundSaveData.Save();

	//���C�����j���[���[�`���̎c�����m�F
	ResetClass();

//	if( m_pBriefing )    delete m_pBriefing;
	if( m_pOpeningDemo ) delete m_pOpeningDemo;
	if( m_pEndingDemo  ) delete m_pEndingDemo;

	if(pGame)			 delete pGame;

	if(CFadeManager::GetInstance())
	{
		CFadeManager::GetInstance()->Destroy();
	}

	CActManager::DeleteInstance();
/*
//	if(g_pActManager)
//	{
//		delete g_pActManager;
//		g_pActManager = NULL;
//	}
*/
}


void CGameGunHound::ResetClass()
{
	if( m_pMainMenu )    delete m_pMainMenu;
	m_pMainMenu = NULL;

	if( m_pSetUp    )    delete m_pSetUp;
	m_pSetUp = NULL;

	if( m_pBriefing )
	{
		delete m_pBriefing;
		m_pBriefing = NULL;
	}
	if( m_pContinue )
	{
		delete m_pContinue;
		m_pContinue = NULL;
	}

	m_bGameStart = gxFalse;

	CCockpit::GetInstance()->Destroy();
	m_bSuperMutekiMode = gxFalse;
	m_stEndingBonus.Reset();
}

void CGameGunHound::Loop()
{
	//-----------------------------------------
	//�n�E���h���C��
	//-----------------------------------------

	if( m_bResetOK )
	{
		if( m_bResetPlease )
		{
			m_bResetPlease = gxFalse;
			m_sEndSeq = -1;
			ChangeGameSeq( enMainSeqGameEnd );
		}
		else if( CDashBoard::GetInstance()->IsMenuBar() )
		{
			m_bPauseFlag = gxTrue;
		}
		else
		{
			m_bPauseFlag = gxFalse;
		}

	}


	switch( m_GameSeq ){
	case enMainSeqInit:
		//-----------------------------------------
		//�Q�[��������
		//-----------------------------------------
		m_bBriefingRoot = gxTrue;
		m_bResetOK      = gxFalse;
		viiDbg::log("�n�E���h�V�[�P���X�FenMainSeqInit");
		//SaveDataInit();
		//GameConfigLoad();
		m_bPauseFlag = gxFalse;
		ChangeGameSeq( enMainSeqStart );
		break;

	case enMainSeqStart:
		//-----------------------------------------
		//�X�R�A������
		//-----------------------------------------
		m_bResetOK = gxFalse;
		viiDbg::log("�n�E���h�V�[�P���X�FenMainSeqStart");

#ifdef _VII_DEBUG_
		CDashBoard::GetInstance()->SetMenuDisable(gxFalse);
		CDashBoard::GetInstance()->SetLogo(gxFalse);
		CDashBoard::GetInstance()->SetWallPaper(gxFalse);
		ChangeGameSeq( enMainSeqRestart );
#else
//		ChangeGameSeq( enMainSeqGameOpeningDemo );
		CDashBoard::GetInstance()->SetMenuDisable(gxFalse);
		CDashBoard::GetInstance()->SetLogo(gxFalse);
		CDashBoard::GetInstance()->SetWallPaper(gxFalse);
		ChangeGameSeq( enMainSeqGameTitle );
#endif
		break;

	case enMainSeqGameTitle:
		//-----------------------------------------
		//�^�C�g���̃V�[�P���X
		//-----------------------------------------
		UploadTexture();
		m_bResetOK = gxFalse;
		ChangeGameSeq( enMainSeqRestart );
		break;

	case enMainSeqRestart:
		//-----------------------------------------
		//���X�^�[�g�̃|�C���g
		//-----------------------------------------
		m_bResetOK = gxFalse;

		ResetClass();

#ifdef GX_DEBUG
		ChangeGameSeq( enMainSeqDebugMenu );
#else
		ChangeGameSeq( enMainSeqGameMainMenu );
#endif
		break;

	case enMainSeqGameMainMenu:
		//-----------------------------------------
		//���C�����j���[�̃V�[�P���X
		//-----------------------------------------
		m_bResetOK = gxTrue;
		CDashBoard::GetInstance()->SetMenuDisable(gxTrue);
		switch( MainMenu() ){
		case enMenuSeqBack:
		case enMenuSeqNext:
			g_StGameInfo.Reset();
			if( m_sMainMenuSelected == CMainMenu::enMainMenuOpening )
			{
				//�I�[�v�j���O��
				ChangeGameSeq( enMainSeqGameOpeningDemo );
			}
			else if( m_sMainMenuSelected == CMainMenu::enMainMenuDebug )
			{
				//�f�o�b�O���[�h��
				g_StGameInfo.m_sGameMode = StGameInfo::enGameModeNormal;
				ChangeGameSeq( enMainSeqDebugMenu );
			}
			else if( m_sMainMenuSelected == CMainMenu::enMainMenuEnding )
			{
				//�G���f�B���O
				CDashBoard::GetInstance()->SetIcon( enIconNone );
				ChangeGameSeq( enMainSeqGameEndingDemo );
			}
			else if( m_sMainMenuSelected == CMainMenu::enMainMenuStart )
			{
				//�Q�[���X�^�[�g��
				CDashBoard::GetInstance()->SetIcon( enIconNone );
				g_StGameInfo.m_sGameMode = StGameInfo::enGameModeNormal;
				g_StGameInfo.m_bMessage = gxTrue;
				ChangeGameSeq( enMainSeqGameBriefing );
			}
			else if( m_sMainMenuSelected == CMainMenu::enMainMenuTutorial )
			{
				//�`���[�g���A���X�^�[�g��
				g_StGameInfo.m_bMusic   = gxTrue;
				g_StGameInfo.m_bMessage = gxTrue;
				g_StGameInfo.m_bReplay  = gxFalse;
				CDashBoard::GetInstance()->SetIcon( enIconNone );
				g_StGameInfo.m_sGameMode = StGameInfo::enGameModeNormal;
				g_StGameInfo.PlayStage = enScenarioTutorial;
				StageSet();
				g_StGameInfo.PlayArea  = m_sSelectedStage;
				ChangeGameSeq( enMainSeqGameInit );
			}
			else if( m_sMainMenuSelected == CMainMenu::enMainMenuMission )
			{
				//�Q�[���X�^�[�g��
				CDashBoard::GetInstance()->SetIcon( enIconNone );
				g_StGameInfo.m_sGameMode = StGameInfo::enGameModeMission;
				g_StGameInfo.PlayStage   = m_sSelectedStage;//enScenarioTutorial;
				g_StGameInfo.PlayArea = 0;

				StageSet();

				ChangeGameSeq( enMainSeqGameInit );

			}
			else if( m_sMainMenuSelected == CMainMenu::enMainMenuExit )
			{
				//�Q�[���I��
				ExitGame();
			}
			break;

		default:
			break;
		}
		break;

	case enMainSeqGameOpeningDemo:
		//-----------------------------------------
		//�I�[�v�j���O�̃V�[�P���X
		//-----------------------------------------
		m_bResetOK = gxFalse;
		if( AdvertiseDemo()  )
		{
			ChangeGameSeq( enMainSeqGameTitle );
		}
		break;

	case enMainSeqDebugMenu:
		//-----------------------------------------
		//�f�o�b�O���j���[
		//-----------------------------------------
		m_bResetOK = gxTrue;
		switch( DebugMenu() ) {
		case enMenuSeqBack:
			ChangeGameSeq( enMainSeqGameMainMenu );
			break;
		case enMenuSeqNext:
//			g_StGameInfo.Reset();
			g_StGameInfo.m_bMessage = gxTrue;
			g_StGameInfo.m_bMusic   = gxTrue;
			g_StGameInfo.m_bReplay  = gxFalse;
			ChangeGameSeq( enMainSeqGameInit );
			break;
		}
		break;

	case enMainSeqGameBriefing:
		g_StGameInfo.ContinueArea = 0;

		CCockpit::GetInstance()->SetHidden();
		CDashBoard::GetInstance()->SetMenuDisable(gxFalse);
		m_bResetOK = gxTrue;

		if( m_pSetUp == NULL )
		{
			//�Z�b�g�A�b�v��ʂ����
			m_pSetUp = new CSetUp( -1 );//g_StGameInfo.PlayStage );
		}

		if( BriefingMain() )
		{
			//�X�e�[�W����
			StageSet();
			m_bBriefingRoot = gxTrue;
			ChangeGameSeq( enMainSeqGameSetUp );
		}

		CockpitControl();
		break;

	case enMainSeqGameSetUp:
		CCockpit::GetInstance()->SetHidden();
		m_bResetOK = gxTrue;

		if( m_pSetUp->IsEnd() )
		{
			ChangeGameSeq( enMainSeqGameBefore );
		}

		CockpitControl();
		break;

	case enMainSeqGameBefore:
		//-----------------------------------------
		//�Q�[��������
		//-----------------------------------------
		CCockpit::GetInstance()->ReNew();
		CCockpit::GetInstance()->SetHidden();

		CockpitControl();

		m_bResetOK = gxTrue;

		if( !m_pSetUp->IsWait() )
		{
			//CCockpit::GetInstance()->Reset();
			viiMus::StopBGM( enSoundBgm1 );
			ChangeGameSeq( enMainSeqGameInit );
		}

		break;

	case enMainSeqGameInit:
		//-----------------------------------------
		//�Q�[��������
		//-----------------------------------------
		CCockpit::GetInstance()->Destroy();
		CCockpit::GetInstance()->SetHidden();
		m_bResetOK = gxFalse;

		if( pGame ) delete pGame;
		pGame = new CGameManager();

		GameInit();

		pGame->init();

		CockpitControl();

		viiMus::Destroy();
		ChangeGameSeq( enMainSeqGameMain );
		break;

	case enMainSeqGameMain:
		//-----------------------------------------
		//�Q�[�����C��
		//-----------------------------------------
		CDashBoard::GetInstance()->SetMenuDisable(gxFalse);
		m_bResetOK = gxTrue;

		if( m_pSetUp )
		{
			m_bResetOK = gxFalse;

			m_pSetUp->Draw();

			if( m_pSetUp->GoDestroy() )
			{
				//----------------------------------------------
				//�Z�b�g�A�b�v�I��
				//----------------------------------------------
				CCockpit::GetInstance()->Reset();
				delete m_pSetUp;
				m_pSetUp = NULL;
			}
		}

		if( !m_bGameStart && pGame->IsGameStart() )
		{
			m_bGameStart = gxTrue;
		}

		GameMain();

		if( pGame->IsStageClear() )
		{
			m_sEndSeq = pGame->GetNextSeq();	//m_sEndSeq = 1;	//����
			ChangeGameSeq( enMainSeqGameEnd );
		}
		else if( CCockpit::GetInstance()->IsGameOver() )
		{
			m_sEndSeq = enClearSeqContinue;
			ChangeGameSeq( enMainSeqGameEnd );
		}

		break;

	case enMainSeqGameEnd:
		//-----------------------------------------
		//�Q�[���I��
		//-----------------------------------------
		m_bResetOK = gxFalse;

		GameEnd();

		if( pGame )
		{
			delete pGame;
			pGame = NULL;
		}

		if( !m_bBriefingRoot )
		{
			m_sEndSeq = 0;
		}

		CCockpit::GetInstance()->ReNew();

		if( m_sMainMenuSelected == CMainMenu::enMainMenuMission || m_sMainMenuSelected == CMainMenu::enMainMenuTutorial)
		{
			//---------------------------------------
			//�X�g�[���[���[�h�ȊO�̎�
			//---------------------------------------

			//�X�R�A�����Z����
			g_CHoundSaveData.UpdateHighScore( g_StGameInfo.m_uAllScore );

			//�~�b�V�������[�h������
			ChangeGameSeq( enMainSeqGameMainMenu );

			//�`���[�g���A�����[�h������
			ChangeGameSeq( enMainSeqGameMainMenu );

		}
		else if( m_sEndSeq == enClearSeqStageClear )
		{
#ifdef _TRIAL_VERSION_
			//�^�C�g���ɖ߂�
			g_CHoundSaveData.UpdateHighScore( g_StGameInfo.m_uAllScore );
			ChangeGameSeq( enMainSeqRestart );
#else
			//�X�e�[�W�N���A������
			g_StGameInfo.SetStageClear( g_StGameInfo.PlayStage );
			switch( g_StGameInfo.PlayStage ){
			case enScenarioJungle:
				g_CHoundSaveData.SetHiddenOpen( enHiddenItemMission2 );
				break;
			case enScenarioRiver:
				g_CHoundSaveData.SetHiddenOpen( enHiddenItemMission3 );
				break;
			case enScenarioMountain:
				g_CHoundSaveData.SetHiddenOpen( enHiddenItemMission4 );
				break;
			case enScenarioBase:
				g_CHoundSaveData.SetHiddenOpen( enHiddenItemMission5 );
				break;
			}

			ChangeGameSeq( enMainSeqGameBriefing );
#endif
		}
		else if( m_sEndSeq == enClearSeqContinue )
		{
			//�X�e�[�W���s�������i�R���e�B�j���[���[�g�j
			ChangeGameSeq( enMainSeqGameContinue );
		}
		else if( m_sEndSeq == enClearSeqGameOver )
		{
			//�Q�[���I�[�o�[������
			ChangeGameSeq( enMainSeqGameContinue );
		}
		else if( m_sEndSeq == enClearSeqEnding )
		{
			//�G���f�B���O������
			g_StGameInfo.SetStageClear( g_StGameInfo.PlayStage );
			g_CHoundSaveData.UpdateHighScore( MISSION_ALLOVER_BONUS );

			Uint32 uScore = 0;

			//�Q�[���X�R�A�����Z
			m_stEndingBonus.uAllScore = g_StGameInfo.m_uAllScore;

			//�I�[���I�[�o�[�{�[�i�X���R���e�B�j���[�񐔂Ŋ���
			m_stEndingBonus.uContinueBonus = MISSION_ALLOVER_BONUS/(g_StGameInfo.m_sContinueCnt+1);
			m_stEndingBonus.uContinueBonus = m_stEndingBonus.uContinueBonus/10;
			m_stEndingBonus.uContinueBonus = m_stEndingBonus.uContinueBonus*10;


			//True�G���f�B���O�̏ꍇ�ꍇ�{�[�i�X
			m_stEndingBonus.uTrueEndBonus = 0;
			if( g_StGameInfo.m_bTrueEnd )
			{
				m_stEndingBonus.uTrueEndBonus = ENDING_TRUEVERSION_BONUS;
				g_CHoundSaveData.SetHiddenOpen( enHiddenItemViewEnding );
			}

			//�m�[�R���e�B�j���[�������ꍇ�Q�{�{�[�i�X
			if( g_StGameInfo.m_sContinueCnt == 0 )
			{
				m_stEndingBonus.uNoContinueBonus = m_stEndingBonus.uAllScore + m_stEndingBonus.uContinueBonus + m_stEndingBonus.uTrueEndBonus;
			}

			uScore =  m_stEndingBonus.uAllScore;
			uScore += m_stEndingBonus.uContinueBonus;
			uScore += m_stEndingBonus.uTrueEndBonus;
			uScore += m_stEndingBonus.uNoContinueBonus;

			m_stEndingBonus.uTotalScore = uScore;

			//�X�R�A�����Z����
			g_CHoundSaveData.UpdateHighScore( uScore );

			ChangeGameSeq( enMainSeqGameEndingDemo );
		}
		else
		{
			//���Z�b�g������
/*
			if( m_bGameStart && m_sMainMenuSelected == CMainMenu::enMainMenuStart )
			{
				m_sResetCount = 120;
				ChangeGameSeq( enMainSeqResetInGame );
			}
			else
			{
				ChangeGameSeq( enMainSeqRestart );
			}
*/
			ChangeGameSeq( enMainSeqRestart );

		}
		break;

	case enMainSeqGameContinue:
		//-----------------------------------------
		//�R���e�B�j���[
		//-----------------------------------------
		m_bResetOK = gxTrue;
		m_bGameStart = gxFalse;
		switch( ContinueScreen() ){
		case enMenuSeqNext:
			{
				//�^�C�g���ɖ߂�
				g_CHoundSaveData.UpdateHighScore( g_StGameInfo.m_uAllScore );
				ChangeGameSeq( enMainSeqRestart );
			}
			break;

		case enMenuSeqBack:
			{
				//�R���e�B�j���[����
//				Sint32 sMem = g_StGameInfo.PlayArea;
//				StageSet();
				//�X�R�A�����Z�b�g����
/*
				g_StGameInfo.m_uAllScore   = (g_StGameInfo.m_uAllScore/2);
				g_StGameInfo.m_uAllScore   -= g_StGameInfo.m_uAllScore%10;;
*/
				//�X�R�A�X�V
				g_CHoundSaveData.UpdateHighScore( g_StGameInfo.m_uAllScore );
				g_StGameInfo.m_uAllScore   = 0;//(g_StGameInfo.m_uAllScore/2);
				g_StGameInfo.m_uStageScore = g_StGameInfo.m_uAllScore;

				if( g_StGameInfo.m_sContinueCnt < 99 ) g_StGameInfo.m_sContinueCnt ++;
				g_StGameInfo.ContinueArea = g_StGameInfo.PlayArea;
				ChangeGameSeq( enMainSeqGameInit );
			}
			break;

		default:
			break;
		}
		break;

	case enMainSeqGameEndingDemo:
		//-----------------------------------------
		//�Q�[���I��
		//-----------------------------------------
		m_bResetOK = gxTrue;
		m_bGameStart = gxFalse;
		CCockpit::GetInstance()->SetHidden();
		CDashBoard::GetInstance()->SetMenuDisable(gxFalse);
		if( EndingDemo() )
		{
			ChangeGameSeq( enMainSeqRestart );
		}
		break;
	case enMainSeqResetInGame:
		if( m_sResetCount > 0 )
		{
			m_sResetCount --;
		}
		else
		{
			g_StGameInfo.m_uAllScore   = 0;
			g_StGameInfo.m_uStageScore = 0;
			g_StGameInfo.ContinueArea = g_StGameInfo.PlayArea;
			CCockpit::GetInstance()->ReNew();
			ChangeGameSeq( enMainSeqGameInit );
		}
		break;
	default:
		break;
	}

	viiMus::GetInstance()->Action();
	CFadeManager::GetInstance()->action();
}


Sint32 CGameGunHound::MainMenu()
{
	//---------------------------------------------
	//���C�����j���[��
	//---------------------------------------------
	Sint32 sRet = 0;

	if( m_pMainMenu == NULL )
	{
		CFadeManager::GetInstance()->set_fadein();
		m_pMainMenu = new CMainMenu( m_sMainMenuSelected );
		if( m_sMainMenuSelected == CMainMenu::enMainMenuMission )
		{
			m_pMainMenu->SetMainMenu( CMainMenu::enMainSeqMission , g_StGameInfo.PlayStage );
		}
		else if( m_sMainMenuSelected == CMainMenu::enMainMenuTutorial )
		{
			m_pMainMenu->SetMainMenu( CMainMenu::enMainSeqTraining , g_StGameInfo.PlayStage );
		}

		//�Q�[���f���̏���
		g_StGameInfo.PlayStage = GetDemoStage();
		StageSet();
	}

	m_pMainMenu->Action();

	if( m_pMainMenu->IsEnd() )
	{
		sRet                = m_pMainMenu->GetNext();
		m_sMainMenuSelected = m_pMainMenu->GetSelect();
		m_sSelectedStage    = m_pMainMenu->GetSelectedItem();

		if( m_pMainMenu->IsMutekiCommandOn() )
		{
			m_bSuperMutekiMode = gxTrue;
		}

		switch( sRet ){
		case enMenuSeqBack:
		case enMenuSeqNext:
			break;
		}

/*
//		if( !CFileLoader::GetInstance()->IsLoadFinish() )
//		{
//			//�ǂ݂����ׁ̈A�f���ɂ͈ڍs�ł��Ȃ�
//			viiDbg::log("�ǂ݂����̃f���C���[�W���c���Ă���");
//			CFileLoader::GetInstance()->Destroy();
//			//while( !CFileLoader::GetInstance()->Read() )
//			viiDbg::log("�ǂ݂����̃f���C���[�W��j��");
//		}
*/
	}
	else
	{
		//-----------------------------
		//�I�[�v�j���O�f������
		//-----------------------------

		if( CGXImage::GetInstance()->IsLoadFinish() )
		{
			m_pMainMenu->SetDemoReady(gxTrue);
		}
	}

	if( sRet )
	{
		delete m_pMainMenu;
		m_pMainMenu = NULL;
	}

	if( m_pMainMenu ) m_pMainMenu->Draw();

	return sRet;
}


void CGameGunHound::GameInit()
{
	//---------------------------------------------
	//�Q�[��������
	//---------------------------------------------
	gtimer_ex = 0;
	CCockpit::GetInstance()->SetBlackMask(gxFalse);

	set_globals_init();		//�O���[�o���̏�����

	srand(100);

	for(Sint32 i=0;i<10;i++)
	{
		_opt_[i] = 0;
	}

	CCockpit::GetInstance()->SetScore( g_StGameInfo.m_uStageScore );

	Sint32 sAdd = 1;
	if( g_CHoundSaveData.m_StConfig.sNoArmor )	sAdd ++;
	CCockpit::GetInstance()->SetScoreBairitsu( sAdd ) ;
}


void CGameGunHound::GameMain()
{
	//---------------------------------------------------------------
	//�Q�[�����C�����[�v
	//---------------------------------------------------------------
	gxBool bSlowFlag = gxFalse;


#ifdef _VII_DEBUG_
/*
	if( g_pActManager )
	{
		viiDbg::printf( 8,WINDOW_H,"ACT= %d / %d ", g_pActManager->GetNum() , ACTION_MAX );
	}
*/

	viiDbg::printf( 8,WINDOW_H+12,"CSN= %d / %d ", CCollisionManager::GetInstance()->GetNum() , MaxCollisionNum );
#endif


	if( m_bSuperMutekiMode )
	{
		pGame->GetHound()->SetNoDamage( gxTrue );
	}

	if( pGame->GetHound() && pGame->GetHound()->IsDestoroyed() )
	{
		bSlowFlag = gxTrue;	
	}

	if( CCockpit::GetInstance()->IsStageClear() )
	{
		//bSlowFlag = gxTrue;	
	}

	if( bSlowFlag )//pGame->GetHound() && pGame->GetHound()->IsDestoroyed() )
	{
		//�x������
		SlowMode(4);
	}
	else
	{
		//�|�[�Y�i�_�b�V���{�[�h�Ȃǁj����
		if( m_bPauseFlag )
		{
			pGame->pBg->Draw();
			CActManager::GetInstance()->Draw();//g_pActManager->Draw();
			pGame->pStage->draw();
			CCockpit::GetInstance()->Draw();
			return;
		}
	}

	if( pGame->Is_the_world() || CCockpit::GetInstance()->IsHitStop() )
	{
		//�Q�[�����̉�ʒ�~
		//���b�Z�[�W�����͓�����

		if( m_bFirstSeq )
		{
			CCockpit::GetInstance()->Action();

			//DRAW
			pGame->pBg->Draw();
			CActManager::GetInstance()->Draw();//g_pActManager->Draw();
			pGame->pStage->draw();
			CCockpit::GetInstance()->Draw();
		}
	}
	else
	{
		//ACTION
		CCockpit::GetInstance()->ResetRaderMarker();
		CCockpit::GetInstance()->Action();
		CCockpit::GetInstance()->AddMissionTime();
		pGame->action();
		CActManager::GetInstance()->Action();//g_pActManager->Action();
		pGame->pHitManager->Action();

		//DRAW
		pGame->pBg->Draw();
		CActManager::GetInstance()->Draw();//g_pActManager->Draw();
		pGame->pStage->draw();
		CCockpit::GetInstance()->Draw();
		m_bFirstSeq = gxTrue;
	}

}


void CGameGunHound::GameEnd()
{
	//---------------------------------------------------------------
	//�Q�[���I��
	//---------------------------------------------------------------

	m_bPauseFlag = gxFalse;
	if(pGame) delete pGame;

//	if( m_pMainMenu    ) delete m_pMainMenu;
	if( m_pBriefing    ) delete m_pBriefing; 
	if( m_pSetUp       ) delete m_pSetUp;
	if( m_pOpeningDemo ) delete m_pOpeningDemo;
	if( m_pEndingDemo  ) delete m_pEndingDemo;

//	m_pMainMenu = NULL;
	m_pBriefing = NULL; 
	m_pSetUp    = NULL;
	m_pOpeningDemo = NULL;
	m_pEndingDemo  = NULL;


	pGame = NULL;

	viiMus::StopBGM( enSoundBgm1 );
	viiMus::StopBGM( enSoundBgm2 );

}


void CGameGunHound::CockpitControl()
{
	//---------------------------------------------------------------
	//�R�N�s�b�g��ʃR���g���[��
	//---------------------------------------------------------------

	CCockpit::GetInstance()->Action();
	CCockpit::GetInstance()->Draw();

	if( m_pSetUp )
	{
		m_pSetUp->Action();
		m_pSetUp->Draw();
	}
}


void CGameGunHound::SlowMode(Sint32 n)
{
//	if(viiSub::GameCounter()%2)	//pGame->Action()�ŃJ�E���^�������邽�ߕʊǗ��̃J�E���^���K�v
	if(viiSub::vSyncTimer()%n)
	{
		pGame->set_the_world();
	}
	else
	{
		pGame->cancel_the_world();
	}
}


gxBool CGameGunHound::BriefingMain()
{
	//---------------------------------------------------------------
	//�u���[�t�B���O���C��
	//---------------------------------------------------------------

	if( m_pBriefing == NULL )
	{
		m_pBriefing = new CBriefing( gxFalse );
		m_pBriefing->SetGold( g_StGameInfo.Gold );

#ifdef _VII_DEBUG_
//		g_StGameInfo.ClearStage[0] = 1;
//		g_StGameInfo.ClearStage[1] = 1;
//		g_StGameInfo.ClearStage[2] = 1;
//		g_StGameInfo.ClearStage[3] = 1;
#endif

		m_pBriefing->SetClearStage( &g_StGameInfo.ClearStage[0] );

		viiMus::StopBGM( enSoundBgm1 );
		viiMus::StopBGM( enSoundBgm2 );

		CGXImage::GetInstance()->Load( "asset/gh/GXI/Briefing.GXI" );
	}

	m_pBriefing->Action();

	if( m_pBriefing->IsPreviewOK() )
	{
		m_pSetUp->SetViewStage ( m_pBriefing->GetStage() );
	}
	else
	{
		m_pSetUp->SetViewStage ( -1 );
	}

	if( m_pBriefing->IsEnd() )
	{
		g_StGameInfo.PlayStage = m_pBriefing->GetStage();

		m_pSetUp->SetViewStage ( m_pBriefing->GetStage() );
		m_pSetUp->SetSequence(0);

		delete m_pBriefing;
		m_pBriefing = NULL;
		return gxTrue;
	}

	m_pBriefing->Draw();

	return gxFalse;
}


gxBool CGameGunHound::GameSetUp()
{


	return gxFalse;
}


Sint32 CGameGunHound::AdvertiseDemo()
{
	//---------------------------------------------
	//�f��
	//---------------------------------------------

	if( m_pOpeningDemo == NULL )
	{
		m_pOpeningDemo = new COpeningDemo();
	}

	m_pOpeningDemo->Action();

	Sint32 opCode = m_pOpeningDemo->GetOperation();

	if( m_pOpeningDemo->IsEnd() )
	{
		delete m_pOpeningDemo;
		m_pOpeningDemo = NULL;
		opCode = COpeningDemo::enOperationEnd;
	}
	else
	{
		m_pOpeningDemo->Draw();
	}

	switch( opCode ){
	case COpeningDemo::enOperationInit:
		//�Q�[����������
		g_StGameInfo.PlayStage = GetDemoStage();
		pGame = new CGameManager();
		GameInit();
		g_StGameInfo.m_bMusic   = gxFalse;
		g_StGameInfo.m_bMessage = gxFalse;
		g_StGameInfo.m_bReplay  = gxTrue;
		pGame->init();
		m_sDemoCount ++;
		break;

	case COpeningDemo::enOperationMain:
		//�Q�[�����X�V

		GameMain();

		if( CCockpit::GetInstance()->IsGameOver() )
		{
			m_pOpeningDemo->SetStatusGameOver();
		}
		break;

	case COpeningDemo::enOperationEnd:
		GameEnd();
		if( pGame )
		{
			delete pGame;
			pGame = NULL;
		}
		CCockpit::GetInstance()->ReNew();
		return -1;

	default:
		break;
	}

	return 0;
}


Sint32 CGameGunHound::ContinueScreen()
{
	//----------------------------------------
	//�R���e�B�j���[���
	//----------------------------------------
	Sint32 sRet = 0;

	if( m_pContinue == NULL )
	{
		viiMus::StopBGM( enSoundBgm1 );
		viiMus::StopBGM( enSoundBgm2 );
		CFadeManager::GetInstance()->set_fadein();
		m_pContinue = new CContinue();
		m_pContinue->SetScore( g_StGameInfo.m_uAllScore , g_StGameInfo.m_uStageScore );
		CCockpit::GetInstance()->Destroy();
	}

	if( !CDashBoard::GetInstance()->IsMenuBar() )
	{
		m_pContinue->Action();
	}

	if(m_pContinue->IsEnd())
	{
		sRet = m_pContinue->GetNext();
		delete m_pContinue;
		m_pContinue = NULL;

		return sRet;
	}

	m_pContinue->Draw();

	return 0;

}


gxBool CGameGunHound::EndingDemo()
{
	//---------------------------------------------
	//�G���f�B���O�f��
	//---------------------------------------------
	if( m_pEndingDemo == NULL )
	{
		m_pEndingDemo = new CEndingDemo();
		if( m_sMainMenuSelected == CMainMenu::enMainMenuEnding )
		{
			m_pEndingDemo->SetScoreDisp(gxTrue);
			m_pEndingDemo->SetAfterEpisode();
		}
		else
		{
			m_pEndingDemo->SetScoreDisp(gxTrue);
			m_pEndingDemo->SetAllScore       (m_stEndingBonus.uAllScore);
			m_pEndingDemo->SetContinueBonus  (m_stEndingBonus.uContinueBonus);
			m_pEndingDemo->SetTrueEndBonus   (m_stEndingBonus.uTrueEndBonus);
			m_pEndingDemo->SetNoContinueBonus(m_stEndingBonus.uNoContinueBonus);
			m_pEndingDemo->SetTotalScore     (m_stEndingBonus.uTotalScore);
		}

		if( g_StGameInfo.m_bTrueEnd )
		{
			m_pEndingDemo->SetAfterEpisode();
		}

		CGXImage::GetInstance()->Load( "asset/gh/GXI/second.GXI" );

		//-------------------------------------------
		//�t�@�C���ǂݍ���
		//-------------------------------------------
		Uint32 sSize = 0;
		Uint8 *p;
		p = CGXImage::GetInstance()->GetFile( "Epilogue.wav" , &sSize );
		viiMus::ReadBGM( enSoundBgm1 , p , sSize );

		p = CGXImage::GetInstance()->GetFile( "Ending.wav" , &sSize );
		viiMus::ReadBGM( enSoundBgm2 , p , sSize );

		CGXImage::GetInstance()->SetTexture( enTexPageBackGround+0   ,"END1.bmp" );
		CGXImage::GetInstance()->SetTexture( enTexPageBackGround+16  ,"END2.bmp" );
		CGXImage::GetInstance()->SetTexture( enTexPageBackGround+24+0,"EndingLaugh1.bmp" );
		CGXImage::GetInstance()->SetTexture( enTexPageBackGround+24+1,"EndingLaugh2.bmp" );
		CGXImage::GetInstance()->SetTexture( enTexPageBackGround+24+2,"EndingLaugh3.bmp" );
		CGXImage::GetInstance()->SetTexture( enTexPageBackGround+24+3,"EndingLaugh4.bmp" );
		CGXImage::GetInstance()->SetTexture( enTexPageBackGround+24+4,"EndingLaugh5.bmp" );
		CGXImage::GetInstance()->SetTexture( enTexPageBackGround+24+5,"EndingLaugh6.bmp" );

		g_StGameInfo.m_bMessage = gxTrue;
		g_StGameInfo.m_bMusic   = gxTrue;
		g_StGameInfo.m_bSound   = gxTrue;
		CCockpit::GetInstance()->LoadMessageFromMemory( "datmsg.txt" );

		UploadTexture();
	}

	if( m_pEndingDemo->IsEnd() )
	{
		delete m_pEndingDemo;
		m_pEndingDemo = NULL;
		viiMus::StopBGM( enSoundBgm1 );
		return gxTrue;
	}

	m_pEndingDemo->Action();
	m_pEndingDemo->Draw();

	return gxFalse;
}


Sint32 CGameGunHound::GetDemoStage()
{
	//---------------------------------------------------------------
	//�f���p�̃X�e�[�W��Ԃ�
	//---------------------------------------------------------------

	switch(m_sDemoCount%2){
	case 0:
		return enScenarioJungle;
	default:
#ifdef _TRIAL_VERSION_
		return enScenarioJungle;
#else
		return enScenarioRiver;
#endif
	}

	return enScenarioJungle;
}

void CGameGunHound::StageSet()
{
	//---------------------------------------------------------------
	//�X�e�[�W�Z���N�g
	//---------------------------------------------------------------

//	g_StGameInfo.PlayStage;
	g_StGameInfo.PlayArea = 0;
	g_StGameInfo.m_bDebugRoot = gxFalse;

	//---------------------------------
	//�b��Q�[���Z�b�g������������
	//---------------------------------

	switch( g_StGameInfo.PlayStage ){
	case enScenarioJungle:
		CGXImage::GetInstance()->Load("asset/gh/GXI/Jungle.GXI");
		break;

	case enScenarioRiver:
		CGXImage::GetInstance()->Load("asset/gh/GXI/River.GXI");
		break;

	case enScenarioMountain:
		CGXImage::GetInstance()->Load("asset/gh/GXI/MountA.GXI");
		break;

	case enScenarioBase:
		CGXImage::GetInstance()->Load("asset/gh/GXI/BaseA.GXI");
		break;

	case enScenarioIronGear:
		CGXImage::GetInstance()->Load("asset/gh/GXI/Extra.GXI");
		break;
	case enScenarioExtra:
	case enScenarioExtra2:
	case enScenarioExtra3:
		break;

	case enScenarioTutorial:
		break;
	default:
		break;
	}

}


Sint32 CGameGunHound::DebugMenu()
{
	//---------------------------------------------------------------
	//�f�o�b�O���j���[
	//---------------------------------------------------------------
	if( CDashBoard::GetInstance()->IsMenuBar() )
	{
		return gxFalse;
	}

	Sint32 sRet = 0;
	static Sint32 scry = 0;
	static Sint32 sCursor = 0;
	static Sint32 sArea[64] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
	static Sint32 sStage[]=
	{
		enScenarioJungle,
		enScenarioRiver,
		enScenarioMountain,
		enScenarioBase,
		enScenarioIronGear,
		enScenarioTutorial,
		99,
	};

	static Sint32 sAreaMax[]=
	{
		6,6,7,9,7,  3,
		5,
	};

	if( Joy[0].rep&JOY_D )
	{
		sCursor ++;
	}
	else if( Joy[0].rep&JOY_U )
	{
		sCursor --;
	}

	sCursor = (sCursor+ARRAY_LENGTH(sStage))%ARRAY_LENGTH(sStage);

	viiSub::MenuPrintf( 32,16+-scry+16*0 ,ARGB_DFLT,"enScenarioJungle (%d)"    ,sArea[0] );
	viiSub::MenuPrintf( 32,16+-scry+16*1 ,ARGB_DFLT,"enScenarioRiver   (%d)"   ,sArea[1] );
	viiSub::MenuPrintf( 32,16+-scry+16*2 ,ARGB_DFLT,"enScenarioMountain   (%d)",sArea[2] );
	viiSub::MenuPrintf( 32,16+-scry+16*3 ,ARGB_DFLT,"enScenarioBase   (%d)"    ,sArea[3] );
	viiSub::MenuPrintf( 32,16+-scry+16*4 ,ARGB_DFLT,"enScenarioExtra   (%d)"   ,sArea[4] );
	viiSub::MenuPrintf( 32,16+-scry+16*5 ,ARGB_DFLT,"�`���[�g���A�� (%d)"      ,sArea[5] );
	viiSub::MenuPrintf( 32,16+-scry+16*6 ,ARGB_DFLT,"Debug�J���p (%d)"         ,sArea[6] );

	Sint32 cur_y = 16*sCursor;

	viiSub::MenuPrintf( 32-16,16+-scry+cur_y,ARGB_DFLT,"��");

	Sint32 height = WINDOW_H-16;

	if( cur_y >= scry+height )
	{
		scry = cur_y - height;
	}
	else if( cur_y < scry )
	{
		scry = cur_y;
	}

	//------------------------------------------------------
	//�R���g���[��
	//------------------------------------------------------

	if( Joy[0].rep&JOY_R )
	{
		sArea[ sCursor ] ++;
	}
	else if( Joy[0].rep&JOY_L )
	{
		sArea[ sCursor ] --;
	}

	sArea[ sCursor ] = ( sArea[ sCursor ]+sAreaMax[sCursor] )%sAreaMax[sCursor];

	Sint32 stage = sStage[ sCursor ]-98;
	Sint32 area  = sArea [ sCursor ];

	if( Joy[0].trg&enBtnCancel )
	{
		sRet = enMenuSeqBack;
	}
	else if( Joy[0].trg&enBtnDecision )
	{
		//�X�^�[�g
		sRet = enMenuSeqNext;
		g_StGameInfo.PlayStage = sStage[ sCursor ];

		StageSet();

		g_StGameInfo.m_bDebugRoot = gxTrue;
		g_StGameInfo.PlayArea = sArea [ sCursor ];

	}

	return sRet;

}


gxBool HoundStartUpLoading()
{
	Uint32 sDummy=0;

	static CGXImage *pCGXImage = CGXImage::GetInstance();

	pCGXImage->Load("asset/gh/GXI/cmn.GXI");

//	Sint32 sPer = pCGXImage->GetLoading();

//	viiDbg::printf( 128,-32,"NOW_LOADING(%d)",sPer);

	if( !pCGXImage->IsLoadFinish() ) return gxFalse;

//	viiSub::LoadTexture(0	,pCGXImage,"HoundData\\vram\\bank00\\DashBoard.bmp" );
	viiSub::LoadTexture(2	,pCGXImage,"HoundData\\vram\\bank00\\UI1.tga" );
//	viiSub::LoadTexture(3	,pCGXImage,"HoundData\\vram\\bank00\\UI3.bmp" );

//	viiSub::LoadTexture(4	,pCGXImage,"HoundData\\vram\\bank00\\AsNEW.bmp" );
	viiSub::LoadTexture(4	,pCGXImage,"HoundData\\vram\\bank00\\GunHound01.bmp" );
//	viiSub::LoadTexture(5	,pCGXImage,"HoundData\\vram\\bank00\\GunHound02.bmp" );	//BLACK�ɍ����ւ�
	viiSub::LoadTexture(6   ,pCGXImage,"HoundData\\vram\\bank00\\TexDummyHound.bmp" );
	viiSub::LoadTexture(enTexPageLineHound	,pCGXImage,"HoundData\\vram\\bank00\\gunhoundLine.bmp" );

	viiSub::LoadTexture(8	,pCGXImage,"HoundData\\vram\\bank00\\face1.bmp" );
//	viiSub::LoadTexture(9	,pCGXImage,"HoundData\\vram\\bank00\\face2.bmp" );
	viiSub::LoadTexture(10	,pCGXImage,"HoundData\\vram\\bank00\\string.tga" );
//	viiSub::LoadTexture(10	,pCGXImage,"HoundData\\vram\\bank00\\face.bmp" );
//	viiSub::LoadTexture(11	,pCGXImage,"HoundData\\vram\\bank00\\face.bmp" );
	viiSub::LoadTexture(12	,pCGXImage,"HoundData\\vram\\bank00\\effect.bmp" );
	viiSub::LoadTexture(13	,pCGXImage,"HoundData\\vram\\bank00\\StageEffect02.bmp" );
	viiSub::LoadTexture(14	,pCGXImage,"HoundData\\vram\\bank00\\cloud.tga" );
	viiSub::LoadTexture(enTexPageEffect4	,pCGXImage,"HoundData\\vram\\bank00\\effect3.bmp" );
	viiSub::LoadTexture(16	,pCGXImage,"HoundData\\vram\\bank01\\ene_soldier.bmp" );
	viiSub::LoadTexture(17	,pCGXImage,"HoundData\\vram\\bank01\\TexEneCmn03.bmp" );
	viiSub::LoadTexture(18	,pCGXImage,"HoundData\\vram\\bank01\\TexEneCmn05.bmp" );
	viiSub::LoadTexture(19	,pCGXImage,"HoundData\\vram\\bank01\\TexEneCmn01.bmp" );
	viiSub::LoadTexture(20	,pCGXImage,"HoundData\\vram\\bank01\\TexEneCmn06.bmp" );
	viiSub::LoadTexture(21	,pCGXImage,"HoundData\\vram\\bank01\\TexEneCmn04.bmp" );
	viiSub::LoadTexture(22	,pCGXImage,"HoundData\\vram\\bank01\\TexEneStg0502.bmp" );

	//viiSub::LoadTexture(23	,pCGXImage,"HoundData\\vram\\bank00\\" );

	viiSub::LoadTexture(32    ,pCGXImage,"HoundData\\vram\\bank02\\font.bmp" );

	viiSub::LoadTexture(38    ,pCGXImage,"HoundData\\vram\\bank02\\briefingObj.tga" );//������������
	viiSub::LoadTexture(34    ,pCGXImage,"HoundData\\vram\\bank02\\brbg.bmp" );
//	viiSub::LoadTexture(39    ,pCGXImage,"HoundData\\vram\\bank02\\briefingObj2.bmp" );
	viiSub::LoadTexture(40    ,pCGXImage,"HoundData\\vram\\bank02\\river.bmp" );
//	viiSub::LoadTexture(42    ,pCGXImage,"HoundData\\vram\\bank02\\dbgene0407.bmp" );
//	viiSub::LoadTexture(44    ,pCGXImage,"HoundData\\vram\\bank02\\TexEneStg0405.bmp" );
//	viiSub::LoadTexture(45    ,pCGXImage,"HoundData\\vram\\bank02\\TexEneStg0406.bmp" );
//	viiSub::LoadTexture(enTexPageSearchLight    ,pCGXImage,"HoundData\\vram\\bank02\\SearchLight.tga" );
#ifdef _TRIAL_VERSION_
	viiSub::LoadTexture(enTexPageSearchLight    ,pCGXImage,"HoundData\\vram\\bank02\\taikenban.bmp" );
#endif
//		viiSub::LoadTexture(48    ,pCGXImage,"HoundData\\vram\\bank03\\gxLogo.tga" );

	//�����������ɓǂ݂��ނ��ƁI�I�I
	viiSub::LoadTexture(52    ,pCGXImage,"HoundData\\vram\\bank03\\ScreenShot.bmp" );	
	//�����������ɓǂ݂��ނ��ƁI�I�I

	viiSub::LoadTexture(50    ,pCGXImage,"HoundData\\vram\\bank03\\TitleScreen1.bmp" );
	viiSub::LoadTexture(56    ,pCGXImage,"HoundData\\vram\\bank03\\TitleScreen2.bmp" );
	viiSub::LoadTexture(enTexPageEyeCatch    ,pCGXImage,"HoundData\\vram\\bank03\\eyecatch.bmp" );

	//viiSub::LoadTexture(58  ,pCGXImage,"HoundData\\vram\\bank00\\" );
	//viiSub::LoadTexture(59  ,pCGXImage,"HoundData\\vram\\bank00\\" );
	//viiSub::LoadTexture(62  ,pCGXImage,"HoundData\\vram\\bank00\\" );
	//viiSub::LoadTexture(63  ,pCGXImage,"HoundData\\vram\\bank00\\" );


	viiSub::LoadSound( enSoundSwitch01 ,pCGXImage, "HoundData\\sound\\sw01.wav");
	viiSub::LoadSound( enSoundSwitch02 ,pCGXImage, "HoundData\\sound\\sw02.wav");
	viiSub::LoadSound( enSoundSwitch03 ,pCGXImage, "HoundData\\sound\\sw03.wav");

	viiSub::LoadSound( enSoundGun01    ,pCGXImage, "HoundData\\sound\\gun01.wav");
	viiSub::LoadSound( enSoundMissile,pCGXImage,"HoundData\\sound\\missile.wav");
	viiSub::LoadSound( enSoundPunch,pCGXImage,"HoundData\\sound\\knouckle.wav");
	viiSub::LoadSound( enSoundBazooka,pCGXImage,"HoundData\\sound\\Bazooka.wav");

	viiSub::LoadSound( enSoundJump,pCGXImage,"HoundData\\sound\\jump.wav");
	viiSub::LoadSound( enSoundLanding,pCGXImage,"HoundData\\sound\\landing.wav");
	viiSub::LoadSound( enSoundBoost,pCGXImage,"HoundData\\sound\\Boost.wav");
	viiSub::LoadSound( enSoundExplosionSmall,pCGXImage,"HoundData\\sound\\exp_s.wav");
	viiSub::LoadSound( enSoundExplosionMiddle,pCGXImage,"HoundData\\sound\\exp_m.wav");
	viiSub::LoadSound( enSoundExplosionLarge,pCGXImage,"HoundData\\sound\\exp_l.wav");
	viiSub::LoadSound( enSoundExplosionMaximum,pCGXImage,"HoundData\\sound\\exp_ll.wav");

//	viiSub::LoadSound( enSoundMachine,pCGXImage,"");
	viiSub::LoadSound( enSoundMenuOpen ,pCGXImage,"HoundData\\sound\\MenuOpen.wav");
	viiSub::LoadSound( enSoundMenuSel  ,pCGXImage,"HoundData\\sound\\MenuSel.wav");
	viiSub::LoadSound( enSoundMenuDec  ,pCGXImage,"HoundData\\sound\\MenuDec.wav");
	viiSub::LoadSound( enSoundMenuCan  ,pCGXImage,"HoundData\\sound\\MenuCan.wav");
	viiSub::LoadSound( enSoundStageClear,pCGXImage,"HoundData\\sound\\StageClear.wav");
	viiSub::LoadSound( enSoundGameOver,pCGXImage,"HoundData\\sound\\GameOver.wav");
	viiSub::LoadSound( enSoundEyeCatch,pCGXImage,"HoundData\\sound\\EyeCatch.wav");
	viiSub::LoadSound( enSoundSplashS ,pCGXImage,  "HoundData\\sound\\w_exp_s.wav" );
	viiSub::LoadSound( enSoundSplashM ,pCGXImage,  "HoundData\\sound\\w_exp_m.wav" );
	viiSub::LoadSound( enSoundSplashL ,pCGXImage,  "HoundData\\sound\\w_exp_l.wav" );
	viiSub::LoadSound( enSoundOutofRange ,pCGXImage,  "HoundData\\sound\\warning.wav" );

	viiSub::LoadSound( enSoundTypeWriter ,pCGXImage,  "HoundData\\sound\\type.wav" );
	viiSub::LoadSound( enSoundDirArrow ,pCGXImage,  "HoundData\\sound\\arrow.wav" );
	viiSub::LoadSound( enSoundShotErase ,pCGXImage,  "HoundData\\sound\\bltClr.wav" );
	viiSub::LoadSound( enSoundDash ,pCGXImage,  "HoundData\\sound\\dash01.wav" );
	viiSub::LoadSound( enSoundHeliRoter ,pCGXImage,  "HoundData\\sound\\heli.wav" );
	viiSub::LoadSound( enSoundHpPowerUp ,pCGXImage,  "HoundData\\sound\\AA_hp_up.wav" );


	viiSub::LoadSound( enSoundCrashA   ,pCGXImage,  "HoundData\\sound\\AA_Crash1.wav" );
	viiSub::LoadSound( enSoundCrashB   ,pCGXImage,  "HoundData\\sound\\AA_Crash2.wav" );
	viiSub::LoadSound( enSoundGachan   ,pCGXImage,  "HoundData\\sound\\AA_Gachan.wav" );
	viiSub::LoadSound( enSoundRelord   ,pCGXImage,  "HoundData\\sound\\AA_Relord.wav" );
//	viiSub::LoadSound( enSoundTank     ,pCGXImage,  "HoundData\\sound\\AA_Tank.wav" );
//	viiSub::LoadSound( enSoundWarning2 ,pCGXImage,  "HoundData\\sound\\AA_Warning2.wav" );

	viiSub::LoadSound( enSoundEneShotSmall ,pCGXImage,  "HoundData\\sound\\AA_EneShot1.wav" );
	viiSub::LoadSound( enSoundEneShotLaser   ,pCGXImage,  "HoundData\\sound\\AA_EneShot2.wav" );
	viiSub::LoadSound( enSoundEneShotBiriBiri   ,pCGXImage,  "HoundData\\sound\\__AA_BiriBiri.wav");

//	delete pCGXImage;

	UploadTexture();

	return gxTrue;
}


//---------------------------------------
void CHoundSaveData::Save()
{
	//--------------------------------------
	//�O���[�o���Z�[�u�f�[�^���Z�[�u����
	//--------------------------------------
	Uint32 uCrc;

	//�N�����邽�тɃX�R�A�����Z����
	g_CHoundSaveData.m_uTotalScore += 10;

	if( g_CHoundSaveData.m_uTotalScore >= HIGH_SCORE_MAX )
	{
		g_CHoundSaveData.m_uTotalScore = HIGH_SCORE_MAX ;
	}

	//CRC���[���Ƃ��Ă���
	g_CHoundSaveData.m_uCrc = 0;

	//�h���p�����_���f�[�^���Z�b�g����
	g_CHoundSaveData.sRandData1 = viiSub::Rand()%0xFFFFFFFF;
	g_CHoundSaveData.sRandData2 = viiSub::Rand()%0xFFFFFFFF;
	g_CHoundSaveData.sRandData3 = viiSub::Rand()%0xFFFFFFFF;
	g_CHoundSaveData.sRandData4 = viiSub::Rand()%0xFFFFFFFF;
	g_CHoundSaveData.sRandData5 = viiSub::Rand()%0xFFFFFFFF;

	//CRC���[���̎��̐��f�[�^��CRC�l���v�Z���Č����
	uCrc = gxUtil::CalcCrc32( &g_CHoundSaveData , sizeof(CHoundSaveData) );

	Uint8 *p;
	CHoundSaveData Temp = g_CHoundSaveData;

	p = (Uint8*)&Temp;

	for(Uint32 ii=0;ii<sizeof(CHoundSaveData); ii++)
	{
		p[ii] ^= 0xFF;
	}

	//CRC�������
	Temp.m_uCrc = uCrc;

	gxLib::SaveFile( "savedata.gxr" , (Uint8*)&Temp , sizeof(CHoundSaveData) );

}


void CHoundSaveData::Load()
{
	//--------------------------------------
	//�O���[�o���Z�[�u�f�[�^�����[�h����
	//--------------------------------------

	Uint8 *p;
	Uint32 uCrc;

	CHoundSaveData m_SaveTemp = g_CHoundSaveData;

	memset(&m_SaveTemp , 0xD9 ,sizeof(CHoundSaveData) );

	Uint32 uSize;
	Uint8 *pData = gxLib::LoadFile("savedata.gxr", &uSize);

	if( pData == NULL )
	{
		viiDbg::log("�Z�[�u�f�[�^�̓ǂݍ��݂����s");
		return;
	}
	memcpy(&m_SaveTemp, pData, sizeof(CHoundSaveData));
	delete[] pData;

	//CRC�l�����o���Ă���
	uCrc = m_SaveTemp.m_uCrc;

	p = (Uint8*)&m_SaveTemp;

	for(Uint32 ii=0;ii<sizeof(CHoundSaveData); ii++)
	{
		p[ii] = p[ii]^0xFF;
	}

	//CRC���[���ɂ��Đ��f�[�^��CRC�l���v�Z
	m_SaveTemp.m_uCrc = 0;

	Uint32 uCrc2 = gxUtil::CalcCrc32(&m_SaveTemp, sizeof(CHoundSaveData));



	if( uCrc == uCrc2 )
	{
		//���o���Ă�����CRC�ƁA����(CRC���[��)��CRC���ׂ�
		g_CHoundSaveData = m_SaveTemp;
	}

}


void CHoundSaveData::UpdateHighScore(Uint32 score)
{
	//------------------------------------------
	//�X�R�A�X�V
	//------------------------------------------

	g_CHoundSaveData.m_uTotalScore += score;

	if( g_CHoundSaveData.m_uTotalScore >= HIGH_SCORE_MAX )
	{
		g_CHoundSaveData.m_uTotalScore = HIGH_SCORE_MAX ;
	}

}

//gxBool s_bPauseFlag = gxFalse;
void gameinit()
{
//	//---------------------------------------------------------------
//	//�Q�[��������
//	//---------------------------------------------------------------
//
//	s_bPauseFlag = gxFalse;
//
//	//-------------------------------------------------------------------
//	viiDbg::log("VII�����������܂��B");
//	//-------------------------------------------------------------------
////	viiSub::GameCounter() = 1;
//
//	set_globals_init();	//�O���[�o���̏�����
//
//	gtimer_ex = 0;
//	srand(100);
//	for(Sint32 i=0;i<10;i++) {
//		_opt_[i] = 0;
//	}
//	//-------------------------------------------------------------------
//	viiDbg::log("�����_���`�F�b�N�P:%d",rand());
//	//-------------------------------------------------------------------
//	pGame = new CGameManager();
//
//	//-------------------------------------------------------------------
//	viiDbg::log("�����_���`�F�b�N�Q:%d",rand());
//	//-------------------------------------------------------------------
//
//	pGame->init();
//
//	//-------------------------------------------------------------------
//	viiDbg::log("�����_���`�F�b�N�R:%d",rand());
//	//-------------------------------------------------------------------
//
//	//-------------------------------------------------------------------
//	extern Sint32 Joy_memory_key;		//�L�[����
//	viiDbg::log("�����_���`�F�b�N�S:%d:%x",rand(),Joy_memory_key);
//	//-------------------------------------------------------------------
//
}

void gamemain()
{
//	//---------------------------------------------------------------
//	//�Q�[�����C�����[�v
//	//---------------------------------------------------------------
//
//	if( Joy[0].trg&BTN_ST)
//	{
//		s_bPauseFlag = !s_bPauseFlag;
//	}
//
//	if(pGame->get_playerout())
//	{
// 		if(CFadeManager::GetInstance()->is_fade())
// 		{
//			//�X���[����
//	 		if(viiSub::GameCounter()%2)
//	 		{
//				pGame->set_the_world();
//			}
//			else
//			{
//				pGame->cancel_the_world();
//			}
//		}
//		else
//		{
//			pGame->cancel_the_world();
//		}
//	}
//	else
//	{
//		if( s_bPauseFlag )
//		{
//			pGame->pBg->action();
//			g_pActManager->Draw();
//			return;
//		}
//	}
//
//	if( pGame->Is_the_world() )
//	{
//
//	}
//	else
//	{
//		pGame->action();
//		g_pActManager->Action();
//		pGame->pHitManager->Action();
//		g_pActManager->Draw();
//	}
//
}


void gameend()
{
	//---------------------------------------------------------------
	//�����I���ɔ������I������
	//---------------------------------------------------------------

//	//-------------------------------------------------------------------
//	viiDbg::log("VII��j�����܂��B");
//	//-------------------------------------------------------------------
//
//	if(pGame)			delete pGame;
//
//	if(CFadeManager::GetInstance())
//	{
//		CFadeManager::GetInstance()->Destroy();
//	}
//
//	if(g_pActManager)
//	{
//		delete g_pActManager;
//		g_pActManager = NULL;
//	}

	if( g_pCGameGunHound )
	{
		delete g_pCGameGunHound;
	}

	g_pCGameGunHound = NULL;
}

//-----------------------------------------------
//�P�O���_�l�@
//-----------------------------------------------
//#define HIGH_SCORE_MAX (999999990)

//�Q�[�����ɂ�200���_�����ƂP�O�O��łQ���_
//�P�X�e�[�W�ɂ��Q�O�O���_�����ƂP�O�O�O���_�{�N���A�{�[�i�X�P�O�O�O���_
//���܂��v���C�ŃX�g�[���[�T�O��V�ׂ΃J�E���^�[�X�g�b�v

//-----------------------------------------------

//����v���C�`�N���A�܂ōŒZ���[�g�̏ꍇ��
//�X�g�[���[���Q�疜
//�~�b�V�������T���Q�O�O���P�O�O�O��
//�`���[�g���A�����R���P�O�O����R�O�O��
//�v�R�R�O�O�����R���P��

//-----------------------------------------------

//����v���C�łP��P�T�O�O��
//���v���C�łP��Q�O�O�O��
//�R�O�O�O���P�ʂł��ق���
//�R���ɒB���鎞�_�łP�O�A�C�e��

//�T�O�O��
//�P�Q�O�O��
//�Q�T�O�O��

//�T�O�O�O��
//�P���Q�O�O�O��
//�Q���T�O�O�O��

//�R���W�O�O�O��
//�U���W�O�O�O��
//�X���X�O�O�O��
//-----------------------------------------------

//�J���X�g�܂�
//�ʂ��v���C�P�O��łP�O���_
//�X�g�[���[�T�O��łP�O���_
//�~�b�V�����T�O�O��łP�O���_
//�`���[�g���A���P�O�O�O��łP�O���_

//-----------------------------------------------


