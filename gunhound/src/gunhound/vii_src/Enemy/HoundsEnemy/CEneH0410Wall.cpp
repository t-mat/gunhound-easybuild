//--------------------------------------------------------------------------------
//
// Stage00:スライドウオール
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
//#include "EnemyTbl/CEneH0410WallTBL.h"
#include "../../Effect/EffectBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -26,
	enKuraiTop   = -640,
	enKuraiRight =  26,
	enKuraiBottom=  0,

	enScore = 20000,
};


gxSprite SprCEneH0410Wall[]=
{
	{enTexCEneH0007YusouHeli,64,176,32,80,16,80},	//壁本体（普通）
	{enTexCEneH0007YusouHeli,96+64,176,32,80,16,80},//壁本体（破壊）

	{enTexCEneH0007YusouHeli,0 ,176,64,80,32,80},	//カバー本体（普通）
	{enTexCEneH0007YusouHeli,96+0 ,176,64,80,32,80},//カバー本体（破壊）

	{enTexCEneH0007YusouHeli,192 ,176,24,48,12,24},//破壊後の破片

};


CEneH0410Wall::CEneH0410Wall( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_bCoverClosed = gxFalse;

	m_HitKurai.set_hp( 100*30 );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;
	m_HitAtari.set_hp( 100*100 );
	m_HitAtari.set_ap( 100 );


	m_sDirection = DIR_LEFT;

	m_bCrash   = gxFalse;
	m_bCover   = gxFalse;
	m_bDamage  = gxFalse;
	m_bMaximum = gxFalse;

	m_sOffset = -WINDOW_W*100;

	m_bForceCrash = gxFalse;

	for(Sint32 ii=0;ii<enWallLength; ii++)
	{
		m_sCrashLv[ii] = 0;
	}

 	m_pCEneCmnSpark = new CEneCmnSpark(m_Pos.x,m_Pos.y,m_Pos.x,m_Pos.y-3200*100);

}


CEneH0410Wall::~CEneH0410Wall()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

	if(m_pCEneCmnSpark) delete m_pCEneCmnSpark;
}


void CEneH0410Wall::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------
	if( m_bForceCrash )
	{
		//強制クラッシュ（爆薬使用でクラッシュ）
		SetActionSeq( enActionSeqCrash );
		return;
	}


	if( !m_bCover )
	{
		//自力で壊した
		if( IsMaximum() &&  m_HitKurai.is_dead() )
		{
			SetCrash(0);
			ScorePlus( enScore );
			SetActionSeq( enActionSeqCrash );
			return;
		}
	}

	LogicAI();

//	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	m_bDamage = m_HitKurai.is_damage();

	if( m_bCoverClosed )
	{
		m_pCEneCmnSpark->Set(m_Pos.x,m_Pos.y,m_Pos.x,m_Pos.y-320*100);
		m_bDamage = gxFalse;
	}

	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

}


void CEneH0410Wall::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	if( viiSub::IsGroundBlock(m_Pos.x , m_Pos.y) )
	{
		//---------------------------------------------
		//着地するまで下がる（家壁）
		//---------------------------------------------
		if(!m_bMaximum)
		{
			pGame->QuakeOn(32);
			m_bMaximum = gxTrue;
			for(Sint32 ii=0;ii<8;ii++)
			{
				new CEffDashStorm( m_Pos.x+(-32+viiSub::Rand()%64)*100 , m_Pos.y, 180 , 60 );
			}
		}
	}
	else
	{
		for(Sint32 ii=-4;ii<5; ii++)
		{
			pGame->pBg->make_vmptip( m_Pos.x/800 +ii, m_Pos.y/800-1 );
		}
		m_Pos.y += 164;
	}

	if( m_bCover )
	{
		//---------------------------
		//シャッター処理
		//---------------------------
		if( m_sOffset < 0 )
		{
			m_sOffset += 164;
			if(m_sOffset >= 0)
			{
				pGame->QuakeOn(32);
				m_sOffset = 0;
				m_bCoverClosed = gxTrue;
			}
		}
	}

}


void CEneH0410Wall::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CEneH0410Wall::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	Sint32 ii;

	if( !m_bDead )
	{
		m_bDead = gxTrue;

		viiMus::PlaySound(enSoundExplosionMiddle);
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

		//--------------------------------------------------
		for(ii=0;ii<enWallLength; ii++)
		{
			Sint32 y;
			y = m_Pos.y - ii*8000;
			y += m_sOffset;

			if( m_sCrashLv[ii] )
			{
				Sint32 y;
				y = m_Pos.y - ii*8000;
				y += m_sOffset;

				CEffectDmgprt *p;
				p = new CEffectDmgprt( m_Pos.x, y-20*100 , 180+85 , 300 );
				p->SetSprite( &SprCEneH0410Wall[ 4 ] );

				p = new CEffectDmgprt( m_Pos.x, y-30*100 , 180+65 , 400 );
				p->SetSprite( &SprCEneH0410Wall[ 4 ] );

				p = new CEffectDmgprt( m_Pos.x, y-40*100, 180+35 , 500 );
				p->SetSprite( &SprCEneH0410Wall[ 4 ] );

				p = new CEffectDmgprt( m_Pos.x, y-50*100, 180+12 , 400 );
				p->SetSprite( &SprCEneH0410Wall[ 4 ] );
			}
		}

		for(Sint32 jj=1;jj<(256/8); jj++)
		{
			for(ii=-4;ii<5; ii++)
			{
				pGame->pBg->del_vmptip( m_Pos.x/800 +ii, m_Pos.y/800-jj);
			}
		}
	}

