//--------------------------------------------------------------------------------
//
// Stage00:ミサイル
//
//--------------------------------------------------------------------------------
class CAtkH0001Missile : public CEnemyBase , public CHoundEnemyBase
{
public:
	enum {
		enForceID_Versis,
		enForceID_Enemy,
	};

	CAtkH0001Missile( Sint32 x, Sint32 y , Sint32 sRot , Sint32 sSpeed);
	~CAtkH0001Missile();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetWait(Sint32 n)
	{
		m_sWait = n;
	}

	void SetForce(Sint32 n)
	{
		//敵、味方設定
		m_sForce = n;
	}

	void SetHomingLevel(Sint32 n)
	{
		m_sLevel = n;
	}

	void SetEndless( gxBool bFlag = gxTrue )
	{
		m_bEndless = bFlag;
	}

private:

	void Atack();
	void LogicAI();
	gxBool getEnemy(Sint32 &px,Sint32 &py);
	gxBool getTargetEnemyPosition(Sint32 x,Sint32 y,Sint32 &px,Sint32 &py);

	CHitCollision m_HitKurai;
	CHitCollision m_HitAtari;

	OBJ_POS_T m_Add;
	OBJ_POS_T m_Kansei;

	Sint32 m_sAtribute;
	Sint32 m_sRotation;
	Sint32 m_sSmokeWide;
	Sint32 m_sMiseRotation;
	Sint32 m_sLevel;

	Sint32 m_sControl;
	Sint32 m_sSpeed;
	Sint32 m_sStartKaku;
	Sint32 m_sKidou;

	Sint32 m_sForce;
	Sint32 m_sWait;
	Sint32 m_sMoveWait;

	gxBool m_bEndless;

};


class CAtkH0001MslSmoke : public CEnemyBase , public CHoundEnemyBase
{
public:

	CAtkH0001MslSmoke( Sint32 x, Sint32 y );
	~CAtkH0001MslSmoke();

	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetDirection( Sint32 sRotation , Sint32 sSpeed );
	void SetClearSpeed(Sint32 n)
	{
		m_sAlphaSpeed = n;
	}
	void SetScale(Sint32 n)
	{
		m_fScale = n/100.f;
	}
private:

	void Atack();
	void LogicAI();

	OBJ_POS_T m_Add;
	Sint32 m_sSpeed;
	Sint32 m_sRotation;
	Sint32 m_sAlpha;
	Sint32 m_sMiseRotation;
	Sint32 m_sRotationAdd;
	Sint32 m_sAlphaSpeed;
	Float32 m_fScale;
};

//-----------------------------------------
//バズーカ弾
//-----------------------------------------
class CAtkH0001Bazooka : public CEnemyBase , public CHoundEnemyBase
{
public:
	enum {
		enForceID_Versis,
		enForceID_Enemy,
	};

	CAtkH0001Bazooka( Sint32 x, Sint32 y , Sint32 sRot , Sint32 sSpeed);
	~CAtkH0001Bazooka();

	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetNoneSmoke()
	{
		m_bSmoke = gxFalse;
	}
	void SetForce(Sint32 n)
	{
		//敵、味方設定
		m_sForce = n;
	}

private:

	void Atack();
	void LogicAI();

	CHitCollision m_HitAtari;

	OBJ_POS_T m_Add;

	Sint32 m_sAtribute;
	Sint32 m_sRotation;
	Sint32 m_sForce;
	Sint32 m_sSpeed;
	Sint32 m_sSmokeCnt;
	gxBool m_bSmoke;
};


//--------------------------------------------------------------------------------
//
// Stage00:スプラインミサイル
//
//--------------------------------------------------------------------------------

class CAtkH001SplineMissile : public CEnemyBase , public CHoundEnemyBase
{
public:
	enum {
		enForceID_Versis,
		enForceID_Enemy,
	};

	CAtkH001SplineMissile( Sint32 x, Sint32 y);
	~CAtkH001SplineMissile();

	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetForce(Sint32 n)
	{
		//敵、味方設定
		m_sForce = n;
	}

