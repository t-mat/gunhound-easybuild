//----------------------------------------------------------------------------
//
// �X�^�[�g�̃X�e�[�W�i�W�����O���j
//
//----------------------------------------------------------------------------

#include <gunvalken.h>
#include <gxLib/util/gxImage/CGXImage.h>
#include "CStageBase.h"
//---------------------------------------------------------------------------
#include "../effect/EffectBase.h"
#include "../enemy/enemy.h"

/*
gxSprite SprStageH02River[]=
{
	{enTexPageEffectStage01,0,0  ,256,96 , 0 , 0},
	{enTexPageEffectStage01,0,104,256,96 , 0 , 0},
};
*/

gxSprite StSprCloud[]={
	{enTexPageBgObj3,0,0,480,256,0,0},
};

enum {
	enSetHoundArea1 = 0,
//	enSetHoundArea2,
//	enSetSankai,
//	enSetHougekichu,
//	enSetGrazziAtack,
//	enSetGrazziDestroy,
//	enSetInthePlant,
	enSetCarryOn,
	enSetTraceGriffon,
	enSetFreeActionStart,
	enSetFreeActionStop,

};

enum {
	enWaraWaraMax = 32,
	enBgColor01 = 0xF0408080,
	enBgColor02 = 0xF0808040,
	enBgColor03 = 0xF0A0A080,
	enBgColor04 = 0xF0802020,
	enGriffonStartPoint = 0,//812,
	enBlaclColor = 0xffa0a0a0,
};


enum
{
	enAreaSeq01,
	enAreaSeq02,
	enAreaSeq03,
	enAreaSeq04,
	enAreaSeq05,
	enAreaTest,
	enAreaSeqMax,
};

enum {
	//�G���A�Q�̃V�[�P���X
//	enZoneSeqMitsurinNukeru=0,
//	enZoneSeqChukeitenArrive,
	enZoneSeqVsGrazziInit=0,
	enZoneSeqVsGrazziBattle,
	enZoneSeqVsGrazziEnd,
	enZoneSeqCannonStart,
	enZoneSeqYusouHeliCome,
	enZoneSeqBleachClear,
	enZoneSeqExplosion,
	enZoneSeqWallWait,
	enZoneSeqAssault,
	enZoneSeqNextArea,
};




CStageStart::CStageStart()
{
	Sint32 ii;

//����Ȃ�	m_pTextName = NULL;

	m_DummyPos.x = 0;
	m_DummyPos.y = 0;

	m_sScrollSeq = 0;
	m_sScrollPos = 0;
	m_sBombPosition = 0;

	for(ii=0;ii<enHeavyTankmax;ii++)
	{
		m_pTank[ii] = NULL;
	}

	m_pNightSpider = NULL;

	for(ii=0;ii<enHoundMax;ii++)
	{
		m_pHound[ii] = NULL;
		m_pHoundFly[ii] = NULL;
	}

	for(ii=0;ii<enTachikomaMax;ii++)
	{
		m_pTachikoma[ii] = NULL;
	}

	for(ii=0;ii<enApacheMax;ii++)
	{
		m_pTransHeli[ii] = NULL;
	}
	m_bGrazzi = gxFalse;

	m_pYusouHeli = NULL;
	m_pGrazzi    = NULL;
	m_pWall      = NULL;
	m_pYagura = NULL;

	m_sFlashAlpha = 0x00;
	m_sWaraWaraCnt = 0;

	m_sCoveredSeq = 0;
}


CStageStart::~CStageStart()
{

}


void CStageStart::Init()
{
	//---------------------------------------------------------------------------
	//������
	//---------------------------------------------------------------------------
	SetFadeIn();

	Sint32 col1 =  0xF0404060;
	Sint32 col2 =  0xF0406080;
//	Sint32 col1 =  0x00000000;
//	Sint32 col2 =  0x00000000;
	pGame->pBg->SetSkyColor(80,col1,col2);

	//---------------------------------------------------------------------------
	//�G���A������
	//---------------------------------------------------------------------------
//	CEneH0105TankGuard *pTankGuard;

//	if( m_bFirst )
//	{
//		if( g_StGameInfo.m_bDebugRoot )
//		{
//			CGXImage::GetInstance()->Load( "asset/gh/GXI/JUNGLE.GXI" );
//		}
//	}

	while( !CGXImage::GetInstance()->IsLoadFinish() )
	{
		//���[�h�҂�
	}

	switch(GetArea( )){
	case enAreaSeq01:
	case enAreaSeq02:
		if( g_StGameInfo.m_sGameMode == StGameInfo::enGameModeMission )
		{
			new CItemBox( 9245*100, 128*100 , 0 );
		}
		CGXImage::GetInstance()->Load( "asset/gh/GXI/JUNGLE.GXI" );
		SetDRQStage( "ScenJungle.txt" );

		//�����^���N�̐���
		m_pTank[enHeavyTankMaster] = new CEneH0101HeavyTank( enGriffonStartPoint*100,650*100);
		m_pTank[enHeavyTankMaster]->SetOperation(CHoundEnemyBase::enForceOpMovePos,(5500+240)*100);
		m_pTank[enHeavyTankMaster]->SetDummy();

		m_pTank[enHeavyTankSlave]  = new CEneH0101HeavyTank((enGriffonStartPoint+400)*100,650*100);
		m_pTank[enHeavyTankSlave]->SetOperation(CHoundEnemyBase::enForceOpMovePos,(6000+240)*100);
		m_pTank[enHeavyTankSlave]->SetDummy();

		//--------------------

//		m_pTankGuard[0] = new CEneH0105TankGuard( (enGriffonStartPoint+242)*100,(650+40)*100 ,gxTrue);
//		m_pTankGuard[0]->SetTarget( &m_pTank[enHeavyTankMaster]->m_Pos ,248*100 );

//		m_pTankGuard[1] = new CEneH0105TankGuard( (enGriffonStartPoint+242)*100,(650+40)*100 ,gxTrue);
//		m_pTankGuard[1]->SetTarget( &m_pTank[enHeavyTankMaster]->m_Pos ,-248*100 );

		//--------------------

//		m_pTankGuard[2] = new CEneH0105TankGuard( (enGriffonStartPoint+650)*100,(650+40)*100 , gxTrue );
//		m_pTankGuard[2]->SetTarget( &m_pTank[enHeavyTankSlave]->m_Pos , 248*100);

//		m_pTankGuard[3] = new CEneH0105TankGuard( (enGriffonStartPoint+650)*100,(650+40)*100 , gxTrue );
//		m_pTankGuard[3]->SetTarget( &m_pTank[enHeavyTankSlave]->m_Pos , -248*100);


		if(GetArea( ) == enAreaSeq01)
		{
			ControlHound( enSetHoundArea1 );
			InitEnemies();
			viiSub::SetScroll_l( GetTargetPlayer()->x+256*100 , GetTargetPlayer()->y );
		}
		else
		{
			viiMus::PlayBGM(enSoundBgm1,enMusicBgmStageA);
			ControlHound( enSetFreeActionStart );
			SetTargetPlayerPosX(5500*100);
			SetTargetPlayerPosY(500*100);
			viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
			InitEnemies(5800);
			CCockpit::GetInstance()->SetMissionSec( 5*60 );
		}
		break;

	case enAreaSeq03:
		CGXImage::GetInstance()->Load( "asset/gh/GXI/JUNGLE.GXI" );
		SetDRQStage( "ScenJungle.txt" );
		viiMus::PlayBGM(enSoundBgm1,enMusicBgmStageA);
		SetTargetPlayerPosX(7350*100);
		SetTargetPlayerPosY(500*100);
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		InitEnemies(7000);
		new CEffScene0A02(320*100,160*100,PRIO_BG1-1,0);
		CCockpit::GetInstance()->SetMissionSec( 5*60 );
		break;

	case enAreaSeq04:
		//�{�X�G���A
		CGXImage::GetInstance()->Load( "asset/gh/GXI/JUNGLE.GXI" );
		SetDRQStage( "ScenJungle.txt" );

		m_pHound[enHoundMaster] = NULL;
		m_pHound[enHoundSlave ] = NULL;
		m_pTank[enHeavyTankMaster] = NULL;
		m_pTank[enHeavyTankSlave ] = NULL;

		CCockpit::GetInstance()->SetMissionSec( 60*5 );
		//g_pActManager->Clear(enActionPrioDefault);
		CActManager::GetInstance()->Clear(enActionPrioDefault);
		viiMus::StopBGM( enSoundBgm1 );
		//viiMus::PlayBGM( enSoundBgm2,enMusicBgmBossSimple);
		CCockpit::GetInstance()->SetMissionSec( 5*60 );
		break;

	case enAreaSeq05:
	case enAreaTest:
		CGXImage::GetInstance()->Load( "asset/gh/GXI/JUNGLE.GXI" );
		SetDRQStage( "ScenJungle.txt" );
		return;
	}

	SetGamePlay( gxTrue );

	m_bFirst = gxFalse;

}


