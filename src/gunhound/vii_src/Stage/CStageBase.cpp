//------------------------------------------------------
//
// �X�e�[�W�̊��N���X
//
//------------------------------------------------------
#include <gunvalken.h>
#include <gxLib/Util/csv/CCsv.h>
#include <gxLib/util/gxImage/CGXImage.h>
#include "CStageBase.h"
#include "CScriptEngine.h"

char *CommandList[enCommandMax]={
/*00*/	"�y�Ȃ��z",
/*01*/	"�y���@�̈ʒu�z",
/*02*/	"�y�a�f�l�ǂݍ��݁z",
/*03*/	"�y�w�i�ǂݍ��݁z",
/*04*/	"�y�e�N�X�`���ǂݍ��݁z",
/*05*/	"�y�u�l�o�ǂݍ��݁z",
/*06*/	"�y�G�ݒ�z",
/*07*/	"�y�X�N���[���ݒ�z",
/*08*/	"�y�e�N�X�`���X�V�z",
/*09*/	"�y���O�o�́z",
/*10*/ "�ydbg_EneSpr�ݒ�z",	//eneIndex,8,page,u,v,w,h,cx,cy,	//sprIndex��0�`7�܂�
/*11*/ "�ydbg_Ene�A�^������z",	//eneIndex,x1,y1,x2,y2,damage
/*12*/ "�ydbg_Ene�N���C����z",	//eneIndex,x1,y1,x2,y2,damage
/*13*/ "�ydbg_Ene�v�l�ݒ�z",	//eneIndex,spd,atk				//�ړ����x(0�`200)�ƍU�����(0:�U�����Ȃ�)��ݒ�
/*14*/ "�ydbg_Ene�o���z",		
/*15*/ "�y�w�i�I�t�Z�b�g�ݒ�z",
/*16*/ "�y�t�H���g�ǂݍ��݁z",
///*16*/ "",

/*108*/	"�y���b�Z�[�W�ݒ�z",
	"Command_None",
	"cmd_set",
	"msg_end",
	"cmd",
	"[LoadTexture]",
	"[Update]",
	NULL,
};


CStageBase::CStageBase()
{
#ifdef _VII_DEBUG_
	pGame->pBg->DebugTipDraw();
#endif

	m_sScrollYMin = 0;
	m_sScrollYMax = 9600*100;

	m_bFirst  = gxTrue;	//�����Init()����gxFlase�ƂȂ�
	m_bGamePlay = gxFalse;

	m_sAreaSeq  = 0;
	m_sZoneSeq  = 0;
//����Ȃ�	m_sMsgCnt   = 0;
	m_sClearTimer = 0;
	m_sBGM = 0;
	m_sZoneTimer = 0;
	m_sTimer  = 0;

//����Ȃ�	m_pTextName = NULL;

//����Ȃ�	for(Sint32 ii=0;ii<MaxMessageNum;ii++)
//����Ȃ�	{
//����Ȃ�		m_sMsgMem[ii] = -1;
//����Ȃ�	}


	m_Pos.x = 0;
	m_Pos.y = 0;

	m_bStageGoodClear = gxFalse;
	m_bStageBadClear  = gxFalse;
	m_bStageClear     = gxFalse;

	BgmFileName[0] = 0x00;
	VmpFileName[0] = 0x00;
	LayerFileName01[0] = 0x00;
	LayerFileName02[0] = 0x00;
	LayerFileName03[0] = 0x00;
	m_sResetPositionX = 0;
	m_sResetPositionY = 0;
	LayerScroll[enBgTypeBack].x = 100;
	LayerScroll[enBgTypeRear].x = 100;
	LayerScroll[enBgTypeFrnt].x = 100;

	LayerScroll[enBgTypeBack].y = 100;
	LayerScroll[enBgTypeRear].y = 100;
	LayerScroll[enBgTypeFrnt].y = 100;

	LayerScroll[enBgTypeBack].ox = 0;
	LayerScroll[enBgTypeRear].ox = 0;
	LayerScroll[enBgTypeFrnt].ox = 0;

	LayerScroll[enBgTypeBack].oy = 0;
	LayerScroll[enBgTypeRear].oy = 0;
	LayerScroll[enBgTypeFrnt].oy = 0;

	m_sScrollMax = 9660*100;
	m_sScrollMin = 0;

	for(Sint32 jj=0; jj<MaxEnemyNum; jj++)
	{
		EnemyIndex[ jj ] = jj;
	}

	//�b��I�Ƀ^�C�����t���ɂ���
	CCockpit::GetInstance()->SetMissionSec( 69*99 );

	for(Sint32 ii=0; ii<enAddTimeMAX; ii++)
	{
		m_bTimeAdd[ii] = gxFalse;
	}
}

