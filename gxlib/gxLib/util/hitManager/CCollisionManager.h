//--------------------------------------------------------------------
//
//当たり判定管理
//
//--------------------------------------------------------------------
#ifdef _CCOLLISION_MANAGER_H_
#else
#define _CCOLLISION_MANAGER_H_

#define MaxCollisionNum (1024+512)

class CCollisionManager;

enum EnCollisionID {
	enCollisionID_None,
	enCollisionID_A_Atack,
	enCollisionID_A_Defence,
	enCollisionID_B_Atack,
	enCollisionID_B_Defence,
	enCollisionID_C_Atack,
	enCollisionID_C_Defence,
};

enum {
	enCollisionAtrGnd = 0x01,
	enCollisionAtrAir = 0x02,
	enCollisionAtrAll = 0x03,
};


enum
{
	enCollisionTypeNone = 0x00,
	enCollisionTypeA = 0x01,
	enCollisionTypeB = 0x02,
	enCollisionTypeC = 0x04,
	enCollisionTypeD = 0x08,
	enCollisionTypeE = 0x10,
	enCollisionTypeF = 0x20,
	enCollisionTypeG = 0x40,
	enCollisionTypeH = 0x80,
	
};

typedef struct StHitRect
{
	//当たり判定管理構造体
	EnCollisionID sType;	//4
	Sint32 sX,sY,sW,sH;		//16
	Float32 fRot;			//4
	Sint32 m_sJudgeType;	//4

	//	Sint32 hp;
	StHitRect()
	{	//初期化
		m_sJudgeType = 0;
		sType = enCollisionID_None;
		sX = sY = sW = sH = 0;
		fRot = 0.f;
	}

} StHitRect;


class CCollision
{
public:
	CCollision();
	virtual ~CCollision();

	void On()
	{
		m_sFlag = gxTrue;
	}

	void Off()
	{
		m_sFlag = gxFalse;
	}

	bool IsDead()
	{
		if(m_sHp == 0) return gxTrue;

		return gxFalse;
	}

	bool IsHit()
	{
		return m_bHit;
	}

	StHitRect stHitRect;

	Sint32 m_sIndex;
	Sint8  m_sFlag,m_sTime;
	Sint32 m_sHitType;
	bool   m_bHit;
	Sint32 m_sHp;
	Sint32 m_sAp;
	Sint32 m_sAtr;

	void Set( EnCollisionID sType , Sint32 x , Sint32 y , Sint32 w , Sint32 h )
	{
		stHitRect.sType = sType;
		stHitRect.sX = x;
		stHitRect.sY = y;
		stHitRect.sW = w;
		stHitRect.sH = h;
		On();
	}

private:

};


class CCollisionManager
{
public:

	CCollisionManager();
/*
	{
		//全判定を初期化
		for(int i=0;i<MaxCollisionNum;i++)
		{
			m_pCollision[i] = NULL;
			m_pStCollisionBackup[i] = NULL;
		}
		//s_pInstance = this;
		m_sMaxCollision = 0;
		m_sCollisionNum = 0;

	};
*/
	virtual ~CCollisionManager();
/*
	{
		//全判定を解放
		for(int i=0;i<MaxCollisionNum;i++)
		{
			if( m_pCollision[i] ) delete m_pCollision[i];
			m_pCollision[i] = NULL;
//if( m_pStCollisionBackup[i] ) delete m_pStCollisionBackup[i];
		}
	};
*/
/*
	static CCollisionManager* GetInstance()
	{
		if( g_pCollisionManager )
		{
			return g_pCollisionManager;
		}
		else
		{
			g_pCollisionManager = new CCollisionManager();
		}
		return g_pCollisionManager;
	}
*/
	Sint32 SetRegist(CCollision* p);
	void UnRegist(Sint32 id);
	void Action();
	void Draw();	//デバッグ用
	bool judgeRect(int my,int en);

	Sint32 GetCollisionNum()
	{
		return m_sMaxCollision;
	}

	CCollision* GetCollision(int n)
	{
		if( n<0 || n >= m_sMaxCollision) return NULL;
		return m_pStCollisionBackup[n];
	}

	Sint32 GetNum()
	{
		return m_sCollisionNum;
	}

	CCollision *m_pCollision[MaxCollisionNum];

	Sint32 m_sMaxCollision;
	Sint32 m_sCollisionNum;

	SINGLETON_DECLARE(CCollisionManager);


private:


	CCollision *m_pStCollisionBackup[MaxCollisionNum];
};

#endif //_CCOLLISION_MANAGER_H_
