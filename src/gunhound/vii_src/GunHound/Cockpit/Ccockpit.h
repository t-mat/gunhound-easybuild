//---------------------------------------------------
//�R�N�s�b�g���
//---------------------------------------------------
#include "CRader.h"
#include "CRaderDamage.h"

class CStageClear;
class CMsgWindow;
class CBellowsSprite;
class CCockpit//	 : public CAction
{
	enum {
		enDanmakuMax = 32,
	};

	typedef struct StDanmaku
	{
		StDanmaku()
		{
			m_sIndex = -1;
			m_bEnable = gxFalse;
		}
		Sint32 m_sIndex;
		gxBool m_bEnable;
		gxBool m_bDummy;
	} StDanmaku;

public:

	CCockpit();
	~CCockpit();

	void Action()
	{
		SeqMain();
	}

	void SeqMain();
	void SeqCrash();

	void PanelSwitching();

	void Draw();

	static CCockpit* GetInstance();

	//-----------------------------
	//�R�N�s�b�g��ʂ���蒼���܂�
	//-----------------------------
	void ReNew();
	void Destroy();

	Sint32 GetScoreBairitsu();
	void   SetScoreBairitsu( Sint32 n );
	gxBool IsDanmakuEnable( Sint32 n );
	void   SetDanmakuIndex( Sint32 n , gxBool bFlag );

	void SetBurniaEnergy(Sint32 per)
	{
		m_sBurniaEnergy = per;
	}

	void SetSubWeaponLamp(gxBool bFlag)
	{
		m_bSubWeapon = bFlag;
	}

	CRaderDamage* GetRader()
	{
		//-----------------------------
		//�_���[�W���[�_�[�ɃA�N�Z�X���܂�
		//-----------------------------
		return m_pRaderDamage;
	}

	void SetMissionSec(Sint32 sSec)
	{
		m_sMissionTime = sSec*60;
		m_sMissionSecFlah = 120;
	}

	void AddMissionSec(Sint32 sSec)
	{
		m_sMissionTime += sSec*60;
		m_sMissionSecFlah = 120;
	}


	Sint32 GetMissionSec()
	{
		return m_sMissionTime/60;
	}

	gxBool IsTimeOver()
	{
		return (m_sMissionTime==0)? gxTrue : gxFalse;
	}

	void SetDead()
	{
		m_bPlayerOut = gxTrue;
	}

	void SetHitPoint(Sint32 sHp)
	{
		m_sHitPoint = sHp;
	}

	void SetShortMessage(char* pMessage);

	void SetScore(Sint32 score)
	{
		m_sScore = score;
		m_sScoreWait = 100;
	}

	void SetWarningSound(Sint32 sFrm )
	{
		m_sWarningFrame = sFrm;
	}

	//����A�C�R���̐ݒ�
	void SetWeaponIcon( Sint32 sIndex , Sint32 sIcon , Sint32 sType , Sint32 sBulletRemain);

	//------------------------------------
	//���G���A�ݒ�
	//------------------------------------
	void SetOperationRange(Sint32 x1,Sint32 x2)
	{
		if(x2 < x1)
		{
			Sint32 tmp = x1;
			x1 = x2;
			x2 = tmp;
		}

		m_sOperationrangeMin = x1/100;
		m_sOperationrangeMax = x2/100;

		m_pRaderControl->SetOperationRange(m_sOperationrangeMin,m_sOperationrangeMax);
	}

	gxBool IsOperationArea(Sint32 x)
	{
		m_sPlayerPosX = x;

		if( (x/100 > m_sOperationrangeMin) && (x/100 < m_sOperationrangeMax) ) return gxTrue;

		return gxFalse;
	}

	Sint32 GetOperationArea()
	{
		Sint32 x = m_sPlayerPosX;

		if( x/100 < m_sOperationrangeMin) return -1;
		if( x/100 > m_sOperationrangeMax) return 1;

		return 0;
	}

	//------------------------------------
	//�X�e�[�W�N���A�֘A
	//------------------------------------
	gxBool IsStageClear();
	void   SetStageClear();
	gxBool IsClearOver();
	void DestroyStageClear();

	//------------------------------------
	//���b�Z�[�W�֘A
	//------------------------------------
	void LoadMessage( char *pFileName );
	void LoadMessageFromMemory( char *pFileName );

	gxBool SetMessage(Sint32 sIndex,gxBool bForce = gxFalse);
	gxBool IsMessageNow();

	void SetMessageEnable(gxBool bFlag)
	{
		m_bMessageEnable = bFlag;
	}

	Sint32 GetMessageIndex(Sint32 n);

	void KanjiPrintf(Sint32 x, Sint32 y,char *pMsg,...);
	void KanjiColorPrintf(Sint32 x, Sint32 y,Sint32 sCol , char *pMsg,...);
	void KanjiPrintfZ(Sint32 x, Sint32 y, Sint32 sPrio, Sint32 sCol , char *pMsg,...);

	//-----------------------------
	//�A���[����i���������p�j
	//-----------------------------
	void SetArrow(Sint32 sRot)
	{
		if( !m_bArrow )
		{
			m_sArrowWait = 100;
			m_bArrow = gxTrue;
		}
		m_sArrowRot = sRot;
	}


	//�~�b�V�����^�C����i�߂�
	void AddMissionTime();

	void AutoStartUp();
	void Reset();

	void SetNowLoading()
	{
		m_sNowLoadingWait = 30;
	}

