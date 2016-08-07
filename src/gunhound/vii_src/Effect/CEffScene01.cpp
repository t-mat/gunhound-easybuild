//------------------------------------------------------------
//@
//@
//@	エフェクト：ステージ７のビーム攻撃演出
//@
//@
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

enum {
	enScrollOutWidth  = 128*100,
	enScrollOutHeight = 32*100,
};

enum {
	enMekaWhite = 0,
	enMekaRed,
	enLaserBeam,
	enCrashTime = 260,
};
extern gxSprite sprScene01[]=
{
	{ TPG_HUMAN,96+0 ,80,32,32,16,16},	//enMekaWhite
	{ TPG_HUMAN,96+32,80,32,32,16,16},	//enMekaRed
	{ TPG_EFFECT,144,192,16,32,8,8 },	//LaserBeam
};

Sint32 ScaleTbl[]={
	100,80,60,80,100,
};

extern gxSprite sprPixel;

CEffScene01::CEffScene01( Sint32 x , Sint32 y , Sint32 z )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_sPrio = z;
	m_sEnsyutuTimer = 0;

	for(Sint32 i=0;i<enLaserMax;i++)
	{
		m_LaserWidth[i] = 300+30*i;
	}

}


CEffScene01::~CEffScene01()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------


}

void CEffScene01::SeqInit()
{
	//----------------------------------------------
	//初期化
	//----------------------------------------------

	m_pMeka[0] = new CEffExPerticle( m_Pos.x+ 200*100, m_Pos.y+ 34*100 , m_sPrio );
	m_pMeka[1] = new CEffExPerticle( m_Pos.x+ 272*100, m_Pos.y+ 56*100 , m_sPrio );
	m_pMeka[2] = new CEffExPerticle( m_Pos.x+ 232*100, m_Pos.y+ 96*100 , m_sPrio );
	m_pMeka[3] = new CEffExPerticle( m_Pos.x+ 90*100 , m_Pos.y+120*100 , m_sPrio );
	m_pMeka[4] = new CEffExPerticle( m_Pos.x+ 30*100 , m_Pos.y+140*100 , m_sPrio );

	m_pMeka[0]->SetSprite( &sprScene01[ enMekaWhite ] );

	for(Sint32 i=0;i<enMekaMax;i++)
	{
		m_pMeka[i]->SetSprite( &sprScene01[ enMekaWhite ] );
		m_pMeka[i]->m_fScale = ScaleTbl[i]/100.f;
		m_pMeka[i]->SetDirection(0,100*ScaleTbl[i]/100);
	}
	m_pMeka[1]->SetSprite( &sprScene01[ enMekaRed ] );
	m_pMeka[3]->SetSprite( &sprScene01[ enMekaRed ] );

	SetActionSeq( enActionSeqMain );

}


void CEffScene01::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------
	Sint32 n;	//メカナンバー

	m_sEnsyutuTimer ++;

	if( m_sEnsyutuTimer > 180 )
	{
		for(Sint32 i=0;i<enLaserMax;i++)
		{
			if( m_LaserWidth[i] > 0) m_LaserWidth[i] -= 3;
			LaserBeam( (380-60*i)*100 , 0*100 ,m_LaserWidth[i]);
		}
	}


	if( m_sEnsyutuTimer < enCrashTime )
	{
		MekaShot();
		MikataShot();
	}

	if( m_sEnsyutuTimer > enCrashTime )
	{
		viiEff::SetBombEffect(
			m_Pos.x+(20+viiSub::Rand()%300)*100,
			m_Pos.y+(20+viiSub::Rand()%200)*100,
			3,5);

		viiEff::SetBombEffect(
			m_Pos.x+(20+viiSub::Rand()%300)*100,
			m_Pos.y+(20+viiSub::Rand()%200)*100,
			3,5);

		m_Pos.x -= 70;
	}

	if( enCrashTime == m_sEnsyutuTimer )
	{
		n = 0;
		viiEff::SetBombEffect( m_pMeka[n]->m_Pos.x,m_pMeka[n]->m_Pos.y,3,5);
		viiEff::Set( EFF_RING , m_pMeka[n]->m_Pos.x,m_pMeka[n]->m_Pos.y , argset(500) );
		m_pMeka[n]->SetEnd();
		m_pMeka[n]->m_Pos.x += 320*100;
	}

	if( enCrashTime+10 == m_sEnsyutuTimer )
	{
		n = 1;
		viiEff::SetBombEffect( m_pMeka[n]->m_Pos.x,m_pMeka[n]->m_Pos.y,3,5);
		viiEff::Set( EFF_RING , m_pMeka[n]->m_Pos.x,m_pMeka[n]->m_Pos.y , argset(500) );
		m_pMeka[n]->SetEnd();
		m_pMeka[n]->m_Pos.x += 320*100;
	}

	if( enCrashTime+20 == m_sEnsyutuTimer )
	{
		n = 2;
		viiEff::SetBombEffect( m_pMeka[n]->m_Pos.x,m_pMeka[n]->m_Pos.y,3,5);
		viiEff::Set( EFF_RING , m_pMeka[n]->m_Pos.x,m_pMeka[n]->m_Pos.y , argset(500) );
		m_pMeka[n]->SetEnd();
		m_pMeka[n]->m_Pos.x += 320*100;
	}

	if( enCrashTime+30 == m_sEnsyutuTimer )
	{
		n = 3;
		viiEff::SetBombEffect( m_pMeka[n]->m_Pos.x,m_pMeka[n]->m_Pos.y,3,5);
		viiEff::Set( EFF_RING , m_pMeka[n]->m_Pos.x,m_pMeka[n]->m_Pos.y , argset(500) );
		m_pMeka[n]->SetEnd();
		m_pMeka[n]->m_Pos.x += 320*100;
	}

	if( enCrashTime+40 == m_sEnsyutuTimer )
	{
		n = 4;
		viiEff::SetBombEffect( m_pMeka[n]->m_Pos.x,m_pMeka[n]->m_Pos.y,3,5);
		viiEff::Set( EFF_RING , m_pMeka[n]->m_Pos.x,m_pMeka[n]->m_Pos.y , argset(500) );
		m_pMeka[n]->SetEnd();
		m_pMeka[n]->m_Pos.x += 320*100;
	}

