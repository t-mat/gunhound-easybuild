//--------------------------------------------------------------------------------
//
// Stage00:ラスボス
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../Effect/EffectBase.h"

enum {
	enPrioBossLeft = PRIO_ENEMY,
	enPrioBossPack,
	enPrioBossMiddle,
	enPrioBossMiddlePlus,
	enPrioBossRight = PRIO_PLAYER+1,
	enPrioBossEffect,
};

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -48,
	enKuraiTop   = -32,
	enKuraiRight =  16,
	enKuraiBottom=  64,

	enHigh = 200*100,//256*100,
#ifdef _VII_DEBUG_
	enHitPointLastBoss = 1,//enHitPointBossSpider,
#else
	enHitPointLastBoss = enHitPointBossSpider,
#endif
	enScore = 300000,
};

enum {
	enBOSS_COLOR = 0xffffc0c0,
	enBOSS_COLOR_oku = 0xffaf8080,
};

gxSprite SprCEneH0504Boss[]=
{
	{ enTexPageEnemyCommon05,0,160,64,64,32,18},//ボディ
};

gxSprite SprCEneH0504BossBody[]={
	{ enTexPageLastBoss,0,0,144,112,76,57},		//スパイダー（ボディ）
	//18,66 --右腕接続位置
	//102,70--左腕接続位置
};

gxSprite SprCEneH0504BossArm[]={
	{ enTexPageLastBoss,160,64,160,64,28,41},	//スパイダー（右腕：：細）
	//130,40--腕先接続位置
	{ enTexPageLastBoss,144,0,160,64,28,31},	//スパイダー（右腕：：太）
	//129,37--ガトリング接続位置

	{ enTexPageLastBoss,416,80,136,72,16,32},	//スパイダー（ガトリングガン）

	{ enTexPageLastBoss,320,0,160,80,128,48},	//スパイダー（シールド：：大）
	{ enTexPageLastBoss,176,128,144,80,128,48},	//スパイダー（シールド：：小）
};

gxSprite SprCEneH0504BossGun[]={
	{ enTexPageLastBoss,424,88 ,88,48,16,24},	//スパイダー（ガトリングガン）
	{ enTexPageLastBoss,424,144,88,48,16,24},	//スパイダー（ガトリングガン）
	{ enTexPageLastBoss,424,200,88,48,16,24},	//スパイダー（ガトリングガン）
};

gxSprite SprCEneH0504BossPack[]={
	{ enTexPageLastBoss,0,128,176,112,136,72},	//スパイダー（キャノピー）

	{ enTexPageLastBoss,320,160,96,64,60,42},	//キャノピーふた（小）
	{ enTexPageLastBoss,320,112,96,48,90,42},	//キャノピーふた（中）
};

class CEneBone
{
	//-----------------------------------------------------
	//ボス用ボーン制御
	//-----------------------------------------------------

public:

	CEneBone()
	{
		sx = sy = 0.f;
		dx = dy = 0.f;
		dr = 0;
		z = 0;
		fLength   = 1.f;
		fRotation = 0.f;
	}
	void DebugDraw()
	{
#ifdef _VII_DEBUG_
		Sint32 x0 = sx/100;
		Sint32 y0 = sy/100;
		Sint32 x1 = dx/100;
		Sint32 y1 = dy/100;
		viiDraw::BoxScrl_s( x1-1,y1-1,x1+1,y1+1,PRIO_DEBUG,gxTrue,0xffff0000,ATR_DFLT);
		viiDraw::Line_s( x0,y0,x1,y1,PRIO_DEBUG,ATR_DFLT,0xffffff00);
#endif
	}

	void Set(Sint32 x , Sint32 y)
	{
		fRotation = viiMath::Atan2d( y*1.f , x*1.f );//RAD2DEG( atan2(y*1.f,x*1.f) );
		fLength   = sqrt((x*x)*1.f + (y*y)*1.f);
	}

	Float32 sx,sy;
	Float32 dx,dy;
	Float32 dr;
	Sint32  z;
	Float32 fLength;
	Float32 fRotation;
};


class CBossBody
{
	//-----------------------------------------------------
	//ボス用ボディ制御
	//-----------------------------------------------------

public:

	CBossBody()
	{
		m_bDamage = gxFalse;
		m_Pos.z = enPrioBossMiddlePlus;
		m_fRotation = 0;
	};

	~CBossBody(){};

	void Action()
	{

		m_Sldr[enRight].Set(-58,11);
		m_Sldr[enLeft] .Set( 32,11);
		m_Back.Set(-62,-28);
		m_Eye.Set(52,-10);

		for(Sint32 ii=0; ii<enArmMax;ii++)
		{
			m_Sldr[ii].dr = m_fRotation;
			m_Sldr[ii].sx = m_Pos.x;
			m_Sldr[ii].sy = m_Pos.y;
			m_Sldr[ii].dx  = m_Sldr[ii].sx + viiMath::Cos( m_Sldr[ii].dr+m_Sldr[ii].fRotation )*m_Sldr[ii].fLength*100;
			m_Sldr[ii].dy  = m_Sldr[ii].sy + viiMath::Sin( m_Sldr[ii].dr+m_Sldr[ii].fRotation )*m_Sldr[ii].fLength*100;
		}

		m_Back.dr  = m_fRotation;
		m_Back.sx  = m_Pos.x;
		m_Back.sy  = m_Pos.y;
		m_Back.dx  = m_Back.sx + viiMath::Cos( m_Back.dr+m_Back.fRotation )*m_Back.fLength*100;
		m_Back.dy  = m_Back.sy + viiMath::Sin( m_Back.dr+m_Back.fRotation )*m_Back.fLength*100;

		m_Eye.dr = m_fRotation;
		m_Eye.sx = m_Pos.x;
		m_Eye.sy = m_Pos.y;
		m_Eye.dx = m_Eye.sx + viiMath::Cos( m_Eye.dr+m_Eye.fRotation )*m_Eye.fLength*100;;
		m_Eye.dy = m_Eye.sy + viiMath::Sin( m_Eye.dr+m_Eye.fRotation )*m_Eye.fLength*100;;
	}

	void Draw()
	{
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0504BossBody[ 0 ] , m_Pos.x , m_Pos.y , m_Pos.z , ATR_DFLT , enBOSS_COLOR , 1.f, m_fRotation );

		Sint32 x0,y0;
		x0 = m_Pos.x/100;
		y0 = m_Pos.y/100;

		m_Sldr[enRight].DebugDraw();
		m_Sldr[enLeft].DebugDraw();
		m_Back.DebugDraw();
		m_Eye.DebugDraw();
	};

	CEneBone* GetShoulder(Sint32 n)
	{
		return &m_Sldr[n];
	}

	CEneBone* GetBack()
	{
		return &m_Back;
	}
	ghPos m_Pos;

	Float32 m_fRotation;

	CEneBone m_Eye;

	void SetDamage( gxBool bFlag = gxTrue )
	{
		m_bDamage = bFlag;
	}

private:

	CEneBone m_Sldr[enArmMax];
	CEneBone m_Back;
	gxBool m_bDamage;

};


class CBossArm
{
	//-----------------------------------------------------
	//ボス用アーム制御
	//-----------------------------------------------------

public:

	CBossArm(Sint32 n)
	{
		m_sIndex = n;

		m_bDamage = gxFalse;
		m_Pos.z = enPrioBossLeft;

		m_fRotArm   = 0;	//ARM
		m_fRotHand  = 0;	//HAND
		m_fRotShld  = 0;	//SHIELD
		m_fRotArmShld = 0;

		//シールド開閉に関する変数
		m_bOpen     = gxFalse;
		m_fGattlingLength = 16.f;
		m_fShieldAddRot   = 0.f;
		//ガトリングガンに関する変数
		m_fGatAdd = 0.f;
		m_fGatCnt = 0.f;

		//キャッチモード
		m_bCatchMode = gxFalse;
		m_bCatchNow  = gxFalse;

		m_bAtackEnable = gxFalse;

		m_Kurai.ax1 = -32;
		m_Kurai.ay1 = -32;
		m_Kurai.ax2 =  32;
		m_Kurai.ay2 =  32;
		m_Kurai.set_ap( 1 );
		m_Kurai.set_hp( enHitPointLastBoss );

		m_KuraiProtect.ax1 = -32;
		m_KuraiProtect.ay1 = -32;
		m_KuraiProtect.ax2 =  32;
		m_KuraiProtect.ay2 =  32;
		m_KuraiProtect.set_ap( 1 );
		m_KuraiProtect.set_hp( enHitPointLastBoss );

		m_bProtecter = gxTrue;
		m_bDead      = gxFalse;
		m_sTimer     = 0;
	};

	~CBossArm(){};