//	SetActionSeq( enActionSeqEnd );

}


void CEneH0410Wall::SetCrash(Sint32 sLv)
{
	//-----------------------------------------------------
	//壁を壊す
	//-----------------------------------------------------

	Sint32 lv;
	Sint32 y;

	viiMus::PlaySound(enSoundExplosionLarge);

	m_bCrash = gxTrue;

	for(Sint32 ii=-1; ii<2; ii++)
	{
		lv = sLv+ii;
		if(lv<0 || lv>=enWallLength) continue;
		if( m_sCrashLv[lv] == 2) continue;
		m_sCrashLv[lv] = 1;

		y = m_Pos.y - ii*8000;
		y += m_sOffset;

		for(Sint32 ii=-4;ii<5; ii++)
		{
			pGame->pBg->make_vmptip( m_Pos.x/800 +ii, m_Pos.y/800-1 ,3);
		}

		CEffectDmgprt *p;
		p = new CEffectDmgprt( m_Pos.x, y-20*100 , 180-25 , 800 );
		p->SetSprite( &SprCEneH0410Wall[ 4 ] );

		p = new CEffectDmgprt( m_Pos.x, y-30*100 , 180-45 , 900 );
		p->SetSprite( &SprCEneH0410Wall[ 4 ] );

		p = new CEffectDmgprt( m_Pos.x, y-40*100, 180+35 , 700 );
		p->SetSprite( &SprCEneH0410Wall[ 4 ] );

		p = new CEffectDmgprt( m_Pos.x, y-50*100, 180+12 , 800 );
		p->SetSprite( &SprCEneH0410Wall[ 4 ] );

	}

	m_sCrashLv[sLv] = 2;


}


void CEneH0410Wall::SetCover()
{
	//-----------------------------------------------------
	//強化壁をセット
	//-----------------------------------------------------

	m_bCover = gxTrue;

}


void CEneH0410Wall::Draw()
{
	//-----------------------------------------------------
	//描画
	//-----------------------------------------------------
	Sint32 y,sSpr;

	for(Sint32 ii=0;ii<enWallLength; ii++)
	{
		if( m_bCrash )
		{
			if( m_bDead ) continue;
			sSpr = 1;
		}
		else
		{
			sSpr = 0;
		}
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0410Wall[ sSpr ] , m_Pos.x , m_Pos.y - ii*8000 , PRIO_ENEMY , ATR_DFLT , ARGB_DFLT );
	}

	if( m_bCover )
	{
		for(Sint32 ii=0;ii<enWallLength; ii++)
		{
			y = m_Pos.y - ii*8000;
			y += m_sOffset;

			switch( m_sCrashLv[ii] ){
			case 2:
				continue;
			case 1:
				sSpr = 1;
				break;
			case 0:
				sSpr = 0;
				break;
			}

			viiDraw::Sprdmg( m_bDamage , &SprCEneH0410Wall[ 2+sSpr ] , m_Pos.x , y , PRIO_ENEMY , ATR_DFLT , ARGB_DFLT );
		}
	}

	if( !m_bCrash && m_bCoverClosed )
	{
		m_pCEneCmnSpark->Draw();
	}
}


CEneH0410Wall2::CEneH0410Wall2( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;


	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;
	m_HitKurai.set_hp( 8*100 );

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;
	m_HitAtari.set_hp( 1 );
	m_HitAtari.set_ap( 100 );


	m_sDirection = DIR_LEFT;

	m_bDamage  = gxFalse;
	m_sOffset = -WINDOW_W*100;

}


CEneH0410Wall2::~CEneH0410Wall2()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneH0410Wall2::SeqMain()
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

	m_bDamage = m_HitKurai.is_damage();

	m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

}


void CEneH0410Wall2::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	while( !viiSub::IsGroundBlock( m_Pos.x , m_Pos.y ) )
	{
		//着地するまで下がる
		m_Pos.y += 164;
	}

}


void CEneH0410Wall2::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEneH0410Wall2::Draw()
{
	//-----------------------------------------------------
	//描画
	//-----------------------------------------------------
	Sint32 sSpr=0;

	for(Sint32 ii=0;ii<enWallLength; ii++)
	{
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0410Wall[ sSpr ] , m_Pos.x , m_Pos.y - ii*8000 , PRIO_ENEMY , ATR_DFLT , ARGB_DFLT );
	}

}

