//--------------------------------------------------------------------------------
//
// Stage00:パンチングボール
//
//--------------------------------------------------------------------------------
class CEneH0000PunchBall : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0000PunchBall( Sint32 x, Sint32 y );
	~CEneH0000PunchBall();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:


	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;

	Sint32 m_sAtribute;
	CPenduram* m_pPenduram;
	ghPos m_Add;
	ghPos m_Base;

	Sint32 m_sWait;
	Sint32 m_sCount;
};