void CStageStart::Action()
{
	//---------------------------------------------------------------------------
	//���t���[���̃A�N�V����
	//---------------------------------------------------------------------------
	//�G���A�O������o���Ȃ�
	CCockpit::GetInstance()->SetOperationRange(0*100,16000*100);

	StageCommon();
	AreaCommon();

	switch(GetArea( )){
	case enAreaSeq01:
		Area01();
		break;

	case enAreaSeq02:
		Area02();
		break;

	case enAreaSeq03:
		Area03();
		break;

	case enAreaSeq04:
		Area04();
		break;

	case enAreaSeq05:
		Area05();
		break;
	case enAreaTest:
		AreaTest();
		break;
	}

}

void CStageStart::Area01()
{
	//---------------------------------------------------------------------------
	//�G���A�P�@�`�o��V�[��
	//---------------------------------------------------------------------------

//	CCockpit::GetInstance()->SetOperationRange( m_pTank[enHeavyTankSlave]->m_Pos.x - 600*100 , m_pTank[enHeavyTankSlave]->m_Pos.x + 1200*100);

	if( IsZoneSeq(0) )
	{
		//----------------------------------
		//������
		//----------------------------------
		SetGamePlay( gxFalse );
		m_sScrollPos = 0;

		m_sScrollYMin = 280*100;
		m_sScrollYMax = m_sScrollYMin;

		m_sScrollMin = (180)*100+m_sScrollPos;
		m_sScrollMax = m_sScrollMin;

		viiSub::SetScroll_l( m_sScrollMin , m_sScrollYMin );

		CCockpit::GetInstance()->SetBlackMask( gxTrue );

		m_sBombPosition = 0;
		pGame->GetHound()->SetPadButton(0);

//		m_pPlayerFly[enHoundSlave] = new CEneH0503HoundFly(  -100*100 , 340*100 );
		m_pPlayerFly[enHoundSlave] = new CEneH0503HoundFly(   80*100 , 200*100 );
		m_pPlayerFly[enHoundSlave]->SetCarryTarget( &m_pHound[enHoundSlave]->m_Pos.x , &m_pHound[enHoundSlave]->m_Pos.y );
		m_pPlayerFly[enHoundSlave]->SetColor(enBlaclColor);
//		m_pHound[enHoundSlave]->GetLeynos()->CarryOn();
//		m_pHound[enHoundSlave]->GetLeynos()->SetARGB(enBlaclColor);


//		m_pPlayerFly[enHoundMaster] = new CEneH0503HoundFly( -450*100 , 380*100 );
		m_pPlayerFly[enHoundMaster] = new CEneH0503HoundFly( -232*100 , 200*100 );
		m_pPlayerFly[enHoundMaster]->SetCarryTarget( &m_pHound[enHoundMaster]->m_Pos.x , &m_pHound[enHoundMaster]->m_Pos.y );
		m_pPlayerFly[enHoundMaster]->SetColor(enBlaclColor);

//		m_pHound[enHoundMaster]->GetLeynos()->CarryOn();
//		m_pHound[enHoundMaster]->GetLeynos()->SetARGB(enBlaclColor);

//		m_pPlayerFly[enHoundPlayer] = new CEneH0503HoundFly(  -800*100 , 400*100 );
		m_pPlayerFly[enHoundPlayer] = new CEneH0503HoundFly(  -578*100 , 120*100 );
		m_pPlayerFly[enHoundPlayer]->SetCarryTarget( &pGame->GetHound()->m_Pos.x , &pGame->GetHound()->m_Pos.y );
//		m_pPlayerFly[enHoundPlayer]->SetColor(enBlaclColor);

		ControlHound(enSetCarryOn);

		m_pYagura  =new CEneH0106Yagura( 2966*100 , 600*100 );

		m_pTransHeli[0] = new CEneH0005TransHeli(   0*100  , 458*100 );
		m_pTransHeli[0]->SetMaxSpeed(420);

		m_pTransHeli[1] = new CEneH0005TransHeli(  100*100  , 408*100 );
		m_pTransHeli[1] ->SetMaxSpeed(280);

		m_pTransHeli[2] = new CEneH0005TransHeli(  -300*100 , 428*100 );
		m_pTransHeli[2] ->SetMaxSpeed(320);

		BgCommon();
		ResetZoneTimer();
		SetZoneSeq(100);
		return;
	}
	else if( IsZoneSeq(100) )
	{
		//----------------------------------
		//�n�E���h�t���C�X�V
		//----------------------------------
		pGame->GetHound()->SetPadButton(0);
		m_sScrollPos += 35;
		m_pPlayerFly[enHoundSlave]->m_Pos.x   += 80*120/100;
		m_pPlayerFly[enHoundSlave]->m_Pos.y   += (340*100 - m_pPlayerFly[enHoundSlave]->m_Pos.y) /120;

		m_pPlayerFly[enHoundMaster ]->m_Pos.x += 80*160/100;
		m_pPlayerFly[enHoundMaster]->m_Pos.y   += (380*100 - m_pPlayerFly[enHoundMaster]->m_Pos.y) /120;

		m_pPlayerFly[enHoundPlayer]->m_Pos.x  += 80*195/100;
		m_pPlayerFly[enHoundPlayer]->m_Pos.y   += (400*100 - m_pPlayerFly[enHoundPlayer]->m_Pos.y) /120;

		m_sScrollMin = (180)*100+m_sScrollPos;
		m_sScrollMax = m_sScrollMin;

		switch( m_sZoneTimer ){
		case 120:
			CCockpit::GetInstance()->SetMessage(1050);	//�{���͗\��ʂ�˓��|�C���g�֓����B
			break;
		case 30:
			viiMus::PlayBGM(enSoundBgm1,enMusicBgmStageA);
			break;
		case 230:
		case 255:
		case 320:
			pGame->QuakeOn(64);
			viiMus::PlaySound( enSoundExplosionLarge );
			break;

		case 260:
		case 290:
			viiMus::PlaySound(enSoundExplosionMiddle);
			break;
		case 330:
			viiMus::PlaySound( enSoundCrashA );
			break;

		case 140:
			m_pTransHeli[0] ->SetBomb();
			viiMus::PlaySound( enSoundMissile );
			break;
		case 165:
			m_pTransHeli[1] ->SetBomb();
			viiMus::PlaySound( enSoundMissile );
			break;
		case 240:
			m_pTransHeli[2] ->SetBomb();
			viiMus::PlaySound( enSoundMissile );
			break;
		}

		if( viiSub::Rand()%6 == 0  && m_sZoneTimer <= 350 )
		{
			CEffEneTwinGun *pTwinGun;
			Sint32 ax,ay;
			ax = m_sScrollMin+(WINDOW_W/2+viiSub::Rand()%WINDOW_W)*100;
			ay = m_sScrollYMin+WINDOW_H*100;
			pTwinGun = new CEffEneTwinGun( ax , ay , 180+45 , 920 );
			pTwinGun->SetAtariOff();
		}

		if( m_sZoneTimer == 460 )
		{
			CEffFire *p;
			p = new CEffFire( 726*100,638*100 , PRIO_BG2+1 );
			p->SetScale(2.f);

			p = new CEffFire( 1200*100,655*100 , PRIO_BG2-1 );
			p->SetScale(2.5f);

			p = new CEffFire( 900*100,655*100 , PRIO_BG2-1 );
			p->SetScale(3.f);

			p = new CEffFire( 1500*100,655*100 , PRIO_BG2-1 );
			p->SetScale(2.f);

			ResetZoneTimer();
			SetZoneSeq(200);
		}

		viiMus::GetInstance()->PlayLoopSound( enSoundHeliRoter , 108 );
		BgCommon();
		return;
	}
	else if( IsZoneSeq(200) )
	{
		//----------------------------------
		//�X�N���[���ʒu������
		//----------------------------------
		pGame->GetHound()->SetPadButton(0);
		m_sScrollPos += 80;
		m_sScrollMin = (145)*100+m_sScrollPos;
		m_sScrollMax = (145)*100+m_sScrollPos;

		m_pPlayerFly[enHoundSlave]->m_Pos.x   += (120);
		m_pPlayerFly[enHoundMaster ]->m_Pos.x += (180);
		m_pPlayerFly[enHoundPlayer]->m_Pos.x  += (195);

		if( m_sZoneTimer == 180 )
		{
/*
			for(Sint32 ii=0;ii<4;ii++)
			{
				CSoldierBase *q;
				q = new CSoldierBase( );
				q->SetPosition( 0 , (720+ii*60)*100, 650*100 , NULL);
				q->SetForce(enForceEnemy);
				q->SetTargetPosition( 9600*100 );
				q->ChangeMoveType( enMoveTypeEscape );
				q->SetEnd();
			}
*/
		}

		if( m_sScrollYMin < 450*100 )
		{
			m_sScrollYMin += 50;
			m_sScrollYMax = m_sScrollYMin;
		}
		else
		{
			ResetZoneTimer();
			SetZoneSeq(300);
		}

		viiMus::GetInstance()->PlayLoopSound( enSoundHeliRoter , 108 );
		BgCommon();
		return;
	}
	else if( IsZoneSeq(300) )
	{
		//----------------------------------
		//�n�E���h����
		//----------------------------------
		pGame->GetHound()->SetPadButton(0);
		switch( m_sZoneTimer){
		case 20:
			m_pPlayerFly[enHoundMaster]->SetCarryOff();
//			m_pHound[enHoundMaster]->GetLeynos()->SetARGB(ARGB_DFLT);
//			m_pHound[enHoundMaster]->GetLeynos()->CarryOff();
			m_pHound[enHoundMaster]->SetCarryOff();
			m_pHound[enHoundMaster]->SetColor(ARGB_DFLT);
			break;

		case 60:
			m_pPlayerFly[enHoundPlayer]->SetCarryOff();
			pGame->GetHound()->SetCarryOff();
			pGame->GetHound()->SetColor(ARGB_DFLT);
			break;

		case 110:
			m_pPlayerFly[enHoundSlave]->SetCarryOff();
//			m_pHound[enHoundSlave]->GetLeynos()->SetARGB(ARGB_DFLT);
//			m_pHound[enHoundSlave]->GetLeynos()->CarryOff();
			m_pHound[enHoundSlave]->SetCarryOff();
			m_pHound[enHoundSlave]->SetColor(ARGB_DFLT);
			break;
		case 120:
			SetZoneSeq(400);
			break;
		}

		viiMus::GetInstance()->PlayLoopSound( enSoundHeliRoter , 108 );
		BgCommon();
		return;
	}
	else if( IsZoneSeq(400) )
	{
		//----------------------------------
		//���o�I��
		//----------------------------------
		//�����w���I��
		m_pTransHeli[0] ->Clear();
		m_pTransHeli[1] ->Clear();
		m_pTransHeli[2] ->Clear();

		m_pTransHeli[0] = NULL;
		m_pTransHeli[1] = NULL;
		m_pTransHeli[2] = NULL;

		CCockpit::GetInstance()->SetBlackMask( gxFalse );
		ResetZoneTimer();
		SetZoneSeq( 1000 );

		viiMus::GetInstance()->PlayLoopSound( enSoundHeliRoter , 108 );
		BgCommon();
		return;
	}
	else if( IsZoneSeq(1000) )
	{
		SetGamePlay( gxTrue );
		m_sScrollMax = 2550*100;
		CCockpit::GetInstance()->SetMissionSec( 60*10 );

		viiMus::GetInstance()->PlayLoopSound( enSoundHeliRoter , 108 );

		if( m_sZoneTimer == 60 )
		{
			CCockpit::GetInstance()->SetMessage(1100);	//�����烉�C�U�A�ڕW�n�_�ɓ��B�I
		}

		if( m_sZoneTimer == 120 )
		{
			SetZoneSeq(1200);
		}
	}
	else if( IsZoneSeq(1200) )
	{
		SetZoneSeq( 1300 );
	}
	else if( IsZoneSeq( 1300 ) )
	{
		//----------------------------------------
		//�ڕW�n�_�`�i���ѓ����j
		//----------------------------------------
		ControlHound( enSetTraceGriffon );

		m_sScrollMax = 2550*100;

		if( viiSub::Rand()%128==0) m_sFlashAlpha = 0xa0;

		gxBool bFlag = gxFalse;

		if( m_pYagura && m_pYagura->IsDead() )
		{
			bFlag = gxTrue;
		}

		if( bFlag )
		{
			//CCockpit::GetInstance()->SetMessage(1100);	//���т𔲂����炱�����͊ی�����B

			//--------------------------------------
			//�^���N�K�[�h�U���J�n
			//--------------------------------------
//			for(Sint32 ii=0;ii<enTankGuardMax;ii++)
//			{
//				m_pTankGuard[ii]->SetRealAtack();
//			}

			m_sScrollMax = 5800*100;
			SetZoneSeq(1400);
		}
	}
	else if( IsZoneSeq( 1400 ) )
	{
		//----------------------------------------
		//�ڕW�n�_�a�i���p�_�j�ɓ��B
		//----------------------------------------
		ControlHound( enSetFreeActionStart );

		m_sScrollYMin = 150*100;
		m_sScrollYMax = 450*100;
		if(GetTargetPlayer()->x < 3200*100)
		{
			CCockpit::GetInstance()->SetArrow( 0 );
			//AddMissionSec( 1, 60*3 );
		}

		Sint32 sx,sy;
		viiSub::GetScroll_s( sx,sy );

		if( sx >= 5700 )
		{
			ChangeArea( enAreaSeq02 );
		}
	}

	BgCommon();
	viiSub::LimitMoveOnlyV();

}

