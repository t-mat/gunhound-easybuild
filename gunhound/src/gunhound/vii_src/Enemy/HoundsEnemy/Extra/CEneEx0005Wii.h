//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneEx0005Wii : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneEx0005Wii( Sint32 x, Sint32 y );
	~CEneEx0005Wii();

	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetAtariHantei()
	{
		m_bAtari = gxTrue;
	}

	void SetHakkyouMode();
	gxBool IsDamage()
	{
		if( m_sDamageWait > 0)
		{
			return gxTrue;
		}
		return gxFalse;
	}

private:


	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;

	gxBool m_bDamage;
	gxBool m_bAtari;
	Sint32 m_sAtribute;
	Sint32 m_sRotation;
	Sint32 m_sDamageWait;
	gxBool m_bhakkyouMode;
	gxBool m_bDummy;

};

