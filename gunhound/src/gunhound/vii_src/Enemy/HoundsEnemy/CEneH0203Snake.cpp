//--------------------------------------------------------------------------------
//
// Stage06:ボスワーム
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../Effect/EffectBase.h"
enum {
	enLogicWaitTime   = 120,
	enScrollOutWidth  = 64*100,
	enScrollOutHeight = 64*100,

	enKuraiLeft  = -12,
	enKuraiTop   = -12,
	enKuraiRight =  12,
	enKuraiBottom=  12,

	enScore = 80000,
	enRotationSpeed = 1,
	enSnakeHead = CEneH0203Snake::enMaxSnakeLength-1,
	enSnakeBody = CEneH0203Snake::enMaxSnakeLength-6,
	enDanmakuIndex = 2,
};

//HoundData\enemychara\st02\TexEneStg0201.bmp
gxSprite SprSeaBossSnake[]={
	{enTexCEneH0203Serpent,   80*0,  176	,80	,80	,8,  40},	//(0)胴体1
	{enTexCEneH0203Serpent,   80*1,  176	,80	,80	,8,  40},	//(1)胴体2
	{enTexCEneH0203Serpent,   80*2,  176	,80	,80	,8,  40},	//(2)頭3

	{enTexCEneH0203Serpent,   80*2,  96	,80	,80	,8,  40},		//(3) 頭3-破壊
	{enTexCEneH0203Serpent,   88  ,  96	,64	,80	,32, 40},		//(4) 胴体つなぎ
};

extern gxSprite SprSeaSnakeShadow[];
//={
//	{enTexCEneH0203Serpent,  0,  0	,48	,16	,24,  8},
//	{enTexCEneH0203Serpent,  0,  16	,48	,16	,24,  8},
//	{enTexCEneH0203Serpent,  0,  32	,48	,16	,24,  8},
//	{enTexCEneH0203Serpent,  0,  48	,48	,16	,24,  8},
//};



CEneH0203Snake::CEneH0203Snake( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;

	m_Base.x = x;
	m_Base.y = y;

	for(Sint32 ii = 0; ii< enMaxSnakeLength ; ii++)
	{
		m_pChild[ii] = new CEneH0203BigBody(ii);
	}

//	m_sRotation = -120;
	m_sRotation = 0;

	//のけぞり制御
	m_sDamageFrm  = 0;
	m_sDamageCnt  = 0;
	m_sAtackTimer = 0;
	m_sDropAtack = 0;

	m_sLogicTimer = 0;
	m_sLogicSeq   = 0;
	m_sLogicPtn   = 0;

	m_sCrashTimer = 0;
	ResetBodyPosition();

	m_sCrashCnt = 0;
	m_sCrashWait = 0;

	m_Add.x = 0;
	m_Add.y = 0;

	m_bCrash = gxFalse;

	CCockpit::GetInstance()->SetDanmakuIndex( enDanmakuIndex , gxTrue );
}


CEneH0203Snake::~CEneH0203Snake()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneH0203Snake::SeqInit()
{
	//-----------------------------------------------------
	//初期化
	//-----------------------------------------------------


//	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
//	{
//		//まだ出ていなければ埋まっておく
//		return;
//	}

	SetActionSeq( enActionSeqMain );

}


void CEneH0203Snake::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------
//	if(Joy[0].psh&JOY_U ) m_sRotation --;
//	if(Joy[0].psh&JOY_D ) m_sRotation ++;
	//-----------------------------------------------------
	//思考パターン
	//-----------------------------------------------------

	LogicAI();

	SetRader( enTypeMarkerMiddleBoss );

	if( m_pChild[ enSnakeHead ]->IsDamage( ) )
	{
		m_sDamageCnt ++;

		if( m_sDamageCnt >= 60 )
		{
			//のけぞりカウンタ
			m_sDamageFrm = 90;
		}
	}

	if( m_pChild[ enSnakeBody ]->IsDamage( ) )
	{
		//全ボディフラッシュ
		for(Sint32 ii=0;ii<enMaxSnakeLength;ii++)
		{
			m_pChild[ ii ]->SetFlash();
		}
	}

	if( m_pChild[ enSnakeBody ]->IsCrash( ) )	//Joy[0].trg&BTN_X ||
	{
		SetActionSeq( enActionSeqCrash );
	}

}


