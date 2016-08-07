//------------------------------------------------------
//
// �X�e�[�W�̊��N���X
//
//------------------------------------------------------

#include "../GunHound/Cmenu.h"
#include "../effect/EffectBase.h"

enum {
	MaxEnemyNum = 256,
	enAddTimeMAX = 32,
};

class CStageBase
{
public:

	CStageBase();
	~CStageBase();
	ghPos m_Pos;

	Sint32 m_sZoneTimer;
	Sint32 m_sClearTimer;
	Sint32 m_sBGM;

	virtual void Init();
	virtual void Action();
	virtual void End();
	virtual void Draw()
	{

	}

	virtual void BgCommon(Sint32 x=0,Sint32 y=0);

	//-----------------------------------------------------
	//�Q�[���X�^�[�g��ʒB
	//-----------------------------------------------------
	void SetGamePlay(gxBool bFlag)
	{
		m_bGamePlay = bFlag;
	}
	//-----------------------------------------------------
	//�G���A�̕ύX���s���i�]�[���V�[�P���X�����Z�b�g�����j
	//-----------------------------------------------------
	void ChangeArea(Sint32 area);

	//-----------------------------------------------------
	//���݂̃G���A�𓾂�
	//-----------------------------------------------------
	Sint32  GetArea( );

	//-----------------------------------------------------
	//���݂̃G���A�^�]�[���V�[�P���X��ݒ�
	//-----------------------------------------------------
	void SetZoneSeq( Sint32 n);

	//-----------------------------------------------------
	//���݂̃G���A�^�]�[���V�[�P���X�𓾂�
	//-----------------------------------------------------
	Sint32 GetZoneSeq( );

	//-----------------------------------------------------
	//����]�[���V�[�P���X�ɓ����Ă��邩�H
	//-----------------------------------------------------
	gxBool IsZoneSeq( Sint32 n);

	//-----------------------------------------------------
	//�G���A�^�]�[���^�C�}�[�����Z�b�g
	//-----------------------------------------------------
	void ResetZoneTimer( );

	//-----------------------------------------------------
	//�t�F�[�h�I���������H
	//-----------------------------------------------------
	gxBool IsFadeEnd();

	//-----------------------------------------------------
	//�t�F�[�h�C��
	//-----------------------------------------------------
	void SetFadeIn(Sint32 n=8,Sint32 col=0xff010101);

	//-----------------------------------------------------
	//�t�F�[�h�A�E�g
	//-----------------------------------------------------
	void SetFadeOut(Sint32 n=8,Sint32 col=0xFF010101 );

	//-----------------------------------------------------
	//���ʂ̊�{�X�N���[������
	//-----------------------------------------------------
	void BgScrollAdjust();

	//-----------------------------------------------------
	//�܂��p�̃t�B���^�[
	//-----------------------------------------------------
	void WeatherFilterCloudy();

	//-----------------------------------------------------
	//�G�f�B�b�g�p
	//-----------------------------------------------------

	//�G�l�~�[�R���t�B�O�t�@�C���̓ǂݍ���
	void   LoadLeynosEnemyConfigFile(char *pFileName );
	gxBool LoadConfig(char *pFileName);

	//�G�l�~�[�̐���
	void MakeLeynosEnemy(Sint32 x1,Sint32 y1,Sint32 no);
	void MakeHoundsEnemy(Sint32 x1,Sint32 y1,Sint32 no);

	//�G64�̂̃C���f�b�N�X
	Sint32 EnemyIndex[MaxEnemyNum];

	//BGM�t�@�C����
	char BgmFileName[255];

	//BG���C���[�P�`�R
	char LayerFileName01[255];
	char LayerFileName02[255];
	char LayerFileName03[255];

	//BG���C���[�X�N���[�����x
	typedef struct StLayerScroll
	{
		Sint32 x,y;
		Sint32 ox,oy;
	} StLayerScroll;

	StLayerScroll LayerScroll[enBgTypeMax];

	//BG�X�N���[���l���E�ݒ�
	Sint32 m_sScrollMax;
	Sint32 m_sScrollMin;
	Sint32 m_sScrollYMin;
	Sint32 m_sScrollYMax;

	//VMP�t�@�C����
	char VmpFileName[255];

	//���X�^�[�g���̏����ʒu
	Sint32 m_sResetPositionX;
	Sint32 m_sResetPositionY;

	gxBool m_bFirst;

	//-------------------------------------
	//�X�e�[�W���ʂ̏���
	//-------------------------------------
	void StageCommon();

	//-------------------------------------
	//����G���A�ɐi�s�������H
	//-------------------------------------
	gxBool IsAreaArrive(Sint32 x1, Sint32 y1, Sint32 x2, Sint32 y2);

	//-------------------------------------
	//DRQ�t�@�C�����̐ݒ�t�@�C�����w�肵�Ď��s
	//-------------------------------------
	gxBool SetDRQStage( char* pFileName );

	//-------------------------------------
	//���W�Ŏw�肵����`���̃}�b�v�`�b�v��j������
	//-------------------------------------
	void ClearMapTip( Sint32 sLayer , Sint32 x1 , Sint32 y1 , Sint32 x2 , Sint32 y2 );

	//-------------------------------------
	//���W�Ŏw�肵����`����VMP�`�b�v��j������
	//-------------------------------------
	void ClearVmpTip( Sint32 x1 , Sint32 y1 , Sint32 x2 , Sint32 y2 );

	//-------------------------------------
	//�f�o�b�O�p�}�b�v�r���[���[�h
	//-------------------------------------
	void AreaTest();
	void AddMissionSec(Sint32 sZone , Sint32 sSec );

private:

	Sint32 m_sTimer;
	Sint32 m_sAreaSeq;
	Sint32 m_sZoneSeq;

	//�G�f�B�b�g�p
	gxBool m_bStageGoodClear;
	gxBool m_bStageBadClear;
	gxBool m_bStageClear;
	gxBool m_bGamePlay;

	void SetConfigData(Sint32 sCmd , char **pArg);
	void   DRQConfig(Sint32 sCmd , char **pArg);

	//-------------------------------------
	//�o�b�h�N���A�����̂��i���g�p�j
	//-------------------------------------
	gxBool IsStageBadClear()
	{
		return m_bStageBadClear;
	}

	gxBool IsStageGoodClear()
	{
		return m_bStageGoodClear;
	}

	void SetStageGoodClear()
	{
		m_bStageGoodClear = gxTrue;
	}

	void SetStageBadClear()
	{
		m_bStageBadClear = gxTrue;
	}

	gxBool IsStageClear()
	{
		if( m_bStageGoodClear ) return gxTrue;
		if( m_bStageBadClear ) return gxTrue;
		return gxFalse;
	}

	gxBool m_bTimeAdd[enAddTimeMAX];
};


#include "../enemy/Enemy.h"

#include "CBaseJungle.h"
#include "CBaseKichi.h"
#include "CBaseMountain.h"
#include "CBaseRiver.h"
#include "CBaseExtra.h"
#include "CStageLabo.h"

#include "CStageStart.h"
#include "CStageTutorial.h"

CStageBase* InitStage(Sint32 num);
