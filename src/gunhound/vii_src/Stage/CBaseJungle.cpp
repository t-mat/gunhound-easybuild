//----------------------------------------------------------------------------
//
//�n�E���h�X�e�[�W �P�i�W�����O���j
//
//----------------------------------------------------------------------------

#include <gunvalken.h>
#include "CStageBase.h"
//---------------------------------------------------------------------------
#include "../effect/EffectBase.h"
#include "../enemy/enemy.h"

#define STAGE_PATH "HoundData\\stageH01\\"

enum
{
	enAreaSeq01,
	enAreaSeq02,
	enAreaSeq03,
	enAreaSeq04,
	enAreaSeq05,
	enAreaSeqMax,
};

enum
{
	enGroundScrY = 695,
};

void CBaseJungle::Init()
{
	//---------------------------------------------------------------------------
	//������
	//---------------------------------------------------------------------------
	SetFadeIn();

	Sint32 col1 =  0xF0408080;
	Sint32 col2 =  0xF0808040;
	pGame->pBg->SetSkyColor(50,col1,col2);

	viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );

	//---------------------------------------------------------------------------
	//�t�@�C��������
	//---------------------------------------------------------------------------
//	LoadTexture( enTexPageSoldier , "HoundData\\enemychara\\common\\ene_soldier.bmp",0xff00ff00);
//	UploadTexture();

	//---------------------------------------------------------------------------
	//�G���A������
	//---------------------------------------------------------------------------

	switch(GetArea( )){
	case enAreaSeq01:
		LoadConfig( "HoundData\\ScenJungle@01.txt" );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
//		viiMus::ReadBGM( 0,"HoundData\\bgm\\A\\A3_Bgm1.ogg");
		viiMus::PlayBGM(enSoundBgm1,enMusicBgmStageA);
//		LoadConfig( "HoundData\\ScenRMJ@01.txt" );
//		LoadConfig( "HoundData\\ScenMBJ@01.txt" );
//		LoadConfig( "HoundData\\ScenMRJ@01.txt" );
//		LoadConfig( "HoundData\\ScenRBJ@01.txt" );
		break;

	case enAreaSeq02:
		break;

	case enAreaSeq03:
		break;

	case enAreaSeq04:
		break;

	case enAreaSeq05:
		break;
	}

	InitEnemies();

}


void CBaseJungle::End()
{
	//---------------------------------------------------------------------------
	//�I������
	//---------------------------------------------------------------------------


}

void CBaseJungle::InitEnemies(Sint32 sMin)
{
	//---------------------------------------------------------------------------
	//�G�̏�����
	//---------------------------------------------------------------------------

	SetEnemy(sMin);

}


void CBaseJungle::Action()
{
	//---------------------------------------------------------------------------
	//���t���[���̃A�N�V����
	//---------------------------------------------------------------------------

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
	}

//	viiDbg::printf(100,0,"ZoneTime= %d",m_sZoneTimer);
//	viiDbg::printf(100,16,"Area    = %d",GetArea( ) );

}


void CBaseJungle::Area01()
{
	//---------------------------------------------------------------------------
	//�G���A�P�@�`�C��܂�
	//---------------------------------------------------------------------------

	m_sScrollMax = 9600*100;

//	ChangeArea( enAreaSeq02 );

	BgCommon();
	viiSub::LimitMoveVH();

}



void CBaseJungle::Area02()
{
	//-----------------------------------------------------
	//�G���A�Q:�e�B�[�K�[��
	//-----------------------------------------------------


	BgCommon();
	viiSub::LimitMoveVH();

}


void CBaseJungle::Area03()
{
	//-----------------------------------------------------
	//�G���A�R�F�ړI�n����
	//-----------------------------------------------------

	BgCommon();
	viiSub::LimitMoveVH();

}


void CBaseJungle::Area04()
{
	//-----------------------------------------------------
	//�G���A�S�F�L���[�^���N
	//-----------------------------------------------------
 	m_sScrollMax = 7600*100;

	BgCommon();
	viiSub::LimitMoveVH();

}


void CBaseJungle::Area05()
{
	//-----------------------------------------------------
	//�G���A�T�F�{�X��
	//-----------------------------------------------------

	if( IsZoneSeq( 0 ) )//&& !pGame->pSystem->is_msg_now() )
	{
		//�N���A
		SetZoneSeq( 10 );
		SetFadeOut(2);
	}
	else if( IsZoneSeq( 10 ) )
	{
		//�t�F�[�h�I���҂�
		if( IsFadeEnd() )
		{
			pGame->stage_clear(enClearSeqStageClear);
			ChangeArea( GAME_NONE );	//�X�e�[�W�I��
		}
	}

}


void CBaseJungle::AreaCommon()
{
	//-----------------------------------------------------
	//�G���A�F����
	//-----------------------------------------------------

}


//---------------------------------------------------------------------------
//�u�l�o����G�̐���
//---------------------------------------------------------------------------


void CBaseJungle::SetEnemy(Sint32 sMin)
{
	for(Sint32 y=0; y<pGame->pBg->getHeight()/8; y++)
	{
		for(Sint32 x=sMin/8; x<pGame->pBg->getWidth()/8; x++)
		{
			Sint32 sIndex = pGame->pBg->get_vmpdat(x,y);

			sIndex = EnemyIndex[sIndex];
			switch(sIndex){
//			case enCEneH0106Yagura:
//				new CEneH0106Yagura(x*800,y*800);
//				break;

			case enCEneH0008GunTank:
				break;

			case enCEneH0101HeavyTank:		//�����d���
				break;

			case enCEneH0102NightSpider:	//BOSS
				break;

			case enCEneH0301Gunpelly:
				new CEneH0301Gunpelly(x*800,y*800,gxTrue);
				break;

			case 1009:	//����
				{
					CEne0000Dummy *p;
					p = new CEne0000Dummy( 9 , x*800 , y*800 );
					p->m_Pos.z = PRIO_BG3-1;
				}
				break;
			default:
				MakeHoundsEnemy(x*800,y*800,sIndex);
				break;
			}

		}
	}

}


void CBaseJungle::BgCommon(Sint32 x,Sint32 y)
{
	//----------------------------------------------------
	//�w�i����
	//----------------------------------------------------
	BgScrollAdjust();

	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );

	StLayerScroll *pScr;

	for(Sint32 ii=enBgTypeBack; ii<=enBgTypeFrnt; ii++)
	{
		Sint32 ax,ay;
		pScr = &LayerScroll[ii];

		ax = pScr->ox+(sx*pScr->x);
		ay = pScr->oy+(sy*pScr->y);

		viiSub::SetBgLayer( ii , ax/100 , ay/100 );

		if( ii == enBgTypeBack )
		{
			m_BgLayerPos.x = (sx-(sx*pScr->x)/100)*100;///100;
			m_BgLayerPos.y = (sy-(sy*pScr->y)/100)*100;///100;
		}

	}

	//------------------------------------
	//�͂̏���
	//------------------------------------

	m_sRiverScroll += 200;
	m_sRiverScroll =  m_sRiverScroll%51200;

}

