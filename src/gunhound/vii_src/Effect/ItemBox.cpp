//--------------------------------------------------------------
//
//	アイテムボックス
//
//--------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

extern gxSprite spr_item[];

gxSprite SprItem[]=
{
	{enTexPageEnemyCommon08,0,0,16,16,8,16},	//箱
	{enTexPageEnemyCommon08,16,0,16,16,8,16},	//1://ゴメンヨ人形
	{TPG_EFFECT,0+16*2,0,16,16,8,8},			//2://HP

	{TPG_EFFECT,0+16*0,0,16,16,8,8},	//POW
	{TPG_EFFECT,0+16*1,0,16,16,8,8},	//WEAPON
	{TPG_EFFECT,0+16*2,0,16,16,8,8},	//HP
	{TPG_EFFECT,0+16*0,32,16,16,8,8},	//BOX
	{TPG_EFFECT,0+16*0,16,16,16,8,8},	//WAKU
};


enum {
	enScore = 50000,
};

CItemBox::CItemBox(Sint32 x,Sint32 y, Sint32 itemType)
{
	m_sStatItem = itemType;

	m_Pos.x = x;
	m_Pos.y = y;

	m_HitKurai.ax1 = -8;
	m_HitKurai.ay1 = -8;
	m_HitKurai.ax2 =  8;
	m_HitKurai.ay2 =  8;

	m_HitKurai.SetDirectHitPosition( 1 );
	m_HitKurai.SetDirectAromorPosition( 0 );

}


CItemBox::~CItemBox()
{
	
	
	
}


/*
void CItemBox::SeqInit()
{
	
	
}


void CItemBox::SeqMain()
{
	
	
}


*/

void CItemBox::SeqCrash()
{
	//-------------------------------------------------
	//破壊されアイテムチップに化ける
	//-------------------------------------------------

	new CItemChip( m_Pos.x , m_Pos.y , m_sStatItem );

	SetActionSeq( enActionSeqEnd );
}


void CItemBox::SeqMain()
{
	//------------------------------------------------
	//メインアクション
	//------------------------------------------------
	if(m_HitKurai.is_damage())
	{
		viiEff::SetBombSimple( m_Pos.x , m_Pos.y, 1,1);
		SetActionSeq( enActionSeqCrash );
		return;
	}

	m_HitKurai.SetHantei( ID_ENEMY_DEF , &m_Pos );

}


void CItemBox::Draw()
{
	//------------------------------------------------
	//アイテムボックス表示
	//------------------------------------------------

	if( viiSub::IsScrollOut( m_Pos.x , m_Pos.y , 32*100,32*100) ) return;

	Sint32 col = m_sTimer%100;
	col = abs( col - 50 );

	viiDraw::Sprite(&SprItem[m_sStatItem] , m_Pos.x , m_Pos.y , PRIO_EFFECT,ATR_DFLT,ARGB(0xf0,0xff,0xff,0xff));
	viiDraw::Sprite(&SprItem[m_sStatItem] , m_Pos.x , m_Pos.y , PRIO_EFFECT,ATR_ALPHA_PLUS,ARGB((col*3),0xff,0xff,0xff) );//,(col*2.5f)/100.0f);


}


//-----------------------------------------------------------------------------------------------------


CItemChip::CItemChip(Sint32 x,Sint32 y, Sint32 itemType)
{
	m_sSprite = 0;

	switch( itemType ){
	case 0:	//BONUS	//POW
		m_sSprite = 1;
		break;
	case 1:	//BONUS	//WEAPON
		m_sSprite = 1;
		break;
	case 2:	//HP
		m_sSprite = 2;
		break;
	}

	m_sStatItem = itemType;

	m_bChakuti = gxFalse;

	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.my = 0;

	if(pGame->is_gravit())	m_Pos.my = -300;

}


CItemChip::~CItemChip()
{
	
	
	
}


void CItemChip::SeqMain()
{
	//------------------------------------------------
	//メインアクション
	//------------------------------------------------

	if( isGet() )
	{
		//-----------------------------------------------------------
		//搾取された
		//-----------------------------------------------------------

		switch( m_sStatItem ){
		case 0:	//BONUS	//POW
			new CEffScore( m_Pos.x , m_Pos.y-800 , enScore );
			viiSub::AddScore( enScore );
			break;
		case 1:	//BONUS	//WEAPON
			break;
		case 2:	//HP
			get_item( m_sStatItem );
			break;
		}
		viiMus::PlaySound( enSoundHpPowerUp );
		SetActionSeq( enActionSeqEnd );
		return;
	}

	if(pGame->is_gravit() && !m_bChakuti )
	{
		//-----------------------------------------------------------
		//落下
		//-----------------------------------------------------------
		m_Pos.y += m_Pos.my;

		CObj *p = new CObj();

//		if( p->is_hit( m_Pos.x , m_Pos.y+800 ) && m_Pos.my>=300)
		if( p->is_hit( m_Pos.x , m_Pos.y ) && m_Pos.my>=300)
		{
//			while( p->is_hit( m_Pos.x , m_Pos.y+800) )
			while( p->is_hit( m_Pos.x , m_Pos.y) )
			{
				m_Pos.y -= 100;
			}
			m_bChakuti = gxTrue;
		}
		else
		{
			if( m_Pos.my <= 800)
			{
				m_Pos.my += 16;
			}
		}

		delete p;
	}

}


gxBool CItemChip::isGet()
{
	//------------------------------------------------
	//取得されたか？
	//------------------------------------------------

//	if( m_Pos.my>=100 || m_bChakuti)	//ちょっと間をおいてから出ないと取得できないとき
	{
		return ( get_valken_dist( m_Pos.x,m_Pos.y+1200) <= 18*100 )? gxTrue : gxFalse;
	}

	return gxFalse;
}


void CItemChip::Draw()
{
	//------------------------------------------------
	//アイテムチップ表示
	//------------------------------------------------

	if( viiSub::IsScrollOut( m_Pos.x , m_Pos.y , 32*100,32*100) ) return;

	viiDraw::Sprite(&SprItem[m_sSprite] , m_Pos.x , m_Pos.y , PRIO_EFFECT,ATR_DFLT,ARGB(0xf0,0xff,0xff,0xff));

	Sint32 col = m_sTimer%100;
	col = abs( col - 50 );

	viiDraw::Sprite(&SprItem[m_sSprite] , m_Pos.x , m_Pos.y , PRIO_EFFECT,ATR_ALPHA_PLUS,ARGB((col*3),0xff,0xff,0xff) );//,(col*2.5f)/100.0f);

}


