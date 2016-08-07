class CEneH0203Body;
class CEneH0203Serpent : public CEnemyBase , public CHoundEnemyBase
{
public:
	typedef struct StChainPos
	{
		Sint32 x,y,r;
	} StChainPos;

	enum 
	{
		enMaxPositionNum = 512,
		enMaxSnakeLength = 9,
		enMaxSnakeSpeed = 220,
	};

	CEneH0203Serpent( Sint32 x, Sint32 y );
	~CEneH0203Serpent();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

	gxBool IsGround()
	{
		return m_bGround;
	}

//	gxBool IsEnd()
//	{
//		return m_bEnd;
//	}

//	void SetDestroy()
//	{
//		m_bDestroy = gxTrue;
//	}
	void Go( Sint32 x ,Sint32 y ,Sint32 r)
	{
		m_Pos.x = x;
		m_Pos.y = y;
		m_sRotation = r;
		m_bGround = gxFalse;
	}

private:

	void Atack();
	void LogicAI();
	void Update();
	void ChildSyori();
	void ResetBodyPosition();
	void CrashSeqBomb();
	void CrashSeqWait();

	Sint32 m_sWait;
	Sint32 m_sSpeed;
	Sint32 m_sPositionCnt;
	Sint32 m_sMineTime;
	Sint32 m_sBombWait;
	Sint32 m_sRemainBody;
	Sint32 m_sAppearWait;
	Sint32 m_sRotation;
	gxBool m_bGround;
//	gxBool m_bEnd;
//	gxBool m_bDestroy;
	gxBool m_bDamage;

	Sint32 m_sControl;
	Sint32 m_sCrashSeq;
	StChainPos m_StChainPos2[ enMaxPositionNum ];

	CHitCollision m_HitKurai;
	CHitCollision m_HitAtari;
	CEneH0203Body* m_pChild[enMaxSnakeLength];
	Sint32 m_sHakaiCnt;

};


//--------------------------------------------------------------------------------
//
// Stage06:“·‘Ì
//
//--------------------------------------------------------------------------------

class CEneH0203Body : public CEnemyBase, public CHoundEnemyBase
{
public:

	CEneH0203Body( Sint32 sID );
	~CEneH0203Body();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetPos(Sint32 x , Sint32 y , Sint32 r)
	{
		m_Pos.x = x;
		m_Pos.y = y;
		m_sRotation = r;
	}

	void SetDestroy()
	{
		m_bDestroy = gxTrue;
	}
	gxBool IsHakai()
	{
		return m_bHakai;
	}

private:
	void Atack();
	void LogicAI();

	Sint32 m_ID;
	Sint32 m_sRotation;
	Sint32 m_sAtackSeq;

	CHitCollision m_HitKurai;
	CHitCollision m_HitAtari;

	gxBool m_bDamage;
	gxBool m_bDestroy;
//	gxBool m_bStart;
	gxBool m_bHakai;
};

