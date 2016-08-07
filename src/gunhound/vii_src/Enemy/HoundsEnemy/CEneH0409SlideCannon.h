//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneH0409SlideCannon : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0409SlideCannon( Sint32 x, Sint32 y );
	~CEneH0409SlideCannon();

	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetDead()
	{
		m_bClear = gxTrue;
	}

	void SetRotation(Sint32 sRot)
	{
		m_sTgtRotation = sRot;
	}

	void SetPosition(Sint32 x)
	{
		m_bArrive = gxFalse;
		m_Tgt.x = x;
	}

	void SetAtack(gxBool bAtack,Sint32 sType = -1)
	{
		m_bAtack = bAtack;
		if( sType != -1 ) m_sAtackType = sType;
	}
	gxBool IsArrive()
	{
		return m_bArrive;
	}

	void SetWait(Sint32 n)
	{
		m_sAtackTimer = n;
	}

private:

	ghPos m_Tgt;
	ghPos m_Add;

	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;
	CEnemyAnime *m_pEnemyAnime;
	gxBool m_bDamage;

	Sint32 m_sAtribute;
	Sint32 m_sShotWait;
	Sint32 m_sShotCnt;
	Sint32 m_sShotTotalCnt;
	Sint32 m_sShotlag;

	gxBool m_bAtack;
	Sint32 m_sRotation;
	Sint32 m_sTgtRotation;
	Sint32 m_sAtackType;
	gxBool m_bArrive;

	Sint32 m_sAtackTimer;
	Sint32 m_sSprite;
};