CStageBase::~CStageBase()
{

}


void CStageBase::AddMissionSec(Sint32 sZone , Sint32 sSec )
{
	//�~�b�V�����^�C�����X�V����

//	if( !m_bTimeAdd[sZone] )
//	{
		m_bTimeAdd[sZone] = gxTrue;
//		CCockpit::GetInstance()->AddMissionSec( sSec );
//	}

}


void CStageBase::Init()
{
	//virtual
}

void CStageBase::Action()
{
	//virtual
}


void CStageBase::End()
{
	//virtual
}


void CStageBase::ClearMapTip( Sint32 sLayer , Sint32 x1 , Sint32 y1 , Sint32 x2 , Sint32 y2 )
{
	//-------------------------------------
	//�}�b�v�`�b�v���폜����
	//-------------------------------------

	for(Sint32 y=y1; y<y2; y+=MAP_BLOCK_SIZE)
	{
		for(Sint32 x=x1; x<x2; x+=MAP_BLOCK_SIZE)
		{
//			pGame->pBg->del_vmptip((sx+x)/8,(sy+y)/8);
			pGame->pBg->del_maptip( sLayer, x/MAP_BLOCK_SIZE, y/MAP_BLOCK_SIZE );
		}
	}

}

void CStageBase::ClearVmpTip( Sint32 x1 , Sint32 y1 , Sint32 x2 , Sint32 y2 )
{
	//-------------------------------------
	//�}�b�v�`�b�v���폜����
	//-------------------------------------

	for(Sint32 y=y1; y<y2; y+=8)
	{
		for(Sint32 x=x1; x<x2; x+=8)
		{
			pGame->pBg->del_vmptip( x/8 , y/8 );
		}
	}

}

//����Ȃ�void CStageBase::SetTimer(Sint32 time)
//����Ȃ�{
//����Ȃ�	m_sTimer = time;
//����Ȃ�}


Sint32 CStageBase::GetArea( )
{
	return m_sAreaSeq;
}

void CStageBase::ChangeArea(Sint32 area)
{
	//---------------------------
	//�G���A�ύX
	//---------------------------

	m_sAreaSeq = area;
	m_sZoneSeq   = 0;
	m_sZoneTimer = 0;
}

gxBool CStageBase::IsZoneSeq( Sint32 n)
{
	//---------------------------
	//�]�[���V�[�P���X�𓾂�
	//---------------------------

	return ( m_sZoneSeq == n);
}

Sint32 CStageBase::GetZoneSeq( )
{
	//---------------------------
	//�]�[���V�[�P���X�𓾂�
	//---------------------------

	return m_sZoneSeq;
}

void CStageBase::SetZoneSeq( Sint32 n)
{
	//---------------------------
	//�]�[���V�[�P���X���Z�b�g
	//---------------------------

	m_sZoneSeq = n;

}

void CStageBase::ResetZoneTimer( )
{
	//---------------------------
	//�]�[���^�C�}�[�����Z�b�g
	//---------------------------

	m_sZoneTimer = 0;

}

gxBool CStageBase::IsFadeEnd()
{
	//---------------------------
	//�t�F�[�h�I���܂������H
	//---------------------------

	return  !CFadeManager::GetInstance()->is_fade();

}

