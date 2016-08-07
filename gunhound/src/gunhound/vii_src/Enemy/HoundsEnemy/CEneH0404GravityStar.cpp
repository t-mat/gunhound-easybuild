//--------------------------------------------------------------------------------
//
// Stage00:装甲列車
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../effect/EffectBase.h"

enum {
	enScrollOutWidth  = 256*100,
	enScrollOutHeight = 256*100,

	enKuraiLeft  = -80,
	enKuraiTop   = -72,
	enKuraiRight =  80,
	enKuraiBottom=  72,

	enScore = 80000,
};


gxSprite SprCEneH0404GravityStar[]=
{
	{enTexCEneH0404GravityStar,0,0,256,192,128,96 },
};


CEneH0404GravityStar::CEneH0404GravityStar( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_HitKurai.set_hp( enHitPointBossOrganiz );
#ifdef _VII_DEBUG_
	m_HitKurai.set_hp( 1 );
#endif
	m_HitAtari.set_ap( enHitPointHigh );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight-24;
	m_HitKurai.ay2 = enKuraiBottom-16;

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;

	m_sDirection = DIR_RIGHT;

	m_Base.x = m_Pos.x;
	m_Base.y = m_Pos.y;

	m_Add.x = 0;
	m_Add.y = 0;

	m_sSeq      = 0;
	m_sHigh     = 0;
	m_sRotation = 0;
	m_bDamage   = gxFalse;
	m_sDist = 64*100;
	m_sCrashTimer = 0;

	m_sFannelRotation = 0;

	m_bDispScreen = gxTrue;
}


CEneH0404GravityStar::~CEneH0404GravityStar()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}

void CEneH0404GravityStar::SeqInit()
{
	//-----------------------------------------------------
	//待機
	//-----------------------------------------------------
	SetRader( enTypeMarkerMiddleBoss );

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}

	for(Sint32 ii=0;ii<enFannelMax; ii++)
	{
		m_pFannel[ii] = new CEneH0413Fannel( m_Pos.x , m_Pos.y , ii*45 );
		m_pFannel[ii]->SetParent( &m_Pos , ii*45 );
	}

	SetActionSeq( enActionSeqMain );
}


void CEneH0404GravityStar::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

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

	LogicAI();

	SetRader( enTypeMarkerMiddleBoss );
	DirectionAdjust();

	//くらい判定登録
	m_bDamage = m_HitKurai.is_damage();

	m_HitKurai.SetHantei( ID_ENEMY_DEF , &m_Pos );

}


void CEneH0404GravityStar::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	//-------------------------------------------------
	//ファンネル制御
	//-------------------------------------------------

	m_sFannelRotation ++;
	m_sFannelRotation = m_sFannelRotation%360;

	for(Sint32 ii=0;ii<enFannelMax; ii++)
	{
		if( m_sTimer%600 < 300 )
		{
			m_pFannel[ii]->SetParent( &m_Pos , m_sFannelRotation+ii*90 );
		}
		else
		{
			m_pFannel[ii]->SetParent( &m_Pos , -(m_sFannelRotation+ii*90) );
		}
	}

	//-------------------------------------------------

	m_Pos.x = m_Base.x + viiMath::Cos100( (m_sTimer*2)%360 )*(m_sDist/100);
	m_Pos.y = m_Base.y + viiMath::Sin100( (m_sTimer*2)%360 )*(m_sDist/100);

	if( m_sTimer%24 == 0 && (m_sTimer%(24*10)< (24*8)) )
	{
		Atack();
	}

	m_sHigh = 0;//( viiMath::Cos100( (m_sTimer*8)%360) );

}


void CEneH0404GravityStar::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------
	Sint32 ax,ay;

	ax = viiMath::Cos100( (m_sTimer*8)%360 )*32;
	ay = viiMath::Sin100( (m_sTimer*8)%360 )*32;

	CEne0001EneSuperBall *p;
	p = new CEne0001EneSuperBall( m_Pos.x+ax , m_Pos.y+ay , 680*100 , 0 );
	p->SetPattern( CEne0001EneSuperBall::enPatternTargetting );

}


