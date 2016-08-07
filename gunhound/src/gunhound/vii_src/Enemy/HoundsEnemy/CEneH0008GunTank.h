//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneH0008GunTank : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0008GunTank( Sint32 x, Sint32 y ,gxBool bDummy=gxFalse);
	~CEneH0008GunTank();

	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetTarget( OBJ_POS_T *pPos )
	{
		m_pTgt = pPos;
	}

private:

	void Init();
	void Atack();
	void AtackGatring();
	void Gimmick();
	void LogicAI();

	CHitCollision m_HitKurai;

	Sint32 m_sGatringRot;
	Sint32 m_sGatringRotAdd;
	Sint32 m_sAtribute;
	Sint32 m_sControl;
	gxBool m_bDamage;

	CEnemyAnime *m_pEnemyAnime;
	CCatapiller *m_pCatapiller;
	OBJ_POS_T m_Add;

	StPatrsOffset stArm;
	StPatrsOffset stGtl;

	Sint32 m_sRotation;
	Sint32 m_sAtackWait;
	Sint32 m_sArmRebound;
	Sint32 m_sReboundWait;
	gxBool m_bDummy;

	OBJ_POS_T *m_pTgt;
};

