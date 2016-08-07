//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneH0207TransItems;
class CEneH0207TransShips : public CEnemyBase , public CHoundEnemyBase
{
public:
	enum {
		enTransItemMax = 6,
	};

	CEneH0207TransShips( Sint32 x, Sint32 y );
	~CEneH0207TransShips();

	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetOperation( Sint32 n , Sint32 opt );

	void SetMove( Sint32 sSpeed = 40 )
	{
		m_sSpeed = sSpeed;
		m_bMoveStop = gxFalse;
	}

	gxBool IsAllItemDead();

//	void SetReduce( Sint32 x , Sint32 y )
//	{
//		m_Pos.x = x;
//		m_Pos.y = y;
//		m_bReduce = gxTrue;
//	}

private:


	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;
//	CEnemyAnime *m_pEnemyAnime;
	gxBool m_bDamage;

	CEneH0207TransItems *m_pCEneH0207TransItems[enTransItemMax];
	OBJ_POS_T m_Add;
	Sint32 m_sAtribute;
	Sint32 m_sSpeed;
	gxBool m_bMoveStop;
//	gxBool m_bReduce;

};

class CEffFire;
class CEneH0207TransItems : public CEnemyBase , public CHoundEnemyBase
{
public:
	CEneH0207TransItems( Sint32 x, Sint32 y , Sint32 sItemType );
	~CEneH0207TransItems();

	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetTarget(OBJ_POS_T* pPos)
	{
		m_pTarget = pPos;
		m_Add.x = m_Pos.x - pPos->x;
		m_Add.y = m_Pos.y - pPos->y;
	}
private:

	OBJ_POS_T *m_pTarget;
	ghPos   m_Add;

	CHitCollision m_HitKurai;
	Sint32 m_sAtribute;
	Sint32 m_sItemType;

	CEffFire *m_pEffFire;

	gxBool m_bDamage;
};