void CEneH0203Snake::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
//	if(Joy[0].trg&BTN_X)
//	{
//		m_sLogicPtn = 1;
//	}


	switch( m_sLogicPtn ){
	case 0:
		LogicNormal();
		break;
	case 1:
		LogicAssault();
		break;
	}

}


void CEneH0203Snake::LogicAssault()
{
	//-----------------------------------
	//突撃
	//-----------------------------------
	Sint32 sLengthW = 52*100;
	Sint32 sLengthH = 52*100;
	Sint32 r = m_sRotation+(viiMath::Cos100((m_sTimer)%360)*40)/100+30;
	Sint32 mr = r;
	Sint32 tr,x,y;
	Sint32 limit = -8;//gxAbs;
	Sint32 sSpd=1;

	ghPos stTgt;

	stTgt.x = pGame->GetHound()->m_Pos.x;
	stTgt.y = pGame->GetHound()->m_Pos.y;
/*
	if( m_sRotation > -120 )
	{
		m_sRotation -= 1;//(-120-m_sRotation)/10;
	}
*/

	switch( m_sLogicSeq ){
	case 0:
		m_Add.x = -400;
		m_sLogicTimer = 0;
		m_sLogicSeq = 100;
		viiMus::PlaySound( enSoundCrashB );
		break;

	case 100:
		//後ろに下がって突進
		stTgt.x = m_Base.x-320*100;
		stTgt.y = m_Base.y+160*100;
		m_sRotation += (-120-m_sRotation)/100;
		if( m_Add.x < 300 )
		{
			m_Add.x += 3;
		};
		if( m_Add.x > 0 && m_Pos.x > m_Base.x-60*100 )
		{
			m_sLogicTimer = 0;
			m_sLogicSeq = 200;
		}
		break;
	case 200:
		stTgt.x = m_Base.x+320*100;
		stTgt.y = m_Base.y+80*100;
		{
			Sint32 sOld = m_sRotation;
			m_sRotation += (20-m_sRotation)/40;
			if( sOld < -50 && m_sRotation >= -50 )
			{
				viiMus::PlaySound( enSoundSplashL );
			}
		}
		if(m_sLogicTimer >= 120)
		{
			m_sLogicTimer = 0;
			m_sLogicSeq = 300;
		}
		break;

	case 300:
		stTgt.x = m_Base.x+320*100;
		stTgt.y = m_Base.y+160*100;
		m_sRotation += (20-m_sRotation)/50;
		m_Pos.x += ((m_Base.x-120*100)-m_Pos.x)/100;
		if(m_sLogicTimer >= 120)
		{
			m_sLogicTimer = 0;
			m_sLogicSeq = 900;
		}
		break;

	case 900:
		m_sLogicSeq = 0;
		m_sLogicPtn = 0;
		break;
	}
	r = m_sRotation;

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	m_sLogicTimer ++;

	x = m_Pos.x;
	y = m_Pos.y;

	m_StChainPos[ 0 ].x   = x;
	m_StChainPos[ 0 ].y   = y;
	m_StChainPos[ 0 ].r   = r;

	for(Sint32 ii=1;ii<enMaxSnakeLength;ii++)
	{
		x = x+(viiMath::Cos100(mr)*sLengthW)/100;
		y = y+(viiMath::Sin100(mr)*sLengthH)/100;

		tr = viiMath::Atan2_100( stTgt.y-m_Pos.y , stTgt.x-m_Pos.x ) /100;
		NORMALIZE(tr);

		r = viiMath::GetTargetAngle( mr , tr );
		if( r < -sSpd )
		{
			m_StChainPos[ ii ].r  -= sSpd;
		}
		else if( r > sSpd )
		{
			m_StChainPos[ ii ].r  += sSpd;
		}

		if(m_StChainPos[ ii ].r < -limit) m_StChainPos[ ii ].r = -limit;
		if(m_StChainPos[ ii ].r > limit)  m_StChainPos[ ii ].r = limit;


		mr += m_StChainPos[ ii ].r;

		m_StChainPos[ ii ].x  = x;
		m_StChainPos[ ii ].y  = y;
		m_StChainPos[ ii ].dr  = mr;

		m_pChild[ii]->SetPos( x , y , mr );//m_StChainPos[ ii ].r );
		limit +=8;
	}

}


