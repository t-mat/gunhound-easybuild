enum {
	enDummyMax = 64,
	enDummyEnemyMax = 64,
	enDummyEnemySprMax = 8,
	enDummyEnemyWaitListMax = 128,
};

//---------------------------------------------------------------------------------

//出現待ち管理用
typedef struct StDummyEnemyWaitList {
	void Init()
	{
		bExist = gxFalse;
		bStart = gxFalse;
		sEneIndex = 0;
		sWaitTime = 0;
		sX = sY = 0;
		sTx = 0;
	}
	gxBool bStart,bExist;	//既に出撃した、このバッファを使用する
	Sint32 sEneIndex;		//出撃させる敵のIndex
	Sint32 sWaitTime;		//いつになれば出撃するのか？
	Sint32 sX,sY;			//出撃させるポイントの設定
	Sint32 sTx;

} StDummyEnemyWaitList;

//敵キャラ設定用
typedef struct StDummyEnemyConfig {
	void Init()
	{
		sSprIndex = 0;

//		for(Sint32 ii=0;ii<enDummyEnemySprMax;ii++)
//		{
//		}
//		stSprdat[ii] = ;

		m_bKurai = gxFalse;
		sKurai_u = 0;
		sKurai_v = 0;
		sKurai_w = 0;
		sKurai_h = 0;
		sKurai_dmg = 0;

		m_bHit = gxFalse;
		sAtari_u = 0;
		sAtari_v = 0;
		sAtari_w = 0;
		sAtari_h = 0;
		sAtari_dmg = 0;

		sSpeed = 0;
		sWeaponType = 0;
	}

	//--------------------------------------------

	Sint32 sSprIndex;
	gxSprite stSprdat[enDummyEnemySprMax];

	gxBool m_bKurai;
	Sint32 sKurai_u;
	Sint32 sKurai_v;
	Sint32 sKurai_w;
	Sint32 sKurai_h;
	Sint32 sKurai_dmg;

	gxBool m_bHit;
	Sint32 sAtari_u;
	Sint32 sAtari_v;
	Sint32 sAtari_w;
	Sint32 sAtari_h;
	Sint32 sAtari_dmg;

	Sint32 sSpeed;
	Sint32 sTx;
	Sint32 sWeaponType;

}StDummyEnemyConfig;

extern StDummyEnemyConfig   g_stDummyEnemyConfig[enDummyEnemyMax];
extern StDummyEnemyWaitList g_StDummyEnemyWaitList[ enDummyEnemyWaitListMax ];
void DummyEnemyWaitCheck(Sint32 sTime);
void DummyEnemyWaitInit();


//---------------------------------------------------------------------------------

class CEne0000Dummy : public CEnemyBase
{
public:

	CEne0000Dummy( Sint32 index , Sint32 x, Sint32 y );
	~CEne0000Dummy();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void Draw();
	void SetTargetPos(Sint32 tx)
	{
		m_sTargetPosX = tx;
	}

private:

	void Atack();
	void LogicAI();

	gxBool m_bDamage;
	gxBool m_bDisp;
	Sint32 m_sPadControl;
	Sint32 m_sLogicWait;

	CObj m_Obj;
	CHitCollision m_HitKurai;
	CHitCollision m_HitAtari;
	Sint32 m_sAtackTimer;
	Sint32 m_sEnemyIndex;
	Sint32 m_sTargetPosX;
};

