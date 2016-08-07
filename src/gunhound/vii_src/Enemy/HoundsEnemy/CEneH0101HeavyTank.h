//--------------------------------------------------------------------------------
//
// Stage01:èdêÌé‘Åiñ°ï˚Åj
//
//--------------------------------------------------------------------------------
class CEneH0101HeavyTank : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0101HeavyTank( Sint32 x, Sint32 y );
	~CEneH0101HeavyTank();

	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetOperation( Sint32 n , Sint32 opt );
	void SetDummy()
	{
		m_bDummy = gxTrue;
	}

	void SetHougeki()
	{
		m_bCannonAtack = gxTrue;
	}
	gxBool IsMokutekichi()
	{
		return m_bMokutekichi;
	}
private:

	void Init();
	void Atack();
	void LogicAI();
	void CannonAtack();

	CHitCollision m_HitKurai;

	Sint32 m_sAtribute;
	Sint32 m_sTagetPosX;
	gxBool m_bMoveStop;
	gxBool m_bDummy;
	CEnemyAnime *m_pEnemyAnime;
	CCatapiller *m_pCatapiller;

	ghPos  m_Add;
	gxBool m_bCannonAtack;
	Sint32 m_sAtackTimer;
	gxBool m_bMokutekichi;
};

