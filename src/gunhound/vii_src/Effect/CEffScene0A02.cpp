//------------------------------------------------------------
//
//エフェクト：レンズフレア
//
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"
#include "../enemy/enemy.h"

Float32 sFlareTbl[]=
{
	10.f,
	20.f,
	25.f,
	40.f,
	70.f,
	75.f,
	80.f,
	90.f,
	95.f,
	99.f,
};

Uint32 ColorTbl[]=
{
	0xfFa00000,
	0xfFa0a000,
	0xfF00a0a0,
	0xfFa000a0,
	0xfF00a0a0,
	0xfF00a000,
	0xfFa00000,
	0xfFa000a0,
	0xfF00a0a0,
	0xfF00a000,
	0xfFa0a000,
	0xfFa000a0,
};

enum {
	enScrollOutWidth  = 32*100,
	enScrollOutHeight = 32*100,
};

enum {
	enSeqNone,
	enSeqBombFirst,
	enSeqBombSecond,
};


gxSprite sprLensFrare[]=
{
	{ enTexPageEffect3 , 0,72, 112,112,56,56 },	//フレア

	{ enTexPageEffect3 , 232,64, 24,32,12,16 },	//レンズフレア

	{ enTexPageEffect3 , 128,0, 64,64,32,32 },	//フレア（●）
};

gxSprite SprWeatherRain[]={
	{TPG_EFFECT,24,16,8,8,4,4},		//15:アメS


	{TPG_EFFECT,112,164,4,4,2,2},		//15:アメS
	{TPG_EFFECT,116,164,4,4,2,2},		//16:アメL
	{TPG_EFFECT,112,168,4,4,2,2},		//17:雨あたり１
	{TPG_EFFECT,116,172,4,4,2,2},		//18:雨あたり２

	{TPG_EFFECT,112,160,4,4,2,2},		//10:雪S
	{TPG_EFFECT,116,160,4,4,2,2},		//11:雪L
	{TPG_EFFECT,120,160,24,16,24,16},	//12:雪煙

};

CEffScene0A02::CEffScene0A02( Sint32 x , Sint32 y , Sint32 z ,Sint32 stat)
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_sPrio = z;
	m_sSunStatus = stat;

	m_sColor = 0;
	m_sColorAdd = 0;

	m_bFinish = gxFalse;
	m_bRain = gxFalse;
	m_sTargetPrio = PRIO_EFFECT;

	StBrizzard *p;

	m_pBrizzard = new StBrizzard[enBRIZZARD_MAX];

	for(Sint32 ii=0;ii<enBRIZZARD_MAX;ii++)
	{
		Sint32 spd;
		p = &m_pBrizzard[ii];

		spd  = 300+viiSub::Rand()%260;

		p->x    = (viiSub::Rand()%WINDOW_W)*100;
		p->y    = (viiSub::Rand()%WINDOW_H)*100;
		p->z    = m_sTargetPrio;
		p->mx   = -spd;
		p->my   = +spd+viiSub::Rand()%50;
		p->type = viiSub::Rand()%2;
	}

}


CEffScene0A02::~CEffScene0A02()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------

	delete[] m_pBrizzard;
}


void CEffScene0A02::SeqInit()
{
	//----------------------------------------------
	//初期化
	//----------------------------------------------

	SetActionSeq( enActionSeqMain );

}


void CEffScene0A02::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------

	if(m_bRain)
	{
		RainEffect();
	}

}




void CEffScene0A02::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffScene0A02::SeqEnd()
{
	
	
}

void CEffScene0A02::RainEffect()
{
	Sint32 ii;
	StBrizzard *p;

	for(ii=0;ii<enBRIZZARD_MAX;ii++)
	{
		gxBool bPrioInit = gxFalse;
		p = &m_pBrizzard[ii];
		p->x += p->mx;
		p->y += p->my;
		if(p->x <     0       )
		{
			bPrioInit = gxTrue;
			p->x += WINDOW_W*100;
		}
		if(p->y > WINDOW_H*100)
		{
			p->y -= WINDOW_H*100;
			bPrioInit = gxTrue;
		}

		if(	bPrioInit )
		{
			p->z = m_sTargetPrio;
		}
	}

}


