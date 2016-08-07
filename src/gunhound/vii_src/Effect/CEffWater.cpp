//------------------------------------------------------------
//
//エフェクト：戦艦のバーニア
//　バーシス艦尾の炎、ICBMの炎など
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

enum {
	enScrollOutWidth  = 128*100,
	enScrollOutHeight = 32*100,
};

extern gxSprite sprSplash[]=
{
	//SIDE_R
	{enTexPageEffectCommon02,0*32,0,32,32,16,32},	//0
	{enTexPageEffectCommon02,1*32,0,32,32,16,32},
	{enTexPageEffectCommon02,2*32,0,32,32,16,32},
	{enTexPageEffectCommon02,3*32,0,32,32,16,32},

	//PILLAR
	{enTexPageEffectCommon02,0*32,32,32,32,16,32},	//4
	{enTexPageEffectCommon02,1*32,32,32,32,16,32},
	{enTexPageEffectCommon02,2*32,32,32,32,16,32},
	{enTexPageEffectCommon02,3*32,32,32,32,16,32},

	{enTexPageEffectCommon02,0*16,64,16,16,8,16},	//8
	{enTexPageEffectCommon02,1*16,64,16,16,8,16},
	{enTexPageEffectCommon02,2*16,64,16,16,8,16},

	//SPLASH
	{enTexPageEffectCommon02,4*32,0,32,32,16,16},	//11

	{enTexPageEffectCommon02,48,48,16,16,8,8},		//12
};

extern gxSprite sprBigPillar[]={
	{enTexPageEffectCommon02, 0,200,32,56,16,56},	//0
	{enTexPageEffectCommon02,32,200,32,56,16,56},	//1
	{enTexPageEffectCommon02,64,224,32,32,16,16},	//2
	{enTexPageEffectCommon03,0,0,64,64,32,32 },
};


void MakeSplash( Sint32 x , Sint32 y , Sint32 prio )
{
	Sint32 ii;
	Sint32 yy = 1600;

	new CEffWaterSpark( x +2*100 , yy+y-16*100 ,-90+-1*8+(viiSub::Rand()%4)-2 , 40*2 ,prio);
	new CEffWaterSpark( x +2*100 , yy+y-16*100 ,-90+-2*8+(viiSub::Rand()%4)-2 , 40*2 ,prio);
	new CEffWaterSpark( x +2*100 , yy+y-16*100 ,-90+-5*8+(viiSub::Rand()%4)-2 , 40*1 ,prio);

	new CEffWaterSpark( x +2*100 , yy+y-16*100 ,-90+1*8+(viiSub::Rand()%4)-2 , 40*2 ,prio);
	new CEffWaterSpark( x +2*100 , yy+y-16*100 ,-90+2*8+(viiSub::Rand()%4)-2 , 40*2 ,prio);
	new CEffWaterSpark( x +2*100 , yy+y-16*100 ,-90+5*8+(viiSub::Rand()%4)-2 , 40*1 ,prio);

	for(ii=-3;ii<3;ii++)
	{
		new CEffWaterSpark( x +ii*1*100 , yy+y+8*100 ,-90 , (640+viiSub::Rand()%1200) ,prio);//- ii*20 );
	}

	for(ii=-3;ii<3;ii++)
	{
		CEffWaterSpark *p;
		p = new CEffWaterSpark( x +ii*1*100 , yy+y+8*100 ,-90 , 640+(viiSub::Rand()%960) ,prio);
		p->SetType(enSparkTypePiller);
	}

}


CEffWater::CEffWater( Sint32 x,Sint32 y , enTypeSplash _eTypeSplash , Sint32 lv , Sint32 rot , Sint32 speed )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_EFFECT;

	m_Add.x = 0;
	m_Add.y = 0;
	m_enTypeSplash = _eTypeSplash;
	m_sRotation = rot;
}


CEffWater::~CEffWater()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------


}


void CEffWater::SeqInit()
{
	//----------------------------------------------
	//初期化
	//----------------------------------------------

//	m_sAlpha = 158- viiSub::Rand()%30;
//	m_sScale = 50 + viiSub::Rand()%50;

	SetActionSeq( enActionSeqMain );
}


