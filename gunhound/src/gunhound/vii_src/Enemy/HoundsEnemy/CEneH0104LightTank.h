//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneH0104LightTank : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0104LightTank( Sint32 x, Sint32 y );
	~CEneH0104LightTank();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:


	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;

	Sint32 m_sLogicTimer;
	gxBool m_bDamage;
	Sint32 m_sAtribute;
};

