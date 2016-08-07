//--------------------------------------------------------------------------------
//
// Stage00:プラズママシーン
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../Effect/EffectBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -104,
	enKuraiTop   = -90,
	enKuraiRight =  104,
	enKuraiBottom=  90,

	enScore = 30000,
};


gxSprite SprCEneH0501PlasmaMachine[]=
{
	{ enTexCEneH0501PlasmaMachine , 0 , 0 , 248 , 208 , 124 , 104 },
};


CEneH0501PlasmaMachine::CEneH0501PlasmaMachine( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

#ifdef _VII_DEBUG_
	m_HitKurai.set_hp( 5 );
#else
	m_HitKurai.set_hp( 500 );
#endif
	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_LEFT;
	m_sAtribute = ATR_DFLT;

	m_sCrashSeq = 0;

	m_bDisp = gxTrue;
	m_bDamage = gxFalse;


	Sint32 x2=x+300*100,y2=y-300*100;
	for(Sint32 ii=0;ii<enPlasmaMax;ii++)
	{
		x2 = x+viiMath::Cos100(ii*40)*80;
		y2 = y+viiMath::Sin100(ii*40)*80;

	 	m_pCEneCmnSpark[ii] = new CEneCmnSpark(x,y,x2,y2);
	}
	m_bPlasmaDraw = gxTrue;
}


CEneH0501PlasmaMachine::~CEneH0501PlasmaMachine()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

	for(Sint32 ii=0;ii<enPlasmaMax;ii++)
	{
	 	if(m_pCEneCmnSpark[ii]) delete m_pCEneCmnSpark[ii];
	}

//	delete m_pEnemyAnime;
}


void CEneH0501PlasmaMachine::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------
	SetRader( enTypeMarkerMiddleBoss );

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

	DirectionAdjust();

	//くらい判定登録
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

}


void CEneH0501PlasmaMachine::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

}


void CEneH0501PlasmaMachine::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CEneH0501PlasmaMachine::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	switch(m_sCrashSeq){
	case 0:
		viiMus::PlaySound(enSoundExplosionMaximum);
		break;
	case 120:
		new CEffectFinalExplosion( m_Pos.x , m_Pos.y ,3 );
		break;
	case 150:
		new CEffectFinalExplosion( m_Pos.x , m_Pos.y ,3 );
		break;
	case 50:
		break;
	case 247:
		viiMus::PlaySound(enSoundExplosionMiddle);
		m_bPlasmaDraw = gxFalse;
		pGame->QuakeOn(32);
		new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+15*1 , 600 );
		new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+45*1 , 800 );
		new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+135*1 ,600 );
		new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+165 , 600 );
		m_bDisp = gxFalse;
		break;
	case 270:
		ScorePlus( enScore );
		break;

	case 420:
		m_bDead = gxTrue;
		break;
	default:
		break;
	}

	m_sCrashSeq ++;
	if( m_sCrashSeq < 230 )
	{
		if( viiSub::Rand()%3 == 0 )
		{
			Sint32 ax = -80+viiSub::Rand()%160;
			Sint32 ay = -80+viiSub::Rand()%160;
			viiEff::SetBombSimple( m_Pos.x+ax*100,m_Pos.y + ay*100 , 3 ,1 );
		}
		m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );
	}

	if( m_bClear )
	{
		SetActionSeq( enActionSeqEnd );
	}

}

void CEneH0501PlasmaMachine::Draw()
{
	if( m_bDisp )
	{
		viiDraw::Sprdmg( m_HitKurai.is_damage() , &SprCEneH0501PlasmaMachine[ 0 ] , m_Pos.x , m_Pos.y,PRIO_ENEMY , m_sAtribute|m_sAtributeDir , 0xA0FFFFFF );
	}

	if( m_bPlasmaDraw )
	{
		if(m_sTimer%8==0)
		{
			for(Sint32 jj=0;jj<enPlasmaMax;jj++)
			{
				Sint32 sKaku = (m_sTimer+jj*40)%360;
				Sint32 x2 = m_Pos.x+viiMath::Cos100(sKaku)*30;
				Sint32 y2 = m_Pos.y+viiMath::Sin100(sKaku)*30;

		 		m_pCEneCmnSpark[jj]->Set(m_Pos.x,m_Pos.y,x2,y2);
//				new CEffBombSmoke(m_Pos.x , m_Pos.y , sKaku , 280  );
			}
		}

		for(Sint32 ii=0;ii<enPlasmaMax;ii++)
		{
		 	m_pCEneCmnSpark[ii]->Draw();
		}
	} 

}

