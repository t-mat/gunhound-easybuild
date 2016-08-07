//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEffectFinalExplosionDeluxe;
class CEneEx0001Cannon : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneEx0001Cannon( Sint32 x, Sint32 y );
	~CEneEx0001Cannon();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:

	gxBool m_bDamage;
	gxBool m_bDisp;

	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;
	CEffectFinalExplosionDeluxe *m_pFinalExplosion;
	Sint32 m_sCrashTimer;
	Sint32 m_sAtribute;
};

