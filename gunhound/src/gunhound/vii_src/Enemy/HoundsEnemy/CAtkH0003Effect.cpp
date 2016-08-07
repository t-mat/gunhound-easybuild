//--------------------------------------------------------------------------------
//
// Stage共通:エフェクト関連
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../Effect/EffectBase.h"

enum {
	enKidouFlickerLeft,
	enKidouFlickerRight,
	enKidouKuruKuruStraight,
	enKidouKuruKuruHook,
	enKidouMax,
};


enum {
	enScrollOutWidth  = 64*100,
	enScrollOutHeight = 64*100,

	enKuraiLeft  = -4,
	enKuraiTop   = -4,
	enKuraiRight =  4,
	enKuraiBottom=  4,

	enSpeedMax = 420,
	enHomingSpeed = 32,
	enRotationAdd = 16,
};

gxSprite stSprEffGroundSmoke[] = {
	{TPG_CLOUD,0 ,0,64,64,32,32},	//雲
};

gxSprite stSprEffBlackSmoke2[] = {
	{TPG_CLOUD,0 ,0,64,64,32,32},	//黒雲
};

gxSprite stSprEffBossFire[] = {
	{ enTexPageEffectCommon02 ,192 ,160,64,96,32,96},	//長い炎
	{ enTexPageEffectCommon02 ,128 ,160,64,32,32,32},	//横の炎
};
//gxSprite SprCAtkH0003EffectHitSmoke[]=
//{
//	{TPG_LEYNOS01,0,192,64,64,32,64},//ボルゾイ
//};

gxSprite SprBurniaEffect[]=
{
//	{enTexPageGunHound,32,128  ,32,32,0,16}, //バーニア
	{enTexPageDmyHound,0,96  ,48,32,0,16},		//バーニア
	{enTexPageGunHoundBdy,192,64  ,48,32,0,16}, //バーニア(バグ版)

};

gxSprite SprDamagePart[]={
	{enTexPageEffect1,80+16*0,128+16*0,16,16,8,8},
	{enTexPageEffect1,80+16*1,128+16*0,16,16,8,8},
	{enTexPageEffect1,80+16*2,128+16*0,16,16,8,8},
	{enTexPageEffect1,80+16*3,128+16*0,16,16,8,8},
	{enTexPageEffect1,80+16*4,128+16*0,16,16,8,8},
	{enTexPageEffect1,80+16*5,128+16*0,16,16,8,8},
	{enTexPageEffect1,80+16*6,128+16*0,16,16,8,8},

	{enTexPageEffect1,80+16*0,128+16*1,16,16,8,8},
	{enTexPageEffect1,80+16*1,128+16*1,16,16,8,8},
	{enTexPageEffect1,80+16*2,128+16*1,16,16,8,8},
	{enTexPageEffect1,80+16*3,128+16*1,16,16,8,8},
	{enTexPageEffect1,80+16*4,128+16*1,16,16,8,8},
};

extern gxSprite spr_cloud[];

CAtkH0003EffectHitSmoke::CAtkH0003EffectHitSmoke( Sint32 x, Sint32 y , Sint32 sRot , Sint32 sSpeed)
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_sRotation  = -90;//sRot-8+viiSub::Rand()%16;
	m_sMiseRotation = m_sRotation;
	m_sWait      = 16+viiSub::Rand()%16;
	m_sKidou     = viiSub::Rand()%enKidouMax;
}


CAtkH0003EffectHitSmoke::~CAtkH0003EffectHitSmoke()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CAtkH0003EffectHitSmoke::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( viiSub::IsGroundBlock(m_Pos.x,m_Pos.y) )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	if( m_sWait > 0)
	{
		m_sWait -- ;
	}
	else
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	LogicAI();

	viiSub::SetGravitMove(m_Pos.x , m_Pos.y);
}


void CAtkH0003EffectHitSmoke::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	if( m_sWait > 0 )
	{
		m_sWait --;
	}

	switch( m_sKidou ){
	case enKidouFlickerLeft:
		m_sMiseRotation = m_sRotation+viiMath::Cos100( (m_sTimer*2)%360 )*8;
		break;
	case enKidouFlickerRight:
		m_sMiseRotation = m_sRotation+viiMath::Sin100( (m_sTimer*2)%360 )*8;
		break;

	case enKidouKuruKuruStraight:
		m_sMiseRotation = m_sRotation+(m_sTimer*2)%360;
		break;

	case enKidouKuruKuruHook:
		m_sMiseRotation = m_sRotation+(m_sTimer*16)%360;
		break;
	}

	CAtkH0003EffectSmoke *p = new CAtkH0003EffectSmoke( m_Pos.x , m_Pos.y );
	p->SetDirection( m_sMiseRotation+180 , 30 );

	m_Pos.y -= 160;
}


