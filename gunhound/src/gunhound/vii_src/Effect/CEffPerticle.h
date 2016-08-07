class CEffPerticle : public CEffectBase
{
public:
	CEffPerticle(Sint32 x,Sint32 y, Sint32 rot , Sint32 speed );
	~CEffPerticle();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

private:

//	CHitCollision m_HitAtari;

	Sint32 m_sGravityAddY;
	Sint32 m_sAdd_X;
	Sint32 m_sAdd_Y;
	Sint32 m_sTimer;
	Sint32 m_sAddY;
};

class CEffExPerticle : public CEffectBase
{
public:
	CEffExPerticle( Sint32 x , Sint32 y , Sint32 prio );
	~CEffExPerticle();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

	void SetEnd();
	void SetNoneScroll()
	{
		m_bNoScroll = gxTrue;
	}
	void SetAlphaAdd(Sint32 n)
	{
		m_sAlphaAdd = n;
	}
	void SetSprite(gxSprite *pSpr);
	void SetAtribute(Sint32 Atribute);
	void SetDirection(Sint32 rot , Sint32 speed);
	void SetGravit(Sint32 g)
	{
		m_sGravit = g;
	}
	void SetTimer(Sint32 n)
	{
		m_sDeadTimer = n;
	}
	Sint32 m_sPrio;
	Sint32 m_sRotation;
	Sint32 m_sARGB;
	Sint32 m_sAtribute;
	Float32 m_fScale;
	Sint32 m_sAlphaAdd;
	
private:

	gxSprite *m_pSprite;

	Sint32 m_sGravit;
	Sint32 m_sAdd_X;
	Sint32 m_sAdd_Y;
	Sint32 m_sDeadTimer;
	gxBool m_bEndFlag;
	gxBool m_bNoScroll;
};

