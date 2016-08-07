//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "EnemyTbl/CEneH0008GunTankTbl.h"
#include "../../Effect/EffectBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -24,
	enKuraiTop   = -70,
	enKuraiRight =  24,
	enKuraiBottom=  0,

	enScore = 300,
	enSpdAdd = 4,
	enSpdMax = 96,
};


CEneH0008GunTank::CEneH0008GunTank( Sint32 x, Sint32 y ,gxBool bDummy)
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_HitKurai.set_hp( HP_ARMORCLASS_A );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_LEFT;
	m_sAtribute = ATR_DFLT;

	m_sGatringRot    = 0;
	m_sGatringRotAdd = 0;
	m_sControl = 0;
	m_sRotation = 0;
	m_sAtackWait = 0;
	m_sArmRebound  = 0;
	m_sReboundWait = 0;
	m_bDamage = gxFalse;

	m_pEnemyAnime = new CEnemyAnime();
	m_pCatapiller = new CCatapiller();

	m_pCatapiller->SetSpriteIndex( enKYATAPIRA1 , enKYATAPIRA22 );

	m_Add.x = 0;
	m_Add.y = 0;

	m_pTgt = NULL;
	m_bDummy = bDummy;

	Init();
}


CEneH0008GunTank::~CEneH0008GunTank()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

	delete m_pCatapiller;
	delete m_pEnemyAnime;
}

void CEneH0008GunTank::Init()
{

	for(Sint32 ii=0; ii<enMax ;ii++)
	{
		m_pEnemyAnime->SetParts( ii , enMax , m_sParentCEneH0008GunTank[ii] , sPosCEneH0008GunTank[ii] );
	}

	m_pEnemyAnime->PosCalc();

	m_pEnemyAnime->GetPartsDist( enUDE  , &stArm );
	m_pEnemyAnime->GetPartsDist( enARM1 , &stGtl );

}


void CEneH0008GunTank::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( m_HitKurai.is_dead() )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

//	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
//	{
//		return;
//	}

	m_pEnemyAnime->PosCalc();

	LogicAI();
	Gimmick();

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	SetRader( enTypeMarkerGroundPartner );
	DirectionAdjust();

	m_bDamage = m_HitKurai.is_damage();

	//くらい判定登録
	//m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );
	if( !m_bDummy )
	{
		m_HitKurai.SetHantei(ID_VERSIS_DEF , &m_Pos );
	}
}


void CEneH0008GunTank::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	Sint32 sDist = 12800;
	m_sControl = 0;//Joy[0].psh;

	Sint32 tx = GetTargetPlayer()->x+160*100;

	if( m_pTgt )
	{
		tx = m_pTgt->x+12800;
	}

	if( tx < m_Pos.x )
	{

	}
	else if( tx > m_Pos.x )
	{
		m_sControl = JOY_R;
	}

	if(m_sTimer%120<64)
	{
		m_sControl |= BTN_X;
	}
	m_sControl |= JOY_U;

	{
		Sint32 kabe = pGame->pBg->get_kabetype( m_Pos.x/100 , m_Pos.y/100 );
		if( kabe == 0 )
		{
			m_Pos.y += 10;
		}
		if( kabe  )
		{
			m_Pos.y -= 10;
		}
	}


}



