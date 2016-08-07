#define DASH_CMD_TIME (8)

class CEasyLeynos;
class CEffect;
class CGameManager;
class CFadeMnager;

extern gxBool leynos_edit;
extern Sint32  turn_kansei;
extern Sint32  debug_mode_vii;
extern Sint32  gtimer_ex;
extern Sint32  GameSeq;

extern CEasyLeynos  *Eleynos;
extern CGameManager *pGame;


void set_globals_init();

//-----------------------------------------
//ステージ連絡用グローバル変数
//-----------------------------------------
#define STAGE_GOLBALS_NUM (32)
extern Sint32 gStageValue[STAGE_GOLBALS_NUM];
