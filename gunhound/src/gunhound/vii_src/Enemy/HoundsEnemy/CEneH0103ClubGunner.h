//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneH0103ClubGunner : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0103ClubGunner( Sint32 x, Sint32 y );
	~CEneH0103ClubGunner();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetPrio(Sint32 sPrio )
	{
		m_Pos.z = sPrio;
	}
	void SetTarget(OBJ_POS_T *pPos ,Sint32 sOffset=0)
	{
		m_pTgt    = pPos;
		m_sOffset = sOffset;
	}

private:

	void Atack();
	void LogicAI();
	void LegCalc();

	void LogicBattle();

	CHitCollision m_HitKurai;

	Sint32 m_sAtribute;
	Sint32 m_sLogicSeq;
	Sint32 m_sHigherSpeed;
	Sint32 m_sHeight;
	gxBool m_bDamage;

	gxBool m_bDoubleDamage;
	CEnemyAnime *m_pEnemyAnime;
	StPatrsOffset stLegs[4*3];

	StPartsInfo*   getLeg( Sint32 sType , Sint32 sPos );
	StPatrsOffset* getLegInfo( Sint32 sType , Sint32 sPos );

	Sint32 m_sMotionTimer;
	Sint32 m_sWalkCnt;
	Sint32 m_sControl;

	OBJ_POS_T *m_pTgt;
	Sint32    m_sOffset;
};

