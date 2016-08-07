//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------
#include <gunvalken.h>
#include "../../enemy/enemy.h"

CRaderControl* CRaderControl::g_sCRaderControl = NULL;

gxSprite AsSprRader[]=
{
	{ enTexPageEffectCommon02,0,80,160,8,0,8},
	{ enTexPageEffectCommon02,0,104,128,8,64,8},
//	{ enTexPageEffectCommon02,0,192,128,8,64,8},
};
gxSprite AsSprmarker[]={
	{ enTexPageEffectCommon02,0,96,8,8,4,8},

	{ enTexPageEffectCommon02,8+8*0,96+8*0,8,8,4,8},		//enTypeMarkerPlayer
	{ enTexPageEffectCommon02,8+8*8,96+8*0,8,8,4,8},		//enTypeMarkerAssault
	{ enTexPageEffectCommon02,8+8*1,96+8*0,8,8,4,8},		//enTypeMarkerGroundObject
	{ enTexPageEffectCommon02,8+8*2,96+8*0,8,8,4,8},		//enTypeMarkerFlyingObject
	{ enTexPageEffectCommon02,8+8*3,96+8*0,8,8,4,8},		//enTypeMarkerGroundPartner
	{ enTexPageEffectCommon02,8+8*4,96+8*0,8,8,4,8},		//enTypeMarkerFlyingPartner
	{ enTexPageEffectCommon02,8+8*5,96+8*0,8,8,4,8},		//中ボス
	{ enTexPageEffectCommon02,8+8*6,96+8*0,8,8,4,8},		//enTypeMarkerDefenceTarget
	{ enTexPageEffectCommon02,8+8*7,96+8*0,8,8,4,8},		//enTypeMarkerOfenceTarget

};

gxSprite AsSprmarkerEffect[]={
	{ enTexPageEffectCommon02,8+8*5,96+8*0,8,8,4,4},		//中ボスリング
};

CRaderControl::CRaderControl()
{

	g_sCRaderControl = this;

	m_sMarkerCnt = 0;

	m_sRangeX1 = 0;
	m_sRangeX2 = 0;

}


CRaderControl::~CRaderControl()
{
	
	g_sCRaderControl = NULL;
	
}


void CRaderControl::Action()
{

//	Draw();

}


void CRaderControl::SetMarker( enTypeMarker eMark , Sint32 x , Sint32 y )
{
	if( m_sMarkerCnt >= enMarkerMax ) return;

	StMarker *p = &stMark[m_sMarkerCnt];

	p->x = x/100;
	p->y = y/100;
	p->enType = eMark;

	m_sMarkerCnt ++;
}