	void Action()
	{
		m_bCatchNow = gxFalse;

		m_Arm .Set(112,0);
//		m_Hand.Set(60,-16);
		m_Hand.Set(m_fGattlingLength,6);
		m_Gun.Set(72,0);
		m_Shield.Set(16,-10);

		m_Arm.dr = m_fRotArm;
		m_Arm.sx = m_Pos.x;
		m_Arm.sy = m_Pos.y;
		m_Arm.dx = m_Arm.sx + viiMath::Cos( m_Arm.dr+m_Arm.fRotation )*m_Arm.fLength*100;
		m_Arm.dy = m_Arm.sy + viiMath::Sin( m_Arm.dr+m_Arm.fRotation )*m_Arm.fLength*100;

		m_Hand.dr = m_fRotArm+m_fRotHand;
		m_Hand.sx = m_Arm.dx;
		m_Hand.sy = m_Arm.dy;
		m_Hand.dx = m_Hand.sx + viiMath::Cos( m_Hand.dr+m_Hand.fRotation )*m_Hand.fLength*100;
		m_Hand.dy = m_Hand.sy + viiMath::Sin( m_Hand.dr+m_Hand.fRotation )*m_Hand.fLength*100;

		m_Gun.dr = m_fRotArm+m_fRotHand;
		m_Gun.sx = m_Hand.dx;
		m_Gun.sy = m_Hand.dy;
		m_Gun.dx = m_Gun.sx + viiMath::Cos( m_Gun.dr+m_Gun.fRotation )*m_Gun.fLength*100;
		m_Gun.dy = m_Gun.sy + viiMath::Sin( m_Gun.dr+m_Gun.fRotation )*m_Gun.fLength*100;

		m_Shield.dr = m_Hand.dr+m_fRotShld;
		m_Shield.sx = m_Hand.sx + viiMath::Cos( m_Hand.dr+m_Shield.fRotation+m_fShieldAddRot )*m_Shield.fLength*100;
		m_Shield.sy = m_Hand.sy + viiMath::Sin( m_Hand.dr+m_Shield.fRotation+m_fShieldAddRot )*m_Shield.fLength*100;
		m_Shield.dx = m_Hand.sx;
		m_Shield.dy = m_Hand.sy;

		//ガトリングガンの回転
		m_fGatCnt += m_fGatAdd;
		if( m_fGatAdd >= 30.f )
		{
			GattlingAtack();
		}

		if( m_bCatchMode )
		{
			viiEff::Set(EFF_ENERGYBALL , m_Gun.sx , m_Gun.sy , argset(100,100,0,100));
			if( m_HitCatch.IsImpacted() )
			{
				m_bCatchNow = gxTrue;
			}

			m_HitCatch.ax1 = -16;
			m_HitCatch.ay1 = -16;
			m_HitCatch.ax2 =  32;
			m_HitCatch.ay2 =  24;
			m_HitCatch.set_ap( 1 );
			m_HitCatch.set_hp( 1 );
			OBJ_POS_T Pos;
			Pos.x = m_Gun.sx;
			Pos.y = m_Gun.sy;
			m_HitCatch.SetHantei( ID_ENEMY_ATK , &Pos );
			m_bCatchMode = gxFalse;
		}

		if( m_Kurai.is_dead() )
		{
			if( !m_bDead )
			{
				//-----------------------------
				//シールドが落ちた
				//-----------------------------
				CEffectDmgprt *pDmg;
				pDmg = new CEffectDmgprt( m_Shield.sx , m_Shield.sy , -90 , 100 );
				pDmg->SetSprite( &SprCEneH0504BossArm[ 4 ] );
				pDmg->SetHeavy();
				pDmg->SetRotation( m_Shield.dr );
				pDmg->SetAtribute( ATR_DFLT|ATR_FLIP_X|ATR_FLIP_Y );
				pDmg->m_Pos.z = m_Pos.z + 1;

				viiMus::PlaySound( enSoundExplosionMiddle );
				EffectMakeBigBomb( m_Shield.sx , m_Shield.sy );
				viiEff::SetBombDeluxe( m_Shield.sx , m_Shield.sy , 1 , 1+viiSub::Rand()%2 );
			}
			m_bDead = gxTrue;
		}

		//プロテクター
		if( m_sIndex == enRight )
		{
			OBJ_POS_T Pos;
			Pos.x = m_Arm.sx;
			Pos.y = m_Arm.sy;
			m_KuraiProtect.set_ap( 1 );
			m_KuraiProtect.set_hp( enHitPointLastBoss );
			m_KuraiProtect.SetHantei( ID_ENEMY_DEF , &Pos );
		}

		if( !m_bDead )
		{
			m_bDamage = m_Kurai.is_damage();

			OBJ_POS_T Pos;
			Pos.x = m_Hand.dx;
			Pos.y = m_Hand.dy;
			m_Kurai.SetHantei( ID_ENEMY_DEF , &Pos );
		}
		else
		{
			m_bDamage = gxFalse;
		}
		m_sTimer ++;
	}

	void GattlingAtack()
	{
		if( !m_bAtackEnable ) return;
		if( m_bDead ) return;
		Sint32 ax,ay,r;
		ax = m_Gun.dx;
		ay = m_Gun.dy;

		r = (m_sTimer*42)%360;
		ax += viiMath::Cos( r )*1500;
		ay += viiMath::Sin( r )*1500;

		viiEff::Set(EFF_MAZZLE,ax , ay,NULL);

		CEne0001EneBullet* pEneBullet;
		pEneBullet = new CEne0001EneBullet( ax , ay, m_Gun.dr-r/30+12 , 600 , 4);	//
		viiMus::PlaySound( enSoundEneShotLaser );
	}

	void Draw()
	{
		Sint32 sGatCnt = s_cast<Sint32>(m_fGatCnt);
		sGatCnt = (sGatCnt/100)%3;
		Sint32 sCol = (m_sIndex==enRight)? enBOSS_COLOR : enBOSS_COLOR_oku;

		//アーム
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0504BossArm[ 0 ] , m_Arm.sx , m_Arm.sy , m_Pos.z , ATR_DFLT , sCol , 1.f, m_Arm.dr );
		m_Arm.DebugDraw();

		if( !m_bDead )
		{
			//ガン
			viiDraw::Sprdmg( m_bDamage , &SprCEneH0504BossGun[ sGatCnt ] , m_Gun.sx , m_Gun.sy , m_Pos.z , ATR_DFLT , ARGB_DFLT , 1.f, m_Gun.dr );
			m_Gun.DebugDraw();
		}

		{
			//ハンド
			Sint32 z = (m_sIndex==enRight)? 2 : -2;
			Sint32 r = (m_sIndex==enRight)? m_fRotArmShld : -m_fRotArmShld;
			Sint32 c = (m_sIndex==enRight)? sCol : 0xff404040;
			viiDraw::Sprdmg( m_bDamage , &SprCEneH0504BossArm[ 1 ] , m_Hand.sx , m_Hand.sy , m_Pos.z , ATR_DFLT , sCol , 1.f, m_Hand.dr );
			if(m_bProtecter)
			{
				viiDraw::Sprdmg( m_bDamage , &SprCEneH0504BossArm[ 3 ] , m_Arm.sx  , m_Arm.sy  , m_Pos.z+z , ATR_DFLT|ATR_FLIP_X|ATR_FLIP_Y , c , 1.f, m_Arm.dr+r );
			}
			m_Hand.DebugDraw();
		}
		if( !m_bDead )
		{
			//シールド
			Sint32 z = (m_sIndex==enRight)? 1 : 0;
			viiDraw::Sprdmg( m_bDamage , &SprCEneH0504BossArm[ 4 ] , m_Shield.sx , m_Shield.sy , m_Pos.z+z , ATR_DFLT|ATR_FLIP_X|ATR_FLIP_Y , sCol , 1.f, m_Shield.dr );
			m_Shield.DebugDraw();
		}
	}

	void SetArmKakudo( Float32 fr )
	{
		Float32 r = gxGetHomingDirection( m_Arm.dr , fr );
		m_fRotArm += r/12.f;
	}

	void SetHandKakudo( Float32 fr )
	{
		Float32 r = gxGetHomingDirection( m_Hand.dr , fr );
		m_fRotHand += r/12.f;
	}

	void OpenCannon(gxBool bOpen )
	{
		if(!m_bOpen && bOpen)
		{
			viiMus::PlaySound( enSoundJump );
		}
		m_bOpen = bOpen;
		if( m_bOpen )
		{
			m_fRotShld += (-15.f-m_fRotShld)/10.f;
			m_fShieldAddRot += (-125.f-m_fShieldAddRot)/10.f;
			m_fGattlingLength += ( 100.f-m_fGattlingLength)/10.f;
			if( m_fRotShld < -14.f )
			{
				m_fGatAdd += (60.f-m_fGatAdd)/30.f;
			}
		}
		else
		{
			if( m_fGatAdd < 10.f )
			{
				m_fRotShld += (0.f-m_fRotShld)/10.f;
				m_fShieldAddRot += (0.f-m_fShieldAddRot)/10.f;
				m_fGattlingLength += ( 52.f-m_fGattlingLength)/10.f;
			}
			m_fGatAdd += (0.f-m_fGatAdd)/30.f;
		}
	}

	void SetCatchMode( gxBool bCatch )
	{
		//キャッチモード
		m_bCatchMode = bCatch;
		if( !m_bCatchMode ) m_HitCatch.SetOffImpact();
	}

	gxBool IsCatch()
	{
		return m_bCatchNow;
	}
	void OpenShield(gxBool bFlag = gxTrue)
	{
		if( !bFlag )
		{
			m_fRotArmShld = 0.f;
			return;
		}
		m_fRotArmShld += (45-m_fRotArmShld)/5.f;
	}

	void GattlingEnable(gxBool bFlag)
	{
		if( !m_bAtackEnable ) m_sTimer = 0;
		m_bAtackEnable = bFlag;
	}

	gxBool IsDead()
	{
		return m_bDead;
	}

	ghPos m_Pos;

	Float32 m_fRotArm;	//ARM
	Float32 m_fRotHand;	//HAND
	Float32 m_fRotShld;	//SHIELD
	Float32 m_fRotArmShld;	//SHIELD

	CEneBone m_Arm;
	CEneBone m_Hand;
	CEneBone m_Gun;
	CEneBone m_Shield;

	void SetDamage( gxBool bFlag = gxTrue )
	{
		m_bDamage = bFlag;
	}

	void SetOffProtecter()
	{
		if( m_bProtecter )
		{
			Sint32 z = (m_sIndex==enRight)? 2 : -2;

			CEffectDmgprt *pDmg;
			pDmg = new CEffectDmgprt( m_Arm.sx  , m_Arm.sy , -90 , 100 );
			pDmg->SetSprite( &SprCEneH0504BossArm[ 3 ] );
			pDmg->SetHeavy();
			pDmg->SetRotation( m_Arm.dr );
			pDmg->SetAtribute( ATR_DFLT|ATR_FLIP_X|ATR_FLIP_Y );
			pDmg->m_Pos.z = m_Pos.z + z;
		}

		m_bProtecter = gxFalse;
	}

