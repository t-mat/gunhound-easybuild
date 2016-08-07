//--------------------------------------------------------------------------------
//
// Stage00:ミサイル
//
//--------------------------------------------------------------------------------
class CAtkH0002GMissile : public CEnemyBase , public CHoundEnemyBase
{
public:
	enum {
		enForceID_Versis,
		enForceID_Enemy,
	};

	CAtkH0002GMissile( Sint32 x, Sint32 y , Sint32 sRot , Sint32 sSpeed);
	~CAtkH0002GMissile();

	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetEndless(Sint32 n=60)
	{
		m_sEndlessCnt = n;
		m_bEndless = gxTrue;
	}
	void SetForce(Sint32 n)
	{
		//敵、味方設定
		m_sForce = n;
	}

	void SetRotationFix(Sint32 sRot)
	{
		m_sFixRotation = sRot;
		m_bRotFix = gxTrue;
	}

private:

	void LogicAI();

	CHitCollision m_HitKurai;
	CHitCollision m_HitAtari;

	OBJ_POS_T m_Add;
	CObj m_stObj;

	Sint32 m_sAtribute;
	Sint32 m_sMiseRotation;

	Sint32 m_sTimeLimit;
	Sint32 m_sForce;
	Sint32 m_sWait;
	Sint32 m_sDir;
	Sint32 m_sEndlessCnt;
	Sint32 m_sFixRotation;

	gxBool m_bEndless;
	gxBool m_bRotFix;
};


class CAtkH0002GMslWave : public CEnemyBase , public CHoundEnemyBase
{
public:

	CAtkH0002GMslWave( Sint32 x, Sint32 y );
	~CAtkH0002GMslWave();

	void SeqMain();
	void SeqCrash();
	void Draw();

	void SetDirection( Sint32 sRotation , Sint32 sSpeed );

private:

	void Atack();
	void LogicAI();

	OBJ_POS_T m_Add;
	Sint32 m_sDir;
	Sint32 m_sSpeed;
	Sint32 m_sRotation;
	Sint32 m_sAlpha;
	Float32 m_fScale;
};