void CEneH0203Snake::LogicNormal()
{
	//-----------------------------------
	//ノーマル
	//-----------------------------------

	Sint32 sLengthW = 52*100;
	Sint32 sLengthH = 52*100;
	Sint32 r = m_sRotation+(viiMath::Cos100((m_sTimer)%360)*40)/100+30;
	Sint32 mr = r;
	Sint32 tr,x,y;
	Sint32 limit = -8;//gxAbs;
	Sint32 sSpd=1;

	m_Pos.x += (m_Base.x-m_Pos.x)/100;

	if( m_sRotation > -120 )
	{
		m_sRotation -= 1;//(-120-m_sRotation)/10;
	}

	if(m_sDamageFrm > 0 )
	{
		//-----------------------------------------------------
		//首ふり
		//-----------------------------------------------------
		for(Sint32 i=0;i<enMaxPositionNum;i++)
		{
			m_StChainPos[ i ].r += (-200-m_StChainPos[ i ].r)/100;
			m_StChainPos[ i ].dr = m_StChainPos[ i ].r;
		}
		m_sDamageFrm --;
		m_sDamageCnt = 0;
		if( m_sDamageFrm == 0 )
		{
			m_sAtackTimer = 240;
		}
	}

	if( m_sAtackTimer > 0 )
	{
		//攻撃カウンター

		m_sAtackTimer --;
		if( m_sAtackTimer%32 == 0 )
		{
			m_pChild[ enSnakeHead ]->SetAtack();
		}
		if( m_sAtackTimer == 0 )
		{
			m_sLogicPtn = 1;
			m_sLogicSeq = 0;
		}

		m_sDropAtack = 0;
	}
	else
	{
		m_sDropAtack ++;
		m_sDropAtack = m_sDropAtack%300;
		if( m_sDropAtack > 128 )
		{
			if( m_sDropAtack%32 == 0 )
			{
				m_pChild[ enSnakeHead ]->SetAtack(1);	//ドロップショット
			}
		}
	}

	x = m_Pos.x;
	y = m_Pos.y;

	m_StChainPos[ 0 ].x   = x;
	m_StChainPos[ 0 ].y   = y;
	m_StChainPos[ 0 ].r   = r;

	for(Sint32 ii=1;ii<enMaxSnakeLength;ii++)
	{
		x = x+(viiMath::Cos100(mr)*sLengthW)/100;
		y = y+(viiMath::Sin100(mr)*sLengthH)/100;

		tr = get_valken_rot( x , y );
		r = viiMath::GetTargetAngle( mr , tr );
		if( r < -sSpd )
		{
			m_StChainPos[ ii ].r  -= sSpd;
		}
		else if( r > sSpd )
		{
			m_StChainPos[ ii ].r  += sSpd;
		}

		if(m_StChainPos[ ii ].r < -limit) m_StChainPos[ ii ].r = -limit;
		if(m_StChainPos[ ii ].r > limit)  m_StChainPos[ ii ].r = limit;


		mr += m_StChainPos[ ii ].r;

		m_StChainPos[ ii ].x  = x;
		m_StChainPos[ ii ].y  = y;
		m_StChainPos[ ii ].dr  = mr;

		m_pChild[ii]->SetPos( x , y , mr );//m_StChainPos[ ii ].r );
		limit +=8;
	}

}


