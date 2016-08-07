//--------------------------------------------------------------------------------
//
// Stage00:グラウンドミサイル
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

	enKuraiLeft  = -8,
	enKuraiTop   = -8,
	enKuraiRight =  8,
	enKuraiBottom=  8,

	enScore = 300,
	enSpeedMax = 420,
	enHomingSpeed = 32,
	enRotationAdd = 16,
};

gxSprite SprCAtkH0002GMissile[]=
{
	{enTexPageEffect1,176,160,16,16,4,8},	//1:ミサイル
	{enTexPageEffect1,144,176,16,16,8,8},	//1:ミサイル煙
	{enTexPageEffect1,112,160,32,16,8,12},	//2:ミサイル　バーニア
	{enTexPageEffect1,120,160,24,16,24,16},	//12:雪煙

//	{TPG_LEYNOS01,0,192,64,64,32,64},//ボルゾイ
};

extern gxSprite SprCAtkH0001Missile[];

/*
gxSprite spr_custom[]={
	//カスタマイズ可能なスプライトはこっちに持ってくる
	{TPG_VALKEN,16*2,160,8,8,4,4},		//0:弾丸
	{TPG_VALKEN,16*2+8*1,160,8,8,0,4},	//1:ミサイル
	{TPG_VALKEN,16*2+8*2,160,8,8,7,4},	//2:ミサイル　バーニア
	{TPG_VALKEN,16*2+8*3,160,8,8,4,4},	//3:ミサイル　スモーク
	{TPG_VALKEN,0,128,32,32,16,16},		//4:パンチエネルギー球
	{TPG_VALKEN,32*3,32*3,32,32,16,8},	//5:バーナー
	{TPG_VALKEN,64,96,16,16,8,8},		//6:Ｄ－Ｄ
	{TPG_VALKEN,80,96,16,16,8,8},		//7:Ｄ－Ｄ1
	{TPG_VALKEN,64,112,16,16,8,8},		//8:Ｄ－Ｄ2
	{TPG_VALKEN,80,112,16,16,8,8},		//9:Ｄ－Ｄ3
	{TPG_VALKEN,32,128,32,16,8,8},		//10:OPTION_BIT
	{TPG_VALKEN,32,168,16,8,8,4},		//11:SHORTLASER
};
*/
extern gxSprite spr_cloud[];

CAtkH0002GMissile::CAtkH0002GMissile( Sint32 x, Sint32 y , Sint32 sRot , Sint32 sSpeed)
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_ATACK;

	m_HitKurai.set_hp( HP_ARMORCLASS_S );
	m_HitKurai.set_ap(100);
	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;
	m_HitKurai.DisableAiming();

	m_HitAtari.set_hp( HP_ARMORCLASS_S );
	m_HitAtari.set_ap(100);
	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;


	m_Add.x = (viiMath::Cos100(sRot)*sSpeed)/100;
	m_Add.y = (viiMath::Sin100(sRot)*sSpeed)/100-160;

	if( m_Add.x < 0 ) m_sDir = -1;
	else 			  m_sDir =  1;

	m_sDirection = DIR_LEFT;
	m_sAtribute  = ATR_DFLT;

	m_sForce        = enForceID_Versis;
	m_sWait         = 16+viiSub::Rand()%16;
	m_sMiseRotation = sRot;

	m_stObj.pos.x = m_Pos.x;
	m_stObj.pos.y = m_Pos.y;

	m_sTimeLimit = 60*8;
//	new CEffBombSmoke( m_Pos.x , m_Pos.y ,-90 , 80 );
	m_bEndless = gxFalse;
	m_sEndlessCnt = 0;

	m_sFixRotation = 0;
	m_bRotFix = gxFalse;

}


CAtkH0002GMissile::~CAtkH0002GMissile()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CAtkH0002GMissile::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------
	if( m_sTimeLimit ) m_sTimeLimit --;

	if( m_HitKurai.is_dead() || m_HitKurai.IsImpacted())
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( m_HitAtari.IsImpacted() || m_sTimeLimit == 0 )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( IsScrollOut( WINDOW_W*100 , enScrollOutHeight*2 ) )
	{
		if( m_bEndless )
		{
			if( m_sEndlessCnt > 0 )
			{
				m_sEndlessCnt --;
				if( m_sEndlessCnt == 0 )
				{
					m_bEndless = gxFalse;
				}
			}
		}
		else
		{
			SetActionSeq( enActionSeqEnd );
		}
		return;
	}

	LogicAI();

//	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	//くらい判定登録
	switch( m_sForce ){
	case enForceID_Versis:
		m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
		m_HitKurai.SetHantei(ID_VERSIS_ATK , &m_Pos );
		break;
	case enForceID_Enemy:
//		m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );
		m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
		m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );
		break;
	}

}


