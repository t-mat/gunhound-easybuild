//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEffFire;
class CEneH0301Gunpelly : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0301Gunpelly( Sint32 x, Sint32 y ,gxBool bAutoClear = gxFalse);
	~CEneH0301Gunpelly();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:


	void Atack();
	void LogicAI();
	void Syasyutu();

	ghPos m_Add;

	CHitCollision m_HitKurai;

	gxBool m_bDamage;
	gxBool m_bAutoClear;
	Sint32 m_sAtribute;
	Sint32 m_sAtkRot;
	Sint32 m_sAtackWait;
	Sint32 m_sAtackCnt;
	Sint32 m_sAtackPtn;

	Sint32 m_sPropera;
	CEffFire *m_pEffFire;
};

