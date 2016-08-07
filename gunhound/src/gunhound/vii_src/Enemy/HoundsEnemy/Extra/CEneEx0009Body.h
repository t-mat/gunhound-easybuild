//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneEx0009Body : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneEx0009Body( Sint32 x, Sint32 y );
	~CEneEx0009Body();

	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetAtariHantei()
	{
		m_bAtari = gxTrue;
	}

private:


	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;

	gxBool m_bDamage;
	gxBool m_bDisp;
	gxBool m_bAtari;
	gxBool m_bDummy;

	Sint32 m_sAtribute;
	Sint32 m_sRotation;

};

