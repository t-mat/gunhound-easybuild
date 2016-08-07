//--------------------------------------------------------------------------------
//
// Stage00:高速スパイダー
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "EnemyTbl/CEneH0403TaranturaTBL.h"
#include "../../effect/EffectBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -16,
	enKuraiTop   = -16,
	enKuraiRight =  16,
	enKuraiBottom=  16,

	enScore = 300,
	enSpeedMax = 600,
	enSpeedXMax = 200,
};

gxSprite SprCEneH0403SlashLine[]=
{
	{ enTexPageEffectCommon02,224,144,32,16,32,8},//ライン
};

CEneH0403Tarantura::CEneH0403Tarantura( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );

	m_Pos.x =  sx*100+x;
	m_Pos.y =  sy*100+y;

	m_Tgt.x = m_Pos.x;
	m_Tgt.y = m_Pos.y;

	m_Add.x = 0;
	m_Add.y = 0;

	m_Offset.x = 0;
	m_Offset.y = 0;

	m_Disp.x = m_Pos.x;
	m_Disp.y = m_Pos.y;

	m_sAtackTimer = 0;

	m_HitKurai.set_hp( enAtkPointBazooka );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_RIGHT;
	m_sAtribute = ATR_DFLT;

	m_sCrashWait = 0;
	m_sBaseLine = m_Pos.y;//160*100;
	m_sPositionIndex = 0;

	m_bDamage = gxFalse;
	m_pEnemyAnime = new CEnemyAnime();

	for(Sint32 ii=0; ii<enMax ;ii++)
	{
		m_pEnemyAnime->SetParts( ii , enMax , m_sParentCEneH0403Tarantura[ii] , sPosCEneH0403Tarantura[ii] );
	}

	m_pEnemyAnime->PosCalc();

}


CEneH0403Tarantura::~CEneH0403Tarantura()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

	delete m_pEnemyAnime;

}


void CEneH0403Tarantura::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( m_HitKurai.is_dead() )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	LogicAI();

	//ライト点滅
	//new CEffZanzo( m_Pos.x+1600,m_Pos.y );
	m_CEneCmnSignal.SetSignal( m_Disp.x+1600 , m_Disp.y , CEneCmnSignal::enSignalTypeNORMAL);

	SetRader( enTypeMarkerGroundObject );
	//DirectionAdjust();

	if(viiSub::GameCounter()%4==0)
	{
		//スラッシュラインを描画
		new CEneH0404SlashLine( m_Disp.x-1600 , m_Disp.y-1450 );
		new CEneH0404SlashLine( m_Disp.x-1600 , m_Disp.y+1450 );
	}

	m_bDamage = m_HitKurai.is_damage();

	//くらい判定登録
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Disp );

}


void CEneH0403Tarantura::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	Sint32 m_sControl = 0;//Joy[0].trg;
	Sint32 sOld = m_sPositionIndex;
	Sint32 hh = 40;
	m_pEnemyAnime->PosCalc();

	m_pEnemyAnime->GetParts( enMIGIASI   )->dr += 20+ ( viiMath::Cos100( (m_sTimer*8)%360 )* 20 )/100;
	m_pEnemyAnime->GetParts( enHIDARIASI )->dr += -20+( viiMath::Cos100( (m_sTimer*8)%360 )*-20 )/100;
	m_pEnemyAnime->GetParts( enMIGITE    )->dr +=     ( viiMath::Cos100( (m_sTimer*16)%360 )* 20 )/100;
	m_pEnemyAnime->GetParts( enHIDARITE  )->dr +=     ( viiMath::Cos100( (m_sTimer*16)%360 )*-20 )/100;

	if(viiSub::GameCounter()%40==0)
	{
		m_sPositionIndex = -2+viiSub::Rand()%4;
	}

	if( m_sControl&JOY_R )
	{
		m_Tgt.x = m_Pos.x+3200;
	}
	else if( m_sControl&JOY_L )
	{
		m_Tgt.x = m_Pos.x-3200;
	}

	if( m_sControl&JOY_U )
	{
		if( m_sPositionIndex > -2)
		{
			m_sPositionIndex --;
		}
	}
	else if( m_sControl&JOY_D )
	{
		if( m_sPositionIndex < 2)
		{
			m_sPositionIndex ++;
		}
	}

	if( m_sPositionIndex != sOld )
	{
		m_Tgt.y = m_sBaseLine+(m_sPositionIndex*hh)*100;
	}

	Float32 fHaba = hh*4;
	Float32 fNow   = (m_Pos.y-m_sBaseLine)/100.f;
	Float32 wari = 90*fNow / fHaba;
	Float32 xOffset = viiMath::Cos100( wari );

	m_Offset.x = (Sint32)xOffset*-120;
