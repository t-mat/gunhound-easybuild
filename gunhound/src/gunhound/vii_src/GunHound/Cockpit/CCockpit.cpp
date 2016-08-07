//---------------------------------------------------
//
//�R�N�s�b�g���
//
//---------------------------------------------------

#include <gunvalken.h>
#include "../../enemy/enemy.h"
#include "CMsgWindow.h"
#include "CStageClear.h"
#include "CMsgManager.h"
#include "../../CBellowsSprite.h"
#include "../../GunHound/GhStringTbl.h"

CCockpit* CCockpit::g_sCCockpit = NULL;

gxSprite StSprWeaponWindow[]=
{
	{enTexPageSystemUI,97,72 ,68,11,0,0},	//1
	{enTexPageSystemUI,97,87 ,56,12,0,0},	//2
	{enTexPageSystemUI,97,103,56,13,0,0},	//4
};

gxSprite StSprWeaponIcon[]=
{
	{enTexPageSystemUI,80,72,16,8,0,0},	//GUN
	{enTexPageSystemUI,80,88,16,8,0,0},	//ROCKET
	{enTexPageSystemUI,80,105,16,8,0,0},	//MISSILE
};

ghPos sBurniaTbl[]={
	//�o�[�j�A�|�W�V����
	{59,151}, {51,151},
	{58,144}, {51,145},
	{56,137}, {49,140},
	{53,133}, {47,137},
	{50,129}, {45,135},
	{46,126}, {43,133},
	{42,124}, {40,131},
	{38,123}, {37,130},
	{34,122}, {34,129},
	{30,122}, {30,129},
};

ghPos sHitPointTbl[]={
	//�q�b�g�|�C���g�Q�[�W�|�W�V����
	{52,168,}, {46,165},
	{45,176,}, {40,168},
	{38,179},  {36,170},
	{30,180},  {30,171},
	{22,179},  {24,170},
	{16,176},  {20,168},
	{8,171},  {16,165},//
	{4,164},   {12,161},
	{2,157},   {10,156},
	{1,150,},  {9,150},
	{3,141,},  {10,144},
	{6,134},   {12,139},
	{11,128},  {16,132},//
};

gxSprite StSprWeapon[]=
{
//	{enTexPageSystemUI,0,120,56,48,28,24},			//�����[�h�Q�[�W
	{enTexPageSystemUI,0,112,10,8,0,0},				//??
	{enTexPageSystemUI,0,116,168,68,30-0,151-116},	//�����[�h�Q�[�W

	{enTexPageSystemUI,64+32,112+32,32,32,16,16},	//�T�u�E�F�|�������v
	{enTexPageSystemUI,64   ,112+32,32,32,16,16},	//�T�u�E�F�|�������vON
};

gxSprite AsSprRemainTime[]=
{
	{ enTexPageEffectCommon02,0*16,112,16,16,0,0},
	{ enTexPageEffectCommon02,1*16,112,16,16,0,0},
	{ enTexPageEffectCommon02,2*16,112,16,16,0,0},
	{ enTexPageEffectCommon02,3*16,112,16,16,0,0},
	{ enTexPageEffectCommon02,4*16,112,16,16,0,0},
	{ enTexPageEffectCommon02,5*16,112,16,16,0,0},
	{ enTexPageEffectCommon02,6*16,112,16,16,0,0},
	{ enTexPageEffectCommon02,7*16,112,16,16,0,0},
	{ enTexPageEffectCommon02,8*16,112,16,16,0,0},
	{ enTexPageEffectCommon02,9*16,112,16,16,0,0},
};

gxSprite StSprNowLoading[]=
{
	{enTexPageSystemUI,208,0,48,48,24,24},
	{enTexPageSystemUI,160,0,48,48,24,24},
	{enTexPageSystemUI,0,24,64,8,32,4},
};

enum{
	enShortMessageLength = 12,
};
CCockpit::CCockpit()
{
	for(Sint32 ii=0;ii<enSwitchMax;ii++)
	{
		m_bSwitch[ii] = gxFalse;
		m_sAutoTimer[ii] = 0;
		m_bAutoSwitch[ii] = gxTrue;
	}
	m_sScore = 0;

	m_pMessage = NULL;
	m_fMessageScroll = ZERO;
	m_sMessageWait = 0;
	m_sMessagelength = 0;
	m_bMessageEnable = gxTrue;

	m_bArrow = gxFalse;
	m_sArrowWait = 100;
	m_sArrowRot  = 0;

	m_pRaderDamage  = NULL;
	m_pRaderControl = NULL;
	m_pMsgWindow    = NULL;
	m_pStageClear   = NULL;
	m_pBellowPoint  = NULL;
	m_pBellowBurn   = NULL;

	m_pRaderDamage  = new CRaderDamage();
	m_pRaderControl = new CRaderControl();
	m_pMsgWindow    = new CMsgWindow();

	//�o�[�j�A�Q�[�W
	m_pBellowPoint = new CBellowsSprite();
	m_pBellowBurn  = new CBellowsSprite();

	for(Sint32 ii=0;ii<ARRAY_LENGTH( sHitPointTbl )/2;ii++)
	{
		m_pBellowPoint->AddPosition( sHitPointTbl[ii*2+0].x , sHitPointTbl[ii*2+0].y ,sHitPointTbl[ii*2+1].x , sHitPointTbl[ii*2+1].y );
	}

	for(Sint32 ii=0;ii<ARRAY_LENGTH( sBurniaTbl )/2;ii++)
	{
		m_pBellowBurn->AddPosition( sBurniaTbl[ii*2+0].x , sBurniaTbl[ii*2+0].y ,sBurniaTbl[ii*2+1].x , sBurniaTbl[ii*2+1].y );
	}

	m_bPlayerOut = gxFalse;
	m_bGameOver  = gxFalse;
	m_sHitPoint  = 100;
	m_sDispHitPoint = 0;

	m_sScore     = 0;
	m_sScoreWait = 0;
	m_sDispScore = 0;
	m_sBurniaEnergy = 0;
	m_bSubWeapon = gxFalse;

	m_bOutogRange = gxFalse;
	m_sPlayerPosX = 0;
	m_sOperationrangeMin = -12000;
	m_sOperationrangeMax =  12000;

	m_sMissionTime = 0*60;
	m_sMissionSecFlah = 0;

	m_sConpaneTimer = 0;
	m_sNowLoadingRot = 0;
	m_sNowLoadingWait = 0;

	m_sWhiteFade   = 0;

	m_bSplash = gxFalse;

	for(Sint32 jj=0;jj<enMessageMemMax;jj++)
	{
		m_sMsgIndex[ jj ] = -1;
	}

	m_sBlackMask = 0;
	m_bBlackMask = gxFalse;
	m_sWarningFrame = 0;
	m_sCockpitTimer = 0;
	m_sSplashAlpha  = 0;

	//-------------------------
	//����A�C�R��
	//-------------------------

	m_sWeaponType[0]   = 0;
	m_sWeaponGauge[0]  = 0;
	m_sWeaponRemain[0] = 0;

	m_sWeaponType[1]   = 0;
	m_sWeaponGauge[1]  = 0;
	m_sWeaponRemain[1] = 0;

	m_sHeight = 100;
	m_sStopCnt = 0;
	m_bSystemSound = gxTrue;

	m_pDanmaku = new StDanmaku[enDanmakuMax];

	m_sScoreBairitsu = 1;

}