void CEneH0203Snake::ResetBodyPosition()
{
	for(Sint32 i=0;i<enMaxPositionNum;i++)
	{
		m_StChainPos[ i ].x = m_Pos.x;
		m_StChainPos[ i ].y = m_Pos.y;
		m_StChainPos[ i ].r = m_sRotation;
		m_StChainPos[ i ].dr = m_sRotation;
	}

}

void CEneH0203Snake::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	pGame->QuakeOn( 32 );

	m_bCrash = gxTrue;

	//全ボディクラッシュ
	if( m_sCrashCnt < enMaxSnakeLength )
	{
		if( m_sCrashWait == 0 )
		{
			m_pChild[ m_sCrashCnt ]->SetDestroy();
			m_sCrashCnt ++;;
			m_sCrashWait = 120;
			if( m_sCrashCnt ==enMaxSnakeLength )
			{
				CCockpit::GetInstance()->SetDanmakuIndex( enDanmakuIndex , gxFalse );
				viiMus::PlaySound( enSoundCrashA );
			}
			else
			{
				viiMus::PlaySound(enSoundExplosionMiddle);
				m_sCrashWait = 6;
			}
		}
		else
		{
			m_sCrashWait --;
		}
	}
	else
	{
		m_sCrashTimer ++;
		if( m_sCrashTimer == 120 )
		{
			m_bDead = gxTrue;
		}
	}

	if( m_bClear )
	{
		SetActionSeq( enActionSeqEnd );
	}

}


void CEneH0203Snake::Draw()
{
	//-----------------------------------------------------
	//描画
	//-----------------------------------------------------


}


//--------------------------------------------------------------------------------
//
// Stage06:胴体
//
//--------------------------------------------------------------------------------

CEneH0203BigBody::CEneH0203BigBody( Sint32 sID )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = 0;
	m_Pos.y = 0;
	m_ID = sID;
	m_bDestroy = gxFalse;
	m_bDamage  = gxFalse;
	m_sRotation = 0;

	m_HitAtari.set_hp(enHitPointBossSnakeHead);
	m_HitAtari.set_ap(1);
	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ay2 = enKuraiBottom;

	m_HitKurai.set_hp(enHitPointBossSnakeBody);
	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ay2 = enKuraiBottom;

	m_bAtack = gxFalse;
	m_bFlash = gxFalse;
	m_bCrashHead = gxFalse;
	m_bCrashBody = gxFalse;
	m_sAtackPtn = 0;

	m_Add.x = 0;
	m_Add.y = 0;

	m_Old.x = 0;
	m_Old.y = 0;
}


CEneH0203BigBody::~CEneH0203BigBody()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneH0203BigBody::SeqInit()
{
	//-----------------------------------------------------
	//初期化
	//-----------------------------------------------------
	SetActionSeq( enActionSeqMain );

}