void CEffScene0A02::Draw()
{
	//----------------------------------------------
	//描画
	//----------------------------------------------
	switch( m_sSunStatus ){
	case 0:
		DrawSunRise();
		break;
	case 1:
		DrawDayTime();
		break;
	}

	if(m_bRain)
	{
		DrawRain();
	}

}

void CEffScene0A02::DrawSunRise()
{
	//-----------------------------------------------------------

	for(Sint32 ii=0;ii<8;ii++)
	{
		Float32 fScl = 3.0f+(viiSub::Rand()%20)/100.f;
		viiDraw::Spr_NoScrl( &sprLensFrare[2] , m_Pos.x , m_Pos.y , m_sPrio , ATR_ALPHA_PLUS, ARGB(0x40,0xff,0xff,0xff) ,fScl );
	}


}


void CEffScene0A02::DrawDayTime()
{
	//-----------------------------------------
	//レンズフレアつきの強い日差し
	//-----------------------------------------
	Sint32 ii,x,y;
	ghPos sPos;
	gxBool bDispFlare = gxTrue;

	sPos.x = m_Pos.x-12800;
	sPos.y = m_Pos.y+19600;

	bDispFlare = !pGame->pBg->IsBgObj( m_Pos.x/100,m_Pos.y/100-8 );

	//-----------------------------------------------------------

	Float32 fScl;

	for(ii=0;ii<10;ii++)
	{
		fScl = 0.5f+(viiSub::Rand()%30)/100.f;
		viiDraw::Spr_NoScrl( &sprLensFrare[0] , m_Pos.x , m_Pos.y , m_sPrio , ATR_ALPHA_PLUS, ARGB(0x40,0xff,0xff,0xff) ,fScl );//,viiSub::Rand()%360);
	}
	if( bDispFlare )
	{
		fScl = 0.5f+(viiSub::Rand()%30)/100.f;
		viiDraw::Spr_NoScrl( &sprLensFrare[0] , m_Pos.x , m_Pos.y , m_sPrio , ATR_ALPHA_PLUS, ARGB(0x20,0xff,0xff,0xff) ,fScl*8 );
	}

	//-----------------------------------------------------------
	//BGがかぶっていればフレア出さない
	if( !bDispFlare ) return;

	//-----------------------------------------------------------
	for(ii=0;ii<10;ii++)
	{
		x = m_Pos.x + sFlareTbl[ii]*( sPos.x - m_Pos.x )/100;
		y = m_Pos.y + sFlareTbl[ii]*( sPos.y - m_Pos.y )/100;

		if( viiSub::Rand()%12==0 ) continue;

		viiDraw::Spr_NoScrl( &sprLensFrare[1] , x , y , PRIO_EFFECT , ATR_ALPHA_PLUS, ColorTbl[ii] ,sFlareTbl[ii]/50.f );
		viiDraw::Spr_NoScrl( &sprLensFrare[1] , x , y , PRIO_EFFECT , ATR_ALPHA_PLUS, ColorTbl[ii] ,sFlareTbl[ii]/50.f );
	}

}


void CEffScene0A02::DrawRain()
{
	//----------------------------------------------
	//アメの表現
	//----------------------------------------------

	Sint32 ii;

	for(ii=0;ii<enBRIZZARD_MAX;ii++)
	{
		viiDraw::Spr_NoScrl(&SprWeatherRain[ m_pBrizzard[ii].type] , m_pBrizzard[ii].x , m_pBrizzard[ii].y , m_pBrizzard[ii].z , ATR_DFLT ,0xB0FFFFFF);
	}

}