private:

	Float32 m_fGattlingLength;
	Float32 m_fShieldAddRot;
	Float32 m_fGatAdd;
	Float32 m_fGatCnt;

	Sint32 m_sIndex;
	Sint32 m_sTimer;

	gxBool m_bDamage;
	gxBool m_bDead;
	gxBool m_bOpen;
	gxBool m_bCatchMode;
	gxBool m_bCatchNow;
	gxBool m_bAtackEnable;
	gxBool m_bProtecter;

	CHitCollision m_HitCatch;
	CHitCollision m_Kurai;
	CHitCollision m_KuraiProtect;

};


class CBossPack
{
	//-----------------------------------------------------
	//ボス用バックパック制御
	//-----------------------------------------------------

public:

	CBossPack()
	{
		m_fRotBody = 0;
		m_fRotFrnt = 0;
		m_fRotRear = 0;

		m_fRotBody = -35;
		m_fRotRear = 0;

		m_bDamage = gxFalse;
		m_Pos.z = enPrioBossPack;
	};
	~CBossPack(){};

	void Action()
	{
		m_Body .Set( -96,-56 );
		m_Front.Set( -16,-38 );
		m_Rear .Set( -62,-48 );

		if( m_bOpen )
		{
			m_fRotRear += (125-m_fRotRear)/10.f;
		}
		else
		{
			m_fRotRear += (0-m_fRotRear)/10.f;
		}

		m_Body.dr = m_fRotBody+m_Pos.r;
		m_Body.sx = m_Pos.x;
		m_Body.sy = m_Pos.y;
		m_Body.dx = m_Body.sx + viiMath::Cos( m_Body.dr + m_Body.fRotation )*m_Body.fLength*100;
		m_Body.dy = m_Body.sy + viiMath::Sin( m_Body.dr + m_Body.fRotation )*m_Body.fLength*100;

		m_Front.dr = m_Body.dr+m_fRotFrnt;
		m_Front.sx = m_Pos.x;
		m_Front.sy = m_Pos.y;
		m_Front.dx = m_Front.sx + viiMath::Cos( m_Front.dr + m_Front.fRotation )*m_Front.fLength*100;
		m_Front.dy = m_Front.sy + viiMath::Sin( m_Front.dr + m_Front.fRotation )*m_Front.fLength*100;

		m_Rear.dr = m_Body.dr+m_fRotRear;
		m_Rear.sx = m_Pos.x;
		m_Rear.sy = m_Pos.y;
		m_Rear.dx = m_Rear.sx + viiMath::Cos( m_Body.dr + m_Rear.fRotation )*m_Rear.fLength*100;
		m_Rear.dy = m_Rear.sy + viiMath::Sin( m_Body.dr + m_Rear.fRotation )*m_Rear.fLength*100;

		m_bOpen = gxFalse;
	}
	void Draw()
	{
		//パック
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0504BossPack[ 0 ] , m_Body.sx  , m_Body.sy  , m_Pos.z , ATR_DFLT , enBOSS_COLOR , 1.f, m_Body.dr );
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0504BossPack[ 1 ] , m_Front.dx , m_Front.dy , m_Pos.z , ATR_DFLT , enBOSS_COLOR , 1.f, m_Front.dr );
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0504BossPack[ 2 ] , m_Rear.dx  , m_Rear.dy  , m_Pos.z , ATR_DFLT , enBOSS_COLOR , 1.f, m_Rear.dr );
		m_Body.DebugDraw();
		m_Front.DebugDraw();
		m_Rear.DebugDraw();
	}

	void CannopyOpen()
	{
		m_bOpen = gxTrue;
	}

	void SetDamage( gxBool bFlag = gxTrue )
	{
		m_bDamage = bFlag;
	}

	ghPos m_Pos;

	CEneBone m_Body;
	CEneBone m_Front;
	CEneBone m_Rear;

private:

	Float32 m_fRotBody;
	Float32 m_fRotFrnt;
	Float32 m_fRotRear;

	gxBool m_bOpen;
	gxBool m_bDamage;

};


CEneH0504Boss::CEneH0504Boss( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_Add.x = 0;
	m_Add.y = 0;

	m_HitKurai.set_hp( enHitPointLastBoss );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_pBody         = new CBossBody();
	m_pArm[enRight] = new CBossArm(enRight);
	m_pArm[enLeft]  = new CBossArm(enLeft);
	m_pPack         = new CBossPack();

/*
	m_pArm[enLeft] ->m_Pos.z = PRIO_BG2;
	m_pArm[enRight]->m_Pos.z = PRIO_BG3;
	m_pPack->m_Pos.z         = PRIO_ENEMY-1;
*/
	m_pArm[enLeft] ->m_Pos.z = enPrioBossLeft;
	m_pArm[enRight]->m_Pos.z = enPrioBossRight;
	m_pPack->m_Pos.z         = enPrioBossPack;

	m_sDirection = DIR_LEFT;
	m_sAtribute  = ATR_DFLT;

	m_sHigh    = 0;
	m_sControl = 0;

	m_bMissile   = gxFalse;
	m_bHanging   = gxFalse;
	m_bGunMode   = gxFalse;
	m_bUpperMode = gxFalse;
	m_bLaserMode = gxFalse;
	m_bChumoku   = gxFalse;
	m_bStart     = gxFalse;

	memset( &m_stHang    , 0x00 ,sizeof(m_stHang)  );
	memset( &m_stUpper   , 0x00 ,sizeof(m_stUpper) );
	memset( &m_stMissile , 0x00 ,sizeof(m_stMissile) );
	memset( &m_stLaser   , 0x00 ,sizeof(m_stLaser) );
	memset( &m_stGunMode , 0x00 ,sizeof(StGumMode) );

	m_Through.x = 0;
	m_Through.y = 0;
	m_TgtPos = m_Add;

	m_sCrashArm[0] = 0;
	m_sCrashArm[1] = 0;
	m_sCrashSeq = 0;
	m_sCrashWait = 0;
	m_sAtackSeq = 0;
	m_sTargettingCnt = 0;

	m_sInitSeq  = 0;
	m_sInitWait = 0;

	m_bCrash = gxFalse;
}


CEneH0504Boss::~CEneH0504Boss()
{
	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

	delete m_pBody;
	delete m_pArm[enRight];
	delete m_pArm[enLeft];
	delete m_pPack;
}


void CEneH0504Boss::SeqInit()
{
	gxBool bDmgParts = gxFalse;

	SetRader( enTypeMarkerMiddleBoss );

	m_sHigh = 0;

	switch( m_sInitSeq ){
	case 0:
		bDmgParts = gxTrue;
		m_pArm[enRight]->OpenShield(gxFalse);
		m_pArm[enLeft]->OpenShield(gxFalse);
		m_pArm[enRight]->m_fRotArm  = 30;
		m_pArm[enRight]->m_fRotHand = 175;
		m_pArm[enLeft ]->m_fRotArm  = 30;
		m_pArm[enLeft ]->m_fRotHand = 170;
		m_sInitWait = 0;
		viiMus::PlaySound( enSoundExplosionMaximum );
		m_sInitSeq = 5;
		break;
	case 5:
		m_Pos.y -= 80;
		if( viiSub::Rand()%32  == 0 )
		{
			viiMus::PlaySound(enSoundExplosionMiddle);
		}
		if( m_bStart )
		{
			viiMus::PlaySound( enSoundCrashB );
			m_sInitWait = 0;
			m_sInitSeq = 10;
		}
		break;

	case 10:
		bDmgParts = gxTrue;
		m_pArm[enRight]->m_fRotArm  += (0-m_pArm[enRight]->m_fRotArm)/10.f;
		m_pArm[enRight]->m_fRotHand += (0-m_pArm[enRight]->m_fRotHand)/10.f;
		m_pArm[enLeft ]->m_fRotArm  += (0-m_pArm[enLeft ]->m_fRotArm)/10.f;
		m_pArm[enLeft ]->m_fRotHand += (0-m_pArm[enLeft ]->m_fRotHand)/10.f;
		if( m_sInitWait >= 30 )
		{
			viiMus::PlaySound( enSoundJump );
			m_sInitWait = 0;
			m_sInitSeq = 20;
		}
		break;

	case 20:
		bDmgParts = gxTrue;
		m_pArm[enRight]->m_fRotArm  += (145-m_pArm[enRight]->m_fRotArm)/15.f;
		m_pArm[enRight]->m_fRotHand += (-145-m_pArm[enRight]->m_fRotHand)/15.f;
		m_pArm[enLeft ]->m_fRotArm  += (105-m_pArm[enLeft ]->m_fRotArm)/15.f;
		m_pArm[enLeft ]->m_fRotHand += (-105-m_pArm[enLeft ]->m_fRotHand)/15.f;
		if( m_sInitWait >= 30 )
		{
			viiMus::PlaySound( enSoundBoost );
			m_sInitWait = 0;
			m_sInitSeq = 30;
		}
		break;
	case 30:
		m_pArm[enRight]->OpenShield();
		m_pArm[enLeft]->OpenShield();

		if( m_sInitWait <= 120 )
		{
			bDmgParts = gxTrue;
		}

		if( m_sInitWait >= 300 )
		{
			m_sInitWait = 0;
			m_sInitSeq = 30;
			m_sHigh = enHigh;
			SetActionSeq( enActionSeqMain );
		}
		break;
	}

	CEffectGroundSmoke *p;
	p = new CEffectGroundSmoke( m_Pos.x+(-128+viiSub::Rand()%256)*100 , m_Pos.y+(-128+viiSub::Rand()%256)*100, DIR_RIGHT );
	p->SetScale(3.f);
	p->SetAlpha(128);

	if( bDmgParts )
	{
		pGame->QuakeOn(32);
		if( m_sTimer%16 == 0 )
		{
			CEffectDmgprt *p;
			p = new CEffectDmgprt( m_Pos.x+(-128+viiSub::Rand()%160)*100, m_Pos.y , 105-viiSub::Rand()%30 , 0 );
			p->m_Pos.z = PRIO_BG2;
		}
	}

	Gimmick();
	DirectionAdjust();

/*
	if( m_bStart )
	{
		m_sHigh = enHigh;
		SetActionSeq( enActionSeqMain );
	}
*/

	m_sInitWait ++;
}


