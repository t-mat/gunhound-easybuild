class CEffFire : public CEffectBase
{
public:
	CEffFire( Sint32 x,Sint32 y, Sint32 prio = PRIO_EFFECT ,Sint32 sAutoCleanWait = -1);
	~CEffFire();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

	void SetScale(Float32 fScale)
	{
		m_sScale = (Sint32)( 100*fScale );
	}

private:
	Sint32 m_sAlpha;
	Sint32 m_sScale;
	Sint32 m_sSprite;
	Sint32 m_sOffsetX;
	Sint32 m_sAutoCleanWait;
};

class CEffFireParts : public CEffectBase
{
public:
	CEffFireParts( Sint32 x,Sint32 y, Sint32 prio = PRIO_EFFECT );
	~CEffFireParts();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();
	void SetScale(Float32 fScale)
	{
		m_sScale = (Sint32)( 100*fScale );
	}

	void Draw();

private:

	Sint32 m_sPrio;
	Sint32 m_sAdd_X;
	Sint32 m_sAdd_Y;
	Sint32 m_sAlpha;
	Sint32 m_sScale;
	Sint32 m_sRotation;
};

class CEffFireSmoke : public CEffectBase
{
public:
	CEffFireSmoke( Sint32 x,Sint32 y, Sint32 prio = PRIO_EFFECT );
	~CEffFireSmoke();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

private:

	Sint32 m_sPrio;
	Sint32 m_sAdd_X;
	Sint32 m_sAdd_Y;
	Sint32 m_sAlpha;
	Sint32 m_sScale;
	Sint32 m_sRotation;
};

