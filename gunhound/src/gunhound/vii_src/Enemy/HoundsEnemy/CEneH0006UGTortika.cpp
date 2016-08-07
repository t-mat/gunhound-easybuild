//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "EnemyTbl/CEneH0006UGTortikaTbl.h"
#include "../../Effect/EffectBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -30,
	enKuraiTop   = -28,
	enKuraiRight =  30,
	enKuraiBottom=  0,

	enScore = 400,
};

enum {
	enCannonL1,
	enCannonL2,
	enCannonR1,
	enCannonR2,
};


CEneH0006UGTortika::CEneH0006UGTortika( Sint32 x, Sint32 y ,gxBool bReverse)
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_sReverse = DIR_RIGHT;

	if( bReverse )
	{
		m_sReverse = DIR_LEFT;
		y += 800;
	}

	m_sHigh = 0;

	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_BG2-1;

	m_Add.x = 0;
	m_Add.y = 0;

	m_HitKurai.set_hp( enDefPointBomb );

	if( bReverse )
	{
		m_HitKurai.ax1 = enKuraiLeft;
		m_HitKurai.ay1 = enKuraiBottom;
		m_HitKurai.ax2 = enKuraiRight;
		m_HitKurai.ay2 = -enKuraiTop;
	}
	else
	{
		m_HitKurai.ax1 = enKuraiLeft;
		m_HitKurai.ay1 = enKuraiTop;
		m_HitKurai.ax2 = enKuraiRight;
		m_HitKurai.ay2 = enKuraiBottom;
	}

	m_sDirection = DIR_RIGHT;
	m_sAtribute = ATR_DFLT;

	m_sAtackWait = 64;
	m_sControl   = 0;
	m_bDamage    = gxFalse;

	//アニメデータの読み込み

	m_pEnemyAnime = new CEnemyAnime();

	for(Sint32 ii=0; ii<enMax ;ii++)
	{
		m_pEnemyAnime->SetParts( ii , enMax , m_sParentCEneH0006UGTortika[ii] , sPosCEneH0006UGTortika[ii] );
	}
	m_pEnemyAnime->PosCalc();

	m_sHeight = 30;
	m_Pos.y += m_sReverse*m_sHeight*100;

	//砲身の設定

	m_stCannon[0].dist = 30;
	m_stCannon[0].rot  = 0;
	m_stCannon[0].x    = m_Pos.x+viiMath::Cos100(m_sRotation[0])*m_stCannon[0].dist;
	m_stCannon[0].y    = m_Pos.y+viiMath::Sin100(m_sRotation[0])*m_stCannon[0].dist-4*100;

	m_stCannon[1].dist = 56;
	m_stCannon[1].rot  = 0;
	m_stCannon[1].x    = m_Pos.x+viiMath::Cos100(m_sRotation[0])*m_stCannon[1].dist;
	m_stCannon[1].y    = m_Pos.y+viiMath::Sin100(m_sRotation[0])*m_stCannon[1].dist-4*100;

	m_stCannon[2].dist = 30;
	m_stCannon[2].rot  = 0;
	m_stCannon[2].x    = m_Pos.x+viiMath::Cos100(m_sRotation[1])*m_stCannon[2].dist;
	m_stCannon[2].y    = m_Pos.y+viiMath::Sin100(m_sRotation[1])*m_stCannon[2].dist-4*100;

	m_stCannon[3].dist = 56;
	m_stCannon[3].rot  = 0;
	m_stCannon[3].x    = m_Pos.x+viiMath::Cos100(m_sRotation[1])*m_stCannon[3].dist;
	m_stCannon[3].y    = m_Pos.y+viiMath::Sin100(m_sRotation[1])*m_stCannon[3].dist-4*100;

	m_sRebound[0]  = 0;
	m_sRebound[1]  = 0;
	m_sRotation[0] = -12 *m_sReverse;
	m_sRotation[1] =  192*m_sReverse;


}


CEneH0006UGTortika::~CEneH0006UGTortika()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

	delete m_pEnemyAnime;
}


