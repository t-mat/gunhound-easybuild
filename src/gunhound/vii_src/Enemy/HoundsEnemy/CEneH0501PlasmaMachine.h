//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneCmnSpark;
class CEneH0501PlasmaMachine : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0501PlasmaMachine( Sint32 x, Sint32 y );
	~CEneH0501PlasmaMachine();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:

	enum {
		enPlasmaMax=8,
	};

	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;
	CEnemyAnime *m_pEnemyAnime;
	gxBool m_bDamage;
	gxBool m_bPlasmaDraw;

	Sint32 m_bDisp;
	Sint32 m_sAtribute;
	Sint32 m_sCrashSeq;

	CEneCmnSpark* m_pCEneCmnSpark[enPlasmaMax];
};

