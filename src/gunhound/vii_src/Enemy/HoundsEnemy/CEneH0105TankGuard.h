//--------------------------------------------------------------------------------
//
// Stage00:åÏâqêÌé‘
//
//--------------------------------------------------------------------------------
class CEneH0105TankGuard : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0105TankGuard( Sint32 x, Sint32 y ,gxBool bDummy = gxFalse);
	~CEneH0105TankGuard();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetTarget( OBJ_POS_T *p ,Sint32 sOffset = 0)
	{
		m_pTgt = p;
		m_sOffset = sOffset;
	}

	void SetRealAtack()
	{
		m_bAtackable = gxTrue;
	}

private:

	gxBool Atack();
	void LogicAI();

	Sint32 m_sSprite;
	Sint32 m_sOffset;
	Sint32 m_sPosition;
	gxBool m_bDamage;
	gxBool m_bAtackable;
	gxBool m_bDummy;

	gxBool m_bPadding;

	CObj m_Obj;
	OBJ_POS_T *m_pTgt;

	CHitCollision m_HitKurai;

};