void CEffWater::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------
	if( m_enTypeSplash == enTypeSplashPillar)
	{
		switch( m_sTimer ){
		case 0:
		case 4:
		case 8:
			new CEffWaterPillar( m_Pos.x, m_Pos.y , m_Pos.z);
			break;
		case 12:
			SetActionSeq( enActionSeqEnd );
			break;
		}
	}
	else if( m_enTypeSplash == enTypeSplashLarge)
	{
		switch( m_sTimer ){
		case 0:
		case 4:
		case 8:
			new CEffWaterWave( m_Pos.x, m_Pos.y , m_Pos.z , -1 );
			new CEffWaterWave( m_Pos.x, m_Pos.y , m_Pos.z ,  1 );
			break;
		case 12:
			SetActionSeq( enActionSeqEnd );
			break;
		}
	}
	else if( m_enTypeSplash == enTypeSplashSmall )
	{
		switch( m_sTimer ){
		case 0:
		case 4:
		case 8:
			{
			CEffWaterPillar *p;
			p = new CEffWaterPillar( m_Pos.x, m_Pos.y , m_Pos.z);
			p->SetScale( 0.4f );
			}
			break;
		case 12:
			SetActionSeq( enActionSeqEnd );
			break;
		}
	}
	else if( m_enTypeSplash == enTypeSplashWave )
	{
		switch( m_sTimer ){
		case 0:
		case 4:
		case 8:
			new CEffWaterWave( m_Pos.x, m_Pos.y , (viiSub::Rand()%2)? PRIO_PLAYER-1 : PRIO_PLAYER+1 /*m_Pos.z*/ , m_sRotation );
			break;
		case 12:
			SetActionSeq( enActionSeqEnd );
			break;
		}
	}


}


void CEffWater::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffWater::Draw()
{
	//------------------------------------------------
	//描画
	//------------------------------------------------

/*
	viiDraw::Sprite(&sprSplash[ 0 ] ,
		 m_Pos.x,
		 m_Pos.y,
		 m_sPrio,
		 ATR_ALPHA_PLUS,
		 ARGB(m_sAlpha,0x80,0x80,0xff),
		 m_sScale/50.0f,m_sRotation);
*/


}


//------------------------------------------------------------
//
//エフェクト：ピラー
//
//------------------------------------------------------------


CEffWaterPillar::CEffWaterPillar( Sint32 x,Sint32 y ,Sint32 z )
{
	
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = z;

	m_sSprite = 0;
	m_sRotation = -8+viiSub::Rand()%16;
	m_sAlpha = 255;
	m_fScale = (50+viiSub::Rand()%50)/100.0f;
	m_sWait  = viiSub::Rand()%4;
}


CEffWaterPillar::~CEffWaterPillar( )
{
	
	
}


void CEffWaterPillar::SeqMain( )
{

	m_sAlpha -= 0x08;

	m_sWait ++;

	m_fScale += 0.05f;

	if( m_sWait%8 == 0 )
	{
		m_sSprite ++;
	}

	if( m_sSprite >= 3)
	{
		m_sSprite = 3;
	}

	if( m_sAlpha <= 0 )
	{
		m_sAlpha = 0;
		SetActionSeq( enActionSeqEnd );
	}
}


void CEffWaterPillar::Draw( )
{
	viiDraw::Sprite(&sprSplash[ 4+m_sSprite ] ,
		 m_Pos.x,
		 m_Pos.y,
		 m_Pos.z,
		 ATR_ALPHA_PLUS,
		 ARGB(m_sAlpha,0xFF,0xFF,0xFF),
		 m_fScale,m_sRotation);

}


//------------------------------------------------------------
//
//エフェクト：ウェーブ
//
//------------------------------------------------------------


CEffWaterWave::CEffWaterWave( Sint32 x,Sint32 y ,Sint32 z , Sint32 dir )
{
	
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = z;

	m_sAtribute = ATR_DFLT;

	m_Add.x = 150+viiSub::Rand()%150;
	m_Add.y = 0;

	if( dir < 0 )
	{
//		new CEffWaterSplash( m_Pos.x , m_Pos.y , m_Pos.z , -45+viiSub::Rand()%8 , abs(m_Add.x));
		new CEffWaterSplash( m_Pos.x , m_Pos.y , m_Pos.z , -4+viiSub::Rand()%8 , abs(m_Add.x));
		m_sAtribute = ATR_FLIP_X;
		m_Add.x *= -1;
	}
	else
	{
//		new CEffWaterSplash( m_Pos.x , m_Pos.y , m_Pos.z , 225-4+viiSub::Rand()%8 , abs(m_Add.x));
		new CEffWaterSplash( m_Pos.x , m_Pos.y , m_Pos.z , 180-4+viiSub::Rand()%8 , abs(m_Add.x));
	}

	m_sSprite = 0;

	m_sRotation = 0;
	m_sAlpha    = 255;
	m_fScale    = (100+viiSub::Rand()%100)/100.0f;
	m_sWait     = viiSub::Rand()%4;

}