CCockpit::~CCockpit()
{
	if(m_pRaderDamage ) delete m_pRaderDamage;
	if(m_pRaderControl) delete m_pRaderControl;
	if(m_pMsgWindow)    delete m_pMsgWindow;
	if( m_pStageClear ) delete m_pStageClear;

	if( m_pDanmaku ) delete[] m_pDanmaku;

	m_pDanmaku = NULL;
	g_sCCockpit = NULL;

}

CCockpit* CCockpit::GetInstance()
{
	if( g_sCCockpit == NULL )
	{
		g_sCCockpit = new CCockpit();
	}

	return g_sCCockpit;
}

void CCockpit::Destroy()
{
	//-----------------------------
	//�R�N�s�b�g�֘A��j�����܂�
	//-----------------------------
	if( g_sCCockpit )
	{
		delete g_sCCockpit;
		g_sCCockpit = NULL;
	}
}

void CCockpit::ReNew()
{
	m_sStopCnt = 0;

	if( m_pRaderDamage ) delete m_pRaderDamage;
	if( m_pRaderControl) delete m_pRaderControl;
	if( m_pMsgWindow   ) delete m_pMsgWindow;
	if( m_pStageClear  ) delete m_pStageClear;

	if( m_pBellowPoint ) delete m_pBellowPoint;
	if( m_pBellowBurn )  delete m_pBellowBurn;

	if( m_pDanmaku ) delete[] m_pDanmaku;

	m_pDanmaku      = new StDanmaku[enDanmakuMax];

	m_pRaderDamage  = new CRaderDamage();
	m_pRaderControl = new CRaderControl();
	m_pMsgWindow    = new CMsgWindow();

	m_pBellowPoint = new CBellowsSprite();
	m_pBellowBurn  = new CBellowsSprite();

	m_pStageClear   = NULL;

	for(Sint32 jj=0;jj<enMessageMemMax;jj++)
	{
		m_sMsgIndex[ jj ] = -1;
	}

	for(Sint32 ii=0;ii<ARRAY_LENGTH( sHitPointTbl )/2;ii++)
	{
		m_pBellowPoint->AddPosition( sHitPointTbl[ii*2+0].x , sHitPointTbl[ii*2+0].y ,sHitPointTbl[ii*2+1].x , sHitPointTbl[ii*2+1].y );
	}

	for(Sint32 ii=0;ii<ARRAY_LENGTH( sBurniaTbl )/2;ii++)
	{
//		m_pBellowPoint->AddPosition( sBurniaTbl[ii*2+0].x , sBurniaTbl[ii*2+0].y ,sBurniaTbl[ii*2+1].x , sBurniaTbl[ii*2+1].y );
		m_pBellowBurn->AddPosition( sBurniaTbl[ii*2+0].x , sBurniaTbl[ii*2+0].y ,sBurniaTbl[ii*2+1].x , sBurniaTbl[ii*2+1].y );
	}

	m_bMessageEnable = gxTrue;
	m_bPlayerOut = gxFalse;
	m_bGameOver  = gxFalse;
	m_sWhiteFade = 0;

	m_sBlackMask = gxFalse;
	m_bBlackMask = gxFalse;

	m_sSplashAlpha = 0;
	m_bSplash = 0;
	m_sWarningFrame = 0;

	m_bMessageEnable = gxTrue;
	m_bSystemSound = gxTrue;

}


gxBool   CCockpit::IsDanmakuEnable( Sint32 n )
{
	//�e���������Ă��邩�H

	//���o�^�̒e���͐����Ă��邱�Ƃɂ���
	if( n < 0 ) return gxTrue;

	return m_pDanmaku[n].m_bEnable;

}


void CCockpit::SetDanmakuIndex( Sint32 n , gxBool bFlag )
{

	m_pDanmaku[n].m_bEnable = bFlag;

}


void CCockpit::AutoStartUp()
{
	//-------------------------------------------
	//�e�p�l���̃I�[�g�I�[�v��
	//-------------------------------------------
#if 1
	for(Sint32 ii=0;ii<enSwitchMax;ii++)
	{
		m_sAutoTimer[ ii ] = 0;
		if( ii == enSwitchRed )
		{
			continue;
		}

		m_sAutoTimer[ ii ] = 1;//20+ii*20+viiSub::Rand()%30;
	}
#endif

}

gxBool CCockpit::IsMessageNow()
{
	if( !g_StGameInfo.m_bMessage )
	{
		//���b�Z�[�W��\���������Ƃɂ��ċA��
		return gxFalse;
	}
	return m_pMsgWindow->IsMessageNow();
}

void CCockpit::SetStageClear()
{
	//-------------------------------------------
	//�X�e�[�W�N���A
	//-------------------------------------------

	m_pStageClear = new CStageClear();

}

gxBool CCockpit::IsStageClear()
{
	//�X�e�[�W�N���A�����̂��H

	return (m_pStageClear)? gxTrue : gxFalse;
}


