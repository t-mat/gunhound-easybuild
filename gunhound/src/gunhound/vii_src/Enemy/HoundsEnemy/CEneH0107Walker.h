//--------------------------------------------------------------------------------
//
// Stage00:ÉEÉTÉM
//
//--------------------------------------------------------------------------------
class CEneH0107Walker : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0107Walker( Sint32 x, Sint32 y );
	~CEneH0107Walker();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

private:


	void Atack();
	void LogicAI();
	void SeqJump();

	CHitCollision m_HitKurai;
	CHitCollision m_HitAtari;
	gxBool m_bDamage;
	Sint32 m_sAtribute;
	ghPos  m_Add;

	gxBool m_bJibaku;
	Sint32 m_sJibakuCnt;
	Sint32 m_sBombWait;

	Sint32 m_sLegRotation;
	Sint32 m_sSprite;
	Sint32 m_sSequence;

	CObj  m_Obj;

	gxBool m_bChakuti;
	Sint32 m_sJumpWait;
};

