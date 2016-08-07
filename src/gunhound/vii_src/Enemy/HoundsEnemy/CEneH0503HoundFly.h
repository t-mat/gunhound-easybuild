//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneH0503HoundFly : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0503HoundFly( Sint32 x, Sint32 y );
	~CEneH0503HoundFly();

	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetCarryTarget(Sint32 *px, Sint32 *py)
	{
		m_pTgtX = px;
		m_pTgtY = py;
		m_bCarryOn = gxTrue;
	}

	void SetCarryOff()
	{
		m_bCarryOn = gxFalse;
		m_bCarryOffNow = gxTrue;
	}

	void SetColor(Uint32 uARGB)
	{
		m_uARGB = uARGB;
	}
private:


	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;
	gxBool m_bDamage;
	ghPos m_Add;

	Sint32 *m_pTgtX;
	Sint32 *m_pTgtY;
	Uint32 m_uARGB;
	Sint32 m_sPropTimer;
	Sint32 m_sHigh;
	Sint32 m_sRotation;

	CPenduram* m_pPenduram;

	gxBool m_bCarryOn;
	gxBool m_bCarryOffNow;

};

