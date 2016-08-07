//--------------------------------------------------------------------------------
//
// Stage00:ニンジャ爆発
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
//#include "EnemyTbl/CEneH0305MissileTBL.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -20,
	enKuraiTop   = -20,
	enKuraiRight =  20,
	enKuraiBottom=  20,

	enScore = 300,
};


gxSprite SprCEneH0305Missile[]=
{
//	{enTexPageEffectCommon02,0,160,64,32,32,32},	//爆発1
	{enTexPageEffectCommon02,192,176,64,80,32,64},	//爆発1
	{enTexPageEffectCommon02,64,160,32,32,16,32},	//爆発2
	{enTexPageEffectCommon02,64,160,32,32,16,32},	//爆発2
	{enTexPageEffectCommon02,4*32,0*32,32,32,16,16},//照準１
	{enTexPageEffectCommon02,4*32,1*32,32,32,16,16},//照準２
	{enTexPageEffectCommon02,224,32,32,32,16,32},	//ブラウン

	{enTexPageEnemyStage03,0*32,0*32,64,64,32,32},//爆発1
	{enTexPageEnemyStage03,2*32,0*32,64,64,32,32},//爆発2
	{enTexPageEnemyStage03,5*32,0*32,8,8,4,4},	  //岩
	{enTexPageEnemyStage03,4*32,0*32,32,32,16,16},//照準１
	{enTexPageEnemyStage03,4*32,1*32,32,32,16,16},//照準２
	{enTexPageEnemyStage03,6*32,0*32,32,32,16,16},//ブラウン
};


CEneH0305Missile::CEneH0305Missile( Sint32 x, Sint32 y ,Sint32 z)
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );

	m_Pos.x =  sx*100+x;
	m_Pos.y =  sy*100+y;
	m_Pos.z = z;//PRIO_EFFECT;

	m_Pos.x = x;
	m_Pos.y = y;

	m_HitAtari.set_hp( HP_ARMORCLASS_B );

	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;

	m_sDirection = DIR_LEFT;
	m_sAtribute = ATR_DFLT;

	m_sRotation = 180;
	m_sWait = 30;

}


CEneH0305Missile::~CEneH0305Missile()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneH0305Missile::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	LogicAI();

}


void CEneH0305Missile::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	if( m_sRotation > 0 )
	{
		m_sRotation -= 2;
		if( m_sRotation <= 0)
		{
			m_sRotation = 0;
		}

	}
	else
	{
		new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x+(-4+viiSub::Rand()%8)*100,m_Pos.y+(-4+viiSub::Rand()%8)*100,m_Pos.z , 45+185,40);
		new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x+(-4+viiSub::Rand()%8)*100,m_Pos.y+(-4+viiSub::Rand()%8)*100,m_Pos.z , 45+180,120);
		new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x+(-4+viiSub::Rand()%8)*100,m_Pos.y+(-4+viiSub::Rand()%8)*100,m_Pos.z , 45+175,80);

		SetActionSeq( enActionSeqCrash );
	}


}


void CEneH0305Missile::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CEneH0305Missile::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	//くらい判定登録
	if( m_sWait > 0)
	{
		m_sWait --;
		m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
		m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );
		return;
	}
/*
	viiMus::PlaySound(enSoundExplosionMiddle);

	ScorePlus( enScore );

	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
*/

	SetActionSeq( enActionSeqEnd );

}

void CEneH0305Missile::Draw()
{
	Sint32 x,y;
	viiSub::GetScroll_s( x,y );

	x =  x*100+m_Pos.x;
	y =  y*100+m_Pos.y;

	x =  m_Pos.x;
	y =  m_Pos.y;

	viiDraw::Sprite( &SprCEneH0305Missile[ 4 ] , x , y , PRIO_ENEMY , m_sAtribute , ARGB_DFLT ,1.0f ,-m_sRotation );
	viiDraw::Sprite( &SprCEneH0305Missile[ 4 ] , x , y , PRIO_ENEMY , m_sAtribute , ARGB_DFLT ,1.0f , m_sRotation );

}

//--------------------------------------------------------------------------------
//
// Stage03:爆発
//
//--------------------------------------------------------------------------------