void CEneH0504Boss::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------
	m_bChumoku = gxFalse;
	m_pArm[enRight]->OpenShield();
	m_pArm[enLeft]->OpenShield();

	if( m_HitKurai.is_dead() )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

/*
	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}
*/
	m_sControl = 0;

	LogicAI();

	Gimmick();

	DirectionAdjust();

	//くらい判定登録
	if( m_pArm[enRight]->IsDead() && m_pArm[enLeft]->IsDead() )
	{
		m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );
	}

}


void CEneH0504Boss::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	SetRader( enTypeMarkerMiddleBoss );

	if( Atack() )
	{
		return;
	}

	LogicCPU();	//m_sControl = Joy[0].psh;

	if( m_sControl&BTN_C )
	{
		//バックミサイル中
		m_stMissile.m_sSeq  = 0;
		m_stMissile.m_sWait = 0;
		m_bMissile = gxTrue;
	}
	else if( m_sControl&BTN_Z )
	{
		//レーザー攻撃中
		m_bLaserMode = gxTrue;
		m_stLaser.m_sSeq = 0;
	}
	else if( m_sControl&BTN_X )
	{
		//ガトリングガン攻撃中
		m_stGunMode.m_sSeq  = 0;
		m_stGunMode.m_sWait = 0;
		m_bGunMode = gxTrue;
	}
	else if( m_sControl&BTN_B )
	{
		//アッパー攻撃中
		m_stUpper.m_sSeq = 0;
		m_bUpperMode     = gxTrue;
	}
	else  if( m_sControl&BTN_Y )
	{
		//キャッチモード中
		m_stHang.m_sSeq = 0;
		m_bHanging = gxTrue;
	}
	else
	{
		//デフォルトスタンス
		m_pArm[enRight]->SetArmKakudo ( 90+55 );
		m_pArm[enRight]->SetHandKakudo( 20 );

		m_pArm[enLeft]->SetArmKakudo ( 90 );
		m_pArm[enLeft]->SetHandKakudo( 15 );

		Sint32 ax,ay,vx,vy,r;

		ax = m_pBody->m_Eye.dx/100;
		ay = m_pBody->m_Eye.dy/100;

		vx = pGame->GetHound()->m_Pos.x/100;
		vy = pGame->GetHound()->m_Pos.y/100;

		r = viiMath::Atan2d( (vy-64)-ay , vx-ax );	//gxAtanDeg

		if( r < -45 ) r = -45;
		if( r >  45 ) r =  45;

		m_pBody->m_fRotation += ( r - m_pBody->m_fRotation ) / 30.f;

	}
}


void CEneH0504Boss::LogicCPU()
{
	//------------------------------------
	//思考ルーチン
	//------------------------------------
	Sint32 vx = pGame->GetHound()->m_Pos.x-128*100;
	Sint32 vy = m_Pos.y;
	Sint32 sRange = 256*100;
	Sint32 sMaxSpeed = 640,sSpeedAdd=10;

/*
	while( !viiSub::IsGroundBlock( vx , vy) )
	{
		vy += 200;
	}
	vy -= enHigh;
*/

	if( m_Pos.x < vx - sRange )
	{
		if( m_Add.x < sMaxSpeed ) m_Add.x += sSpeedAdd;
	}
	else if( m_Pos.x > vx + sRange )
	{
		if( m_Add.x > -sMaxSpeed ) m_Add.x -= sSpeedAdd;
	}
	else
	{
		m_Add.x += (0 - m_Add.x)/30;
	}

	if( m_Pos.y < vy-3200 )
	{
		if( m_Add.y < sMaxSpeed ) m_Add.y += sSpeedAdd;
	}
	else if( m_Pos.y > vy+3200 )
	{
		if( m_Add.y > -sMaxSpeed ) m_Add.y -= sSpeedAdd;
	}
	else
	{
		m_Add.y += (0 - m_Add.y)/30;
	}

	if( m_Add.x < -sMaxSpeed ) m_Add.x = -sMaxSpeed;
	if( m_Add.x >  sMaxSpeed ) m_Add.x =  sMaxSpeed;
	if( m_Add.y < -sMaxSpeed ) m_Add.y = -sMaxSpeed;
	if( m_Add.y >  sMaxSpeed ) m_Add.y =  sMaxSpeed;

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	//----------------------------------------------
	//キャッチエリアを移動しているか？
	//----------------------------------------------
	if( IsCatchArea() )
	{
		m_sTargettingCnt ++;
		if( m_sTargettingCnt >= 128 )
		{
			//パンチモードにする
			m_sTargettingCnt = 0;
			m_sControl = BTN_Y;
			return;
		}
	}
	else
	{
		m_sTargettingCnt = 0;
	}

	//------------------------------
	//近づきすぎたらつかまれる
	//------------------------------
	Sint32 sDistance = (Sint32)viiMath::Dist(pGame->GetHound()->m_Pos.x-m_Pos.x , pGame->GetHound()->m_Pos.y-m_Pos.y );

	if( sDistance <= 64*100 )
	{
		m_sControl = BTN_Y;
		return;
	}

	if( sDistance >= 480*100 )
	{
		return;
	}

	if( gxAbs(m_Add.x)>=300)
	{
		return;
	}

	vx = pGame->GetHound()->m_Pos.x;
	vy = pGame->GetHound()->m_Pos.y;

	if( vx > m_Pos.x+800*100 && vy >= m_Pos.y )
	{
		m_sControl = BTN_Z;
		return;
	}

	switch( m_sAtackSeq ){
	case enAtackGattling:
		//ガトリングガン攻撃
		m_sControl = BTN_X;
		break;

	case enAtackReppuken:
		//アッパー攻撃中
		m_sControl = BTN_B;
		break;

	case enAtackLaser:
		//レーザー攻撃中
		m_sControl = BTN_Z;
		break;

	case enAtackMissile:
		//バックミサイル中
		m_sControl = BTN_C;
		break;

	case enAtackCatch:
		//キャッチモード中
		m_sControl = BTN_Y;
		break;
	}

	m_sAtackSeq ++;
	m_sAtackSeq = m_sAtackSeq%enAtackMax;

}


void CEneH0504Boss::Gimmick()
{
	//------------------------------------
	//左ガトリングガン用意
	//------------------------------------
	if( m_sControl&BTN_R1 )
	{
		m_pArm[enRight]->OpenCannon(gxTrue);
	}
	else
	{
		m_pArm[enRight]->OpenCannon(gxFalse);
	}

	//------------------------------------
	//左ガトリングガン用意
	//------------------------------------
	if( m_sControl&BTN_L1 )
	{
		m_pArm[enLeft]->OpenCannon(gxTrue);
	}
	else
	{
		m_pArm[enLeft]->OpenCannon(gxFalse);
	}

	//------------------------------------
	//砂煙
	//------------------------------------
	for(Sint32 jj=0;jj<1;jj++)
	{
		CEffectGroundSmoke *p;
		p = new CEffectGroundSmoke( m_Pos.x+(-240+viiSub::Rand()%480)*100,900*100, DIR_RIGHT );
		p->SetScale(3.f);
		p->SetAlpha(64);
	}

	if( m_sTimer%2 == 0 )
	{
		new CEffectBossFire( m_Pos.x-3200 , m_Pos.y+1200 , 80+viiSub::Rand()%80 , 1800+viiSub::Rand()%300 );
	}

	Sint32 vx = pGame->GetHound()->m_Pos.x-128*100;
	Sint32 vy = m_Pos.y;

	while( !viiSub::IsGroundBlock( vx , vy) )
	{
		vy += 200;
	}

	if( m_sHigh )
	{
		vy -= m_sHigh;
		m_Pos.y += (vy-m_Pos.y)/100.f;
		//m_pBody->m_Pos.x = m_Pos.x;
		//m_pBody->m_Pos.y += (vy-m_Pos.y)/100.f;

		//フワフワ
		m_Pos.y += viiMath::Cos((m_sTimer*8)%360)*50;

		m_pBody->m_Pos.x = m_Pos.x;
		m_pBody->m_Pos.y = m_Pos.y;
	}
	else
	{
		m_pBody->m_Pos.x = m_Pos.x;
		m_pBody->m_Pos.y = m_Pos.y;
	}


	m_pBody->Action();

	m_pArm[enRight]->m_Pos.x = m_pBody->GetShoulder(enRight)->dx;
	m_pArm[enRight]->m_Pos.y = m_pBody->GetShoulder(enRight)->dy;
	m_pArm[enRight]->Action();

	m_pArm[enLeft]->m_Pos.x = m_pBody->GetShoulder(enLeft)->dx;
	m_pArm[enLeft]->m_Pos.y = m_pBody->GetShoulder(enLeft)->dy;
	m_pArm[enLeft]->Action();

	m_pPack->m_Pos.x = m_pBody->GetBack()->dx;
	m_pPack->m_Pos.y = m_pBody->GetBack()->dy;
	m_pPack->m_Pos.r = m_pBody->m_fRotation;
	m_pPack->Action();

}


gxBool CEneH0504Boss::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

	if( m_bHanging )
	{
		//ハンギングモード
		LogicCatchMode();
		return gxTrue;
	}
	else if( m_bGunMode )
	{
		//ガン乱射モード
		LogicGunMode();
		return gxTrue;
	}
	else if( m_bUpperMode )
	{
		//アッパーモード
		LogicUpperMode();
		return gxTrue;
	}
	else if( m_bMissile )
	{
		//ミサイルモード
		LogicMissileMode();
		return gxTrue;
	}
	else if( m_bLaserMode )
	{
		LaserBeemMode();
		return gxTrue;
	}

	return gxFalse;

}


