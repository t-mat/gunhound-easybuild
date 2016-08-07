//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneBulletBase
{
public:

	CEneBulletBase()
	{
		m_sBulletIndex = -1;
	}

	~CEneBulletBase()
	{

	}
	gxBool IsClearBullet();
	void SetBulletIndex(Sint32 n)
	{
		m_sBulletIndex = n;
	}

protected:

	Sint32 m_sBulletIndex;

private:

};

//直線弾丸

class CEne0001EneBullet : public CEnemyBase , public CEneBulletBase
{
public:

	CEne0001EneBullet(  Sint32 x, Sint32 y ,Sint32 r ,Sint32 spd ,Sint32 sSpr=2);
	~CEne0001EneBullet();

	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetSprite( gxSprite *p )
	{
		m_pSprite = p;
	}

	void SetDrop();

private:

	void Atack();
	void LogicAI();

	ghPos m_Drop;
	gxBool m_bDrop;
	OBJ_POS_T m_Add;
	CHitCollision m_HitAtari;
	Sint32 m_sRotation;
	Sint32 m_sType;
	gxSprite* m_pSprite;
};


//放物線ミサイル

class CEne0001EneMissile : public CEnemyBase
{
public:

	CEne0001EneMissile(  Sint32 x, Sint32 y ,Sint32 r ,Sint32 spd );
	~CEne0001EneMissile();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:

//	CEffExPerticle *m_pEff;

	void Atack();
	void LogicAI();

	OBJ_POS_T m_Add;
	CHitCollision m_HitAtari;
	CHitCollision m_HitKurai;
	Sint32 m_sRotation;

	Sint32 m_sSpeed;
	Sint32 m_sSpeedAdd;
	Sint32 m_sGravitAdd;
};

//放物線ボム

class CEne0001EneBomb : public CEnemyBase
{
public:

	CEne0001EneBomb(  Sint32 x, Sint32 y ,Sint32 r ,Sint32 spd );
	~CEne0001EneBomb();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:

//	CEffExPerticle *m_pEff;

	void Atack();
	void LogicAI();

	CHitCollision m_HitAtari;

	OBJ_POS_T m_Add;
	Sint32 m_sRotation;

};

//榴弾

class CEne0001Ryudan : public CEnemyBase
{
public:

	CEne0001Ryudan(  Sint32 x, Sint32 y ,Sint32 r ,Sint32 spd );
	~CEne0001Ryudan();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:

//	CEffExPerticle *m_pEff;

	void Atack();
	void LogicAI();

	CHitCollision m_HitAtari;
	CHitCollision m_HitKurai;

	OBJ_POS_T m_Add;
	Sint32 m_sRotation;

};

//直線砲弾

class CEne0001EneCannon : public CEnemyBase
{
public:

	CEne0001EneCannon(  Sint32 x, Sint32 y ,Sint32 r ,Sint32 spd );
	~CEne0001EneCannon();

	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetDrop()
	{
		m_bDrop = gxTrue;
	}

	void SetSound(gxBool bFlag)
	{
		m_bSound = bFlag;
	}

	void SetScale(Float32 fScale)
	{
		m_fScale = fScale;
	}

private:

	void Atack();
	void LogicAI();

	OBJ_POS_T m_Add;
	CHitCollision m_HitAtari;
	Sint32 m_sRotation;
	Sint32 m_sDispRotation;
	Sint32 m_sSpeed;
	Float32 m_fScale;
	gxBool m_bDrop;
	gxBool m_bSound;
};


//直線弾丸（破壊可能）

class CEne0001EneCrasher : public CEnemyBase
{
public:

	CEne0001EneCrasher(  Sint32 x, Sint32 y ,Sint32 r ,Sint32 spd );
	~CEne0001EneCrasher();

	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetDelay( Sint32 minSpd , Sint32 sFrm );

private:


	void Atack();
	void LogicAI();

	OBJ_POS_T m_Add;
	CHitCollision m_HitAtari;
	CHitCollision m_HitKurai;
	Sint32 m_sDirection;
	Sint32 m_sRotation;
	Sint32 m_sSpeed;
	Sint32 m_sSpeedAdd;
	Sint32 m_sDelayTime;
};

//炸裂弾

class CEne0001EneSakretu : public CEnemyBase , public CEneBulletBase
{
public:

	CEne0001EneSakretu(  Sint32 x, Sint32 y ,Sint32 r ,Sint32 spd );
	~CEne0001EneSakretu();

	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetSprite( gxSprite *p )
	{
		m_pSprite = p;
	}

private:

	void Atack();
	void LogicAI();

	OBJ_POS_T m_Add;
	CHitCollision m_HitAtari;
	Sint32 m_sRotation;
	Sint32 m_sSpeed;
	Sint32 m_sBombTimer;
	gxSprite* m_pSprite;
};


class CEne0001PlasmaBall : public CEnemyBase
{
public:

	CEne0001PlasmaBall(  Sint32 x, Sint32 y);
	~CEne0001PlasmaBall();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:

	void Atack();
	void LogicAI();

	ghPos m_Add;
	Sint32 m_sSpeed;
	Sint32 m_sRotation;
	CHitCollision m_HitAtari;

};


//--------------------------------------------------------------------------------
//
// Stage00:スーパーボール
//
//--------------------------------------------------------------------------------
class CEne0001EneSuperBall : public CEnemyBase , public CEneBulletBase
{
public:
	enum {
		enPatternDefault,
		enPatternTargetting,
		enPatternDrop,
		enPatternHoming,
		enPatternMicroMissile,
	};
	enum {
		enOptionMax = 128,
	};

	CEne0001EneSuperBall(  Sint32 x, Sint32 y ,Sint32 r=0 ,Sint32 spd=0 );
	~CEne0001EneSuperBall();

	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetLevel( Sint32 sLevel )
	{
		m_sLevel = sLevel;
	}

	void SetSprite( gxSprite *p )
	{
		m_pSprite = p;
	}

	void SetDrop()
	{
		m_bDrop = gxTrue;
	}

	void SetPattern(Sint32 n)
	{
		m_sPattern = n;
	}
	void SetEndless( )
	{
		m_bEndless = gxTrue;
	}
	void SetLeft4Dead()
	{
		m_bLeft4Dead = gxTrue;
	}
	void SetMaxSpeed( Sint32 sMax )
	{
		m_sMaxSpeed = sMax;
	}

	void SetSpeed( Sint32 sMax )
	{
		m_sSpeed = sMax;
	}
	
	void SetScrollOutRange(Sint32 n)
	{
		m_sScrollOutAdd = n;
	}

	void SetScale( Float32 fScale )
	{
		m_fScale = fScale;
	}

	void SetDropGensui( gxBool bGensui , Sint32 sDropMax = 200 )
	{
		m_sDropMax = sDropMax;
		m_bGensui = bGensui;
	}

private:

	void Atack();
	void LogicAI();

	//-----------------------------
	void LogicDrop();
	void LogicHoming();
	void LogicTargetting();
	void LogicMicroMissile();

	gxBool m_bDrop;
	gxBool m_bHoming;

	ghPos m_Drop;	//落下速度制御
	ghPos m_Add;	//移動増分地制御
	ghPos m_Tgt;	//ターゲット位置記録

	ghPos m_StOption[enOptionMax];	//残像表示用
	Sint32 m_sOptionCnt;

	Float32 m_fScale;
	Sint32 m_sLevel;
	Sint32 m_sWait;
	Sint32 m_sSpeed;
	Sint32 m_sRotation;
	Sint32 m_sPattern;
	Sint32 m_sMaxSpeed;
	Sint32 m_sScrollOutAdd;
	Sint32 m_sDropMax;
	CHitCollision m_HitAtari;
	gxSprite* m_pSprite;
	gxBool m_bEndless;
	gxBool m_bLeft4Dead;
	gxBool m_bGensui;
	gxBool m_bDummy;
};


//-------------------------------------------------
//
//ハイパーボール
//
//-------------------------------------------------
class CEne0001EneHyperBall : public CEnemyBase
{
public:
	enum {
		enPatternDefault,
		enPatternTargetting,
		enPatternDrop,
		enPatternHoming,
		enPatternMicroMissile,
	};
	enum {
		enOptionMax = 128,
	};

	CEne0001EneHyperBall(  Sint32 x, Sint32 y ,Sint32 r=0 ,Sint32 spd=0 );
	~CEne0001EneHyperBall();

	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetSprite( gxSprite *p )
	{
		m_pSprite = p;
	}

	void SetDrop()
	{
		m_bDrop = gxTrue;
	}

	void SetPattern(Sint32 n)
	{
		m_sPattern = n;
	}
	void SetEndless( )
	{
		m_bEndless = gxTrue;
	}

	void SetMaxSpeed( Sint32 sMax )
	{
		m_sMaxSpeed = sMax;
	}
	void SetScrollOutRange(Sint32 n)
	{
		m_sScrollOutAdd = n;
	}

private:

	void Atack();
	void LogicAI();
	void LogicHoming();
	void LogicTargetting();

	Sint32 m_sChakutiCnt;

	gxBool m_bDrop;
	gxBool m_bHoming;
	gxBool m_bChakuti;
	gxBool m_bEndless;
	gxBool m_bFlash;

	ghPos m_Drop;	//落下速度制御
	ghPos m_Add;	//移動増分地制御
	ghPos m_Tgt;	//ターゲット位置記録

	ghPos m_StOption[enOptionMax];	//残像表示用
	Sint32 m_sOptionCnt;

	Sint32 m_sWait;
	Sint32 m_sSpeed;
	Sint32 m_sRotation;
	Sint32 m_sPattern;
	Sint32 m_sMaxSpeed;
	Sint32 m_sScrollOutAdd;
	CHitCollision m_HitAtari;
	gxSprite* m_pSprite;
};


