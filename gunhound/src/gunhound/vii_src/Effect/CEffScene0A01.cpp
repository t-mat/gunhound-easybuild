//------------------------------------------------------------
//
//エフェクト：補給ヘリ到着
//
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"
#include "../enemy/enemy.h"

enum {
	enScrollOutWidth  = 32*100,
	enScrollOutHeight = 32*100,
};

enum {
	enSeqNone,
	enSeqBombFirst,
	enSeqBombSecond,
};


gxSprite sprSupplyHeli[]=
{
	{ TPG_ENEMY06,0  ,96 ,  128,48,64,48},	//補給ヘリ

};

CEffScene0A01::CEffScene0A01( Sint32 x , Sint32 y , Sint32 z )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_sPrio = z;
	m_sEnsyutuTimer = 0;

	m_sColor = 0;
	m_sColorAdd = 0;

	m_pHeli = NULL;
	m_sSoldierCnt = 0;

	m_bFinish = gxFalse;
}


CEffScene0A01::~CEffScene0A01()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------
	if(m_pHeli) m_pHeli->SetEnd();

}


void CEffScene0A01::SeqInit()
{
	//----------------------------------------------
	//初期化
	//----------------------------------------------

	m_pHeli = new CEffExPerticle( m_Pos.x+ 120*100, m_Pos.y , m_sPrio );
	m_pHeli->SetDirection( 70,150 );
	m_pHeli->SetSprite( &sprSupplyHeli[0] );
	m_pHeli->m_fScale = 1.5f;
	m_pHeli->SetDirection( 200,150 );

	SetActionSeq( enActionSeqMain );

}


void CEffScene0A01::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------

	m_sEnsyutuTimer ++;

	Sint32 SolX;
	switch(m_sEnsyutuTimer){
	case 50:
		m_pHeli->SetDirection( 180,150 );
		break;
	case 100:
		m_pHeli->SetDirection( 160,130 );
		break;
	case 150:
		m_pHeli->SetDirection( 140,110 );
		break;
	case 200:
		m_pHeli->SetDirection( 120,90 );
		break;
	case 250:
		m_pHeli->SetDirection( 100,70 );
		break;
	case 300:
		m_pHeli->SetDirection( 80,50 );
		break;
	case 350:
		m_pHeli->SetDirection( 60,30 );
		break;
	case 400:
		m_pHeli->SetDirection( 40,20 );
		break;
	case 450:
		m_pHeli->SetDirection( 20,10 );
		break;
	case 500:
		m_pHeli->SetDirection( 0,0 );
		break;

	case 600:
		new CItemChip( m_pHeli->m_Pos.x-3200 , m_pHeli->m_Pos.y , enItemEnergyChip );
		break;
	case 660:
		new CItemChip( m_pHeli->m_Pos.x+3200 , m_pHeli->m_Pos.y , enItemPowerChip );
		break;

	case 720:
		new CItemChip( m_pHeli->m_Pos.x+6400 , m_pHeli->m_Pos.y , enItemPowerChip );
		break;

	case 620:
		SolX = m_pHeli->m_Pos.x+2800;
		m_pSoldier[m_sSoldierCnt] = new CSoldierBase();
		m_pSoldier[m_sSoldierCnt]->SetPosition(0,SolX, m_pHeli->m_Pos.y,NULL);
		m_pSoldier[m_sSoldierCnt]->SetTargetPosition( SolX+3200 );
		m_sSoldierCnt ++;
		break;

	case 650:
		SolX = m_pHeli->m_Pos.x+3200;
		m_pSoldier[m_sSoldierCnt] = new CSoldierBase();
		m_pSoldier[m_sSoldierCnt]->SetPosition(0,SolX, m_pHeli->m_Pos.y,NULL);
		m_pSoldier[m_sSoldierCnt]->SetTargetPosition( SolX+3200 );
		m_sSoldierCnt ++;
		break;

	case 680:
		SolX = m_pHeli->m_Pos.x+2800;
		m_pSoldier[m_sSoldierCnt] = new CSoldierBase();
		m_pSoldier[m_sSoldierCnt]->SetPosition(0,SolX, m_pHeli->m_Pos.y,NULL);
		m_pSoldier[m_sSoldierCnt]->SetTargetPosition( SolX+3200 );
		m_sSoldierCnt ++;
		break;

	case 710:
		SolX = m_pHeli->m_Pos.x+3600;
		m_pSoldier[m_sSoldierCnt] = new CSoldierBase();
		m_pSoldier[m_sSoldierCnt]->SetPosition(0,SolX, m_pHeli->m_Pos.y,NULL);
		m_pSoldier[m_sSoldierCnt]->SetTargetPosition( SolX+3200 );
		m_sSoldierCnt ++;
		break;

	case 740:
		SolX = m_pHeli->m_Pos.x+2800;
		m_pSoldier[m_sSoldierCnt] = new CSoldierBase();
		m_pSoldier[m_sSoldierCnt]->SetPosition(0,SolX, m_pHeli->m_Pos.y,NULL);
		m_pSoldier[m_sSoldierCnt]->SetTargetPosition( SolX+3200 );
		m_sSoldierCnt ++;
		break;

	case 770:
		SolX = m_pHeli->m_Pos.x+3200;
		m_pSoldier[m_sSoldierCnt] = new CSoldierBase();
		m_pSoldier[m_sSoldierCnt]->SetPosition(0,SolX, m_pHeli->m_Pos.y,NULL);
		m_pSoldier[m_sSoldierCnt]->SetTargetPosition( SolX+3200 );
		m_sSoldierCnt ++;
		m_bFinish = gxTrue;
		break;

	case 1020:
		m_pHeli->SetDirection( -110,30 );
		break;
	case 1120:
		m_pHeli->SetDirection( -120,50 );
		break;
	case 1220:
		m_pHeli->SetDirection( -130,70 );
		break;
	}


	viiDbg::printf (100,200,"演出タイマー[%d,%d]",m_pHeli->m_Pos.x,m_pHeli->m_Pos.y);

}




void CEffScene0A01::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffScene0A01::SeqEnd()
{
	
	
}


void CEffScene0A01::Draw()
{
	//----------------------------------------------
	//描画
	//----------------------------------------------



}


