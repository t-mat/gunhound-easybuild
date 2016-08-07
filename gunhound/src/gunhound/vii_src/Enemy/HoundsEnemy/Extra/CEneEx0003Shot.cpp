//--------------------------------------------------------------------------------
//
// Stage00:対空ショット(ver.2)
//
//--------------------------------------------------------------------------------

#include "../HoundsEnemy.h"
#include "../../../Effect/EffectBase.h"
//#include "../EnemyTbl/CEneEx0003ShotTBL.h"
#include "../../../Stage/CStageBase.h"

enum {
	enScrollOutWidth  = 256*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -16,
	enKuraiTop   = -16,
	enKuraiRight =  16,
	enKuraiBottom=  0,

	enScore = 300,
};

gxSprite SprCEneEx0003Shot[]=
{
	{ enTexCEneEx0003ShotRight , 0,0,32,32,16,32},
	{ enTexCEneEx0003ShotRight , 32,0,32,32,16,32},
};

CEneEx0003Shot::CEneEx0003Shot( Sint32 x, Sint32 y ,Sint32 sDir)
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y+800;

	m_HitKurai.set_hp( enHitPointHigh );

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

	m_sBlackSmokeWait = 0;
}


CEneEx0003Shot::~CEneEx0003Shot()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneEx0003Shot::SeqMain()
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

	m_bDamage = m_HitKurai.is_damage();

	//くらい判定登録
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );
//	m_HitShield[0].SetHantei(ID_ENEMY_DEF , &m_Pos );
//	m_HitShield[1].SetHantei(ID_ENEMY_DEF , &m_Pos );

}


void CEneEx0003Shot::LogicAI()
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

	if(viiSub::GameCounter()%4==0 && viiSub::GameCounter()%(8*6)< (8*4))
	{
		Sint32 ax,ay;
		ax = m_Pos.x+(20*100)*m_sDirection;//+viiMath::Cos100( m_sRotation )*24;
		ay = m_Pos.y-28*100;//+viiMath::Sin100( m_sRotation )*24;

		viiEff::Set( EFF_MAZZLE , ax , ay , NULL );
		//new CEne0001EneMissile(   ax , ay , m_sRotation , 580 );
		new CEne0001EneBullet( ax , ay , viiSub::SetKakudo(m_sDirection,-30) , 980 ,1);
		m_sAtackWait = 32;
	}


}


void CEneEx0003Shot::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CEneEx0003Shot::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	if( !m_bDead )
	{
		viiMus::PlaySound(enSoundExplosionMiddle);
		ScorePlus( enScore );
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
		m_bDead = gxTrue;
		m_bDamage = gxFalse;

	}

	if( viiSub::Rand()%128 == 0 )
	{
		m_sBlackSmokeWait = 32;
	}
	if( m_sBlackSmokeWait > 0 )
	{
		if( m_sBlackSmokeWait%4==0)
		{
			CEffectBlackFire *pBlack;
			Sint32 ax,ay;
			ax = m_Pos.x+(-8+viiSub::Rand()%16)*100;
			ay = m_Pos.y+(-8+viiSub::Rand()%16)*100;

			pBlack = new CEffectBlackFire( ax , ay , NULL );
			pBlack->m_Pos.z = PRIO_BG2-1;
		}
		m_sBlackSmokeWait --;
	}

//	new CEffFire( m_Pos.x , m_Pos.y , PRIO_ENEMY-1);
//	SetActionSeq( enActionSeqEnd );

}

void CEneEx0003Shot::Draw()
{
	Sint32 yy = -CBaseExtra::GetOverLayerY()*50;

	if( m_bDead )
	{
		viiDraw::Sprdmg( m_bDamage , &SprCEneEx0003Shot[ 1 ] , m_Pos.x , m_Pos.y+yy,PRIO_ENEMY , m_sAtribute|m_sAtributeDir , ARGB_DFLT ,1.0f );//, 360-(180+45)+m_sRotation );
	}
	else
	{
		viiDraw::Sprdmg( m_bDamage , &SprCEneEx0003Shot[ 0 ] , m_Pos.x , m_Pos.y+yy,PRIO_ENEMY , m_sAtribute|m_sAtributeDir , ARGB_DFLT ,1.0f ,10);//, 360-(180+45)+m_sRotation );
	}
}