void CStageBase::SetFadeIn(Sint32 n,Sint32 col)
{
	//---------------------------
	//�t�F�[�h�C��
	//---------------------------

	CFadeManager::GetInstance()->set_fadein(8,col);
}

void CStageBase::SetFadeOut(Sint32 n,Sint32 col)
{
	//---------------------------
	//�t�F�[�h�A�E�g
	//---------------------------

	CFadeManager::GetInstance()->set_fadeout( STAGE_CLEAR_FADE_TIME ,col );
}


void CStageBase::BgScrollAdjust()
{
	//---------------------------
	//BG�̊�{����
	//---------------------------

	Sint32 sx,sy;
	viiSub::GetIdealScroll_l(sx,sy);

	Sint32 sYmax = pGame->pBg->getHeight()+512;

	if(sx < m_sScrollMin) sx = m_sScrollMin;
	if(sy < m_sScrollYMin) sy = m_sScrollYMin;
	if(sx > m_sScrollMax) sx = m_sScrollMax;
	if(sy > sYmax*100)
	{
		sy = sYmax*100;
	}

	if(sy > m_sScrollYMax )
	{
		sy = m_sScrollYMax;
	}

	viiSub::AdjScroll_l( sx,sy );
	viiSub::GetScroll_s( sx,sy );

	viiSub::SetBgDisp ( enBgTypeStar , gxFalse );
	viiSub::SetBgLayer( enBgTypeStar , sx*LayerScroll[enBgTypeBack].x/10,sy*LayerScroll[enBgTypeBack].y/100 );

/*
	Sint32 sx,sy;

	viiSub::GetIdealScroll_l(sx,sy);

	if(sx <= x1) sx = x1;
	if(sy <= y1) sy = y1;
	if(sx >= x2) sx = x2;
	if(sy >= y2) sy = y2;

	viiSub::AdjScroll_l( sx,sy );
*/

}

void CStageBase::WeatherFilterCloudy()
{
	//----------------------------------------------
	//�V��Č��p�̃t�B���^�[
	//----------------------------------------------

	viiDraw::Box(0,0,WINDOW_W,WINDOW_H,PRIO_BG1+1,gxTrue,0x800000f0,ATR_ALPHA_MINUS);

}

gxBool CStageBase::IsAreaArrive(Sint32 x1, Sint32 y1, Sint32 w, Sint32 h)
{
	//--------------------------------------------
	//����G���A�ɓ����Ă��邩�H
	//--------------------------------------------
	gxBool bFlag = gxFalse;

	if( GetTargetPlayer() == NULL )
	{
		return gxFalse;
	}

	Sint32 x = GetTargetPlayer()->x/100;
	Sint32 y = GetTargetPlayer()->y/100;
	Sint32 x2,y2;

	x2 = x1+w;
	y2 = y1+h;

	if( x>x1 && x<x2 )
	{
		if( y>y1 && y<y2 )
		{
			bFlag = gxTrue;
		}
	} 

#ifdef _VII_DEBUG_
	if( bFlag )
	{
		viiDraw::BoxScrl_s( x1 , y1 , x2 , y2 , PRIO_DEBUG , gxTrue , 0x8000FF00 , ATR_ALPHA_PLUS );
	}
	else
	{
		viiDraw::BoxScrl_s( x1 , y1 , x2 , y2 , PRIO_DEBUG , gxTrue , 0x80FFFF00 , ATR_ALPHA_PLUS );
	}
#endif

	return bFlag;
}

void CStageBase::BgCommon(Sint32 x,Sint32 y)
{
	
	//virtual
}


