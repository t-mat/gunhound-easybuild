//------------------------------------------------------------
//
//エフェクト：基本爆発
//
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

enum {
	enTypeWhite=3,
	enTypeBlack=8,
};

extern gxSprite spr_cloud[];
extern gxSprite SprCEffectFinalExplosion[];

gxSprite stSprEffBombSmoke[] = {
	{TPG_CLOUD,0 ,0,64,64,32,32},	//雲
};

gxSprite stSprSmallBom[] = {
	{	enTexPageEffectCommon02,192,168,32,32,16,16 },
//	{	enTexPageEffectCommon02,32,128,16,16,8,8 },
};

gxSprite stSprMidiamBomb[] = {
	{enTexPageEffectCommon02,192,128,32,32,16,16 },
	{TPG_CLOUD,64,0,32,32,16,16},	//炎
};

gxSprite stSprExplosion[] = {
	{TPG_CLOUD,64,0,64,64,32,32},	//炎
	{TPG_CLOUD,0 ,0,64,64,32,32},	//雲

//	{TPG_EFFECT,64,0  ,32,32 ,16,16},	//0:爆発パターン
};

void EffectSun( Sint32 x, Sint32 y ,ghPos *pTgtPos)
{

	new CEffSun( x , y ,pTgtPos );
//	new CEffSun( x , y );
//	new CEffSun( x , y );

}



void EffectMakeBomb( Sint32 x, Sint32 y, Sint32 num , Sint32 spd)
{
	//----------------------------------------------
	//汎用爆発パターンの制御
	//（数と速度から演出レベルが変化します）
	//----------------------------------------------
	Sint32 sRotation,sSpeed;
	Sint32 sStartRot = viiSub::Rand()%360;

	if( spd < 0)	spd = 1;	//安全ソフト

	Sint32 n = 360/num;

	for( Sint32 i=0; i<num; i++ )
	{
		sRotation = sStartRot + n*i;
		sSpeed    = 1+viiSub::Rand()%spd;

		new CEffBomb( x , y , sRotation , sSpeed );
	}
}


void EffectMakeBigBomb( Sint32 x, Sint32 y  ,Sint32 z)
{
	//----------------------------------------------
	//汎用爆発パターンの制御
	//（数と速度から演出レベルが変化します）
	//----------------------------------------------
	CEffBigBomb *p;
	p = new CEffBigBomb( x , y , 0 , 0 );
	p->m_Pos.z = z;
}

void EffectMakeSun( Sint32 x, Sint32 y )
{
	//----------------------------------------------
	//汎用爆発パターンの制御
	//（数と速度から演出レベルが変化します）
	//----------------------------------------------
	new CEffBigBomb( x , y , 0 , 0 );

}


CEffBomb::CEffBomb(Sint32 x , Sint32 y, Sint32 sRotation , Sint32 sSpeed )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;

	m_sSpeed    = sSpeed;
	m_sRotation = sRotation;
	m_sColor = 0;
	m_sScale = 0;
	m_Add.x = 0;
	m_Add.y = 0;

	m_sWait = 0;
	m_sType = 0;

}


CEffBomb::~CEffBomb()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------
	
	
}


void CEffBomb::SeqInit()
{
	//----------------------------------------------
	//初期化
	//----------------------------------------------

	m_sRotation = viiSub::Rand()%360;
	m_sColor    = 128+viiSub::Rand()%20;		//色
	m_sScale    = 100+viiSub::Rand()%20;		//大きさ
	m_sType     = viiSub::Rand()%10;

	if( !pGame->is_gravit() )
	{
		m_sType = 0;	//宇宙空間では不完全燃焼ナシ
	}

	m_Add.x = viiMath::Cos100(m_sRotation)*m_sSpeed;	//角度xスピード
	m_Add.y = viiMath::Sin100(m_sRotation)*m_sSpeed;	//角度xスピード

	SetActionSeq( enActionSeqMain );

}


