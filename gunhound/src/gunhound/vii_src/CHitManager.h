//#define HITTEST_MAX (1024)
#define ID_VERSIS_ATK (enCollisionID_A_Atack)
#define ID_VERSIS_DEF (enCollisionID_A_Defence)
#define ID_ENEMY_ATK  (enCollisionID_B_Atack)
#define ID_ENEMY_DEF  (enCollisionID_B_Defence)

typedef struct StHitObjSml{
	Sint32 x,y,id;
}StHitObjSml;

class CHitCollision;

class CHitManager : public CCollisionManager
{

public:


	CHitManager()
	{
		m_sOldHitMax = 0;
	}

	~CHitManager()
	{

	}

	void init()
	{

	}

	void Action();

	Sint32 GetOldMax()
	{
		return m_sOldHitMax;
	}

	Sint32 GetOldID( Sint32 n )
	{
		//@HIT@
		return m_stHit[n].id;//ID_ENEMY_DEF;
	}

/*
	Sint32 GetID( Sint32 n )
	{
		//@HIT@
		return 0;
	}
*/

	Sint32 GetOldX( Sint32 n )
	{
		//@HIT@
		n = ( n >= 0 && n <= m_sOldHitMax)? n : -1;
		return m_stHit[n].x;//*100;
	}

	Sint32 GetOldY( Sint32 n )
	{
		//@HIT@
		n = ( n >= 0 && n <= m_sOldHitMax)? n : -1;
		return m_stHit[n].y;//*100;
	}


private:
	void ClearHitCount()
	{
		//超重要
		m_sMaxCollision = 0;
	}

	gxBool HitCheck( Sint32 i,Sint32 j );
	void DrawDebug(Sint32 n);

	//@HIT@ old_hit[i].id
	Sint32 m_sOldHitMax;
	StHitObjSml m_stHit[MaxCollisionNum];
//	CHitCollision *m_pHitCollision[MaxCollisionNum];
};


class CHitCollision : public CCollision
{
public:
	CHitCollision()
	{
		m_bJudge = gxFalse;

		m_sForceID = 0;			//自分の攻撃、敵の攻撃、自分のくらい、敵のくらい
		x1=y1=x2=y2=0;			//当たり（くらい）判定用矩形
		ax1=ay1=ax2=ay2=0;			//当たり（くらい）判定用矩形
		dx = dy = 0;
		m_sAtackType = 0;		//あたった場所の差分保存用x,y,攻撃タイプ
		m_sHP = 1;				//体力
		m_sAP = 1;				//攻撃力
		m_bImpact = gxFalse;	//あたった瞬間にだけ立つフラグ
		m_sDamageCount = 0;		//ダメージ用カウンタ
		m_bFisrtDead = gxFalse;
		m_bFlag = gxFalse;		//２度当たり防止用フラグ
		m_bMuteki = gxFalse;
		m_bHomingable = gxTrue;
	}

	~CHitCollision()
	{

	}

	void init()
	{

	}

	void DisableAiming()
	{
		m_bHomingable = gxFalse;
	}

	gxBool is_damage()
	{
		//ダメージ表現中か？

		if( m_sDamageCount > 0 )
		{
			m_sDamageCount --;
			return gxTrue;
		}
		return gxFalse;
	}

	gxBool is_dead()
	{
		//体力切れているか？
		if( m_sHP <= 0 ) return gxTrue;
		return gxFalse;
	}

	void SetAtackType( Sint32 type )
	{
		m_sAtackType = type;
	}

	Sint32 IsAtackType( Sint32 type )
	{
		return ( m_sAtackType == type)? gxTrue : gxFalse;
	}

	gxBool IsImpacted()
	{
		return m_bImpact;
	}

	void SetOffImpact()
	{
		m_bImpact = gxFalse;
	}

	void set_hp(Sint32 n)
	{
		//体力設定
		m_sMax = n*100;
		m_sHP = n*100;
	}
	Sint32 get_hp()
	{
		//体力設定
		return m_sHP/100;
	}

	void set_ap(Sint32 n)
	{
		m_sAP = n*100;
	}

	Sint32 GetTotalDamage()
	{
		Sint32 n = (m_sMax-m_sHP)/100;
		if( n <= 0 ) n= 0;
		if( n >= m_sMax/100 ) n = m_sMax/100;
		return n;
	}

	void SetDirectHitPosition(Sint32 n)
	{
		m_sHP = n;
	}

	Sint32 GetDirectHitPosition()
	{
		return m_sHP;
	}

	void SetDirectAromorPosition(Sint32 n)
	{
		m_sAP = n;
	}

	void Activate()
	{
		//当たり判定を有効にする
		m_bJudge = gxTrue;
	}

	Sint32 GetDamageDir()
	{
		//ダメージ方向を返す
		return 1;
	}

	gxBool isFirstdead()
	{
		//今死んだ瞬間か？
		return m_bFisrtDead;
	}

	void SetMutekiOneTime()
	{
		m_bMuteki = gxTrue;
	}


	void SetHantei(Sint32 id , OBJ_POS_T *pPos)
	{
		m_sForceID = id;

		x1 = pPos->x/100 + ax1;
		y1 = pPos->y/100 + ay1;
		x2 = pPos->x/100 + ax2;
		y2 = pPos->y/100 + ay2;

		Sint32 tmpx,tmpy;
		tmpx = x2;
		tmpy = y2;

		if( x2<x1 )
		{
			x2 = x1;
			x1 = tmpx;
		}

		if( y2<y1 )
		{
			y2 = y1;
			y1 = tmpy;
		}

		Activate();
	}

	void Set(Sint32 id , OBJ_POS_T *pPos)
	{
		m_sForceID = id;

		if( m_sForceID&(ID_VERSIS_ATK|ID_ENEMY_ATK) )
		{
			x1 = pPos->x/100+pPos->ax1;
			y1 = pPos->y/100+pPos->ay1;
			x2 = pPos->x/100+pPos->ax2;
			y2 = pPos->y/100+pPos->ay2;
		}
		else
		{
			x1 = pPos->x/100+pPos->kx1;
			y1 = pPos->y/100+pPos->ky1;
			x2 = pPos->x/100+pPos->kx2;
			y2 = pPos->y/100+pPos->ky2;
		}
	}

	Sint32  m_sForceID;		//自分の攻撃、敵の攻撃、自分のくらい、敵のくらい
	Sint32  x1,y1,x2,y2;		//当たり（くらい）判定用矩形
	Sint32  ax1,ay1,ax2,ay2;	//当たり（くらい）判定用範囲
	Sint32  dx,dy;			//あたった場所の差分保存用x,y
	Sint32  m_sAtackType;	//攻撃タイプ
	Sint32  m_sMax;			//体力
	Sint32  m_sHP;			//体力
	Sint32  m_sAP;			//攻撃力
	Sint32  m_sDamageCount;//ダメージ用カウンタ
	gxBool m_bImpact;		//あたった瞬間にだけ立つフラグ→一度でもあたれば立つ
	gxBool m_bFisrtDead;	//体力がなくなったフレームのみ立つ
	gxBool m_bFlag;		//２度当たり防止用フラグ
	gxBool m_bJudge;		//判定を有効にする
	gxBool m_bMuteki;
	gxBool m_bHomingable;	//ホーミングできる
private:

};

