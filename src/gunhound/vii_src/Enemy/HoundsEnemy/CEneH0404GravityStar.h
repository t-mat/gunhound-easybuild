//--------------------------------------------------------------------------------
//
// Stage00:モンスタートレイン
//
//--------------------------------------------------------------------------------
class CEneH0404GravityStar : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0404GravityStar( Sint32 x, Sint32 y );
	~CEneH0404GravityStar();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

private:

	enum {
		enFannelMax  = 4,
	};

	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;
	CHitCollision m_HitAtari;

	CEneH0413Fannel *m_pFannel[enFannelMax];

	ghPos m_Base;
	ghPos m_Add;

	gxBool m_bDamage;
	Sint32 m_sRotation;
	Sint32 m_sFannelRotation;
	Sint32 m_sHigh;
	Sint32 m_sDist;
	Sint32 m_sSeq;
	Sint32 m_sCrashTimer;

	gxBool m_bDispScreen;
};