void CAtkH0003EffectHitSmoke::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	SetActionSeq( enActionSeqEnd );

}

void CAtkH0003EffectHitSmoke::Draw()
{

}


//--------------------------------------------------------------------------------
//
// Stage00:スモーク
//
//--------------------------------------------------------------------------------


CAtkH0003EffectSmoke::CAtkH0003EffectSmoke( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_fScale     = (300+viiSub::Rand()%200)/5000.f;// 0.05f;

	m_sSpeed    = 0;
	m_sRotation = 0;
	m_sAlpha    = 128+viiSub::Rand()%64;

	m_Add.x = 0;
	m_Add.y = 0;
}


CAtkH0003EffectSmoke::~CAtkH0003EffectSmoke()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CAtkH0003EffectSmoke::SetDirection( Sint32 sRotation , Sint32 sSpeed )
{
	m_sSpeed    = sSpeed;
	m_sRotation = sRotation;
}


void CAtkH0003EffectSmoke::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	LogicAI();

	if( m_sAlpha <= 0 )
	{
		m_sAlpha = 0;
		SetActionSeq( enActionSeqCrash );
	}

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	viiSub::SetGravitMove(m_Pos.x , m_Pos.y);
}


void CAtkH0003EffectSmoke::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	if( m_sSpeed > 0 ) m_sSpeed --;
	if( m_sAlpha > 0 ) m_sAlpha -= 3;

	m_Add.x = (viiMath::Cos100(m_sRotation)*m_sSpeed)/100;
	m_Add.y = (viiMath::Sin100(m_sRotation)*m_sSpeed)/100;

	m_fScale += 0.001f;

}


void CAtkH0003EffectSmoke::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CAtkH0003EffectSmoke::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	SetActionSeq( enActionSeqEnd );

}

void CAtkH0003EffectSmoke::Draw()
{

	Sint32 sAlpha = m_sAlpha*2;
	if( sAlpha >= 255 ) sAlpha = 255;

	viiDraw::Sprite( &spr_cloud[ 0 ] , m_Pos.x , m_Pos.y , PRIO_EFFECT , ATR_ALPHA_PLUS , ARGB(sAlpha,0xff,0xff,0xff) , m_fScale );

}



//--------------------------------------------------------------------------------
//
// Stage共通:バーニアエフェクト
//
//--------------------------------------------------------------------------------

CEffectBurnia::CEffectBurnia(Sint32 x , Sint32 y , Sint32 sRot ,Sint32 sSpeed)
{
	m_Pos.x = x;
	m_Pos.y = y;

	m_fScale     = (80+viiSub::Rand()%40)/100.f;

	m_sSpeed    = sSpeed;
	m_sRotation = sRot;
	m_sAlpha    = 128+viiSub::Rand()%64;

	m_Add.x = 0;
	m_Add.y = 0;

	m_pPos = NULL;
	m_pDirection = NULL;

	SetActionSeq( enActionSeqMain );
}


CEffectBurnia::~CEffectBurnia()
{
	
	
	
}


void CEffectBurnia::SeqMain()
{
	LogicAI();

	if( m_sAlpha <= 0 )
	{
		m_sAlpha = 0;
		SetActionSeq( enActionSeqCrash );
	}

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	viiSub::SetGravitMove(m_Pos.x , m_Pos.y);
}

void CEffectBurnia::LogicAI()
{
	m_sSpeed += (0-m_sSpeed)/10;
	if( m_sAlpha > 0 ) m_sAlpha -= 16;

	m_Add.x = (viiMath::Cos100(m_sRotation)*m_sSpeed)/100;
	m_Add.y = (viiMath::Sin100(m_sRotation)*m_sSpeed)/100;

	m_fScale += 0.02f;
}


void CEffectBurnia::SeqCrash()
{

	SetActionSeq( enActionSeqEnd );

}


