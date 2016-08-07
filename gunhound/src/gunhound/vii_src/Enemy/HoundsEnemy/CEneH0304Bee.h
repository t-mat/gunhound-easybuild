//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneH0304Bee : public CEnemyBase
{
public:

	CEneH0304Bee( Sint32 x, Sint32 y ,gxBool bScrollWait = gxTrue );
	~CEneH0304Bee();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

private:

	OBJ_POS_T m_Add;

	void Atack();
	void LogicAI();

	CHitCollision m_HitAtari;
	CHitCollision m_HitKurai;

	gxBool m_bDamage;
	Sint32 m_sLogicTimer;
	Sint32 m_sAnimeCount;
	Sint32 m_sBombWait;

	CEneCmnSignal m_CEneCmnSignal;
	gxBool m_bScrollWait;
};

