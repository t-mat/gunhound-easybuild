//--------------------------------------------------------------------------------
//
// Stage00:ƒXƒ^ƒ“ƒoƒŠƒ„
//
//--------------------------------------------------------------------------------
class CEneH0411Stunbarrier : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0411Stunbarrier( Sint32 x, Sint32 y );
	~CEneH0411Stunbarrier();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:
	enum {
		enPlasmaMax = 2,
	};

	void Atack();
	void LogicAI();

	gxBool m_bDamage;
	Sint32 m_sAtribute;

	CHitCollision m_HitAtari;

	CEneCmnSpark* m_pCEneCmnSpark;
};