void CEffBomb::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------

	if(m_sWait > 0)
	{
		m_sWait --;
		return;
	}

	if(m_sColor > 0)
	{
		m_sColor -= 2;
		m_sScale += 1;
	}

	if( m_sColor <=0 )
	{
		//透過度ゼロパーセントで消滅
		SetActionSeq( enActionSeqCrash );
		return;
	}

	m_sRotation += 2;

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	viiSub::SetGravitMove( m_Pos.x , m_Pos.y );

	m_Add.x += -m_Add.x/10;
	m_Add.y += -m_Add.y/10;

}


void CEffBomb::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffBomb::SeqEnd()
{
	
	
}


void CEffBomb::Draw()
{
	//------------------------------------------------
	//描画
	//------------------------------------------------
	Sint32  col = m_sColor*2;
	Float32 scl = m_sScale/100.0f;

	if(m_sWait > 0) return;

	if(col >= 255)
	{
		col = 255;
	}
	else if( col <= 0 )
	{
		col = 0;
	}


#ifdef GUNHOUND
	switch(m_sType){
	case enTypeWhite:
		//不完全燃焼
//		viiDraw::Sprite( &stSprMidiamBomb[0] , m_Pos.x , m_Pos.y , PRIO_EFFECT+1,ATR_ALPHA_MINUS ,ARGB(col/8,0x01,0x01,0x01),1.0f,m_sRotation);
//		viiDraw::Sprite( &stSprMidiamBomb[ 0 ]  , m_Pos.x , m_Pos.y , PRIO_EFFECT+1,ATR_ALPHA_MINUS ,ARGB(col/8,0x01,0x01,0x01),1.0f,-m_sRotation);
//		viiDraw::Sprite( &stSprMidiamBomb[0] , m_Pos.x , m_Pos.y , PRIO_EFFECT+1,ATR_ALPHA_PLUS ,ARGB(col/2,0xff,0xff,0xff),1.0f,m_sRotation);
//		viiDraw::Sprite( &stSprMidiamBomb[ 0 ]  , m_Pos.x , m_Pos.y , PRIO_EFFECT+1,ATR_ALPHA_PLUS ,ARGB(col/2,0xff,0xff,0xff),1.0f,-m_sRotation);
		viiDraw::Sprite( &stSprMidiamBomb[0] , m_Pos.x , m_Pos.y , PRIO_EFFECT  ,ATR_ALPHA_PLUS ,ARGB(col/2,0xff,0xff,0xff),0.25f+scl, m_sRotation);
		viiDraw::Sprite( &spr_cloud[ 0 ] , m_Pos.x , m_Pos.y , PRIO_EFFECT,ATR_DFLT,ARGB(col,0x80,0x80,0x80),1.0f,m_sRotation);
		viiDraw::Sprite( &spr_cloud[ 0 ] , m_Pos.x , m_Pos.y , PRIO_EFFECT,ATR_ALPHA_MINUS,ARGB(col,0x80,0x80,0x80),1.0f,-m_sRotation);
		break;
	case enTypeBlack:
	default:
		//ノーマル
		viiDraw::Sprite( &stSprMidiamBomb[0] , m_Pos.x , m_Pos.y , PRIO_EFFECT+1,ATR_ALPHA_PLUS ,ARGB(col/2,0xff,0xff,0xff),1.0f,m_sRotation);
		viiDraw::Sprite( &stSprMidiamBomb[ 0 ]  , m_Pos.x , m_Pos.y , PRIO_EFFECT+1,ATR_ALPHA_PLUS ,ARGB(col/2,0xff,0xff,0xff),1.0f,-m_sRotation);
//		viiDraw::Sprite( &stSprMidiamBomb[1] , m_Pos.x , m_Pos.y , PRIO_EFFECT+1,ATR_ALPHA_PLUS ,ARGB(col/2,0xff,0xff,0xff),0.5f+scl,-m_sRotation);
		viiDraw::Sprite( &stSprMidiamBomb[1] , m_Pos.x , m_Pos.y , PRIO_EFFECT  ,ATR_ALPHA_PLUS ,ARGB(col/2,0xff,0xff,0xff),0.25f+scl, m_sRotation);
		break;
	}
#else
	switch(m_sType){
	case enTypeBlack:
		//明るい煙つき
		viiDraw::Sprite( &spr_bomb[ 0 ]  , m_Pos.x , m_Pos.y , PRIO_EFFECT+1,ATR_ALPHA_PLUS ,ARGB(col/2,0xff,0xff,0xff),1.0f,-m_sRotation);
		viiDraw::Sprite( &spr_cloud[ 0 ] , m_Pos.x , m_Pos.y , PRIO_EFFECT  ,ATR_ALPHA_PLUS,ARGB(col,0x80,0x80,0x80),1.0f,m_sRotation);
		break; 
	case enTypeWhite:
		//不完全燃焼
		viiDraw::Sprite( &spr_bomb[ 0 ]  , m_Pos.x , m_Pos.y , PRIO_EFFECT+1,ATR_ALPHA_PLUS ,ARGB(col/2,0xff,0xff,0xff),1.0f,-m_sRotation);
		viiDraw::Sprite( &spr_cloud[ 0 ] , m_Pos.x , m_Pos.y , PRIO_EFFECT,ATR_ALPHA_MINUS,ARGB(col,0xff,0xff,0xff),1.0f,m_sRotation);
		break;
	default:
		//ノーマル
		viiDraw::Sprite( &spr_bomb[0] , m_Pos.x , m_Pos.y , PRIO_EFFECT+1,ATR_ALPHA_PLUS ,ARGB(col/2,0xff,0xff,0xff),1.0f,-m_sRotation);
		viiDraw::Sprite( &spr_bomb[0] , m_Pos.x , m_Pos.y , PRIO_EFFECT  ,ATR_ALPHA_PLUS ,ARGB(col,0xff,0xff,0xff)  ,scl, m_sRotation);
		break;
	}
#endif

}