CEneH0305Bomb::CEneH0305Bomb( Sint32 sType , Sint32 x, Sint32 y , Sint32 z , Sint32 sRot , Sint32 sSpeed )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = z;

	m_HitAtari.ax1 = -48;
	m_HitAtari.ay1 = -48;
	m_HitAtari.ax2 = +48;
	m_HitAtari.ay2 = +48;
	m_HitAtari.set_hp( 1 );
	m_HitAtari.set_ap( enAtkPointBazooka );

	m_sDirection = DIR_LEFT;
	m_sAtribute  = ATR_DFLT;

	m_sColor = 200;
	m_sScale = 100;

	m_sType = sType;
	m_sRotation = sRot;//viiSub::Rand()%360;
	m_sTimer = 0;

	m_sHighMax = 130;
	m_sHigh    = 0;
	m_sHighAdd = -m_sHighMax;

	switch( m_sType ){
	case enTypeBrown:
		m_sRotation = sRot;
		m_Add.x = (viiMath::Cos100(sRot)*sSpeed)/100;
		m_Add.y = (viiMath::Sin100(sRot)*sSpeed)/100;
		m_sScale = 200+viiSub::Rand()%40;
		break;

	case enTypeFire:
//		viiEff::SetBombSimple( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
//		m_sRotation = -(sRot+90);
		new CEneH0305Bomb( CEneH0305Bomb::enTypeBrown , m_Pos.x+((-4+viiSub::Rand()%8))*100+2400 , m_Pos.y+(-4+viiSub::Rand()%8)*100+2400, m_Pos.z ,m_sRotation/*45+155+viiSub::Rand()%10*/,60+viiSub::Rand()%30  );
		new CEneH0305Bomb( CEneH0305Bomb::enTypeRock  , m_Pos.x+((-4+viiSub::Rand()%8))*100+1600 , m_Pos.y+(-4+viiSub::Rand()%8)*100+3200, m_Pos.z ,m_sRotation-10+viiSub::Rand()%20,380+viiSub::Rand()%60 );
		new CEneH0305Bomb( CEneH0305Bomb::enTypeRock  , m_Pos.x+((-4+viiSub::Rand()%8))*100+1600 , m_Pos.y+(-4+viiSub::Rand()%8)*100+3200, m_Pos.z ,m_sRotation-10+viiSub::Rand()%20,380+viiSub::Rand()%60 );
		new CEneH0305Bomb( CEneH0305Bomb::enTypeRock  , m_Pos.x+((-4+viiSub::Rand()%8))*100+1600 , m_Pos.y+(-4+viiSub::Rand()%8)*100+3200, m_Pos.z ,m_sRotation-10+viiSub::Rand()%20,380+viiSub::Rand()%60 );
		new CEneH0305Bomb( CEneH0305Bomb::enTypeRock  , m_Pos.x+((-4+viiSub::Rand()%8))*100+1600 , m_Pos.y+(-4+viiSub::Rand()%8)*100+3200, m_Pos.z ,m_sRotation-10+viiSub::Rand()%20,380+viiSub::Rand()%60 );
		new CEneH0305Bomb( CEneH0305Bomb::enTypeRock  , m_Pos.x+((-4+viiSub::Rand()%8))*100+1600 , m_Pos.y+(-4+viiSub::Rand()%8)*100+3200, m_Pos.z ,m_sRotation-10+viiSub::Rand()%20,380+viiSub::Rand()%60 );
		break;

	case enTypeRock:
		m_Pos.x = x+(-8+viiSub::Rand()%16)*30;
		m_Pos.y = y+(-8+viiSub::Rand()%16)*30;
		m_Add.x = (viiMath::Cos100(sRot)*sSpeed)/100;
		m_Add.y = (viiMath::Sin100(sRot)*sSpeed)/100;
		m_sScale = 60+viiSub::Rand()%40;
		break;
	default:
		m_Add.x = (viiMath::Cos100(sRot)*sSpeed)/100;
		m_Add.y = (viiMath::Sin100(sRot)*sSpeed)/100;
		break;
	}
}


CEneH0305Bomb::~CEneH0305Bomb()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneH0305Bomb::SeqMain()
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

//	SetRader( enTypeMarkerGroundObject );
//	DirectionAdjust();


	switch( m_sType ){
	case enTypeFire:
		//m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
		//m_HitAtari.SetHantei(ID_VERSIS_ATK , &m_Pos );
		break;
	case enTypeBrown:
	case enTypeRock:
	default:
		break;
	}

	m_sTimer ++;

	viiSub::SetGravitMove(m_Pos.x , m_Pos.y);

}