void CEneH0404GravityStar::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	switch( m_sSeq ){
	case 0:
		//初期化
		m_bDead = gxTrue;
		m_sCrashTimer = 0;
		viiMus::PlaySound(enSoundExplosionMaximum);
		viiMus::PlaySound( enSoundCrashB );
		m_sSeq = 5;
		break;

	case 5:
		//------------------------------------------
		//中心に位置をあわせる
		//------------------------------------------
		m_bDamage = m_HitKurai.is_damage();
		m_HitKurai.set_hp( enHitPointBossOrganiz );
		m_HitKurai.SetHantei( ID_ENEMY_DEF , &m_Pos );
		if( m_sDist > 0 )
		{
			m_sDist -= 40;
		}
		else
		{
			EffectMakeBigBomb(m_Pos.x , m_Pos.y);
			new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+15*1 , 600 );
			new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+45*1 , 800 );
			new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+135*1 ,600 );
			new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+165 , 600 );
			m_sCrashTimer = 0;
			m_sDist = 0;
			m_sSeq = 100;
		}

		if( m_sTimer%4==0) viiEff::SetBombSimple( m_Pos.x + (-64+viiSub::Rand()%128)*100, m_Pos.y + (-64+viiSub::Rand()%128)*100 , 1 , 1+viiSub::Rand()%2 );
		if( m_sTimer%15==0) viiEff::SetBombDeluxe( m_Pos.x + (-64+viiSub::Rand()%128)*100, m_Pos.y + (-64+viiSub::Rand()%128)*100 , 1 , 1+viiSub::Rand()%2 );

		m_Pos.x = m_Base.x + viiMath::Cos100( (m_sTimer*2)%360 )*(m_sDist/100);
		m_Pos.y = m_Base.y + viiMath::Sin100( (m_sTimer*2)%360 )*(m_sDist/100);
		m_sHigh = (viiSub::Rand()%4)*100;
		break;

	case 100:
		m_bDamage = gxFalse;
		m_sSeq = 200;
		m_sHigh = (viiSub::Rand()%4)*100;
		break;

	case 200:
		//------------------------------------------
		//落ちる
		//------------------------------------------
		if( m_sTimer%4==0) viiEff::SetBombSimple( m_Pos.x + (-64+viiSub::Rand()%128)*100, m_Pos.y + (-64+viiSub::Rand()%128)*100 , 1 , 1+viiSub::Rand()%2 );
		if( m_sTimer%15==0) viiEff::SetBombDeluxe( m_Pos.x + (-64+viiSub::Rand()%128)*100, m_Pos.y + (-64+viiSub::Rand()%128)*100 , 1 , 1+viiSub::Rand()%2 );
		m_sRotation += 30;
		if( m_Add.y < 800 )
		{
			m_Add.y += 4;
		}
		m_Pos.y += m_Add.y;
		if( viiSub::IsGroundBlock( m_Pos.x , m_Pos.y ) )
		{
			new CEffectFinalExplosion( m_Pos.x , m_Pos.y ,8 );
			m_sCrashTimer = 0;

			//ファンネルを破棄

			for(Sint32 ii=0;ii<enFannelMax; ii++)
			{
				m_pFannel[ii]->SetParent( NULL , ii*45 );
				m_pFannel[ii]->Clear();
//				m_pFannel[ii]->SetDead();
			}

			viiMus::PlaySound(enSoundExplosionMiddle);
			m_sSeq = 210;
		}
		break;

	case 210:
		//------------------------------------------
		//落ちきった
		//------------------------------------------
		pGame->QuakeOn(32);
		m_Pos.y += 15;
		if(	m_sCrashTimer == 30 )
		{
			new CEffectFinalExplosion( m_Pos.x , m_Pos.y ,8 );
		}
		if(	m_sCrashTimer == 100 )
		{
			viiMus::PlaySound( enSoundCrashA );
			viiMus::PlaySound(enSoundExplosionLarge);
			ScorePlus( enScore, 0 , -64);
			m_sSeq = 300;
		}
		break;

	case 300:
		if( m_bClear )
		{
			SetActionSeq( enActionSeqEnd );
		}
		break;
	}

	if( m_bClear )
	{
		m_bDispScreen = gxFalse;
	}

	m_sCrashTimer ++;

}


void CEneH0404GravityStar::Draw()
{
	if( !m_bDispScreen ) return;

	viiDraw::Sprdmg( m_bDamage , &SprCEneH0404GravityStar[ 0 ] , m_Pos.x , m_Pos.y+m_sHigh , PRIO_ENEMY , m_sAtributeDir , ARGB_DFLT ,1.0f , m_sRotation/100 );

}

