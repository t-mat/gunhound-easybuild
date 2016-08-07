class CEffEneGun : public CEffectBase
{
public:
	CEffEneGun(Sint32 x,Sint32 y, Sint32 rot , Sint32 Speed );
	~CEffEneGun();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

private:

	CHitCollision m_HitAtari;

	Sint32 m_sAdd_X;
	Sint32 m_sAdd_Y;

};

class CEffEneTwinGun : public CEffectBase
{
public:
	CEffEneTwinGun(Sint32 x,Sint32 y, Sint32 rot , Sint32 Speed );
	~CEffEneTwinGun();

	void SetAtariOff()
	{
		m_bAtariOff = gxTrue;
	}
	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

private:

	CHitCollision m_HitAtari;

	Sint32 m_sRotation;
	Sint32 m_sAdd_X;
	Sint32 m_sAdd_Y;
	gxBool m_bAtariOff;

};