void CEneH0504Boss::LogicUpperMode()
{
	//-----------------------------------------------------
	//アッパーモード
	//-----------------------------------------------------
	gxBool bAtack = gxFalse;

	switch( m_stUpper.m_sSeq ){
	case 0:
		normalize(  m_pArm[enRight]->m_fRotArm  );
		normalize(  m_pArm[enRight]->m_fRotHand );
		normalize(  m_pArm[enLeft]->m_fRotArm   );
		normalize(  m_pArm[enLeft]->m_fRotHand  );
		normalize(  m_pBody->m_fRotation        );
		m_stUpper.m_sGahaCounter = 0;
		m_stUpper.m_sWait        = 0;
		m_stUpper.m_sRotation    = 90+15;
		m_stUpper.m_sDistance    = 0;
		m_stUpper.m_sBodyRot     = 0;

		m_stUpper.m_sArmRot[enRight]  = 170;
		m_stUpper.m_sHandRot[enRight] = 10;
		m_stUpper.m_sArmRot[enLeft]   = 45;
		m_stUpper.m_sHandRot[enLeft]  = -20;
		m_stUpper.m_sBodyRot          = 15;

		m_stUpper.m_sSeq         = 100;
		break;

	case 100:
		if( m_stUpper.m_sWait >= 90 )
		{
			m_stUpper.m_sSeq = 120;
			m_stUpper.m_sWait = 0;
		}
		break;

	case 120:
		m_stUpper.m_sArmRot[enRight]  = 90+45;
		m_stUpper.m_sHandRot[enRight] = 20;
		m_stUpper.m_sArmRot[enLeft]   = 70+45;
		m_stUpper.m_sHandRot[enLeft]  = -120;
		m_stUpper.m_sBodyRot          = 20;
		if( m_stUpper.m_sWait >= 20 )
		{
			viiMus::PlaySound( enSoundMissile );
			m_stUpper.m_sSeq = 140;
			m_stUpper.m_sWait = 0;
		}
		break;

	case 140:
		m_stUpper.m_sRotation        -= 2;
		m_stUpper.m_sArmRot[enRight]  = -80;
		m_stUpper.m_sHandRot[enRight] = -40;
		m_stUpper.m_sArmRot[enLeft]   = 120;
		m_stUpper.m_sHandRot[enLeft]  = -100;
		m_stUpper.m_sBodyRot          = -45;
		if( m_stUpper.m_sWait >= 8 )
		{
			bAtack = gxTrue;
		}
		if( m_stUpper.m_sWait >= 120 )
		{
			m_stUpper.m_sSeq = 900;
			m_stUpper.m_sWait = 0;
		}
		break;

	case 900:
		m_stUpper.m_sBodyRot          = 0;
		if( m_stUpper.m_sWait >= 120 )
		{
			m_bUpperMode = gxFalse;
			m_stUpper.m_sWait = 0;
		}
		break;
	}

	Sint32 r = m_stUpper.m_sRotation;

	if( bAtack )
	{
		m_stUpper.m_sDistance = 120*100;

		if( m_stUpper.m_sWait%8 == 0 )
		{
			Sint32 ax,ay,r;

			ax = viiMath::Cos( m_stUpper.m_sRotation )*m_stUpper.m_sDistance*2;
			ay = viiMath::Sin( m_stUpper.m_sRotation )*m_stUpper.m_sDistance*1;
			ax += m_Pos.x-12000;
			ay += m_Pos.y+6400;
			r = m_stUpper.m_sRotation;//-30;

			for(Sint32 ii=0;ii<5;ii++)
			{
				CEne0001EneSuperBall *q;
				q = new CEne0001EneSuperBall( ax , ay , r, 240*ii  );
				q->SetPattern( CEne0001EneSuperBall::enPatternDrop );
				q->SetScale( 1.f );
				q->SetEndless();
				q->SetDropGensui(gxTrue, 500 );

			}
		}
	}

	m_pBody->m_fRotation += ( m_stUpper.m_sBodyRot - m_pBody->m_fRotation ) / 20.f;
	m_pArm[enRight]->m_fRotArm  += ( m_stUpper.m_sArmRot[enRight] - m_pArm[enRight]->m_fRotArm ) / 20.f;
	m_pArm[enRight]->m_fRotHand += ( m_stUpper.m_sHandRot[enRight] - m_pArm[enRight]->m_fRotHand) / 20.f;

	m_pArm[enLeft]->m_fRotArm  += ( m_stUpper.m_sArmRot[enLeft] - m_pArm[enLeft]->m_fRotArm ) / 20.f;
	m_pArm[enLeft]->m_fRotHand += ( m_stUpper.m_sHandRot[enLeft] - m_pArm[enLeft]->m_fRotHand) / 20.f;

	m_stUpper.m_sWait ++;

}


void CEneH0504Boss::LogicMissileMode()
{
	//-----------------------------------------------------
	//ミサイルモード
	//-----------------------------------------------------
	gxBool bMissile = gxFalse;

	m_pPack->CannopyOpen();

	switch( m_stMissile.m_sSeq ){
	case 0:
		normalize(  m_pArm[enRight]->m_fRotArm  );
		normalize(  m_pArm[enRight]->m_fRotHand );
		normalize(  m_pArm[enLeft]->m_fRotArm   );
		normalize(  m_pArm[enLeft]->m_fRotHand  );
		normalize(  m_pBody->m_fRotation        );
		m_stMissile.m_sGahaCounter = 0;
		m_stMissile.m_sWait        = 0;
		m_stMissile.m_sRotation    = 90-20;
		m_stMissile.m_sDistance    = 0;
		m_stMissile.m_sSeq         = 100;
		break;

	case 100:
		if( m_stMissile.m_sWait%18 == 0 )
		{
			bMissile = gxTrue;
		}
		if( m_stMissile.m_sWait >= 60*5 )
		{
			m_stMissile.m_sSeq         = 900;
		}
		break;

	case 900:
		m_bMissile = gxFalse;
		break;
	}


	if( bMissile )
	{
		Sint32 ax,ay;
		CEne0001EneSuperBall *p;

		ax = m_pPack->m_Body.dx+(-16+viiSub::Rand()%32)*100;
		ay = m_pPack->m_Body.dy+(-16+viiSub::Rand()%32)*100;

		Sint32 sRotation = get_valken_rot( ax , ay+2400 )+180;
		p = new CEne0001EneSuperBall( ax , ay , sRotation-30+viiSub::Rand()%60 ,300 );
		p->SetPattern( CEne0001EneSuperBall::enPatternMicroMissile );
		p->SetMaxSpeed( 720 );
		p->SetScrollOutRange( WINDOW_W*200 );
		p->SetScale( 1.0f );
		p->SetLevel(1);
		p->SetEndless();
		viiMus::PlaySound(enSoundEneShotLaser);

		viiEff::Set( EFF_MAZZLE , ax , ay , NULL );
		viiEff::SetBombSimple(    ax , ay , 1 , 1+viiSub::Rand()%2 );
		viiMus::PlaySound( enSoundMissile );
	}

	m_stMissile.m_sWait ++;
}


void CEneH0504Boss::LaserBeemMode()
{
	//-----------------------------------------------------
	//レーザービームモード
	//-----------------------------------------------------
	gxBool bLaser = gxFalse;
	gxBool bEnergy = gxFalse;

	switch( m_stLaser.m_sSeq ){
	case 0:
		normalize(  m_pArm[enRight]->m_fRotArm  );
		normalize(  m_pArm[enRight]->m_fRotHand );
		normalize(  m_pArm[enLeft]->m_fRotArm   );
		normalize(  m_pArm[enLeft]->m_fRotHand  );
		normalize(  m_pBody->m_fRotation        );
		m_stLaser.m_sGahaCounter = 0;
		m_stLaser.m_sWait        = 0;
		m_stLaser.m_sRotation    = 90-20;
		m_stLaser.m_sDistance    = 0;
		m_stLaser.m_sBodyRot     = 0;

		m_stLaser.m_sArmRot[enRight]  = -20;
		m_stLaser.m_sHandRot[enRight] = -45;
		m_stLaser.m_sArmRot[enLeft]   = -20;
		m_stLaser.m_sHandRot[enLeft]  = -120;
		m_stLaser.m_sBodyRot          = 0;
		viiMus::PlaySound( enSoundCrashB );

		m_stLaser.m_sSeq         = 100;
		break;

	case 100:
		bEnergy = gxTrue;
		if( m_stLaser.m_sWait >= 60*3 )
		{
			m_stLaser.m_sSeq = 120;
			m_stLaser.m_sWait = 0;
		}
		break;

	case 120:
		m_stLaser.m_sArmRot[enRight]  = 90+45;
		m_stLaser.m_sHandRot[enRight] = -125;
		m_stLaser.m_sArmRot[enLeft]   = 90;
		m_stLaser.m_sHandRot[enLeft]  = -90;
		bLaser = gxTrue;
		if( m_stLaser.m_sWait%4 == 0 )
		{
			viiMus::PlaySound(enSoundEneShotSmall);
		}
		if( m_stLaser.m_sWait <= 60*3 )
		{
			bEnergy = gxTrue;
		}
		if( m_stLaser.m_sWait >= 60*5 )
		{
			m_stLaser.m_sSeq = 900;
			m_stLaser.m_sWait = 0;
		}
		break;
	case 900:
		if( m_stLaser.m_sWait >= 60*2 )
		{
			m_bLaserMode = gxFalse;
		}
		break;
	}


	Sint32 ax,ay;

	ax = m_pBody->m_Eye.dx;
	ay = m_pBody->m_Eye.dy;

	if( bEnergy )
	{
		viiEff::Set(EFF_ENERGYBALL , ax , ay , argset(200,100,0,100));
	}

	if( bLaser )
	{
		if( m_stLaser.m_sWait%48 == 0)
		{
			CAtkH0002GMissile *p;
			p = new CAtkH0002GMissile( m_Pos.x , m_Pos.y , 100, 480 );
			p->SetForce( CAtkH0001Missile::enForceID_Enemy );
			p->SetRotationFix(180+20);
			viiMus::PlaySound(enSoundEneShotLaser);
		}
		LaserBeemAtack();
	}

	m_pBody->m_fRotation += ( m_stLaser.m_sBodyRot - m_pBody->m_fRotation ) / 30.f;
	m_pArm[enRight]->m_fRotArm  += ( m_stLaser.m_sArmRot[enRight] - m_pArm[enRight]->m_fRotArm ) / 10.f;
	m_pArm[enRight]->m_fRotHand += ( m_stLaser.m_sHandRot[enRight] - m_pArm[enRight]->m_fRotHand) / 10.f;

	m_pArm[enLeft]->m_fRotArm  += ( m_stLaser.m_sArmRot[enLeft] - m_pArm[enLeft]->m_fRotArm ) / 10.f;
	m_pArm[enLeft]->m_fRotHand += ( m_stLaser.m_sHandRot[enLeft] - m_pArm[enLeft]->m_fRotHand) / 10.f;

	m_stLaser.m_sWait ++;
}


