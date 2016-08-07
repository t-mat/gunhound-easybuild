//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
#include "../CPenduram.h"
#include "HoundsEnemy/CEneCommon.h"
#include "../CMechBone.h"

enum {
	//ウェポンゲージアイコン
	enWeaponIconNone,
	enWeaponIconBullet,
	enWeaponIconPunch,
	enWeaponIconMissile,
	enWeaponIconRocket,
};

enum {
	//ゲージタイプ
	enWeaponGaugeNone,
	enWeaponGaugeOne,
	enWeaponGaugeTwo,
	enWeaponGaugeThree,
	enWeaponGaugeFour,
	enWeaponGaugeMeter,
};

typedef struct StPunch
{
	//パンチ管理用の構造体
	Sint32 sAtackCnt;
	Sint32 timelag;
	CHitCollision m_HitAtari;
	StPunch()
	{
		sAtackCnt = 1;
		timelag = 0;
	}
}StPunch;

enum {
	enShotN1,
	enShotN2,
	enShotS1,
	enShotS2,
	enShotMax,
};

typedef struct StArms
{
	//-------------------------------
	//攻撃関連の構造体
	//-------------------------------

//	Sint32 m_sWeapon1Rotation;
//	Sint32 m_sWeapon2Rotation;
//	Sint32 m_sWeapon3Rotation;
	Sint32 sType;

	Sint32 sRotation;

	//	Sint32 m_sGattlingSpd;	//ガトリングガン用スピードカウンタ
	Sint32 sRotSpd;

	//	Sint32 m_sGattlingCnt;	//ガトリングガン用カウンタ
	Sint32 sRotCnt;

	//マガジン交換までのタイムラグ
	//Sint32 m_sRelordTime;
	Sint32 sRelordTime;

	//発射間隔の制御用
	//Sint32 m_sAtackTimeLag;
	//Sint32 m_sSubWeaponTimeLag;
	Sint32 sTimeLag;

	//残弾数
	//Sint32 m_sMagazine;
	Sint32 sRemain;
	Sint32 sMaxRemain;

	//Sint32 m_sSubWeaponShotCnt;

	Sint32 sIcon;			//
	Sint32 sRemainBullet;	//0:ONE / 1:TWO /2:THREE / 3:
	Sint32 sGaugeType;		//0:None 

	StArms()
	{
		sType = 0;	//武器の種類
		sRotation = 0;
		sRotSpd = 0;
		sRotCnt = 0;
		sRelordTime = 0;
		sTimeLag = 0;
		sRemain  = 0;


		sIcon          = 0;
		sRemainBullet  = 0;
		sGaugeType      = 0;

	}

	void Relord()
	{
		//弾丸をマックスに戻す
		sRemain = sMaxRemain;
		sRemainBullet = sRemain;
	}

	gxBool IsEmpty()
	{
		//弾丸切れた？
		return (sRemain <= 0)? gxTrue : gxFalse;
	}

}StArms;


//--------------------------------------------------------------------------------
//
// プレイヤー機
//
//--------------------------------------------------------------------------------
class CWazaCommand;
class CGunHound : public CEnemyBase
{
public:
	enum {
		enHitPointStausNormal,
		enHitPointStausAlert,
	};

	enum {
		enHoundModeNone,
		enHoundModeNormal,
		enHoundModeSpace,
		enHoundModeGake,
		enHoundModeKickStart,
		enHoundModePodStart,
		enHoundModeCarreer,
		enHoundModeNPC,
	};

	enum {
		enDamageBody,
		enDamageArmR,
		enDamageLegL,
		enDamageLegR,
		enDamageMax,
	};

	enum {
		enZettaiMuteki = -1,
	};

	CGunHound( Sint32 x, Sint32 y  ,gxBool bPlayer );
	~CGunHound();

	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetAdd(Sint32 x , Sint32 y)
	{
		m_Add.x = x;
		m_Add.y = y;
	}
	void SetColor(Sint32 sCol)
	{
		m_sColor = sCol;
		m_pMech->SetColor(sCol);
	}

	//動作モードを決定する
	void SetMode(Sint32 eMode);

	void SetCarryOff();
	void SetSignal()
	{
		//信号をセット
		m_bSignal = gxTrue;
	}

	void SetRide( gxBool bFlag , OBJ_POS_T* pPos )
	{
		//未使用
		m_bRideOn = bFlag;
		if( m_bRideOn )
		{
			m_pRidePos = pPos;
		}
	}

	void SetPadButton( Sint32 sPadPush=0, Sint32 sPadTrig=0 )
	{
		//強制パッドコントロール
		m_bCpuControl = gxTrue;
		m_sForcePadControlPush = sPadPush;
		m_sForcePadControlTrig = sPadTrig;
	}

