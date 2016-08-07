//--------------------------------------------------------------------------------
//
// Stage00:アイアンギアボディタンク(ver.2)
//
//--------------------------------------------------------------------------------

#include "../HoundsEnemy.h"
#include "../../../Effect/EffectBase.h"
//#include "../EnemyTbl/CEneEx0006TankTBL.h"

enum {
	enScrollOutWidth  = 128*100,
	enScrollOutHeight = 128*100,

	enKuraiLeft  = -16,
	enKuraiTop   = -16,
	enKuraiRight =  16,
	enKuraiBottom=  16,

	enScore = 30000,
};

enum {
	enPhase1,
	enPhase2,
	enPhase3,
};

gxSprite SprCEneEx0006Tank[]=
{
	{enTexPageEnemyGear01 , 0,32,128,32,16,16	},	//シャフト
	{enTexPageEnemyGear01 , 0,64,16,16,8,8 		},	//ギア（小）
	{enTexPageEnemyGear01 , 16,64,32,32,16,16 	},	//ギア（大）
	{enTexPageEnemyGear01 , 0,96,32,16,16,8 	},	//フタ
	{enTexPageEnemyGear01 , 0,112,32,64,16,64 	},	//シリンダー
	{enTexPageEnemyGear01 , 32,112,80,64,40,32 	},	//コア
};

CEneEx0006Tank::CEneEx0006Tank( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_ENEMY;

	m_HitKurai.set_hp( enHitPointShipSubCoreBefore );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_HitAtari.ax1 = enKuraiLeft*2;
	m_HitAtari.ay1 = enKuraiTop*2;
	m_HitAtari.ax2 = enKuraiRight*2;
	m_HitAtari.ay2 = enKuraiBottom*2;

	m_bDisp = gxTrue;

	m_sDirection = DIR_RIGHT;
	m_sCrashTimer = 0;
	m_fRotation = 0;
	m_sAtackWait = 0;

	m_sSeq = enPhase1;
	m_bDamage = gxFalse;
	m_bCoreDamage = gxFalse;

	m_Add.x = m_Add.y = 0;

	m_fRotationAdd = 3.f;

}


