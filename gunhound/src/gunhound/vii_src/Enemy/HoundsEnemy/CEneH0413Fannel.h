//--------------------------------------------------------------------------------
//
// Stage00:ƒtƒ@ƒ“ƒlƒ‹
//
//--------------------------------------------------------------------------------
class CEneH0413Fannel : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0413Fannel( Sint32 x, Sint32 y ,Sint32 sRotation );
	~CEneH0413Fannel();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetParent( OBJ_POS_T* p_Pos , Sint32 sRot )
	{
		m_pTgt = p_Pos;
		m_sRotation = sRot;
	}

	void SetDead()
	{
		m_bDead = gxTrue;
	}

private:

	gxBool Atack();
	void LogicAI();

	Sint32 m_sSprite;
	Sint32 m_sRotation;
	Sint32 m_sAtackRot;

	ghPos  m_Add;
	gxBool m_bDamage;

	OBJ_POS_T *m_pTgt;

	CHitCollision m_HitKurai;

	Sint32 m_sWaitMax;
	Sint32 m_sWaitCnt;

};