void CRaderControl::Draw( Sint32 bx , Sint32 by )
{
	//------------------------------------------
	//レーダーマップ表示
	//------------------------------------------

	Sint32 scx=0,scy=0;
	Sint32 sx=0,sy=0,sz = PRIO_SYSTEM;

	viiSub::GetScroll_s( scx,scy );
	scx = scx/10;
	scy = scy/10;

	sx = -(scx+WINDOW_W/20)+WINDOW_W/2;
	if(sx > 0) sx = 0;
	Sint32 ux=bx+sx,uy=by+sy;

	for(Sint32 jj=0;jj<16;jj++)
	{
		if( ux+160*jj+160 < 0    ) continue;
		if( ux+160*jj > WINDOW_W ) continue;
		viiSub::MenuSprite( &AsSprRader[0]  ,    (ux+160*jj) , uy , sz );
	}
//	viiSub::MenuSprite( &AsSprRader[0]  ,(ux+160*1) , uy , sz );
//	viiSub::MenuSprite( &AsSprRader[0]  ,(ux+160*2) , uy , sz );
//	viiSub::MenuSprite( &AsSprRader[0]  ,(ux+160*3) , uy , sz );
//	viiSub::MenuSprite( &AsSprRader[0]  ,(ux+160*4) , uy , sz );

	//--------------------------------------------
	//エリア外判定表示
	//--------------------------------------------

	Sint32 sCol[2];
	Sint32 sAtr[2];

	sCol[0] = 0x80800080;
	sCol[1] = 0x80800080;
	sAtr[0] = ATR_ALPHA_MINUS;
	sAtr[1] = ATR_ALPHA_MINUS;

	switch( CCockpit::GetInstance()->GetOperationArea() ){
	case 0:
		break;
	case -1:
		if( viiSub::GameCounter()%32<16)
		{
			sCol[0] = 0xF0800080;
			sAtr[0] = ATR_DFLT;
		}
		break;
	case 1:
		if( viiSub::GameCounter()%32<16)
		{
			sCol[1] = 0xF0800080;
			sAtr[1] = ATR_DFLT;
		}
		break;
	}

	viiDraw::Box ( 0 , uy-8 ,WINDOW_W        ,uy ,sz  ,gxTrue ,0xff010101 ,ATR_DFLT );
	viiDraw::Box ( ux+m_sRangeX1/10-WINDOW_W , uy-8 ,ux+m_sRangeX1/10          ,uy ,sz  ,gxTrue ,sCol[0] ,sAtr[0] );
	viiDraw::Box ( ux+m_sRangeX2/10          , uy-8 ,ux+m_sRangeX2/10+WINDOW_W ,uy ,sz  ,gxTrue ,sCol[1] ,sAtr[1] );

	//--------------------------------------------
	//レーダー表示
	//--------------------------------------------
	if( m_sMarkerCnt == 0 )
	{
		if( viiSub::GameCounter()%16 <= 8 )
		{
//			viiSub::MenuSprite( &AsSprRader[ 1 ]  ,  (ux+WINDOW_W/2),uy , sz );
			viiSub::MenuSprite( &AsSprRader[ 1 ]  ,  ( WINDOW_W/2),uy , sz ,ATR_DFLT, 0x80ffffff);
		}
		//-----------!!!!!!!!!!!!!!!!
//		return ;
		//-----------!!!!!!!!!!!!!!!!
	}
	else
	{
//		viiDraw::Grad( ux+scx+WINDOW_W/20 , uy-8 ,ux+scx+WINDOW_W/10 ,uy     ,sz  ,ATR_DFLT,0xF0FF0000,0x00FF0000,0x00FF00FF,0xF0FF00FF );
//		viiDraw::Grad( ux+scx-WINDOW_W/20 , uy-8 ,ux+scx+WINDOW_W/10 ,uy     ,sz  ,ATR_DFLT,0x40FF0000,0xF0FF0000,0xF0FF00FF,0x40FF00FF );

		Sint32 ax = ux+scx+WINDOW_W/20;
		viiDraw::Grad( ax-WINDOW_W/2 , uy-8 ,ax-WINDOW_W/2+WINDOW_W/2 ,uy     ,sz  ,ATR_DFLT,0x00800000,0xA0800000,0xA08000FF,0x008000FF );
		viiDraw::Grad( ax             , uy-8 ,ax+WINDOW_W/2             ,uy     ,sz  ,ATR_DFLT,0xA0800000,0x00800000,0x008000FF,0xA08000FF );
		viiDraw::Box ( ux+scx , uy-8 ,ux+scx+WINDOW_W/10 ,uy     ,sz  ,gxFalse ,0xFFFF00FF ,ATR_DFLT);
	}
	//--------------------------------------------
	//マーカー表示
	//--------------------------------------------

	StMarker *p;
	Sint32 ax,ay;
	Float32 fBossScl = 1.f+gxAbs( (-32+viiSub::GameCounter()%64) ) / 32.f;
	Float32 fScl = 1.f;
	for(Sint32 ii=0;ii<m_sMarkerCnt; ii++)
	{
		p = &stMark[ii];
		ax = p->x/10;
		ay = 0;

		fScl = 1.f;
		if( p->enType == enTypeMarkerMiddleBoss )
		{
			//中ボスの場合
			fScl = fBossScl;
			viiSub::MenuSprite( &AsSprmarkerEffect[ 0 ]  ,  (ux+ax),(uy+ay)-4 , sz ,ATR_ALPHA_PLUS , 0x80ffff00 ,fScl);
		}

		viiSub::MenuSprite( &AsSprmarker[ p->enType ]  ,  (ux+ax),(uy+ay) , sz ,ATR_DFLT , ARGB_DFLT );
	}



}

