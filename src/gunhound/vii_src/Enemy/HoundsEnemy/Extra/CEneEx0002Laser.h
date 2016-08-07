//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneEx0002Laser : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneEx0002Laser( Sint32 x, Sint32 y );
	~CEneEx0002Laser();

	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetOffset(Sint32 n)
	{
		m_sOffset = n;
	}

	void SetType(Sint32 sType )
	{
		m_sType = sType;
	}

private:


	void Atack();
	void LogicAI();
	void Gimmick();
	void AtackDrop();

	CHitCollision m_HitKurai;

	gxBool m_bDamage;
	gxBool m_bDisp;

	Sint32  m_sAtribute;
	Float32 m_fRotation;
	Sint32  m_sCrashSeq;

	ghPos m_CannonRoot[2];
	ghPos m_CannonBase[2];
	ghPos m_CannonSaki[2];

	Sint32 m_sTackLag;
	Sint32 m_sRebound;
	Sint32 m_sOffset;
	Sint32 m_sType;
	Sint32 m_sAtackCnt;
	Sint32 m_sAtackWait;
	Sint32 m_sSyuhouCnt;
};

