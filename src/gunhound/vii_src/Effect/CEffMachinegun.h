class CEffMachinegun : public CEffectBase
{
public:
	CEffMachinegun(Sint32 x,Sint32 y, Sint32 rot , Sint32 lv , Sint32 reboundCnt );
	~CEffMachinegun();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

private:
	gxBool Reflect();

	CHitCollision m_HitAtari;

	Sint32 m_sAdd_X;
	Sint32 m_sAdd_Y;
	Sint32 m_sLevel;
	Sint32 m_sCount;

};


class CEffGattlingGun : public CEffectBase
{
public:
	CEffGattlingGun(Sint32 x,Sint32 y, Sint32 sRot , Sint32 sSpd );
	~CEffGattlingGun();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();
	void SetAtackPower(Sint32 sAtackPower)
	{
		m_sAtackPower = sAtackPower;
	}
	void SetKantsu(gxBool bFlag)
	{
		m_bKantsu = bFlag;
	}
private:

	CHitCollision m_HitAtari;
	Sint32 m_sRotation;
	Sint32 m_sAtackPower;
	OBJ_POS_T m_Add;
	gxBool m_bKantsu;

};

class CEffNapalmGun : public CEffectBase
{
public:
	enum {
		enOldMax = 8,
	};
	CEffNapalmGun(Sint32 x,Sint32 y, Sint32 sRot , Sint32 sSpd );
	~CEffNapalmGun();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();
	void SetAtackPower(Sint32 sAtackPower)
	{
		m_sAtackPower = sAtackPower;
	}

private:

	CHitCollision m_HitAtari;
	Sint32 m_sRotation;
	Sint32 m_sAtackPower;
	Sint32 m_sAlpha;
	OBJ_POS_T m_Add;
	Sint32 m_sOldCnt;
	Sint32 m_sSpeed;
	Float32 m_fScale;

	ghPos m_Old[enOldMax];
};

class CEffShotGun : public CEffectBase
{
public:
	CEffShotGun(Sint32 x,Sint32 y, Sint32 sRot , Sint32 sSpd );
	~CEffShotGun();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();
	void SetAtackPower(Sint32 sAtackPower)
	{
		m_sAtackPower = sAtackPower;
	}
	void SetKantsu(gxBool bFlag)
	{
		m_bKantsu = bFlag;
	}
private:

	CHitCollision m_HitAtari;
	CHitCollision m_HitKurai;
	Sint32 m_sRotation;
	Sint32 m_sAtackPower;
	OBJ_POS_T m_Add;
	gxBool m_bKantsu;

};