//------------------------------------------------------------
//
//エフェクト：爆発煙
//
//------------------------------------------------------------

CEffBigBomb::CEffBigBomb(Sint32 x , Sint32 y, Sint32 sRotation , Sint32 sSpeed )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_EFFECT;

//	m_Add;
	m_bYubaku = gxTrue;

	m_fAddScale = ZERO;
	m_sRotation = 0;
	m_fScale    = 1.f+m_fAddScale;
	m_sSeq      = 0;
	m_sWait     = 0;

	m_bFlair = gxFalse;
	m_fFlairScale = 1.25f+m_fAddScale;
	m_fFlairCol = 1.f;
	m_sFlairRot = 0;

	m_fFadeCol  = ZERO;

	m_fOrangeCnt =270;

	m_HitAtari.set_hp(1);
	m_HitAtari.set_ap(1);

	m_HitAtari.ax1 = -32*2;
	m_HitAtari.ay1 = -32*2;
	m_HitAtari.ax2 =  32*2;
	m_HitAtari.ay2 =  32*2;

	SetActionSeq( enActionSeqMain );
}


CEffBigBomb::~CEffBigBomb()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------
	
	
}


void CEffBigBomb::SeqInit()
{
	//----------------------------------------------
	//初期化
	//----------------------------------------------

	SetActionSeq( enActionSeqMain );

}


