//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneH0305Missile : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0305Missile( Sint32 x, Sint32 y ,Sint32 z = PRIO_EFFECT);
	~CEneH0305Missile();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:


	void Atack();
	void LogicAI();

	CHitCollision m_HitAtari;

	Sint32 m_sAtribute;
	Sint32 m_sRotation;
	Sint32 m_sWait;
};


//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------

class CEneH0305Bomb : public CEnemyBase , public CHoundEnemyBase
{
public:
	enum {
		enTypeSmoke,
		enTypeSmokeBlack,
		enTypeFire,
		enTypeBrown,
		enTypeRock,
	};

	CEneH0305Bomb( Sint32 sType , Sint32 x, Sint32 y ,Sint32 z, Sint32 rot , Sint32 spd);
	~CEneH0305Bomb();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:

	CHitCollision m_HitAtari;
	void Atack();
	void LogicAI();

	OBJ_POS_T m_Add;
	Sint32 m_sColor;
	Sint32 m_sScale;

	Sint32 m_sAtribute;
	Sint32 m_sRotation;

	Sint32 m_sType;
	Sint32 m_sTimer;

	Sint32 m_sHigh;
	Sint32 m_sHighAdd;
	Sint32 m_sHighMax;

};

