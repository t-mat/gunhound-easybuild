//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------
#include <gunvalken.h>
#include "../../enemy/enemy.h"

/*
gxSprite AsSprRemainTime[]=
{
	{ enTexPageEffectCommon02,0*16,112,16,16,0,0},
	{ enTexPageEffectCommon02,1*16,112,16,16,0,0},
	{ enTexPageEffectCommon02,2*16,112,16,16,0,0},
	{ enTexPageEffectCommon02,3*16,112,16,16,0,0},
	{ enTexPageEffectCommon02,4*16,112,16,16,0,0},
	{ enTexPageEffectCommon02,5*16,112,16,16,0,0},
	{ enTexPageEffectCommon02,6*16,112,16,16,0,0},
	{ enTexPageEffectCommon02,7*16,112,16,16,0,0},
	{ enTexPageEffectCommon02,8*16,112,16,16,0,0},
	{ enTexPageEffectCommon02,9*16,112,16,16,0,0},
};
*/

gxSprite StSprSystemUI[]=
{
	{ enTexPageSystemUI ,32+0,0,200,48,0,0},		//BASE
	{ enTexPageSystemUI ,16*0,48,16,32,16,16},	//ARM(L)
	{ enTexPageSystemUI ,16*1,48,16,32,16,16},	//LEG(L)
	{ enTexPageSystemUI ,16*2,48,16,32,8,16},	//BODY(M)
	{ enTexPageSystemUI ,16*3,48,16,32,0,16},	//LEG(R)
	{ enTexPageSystemUI ,16*4,48,16,32,0,16},	//ARM(R)

};

gxSprite StSprSystemUI2[]=
{
	{ enTexPageSystemUI ,0,80,16,16,8,8},	//Light
	{ enTexPageSystemUI ,0,0,16,48,8,0},	//Gauge(G)
	{ enTexPageSystemUI ,16,0,16,48,8,0},	//Gauge(R)
};

gxSprite StSprSystemUI3[]={
	{ enTexPageSystemUI ,80,48+24*0,64,24,0,0},	//HP GAUGE
	{ enTexPageSystemUI ,80,48+24*1,64,24,0,0},	//HP BASE
	{ enTexPageSystemUI ,80,48+24*2,64,24,0,0},	//HP OVER
};

CRaderDamage::CRaderDamage()
{

	for(Sint32 ii=0;ii<enMax; ii++)
	{
		m_CDamageDisp[ii].SetID( ii );
	}

	m_bSwSekigaisen   = gxFalse;
	m_bDispSekigaisen = gxFalse;
	m_sPosX = 0;
	m_sTimer = 0;

	m_sHitPoint = 0;

}


CRaderDamage::~CRaderDamage()
{


}


void CRaderDamage::Action()
{
	for(Sint32 ii=0;ii<enMax; ii++)
	{
		m_CDamageDisp[ii].Action();
	}

	if( m_bDispSekigaisen )
	{
		if( m_sPosX < 24 )	m_sPosX ++;
	}
	else
	{
		if( m_sPosX > 0 ) m_sPosX --;
	}

	m_sTimer ++;
}


void CRaderDamage::Draw( Sint32 bx , Sint32 by )
{
	//-----------------------------------------------------
	//部位ダメージ表示
	//-----------------------------------------------------

	Sint32 sCol[enMax];

	for(Sint32 ii=0;ii<enMax; ii++)
	{
		sCol[ ii ] = 0xF0FFFFFF;	//ARGB( 0xf0,1,36,32 );//

		if( m_CDamageDisp[ii].bDead )
		{
			sCol[ ii ] = 0xF0FFFFFF;
			if( m_CDamageDisp[ii].sTime == 0 ) m_CDamageDisp[ii].SetDamage();
		}

		if( m_CDamageDisp[ii].sTime > 0 )
		{
			if( m_CDamageDisp[ii].sTime && viiSub::GameCounter()%32 <= 16 )
			{

			}
			else
			{
				sCol[ ii ] = 0xF0FF8080;
			}
		}

	}

	//ロボットのパーツダメージ

	ghPos stCenter = {bx-m_sPosX,by+4};
	ghPos stParts  = {stCenter.x+28,stCenter.y+18};

	viiSub::MenuSprite( &StSprSystemUI[ 1 ]  ,  stParts.x -2, stParts.y + 0 , PRIO_SYSTEM+1 ,ATR_DFLT , sCol[ 1 ] );
	viiSub::MenuSprite( &StSprSystemUI[ 2 ]  ,  stParts.x -2, stParts.y + 12, PRIO_SYSTEM+1 ,ATR_DFLT , sCol[ 2 ] );

	viiSub::MenuSprite( &StSprSystemUI[ 5 ]  ,  stParts.x +2, stParts.y + 0 , PRIO_SYSTEM+1 ,ATR_DFLT , sCol[ 1 ] );
	viiSub::MenuSprite( &StSprSystemUI[ 4 ]  ,  stParts.x +2, stParts.y + 12, PRIO_SYSTEM+1 ,ATR_DFLT , sCol[ 3 ] );

	viiSub::MenuSprite( &StSprSystemUI[ 3 ]  ,  stParts.x +0, stParts.y + 0 , PRIO_SYSTEM+1 ,ATR_DFLT , sCol[ 0 ] );	//ボディの優先順位が一番上
}


