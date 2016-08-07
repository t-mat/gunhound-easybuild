//---------------------------------------
//ステージ個別の処理
//---------------------------------------
#include <gunvalken.h>
#include "stage/CStageBase.h"

enum {
	STAGE_ACTSEQ_INIT,
	STAGE_ACTSEQ_MAIN,
};

CStage::CStage(Sint32 no)
{
	Sint32 i;

	//常にバーシスは正常なテクスチャに戻しておく
//	LoadTexture(TPG_COMMON,"viidata\\gra\\eagis01.bmp",0xff00ff00);

	stg_no = no;
	timer_ = 0;

	for(i=0;i<64;i++)
	{
		flag[i] = gxFalse;
		val[i] = 0;
		msgflg[i] = 0;
	}

	for(i=0;i<STAGE_GOLBALS_NUM;i++) {
		gStageValue[i] = 0;
	}

	ActSeq = STAGE_ACTSEQ_INIT;

	scx = scy = 0;
	area = g_StGameInfo.ContinueArea;//g_StGameInfo.PlayArea;
//	g_StGameInfo.ContinueArea = g_StGameInfo.PlayArea;
	m_pStageBase = NULL;

}

CStage::~CStage()
{
	if(m_pStageBase)
	{
		m_pStageBase->End();
		delete m_pStageBase;
	}
}

void CStage::action()
{
	//-----------------------------------
	//ステージメインループ
	//-----------------------------------
	switch(ActSeq){
	case STAGE_ACTSEQ_INIT:
		initseq();
		UploadTexture();

	case STAGE_ACTSEQ_MAIN:
		mainseq();
		break;
	}

//	//エリア更新
//	g_StGameInfo.PlayArea = area;
	g_StGameInfo.PlayArea = m_pStageBase->GetArea();//area;

}

void CStage::initseq()
{
	//-----------------------------------
	//初期化シーケンス
	//-----------------------------------
	m_pStageBase = InitStage( stg_no );
	m_pStageBase->ChangeArea( g_StGameInfo.PlayArea );
	m_pStageBase->Init();
	ActSeq = STAGE_ACTSEQ_MAIN;
	return;

/*
	switch(stg_no){
	case 0:
//		area_init_stage00();
		break;
	case 1:
	case 2:
		m_pStageBase = InitStage( stg_no );
		m_pStageBase->Init();
		break;
	case 3:
		area_init_stage03();
		break;
	case 4:
		area_init_stage04();
		break;
	case 5:
		area_init_stage05();
		break;
	case 6:
		area_init_stage06();
		break;
	case 7:
		area_init_stage07();
		break;
	case 99:
		stage99();
		break;
	}
*/

	ActSeq = STAGE_ACTSEQ_MAIN;
}


void CStage::mainseq()
{
	//-----------------------------------
	//メインシーケンス
	//-----------------------------------
	Sint32 old_area = area;

	m_pStageBase->Action();

#if 0
	switch(stg_no){
	case 0:
//		m_pStageBase->Action();
//		stage0();
//		stage_debug();
		break;
	case 1:
		m_pStageBase->Action();
//		stage1();
		break;
	case 2:
		stage2();
		break;
	case 3:
		stage3();
		break;
	case 4:
		stage4();
		break;
	case 5:
		stage5();
		break;
	case 6:
		stage6();
		break;
	case 7:
		stage7();
		break;
	case 99:
		stage99();
		break;
	}
	
#endif

	UploadTexture();

	timer_ ++;

	if(timer_ >=0x70000000){
		//絶対にタイマーをゼロにしない
		timer_ = 1;
	}

//いらない	m_pStageBase->SetTimer(timer_);

}


void CStage::draw()
{
	m_pStageBase->Draw();
}

void CStage::stage_debug()
{

}

