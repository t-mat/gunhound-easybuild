//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneH0406Carrymissile;
class CEneH0405Nancy : public CEnemyBase , public CHoundEnemyBase
{
public:
	enum {
		enSizeLarge,
		enSizeMiddle,
		enSizeSmall,
	};
	CEneH0405Nancy( Sint32 x, Sint32 y , Sint32 sSize=0 );
	~CEneH0405Nancy();

	void SeqMain();
	void SeqCrash();
	void Draw();

	CEneH0406Carrymissile* GetMissile()
	{
		return m_pTorpedo;
	}
	void SetMissile();
	void SetAdjust(gxBool bFlag )
	{
		m_bAutoAdjust = bFlag;
	}

	void SetStop( Sint32 sRot , Sint32 sSpd )
	{
		m_bStop = gxTrue;
	}
private:


	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;

	Sint32 m_sAtribute;
	Sint32 m_sSize;
	Sint32 m_sMissileTimer;

	ghPos m_MissilePos;
	ghPos m_MissileAdd;
	gxBool m_bMissile;
	gxBool m_bDamage;
	gxBool m_bAutoAdjust;
	gxBool m_bStop;

	CEneH0406Carrymissile  *m_pTorpedo;						//魚雷
};