void CStageStart::Area02()
{
	//---------------------------------------------------------------------------
	//�G���A�Q�@�`�C��܂Łi��Ԍ�q�j
	//---------------------------------------------------------------------------
	m_sScrollMin = 5500*100;

	if( IsZoneSeq( enZoneSeqVsGrazziInit ) )
	{
		//----------------------------------------
		//�O���[�W�o��
		//----------------------------------------
		m_sScrollMax = 5800*100;

		if( m_pGrazzi == NULL )
		{
			m_pGrazzi = new CEneH0204Grazzi(6500*100,654*100);
			m_pGrazzi->SetMoveArea( 5800*100 , 6200*100 );

			//�V���b�^�[�쐬
			m_pWall   = new CEneH0410Wall( 7132*100 , 0*100 );
			SetZoneSeq( enZoneSeqVsGrazziBattle );
		}
	}
	else if( IsZoneSeq( enZoneSeqVsGrazziBattle ) )
	{
		//-------------------------------------
		//�O���[�W�j��҂�
		//-------------------------------------

//		m_sScrollMax = m_pGrazzi->m_Pos.x-420*100;
//		m_sScrollMin = m_pGrazzi->m_Pos.x-420*100;

		if( m_pGrazzi->m_Pos.x < 6300*100 )
		{
			if( !CCockpit::GetInstance()->IsMessageNow() )
			{
				CCockpit::GetInstance()->SetMessage(1300);	//�z�������ȏ�A�N��������ȁI�I
			}
		}

		if( m_pGrazzi->IsDead())
		{
			new CEffScene0A02(320*100,160*100,PRIO_BG1-1,0);
			m_bBgColorChange = gxTrue;

			m_pGrazzi->Clear();
			m_pGrazzi = NULL;

			ResetZoneTimer();
			SetZoneSeq( enZoneSeqVsGrazziEnd );
		}
		else
		{
			if( m_pTank[enHeavyTankSlave]->IsMokutekichi() )
			{
				if( !CCockpit::GetInstance()->IsMessageNow() )
				{
					CCockpit::GetInstance()->SetMessage(1350);	//������A���w���B�����n�_�ɓG�e�A
				}
			}
		}
	}
	else if( IsZoneSeq( enZoneSeqVsGrazziEnd ) )
	{
		//-------------------------------------
		//�O���[�W�j�󒼌�
		//-------------------------------------
		m_sScrollMax = 6760*100;
		m_sScrollYMin = 0*100;
		m_sScrollYMax = 9600*100;

		if( 100 == m_sZoneTimer )
		{
			//�܂��A���w�������Ă��Ȃ��ꍇ
			if( !m_pTank[enHeavyTankSlave]->IsMokutekichi() )
			{
				CCockpit::GetInstance()->SetMessage(1400);	//�����n�_���m�ۂ������B�A���w���͂܂����H
			}
		}

		if( m_pTank[enHeavyTankSlave]->IsMokutekichi() )
		{
			if( !m_pYusouHeli->IsLanding() )
			{
				//��������
				m_pYusouHeli->SetLanding();

				//�J�o�[����Ĕ��j�҂�
				m_sCoveredSeq = 1;
				m_pWall->SetCover();
			}

			ResetZoneTimer();
			SetZoneSeq(enZoneSeqCannonStart);
		}

		if(m_pWall->IsDead())
		{
			//---------------------------------------------------
			//�㋉�҃R�[�X
			//�w�������܂łɃV���b�^�[���j�󂳂�Ă���Ύ��֐i�s����
			//---------------------------------------------------
			CCockpit::GetInstance()->SetMessage(1850);	//�S�@�A�v�����g���̃j���[�g�����|���}�[
			SetZoneSeq(enZoneSeqWallWait);
		}

	}
	else if( IsZoneSeq(enZoneSeqCannonStart) )
	{
		//----------------------------------------
		//�O���t�H���C���J�n
		//----------------------------------------

		m_pTank[enHeavyTankMaster]->SetHougeki();
		m_pTank[enHeavyTankSlave ]->SetHougeki();

		ResetZoneTimer();

		SetZoneSeq(enZoneSeqYusouHeliCome);
	}
	else if( IsZoneSeq(enZoneSeqYusouHeliCome) )
	{
		//----------------------------------------
		//�w�������҂�
		//----------------------------------------

		if(m_pYusouHeli->IsSoldierRideOff())
		{
			//--------------------------------------
			//�\���W���[�W���҂�
			//--------------------------------------

			if( m_sZoneTimer >= 300 )
			{
				if( m_pYusouHeli->GetSoldierLeader() )
				{
					m_pYusouHeli->GetSoldierLeader()->SetTargetPosition( 6916*100 );
					m_pYusouHeli->GetSoldierLeader()->CommandMember( );
					m_pYusouHeli->GetSoldierLeader()->SetTargetPosition( 6948*100 );

					CCockpit::GetInstance()->SetMessage( 1600 );	//�����I�@�Δ��p�̓���u�ǂ��B

					ResetZoneTimer();
					SetZoneSeq(enZoneSeqBleachClear);
				}
			}
		}
	}
	else if( IsZoneSeq(enZoneSeqBleachClear) )
	{
		//--------------------------------------
		//�u���[�`�N���A
		//--------------------------------------

		if( 1000 == m_sZoneTimer )
		{
			m_pYusouHeli->GetSoldierLeader()->CommandMember( enMoveTypeBombSet );
		}
		else if( 1600 <= m_sZoneTimer )
		{
			if( m_pYusouHeli->GetSoldierLeader()->GetMember( 0 )->GetSeq()==40 )
			{
				m_pYusouHeli->GetSoldierLeader()->SetTargetPosition( 6000*100 );
				m_pYusouHeli->GetSoldierLeader()->CommandMember( );
				m_pYusouHeli->GetSoldierLeader()->SetTargetPosition( 6000*100 );
				ResetZoneTimer();
				CCockpit::GetInstance()->SetMessage(1700);	//�b�S�O������Z�b�g�A
				SetZoneSeq(enZoneSeqExplosion);
			}
		}
	}
	else if( IsZoneSeq(enZoneSeqExplosion) )
	{
		//--------------------------------------
		//�V���b�^�[�j��
		//--------------------------------------

		if( 300 == m_sZoneTimer )
		{
			//�����e����
			viiMus::PlaySound( enSoundCrashB );
			new CEffectFinalExplosion(7150*100, 530*100 ,3);
		}
		else if( 400 == m_sZoneTimer )
		{
			//�j��G�t�F�N�g���o��
			m_pWall->ForceCrash();

			SetCrashDoorEffect();

			//�v�����g���ɉ΂����
			//SetFirePlantEffect();
		}
		else if( 500 == m_sZoneTimer )
		{
			CCockpit::GetInstance()->SetMessage(1800);	//�d���V���b�^�[�̏������m�F�B
			SetZoneSeq(enZoneSeqWallWait);
		}
	}
	else if( IsZoneSeq(enZoneSeqWallWait) )
	{
		if( m_pWall->IsDead() )
		{
			SetZoneSeq(enZoneSeqAssault);
		}

	}
	else if( IsZoneSeq(enZoneSeqAssault) )
	{
		m_sScrollMax = 9300*100;

		CCockpit::GetInstance()->SetArrow( 0 );
		//AddMissionSec( 2, 60*3 );

		Sint32 sx,sy;
		viiSub::GetScroll_s( sx,sy );
		if(sx>7250)
		{
			ChangeArea( enAreaSeq03 );
		}
	}


	//-----------------------------------------------
	//�V���b�^�[����
	//-----------------------------------------------

	BgCommon();
	viiSub::LimitMoveVH();
}


