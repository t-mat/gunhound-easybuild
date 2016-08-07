enum enTypeSplash
{
	enTypeSplashPillar,
	enTypeSplashLarge,
	enTypeSplashSmall,
	enTypeSplashWave,
};


class CEffWaterPillar;

class CEffWater : public CEffectBase
{
public:
	CEffWater(Sint32 x,Sint32 y , enTypeSplash _eTypeSplash , Sint32 lv , Sint32 rot = 0 , Sint32 speed = 0);
	~CEffWater();

	void SeqInit();
	void SeqMain();
	void SeqCrash();

	void Draw();

private:

	OBJ_POS_T m_Add;
	enTypeSplash m_enTypeSplash;

	Sint32 m_sSprite;
	Sint32 m_sSeq;
	Sint32 m_sAlpha;
	Sint32 m_sScale;
	Sint32 m_sRotation;
};

class CEffWaterPillar : public CEffectBase
{
public:
	CEffWaterPillar( Sint32 x,Sint32 y ,Sint32 z);
	~CEffWaterPillar();

	void SeqMain();
	void Draw();
	void SetScale(Float32 fScale)
	{
		m_fScale = fScale;
	}

private:

	OBJ_POS_T m_Add;

	Sint32 m_sWait;
	Sint32 m_sSprite;
	Sint32 m_sSeq;
	Sint32 m_sAlpha;
	Float32 m_fScale;
	Sint32 m_sRotation;
};

class CEffWaterWave : public CEffectBase
{
public:
	CEffWaterWave( Sint32 x,Sint32 y ,Sint32 z , Sint32 dir);
	~CEffWaterWave();

	void SeqMain();
	void Draw();

private:

	OBJ_POS_T m_Add;

	Sint32 m_sDir;
	Sint32 m_sAtribute;
	Sint32 m_sWait;
	Sint32 m_sSprite;
	Sint32 m_sAlpha;
	Float32 m_fScale;
	Sint32 m_sRotation;
};

class CEffWaterSplash : public CEffectBase
{
public:
	CEffWaterSplash( Sint32 x,Sint32 y ,Sint32 z , Sint32 rot , Sint32 spd );
	~CEffWaterSplash();

	void SeqMain();
	void Draw();

private:

	OBJ_POS_T m_Add;

	Sint32 m_sAtribute;
	Sint32 m_sAlpha;
	Sint32 m_sRotation;
};

//--------------------------------------------
//Å@êÏñ ÇÃêÖó¨
//--------------------------------------------
class CEffWaterStorm : public CEffectBase
{
public:
	CEffWaterStorm(Sint32 x,Sint32 y, Sint32 rot , Sint32 speed );
	~CEffWaterStorm();

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


enum{
	enSparkTypeWhiteWave,
	enSparkTypePiller,
	enSparkTypeFog,
};
class CEffWaterSpark : public CEffectBase
{
public:
	CEffWaterSpark( Sint32 x,Sint32 y ,Sint32 rot , Sint32 spd ,Sint32 prio=PRIO_ENEMY);
	~CEffWaterSpark();

	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetType(Sint32 n)
	{
		m_sType = n;
	}
	void SetPrio(Sint32 n)
	{
		m_Pos.z = n;
	}
private:

	OBJ_POS_T m_Add;

	Sint32  m_sAlpha;
	Float32 m_fScale;
	Sint32  m_sRotation;
	Sint32  m_sType;
	Sint32  m_sWait;
};


void MakeSplash( Sint32 x , Sint32 y , Sint32 sPrio=PRIO_ENEMY );