CEneEx0006Tank::~CEneEx0006Tank()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneEx0006Tank::SeqInit()
{
	//一番左
	m_stItem[ 0 ].sType = enEngineItemGear_S;
	m_stItem[ 0 ].sSpr  = 1;
	m_stItem[ 0 ].stPos.x = -64;
	m_stItem[ 0 ].stPos.y = 0;
	m_stItem[ 0 ].stPos.z = -1;
	m_stItem[ 0 ].fRAdd = 16.f;

	m_stItem[ 1 ].sType = enEngineItemGear_L;
	m_stItem[ 1 ].sSpr  = 2;
	m_stItem[ 1 ].stPos.x = -40;
	m_stItem[ 1 ].stPos.y = 0;
	m_stItem[ 1 ].stPos.z = -1;
	m_stItem[ 1 ].fRAdd = -8.f;

	//左下から斜め右上
	m_stItem[ 2 ].sType = enEngineItemShaft;
	m_stItem[ 2 ].sSpr  = 0;
	m_stItem[ 2 ].stPos.x = -92;
	m_stItem[ 2 ].stPos.y = 32;
	m_stItem[ 2 ].stPos.z = -3;
	m_stItem[ 2 ].fRAdd   = -4.f;
	m_stItem[ 2 ].fScl    = 1.8f;
	m_stItem[ 2 ].fRange  = -3.f;
	m_stItem[ 2 ].fBaseRot = -30.f;
	m_stItem[ 2 ].fPosRange = 24.f;

	m_stItem[ 3 ].sType = enEngineItemCyrinder;
	m_stItem[ 3 ].sSpr  = 4;
	m_stItem[ 3 ].stPos.x = 32;
	m_stItem[ 3 ].stPos.y = 32;
	m_stItem[ 3 ].stPos.z = -2;
	m_stItem[ 3 ].fRAdd   = -8.f;
	m_stItem[ 3 ].fScl    = 0.5f;
	m_stItem[ 3 ].fRange  =  0.f;
	m_stItem[ 3 ].fBaseRot =  0.f;
	m_stItem[ 3 ].fPosRange = 32.f;

	m_stItem[ 4 ].sType = enEngineItemCyrinder;
	m_stItem[ 4 ].sSpr  = 4;
	m_stItem[ 4 ].stPos.x = -16;
	m_stItem[ 4 ].stPos.y = 0;
	m_stItem[ 4 ].stPos.z = -3;
	m_stItem[ 4 ].fRAdd = -16.f;
	m_stItem[ 4 ].fRange = ZERO;
	m_stItem[ 4 ].fPosRange = 16.f;
	m_stItem[ 4 ].fScl = 0.5f;

	m_stItem[ 5 ].sType = enEngineItemCyrinder;
	m_stItem[ 5 ].sSpr  = 4;
	m_stItem[ 5 ].stPos.x = -48;
	m_stItem[ 5 ].stPos.y = 32;
	m_stItem[ 5 ].stPos.z = -3;
	m_stItem[ 5 ].fRAdd = -16.f;
	m_stItem[ 5 ].fRange = ZERO;
	m_stItem[ 5 ].fPosRange = -16.f;
	m_stItem[ 5 ].fScl = 0.5f;

	m_stItem[ 6 ].sType = enEngineItemShaft;
	m_stItem[ 6 ].sSpr  = 0;
	m_stItem[ 6 ].stPos.x = 64;
	m_stItem[ 6 ].stPos.y = 32;
	m_stItem[ 6 ].stPos.z = -2;
	m_stItem[ 6 ].fRAdd = -4.f;
	m_stItem[ 6 ].fScl = 1.5f;
	m_stItem[ 6 ].fRange = ZERO;
	m_stItem[ 6 ].fBaseRot = 180.f;
	m_stItem[ 6 ].fPosRange = 16.f;

	//左上
	m_stItem[ 7 ].sType = enEngineItemGear_L;
	m_stItem[ 7 ].sSpr  = 2;
	m_stItem[ 7 ].stPos.x = -80*1;
	m_stItem[ 7 ].stPos.y = 48*-1;
	m_stItem[ 7 ].stPos.z = -2;
	m_stItem[ 7 ].fScl = 2.0f;
	m_stItem[ 7 ].fRAdd = 2.f;

	//左下
	m_stItem[ 8 ].sType = enEngineItemGear_L;
	m_stItem[ 8 ].sSpr  = 2;
	m_stItem[ 8 ].stPos.x = -92*1;
	m_stItem[ 8 ].stPos.y = 32;
	m_stItem[ 8 ].stPos.z = -3;
	m_stItem[ 8 ].fRAdd = -4.f;
	m_stItem[ 8 ].fScl = 1.5f;

	m_stItem[ 9 ].sType = enEngineItemCore;
	m_stItem[ 9 ].stPos.x = 0;
	m_stItem[ 9 ].stPos.y = 0;
	m_stItem[ 9 ].stPos.z = -1;

	m_stItem[ 10 ].sType = enEngineItemGear_S;
	m_stItem[ 10 ].sSpr  = 1;
	m_stItem[ 10 ].stPos.x = -64*-1;
	m_stItem[ 10 ].stPos.y = 0;
	m_stItem[ 10 ].stPos.z = -2;
	m_stItem[ 10 ].fRAdd = 16.f;

	m_stItem[ 11 ].sType = enEngineItemGear_L;
	m_stItem[ 11 ].sSpr  = 2;
	m_stItem[ 11 ].stPos.x = -48*-1;
	m_stItem[ 11 ].stPos.y = 0*-1;
	m_stItem[ 11 ].stPos.z = -2;
	m_stItem[ 11 ].fRAdd = -8.f;

/*
	m_stItem[ 12 ].sType = enEngineItemShaft;
	m_stItem[ 12 ].sSpr  = 0;
	m_stItem[ 12 ].stPos.x = -32*-1;
	m_stItem[ 12 ].stPos.y = 32*-1;
	m_stItem[ 12 ].stPos.z = -2;
	m_stItem[ 12 ].fRAdd   = 8.f;
	m_stItem[ 12 ].fScl    = 1.f;
	m_stItem[ 12 ].fRange  =  24.f;
	m_stItem[ 12 ].fBaseRot = 145+24.f;
	m_stItem[ 12 ].fPosRange = 16.f;
*/

	m_stItem[ 13 ].sType = enEngineItemCyrinder;
	m_stItem[ 13 ].sSpr  = 4;
	m_stItem[ 13 ].stPos.x = 32*-1;
	m_stItem[ 13 ].stPos.y = 32*-1;
	m_stItem[ 13 ].stPos.z = -2;
	m_stItem[ 13 ].fRAdd   = -8.f;
	m_stItem[ 13 ].fScl    = 0.5f;
	m_stItem[ 13 ].fRange  =  0.f;
	m_stItem[ 13 ].fBaseRot =  0.f;
	m_stItem[ 13 ].fPosRange = 32.f;

	m_stItem[ 14 ].sType = enEngineItemCyrinder;
	m_stItem[ 14 ].sSpr  = 4;
	m_stItem[ 14 ].stPos.x = -16*-1;
	m_stItem[ 14 ].stPos.y = 0*-1;
	m_stItem[ 14 ].stPos.z = -3;
	m_stItem[ 14 ].fRAdd = -16.f;
	m_stItem[ 14 ].fRange = ZERO;
	m_stItem[ 14 ].fPosRange = 16.f;
	m_stItem[ 14 ].fScl = 0.5f;

	m_stItem[ 15 ].sType = enEngineItemCyrinder;
	m_stItem[ 15 ].sSpr  = 4;
	m_stItem[ 15 ].stPos.x = -48*-1;
	m_stItem[ 15 ].stPos.y = 32*-1;
	m_stItem[ 15 ].stPos.z = -3;
	m_stItem[ 15 ].fRAdd = -16.f;
	m_stItem[ 15 ].fRange = ZERO;
	m_stItem[ 15 ].fPosRange = -12.f;
	m_stItem[ 15 ].fScl = 0.5f;

	//上部シャフト
	m_stItem[ 16 ].sType = enEngineItemShaft;
	m_stItem[ 16 ].sSpr  = 0;
	m_stItem[ 16 ].stPos.x = 64*1;
	m_stItem[ 16 ].stPos.y = 48*-1;
	m_stItem[ 16 ].stPos.z = -2;
	m_stItem[ 16 ].fRAdd = 4.f;
	m_stItem[ 16 ].fScl = 1.5f;
	m_stItem[ 16 ].fRange = ZERO;
	m_stItem[ 16 ].fBaseRot = 180.f;
	m_stItem[ 16 ].fPosRange = 16.f;

	//コア右横
	m_stItem[ 17 ].sType = enEngineItemGear_L;
	m_stItem[ 17 ].sSpr  = 2;
	m_stItem[ 17 ].stPos.x = -64*-1;
	m_stItem[ 17 ].stPos.y = 48*-1;
	m_stItem[ 17 ].stPos.z = -2;
	m_stItem[ 17 ].fRAdd   = 4.f;
	m_stItem[ 17 ].fScl = 1.5f;

	//右下
	m_stItem[ 18 ].sType = enEngineItemGear_L;
	m_stItem[ 18 ].sSpr  = 2;
	m_stItem[ 18 ].stPos.x = -64*-1;
	m_stItem[ 18 ].stPos.y = -26*-1;;
	m_stItem[ 18 ].stPos.z = -2;
	m_stItem[ 18 ].fRAdd = -4.f;
	m_stItem[ 18 ].fScl = 1.5f;

	//右上奥
	m_stItem[ 19 ].sType = enEngineItemGear_L;
	m_stItem[ 19 ].sSpr  = 2;
	m_stItem[ 19 ].stPos.x = -64*-1;
	m_stItem[ 19 ].stPos.y = -56*1;;
	m_stItem[ 19 ].stPos.z = -4;
	m_stItem[ 19 ].fRAdd = -4.f;
	m_stItem[ 19 ].fScl = 2.5f;

	//一番左の上
	m_stItem[ 20 ].sType = enEngineItemGear_S;
	m_stItem[ 20 ].sSpr  = 1;
	m_stItem[ 20 ].stPos.x = -56;
	m_stItem[ 20 ].stPos.y = -12;
	m_stItem[ 20 ].stPos.z = -1;
	m_stItem[ 20 ].fRAdd = 16.f;
	m_stItem[ 19 ].fScl = 1.5f;

	SetActionSeq( enActionSeqMain );

}