void CStageStart::Area03()
{
	//-----------------------------------------------------
	//�G���A�R�F�v�����g����
	//-----------------------------------------------------
	m_sScrollMin = 7250*100;
	m_sScrollMax = 9300*100;

//	CCockpit::GetInstance()->SetOperationRange( m_sScrollMin,m_sScrollMax);

	if( m_sCoveredSeq == 1 )
	{
		CCockpit::GetInstance()->SetMessage(1900);	//�S�@�A�v�����g���̃j���[�g�����|���}�[
	}

	ControlHound( enSetFreeActionStop );

	if( IsZoneSeq(0) )
	{
		//-------------------------------
		//�~�b�V�����^�C���X�V
		//-------------------------------
		SetFirePlantEffect();
		SetZoneSeq( 100 );
	}
	else if( IsZoneSeq(100) )
	{
		if( IsAreaArrive(8000,0,50,1000) )
		{
			CCockpit::GetInstance()->SetMessage(1900);
		}

		if( IsAreaArrive(9750,680,50,320) )
		{
			new CEntranceControl( CEntranceControl::enExit , DIR_RIGHT );
			viiMus::StopBGM( enSoundBgm1 , 90 );
			SetFadeOut();
			SetZoneSeq( 200 );
		}
	}
	else if( IsZoneSeq(200) )
	{
		//�t�F�[�h�҂�
		if( IsFadeEnd() )
		{
			if( !viiMus::IsPlayBGM( enSoundBgm1 ) )
			{
				CCockpit::GetInstance()->ResetMessageWindow();
				//g_pActManager->Clear(enActionPrioDefault);
				CActManager::GetInstance()->Clear(enActionPrioDefault);
				m_pYusouHeli = NULL;
				m_pGrazzi    = NULL;
				m_pWall      = NULL;
				m_pYagura = NULL;
				m_pHound[enHoundMaster]    = NULL;
				m_pHound[enHoundSlave ]    = NULL;
				m_pTank[enHeavyTankMaster] = NULL;
				m_pTank[enHeavyTankSlave ] = NULL;
				ChangeArea( enAreaSeq04 );
			}
			return;
		}
	}

//	CCockpit::GetInstance()->SetOperationRange( m_sScrollMin,m_sScrollMax);

	BgCommon();
	viiSub::LimitMoveVH();

}


