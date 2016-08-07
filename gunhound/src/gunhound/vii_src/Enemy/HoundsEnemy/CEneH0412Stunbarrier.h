//--------------------------------------------------------------------------------
//
// Stage00:ƒXƒ^ƒ“ƒoƒŠƒ„
//
//--------------------------------------------------------------------------------
class CEneH0412StunbarrierTate : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0412StunbarrierTate( Sint32 x, Sint32 y );
	~CEneH0412StunbarrierTate();

	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetWaitIndex( Sint32 n );

private:

	enum
	{
		enPlasmaMax = 2,
	};

	ghPos m_Tgt;

	void Atack();
	void LogicAI();

	gxBool m_bDamage;
	gxBool m_bThunder;

	Sint32 m_sAtribute;
	Sint32 m_sWaitTime;
	Sint32 m_sWaitIndex;

	CHitCollision m_HitAtari;
	CEneCmnSpark* m_pCEneCmnSpark;

};


