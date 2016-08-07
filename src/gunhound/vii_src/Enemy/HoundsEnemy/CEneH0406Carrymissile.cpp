//--------------------------------------------------------------------------------
//
// Stage00:魚雷
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "EnemyTbl/CEneH0406CarrymissileTBL.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -30,
	enKuraiTop   = -34,
	enKuraiRight =  30,
	enKuraiBottom=  0,

	enScore = 300,
};

/*
gxSprite SprCEneH0406Carrymissile[]=
{
	{TPG_LEYNOS01,0,192,64,64,32,64},//ボルゾイ
};
*/

CEneH0406Carrymissile::CEneH0406Carrymissile( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_Tgt.x = 0;
	m_Tgt.y = 0;

	m_sOpenSeq=0;
	m_sOpenTimer=0;
	m_HitKurai.set_hp( HP_ARMORCLASS_B );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_RIGHT;
	m_sAtribute = ATR_DFLT;

	m_bDamage = gxFalse;
	m_pEnemyAnime = new CEnemyAnime();

	for(Sint32 ii=0; ii<enMax ;ii++)
	{
		m_pEnemyAnime->SetParts( ii , enMax , m_sParentCEneH0406Carrymissile[ii] , sPosCEneH0406Carrymissile[ii] );
	}

	m_pEnemyAnime->PosCalc();

		FutaAdd[0].x = 0;
		FutaAdd[0].y = 0;
		FutaAdd[1].x = 0;
		FutaAdd[1].y = 0;

		m_sOpenSeq = 0;

	m_sJoy = 0;
	m_Add.x = 0;
	m_Add.y = 0;
	m_bDestroy = gxFalse;
}


CEneH0406Carrymissile::~CEneH0406Carrymissile()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

	delete m_pEnemyAnime;
}


void CEneH0406Carrymissile::SeqMain()
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

	if( m_Tgt.x != 0 && m_Tgt.y != 0)
	{
//		m_Pos.x += (m_Tgt.x-m_Pos.x)/20;
//		m_Pos.y += (m_Tgt.y-m_Pos.y)/20;
		if( m_Pos.x < m_Tgt.x ) m_sJoy |= JOY_R;
		else if( m_Pos.x > m_Tgt.x ) m_sJoy |= JOY_L;

		if( m_Pos.y > m_Tgt.y ) m_sJoy |= JOY_D;
		else if( m_Pos.y > m_Tgt.y ) m_sJoy |= JOY_U;
	}

	if( m_sJoy&JOY_L )
	{
		if( m_Add.x > -300 ) m_Add.x -= 3;
	}
	else if( m_sJoy&JOY_R )
	{
		if( m_Add.x < 550 ) m_Add.x += 5;
	}
	else
	{
		m_Add.x += (0-m_Add.x)/10;
	}

	if( m_sJoy&JOY_U )
	{
		if( m_Add.y > -100 ) m_Add.y -= 2;
	}
	else if( m_sJoy&JOY_D )
	{
		if( m_Add.y < 100 ) m_Add.y += 2;
	}

	m_Pos.x += m_Add.x;
	m_Pos.x += m_Add.y;
	m_sJoy = 0;

	LogicAI();

	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	//くらい判定登録
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

}