	gxBool IsGameOver()
	{
		return m_bGameOver;
	}

	void SetSplash(gxBool bFlag)
	{

		m_bSplash = bFlag;

	}

	void SetBlackMask(gxBool bFlag)
	{
		m_bBlackMask = bFlag;
	}

	void SetHidden()
	{
		m_sHeight = 100;
	}

	void ResetMessageWindow();

	void SetHitStop(Sint32 sStopCnt = 4 )
	{
		m_sStopCnt = sStopCnt;
	}

	gxBool IsHitStop()
	{
		return (m_sStopCnt > 0 )? gxTrue : gxFalse;
	}

	void ResetRaderMarker()
	{
		if( m_pRaderControl )
		{
			m_pRaderControl->ResetMarkerCnt();
		}
	}

	void SetSystemSound(gxBool bFlag)
	{
		m_bSystemSound = bFlag;
	}
private:

	enum {
		enMessageMemMax = 64,
	};

	enum {
		enSwitchDummy,		//�_�~�[
		enSwitchRader,		//���C�h���[�_�[(ON/OFF)
		enSwitchPosition,	//�_���[�W���ʕ\��
		enSwitchBullet,		//�c�e���iON/OFF)
		enSwitchScore,		//�X�R�A(ON/OFF)
		enSwitchTime,		//�c�莞�ԕ\��
		enSwitchMessage,	//�V���[�g���b�Z�[�W(ON/OFF)

		enSwitchRed,		//�ԊO��(ON/OFF)
		enSwitchDamage,		//�c��HP(ON/OFF)
		enSwitchLog,		//���b�Z�[�W���O(ON/OFF)
		enSwitchArmLock,	//�A�[�����b�N�iON/OFF�jL1�Œ�/�{�^���������ςȂ��Œ�
		enSwitchSave,		//�V�X�e���Z�[�u
		enSwitchLoad,		//�V�X�e�����[�h
		enSwitchMax,
		enSwitchTotal,		//�_�~�[
	};

	void DrawArrow( Sint32 bx , Sint32 by );
	void DrawScore( Sint32 bx , Sint32 by );
	void DrawShortMessage( Sint32 bx , Sint32 by );
	void DrawRelord( Sint32 bx , Sint32 by );
	void DrawWeaponGauge(Sint32 b , Sint32 px, Sint32 py );
//	void DrawHitPoint();
	void DrawOutOfArea( Sint32 bx , Sint32 by );
	void DrawTime( Sint32 bx , Sint32 by );
	void DrawNowLoading();
	void DrawGameOver();
	void DrawSplash();
	void DrawBlackmask();

	void UISound(Sint32 n);

	static CCockpit *g_sCCockpit;

	CRaderDamage  *m_pRaderDamage;
	CRaderControl *m_pRaderControl;
	CMsgWindow    *m_pMsgWindow;
	CStageClear   *m_pStageClear;

	Sint32 m_sAutoTimer[enSwitchTotal];
	Float32 m_fMessageScroll;
	Sint32 m_sMsgIndex[enMessageMemMax];

	Sint32 m_sArrowWait;
	Sint32 m_sArrowRot;

	Sint32 m_sMissionTime;
	Sint32 m_sMissionSecFlah;

	Sint32 m_sNowLoadingRot;
	Sint32 m_sNowLoadingWait;
	Sint32 m_sConpaneTimer;
	Sint32 m_sMessageWait;
	Sint32 m_sMessagelength;
	Sint32 m_sStopCnt;


	Sint32 m_sWarningFrame;
	Sint32 m_sCockpitTimer;
	Sint32 m_sHeight;

	Sint32 m_sBlackMask;
	Sint32 m_sSplashAlpha;

	gxBool m_bSplash;
	gxBool m_bBlackMask;
	gxBool m_bMessageEnable;
	gxBool m_bDummy;

	//----------------------------------
	//����A�C�R��
	//----------------------------------
	Sint32 m_sWeaponType[2];
	Sint32 m_sWeaponGauge[2];
	Sint32 m_sWeaponRemain[2];


	//----------------------------------
	//�q�b�g�|�C���g�n
	//----------------------------------
	gxBool m_bPlayerOut;
	gxBool m_bGameOver;
	Sint32 m_sWhiteFade;
	Sint32 m_sHitPoint;
	Sint32 m_sDispHitPoint;

	//----------------------------------
	//��ʊO
	//----------------------------------
	Sint32 m_sOperationrangeMin;
	Sint32 m_sOperationrangeMax;
	Sint32 m_sPlayerPosX;

	//----------------------------------
	//�X�R�A�֘A
	//----------------------------------
	Sint32 m_sScore;
	Sint32 m_sScoreWait;
	Sint32 m_sDispScore;

	//----------------------------------
	//�o�[�j�A
	//----------------------------------
	Sint32 m_sBurniaEnergy;
	Sint32 m_sScoreBairitsu;

	void ControlSekigaisen();
	void ControlHitPoint();
	void ControlScore();

	CBellowsSprite *m_pBellowPoint;
	CBellowsSprite *m_pBellowBurn;

	char   *m_pMessage;
	char   m_cShortMsgBuf[256];

	StDanmaku *m_pDanmaku;

	gxBool m_bSwitch[enSwitchTotal];
	gxBool m_bAutoSwitch[enSwitchTotal];
	gxBool m_bSubWeapon;
	gxBool m_bArrow;
	gxBool m_bOutogRange;
	gxBool m_bSystemSound;
};