void CEneEx0006Tank::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( m_HitKurai.is_dead() )
	{
		viiMus::PlaySound( enSoundExplosionLarge , gxFalse );
		SetActionSeq( enActionSeqCrash );
		return;
	}

/*
	if(Joy[0].psh&BTN_X)
	{
		SeqCrashCore();
	}
	else
	{
		m_sCrashTimer = 0;
	}
*/

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}

	m_bDamage = m_HitKurai.is_damage();
	m_bCoreDamage = m_bDamage;

	LogicAI();

	//くらい判定登録
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

	m_HitAtari.set_ap( enAtkPointBazooka );
	m_HitAtari.set_hp( HP_ARMORCLASS_B );
	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
	m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );

}


void CEneEx0006Tank::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	if( viiSub::GameCounter()%4 == 0 )
	{
		viiEff::Set(EFF_ENERGYBALL , m_Pos.x , m_Pos.y , argset(300,120,0,200));
	}

	if( m_sAtackWait > 0 )
	{
		m_sAtackWait --;
	}
	else
	{
		Atack();
	}

	switch( m_sSeq ){
	case enPhase1:
		//通常
		m_bDamage = gxFalse;
		m_fRotation += m_fRotationAdd;
		break;

	case enPhase2:
		//コア
		{
			Float32 fRot = ZERO;
			if( m_fRotationAdd > 0.f )
			{
				m_fRotationAdd -= 0.01f;
				if( m_fRotationAdd < 0.5f )
				{
					m_fRotationAdd = 0.5f;
				}
			}
			fRot = m_fRotationAdd;

			CEffectBlackFire *pBlack;
			Sint32 ax,ay;
			ax = m_Pos.x+(-40+viiSub::Rand()%80)*100;
			ay = m_Pos.y+(-40+viiSub::Rand()%80)*100;

			//煙出しとく

			if( m_bDamage )//|| viiSub::Rand()%16 == 0 )
			{
				fRot = -1.f;
				pBlack = new CEffectBlackFire( ax , ay , NULL );
				pBlack->m_Pos.z = PRIO_BG2+1;
			}

//			m_bDamage = gxFalse;
			m_fRotation += fRot;
		}
		break;
	}

	Gimmick();
}


