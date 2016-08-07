//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------

class CEneH0002Commanch : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0002Commanch( Sint32 x, Sint32 y ,gxBool bDeadWait=gxFalse);
	~CEneH0002Commanch();

	void Init();
	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetTarget( OBJ_POS_T* pPos )
	{
		m_pTargetPos = pPos;
	}

private:


	void Gimmick();
	void Atack();
	void LogicAI();
	void SetKaiten();

	CHitCollision m_HitKurai;
	OBJ_POS_T m_Add;
	OBJ_POS_T *m_pTargetPos;

	gxBool m_bCrash;
	gxBool m_bDamage;
	gxBool m_bEnd;
	Sint32 m_sAtribute;

	Sint32 m_sRotation;
	Sint32 m_sKatamuki;

	StPatrsOffset stPropera;
	StPatrsOffset stRotar;
	StPatrsOffset stMissilePod;
	gxBool m_bKaiten;
	Sint32 m_sKaitenDir;
	Sint32 m_sCrashTimer;

	CEnemyAnime *m_pEnemyAnime;

};