void CStageStart::Area04()
{
	//-----------------------------------------------------
	//�G���A�S�F�{�X�G���A
	//-----------------------------------------------------

	m_sScrollMax = 11000*100;
	m_sScrollMin = 9856*100;

//	CCockpit::GetInstance()->SetOperationRange( m_sScrollMin,m_sScrollMax);
	LayerScroll[enBgTypeRear].ox  = 2000*100;
	LayerScroll[enBgTypeRear].oy  = 0*100;
	LayerScroll[enBgTypeRear].x  = 80;
	LayerScroll[enBgTypeRear].y  = 100;
	LayerScroll[enBgTypeFrnt].x  = 100;
	LayerScroll[enBgTypeFrnt].y  = 100;
	LayerScroll[enBgTypeFrnt].ox  = 1164*100;
	LayerScroll[enBgTypeFrnt].oy  = 0;

	if( IsZoneSeq(0) )
	{
		//g_pActManager->Clear(enActionPrioDefault);
		CActManager::GetInstance()->Clear(enActionPrioDefault);

		SetFadeIn();
		SetTargetPlayerPosX( 9700*100 );
		SetTargetPlayerPosY( (895-32)*100);

		new CEntranceControl( CEntranceControl::enEntrance , DIR_RIGHT );
		viiSub::SetScroll_l( 9847*100,665*100 );//GetTargetPlayer()->x , GetTargetPlayer()->y );

		m_pNightSpider = new CEneH0102NightSpider( 10845*100, 825*100 );

		SetZoneSeq(10);
	}
	else if( IsZoneSeq(10) )
	{
		if( m_sZoneTimer == 20 )
		{
			viiMus::PlayBGM( enSoundBgm2,enMusicBgmBossSimple);
		}

		if( m_sZoneTimer == 120 )
		{
			CCockpit::GetInstance()->SetMessage(2000);	//������p�[�v���Q�A�i�[�ɂɐ헪�@������𔭌��I�I
		}

		if( m_pNightSpider->IsDead() )
		{
			m_pNightSpider->Clear();
			m_pNightSpider = NULL;
			ChangeArea( enAreaSeq05 );
		}
	}
	else if( IsZoneSeq(20) )
	{

	}


	BgCommon();
	viiSub::LimitMoveVH();

}