	void SetRout( Sint16* pTbl, Sint32 size )
	{
		m_pTbl = pTbl;
		m_sSize = size/2;
	}

	void SetLife( Sint32 sWait )
	{
		m_sWait = sWait;
	}

private:

	void LogicAI();

	CHitCollision m_HitKurai;
	CHitCollision m_HitAtari;

	Sint32 m_sAtribute;
	Sint32 m_sRotation;

	ghPos  m_Add;
	Sint16 *m_pTbl;
	Sint32 m_sSize;
	Sint32 m_sCurrent;

	Sint32 m_sForce;
	Sint32 m_sWait;

};


class CAtkH0001Ryudan : public CEnemyBase , public CHoundEnemyBase
{
public:
	enum {
		enForceID_Versis,
		enForceID_Enemy,
	};

	CAtkH0001Ryudan( Sint32 x, Sint32 y , Sint32 sRot , Sint32 sSpeed);
	~CAtkH0001Ryudan();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetMoveWait(Sint32 n )
	{
		m_sMoveWait = n;
	}
	void SetSpeedMax(Sint32 n )
	{
		m_sSpeed = n;
	}
	void SetForce(Sint32 n)
	{
		//敵、味方設定
		m_sForce = n;
	}

	void SetHomingLevel(Sint32 n)
	{
		m_sLevel = n;
	}

	void SetEndless( gxBool bFlag = gxTrue )
	{
		m_bEndless = bFlag;
	}

private:

	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;
	CHitCollision m_HitAtari;

	OBJ_POS_T m_Add;
	OBJ_POS_T m_Kansei;

	Sint32 m_sAtribute;
	Sint32 m_sRotation;
	Sint32 m_sSmokeWide;
	Sint32 m_sMiseRotation;
	Sint32 m_sLevel;

	Sint32 m_sControl;
	Sint32 m_sSpeed;
	Sint32 m_sStartKaku;
	Sint32 m_sKidou;

	Sint32 m_sForce;
	Sint32 m_sWait;
	Sint32 m_sMoveWait;

	gxBool m_bEndless;

};
//-------------------------------------------------
//電気アニメ
//-------------------------------------------------

class CAtkH0001Thunder
{
public:

	CAtkH0001Thunder(Sint32 x1, Sint32 y1 ,Sint32 x2,Sint32 y2);
	~CAtkH0001Thunder();

	void Set(Sint32 x1, Sint32 y1 ,Sint32 x2,Sint32 y2);
	void Action();
	void Draw();


private:

	gxBool m_bSet;
	OBJ_POS_T m_Pos;
	OBJ_POS_T m_Tgt;
	Sint32 m_sDist;
	Sint32 m_sCos;

	Sint32 m_sLoop;
	Float32 m_fRot;
	Float32 m_fTgtRot;
	Float32 m_fRotAdd;
	Float32 m_fScale;

	Sint32 m_sSyousyaCnt;
	Sint32 m_sTimer;
	Sint32 m_sAlpha;

};


class CAtkH0001Grenade : public CEnemyBase , public CHoundEnemyBase
{
public:
	enum {
		enForceID_Versis,
		enForceID_Enemy,
	};

	CAtkH0001Grenade( Sint32 x, Sint32 y , Sint32 sRot , Sint32 sSpeed);
	~CAtkH0001Grenade();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();

	void AddSpeed(Sint32 x , Sint32 y)
	{
		m_Add.x += x;
		m_Add.y += y;
	}
private:

	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;
	CHitCollision m_HitAtari;

	OBJ_POS_T m_Add;
	OBJ_POS_T m_Kansei;

	Sint32 m_sRotation;

	Sint32 m_sSpeed;
	Sint32 m_sBound;

	Sint32 m_sForce;
	Sint32 m_sWait;
	Sint32 m_sCrashSeq;

	gxBool m_bEndless;
	gxBool m_bDisp;

};