void CEffectBurnia::Draw()
{
	Sint32 x,y,atr=ATR_DFLT;
	Sint32 sAlpha = m_sAlpha*2;
	if( sAlpha >= 255 ) sAlpha = 255;

	x = m_Pos.x;
	y = m_Pos.y;

	if( m_pDirection )
	{
		if( *m_pDirection == DIR_LEFT)
		{

			x = -x;
			atr = ATR_FLIP_X;
		}
	}

	if( m_pPos )
	{
		x += m_pPos->x;
		y += m_pPos->y;
	}
	viiDraw::Sprite( &SprBurniaEffect	[ 0 ] , x , y , PRIO_PLAYER-1 , atr|ATR_ALPHA_PLUS , ARGB(sAlpha,0xff,0xff,0xff) , m_fScale ,m_sRotation );

	
}


//--------------------------------------------------------------------------------
//
// Stage共通:ダメージパーツ
//
//--------------------------------------------------------------------------------


CEffectDmgprt::CEffectDmgprt(Sint32 x , Sint32 y , Sint32 sRot ,Sint32 sSpeed)
{
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_BG3-1;

	m_Add.x = (viiMath::Cos100(sRot)*sSpeed)/100;
	m_Add.y = (viiMath::Sin100(sRot)*sSpeed)/100;

	m_sSprite   = viiSub::Rand()%12;
	m_sRotation = viiSub::Rand()%360;

	m_sBoundCnt = 0;
	m_sScale = 120;

	m_pSprite = NULL;
	m_sAtribute = 0;
	m_bHeavy = gxFalse;
	m_bSmokeDisp = gxTrue;
	SetActionSeq( enActionSeqMain );
}


CEffectDmgprt::~CEffectDmgprt()
{



}


void CEffectDmgprt::SeqMain()
{
	if( m_pSprite == NULL )
	{
		m_pSprite = &SprDamagePart[ m_sSprite ];
	}

	LogicAI();

	if( m_sBoundCnt>3 || ( m_bHeavy && m_sBoundCnt) )
	{
		SetActionSeq( enActionSeqEnd );
	}

	if( pGame->GetMarineLine() && m_Pos.y >= pGame->GetMarineLine() )
	{
		new CEffWater( m_Pos.x , m_Pos.y, enTypeSplashSmall , 0);
		viiMus::PlaySound( enSoundSplashS );
		SetActionSeq( enActionSeqEnd );
	}

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	if( m_bHeavy )
	{
		m_sRotation += 1;
	}
	else
	{
		m_sRotation += 4;
	}

//	new CAtkH0003EffectHitSmoke( m_Pos.x, m_Pos.y , m_sTimer%360 , 80 );
	if(m_sBoundCnt==0 && m_sTimer%3 == 0)
	{
		if( m_bSmokeDisp )
		{
			CAtkH0001MslSmoke *p = new CAtkH0001MslSmoke( m_Pos.x , m_Pos.y );
		//	p->SetDirection( viiMath::Cos100((m_sTimer*8)%360)*360 , 120 );
			p->SetDirection( (m_sTimer*8)%360 , 80 );
			p->SetClearSpeed( 1 );
			p->SetScale(m_sScale);
			m_sScale -= 4;
		}
	}

	viiSub::SetGravitMove(m_Pos.x , m_Pos.y);
}

void CEffectDmgprt::LogicAI()
{

	if( viiSub::GetBoundBlock_Player( m_Pos.x , m_Pos.y ) )
	{
		m_Pos.y = m_Pos.y/100;
		m_Pos.y = (m_Pos.y/8)*800;
		m_Add.y = (Sint32)(m_Add.y*-0.5f);
		m_Add.x = (Sint32)(m_Add.x*0.75f);
		m_sBoundCnt ++;
	}
	else
	{
		if( m_Add.y < 800 )
		{
			m_Add.y += 16;
		}
	}

}


void CEffectDmgprt::SeqCrash()
{

	SetActionSeq( enActionSeqEnd );

}


void CEffectDmgprt::Draw()
{
	if( m_pSprite == NULL ) return;

	viiDraw::Sprite( m_pSprite , m_Pos.x , m_Pos.y , m_Pos.z , ATR_DFLT|m_sAtribute, ARGB_DFLT , 1.f ,m_sRotation );
}

//--------------------------------------------------------------------------------
//
// Stage共通:土石流
//
//--------------------------------------------------------------------------------


