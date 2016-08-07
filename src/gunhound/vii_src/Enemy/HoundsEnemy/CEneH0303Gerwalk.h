//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneH0303Valkylie : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0303Valkylie( Sint32 x, Sint32 y );
	~CEneH0303Valkylie();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:


	void Init();
	void Atack();
	void AtackValcun(Sint32 x ,Sint32 y);
	void LogicAI();
	void LogicGimmick();
	void GimmickArm();
	void GimmickLeg();

	Sint32 m_sLogicSeq;
	Sint32 m_sLogicWait;
	Sint32 m_sControl;

	CHitCollision m_HitKurai;
	CEnemyAnime *m_pEnemyAnime;
	gxBool m_bDamage;
	gxBool m_bDisp;

	StPatrsOffset m_stFoot[2];
	StPatrsOffset m_stLeg[2];
	StPatrsOffset m_stKakato[2];
	StPatrsOffset m_stArm[2];

	Sint32 m_sWalkCycle;

	Sint32 m_sArmCycle;
	Sint32 m_sHandCycle;
	Sint32 m_Foot[2],m_Leg[2];

	Sint32 m_sAtribute;
	Sint32 m_sAtackCnt;

	Sint32 m_sBuraBuraArm;
	Sint32 m_sBuraBura[2];
	Sint32 m_sBuraTime[2];

	ghPos m_Add;
	ghPos m_Tgt;
	ghPos m_Base;

	ghPos m_stTarget;
	ghPos m_Drop;

	Sint32 m_sAtackSeq;
	Sint32 m_sCrashSeq;
	Sint32 m_sCrashTimer;
	Sint32 m_sCrashCnt;
	CEffFire* m_pEffFire;

};