CEffWaterWave::~CEffWaterWave( )
{
	
	
}


void CEffWaterWave::SeqMain( )
{

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	m_Pos.y += 2;

	m_sAlpha -= 0x08;

	m_sWait ++;

	if( m_sWait == 12 )
	{
		if(m_Add.x > 0)
		{
			new CEffWaterSplash( m_Pos.x , m_Pos.y , m_Pos.z , -4+viiSub::Rand()%8 , abs(m_Add.x));
		}
		else
		{
			new CEffWaterSplash( m_Pos.x , m_Pos.y , m_Pos.z , 180-4+viiSub::Rand()%8 , abs(m_Add.x));
		}
	}

	m_fScale += 0.05f;

	if( m_sWait%8 == 0 )
	{
		m_sSprite ++;
	}

	if( m_sSprite >= 3)
	{
		m_sSprite = 3;
	}

	if( m_sAlpha <= 0 )
	{
		m_sAlpha = 0;
		SetActionSeq( enActionSeqEnd );
	}
}


void CEffWaterWave::Draw( )
{
	viiDraw::Sprite(&sprSplash[ m_sSprite ] ,
		 m_Pos.x,
		 m_Pos.y,
		 m_Pos.z,
		 m_sAtribute|ATR_ALPHA_PLUS,
		 ARGB(m_sAlpha,0xFF,0xFF,0xFF),
		 m_fScale,m_sRotation-4+viiSub::Rand()%8);

}


//------------------------------------------------------------
//
//エフェクト：スプラッシュ
//
//------------------------------------------------------------


CEffWaterSplash::CEffWaterSplash( Sint32 x,Sint32 y ,Sint32 z , Sint32 rot , Sint32 spd )
{
	
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = z;

	m_sAtribute = ATR_DFLT;

	m_Add.x = (viiMath::Cos100(rot)*spd)/100;
	m_Add.y = (viiMath::Sin100(rot)*spd)/100;

	m_sRotation = viiSub::Rand()%360;
	m_sAlpha    = 255;


}


CEffWaterSplash::~CEffWaterSplash( )
{
	
	
}


void CEffWaterSplash::SeqMain( )
{

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

//	m_Pos.y += 1;

	m_sAlpha -= 0x08;

	if( m_sAlpha <= 0 )
	{
		m_sAlpha = 0;
		SetActionSeq( enActionSeqEnd );
	}

	m_sRotation += 2;
}


void CEffWaterSplash::Draw( )
{
	viiDraw::Sprite(&sprSplash[ 11 ] ,
		 m_Pos.x,
		 m_Pos.y,
		 m_Pos.z,
		 m_sAtribute|ATR_ALPHA_PLUS,
		 ARGB(m_sAlpha,0xFF,0xFF,0xFF),
		 1.0f,m_sRotation);

}


//------------------------------------------------------------
//
//エフェクト：水流水用スモーク
//
//------------------------------------------------------------

CEffWaterStorm::CEffWaterStorm(Sint32 x , Sint32 y, Sint32 rot , Sint32 Speed )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	if( Speed <= 0) Speed = 2;	//安全ソフト
	Sint32 spd = Speed/2+viiSub::Rand()%(Speed/2);

	m_sAdd_X = ( viiMath::Cos100(rot)*spd )/100;
	m_sAdd_Y = ( viiMath::Sin100(rot)*spd )/100;

	m_sRotation = viiSub::Rand()%360;
	m_sScale    = 40;

}


CEffWaterStorm::~CEffWaterStorm()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------


}


void CEffWaterStorm::SeqInit()
{
	//----------------------------------------------
	//初期化
	//----------------------------------------------

	m_sAlpha = (158- viiSub::Rand()%60);
	m_sScale = 40 + viiSub::Rand()%20;

	SetActionSeq( enActionSeqMain );
}


void CEffWaterStorm::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------


	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	m_sAlpha -= 4;

	if(m_sAlpha <= 0)
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	m_Pos.x += m_sAdd_X;
	m_Pos.y += m_sAdd_Y;

	m_sScale    -= 1;
	m_sRotation += 2;

	viiSub::SetGravitMove( m_Pos.x , m_Pos.y );

}


void CEffWaterStorm::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffWaterStorm::SeqEnd()
{
	
	
}


void CEffWaterStorm::Draw()
{
	//------------------------------------------------
	//描画
	//------------------------------------------------

	viiDraw::Sprite(&sprSplash[ 12 ],m_Pos.x,m_Pos.y,PRIO_EFFECT,ATR_ALPHA_PLUS,ARGB(m_sAlpha,0xff,0xff,0xff),m_sScale/100.0f,m_sRotation);


}





