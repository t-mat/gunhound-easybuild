//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneH0201VorzoyFly : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0201VorzoyFly( Sint32 x, Sint32 y );
	~CEneH0201VorzoyFly();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

	void CarryOn( CEasyLeynos* pEasyLeynos )
	{
		//ボルゾイをアサインする
		m_pEasyLeynos = pEasyLeynos;
		m_pEasyLeynos->CarryOn();
	}
	void CarryOff( );
	void SetTarget(Sint32 *pPosx ,Sint32 sOffset)
	{
		m_pTargetPos = pPosx;
		m_sOffset    = sOffset;
	}

private:


	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;
	CEnemyAnime *m_pEnemyAnime;

	Sint32 m_sControl;

	Sint32 m_sLogicSeq;
	Sint32 m_sLogicWait;
	Sint32 m_sAtribute;
	Sint32 m_sPropTimer;
	Sint32* m_pTargetPos;
	Sint32 m_sOffset;

	CEasyLeynos *m_pEasyLeynos;
	CPenduram* m_pPenduram;

	ghPos m_Add;
	ghPos m_Tgt;

	gxBool m_bDamage;
	gxBool m_bTettai;
};