void CEneH0504Boss::LogicGunMode()
{
	//-----------------------------------------------------
	//ガトリングガンモード
	//-----------------------------------------------------

	if( m_pArm[enRight]->IsDead() && m_pArm[enLeft]->IsDead() )
	{
		//両腕とも壊れていれば使わない
		m_stGunMode.m_sSeq = 999;
	}

	switch( m_stGunMode.m_sSeq ){
	case 0:
		//初期化
		normalize(  m_pArm[enRight]->m_fRotArm  );
		normalize(  m_pArm[enRight]->m_fRotHand );
		normalize(  m_pArm[enLeft]->m_fRotArm   );
		normalize(  m_pArm[enLeft]->m_fRotHand  );
		normalize(  m_pBody->m_fRotation        );
		m_stGunMode.m_sWait = 0;
		m_stGunMode.m_sLeft  = -1;
		m_stGunMode.m_sRight = -1;
		m_stGunMode.m_sSeq = 100;
		break;

	case 100:
		//右腕準備
		m_stGunMode.m_sRight  = 0;
		m_stGunMode.m_sSeq    = 200;
		break;

	case 200:
		if( m_stGunMode.m_sWait >= 60 )
		{
			m_stGunMode.m_sLeft  = 0;
			m_stGunMode.m_sSeq   = 900;
		}
		break;

	case 900:
		if( m_stGunMode.m_sWait >= 60*10 )
		{
			m_stGunMode.m_sSeq = 999;
			m_stGunMode.m_sWait = 0;
		}
		break;

	case 999:
		m_bGunMode = gxFalse;
		break;
	}

	m_sControl = 0;

	if( m_stGunMode.m_sLeft >= 0 )
	{
		m_stGunMode.m_sLeft ++;
		m_stGunMode.m_sLeft = m_stGunMode.m_sLeft%120;

		m_sControl |= BTN_L1;

		if( m_stGunMode.m_sLeft < 40 )
		{
			//照準を合わせる
			m_TgtPos.x = pGame->GetHound()->m_Pos.x;
			m_TgtPos.y = pGame->GetHound()->m_Pos.y-3200;
			CatchTheEnemy( enLeft );
		}
		else if( m_stGunMode.m_sLeft < 60 )
		{
			m_pArm[enLeft]->GattlingEnable( gxTrue );
		}
		else
		{
			m_pArm[enLeft]->GattlingEnable( gxFalse );
		}
	}

	if( m_stGunMode.m_sRight >= 0 )
	{
		m_stGunMode.m_sRight ++;
		m_stGunMode.m_sRight = m_stGunMode.m_sRight%120;

		m_sControl |= BTN_R1;

		if( m_stGunMode.m_sRight < 40 )
		{
			//照準を合わせる
			m_TgtPos.x = pGame->GetHound()->m_Pos.x;
			m_TgtPos.y = pGame->GetHound()->m_Pos.y-3200;
			CatchTheEnemy( enRight );
		}
		else if( m_stGunMode.m_sRight < 60 )
		{
			m_pArm[enRight]->GattlingEnable( gxTrue );
		}
		else
		{
			m_pArm[enRight]->GattlingEnable( gxFalse );
		}
	}

	m_stGunMode.m_sWait ++;
}


void CEneH0504Boss::LogicCatchMode()
{
	//--------------------------------------------
	// キャッチングモード（既に捕まえたあと）
	//--------------------------------------------
	gxBool bGacha = gxFalse;
	gxBool bSnap  = gxTrue;
	gxBool bPunch = gxFalse;

	m_bChumoku = gxTrue;

	switch( m_stHang.m_sSeq ){
	case 0:
		//初期化
		m_bChumoku = gxFalse;
		normalize(  m_pArm[enRight]->m_fRotArm  );
		normalize(  m_pArm[enRight]->m_fRotHand );
		normalize(  m_pArm[enLeft]->m_fRotArm   );
		normalize(  m_pArm[enLeft]->m_fRotHand  );
		normalize(  m_pBody->m_fRotation        );
		m_pArm[enRight]->SetCatchMode(gxFalse);
		m_pArm[enLeft ]->SetCatchMode(gxFalse);
		m_stHang.m_sGahaCounter = 0;
		m_stHang.m_sWait        = 0;
		m_stHang.m_sRotation    = 0;
		m_stHang.m_sDistance    = 0;
		m_stHang.m_sSeq         = 50;
		return;

	case 10:
		//捕まえモード
		m_bChumoku = gxFalse;
		m_TgtPos.x = pGame->GetHound()->m_Pos.x;
		m_TgtPos.y = pGame->GetHound()->m_Pos.y;
//		m_pArm[enRight]->SetCatchMode(gxTrue);
		m_pArm[enLeft ]->SetCatchMode(gxTrue);
		CatchTheEnemy(enRight);
		CatchTheEnemy(enLeft);
		if( m_pArm[enLeft]->IsCatch() )
		{
			viiMus::PlaySound( enSoundGachan );
			m_stHang.m_sWait        = 0;
			m_stHang.m_sSeq         = 100;
		}
		else if( m_stHang.m_sWait >= 60*5 )
		{
			m_stHang.m_sWait        = 0;
			m_stHang.m_sSeq         = 999;
		}
		m_stHang.m_sWait ++;
		return;

	case 50:
		bPunch = gxTrue;
		m_bChumoku = gxFalse;
		if( m_stHang.m_sWait < 5 )
		{
			m_stHang.m_sArmRot[enRight]  =  45;
			m_stHang.m_sHandRot[enRight] = -45;
			m_stHang.m_sArmRot[enLeft]   = (90+70);
			m_stHang.m_sHandRot[enLeft]  = -160;
			m_stHang.m_sBodyRot = -15;
			m_stHang.m_sBodyRot          = 0;
		}
		else if( m_stHang.m_sWait >= 50 )
		{
			m_TgtPos.x = pGame->GetHound()->m_Pos.x;
			m_TgtPos.y = pGame->GetHound()->m_Pos.y-6400;

			m_stHang.m_sBodyRot           = 0;

			CatchTheEnemy(enLeft , &m_stHang );

			m_stHang.m_sArmRot[enLeft]   = m_stHang.m_sArmRot[0];
			m_stHang.m_sHandRot[enLeft]  = m_stHang.m_sHandRot[0];
			m_stHang.m_sArmRot[enRight]   = 160;
			m_stHang.m_sHandRot[enRight]  = -160;
			m_stHang.m_sBodyRot = -15;//m_stHang.m_sArmRot[enLeft];

			m_stHang.m_sWait        = 0;
			m_stHang.m_sSeq         = 10;//60;
		}
		break;

	case 60:
		bPunch = gxTrue;
		m_bChumoku = gxFalse;
		m_pArm[enLeft ]->SetCatchMode(gxTrue);
		if( m_pArm[enLeft]->IsCatch() )
		{
			viiMus::PlaySound( enSoundLanding );
			m_stHang.m_sWait        = 0;
			m_stHang.m_sSeq         = 100;
		}
		else if( m_stHang.m_sWait >= 45 )
		{
			m_stHang.m_sWait        = 0;
			m_stHang.m_sSeq         = 999;
		}
		break;

	case 100:
		bGacha = gxTrue;
		{
			Sint32 sDist = 120+m_stHang.m_sGahaCounter*2;
			m_TgtPos.x = m_Pos.x+viiMath::Cos(20)*sDist*100;
			m_TgtPos.y = m_Pos.y+viiMath::Sin(20)*sDist*100;
			m_pArm[enRight]->GattlingEnable(gxTrue);
		}
		if( m_stHang.m_sWait >= 60*5 )
		{
			//５秒以内に脱出できなければお仕置き
			viiMus::PlaySound( enSoundJump );
			m_stHang.m_sWait        = 0;
			m_stHang.m_sSeq         = 200;
			if( m_pArm[enRight]->IsDead() )
			{
				m_stHang.m_sSeq         = 250;
			}
		}
		break;

	case 200:
		//お仕置き開始
		{
			Sint32 sDist = 160;
			m_TgtPos.x = m_Pos.x+viiMath::Cos(0)*sDist*100;
			m_TgtPos.y = m_Pos.y+viiMath::Sin(0)*sDist*100;
		}
		m_sControl = BTN_R1;

		if( m_stHang.m_sWait >= 60*2 )
		{
			m_stHang.m_sWait        = 0;
			m_stHang.m_sSeq         = 300;
		}
		break;

	case 250:
		//レーザーお仕置き
		{
			Sint32 sDist = 160;
			m_TgtPos.x = m_Pos.x+viiMath::Cos(0)*sDist*100;
			m_TgtPos.y = m_Pos.y+viiMath::Sin(0)*sDist*100;
		}

		m_pBody->m_fRotation += (-15-m_pBody->m_fRotation)/10.f;
		m_pArm[enRight]->m_fRotArm  += ( 65 - m_pArm[enRight]->m_fRotArm ) / 5.f;
		m_pArm[enRight]->m_fRotHand += ( -90 - m_pArm[enRight]->m_fRotHand) / 5.f;

		LaserBeemAtack();

		if( m_stHang.m_sWait >= 60*2 )
		{
			m_stHang.m_sWait        = 0;
			m_stHang.m_sSeq         = 300;
		}
		break;

	case 300:
		//スローイング初期化
//		m_bChumoku = gxFalse;
		viiMus::PlaySound( enSoundJump );
		m_pArm[enRight]->GattlingEnable(gxFalse);
		m_stHang.m_sRotation    = 0;
		m_stHang.m_sDistance    = 160;
		m_TgtPos.x = m_Pos.x+viiMath::Cos( m_stHang.m_sRotation )*m_stHang.m_sDistance*100;
		m_TgtPos.y = m_Pos.y+viiMath::Sin( m_stHang.m_sRotation )*m_stHang.m_sDistance*100;
		m_stHang.m_sWait        = 0;
		m_stHang.m_sSeq         = 310;
		break;

	case 310:
		//スローイング（振りかぶり）
//		m_bChumoku = gxFalse;
		if( m_stHang.m_sRotation > -120 )
		{
			m_TgtPos.x = m_Pos.x+viiMath::Cos( m_stHang.m_sRotation )*m_stHang.m_sDistance*100;
			m_TgtPos.y = m_Pos.y+viiMath::Sin( m_stHang.m_sRotation )*m_stHang.m_sDistance*100;
			m_stHang.m_sRotation --;
		}
		else
		{
			m_stHang.m_sWait        = 0;
			m_stHang.m_sSeq         = 320;
		}
		break;

	case 320:
		//スローイング（ため）
		m_bChumoku = gxFalse;
		if( m_stHang.m_sWait > 90 )
		{
			m_stHang.m_sWait        = 0;
			m_stHang.m_sSeq         = 330;
			viiMus::PlaySound( enSoundJump );
		}
		break;

	case 330:
		//スローイング（投げ）
		m_bChumoku = gxFalse;
		m_stHang.m_sRotation += (70-m_stHang.m_sRotation)/20;
		m_TgtPos.x = m_Pos.x+viiMath::Cos( m_stHang.m_sRotation )*m_stHang.m_sDistance*100;
		m_TgtPos.y = m_Pos.y+viiMath::Sin( m_stHang.m_sRotation )*m_stHang.m_sDistance*100;
		if( m_stHang.m_sWait > 25 )
		{
			//投げオブジェを初期化
			m_Through.x = pGame->GetHound()->m_Pos.x;
			m_Through.y = pGame->GetHound()->m_Pos.y;
			m_stHang.m_sWait        = 0;
			m_stHang.m_sSeq         = 340;
		}
		break;

	case 340:
		//スローイング（投げ中）
		{
			m_bChumoku = gxFalse;
			Sint32 sx = 1200, sy = 300;
			m_Through.x += sx;
			m_Through.y += sy;
			if( m_stHang.m_sWait > 25 || viiSub::IsGroundBlock( m_Through.x , m_Through.y) )
			{
				while( viiSub::IsGroundBlock( m_Through.x , m_Through.y) )
				{
					m_Through.y -= 100;
				}
				m_stHang.m_sWait        = 0;
				m_stHang.m_sSeq         = 999;
			}
			pGame->GetHound()->m_Pos.x = m_Through.x;
			pGame->GetHound()->m_Pos.y = m_Through.y;
			pGame->GetHound()->SetCarryOff();
			pGame->GetHound()->SetAdd( sx , sy );
			bSnap = gxFalse;
		}
		break;

	case 999:
		m_bChumoku = gxFalse;
		normalize(  m_pArm[enRight]->m_fRotArm  );
		normalize(  m_pArm[enRight]->m_fRotHand );
		normalize(  m_pArm[enLeft]->m_fRotArm   );
		normalize(  m_pArm[enLeft]->m_fRotHand  );
		normalize(  m_pBody->m_fRotation        );
		bSnap = gxFalse;
		m_bHanging = gxFalse;
		pGame->GetHound()->SetMode( CGunHound::enHoundModeNormal );
		return;
	}

/*
	//マウスカーソルを追いかけるテスト
	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );
	m_TgtPos.x = gxLib::Joy(0)->mx*100;
	m_TgtPos.y = gxLib::Joy(0)->my*100;
	m_TgtPos.x += sx*100;
	m_TgtPos.y += sy*100;
	//--------------------------------------------
	//	m_TgtPos.x = pGame->GetHound()->m_Pos.x;
	//	m_TgtPos.y = pGame->GetHound()->m_Pos.y;
*/

	if( bPunch )
	{
		m_pBody->m_fRotation += ( m_stHang.m_sBodyRot - m_pBody->m_fRotation ) / 5.f;
		m_pArm[enRight]->m_fRotArm  += ( m_stHang.m_sArmRot[enRight] - m_pArm[enRight]->m_fRotArm ) / 5.f;
		m_pArm[enRight]->m_fRotHand += ( m_stHang.m_sHandRot[enRight] - m_pArm[enRight]->m_fRotHand) / 5.f;

		m_pArm[enLeft]->m_fRotArm  += ( m_stHang.m_sArmRot[enLeft] - m_pArm[enLeft]->m_fRotArm ) / 5.f;
		m_pArm[enLeft]->m_fRotHand += ( m_stHang.m_sHandRot[enLeft] - m_pArm[enLeft]->m_fRotHand) / 5.f;
		m_stHang.m_sWait ++;
		return;
	}

	CatchTheEnemy(enRight);
	CatchTheEnemy(enLeft);

	//--------------------------------------------

	if( bSnap )
	{
		//吸着
	//	pGame->GetHound()->SetMode( CGunHound::enHoundModeNone );
	//	pGame->GetHound()->SetPadButton(0);
		pGame->GetHound()->m_Pos.x = m_pArm[enLeft]->m_Gun.dx;
		pGame->GetHound()->m_Pos.y = m_pArm[enLeft]->m_Gun.dy+3200;
	}

	if( bGacha && Joy[0].trg )
	{
		m_stHang.m_sGahaCounter ++;
		if( m_stHang.m_sGahaCounter >= 60 )
		{
			m_stHang.m_sSeq = 999;
			//m_bHanging = gxFalse;
		}
	}

	if( !m_bHanging )
	{
		//ハングタイム終了
		m_stHang.m_sSeq = 999;
	}

	m_stHang.m_sWait ++;
}