void CEneEx0006Tank::Gimmick()
{
	//-----------------------------------------------------
	//ギミック
	//-----------------------------------------------------

	for(Sint32 ii=0;ii<enPartsMax;ii++)
	{
		switch( m_stItem[ ii ].sType ){
		case enEngineItemNone:
			break;
		case enEngineItemShaft:
			LogicShaft( ii );
			break;
		case enEngineItemGear_S:
		case enEngineItemGear_L:
			LogicGear( ii );
			break;
		case enEngineItemFuta:
			LogicFuta( ii );
			break;
		case enEngineItemCyrinder:
			LogicCyrinder( ii );
			break;
		case enEngineItemCore:
			LogicCore( ii );
			break;
		}
	}

}


void CEneEx0006Tank::LogicShaft(Sint32 n)
{
	//シャフト
	StItem *p = &m_stItem[ n ];

	p->fRot = m_fRotation*p->fRAdd;
	p->fDspRot = p->fBaseRot+(viiMath::Cos100( p->fRot )*p->fRange)/100.f;

	p->fPosRot = p->fRot;
	p->stAdd.x = (viiMath::Cos100( p->fPosRot )*p->fPosRange)/100;
	p->stAdd.y = (viiMath::Sin100( p->fPosRot )*p->fPosRange)/100;
}


void CEneEx0006Tank::LogicGear(Sint32 n)
{
	//ギア
	StItem *p = &m_stItem[ n ];

	//ギア回転

	p->fRot = m_fRotation*p->fRAdd;

	p->fDspRot = p->fRot;

}

void CEneEx0006Tank::LogicCyrinder(Sint32 n)
{
	//シリンダー
	StItem *p = &m_stItem[ n ];

	p->fRot = m_fRotation*p->fRAdd;

	p->fPosRot = p->fRot;

	p->stAdd.y = (viiMath::Sin100( p->fPosRot )*p->fPosRange)/100;
}

void CEneEx0006Tank::LogicFuta(Sint32 n)
{
	//クランク
	StItem *p = &m_stItem[ n ];

	p->fRot = m_fRotation*p->fRAdd;

	p->fDspRot = p->fBaseRot+(viiMath::Cos100( p->fRot )*p->fRange)/100.f;

	
}

void CEneEx0006Tank::LogicCore(Sint32 n)
{
	StItem *p = &m_stItem[ n ];

	switch( m_sSeq ){
	case enPhase1:
		p->fDspRot = -m_fRotation;
		//if( m_bCoreDamage )
		{
		//	m_sAtackWait = 90;
		}
		break;

	case enPhase2:
		if( m_HitKurai.is_dead() )
		{
			p->fRAdd += 0.2f;
			p->fDspRot += p->fRAdd;
		}
		else
		{
			p->fDspRot = -m_fRotation;
		}
		break;
	}
}


