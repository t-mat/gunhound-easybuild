//------------------------------------------------------------
//
//パンチ（エネルギー球） (角度,速度,大きさ(%))
//
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

CEffAtkHomingMissile::CEffAtkHomingMissile( Sint32 x,Sint32 y , Sint32 sRot , Sint32 sLevel )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;

	m_sRotation        = sRot;				//初期方向
	m_sRotationAdd     = (3+viiSub::Rand()%3)*(viiSub::Rand()%2)? 4 : -4;
	m_sRotationMaster  = sRot;
	m_sHomingLevel     = sLevel;			//ホーミングレベル
	m_sHomingWait      = 30+viiSub::Rand()%20;	//ホーミング開始時間
	m_sHomingAdjustSpd = -3+viiSub::Rand()%7;	//軌道修正（ずらし）レベル
	m_sSpeed = 300;							//スピード

	m_HitAtari.set_hp(1);

	m_Add.x = (viiMath::Cos100(m_sRotation)*m_sSpeed)/100;
	m_Add.y = (viiMath::Sin100(m_sRotation)*m_sSpeed)/100;

	m_HitAtari.ax1 = -6;
	m_HitAtari.ay1 = -6;
	m_HitAtari.ax2 =  6;
	m_HitAtari.ay2 =  6;

}


CEffAtkHomingMissile::~CEffAtkHomingMissile()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------
	
	
}


void CEffAtkHomingMissile::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------

	//ホーミング
	gxBool bHoming = gxFalse;

	if(m_sHomingWait > 0)
	{
		//ホーミング開始までの時間制御
		m_sHomingWait--;
	}

	Sint32 sRangeScreenOut = 0;	//画面外許容範囲

	switch(m_sHomingLevel){
	case 0:
		//ホーミング性能ナシ
		m_HitAtari.set_ap(5);
		missileLv1();
		sRangeScreenOut = 0*100;
		break;
	case 1:
		//ＬＶ２簡易ホーミング（壁判定なし）
		m_HitAtari.set_ap(8);
		sRangeScreenOut = 32*100;
		missileLv2();
		break;
	default:
		//Ｌｖ３ホーミング（壁判定なし）
		m_HitAtari.set_ap(10);
		sRangeScreenOut = 64*100;
		missileLv3();
		break;
	}

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	viiEff::Set(EFF_MISSILE_SMOKE,m_Pos.x           , m_Pos.y           , NULL );
	viiEff::Set(EFF_MISSILE_SMOKE,m_Pos.x+m_Add.x/2 , m_Pos.y+m_Add.y/2 , NULL );

	//存在チェック
	if(viiSub::IsScrollOut( m_Pos.x , m_Pos.y , sRangeScreenOut , sRangeScreenOut ) )
	{
		SetActionSeq( enActionSeqCrash );
	}

	if( m_HitAtari.IsImpacted() )
	{
		viiEff::SetBombEffect( m_Pos.x , m_Pos.y , 3 , 5 );
		SetActionSeq( enActionSeqCrash );
		return;
	}

	m_HitAtari.SetHantei( ID_VERSIS_ATK,&m_Pos );

}


void CEffAtkHomingMissile::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

	SetActionSeq( enActionSeqEnd );

}


void CEffAtkHomingMissile::missileLv1()
{
	//-------------------------------------------
	//ミサイルロジックLV1（ホーミング性能ナシ）
	//-------------------------------------------
	if(m_sSpeed <= 800)
	{
		m_sSpeed += 8;
	}

	if( m_sRotation < m_sRotationMaster ) m_sRotationAdd += 1;
	if( m_sRotation > m_sRotationMaster ) m_sRotationAdd -= 1;
	m_sRotation += m_sRotationAdd;

	m_Add.x = (viiMath::Cos100(m_sRotation)*m_sSpeed)/100;
	m_Add.y = (viiMath::Sin100(m_sRotation)*m_sSpeed)/100;

}

