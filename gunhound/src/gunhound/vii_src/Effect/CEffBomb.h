class CEffBomb : public CEffectBase
{
public:
	CEffBomb( Sint32 x,Sint32 y , Sint32 rot , Sint32 Speed );
	~CEffBomb();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

private:

	Sint32 m_sRotation;
	Sint32 m_sColor;
	Sint32 m_sSpeed;
	Sint32 m_sScale;
	ghPos  m_Add;
	Sint32 m_sWait;
	Sint32 m_sType;

};

class CEffBigBomb : public CEffectBase
{
public:
	CEffBigBomb( Sint32 x,Sint32 y , Sint32 rot , Sint32 Speed );
	~CEffBigBomb();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();
	void SetYubaku(gxBool bFlag)
	{
		m_bYubaku = bFlag;
	}

private:

	OBJ_POS_T m_Add;

	CHitCollision m_HitAtari;

	Sint32  m_sRotation;
	Float32 m_fAddScale;
	Float32 m_fScale;
	Sint32 m_sSeq;
	Sint32 m_sWait;

	Float32 m_fOrangeCnt;
	Sint32 m_sOrangeCol;

	Float32 m_fFlairScale;
	Float32  m_fFlairCol;
	Sint32  m_sFlairRot;

	Float32  m_fFadeCol;

	gxBool m_bFlair;
	gxBool m_bYubaku;
};


class CEffBombSmoke : public CEffectBase
{
public:
	CEffBombSmoke( Sint32 x,Sint32 y , Sint32 rot , Sint32 Speed );
	~CEffBombSmoke();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

	void SetTgt(ghPos *pPos)
	{
		m_pTgt = pPos;
	}
	void SetType(Sint32 n)
	{
		m_sType = n;
	}
	void RotFix()
	{
		m_bRotFix = gxTrue;
	}

private:

	Sint32 m_sRotation;
	Sint32 m_sColor;
	Sint32 m_sSpeed;
	Sint32 m_sScale;
	ghPos  m_Add;
	Sint32 m_sWait;
	Sint32 m_sType;
	gxBool m_bRotFix;
	ghPos *m_pTgt;

};


class CEffBombSmall : public CEffectBase
{
public:
	CEffBombSmall( Sint32 x,Sint32 y , Sint32 rot , Sint32 Speed );
	~CEffBombSmall();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

private:

	Sint32 m_sRotation;
	Sint32 m_sColor;
	Sint32 m_sSpeed;
	Sint32 m_sScale;

	ghPos  m_Add;
	Sint32 m_Spd;
	Sint32 m_sWait;
	Sint32 m_sType;
};

class CEffSun : public CEffectBase
{
public:
	CEffSun( Sint32 x,Sint32 y,ghPos *pPos=NULL);
	~CEffSun();

	void SeqMain();
	void Draw();

private:

	Sint32 m_sRotation;
	Sint32 m_sColor;
	Sint32 m_sScale;
	ghPos  m_Add;
	ghPos *m_pPos;

};


void MakeEffectSmallBombs(Sint32 x ,Sint32 y, Sint32 num );
void EffectMakeBomb( Sint32 x, Sint32 y, Sint32 num , Sint32 spd );
void EffectMakeBigBomb( Sint32 x, Sint32 y ,Sint32 z=PRIO_EFFECT);
void EffectSun( Sint32 x, Sint32 y ,ghPos *pTgtPos= NULL);
