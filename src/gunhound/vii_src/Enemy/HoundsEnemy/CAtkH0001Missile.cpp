//--------------------------------------------------------------------------------
//
// Stage00:ホーミングミサイル＆バズーカ
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../Effect/EffectBase.h"

enum {
	enKidouFlickerLeft,
	enKidouFlickerRight,
	enKidouKuruKuruStraight,
	enKidouKuruKuruHook,
	enKidouMax,
};


enum {
	enScrollOutWidth  = 64*100,
	enScrollOutHeight = 64*100,

	enKuraiLeft  = -6,
	enKuraiTop   = -6,
	enKuraiRight =  6,
	enKuraiBottom=  6,

	enAtackPoint = 100,
	enMissileSpeedMax = 560,
	enSpeedMax = 420,
	enHomingSpeed = 12,
	enRotationAdd = 12,
};

gxSprite SprCAtkH0001Missile[]=
{
//	{enTexPageGunHound,40,160    ,8,8,-2,4}, //マイクロミサイル
	{enTexPageGunHoundBdy,192,48    ,16,16,2,8}, //マイクロミサイル
	{enTexPageEffectCommon02,128,128    ,16,16,12,8}, //バーナー
	{enTexPageEffectCommon02,128+16,128 ,16,16,12,8}, //バーナー
	{enTexPageEffectCommon02,128-16,128 ,16,16,14,8}, //スモーク
	{enTexPageEffectCommon03,96,0,32,32,16,16},		//フラッシュ

//	{enTexPageEffectCommon02,40,160-8,8,8,4,4}, //バーニア
//	{enTexPageGunHound,40,160-8,8,8,4,4}, //バーニア
//	{enTexPageGunHound,0,160  ,16,16,8,8}, //スモーク
//	{enTexPageGunHound,56,160  ,8,8,4,4}, //スモーク
//	{TPG_LEYNOS01,0,192,64,64,32,64},//ボルゾイ
};

gxSprite SprCAtkH0001Bazooka[]=
{
	{enTexPageEffect1,176,160,16,16,4,8},	//1:ミサイル
//	{enTexPageGunHound,80,160,16,16,8,8}, //バズーカ弾
};

extern gxSprite spr_cloud[];
extern gxSprite SprSpark[];
static Sint32 g_KidouControl = 0;

CAtkH0001Missile::CAtkH0001Missile( Sint32 x, Sint32 y , Sint32 sRot , Sint32 sSpeed)
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_HitKurai.DisableAiming();
	m_HitKurai.set_hp( enHitPointDefault );
	m_HitKurai.set_ap( enAtkPointBomb );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;
	m_HitAtari.set_ap( enAtkMicroMissile );
	m_HitAtari.set_hp( 1 );

	m_Add.x    = m_Add.y    = 0;
	m_Kansei.x = m_Kansei.y = 0;

	m_sDirection = DIR_LEFT;
	m_sAtribute  = ATR_DFLT;
	m_sRotation  = sRot;//-8+viiSub::Rand()%16;
	m_sControl   = 0;
	m_sSpeed     = sSpeed;
	m_sForce     = enForceID_Versis;

	m_sWait  = 16+viiSub::Rand()%16;
	m_sLevel = 1;

	m_sKidou        = g_KidouControl%enKidouMax;//viiSub::Rand()%enKidouMax;

	m_bEndless = gxFalse;

	//-----------------------------
	g_KidouControl ++;
	//-----------------------------

	m_sStartKaku    = sRot;
	m_sMiseRotation = sRot;

	m_sSmokeWide    = 0;
	m_sMoveWait = viiSub::Rand()%4;

	new CEffBombSmoke( m_Pos.x , m_Pos.y ,-90 , 80 );

}


CAtkH0001Missile::~CAtkH0001Missile()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}

void CAtkH0001Missile::SeqInit()
{
	if ( m_sMoveWait > 0 )
	{
		m_sMoveWait --;
		return;
	}
	SetActionSeq( enActionSeqMain );
}


void CAtkH0001Missile::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( pGame->GetMarineLine() && m_Pos.y >= pGame->GetMarineLine() )
	{
		CEffWaterPillar *p;
		viiMus::PlaySound(enSoundSplashM);
		p = new CEffWaterPillar( m_Pos.x, m_Pos.y , PRIO_EFFECT);
		p->SetScale( 1.7f );
		new CEffWater( m_Pos.x , m_Pos.y, enTypeSplashPillar , 0);
		SetActionSeq( enActionSeqEnd );
		return;
	}
	if( viiSub::GetBoundBlock_Player(m_Pos.x,m_Pos.y) )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}
	else if( m_HitKurai.is_dead() || m_HitKurai.IsImpacted())
	{
		viiEff::Set( EFF_RING , m_Pos.x , m_Pos.y , argset(300) );
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 3 , 3+viiSub::Rand()%2 );
		SetActionSeq( enActionSeqCrash );
		return;
	}
	else if( m_HitAtari.is_dead() || m_HitAtari.IsImpacted())
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( !m_bEndless && IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	LogicAI();

//	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	//くらい判定登録
	switch( m_sForce ){
	case enForceID_Versis:
		m_HitKurai.SetAtackType( ATK_TYPE_CANNON );
		m_HitKurai.SetHantei(ID_VERSIS_ATK , &m_Pos );
		break;
	case enForceID_Enemy:
		m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
		m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );
		m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );
		break;
	}

}


