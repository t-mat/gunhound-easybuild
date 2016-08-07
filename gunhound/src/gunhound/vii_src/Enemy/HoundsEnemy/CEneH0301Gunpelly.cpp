//--------------------------------------------------------------------------------
//
// Stage00:輸送コウゲキヘリ
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../Effect/EffectBase.h"
enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -60,
	enKuraiTop   = -34,
	enKuraiRight =  60,
	enKuraiBottom=  0,

	enScore = 10000,
};

gxSprite SprCEneH0301Gunpelly[]=
{
	{ enTexCEneH0301Gunpelly,32,192,208,64,104,32},	//輸送戦闘ヘリ

	{ enTexCEneH0301Gunpelly,0,0*16,176,16,88,16},	//プロペラ
	{ enTexCEneH0301Gunpelly,0,1*16,176,16,88,16},	//プロペラ
	{ enTexCEneH0301Gunpelly,0,2*16,176,16,88,16},	//プロペラ
	{ enTexCEneH0301Gunpelly,0,3*16,176,16,88,16},	//プロペラ
	{ enTexCEneH0301Gunpelly,0,2*16,176,16,88,16},	//プロペラ
	{ enTexCEneH0301Gunpelly,0,1*16,176,16,88,16},	//プロペラ

};

CEneH0301Gunpelly::CEneH0301Gunpelly( Sint32 x, Sint32 y ,gxBool bAutoClear)
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_bAutoClear = bAutoClear;

	m_HitKurai.set_hp( enHitPointBossBombShip*2 );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_LEFT;
	m_sAtribute  = ATR_DFLT;

	m_Add.x = 0;
	m_Add.y = 0;

	m_sAtkRot = 0;

	m_sPropera = 0;

	m_sAtackWait = 0;
	m_sAtackCnt  = 0;

	m_sAtackPtn = 0;
	m_bDamage = gxFalse;

	m_pEffFire = NULL;
}


CEneH0301Gunpelly::~CEneH0301Gunpelly()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneH0301Gunpelly::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( m_HitKurai.is_dead() )
	{
		m_Add.x = -80*m_sDirection;
		m_Add.y = 0;
		m_pEffFire = new CEffFire( m_Pos.x , m_Pos.y-8*100 , PRIO_EFFECT , 300 );
		viiEff::Set( EFF_RING , m_Pos.x , m_Pos.y , argset(1000) );
		viiMus::PlaySound(enSoundExplosionMiddle);
		viiMus::PlaySound(enSoundExplosionLarge);

		new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+15*1  , 600 );
		new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+45*1  , 800 );
		new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+135*1 , 600 );
		new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+165   , 600 );

		ScorePlus( enScore );
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( !IsScrollOut( WINDOW_W*100 , enScrollOutHeight ) )
	{
		viiMus::GetInstance()->PlayLoopSound( enSoundHeliRoter , 108 );
	}

	m_sPropera = 1+(viiSub::GameCounter()%(2*6)/2);

	LogicAI();

	m_Pos.x += m_Add.x;

	SetRader( enTypeMarkerMiddleBoss );
	DirectionAdjust();

	m_bDamage = m_HitKurai.is_damage();

	//くらい判定登録
	m_HitKurai.SetHantei( ID_ENEMY_DEF , &m_Pos );

}


void CEneH0301Gunpelly::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	m_Add.x = 80*m_sDirection;

	new CEffectGroundSmoke( m_Pos.x+viiSub::Rand()%1600 , m_Pos.y, m_sDirection*-1 );
	Atack();

	Syasyutu();
}


void CEneH0301Gunpelly::Syasyutu()
{
	if( viiSub::GameCounter()%120 == 0 )
	{
		new CEneH0107Walker( m_Pos.x , m_Pos.y );
	}
}


