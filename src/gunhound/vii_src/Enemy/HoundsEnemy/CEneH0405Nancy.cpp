//--------------------------------------------------------------------------------
//
// Stage00:魚雷発射ポッド
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
//#include "EnemyTbl/CEneH0405NancyTBL.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -30,
	enKuraiTop   = -34,
	enKuraiRight =  30,
	enKuraiBottom=  0,

	enScore = 300,
};

gxSprite SprCEneH0405Nancy[]=
{
	{ enTexCEneH0405Nancy , 112,64,192,96,56,32},	//ボディ
	{ enTexCEneH0405Nancy ,   0,64,112,80,56,40},	//ボディタンク

	{ enTexCEneH0405Nancy , 320,64,96,48,48,24},	//ボディ中

	{ enTexCEneH0405Nancy , 416,64,48,32,24,16},	//ボディ小
};

gxSprite SprCEneH0405Missile[]=
{
	{ enTexCEneH0405Nancy , 256,0  ,224,64,64 ,32},	//ミサイル大
	{ enTexCEneH0405Nancy , 320,112,96,32 ,32 ,16-4},	//ミサイル中
	{ enTexCEneH0405Nancy , 416,112,64,32 ,16 ,16-2},	//ミサイル小
};

CEneH0405Nancy::CEneH0405Nancy( Sint32 x, Sint32 y ,Sint32 sSize)
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_ENEMY;

	m_MissilePos.x = x;
	m_MissilePos.y = y;
	m_MissilePos.z = m_Pos.z-1;

	m_MissileAdd.x = 0;
	m_MissileAdd.y = 0;

	m_bMissile = gxFalse;

	m_sSize = sSize;

	m_sDirection = DIR_RIGHT;
	m_sAtribute = ATR_DFLT;

	m_bDamage = gxFalse;
	m_bStop = gxFalse;

	m_pTorpedo = NULL;

	m_sMissileTimer = 0;

	if( m_sSize == enSizeLarge )
	{
		m_MissilePos.z = PRIO_BG3-1;
//		m_pTorpedo = new CEneH0406Carrymissile( m_MissilePos.x , m_MissilePos.y );
	}
	else if( m_sSize == enSizeMiddle )
	{
		m_MissilePos.z = PRIO_BG2+1;
//		m_pTorpedo = new CEneH0406Carrymissile( m_MissilePos.x , m_MissilePos.y );
	}
	else if( m_sSize == enSizeSmall )
	{
		m_MissilePos.z = PRIO_BG2-1;
//		m_pTorpedo = new CEneH0406Carrymissile( m_MissilePos.x , m_MissilePos.y );
	}

	m_bAutoAdjust = gxTrue;
}


CEneH0405Nancy::~CEneH0405Nancy()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneH0405Nancy::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

/*
	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}
*/
	Float32 sSizeAdd = m_sSize+1;

	if( m_bClear )
	{
		SetActionSeq( enActionSeqCrash );
	}


	LogicAI();

	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	if( m_bMissile )
	{
		if( m_sMissileTimer < 980 ) m_sMissileTimer +=32;

		CEffectBubble *p;
		Sint32 ax,ay;

		ax = (-56+viiSub::Rand()%32)*100;
		ay = (-16+viiSub::Rand()%32)*100;

		ax = ax/(m_sSize+1);
		ay = ay/(m_sSize+1);

		Sint32 sSpeed = m_sMissileTimer/(m_sSize+1);

		p = new CEffectBubble(	m_MissilePos.x+ax , 	m_MissilePos.y+ay ,180, sSpeed);
		p->SetWide( m_sTimer%360 , 90/(m_sSize+1) );
		p->SetScale( 1.f/POW( (m_sSize+1) ) );
		p->m_Pos.z = m_MissilePos.z;

//		if( m_sTimer%8==0)
//		{
//			p = new CEffectBubble(	m_MissilePos.x+ax , 	m_MissilePos.y+ay ,180, sSpeed/2);
//			p->SetWide( 180 , 390 );
//			p->SetScale( 1.5f/POW( (m_sSize+1) ) );
//			p->m_Pos.z = m_MissilePos.z;
//		}
	}

}


void CEneH0405Nancy::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	Float32 sSizeAdd = m_sSize+1;

/*
	if( Joy[0].trg&BTN_X)
	{
		SetMissile();
	}
*/
	if( m_bMissile )
	{
		if( m_bStop )
		{
			m_MissileAdd.y = 0;
			if( m_MissileAdd.x > 0 )
			{
				m_MissileAdd.x = 0;
			}
			else
			{
				if( m_MissileAdd.x > -100 ) m_MissileAdd.x-=10;
			}
		}
		else
		{
			if( m_MissileAdd.x <= 120/sSizeAdd ) m_MissileAdd.x += (6/sSizeAdd);
			if( m_MissileAdd.y  > -50/sSizeAdd ) m_MissileAdd.y -= 1;
		}
		m_MissilePos.x += m_MissileAdd.x;
		m_MissilePos.y += m_MissileAdd.y;
	}
	else
	{
		m_MissilePos.x = m_Pos.x+m_MissileAdd.x;
		m_MissilePos.y = m_Pos.y+m_MissileAdd.y;
		if( m_sSize == enSizeLarge && m_bAutoAdjust)
		{
			m_MissilePos.y = m_Pos.y+4800;
		}
	}

}


void CEneH0405Nancy::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CEneH0405Nancy::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------


	SetActionSeq( enActionSeqEnd );

}

void CEneH0405Nancy::Draw()
{

	switch( m_sSize ){
	case enSizeLarge:
		viiDraw::Sprdmg( gxFalse , &SprCEneH0405Nancy[ 0 ] , m_Pos.x , m_Pos.y , m_Pos.z , m_sAtribute|m_sAtributeDir , ARGB_DFLT );
		viiDraw::Sprdmg( gxFalse , &SprCEneH0405Missile[ 0 ] , m_MissilePos.x , m_MissilePos.y , m_MissilePos.z , m_sAtribute|m_sAtributeDir , ARGB_DFLT );
		viiDraw::Sprdmg( gxFalse , &SprCEneH0405Nancy[ 1 ] , m_Pos.x , m_Pos.y+3200 , m_Pos.z , m_sAtribute|m_sAtributeDir , ARGB_DFLT );
		break;
	case enSizeMiddle:
		viiDraw::Sprdmg( gxFalse , &SprCEneH0405Missile[ 1 ] , m_MissilePos.x , m_MissilePos.y , m_MissilePos.z , m_sAtribute|m_sAtributeDir , ARGB_DFLT );
		viiDraw::Sprdmg( gxFalse , &SprCEneH0405Nancy[ 2 ]   , m_Pos.x , m_Pos.y , m_Pos.z , m_sAtribute|m_sAtributeDir , ARGB_DFLT );
		break;
	case enSizeSmall:
		viiDraw::Sprdmg( gxFalse , &SprCEneH0405Missile[ 2 ] , m_MissilePos.x , m_MissilePos.y , m_MissilePos.z , m_sAtribute|m_sAtributeDir , ARGB_DFLT );
		viiDraw::Sprdmg( gxFalse , &SprCEneH0405Nancy[ 3 ] , m_Pos.x , m_Pos.y , m_Pos.z , m_sAtribute|m_sAtributeDir , ARGB_DFLT );
		break;
	}
}

void CEneH0405Nancy::SetMissile()
{
	m_bMissile = gxTrue;
	m_MissileAdd.x = 100/(m_sSize+1);
	m_MissileAdd.y = 100/(m_sSize+1);
}
