//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------
class CEneCmnSpark;
class CEneH0410Wall : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0410Wall( Sint32 x, Sint32 y );
	~CEneH0410Wall();

	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetCrash(Sint32 n);
	void SetCover();

	gxBool IsCoverClosed()
	{
		//カバー閉じた？
		return m_bCoverClosed;
	}

	gxBool IsMaximum()
	{
		//内壁閉じた？
		return m_bMaximum;
	}

	void ForceCrash()
	{
		//強制的にクラッシュせよ
		m_bForceCrash = gxTrue;
		SetCrash(0);
	}

private:
	enum {
		enWallLength = 6,
	};

	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;
	CHitCollision m_HitAtari;

	CEneCmnSpark* m_pCEneCmnSpark;


	gxBool m_bCoverClosed;

	gxBool m_bDamage;
	gxBool m_bCover;
	gxBool m_bMaximum;
	gxBool m_bCrash;
	gxBool m_bForceCrash;

	Sint32 m_sSequence;
	Sint32 m_sDistance;
	Sint32 m_sOffset;

	Sint32 m_sCrashLv[enWallLength];

};

class CEneH0410Wall2 : public CEnemyBase , public CHoundEnemyBase
{
public:

	CEneH0410Wall2( Sint32 x, Sint32 y );
	~CEneH0410Wall2();

	void SeqMain();
	void SeqCrash();
	void Draw();

private:
	enum {
		enWallLength = 6,
	};

	void Atack();
	void LogicAI();

	CHitCollision m_HitKurai;
	CHitCollision m_HitAtari;

	Sint32 m_sOffset;

	gxBool m_bDamage;
	gxBool m_bCrash;


};

