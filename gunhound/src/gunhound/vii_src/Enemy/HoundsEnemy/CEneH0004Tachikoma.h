//--------------------------------------------------------------------------------
//
// Stage00:タチコマ
//
//--------------------------------------------------------------------------------
class CEneH0004Tachikoma : public CEnemyBase,public CHoundEnemyBase
{
public:

	CEneH0004Tachikoma( Sint32 x, Sint32 y );
	~CEneH0004Tachikoma();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetWaraWara()
	{
		m_bWaraWara = gxTrue;
	}

private:


	void Atack();
	void LogicAI();
	void UpdateTargetPos();

	CHitCollision m_HitKurai;
	CEasyLeynos m_Leynos;

	ghPos m_Tgt;

	Sint32 m_sControl;		//PAD
	Sint32 m_sLogicWait;	//思考時間
	Sint32 m_sJumpWait;		//ジャンプ待機時間

	Sint32 m_sJumpCnt;
	Sint32 m_sAtackLag;
	Sint32 m_sAtackWait;

	gxBool m_bWaraWara;
};

