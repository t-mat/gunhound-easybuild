#ifndef _GUNVALKEN_H_
#define _GUNVALKEN_H_
#include <gunhound/gga2.h>
#include <gunhound/gga2x.h>
#include <gxLib/Util/actManager/CActManager.h>
#include "gunhound/vii_src/vii_def.h"
#include "gunhound/vii_src/GunHound.h"
#include "gunhound/vii_src/sprdat.h"
#include "gunhound/vii_src/globals.h"
#include "gunhound/vii_src/CFadeManager.h"

#define DIR_LEFT (-1)
#define DIR_RIGHT (1)
#define SHOOT_SCROLL_SPEED (300)	//�V���[�e�B���O�X�e�[�W�X�N���[�����x
#define STAGE_CLEAR_FADE_TIME (3)	//�X�e�[�W�N���A���̋��ʃt�F�[�h�A�E�g�^�C��

#define HIGH_SCORE_MAX (999999990)
#define MISSION_ALLOVER_BONUS (10000000)
#define ENDING_TRUEVERSION_BONUS (1000000)


/*
TEXTURE�y�[�W�̐؂�ւ��ɂ���
1024x1024 = 4M (256x256��16������)
�Œ�ł�VRAM��32M�͂���Ƃ����8�y�[�W�̃e�N�X�`����
���邱�ƂɂȂ�B
���ō�256x256��96���i�[�\

������S�y�[�W�ڂł�������؂蕪�����

16MB�܂�(4�y�[�W�ڂ܂�)��256x256�T�C�Y�̂��̂��i�[
256x256��16x4��48���i�[�\

�ׂ��������Ă݂�
�y�[�W00 �풓 256x256 16��
�y�[�W01 �X�e�[�W�ŗL 256x256 16��
�y�[�W02 �X�e�[�W�w�i 256x256 16��
�y�[�W03 �X�e�[�W�w�i 256x256 16��

16MB�ȍ~(4�y�[�W�ڈȍ~)��512x512�T�C�Y�̂��̂��i�[
512x512��16���i�[�\

�y�[�W00 �풓 256x256 16��
�y�[�W01 �X�e�[�W�ŗL 256x256 16��
�y�[�W02 �X�e�[�W�w�i 256x256 16��
�y�[�W03 �X�e�[�W�w�i 256x256 16��

//--------------------------------
//�풓
//--------------------------------
�y�[�W00 �풓 256x256 16���@���@�A�o�[�V�X�A�V�X�e�������A�l�Ȃǁi�G�����邩���j
�y�[�W01 �풓 512x512  4���@�^�C�g���A�G�t�F�N�g�A���b�Z�[�W�t�H���g�Ȃ�

//--------------------------------
//�Q�[����
//--------------------------------
�y�[�W02 �X�e�[�W�ŗL 256x256 16���@�G�A�G�e�Ȃ�
�y�[�W03 �X�e�[�W�ŗL 256x256 16���@��p�G�t�F�N�g�Ȃ�

�y�[�W04 �X�e�[�W�ŗL 512x512 4���@�h�����J�[�A�V���g���Ȃ�
�y�[�W05 �X�e�[�W�w�i 256x256 16���@�X�e�[�W�w�i�P
�y�[�W06 �X�e�[�W�w�i 256x256 16���@�X�e�[�W�w�i�Q
�y�[�W07 �X�e�[�W�w�i 256x256 16���@�X�e�[�W�w�i�R

//---------------------------------
//�u���[�t�B���O��
//--------------------------------
�y�[�W02 �X�e�[�W�ŗL 256x256 16���@�G�A�G�e�Ȃ�
�y�[�W03 �X�e�[�W�ŗL 256x256 16���@��p�G�t�F�N�g�Ȃ�

�y�[�W04 �u���[�t�B���O 512x512 4��
�y�[�W05 �u���[�t�B���O 512x512 4��
�y�[�W06 �u���[�t�B���O 512x512 4��
�y�[�W07 �u���[�t�B���O 512x512 4��

*/


enum {
	//�A�T���g�̃^�C�v
	ASTYPE_NONE = -1,
	ASTYPE_GUNVALKEN_JQ = 0,
	ASTYPE_GUNVALKEN_OTHER = 1,
	ASTYPE_SHUMITS= 10,
};



