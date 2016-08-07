//--------------------------------------------------------------------------------
//
//íeñãópÇÃíeÉNÉâÉX
//
//--------------------------------------------------------------------------------
class CEneBulletBase;
class CDanmakuBullet : public CAction , public CEneBulletBase
{
public:

	CDanmakuBullet(Sint32 sType , Sint32 x , Sint32 y);
	~CDanmakuBullet();

//	virtual void SeqInit();
//	virtual void SeqCrash();
	void SeqMain();
	virtual void Draw();

	ghPos m_Add;
	ghPos m_Kasoku;

	Sint32 m_sAddSpd;
	Float32 m_fRotSpd;
	Float32 m_fWeight;
	Float32 m_fWeightAdd;
	Float32 m_fRot;
	Sint32 m_sSpd;
	Sint32 m_sMaxSpd;
	Sint32 m_sWait;
	Sint32 m_sSprite;
	gxBool m_bScrollOut;

	Sint32 m_sBulKaku;

private:

	void PatternWaveShot();
	void PatternTyphoon();
	void PatternKasoku();
	void PatternNormal();
	void PatternFall();
	void PatternCandy();

	Sint32 m_sPattern;
	OBJ_POS_T m_Pos;

	CHitCollision m_HitAtari;
};

//--------------------------------------------
//íeñãê∂ê¨óp
//--------------------------------------------
class CDanmakuRoot : public CAction , public CEneBulletBase
{

public:
	enum {
		enDanmakuCandyShot,
		enDanmakuDropShot,
		enDanmakuAirCannon,
		enDanmakuSpiralGat,
		enDanmakuRolling,	//å¯â Ç»Çµ
		enDanmakuGattling,
		enDanmakuPileBankerShot,
		enDanmakuDonutsShot,
		enDanmakuTyphoonShot,
		enDanmakuWaveShot,
	};

	CDanmakuRoot( Sint32 sType , Sint32 x , Sint32 y , Sint32 sRot , Sint32 sSpd );
	~CDanmakuRoot()
	{

	}

//	virtual void SeqInit();
//	virtual void SeqCrash();
	void SeqMain();
	void Draw();

	Float32 m_fRotSpd;

private:
	void MakeCandyShot();
	void MakeDropShot();
	void MakeAirCannon();
	void MakeGattlingStraight();
	void MakeGattlingSpiral();
	void MakeWaveShot();
	void MakeTyphoonShot();
	void MakeDonutsShot();
	void MakePileBankerShot();

	Sint32 m_sPattern;

	ghPos m_Pos;
	Float32 m_fRot;
	Sint32 m_sSpd;
	Sint32 m_sTimer;
};

