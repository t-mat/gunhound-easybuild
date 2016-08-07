//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneEx0003Shot : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneEx0003Shot( Sint32 x, Sint32 y , Sint32 sDir=0 );
	~CEneEx0003Shot();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:


	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;
	CHitCollision m_HitShield[2];

	gxBool m_bDamage;
	Sint32 m_sAtribute;
	Sint32 m_sRotation;
	Sint32 m_sAtackWait;
	Sint32 m_sBlackSmokeWait;

};
