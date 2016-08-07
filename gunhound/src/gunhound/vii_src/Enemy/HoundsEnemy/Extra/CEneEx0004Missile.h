//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneEx0004Missile : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneEx0004Missile( Sint32 x, Sint32 y );
	~CEneEx0004Missile();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:


	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;

	ghPos m_Saki;
	Sint32 m_sAtribute;
	Sint32 m_sRotation;
	Sint32 m_sRebound;
	Sint32 m_sAtackWait;

	gxBool m_bDamage;
};

