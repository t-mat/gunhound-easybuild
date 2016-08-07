//--------------------------------------------------------------------------------
//
// Stage00:グリフォン
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "EnemyTbl/CEneH0101HeavyTankTbl.h"
#include "../../Effect/EffectBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -64,
	enKuraiTop   = -64,
	enKuraiRight =  64,
	enKuraiBottom=  0,

	enSpeed = 90,
	enScore = 300,
};

/*
gxSprite SprCEneH0101HeavyTank[]=
{
	{enTexPageEnemyStage02,0,88,232,96,116,96},//味方重戦車
	{TPG_LEYNOS02,0,88,232,96,116,96},	//味方重戦車
	{TPG_LEYNOS02,0,88,232,96,116,96},
	{TPG_LEYNOS02, 0,104,128,56,64,56},
};
*/


CEneH0101HeavyTank::CEneH0101HeavyTank( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_ENEMY;

	m_Add.x = 0;
	m_Add.y = 0;

	m_HitKurai.set_hp( HP_ARMORCLASS_B );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_RIGHT;
	m_sAtribute  = ATR_DFLT;
	m_sTagetPosX = 9600*100;
	m_bMoveStop  = gxFalse;
	m_bDummy = gxFalse;

	m_bMokutekichi  =gxFalse;
	m_bCannonAtack = gxFalse;
	m_sAtackTimer = 0;

	m_pEnemyAnime = new CEnemyAnime();
	m_pCatapiller = new CCatapiller();

	m_pCatapiller->SetSpriteIndex( enKYATAPIRA1 , enKYATAPIRA43 );

	Init();
}


CEneH0101HeavyTank::~CEneH0101HeavyTank()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

	delete m_pCatapiller;
	delete m_pEnemyAnime;

}


void CEneH0101HeavyTank::Init()
{

	for(Sint32 ii=0; ii<enMax ;ii++)
	{
		m_pEnemyAnime->SetParts( ii , enMax , m_sParentCEneH0101HeavyTank[ii] , sPosCEneH0101HeavyTank[ii] );
	}

}


void CEneH0101HeavyTank::SeqMain()
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

//	m_HitKurai.set_hp( HP_ARMORCLASS_B );

	m_pEnemyAnime->PosCalc();

	LogicAI();

	Atack();

	SetRader( enTypeMarkerDefenceTarget );
	DirectionAdjust();

	//くらい判定登録
	if( !m_bDummy )
	{
		m_HitKurai.SetHantei(ID_VERSIS_DEF , &m_Pos );
	}

}


void CEneH0101HeavyTank::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	Sint32 sControl = 0;
	Sint32 sSpeed = enSpeed;

	if( m_bMoveStop )
	{
		m_Add.x = 0;
		sControl = 0;
	}
	else if( m_Pos.x < m_sTagetPosX )
	{
		m_Add.x = sSpeed;
		sControl = JOY_R;
	}
	else if( m_Pos.x >= m_sTagetPosX )
	{
		Sint32 sAdd = (0-m_Add.x)/10;
		if(sAdd==0)
		{
			if( m_Add.x < 0 ) m_Add.x ++;
			if( m_Add.x > 0 ) m_Add.x --;
		}
		else
		{
			m_Add.x += sAdd;
		}
		m_bMokutekichi = gxTrue;
	}

	m_Pos.x += m_Add.x;

	Sint32 x = m_Pos.x , y = m_Pos.y-800;

		
//	while( gxTrue )
	{
		Sint32 kabe = pGame->pBg->get_kabetype( x/100 , y/100 );
		if( kabe == 0 )
		{
			m_Pos.y += 60;
		}
		if( kabe  )
		{
			m_Pos.y -= 10;
		}
	}

/*
	if( pGame->GetHound()->m_Pos.x >= m_Pos.x-6400)
	{
		if( pGame->GetHound()->m_Pos.x <= m_Pos.x+6400)
		{
			if( pGame->GetHound()->m_Pos.y >= m_Pos.y-3200 )
			{
				if( pGame->GetHound()->m_Pos.y <= m_Pos.y )
				{
					pGame->GetHound()->SetRide( gxTrue , &m_Add );
				}
			}
		}
	}
*/
	//-----------------------------------------------------------
	//キャタピラ稼動
	//-----------------------------------------------------------

	if( sControl & JOY_L )
	{
		m_pCatapiller->MoveLeft();
	}
	else if( sControl & JOY_R )
	{
		m_pCatapiller->MoveRight();
	}
	else
	{
		m_pCatapiller->MoveNone();
	}

	m_pCatapiller->Action( m_pEnemyAnime );

//	new CEffDashStorm( m_Pos.x+(-100+viiSub::Rand()%200)*100 , m_Pos.y, 180 , 30 );

}


void CEneH0101HeavyTank::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

	if( m_bCannonAtack )
	{
		CannonAtack();
		return;
	}

/*
	if( viiSub::GameCounter()%60 <= 32)
	{
		if( viiSub::GameCounter()%4==0)
		{
			viiEff::Set( EFF_MAZZLE , m_Pos.x , m_Pos.y-6400 , NULL );
			if( !m_bDummy )
			{
				new CEffGattlingGun( m_Pos.x , m_Pos.y-6400 , -36+viiSub::Rand()%8 , 1050 );
			}
		}
	}
*/
//	if( viiSub::GameCounter()%150 == 0 )
//	{
//		if( !m_bDummy )
//		{
//			CAtkH0001Bazooka *p;
//			p = new CAtkH0001Bazooka( m_Pos.x , m_Pos.y-6400 , 0 , 2400);
//			p->SetNoneSmoke();
//		}
//	}


}

