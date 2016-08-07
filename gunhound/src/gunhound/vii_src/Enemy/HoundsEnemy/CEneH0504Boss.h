//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
enum {
	enRight,
	enLeft,
	enArmMax,
};

class CBossBody;
class CBossArm;
class CBossPack;
class CEneH0504Boss : public CEnemyBase , public CHoundEnemyBase
{
	enum {
		enAtackGattling,
		enAtackReppuken,
		enAtackLaser,
		enAtackMissile,
		enAtackCatch,
		enAtackMax,
	};

	typedef struct StGumMode {
		Sint32 m_sSeq;
		Sint32 m_sWait;
		Sint32 m_sLeft;
		Sint32 m_sRight;
	}StGumMode;

	typedef struct StHang {
		Sint32 m_sRotation;
		Sint32 m_sDistance;
		Sint32 m_sGahaCounter;
		Sint32 m_sSeq;
		Sint32 m_sWait;

		Sint32 m_sBodyRot;
		Sint32 m_sArmRot[2];
		Sint32 m_sHandRot[2];
	}StHang;

public:

	CEneH0504Boss( Sint32 x, Sint32 y );
	~CEneH0504Boss();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

	void Go()
	{
		m_bStart = gxTrue;
	}
	gxBool IsChumoku()
	{
		return m_bChumoku;
	}

	gxBool IsCrash()
	{
		return m_bCrash;
	}

private:

	void Gimmick();

	gxBool Atack();
	void LogicAI();
	void LogicCPU();
	void LogicCatchMode();
	void LogicGunMode();
	void LogicUpperMode();
	void LogicMissileMode();
	void LaserBeemMode();

	void LaserBeemAtack();
	void CatchTheEnemy(Sint32 n , StHang* pHang=NULL);
	Float32 normalize(Float32 fr);
	gxBool IsCatchArea();

	CHitCollision m_HitKurai;

	Sint32 m_sAtribute;
	Sint32 m_sControl;

	Sint32 m_sAtackSeq;
	Sint32 m_sTargettingCnt;

	Sint32 m_sCrashSeq;
	Sint32 m_sCrashWait;

	Sint32 m_sCrashArm[2];
	Sint32 m_sCrashCannopy;

	Sint32 m_sHigh;
	Sint32 m_sInitSeq;
	Sint32 m_sInitWait;

	ghPos m_TgtPos;
	ghPos m_Through;
	ghPos m_Add;

	StHang m_stHang;
	StHang m_stUpper;
	StHang m_stMissile;
	StHang m_stLaser;

	StGumMode m_stGunMode;

	CBossBody* m_pBody;
	CBossArm * m_pArm[enArmMax];
	CBossPack* m_pPack;

	gxBool m_bMissile;
	gxBool m_bHanging;
	gxBool m_bGunMode;
	gxBool m_bUpperMode;
	gxBool m_bLaserMode;
	gxBool m_bChumoku;
	gxBool m_bStart;

	gxBool m_bCrash;
};

/*
	CEnemyAnime *m_pEnemyAnime;
	gxBool m_bDamage;


	//---------------------------------------------------

	m_bDamage = gxFalse;
	m_pEnemyAnime = new CEnemyAnime();

	for(Sint32 ii=0; ii<enMax ;ii++)
	{
		m_pEnemyAnime->SetParts( ii , enMax , m_sParent@@@@@@@@[ii] , sPos@@@@@@@@[ii] );
	}

	m_pEnemyAnime->PosCalc();

	//---------------------------------------------------

	delete m_pEnemyAnime;

	//---------------------------------------------------
	Logic

	m_pEnemyAnime->PosCalc();
//	m_pEnemyAnime->GetParts( enKOUSYAHOU2 )->dr = ( viiMath::Cos100( (m_sTimer*8)%360 )*45 )/100;

	//---------------------------------------------------

	m_pEnemyAnime->Draw( m_bDamage , m_Pos.x , m_Pos.y , ( m_sDirection == DIR_LEFT )? gxTrue : gxFalse , &Spr@@@@@@@@[0] );
*/