CEffectDosekiryu::CEffectDosekiryu(Sint32 x , Sint32 y , Sint32 sRot ,Sint32 sSpeed ,Sint32 sReverse)
{
	m_Pos.x = x;
	m_Pos.y = y;

	m_Add.x = (viiMath::Cos100(sRot)*sSpeed)/100;
	m_Add.y = (viiMath::Sin100(sRot)*sSpeed)/100;

	m_sSprite   = viiSub::Rand()%12;
	m_sRotation = viiSub::Rand()%360;

	m_sBoundCnt = 0;
	m_sScale = 120;

	m_pSprite = NULL;
	m_sAtribute = 0;
	m_bHeavy = gxFalse;
	m_sReverse = sReverse;
	SetActionSeq( enActionSeqMain );
}


CEffectDosekiryu::~CEffectDosekiryu()
{



}


void CEffectDosekiryu::SeqMain()
{
	if( m_pSprite == NULL )
	{
		m_pSprite = &SprDamagePart[ m_sSprite ];
	}

	LogicAI();

	if( m_sBoundCnt>3 || ( m_bHeavy && m_sBoundCnt) )
	{
		SetActionSeq( enActionSeqEnd );
	}

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	if( m_bHeavy )
	{
		m_sRotation += 1;
	}
	else
	{
		m_sRotation += 4;
	}

}

void CEffectDosekiryu::LogicAI()
{

	if( viiSub::GetBoundBlock_Player( m_Pos.x , m_Pos.y ) )
	{
		m_Pos.y = m_Pos.y/100;
		m_Pos.y = (m_Pos.y/8)*800;
		if( m_sReverse == DIR_LEFT )
		{
			m_Pos.y += 800;
		}
		m_Add.y = (Sint32)(m_Add.y*-0.5f);
		m_Add.x = (Sint32)(m_Add.x*0.75f);
		m_sBoundCnt ++;
	}
	else
	{
		if( m_sReverse == DIR_LEFT )
		{
			if( m_Add.y > -800 )
			{
				m_Add.y -= 16;
			}
		}
		else
		{
			if( m_Add.y < 800 )
			{
				m_Add.y += 16;
			}
		}
	}

}


void CEffectDosekiryu::SeqCrash()
{

	SetActionSeq( enActionSeqEnd );

}


void CEffectDosekiryu::Draw()
{
	if( m_pSprite == NULL ) return;

	viiDraw::Sprite( m_pSprite , m_Pos.x , m_Pos.y , PRIO_BG3-1 , ATR_DFLT|m_sAtribute, ARGB_DFLT , 1.f ,m_sRotation );
}



//--------------------------------------------------------------------------------
//
// Stage共通:最終大爆発
//
//--------------------------------------------------------------------------------
gxSprite SprCEffectFinalExplosion[]={
	{ enTexPageEffect3, 128,0 ,64,64,32,32},	//オレンジの光
	{ enTexPageEffect3, 192,0 ,64,64,32,32},	//白いリング
	{ enTexPageEffect3,0,80,96,96,48,48},	//レンズフレア
};

CEffectFinalExplosion::CEffectFinalExplosion(Sint32 x , Sint32 y,Sint32 sAddScale)
{
	m_Pos.x = x;
	m_Pos.y = y;

	m_Add;

	m_fAddScale = (Float32)sAddScale;
	m_sRotation = 0;
	m_fScale    = 3.f+m_fAddScale;
	m_sSeq      = 0;
	m_sWait     = 0;

	m_bFlair = gxFalse;
	m_fFlairScale = 1.25f+m_fAddScale;
	m_fFlairCol = 1.f;
	m_sFlairRot = 0;

	m_fFadeCol  = ZERO;

	m_fOrangeCnt =270;
	SetActionSeq( enActionSeqMain );
}


CEffectFinalExplosion::~CEffectFinalExplosion()
{



}


void CEffectFinalExplosion::SeqMain()
{
	LogicAI();


}