gxBool CCockpit::IsClearOver()
{
	//-------------------------------------------
	//�X�e�[�W�N���A�������H�̔���
	//-------------------------------------------

	if( IsMessageNow() )
	{
		return gxFalse;
	}
	if( m_pStageClear )
	{
		return m_pStageClear->IsEnd();
	}

	return gxFalse;
}

void CCockpit::DestroyStageClear()
{
	//--------------------------------------
	//�X�e�[�W�N���A�\������������
	//--------------------------------------

	if( m_pStageClear )
	{
		delete m_pStageClear;
		m_pStageClear = NULL;
	}
}

void CCockpit::LoadMessage( char *pFileName )
{
	//--------------------------------------
	//�t�H���g
	//--------------------------------------

	m_pMsgWindow->GetFont()->Load( pFileName );

}

void CCockpit::LoadMessageFromMemory( char *pFileName )
{
	//--------------------------------------
	//�t�H���g
	//--------------------------------------

	m_pMsgWindow->GetFont()->LoadMemory( pFileName );

}

gxBool CCockpit::SetMessage(Sint32 sIndex , gxBool bForce)
{
	//--------------------------------------
	//���b�Z�[�W�̕\��
	//--------------------------------------
	Sint32 jj = 0;

	if( !g_StGameInfo.m_bMessage )
	{
		//���b�Z�[�W�Ȃ����[�h�i�~�b�V�������[�h/�f���j�Ȃ烁�b�Z�[�W���o���Ȃ�
		//���b�Z�[�W��\���������Ƃɂ��ċA��
		return gxTrue;
	}

	if( !m_bMessageEnable )
	{
		return gxFalse;
	}

	while( gxTrue ) 
	{
		if( m_sMsgIndex[ jj ] ==  -1 )
		{
			//�܂��\�����Ă��Ȃ���΃o�b�t�@�ɒǉ����ă��b�Z�[�W��\������
			m_sMsgIndex[ jj ] = sIndex;
			m_pMsgWindow->Run(sIndex);
			return gxTrue;
		}

		if( sIndex == m_sMsgIndex[ jj ] )
		{
			//���ɕ\�����Ă���
			if( bForce )
			{
				//�����̏ꍇ�͕\�����s��
				m_pMsgWindow->Run(sIndex);
			}
			return gxFalse;
		}

		jj ++;

		if( jj >= enMessageMemMax )
		{
			//������Ȃ�����
			break;
		}
	}

	return gxFalse;
}


void CCockpit::SeqMain()
{
	//--------------------------------------------------
	//���C���V�[�P���X
	//--------------------------------------------------

	if( m_sNowLoadingWait > 0 )
	{
		m_sNowLoadingWait --;
		m_sNowLoadingRot ++;
		m_sNowLoadingRot = m_sNowLoadingRot%360;
	}

	if( m_sMissionSecFlah > 0 )
	{
		m_sMissionSecFlah --;
	}

	if( m_sStopCnt > 0 )
	{
		m_sStopCnt --;
	}

	m_sConpaneTimer ++;

	m_pRaderDamage->Action();
	m_pRaderControl->Action();
	m_pMsgWindow->Action();

	if( m_pStageClear )
	{
		//�X�e�[�W�N���A
		pGame->GetHound()->SetMuteki();
		m_pStageClear->Action();
	}

	PanelSwitching();

	if( m_sBurniaEnergy == 0 )
	{
		CCockpit::GetInstance()->SetShortMessage( StrShortMessage[7] );//"BOOST POWER IS EMPTY...");
	}

	//--------------------------------------------------
	//�ԊO�����[�_�[����
	//--------------------------------------------------
	ControlSekigaisen();

	//--------------------------------------------------
	//HITPOINT����
	//--------------------------------------------------
	ControlHitPoint();

	if( m_bPlayerOut )//&& m_sDispHitPoint == 0 )
	{
		Sint32 sAdd = (255-m_sWhiteFade)/30;
		if(sAdd==0) sAdd = 1;
		m_sWhiteFade += sAdd;
		if( m_sWhiteFade >= 255 )
		{
			m_bGameOver = gxTrue;
			m_sWhiteFade = 255;
		}
	}

	//--------------------------------------------------
	//�X�R�A����
	//--------------------------------------------------
	ControlScore();

	//--------------------------------------------------
	//�G���A�O����
	//--------------------------------------------------
	if( (m_sPlayerPosX/100 > m_sOperationrangeMin) && (m_sPlayerPosX/100 < m_sOperationrangeMax) )
	{
		m_bOutogRange = gxFalse;

	}
	else
	{
//		if( m_sConpaneTimer%64==0)
//		{
//			viiMus::PlaySound( enSoundOutofRange );
//		}
		m_bOutogRange = gxTrue;
	}

	//--------------------------------------------------
	//�A���[����
	//--------------------------------------------------
	if( m_bArrow )
	{
		if( m_sArrowWait > 0 )
		{
			m_sArrowWait -= 2;
			if(m_sArrowWait <= 0 )
			{
				viiMus::PlaySound( enSoundDirArrow );
				m_sArrowWait = 0;
			}
		}
		else
		{
			m_bArrow = gxFalse;
		}
	}

	//---------------------------------------
	//�I�[�g�C���^�[�t�F�[�X
	//---------------------------------------
	for(Sint32 ii=0;ii<enSwitchMax;ii++)
	{
		if( m_sAutoTimer[ ii ] > 0 )
		{
			m_sAutoTimer[ ii ] --;
			if( m_sAutoTimer[ ii ] == 0)
			{
//				UISound( ii );
				m_bSwitch[ii] = gxTrue;
			}
		}
	}

	//---------------------------------------
	//�V���[�g���b�Z�[�W�֘A
	//---------------------------------------
	if( m_pMessage )
	{
		if( m_sMessageWait > 0 )
		{
			m_sMessageWait --;
			if( m_sMessageWait == 0 )
			{
				if( m_fMessageScroll >= m_sMessagelength-enShortMessageLength )
				{
					m_cShortMsgBuf[0] = 0x00;
					m_pMessage = NULL;
				}
			}
		}
		else
		{
			if( m_fMessageScroll < m_sMessagelength-enShortMessageLength )
			{
				m_fMessageScroll += 0.1f;
			}
			else
			{
				m_sMessageWait = 60;
			}
		}
	}

	//-----------------------------
	//�x����
	//-----------------------------
	if( m_sWarningFrame > 0 )
	{
		m_sWarningFrame --;
		if( m_sCockpitTimer%64 == 0)
		{
			if( m_bSystemSound ) viiMus::PlaySound( enSoundOutofRange );
		}
		
	}

	//-----------------------------
	//�u���b�N�}�X�N
	//-----------------------------
	m_sHeight -= 8;

	if( m_bBlackMask )
	{
		m_sBlackMask += 8;
		m_sHeight += 16;
		if( m_sBlackMask >= 100 ) m_sBlackMask = 100;
	}
	else 
	{
		m_sBlackMask -= 8;
		if( m_sBlackMask <= 0 ) m_sBlackMask = 0;
	}

	if( m_sHeight >= 100 )    m_sHeight = 100;
	if( m_sHeight <= 0   )    m_sHeight = 0;

	//-----------------------------
	//�X�v���b�V��
	//-----------------------------

	if( m_bSplash )
	{
		m_sSplashAlpha += 0x10;
		if(m_sSplashAlpha >= 255) m_sSplashAlpha = 255;
	}
	else
	{
		m_sSplashAlpha -= 0x10;
		if(m_sSplashAlpha <= 0) m_sSplashAlpha = 0;
	}

	m_sCockpitTimer ++;
}