void CEffBigBomb::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------

	gxBool bOrange = gxFalse;

	switch( m_sSeq ){
	case 0:
		m_sOrangeCol = 0;
		m_fOrangeCnt = ZERO;
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
			m_fOrangeCnt += 2.95f;
		}
		if(m_fOrangeCnt > -5.f)
		{
			m_bFlair = gxTrue;
		}
		if(m_fOrangeCnt > 0.f && m_fOrangeCnt < 45.f )
		{

			if( m_fOrangeCnt < 1.f ) m_fFadeCol = 1.f;
			if( m_fOrangeCnt < 5.f ) m_fOrangeCnt = 10.f;
		}

		if(m_fOrangeCnt >= 90.f)
		{
			m_fOrangeCnt = 90.f;
		}
		Sint32 cnt = (Sint32)(m_fOrangeCnt);
		m_fScale = -(6+m_fAddScale)*viiMath::Sin100(cnt)/100.f;
		m_sOrangeCol = 0xFF*gxAbs(viiMath::Cos100(cnt))/100;
	}

	if( m_bFlair )
	{
		m_sFlairRot --;
		m_fFlairCol -= 0.02f;
		if(m_fFlairCol<=0.f)
		{
			SetActionSeq( enActionSeqEnd );
		}
		m_fFlairScale += 0.1f;

		if(m_fFlairCol <= ZERO) m_fFlairCol = ZERO;
	}

	if( m_fFadeCol > 0)
	{
		m_fFadeCol -= 0.025f;
		if( m_fFadeCol < 0)
		{
			m_fFadeCol = ZERO;
		}

	}

	if( m_sWait < 60*1 && m_sWait%2==0 )
	{
		if( m_bYubaku )
		{
			m_HitAtari.SetHantei( ID_VERSIS_ATK , &m_Pos );
		}
	}
	m_sWait++;

}


void CEffBigBomb::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffBigBomb::SeqEnd()
{
	
	
}


void CEffBigBomb::Draw()
{
	//------------------------------------------------
	//描画
	//------------------------------------------------
	Sint32 sFadeCol = (Sint32)(0x40*m_fFadeCol);

//	viiDraw::Box(0,0,WINDOW_W,WINDOW_H,PRIO_EFFECT,gxTrue,ARGB(sFadeCol,0xff,0xff,0xff));

	viiDraw::Sprite( &SprCEffectFinalExplosion[0] , m_Pos.x , m_Pos.y , m_Pos.z , ATR_ALPHA_PLUS, ARGB((m_sOrangeCol/2), 0xff,0xff,0xff) ,6.f+m_fScale*2.f);//,  );
	viiDraw::Sprite( &SprCEffectFinalExplosion[0] , m_Pos.x , m_Pos.y , m_Pos.z , ATR_ALPHA_PLUS, ARGB((m_sOrangeCol), 0xff,0xff,0xff) , m_fScale );
	viiDraw::Sprite( &SprCEffectFinalExplosion[1] , m_Pos.x , m_Pos.y , m_Pos.z , ATR_ALPHA_PLUS, ARGB((m_sOrangeCol), 0xff,0xff,0xff) , m_fScale*0.75f );
	viiDraw::Sprite( &SprCEffectFinalExplosion[2] , m_Pos.x , m_Pos.y , m_Pos.z , ATR_ALPHA_PLUS, ARGB((m_sOrangeCol), 0xff,0xff,0xff) , m_fScale ,m_sTimer);

	if( m_bFlair )
	{
		Sint32 sCol = (Sint32)(0xFF*m_fFlairCol);
		viiDraw::Sprite( &SprCEffectFinalExplosion[2] , m_Pos.x , m_Pos.y , m_Pos.z , ATR_ALPHA_PLUS, ARGB((sCol), 0xff,0xff,0xff) , m_fFlairScale ,m_sFlairRot);
	}

}


//------------------------------------------------------------
//
//エフェクト：爆発煙
//
//------------------------------------------------------------

CEffBombSmoke::CEffBombSmoke(Sint32 x , Sint32 y, Sint32 sRotation , Sint32 sSpeed )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;

	m_sSpeed    = sSpeed;
	m_sRotation = sRotation;
	m_sColor = 0;
	m_sScale = 0;
	m_Add.x = 0;
	m_Add.y = 0;

	m_sWait = 0;
	m_sType = 0;
	m_pTgt = NULL;
	m_bRotFix = gxFalse;
}


