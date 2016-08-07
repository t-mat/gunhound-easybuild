class CEffSandStorm : public CEffectBase
{
public:
	CEffSandStorm(Sint32 x,Sint32 y, Sint32 rot , Sint32 speed );
	~CEffSandStorm();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

private:

	Sint32 m_sAdd_X;
	Sint32 m_sAdd_Y;
	Sint32 m_sAlpha;
	Sint32 m_sScale;
	Sint32 m_sRotation;
};



class CEffGakeSmoke  : public CEffectBase
{
public:
	enum enSmokeType{
		enSmokeTypeSlash,
		enSmokeTypeWhite,
		enSmokeTypeBrown,
	};

	CEffGakeSmoke(enSmokeType eType , Sint32 x,Sint32 y, Sint32 rot = 0, Sint32 speed = 0);
	~CEffGakeSmoke();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

private:

	Sint32 m_sAdd_X;
	Sint32 m_sAdd_Y;
	Sint32 m_sAlpha;
	Sint32 m_sScale;
	Sint32 m_sRotation;
	enSmokeType m_enType;
};

class CEffDashStorm : public CEffectBase
{
public:
	CEffDashStorm(Sint32 x,Sint32 y, Sint32 rot , Sint32 speed );
	~CEffDashStorm();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

private:

	Sint32 m_sAdd_X;
	Sint32 m_sAdd_Y;
	Sint32 m_sAlpha;
	Sint32 m_sScale;
	Sint32 m_sRotation;
};


class CEffBreakStorm : public CEffectBase
{
public:
	CEffBreakStorm(Sint32 x,Sint32 y, Sint32 rot , Sint32 speed );
	~CEffBreakStorm();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

private:

	Sint32 m_sAdd_X;
	Sint32 m_sAdd_Y;
	Sint32 m_sAlpha;
	Sint32 m_sScale;
	Sint32 m_sRotation;
};


class CEffScore : public CEffectBase
{
public:
	CEffScore(Sint32 x,Sint32 y, Sint32 score );
	~CEffScore();

	void SeqMain();

	void Draw();

private:

	ghPos m_Add;
	Sint32 m_WaitTimer;
	Sint32 m_sScore;
};