void CEneH0006UGTortika::SeqInit()
{
	//-----------------------------------------------------
	//近距離に接近したら起動
	//-----------------------------------------------------

	Sint32 tx = GetTargetPlayer()->x ;
	Sint32 dst = tx- m_Pos.x;

	if( gxAbs(dst*100) <= 160*100 )
	{
		return;
	}

	SetActionSeq( enActionSeqMain );

}


void CEneH0006UGTortika::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( m_HitKurai.is_dead() )
	{
		//破壊されたら
		if(!m_bDead)
		{
			m_bDead = gxTrue;

			new CEffectDmgprt( m_Pos.x, m_Pos.y -3200, -90 , 100 );
			new CEffectDmgprt( m_Pos.x, m_Pos.y -3200, -88 , 120 );
			new CEffectDmgprt( m_Pos.x, m_Pos.y -3200, -92 , 80 );

			viiMus::PlaySound(enSoundExplosionMiddle);
			ScorePlus( enScore );
			viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
		}

		if( m_sReverse == DIR_LEFT)
		{
			m_Pos.z = PRIO_ENEMY;

			if( m_Add.y < 800 )
			{
				m_Add.y += 12;
			}

			m_Pos.y += m_Add.y;

			if( viiSub::IsGroundBlock(m_Pos.x,m_Pos.y) )
			{
				SetActionSeq( enActionSeqCrash );
			}
		}
		else
		{
			SetActionSeq( enActionSeqCrash );
		}
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}

	LogicAI();

	Gimmick();

	SetRader( enTypeMarkerGroundObject );
//	DirectionAdjust();

	m_bDamage = m_HitKurai.is_damage();

	//くらい判定登録
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

}


void CEneH0006UGTortika::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
//	m_sControl = Joy[0].psh;

	if( m_HitKurai.is_dead() )
	{
		return;
	}

	if( m_sAtackWait > 0 )
	{
		m_sAtackWait --;

	}
	switch( m_sAtackWait )
	{
//	case 40:
//	case 30:
	case 20:
		Atack();
		break;
	case 0:
		m_sAtackWait = 290;
		break;
	}
}


void CEneH0006UGTortika::Gimmick()
{
	//-----------------------------------------------------
	//ギミック
	//-----------------------------------------------------

	if( m_sHigh < m_sHeight*100 )
	{
		m_sHigh += 50;
		m_sAtackWait = 64;
		m_Pos.y -= 50*m_sReverse;

		m_sRebound[0] = 0;
		m_sRebound[1] = 0;
	}

	for(Sint32 ii=0;ii<2;ii++)
	{
		m_sRebound[ii] += (100-m_sRebound[ii])/10;
	}

	if( m_sControl&JOY_L )
	{
		m_sRotation[0] --;
		m_sRotation[1] ++;
	}
	else if( m_sControl&JOY_R )
	{
		m_sRotation[0] ++;
		m_sRotation[1] --;
	}

	m_stCannon[0].x = m_Pos.x+viiMath::Cos100(m_sRotation[0])*(m_stCannon[0].dist*m_sRebound[0]/100);
	m_stCannon[0].y = m_Pos.y+viiMath::Sin100(m_sRotation[0])*(m_stCannon[0].dist*m_sRebound[0]/100)-4*100;

	m_stCannon[1].x = m_Pos.x+viiMath::Cos100(m_sRotation[0])*(m_stCannon[1].dist*m_sRebound[0]/100);
	m_stCannon[1].y = m_Pos.y+viiMath::Sin100(m_sRotation[0])*(m_stCannon[1].dist*m_sRebound[0]/100)-4*100;

	m_stCannon[2].x = m_Pos.x+viiMath::Cos100(m_sRotation[1])*(m_stCannon[2].dist*m_sRebound[1]/100);
	m_stCannon[2].y = m_Pos.y+viiMath::Sin100(m_sRotation[1])*(m_stCannon[2].dist*m_sRebound[1]/100)-4*100;

	m_stCannon[3].x = m_Pos.x+viiMath::Cos100(m_sRotation[1])*(m_stCannon[3].dist*m_sRebound[1]/100);
	m_stCannon[3].y = m_Pos.y+viiMath::Sin100(m_sRotation[1])*(m_stCannon[3].dist*m_sRebound[1]/100)-4*100;

}