void CEffectFinalExplosion::LogicAI()
{
	gxBool bOrange = gxFalse;

	switch( m_sSeq ){
	case 0:
		m_sOrangeCol = 0;
		m_fOrangeCnt = -90.f;
		m_sWait = 0;
		m_sSeq = 100;
		break;

	case 100:
		bOrange = gxTrue;
		break;

	case 200:
		if( m_sWait> 60*10 )
		{
			SetActionSeq( enActionSeqEnd );
		}
		break;

	}

	if( bOrange )
	{
		//-----------------------------
		//オレンジ制御
		//-----------------------------

		if(m_fOrangeCnt<90.f)
		{
			m_fOrangeCnt += 0.95f;
		}
		if(m_fOrangeCnt > 0.f)
		{
			m_bFlair = gxTrue;
		}
		if(m_fOrangeCnt > -2.5f && m_fOrangeCnt < 45.f )
		{

//			if( m_fOrangeCnt < 1.f ) m_fFadeCol = 1.f;
//			if( m_fOrangeCnt < 2.5f ) m_fOrangeCnt = 2.5f;
			Sint32 ax = -64+viiSub::Rand()%128;
			Sint32 ay = -64+viiSub::Rand()%128;
			viiEff::SetBombSimple( m_Pos.x+ax*100,m_Pos.y + ay*100 , 3 ,1 );
		}

		if(m_fOrangeCnt >= 90.f)
		{
			m_fOrangeCnt = 90.f;
		}
		Sint32 cnt = (Sint32)(m_fOrangeCnt);
		m_fScale = -(8+m_fAddScale)*viiMath::Sin100(cnt)/100.f;
		m_sOrangeCol = 0xFF*gxAbs(viiMath::Cos100(cnt))/100;
	}

	if( m_bFlair )
	{
		m_sFlairRot --;
		m_fFlairCol -= 0.01f;
		m_fFlairScale += 0.025f;

		if(m_fFlairCol <= 0.f)
		{
			m_fFlairCol = ZERO;
			SetActionSeq( enActionSeqEnd );
		}
	}

	if( m_fFadeCol > 0)
	{
		m_fFadeCol -= 0.025f;
		if( m_fFadeCol < 0)
		{
			m_fFadeCol = ZERO;
		}

	}

	m_sWait++;
}


void CEffectFinalExplosion::SeqCrash()
{

	SetActionSeq( enActionSeqEnd );

}


void CEffectFinalExplosion::Draw()
{
	Sint32 sFadeCol = (Sint32)(0x40*m_fFadeCol);

	viiDraw::Box(0,0,WINDOW_W,WINDOW_H,PRIO_EFFECT,gxTrue,ARGB(sFadeCol,0xff,0xff,0xff));

	viiDraw::Sprite( &SprCEffectFinalExplosion[0] , m_Pos.x , m_Pos.y , PRIO_EFFECT , ATR_ALPHA_PLUS, ARGB((m_sOrangeCol/2), 0xff,0xff,0xff) ,6.f+m_fScale*2.f);//,  );
	viiDraw::Sprite( &SprCEffectFinalExplosion[0] , m_Pos.x , m_Pos.y , PRIO_EFFECT , ATR_ALPHA_PLUS, ARGB((m_sOrangeCol), 0xff,0xff,0xff) , m_fScale*2.f );
	viiDraw::Sprite( &SprCEffectFinalExplosion[1] , m_Pos.x , m_Pos.y , PRIO_EFFECT , ATR_ALPHA_PLUS, ARGB((m_sOrangeCol), 0xff,0xff,0xff) , m_fScale*0.75f );
	viiDraw::Sprite( &SprCEffectFinalExplosion[2] , m_Pos.x , m_Pos.y , PRIO_EFFECT , ATR_ALPHA_PLUS, ARGB((m_sOrangeCol), 0xff,0xff,0xff) , m_fScale ,m_sTimer);

	if( m_bFlair )
	{
		Sint32 sCol = (Sint32)(0xFF*m_fFlairCol);
		viiDraw::Sprite( &SprCEffectFinalExplosion[2] , m_Pos.x , m_Pos.y , PRIO_EFFECT , ATR_ALPHA_PLUS, ARGB((sCol), 0xff,0xff,0xff) , m_fFlairScale ,m_sFlairRot);
	}

}


//--------------------------------------------------------------------------------
//
// Stage共通:黒爆炎エフェクト関連
//
//--------------------------------------------------------------------------------

CEffectBlackExplosion::CEffectBlackExplosion(Sint32 x , Sint32 y , ghPos* pPos)
{
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_BG1+1;
	m_pPos = pPos;
}


CEffectBlackExplosion::~CEffectBlackExplosion()
{

}


void CEffectBlackExplosion::SeqMain()
{
//	if( m_sTimer%8==0 )
	{
//		for(Sint32 ii=0;ii<6;ii++)
		{
			Sint32 ax,ay;
			ax = m_Pos.x+(-32+viiSub::Rand()%80)*100;
			ay = m_Pos.y+(-64+viiSub::Rand()%64)*100;

			CEffectBlackFire *p;
			p = new CEffectBlackFire(ax,ay,m_pPos );
			p->m_Pos.z = m_Pos.z;
		}
	}
	viiSub::SetGravitMove(m_Pos.x , m_Pos.y);
}


