//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneH0407Nautilus : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0407Nautilus( Sint32 x, Sint32 y );
	~CEneH0407Nautilus();

	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetStart()
	{
		m_bStart = gxTrue;
	}

	void GetOffset(Sint32 &x,Sint32 &y)
	{
		x = m_Pos.x-m_Base.x;
		y = m_Pos.y-m_Base.y;
	}

private:


	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;

	OBJ_POS_T m_RidePos;
	ghPos m_Add;
	ghPos m_Base;

	Sint32 m_sAtribute;

	gxBool m_bStart;
	gxBool m_bDamage;
};