void CStageBase::AreaTest()
{
	//-----------------------------------------------------
	//�G���AB�F�e�X�g
	//-----------------------------------------------------

	static gxBool s_bFlag;

	pGame->GetHound()->m_Pos.x = 0;
	pGame->GetHound()->m_Pos.y = 0;

	if( IsZoneSeq(0) )
	{
		SetFadeIn();
		s_bFlag = gxFalse;
		m_Pos.x = pGame->GetHound()->m_Pos.x;
		m_Pos.y = pGame->GetHound()->m_Pos.y;
		pGame->GetHound()->SetMode( CGunHound::enHoundModeNone );
		SetZoneSeq(10);
	}
	else if( IsZoneSeq(10) )
	{
		Sint32 sSpd = 300;
		if( Joy[0].psh&BTN_A) sSpd = 1000;
		if( Joy[0].trg&BTN_B)
		{
			s_bFlag = !s_bFlag;
			pGame->pBg->DebugTipDraw( s_bFlag );
		}
		if( Joy[0].psh&JOY_U) m_Pos.y -= sSpd;
		if( Joy[0].psh&JOY_D) m_Pos.y += sSpd;
		if( Joy[0].psh&JOY_L) m_Pos.x -= sSpd;
		if( Joy[0].psh&JOY_R) m_Pos.x += sSpd;


		m_sScrollMax  = m_Pos.x;
		m_sScrollYMax = m_Pos.y;

		m_sScrollMin  = m_sScrollMax;
		m_sScrollYMin = m_sScrollYMax;
	}

	BgCommon();
	viiSub::LimitMoveVH();
}


void CStageBase::StageCommon()
{
	//------------------------------------------------
	//�X�e�[�W���ʏ���
	//------------------------------------------------

	//�������u
	DummyEnemyWaitCheck( m_sTimer );

	if( m_bGamePlay )
	{
		if( pGame )
		{
			//�̗͂�΂��̌x��
			if( pGame->GetHound()->GetHitPointStatus() == CGunHound::enHitPointStausAlert )
			{
				if( !CCockpit::GetInstance()->IsMessageNow() )
				{
					CCockpit::GetInstance()->SetMessage( 9900 );
				}
			}
		}

		//���Ԃ�΂��̌x��
//		if( CCockpit::GetInstance()->GetMissionSec() < 60 )
//		{
//			if( !CCockpit::GetInstance()->IsMessageNow() )
//			{
//				CCockpit::GetInstance()->SetMessage( 9910 );
//			}
//		}

		//���Ԑ؂�̌x��
//		if( CCockpit::GetInstance()->IsTimeOver() )
//		{
//			if( !CCockpit::GetInstance()->IsMessageNow() )
//			{
//				CCockpit::GetInstance()->SetMessage( 9920 );
//			}
//		}
	}

	m_sZoneTimer ++;

#ifdef _VII_DEBUG_
	DEBUG_ATARIMAP = 1;
	DEBUG_ATARIOBJ = 1;

	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );
//����Ȃ�	if( m_pTextName )
//����Ȃ�	{
//����Ȃ�		viiDbg::printf(312,WINDOW_H,"%s",m_pTextName);
//����Ȃ�	}
	viiDbg::printf(200,WINDOW_H+16*0,"Area (%d-%d)",m_sAreaSeq, m_sZoneSeq );
	viiDbg::printf(200,WINDOW_H+16*1,"ZTime (%d)"  ,m_sZoneTimer);

	viiDbg::printf(312,WINDOW_H+16*1,"HOUND (%d,%d)",GetTargetPlayer()->x/100 ,GetTargetPlayer()->y/100);
	viiDbg::printf(312,WINDOW_H+16*2,"SCROLL(%d,%d)",sx,sy);
#endif


}

void CStageBase::MakeHoundsEnemy(Sint32 x,Sint32 y,Sint32 sIndex)
{
	//----------------------------------------------------------
	//�G����
	//----------------------------------------------------------
	Sint32 no = sIndex;
	Sint32 enemyID = 0;

	enemyID = sIndex;//EnemyIndex[no];

	if( enemyID >= 1000 && enemyID <=1063 )
	{
		//�_�~�[�L�����ݒ�
		new CEne0000Dummy( enemyID-1000 , x , y );
		return;
	}

	viiSub::MakeEnemy( enemyID , x , y );

}