void CEffectBlackExplosion::SeqCrash()
{

}


void CEffectBlackExplosion::Draw()
{

}


//--------------------------------------------------------------------------------
// Stage共通:黒爆炎エフェクト関連
//--------------------------------------------------------------------------------
CEffectBlackFire::CEffectBlackFire(Sint32 x , Sint32 y , ghPos* pPos)
{
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_BG1+1;
	m_Add.x = 150+viiSub::Rand()%200;
	m_Add.y = 200+viiSub::Rand()%32;

	m_sAlpha = 150+viiSub::Rand()%32;
	m_pPos = pPos;
}


CEffectBlackFire::~CEffectBlackFire()
{

}


void CEffectBlackFire::SeqMain()
{

	if( m_Add.y > 0 )
	{
		m_Add.y -= 1;
	}

	if( m_Add.x > 0 )
	{
		m_Add.x -= 2;
	}
	else if( m_Add.x < 0 )
	{
		m_Add.x += 2;
	}
	m_Pos.x += m_Add.x;
	m_Pos.y -= m_Add.y;

	if( m_sAlpha >= 0 )
	{
		m_sAlpha -= 1;
		if( m_sAlpha < 0 )
		{
			m_sAlpha = 0;
			SetActionSeq( enActionSeqEnd );
		}
	}
	viiSub::SetGravitMove(m_Pos.x , m_Pos.y);
}


void CEffectBlackFire::SeqCrash()
{

}


void CEffectBlackFire::Draw()
{

	Sint32 ax,ay;

	if( m_pPos )
	{
		ax = m_pPos->x + m_Pos.x;
		ay = m_pPos->y + m_Pos.y;
	}
	else
	{
		ax = m_Pos.x;
		ay = m_Pos.y;
	}
//	viiDraw::Sprite( &SprCEffectFinalExplosion[0] , ax, ay , m_Pos.z , ATR_ALPHA_MINUS, ARGB( m_sAlpha, 0x20,0x20,0x20) , 1.f ,(m_sTimer*3)%360 );
	viiDraw::Sprite( &stSprEffBlackSmoke2[0] , ax, ay , m_Pos.z , ATR_ALPHA_MINUS, ARGB( m_sAlpha, 0x20,0x20,0x20) , 1.f ,(m_sTimer*3)%360 );


}


//--------------------------------------------------------------------------------
// Stage共通:グラウンドの煙
//--------------------------------------------------------------------------------

CEffectGroundSmoke::CEffectGroundSmoke(Sint32 x , Sint32 y ,Sint32 sDir )
{

	m_Pos.x = x;
	m_Pos.y = y;

	Sint32 sRand = viiSub::Rand();

	m_sDirection = sDir;
	m_sAlpha = 188 + sRand%60;
	m_fScale = 0.25f+(sRand%32)/100.f;
	m_sRotation = 0;

	m_Add.x = (400+sRand%200)*sDir;

	m_sAtribute = ATR_FLIP_X;

	if( sDir == DIR_LEFT)
	{
		m_sAtribute = ATR_DFLT;
	}

	m_bDisp = gxFalse;
}


CEffectGroundSmoke::~CEffectGroundSmoke()
{

}


void CEffectGroundSmoke::SeqInit()
{
	Sint32 kabe = 0;
	Sint32 sCount = 0;

	m_Pos.x += m_Add.x;

	while( kabe == 0 )
	{
		kabe = pGame->pBg->get_kabetype( m_Pos.x/100 , m_Pos.y/100 );

		if( kabe == 0 )
		{
			m_Add.y = 260;
			m_Pos.y += m_Add.y;
		}
		else if( kabe  )
		{
			m_Add.y = -10;
			m_Pos.y += m_Add.y;
			break;
		}

		sCount ++;
		if( sCount >= 190 )
		{
			SetActionSeq( enActionSeqEnd );
			return;
		}
	}

	if( kabe )
	{
		if( pGame->GetMarineLine() )
		{
			if( m_Pos.y >= pGame->GetMarineLine() )
			{
				if( m_Add.x < 0 )
				{
					new CEffWater( m_Pos.x , pGame->GetMarineLine() , enTypeSplashWave , 0, -1);
				}
				else
				{
					new CEffWater( m_Pos.x , pGame->GetMarineLine() , enTypeSplashWave , 0, 1);
				}
				SetActionSeq( enActionSeqEnd );
				return;
			}
		}

		m_bDisp = gxTrue;
		SetActionSeq( enActionSeqMain );
	}


}