//	m_Offset.x = 0;

	Sint32 sSpdY=0;

	sSpdY = (m_Tgt.y-m_Pos.y)/10;

	if( sSpdY <= -enSpeedMax )
	{
		sSpdY = -enSpeedMax;
	}

	if( sSpdY >= enSpeedMax )
	{
		sSpdY = enSpeedMax;
	}

	m_Add.y = sSpdY;

	//------------------------------------------------

	if(m_Pos.x < m_Tgt.x)
	{
		if( m_Add.x < enSpeedXMax ) m_Add.x += 16;
	}
	else if(m_Pos.x > m_Tgt.x)
	{
		if( m_Add.x > -enSpeedXMax ) m_Add.x -= 16;
	}

	m_Pos.y += sSpdY;
	m_Pos.x += m_Add.x;

	m_Disp.x = (m_Pos.x+m_Offset.x);
	m_Disp.y = (m_Pos.y);

	if( m_sAtackTimer > 0 )
	{
		Atack();
		m_sAtackTimer--;
		if( m_sAtackTimer <80 )
		{
			m_CEneCmnSignal.On();
		}
	}
	else
	{
		m_CEneCmnSignal.Off();
		m_sAtackTimer = 180;
	}

	new CEffZanzo( m_Disp.x , m_Disp.y );

}


void CEneH0403Tarantura::SetTargetPos( Sint32 x , Sint32 y )
{
	//-----------------------------------------------------
	//進行可能な座標を設定する
	//-----------------------------------------------------

	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );

	m_Tgt.x =  sx*100+x;
//	m_Tgt.y =  sy*100+y;

}


void CEneH0403Tarantura::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------
	CEne0001EneSuperBall *p;
	Sint32 ax,ay;
	switch(m_sAtackTimer){
//	case 5:
//	case 10:
//	case 15:
//	case 20:
//	case 25:
//		new CEne0001EneSakretu( m_Pos.x , m_Pos.y , -30+m_sAtackTimer*2, 280 );
	case 20:
	case 80:
		viiMus::PlaySound(enSoundEneShotLaser);
		ax = viiMath::Cos100(-25)*8;
		ay = viiMath::Sin100(-25)*8;
		p = new CEne0001EneSuperBall( m_Disp.x+ax , m_Disp.y+ay , -25, 132 );
		p->SetPattern( CEne0001EneSuperBall::enPatternTargetting );
		p->SetScale( 0.5f);
		p->SetLevel( 10 );
		break;

	case 40:
	case 100:
		viiMus::PlaySound(enSoundEneShotLaser);
		ax = viiMath::Cos100(25)*8;
		ay = viiMath::Sin100(25)*8;
		p = new CEne0001EneSuperBall( m_Disp.x+ax , m_Disp.y+ay , 25, 132 );
		p->SetPattern( CEne0001EneSuperBall::enPatternHoming );
		p->SetScale( 0.5f);
		p->SetLevel( 10 );
		p->SetScrollOutRange(1);
		break;
	}
}


void CEneH0403Tarantura::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	m_bDamage = gxFalse;

	m_Pos.x += m_Add.x;
	m_Disp.x = (m_Pos.x+m_Offset.x);
	m_Disp.y = (m_Pos.y);

	if( m_sCrashWait == 0 )
	{
		viiEff::Set( EFF_RING , m_Disp.x , m_Disp.y , argset(500) );
		viiEff::SetBombDeluxe(  m_Disp.x , m_Disp.y , 1 , 1+viiSub::Rand()%2 );
		viiMus::PlaySound( enSoundExplosionMiddle );
		ScorePlus( enScore );
	}


	if( m_Add.x > -800) m_Add.x -= 12;

	if( m_sCrashWait%12 == 0 )
	{
		viiEff::SetBombEffect( m_Disp.x , m_Disp.y , 1 , 1+viiSub::Rand()%2 );
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		m_bDead = gxTrue;

		if( m_bClear )
		{
			SetActionSeq( enActionSeqEnd );
		}
	}
	else
	{

	}

	m_sCrashWait++;

	if(viiSub::GameCounter()%8==0)
	{
		//スラッシュラインを描画
		new CEneH0404SlashLine( m_Pos.x-1600 , m_Pos.y-1450 );
		new CEneH0404SlashLine( m_Pos.x-1600 , m_Pos.y+1450 );
	}

}


void CEneH0403Tarantura::Draw()
{

	m_CEneCmnSignal.Draw();
	m_pEnemyAnime->Draw( m_bDamage , m_Disp.x , m_Disp.y , ( m_sDirection == DIR_LEFT )? gxTrue : gxFalse , &SprCEneH0403Tarantura[0] );

}


//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------

CEneH0404SlashLine::CEneH0404SlashLine( Sint32 x, Sint32 y )
{
	m_Pos.x = x;
	m_Pos.y = y;
	m_sColor = 0xff;
	m_sScale = 100+viiSub::Rand()%20;
	viiEff::Set(EFF_FIREFLOWER , x , y , argset( 90+viiSub::Rand()%180) );
}


CEneH0404SlashLine::~CEneH0404SlashLine( )
{
	
	
}

void CEneH0404SlashLine::SeqMain( )
{
	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}
	if( m_sColor > 0)
	{
		m_sColor -= 16;
		if( m_sColor <= 0)
		{
			m_sColor = 0;
		}
	}
	else
	{
		SetActionSeq( enActionSeqCrash );
		m_sColor = 0;
	}

	m_Pos.x -= enBackSpeed;
	
}

void CEneH0404SlashLine::SeqCrash( )
{
	SetActionSeq( enActionSeqEnd );
}


void CEneH0404SlashLine::Draw()
{
	
	viiDraw::Sprite( &SprCEneH0403SlashLine[ 0 ] , m_Pos.x , m_Pos.y , PRIO_ENEMY-2 , ATR_ALPHA_PLUS , ARGB(m_sColor,0xff,0xff,0xff) );
}