enum {
	ATK_TYPE_NONE,
	ATK_TYPE_CANNON=1,
};
enum {
	//�v���C�I���e�B�ݒ�
	PRIO_BG0 = 50,	//��
	PRIO_BG1 = 70,		//��Ȃ�
	PRIO_BG1_5 = 90,		//��Ȃ�
	PRIO_BG2   = 110,		//�ǌ�
	PRIO_ENEMY =130 ,
	PRIO_ENE_B =140,
	PRIO_VERSIS_B =145,
	PRIO_PLAYER =150,
	PRIO_ENE_F =160,
	PRIO_VERSIS_F =170,
	PRIO_BG3 =180,		//�ǑO
	PRIO_EFFECT =190,
	PRIO_ATACK = 192,
	PRIO_ENE_BULLET =195,
	PRIO_GAMETOP=199,
	PRIO_BLACKTONE=200,
	PRIO_SYSTEM,
	PRIO_SYSTEMWINDOW,
	PRIO_SYSTEMTEXT,
	PRIO_FADE,
	PRIO_FADEOVER,
	PRIO_DEBUG_TIP,
	//PRIO_MAX -10 //�_�b�V���{�[�h
	//PRIO_MAX -5  //HD-MASK
	PRIO_DEBUG = (PRIO_MAX-2),
};


enum EnControlMode {
	//GV���샂�[�h
	enControlMode_NONE,
	enControlMode_NORMAL,
	enControlMode_SPACE,
	enControlMode_SHOOT,
	enControlMode_DASH,
	enControlMode_LANDING,	//�o�[�V�X����̔���
	enControlMode_DECENT,
	enControlMode_KICKSTART,
	enControlMode_CPU,		//�b�o�t���W�b�N�ő��삳���

	//�n�E���h�p
	enControlMode_SuperSpace,
	enControlMode_SuperRiver,
};

enum {
	GSEQ_INIT,
	GSEQ_DEBUGMENU,
	GSEQ_OPENING,
	GSEQ_TITLE,
	GSEQ_DEMOINIT,
	GSEQ_DEMO,
	GSEQ_OPTION,
	GSEQ_DEBUG,
	GSEQ_BRIEFING,

	GSEQ_OPDEMO_SEQ1,
	GSEQ_OPDEMO_SEQ2,
	GSEQ_OPDEMO_SEQ3,

	GSEQ_GAMEINIT,
	GSEQ_GAMEMAIN,
	GSEQ_STAGECLEAR,
	GSEQ_GAMEEND,
	GSEQ_CONTINUE,
	GSEQ_OVER,
	GSEQ_ENDING,

	GSEQ_GUNHOUND,

};


typedef struct OBJ_POS_T{
	Sint32 x,y,z;
	Sint32 ax1,ay1,ax2,ay2;	//���S����̂����蔻��
	Sint32 kx1,ky1,kx2,ky2;	//���S����̂��炢����

	Sint32 mx,my;
	Sint32 now_x,now_y;
	Sint32 old_x,old_y;
	Sint16 rot,sx,sy;
}OBJ_POS_T;

#include "gunhound/vii_src/CHitManager.h"
#include "gunhound/vii_src/CStage.h"
#include "gunhound/vii_src/Cbackground.h"
#include "gunhound/vii_src/CObj.h"
#include "gunhound/vii_src/CEasyLeynos.h"
#include "gunhound/vii_src/CEffect.h"
#include "gunhound/vii_src/Vii_Sub.h"
#include "gunhound/vii_src/enemy/enemy.h"

class CGunHound;


enum {
 enScopeTypeNormal,
 enScopeTypeKoukaku,
 enScopeTypeSnipe,
};

class CGameManager {
public:

	CGameManager();
	~CGameManager();
	void init();
	void stage_clear(Sint32 sNextSeq=1);

	Sint32 GetNextSeq();

	void player_out();

	void action();

	CBackGround *pBg;
	CStage *pStage;

	CEffManager *pEffManager;
	CHitManager* pHitManager;

	gxBool QuakeFlag;
	gxBool bAutoDetectUnderLine;
	Sint32 m_sSuiheisen;

	//�����I�ɕt������ړ������l�i��C���˓����̖�䰂Ȃǂł����j
	Sint32 power_x;
	Sint32 power_y;

	Sint32 m_sQuakeTime;

	void SetMarineLine(Sint32 y)
	{
		m_sSuiheisen = y;
	}

	Sint32 GetMarineLine()
	{
		return m_sSuiheisen;
	}
	CGunHound* GetHound()
	{
		return m_pPlayerHound;
	}

	void SetDetectUnderLine(gxBool bFlag)
	{
		bAutoDetectUnderLine = bFlag;
	}
	gxBool GetDetectUnderLine()
	{
		return bAutoDetectUnderLine;
	}
	Sint32 GetGameTimer()
	{
		return m_sWorldTimer;
	}

	Sint32 Quake(){

#ifdef GUNHOUND
		if(m_sQuakeTime>0)
		{
			return (viiMath::Cos( (m_sWorldTimer%36)*60 )*m_sQuakeTime)/400;
		}
		QuakeFlag = gxFalse;
		return 0;
#endif
		if(QuakeFlag)	return -2+(viiSub::GameCounter()%4);

		return 0;
	}
	void GameStart(gxBool bFlag )
	{
		m_bGameStart = bFlag;
	}
	gxBool IsGameStart( )
	{
		return m_bGameStart;
	}