void CEneH0504Boss::LaserBeemAtack()
{
	//-----------------------------------------------------
	//レーザー攻撃
	//-----------------------------------------------------
	Sint32 ax,ay;

	ax = m_pBody->m_Eye.dx;
	ay = m_pBody->m_Eye.dy;

	if( m_sTimer%8==0)
	{
		ax += (-8+viiSub::Rand()%16)*100;
		ay += (-8+viiSub::Rand()%16)*100;

		CEne0001EneCannon *p;
		p = new CEne0001EneCannon( ax , ay , m_pBody->m_fRotation+25 , 1640 );
		p->SetSound(gxFalse);
		p->SetScale( 6.2f );
	}
}


void CEneH0504Boss::CatchTheEnemy(Sint32 n , StHang* pHang )
{
	//-----------------------------------------------------
	//捕まえる
	//-----------------------------------------------------
	ghPos tgtPos = m_TgtPos;
	ghPos *pPos;
	Sint32 x1,y1,r1;
	Sint32 x2,y2,r2;
	Sint32 nx,ny;

	x1 = m_pArm[n]->m_Arm.sx/100;
	y1 = m_pArm[n]->m_Arm.sy/100;
	r1 = m_pArm[n]->m_Arm.fLength;

	x2 = tgtPos.x/100;
	y2 = tgtPos.y/100;
	r2 = m_pArm[n]->m_Hand.fLength*2;

	nx = x2;
	ny = y2;

	CircleCircle *pCc = new CircleCircle();
	pCc->SetCircle(0,x1,y1,r1);
	pCc->SetCircle(1,x2,y2,r2);
	pCc->SetNear(nx,ny);

	if( pCc->Calc() )
	{
		//交点があった場合は近い方を選ぶ
		gxPos *pPos2a;
		ghPos pos;
		pPos = &pos;

		pPos2a  = pCc->GetKouten( pCc->GetNearest() );

		pPos->x = pPos2a->x;
		pPos->y = pPos2a->y;

		Float32 fr1,fr2;

		fr1 = viiMath::Atan2d( pPos->y-y1*1.f , pPos->x-x1*1.f );
		//fr1 = RAD2DEG( fr1 );

		fr2 = viiMath::Atan2d( tgtPos.y/100-pPos->y*1.f , tgtPos.x/100-pPos->x*1.f );
		//fr2 = RAD2DEG( fr2 );
		m_pArm[n]->SetArmKakudo ( fr1 );
		m_pArm[n]->SetHandKakudo( fr2 );

		if( pHang )
		{
			pHang->m_sArmRot[0]  = fr1;
			pHang->m_sHandRot[0] = fr2;
		}

	}
	else
	{
		//交点がなかった場合は最大限腕を伸ばす
		Float32 fr;
		fr = viiMath::Atan2d( tgtPos.y/100-y1*1.f , tgtPos.x/100-x1*1.f );
		//fr = RAD2DEG( fr );

		m_pArm[n]->SetArmKakudo ( fr );
		m_pArm[n]->SetHandKakudo( fr );

		if( pHang )
		{
			pHang->m_sArmRot[0]  = fr;
			pHang->m_sHandRot[0] = fr;
		}

	}

	delete pCc;

}


gxBool CEneH0504Boss::IsCatchArea()
{
	//-----------------------------------------------------
	//キャッチエリアに入っているか？
	//-----------------------------------------------------
	Sint32 vx,vy;

	Sint32 x0 = m_Pos.x/100;
	Sint32 y0 = m_Pos.y/100-64;
	Sint32 x1 = x0+350;
	Sint32 y1 = y0+192;
	Sint32 col = 0x80ff0000;

	vx = pGame->GetHound()->m_Pos.x/100;
	vy = pGame->GetHound()->m_Pos.y/100;
	if( vx > x0 && vx < x1 && vy > y0 && vy < y1 )
	{
		col = 0x80ffff00;
		return gxTrue;
	}

#ifdef _VII_DEBUG_
	viiDbg::printf(100,50,"WII POINT(%d)",m_sTargettingCnt );
	viiDraw::BoxScrl_s( x0,y0,x1,y1,PRIO_DEBUG,gxTrue,col,ATR_DFLT);
#endif

	return gxFalse;
}


