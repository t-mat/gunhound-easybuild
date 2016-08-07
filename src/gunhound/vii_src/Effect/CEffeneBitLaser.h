class CEffeneBitLaser : public CEffectBase
{
public:
	CEffeneBitLaser(Sint32 x , Sint32 y, Sint32 rot ,Sint32 spd ,gxBool bBVflag=gxFalse );

	~CEffeneBitLaser();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

private:

	CHitCollision m_HitAtari;
	CHitCollision m_HitAtariMe;

	Sint32 m_sAdd_X;
	Sint32 m_sAdd_Y;
	Sint32 m_sRotation;
	gxBool m_bBuildVorg;

};