void CEneH0305Bomb::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	Sint32 sColorAdd = 4;

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	switch( m_sType ){
	case enTypeBrown:
		m_sScale ++;
		sColorAdd = 4;
		break;

	case enTypeFire:
		m_Pos.x -= m_Add.x;
		m_Pos.y -= m_Add.y;
//		m_sRotation ++;
		if( m_sColor >= 120 && m_sTimer%3==0)
		{
			//小さい赤い炎
			new CEneH0305Bomb( CEneH0305Bomb::enTypeSmoke , m_Pos.x+viiMath::Cos100(m_sRotation)*8 , m_Pos.y+viiMath::Sin100(m_sRotation)*8,m_Pos.z,m_sRotation-30+viiSub::Rand()%60, 220+viiSub::Rand()%30);
		}
		if( m_sColor >60 && m_sTimer%8==0)
		{
			Sint32 sDist = 16+viiSub::Rand()%32;
			new CEneH0305Bomb( CEneH0305Bomb::enTypeSmokeBlack , m_Pos.x+viiMath::Cos100(m_sRotation)*sDist , m_Pos.y+viiMath::Sin100(m_sRotation)*sDist,m_Pos.z,m_sRotation, 90+viiSub::Rand()%30);
		}
		break;

	case enTypeRock:
		sColorAdd = 4;
		m_Add.x += -m_Add.x/20;
		m_Add.y += -m_Add.y/20;
		break;

	case enTypeSmokeBlack:
		sColorAdd = 3;
		break;
	case enTypeSmoke:
		m_sRotation += 8;
		break;
	}

	if( m_sColor > 0 )
	{
		m_sColor -= sColorAdd;
		if( m_sColor <= 0 )
		{
			m_sColor = 0;
		}
	}
	else
	{
		SetActionSeq( enActionSeqCrash );
		m_sColor = 0;
	}

	m_sScale ++;

}


void CEneH0305Bomb::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CEneH0305Bomb::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

/*
	viiMus::PlaySound(enSoundExplosionMiddle);

	ScorePlus( enScore );

	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
*/

	SetActionSeq( enActionSeqEnd );

}

void CEneH0305Bomb::Draw()
{
	Sint32 x,y;

	x =  m_Pos.x;
	y =  m_Pos.y;

	switch( m_sType ){
	case enTypeBrown:
		//土煙
		viiDraw::Sprite( &SprCEneH0305Missile[ 5 ] , x , y , m_Pos.z+2 , ATR_ALPHA_PLUS , ARGB(m_sColor,0xff,0xff,0xff) , m_sScale/100.0f );
		break;
	case enTypeFire:
		//メインの爆発
		viiDraw::Sprite( &SprCEneH0305Missile[ 0 ] , x , y , m_Pos.z+1 , ATR_ALPHA_PLUS , ARGB(m_sColor,0xff,0xff,0xff) , m_sScale/100.0f ,-5+m_sRotation+90);
		viiDraw::Sprite( &SprCEneH0305Missile[ 0 ] , x , y , m_Pos.z+1 , ATR_ALPHA_PLUS , ARGB(m_sColor,0xff,0xff,0xff) , m_sScale/150.0f  ,5+m_sRotation+90);
		break;
	case enTypeSmoke:
		viiDraw::Sprite( &SprCEneH0305Missile[ 1 ] , x , y , m_Pos.z , ATR_ALPHA_PLUS , ARGB(m_sColor,0xff,0xff,0xff) , m_sScale/300.0f , m_sRotation );
		break;
	case enTypeRock:
		viiDraw::Sprite( &SprCEneH0305Missile[ 2 ] , x , y+m_sHigh , m_Pos.z+3 , ATR_ALPHA_PLUS , ARGB(m_sColor,0xff,0xff,0xff) , 1.0f ,m_sRotation);
		break;
	case enTypeSmokeBlack:
		viiDraw::Sprite( &SprCEneH0305Missile[ 1 ] , x , y , m_Pos.z+1 , ATR_ALPHA_MINUS , ARGB(m_sColor/2,0xff,0xff,0xff) , m_sScale/70.0f , m_sRotation );
		break;
	}
}

