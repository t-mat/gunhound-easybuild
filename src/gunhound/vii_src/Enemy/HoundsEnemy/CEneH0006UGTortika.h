//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
typedef struct StCannon
{
	Sint32 x,y,dist,rot;

}StCannon;

class CEneH0006UGTortika : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0006UGTortika( Sint32 x, Sint32 y ,gxBool bReverse=gxFalse);
	~CEneH0006UGTortika();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

private:


	void Gimmick();
	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;
	CEnemyAnime *m_pEnemyAnime;
//	StPatrsOffset m_stCannon[4];
	Sint32 m_sHeight;
	Sint32 m_sRotation[2];

	Sint32 m_sRebound[2];

	gxBool m_bDamage;
	Sint32 m_sAtribute;

	StCannon m_stCannon[4];

	Sint32 m_sHigh;
	Sint32 m_sAtackWait;
	Sint32 m_sControl;

	Sint32 m_sReverse;

	ghPos m_Add;

};

