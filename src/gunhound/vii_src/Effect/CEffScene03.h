class CEffScene03 : public CEffectBase
{
public:

	CEffScene03(Sint32 x,Sint32 y ,Sint32 z );
	~CEffScene03();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

	void SetBombStart();

private:

	Sint32 m_sPrio;
	Sint32 m_sBombSeq;
	Sint32 m_sEnsyutuTimer;
	gxBool m_bBombCloudDisp;

	CEffExPerticle *m_pMarsGear;	//パーティクルエフェクト
	Sint32 m_sColor;
	Sint32 m_sColorAdd;
};

