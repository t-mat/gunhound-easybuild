//--------------------------------------------------------------------------------
//
// Stage00:護衛用戦車（未使用）
//
//--------------------------------------------------------------------------------
#include "HoundsEnemy.h"
#include "../../Effect/EffectBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -32,
	enKuraiTop   = -64,
	enKuraiRight =  32,
	enKuraiBottom=  0,

	enScore = 300,
	enSpeed = 142,
};

gxSprite SprCEneH0105TankGuard[]=
{
	{enTexCEneH0105TankGuard,168,128+64*0,88,64,44,56},
	{enTexCEneH0105TankGuard,168,128+64*1,88,64,44,56},
};

CEneH0105TankGuard::CEneH0105TankGuard( Sint32 x, Sint32 y ,gxBool bDummy )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_sSprite = 0;
	m_sOffset = 0;

	m_HitKurai.set_hp( enAtkPointBazooka );

	m_Obj.pos.x = m_Pos.x = x;
	m_Obj.pos.y = m_Pos.y = y;

	m_pTgt = NULL;

	m_sPosition = 0;
	m_bDamage = gxFalse;
	m_bDummy  = bDummy;
	m_bAtackable  = !bDummy;
}


CEneH0105TankGuard::~CEneH0105TankGuard()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------


}


void CEneH0105TankGuard::SeqInit()
{
	//-----------------------------------------------------
	//初期化
	//-----------------------------------------------------

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	SetActionSeq( enActionSeqMain );
}


void CEneH0105TankGuard::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------
	m_sSprite = 0;

	if (m_HitKurai.is_dead())
	{
		viiEff::Set( EFF_RING , m_Pos.x , m_Pos.y , argset(500) );
		SetActionSeq( enActionSeqCrash );
		return;
	}

	LogicAI();

	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	Atack();

	m_Pos.x = m_Obj.pos.x;
	m_Pos.y = m_Obj.pos.y;

//	new CEffDashStorm( m_Pos.x+(-48+viiSub::Rand()%96)*100 , m_Obj.pos.y, 180 , 30 );

	if( m_bDummy )
	{
		m_HitKurai.set_hp( enAtkPointBazooka );
	}
	else
	{
		m_bDamage = m_HitKurai.is_damage();
		m_HitKurai.SetHantei( ID_VERSIS_DEF,&m_Pos );
	}
}


void CEneH0105TankGuard::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	Sint32 tx = 9600*100;//m_sTargetX;

	if( m_pTgt )
	{
		tx = m_pTgt->x;
		tx += m_sOffset;
	}

	if(m_Obj.pos.x < tx-3200)
	{
		m_Obj.mov.x += 1;
	}
	else if(m_Obj.pos.x > tx+3200)
	{
		m_Obj.mov.x -= 1;
	}
	else
	{
		if( m_Obj.mov.x > 0 )
		{
			m_Obj.mov.x -= 2;
			if( m_Obj.mov.x < 0 )
			{
				m_Obj.mov.x = 0;
			}
		}
		else if( m_Obj.mov.x < 0 )
		{
			m_Obj.mov.x += 2;
			if( m_Obj.mov.x > 0 )
			{
				m_Obj.mov.x = 0;
			}
		}
	}

	if(m_Obj.mov.x <= -enSpeed) m_Obj.mov.x = -enSpeed;
	if(m_Obj.mov.x >=  enSpeed) m_Obj.mov.x =  enSpeed;
	if(m_Obj.mov.y <= -enSpeed) m_Obj.mov.y = -enSpeed;
	if(m_Obj.mov.y >=  enSpeed) m_Obj.mov.y =  enSpeed;

//	m_Obj.mov.x = m_Add.x;
	m_Obj.mov.y = 120;

	if( m_Obj.easy_collision_check() )
	{
		m_Obj.mov.y = -120;
	}
	else
	{

	}

	m_Obj.pos.x += m_Obj.mov.x;
	m_Obj.pos.y += m_Obj.mov.y;

}


gxBool CEneH0105TankGuard::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------
	return gxFalse;

	Sint32 sPosTbl[]={
		24,-52,
		34,-48,
		24,-40,
		34,-38,
	};
	Sint32 ax,ay;

	if( viiSub::GameCounter()%(64*2) < 16*4)
	{
		if( viiSub::GameCounter()%4==0)
		{
			ax = sPosTbl[(m_sPosition/1)*2+0]*100;
			ay = sPosTbl[(m_sPosition/1)*2+1]*100;

			viiEff::Set( EFF_MAZZLE , m_Pos.x+ax , m_Pos.y+ay , NULL );

			if( m_bAtackable )
			{
				new CEffGattlingGun( m_Pos.x+ax , m_Pos.y+ay , -40+viiSub::Rand()%40 , 1050 );
			}
			m_sPosition ++;
			m_sPosition = m_sPosition%4;
		}
	}

	return gxFalse;
}


void CEneH0105TankGuard::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	ScorePlus( enScore );
	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 5 , 3 );

	EffectMakeBigBomb(m_Pos.x , m_Pos.y);

	CEffectDmgprt *p;
	p = new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, 180+15 , 800 );
	p = new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, 180+65 , 600 );
	p = new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, -60    , 800 );

	SetActionSeq( enActionSeqEnd );

}


void CEneH0105TankGuard::Draw()
{
	//---------------------------------------------
	//表示
	//---------------------------------------------
	return;

	m_Pos.z = PRIO_ENEMY;

	if( m_bDummy )
	{
		m_Pos.z = PRIO_BG1_5;
	}

	viiDraw::Sprdmg(
		m_bDamage ,
		&SprCEneH0105TankGuard[m_sSprite],
		m_Pos.x,
		m_Pos.y,
		m_Pos.z,
		ATR_DFLT,
		ARGB_DFLT);
}


