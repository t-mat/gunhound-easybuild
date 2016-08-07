//--------------------------------------------
//
// フェイスディスプレイ
//
//--------------------------------------------
#include <gunvalken.h>
#include "CFaceScreen.h"

enum {
	enFACE_WINDOW_W = 64,
	enFACE_WINDOW_H = 62,
};
	
CFaceScreen::CFaceScreen()
{

//	LoadTexture( enTpgFaceDisp , "HoundData\\tex\\face.bmp" , 0xff00ff00 );

	m_fTimer = ZERO;
	m_bKuchiPaku = gxFalse;
	m_sFaceIndex = 0;
}


CFaceScreen::~CFaceScreen()
{
	
	
	
}


void CFaceScreen::Action()
{
	
	m_fTimer += 0.8f;
	if( m_fTimer >= 96.f)
	{
//		m_fTimer -= 96.f;
	}
	
}

void CFaceScreen::SetKuchiPaku(gxBool bFlag)
{
	
	m_bKuchiPaku = bFlag;
}


void CFaceScreen::Draw(Sint32 bx,Sint32 by,Sint32 sPersent)
{
	ghPos base;
	Sint32 x,y,z,yy;

	base.x = bx;//408;
	base.y = by;//180;//8;
	base.z = PRIO_SYSTEMWINDOW;

	x = base.x-1;
	y = base.y;
	z = base.z;


	//----------------------------------------
	//フェイス表示（走査線エフェクト込み）
	//----------------------------------------
	Sint32 tx,ty,tpg;
	Sint32 y0,y1;

	tpg = enTpgFaceDisp;//+m_sFaceIndex/12;
	tx  = m_sFaceIndex%8;//(m_sFaceIndex%12)%4;
	ty  = m_sFaceIndex/8;//(m_sFaceIndex%12)/4;

	tx = tx*64;
	ty = ty*64;

	Sint32 w = enFACE_WINDOW_W/2;
	Sint32 h = enFACE_WINDOW_H/2;


	//Window
	viiSub::MenuSprite( x+w-6,y+h,z, enTexPageSystemUI,168,56 ,58,64 ,29,32 ,ATR_DFLT, 0x80ffffff,0,sPersent/100.f ,1.f);

	//FACE
	viiSub::MenuSprite( x+w-3,y+h-1,z-1, enTpgFaceDisp,tx,ty ,enFACE_WINDOW_W,enFACE_WINDOW_H ,enFACE_WINDOW_W/2,enFACE_WINDOW_H/2  ,ATR_DFLT, ARGB(0x80,0xAF,0xFF,0xAF) ,0,sPersent/100.f ,1.0f);

	if( sPersent < 100 )
	{
		return;
	}

	yy = (Sint32)m_fTimer;

	//画像縦サイズ＋３２の領域まで走査し、その領域-16のところをtopとする
	y0 = yy%(enFACE_WINDOW_H+32)-16;

	//top+16をbottomとする
	y1 = y0+16;

	if( y0 <= 0               ) y0 = 0;
	if( y0 >= enFACE_WINDOW_H ) y0 = enFACE_WINDOW_H;
	if( y1 <= 0               ) y1 = 0;
	if( y1 >= enFACE_WINDOW_H ) y1 = enFACE_WINDOW_H;

//return;
	viiSub::MenuSprite( x-3  ,-1+y+0   ,z-1, tpg,tx,ty+ 0 ,enFACE_WINDOW_W   ,y0                 ,0 , 0 , ATR_DFLT, ARGB(0xC0,0xAF,0xFF,0xAF) );
	viiSub::MenuSprite( x-3+1,-1+y+y0  ,z-1, tpg,tx,ty+y0 ,enFACE_WINDOW_W-11   ,y1-y0              ,0 , 0 , ATR_DFLT, ARGB(0xC0,0xAF,0xFF,0xAF) );
	viiSub::MenuSprite( x-3  ,-1+y+y1  ,z-1, tpg,tx,ty+y1 ,enFACE_WINDOW_W   ,enFACE_WINDOW_H-y1 ,0 , 0 , ATR_DFLT, ARGB(0xC0,0xAF,0xFF,0xAF) );


}