void CEneH0301Gunpelly::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------
	Sint32 sKaku = 30;
	Sint32 sDist = 25;
	Sint32 ax,ay;


	sDist = 15;

	ax = m_Pos.x-3200;
	ay = m_Pos.y;

	Sint32 r = get_valken_rot( ax , ay+3200);

	if( m_sAtackWait > 0 )
	{
		m_sAtackWait --;
		if( m_sAtackWait == 0 )
		{
			m_sAtackCnt = 0;			
			m_sAtkRot = r;
		}
		return;
	}

	if( m_bAutoClear )
	{
		if( m_sTimer%72 == 0 )
		{
			CEne0001EneSuperBall *q;

			q = new CEne0001EneSuperBall( m_Pos.x+16*100 , m_Pos.y , 90, 100  );
			q->SetPattern( CEne0001EneSuperBall::enPatternDrop );
			q->SetDropGensui(gxTrue,200);
			q->SetScale( 0.75f );
		}
		return;
	}

	if( viiSub::GameCounter()%4  < 2) return;

	sKaku = m_sAtkRot;

	ax += viiMath::Cos100( sKaku )*sDist;
	ay += viiMath::Sin100( sKaku )*sDist;

	viiEff::Set( EFF_MAZZLE , ax , ay , NULL );

	if( IsScrollOut( enScrollOutWidth*2 , enScrollOutHeight ) )
	{
		return;
	}

	{
		if( m_sAtackPtn%2 )
		{
	//		new CEne0001EneBullet( ax , ay , 90+45+20 , 320+m_sAtackCnt*30 , 1);
	//		new CEne0001EneBullet( ax , ay , sKaku-20 , 320+m_sAtackCnt*30 , 0);
			new CEne0001EneBullet( ax , ay , sKaku+20 , 320+m_sAtackCnt*30 , 0);
			new CEne0001EneBullet( ax , ay , 90+45-20 , 320+m_sAtackCnt*30 , 1);
		}

		if( m_sTimer%12 == 0 )
		{
			CEne0001EneSuperBall *q;

			q = new CEne0001EneSuperBall( m_Pos.x , m_Pos.y , 45, 120  );
			q->SetPattern( CEne0001EneSuperBall::enPatternDrop );
			q->SetScale( 0.75f );

			q = new CEne0001EneSuperBall( m_Pos.x , m_Pos.y , 65, 120  );
			q->SetPattern( CEne0001EneSuperBall::enPatternDrop );
			q->SetScale( 0.75f );
		}
	}

	m_sAtackCnt ++;

	if( m_sAtackCnt >= 12 )
	{
		m_sAtackWait = 24;
		m_sAtackPtn ++;
	}
}


void CEneH0301Gunpelly::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	m_sPropera = 1+(viiSub::GameCounter()%(3*6)/3);

	m_bDamage = gxFalse;

	//落下
	if( m_Add.y <= 300 ) m_Add.y += 2;

	//減速
	if( m_Add.x > 40*m_sDirection ) m_Add.x--;

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	if(m_sTimer%2==0)
	{
		new CEffBombSmoke(m_Pos.x+(viiSub::Rand()%32)*100 , m_Pos.y+(viiSub::Rand()%8)*100 , -90 , 120  );
	}

	//-------------------------------------------
	//激突
	//-------------------------------------------
	if( !m_bDead && viiSub::IsGroundBlock( m_Pos.x , m_Pos.y ) )
	{
		viiMus::PlaySound( enSoundCrashA );
		pGame->QuakeOn(16);

		CEffectDmgprt *p;
		p = new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, 180+15 , 300 );
		p->SetSprite( &SprCEneH0301Gunpelly[ 1 ] );

		p=new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, 180+65 , 400 );
		p->SetSprite( &SprCEneH0301Gunpelly[ 2 ] );

		EffectMakeBigBomb(m_Pos.x , m_Pos.y);
		EffectMakeBigBomb(m_Pos.x+3200 , m_Pos.y);
		EffectMakeBigBomb(m_Pos.x-3200 , m_Pos.y);

		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 3 , 1+viiSub::Rand()%5 );
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 3 , 1+viiSub::Rand()%5 );
		m_bDead = gxTrue;
	}

	if( m_pEffFire )
	{
		m_pEffFire->m_Pos.x = m_Pos.x;
		m_pEffFire->m_Pos.y = m_Pos.y;
		m_pEffFire->m_Pos.z = PRIO_ENEMY;
	}

	if( m_bDead )
	{
		if( m_bClear || m_bAutoClear)
		{
			SetActionSeq( enActionSeqEnd );
		}
	}

}

void CEneH0301Gunpelly::Draw()
{
	if( m_bDead ) return;

	viiDraw::Sprdmg( m_bDamage , &SprCEneH0301Gunpelly[ 0 ] , m_Pos.x , m_Pos.y , PRIO_ENEMY+1 , m_sAtributeDir , ARGB_DFLT );

	viiDraw::Sprdmg( m_bDamage , &SprCEneH0301Gunpelly[ m_sPropera ] , m_Pos.x-2600 , m_Pos.y-16*100 , PRIO_ENEMY+1 , m_sAtributeDir , ARGB_DFLT );
	viiDraw::Sprdmg( m_bDamage , &SprCEneH0301Gunpelly[ m_sPropera ] , m_Pos.x+6400 , m_Pos.y-24*100 , PRIO_ENEMY+1 , m_sAtributeDir , ARGB_DFLT );

}