void CEneH0101HeavyTank::CannonAtack()
{
	//-----------------------------------------
	//砲撃
	//-----------------------------------------

	ghPos stPos;
	gxBool bAtack = gxFalse;

	stPos.x = m_Pos.x+4800;
	stPos.y = m_Pos.y-4800;

	switch(m_sAtackTimer){
	case 10:
		if( !IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
		{
			pGame->QuakeOn(32);
		}
		stPos.x += 2400-1200*0;
		stPos.y += -11000+300*0;
		bAtack = gxTrue;
		break;
	case 20:
		stPos.x += 2400-1200*1;
		stPos.y += -11000+300*1;
		bAtack = gxTrue;
		break;
	case 30:
		stPos.x += 2400-1200*2;
		stPos.y += -11000+300*2;
		bAtack = gxTrue;
		break;
	case 40:
		stPos.x += 2400-1200*3;
		stPos.y += -11000+300*3;
		bAtack = gxTrue;
		break;

	case 50:
		stPos.x += 2400-1200*4;
		stPos.y += -11000+300*4;
		bAtack = gxTrue;
		break;

	case 60:
		stPos.x += 2400-1200*0;
		stPos.y += -9000+300*0;
		bAtack = gxTrue;
		break;
	case 70:
		stPos.x += 2400-1200*1;
		stPos.y += -9000+300*1;
		bAtack = gxTrue;
		break;
	case 80:
		stPos.x += 2400-1200*2;
		stPos.y += -9000+300*2;
		bAtack = gxTrue;
		break;
	case 90:
		stPos.x += 2400-1200*3;
		stPos.y += -9000+300*3;
		bAtack = gxTrue;
		break;

	case 100:
		stPos.x += 2400-1200*4;
		stPos.y += -9000+300*4;
		bAtack = gxTrue;
		break;
	}

	if(bAtack)
	{
		viiEff::Set( EFF_MAZZLE , stPos.x , stPos.y , NULL );
		viiEff::SetBombSimple( stPos.x , stPos.y , 1 , 1+viiSub::Rand()%2 );
		new CAtkH0001Bazooka(stPos.x , stPos.y , -25-2+viiSub::Rand()%4 , 2400);
	}

	m_sAtackTimer ++;
	m_sAtackTimer = m_sAtackTimer%180;
}

void CEneH0101HeavyTank::SeqCrash()
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
	}

	if( m_bClear )
	{
		SetActionSeq( enActionSeqEnd );
	}


}


void CEneH0101HeavyTank::SetOperation( Sint32 op , Sint32 dat )
{
	//----------------------------------------------------------
	//指令受付
	//----------------------------------------------------------

	switch( op ){
	case enForceOpGo:	//進め
		m_bMoveStop = gxFalse;
		break;

	case enForceOpStop:	//とまれ
		m_bMoveStop = gxTrue;
		break;

	case enForceOpMovePos:	//指定の場所へ集合
		m_bMokutekichi = gxFalse;
		m_sTagetPosX = dat;
		break;

	case enForceOpMoveAdd:	//現在位置の前方datドットへ進め
		m_bMokutekichi = gxFalse;
		m_sTagetPosX = m_Pos.x + dat;
		break;

	default:
		break;
	}

}


void CEneH0101HeavyTank::Draw()
{
	StPartsInfo *p;
	gxBool bDamage = m_HitKurai.is_damage();

	if( m_bDummy )
	{
		m_Pos.z = PRIO_BG1_5;
	}
	else
	{
		m_Pos.z = PRIO_BG2+5;
	}

	Sint32 yyy,height = ((m_sTimer%8)<4 )? 100 : 0;

	for(Sint32 ii=0; ii<enMax ;ii++)
	{
		yyy = 0;

		switch( ii ){
	    case enPARENT:
	    case enDOUTAI:
	    case enHOUDAI:
//	    case enSHOCKDAI1:
//	    case enSHOCKDAI2:
//	    case enSHOCK:
	    case enASI:
			yyy = height;
			break;
//	    case enYOKOHOUDAI1:
//	    case enYOKOHOUDAI2:
//	    case enYOKOHOUDAI3:
//	    case enYOKOHOUDAI4:
//	    case enYOKOHOUDAI5:
//	    case enYOKOHOUDAI6:
//	    case enYOKOHOUDAI7:
//	    case enYOKOHOUDAI8:
//			yyy = height/2;
			break;
		}
		p = m_pEnemyAnime->GetParts( ii );

		viiDraw::Sprdmg(
			bDamage ,
			&SprCEneH0101HeavyTank[ p->sDispSprite ] ,
			m_Pos.x + p->dx*100,
			m_Pos.y + p->dy*100 + yyy-1600,
			m_Pos.z + p->dz,
			m_sAtribute,	//|m_sAtributeDir ,
			ARGB_DFLT,
			p->sx/100.f,
			p->dr);
	}

//	viiDraw::Sprdmg( m_HitKurai.is_damage() , &SprCEneH0101HeavyTank[ 0 ] , m_Pos.x , m_Pos.y,PRIO_ENEMY-1 , m_sAtribute|m_sAtributeDir , ARGB_DFLT );

}