	Sint32 GetArmRotation()
	{
		//ハウンドの腕の角度を返す（背景スクロール制御用）
		return m_sArmRotation;
	}

	Sint32 GetDirection()
	{
		//HOUNDの方向を返す（背景スクロール制御用）
		return m_sDirection;
	}

	void SetWaterProof(gxBool bFlag = gxTrue)
	{
		//ウォーターエフェクトを有効にする
		m_bWaterEffect = bFlag;
	}

	void SetForceRollerDash(gxBool bFlag)
	{
		//強制ローラーダッシュモードにする
		m_bForceRollerDash = bFlag;
	}

	void SetForceShift(gxBool bFlag)
	{
		//強制武器シフト
		m_bForceShift = bFlag;
	}

	void SetForceBurnia(gxBool bFlag)
	{
		//強制武器シフト
		m_bForceBurnia = bFlag;
	}

	OBJ_POS_T* GetKasoku()
	{
		return &m_Force;
	}

	void SetMuteki()
	{
		m_sMutekiTime = enZettaiMuteki;
	}

	gxBool IsDestoroyed()
	{
		return m_bGameOver;
	}

	//スクロールにはさまれるなど強制的に終わりにしたい場合に使用する
	void SetGameOver();

	gxBool AddHitPoint(Sint32 sPoint);

	void SetSearchLight(gxBool bFlag)
	{
		m_bSearchLight = bFlag;
	}

	gxBool IsOutofScreen( Sint32 sx , Sint32 sy );


	//--------------------------------------
	//飛び出し制御
	//--------------------------------------
	void SetTobidashi(gxBool bFlag=gxTrue)
	{
		m_bForceTobidasi = bFlag;
	}
	gxBool GetTobidashi()
	{
		return m_bForceTobidasi;
	}
	gxBool IsChakuti()
	{
		return m_bChakuti;
	}

	gxBool IsMuteki()
	{
		//"絶対無敵"の判定
		//スクロールに巻き込まれてもやられない
		if( m_sMutekiTime == enZettaiMuteki ) return gxTrue;

		return gxFalse;
	}

	//武器の設定
	void SetWeapon(Sint32 n , Sint32 sType);

//	void SetDefault();
//	void SetPosition( StSprPos *pPos );
//	void SetSprite( StSprConfig* pMecha , StSprConfig* pBody , StSprConfig* pArmor );
	void SetHyperBooster(gxBool bFlag = gxTrue)
	{
		m_bInfinityHover = bFlag;
		SetBooster( bFlag );
	}

	void SetLeader(gxBool bflag);

	void SetBooster(gxBool bflag);

	Sint32 GetHitPointStatus();

	void SetLogicPattern(Sint32 n)
	{
		m_sNPCLogicPattern = n;
		m_sNPCLogicCnt = 0;
	}
	void SetNoDamage(gxBool bFlag)
	{
		m_bNoDamage = bFlag;
	}
	void SetTarget( Sint32 x ,Sint32 y ,OBJ_POS_T *pTgt=NULL)
	{
		m_NpcTgt.x = x;
		m_NpcTgt.y = y;
	}

	gxBool IsDead()
	{
		return m_bDead;
	}

	Sint32 GetHitPoint()
	{
		return m_sHitPoint;
	}
	void SetLeft()
	{
		m_sDirection = -1;
	}
	void SetAutoPunch()
	{
		m_bAutoPunch = gxTrue;
	}

	void Reset();

private:

	//至近距離に敵がイルカ？
	gxBool IsNeedPunch( );

	void Atack();
	void LogicAI();
	void RecoveryHitPoint();
	void ControlArm();
	void ControlDirection(Sint32 sDir);
	void SetDamagePosition();
	void Crash();
	void Controls();
		void ControlTypeNormal();
			void ControlMoveNormal();
			void ControlMoveJump();

	void ControlTypeSpace();
		void ControlMoveSpace();

	void ControlTypeGake();
		void ControlMoveGake();

	void ControlCarry();
	void ControlNPC();

	//---------------------------------------
	//攻撃
	//---------------------------------------
	void ControlAtack();
	void ControlList( Sint32 sAtackType ,Sint32 n);

	void MachineGun(Sint32 n);
	void ThreeWayShot(Sint32 n);

	void HardBrow(Sint32);
	void ArmGattling(Sint32 n);
	void StrikeCrow(Sint32 n);
	void ZGokCrow(Sint32 n);
	void ShieldCrow(Sint32 n);

	void Bazooka(Sint32 n);
	void ShotGun(Sint32 n);
	void Napalm(Sint32 n);

