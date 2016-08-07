class CEffShipBurnia : public CEffectBase
{
public:
	CEffShipBurnia(Sint32 x,Sint32 y, Sint32 prio = PRIO_EFFECT , Sint32 rot = 0 , Sint32 speed = 0);
	~CEffShipBurnia();

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