void CEneH0203BigBody::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------
	OBJ_POS_T StPos;
	StPos.x = m_Pos.x;
	StPos.y = m_Pos.y;

	if( m_bDestroy )
	{
//		viiMus::PlaySound( enSoundCrashA );
		EffectMakeBigBomb(m_Pos.x , m_Pos.y);
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 3 , 5 );
		m_Add.x = (1+viiSub::Rand()%3)*30;
		m_Add.y = -viiSub::Rand()%200;

		if( m_ID == enSnakeHead )
		{
			new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+15*1 , 600 );
			new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+45*1 , 800 );
			new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+135*1 ,600 );
			new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+165 , 600 );
			ScorePlus( enScore );
		}
		SetActionSeq( enActionSeqCrash );
		
		if( m_ID%4==0)
		{
			new CEffectDmgprt( m_Pos.x, m_Pos.y -3200, -90 , 100 );
			new CEffectDmgprt( m_Pos.x, m_Pos.y -3200, -88 , 120 );
			new CEffectDmgprt( m_Pos.x, m_Pos.y -3200, -92 , 80 );
		}
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{

	}

	//-----------------------------------------------------
	//思考パターン
	//-----------------------------------------------------
	LogicAI();

	//-----------------------------------------------------
	//くらい判定登録
	//-----------------------------------------------------
	Sint32 ax,ay;

	if( m_ID == enSnakeHead )
	{
		m_bDamage = m_HitKurai.is_damage();
		m_HitKurai.ax1 = enKuraiLeft*3;
		m_HitKurai.ax2 = enKuraiRight*3;
		m_HitKurai.ay1 = enKuraiTop*3;
		m_HitKurai.ay2 = enKuraiBottom*3;

		ax = m_Pos.x+viiMath::Cos100( m_sRotation )*24;
		ay = m_Pos.y+viiMath::Sin100( m_sRotation )*24;

		StPos.x = ax;
		StPos.y = ay;

		if( !m_bCrashHead && m_HitKurai.is_dead() )
		{
			viiEff::SetBombDeluxe( ax , ay , 5 , 5+viiSub::Rand()%2 );
			m_bCrashHead = gxTrue;
			m_bDamage    = gxFalse;
		}
	}
	else if( m_ID == enSnakeBody )
	{
		m_bDamage = m_HitKurai.is_damage();
		m_HitKurai.ax1 = enKuraiLeft*3;
		m_HitKurai.ax2 = enKuraiRight*3;
		m_HitKurai.ay1 = enKuraiTop*3;
		m_HitKurai.ay2 = enKuraiBottom*3;

		ax = m_Pos.x+viiMath::Cos100( m_sRotation )*24;
		ay = m_Pos.y+viiMath::Sin100( m_sRotation )*24;

		StPos.x = ax;
		StPos.y = ay;

		if( m_HitKurai.is_dead() )
		{
			m_bDead = gxTrue;
		}

	}
	else
	{
		ax = m_Pos.x+viiMath::Cos100( m_sRotation )*24;
		ay = m_Pos.y+viiMath::Sin100( m_sRotation )*24;

		m_HitKurai.set_hp(1);
		m_HitKurai.set_ap(1);
		m_bDamage = m_bFlash;
		m_bFlash = gxFalse;
	}

	Sint32 sLine = pGame->GetMarineLine();
	if( sLine && m_Pos.y>sLine-3800 && m_Pos.y<sLine )
	{
		if( m_Old.y < sLine-3800 )//viiSub::GameCounter()%16 == 0 )
		{
			new CEffWater( ax , sLine , enTypeSplashLarge , 0);
			MakeSplash( ax+(-8+viiSub::Rand()%16)*100 , sLine , PRIO_EFFECT );
		}
	}
	else if( m_Old.y > sLine-3800 && m_Pos.y<sLine-3800)//viiSub::GameCounter()%16 == 0 )
	{
		new CEffWater( ax , sLine , enTypeSplashLarge , 0);
		MakeSplash( ax+(-8+viiSub::Rand()%16)*100 , sLine , PRIO_EFFECT );
	}

 

	//-----------------------------------
	//くらい判定登録
	//-----------------------------------
	m_HitKurai.SetHantei(ID_ENEMY_DEF,&StPos );

	//-----------------------------------
	//あたり判定登録
	//-----------------------------------
	m_HitAtari.set_hp(1);
	m_HitAtari.set_ap(1);
	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
	m_HitAtari.SetHantei(ID_ENEMY_ATK,&m_Pos );

	m_bAtack = gxFalse;

}


void CEneH0203BigBody::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	if(	m_bAtack )
	{
		switch( m_sAtackPtn ){
		case 1:
			Atack2();
			break;
		case 0:
		default:
			Atack();
			break;
		}
	}

}


