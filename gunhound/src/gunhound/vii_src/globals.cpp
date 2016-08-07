//---------------------------------------------------------------
//グローバル変数
//---------------------------------------------------------------
#include <gunvalken.h>

gxBool leynos_edit = gxFalse;
Sint32  turn_kansei;
Sint32  debug_mode_vii=1;
Sint32  gtimer_ex = 0;
Sint32  GameSeq  = GSEQ_INIT;
Sint32 gStageValue[STAGE_GOLBALS_NUM];

CEasyLeynos  *Eleynos = NULL;
CGameManager *pGame   = NULL;

void set_globals_init()
{
	//--------------------------------
	//グローバルの初期化
	//--------------------------------
	Sint32 i;

	leynos_edit = gxFalse;
	turn_kansei = 0;
	debug_mode_vii=1;
	gtimer_ex = 0;
//	GameSeq  = GSEQ_INIT;

	for(i=0;i<STAGE_GOLBALS_NUM;i++)
	{
		gStageValue[i] = 0;
	}

//	if(pActManager==NULL){
//		pActManager = new CActManager();
//	}

}

