//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneEx0007Nozzle : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneEx0007Nozzle( Sint32 x, Sint32 y );
	~CEneEx0007Nozzle();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:


	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;
	ghPos m_Add;

	gxBool m_bDamage;

	Sint32 m_sAtribute;
	Sint32 m_sRotation;

};