void CEneH0008GunTank::Gimmick()
{
	//-----------------------------------------------------
	//動作ギミック
	//-----------------------------------------------------

	if( m_sReboundWait > 0)
	{
		m_sControl = BTN_L1;

		m_sReboundWait -= 1;

		if( m_sReboundWait <= 0 )
		{
			m_sReboundWait = 0;
		}

		if( gxAbs(m_Add.x) > 100 )
		{
			m_sReboundWait = 1;
		}
	}
	else if(m_sControl & JOY_U )
	{
		if( m_pEnemyAnime->GetParts(enCANON)->rz > -45 ) m_pEnemyAnime->GetParts(enCANON)->rz -- ;
	}
	else if(m_sControl & JOY_D )
	{
		if( m_pEnemyAnime->GetParts(enCANON)->rz < 0 ) m_pEnemyAnime->GetParts( enCANON )->rz ++ ;
	}

	if(m_sControl & BTN_X )
	{
		//ガトリング砲回転

		m_sGatringRotAdd = 100;
	}
	else
	{
		if( m_sGatringRotAdd > 0) m_sGatringRotAdd --;
	}

	if( (m_sReboundWait == 0) && (m_sControl & BTN_Y) )
	{
		if( m_sDirection == DIR_LEFT )
		{
			m_Add.x = enSpdMax;
		}
		else
		{
			m_Add.x = -enSpdMax;
		}
		m_sReboundWait = 100;
		m_sControl |= BTN_L1;
	}

	m_sGatringRot += m_sGatringRotAdd;

	//-----------------------------------------------------------
	//キャタピラ稼動
	//-----------------------------------------------------------

	if(m_sControl & JOY_L )
	{
		if( m_Add.x >= -enSpdMax ) m_Add.x -= enSpdAdd;
		m_sRotation += (45-m_sRotation)/10;

		if( m_sDirection == DIR_LEFT )
		{
			m_pCatapiller->MoveLeft();
		}
		else
		{
			m_pCatapiller->MoveRight();
		}

	}
	else if(m_sControl & JOY_R )
	{
		if( m_Add.x <= enSpdMax  ) m_Add.x += enSpdAdd;
		m_sRotation += (135-m_sRotation)/10;

		if( m_sDirection == DIR_LEFT )
		{
			m_pCatapiller->MoveRight();
		}
		else
		{
			m_pCatapiller->MoveLeft();
		}
	}
	else
	{
		m_pCatapiller->MoveNone();

		if( m_Add.x > 0 )
		{
			m_Add.x -= enSpdAdd;
			if( m_Add.x <= 0 )
			{
				m_Add.x = 0;
			}
		}
		else if( m_Add.x < 0 )
		{
			m_Add.x += enSpdAdd;
			if( m_Add.x >= 0 )
			{
				m_Add.x = 0;
			}
		}
	}


	if( m_sControl&BTN_L1 )
	{

	}
	else
	{
		if(m_Add.x < -enSpdMax/2 )
		{
			m_sDirection = DIR_LEFT;
		}
		else if(m_Add.x > enSpdMax/2 )
		{
			m_sDirection = DIR_RIGHT;
		}
	}

	m_pCatapiller->Action( m_pEnemyAnime );

	//-----------------------------------------------------------
	//車輪稼動
	//-----------------------------------------------------------

	for(Sint32 ii=enTIYA1; ii<=enTIYA7; ii++)
	{
		//タイヤ回転処理
		m_pEnemyAnime->GetParts( ii )->dr = -( m_pEnemyAnime->GetParts( ii )->rz+360*m_pCatapiller->GetRotation()/100);
	}


	//-----------------------------------------------------------
	//腕回転
	//-----------------------------------------------------------
	m_pEnemyAnime->GetParts( enKATA )->dr = m_sRotation+(viiMath::Cos100(m_sArmRebound)*10)/100;

	Sint32 sRot,ox,oy,ax,ay;

	sRot = m_pEnemyAnime->GetParts( enKATA )->dr;
	ox = m_pEnemyAnime->GetParts( m_pEnemyAnime->GetParts( enUDE )->sParent )->dx;
	oy = m_pEnemyAnime->GetParts( m_pEnemyAnime->GetParts( enUDE )->sParent )->dy;
	ax = viiMath::Cos100( sRot )*stArm.dist/100;
	ay = viiMath::Sin100( sRot )*stArm.dist/100;
	m_pEnemyAnime->GetParts( enUDE )->dx = ox + ax;
	m_pEnemyAnime->GetParts( enUDE )->dy = oy + ay;
	m_pEnemyAnime->GetParts( enUDE )->dr = 0;

	sRot = m_pEnemyAnime->GetParts( enUDE )->dr;
	ox = m_pEnemyAnime->GetParts( m_pEnemyAnime->GetParts( enARM1 )->sParent )->dx;
	oy = m_pEnemyAnime->GetParts( m_pEnemyAnime->GetParts( enARM1 )->sParent )->dy;
	ax = viiMath::Cos100( sRot )*stArm.dist/100;
	ay = viiMath::Sin100( sRot )*stArm.dist/100;
	m_pEnemyAnime->GetParts( enARM1 )->dx = ox + ax;
	m_pEnemyAnime->GetParts( enARM1 )->dy = oy + ay;
	m_pEnemyAnime->GetParts( enARM1 )->dr = 0;


	if( m_sArmRebound > 0)
	{
		m_sArmRebound -= 8;
		if( m_sArmRebound < 0)
		{
			m_sArmRebound = 0;
		}
	}


	Atack();

}


