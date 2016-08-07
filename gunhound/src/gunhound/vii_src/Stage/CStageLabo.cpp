//----------------------------------------------------------------------------
//
//�@�G�N�X�g���X�e�[�W �P�i�A�C�A���M�A��j
//
//----------------------------------------------------------------------------

#include <gunvalken.h>
#include "CStageBase.h"
//---------------------------------------------------------------------------
#include "../effect/EffectBase.h"
#include "../enemy/enemy.h"

enum
{
	enAreaSeq01,
	enAreaSeq02,
	enAreaSeq03,
	enAreaSeq04,
	enAreaSeq05,
	enAreaSeqMax,
};

enum {
	enRollerDashHeight = 850*100,
};

void CStageLabo::Init()
{
	//---------------------------------------------------------------------------
	//������
	//---------------------------------------------------------------------------
	SetFadeIn();

	Sint32 col1 =  0xF0408080;
	Sint32 col2 =  0xF0808040;
	pGame->pBg->SetSkyColor(50,col1,col2);

	//---------------------------------------------------------------------------
	//�t�@�C��������
	//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------
	//�G���A������
	//---------------------------------------------------------------------------

	switch(GetArea( )){
	case enAreaSeq01:
		LoadConfig( "HoundData\\StageLabo.txt" );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		CCockpit::GetInstance()->LoadMessage("HoundData\\font\\ScenJungle\\DatMsg.txt");	//��
		break;

	case enAreaSeq02:
		pGame->SetMarineLine(900*100);
		LoadConfig( "HoundData\\StageLabo2.txt" );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		CCockpit::GetInstance()->LoadMessage("HoundData\\font\\ScenRiver\\DatMsg.txt");	//��
		break;

	case enAreaSeq03:
		LoadConfig( "HoundData\\StageLabo3.txt" );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		CCockpit::GetInstance()->LoadMessage("HoundData\\font\\ScenMount\\DatMsg.txt");	//��
		break;

	case enAreaSeq04:
		LoadConfig( "HoundData\\StageLabo4.txt" );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		CCockpit::GetInstance()->LoadMessage("HoundData\\font\\ScenKichi\\DatMsg.txt");	//��
		break;

	case enAreaSeq05:
		LoadConfig( "HoundData\\StageLabo5.txt" );
		viiSub::SetScroll_l( GetTargetPlayer()->x , GetTargetPlayer()->y );
		CCockpit::GetInstance()->LoadMessage("HoundData\\font\\ScenExtra\\DatMsg.txt");	//��
		break;
	}

	InitEnemies();

}


void CStageLabo::End()
{
	//---------------------------------------------------------------------------
	//�I������
	//---------------------------------------------------------------------------


}

void CStageLabo::InitEnemies()
{
	//---------------------------------------------------------------------------
	//�G�̏�����
	//---------------------------------------------------------------------------

	SetEnemy();

}

void CStageLabo::Action()
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


void CStageLabo::Area01()
{
	//---------------------------------------------------------------------------
	//�G���A�P
	//---------------------------------------------------------------------------
	static ghPos s_Pos;
	m_sScrollMax = 9600*100;
	static CEneCmnSpark* m_pCEneCmnSpark=NULL;
	static CEneH0503HoundFly *pHoundFly = NULL;
	static CEneH0007YusouHeli *m_pYusouHeli = NULL;

	static Sint32 sTimer = 0;
	sTimer ++;

	s_Pos.x = pGame->GetHound()->m_Pos.x;
	s_Pos.y = pGame->GetHound()->m_Pos.y;

	if(Joy[0].trg&BTN_C)
	{
		CCockpit::GetInstance()->SetDanmakuIndex( 0 , gxFalse );
	}
	if( m_pYusouHeli )
	{
		if(Joy[0].trg&BTN_C)
		{
			m_pYusouHeli->GetSoldierLeader()->SetTargetPosition( m_pYusouHeli->m_Pos.x-16*100 );
			m_pYusouHeli->GetSoldierLeader()->CommandMember( enMoveTypeRideOn );
		}
	}

	if( Joy[0].trg&BTN_Z )
	{
		Sint32 msgTbl[]={
			1050,//�p�[�v�����[�_�[���i�ߕ��ցB
			1100,//�����烉�C�U�A�ڕW�n�_�ɓ��B�I
			1200,//�܂������g����������Ԃ��A
			1300,//�d���V���b�^�[���~�낹�A
			1350,//������A���w���B�����n�_�ɓG�e�A
			1400,//�p�[�v���Q���A���[�_�[��
			1600,//�Δ��p�̓���u�ǂ��B

			1700,//�b�S�O������Z�b�g�A
			1800,//�d���V���b�^�[�̏������m�F�B

			1850,//�˓����邼�A
			1900,//�O���t�H���̖C����

			2000,//������p�[�v���Q�A�i�[�ɂ�
			3000,//�I�y���[�V�����E���|�[�g�B
			9900,9910,9920,
		};
	}

	BgCommon();
	viiSub::LimitMoveVH();

}