void CStageStart::Area05()
{
	//-----------------------------------------------------
	//�G���A�T�F�X�e�[�W�N���A
	//-----------------------------------------------------

	if( IsZoneSeq( 0 ) )
	{
		//�N���A
		if( m_sZoneTimer >= 60 )
		{
			SetZoneSeq( 100 );
		}
	}
	else if( IsZoneSeq( 100 ) )
	{
		SetGamePlay( gxFalse );

#ifdef _TRIAL_VERSION_
		CCockpit::GetInstance()->SetMessage(3200);	//�I�y���[�V�����E���|�[�g�B�\�񂹂�
#else
		CCockpit::GetInstance()->SetMessage(3000);	//�I�y���[�V�����E���|�[�g�B
#endif
		CCockpit::GetInstance()->SetStageClear();
		SetZoneSeq( 200 );

	}
	else if( IsZoneSeq( 200 ) )
	{
//		if( !CCockpit::GetInstance()->IsMessageNow() )
		{
			if( CCockpit::GetInstance()->IsClearOver() )
			{
				CCockpit::GetInstance()->DestroyStageClear();
				SetFadeOut(2);
				SetZoneSeq( 300 );
			}
		}
	}
	else if( IsZoneSeq( 300 ) )
	{
		//�t�F�[�h�I���҂�
		if( IsFadeEnd() )
		{
			pGame->stage_clear(enClearSeqStageClear);
			ChangeArea( GAME_NONE );	//�X�e�[�W�I��
		}
	}

}


gxBool CStageStart::WaraWaraSpider()
{
	//----------------------------------------
	//���������X�p�C�_�[�Q�[��
	//----------------------------------------

	return gxFalse;
}


void CStageStart::SetCrashDoorEffect()
{
	//---------------------------------------
	//�V���b�^�[�j��G�t�F�N�g
	//---------------------------------------

	new CEffectDmgprt( 7150*100, 530*100, 180+15 , 800 );
	new CEffectDmgprt( 7150*100, 530*100, 180+45 , 600 );
	new CEffectDmgprt( 7150*100, 530*100, 180-15 , 500 );
	new CEffectDmgprt( 7150*100, 530*100, 180-32 , 700 );

}

void CStageStart::SetFirePlantEffect()
{
	//---------------------------------------
	//�v�����g�����ɉ΂����
	//---------------------------------------
	new CEffectBlackExplosion( 4200*100,600*100 , &m_BgLayerPos );

	EffectSun((4200-340+viiSub::Rand()%(WINDOW_W*2))*100,500*100 , &m_BgLayerPos );
	EffectSun((4200-340+viiSub::Rand()%(WINDOW_W*2))*100,500*100 , &m_BgLayerPos );

	new CEffFire( 7503*100,582*100 , PRIO_BG2-1 );
	new CEffFire( 7583*100,582*100 , PRIO_BG2-1 );
	new CEffFire( 7420*100,582*100 , PRIO_BG2-1 );

	new CEffFire( 8000*100,582*100 , PRIO_BG2 );
	new CEffFire( 8080*100,582*100 , PRIO_EFFECT );
	new CEffFire( 8170*100,582*100 , PRIO_BG2 );

	new CEffFire( 8484*100,582*100 , PRIO_BG2-1 );
	new CEffFire( 8554*100,582*100 , PRIO_BG2-1 );
	new CEffFire( 8620*100,582*100 , PRIO_BG2-1 );


//	new CEffFire( 9720*100,455*100 , PRIO_ENEMY );

}


