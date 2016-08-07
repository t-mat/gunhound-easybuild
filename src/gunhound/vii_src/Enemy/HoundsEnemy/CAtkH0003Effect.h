//--------------------------------------------------------------------------------
//
// Stage00:エフェクト
//
//--------------------------------------------------------------------------------
class CAtkH0003EffectHitSmoke : public CEnemyBase , public CHoundEnemyBase
{
public:

	CAtkH0003EffectHitSmoke( Sint32 x, Sint32 y , Sint32 sRot , Sint32 sSpeed);
	~CAtkH0003EffectHitSmoke();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:

	void LogicAI();

	OBJ_POS_T m_Add;

	Sint32 m_sRotation;
	Sint32 m_sMiseRotation;
	Sint32 m_sWait;
	Sint32 m_sKidou;
};


class CAtkH0003EffectSmoke : public CEnemyBase , public CHoundEnemyBase
{
public:

	CAtkH0003EffectSmoke( Sint32 x, Sint32 y );
	~CAtkH0003EffectSmoke();

	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetDirection( Sint32 sRotation , Sint32 sSpeed );

private:

	void Atack();
	void LogicAI();

	OBJ_POS_T m_Add;
	Sint32 m_sSpeed;
	Sint32 m_sRotation;
	Sint32 m_sAlpha;
	Float32 m_fScale;
};


class CEffectBurnia : public CEnemyBase
{
public:
	CEffectBurnia(Sint32 x , Sint32 y , Sint32 sRot ,Sint32 sSpeed );
	~CEffectBurnia();

	void SetParent( OBJ_POS_T *pPos , Sint32 *pDirection)
	{
		m_pPos = pPos;
		m_pDirection = pDirection;
	}
	void SeqMain();
	void SeqCrash();
	void Draw();

private:

	void LogicAI();

	OBJ_POS_T m_Add;
	OBJ_POS_T *m_pPos;
	Sint32* m_pDirection;
	Sint32 m_sSpeed;
	Sint32 m_sRotation;
	Sint32 m_sAlpha;
	Float32 m_fScale;

};


//--------------------------------------------------------------------------------
//
// ダメージパーツ
//　コマンチがやられたときなど、破片が煙をまとって放射状に飛ぶ
//--------------------------------------------------------------------------------

class CEffectDmgprt : public CEnemyBase
{
public:

	CEffectDmgprt(Sint32 x , Sint32 y , Sint32 sRot ,Sint32 sSpeed );
	~CEffectDmgprt();

	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetSprite( gxSprite *p)
	{
		m_pSprite = p;
	}
	void SetHeavy( )
	{
		m_bHeavy  =gxTrue;
	}
	void SetRotation( Sint32 n)
	{
		m_sRotation = n;
	}
	void SetAtribute( Sint32 n)
	{
		m_sAtribute = n;
	}

	void SetSmoke( gxBool bOn )
	{
		m_bSmokeDisp = bOn;
	}

private:

	void LogicAI();

	OBJ_POS_T m_Add;

	Sint32 m_sAtribute;
	Sint32 m_sBoundCnt;
	Sint32 m_sSprite;
	Sint32 m_sRotation;
	Sint32 m_sScale;
	gxSprite *m_pSprite;
	gxBool m_bSmokeDisp;
	gxBool m_bHeavy;
};

class CEffectDosekiryu : public CEnemyBase
{
public:

	CEffectDosekiryu(Sint32 x , Sint32 y , Sint32 sRot ,Sint32 sSpeed ,Sint32 sReverse);
	~CEffectDosekiryu();

	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetSprite( gxSprite *p)
	{
		m_pSprite = p;
	}
	void SetHeavy( )
	{
		m_bHeavy  =gxTrue;
	}
	void SetRotation( Sint32 n)
	{
		m_sRotation = n;
	}
	void SetAtribute( Sint32 n)
	{
		m_sAtribute = n;
	}
private:

	void LogicAI();

	OBJ_POS_T m_Add;

	Sint32 m_sAtribute;
	Sint32 m_sBoundCnt;
	Sint32 m_sReverse;
	Sint32 m_sSprite;
	Sint32 m_sRotation;
	Sint32 m_sScale;
	gxSprite *m_pSprite;
	gxBool m_bHeavy;
};


//--------------------------------------------------------------------------------
//
// 超大爆発エフェクト
//
//--------------------------------------------------------------------------------
class CEffectFinalExplosion : public CEnemyBase
{
public:

