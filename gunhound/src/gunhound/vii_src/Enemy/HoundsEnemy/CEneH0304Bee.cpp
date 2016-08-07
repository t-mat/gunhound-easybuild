//--------------------------------------------------------------------------------
//
// Stage00:浮遊機雷
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../Effect/EffectBase.h"
//#include "EnemyTbl/CEneH0304BeeTBL.h"

enum {
	enScrollOutWidth  = 32*100,
	enScrollOutHeight = 32*100,

	enKuraiLeft  = -12,
	enKuraiTop   = -12,
	enKuraiRight =  12,
	enKuraiBottom=  12,

	enScore = 300,
};

gxSprite SprCEneH0304Bee[]=
{
	{enTexCEneH0304Bee, 0*16,224,16,16,8,8},
	{enTexCEneH0304Bee, 1*16,224,16,16,8,8},
	{enTexCEneH0304Bee, 2*16,224,16,16,8,8},
};

CEneH0304Bee::CEneH0304Bee( Sint32 x, Sint32 y ,gxBool bFlag )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_bScrollWait = bFlag;
	m_HitKurai.set_hp(HP_ARMORCLASS_A);

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;

	m_bDamage     = 0;
	m_sLogicTimer = viiSub::Rand()%240;
	m_sAnimeCount = 0;
	m_sBombWait   = 0;

	m_Add.x = m_Pos.x;
	m_Add.y = m_Pos.y;

}


CEneH0304Bee::~CEneH0304Bee()
{
	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneH0304Bee::SeqInit()
{
	//-----------------------------------------------------
	//スクロールインするまで待つ
	//-----------------------------------------------------

	if( m_bScrollWait && IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}

	SetActionSeq( enActionSeqMain );

}


void CEneH0304Bee::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if(m_HitKurai.is_dead() || m_HitAtari.IsImpacted())
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}


/*
*/

	m_bDamage = m_HitKurai.is_damage();

	LogicAI();

	m_Pos.x += (m_Add.x-m_Pos.x)/60;
	m_Pos.y += (m_Add.y-m_Pos.y)/60;

	//くらい判定登録
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

	//あたり判定登録
	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
	m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );

}


void CEneH0304Bee::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	if(m_sLogicTimer == 0)
	{
		Sint32 r = get_valken_rot(m_Pos.x,m_Pos.y+2400);
		m_Add.x = m_Pos.x + viiMath::Cos100(r)*(250*96/100);
		m_Add.y = m_Pos.y + viiMath::Sin100(r)*(250*96/100);

	}
	else if(m_sLogicTimer < 40)
	{
		if(m_sAnimeCount > 0) m_sAnimeCount --;

	}
	else if(m_sLogicTimer < 80)
	{
		if(m_sAnimeCount < 16) m_sAnimeCount ++;
	}

	if( m_sAnimeCount )
	{
		m_CEneCmnSignal.On();
	}
	m_CEneCmnSignal.SetSignal( m_Pos.x , m_Pos.y , CEneCmnSignal::enSignalTypeNORMAL);

	m_sLogicTimer ++;
	m_sLogicTimer = m_sLogicTimer%90;

}


void CEneH0304Bee::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CEneH0304Bee::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	viiMus::PlaySound(enSoundExplosionMiddle);
	ScorePlus( enScore );

	EffectMakeBigBomb(m_Pos.x , m_Pos.y);
	viiEff::SetBombEffect(m_Pos.x,m_Pos.y,8,5);

	SetActionSeq( enActionSeqEnd );


}

void CEneH0304Bee::Draw()
{
	//-----------------------------------------------------
	//表示
	//-----------------------------------------------------

	Sint32 yy = m_sAnimeCount;

	yy = abs( -16+yy );
	yy *= 20;

	m_CEneCmnSignal.Draw();

	viiDraw::Sprdmg( m_bDamage ,&SprCEneH0304Bee[1] , m_Pos.x , m_Pos.y   ,PRIO_ENEMY , ATR_DFLT , ARGB_DFLT );
	viiDraw::Sprdmg( m_bDamage ,&SprCEneH0304Bee[0] , m_Pos.x , m_Pos.y-yy,PRIO_ENEMY , ATR_DFLT , ARGB_DFLT );
	viiDraw::Sprdmg( m_bDamage ,&SprCEneH0304Bee[2] , m_Pos.x , m_Pos.y+yy,PRIO_ENEMY , ATR_DFLT , ARGB_DFLT );


}