void CCockpit::SetShortMessage(char* pMessage)
{
	//-----------------------------
	//�V���[�g���b�Z�[�W����t
	//-----------------------------
	if( strcmp(pMessage,m_cShortMsgBuf) )
	{
		m_fMessageScroll = 0;
		m_pMessage       = pMessage;
		m_sMessageWait   = 60;
		m_sMessagelength = (Sint32)strlen(pMessage);
		sprintf( m_cShortMsgBuf , "%s" , pMessage);
	}
}


void CCockpit::ControlSekigaisen()
{
	//--------------------------------------------------
	//�ԊO�����[�_�[����
	//--------------------------------------------------
	if( pGame == NULL ) return;


	if( m_bSwitch[enSwitchRed] )
	{
		m_pRaderDamage->SetDisp(gxTrue);
		if( Joy[0].my < 64 )
		{
			m_pRaderDamage->SetSwitch(gxTrue);
			pGame->SetRaderScope( enScopeTypeSnipe );
		}
		else
		{
			m_pRaderDamage->SetSwitch(gxFalse);
			pGame->SetRaderScope( enScopeTypeNormal );
		}
	}
	else
	{
		m_pRaderDamage->SetDisp(gxFalse);
		m_pRaderDamage->SetSwitch(gxFalse);
		pGame->SetRaderScope( enScopeTypeNormal );
	}
}



void CCockpit::ControlHitPoint()
{
	//--------------------------------------------------
	//HITPOINT����
	//--------------------------------------------------

	if( m_sDispHitPoint < m_sHitPoint )
	{
		m_sDispHitPoint += 140;	//140
		if(m_sDispHitPoint > m_sHitPoint)
		{
			m_sDispHitPoint = m_sHitPoint;
		}
	}
	else if( m_sDispHitPoint > m_sHitPoint )
	{
		Sint32 sAdd = 0;
//		m_sDispHitPoint -= 140;	//140

		sAdd = (m_sHitPoint-m_sDispHitPoint)/10;

		if( sAdd > -100 )
		{
			sAdd = -100 ;
		}

		m_sDispHitPoint += sAdd;
//		m_sDispHitPoint = m_sHitPoint;

		if(m_sDispHitPoint < m_sHitPoint)
		{
			m_sDispHitPoint = m_sHitPoint;
		}
	}
}

void CCockpit::ControlScore()
{
	if(m_sDispScore < m_sScore )
	{
		Sint32 sAdd = (m_sScore-m_sDispScore)/10;
		if( sAdd == 0 ) sAdd = 1;

		m_sDispScore += sAdd;
	}

	if(	m_sScoreWait > 0 ) m_sScoreWait --;

}


void CCockpit::UISound(Sint32 n)
{

	viiMus::PlaySound( enSoundSwitch03 );

}


void CCockpit::PanelSwitching()
{
	//-------------------------------------------------
	//�p�l���̃X�C�b�`��ON/OFF����
	//�L�[�{�[�h����̂�
	//-------------------------------------------------

	if(gxLib::KeyBoard(('1')&enStatTrig ) )
	{
		//���[�_�[
		Sint32 n = enSwitchRader;
		m_bSwitch[n] = !m_bSwitch[n];
		m_bAutoSwitch[n] = gxFalse;
		UISound( n );
	}
	else if( gxLib::KeyBoard(('2')&enStatTrig ) )
	{
		//�p�l��
		Sint32 n = enSwitchPosition;
		m_bSwitch[n] = !m_bSwitch[n];
		m_bAutoSwitch[n] = gxFalse;
		UISound( n );

		n = enSwitchBullet;
		m_bSwitch[n] = !m_bSwitch[n];
		m_bAutoSwitch[n] = gxFalse;

	}
	else if(gxLib::KeyBoard(('3')&enStatTrig) )
	{
		//�X�R�A
		Sint32 n = enSwitchScore;
		m_bSwitch[n] = !m_bSwitch[n];
		m_bAutoSwitch[n] = gxFalse;
		UISound( n );
	}
	else if(gxLib::KeyBoard(('4')&enStatTrig) )
	{
		//�^�C��
		Sint32 n = enSwitchTime;
		m_bSwitch[n] = !m_bSwitch[n];
		m_bAutoSwitch[n] = gxFalse;
		UISound( n );
	}
	else if (gxLib::KeyBoard(('5')&enStatTrig))
	{
		//�V���[�g���b�Z�[�W
		Sint32 n = enSwitchMessage;
		m_bSwitch[n] = !m_bSwitch[n];
		m_bAutoSwitch[n] = gxFalse;
		UISound( n );
	}
#ifdef _VII_DEBUG_
	else if (gxLib::KeyBoard(('0')&enStatTrig))
	{
		Sint32 n = enSwitchRed;
		m_bSwitch[n] = !m_bSwitch[n];
		m_bAutoSwitch[n] = gxFalse;
		UISound( n );
	}
#endif
/*
	else if(CPadControl::GetInstance()->IsTriggerKeyBoard('4'))
	{
		Sint32 n = 4;
		m_bSwitch[n] = !m_bSwitch[n];
		m_bAutoSwitch[n] = gxFalse;
		UISound( n );
	}
*/
/*
	else if(CPadControl::GetInstance()->IsTriggerKeyBoard('8'))
	{
		Sint32 n = 8;
		m_bSwitch[n] = !m_bSwitch[n];
		m_bAutoSwitch[n] = gxFalse;
		UISound( n );
	}
	else if(CPadControl::GetInstance()->IsTriggerKeyBoard('9'))
	{
		Sint32 n = 9;
		m_bSwitch[n] = !m_bSwitch[n];
		m_bAutoSwitch[n] = gxFalse;
		UISound( n );
	}
*/

}