//------------------------------------------------------------
//
//エフェクト：水用スプラッシュ
//
//------------------------------------------------------------

CEffWaterSpark::CEffWaterSpark(Sint32 x , Sint32 y, Sint32 rot , Sint32 Speed ,Sint32 sPrio)
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = sPrio;

	m_Add.x = (viiMath::Cos100(rot)*Speed)/100;
	m_Add.y = (viiMath::Sin100(rot)*Speed)/100;

	m_sWait  = 1+viiSub::Rand()%32;
	m_sAlpha = 180+viiSub::Rand()%64;;

	m_fScale = 1.0f;
	m_sType = 0;
	m_sRotation = rot+90;

}


CEffWaterSpark::~CEffWaterSpark()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------


}



void CEffWaterSpark::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------

	if( m_sWait > 0 )
	{
		m_sWait--;
		if( m_sWait > 0 ) return;
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	if( m_sType != 2 )
	{
		if( m_sTimer == 8 )
		{
			CEffWaterSpark *p;
			for(Sint32 jj=0;jj<6;jj++)
			{
				p = new CEffWaterSpark( m_Pos.x+(-2+viiSub::Rand()%4)*700 , m_Pos.y-320-(viiSub::Rand()%16)*100 ,-90+(-16+viiSub::Rand()%32) , 160+(viiSub::Rand()%160) );
				p->SetType(enSparkTypeFog);
			}
		}
	}

	switch( m_sType ){
	case enSparkTypeWhiteWave:	//白波
		m_Pos.x += m_Add.x;
		m_Pos.y += m_Add.y/2;
		m_Add.y += (0-m_Add.y)/10;
		m_fScale = 0.95f+m_sTimer/100.f;
		m_sAlpha -= 4;
		break;

	case enSparkTypePiller:	//水柱
		m_Pos.x += m_Add.x;
		m_Pos.y += m_Add.y/4;
		m_Add.y += (0-m_Add.y)/10;
		m_fScale = 0.75f+m_sTimer/200.f;
		m_sAlpha -= 4;
		break;

	case enSparkTypeFog:	//きり
		m_Pos.x += m_Add.x*2;
		m_Pos.y += m_Add.y;
		m_Add.y += 4;
		m_sRotation += 4;
		m_sAlpha -= 2;
		m_fScale = 2.25f+m_sTimer/200.f;
		break;
	}

	if(m_sAlpha <= 0)
	{
		m_sAlpha = 0;
		SetActionSeq( enActionSeqEnd );
		return;
	}
}


void CEffWaterSpark::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffWaterSpark::Draw()
{
	//------------------------------------------------
	//描画
	//------------------------------------------------

	if( m_sWait > 0 )
	{
		return;
	}

	if( m_sType == enSparkTypeWhiteWave )
	{
		//泡
		viiDraw::Sprite( &sprBigPillar[ 0 ] , m_Pos.x , m_Pos.y , m_Pos.z , ATR_ALPHA_PLUS , ARGB(m_sAlpha,0xff,0xff,0xff) ,m_fScale ,m_sRotation);
		viiDraw::Sprite( &sprBigPillar[ 0 ] , m_Pos.x , m_Pos.y , m_Pos.z , ATR_DFLT , ARGB(m_sAlpha/2,0xff,0xff,0xff) ,m_fScale*1.5f ,m_sRotation);
	}
	if( m_sType == enSparkTypePiller )
	{
		//水柱
		viiDraw::Sprite( &sprBigPillar[ 1 ] , m_Pos.x , m_Pos.y , m_Pos.z-1 , ATR_DFLT , ARGB(m_sAlpha  ,0xff,0xff,0xff) ,m_fScale*2.0f ,m_sRotation);
		viiDraw::Sprite( &sprBigPillar[ 1 ] , m_Pos.x , m_Pos.y , m_Pos.z-1 , ATR_DFLT , ARGB(m_sAlpha/2,0xff,0xff,0xff) ,m_fScale*3.0f ,m_sRotation);
	}
	if( m_sType == enSparkTypeFog )
	{
		//きり
		viiDraw::Sprite( &sprBigPillar[ 2 ] , m_Pos.x , m_Pos.y , m_Pos.z , ATR_DFLT , ARGB(m_sAlpha,0xff,0xff,0xff) ,1.5f ,-m_sRotation);
		viiDraw::Sprite( &sprBigPillar[ 3 ] , m_Pos.x , m_Pos.y , m_Pos.z , ATR_ALPHA_PLUS , ARGB(m_sAlpha/8,0xff,0xff,0xff) ,1.5f ,m_sRotation/2);
	}

}