void CStageStart::ControlHound(Sint32 sOperation )
{
	//-----------------------------------------
	//�n�E���h�̃R���g���[��
	//-----------------------------------------

	switch(sOperation){
	case enSetHoundArea1:
		//-------------------------------
		//�V�K�n�E���h���Z�b�g
		//-------------------------------
		m_pHound[enHoundMaster] = new CGunHound( 300*100,100*100 ,gxFalse);
		m_pHound[enHoundMaster]->SetLeader(gxTrue);

		m_pHound[enHoundSlave] = new CGunHound( 300*100,100*100 ,gxFalse);
		break;

	case enSetCarryOn:
		//-------------------------------
		//�邳��Ă���
		//-------------------------------
		m_pHound[enHoundSlave]->SetMode( CGunHound::enHoundModeCarreer );
		m_pHound[enHoundSlave]->SetColor(enBlaclColor);
		m_pHound[enHoundMaster]->SetMode( CGunHound::enHoundModeCarreer );
		m_pHound[enHoundMaster]->SetColor(enBlaclColor);
		pGame->GetHound()->SetMode( CGunHound::enHoundModeCarreer );
		pGame->GetHound()->SetColor(enBlaclColor);
		m_pHound[enHoundMaster]->m_Pos.z = PRIO_BG2-1;
		m_pHound[enHoundSlave ]->m_Pos.z = PRIO_BG2-1;
		break;

	case enSetTraceGriffon:
		//-------------------------------
		//�O���t�H������q
		//-------------------------------
		if( m_pHound[enHoundMaster] == NULL ) m_pHound[enHoundMaster] = new CGunHound( 300*100,100*100 ,gxFalse);
		m_pHound[enHoundMaster]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundMaster]->SetLogicPattern( 101 );
		if( viiSub::Rand()%120 == 0 )
		{
			m_pHound[enHoundMaster]->SetTarget( m_pTank[enHeavyTankSlave]->m_Pos.x+320*100 , m_pTank[enHeavyTankSlave]->m_Pos.y );
		}

		m_pHound[enHoundSlave]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundSlave ]->SetLogicPattern( 101 );
		if( viiSub::Rand()%120 == 0 )
		{
			m_pHound[enHoundSlave ]->SetTarget( m_pTank[enHeavyTankMaster ]->m_Pos.x+320*100 , m_pTank[enHeavyTankMaster ]->m_Pos.y );
		}
		break;

	case enSetFreeActionStart:
		//---------------------------------------------------------------
		//�o�c�@�Ǐ]
		//---------------------------------------------------------------
		if( m_pHound[enHoundSlave] == NULL ) m_pHound[enHoundSlave] = new CGunHound( 300*100,100*100 ,gxFalse);
		m_pHound[enHoundSlave]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundSlave ]->SetLogicPattern( 150 );
		break;

	case enSetFreeActionStop:
		//---------------------------------------------------------------
		//�o�c�@�Ǐ]�i��~�j
		//---------------------------------------------------------------
		if( m_pHound[enHoundSlave] == NULL ) m_pHound[enHoundSlave] = new CGunHound( 300*100,100*100 ,gxFalse);
		m_pHound[enHoundSlave]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundSlave ]->SetLogicPattern( 101 );
		if( viiSub::Rand()%120 == 0 )
		{
			if( m_pTank[enHeavyTankMaster] == NULL ) m_pTank[enHeavyTankMaster] = new CEneH0101HeavyTank( enGriffonStartPoint*100,650*100);
			m_pHound[enHoundSlave ]->SetTarget( m_pTank[enHeavyTankMaster ]->m_Pos.x+320*100 , m_pTank[enHeavyTankMaster ]->m_Pos.y );
		}
		break;


#if 0

	case enSetHoundArea2:
		m_pHound[enHoundMaster] = new CGunHound( 300*100,100*100 ,gxFalse);
		m_pHound[enHoundMaster]->SetLeader(gxTrue);

		m_pHound[enHoundSlave] = new CGunHound( 300*100,100*100 ,gxFalse);

		m_pHound[enHoundMaster]->m_Pos.x = 5300*100;
		m_pHound[enHoundMaster]->m_Pos.y = 600*100;

/*
		m_pHound[enHoundSlave]->m_Pos.x = 5300*100;
		m_pHound[enHoundSlave]->m_Pos.y = 600*100;

		m_pTank[enHeavyTankMaster]->m_Pos.x = 3200*100;
		m_pTank[enHeavyTankSlave ]->m_Pos.x = (3200+400)*100;
*/
		break;

	case enSetSankai:
		//-------------------------------
		//���т𔲂��ĎU��
		//-------------------------------
		m_pHound[enHoundSlave ]->SetLogicPattern( 150 );
		break;

	case enSetHougekichu:
		//-------------------------------
		//�C���J�n�Œ�C�䉻
		//-------------------------------
		m_TargetPos.x = 6360*100;
		m_TargetPos.y = 400*100;
		break;

	case enSetGrazziAtack:
		//-------------------------------
		//�O���[�W�j��s��
		//-------------------------------
//		m_pHound[enHoundMaster]->SetMode( CGunHound::enHoundModeNPC );
//		m_pHound[enHoundMaster]->SetLogicPattern( 110 );

		m_pHound[enHoundSlave ]->SetLogicPattern( 150 );

		m_pHound[enHoundMaster]->m_Pos.z = PRIO_BG2+1;
		{
			if( m_pGrazzi )
			{
				m_pHound[enHoundMaster]->SetTarget( m_pGrazzi->m_Pos.x+(-16+viiSub::Rand()%32)*2000 , m_pGrazzi->m_Pos.y );
				m_pHound[enHoundMaster]->SetTarget( m_pGrazzi->m_Pos.x, m_pGrazzi->m_Pos.y );
			}
		}

		m_pHound[enHoundMaster]->SetMode( CGunHound::enHoundModeNPC );
		m_pHound[enHoundMaster ]->SetLogicPattern( 101 );
		if( viiSub::Rand()%120 == 0 )
		{
			m_pHound[enHoundMaster ]->SetTarget( m_pTank[enHeavyTankMaster ]->m_Pos.x+320*100 , m_pTank[enHeavyTankMaster ]->m_Pos.y );
		}
		break;

	case enSetGrazziDestroy:
		//-------------------------------
		//�O���[�W�j���V���b�^�[���U��
		//-------------------------------