void CCockpit::SeqCrash()
{



}


void CCockpit::Draw()
{
	//---------------------------------
	//�`�揈��
	//---------------------------------
	Sint32 bx=0,by=0;
	Sint32 wx,wy;

	if( m_sNowLoadingWait )
	{
		DrawNowLoading();
	}

	if( m_bSplash )
	{
		DrawSplash();
	}

	wx = 0;
	wy = m_sHeight;

	m_pRaderDamage->SetHitPoint( m_sDispHitPoint );

	if( m_bArrow )                     DrawArrow(WINDOW_W/2,WINDOW_H/2);
	
	if( m_bSwitch[enSwitchRader]    )  m_pRaderControl->Draw(0,WINDOW_H+wy);
	if( m_bSwitch[enSwitchRed]      )  m_pRaderDamage->DrawSekigaisen(470+24,5);

	//����U.I
	if( m_bSwitch[enSwitchBullet]   ) DrawRelord(36,36-wy);	//�g�o�\��
	if( m_bSwitch[enSwitchPosition] ) m_pRaderDamage->Draw(8,10+by-wy);

	if( m_bSwitch[enSwitchTime]     )  DrawTime(400-16,24-wy);
	if( m_bSwitch[enSwitchScore]    )  DrawScore(400-32,8-wy);
	if( m_bOutogRange )                DrawOutOfArea(320,240+wy);
	if( m_bSwitch[enSwitchMessage]   ) DrawShortMessage(64,44-wy);

	if( m_sBlackMask ) 				   DrawBlackmask();

	if( m_bSwitch[enSwitchDamage]   ) 
	{
		//DrawHitPoint();
	}
	else
	{
		m_sDispHitPoint = 0;
	}

	if( m_sWhiteFade )
	{
		DrawGameOver();
	}

//		enSwitchDamage,		//�c��HP(ON/OFF)
//		enSwitchMessage,	//�V���[�g���b�Z�[�W(ON/OFF)
//		enSwitchBullet,		//�c�e���iON/OFF)
//		enSwitchHigher,		//�G���x�\��(ON/OFF)

	m_pMsgWindow->Draw(6,WINDOW_H-74-m_sBlackMask*28/100);

	if( m_pStageClear ) m_pStageClear->Draw();

}


void CCockpit::DrawShortMessage( Sint32 bx , Sint32 by )
{
	//------------------------------------
	//�V���[�g���b�Z�[�W
	//------------------------------------
	Sint32 sLen;
	char buf[256];

	if( m_pMessage == NULL ) return;

	Sint32 sPos = (Sint32)m_fMessageScroll;

	sprintf(buf,"%s",&m_cShortMsgBuf[sPos] );
	sLen = strlen(buf);

	if( sLen >= enShortMessageLength )
	{
		buf[enShortMessageLength] = 0x00;
	}

//	viiSub::MenuPrintf(bx,by,0xFFf0f0f0,buf );
	viiDraw::sml_printf( bx,by,0xFFFFFFFF,buf );
	//m_pMessage
}


