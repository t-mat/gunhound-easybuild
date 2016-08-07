//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneEx0010Suihei : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneEx0010Suihei( Sint32 x, Sint32 y );
	~CEneEx0010Suihei();

	void SetRight();
	void SeqMain();
	void SeqCrash();
	void Draw();

private:


	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;

	gxBool m_bDamage;
	gxBool m_bDisp;
	Sint32 m_sAtribute;
	Sint32 m_sRotation;
	Sint32 m_sAtackCnt;
	Sint32 m_sRebound;
	Sint32 m_sBlackSmokeWait;

};