void CEneH0006UGTortika::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------
//	CAtkH0001Ryudan *p;

	m_sRebound[0] = 25;
	m_sRebound[1] = 25;
	Sint32 ax,ay,rot;

	rot = -25*m_sReverse;
	ax = m_Pos.x + viiMath::Cos100(rot)*32;
	ay = m_Pos.y + viiMath::Sin100(rot)*32;

	viiEff::Set( EFF_MAZZLE , ax , ay , NULL );
	viiEff::Set( EFF_RING , ax , ay , argset(200) );
//	new CEne0001EneMissile(   ax , ay , -15 , 380 );

	if( m_sReverse == -1)
	{
		ay -= 400;
//		p = new CAtkH0001Ryudan( ax , ay , 0, 480 );
//		p->SetForce(CAtkH0001Ryudan::enForceID_Enemy);

		CEne0001EneSuperBall *q;
		q = new CEne0001EneSuperBall( ax , ay , 0, 480  );
		q->SetPattern( CEne0001EneSuperBall::enPatternDrop );
		q->SetScale( 0.5f );

	}
	else
	{
//		p = new CAtkH0001Ryudan( ax , ay , -35*m_sReverse, 480 );
//		p->SetForce(CAtkH0001Ryudan::enForceID_Enemy);

		CEne0001EneSuperBall *q;
		q = new CEne0001EneSuperBall( ax , ay , -15*m_sReverse, 480  );
		q->SetPattern( CEne0001EneSuperBall::enPatternDrop );
		q->SetScale( 0.5f );

	}


	rot = 180+25*m_sReverse;

	ax = m_Pos.x + viiMath::Cos100(rot)*32;
	ay = m_Pos.y + viiMath::Sin100(rot)*32;
	viiEff::Set( EFF_MAZZLE , ax , ay , NULL );
	viiEff::Set( EFF_RING , ax , ay , argset(200) );

	if( m_sReverse == -1)
	{
		ay -= 400;
//		p = new CAtkH0001Ryudan( ax , ay , 180, 480 );
//		p->SetForce(CAtkH0001Ryudan::enForceID_Enemy);

		CEne0001EneSuperBall *q;
		q = new CEne0001EneSuperBall( ax , ay , 180, 480  );
		q->SetPattern( CEne0001EneSuperBall::enPatternDrop );
		q->SetScale( 0.5f );

	}
	else
	{
//		p = new CAtkH0001Ryudan( ax , ay , 180+35, 480 );
//		p->SetForce(CAtkH0001Ryudan::enForceID_Enemy);

		CEne0001EneSuperBall *q;
		q = new CEne0001EneSuperBall( ax , ay , 180+15, 480  );
		q->SetPattern( CEne0001EneSuperBall::enPatternDrop );
		q->SetScale( 0.5f );
	}

}


void CEneH0006UGTortika::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	viiMus::PlaySound(enSoundExplosionMiddle);
	ScorePlus( enScore );
	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

	if( m_sReverse == DIR_LEFT )
	{
		//----------------------------------
		//リバース
		//----------------------------------
		EffectMakeBigBomb(m_Pos.x , m_Pos.y);

		new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x+(-4+viiSub::Rand()%8)*100,m_Pos.y+(-4+viiSub::Rand()%8)*100 , PRIO_ENEMY , 45 ,40 );
		new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x+(-4+viiSub::Rand()%8)*100,m_Pos.y+(-4+viiSub::Rand()%8)*100 , PRIO_ENEMY , 45 ,120);
		new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x+(-4+viiSub::Rand()%8)*100,m_Pos.y+(-4+viiSub::Rand()%8)*100 , PRIO_ENEMY , 45 ,80 );

		CEffectDmgprt *p;
		p = new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, 180+15 , 800 );
		p->SetSprite( &SprCEneH0006UGTortika[ m_pEnemyAnime->GetParts( enMIGIHOUDAI2 )->sSprite ] );

		p=new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, 180+65 , 600 );
		p->SetSprite( &SprCEneH0006UGTortika[ m_pEnemyAnime->GetParts( enMIGIHOUDAI1 )->sSprite ] );

		p=new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, -60    , 800 );
		p->SetSprite( &SprCEneH0006UGTortika[ m_pEnemyAnime->GetParts( enHIDARIHOUDAI1 )->sSprite ] );

		p=new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, -20    , 700 );
		p->SetSprite( &SprCEneH0006UGTortika[ m_pEnemyAnime->GetParts( enHIDARIHOUDAI2 )->sSprite ] );
	}
	else
	{
		new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x+(-4+viiSub::Rand()%8)*100,m_Pos.y+(-4+viiSub::Rand()%8)*100 , PRIO_ENEMY , 270+45 ,40 );
		new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x+(-4+viiSub::Rand()%8)*100,m_Pos.y+(-4+viiSub::Rand()%8)*100 , PRIO_ENEMY , 270+45 ,120);
		new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x+(-4+viiSub::Rand()%8)*100,m_Pos.y+(-4+viiSub::Rand()%8)*100 , PRIO_ENEMY , 270+45 ,80 );
	}

	SetActionSeq( enActionSeqEnd );

}