void CCockpit::DrawRelord( Sint32 bx , Sint32 by )
{
	//------------------------------------
	//H.P �o�[�j�A�\��
	//------------------------------------

	Sint32 pow = m_sBurniaEnergy;

	//�g�̕`��
	Sint32 mx,my;
	mx = bx;
	my = by;

	viiSub::MenuSprite( &StSprWeapon[1]  , mx , my , PRIO_SYSTEMWINDOW ,ATR_DFLT,0xc0ffffff);
 
/*
	static Float32 m_fTest = 50.f;

	if(Joy[0].psh&JOY_U)
	{
		m_fTest += 1.f;
	}
	else if(Joy[0].psh&JOY_D)
	{
		m_fTest -= 1.f;
	}

	delete m_pBellowBurn;
	m_pBellowBurn = new CBellowsSprite();
	ghPos xsHitPointTbl[]={
	{59,151}, {51,151},
	{58,144}, {51,145},
	{56,137}, {49,140},
	{53,133}, {47,137},
	{50,129}, {45,135},
	{46,126}, {43,133},
	{42,124}, {40,131},
	{38,123}, {37,130},
	{34,122}, {34,129},
	{30,122}, {30,129},
	};

	for(Sint32 ii=0;ii<ARRAY_LENGTH( xsHitPointTbl )/2;ii++)
	{
		m_pBellowBurn->AddPosition( xsHitPointTbl[ii*2+0].x , xsHitPointTbl[ii*2+0].y ,xsHitPointTbl[ii*2+1].x , xsHitPointTbl[ii*2+1].y );
	}
*/

	DrawWeaponGauge(0 , bx,by+0 );
	DrawWeaponGauge(1 , bx,by+14);
//	DrawWeaponGauge(bx,by+28,0,100);

	//�q�b�g�|�C���g
	Sint32 sArgb = ARGB(0xff,0,250,0);
	m_pBellowPoint->m_Pos.x = bx+7-36;
	m_pBellowPoint->m_Pos.y = by+-114-36;
	m_pBellowPoint->m_Pos.z = PRIO_SYSTEMWINDOW-1;
	m_pBellowPoint->SetLength( m_sDispHitPoint/100.f );
	if( m_sDispHitPoint/100 < 20 )
	{
		//�_��
		if( viiSub::GameCounter()%16 < 8 )
		{
			sArgb = ARGB(0xff,250,0,0);
		}
		else
		{
			sArgb = ARGB(0xff,0x01,0x01,0x01);
		}
	}
	else if( m_sDispHitPoint/100 < 50 )
	{
		sArgb = ARGB(0xff,0xa0,0xa0,0x01);
	}

	m_pBellowPoint->SetColor(sArgb , ARGB_DFLT);
	m_pBellowPoint->Action();
	m_pBellowPoint->Draw();

	m_pBellowPoint->m_Pos.x = bx+6-36;
	m_pBellowPoint->m_Pos.y = by+-115-36;
	m_pBellowPoint->Draw();

	//�o�[�j�A�G�l���M�[

	m_pBellowBurn->m_Pos.x = bx+7-36;
	m_pBellowBurn->m_Pos.y = by+-114-36;
	m_pBellowBurn->m_Pos.z = PRIO_SYSTEMWINDOW-1;
	m_pBellowBurn->SetLength( m_sBurniaEnergy*1.f );
//	m_pBellowPoint->SetLength( m_fTest );
	m_pBellowBurn->SetColor(ARGB(0xff,230,162,25) , ARGB_DFLT);
	m_pBellowBurn->Draw();
	
	m_pBellowBurn->m_Pos.x = bx+6-36;
	m_pBellowBurn->m_Pos.y = by+-115-36;
	m_pBellowBurn->Draw();
	
/*
	if( m_bSubWeapon )
	{
		Float32 fScl;
		fScl = 1.f+0.5f*gxAbs(-32+viiSub::GameCounter()%64)/32.f;
		viiSub::MenuSprite( &StSprWeapon[2]  , mx-2 , my , PRIO_SYSTEMWINDOW ,ATR_DFLT,0xA0FFFFFF);
		viiSub::MenuSprite( &StSprWeapon[3]  , mx-2 , my , PRIO_SYSTEMWINDOW ,ATR_ALPHA_PLUS,0x40ffffff,fScl);
	}
	else
	{
		viiSub::MenuSprite( &StSprWeapon[2]  , mx-2 , my , PRIO_SYSTEMWINDOW ,ATR_DFLT,0x80808080);
	}
*/

}


void CCockpit::DrawWeaponGauge(Sint32 n , Sint32 px, Sint32 py )
{
	//-----------------------------------------
	//�E�F�|���c�e�Q�[�W��`�悷��
	//-----------------------------------------
	Sint32 x,y,z,c,c2,c3,w;

	x = px+34;
	y = py-26;
	z = PRIO_SYSTEMTEXT;
	c = 0xf0ffffff;
	c2 = ARGB(240,242,238,130);
	c3 = 0xf0404040;

	Sint32 sIndex        = m_sWeaponType[n];
	Sint32 sType         = m_sWeaponGauge[n];
	Sint32 sBulletRemain = m_sWeaponRemain[n];

	//--------------------------------
	//�Q�[�W
	//--------------------------------
	switch( sType ){
	case enWeaponGaugeMeter:	//�}�V���K�����[�^
		viiSub::MenuSprite( &StSprWeaponWindow[0]  , x+18   , y   , z ,ATR_DFLT,c);
		if(sBulletRemain <= 0)
		{
			sBulletRemain = 100;
			c2 = ( viiSub::GameCounter()%8>4)? 0xf0800000 : c3;
		}
		w = sBulletRemain*(82-21)/100;
		viiDraw::Box( x+21, y+2, x+21+w ,y+8 , z , gxTrue, c2  , ATR_DFLT );
		break;

	case enWeaponGaugeTwo:	//���P�b�g
		viiSub::MenuSprite( &StSprWeaponWindow[1]  , x+18   , y   , z ,ATR_DFLT,c);
		if(sBulletRemain <= 0)
		{
			sBulletRemain = 2;
			c2 = ( viiSub::GameCounter()%8>4)? 0xf0800000 : c3;
		}
		switch( sBulletRemain ){
		case 2:
			viiDraw::Box( x+48, y+3, x+69 ,y+9 , z , gxTrue, c2  , ATR_DFLT );
		case 1:
			viiDraw::Box( x+21, y+3, x+44 ,y+9 , z , gxTrue, c2  , ATR_DFLT );
		default:
			break;
		}
		break;

	case enWeaponGaugeFour:	//�~�T�C��
		viiSub::MenuSprite( &StSprWeaponWindow[2]  , x+18   , y   , z ,ATR_DFLT,c);
		if(sBulletRemain <= 0)
		{
			sBulletRemain = 4;
			c2 = ( viiSub::GameCounter()%8>4)? 0xf0800000 : c3;
		}
		switch( sBulletRemain ){
		case 4:
			viiDraw::Box( x+60, y+3, x+69 ,y+9 , z , gxTrue, c2  , ATR_DFLT );
		case 3:
			viiDraw::Box( x+47, y+3, x+56 ,y+9 , z , gxTrue, c2  , ATR_DFLT );
		case 2:
			viiDraw::Box( x+34, y+3, x+43 ,y+9 , z , gxTrue, c2  , ATR_DFLT );
		case 1:
			viiDraw::Box( x+21, y+3, x+30 ,y+9 , z , gxTrue, c2  , ATR_DFLT );
		default:
			break;
		}
		break;
	case enWeaponGaugeOne:
	case enWeaponGaugeNone:
	case enWeaponGaugeThree:
	default:
		break;
	}

	//--------------------------------
	//�A�C�R��
	//--------------------------------
	switch( sIndex ){
	case enWeaponIconBullet:	//�}�V���K��
		viiSub::MenuSprite( &StSprWeaponIcon[0]    , x      , y   , z ,ATR_DFLT,c);
		break;

	case enWeaponIconRocket:	//���P�b�g
		viiSub::MenuSprite( &StSprWeaponIcon[1]    , x      , y   , z ,ATR_DFLT,c);
		break;

	case enWeaponIconMissile:	//�~�T�C��
		viiSub::MenuSprite( &StSprWeaponIcon[2]    , x      , y   , z ,ATR_DFLT,c);
		break;
	case enWeaponIconNone:
	case enWeaponIconPunch:
	default:
		break;
	}

}


