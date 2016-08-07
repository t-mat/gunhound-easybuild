//--------------------------------------------------------------------------------
//
// Stage00:ヤグラ
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../Effect/EffectBase.h"
#include "EnemyTbl/CEneH0106YaguraTbl.h"


enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -32,
	enKuraiTop   = -120-16,
	enKuraiRight =  32,
	enKuraiBottom=  -120+24,

	enScore = 600,
};


CEneH0106Yagura::CEneH0106Yagura( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_HitKurai.set_hp( enAtkPointBazooka*3 );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_LEFT;
	m_sAtribute = ATR_DFLT;

	m_bDamage = gxFalse;

	m_pEnemyAnime = new CEnemyAnime();

	for(Sint32 ii=0; ii<enMax ;ii++)
	{
		m_pEnemyAnime->SetParts( ii , enMax , m_sParentCEneH0106Yagura[ii] , sPosCEneH0106Yagura[ii] );
	}

	m_pEnemyAnime->PosCalc();

	m_sRotation   = 0;
	m_sAtackTimer = 0;

}


CEneH0106Yagura::~CEneH0106Yagura()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

	delete m_pEnemyAnime;

}


void CEneH0106Yagura::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( m_HitKurai.is_dead() )
	{
		CEffFire *pFire;
		pFire = new CEffFire( m_Pos.x  , m_Pos.y + m_pEnemyAnime->GetParts( enGUN   )->dy*100 , PRIO_BG2 );
		pFire->SetScale(0.5f);

		CSoldierBase *p = new CSoldierBase( );
		p->SetPosition( 0 , m_Pos.x  , m_Pos.y + m_pEnemyAnime->GetParts( enGUN   )->dy*100 , NULL);
		p->SetForce(enForceEnemy);
		p->Command( enSoldierCommandFuttobi );

		p = new CSoldierBase( );
		p->SetPosition( 0 , m_Pos.x  , m_Pos.y + m_pEnemyAnime->GetParts( enGUN   )->dy*100 , NULL);
		p->SetForce(enForceEnemy);
		p->Command( enSoldierCommandFuttobi );

		pGame->QuakeOn(32);
		viiMus::PlaySound(enSoundExplosionMiddle);
		ScorePlus( enScore );

		EffectMakeBigBomb(m_Pos.x , m_Pos.y-120*100);
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y-120*100 , 1 , 1+viiSub::Rand()%2 );
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}

	LogicAI();

	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	m_bDamage = m_HitKurai.is_damage();

	//くらい判定登録
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

}


void CEneH0106Yagura::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	//狙い撃ち

	m_sAtackTimer ++;
	m_sAtackTimer = m_sAtackTimer%90;

	if( m_sAtackTimer < 60 )
	{
		Sint32 r = get_valken_rot( m_Pos.x , m_Pos.y - 96*100);

		r = r/30*30;

		if(r >90 && r<270)
		{
			r = 90-(r-90);
		}
		if(r >270)	r -= 360;

		if( m_sRotation < r)
		{
			m_sRotation += 3;
		}
		else if( m_sRotation > r)
		{
			m_sRotation -= 3;
		}
	}
	else if( m_sAtackTimer < 75 )
	{
		if( m_sTimer%4==0) Atack();
	}


	m_pEnemyAnime->SetPrio( PRIO_BG2+1 );
	m_pEnemyAnime->PosCalc();

	m_pEnemyAnime->GetParts( enGUN )->dr = m_sRotation;
	m_pEnemyAnime->GetParts( enGUNTATE )->dr = 0;
	

	if( m_bDamage )
	{
		m_sFireTimer = 4;
	}

	if( m_sFireTimer > 0 )
	{
		m_sFireTimer --;
		if(m_sTimer%2==0)
		{
			new CEffBombSmoke(m_Pos.x , m_Pos.y-120*100 , -90 , 120 );
		}
	}

}


void CEneH0106Yagura::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

	Sint32 sRot = m_pEnemyAnime->GetParts( enGUN )->dr;
	Sint32 x,y;

	m_pEnemyAnime->GetParts( enGUNNER )->sDispSprite = ((viiSub::GameCounter()%8)>4)? 4 : 5;

	x = m_pEnemyAnime->GetParts( enGUN )->dx*100;
	y = m_pEnemyAnime->GetParts( enGUN )->dy*100;

	if( m_sDirection == DIR_RIGHT)
	{
		x = m_pEnemyAnime->GetParts( enGUN )->dx*100+viiMath::Cos100(sRot)*29;
		y = m_pEnemyAnime->GetParts( enGUN )->dy*100+viiMath::Sin100(sRot)*29-400;

		viiEff::Set( EFF_MAZZLE , m_Pos.x+x*1 , m_Pos.y+y , NULL );
		new CEne0001EneBullet( m_Pos.x+x*1, m_Pos.y+y , sRot , 320 );

	}
	else if( m_sDirection == DIR_LEFT)
	{
		x = m_pEnemyAnime->GetParts( enGUN )->dx*100+viiMath::Cos100(sRot)*29;
		y = m_pEnemyAnime->GetParts( enGUN )->dy*100+viiMath::Sin100(sRot)*29-400;
		sRot = 180-sRot;

		viiEff::Set( EFF_MAZZLE , m_Pos.x+x*-1 , m_Pos.y+y , NULL );
		new CEne0001EneBullet( m_Pos.x+x*-1, m_Pos.y+y , sRot  , 320 );

	}

}


void CEneH0106Yagura::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	m_bDead = gxTrue;
	m_pEnemyAnime->GetParts( enGUNNER )->bDisp = gxFalse;
	m_bDamage = gxFalse;
	m_pEnemyAnime->GetParts( enGUN )->bDisp = gxFalse;

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}

	if( viiSub::Rand()%4==0 )
	{
		new CEffectBlackFire( m_Pos.x+(-8+viiSub::Rand()%16)*100 , m_Pos.y+(-8+viiSub::Rand()%16)*100 -100*100, NULL );
	}

//	SetActionSeq( enActionSeqEnd );

}

void CEneH0106Yagura::Draw()
{

//	viiDraw::Sprdmg( m_HitKurai.is_damage() , &SprCEneH0106Yagura[ 0 ] , m_Pos.x , m_Pos.y,PRIO_ENEMY , m_sAtribute|m_sAtributeDir , ARGB_DFLT );

	m_pEnemyAnime->Draw( m_bDamage , m_Pos.x , m_Pos.y , ( m_sDirection == DIR_LEFT )? gxTrue : gxFalse , &SprCEneH0106Yagura[0] );

}