CStageBase* InitStage(Sint32 num)
{
	CStageBase* p=NULL;
	CCockpit::GetInstance()->AutoStartUp();

	switch(num) {
	case 99:
		p = new CStageLabo();
		break;

	case enScenarioJungle:
		p = new CStageStart();
//����Ȃ�		p->SetStageIndex( enScenarioJungle , NULL );
		break;

	case enScenarioRiver:
		p = new CBaseRiver();
//����Ȃ�		p->SetStageIndex(enScenarioRiver , NULL );
		break;

	case enScenarioMountain:
		p = new CBaseMountain();
//����Ȃ�		p->SetStageIndex(enScenarioMountain, NULL );
		break;

	case enScenarioBase:
		p = new CBaseKichi();
//����Ȃ�		p->SetStageIndex(enScenarioBase, NULL );
		break;

	case enScenarioIronGear:
		p = new CBaseExtra();
//����Ȃ�		p->SetStageIndex(enScenarioExtra, NULL );
		break;

	case enScenarioTutorial:
		p = new CStageTutorial();
//����Ȃ�		p->SetStageIndex(enScenarioTutorial, NULL );
		break;

	case enScenarioExtra:
	case enScenarioExtra2:
	case enScenarioExtra3:
	default:
		break;
	}

	return p;

}


