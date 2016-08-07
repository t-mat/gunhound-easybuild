//--------------------------------------------------------------------------------
//
// Stage00:爆弾投射機(ver.2)
//
//--------------------------------------------------------------------------------

#include "../HoundsEnemy.h"
#include "../../../Effect/EffectBase.h"
//#include "../EnemyTbl/CEneEx0008BombTBL.h"

enum {
	enScrollOutWidth  = 256*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -16,
	enKuraiTop   = -16,
	enKuraiRight =  16,
	enKuraiBottom=  0,

	enScore = 300,
};

/*
gxSprite SprCEneEx0008Bomb[]=
{
	{2*16+0 , 200,0,32,32,16,24},
};
*/

CEneEx0008Bomb::CEneEx0008Bomb( Sint32 x, Sint32 y ,Sint32 sDir)
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y+800;

	m_HitKurai.set_hp( 80 );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	switch( sDir ){
	case 0:	//45
		m_sDirection = DIR_RIGHT;
		break;
	case 1:	//180+45
	default:
		m_sDirection = DIR_LEFT;
		break;
	}
	m_sAtribute = ATR_DFLT;
	m_sRotation = 180+45;


	m_bDamage = gxFalse;
	m_sAtackWait = 0;

}


CEneEx0008Bomb::~CEneEx0008Bomb()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneEx0008Bomb::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------
	m_HitKurai.ax1 = -20*m_sDirection*-1;
	m_HitKurai.ay1 = -20;
	m_HitKurai.ax2 =  20*m_sDirection*-1;
	m_HitKurai.ay2 =  -8;


	m_HitShield[0].ax1 = -36*m_sDirection*-1;
	m_HitShield[0].ay1 = -28;
	m_HitShield[0].ax2 = -28*m_sDirection*-1;
	m_HitShield[0].ay2 = 4;
	m_HitShield[0].set_hp( 80 );

	m_HitShield[1].ax1 = -36*m_sDirection*-1;
	m_HitShield[1].ay1 = -8;
	m_HitShield[1].ax2 = 16*m_sDirection*-1;
	m_HitShield[1].ay2 = 4;
	m_HitShield[1].set_hp( 80 );

	if( m_HitKurai.is_dead() )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}

	LogicAI();

	//くらい判定登録
//	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );
//	m_HitShield[0].SetHantei(ID_ENEMY_DEF , &m_Pos );
//	m_HitShield[1].SetHantei(ID_ENEMY_DEF , &m_Pos );

}


void CEneEx0008Bomb::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	if( m_sAtackWait > 0 )
	{
		m_sAtackWait --;
	}
	else
	{
		m_sRotation += (get_valken_rot( m_Pos.x , m_Pos.y )-m_sRotation)/10;
	}

	if(viiSub::GameCounter()%(32*6) <= 32*3 )
	{
		if(viiSub::GameCounter()%32==0)
		{
			Atack();
		}
	}

}


void CEneEx0008Bomb::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

		Sint32 ax,ay;
		ax = m_Pos.x+viiMath::Cos100( m_sRotation )*24;
		ay = m_Pos.y+viiMath::Sin100( m_sRotation )*24;

		viiEff::Set( EFF_MAZZLE , ax , ay , NULL );
		CAtkH0002GMissile *p;
		p = new CAtkH0002GMissile(ax,ay+800 , 0, 160 );
		p->SetForce( CAtkH0001Missile::enForceID_Enemy );
		m_sAtackWait = 32;
}


void CEneEx0008Bomb::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	viiMus::PlaySound(enSoundExplosionMiddle);

	ScorePlus( enScore );

	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

//	new CEffFire( m_Pos.x , m_Pos.y , PRIO_ENEMY-1);

	SetActionSeq( enActionSeqEnd );

}

void CEneEx0008Bomb::Draw()
{

//	viiDraw::Sprdmg( m_HitKurai.is_damage() , &SprCEneEx0008Bomb[ 0 ] , m_Pos.x , m_Pos.y,PRIO_ENEMY , m_sAtribute|m_sAtributeDir , ARGB_DFLT ,1.0f, 360-(180+45)+m_sRotation );

}