void CEneH0006UGTortika::Draw()
{

	//	m_pEnemyAnime->Draw( m_bDamage , m_Pos.x , m_Pos.y , ( m_sDirection == DIR_LEFT )? gxTrue : gxFalse , &SprCEneH0006UGTortika[0] );
	if(m_sReverse==1)
	{
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0006UGTortika[ m_pEnemyAnime->GetParts( enMIGIHOUDAI2 )->sSprite ] , m_stCannon[1].x , m_stCannon[1].y,m_Pos.z , m_sAtribute , ARGB_DFLT ,1.0f,m_sRotation[0]);
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0006UGTortika[ m_pEnemyAnime->GetParts( enMIGIHOUDAI1 )->sSprite ] , m_stCannon[0].x , m_stCannon[0].y,m_Pos.z , m_sAtribute , ARGB_DFLT ,1.0f,m_sRotation[0]);

		viiDraw::Sprdmg( m_bDamage , &SprCEneH0006UGTortika[ m_pEnemyAnime->GetParts( enMIGIHOUDAI2 )->sSprite ] , m_stCannon[3].x , m_stCannon[3].y,m_Pos.z , m_sAtribute , ARGB_DFLT ,1.0f,m_sRotation[1]);
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0006UGTortika[ m_pEnemyAnime->GetParts( enMIGIHOUDAI1 )->sSprite ] , m_stCannon[2].x , m_stCannon[2].y,m_Pos.z , m_sAtribute , ARGB_DFLT ,1.0f,m_sRotation[1]);

		viiDraw::Sprdmg( gxFalse , &SprCEneH0006UGTortika[ m_pEnemyAnime->GetParts( enHOUDAI )->sSprite ] , m_Pos.x , m_Pos.y , m_Pos.z , m_sAtribute|m_sAtributeDir , ARGB_DFLT );
	}
	else
	{
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0006UGTortika[ m_pEnemyAnime->GetParts( enMIGIHOUDAI2 )->sSprite ] , m_stCannon[1].x , m_stCannon[1].y,m_Pos.z , m_sAtribute , ARGB_DFLT ,1.0f,m_sRotation[0]);
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0006UGTortika[ m_pEnemyAnime->GetParts( enMIGIHOUDAI1 )->sSprite ] , m_stCannon[0].x , m_stCannon[0].y,m_Pos.z , m_sAtribute , ARGB_DFLT ,1.0f,m_sRotation[0]);

		viiDraw::Sprdmg( m_bDamage , &SprCEneH0006UGTortika[ m_pEnemyAnime->GetParts( enMIGIHOUDAI2 )->sSprite ] , m_stCannon[3].x , m_stCannon[3].y,m_Pos.z , m_sAtribute , ARGB_DFLT ,1.0f,m_sRotation[1]);
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0006UGTortika[ m_pEnemyAnime->GetParts( enMIGIHOUDAI1 )->sSprite ] , m_stCannon[2].x , m_stCannon[2].y,m_Pos.z , m_sAtribute , ARGB_DFLT ,1.0f,m_sRotation[1]);

		viiDraw::Sprdmg( gxFalse , &SprCEneH0006UGTortika[ m_pEnemyAnime->GetParts( enHOUDAI )->sSprite ] , m_Pos.x , m_Pos.y,m_Pos.z , m_sAtribute|m_sAtributeDir|ATR_FLIP_Y , ARGB_DFLT );
	}
}