	CEffectFinalExplosion(Sint32 x , Sint32 y ,Sint32 sScale=0);
	~CEffectFinalExplosion();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:

	void LogicAI();

	OBJ_POS_T m_Add;

	Sint32  m_sRotation;
	Float32 m_fAddScale;
	Float32 m_fScale;
	Sint32 m_sSeq;
	Sint32 m_sWait;

	Float32 m_fOrangeCnt;
	Sint32 m_sOrangeCol;

	gxBool m_bFlair;
	Float32 m_fFlairScale;
	Float32  m_fFlairCol;
	Sint32  m_sFlairRot;

	Float32  m_fFadeCol;
};

//--------------------------------------------------------------------------------
//
// 黒爆炎エフェクト
//
//--------------------------------------------------------------------------------


class CEffectBlackExplosion : public CEnemyBase
{
public:

	CEffectBlackExplosion(Sint32 x , Sint32 y , ghPos* pPos = NULL);
	~CEffectBlackExplosion();
	void SetLayerPrio(Sint32 n)
	{
		m_Pos.z = n;
	}
private:

	void SeqMain();
	void SeqCrash();
	void Draw();

	ghPos *m_pPos;

};

class CEffectBlackFire : public CEnemyBase
{
public:

	CEffectBlackFire(Sint32 x , Sint32 y , ghPos* pPos = NULL);
	~CEffectBlackFire();

private:

	void SeqMain();
	void SeqCrash();
	void Draw();

	Sint32 m_sAlpha;
	ghPos m_Add;
	ghPos *m_pPos;
};


class CEffectGroundSmoke : public CEnemyBase
{
public:

	CEffectGroundSmoke(Sint32 x , Sint32 y,Sint32 sDir );
	~CEffectGroundSmoke();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetScale(Float32 fScale)
	{
		m_fScale = fScale;
	}

	void SetAlpha(Sint32 sAlpha)
	{
		m_sAlpha = sAlpha;
	}

private:

	Sint32 m_sDirection;
	Sint32 m_sAlpha;
	Sint32 m_sAtribute;
	Sint32 m_sRotation;
	Float32 m_fScale;

	ghPos m_Add;
	gxBool m_bDisp;
};

class CEffectBubble : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEffectBubble( Sint32 x, Sint32 y ,Sint32 sRot , Sint32 sSpd);
	~CEffectBubble();

	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetScale( Float32 fScl )
	{
		m_fScale *= fScl;
	}

	void SetTarget(OBJ_POS_T *pTgt)
	{
		m_pTgt = pTgt;
	}

	void SetWide( Sint32 sRot , Sint32 sSpd )
	{
		m_sSpeed    = sSpd;
		m_sRotation = sRot;
	}


private:

	void Atack();
	void LogicAI();

	OBJ_POS_T *m_pTgt;
	ghPos  m_Add;
	ghPos  m_Float;
	Sint32 m_sSpeed;
	Sint32 m_sRotation;
	Sint32 m_sAlpha;
	Float32 m_fScale;

};


//--------------------------------------------------------------------------------
//
// 超大爆発エフェクト
//
//--------------------------------------------------------------------------------
class CEffectFinalExplosionDeluxe : public CEnemyBase, public CHoundEnemyBase
{
public:

	CEffectFinalExplosionDeluxe(Sint32 x , Sint32 y);
	~CEffectFinalExplosionDeluxe();

	void SeqMain();
	void SeqCrash();
	void Draw();
	gxBool IsFinish();

private:
	gxBool m_bFinish;
};


//--------------------------------------------------------------------------------
//
// バーニアエフェクト
//
//--------------------------------------------------------------------------------
class CEffectBossFire : public CEnemyBase
{
public:

	CEffectBossFire(Sint32 x , Sint32 y,Sint32 sRot ,Sint32 sSpeed );
	~CEffectBossFire();

	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetScale(Float32 fScale)
	{
		m_fScale = fScale;
	}

	void SetAlpha(Sint32 sAlpha)
	{
		m_sAlpha = sAlpha;
	}

private:

	Sint32 m_sDirection;
	Sint32 m_sAlpha;
	Sint32 m_sAtribute;
	Sint32 m_sRotation;
	Float32 m_fScale;

	ghPos m_Add;
	gxBool m_bDisp;
};