CEffBombSmoke::~CEffBombSmoke()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------
	
	
}


void CEffBombSmoke::SeqInit()
{
	//----------------------------------------------
	//初期化
	//----------------------------------------------

	if( !m_bRotFix )
	{
		m_sRotation = viiSub::Rand()%360;
	}
	m_sColor    = 128+viiSub::Rand()%20;		//色
	m_sScale    = 45+viiSub::Rand()%20;		//大きさ
	//m_sType     = viiSub::Rand()%2;

	m_Add.x = viiMath::Cos100(m_sRotation);
	m_Add.y = viiMath::Sin100(m_sRotation);

	SetActionSeq( enActionSeqMain );

}


void CEffBombSmoke::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------

	if(m_sWait > 0)
	{
		m_sWait --;
		return;
	}

	if(m_sColor > 0)
	{
		m_sColor -= 2;
		m_sScale += 7;
	}

	if( m_sColor <=0 )
	{
		//透過度ゼロパーセントで消滅
		SetActionSeq( enActionSeqCrash );
		return;
	}

	m_sRotation += 2;

	Sint32 ax,ay;
	ax = (m_Add.x*m_sSpeed)/100;
	ay = (m_Add.y*m_sSpeed)/100;

	m_Pos.x += ax;
	m_Pos.y += ay;

	m_sSpeed += -m_sSpeed/30;

}


void CEffBombSmoke::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffBombSmoke::SeqEnd()
{
	
	
}


void CEffBombSmoke::Draw()
{
	//------------------------------------------------
	//描画
	//------------------------------------------------
	Sint32  col = m_sColor*2;
	Float32 scl = m_sScale/100.0f;
	Sint32 x,y;

	x = m_Pos.x;
	y = m_Pos.y;

	if( m_pTgt )
	{
		x += m_pTgt->x;
		y += m_pTgt->y;
	}

	if(m_sWait > 0) return;

	if(col >= 255)
	{
		col = 255;
	}
	else if( col <= 0 )
	{
		col = 0;
	}

	if( m_sType == 0 )
	{
		viiDraw::Sprite( &stSprEffBombSmoke[0] , x , y , PRIO_EFFECT+1,ATR_ALPHA_MINUS ,ARGB(col/2,0xff,0xff,0xff),0.75f,-m_sRotation);
		viiDraw::Sprite( &stSprEffBombSmoke[0] , x , y , PRIO_EFFECT+1,ATR_ALPHA_MINUS ,ARGB(col/2,0xff,0xff,0xff),0.75f,m_sRotation);
	}
	else
	{
		viiDraw::Sprite( &stSprEffBombSmoke[0] , x , y , PRIO_EFFECT+1,ATR_DFLT       ,ARGB(col,0x80,0x80,0x80),0.75f,-m_sRotation);
		viiDraw::Sprite( &stSprEffBombSmoke[0] , x , y , PRIO_EFFECT+1,ATR_ALPHA_PLUS ,ARGB(col,0x80,0x80,0x80),0.75f,m_sRotation);
	}

}

//------------------------------------------------------------
//
//エフェクト：小さい内部爆発
//
//------------------------------------------------------------
void MakeEffectSmallBombs(Sint32 x ,Sint32 y, Sint32 num )
{
	Sint32 sKaku = viiSub::Rand()%360;

	for(Sint32 jj=0;jj<num;jj++)
	{
		for( Sint32 ii=0;ii<num;ii++ )
		{
			sKaku = viiSub::Rand()%360;
			new CEffBombSmall(x,y,sKaku,jj*16+ii*8 );
		}
	}

}