void CRaderDamage::DrawSekigaisen( Sint32 bx , Sint32 by )
{
	//--------------------------------------------
	//赤外線ゲージ描画
	//--------------------------------------------

	if( m_bDispSekigaisen )
	{
		Sint32 sCol = (m_sTimer*4)%360;
		sCol = 255*viiMath::Cos100(sCol)/100;
		sCol = gxAbs(sCol);
		Sint32 x,y;
		x = bx-m_sPosX;
		y = by;//5;
		if( m_bSwSekigaisen  )
		{
			viiSub::MenuSprite( &StSprSystemUI2[ 2 ]  ,  x , y , PRIO_SYSTEM ,ATR_DFLT );
			viiSub::MenuSprite( &StSprSystemUI2[ 0 ]  ,  x , y+30 , PRIO_SYSTEM ,ATR_ALPHA_PLUS ,ARGB( (sCol/2) , 0xff, 0xff , 0xff ) );
		}
		else
		{
			viiSub::MenuSprite( &StSprSystemUI2[ 1 ]  ,  x , y , PRIO_SYSTEM ,ATR_DFLT );
			viiSub::MenuSprite( &StSprSystemUI2[ 0 ]  ,  x , y+11 , PRIO_SYSTEM ,ATR_ALPHA_PLUS ,ARGB( (sCol/2) , 0xff, 0xff , 0xff ) );
		}
	}

	if( pGame && pGame->GetRaderScope( ) == enScopeTypeSnipe )
	{
		viiDraw::Box(0,0,WINDOW_W,WINDOW_H,PRIO_SYSTEM-1,gxTrue,0x106010,ATR_ALPHA_CRS);
	}

}


void CRaderDamage::DrawHitPoint(Sint32 bx , Sint32 by)
{
	//------------------------------------
	//武器の切り替え表示
	//------------------------------------

/*
	if(m_sHitPoint == 0)
	{
		viiSub::MenuPrintf(bx+64,by+32,0xFFf0f0f0,"GAME OVER" );
	}
	else
	{
//		viiSub::MenuPrintf(bx+64,by+32,0xFFf0f0f0,"%.2f pts" ,m_sHitPoint/100.f );

		Sint32 nx = bx+64;
		Sint32 ny = by+2;
		Sint32 sCol = 0x4000ff00;
//		m_sHitPoint = 25*100;
		if(m_sHitPoint/100 <= 25)
		{
			sCol = ((m_sTimer%12)<6)? 0x80ff0000 : 0x80ffffff;
		}
		viiSub::MenuSprite( &StSprSystemUI3[ 1 ]  ,  nx , ny , PRIO_SYSTEM ,ATR_DFLT ,0x40ffffff);
		PutSpriteDX( nx , ny , PRIO_SYSTEM,	
			StSprSystemUI3[0].page,
			StSprSystemUI3[0].u,
			StSprSystemUI3[0].v,
			m_sHitPoint*StSprSystemUI3[0].w/10000,
			StSprSystemUI3[0].h,
			StSprSystemUI3[0].cx,
			StSprSystemUI3[0].cy,
			ATR_DFLT,
			sCol,
			0,
			1.f,//m_sHitPoint/10000.f,
			1.f);
		viiSub::MenuSprite( &StSprSystemUI3[ 2 ]  ,  nx , ny , PRIO_SYSTEM ,ATR_DFLT ,0x40ffffff);
	}
*/

}


void CRaderDamage::SetDamage( Sint32 sIndex )
{
	//----------------------------------------------
	//ダメージのセッティング
	//----------------------------------------------

	m_CDamageDisp[ sIndex ].SetDamage();
}


void CRaderDamage::SetCrash( Sint32 sIndex )
{
	//----------------------------------------------
	//ダメージ部位のクラッシュ
	//----------------------------------------------

	m_CDamageDisp[ sIndex ].SetCrash();
}