	void MicroMisiile(Sint32 n);
	void LinarCannon(Sint32 n);
	void GrenadeCannon(Sint32 n);
	void GunCannon(Sint32 n);

	//---------------------------------------
	//ギミック
	//---------------------------------------
	void ControlGimmick();
		void GimmickApogyMotor( Sint32 sControl );
		void GimmickWaterEffect();
		void GimmickRollerEffect();
		void GimmickSandStorm();
		void GimmickGattling();
		void GimmickLanding();
		void GimmickBoost();
		void SetSandStorm(Sint32 n)
		{
			m_sSandStormCnt = n;
		}

	//----------------------------------------
	//その他サブルーチン
	//----------------------------------------
	gxBool IsNoDamage()
	{
		return m_bNoDamage ;
	}

	gxBool IsRollerDash()
	{
		return (m_sRollerCnt)? gxTrue : gxFalse;
	}

	void SpeedDown();

	gxBool IsDashControl();

	void RecoverHoverEnergy();
	//---------------------------
	//必須変数
	//---------------------------
	Sint32 m_eMode;

	OBJ_POS_T m_Add;
	OBJ_POS_T m_Old;
	CObj      *m_pObj;
	CMechBone *m_pMech;
	CPenduram *m_pPenduram;

	Sint32 m_sControlPush;
	Sint32 m_sControlTrig;
	Sint32 m_sArmRotation;
	Sint32 m_sDirection;
	Sint32 m_sGravitation;
	Sint32 m_sHitPoint;
	Sint32 m_sMutekiTime;
	Sint32 m_sHitPointStatus;

	//---------------------------
	//ジャンプ関連
	//---------------------------
	gxBool m_bChakuti;
	gxBool m_bSearchLight;
	Sint32 m_sJumpSeq;
	Sint32 m_sJumpRate;

	Sint32 m_sBoosterRot;
	Sint32 m_sHoverEnergy;
	Sint32 m_sHoverSpd;
	Sint32 m_sHoverLag;
	gxBool m_bInfinityHover;
	gxBool m_bHoverAble;

	gxBool m_bLeader;
	gxBool m_bBooster;

	gxBool m_bNoDamage;
	gxBool m_bOutofScreen;
	//---------------------------
	//---------------------------

	gxBool m_bForceRollerDash;
	gxBool m_bGameOver;
	Sint32 m_sRollerCnt;	//ローラーダッシュ用カウンタ
	Sint32 m_sJumpCnt;		//ジャンプ用カウンタ
	Sint32 m_sFreezeWait;

	Sint32 m_sSandStormCnt;	//砂埃カウンタ
	Sint32 m_sPadRotation;
	Sint32 m_sPadSpeed;

	CHitCollision m_HitKurai[enDamageMax];
	CHitCollision m_HitBarrier;
	StPunch m_stPunch;				//パンチ制御構造体
	StArms  m_stAtack[enShotMax];	//攻撃関連構造体
	gxBool  m_bSubWeaponReady;		//シフトしているか？
	gxBool  m_bSwitchNow;			//武器切り替え中
	Sint32  m_sColor;

	gxBool m_bOffenceControll;
	gxBool m_bPlayer;

	//---------------------------
	//削除予定
	//---------------------------
	gxBool m_bBoost;	//バーニアエフェクトを出したいときにたてる（外部からは初期化されるので無理）
	gxBool m_bBurnia;	//バーニアエフェクト一回目にのみON
	gxBool m_bRideOn;

	OBJ_POS_T *m_pRidePos;

	OBJ_POS_T m_Force;

	CWazaCommand *m_pWazaCommand;

	ghPos m_NpcTgt;
	Sint32 m_sNPCLogicPattern;
	Sint32 m_sNPCLogicCnt;
	Sint32 m_sFreezeTimeMax;
	Sint32 m_sJumpOmosi;
	gxBool m_bLightWeightHound;
	//---------------------------
	//自動設定される
	//---------------------------
	Sint32 m_bCpuControl;
	Sint32 m_sForcePadControlPush;
	Sint32 m_sForcePadControlTrig;
	Sint32 m_sGravitSpeed;
	gxBool m_bWaterEffect;	//水面モード
	Sint32 m_sArmor[enDamageMax];
	Sint32 m_sHitEffectCnt[enDamageMax];

	CEneCmnSignal m_CEneCmnSignal;
	gxBool m_bSignal;
	gxBool m_bForceShift;
	gxBool m_bForceBurnia;
	gxBool m_bForceTobidasi;
	gxBool m_bDead;
	gxBool m_bAutoPunch;
	gxBool m_bDummy2;	//パディング用
};

