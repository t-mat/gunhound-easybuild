//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneH0401MovingCannon : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0401MovingCannon( Sint32 x, Sint32 y );
	~CEneH0401MovingCannon();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:


	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;
	CEnemyAnime *m_pEnemyAnime;
	gxBool m_bDamage;

	Sint32 m_sAtribute;
};