void CEneH0406Carrymissile::LogicAI()
{
	//-----------------------------------------------------
	//出撃ロジック
	//-----------------------------------------------------
//	m_sControl = Joy[0].trg;
//	if(m_sControl&BTN_X)
//	{
//		m_sOpenSeq = 5;
//	}


	m_pEnemyAnime->PosCalc();

	switch(m_sOpenSeq){
	case 0:
		//-----------------------------------------------
		//どーん（待機）
		//-----------------------------------------------
		FutaPos[0].x = m_pEnemyAnime->GetParts( enKAIKOUBU1 )->dx*100;
		FutaPos[0].y = m_pEnemyAnime->GetParts( enKAIKOUBU1 )->dy*100;
		FutaPos[0].z = 0;

		FutaPos[1].x = m_pEnemyAnime->GetParts( enKAIKOUBU2 )->dx*100;
		FutaPos[1].y = m_pEnemyAnime->GetParts( enKAIKOUBU2 )->dy*100;
		FutaPos[1].z = 0;

		pGame->GetHound()->SetMode( CGunHound::enHoundModePodStart );
		pGame->GetHound()->SetForceShift(gxTrue);
		HoundPos.x = m_Pos.x+3200;
		HoundPos.y = m_Pos.y+2400;
		HoundTgt.x = HoundPos.x+8000;
		HoundTgt.y = 0;
		HoundAdd.x = 0;
		HoundAdd.y = 0;
		m_sOpenTimer = 0;
		break;

	case 5:
		//-----------------------------------------------
		//
		//-----------------------------------------------
		FutaAdd[0].x = 0;
		FutaAdd[0].y = 0;
		FutaAdd[1].x = 0;
		FutaAdd[1].y = 0;
		m_sOpenSeq   = 10;
		m_sOpenTimer = 0;
		break;

	case 10:
		//-----------------------------------------------
		//がくがく
		//-----------------------------------------------

		
		if(m_sOpenTimer<15)
		{
			m_sOpenSeq = 20;
//			FutaPos[0].x += ((m_sOpenTimer%4)<2)? 2 : -2;
//			FutaPos[1].x += ((m_sOpenTimer%4)<2)? 2 : -2;
//			FutaPos[0].y = 0;
//			FutaPos[1].y = 0;
		}
		else
		{

//			FutaAdd[0].x =120;
//			FutaAdd[0].y = -24;
//
//			FutaAdd[1].x =120;
//			FutaAdd[1].y =0;
//			m_sOpenSeq = 20;
//			m_sOpenTimer = 0;

		}
		break;

	case 20:
		//-----------------------------------------------
		//ポロ
		//-----------------------------------------------
		FutaAdd[0].x = viiMath::Cos100(FutaPos[0].z)*32-2400;
		FutaAdd[0].y = viiMath::Sin100(FutaPos[0].z)*16;

		FutaAdd[1].x = viiMath::Cos100(FutaPos[1].z)*32-2400;
		FutaAdd[1].y = viiMath::Sin100(FutaPos[1].z)*16;

		FutaPos[0].z += (-115-FutaPos[0].z)/10;
		FutaPos[1].z += ( 115-FutaPos[1].z)/10;
		if( m_sOpenTimer >= 60 )
		{
			m_sOpenTimer = 0;
			m_sOpenSeq   = 100;
		}
		break;

	case 100:
		//-----------------------------------------------
		//発射
		//-----------------------------------------------
		HoundPos.x += (HoundTgt.x-HoundPos.x)/10;
		if( m_sOpenTimer >= 40 )
		{
			HoundAdd.x = 480;
			m_sOpenTimer = 0;
			m_sOpenSeq   = 300;
			pGame->GetHound()->SetForceShift(gxFalse);
			pGame->GetHound()->SetMode( CGunHound::enHoundModeNormal );
		}
		break;

	case 300:
		//-----------------------------------------------
		//びゅーん
		//-----------------------------------------------
		//if( HoundAdd.x > -248 ) HoundAdd.x -= 4;


		if( m_sOpenTimer <  45 )
		{
			pGame->GetHound()->SetForceBurnia(gxTrue);
			pGame->GetHound()->SetPadButton( JOY_R|BTN_A );
		}
		else if( m_sOpenTimer <  95 )
		{
//			pGame->GetHound()->SetPadButton( JOY_R );
			pGame->GetHound()->SetPadButton( 0 );
		}
		else if( m_sOpenTimer <  90 )
		{
//			pGame->GetHound()->SetForceBurnia(gxFalse);
//			HoundAdd.x = 0;
//			pGame->GetHound()->SetPadButton( JOY_L|BTN_L1 );
//			if( pGame->GetHound()->m_Pos.x < 3370*100 )
			{
	//			m_sOpenTimer = 0;
	//			m_sOpenSeq   = 500;
			}
		}
		else if( m_sOpenTimer <  100 )
		{
			pGame->GetHound()->SetPadButton( 0 );
		}
		else
		{
			m_sOpenSeq   = 500;
		}
		pGame->GetHound()->m_Pos.x += HoundAdd.x;
		break;

	case 500:
//		if( HoundAdd.x < 0 ) HoundAdd.x += 8;
//		pGame->GetHound()->m_Pos.x += HoundAdd.x;
		break;

	}


//	FutaPos[0].x = FutaAdd[0].x;
//	FutaPos[0].y = FutaAdd[0].y;

//	FutaPos[1].x = FutaAdd[1].x;
//	FutaPos[1].y = FutaAdd[1].y;

	m_pEnemyAnime->GetParts( enKAIKOUBU1 )->dx = (FutaPos[0].x+FutaAdd[0].x)/100;
	m_pEnemyAnime->GetParts( enKAIKOUBU1 )->dy = (FutaPos[0].y+FutaAdd[0].y)/100;
	m_pEnemyAnime->GetParts( enKAIKOUBU1 )->dr = FutaPos[0].z/10;

	m_pEnemyAnime->GetParts( enKAIKOUBU2 )->dx = (FutaPos[1].x+FutaAdd[1].x)/100;
	m_pEnemyAnime->GetParts( enKAIKOUBU2 )->dy = (FutaPos[1].y+FutaAdd[1].y)/100;;
	m_pEnemyAnime->GetParts( enKAIKOUBU2 )->dr = FutaPos[1].z/10;
	m_pEnemyAnime->SetPrio( PRIO_PLAYER + 1 );

	if( m_sOpenSeq < 300 )
	{
	   pGame->GetHound()->m_Pos.x = HoundPos.x;
	   pGame->GetHound()->m_Pos.y = HoundPos.y;
	}
	m_sOpenTimer ++;

}


void CEneH0406Carrymissile::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CEneH0406Carrymissile::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	viiMus::PlaySound(enSoundExplosionMiddle);

	ScorePlus( enScore );

	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

	SetActionSeq( enActionSeqEnd );

}

void CEneH0406Carrymissile::Draw()
{

//	viiDraw::Sprdmg( m_HitKurai.is_damage() , &SprCEneH0406Carrymissile[ 0 ] , m_Pos.x , m_Pos.y,PRIO_ENEMY , m_sAtribute|m_sAtributeDir , ARGB_DFLT );
	m_pEnemyAnime->Draw( m_bDamage , m_Pos.x , m_Pos.y , ( m_sDirection == DIR_LEFT )? gxTrue : gxFalse , &SprCEneH0406Carrymissile[0] );

}

