enum {
	enMekaMax = 5,
	enLaserMax = 10,
};


class CEffScene01 : public CEffectBase
{
public:
	CEffScene01(Sint32 x,Sint32 y ,Sint32 z );
	~CEffScene01();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

private:

	Sint32 m_sPrio;
	Sint32 m_LaserWidth[enLaserMax];
	Sint32 m_sEnsyutuTimer;
	CEffExPerticle *m_pMeka[enMekaMax];

	void MikataShot();
	void MekaShot();
	void LaserBeam( Sint32 x, Sint32 y , Sint32 sScale );

	

};