void CEffectGroundSmoke::SeqMain()
{

	//-----------------------------------------------
	//動作
	//-----------------------------------------------

	if( m_Add.x > 0 )
	{
		m_Add.x -= 10;
		if( m_Add.x < 0 )
		{
			m_Add.x = 0;
		}
	}
	else if( m_Add.x < 0 )
	{
		m_Add.x += 10;
		if( m_Add.x > 0 )
		{
			m_Add.x = 0;
		}
	}

	Sint32 kabe = 0;
	Sint32 sCount = 0;

	m_Pos.x += m_Add.x;
	kabe = pGame->pBg->get_kabetype( m_Pos.x/100 , m_Pos.y/100 );

	if( kabe == 0 )
	{
		m_Add.y = 180;
		m_Pos.y += m_Add.y;
	}
	else if( kabe  )
	{
//		m_Add.y = -30;
		m_Add.y = -130;
		m_Pos.y += m_Add.y;
	}

	//-----------------------------------------------
	//アルファ調整
	//-----------------------------------------------

	m_sAlpha -= 4;
	m_fScale += 0.015f;
	m_sRotation += 4;

	if( m_sAlpha <= 0 )
	{
		m_sAlpha = 0;
		SetActionSeq( enActionSeqEnd );
	}

	viiSub::SetGravitMove(m_Pos.x , m_Pos.y);
}


void CEffectGroundSmoke::SeqCrash()
{

	SetActionSeq( enActionSeqEnd );

}


void CEffectGroundSmoke::Draw()
{
	extern gxSprite spr_sceneobj[];

	gxSprite *p = &stSprEffGroundSmoke[0];

	if( !m_bDisp ) return;

	viiDraw::Sprite( p , m_Pos.x , m_Pos.y , PRIO_BG3-1 , m_sAtribute ,ARGB( m_sAlpha,0xff,0xff,0xff) ,m_fScale , m_sRotation );

}


//--------------------------------------------------------------------------------
// Stage共通:バブル
//--------------------------------------------------------------------------------

CEffectBubble::CEffectBubble( Sint32 x, Sint32 y , Sint32 sRot , Sint32 sSpeed)
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_EFFECT;
	m_pTgt  = NULL;
	m_Add.x = (viiMath::Cos100( sRot )*sSpeed ) / 100;
	m_Add.y = (viiMath::Sin100( sRot )*sSpeed ) / 100;

	m_Float.x = 0;
	m_Float.y = 0;

	m_sRotation  = 0;
	m_sSpeed = 0;
	m_sAlpha = 0xff;
	m_fScale = (80+viiSub::Rand()%40)/100.f;

}


CEffectBubble::~CEffectBubble()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEffectBubble::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

//	if( viiSub::IsGroundBlock(m_Pos.x,m_Pos.y) )
//	{
//		SetActionSeq( enActionSeqCrash );
//		return;
//	}

//	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
//	{
//		SetActionSeq( enActionSeqEnd );
//		return;
//	}

	if( m_sAlpha > 0 )
	{
		m_sAlpha -= 1;
	}
	else
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	LogicAI();

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	m_Add.y -= 3*m_fScale;

	if( m_sSpeed  > 0 ) m_sSpeed += (0-m_sSpeed)/20;

	m_Float.x = (viiMath::Cos100( m_sRotation )*m_sSpeed ) / 100;
	m_Float.y = (viiMath::Sin100( m_sRotation )*m_sSpeed ) / 100;

	m_Pos.x += m_Float.x;
	m_Pos.y += m_Float.y;
}


void CEffectBubble::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------



}


void CEffectBubble::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	SetActionSeq( enActionSeqEnd );

}

void CEffectBubble::Draw()
{

	viiDraw::Sprite( &stSprEffGroundSmoke[0] , m_Pos.x , m_Pos.y , m_Pos.z, ATR_ALPHA_PLUS , ARGB( m_sAlpha/2,0xff,0xff,0xff) ,m_fScale , m_sRotation );
	viiDraw::Sprite( &stSprEffGroundSmoke[0] , m_Pos.x , m_Pos.y , m_Pos.z , ATR_ALPHA_PLUS , ARGB( m_sAlpha/2,0xff,0xff,0xff) ,m_fScale , -m_sRotation );


}