void CEneEx0006Tank::Atack()
{
	Sint32 ax,ay,r;

	ax = m_Pos.x;
	ay = m_Pos.y;

	if( viiSub::GameCounter()%60 <= 10 ) return;

	CEne0001EneHyperBall *q;

	r = get_valken_rot( ax , ay );

	q = new CEne0001EneHyperBall( ax , ay , r, 120 );
	q->SetPattern( CEne0001EneHyperBall::enPatternTargetting );

	q = new CEne0001EneHyperBall( ax , ay , r, 120 );
	q->SetPattern( CEne0001EneHyperBall::enPatternTargetting );

	m_sAtackWait = 220;

/*
	for(Sint32 ii=0;ii<4;ii++)
	{
		new CEne0001EneBullet( ax , ay , m_fRotation+45+ii*90 , 240 ,0);
	}
*/

}


void CEneEx0006Tank::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	switch( m_sSeq ){
	case enPhase1:
		ScorePlus( enScore );

		EffectMakeBigBomb(m_Pos.x , m_Pos.y);
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

		m_sSeq    = enPhase2;
		m_bDamage = gxFalse;
		m_HitKurai.set_hp( enHitPointShipSubCoreAfter );
		SetActionSeq( enActionSeqMain );
		return;

	case enPhase2:
		m_fRotation -= m_fRotationAdd;
		Gimmick();
		if( SeqCrashCore() )
		{
			new CEffFire( m_Pos.x , m_Pos.y , PRIO_BG2+1);
			m_bDead = gxTrue;
			m_sSeq  = enPhase3;
		}
		break;

	case enPhase3:
		if( viiSub::Rand()%64 == 0 )
		{
			viiEff::Set( EFF_BREAK_THUNDER , m_Pos.x+(-48+viiSub::Rand()%96)*100 , m_Pos.y+(-64+viiSub::Rand()%128)*100 );
		}
		break;
	}

	//-------------------------------------------
	//煙
	//-------------------------------------------
	CEffectBlackFire *pBlack;
	Sint32 ax,ay;
	ax = m_Pos.x+(-40+viiSub::Rand()%80)*100;
	ay = m_Pos.y+(-40+viiSub::Rand()%80)*100;

	if( viiSub::Rand()%4 == 0 )
	{
		pBlack = new CEffectBlackFire( ax , ay , NULL );
		pBlack->m_Pos.z = PRIO_EFFECT;
	}
	//--------------------------------------------

	if( m_bClear )
	{
		SetActionSeq( enActionSeqEnd );
	}

}


gxBool CEneEx0006Tank::SeqCrashCore()
{
	//---------------------------------------------
	//破壊シーケンス
	//---------------------------------------------

	if( m_sCrashTimer == 10 )
	{
		m_bDisp = gxTrue;
		new CEffectFinalExplosion( m_Pos.x , m_Pos.y  , 3 );
	}
	else if( m_sCrashTimer == 140 )
	{
		m_bDisp = gxFalse;
		viiMus::PlaySound( enSoundCrashA );
		for(Sint32 ii=0;ii<enPartsMax;ii++)
		{
			CEffectDmgprt *pDmg;
			Sint32 ax=0,ay=0,az=0,r,sSpr;

			switch( m_stItem[ ii ].sType ){
			case enEngineItemNone:
			case enEngineItemFuta:
			case enEngineItemCore:
				continue;

			case enEngineItemShaft:
			case enEngineItemGear_S:
			case enEngineItemGear_L:
			case enEngineItemCyrinder:
				sSpr = m_stItem[ ii ].sSpr;
				ax = m_Pos.x + m_stItem[ ii ].stPos.x*100 + m_stItem[ ii ].stAdd.x*100;
				ay = m_Pos.y + m_stItem[ ii ].stPos.y*100 + m_stItem[ ii ].stAdd.y*100;
				az = m_Pos.z + m_stItem[ ii ].stPos.z;
				r = viiMath::Atan2_100( (ay-m_Pos.y)/100 , (ax-m_Pos.x)/100 )/100;
				pDmg = new CEffectDmgprt( ax, ay , r , 320 );
				pDmg->SetSprite( &SprCEneEx0006Tank[sSpr] );
				break;
			}
		}

	}
	else if( m_sCrashTimer > 180 )
	{
		return gxTrue;
	}

	m_sCrashTimer ++;

	return gxFalse;

}