void CCockpit::DrawScore( Sint32 bx , Sint32 by )
{
	//------------------------------------
	//�X�R�A�\��
	//------------------------------------

	Sint32 sCol = 0xFFf0f0f0;

	if(	m_sScoreWait > 0 )
	{
		m_sScoreWait --;
		if((m_sScoreWait/2)%2)
		{
			sCol = 0xFFf00000;
		}
	}

	viiDraw::sml_printf( bx,by,sCol,"%s %02d" ,GameOverString[3] , m_sDispScore );
}


void CCockpit::DrawArrow( Sint32 bx , Sint32 by )
{
	//------------------------------------
	//�A���[�\��
	//------------------------------------

	Sint32 z = PRIO_GAMETOP;
	Float32 fScl=1.f;
	Sint32 sCol = 0xA0*m_sArrowWait/100;
	Sint32 sRot = viiSub::GameCounter()%360;//m_sArrowWait;
	Sint32 x,y;

	fScl = 1.f+m_sArrowWait/100.f;

	x = bx;
	y = by;

	if( m_sArrowRot == 1000 )
	{
		//�r�b�N���}�[�N
//		viiSub::MenuSprite( x,y,z, enTexPageSystemUI,16,80 ,16,32,8,16 ,ATR_DFLT, ARGB((0xA0-sCol),0xFF,0xFF,0xFF) , 0 );
//		viiSub::MenuSprite( x,y,z, enTexPageGrowObj,0,208 ,192,48,96,24 ,ATR_DFLT, ARGB((sCol),0xFF,0xFF,0xFF) , 0 , 2.f , 1.f);
		viiSub::MenuSprite( x,y,z, enTexPageGrowObj,0,208 ,192,48,96,24 ,ATR_DFLT, ARGB((sCol),0xFF,0xFF,0xFF) , 0 , 0.5f , 0.5f);
		return;
	}
	else if( m_sArrowRot == 2000 )
	{
		viiSub::MenuSprite( WINDOW_W-64,WINDOW_H/2,z, enTexPageGrowObj,0,208 ,192,48,96,24 ,ATR_DFLT, ARGB((sCol),0xFF,0xFF,0xFF) , 0 , 0.5f , 0.5f);
		return;
	}
	else if( m_sArrowRot == 3000 )
	{
		viiSub::MenuSprite( WINDOW_W/2,WINDOW_H-32,z, enTexPageGrowObj,0,208 ,192,48,96,24 ,ATR_DFLT, ARGB((sCol),0xFF,0xFF,0xFF) , 0 , 0.5f , 0.5f);
		return;
	}
	else if( m_sArrowRot == 4000 )
	{
		viiSub::MenuSprite( 64,WINDOW_H/2,z, enTexPageGrowObj,0,208 ,192,48,96,24 ,ATR_DFLT, ARGB((sCol),0xFF,0xFF,0xFF) , 0 , 0.5f , 0.5f);
		return;
	}
	else
	{
		//�A���[
		Sint32 ax = (viiMath::Cos100(m_sArrowRot)*(100-m_sArrowWait))/1000;
		Sint32 ay = (viiMath::Sin100(m_sArrowRot)*(100-m_sArrowWait))/1000;
		viiSub::MenuSprite( x+ax,y+ay,z, enTexPageSystemUI,32,80 ,16,32,8,16 ,ATR_DFLT, ARGB((sCol),251,245,198) , m_sArrowRot*1.f );
	}

	viiSub::MenuSprite( x,y,z, enTexPageSystemUI,48,80 ,32,32,16,16 ,ATR_DFLT, ARGB(sCol,32,243,198) , sRot*1.f ,fScl,fScl);
//	viiSub::MenuSprite( x,y,z, enTexPageSystemUI,48,80 ,32,32,16,16 ,ATR_DFLT, ARGB(sCol,0xFF,0xFF,0xFF) ,-sRot*1.f ,fScl);

}


void CCockpit::DrawOutOfArea( Sint32 bx , Sint32 by )
{
	//------------------------------------
	//�͈͊O
	//------------------------------------

	if( viiSub::GameCounter()%24<12) return;

	viiDraw::printf( bx,by,0xFFFF00FF,GameOverString[4] );//"OUT OF RANGE"  );

}


void CCockpit::DrawTime( Sint32 bx , Sint32 by )
{
	//-----------------------------------------------
	//�^�C���\��
	//-----------------------------------------------

	Sint32 sec = m_sMissionTime/60;
	Sint32 min = sec/60;
	Sint32 ux=bx,uy=by,ax=0,ay=0,az=PRIO_SYSTEM;;
	Sint32 num,sCol = 0xfff0f0f0;

	if( m_sMissionSecFlah > 0 )
	{
		//���ԍX�V�t���b�V��
		if( m_sMissionSecFlah%16 < 8 )
		{
			sCol = 0xf000f000;
		}
		else
		{
			sCol = 0xf0f0f000;
		}
	}
	else if( m_sMissionTime < 60 )	//(60*60*1)/2 )
	{
		
		if(viiSub::GameCounter()%4<2) sCol = 0xf0f00000;
	}

	viiDraw::printf( bx,by,sCol,"%02d:%02d" ,min,sec%60 );

	return;
	sec = sec%60;

	ax = 0;

	num = min/10;
	viiSub::MenuSprite( &AsSprRemainTime[ num ]  ,  (ux+ax),(uy+ay) , az ,ATR_DFLT, sCol);

	ax += 16;

	num = min%10;
	viiSub::MenuSprite( &AsSprRemainTime[ num ]  ,  (ux+ax),(uy+ay) , az ,ATR_DFLT, sCol );

	ax += 16;

	num = sec/10;
	viiSub::MenuSprite( &AsSprRemainTime[ num ]  ,  (ux+ax),(uy+ay) , az ,ATR_DFLT, sCol );

	ax += 16;

	num = sec%10;
	viiSub::MenuSprite( &AsSprRemainTime[ num ]  ,  (ux+ax),(uy+ay) , az ,ATR_DFLT, sCol );

}


