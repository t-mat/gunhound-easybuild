//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneH0106Yagura : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0106Yagura( Sint32 x, Sint32 y );
	~CEneH0106Yagura();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:

	void Atack();
	void LogicAI();

	gxBool m_bDamage;
	CEnemyAnime *m_pEnemyAnime;
	CHitCollision m_HitKurai;

	Sint32 m_sAtribute;
	Sint32 m_sFireTimer;

	Sint32 m_sAtackTimer;
	Sint32 m_sRotation;
};

