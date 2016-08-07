//------------------------------------------------------------
//
//エフェクト：イージス艦
//
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

extern gxSprite spr_versis[];

CEffEagis::CEffEagis(Sint32 x , Sint32 y, Sint32 rot , Sint32 Speed )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_sTimer = 0;
	m_sAdd_Y = 0;
	m_sColor = 0;

	m_bDisp   = gxTrue;
	m_bBurnia = gxFalse;
	m_bShockA = gxFalse;
	m_bShockB = gxFalse;

	m_bControl = gxFalse;

	SetPrio(enActionPrio70);
}


CEffEagis::~CEffEagis()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------
	
	
}


void CEffEagis::SeqInit()
{
	m_sColor = 0;	//点滅用のカラー

	SetActionSeq( enActionSeqMain );
}


void CEffEagis::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------

	m_sAdd_Y = 0;
	m_sTimer ++;

	if( m_bControl ) Control();

	if( m_bShockA )
	{
		if(viiSub::Rand()%2)
		{
			m_sAdd_Y = ((m_sTimer%6)<3)? 200 : 0;
		}
	}

	if( m_bShockB )
	{
		if(viiSub::Rand()%2)
		{
			m_sAdd_Y = ((m_sTimer%12)<6)? 100 : 0;
		}
		else
		{
			m_sAdd_Y = 0;
		}
	}

	m_sColor += 2;
	m_sColor = m_sColor%250;

	if( m_bBurnia )
	{
		new CEffShipBurnia( m_Pos.x -200*100, m_Pos.y-7200+(viiSub::Rand()%7)*500 , PRIO_VERSIS_B-1 , 180 ,200 );
		new CEffShipBurnia( m_Pos.x -250*100, m_Pos.y-7200+(viiSub::Rand()%7)*500 , PRIO_VERSIS_B-1 , 180 ,200 );
		new CEffShipBurnia( m_Pos.x -200*100, m_Pos.y-7200+(viiSub::Rand()%7)*500 , PRIO_VERSIS_B-1 , 180 ,200 );

		new CEffShipBurnia( m_Pos.x -200*100, m_Pos.y-7200+(viiSub::Rand()%7)*500 , PRIO_VERSIS_B-1 , 200 ,10 );
		new CEffShipBurnia( m_Pos.x -200*100, m_Pos.y-7200+(viiSub::Rand()%7)*500 , PRIO_VERSIS_B-1 , 180 ,10 );
		new CEffShipBurnia( m_Pos.x -200*100, m_Pos.y-7400+(viiSub::Rand()%7)*500 , PRIO_VERSIS_B-1 , 160 ,10 );


	}

}


void CEffEagis::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffEagis::SeqEnd()
{
	
	
}


void CEffEagis::Control()
{
	//------------------------------------------------
	//操作
	//------------------------------------------------

	if( Joy[0].psh&JOY_L ) m_Pos.x -= 400;
	if( Joy[0].psh&JOY_R ) m_Pos.x += 400;
	if( Joy[0].psh&JOY_U ) m_Pos.y -= 400;
	if( Joy[0].psh&JOY_D ) m_Pos.y += 400;

	if( Joy[0].trg&BTN_A ) m_bBurnia = !m_bBurnia;

}


void CEffEagis::Draw()
{
	//------------------------------------------------
	//描画
	//------------------------------------------------
	Sint32 sx,sy,x1,y1,x2,y2;
	Sint32 col;

	if( !m_bDisp ) return;

	col = abs(m_sColor-125);

	viiSub::GetScroll_s( sx,sy );

	//------------------------------------
	//ランプ１
	//------------------------------------
	x1 = m_Pos.x/100 -10 - sx;
	y1 = m_sAdd_Y+m_Pos.y/100 -115 - sy+m_sAdd_Y;
	x2 = m_Pos.x/100 +220 - sx;
	y2 = m_sAdd_Y+m_Pos.y/100 -65 - sy+m_sAdd_Y;
	viiDraw::Box(x1,y1,x2,y2,PRIO_VERSIS_B,gxTrue,ARGB(0xff,col*2,0x00,0x00));

	//------------------------------------
	//ランプ２
	//------------------------------------
	x1 = m_Pos.x/100 -200 - sx;
	y1 = m_Pos.y/100 -82 - sy+m_sAdd_Y;
	x2 = m_Pos.x/100 -60 - sx;
	y2 = m_Pos.y/100 -33 - sy+m_sAdd_Y;
	viiDraw::Box(x1,y1,x2,y2,PRIO_VERSIS_B,gxTrue,ARGB(0xFF,col*2,0x00,0x00));

	//------------------------------------
	//イージス
	//------------------------------------
	viiDraw::Sprite (
		&spr_versis[0],
		m_Pos.x ,
		m_sAdd_Y+m_Pos.y,
		PRIO_VERSIS_B,
		ATR_DFLT,
		ARGB(0xff,0xff,0xff,0xff));

}




