//------------------------------------------------------------
//
//倒木シーン
//
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

enum {
	enScrollOutWidth  = 32*100,
	enScrollOutHeight = 32*100,
};

enum {
	enSeqNone,
	enSeqBombFirst,
	enSeqBombSecond,
};


extern gxSprite sprDownTree[]=
{
	{ enTexPageStgEffWood,0  ,0  ,48,112,24,112},	//木（上）
	{ enTexPageStgEffWood,48 ,0  ,16,48,8,24},		//木（下）
	{ enTexPageStgEffWood,64+4*0 ,0 ,  4,4,2,2},	//はっぱ1
	{ enTexPageStgEffWood,64+4*1 ,0 ,  4,4,2,2},	//はっぱ2
	{ enTexPageStgEffWood,64+4*2 ,0 ,  4,4,2,2},	//はっぱ3
	{ enTexPageStgEffWood,64 	 ,16 ,  32,32,16,16},	//枝

};


CEffScn04Tree::CEffScn04Tree( Sint32 x , Sint32 y , Sint32 z )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = z;
	m_sEnsyutuTimer = 0;

	for(Sint32 ii=0; ii<enTreeMax; ii++)
	{
		m_pTree[ii] = NULL;
	}
}


CEffScn04Tree::~CEffScn04Tree()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------

//	if( m_pMarsGear ) delete m_pMarsGear;

}

void CEffScn04Tree::SeqInit()
{
	//----------------------------------------------
	//初期化
	//----------------------------------------------

/*
	m_pMarsGear = new CEffExPerticle( m_Pos.x+ 120*100, m_Pos.y+ 80*100 , m_sPrio );
	m_pMarsGear->SetDirection( 70,10 );
	m_pMarsGear->SetSprite( &sprMarsGear[0] );
	m_pMarsGear->SetNoneScroll();
//	m_pMarsGear->SetEnd();
*/

	SetActionSeq( enActionSeqMain );

}


void CEffScn04Tree::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------

	m_sEnsyutuTimer ++;
	switch( m_sEnsyutuTimer ) {
	case 10:
		TreeDown(0,20*100,100*100);
		break;
	case 30:
		TreeDown(1,60*100,100*100);
		break;
	default:
//		TreeDrop(100,100);
//		TreeLeaf(100,100);
		break;
	}
	if( m_sEnsyutuTimer%30==0)
	{
		TreeDrop(60*100,100*100);
	}

	for(Sint32 ii=0; ii<enTreeMax; ii++)
	{
		if( m_pTree[ii] )
		{
			m_pTree[ii]->m_sRotation --;
			if( m_pTree[ii]->m_sRotation < -90 )
			{
				m_pTree[ii]->SetEnd();
				m_pTree[ii]->SetAlphaAdd( -1 );
				m_pTree[ii] = NULL;
			}

		}
	}

}




void CEffScn04Tree::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffScn04Tree::SeqEnd()
{
	
	
}


void CEffScn04Tree::Draw()
{
	//----------------------------------------------
	//描画
	//----------------------------------------------


}


void CEffScn04Tree::TreeDown(Sint32 id , Sint32 x,Sint32 y )
{
	//----------------------------------------------
	//倒木セット
	//----------------------------------------------


	m_pTree[id] = new CEffExPerticle( m_Pos.x+x, m_Pos.y+y , m_Pos.z );
//	p->SetDirection( 250,20+viiSub::Rand()%20 );
//	p->SetNoneScroll();
	m_pTree[id]->SetSprite( &sprDownTree[0] );
//	m_pTree[id]->SetEnd();


}


void CEffScn04Tree::TreeDrop( Sint32 x,Sint32 y )
{
	new CEffTreeEda( m_Pos.x + 60*100, m_Pos.y+100*100 );

/*
	CEffExPerticle *p;

	p = new CEffExPerticle( m_Pos.x+x, m_Pos.y+y , m_Pos.z );
	p->SetDirection( -250,120+viiSub::Rand()%20 );
//	p->SetNoneScroll();
	p->SetAlphaAdd( -8 );
	p->SetSprite( &sprDownTree[5] );
	p->SetEnd();
*/

}


void CEffScn04Tree::TreeLeaf( Sint32 x,Sint32 y )
{
	
	
}


//------------------------------------------------------------
//
//倒木シーン
//
//------------------------------------------------------------
CEffTreeEda::CEffTreeEda(Sint32 x , Sint32 y )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------
//	Sint32 sx,sy;
//	viiSub::GetScroll_s( sx,sy );
//	x -=  sx*100;
//	y -=  sy*100;

	Sint32 kaku = viiSub::Rand()%45;

	m_Add.x = (viiMath::Cos100(-kaku)*100)/100*-1;
	m_Add.y = (viiMath::Sin100(-kaku)*100)/100;

	m_Pos.x = x;
	m_Pos.y = y;

	m_sScale = 100;
	m_sColor = 200;
	m_sRotation = 0;
}


CEffTreeEda::~CEffTreeEda()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------
	
	
}


void CEffTreeEda::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	if( m_Add.y <= 120 ) m_Add.y += 10;

	if(m_sColor > 0)
	{
		m_sColor -= 2;
	}

	m_sRotation += 12;

	if( m_sColor <= 0 || m_sScale <= 0)
	{
		//透過度ゼロパーセントで消滅
		SetActionSeq( enActionSeqCrash );
		return;
	}

}


void CEffTreeEda::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}



void CEffTreeEda::Draw()
{
	//------------------------------------------------
	//描画
	//------------------------------------------------
	Sint32  col = m_sColor/2;
	Float32 scl = m_sScale/100.0f;

	viiDraw::Sprite( &sprDownTree[5] , m_Pos.x , m_Pos.y , PRIO_EFFECT  ,ATR_ALPHA_PLUS ,ARGB(col,0xff,0xff,0xff)  ,scl , m_sRotation );

}


//------------------------------------------------------------
//
//倒木シーン
//
//------------------------------------------------------------


CEffTreeLeaf::CEffTreeLeaf(Sint32 x , Sint32 y )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------
	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );

	x -=  sx*100;
	y -=  sy*100;

	m_Pos.x = x;
	m_Pos.y = y;

	m_sScale = 100;
	m_sColor = 200;
}


CEffTreeLeaf::~CEffTreeLeaf()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------
	
	
}


void CEffTreeLeaf::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------


	if(m_sColor > 0)
	{
		m_sColor -= 2;
	}
	if( m_sScale > 0)
	{
		m_sScale -= 1;
	}

	if( m_sColor <= 0 || m_sScale <= 0)
	{
		//透過度ゼロパーセントで消滅
		SetActionSeq( enActionSeqCrash );
		return;
	}

}


void CEffTreeLeaf::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}



void CEffTreeLeaf::Draw()
{
	//------------------------------------------------
	//描画
	//------------------------------------------------
	Sint32  col = m_sColor/2;
	Float32 scl = m_sScale/100.0f;

//	viiDraw::Spr_NoScrl( &sprZanzo[0] , m_Pos.x , m_Pos.y , PRIO_EFFECT  ,ATR_ALPHA_PLUS ,ARGB(col,0xff,0xff,0xff)  ,scl );

}