Float32 CEneH0504Boss::normalize(Float32 fr)
{
	//-------------------------------------------
	//値のノーマライズ
	//-------------------------------------------

	while(fr < -180)
	{
		fr += 360; 
	}
	
	while(fr > 360)
	{
		fr -= 360; 
	}

	return fr;
}


void CEneH0504Boss::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	gxBool bFinalExplosion = gxFalse;
	gxBool bParts = gxFalse;
	Sint32 ax,ay;

	m_bCrash = gxTrue;
	m_bChumoku = gxTrue;

	m_pArm[enRight]->OpenShield();
	m_pArm[enLeft]->OpenShield();

	switch(m_sCrashSeq){
	case 0:
		m_sCrashArm[enRight] = 0;
		m_sCrashArm[enLeft ] = 0;
		m_sCrashCannopy      = 0;
		m_sCrashWait = 0;
		m_sCrashSeq = 300;
		pGame->QuakeOn(32);
		bParts = gxTrue;
		viiMus::PlaySound( enSoundExplosionLarge );
		break;
	case 300:
		CEffectBlackFire *p;
		if( viiSub::Rand()%24==0 )
		{
			switch( viiSub::Rand()%4 ){
			case 0:
				m_sCrashArm[enRight ] = 32;
				ax = m_pArm[enRight]->m_Arm.sx;
				ay = m_pArm[enRight]->m_Arm.sy;
				m_pArm[enRight]->SetOffProtecter();
				viiMus::PlaySound( enSoundExplosionMiddle );
				break;
			case 1:
				m_sCrashArm[enLeft ] = 36;
				ax = m_pArm[enLeft]->m_Hand.sx;
				ay = m_pArm[enLeft]->m_Hand.sy;
				m_pArm[enLeft]->SetOffProtecter();
				viiMus::PlaySound( enSoundExplosionMiddle );
				break;
			case 2:
				ax = m_pPack->m_Rear.dx;
				ay = m_pPack->m_Rear.dx;
				viiMus::PlaySound( enSoundExplosionSmall );
				break;
			case 3:
				ax = m_pArm[enRight]->m_Hand.sx;
				ay = m_pArm[enRight]->m_Hand.sy;
				viiMus::PlaySound( enSoundExplosionSmall );
				break;
			}
			EffectMakeBigBomb( ax,ay );
			pGame->QuakeOn(32);
			new CEffectDmgprt( ax, ay , 180+15*1 , 600 );
			new CEffectDmgprt( ax, ay , 180+45*1 , 800 );
			new CEffectDmgprt( ax, ay , 180+135*1 ,600 );
			new CEffectDmgprt( ax, ay , 180+165 , 600 );
			viiEff::SetBombDeluxe( ax , ay , 8 , 1+viiSub::Rand()%2 );
		}

		if( viiSub::Rand()%32==0 )
		{
			m_pBody->m_fRotation -= 0.1f;
			m_sCrashCannopy = 3;

			ax = m_pPack->m_Rear.dx;
			ay = m_pPack->m_Rear.dy;
			viiEff::SetBombDeluxe( ax+(-2+viiSub::Rand()%4)*100 , ay+(-2+viiSub::Rand()%4)*100 , 2 , 1+viiSub::Rand()%2 );

			p = new CEffectBlackFire( m_Pos.x+(-16+viiSub::Rand()%24)*800 , m_Pos.y+(-16+viiSub::Rand()%16)*800 , NULL );

			p = new CEffectBlackFire( m_pArm[enRight]->m_Hand.sx , m_pArm[enRight]->m_Hand.sy , NULL );
			p->m_Pos.z = enPrioBossEffect;

			p = new CEffectBlackFire( m_pArm[enRight]->m_Arm.sx , m_pArm[enRight]->m_Arm.sy , NULL );
			p->m_Pos.z = enPrioBossEffect;

		}

		if( viiSub::Rand()%16==0 )
		{
			p = new CEffectBlackFire( m_pArm[enRight]->m_Shield.dx , m_pArm[enRight]->m_Shield.dy , NULL );
			p->m_Pos.z = enPrioBossEffect;

			p = new CEffectBlackFire( m_pArm[enLeft]->m_Arm.sx , m_pArm[enLeft]->m_Arm.sy , NULL );
			p->m_Pos.z = enPrioBossEffect;

			p = new CEffectBlackFire( m_pPack->m_Rear.dx , m_pPack->m_Rear.dy , NULL );
			p->m_Pos.z = enPrioBossEffect;

			p = new CEffectBlackFire( m_pArm[enLeft]->m_Shield.dx , m_pArm[enLeft]->m_Shield.dy , NULL );
			p->m_Pos.z = enPrioBossEffect;
		}

		if(m_sCrashWait>=240*2)
		{
			viiMus::PlaySound( enSoundExplosionMaximum );
			viiMus::PlaySound( enSoundCrashB );
			m_sCrashWait = 0;
			m_sCrashSeq = 500;
		}
		break;

	case 500:
		new CEffectBlackFire( m_Pos.x+(-8+viiSub::Rand()%16)*100 , m_Pos.y+(-32+viiSub::Rand()%16)*100 , NULL );
		if( m_sCrashWait%24 == 0)
		{
			bParts = gxTrue;
		}
		switch( m_sCrashWait ){
		case 60:
			bFinalExplosion = gxTrue;
			ax = m_Pos.x-0;
			ay = m_Pos.y-3200;
			viiMus::PlaySound( enSoundExplosionSmall );
			new CEffectFinalExplosion( ax , ay ,4 );
			break;
		case 100:
			viiMus::PlaySound( enSoundExplosionMiddle );
			bFinalExplosion = gxTrue;
			ax = m_Pos.x-3200;
			ay = m_Pos.y+3200*2;
			new CEffectFinalExplosion( ax , ay ,4 );
			break;
		case 140:
			viiMus::PlaySound( enSoundExplosionSmall );
			bFinalExplosion = gxTrue;
			ax = m_Pos.x+3200;
			ay = m_Pos.y+3200*2;
			new CEffectFinalExplosion( ax , ay ,4 );
			break;
		case 180:
			bFinalExplosion = gxTrue;
			ax = m_Pos.x;
			ay = m_Pos.y;
			CFadeManager::GetInstance()->set_fadeout(1,0xffffffff);
			new CEffectFinalExplosion( ax , ay ,32 );
			break;
		case 300:
			viiMus::PlaySound( enSoundCrashA );
			m_sCrashSeq = 900;
			break;
		}
		break;

	case 900:
		m_bDead = gxTrue;
		bParts = gxTrue;
		viiMus::PlaySound(enSoundExplosionMiddle);
		ScorePlus( enScore );
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
		m_sCrashSeq = 999;
		break;

	case 999:
		if( m_bClear )
		{
			SetActionSeq( enActionSeqEnd );
		}
		break;
	}

	if( m_sCrashCannopy > 0 )
	{
		m_sCrashCannopy --;
		m_pPack->CannopyOpen();
	}

	if( m_sCrashArm[enRight] > 0 )
	{
		m_sCrashArm[enRight] --;
		m_pArm[enRight]->m_fRotArm  += (-160-m_pArm[enRight]->m_fRotArm)/100.f;
		m_pArm[enRight]->m_fRotHand += (45-m_pArm[enRight]->m_fRotHand)/100.f;
	}

	if( m_sCrashArm[enLeft ] > 0 )
	{
		m_sCrashArm[enLeft ] --;
		m_pArm[enLeft]->m_fRotArm  += (0-m_pArm[enLeft]->m_fRotArm)/100.f;
		m_pArm[enLeft]->m_fRotHand += (45-m_pArm[enLeft]->m_fRotHand)/100.f;
	}

	if( bParts )
	{
		Sint32 sx,sy;
		sx = (-96+viiSub::Rand()%128)*100;
		sy = (-32+viiSub::Rand()%128)*100;

		pGame->QuakeOn(32);
		new CEffectDmgprt( m_Pos.x+sx, m_Pos.y+sy , 180+15*1 , 600 );
		new CEffectDmgprt( m_Pos.x+sx, m_Pos.y+sy , 180+45*1 , 800 );
		new CEffectDmgprt( m_Pos.x+sx, m_Pos.y+sy , 180+135*1 ,600 );
		new CEffectDmgprt( m_Pos.x+sx, m_Pos.y+sy , 180+165 , 600 );
	}

	m_Pos.x += (m_sTimer%4 < 2)? -300 : +300 ;

//	if( bFinalExplosion )
//	{
//		new CEffectFinalExplosion( ax , ay ,32 );
//	}

	Gimmick();

	m_pBody->m_Pos.x = m_Pos.x;
	m_pBody->m_Pos.y = m_Pos.y;

	m_pBody->Action();

	m_pArm[enRight]->m_Pos.x = m_pBody->GetShoulder(enRight)->dx;
	m_pArm[enRight]->m_Pos.y = m_pBody->GetShoulder(enRight)->dy;
	m_pArm[enRight]->Action();

	m_pArm[enLeft]->m_Pos.x = m_pBody->GetShoulder(enLeft)->dx;
	m_pArm[enLeft]->m_Pos.y = m_pBody->GetShoulder(enLeft)->dy;
	m_pArm[enLeft]->Action();

	m_pPack->m_Pos.x = m_pBody->GetBack()->dx;
	m_pPack->m_Pos.y = m_pBody->GetBack()->dy;
	m_pPack->m_Pos.r = m_pBody->m_fRotation;
	m_pPack->Action();

	m_sCrashWait ++;
}


void CEneH0504Boss::Draw()
{
	//-----------------------------------------------------
	//描画
	//-----------------------------------------------------

	viiDraw::Sprdmg( m_HitKurai.is_damage() , &SprCEneH0504Boss[ 0 ] , m_Pos.x-1600 , m_Pos.y+0,enPrioBossMiddle , m_sAtribute|m_sAtributeDir  , 0xffc08080/*enBOSS_COLOR*/ ,1.55f , -20);
	m_pBody->Draw();
	m_pArm[enRight]->Draw();
	m_pArm[enLeft]->Draw();
	m_pPack->Draw();

}

