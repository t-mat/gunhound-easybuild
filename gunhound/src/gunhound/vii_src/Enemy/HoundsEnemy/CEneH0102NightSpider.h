//--------------------------------------------------------------------------------
//
// Stage00:�T���v���G�l�~�[
//
//--------------------------------------------------------------------------------

class CEffFire;
class CEneH0409SlideCannon;
class CEneH0102NightSpider : public CEnemyBase , public CHoundEnemyBase
{
public:
	enum{
		enLogicModeDefault,
		enLogicModeKousoku,
		enLogicModeCrash,
	};
	enum {
		enAtackModeDefault,
		enAtackModeNormal,
		enAtackModeMissile,
		enAtackModeMax,
	};
	enum {
		enCannonMax = 3,
	};

	CEneH0102NightSpider( Sint32 x, Sint32 y );
	~CEneH0102NightSpider();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetDirection(Sint32 sDirection)
	{
		m_sDirection = sDirection;
	}
	void SetLogicMode(Sint32 sMode)
	{
		m_sMode = sMode;
	}
	void SetTargetPosition(Sint32 x)
	{
		m_sLogicSeq = 0;
		m_bCrashTgt = gxTrue;
		m_CrashTgt.x = x;
	}

	gxBool IsLogicSec(Sint32 n)
	{
		return (m_sLogicSeq == n)? gxTrue : gxFalse;
	}

	void SetLogicSec(Sint32 n)
	{
		m_sLogicSeq = n;
	}
private:

	void Atack();
	void LogicAI();
	void LogicNormal();
	void LogicMissile();
	void LogicNormalMode();
	void LogicKousokuScroll();
	void LogicCrashMode();

	gxBool WaraWaraDanmaku();

	void Gimmick();
	void GimmickRebound();
	void GimmickLegOpen();
	void GimmickCannopyOpen();
	void GimmickMaffler();

	CEnemyAnime *m_pEnemyAnime;
	CHitCollision m_HitKurai;
	CHitCollision m_HitShield[16];

	CObj m_stObj;
	static ghPos m_TgtPos;	//�}�t���[�p�̉��|�W�V��������p

	Sint32 m_sAtribute;
	Sint32 m_sLogicSeq;
	Sint32 m_sGround;

	gxBool m_bDamage;
	gxBool m_bJump;

	//�}�t���[����p
	gxBool m_bMaffler;
	ghPos m_Offset;	//�㉺�̐U���p

	//�K�g�����O�K���A���o�E���h����p
	gxBool m_bAbsorber;
	Sint32 m_sAbsorber;
	Sint32 m_sRebound;
	ghPos m_stRebound;

	//�J�r�����p
	gxBool  m_bLegOpen;
	Float32 m_fLegKakudo;

	//�L���m�s�[�����p
	gxBool  m_bOpenCannopy;
	Float32 m_fCannopyKakudo;
	Sint32 m_sMissileTimer;
	Sint32 m_sMissileSeq;
	Sint32 m_sMissileCnt;

	//���W�b�N����p
	Sint32 m_sControl;
	Sint32 m_sMode;
	Sint32 m_sAtackPattern;
	ghPos m_Add;

	//�^�C������p
	StPatrsOffset m_stTire[3];
	ghPos         m_PosTire[3];
	Sint32 m_sTireRot;	//�^�C����]
	Sint32 m_sTireAdd;	//�^�C����]����

	//�V�[���h�h�ꐧ��
	Sint32 m_sShieldHeight;

	Sint32 m_sLogicWait;

	Sint32 m_sAtackSeq;
	Sint32 m_sAtackSec;
	ghPos m_Base;

	Sint32 m_sHitpointShield;
	Sint32 m_sHitpointBody;

	Sint32 m_sCrashSeq;
	gxBool m_bDamageShield;
	gxBool m_bDisp;
	gxBool m_bShield;

	gxBool m_bCrashTgt;
	ghPos  m_CrashTgt;

	CEffFire* m_pEffFire;

	CEneH0409SlideCannon *m_pSlideCannon[enCannonMax];
};