void CEffAtkHomingMissile::missileLv2()
{
	//-------------------------------------
	//ミサイルロジックLV2
	//-------------------------------------
	Sint32 px,py;

	if(m_sHomingWait > 15)
	{
		//途中まではLv1と同じ
		missileLv1();
		return;
	}

	if( m_sHomingAdjustSpd < 32 ) m_sHomingAdjustSpd ++;

	if( getTargetEnemyPosition(m_Pos.x/100 , m_Pos.y/100 , px , py) )
	{
		//ホーミング性能が大幅にアップ
		px *= 100;
		py *= 100;

		if(px < m_Pos.x) m_Add.x -= m_sHomingAdjustSpd;
		if(px > m_Pos.x) m_Add.x += m_sHomingAdjustSpd;
		if(py < m_Pos.y) m_Add.y -= m_sHomingAdjustSpd;
		if(py > m_Pos.y) m_Add.y += m_sHomingAdjustSpd;
	}

	m_sRotation = (Sint32)viiMath::Atan2d( m_Add.y , m_Add.x );//(RAD2DEG(gxAtan(m_Add.y,m_Add.x)));

	if( viiSub::GetBoundBlock_Player( m_Pos.x , m_Pos.y ) )
	{
		//壁にあたった
		new CEffBomb( m_Pos.x , m_Pos.y , 1 , 0 );
		SetActionSeq( enActionSeqCrash );
		return;
	}

}


void CEffAtkHomingMissile::missileLv3()
{
	//-------------------------------------
	//ミサイルロジックLV3
	//-------------------------------------
	Sint32 px,py;

	if(m_sHomingWait > 0)
	{
		//途中まではLv1と同じ
		missileLv1();
		return;
	}

	if( getTargetEnemyPosition(m_Pos.x/100 , m_Pos.y/100 , px , py) )
	{
		//ホーミング性能が大幅にアップ
		px *= 100;
		py *= 100;

		if(px < m_Pos.x) m_Add.x -= 48;
		if(px > m_Pos.x) m_Add.x += 48;
		if(py < m_Pos.y) m_Add.y -= 48;
		if(py > m_Pos.y) m_Add.y += 48;
	}

	m_sRotation = (Sint32)viiMath::Atan2d( m_Add.y , m_Add.x );//(RAD2DEG(gxAtan(m_Add.y,m_Add.x)));

	if( viiSub::GetBoundBlock_Player( m_Pos.x , m_Pos.y ) )
	{
		//壁にあたった
		new CEffBomb( m_Pos.x , m_Pos.y , 1 , 0 );
		SetActionSeq( enActionSeqCrash );
		return;
	}

}


gxBool CEffAtkHomingMissile::getTargetEnemyPosition(Sint32 x,Sint32 y,Sint32 &px,Sint32 &py)
{
	//-------------------------------------
	//ロックオンロジック
	//最も近い敵の座標を得る
	//該当の敵がいない場合にはgxFalseを返す
	//-------------------------------------
	double kyori,min_dist = SCR_W*SCR_H;
	Sint32 min=-1;
	Sint32 x1,y1;

	//座標は1/100サイズ

	for(Sint32 i=0;i<pGame->pHitManager->GetOldMax();i++)
	{

		if( pGame->pHitManager->GetOldID(i) == ID_ENEMY_DEF)
		{
			x1 = pGame->pHitManager->GetOldX(i);
			y1 = pGame->pHitManager->GetOldY(i);

			kyori = viiMath::Dist( x1-x , y1-y );

			if(kyori < min_dist)
			{
				if(!viiSub::IsScrollOut(x1*100,y1*100,0,0))
				{
					//画面範囲外のヤツは相手にしない
					min_dist = kyori;
					min = i;
				}
			}
		}
	}

	if(min==-1) return gxFalse;	//見つからなかった

	px = pGame->pHitManager->GetOldX(min);
	py = pGame->pHitManager->GetOldY(min);

	return gxTrue;
}


void CEffAtkHomingMissile::Draw()
{
	//---------------------------------------------------------------
	//描画
	//---------------------------------------------------------------

	float scl = 1.0f;
	viiDraw::Sprite(&spr_custom[SPR_MISSILE]        , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_DEFAULT,ARGB_DFLT,scl,m_sRotation );
	viiDraw::Sprite(&spr_custom[SPR_MISSILE_BURNIA] , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_DEFAULT,ARGB_DFLT,scl,m_sRotation );

}