void CAtkH0001Missile::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	Sint32 px,py,sRot = m_sRotation;

	if( m_sWait > 0 )
	{
		m_sWait --;
	}
	else
	{
		if( m_sLevel == 0 )
		{
			sRot = m_sRotation;
		}
		else if( getEnemy( px , py ) )
		{
			px = px*100;
			py = py*100;
			sRot = viiMath::Atan2d( py - m_Pos.y  , px-m_Pos.x );
		}
	}

	Sint32 sTargetAngle = viiMath::GetTargetAngle( m_sRotation , sRot );

	m_sRotation += sTargetAngle;

	Sint32 sHomingSpeed = (m_sTimer/5)+enHomingSpeed+m_sLevel*2;
	m_Add.x = viiMath::Cos100(m_sRotation)*(sHomingSpeed);
	m_Add.y = viiMath::Sin100(m_sRotation)*(sHomingSpeed);

	m_Kansei.x += m_Add.x;
	m_Kansei.y += m_Add.y;

	Sint32 sDist = viiMath::Dist(m_Kansei.y , m_Kansei.x);

	if( sDist >= enMissileSpeedMax*100 )
	{
		Sint32 sKaku = viiMath::Atan2d(m_Kansei.y , m_Kansei.x);
		m_Kansei.x = viiMath::Cos100(sKaku)*enMissileSpeedMax;//(sHomingSpeed*m_sLevel);
		m_Kansei.y = viiMath::Sin100(sKaku)*enMissileSpeedMax;//(sHomingSpeed*m_sLevel);
	}

	m_Pos.x += m_Kansei.x/100;
	m_Pos.y += m_Kansei.y/100;


	switch( m_sKidou ){
	case enKidouFlickerLeft:
		m_sMiseRotation = m_sRotation+viiMath::Cos100( (m_sTimer*2+m_sStartKaku)%360 )*8;
		break;
	case enKidouFlickerRight:
		m_sMiseRotation = m_sRotation+viiMath::Sin100( (m_sTimer*2+m_sStartKaku)%360 )*8;
		break;

	case enKidouKuruKuruStraight:
		m_sMiseRotation = m_sRotation+(m_sTimer*2+m_sStartKaku)%360;
		break;

	case enKidouKuruKuruHook:
		m_sMiseRotation = m_sRotation+(m_sTimer*16+m_sStartKaku)%360;
		break;
	}

	m_sSmokeWide = m_sTimer;
	if( m_sSmokeWide >= 90 ) m_sSmokeWide = 90;
	CAtkH0001MslSmoke *p = new CAtkH0001MslSmoke( m_Pos.x , m_Pos.y );
	p->SetDirection( m_sMiseRotation+180 , m_sSmokeWide );


}


gxBool CAtkH0001Missile::getEnemy(Sint32 &px,Sint32 &py)
{
	//-------------------------------------
	//敵を取得する
	//（Forceにより取得相手が変わるので注意）
	//-------------------------------------

	switch( m_sForce ){
	case enForceID_Versis:
		return getTargetEnemyPosition( m_Pos.x/100 , m_Pos.y/100 , px , py);

	case enForceID_Enemy:
		px = GetTargetPlayer()->x/100;
		py = GetTargetPlayer()->y/100-24;
		return gxTrue;
	}

	return gxFalse;

}