void CStageBase::SetConfigData(Sint32 sCmd , char **pArg)
{
	//-------------------------------------
	//�R�}���h�����s����
	//-------------------------------------

	switch(sCmd){
	case 1:	//"�y���@�̈ʒu�z"
		SetTargetPlayerPosX( atoi(pArg[0])*100 );
		SetTargetPlayerPosY( atoi(pArg[1])*100 );
		break;

	case 2:	//"�y�a�f�l�ǂݍ��݁z"
		//viiMus::ReadBGM( atoi(pArg[0]) , pArg[1] );
		break;

	case 3:	//"�y�w�i�ǂݍ��݁z"
		pGame->pBg->LoadMapFile( atoi(pArg[0]) , pArg[1] );
		LayerScroll[ atoi(pArg[0]) ].x = atoi(pArg[2]);
		LayerScroll[ atoi(pArg[0]) ].y = atoi(pArg[3]);
		break;

	case 4:	//"�y�e�N�X�`���ǂݍ��݁z",
		LoadTexture( atoi(pArg[0])*16+atoi(pArg[1]) , pArg[2] , 0xff00ff00);
		break;

	case 5:	//"�y�u�l�o�ǂݍ��݁z",
		pGame->pBg->LoadVmpFile( pArg[0] );
		break;

	case 6:	//"�y�G�ݒ�z",
		EnemyIndex[ atoi(pArg[0]) ] = atoi(pArg[1]);
		break;

	case 7://"�y�X�N���[���ݒ�z",
		m_sScrollMin = atoi(pArg[0])*100;
		m_sScrollMax = atoi(pArg[1])*100;
		break;

	case 8:	//�y�e�N�X�`���X�V�z
		UploadTexture();
		break;

	case 9://�y���O�o�́z
		viiDbg::log( "�X�N���v�g�F%s",pArg[0] );
		break;

	case 10: // "�ydbg_EneSpr�ݒ�z",	//eneIndex,page,u,v,w,h,cx,cy,
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].page =  atoi(pArg[1])*16+atoi(pArg[2]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].u    =  atoi(pArg[3]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].v    =  atoi(pArg[4]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].w    =  atoi(pArg[5]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].h    =  atoi(pArg[6]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].cx   =  atoi(pArg[7]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].cy   =  atoi(pArg[8]);
		break;
	case 11: // "�ydbg_Ene�A�^������z",	//eneIndex,x1,y1,x2,y2,damage
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].m_bHit = atoi(pArg[5])? gxTrue : gxFalse;
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_u =  atoi(pArg[1]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_v  =  atoi(pArg[2]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_w  =  atoi(pArg[3]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_h  =  atoi(pArg[4]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_dmg=  atoi(pArg[5]);
		break;
	case 12: // "�ydbg_Ene�N���C����z",	//eneIndex,x1,y1,x2,y2,damage
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].m_bKurai = atoi(pArg[5])? gxTrue : gxFalse;
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_u =  atoi(pArg[1]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_v  =  atoi(pArg[2]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_w  =  atoi(pArg[3]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_h  =  atoi(pArg[4]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_dmg=  atoi(pArg[5]);
		break;

	case 13: // "�ydbg_Ene�v�l�ݒ�z",	//eneIndex,spd,atk				//�ړ����x(0�`200)�ƍU�����(0:�U�����Ȃ�)��ݒ�
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sSpeed      = atoi(pArg[1]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sWeaponType = atoi(pArg[2]);
		break;

	case 14:
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sWaitTime = atoi( pArg[1] );
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sEneIndex = EnemyIndex[ atoi( pArg[2] ) ];
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].bExist = gxTrue;
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sX  = atoi( pArg[3] )*100;
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sY  = atoi( pArg[4] )*100;
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sTx = atoi( pArg[5] )*100;
		break;

	case 15:	//"�y�w�i�I�t�Z�b�g�z"
		LayerScroll[ atoi(pArg[0]) ].ox = atoi(pArg[1]);
		LayerScroll[ atoi(pArg[0]) ].oy = atoi(pArg[2]);
		break;

	case 16:	//"�y�t�H���g�ǂݍ��݁z"
		CCockpit::GetInstance()->LoadMessage( pArg[0] );
		break;
	default:
		break;
	}
}


gxBool CStageBase::LoadConfig(char* pFileName )
{
	//-------------------------------------
	//�X�N���v�g�f�[�^����ǂݍ���
	//-------------------------------------
	DummyEnemyWaitInit();

	CScriptEngine *pScript = new CScriptEngine(CommandList);

	if( !pScript->Load(pFileName) ) return gxFalse;

	for(Sint32 ii=0; ii<pScript->GetLineMax(); ii++)
	{
		pScript->Read( ii );
		SetConfigData( pScript->GetCommandIndex( ) , pScript->GetArguments() );
	}

	delete pScript;
	return gxTrue;
}


gxBool CStageBase::SetDRQStage( char* pFileName )
{
	//-------------------------------------
	//�X�N���v�g�f�[�^����ǂݍ���
	//-------------------------------------
	DummyEnemyWaitInit();

	CScriptEngine *pScript = new CScriptEngine(CommandList);

	Uint32 sSize = 0;
	Uint8 *pFile = CGXImage::GetInstance()->GetFile( pFileName ,&sSize );

	if( !pScript->LoadMemory( pFile , sSize) ) return gxFalse;

	for(Sint32 ii=0; ii<pScript->GetLineMax(); ii++)
	{
		pScript->Read( ii );
		DRQConfig( pScript->GetCommandIndex( ) , pScript->GetArguments() );
	}

	delete pScript;

	pGame->GameStart(gxTrue);

	return gxTrue;
}


void CStageBase::DRQConfig(Sint32 sCmd , char **pArg)
{
	//-------------------------------------
	//�R�}���h�����s����
	//-------------------------------------

	switch(sCmd){
	case 1:	//"�y���@�̈ʒu�z"
		SetTargetPlayerPosX( atoi(pArg[0])*100 );
		SetTargetPlayerPosY( atoi(pArg[1])*100 );
		break;

	case 2:	//"�y�a�f�l�ǂݍ��݁z"
		{
			Uint32 sSize;
			Uint8 *p;
			p = CGXImage::GetInstance()->GetFile( pArg[1] , &sSize );
			viiMus::ReadBGM( enSoundStartPage+atoi( pArg[0]) , p ,sSize );
			//CGXImage::GetInstance()->DeleteFile( pArg[1] );
		}
		break;

	case 3:	//"�y�w�i�ǂݍ��݁z"
		pGame->pBg->LoadMapData( atoi(pArg[0]) , pArg[1] );
		LayerScroll[ atoi(pArg[0]) ].x = atoi(pArg[2]);
		LayerScroll[ atoi(pArg[0]) ].y = atoi(pArg[3]);
		break;

	case 4:	//"�y�e�N�X�`���ǂݍ��݁z",
//		viiSub::LoadTexture( atoi(pArg[0])*16+atoi(pArg[1]) , CCockpit::GetInstance()->Loader() , pArg[2] , 0xff00ff00);
		CGXImage::GetInstance()->SetTexture( atoi(pArg[0])*16+atoi(pArg[1]),pArg[2] );
		break;

	case 5:	//"�y�u�l�o�ǂݍ��݁z",
		{
			Uint32 uSize;
			Uint8 *p = CGXImage::GetInstance()->GetFile( pArg[0] ,&uSize );
			pGame->pBg->LoadVmpData( p );
		}
		break;

	case 6:	//"�y�G�ݒ�z",
		EnemyIndex[ atoi(pArg[0]) ] = atoi(pArg[1]);
		break;

	case 7://"�y�X�N���[���ݒ�z",
		m_sScrollMin = atoi(pArg[0])*100;
		m_sScrollMax = atoi(pArg[1])*100;
		break;

	case 8:	//�y�e�N�X�`���X�V�z
		UploadTexture();
		break;

	case 9://�y���O�o�́z
		viiDbg::log( "�X�N���v�g�F%s",pArg[0] );
		break;

	case 10: // "�ydbg_EneSpr�ݒ�z",	//eneIndex,page,u,v,w,h,cx,cy,
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].page =  atoi(pArg[1])*16+atoi(pArg[2]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].u    =  atoi(pArg[3]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].v    =  atoi(pArg[4]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].w    =  atoi(pArg[5]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].h    =  atoi(pArg[6]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].cx   =  atoi(pArg[7]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].stSprdat[0].cy   =  atoi(pArg[8]);
		break;
	case 11: // "�ydbg_Ene�A�^������z",	//eneIndex,x1,y1,x2,y2,damage
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].m_bHit = atoi(pArg[5])? gxTrue : gxFalse;
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_u =  atoi(pArg[1]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_v  =  atoi(pArg[2]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_w  =  atoi(pArg[3]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_h  =  atoi(pArg[4]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sAtari_dmg=  atoi(pArg[5]);
		break;
	case 12: // "�ydbg_Ene�N���C����z",	//eneIndex,x1,y1,x2,y2,damage
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].m_bKurai = atoi(pArg[5])? gxTrue : gxFalse;
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_u =  atoi(pArg[1]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_v  =  atoi(pArg[2]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_w  =  atoi(pArg[3]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_h  =  atoi(pArg[4]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sKurai_dmg=  atoi(pArg[5]);
		break;

	case 13: // "�ydbg_Ene�v�l�ݒ�z",	//eneIndex,spd,atk				//�ړ����x(0�`200)�ƍU�����(0:�U�����Ȃ�)��ݒ�
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sSpeed      = atoi(pArg[1]);
		g_stDummyEnemyConfig[ atoi(pArg[0]) ].sWeaponType = atoi(pArg[2]);
		break;

	case 14:
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sWaitTime = atoi( pArg[1] );
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sEneIndex = EnemyIndex[ atoi( pArg[2] ) ];
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].bExist = gxTrue;
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sX  = atoi( pArg[3] )*100;
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sY  = atoi( pArg[4] )*100;
		g_StDummyEnemyWaitList[ atoi(pArg[0]) ].sTx = atoi( pArg[5] )*100;
		break;

	case 15:	//"�y�w�i�I�t�Z�b�g�z"
		LayerScroll[ atoi(pArg[0]) ].ox = atoi(pArg[1]);
		LayerScroll[ atoi(pArg[0]) ].oy = atoi(pArg[2]);
		break;

	case 16:	//"�y�t�H���g�ǂݍ��݁z"
		{
			CCockpit::GetInstance()->LoadMessageFromMemory( pArg[0] );
		}
		break;
	default:
		break;
	}
}