	void QuakeOn(Sint32 sTime=60 ){
		QuakeFlag = gxTrue;
		m_sQuakeTime = sTime;
	}
	void QuakeOff(){
		m_sQuakeTime = 0;
		QuakeFlag = gxFalse;
	}
	//-------------------------
	Sint32 get_scroll_power_x(){
		return power_x;
	}
	Sint32 get_scroll_power_y(){
		return power_y;
	}
	Sint32 is_gravit(){
		return gravitation;
	}
	void gravitation_off(){
		gravitation = gxFalse;
	}
	void gravitation_on(){
		gravitation = gxTrue;
	}
	gxBool get_playerout(){
		return outflag;
	}

	void set_the_world(){
		stop_all_ = gxTrue;
	}
	void cancel_the_world(){
		stop_all_ = gxFalse;
	}
	gxBool Is_the_world(){
		return stop_all_;
	}

	void SetRaderScope(Sint32 sType)
	{
		m_sScopeType = sType;
	}

	Sint32 GetRaderScope()
	{
		return m_sScopeType;
	}

	gxBool IsStageClear()
	{
		return m_bMissionClear;
	}

	gxBool IsGameOver()
	{
		return m_bMissionFailed;
	}

private:

	Sint32 m_sScopeType;
	Sint32 m_sNextSeq;
	Sint32 m_sWorldTimer;

	gxBool stop_all_ ;
	gxBool m_bMissionClear;
	gxBool m_bMissionFailed;
	gxBool outflag;
	gxBool gravitation;
	gxBool m_bGameStart;

	CGunHound *m_pPlayerHound;

	union free {
		char   b[32];
		short  w[16];
		Sint32 l[8];
	}free;


};

typedef struct StGameInfo
{
	enum {
		enGameModeNormal = 0,
		enGameModeMission,
		enGameModeDebug,
	};

	Sint32 PlayStage;
	Sint32 PlayArea;
	Sint32 ClearStage[32];
	Sint32 Gold;
	Sint32 m_sGameMode;
	Sint32 ContinueArea;

	Uint32 m_uAllScore;
	Uint32 m_uStageScore;
	Uint32 m_uHighScore;
	Sint32 m_sContinueCnt;

	gxBool m_bDebugRoot;
	gxBool m_bSound;
	gxBool m_bMusic;
	gxBool m_bMessage;
	gxBool m_bReplay;
	gxBool m_bTrueEnd;

	StGameInfo()
	{
		Reset();
	}

	void Reset()
	{
		for(Sint32 ii=0;ii<32;ii++)
		{
			ClearStage[ii] = 0;
		}

		m_uAllScore = 0;
		m_uStageScore = 0;
		m_uHighScore = 0;

		PlayStage  = 0;
		PlayArea   = 0;
		Gold  = 20000;

		ContinueArea = 0;
		m_bSound = gxTrue;
		m_bMusic = gxTrue;
		m_bMessage = gxFalse;
		m_bReplay  = gxFalse;
		m_bTrueEnd = gxTrue;
		m_sContinueCnt = 0;
	}

	void AddScore(Uint32 score)
	{
		m_uAllScore += score;
		m_uStageScore += score;
		if( m_uAllScore >= m_uHighScore )
		{
			m_uHighScore = m_uAllScore;
		}
	}

	void SetStageClear(Sint32 n)
	{
		ClearStage[n] = 1;
	}

	gxBool IsStageClear(Sint32 n)
	{
		return (ClearStage[n])? gxTrue : gxFalse;
	}

} StGameInfo;

extern CGameManager *pGame;

extern StGameInfo g_StGameInfo;

extern Sint32 _opt_[10];


#define MAX_PAD_MEMORY_FRM (60*60*10)
#define DEBUG_ATARIMAP ghLib::Value(0)
#define DEBUG_ATARIOBJ ghLib::Value(1)

#ifdef GX_DEBUG
	//----------------------------------
	//�f�o�b�O�R���p�C����
	//----------------------------------

	#define _VII_DEBUG_
	#define NO_SOUND
	#define _READ_DRQ_

#else
	//----------------------------------
	//�����[�X�R���p�C����
	//----------------------------------
	#define _VII_RELEASE_
	#define _READ_DRQ_
#endif


//#define BTN_NEXT (BTN_A|BTN_B|BTN_X|BTN_Y)
//#define BTN_CANCEL (BTN_A|BTN_X)

//���j���[����X�ASPACE�AENTER������{�^���Ƃ��܂��i�L�����Z����Z�j
#define BTN_MENU_DECISION (BTN_B|BTN_SELECT|BTN_START)

enum {
	enMenuSeqNone = 0,
	enMenuSeqNext =  1,
	enMenuSeqBack = -1,
};

extern gxSprite AsSpr[];
#endif
