//--------------------------------------------------------------------------------
//
// Stage00:巨大キャノン砲台（ver.2）
//
//--------------------------------------------------------------------------------

#include "../HoundsEnemy.h"
#include "../../../Effect/EffectBase.h"
//#include "../EnemyTbl/CEneEx0002LaserTBL.h"

enum {
	enScrollOutWidth  = 128*100,
	enScrollOutHeight = 128*100,

	enKuraiLeft  = -64,
	enKuraiTop   = -64,
	enKuraiRight =  64,
	enKuraiBottom=  64,

	enScore = 300,
};


gxSprite SprCEneEx0002Laser[]=
{
	{ enTexPageEnemyStage01 , 0,0  ,256,144,64 ,72 },
	{ enTexPageEnemyStage01 , 0,160,112,48 ,32  ,24  },
	{ enTexPageEnemyStage01 , 0,208,144,48 ,16  ,24  },
};


CEneEx0002Laser::CEneEx0002Laser( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_HitKurai.set_hp( enHitPointShipSubCannon );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_RIGHT;
	m_sAtribute = ATR_DFLT;

	m_sRebound = 100;

	m_bDamage   = gxFalse;
	m_bDisp     = gxTrue;

	m_fRotation = 180.f;
	m_sOffset   = 0;
	m_sCrashSeq = 0;
	m_sTackLag = 0;
	m_sType = 0;

	m_sAtackCnt  = 0;
	m_sAtackWait = 0;

}


CEneEx0002Laser::~CEneEx0002Laser()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneEx0002Laser::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( m_HitKurai.is_dead() )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

/*
	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}
*/

	LogicAI();

	NORMALIZE( m_fRotation );
/*
	while( m_fRotation < -360.f )
	{
		m_fRotation += 360.f;
	}
	while( m_fRotation > 360.f )
	{
		m_fRotation -= 360.f;
	}
*/

	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	m_bDamage = m_HitKurai.is_damage();

	//くらい判定登録
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

}


void CEneEx0002Laser::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	Gimmick();

	AtackDrop();

	if( m_sTackLag > 0 )
	{
		m_sTackLag --;
	}
	else
	{
		m_sTackLag = 150;
		return;
	}

	if( m_sTackLag >= 134 )
	{

	}
	else
	{
		if( m_sTackLag >= 128 )
		{
			Sint32 ax,ay;

			ax = m_CannonSaki[0].x;
			ay = m_CannonSaki[0].y;

			viiEff::Set(EFF_ENERGYBALL , ax , ay , argset(200,100,0,100));

			ax = m_CannonSaki[1].x;
			ay = m_CannonSaki[1].y;

			viiEff::Set(EFF_ENERGYBALL , ax , ay , argset(200,100,0,100));
			m_sSyuhouCnt = 0;
		}

		if( m_sTackLag < 12 && m_sTackLag%2 == 0 )
		{
			if( m_sSyuhouCnt == 0 )
			{
				viiMus::PlaySound( enSoundBazooka );
				//viiMus::PlaySound(enSoundEneShotLaser);
			}
			Atack();
			m_sRebound = 0;
			m_sSyuhouCnt ++;
		}
		return;
	}

	//----------------------------------------------------
	//角度回転
	//----------------------------------------------------

	Sint32 r   = get_valken_rot( m_Pos.x , m_Pos.y+2800 );
	Sint32 dir = viiMath::GetTargetAngle( m_fRotation , r  );

	if( dir < -2 )
	{
		m_sTackLag = 150;
		m_fRotation -= 0.6f;
	}
	else if( dir >  2  )
	{
		m_sTackLag = 150;
		m_fRotation += 0.6f;
	}

	if( m_sType == 1 )
	{
		//後部リアキャノンの専用処理（-45～+45）
		NORMALIZE( m_fRotation );
		if( m_fRotation > 180 )
		{
			m_fRotation = -(180+(180-m_fRotation));
		}
		if( m_fRotation < -45.f ) m_fRotation = -45.f;
		if( m_fRotation >  45.f ) m_fRotation =  45.f;

	}
}


void CEneEx0002Laser::Gimmick()
{
	//-----------------------------------------------------
	//ギミック
	//-----------------------------------------------------

	Sint32 sRootDist; Float32 fRootRot;
	Sint32 sBaseDist; Float32 fBaseRot;
	Sint32 sSakiDist; Float32 fSakiRot;

	sRootDist = 10;
	fRootRot  = -90+m_fRotation;

	m_CannonRoot[0].x = m_Pos.x + viiMath::Cos100( fRootRot )*sRootDist;
	m_CannonRoot[0].y = m_Pos.y + viiMath::Sin100( fRootRot )*sRootDist;

	sBaseDist = 32+20*m_sRebound/100;
	fBaseRot  = m_fRotation;

	m_CannonBase[0].x = m_CannonRoot[0].x + viiMath::Cos100( fBaseRot )*sBaseDist;
	m_CannonBase[0].y = m_CannonRoot[0].y + viiMath::Sin100( fBaseRot )*sBaseDist;

	sSakiDist = 40+40*m_sRebound/100;
	fSakiRot  = m_fRotation;

	m_CannonSaki[0].x = m_CannonBase[0].x + viiMath::Cos100( fSakiRot )*sSakiDist;
	m_CannonSaki[0].y = m_CannonBase[0].y + viiMath::Sin100( fSakiRot )*sSakiDist;

	//--------------------

	sRootDist = 35;
	fRootRot  = 90+m_fRotation;

	m_CannonRoot[1].x = m_Pos.x + viiMath::Cos100( fRootRot )*sRootDist;
	m_CannonRoot[1].y = m_Pos.y + viiMath::Sin100( fRootRot )*sRootDist;

	sBaseDist = 32+20*m_sRebound/100;
	fBaseRot  = m_fRotation;

	m_CannonBase[1].x = m_CannonRoot[1].x + viiMath::Cos100( fBaseRot )*sBaseDist;
	m_CannonBase[1].y = m_CannonRoot[1].y + viiMath::Sin100( fBaseRot )*sBaseDist;

	sSakiDist = 40+40*m_sRebound/100;
	fSakiRot  = m_fRotation;

	m_CannonSaki[1].x = m_CannonBase[1].x + viiMath::Cos100( fSakiRot )*sSakiDist;
	m_CannonSaki[1].y = m_CannonBase[1].y + viiMath::Sin100( fSakiRot )*sSakiDist;

	if( m_sRebound < 100 )
	{
		Sint32 sAdd = (100-m_sRebound)/10;
		if( sAdd == 0 )
		{
			sAdd = 1;
		}
		m_sRebound += sAdd;
	}

}


