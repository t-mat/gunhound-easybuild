class CEneH0203BigBody;
class CEneH0203Snake : public CEnemyBase , public CHoundEnemyBase
{
public:
	typedef struct StChainPos
	{
		Sint32 x,y,r,dr;
	} StChainPos;

	enum 
	{
		enMaxPositionNum = 512,
		enMaxSnakeLength = 10,
		enMaxSnakeSpeed = 220,
	};

	CEneH0203Snake( Sint32 x, Sint32 y );
	~CEneH0203Snake();

	void ResetBodyPosition();
	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

	gxBool IsCrash()
	{
		return m_bCrash;
	}

private:

	void Atack();
	void LogicAI();
	void LogicNormal();
	void LogicAssault();
//	void MoveJoints(Float32 vRad, Float32 lRad, Float32 distance, Float32 x, Float32 y);

	Sint32 m_sRotation;
	Sint32 m_sDamageFrm;
	Sint32 m_sDamageCnt;
	Sint32 m_sAtackTimer;
	Sint32 m_sCrashTimer;

	ghPos m_Add;
	ghPos m_Base;
	Sint32 m_sLogicTimer;
	Sint32 m_sLogicSeq;
	Sint32 m_sLogicPtn;

	Sint32 m_sCrashCnt;
	Sint32 m_sCrashWait;
	Sint32 m_sDropAtack;

	StChainPos m_StChainPos[ enMaxPositionNum ];
	CEneH0203BigBody* m_pChild[enMaxSnakeLength];
	gxBool m_bCrash;

};


//--------------------------------------------------------------------------------
//
// Stage06:胴体
//
//--------------------------------------------------------------------------------

class CEneH0203BigBody : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0203BigBody( Sint32 sID );
	~CEneH0203BigBody();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetPos(Sint32 x , Sint32 y , Sint32 r)
	{
		m_Old.x = m_Pos.x;
		m_Old.y = m_Pos.y;
		m_Pos.x = x;
		m_Pos.y = y;
		m_sRotation = r;
	}

	void SetDestroy()
	{
		m_bDestroy = gxTrue;
	}

	gxBool IsDamage()
	{
		return m_bDamage;
	}

	void SetFlash()
	{
		m_bFlash = gxTrue;
	}

	void SetAtack(Sint32 sPtn = 0)
	{
		m_bAtack = gxTrue;
		m_sAtackPtn = sPtn;
	}

	gxBool IsCrash()
	{
		if( m_HitKurai.is_dead() )
		{
			return gxTrue;
		}

		return gxFalse;
	}

private:

	void Atack();
	void Atack2();
	void LogicAI();

	Sint32 m_ID;

	Sint32 m_sRotation;
	Sint32 m_sAtackSeq;

	CHitCollision m_HitKurai;
	CHitCollision m_HitAtari;

	ghPos m_Add;
	ghPos m_Old;

	gxBool m_bDamage;
	gxBool m_bDestroy;		//破壊命令を受けた
	gxBool m_bAtack;		//攻撃を行うようセンターからの指令を受ける
	gxBool m_bFlash;		//全身のダメージ判定用

	gxBool m_bCrashHead;	//ヘッドの破壊されフラグ
	gxBool m_bCrashBody;	//ボディの破壊されフラグ
	Sint32 m_sAtackPtn;


};