gxBool CAtkH0001Missile::getTargetEnemyPosition(Sint32 x,Sint32 y,Sint32 &px,Sint32 &py)
{
	//-------------------------------------
	//ロックオンロジック
	//最も近い敵の座標を得る
	//該当の敵がいない場合にはgxFalseを返す
	//-------------------------------------
	double kyori,min_dist = 96*2;//SCR_W*SCR_H;
	Sint32 min=-1;
	Sint32 x1,y1;

	//座標は1/100サイズ

#if 0
	for(Sint32 i=0;i<pGame->pHitManager->GetOldMax();i++)
	{
		if( pGame->pHitManager->GetOldID(i) == ID_ENEMY_DEF)
		{
			x1 = pGame->pHitManager->GetOldX(i);
			y1 = pGame->pHitManager->GetOldY(i);

//			kyori = viiMath::Dist(POW(x1-x)+POW(y1-y));
			kyori = viiMath::Dist(x1,y1 , x,y );
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
#endif

	for(Sint32 i=0;i<pGame->pHitManager->GetOldMax();i++)
	{
		if( pGame->pHitManager->GetOldID(i) == ID_ENEMY_DEF)
		{
			x1 = pGame->pHitManager->GetOldX(i);
			y1 = pGame->pHitManager->GetOldY(i);

			if( viiSub::IsScrollOut(x1*100,y1*100,0,0) ) continue;

			kyori = viiMath::Dist(x1,y1 , x,y );

			if(kyori < min_dist)
			{
				min_dist = kyori;
				min = i;
			}
		}
	}

	if(min==-1) return gxFalse;	//見つからなかった

	px = pGame->pHitManager->GetOldX(min);
	py = pGame->pHitManager->GetOldY(min);

	return gxTrue;
}


void CAtkH0001Missile::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CAtkH0001Missile::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	viiMus::PlaySound(enSoundExplosionSmall);

	if( m_HitAtari.IsImpacted() )
	{
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 3 , 1+viiSub::Rand()%5 );
	}
	else
	{
		viiEff::SetBombSimple( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
	}

	SetActionSeq( enActionSeqEnd );

}

void CAtkH0001Missile::Draw()
{

	Sint32 sRot = m_sRotation;

	viiDraw::Sprite(&SprCAtkH0001Missile[0] , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_DEFAULT,ARGB_DFLT,1.f,sRot );
	viiDraw::Sprite(&SprCAtkH0001Missile[4] , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_ALPHA_PLUS,0x80FFFFFF,1.5f,(m_sTimer*8)%360 );
	if( (m_sKidou+m_sTimer)%2 )
	{
		viiDraw::Sprite(&SprCAtkH0001Missile[1] , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_ALPHA_PLUS,0x80ffffff,1.f,m_sRotation );
	}
	else
	{
		viiDraw::Sprite(&SprCAtkH0001Missile[2] , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_ALPHA_PLUS,0x80ffffff,1.f,m_sRotation );
	}
}


//--------------------------------------------------------------------------------
//
// Stage00:スモーク
//
//--------------------------------------------------------------------------------


CAtkH0001MslSmoke::CAtkH0001MslSmoke( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	Sint32 rnd = viiSub::Rand();
	m_fScale     = (30+rnd%50)/100.f;// 1.f;(300+%200)/10000.f;// 0.05f;

	m_sSpeed    = 0;
	m_sRotation = rnd%360;
	m_sAlpha    = 112+rnd%32;

	m_sMiseRotation = rnd%32;
	m_Add.x = 0;
	m_Add.y = 0;

	m_sAlphaSpeed = 1;
	m_sRotationAdd = 320+rnd%80;


}


CAtkH0001MslSmoke::~CAtkH0001MslSmoke()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CAtkH0001MslSmoke::SetDirection( Sint32 sRotation , Sint32 sSpeed )
{
	m_sSpeed    = sSpeed;
	m_sRotation = sRotation;
}


void CAtkH0001MslSmoke::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	LogicAI();

	if( m_sAlpha <= 0 )
	{
		m_sAlpha = 0;
		SetActionSeq( enActionSeqCrash );
	}

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

}


void CAtkH0001MslSmoke::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	if( m_sSpeed > 0 ) m_sSpeed --;
	if( m_sAlpha > 0 ) m_sAlpha -= m_sAlphaSpeed;
	if( m_sRotationAdd > 0 )
	{
		m_sRotationAdd -= 2;
		if( m_sRotationAdd <= 0 )
		{
			m_sRotationAdd = 0;
		}
	}

	m_Add.x = (viiMath::Cos100(m_sRotation)*m_sSpeed)/100;
	m_Add.y = (viiMath::Sin100(m_sRotation)*m_sSpeed)/100;

	m_fScale += 0.005f;
	m_sMiseRotation += m_sRotationAdd/100;
}


void CAtkH0001MslSmoke::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CAtkH0001MslSmoke::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	SetActionSeq( enActionSeqEnd );

}

void CAtkH0001MslSmoke::Draw()
{

	Sint32 sAlpha = m_sAlpha*2;
	if( sAlpha >= 255 ) sAlpha = 255;
	viiDraw::Sprite( &SprCAtkH0001Missile[ 3 ] , m_Pos.x , m_Pos.y , PRIO_EFFECT , ATR_DFLT , ARGB( (sAlpha/2) ,0xff,0xff,0xff) , m_fScale , m_sMiseRotation);


}

