//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneH0406Carrymissile : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0406Carrymissile( Sint32 x, Sint32 y );
	~CEneH0406Carrymissile();

	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetAssault()
	{
		m_sOpenSeq = 5;
	}

	void SetTarget(Sint32 x, Sint32 y)
	{
		m_Tgt.x = x;
		m_Tgt.y = y;
	}

	void Destroy()
	{
		m_bDestroy = gxTrue;
	}
	void SetControl(Sint32 sJoy)
	{
		m_sJoy = sJoy;
	}

private:


	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;
	CEnemyAnime *m_pEnemyAnime;
	gxBool m_bDamage;

	Sint32 m_sAtribute;

	Sint32 m_sOpenSeq;
	Sint32 m_sOpenTimer;
	Sint32 m_sControl;
	Sint32 m_sJoy;

	ghPos m_Tgt;
	ghPos m_Add;

	ghPos FutaPos[2];
	ghPos FutaAdd[2];
	ghPos HoundPos;
	ghPos HoundAdd;
	ghPos HoundTgt;
	gxBool m_bDestroy;
};