void CEneH0008GunTank::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------
	Sint32 sRotParts = 0;

	if( m_bDummy ) return;

	if( m_sGatringRotAdd > 0)
	{
		sRotParts = m_sGatringRot/400;
		sRotParts = sRotParts%3;

		m_sRotation += (25-m_sRotation)/10;

		switch( sRotParts ){
		case 0:
			m_pEnemyAnime->GetParts( enARM1 )->sDispSprite = m_pEnemyAnime->GetParts( enARM1 )->sSprite;
			break;
		case 1:
			m_pEnemyAnime->GetParts( enARM1 )->sDispSprite = m_pEnemyAnime->GetParts( enARM2 )->sSprite;
			break;
		case 2:
			m_pEnemyAnime->GetParts( enARM1 )->sDispSprite = m_pEnemyAnime->GetParts( enARM3 )->sSprite;
			break;
		}

		if( m_sAtackWait > 0 )
		{
			m_sAtackWait --;

		}
		else
		{
			if( m_sGatringRot/100 >= 60 && m_sGatringRotAdd >= 100)
			{
				AtackGatring();
				m_sArmRebound = 100;
				m_sAtackWait = 8;
			}
		}
	}
	else
	{
		m_sGatringRot = 0;
	}

	ghPos stPos;
	stPos.x = m_Pos.x+0;
	stPos.y = m_Pos.y-6400;
	if( m_sTimer%128==0)
	{
		viiEff::Set( EFF_MAZZLE , stPos.x , stPos.y , NULL );
		//new CAtkH0001Bazooka(stPos.x , stPos.y , -32 , 2400);
		new CAtkH0001Ryudan( stPos.x , stPos.y , -45, 1780 );
		new CAtkH0001Ryudan( stPos.x , stPos.y , -45, 1880 );
	}
}


void CEneH0008GunTank::AtackGatring()
{
	//----------------------------------------
	//ガトリングガン
	//----------------------------------------

	Sint32 sDir = 1,ax,ay;

	if( m_sDirection == DIR_LEFT )
	{
		sDir = -1;
	}

	ax = m_Pos.x + m_pEnemyAnime->GetParts( enARM1 )->dx*100*sDir;
	ay = m_Pos.y + m_pEnemyAnime->GetParts( enARM1 )->dy*100;

	Sint32 sRot = -6+viiSub::Rand()%12;
	sRot = (m_sDirection == DIR_RIGHT)? sRot : ( sRot*-1+180);

	if( !m_bDummy )
	{
		new CEffGattlingGun( ax, ay , sRot , 800 );
	}
//	new CEne0001EneCrasher( ax, ay , sRot , 400 );

}


void CEneH0008GunTank::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	viiMus::PlaySound(enSoundExplosionMiddle);

	ScorePlus( enScore );

	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

	Sint32 sDir = 1;

	if( m_sDirection == DIR_LEFT )
	{
		sDir = -1;
	}

	for(Sint32 ii=0; ii<enMax ;ii++)
	{
		switch( ii ){
		case enARM2:
		case enARM3:
			continue;
		}

		StPartsInfo *p = m_pEnemyAnime->GetParts( ii );

		_opt_[0] = 0;//(Sint32)&SprCEneH0008GunTank[ p->sDispSprite ];
		_opt_[1] = (sDir==-1)? ATR_FLIP_X : ATR_DFLT;
		_opt_[2] = ARGB_DFLT;
		_opt_[3] = p->dr;
		_opt_[4] = 100;
		_opt_[5] = 1;
		viiEff::Set(EFF_DAMAGE_PARTS,m_Pos.x+p->dx*100*sDir,m_Pos.y + p->dy*100, _opt_,&SprCEneH0008GunTank[ p->sDispSprite ] );
		viiEff::SetBombSimple( m_Pos.x+p->dx*100*sDir,m_Pos.y + p->dy*100 , 1 , 9 );

	}

	SetActionSeq( enActionSeqEnd );

}

void CEneH0008GunTank::Draw()
{

	gxBool bDamage = m_HitKurai.is_damage();

	if( m_bDummy )
	{
		m_pEnemyAnime->SetPrio( PRIO_BG1+1 );
	}

	m_pEnemyAnime->GetParts( enARM2 )->bDisp = gxFalse;
	m_pEnemyAnime->GetParts( enARM3 )->bDisp = gxFalse;

	m_pEnemyAnime->Draw( m_bDamage , m_Pos.x , m_Pos.y , ( m_sDirection == DIR_LEFT )? gxTrue : gxFalse , &SprCEneH0008GunTank[0] );

#if 0
	for(Sint32 ii=0; ii<enMax ;ii++)
	{
		switch( ii ){
		case enARM2:
		case enARM3:
			continue;
		}

		p = m_pEnemyAnime->GetParts( ii );

/*
		if( ii == enKYATAPIRA1 )
		{
			viiDbg::printf(200,60,32,"CAP(%03d/%03d)",p->dx , p->dy);
		}
*/

		viiDraw::Sprdmg(
			bDamage ,
			&SprCEneH0008GunTank[ p->sDispSprite ] ,
			m_Pos.x + p->dx*100,
			m_Pos.y + p->dy*100,
			PRIO_ENEMY + p->dz,
			m_sAtribute,	//|m_sAtributeDir ,
			ARGB_DFLT,
			p->sx/100.f,
			p->dr);
	}
#endif

}

//センター位置は必ず０，０とする
//親は必ず自分より前に定義すること
//回転方向は次のやつとうまくつながるように
