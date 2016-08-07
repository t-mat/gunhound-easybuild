//--------------------------------------------------------------------------------
//
// Stage00:輸送船
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../Effect/EffectBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -30,
	enKuraiTop   = -34,
	enKuraiRight =  30,
	enKuraiBottom=  0,

	enSpeedMax = 40,
	enScore = 300,
};

gxSprite SprCEneH0207TransShips[]={
	{enTexPageEnemyStage02,0,0 ,512,144,512-64,144},
};

gxSprite SprCEneH0207TransItems[]={
	{enTexPageEnemyStage02,0,160 ,80,48,40,48},
	{enTexPageEnemyStage02,0,208 ,80,48,40,48},
	{enTexPageEnemyStage02,80,160,48,48,24,48},
	{enTexPageEnemyStage02,80,208,80,48,40,48},
	{enTexPageEnemyStage02,0,208 ,80,48,40,48},

//	{enTexPageEnemyStage04,112,0,80,48,40,48},
};


CEneH0207TransShips::CEneH0207TransShips( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_ENEMY-2;

	m_HitKurai.set_hp( HP_ARMORCLASS_B );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_Add.x = m_Add.y = 0;

	m_sDirection = DIR_RIGHT;
	m_sAtribute = ATR_DFLT;
	m_bMoveStop = gxTrue;

	m_sSpeed = enSpeedMax;

	m_bDamage = gxFalse;

	for(Sint32 ii=0;ii<enTransItemMax;ii++)
	{
		m_pCEneH0207TransItems[ii] = NULL;
	}
//	CEneH0207TransItems *p;
	m_pCEneH0207TransItems[0] = new CEneH0207TransItems( -12800+m_Pos.x , m_Pos.y-3200 , 0 );
	m_pCEneH0207TransItems[0]->SetTarget( &m_Pos );

	m_pCEneH0207TransItems[1] = new CEneH0207TransItems( -12800+m_Pos.x-6400   , m_Pos.y-3200 , 1 );
	m_pCEneH0207TransItems[1]->SetTarget( &m_Pos );

	m_pCEneH0207TransItems[2] = new CEneH0207TransItems( -12800+m_Pos.x-6400*2 , m_Pos.y-3200 , 2 );
	m_pCEneH0207TransItems[2]->SetTarget( &m_Pos );

	m_pCEneH0207TransItems[3] = new CEneH0207TransItems( +0+m_Pos.x , m_Pos.y-3200 , 2 );
	m_pCEneH0207TransItems[3]->SetTarget( &m_Pos );

	m_pCEneH0207TransItems[4] = new CEneH0207TransItems( +0+m_Pos.x-6400   , m_Pos.y-3200 , 3 );
	m_pCEneH0207TransItems[4]->SetTarget( &m_Pos );

	m_pCEneH0207TransItems[5] = new CEneH0207TransItems( +0+m_Pos.x-6400*2 , m_Pos.y-3200 , 4 );
	m_pCEneH0207TransItems[5]->SetTarget( &m_Pos );

}


CEneH0207TransShips::~CEneH0207TransShips()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------



}


void CEneH0207TransShips::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	m_HitKurai.set_hp( HP_ARMORCLASS_B );

	if( m_HitKurai.is_dead() )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	LogicAI();

	SetRader( enTypeMarkerDefenceTarget );
	DirectionAdjust();

	//くらい判定登録
//	m_HitKurai.SetHantei(ID_VERSIS_DEF , &m_Pos );

}


void CEneH0207TransShips::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	Sint32 sSpeedMax = m_sSpeed;

	if( !m_bMoveStop )
	{
		if( m_Add.x < sSpeedMax ) m_Add.x +=1;
		if( m_Add.x >= sSpeedMax )
		{
			m_Add.x = sSpeedMax;
		}

		m_Pos.x += m_Add.x;
	}

}


void CEneH0207TransShips::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}

gxBool CEneH0207TransShips::IsAllItemDead()
{
	//-----------------------------------------------------
	//アイテムが全て破壊されたか？
	//-----------------------------------------------------
	Sint32 sCnt = 0;

	for(Sint32 ii=0;ii<enTransItemMax;ii++)
	{
		if( m_pCEneH0207TransItems[ii] == NULL ) continue;
		if( m_pCEneH0207TransItems[ii]->IsDead() ) sCnt ++;
	}

	if( sCnt == enTransItemMax )
	{
		return gxTrue;
	}

	return gxFalse;
}


void CEneH0207TransShips::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	viiMus::PlaySound(enSoundExplosionMiddle);

	ScorePlus( enScore );

	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

	SetActionSeq( enActionSeqEnd );

}

void CEneH0207TransShips::SetOperation( Sint32 op , Sint32 dat )
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

	case enForceOpMoveAdd:	//現在位置の前方datドットへ進め
		break;

	default:
		break;
	}

}


void CEneH0207TransShips::Draw()
{

	viiDraw::Sprdmg( m_bDamage , &SprCEneH0207TransShips[ 0 ] , m_Pos.x , m_Pos.y , m_Pos.z , m_sAtribute , ARGB_DFLT );
	
}

//--------------------------------------------------------------------------------
//
// Stage02:アイテム
//
//--------------------------------------------------------------------------------

CEneH0207TransItems::CEneH0207TransItems( Sint32 x, Sint32 y ,Sint32 sItemType )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_ENEMY-3;

	m_Add.x = 0;
	m_Add.y = 0;

	m_HitKurai.set_hp( enDefPointBomb );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_bDamage = gxFalse;
	m_sAtribute = ATR_DFLT;
	m_sItemType = sItemType;

	m_pEffFire = NULL;
	m_pTarget  = NULL;
}


CEneH0207TransItems::~CEneH0207TransItems()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneH0207TransItems::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( m_HitKurai.is_dead() )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	m_bDamage = m_HitKurai.is_damage();
	if( m_bDamage )
	{
		m_bDamage  = gxFalse;
	}

	if( m_pTarget )
	{
		m_Pos.x = m_pTarget->x + m_Add.x;
		m_Pos.y = m_pTarget->y + m_Add.y;
	}

	DirectionAdjust();

	//くらい判定登録
	m_HitKurai.SetHantei(ID_VERSIS_DEF , &m_Pos );

}


void CEneH0207TransItems::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	if( m_pTarget )
	{
		m_Pos.x = m_pTarget->x + m_Add.x;
		m_Pos.y = m_pTarget->y + m_Add.y;
	}

	if( !m_bDead )
	{
		m_bDead = gxTrue;
		viiMus::PlaySound(enSoundExplosionMiddle);

		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
		m_pEffFire = new CEffFire( m_Pos.x , m_Pos.y-8*100 , PRIO_BG2+1);

		if( m_sItemType == 2 )
		{
			new CItemChip( m_Pos.x, m_Pos.y, enItemEnergyChip);
		}

	}

	if( m_pEffFire )
	{
		m_pEffFire->m_Pos.x = m_Pos.x;
	}

}


void CEneH0207TransItems::Draw()
{
	if( m_bDead ) return;

	viiDraw::Sprdmg( m_bDamage , &SprCEneH0207TransItems[ m_sItemType ] , m_Pos.x , m_Pos.y , m_Pos.z , m_sAtribute , ARGB_DFLT );

}

