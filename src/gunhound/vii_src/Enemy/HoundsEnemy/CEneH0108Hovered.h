//--------------------------------------------------------------------------------
//
// Stage00:ホバー戦車
//
//--------------------------------------------------------------------------------
class CEneH0108Hovered : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0108Hovered( Sint32 x, Sint32 y ,gxBool bForceStart =gxFalse);
	~CEneH0108Hovered();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetReverse()
	{
		//逆回転
		m_Base.z = 90*100;
		m_bReverse = gxTrue;
	}

private:

	gxBool AtackChicken();
	gxBool AtackNormal();

	void LogicAI();
	void LogicNormal();
	void LogicChicken();
	void Gimmick();

	Sint32 m_sAtribute;
	Sint32 m_sLegRot;
	Sint32 m_sKasokuX;
	Sint32 m_sKasokuY;
	Sint32 m_sHigh;		//ホバリングの高さ
	Sint32 m_sWait;		//ウェイト時間
	Sint32 m_sRot;
	Sint32 m_sBombTimer;
	Sint32 m_sDirection;

	Sint32 m_sAtackWait;
	Sint32 m_sAtackCnt;
	Sint32 m_sAtackLag;
	Sint32 m_sWaitTime;
	Sint32 m_sTimeUpCnt;

	Sint32 m_sChangeTime;

	ghPos m_Add;
	ghPos m_Base;

	CObj m_Obj;

	CHitCollision m_HitKurai;

	gxBool m_bForceStart;
	gxBool m_bDeadable;
	gxBool m_bChicken;
	gxBool m_bReverse;
	gxBool m_bDamage;
	gxBool m_bDisp;


};