CEffectFinalExplosionDeluxe::CEffectFinalExplosionDeluxe(Sint32 x , Sint32 y)
{
	m_Pos.x = x;
	m_Pos.y = y;

	m_bFinish = gxFalse;
}


CEffectFinalExplosionDeluxe::~CEffectFinalExplosionDeluxe()
{
	
}


void CEffectFinalExplosionDeluxe::SeqMain()
{
	switch(m_sTimer){
	case 10:
		viiMus::PlaySound(enSoundExplosionMaximum);
		break;
	case 120:
		viiMus::PlaySound(enSoundCrashB);
		new CEffectFinalExplosion( m_Pos.x , m_Pos.y ,3 );
		break;
	case 150:
		new CEffectFinalExplosion( m_Pos.x , m_Pos.y ,3 );
		break;
	case 50:
		break;
	case 247:
		m_bFinish = gxTrue;
		viiMus::PlaySound(enSoundExplosionMiddle);
		viiMus::PlaySound(enSoundCrashA);
		pGame->QuakeOn(32);
		new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+15*1 , 600 );
		new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+45*1 , 800 );
		new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+135*1 ,600 );
		new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+165 , 600 );
		break;

	case 270:
		break;

	case 420:
		SetActionSeq( enActionSeqCrash );
		break;
	default:
		break;
	}

	if( m_sTimer < 230 )
	{
		if( viiSub::Rand()%3 == 0 )
		{
			Sint32 ax = -80+viiSub::Rand()%160;
			Sint32 ay = -80+viiSub::Rand()%160;
			viiEff::SetBombSimple( m_Pos.x+ax*100,m_Pos.y + ay*100 , 3 ,1 );
		}
	}

}


gxBool CEffectFinalExplosionDeluxe::IsFinish()
{

	return m_bFinish;

}


void CEffectFinalExplosionDeluxe::SeqCrash()
{
	m_bDead = gxTrue;

	if( m_bClear )
	{
		SetActionSeq( enActionSeqEnd );
	}
	
}

void CEffectFinalExplosionDeluxe::Draw()
{
	
	
}


//--------------------------------------------------------------------------------
//
// ボスファイヤー
//
//--------------------------------------------------------------------------------

CEffectBossFire::CEffectBossFire(Sint32 x , Sint32 y ,Sint32 sRot , Sint32 sSpeed )
{

	m_Pos.x = x;
	m_Pos.y = y;

	Sint32 sRand = viiSub::Rand();

	m_sDirection = sRot;
	m_sAlpha = 188 + sRand%60;
	m_fScale = 1.0f;//+(sRand%32)/100.f;
	m_sRotation = sRot+90;

	m_Add.x = viiMath::Cos100(sRot)*sSpeed;
	m_Add.y = viiMath::Sin100(sRot)*sSpeed;

	m_bDisp = gxTrue;
}


CEffectBossFire::~CEffectBossFire()
{

}


void CEffectBossFire::SeqMain()
{
	//-----------------------------------------------
	//動作
	//-----------------------------------------------

	m_Add.x += (0-m_Add.x)/5;
	m_Add.y += (0-m_Add.y)/5;

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	//-----------------------------------------------
	//アルファ調整
	//-----------------------------------------------

	m_sAlpha -= 8;
	m_fScale += 0.005f;
//	m_sRotation += 4;

	if( m_sAlpha <= 0 )
	{
		m_sAlpha = 0;
		SetActionSeq( enActionSeqEnd );
	}

}


void CEffectBossFire::SeqCrash()
{

	SetActionSeq( enActionSeqEnd );

}


void CEffectBossFire::Draw()
{
	if( !m_bDisp ) return;

	viiDraw::Sprite( &stSprEffBossFire[0] , m_Pos.x , m_Pos.y , PRIO_BG2 , ATR_ALPHA_PLUS       ,ARGB( m_sAlpha,0xff,0xff,0xff) ,m_fScale , m_sRotation );
	viiDraw::Sprite( &stSprEffBossFire[1] , m_Pos.x , m_Pos.y , PRIO_BG2 , ATR_ALPHA_PLUS ,ARGB( (m_sAlpha/2),0xff,0xff,0xff) ,m_fScale*2 , m_sRotation );

}