void CEneEx0006Tank::Draw()
{
	//----------------------------------------------
	//描画
	//----------------------------------------------
	Sint32 ax,ay;

	if( !m_bDisp ) return;

	ax = m_Pos.x + 0*100;
	ay = m_Pos.y + 0*100;

	for(Sint32 ii=0;ii<enPartsMax;ii++)
	{
		switch( m_stItem[ ii ].sType ){
		case enEngineItemNone:
			break;
		case enEngineItemShaft:
			DrawShaft( ii );
			break;
		case enEngineItemGear_S:
		case enEngineItemGear_L:
			DrawGear( ii );
			break;
		case enEngineItemFuta:
			DrawFuta( ii );
			break;
		case enEngineItemCyrinder:
			DrawCyrinder( ii );
			break;
		case enEngineItemCore:
			DrawCore( ii );
			break;
		}
	}

}

void CEneEx0006Tank::DrawShaft(Sint32 n)
{
	//----------------------------------------------
	//シャフト描画
	//----------------------------------------------

	Sint32 ax=0,ay=0,az=0;
	StItem *p = &m_stItem[ n ];

	ax = m_Pos.x + p->stPos.x*100 + p->stAdd.x*100;
	ay = m_Pos.y + p->stPos.y*100 + p->stAdd.y*100;
	az = m_Pos.z + p->stPos.z;

	viiDraw::Sprdmg( m_bDamage , &SprCEneEx0006Tank[ 0 ] , ax , ay , az , ATR_DFLT , ARGB_DFLT ,p->fScl , (Sint32)p->fDspRot );

}


void CEneEx0006Tank::DrawGear(Sint32 n)
{
	//----------------------------------------------
	//ギア描画
	//----------------------------------------------
	Sint32 ax=0,ay=0,az=0;
	StItem *p = &m_stItem[ n ];

	ax = m_Pos.x + p->stPos.x*100 + p->stAdd.x*100;
	ay = m_Pos.y + p->stPos.y*100 + p->stAdd.y*100;
	az = m_Pos.z + p->stPos.z;

	switch(p->sType){
	case enEngineItemGear_S:
		viiDraw::Sprdmg( m_bDamage , &SprCEneEx0006Tank[ 1 ] , ax , ay , az , ATR_DFLT , ARGB_DFLT ,p->fScl , (Sint32)p->fDspRot );
		break;

	case enEngineItemGear_L:
		viiDraw::Sprdmg( m_bDamage , &SprCEneEx0006Tank[ 2 ] , ax , ay , az , ATR_DFLT , ARGB_DFLT ,p->fScl , (Sint32)p->fDspRot );
		break;
	}
}


void CEneEx0006Tank::DrawCyrinder(Sint32 n)
{
	//----------------------------------------------
	//シリンダー描画
	//----------------------------------------------
	Sint32 ax=0,ay=0,az=0;
	StItem *p = &m_stItem[ n ];

	ax = m_Pos.x + p->stPos.x*100 + p->stAdd.x*100;
	ay = m_Pos.y + p->stPos.y*100 + p->stAdd.y*100;
	az = m_Pos.z + p->stPos.z;

	viiDraw::Sprdmg( m_bDamage , &SprCEneEx0006Tank[ 4 ] , ax , ay , az , ATR_DFLT , ARGB_DFLT ,p->fScl , (Sint32)p->fDspRot );
}


void CEneEx0006Tank::DrawFuta(Sint32 n)
{
	//----------------------------------------------
	//フタ描画
	//----------------------------------------------
	Sint32 ax=0,ay=0,az=0;
	StItem *p = &m_stItem[ n ];

	ax = m_Pos.x + p->stPos.x*100 + p->stAdd.x*100;
	ay = m_Pos.y + p->stPos.y*100 + p->stAdd.y*100;
	az = m_Pos.z + p->stPos.z;

	viiDraw::Sprdmg( m_bDamage , &SprCEneEx0006Tank[ 3 ] , ax , ay , az , ATR_DFLT , ARGB_DFLT ,1.0f , m_fRotation );
}


void CEneEx0006Tank::DrawCore(Sint32 n)
{
	//----------------------------------------------
	//コア描画
	//----------------------------------------------
	Sint32 ax=0,ay=0,az=0;
	StItem *p = &m_stItem[ n ];

	ax = m_Pos.x + p->stPos.x*100 + p->stAdd.x*100;
	ay = m_Pos.y + p->stPos.y*100 + p->stAdd.y*100;
	az = m_Pos.z + p->stPos.z;

	viiDraw::Sprdmg( m_bCoreDamage , &SprCEneEx0006Tank[ 5 ] , ax , ay , az , ATR_DFLT , ARGB_DFLT ,1.0f , (Sint32)p->fDspRot );
}


