//--------------------------------------------------------------------------------
//
// Stage04:トータス
//
//--------------------------------------------------------------------------------
class CEneH0403Tarantura : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0403Tarantura( Sint32 x, Sint32 y );
	~CEneH0403Tarantura();

	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetTargetPos(Sint32 x , Sint32 y);

private:


	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;

	ghPos m_Add;
	ghPos m_Tgt;
	ghPos m_Offset;
	OBJ_POS_T m_Disp;
	CEneCmnSignal m_CEneCmnSignal;

	Sint32 m_sAtribute;
	Sint32 m_sPositionIndex;
	Sint32 m_sBaseLine;
	Sint32 m_sCrashWait;

	CEnemyAnime *m_pEnemyAnime;
	gxBool m_bDamage;

	Sint32 m_sAtackTimer;
};


//--------------------------------------------------------------------------------
//
// Stage04:スライドエフェクト
//
//--------------------------------------------------------------------------------


class CEneH0404SlashLine : public CEnemyBase , public CHoundEnemyBase
{
public:
	
	CEneH0404SlashLine( Sint32 x, Sint32 y );
	~CEneH0404SlashLine();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:

	enum {
		enBackSpeed = 800,
	};

	OBJ_POS_T m_Add;

	Sint32 m_sColor;
	Sint32 m_sScale;
};