//--------------------------------------------------------------------------------
//
// Stage00:バズーカ弾丸
//
//--------------------------------------------------------------------------------

CAtkH0001Bazooka::CAtkH0001Bazooka( Sint32 x, Sint32 y , Sint32 sRot , Sint32 sSpeed)
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_HitAtari.ax1 = -12;
	m_HitAtari.ay1 = -12;
	m_HitAtari.ax2 = 12;
	m_HitAtari.ay2 = 12;
	m_HitAtari.set_ap( enAtkPointBazooka );
	m_HitAtari.set_hp( HP_ARMORCLASS_B );

	m_Add.x    = m_Add.y    = 0;

	m_sDirection = DIR_LEFT;
	m_sAtribute  = ATR_DFLT;
	m_sRotation  = sRot;
	m_sSpeed     = sSpeed;
	m_sForce     = enForceID_Versis;
	m_sSmokeCnt     = viiSub::Rand()%360;
	m_bSmoke = gxTrue;

	new CEffBombSmoke( m_Pos.x , m_Pos.y ,-135 , 120 );
	new CEffBombSmoke( m_Pos.x , m_Pos.y , 135 , 120 );
	new CEffBombSmoke( m_Pos.x , m_Pos.y ,-180 , 120 );
}


CAtkH0001Bazooka::~CAtkH0001Bazooka()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CAtkH0001Bazooka::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( pGame->GetMarineLine() && m_Pos.y >= pGame->GetMarineLine() )
	{
		CEffWaterPillar *p;
		viiMus::PlaySound(enSoundSplashM);
		p = new CEffWaterPillar( m_Pos.x, m_Pos.y , PRIO_EFFECT);
		p->SetScale( 1.7f );
		new CEffWater( m_Pos.x , m_Pos.y, enTypeSplashPillar , 0);
		SetActionSeq( enActionSeqEnd );
		return;
	}
	if( viiSub::GetBoundBlock_Player(m_Pos.x,m_Pos.y) )
	{
		EffectMakeBigBomb( m_Pos.x , m_Pos.y );
		SetActionSeq( enActionSeqCrash );
		return;
	}
	else if( m_HitAtari.is_dead() || m_HitAtari.IsImpacted())
	{
		viiMus::PlaySound(enSoundExplosionMiddle);
		pGame->QuakeOn(32);
		EffectMakeBigBomb( m_Pos.x , m_Pos.y );
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	LogicAI();

	DirectionAdjust();

	//くらい判定登録
	switch( m_sForce ){
	case enForceID_Versis:
		m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
		m_HitAtari.SetHantei(ID_VERSIS_ATK , &m_Pos );
		break;
	case enForceID_Enemy:
		m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
		m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );
		break;
	}

}


void CAtkH0001Bazooka::LogicAI()
{
	Sint32 x,y,sx,sy;

	x = m_Pos.x;
	y = m_Pos.y;

	m_Add.x = (viiMath::Cos100(m_sRotation)*m_sSpeed)/100;
	m_Add.y = (viiMath::Sin100(m_sRotation)*m_sSpeed)/100;

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	sx = m_Pos.x - x;
	sy = m_Pos.y - y;

	sx = sx / 4;
	sy = sy / 4;

	for(Sint32 ii=0;ii<4;ii++)
	{
		CAtkH0001MslSmoke *p = new CAtkH0001MslSmoke( m_Pos.x+sx*ii , m_Pos.y+sy*ii );
		p->SetDirection( (viiMath::Cos100(m_sSmokeCnt%360)*360)/100 , 30 );
		m_sSmokeCnt += 2;
	}

}


void CAtkH0001Bazooka::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	if( m_HitAtari.IsImpacted() )
	{
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
	}
	else
	{
		viiEff::SetBombSimple( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
	}

	SetActionSeq( enActionSeqEnd );

}

void CAtkH0001Bazooka::Draw()
{

	Sint32 sRot = m_sRotation;

	viiDraw::Sprite(&SprCAtkH0001Bazooka[0] , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_DEFAULT,ARGB_DFLT,1.f,sRot );
	viiDraw::Sprite(&SprCAtkH0001Missile[1] , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_ALPHA_PLUS,0x80ffffff,1.f,m_sRotation );
	viiDraw::Sprite(&SprCAtkH0001Missile[2] , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_ALPHA_PLUS,0x80ffffff,1.f,m_sRotation );
	viiDraw::Sprite(&SprCAtkH0001Missile[4] , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_ALPHA_PLUS,0x80FFFFFF,1.5f,(m_sTimer*8)%360 );
	viiDraw::Sprite(&SprCAtkH0001Missile[4] , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_ALPHA_PLUS,0x80FFFFFF,2.0f,-((m_sTimer*8)%360) );
}