void CStageLabo::Area02()
{
	//-----------------------------------------------------
	//�G���A�Q
	//-----------------------------------------------------
	static Sint32 s_Col = 0;

	double n = viiMath::Sqrt(300);
	pGame->GetHound()->SetWaterProof();
	
	if( Joy[0].trg&BTN_Z )
	{
		Sint32 msgTbl[]={
			2000,//�G�P�[�[���I�I
			2100,//�������A�󒆂���̔����͂Ȃ�Ƃ�
			2200,//�������I�I
			2300,//���C�U�I�������ɂP�@������
			2500,//�o�c�I������B�����͂�������
			2600,//�A���D�A���S���ɒE�o�I
			3000,//�I�y���[�V�������|�[�g�B
			9900,9910,9920,
		};
		static Sint32 msg_id=0;
		CCockpit::GetInstance()->SetMessage( msgTbl[msg_id] );
		msg_id ++;

		Sint32 max = ARRAY_LENGTH(msgTbl);
		msg_id = (max+msg_id)%max;
	}

	BgCommon();
	viiSub::LimitMoveVH();

}


void CStageLabo::Area03()
{
	//-----------------------------------------------------
	//�G���A�R
	//-----------------------------------------------------

	if( Joy[0].trg&BTN_Z )
	{
		Sint32 msgTbl[]={
			3000,//�S�@�U��I
			3100,//�~���ǁA�܂��Ȃ��������܂��B
			3200,//�S���ޔ�����[�[��
			3300,//�Z���̔����I
			3400,//�����I�I���̓o�c��A��߂��܂�
			3500,//�j���[�g�����|���}�[�̌@���ɐ����B
			3600,//�������̖h�q�{�݂�

			3700,//�҂������I
			3800,//������i�ߎ��B�p�[�v���c�[

			3900,//�G�A�q�󕔑����m�F�I�I
			4000,//�u�~���w���A�G���W�����ɔ�e�I�v
			4100,//�u�_�����A�~���w�����������I�I�v
			4200,//�S���~�������A�A���w��
			4300,//������I���̂�낤�A��������
			4400,//�I�y���[�V�������|�[�g�I
			9900,9910,9920,

		};
		static Sint32 msg_id=0;
		CCockpit::GetInstance()->SetMessage( msgTbl[msg_id] );
		msg_id ++;

		Sint32 max = ARRAY_LENGTH(msgTbl);
		msg_id = (max+msg_id)%max;
	}

	BgCommon();
	viiSub::LimitMoveVH();

}


void CStageLabo::Area04()
{
	//-----------------------------------------------------
	//�G���A�S
	//-----------------------------------------------------
 	m_sScrollMax = 7600*100;

	static CEneH0409SlideCannon *pCEneH0409SlideCannon = NULL;

	if( Joy[0].trg&BTN_Z )
	{
		Sint32 msgTbl[]={
			4000,//�܂��Ȃ����G���A�ɓ��B���܂��B
			4100,//�����{�ݓ����̃_�N�g�ɐi�������I
			4200,//�_�N�g�𔲂����I
			4300,//�j���[�g�����|���}�[�����{�݂�
			4400,//�������傤�A�r�Ɉꔭ�H������܂���
			4500,//�܂��Ȃ��E�o�|�C���g�֓��B
			4600,//���C���Z���F���m�F
			4700,//�Z���F��j�󂵂��A�E�o�����J�����I
			4800,//�I�y���[�V�������|�[�g
			9900,9910,9920,
		};
		static Sint32 msg_id=0;
		CCockpit::GetInstance()->SetMessage( msgTbl[msg_id] );
		msg_id ++;

		Sint32 max = ARRAY_LENGTH(msgTbl);
		msg_id = (max+msg_id)%max;
	}

	BgCommon();
	viiSub::LimitMoveVH();

}


void CStageLabo::Area05()
{
	//-----------------------------------------------------
	//�G���A�T�F�{�X��
	//-----------------------------------------------------

	if( Joy[0].trg&BTN_Z )
	{
		Sint32 msgTbl[]={
/*
			9000,//���ĂĂĂ�
*/
			5000,//����퓬�͂��G���A�c�ɐi�s�I
			5100,//�G���W���u���b�N�̔j����m�F�I
			5200,//��C�̔M���������g�咆�B
			5300,//��C��j�󂵂��A
			5400,//�S�@�ɒʒB�I�j���[�g�����|���}�[
			5500,//�G�l���M�[���ՊE�_��˔j���܂��I
			9900,9910,9920,
		};
		static Sint32 msg_id=0;
		CCockpit::GetInstance()->SetMessage( msgTbl[msg_id] );
		msg_id ++;

		Sint32 max = ARRAY_LENGTH(msgTbl);
		msg_id = (max+msg_id)%max;
	}

	BgCommon();
	viiSub::LimitMoveVH();
}


void CStageLabo::AreaCommon()
{
	//-----------------------------------------------------
	//�G���A�F����
	//-----------------------------------------------------

	m_sLoop += 8;
	if( m_sLoop >= 480)
	{
		m_sLoop -= 480;
	}
}


//---------------------------------------------------------------------------
//�u�l�o����G�̐���
//---------------------------------------------------------------------------


void CStageLabo::SetEnemy()
{
	return;
}


void CStageLabo::BgCommon(Sint32 x,Sint32 y)
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
		pScr = &LayerScroll[ii];
		viiSub::SetBgLayer( ii , pScr->ox+(sx*pScr->x)/100 , pScr->oy+(sy*pScr->y)/100 );
	}

}