/*
		{
			Sint32 sPos = 6650*100;
			Sint32 sDist = sPos - pGame->GetHound()->m_Pos.x;
			m_pHound[enHoundMaster]->SetMode( CGunHound::enHoundModeNPC );
			m_pHound[enHoundMaster]->SetLogicPattern( 120 );
			m_pHound[enHoundMaster]->SetTarget( sPos+sDist, pGame->GetHound()->m_Pos.y );

			m_pHound[enHoundSlave]->SetMode( CGunHound::enHoundModeNPC );
			m_pHound[enHoundSlave ]->SetLogicPattern( 101 );
			if( viiSub::Rand()%120 == 0 )
			{
				m_pHound[enHoundSlave ]->SetTarget( m_pTank[enHeavyTankMaster ]->m_Pos.x+320*100 , m_pTank[enHeavyTankMaster ]->m_Pos.y );
			}
		}
*/
		m_pHound[enHoundSlave ]->SetLogicPattern( 150 );
		break;

	case enSetInthePlant:
		if( m_pHound[enHoundSlave] )
		{
			delete m_pHound[enHoundSlave];
			m_pHound[enHoundSlave] = NULL;
		}
		if( m_pHound[enHoundMaster] )
		{
			delete m_pHound[enHoundMaster];
			m_pHound[enHoundMaster] = NULL;
		}
		if( m_pTank[enHeavyTankMaster] )
		{
			delete m_pTank[enHeavyTankMaster];
			m_pTank[enHeavyTankMaster] = NULL;
		}
		if( m_pTank[enHeavyTankSlave] )
		{
			delete m_pTank[enHeavyTankSlave];
			m_pTank[enHeavyTankSlave] = NULL;
		}
		break;
#endif

	}
}

void CStageStart::AreaCommon()
{
	//-----------------------------------------------------
	//�G���A�F����
	//-----------------------------------------------------

	if( m_sFlashAlpha > 0 )
	{
		//------------------------------------------
		//�t���b�V��
		//------------------------------------------
		Sint32 sAdd = -m_sFlashAlpha/10;
		if( sAdd == 0 ) sAdd = -1;
		m_sFlashAlpha += sAdd;
	}

	if( m_bBgColorChange )
	{
		m_sMorphColorCnt += 8;
		if( m_sMorphColorCnt >= 100*100 )
		{
			m_sMorphColorCnt = 100*100;
		}

		Sint32 col1,col2;

		col1 = viiSub::GetMorphingColor( m_sMorphColorCnt/100 , enBgColor01 , enBgColor02 );
		col2 = viiSub::GetMorphingColor( m_sMorphColorCnt/100 , enBgColor03 , enBgColor04 );

		pGame->pBg->SetSkyColor( 50 , col1 , col2 );
	}

	if( m_pTank[enHeavyTankSlave] )
	{
		if( m_pTank[enHeavyTankSlave]->m_Pos.x >= 3200*100 )
		{
			if( !CCockpit::GetInstance()->IsMessageNow() )
			{
				CCockpit::GetInstance()->SetMessage(1200);	//�܂������g����������Ԃ��A
			}

			if( m_pYusouHeli == NULL )
			{
				m_pYusouHeli = new CEneH0007YusouHeli( -1100*100 , 446*100 );
				m_pYusouHeli->SetTargetPos( 6200*100 );
			}
		}
	}

}


void CStageStart::Draw()
{

	//-------------------------------------
	//��
	//-------------------------------------

	viiDraw::Spr_NoScrl( &StSprCloud[ 0 ] , 0,0,PRIO_BG1-1 , ATR_DFLT , 0x40808080 );


	//-------------------------------------
	//�t���b�V��
	//-------------------------------------

	if( m_sFlashAlpha > 0 )
	{
		viiDraw::BoxScrl_s( 0 , 500 , 3000 , 1000 , PRIO_BG1+1 , gxTrue , ARGB(m_sFlashAlpha,0xa0,0xa0,0xa0) , ATR_ALPHA_PLUS );
	}

/*
	Sint32 yy1 = 60*WINDOW_H/100;
	Sint32 yy2 = 80*WINDOW_H/100;

	Uint32 sColor[4] = 
	{
		ARGB( 0xff, 14,  18, 21 ),
		ARGB( 0xff, 45,  77, 135 ),
		ARGB( 0xff, 129, 158,193 ),
		ARGB( 0xff, 255, 255,233 ),
	};


//	if( !m_bBgColorChange )
	{
		viiDraw::Grad(0 ,0   ,WINDOW_W , yy1	  , PRIO_BG0+0  ,ATR_DFLT,sColor[0],sColor[0],sColor[1],sColor[1] );
		viiDraw::Grad(0 ,yy1 ,WINDOW_W , yy2	  , PRIO_BG0+0  ,ATR_DFLT,sColor[1],sColor[1],sColor[2],sColor[2] );
		viiDraw::Grad(0 ,yy2 ,WINDOW_W , WINDOW_H , PRIO_BG0+0  ,ATR_DFLT,sColor[2],sColor[2],sColor[3],sColor[3] );
	}
*/

	//-------------------------------------
	//��
	//-------------------------------------
/*

	Sint32 sx,sy;

	sx = 0-(m_sRiverScroll/100)*100 + ( ( GetTargetPlayer()->x/100)/512)*51200 - 51200;
	sy = 600;

	for(Sint32 ii=0; ii<5; ii++)
	{
		viiDraw::Sprite( &SprStageH02River[ 0 ] , sx+(0    )+ii*51200 , sy*100,PRIO_BG1+1 , ATR_DFLT , 0xF0FFFFFF );
		viiDraw::Sprite( &SprStageH02River[ 1 ] , sx+(25600)+ii*51200 , sy*100,PRIO_BG1+1 , ATR_DFLT , 0xF0FFFFFF );
	}
*/

}


