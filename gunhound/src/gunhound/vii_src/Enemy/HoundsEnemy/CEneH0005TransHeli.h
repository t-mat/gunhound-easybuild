//--------------------------------------------------------------------------------
//
// Stage00:–¡•ûƒwƒŠ
//
//--------------------------------------------------------------------------------

class CEneH0005TransHeli : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0005TransHeli( Sint32 x, Sint32 y );
	~CEneH0005TransHeli();

	void Init();
	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetTarget( OBJ_POS_T* pPos )
	{
		m_pTargetPos = pPos;
	}

	void SetMaxSpeed(Sint32 n)
	{
		m_sMaxSpeed = n;
	}

	void SetBomb()
	{
		m_sBombSeq = 100;
	}
private:

	void Gimmick();
	void Atack();
	void LogicAI();
	void SetKaiten();
	void LogicRyoki();

	CHitCollision m_HitKurai;
	OBJ_POS_T m_Add;
	OBJ_POS_T *m_pTargetPos;

	gxBool m_bCrash;
	gxBool m_bDamage;
	Sint32 m_sAtribute;

	Sint32 m_sRotation;
	Sint32 m_sKatamuki;

	StPatrsOffset stPropera;
	StPatrsOffset stRotar;
	StPatrsOffset stMissilePod;
	gxBool m_bKaiten;
	Sint32 m_sKaitenDir;
	Sint32 m_sCrashTimer;

	Sint32 m_sMaxSpeed;
	Sint32 m_sBombSeq;
	CEnemyAnime *m_pEnemyAnime;

};