//--------------------------------------------------------------------------------
//
// Stage00:スプラインミサイル
//
//--------------------------------------------------------------------------------

CAtkH001SplineMissile::CAtkH001SplineMissile( Sint32 x, Sint32 y)
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_HitKurai.DisableAiming();
	m_HitKurai.set_hp( HP_ARMORCLASS_B );
	m_HitKurai.set_ap( enAtkPointMissile );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;
	m_HitAtari.set_ap( enAtkPointMissile );
	m_HitAtari.set_hp( HP_ARMORCLASS_B );


	m_sDirection = DIR_LEFT;
	m_sAtribute  = ATR_DFLT;
	m_sRotation  = 0;

	m_Add.x = 0;
	m_Add.y = 0;

	m_pTbl     = NULL;
	m_sSize    = 0;
	m_sCurrent = 0;

	m_sWait   = 0;
	m_sForce  = enForceID_Enemy;

}


CAtkH001SplineMissile::~CAtkH001SplineMissile()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CAtkH001SplineMissile::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( viiSub::GetBoundBlock_Player(m_Pos.x,m_Pos.y) )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}
	else if( m_HitKurai.is_dead() || m_HitKurai.IsImpacted())
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}
	else if( m_HitAtari.is_dead() || m_HitAtari.IsImpacted())
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	LogicAI();

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

//	SetRader( enTypeMarkerGroundObject );
//	DirectionAdjust();

	//くらい判定登録
	switch( m_sForce ){
	case enForceID_Versis:
		m_HitKurai.SetAtackType( ATK_TYPE_CANNON );
		m_HitKurai.SetHantei(ID_VERSIS_ATK , &m_Pos );
		break;
	case enForceID_Enemy:
		m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
		m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );
		m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );
		break;
	}

}


void CAtkH001SplineMissile::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	CAtkH0001MslSmoke *p = new CAtkH0001MslSmoke( m_Pos.x , m_Pos.y );
	p->SetDirection( 0 , 30 );

	if( m_sCurrent >= m_sSize )
	{
		if( m_sWait > 0 )
		{
			m_sWait --;
			return;
		}
		SetActionSeq( enActionSeqCrash );
		return;
	}

	m_Add.x = m_pTbl[m_sCurrent+0]*50;
	m_Add.y = m_pTbl[m_sCurrent+1]*50;

	m_sCurrent +=2;


/*
	if( m_sSmokeWide >= 90 ) m_sSmokeWide = 90;
*/


}


void CAtkH001SplineMissile::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	viiMus::PlaySound(enSoundExplosionMiddle);

	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

	SetActionSeq( enActionSeqEnd );

}

void CAtkH001SplineMissile::Draw()
{

	Sint32 sRot = m_sRotation;

//	viiDraw::Sprite(&SprCAtkH001SplineMissile[0] , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_DEFAULT,ARGB_DFLT,1.f,sRot );
//	viiDraw::Sprite(&SprCAtkH001SplineMissile[1] , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_ALPHA_PLUS,0x80ffffff,1.f,m_sRotation );
}



//--------------------------------------------------------------------------------
//
// Stage00:榴弾
//
//--------------------------------------------------------------------------------

CAtkH0001Ryudan::CAtkH0001Ryudan( Sint32 x, Sint32 y , Sint32 sRot , Sint32 sSpeed)
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_HitKurai.DisableAiming();
	m_HitKurai.set_hp( HP_ARMORCLASS_B );
	m_HitKurai.set_ap( enAtkPointMissile );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;
	m_HitAtari.set_ap( enAtkPointMissile );
	m_HitAtari.set_hp( HP_ARMORCLASS_B );

	m_Add.x    = m_Add.y    = 0;
	m_Kansei.x = m_Kansei.y = 0;

	m_sDirection = DIR_LEFT;
	m_sAtribute  = ATR_DFLT;
	m_sRotation  = sRot-8+viiSub::Rand()%16;
	m_sControl   = 0;
	m_sSpeed     = enSpeedMax;//sSpeed;
	m_sForce     = enForceID_Versis;

	m_sWait  = 16+viiSub::Rand()%16;
	m_sLevel = 0;

	m_sKidou        = g_KidouControl%enKidouMax;//viiSub::Rand()%enKidouMax;

	m_bEndless = gxFalse;

	m_sStartKaku    = sRot;
	m_sMiseRotation = sRot;

	m_sSmokeWide    = 0;
	m_sMoveWait = 0;//viiSub::Rand()%4;

	new CEffBombSmoke( m_Pos.x , m_Pos.y ,-90 , 80 );

	SetActionSeq( enActionSeqMain );
}


CAtkH0001Ryudan::~CAtkH0001Ryudan()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}

