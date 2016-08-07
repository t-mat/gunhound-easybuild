class CEffAtkLaser : public CEffectBase
{
public:
	typedef struct StLaserLayer
	{
		Sint32 x,y;
		Float32 fy;
	}StLaserLayer;

	CEffAtkLaser( Sint32 x , Sint32 y , Sint32 lv );
	~CEffAtkLaser();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

	void SetEnemyAtack()
	{
		m_sAtackType = ID_ENEMY_ATK;
	}
	void SetLaser(Sint32 x, Sint32 y, Sint32 rot)
	{
		//レーザーポジションを設定する

		m_Pos.x = x;
		m_Pos.y = y;
		m_sRotation = rot;
	}

	void SetLaserSpeed(Sint32 n)
	{
		m_sSpeed = n;
	}

	void EnergyIn()
	{
		m_bPowerIn = gxTrue;
	}
	void SetLaserLevel( Sint32 n )
	{
		m_sLevel = n;
	}

	gxBool IsAtack()
	{
		if( m_sLength <= 0) return gxFalse; else return gxTrue;
	}

private:
	void SetLaserEffect();

	Sint32 GetLaserColor();
	CHitCollision m_HitAtari[32];

	Sint32 m_sAdd_X;
	Sint32 m_sAdd_Y;
	Sint32 m_sLevel;
	Sint32 m_sLength;
	Sint32 m_sRotation;

	gxBool   m_bPowerIn;
	gxBool   m_bDisp;
	Sint32 m_sSpeed;
	Sint32 m_sAtackType;
	Sint32 m_sLaserX;
	Sint32 m_sLaserY;
	Float32 m_fLength;

	StLaserLayer stLayer[3];
	gxSprite* m_pSprite;

};

