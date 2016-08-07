//--------------------------------------------------------------------------------
//
// Stage01:éGãõÅiìGÅj
//
//--------------------------------------------------------------------------------
class CEneH0206Bee : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0206Bee( Sint32 x, Sint32 y ,Sint32 sType );
	~CEneH0206Bee();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

private:

	void Atack();
	void LogicAI();

	ghPos m_Add;
	CHitCollision m_HitKurai;
	CHitCollision m_HitAtari;
	Sint32 m_sLogicSeq;
	Sint32 m_sLogicWait;
	Float32 m_fRotation;
	Sint32 m_fSpeed;

	gxBool m_bLeader;
	gxBool m_bDisp;
	gxBool m_bDamage;

};