void CAtkH0002GMissile::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	Sint32 sAddY = 0;
	gxBool bOnWater = gxFalse;

	m_Add.y += 8;

	m_stObj.mov.x = m_Add.x;
	m_stObj.mov.y = m_Add.y+120;

	if(pGame->GetMarineLine() && m_Pos.y >= pGame->GetMarineLine() )
	{
		bOnWater = gxTrue;
	}

	if(m_stObj.easy_collision_check() )
	{
		m_Add.x += m_sDir*12;
		m_Add.y = 0;

		sAddY = viiMath::Cos100( (m_sTimer*16)%360 )*2;

		if( m_sDir < 0 )
		{
			if( m_Add.x <= -enSpeedMax ) m_Add.x = -enSpeedMax ;
		}
		else
		{
			if( m_Add.x >= enSpeedMax ) m_Add.x = enSpeedMax ;
		}

		CAtkH0002GMslWave *q = new CAtkH0002GMslWave( m_Pos.x , m_Pos.y );
		q->SetDirection( m_sMiseRotation+180 , 70 );

		CAtkH0001MslSmoke *p = new CAtkH0001MslSmoke( m_Pos.x , m_Pos.y );
		p->SetDirection( m_sTimer%360 , 30 );
		p->SetClearSpeed(4);
	}
	else
	{
		if( bOnWater )
		{

		}
		else
		{
			if( m_sTimer%2==0)
			{
				CAtkH0001MslSmoke *p = new CAtkH0001MslSmoke( m_Pos.x , m_Pos.y );
				p->SetDirection( m_sTimer%360 , 80 );
			}
		}
	}

	m_stObj.pos.x += m_stObj.mov.x;
	m_stObj.pos.y += m_stObj.mov.y;

	Sint32 r = viiMath::Atan2_100(m_stObj.mov.y , m_stObj.mov.x);
	Sint32 sAdd = (r/100 - m_sMiseRotation)/5;
	if( sAdd == 0)
	{
		if( m_sMiseRotation < r ) m_sMiseRotation += 1;
		if( m_sMiseRotation > r ) m_sMiseRotation -= 1;
	}
	else
	{
		m_sMiseRotation += sAdd;
	}
	m_Pos.x = m_stObj.pos.x;
	m_Pos.y = m_stObj.pos.y+sAddY;

	if( bOnWater )
	{
		m_Pos.y = pGame->GetMarineLine();
		if( m_sTimer%4==0)
		{
			new CEffWater( m_Pos.x , m_Pos.y, enTypeSplashPillar , 0 , 0);
		}
	}

	if( m_bRotFix )
	{
		m_sMiseRotation = m_sFixRotation;
	}

}


void CAtkH0002GMissile::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	viiMus::PlaySound(enSoundExplosionMiddle);
	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

	SetActionSeq( enActionSeqEnd );

}

void CAtkH0002GMissile::Draw()
{

	Sint32 sRot = m_sMiseRotation;

//	viiDraw::Sprite(&SprCAtkH0002GMissile[1] , m_Pos.x , m_Pos.y , PRIO_ATACK,ATR_DEFAULT,ARGB_DFLT,1.f,sRot );
	viiDraw::Sprite(&SprCAtkH0002GMissile[0] , m_Pos.x , m_Pos.y , m_Pos.z , ATR_DEFAULT,ARGB_DFLT,2.f,sRot );
	viiDraw::Sprite(&SprCAtkH0001Missile[4]  , m_Pos.x , m_Pos.y , m_Pos.z , ATR_ALPHA_PLUS,0x80FFFFFF,3.0f,(m_sTimer*8)%360 );
}


//--------------------------------------------------------------------------------
//
// Stage00:スモーク
//
//--------------------------------------------------------------------------------


CAtkH0002GMslWave::CAtkH0002GMslWave( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_fScale     = (500+viiSub::Rand()%400)/1000.f;// 0.05f;

	m_sSpeed    = 0;
	m_sRotation = 0;
	m_sAlpha    = 128+viiSub::Rand()%64;
	m_sDir      = 1;

	m_Add.x = 0;
	m_Add.y = 0;
}


CAtkH0002GMslWave::~CAtkH0002GMslWave()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CAtkH0002GMslWave::SetDirection( Sint32 sRotation , Sint32 sSpeed )
{
	m_sSpeed    = sSpeed;
	m_sRotation = sRotation;

	if( viiMath::Cos100(m_sRotation) < 0)
	{
		m_sDir = -1;
	}
	else
	{
		m_sDir = 1;
	}

}


void CAtkH0002GMslWave::SeqMain()
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


void CAtkH0002GMslWave::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	if( m_sSpeed > 0 ) m_sSpeed --;
	if( m_sAlpha > 0 ) m_sAlpha -= 8;

	m_Add.x = (viiMath::Cos100(m_sRotation)*m_sSpeed)/100;
	m_Add.y = (viiMath::Sin100(m_sRotation)*m_sSpeed)/100;

	m_fScale += 0.002f;

}


void CAtkH0002GMslWave::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CAtkH0002GMslWave::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	SetActionSeq( enActionSeqEnd );

}

void CAtkH0002GMslWave::Draw()
{

	Sint32 sAlpha = m_sAlpha;
	if( sAlpha >= 255 ) sAlpha = 255;

	Sint32 atr = ATR_DFLT;
	
	if( m_sDir > 0 )
	{
		atr = ATR_FLIP_X;	
	}
	else
	{
		atr = ATR_DFLT;	
	}

	viiDraw::Sprite( &SprCAtkH0002GMissile[2] , m_Pos.x , m_Pos.y , PRIO_ENEMY , atr|ATR_ALPHA_PLUS , ARGB(sAlpha/2,0xff,0xff,0xff) , m_fScale );
}