//	msg_prSint32f (100,200,PRIO_DEBUG,"演出タイマー[%d]",m_sEnsyutuTimer);

}


void CEffScene01::LaserBeam( Sint32 x, Sint32 y , Sint32 sScale )
{
	//----------------------------------------------
	//レーザービーム
	//----------------------------------------------

//	if(Joy[0].psh&BTN_R1) m_sScale = (m_sScale>0)? m_sScale -1 : m_sScale;
//	if(Joy[0].psh&BTN_L1) m_sScale = (m_sScale<200)? m_sScale +1 : m_sScale;
	if( sScale >= 250 ) return;
	if( sScale <= 0 ) return;

	if( sScale >= 100 ) sScale = 100;

	Float32 scale = sScale/1.f;

	CEffExPerticle *p;
	p = new CEffExPerticle( x , y , m_sPrio );
	p->SetSprite( &sprScene01[ enLaserBeam ] );
	p->m_fScale = scale/100.0f;
	p->m_sAtribute = ATR_ALPHA_PLUS;
	p->m_sARGB = 0x80FFFFFF;
	p->m_sRotation = 10;
	p->SetDirection(110,2400);
	p->SetEnd();

	p = new CEffExPerticle( x , y , m_sPrio );
	p->SetSprite( &sprScene01[ enLaserBeam ] );
	p->m_fScale = (scale+viiSub::Rand()%20)/100.0f;
	p->m_sAtribute = ATR_ALPHA_PLUS;
	p->m_sARGB = 0x80FFFFFF;
	p->m_sRotation = 10;
	p->SetDirection(110,1600+viiSub::Rand()%100);
	p->SetEnd();


}

void CEffScene01::MikataShot()
{
	//----------------------------------------------
	//味方：GUNを撃つ
	//----------------------------------------------
	CEffExPerticle *p;
	Sint32 x,y;

	for(Sint32 i=0;i<5;i++)
	{
		if( viiSub::Rand()%32 == 16)
		{
			viiSub::GetScroll_s( x,y );

			p = new CEffExPerticle( x, y+(viiSub::Rand()%20)*1000 , m_sPrio );
			p->SetSprite( &sprPixel );
			p->m_fScale = 1.0f;
			p->m_sARGB = 0xFFFFFFFF;
			p->SetDirection(0,500+(viiSub::Rand()%200) );
			p->SetEnd();
		}
	}
}


void CEffScene01::MekaShot()
{
	//----------------------------------------------
	//Meka:GUNを撃つ
	//----------------------------------------------
	CEffExPerticle *p;

	for(Sint32 i=0;i<5;i++)
	{
		if( viiSub::Rand()%24 == 16)
		{
			if( i == 1 || i == 3)
			{
				if( viiSub::Rand()%3 == 1) continue;
				//3WAY
				p = new CEffExPerticle( m_pMeka[i]->m_Pos.x, m_pMeka[i]->m_Pos.y , m_sPrio );
				p->SetSprite( &sprPixel );
				p->m_fScale = 1.0f;
				p->SetDirection(150,700*ScaleTbl[i]/100);
				p->SetEnd();

				p = new CEffExPerticle( m_pMeka[i]->m_Pos.x, m_pMeka[i]->m_Pos.y , m_sPrio );
				p->SetSprite( &sprPixel );
				p->m_fScale = 1.0f;
				p->SetDirection(180,700*ScaleTbl[i]/100);
				p->SetEnd();

				p = new CEffExPerticle( m_pMeka[i]->m_Pos.x, m_pMeka[i]->m_Pos.y , m_sPrio );
				p->SetSprite( &sprPixel );
				p->m_fScale = 1.0f;
				p->SetDirection(210,700*ScaleTbl[i]/100);
				p->SetEnd();
			}
			else
			{
				//ノーマルショット
				p = new CEffExPerticle( m_pMeka[i]->m_Pos.x, m_pMeka[i]->m_Pos.y , m_sPrio );
				p->SetSprite( &sprPixel );
				p->m_fScale = 1.0f;
				p->SetDirection(180,700*ScaleTbl[i]/100);
				p->SetEnd();

			}

		}
	}


}


void CEffScene01::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffScene01::SeqEnd()
{
	
	
}


void CEffScene01::Draw()
{



}



/*
CEffectLaseBeam::CEffectLaseBeam( Sint32 x,Sint32 y ,Sint32 z ,Float32 fScl )
{
	
	
	
}


CEffectLaseBeam::~CEffectLaseBeam()
{
	
	
}


void CEffectLaseBeam::SeqInit()
{
	
	
}


void CEffectLaseBeam::SeqMain()
{
	
	
}


void CEffectLaseBeam::SeqCrash()
{
	
	
}


void CEffectLaseBeam::SeqEnd()
{
	
	
}


void CEffectLaseBeam::Draw()
{
	
	
}


*/