void CAtkH0001Ryudan::SeqInit()
{
	if ( m_sMoveWait > 0 )
	{
		m_sMoveWait --;
		return;
	}
	SetActionSeq( enActionSeqMain );
}


void CAtkH0001Ryudan::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( pGame->GetMarineLine() && m_Pos.y >= pGame->GetMarineLine() )
	{
		CEffWaterPillar *p;
		viiMus::PlaySound(enSoundSplashM);
		p = new CEffWaterPillar( m_Pos.x, m_Pos.y , PRIO_EFFECT);
		p->SetScale( 1.7f );
		new CEffWater( m_Pos.x , m_Pos.y, enTypeSplashPillar , 0);
		SetActionSeq( enActionSeqEnd );
		return;
	}
	if( viiSub::GetBoundBlock_Player(m_Pos.x,m_Pos.y) )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}
	else if( m_HitKurai.is_dead() || m_HitKurai.IsImpacted())
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}
	else if( m_HitAtari.is_dead() || m_HitAtari.IsImpacted())
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( !m_bEndless && IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	LogicAI();

//	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	//くらい判定登録
	switch( m_sForce ){
	case enForceID_Versis:
		m_HitKurai.SetAtackType( ATK_TYPE_CANNON );
		m_HitKurai.SetHantei(ID_VERSIS_ATK , &m_Pos );
		break;
	case enForceID_Enemy:
		m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
		m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );
		m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );
		break;
	}

}


void CAtkH0001Ryudan::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	Sint32 sRot = m_sRotation;

	if( m_sWait > 0 )
	{
		m_sWait --;
	}
	else
	{
		m_sRotation = (360+m_sRotation)%360;

		Sint32 sTargetAngle = viiMath::GetTargetAngle( m_sRotation , 90 );
		if( sTargetAngle > 3 ) m_sRotation += 2;
		if( sTargetAngle < -3 ) m_sRotation -= 2;
	}

	m_Add.x = viiMath::Cos100(m_sRotation)*(enHomingSpeed);
	m_Add.y = viiMath::Sin100(m_sRotation)*(enHomingSpeed);
	m_Kansei.x += m_Add.x;
	m_Kansei.y += m_Add.y;

	Float32 sKaku = viiMath::Atan2d(m_Kansei.y , m_Kansei.x);
	Sint32  sDist = viiMath::Dist(m_Kansei.y , m_Kansei.x);

	if( sDist >= (m_sLevel*10+m_sSpeed)*100 )
	{
		m_Kansei.x = viiMath::Cos100(sKaku)*(m_sLevel*10+m_sSpeed);
		m_Kansei.y = viiMath::Sin100(sKaku)*(m_sLevel*10+m_sSpeed);
	}

	m_Pos.x += m_Kansei.x/100;
	m_Pos.y += m_Kansei.y/100+100;


	switch( m_sKidou ){
	case enKidouFlickerLeft:
		m_sMiseRotation = m_sRotation+viiMath::Cos100( (m_sTimer*2+m_sStartKaku)%360 )*8;
		break;
	case enKidouFlickerRight:
		m_sMiseRotation = m_sRotation+viiMath::Sin100( (m_sTimer*2+m_sStartKaku)%360 )*8;
		break;

	case enKidouKuruKuruStraight:
		m_sMiseRotation = m_sRotation+(m_sTimer*2+m_sStartKaku)%360;
		break;

	case enKidouKuruKuruHook:
		m_sMiseRotation = m_sRotation+(m_sTimer*16+m_sStartKaku)%360;
		break;
	}

	m_sSmokeWide = m_sTimer;
	if( m_sSmokeWide >= 90 ) m_sSmokeWide = 90;
	CAtkH0001MslSmoke *p = new CAtkH0001MslSmoke( m_Pos.x , m_Pos.y );
	p->SetDirection( m_sMiseRotation+180 , m_sSmokeWide );
//	p->SetClearSpeed(8);


}


void CAtkH0001Ryudan::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	viiMus::PlaySound(enSoundExplosionMiddle);

	if( m_HitAtari.IsImpacted() )
	{
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
	}
	else
	{
		viiEff::SetBombSimple( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
	}

	SetActionSeq( enActionSeqEnd );

}