CEffBombSmall::CEffBombSmall(Sint32 x , Sint32 y, Sint32 sRotation , Sint32 sSpeed )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;

	m_sSpeed    = 240+viiSub::Rand()%80;;
	m_sRotation = sRotation;
	m_sColor = 0;
	m_sScale = 0;

	m_Add.x = viiMath::Cos100(m_sRotation);
	m_Add.y = viiMath::Sin100(m_sRotation);

	m_sWait = sSpeed;
	m_sType = 0;

	m_sRotation = viiSub::Rand()%360;
	m_sColor    = 128+viiSub::Rand()%20;		//色
	m_sScale    = 100;
	m_sType     = viiSub::Rand()%2;

}


CEffBombSmall::~CEffBombSmall()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------
	
	
}


void CEffBombSmall::SeqInit()
{
	//----------------------------------------------
	//初期化
	//----------------------------------------------

	SetActionSeq( enActionSeqMain );

}


void CEffBombSmall::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------

	if(m_sWait > 0)
	{
		m_sWait --;
		return;
	}

	if(m_sColor > 0)
	{
		m_sColor -= 2;
		m_sScale -= 1;
	}

	if( m_sScale < 0)
	{
		m_sScale = 0;
		m_sColor = 0;
	}

	if( m_sColor <=0 )
	{
		//透過度ゼロパーセントで消滅
		SetActionSeq( enActionSeqCrash );
		return;
	}

	m_sSpeed += (0-m_sSpeed)/20;

	m_Pos.x += (m_Add.x*m_sSpeed)/100;
	m_Pos.y += (m_Add.y*m_sSpeed)/100;

//	m_Add.x += -m_Add.x/10;
//	m_Add.y += -m_Add.y/10;

}


void CEffBombSmall::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffBombSmall::SeqEnd()
{
	
	
}


void CEffBombSmall::Draw()
{
	//------------------------------------------------
	//描画
	//------------------------------------------------
	Sint32  col = m_sColor;
	Float32 scl = m_sScale/100.0f;

	if(m_sWait > 0) return;

	else if( col <= 0 )
	{
		col = 0;
	}

	viiDraw::Sprite( &stSprSmallBom[0] , m_Pos.x , m_Pos.y , PRIO_EFFECT,ATR_ALPHA_PLUS ,ARGB( col,0xff,0xff,0xff),scl );
}


//------------------------------------------------------------
//
//太陽処理
//
//------------------------------------------------------------

CEffSun::CEffSun(Sint32 x , Sint32 y ,ghPos *pPos )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;

	m_sRotation = viiSub::Rand()%360;
	m_sColor    = 128+viiSub::Rand()%20;		//色
	m_sScale    = 100+viiSub::Rand()%20;		//大きさ

	m_Add.x = viiMath::Cos100(m_sRotation);	//角度xスピード
	m_Add.y = viiMath::Sin100(m_sRotation);	//角度xスピード
	m_pPos = pPos;
}


CEffSun::~CEffSun()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------
	
	
}


void CEffSun::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------

	if(m_sColor > 0)
	{
		m_sColor -= 2;
		m_sScale += 1;
	}

	if( m_sColor <=0 )
	{
		//透過度ゼロパーセントで消滅
		SetActionSeq( enActionSeqEnd );
		return;
	}

	m_sRotation += 0;

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	m_Add.x += -m_Add.x/10;
	m_Add.y += -m_Add.y/10;

}


void CEffSun::Draw()
{
	//------------------------------------------------
	//描画
	//------------------------------------------------
	Sint32  col = m_sColor*2;
	Float32 scl = m_sScale/100.0f;

	if(col >= 255)
	{
		col = 255;
	}
	else if( col <= 0 )
	{
		col = 0;
	}

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

	viiDraw::Sprite( &stSprMidiamBomb[0] , ax , ay , PRIO_BG1-1,ATR_ALPHA_PLUS ,ARGB(col/2,0xff,0xff,0xff),3.0f,m_sRotation);

}