void CCockpit::KanjiPrintf(Sint32 x, Sint32 y,char *pMsg,...)
{
	//-----------------------------------
	//�����\��
	//-----------------------------------
	if( !m_pMsgWindow) return;
	if( !m_pMsgWindow->GetFont() ) return;

	char buf[0xff];
	va_list app;

	va_start(app, pMsg);
	vsprintf(buf, pMsg, app);
	va_end(app);

	m_pMsgWindow->GetFont()->Printf(x,y,PRIO_SYSTEM,0xf0ffffff,buf);

}


void CCockpit::KanjiColorPrintf(Sint32 x, Sint32 y, Sint32 sCol , char *pMsg,...)
{
	//-----------------------------------
	//�����\��
	//-----------------------------------
	if( !m_pMsgWindow) return;
	if( !m_pMsgWindow->GetFont() ) return;

	char buf[0xff];
	va_list app;

	va_start(app, pMsg);
	vsprintf(buf, pMsg, app);
	va_end(app);

	m_pMsgWindow->GetFont()->Printf(x,y,PRIO_SYSTEM,sCol,buf);

}


void CCockpit::KanjiPrintfZ(Sint32 x, Sint32 y, Sint32 sPrio, Sint32 sCol , char *pMsg,...)
{
	//-----------------------------------
	//�����\��
	//-----------------------------------
	if( !m_pMsgWindow) return;
	if( !m_pMsgWindow->GetFont() ) return;

	char buf[0xff];
	va_list app;

	va_start(app, pMsg);
	vsprintf(buf, pMsg, app);
	va_end(app);

	m_pMsgWindow->GetFont()->Printf(x,y,sPrio,sCol,buf);

}

void CCockpit::Reset()
{
	m_sStopCnt = 0;

	if(m_pMsgWindow)
	{
		m_pMsgWindow->Reset();
	}
}



void CCockpit::DrawNowLoading()
{
	//-----------------------------------
	//�i�E���[�f�B���O�}�[�N
	//-----------------------------------
	Sint32 ux,uy,uz;
	
	ux = WINDOW_W-48;
	uy = 48;
	uz = PRIO_SYSTEM;

	Sint32 sAlp = 128*viiMath::Cos100((m_sNowLoadingRot*8)%360)/100;
	Sint32 sCol = ARGB( (100+sAlp) , 0xf0 , 0xf0 ,0xf0 );

	viiSub::MenuSprite( &StSprNowLoading[ 0 ]  ,  ux , uy , uz    , ATR_DFLT , ARGB_DFLT , 1.f, m_sNowLoadingRot );
	viiSub::MenuSprite( &StSprNowLoading[ 1 ]  ,  ux , uy , uz    , ATR_DFLT , ARGB_DFLT , 1.f, -m_sNowLoadingRot );
	viiSub::MenuSprite( &StSprNowLoading[ 2 ]  ,  ux , uy+32 , uz , ATR_DFLT , sCol      , 1.f );

}

void CCockpit::DrawGameOver()
{
	//-----------------------------------
	//�Q�[���I�[�o�[�p�̃t�F�[�h�}�X�N
	//-----------------------------------

	Sint32 alpha = 0xff*m_sWhiteFade/255;

	viiDraw::Box( 0, 0, WINDOW_W ,WINDOW_H , PRIO_FADE , gxTrue, ARGB(alpha,0xff,0xff,0xff)  , ATR_DFLT );

}

void CCockpit::DrawSplash()
{

	viiSub::MenuSprite( 0,0,PRIO_FADEOVER,  enTexPageEyeCatch,0,0,480,256,0,0 ,ATR_DFLT , ARGB(m_sSplashAlpha,0xff,0xff,0xff) );

}



void CCockpit::DrawBlackmask()
{
	//------------------------------------------
	//���}�X�N��ݒ肷��
	//------------------------------------------
	Sint32 yy = 32;
	Sint32 y = yy*m_sBlackMask/100;
	Sint32 sy;

	sy = 0-yy+y;
	viiDraw::Box( 0, sy, WINDOW_W ,sy+yy , PRIO_FADE , gxTrue, ARGB(0xff,0x01,0x01,0x01)  , ATR_DFLT );

	sy = WINDOW_H-y;
	viiDraw::Box( 0, sy, WINDOW_W ,sy+yy , PRIO_FADE , gxTrue, ARGB(0xff,0x01,0x01,0x01)  , ATR_DFLT );

}

void CCockpit::AddMissionTime()
{
	//-------------------------------------
	//�~�b�V�����^�C�}�[��i�߂�
	//-------------------------------------

	if( m_sMissionTime > 0 )
	{
		m_sMissionTime -= 2;
	}
}

void CCockpit::SetWeaponIcon( Sint32 sIndex , Sint32 sIcon , Sint32 sType , Sint32 sBulletRemain)
{
	//-------------------------------------
	//����A�C�R���̃Z�b�g
	//-------------------------------------

	m_sWeaponType[sIndex]   = sIcon;
	m_sWeaponGauge[sIndex]  = sType;
	m_sWeaponRemain[sIndex] = sBulletRemain;
}

void CCockpit::ResetMessageWindow()
{
	//-------------------------------------
	//���b�Z�[�W�E�C���h�E�̃��Z�b�g
	//-------------------------------------

	if( m_pMsgWindow )
	{
		m_pMsgWindow->Reset();
	}
}

Sint32 CCockpit::GetMessageIndex(Sint32 n)
{
	if( m_pMsgWindow == NULL ) return -1;
	if( m_pMsgWindow->GetFont() == NULL ) return -1;
	Sint32 sMax = m_pMsgWindow->GetFont()->GetMessageMax();

	if( sMax == 0 ) return -1;
	n = n%sMax;

	return m_pMsgWindow->GetFont()->GetMessageIndex( n );

}

Sint32 CCockpit::GetScoreBairitsu()
{
	return m_sScoreBairitsu;
}


void CCockpit::SetScoreBairitsu( Sint32 n )
{
	m_sScoreBairitsu = n;

}