void CEneEx0002Laser::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

	Sint32 ax,ay;

	ax = m_CannonSaki[0].x;
	ay = m_CannonSaki[0].y;

	new CEne0001EneCannon( ax , ay , m_fRotation , 1640 );

	ax = m_CannonSaki[1].x;
	ay = m_CannonSaki[1].y;

	new CEne0001EneCannon( ax , ay , m_fRotation , 1640 );

}


void CEneEx0002Laser::AtackDrop()
{
	//-------------------------------------
	//ドロップショット攻撃
	//-------------------------------------

	if( m_sType == 1 )
	{
		if( m_sAtackWait > 60 )
		{
			m_sAtackCnt = 0;
			m_sAtackWait --;
		}
		else
		{
			if( m_sAtackCnt %10 == 0 )
			{
				Sint32 ax,ay;
				ax = -3200*2+m_Pos.x+(m_sAtackCnt/10)*3200;
				ay = m_Pos.y;
				CEne0001EneSuperBall *q;
				q = new CEne0001EneSuperBall( ax , ay , -90+(m_sAtackCnt/10)*10, 1120  );	//+viiSub::Rand()%45
				q->SetPattern( CEne0001EneSuperBall::enPatternDrop );
				q->SetScale( 0.5f );
				q->SetEndless(  );
				q->SetDropGensui(gxTrue);

				q = new CEne0001EneSuperBall( ax , ay , -90+(m_sAtackCnt/10)*10, 920  );	//+viiSub::Rand()%45
				q->SetPattern( CEne0001EneSuperBall::enPatternDrop );
				q->SetScale( 0.5f );
				q->SetEndless(  );
				q->SetDropGensui(gxTrue);

				viiEff::Set( EFF_MAZZLE , ax , ay , NULL );
				viiEff::SetBombSimple( ax , ay , 1 , 1+viiSub::Rand()%2 );
			}
			m_sAtackCnt ++;
			if( m_sAtackCnt == 60 )
			{
				m_sAtackWait = 300;
			}
		}
	}
}


void CEneEx0002Laser::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	switch( m_sCrashSeq ){
	case 0:
		viiMus::PlaySound( enSoundExplosionLarge , gxFalse );
		ScorePlus( enScore );
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
		new CEffectFinalExplosion( m_Pos.x , m_Pos.y , 8 );
		break;

	case 110:
		viiMus::PlaySound( enSoundCrashA );
		m_bDisp = gxFalse;
		m_bDead = gxTrue;
		break;
	}

	m_sCrashSeq ++;

	//-------------------------------------------
	//煙
	//-------------------------------------------
	CEffectBlackFire *pBlack;
	Sint32 ax,ay;
	ax = m_Pos.x+(-32+viiSub::Rand()%80)*100;
	ay = m_Pos.y+(-64+viiSub::Rand()%64)*100;

	pBlack = new CEffectBlackFire( ax , ay , NULL );
	pBlack->m_Pos.z = PRIO_EFFECT;

	//--------------------------------------------

	AtackDrop();

	if( m_bClear )
	{
		SetActionSeq( enActionSeqEnd );
	}

}

void CEneEx0002Laser::Draw()
{
	Sint32 y = m_sOffset;

	if( !m_bDisp ) return;

	viiDraw::Sprdmg( m_bDamage , &SprCEneEx0002Laser[ 0 ] , m_Pos.x , m_Pos.y + y ,PRIO_BG3+1 , m_sAtribute|m_sAtributeDir , ARGB_DFLT ,1.f ,m_fRotation );

	for(Sint32 ii=0;ii<2;ii++)
	{
		viiDraw::Sprdmg( m_bDamage , &SprCEneEx0002Laser[ 1 ] , m_CannonBase[ii].x , m_CannonBase[ii].y + y ,PRIO_BG3+1 , m_sAtribute|m_sAtributeDir , ARGB_DFLT ,1.f ,m_fRotation );
		viiDraw::Sprdmg( m_bDamage , &SprCEneEx0002Laser[ 2 ] , m_CannonSaki[ii].x , m_CannonSaki[ii].y + y ,PRIO_BG3+1 , m_sAtribute|m_sAtributeDir , ARGB_DFLT ,1.f ,m_fRotation );
	}
}