void CAtkH0001Ryudan::Draw()
{

	Sint32 sRot = m_sRotation;

	switch( m_sForce ){
	case enForceID_Versis:
		break;
	case enForceID_Enemy:
		return;
	}

	viiDraw::Sprite(&SprCAtkH0001Missile[0] , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_DEFAULT,ARGB_DFLT,1.f,sRot );
	viiDraw::Sprite(&SprCAtkH0001Missile[4] , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_ALPHA_PLUS,0x80FFFFFF,1.5f,(m_sTimer*8)%360 );
	if( (m_sKidou+m_sTimer)%2 )
	{
		viiDraw::Sprite(&SprCAtkH0001Missile[1] , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_ALPHA_PLUS,0x80ffffff,1.f,m_sRotation );
	}
	else
	{
		viiDraw::Sprite(&SprCAtkH0001Missile[2] , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_ALPHA_PLUS,0x80ffffff,1.f,m_sRotation );
	}
}


CAtkH0001Thunder::CAtkH0001Thunder(Sint32 x1, Sint32 y1 ,Sint32 x2,Sint32 y2)
{

	m_sTimer = 0;
	m_sAlpha = 0;

	m_sDist = 1;
	m_sLoop   = 1;
	m_fRot    = ZERO;
	m_fTgtRot = ZERO;
	m_fRotAdd = ZERO;
	m_fScale  = 1.f;
	m_sCos = viiSub::Rand()%360;
	Set(x1,y1,x2,y2);
	m_bSet = gxTrue;
	m_sSyousyaCnt = 0;

}


CAtkH0001Thunder::~CAtkH0001Thunder()
{
	
	
	
}


void CAtkH0001Thunder::Set(Sint32 x1, Sint32 y1 ,Sint32 x2,Sint32 y2)
{
	//-------------------------------------------------
	//２点間から距離計算
	//-------------------------------------------------
	m_bSet = gxTrue;

	m_Pos.x = x1;
	m_Pos.y = y1;

	m_Tgt.x = x2;
	m_Tgt.y = y2;

	x1 = x1/100;
	y1 = y1/100;
	x2 = x2/100;
	y2 = y2/100;

	m_fTgtRot = viiMath::Atan2_100(y2-y1,x2-x1)/100;
	m_fRot    = m_fTgtRot-20+viiSub::Rand()%40;

	m_sDist = viiMath::Dist( (y2-y1) , (x2-x1) );

	if(m_sDist>=80)
	{
		m_sLoop = m_sDist/80+1;
		m_fScale = (1.0f*m_sDist/m_sLoop)/80.f;
		m_fRotAdd = (m_fTgtRot - m_fRot)/m_sLoop;
	}
	else
	{
		m_sLoop = 1;
		m_fScale = (1.0f*m_sDist)/80.f;
		m_fRotAdd = 0;
	}

	m_fScale = 0.6f;

}

void CAtkH0001Thunder::Action()
{
/*
	if( !m_bSet )
	{
		//リクエストがなければ自動的に消滅
		SetActionSeq( enActionSeqEnd );
	}
*/
	m_bSet = gxFalse;
}


void CAtkH0001Thunder::Draw()
{
	Sint32 x,y,x2,y2;
	Float32 r,tr=0.f;

	x = m_Pos.x;
	y = m_Pos.y;
	r = m_fRot;

	x2 = m_Tgt.x;
	y2 = m_Tgt.y;

	Sint32 rnd= 0;
	gxBool m_bSyousya=gxFalse;
	for(Sint32 ii=0;ii<32;ii++)
	{
		rnd = viiSub::Rand()%3;
		viiDraw::Sprite( &SprSpark[ rnd ] , x , y , PRIO_ENEMY-1 , ATR_ALPHA_PLUS , 0xFFFFFFFF , m_fScale ,(Sint32)r );
		x += viiMath::Cos100(r)*(80*m_fScale);
		y += viiMath::Sin100(r)*(80*m_fScale);

		m_fTgtRot = viiMath::Atan2_100(y2-y,x2-x)/100;
		Sint32 b = viiMath::GetTargetAngle( r ,m_fTgtRot );
		if( b<-20) b= -20;
		if( b> 20) b= 20;
		r += b;
		if( viiMath::Dist( (y2-y) , (x2-x) ) <= 24*100 )
		{
			m_bSyousya = gxTrue;
			if(m_sSyousyaCnt >= 1000)
			{
				CAtkH0001Ryudan *p;
				p = new CAtkH0001Ryudan( x2 , y2 , 45, 48 );
				p->SetForce(CAtkH0001Ryudan::enForceID_Enemy);
				m_sSyousyaCnt = 0;
			}
			break;
		}
		tr += 0.05f;
	}
	if(m_bSyousya)
	{
		m_sSyousyaCnt++;
		if( m_sSyousyaCnt >= 120 )
		{
			m_sSyousyaCnt = 1000;
		}
	}
	else
	{
		m_sSyousyaCnt = 0;
	}
}


//--------------------------------------------------------------------------------
//
// Stage00:グレネード
//
//--------------------------------------------------------------------------------

