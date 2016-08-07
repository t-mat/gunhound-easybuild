//------------------------------------------------------------
//
//エフェクト：マシンガン
//
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

extern gxSprite sprPixel;

CEffEneGun::CEffEneGun(Sint32 x , Sint32 y, Sint32 rot , Sint32 speed )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;

	m_sAdd_X = (viiMath::Cos100(rot)*speed)/100;
	m_sAdd_Y = (viiMath::Sin100(rot)*speed)/100;

	m_HitAtari.set_hp(1);
}


CEffEneGun::~CEffEneGun()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------
	
	
}


void CEffEneGun::SeqInit()
{

	m_HitAtari.ax1 = -1;
	m_HitAtari.ay1 = -1;
	m_HitAtari.ax2 =  1;
	m_HitAtari.ay2 =  1;

	SetActionSeq( enActionSeqMain );

}


void CEffEneGun::SeqMain()
{
	//----------------------------------------------
	//マシンガンメイン
	//----------------------------------------------

	m_Pos.x += m_sAdd_X;
	m_Pos.y += m_sAdd_Y;

	if(m_HitAtari.IsImpacted() || viiSub::GetBoundBlock_Player( m_Pos.x , m_Pos.y ) )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	//存在チェック
	if(IsScrollOut( 0,0) )
	{
		SetActionSeq( enActionSeqEnd );
	}

	m_HitAtari.SetHantei( ID_ENEMY_ATK , &m_Pos );
	m_HitAtari.SetDirectAromorPosition( 50 );

}


void CEffEneGun::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	viiEff::Set( EFF_HITFLASH , m_Pos.x , m_Pos.y , argset(1) );

	SetActionSeq( enActionSeqEnd );

}


void CEffEneGun::SeqEnd()
{
	
	
}


void CEffEneGun::Draw()
{
	//------------------------------------------------
	//描画
	//------------------------------------------------

	viiDraw::Sprite( &sprPixel , m_Pos.x , m_Pos.y , PRIO_ATACK , ATR_DEFAULT , ARGB_DFLT , 1.0f , 0 );

}


//------------------------------------------------------------
//
//エフェクト：ツイン砲
//
//------------------------------------------------------------


CEffEneTwinGun::CEffEneTwinGun(Sint32 x , Sint32 y, Sint32 rot , Sint32 speed )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_ATACK;
	m_sRotation = rot;
	m_sAdd_X = (viiMath::Cos100(rot)*speed)/100;
	m_sAdd_Y = (viiMath::Sin100(rot)*speed)/100;

	m_HitAtari.set_hp(1);

	m_HitAtari.ax1 = -1;
	m_HitAtari.ay1 = -1;
	m_HitAtari.ax2 =  1;
	m_HitAtari.ay2 =  1;

	m_bAtariOff = gxFalse;
	SetActionSeq( enActionSeqMain );

}


CEffEneTwinGun::~CEffEneTwinGun()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------
	
	
}


void CEffEneTwinGun::SeqInit()
{


}


void CEffEneTwinGun::SeqMain()
{
	//----------------------------------------------
	//マシンガンメイン
	//----------------------------------------------
	gxBool bHit = gxFalse;

	m_Pos.x += m_sAdd_X;
	m_Pos.y += m_sAdd_Y;

	if(m_HitAtari.IsImpacted() )
	{
		bHit = gxTrue;
	}

	if( viiSub::GetBoundBlock_Player( m_Pos.x , m_Pos.y ) )
	{
		bHit = gxTrue;
	}

	if( bHit )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	//存在チェック
	if(!m_bAtariOff && IsScrollOut( 128*100,64*100) )
	{
		SetActionSeq( enActionSeqEnd );
	}

	//残像

	CEffExPerticle *pPerticle;
	pPerticle = new CEffExPerticle( m_Pos.x, m_Pos.y , m_Pos.z+1 );
	pPerticle->m_sRotation = m_sRotation;
	pPerticle->SetSprite( &spr_ene_bullet[7] );
	pPerticle->m_fScale = 1.f;
	pPerticle->m_sAtribute = ATR_ALPHA_PLUS;
	pPerticle->SetEnd();
	pPerticle->SetAlphaAdd( -(32+viiSub::Rand()%8) );

	if( m_bAtariOff )
	{
		if( m_sTimer >= 60*30)
		{
			SetActionSeq( enActionSeqEnd );
		}
		return;
	}

	m_HitAtari.SetHantei( ID_ENEMY_ATK , &m_Pos );
	m_HitAtari.SetDirectAromorPosition( 50 );

}


void CEffEneTwinGun::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	viiEff::SetBombEffect(m_Pos.x,m_Pos.y,3,5);
	viiMus::PlaySound(SE_BOMB_SMALL);

	SetActionSeq( enActionSeqEnd );

}


void CEffEneTwinGun::SeqEnd()
{
	
	
}


void CEffEneTwinGun::Draw()
{
	//------------------------------------------------
	//描画
	//------------------------------------------------

	viiDraw::Sprite( &spr_ene_bullet[7] , m_Pos.x , m_Pos.y , m_Pos.z , ATR_DEFAULT , ARGB_DFLT , 1.0f , m_sRotation );

}