void CEneH0203BigBody::Atack()
{
	//-----------------------------------------------------
	//攻撃
	//-----------------------------------------------------

	Sint32 ax,ay;
	ax = m_Pos.x+viiMath::Cos100( m_sRotation )*32;
	ay = m_Pos.y+viiMath::Sin100( m_sRotation )*32;

	CEne0001EneSuperBall *p;
	p = new CEne0001EneSuperBall( ax , ay , m_sRotation ,0 );
	p->SetPattern( CEne0001EneSuperBall::enPatternTargetting );
	p->SetBulletIndex(enDanmakuIndex);

}


void CEneH0203BigBody::Atack2()
{
	//-----------------------------------------------------
	//攻撃２
	//-----------------------------------------------------

	Sint32 ax,ay;
	ax = m_Pos.x;
	ay = m_Pos.y;

	CEne0001EneSuperBall *q;
	q = new CEne0001EneSuperBall( ax , ay , -70, 920  );
	q->SetPattern( CEne0001EneSuperBall::enPatternDrop );
	q->SetScale( 1.f );
	q->SetEndless(  );
	q->SetDropGensui(gxTrue);
	q->SetBulletIndex(enDanmakuIndex);

	viiMus::PlaySound(enSoundEneShotLaser);

}


void CEneH0203BigBody::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	m_bDamage = gxFalse;

	if( m_Add.y < 800 ) m_Add.y += 2;

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	Sint32 sLine = pGame->GetMarineLine();

	if( sLine && m_Pos.y > sLine )
	{
		MakeSplash( m_Pos.x , m_Pos.y );
		viiMus::PlaySound( enSoundSplashM );

		SetActionSeq( enActionSeqEnd );
	}

}


void CEneH0203BigBody::Draw()
{
	
	//-----------------------------------------------------
	//描画
	//-----------------------------------------------------
	int ptn = (viiSub::GameCounter()%12 <6)? 0 : 1;

	Sint32 sLine = pGame->GetMarineLine();
	if( sLine && m_Pos.y>sLine )
	{
		viiDraw::Sprite( &SprSeaSnakeShadow[0],m_Pos.x,sLine,PRIO_EFFECT,ATR_ALPHA_MINUS,0x40202020 );
//		return;
	}

	if( m_ID == enSnakeHead )
	{
		if( m_bCrashHead )
		{
			viiDraw::Sprdmg( m_bDamage , &SprSeaBossSnake[3],m_Pos.x,m_Pos.y,PRIO_ENEMY,ATR_DFLT,ARGB_DFLT,1.0f,m_sRotation);
		}
		else
		{
			viiDraw::Sprdmg( m_bDamage , &SprSeaBossSnake[2],m_Pos.x,m_Pos.y,PRIO_ENEMY,ATR_DFLT,ARGB_DFLT,1.0f,m_sRotation);
		}
		viiDraw::Sprdmg( gxFalse , &SprSeaBossSnake[4],m_Pos.x,m_Pos.y,PRIO_ENEMY-1,ATR_DFLT,ARGB_DFLT,1.0f,m_sTimer%360);
	}
	else if( m_ID == enSnakeBody )
	{
		viiDraw::Sprdmg( m_bDamage , &SprSeaBossSnake[1],m_Pos.x,m_Pos.y,PRIO_ENEMY,ATR_DFLT,ARGB_DFLT,1.0f,m_sRotation);
		viiDraw::Sprdmg( gxFalse , &SprSeaBossSnake[4],m_Pos.x,m_Pos.y,PRIO_ENEMY-1,ATR_DFLT,ARGB_DFLT,1.0f,m_sTimer%360);
	}
	else
	{
		viiDraw::Sprdmg( m_bDamage , &SprSeaBossSnake[0],m_Pos.x,m_Pos.y,PRIO_ENEMY,ATR_DFLT,ARGB_DFLT,1.0f,m_sRotation);
		viiDraw::Sprdmg( gxFalse , &SprSeaBossSnake[4],m_Pos.x,m_Pos.y,PRIO_ENEMY-1,ATR_DFLT,ARGB_DFLT,1.0f,m_sTimer%360);
	}

}