CAtkH0001Grenade::CAtkH0001Grenade( Sint32 x, Sint32 y , Sint32 sRot , Sint32 sSpeed)
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_HitKurai.DisableAiming();
	m_HitKurai.set_hp( HP_ARMORCLASS_B );
	m_HitKurai.set_ap( enAtkPointMissile );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;
	m_HitAtari.set_ap( enAtkPointBomb );
	m_HitAtari.set_hp( HP_ARMORCLASS_B );

	m_Add.x    = (viiMath::Cos100(sRot)*sSpeed)/100;
	m_Add.y    = (viiMath::Sin100(sRot)*sSpeed)/100;
	m_Kansei.x = m_Kansei.y = 0;

	m_sRotation  = viiSub::Rand()%360;
	m_sSpeed     = enSpeedMax;//sSpeed;
	m_sForce     = enForceID_Versis;

	m_bEndless = gxFalse;

	m_sBound = 0;
	m_sCrashSeq = 0;

	m_bDisp = gxTrue;

	SetActionSeq( enActionSeqMain );
}


CAtkH0001Grenade::~CAtkH0001Grenade()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}

void CAtkH0001Grenade::SeqInit()
{

	SetActionSeq( enActionSeqMain );

}


void CAtkH0001Grenade::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( pGame->GetMarineLine() && m_Pos.y >= pGame->GetMarineLine() )
	{
		CEffWaterPillar *p;
		viiMus::PlaySound(enSoundSplashM);
		p = new CEffWaterPillar( m_Pos.x, m_Pos.y , PRIO_EFFECT);
		p->SetScale( 1.7f );
		new CEffWater( m_Pos.x , m_Pos.y, enTypeSplashPillar , 0);
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( m_sBound == 3 )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}
	else if( m_HitKurai.is_dead() || m_HitKurai.IsImpacted())
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}
	else if( m_HitAtari.is_dead() || m_HitAtari.IsImpacted())
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( !m_bEndless && IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	LogicAI();

	m_HitKurai.SetAtackType( ATK_TYPE_CANNON );
	m_HitKurai.SetHantei(ID_VERSIS_ATK , &m_Pos );

}


void CAtkH0001Grenade::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	gxBool bBound =  gxFalse;

	if( m_Add.y < 800 )
	{
		m_Add.y+=30;
	}
	m_Add.x += (0-m_Add.x)/200;

	Sint32 x,y;
	x = m_Add.x;
	y = m_Add.y;

	if( viiSub::GetBoundBlock_Player( m_Pos.x + x , m_Pos.y ) )
	{
		x = 0;
		m_Add.x *= -1;
		m_Add.x = m_Add.x/3;
		bBound = gxTrue;
	}

	if( viiSub::GetBoundBlock_Player( m_Pos.x , m_Pos.y + y ) )
	{
		y = 0;
		m_Add.y *= -1;
		bBound = gxTrue;
	}

	m_Pos.x += x;
	m_Pos.y += y;

	m_sRotation += x/30;

	if( bBound )
	{
		m_Add.x = m_Add.x/2;
		m_Add.y = m_Add.y/2;
		m_sBound ++;
	}

}


void CAtkH0001Grenade::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	Sint32 n = 18;

	m_bDisp = gxFalse;

	if( m_sCrashSeq%n == 0 )
	{
		Sint32 ax,ay;
		ax = m_Pos.x+(-16+viiSub::Rand()%32)*300;
		ay = m_Pos.y+(-16+viiSub::Rand()%32)*300;

		if( pGame->GetMarineLine() && m_Pos.y >= pGame->GetMarineLine() )
		{
			ay = pGame->GetMarineLine();
			MakeSplash( ax , ay );
			EffectMakeBigBomb( ax , ay );
			if( !m_bDead )
			{
				viiMus::PlaySound( enSoundSplashM );
				m_bDead = gxTrue;
			}
		}
		else
		{
			viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

			CEffBigBomb *p;

			p = new CEffBigBomb( ax , ay , 0 , 0 );
			p->m_Pos.z = PRIO_BG2;

			if( !m_bDead )
			{
				EffectMakeBigBomb( ax , ay );
				viiMus::PlaySound(enSoundExplosionMiddle);
				m_bDead = gxTrue;
			}
		}
	}

	m_sCrashSeq ++;

/*
	if( m_HitAtari.IsImpacted() )
	{
	}
	else
	{
		viiEff::SetBombSimple( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
	}
*/

	if( m_sCrashSeq >= n*4 )
	{
		SetActionSeq( enActionSeqEnd );
	}

}

void CAtkH0001Grenade::Draw()
{

	if( !m_bDisp ) return;

	viiDraw::Sprite(&SprCAtkH0001Missile[0] , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_DEFAULT,ARGB_DFLT,1.f,m_sRotation );
}


