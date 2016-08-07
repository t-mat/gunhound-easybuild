//------------------------------------------------------------
//
//エフェクト：魚雷発射シーン
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

enum {
	enScrollOutWidth  = 128*100,
	enScrollOutHeight = 32*100,
	enWalkCycle   = 6,
	enWalkPattern = 3,
	enWalkMax     = enWalkPattern*enWalkCycle,
};

CEffScene02::CEffScene02( Sint32 x , Sint32 y , Sint32 z )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = z;

	m_ScenePos.x = 0;
	m_ScenePos.y = 0;
	m_ScenePos.z = 0;

	m_Add.x = 0;
	m_Add.y = 0;

	m_pTorpedo = NULL;
	m_sEnsyutuTimer = 0;

	m_sSeq = 0;

	m_pNacy[enNancySmall]  = new CEneH0405Nancy( m_Pos.x, m_Pos.y , CEneH0405Nancy::enSizeSmall );
	m_pNacy[enNancyMiddle] = new CEneH0405Nancy( m_Pos.x, m_Pos.y , CEneH0405Nancy::enSizeMiddle );
	m_pNacy[enNancyLarge]  = new CEneH0405Nancy( m_Pos.x, m_Pos.y , CEneH0405Nancy::enSizeLarge );

	m_sControl = 0;

	m_pNautilus = new CEneH0407Nautilus( m_Pos.x  , m_Pos.y );

}


CEffScene02::~CEffScene02()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------

}

void CEffScene02::SeqInit()
{
	//----------------------------------------------
	//初期化
	//----------------------------------------------

	SetActionSeq( enActionSeqMain );

}


void CEffScene02::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------

	if( m_pNacy[enNancyLarge] )
	{
		m_pNacy[enNancyLarge]->m_Pos.x = m_Pos.x+(400*100)+(4*m_ScenePos.x/4);
		m_pNacy[enNancyLarge]->m_Pos.y = m_Pos.y+(50*100) +(4*m_ScenePos.y/4);
		m_pNacy[enNancyLarge]->m_Pos.z = PRIO_BG3+1;
	}

	if( m_pNacy[enNancyMiddle] )
	{
		m_pNacy[enNancyMiddle]->m_Pos.x = m_Pos.x+(430*100)+(2*m_ScenePos.x/4);
		m_pNacy[enNancyMiddle]->m_Pos.y = m_Pos.y+(80*100)+(2*m_ScenePos.y/4);
		m_pNacy[enNancyMiddle]->m_Pos.z = PRIO_BG2+1;
	}

	if( m_pNacy[enNancySmall] )
	{
		m_pNacy[enNancySmall]->m_Pos.x = m_Pos.x +(200*100)+(1*m_ScenePos.x/6);
		m_pNacy[enNancySmall]->m_Pos.y = m_Pos.y +(60*100)  +(1*m_ScenePos.y/5);
		m_pNacy[enNancySmall]->m_Pos.z = PRIO_BG2-1;
	}

	m_pNautilus->m_Pos.x = m_Pos.x+0*100;
	m_pNautilus->m_Pos.y = m_Pos.y;
	m_pNautilus->m_Pos.z = PRIO_BG2;

	enum {
		enSeqNone,
		enSeqMissileHassyaCall,
		enSeqMissileHassyaMain,
	};


	if( m_sControl&JOY_L)
	{
		if( m_Add.x > -200 ) m_Add.x -= 1;
	}
	else if( m_sControl&JOY_R)
	{
		if( m_Add.x < 200 ) m_Add.x += 1;
	}
	else
	{
		m_Add.x += -m_Add.x/100;
	}
	m_Pos.x      += m_Add.x;
	m_ScenePos.x += m_Add.x;

	m_sControl = 0;

	switch(m_sSeq){
	case enSeqNone:
		break;
	case enSeqMissileHassyaCall:
		m_sEnsyutuTimer = 0;
		m_sSeq = enSeqMissileHassyaMain;
		break;

	case enSeqMissileHassyaMain:
		if( m_sEnsyutuTimer == 1 )
		{
			viiMus::PlaySound( enSoundJump );
			m_pNacy[enNancyMiddle]->SetMissile();
		}
		if( m_sEnsyutuTimer == 10 )
		{
			viiMus::PlaySound( enSoundMissile );
		}

		if( m_sEnsyutuTimer == 20 )
		{
			viiMus::PlaySound( enSoundJump );
		}
		if( m_sEnsyutuTimer == 30 )
		{
			m_pNacy[enNancyLarge ]->SetMissile();
			m_pTorpedo = m_pNacy[enNancyLarge ]->GetMissile();
		}

		if( m_sEnsyutuTimer == 40 )
		{
			viiMus::PlaySound( enSoundJump );
		}
		if( m_sEnsyutuTimer == 50 )
		{
			viiMus::PlaySound( enSoundMissile );
			m_pNacy[enNancySmall ]->SetMissile();
		}

		if( m_sEnsyutuTimer == 200 )
		{
			m_pNacy[enNancyLarge ]->SetAdjust(gxFalse);
		}
		if( m_sEnsyutuTimer == 300 )
		{
			m_pNacy[enNancyMiddle ]->SetStop(0,0);
		}
		if( m_sEnsyutuTimer == 380 )
		{
			viiMus::PlaySound( enSoundExplosionLarge );
			m_pNacy[enNancyLarge ]->SetStop(0,0);
		}
		if( m_sEnsyutuTimer == 424 )
		{
			viiMus::PlaySound( enSoundCrashA );

			delete m_pNacy[enNancyLarge ];
			m_pNacy[enNancyLarge ] = NULL;

			delete m_pNacy[enNancyMiddle ];
			m_pNacy[enNancyMiddle ] = NULL;

			delete m_pNacy[enNancySmall ];
			m_pNacy[enNancySmall ] = NULL;
		}
		if( m_sEnsyutuTimer == 385 )
		{
			EffectMakeBigBomb( 6050*100 ,700*100);
			viiEff::SetBombDeluxe( 6050*100 ,700*100 , 1 , 1+viiSub::Rand()%2 );
//			new CEffectDmgprt( 5800*100, 700*100, 180+15 , 1800 );
//			new CEffectDmgprt( 5800*100, 660*100, 180+10 , 1600 );
//			new CEffectDmgprt( 5800*100, 690*100, 180+45 , 1200 );
//			new CEffectDmgprt( 5800*100, 680*100, 180-32 , 1700 );
		}
		if( m_sEnsyutuTimer == 620 )
		{
			viiMus::PlaySound( enSoundExplosionMiddle );
			EffectMakeBigBomb( 5650*100 , 680*100);
			new CEffectDmgprt( 5600*100, 700*100, 0+15 , 800 );
			new CEffectDmgprt( 5600*100, 670*100, 0+45 , 600 );
			new CEffectDmgprt( 5600*100, 660*100, 0+10 , 600 );
			new CEffectDmgprt( 5600*100, 690*100, 0+45 , 1200 );
			new CEffectDmgprt( 5600*100, 630*100, 0    , 1200 );
			new CEffectDmgprt( 5600*100, 680*100, 0-32 , 700 );
		}
		break;
	}


	m_sEnsyutuTimer ++;
}



void CEffScene02::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffScene02::SeqEnd()
{
	
	
}


void CEffScene02::Draw()
{



}


