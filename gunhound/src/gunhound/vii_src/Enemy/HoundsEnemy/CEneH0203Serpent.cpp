//--------------------------------------------------------------------------------
//
// Stage06:サーペント
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

	enScore = 3000,
	enScoreDeluxe = 10100,
	enRotationSpeed = 1,
};

//HoundData\enemychara\st02\TexEneStg0201.bmp
gxSprite SprSeaSnake[]={
	{enTexCEneH0203Serpent,  224,  0	,32	,48	,16,  24},	//(0)頭
	{enTexCEneH0203Serpent,  192,  0	,32	,48	,16,  24},	//(1)胴体１A
	{enTexCEneH0203Serpent,  160,  0	,32	,48	,16,  24},	//(2)胴体２B
	{enTexCEneH0203Serpent,  128,  0	,32	,32	,16,  16},	//(3)胴体２A
	{enTexCEneH0203Serpent,  96 ,  0	,32	,32	,16,  16},	//(4)胴体２B
};

gxSprite SprSeaSnakeShadow[]={
	{enTexCEneH0203Serpent,  0,  0	,48	,16	,24,  8},
	{enTexCEneH0203Serpent,  0,  16	,48	,16	,24,  8},
	{enTexCEneH0203Serpent,  0,  32	,48	,16	,24,  8},
	{enTexCEneH0203Serpent,  0,  48	,48	,16	,24,  8},
};


CEneH0203Serpent::CEneH0203Serpent( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------

	m_Pos.x = x;
	m_Pos.y = y;

	m_HitAtari.set_hp(200);

	m_HitAtari.set_ap(1);
	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ay2 = enKuraiBottom;

	m_HitKurai.set_hp(200);
	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sHakaiCnt = 0;

#ifdef _VII_DEBUG_
//	m_HitKurai.set_hp(2);
#endif

//	m_bEnd      = gxFalse;
//	m_bDestroy  = gxFalse;
	m_bDamage = gxFalse;
	m_sCrashSeq = 0;
	m_sRotation = 0;

	for(Sint32 ii = 0; ii< enMaxSnakeLength ; ii++)
	{
		m_pChild[ii] = new CEneH0203Body(ii);
	}

	m_sPositionCnt = 0;	//座標保存用カウンタ
	m_sSpeed = 0;		//速度
	m_sWait = 90;		//時間
	m_sMineTime = 0;	//埋まり時間
	m_sBombWait = 0;	//爆破時間
	m_sRemainBody = 0;	//残存胴体
	m_sAppearWait = 60;	//でてくるまでのタイムラグ
	m_bGround = gxFalse;

//	m_bStart = gxFalse;
	ResetBodyPosition();
}


CEneH0203Serpent::~CEneH0203Serpent()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneH0203Serpent::SeqInit()
{
	//-----------------------------------------------------
	//初期化
	//-----------------------------------------------------
	if( m_bGround ) return;

	ResetBodyPosition();

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		//まだ出ていなければ埋まっておく
//		return;
	}

	m_sPositionCnt = 0;
	m_sWait        = 90;
	m_sMineTime    = 0;
	m_sAppearWait  = 60;

	SetActionSeq( enActionSeqMain );

}


void CEneH0203Serpent::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------
	Sint32 old = m_Pos.y;

	if( m_bClear )
	{
		for(int i=0;i<enMaxSnakeLength;i++)
		{
			if( m_pChild[i] )
			{
				m_pChild[i]->Clear();
			}
		}
		
		SetActionSeq( enActionSeqEnd );
		return;
	}

	ChildSyori();

	if(m_sAppearWait > 0)
	{
//		pGame->QuakeOn();
//		viiEff::Set(EFF_DUNGEONROCK,m_Pos.x,m_Pos.y-3200,argset(viiSub::Rand()%360,300,1) );
//		viiEff::Set(EFF_SANDSTORM,m_Pos.x+(-30+viiSub::Rand()%60)*100,m_Pos.y-3200,NULL);
//		new CEffSandStorm(	m_Pos.x+(-30+viiSub::Rand()%60)*100,m_Pos.y-3200 ,  0,	0	);
		m_sAppearWait --;
		return;
	}
	else
	{
		pGame->QuakeOff();
	}

	//-----------------------------------
	//破壊処理
	//-----------------------------------
	if (m_HitKurai.is_dead() )
	{
		SetActionSeq( enActionSeqCrash );
	}

	//-----------------------------------------------------
	//ダメージを受けた
	//-----------------------------------------------------
	m_bDamage = m_HitKurai.is_damage();

	m_sControl = Joy[0].psh;

	//-----------------------------------------------------
	//思考パターン
	//-----------------------------------------------------
	LogicAI();

	Update();

	SetRader( enTypeMarkerMiddleBoss );

	//-----------------------------------------------------
	//水面判定
	//-----------------------------------------------------

	if( !IsScrollOut( enScrollOutWidth*2 , enScrollOutHeight ) )
	{
		if( old > pGame->GetMarineLine() )
		{
			if( m_Pos.y < pGame->GetMarineLine() )
			{
				viiMus::PlaySound( enSoundSplashS );
				new CEffWater( m_Pos.x     , m_Pos.y, enTypeSplashPillar , 0);
				new CEffWater( m_Pos.x-800 , m_Pos.y, enTypeSplashPillar , 0);
				new CEffWater( m_Pos.x+800 , m_Pos.y, enTypeSplashPillar , 0);
			}
		}
		else if( old < pGame->GetMarineLine() )
		{
			if( m_Pos.y > pGame->GetMarineLine() )
			{
				viiMus::PlaySound( enSoundSplashM );
				new CEffWater( m_Pos.x , m_Pos.y, enTypeSplashLarge , 0);
			}
		}
	}

	//-----------------------------------------------------
	//くらい判定登録
	//-----------------------------------------------------

	m_HitKurai.SetHantei(ID_ENEMY_DEF,&m_Pos);

	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
	m_HitAtari.SetHantei(ID_ENEMY_ATK,&m_Pos);


#ifdef _VII_VII_DEBUG_
	viiDbg::printf(100,50,"SNAKE:%d,%d,%d",m_Pos.x,m_Pos.y,m_sRotation);
#endif


}


void CEneH0203Serpent::Update()
{
	m_Pos.x += (viiMath::Cos100(m_sRotation)*m_sSpeed)/100;
	m_Pos.y += (viiMath::Sin100(m_sRotation)*m_sSpeed)/100;

	//-----------------------------------
	//座標リストを更新
	//-----------------------------------
	m_StChainPos2[ m_sPositionCnt ].x   = m_Pos.x;
	m_StChainPos2[ m_sPositionCnt ].y   = m_Pos.y;
	m_StChainPos2[ m_sPositionCnt ].r   = m_sRotation;

	m_sPositionCnt ++;
	m_sPositionCnt = (enMaxPositionNum+m_sPositionCnt)%enMaxPositionNum;

}


void CEneH0203Serpent::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	m_sRotation = 65*viiMath::Cos100( m_sTimer*2 )/100;
	m_sSpeed = enMaxSnakeSpeed ;

	return;

	if (m_sWait == 0)
	{
		int rot = get_valken_rot( m_Pos.x,m_Pos.y);
		int dir = viiMath::GetTargetAngle(m_sRotation,rot);
		if(dir < -5)
		{
			m_sControl |= JOY_L;
		}
		else if(dir > 5)
		{
			m_sControl |= JOY_R;
		}
	}
	else
	{
		m_sWait --;
	}

	m_sControl |= JOY_U;

	//-----------------------------------
	//埋まりチェック
	//-----------------------------------
	if (m_sWait == 0)
	{
		int cnt = 0;
		int xx = m_Pos.x/800;
		int yy = m_Pos.y/800;

		for(int y=-2;y<3;y++)
		{
			for(int x=-2;x<3;x++)
			{
				if(pGame->pBg->get_kabetype8(xx+x,yy+y))
				{
					//cnt ++;
				}
			}
		}

		if(cnt >= 25)
		{
			m_sMineTime ++;
			viiEff::Set(EFF_DUNGEONROCK , m_Pos.x , m_Pos.y-3200,argset(viiSub::Rand()%360,300,1) );
			new CEffSandStorm(	m_Pos.x+(-30+viiSub::Rand()%60)*100,m_Pos.y-3200 ,  0,	0	);
			if(m_sMineTime >= 6)
			{
				m_sControl = JOY_D;	//動きを止める

				if(m_sMineTime >= 250)
				{
					m_bGround = gxTrue;
					SetActionSeq( enActionSeqInit );
				}
			}
		}
		else
		{
			m_sMineTime = 0;
		}
	}

	//-----------------------------------
	//コントロール
	//-----------------------------------
	if(m_sControl&JOY_L) m_sRotation -= enRotationSpeed;
	if(m_sControl&JOY_R) m_sRotation += enRotationSpeed;
	if(m_sControl&JOY_D) m_sSpeed = 0;
	if(m_sControl&JOY_U) m_sSpeed = enMaxSnakeSpeed;

}


void CEneH0203Serpent::ResetBodyPosition()
{
	for(Sint32 i=0;i<enMaxPositionNum;i++)
	{
		m_StChainPos2[ i ].x = m_Pos.x;
		m_StChainPos2[ i ].y = m_Pos.y;
		m_StChainPos2[ i ].r = m_sRotation;
	}

}

void CEneH0203Serpent::ChildSyori()
{

	Sint32 n = m_sPositionCnt;

	for(int i=0;i<enMaxSnakeLength;i++)
	{
		n = m_sPositionCnt-( (i+1)*12);
		n = (enMaxPositionNum+n)%enMaxPositionNum;
		if( m_pChild[i] )
		{
			Sint32 old = m_pChild[i]->m_Pos.y;
			Sint32 x,y;
			x = m_StChainPos2[n].x;
			y = m_StChainPos2[n].y;
			m_pChild[i]->SetPos( x , y , m_StChainPos2[n].r );

			if( old > pGame->GetMarineLine() )
			{
				if( y < pGame->GetMarineLine() )
				{
					new CEffWater( x     , y, enTypeSplashPillar , 0);
					new CEffWater( x-800 , y, enTypeSplashPillar , 0);
					new CEffWater( x+800 , y, enTypeSplashPillar , 0);
				}
			}
			else if( old < pGame->GetMarineLine() )
			{
				if( y > pGame->GetMarineLine() )
				{
					new CEffWater( x , y, enTypeSplashLarge , 0);
				}
			}
			
		}
	}

}

void CEneH0203Serpent::Atack()
{
	//-----------------------------------------------------
	//攻撃
	//-----------------------------------------------------


}


void CEneH0203Serpent::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	Update();

	ChildSyori();

	switch(m_sCrashSeq){
	case 0:
		CrashSeqWait();
		break;
	case 10:
		CrashSeqBomb();
		break;
	}

}

void CEneH0203Serpent::CrashSeqBomb()
{
	if( m_bClear )
	{
		//-------------------------------------------------------------------
		viiDbg::log("ムカデ終了");
		//-------------------------------------------------------------------
		SetActionSeq( enActionSeqEnd );
	}
}


void CEneH0203Serpent::CrashSeqWait()
{
	gxBool bEnd = gxFalse;
	m_sBombWait ++;	//爆破時間

	//Child破壊

	if(m_sBombWait%8==0)
	{
		m_sRemainBody++;
		//viiMus::PlaySound( enSoundCrashA );
		Sint32 n = enMaxSnakeLength-m_sRemainBody;

		if( n >= 0  && m_pChild[n] )
		{
			m_pChild[n]->SetDestroy();
			if( m_pChild[n]->IsHakai() )
			{
				m_sHakaiCnt ++;
			}
			m_pChild[n] = NULL;
		}
		else
		{
			bEnd = gxTrue;
		}
	}

	if( bEnd )	//m_sBombWait>=20*enMaxSnakeLength)
	{
		if ( !m_bDead )
		{
			viiMus::PlaySound( enSoundCrashA );
			viiEff::Set(EFF_BIGBOMB,m_Pos.x+(-30+viiSub::Rand()%60)*100, m_Pos.y+(-40+viiSub::Rand()%80)*100,argset(1));
			if( m_sHakaiCnt == enMaxSnakeLength )
			{
				ScorePlus( enScoreDeluxe );
			}
			else
			{
				ScorePlus(enScore);
			}

			viiEff::SetBombEffect(m_Pos.x, m_Pos.y,3,5);
			pGame->QuakeOff();
		}
		m_bDead = gxTrue;
		m_sCrashSeq = 10;
	}
}


void CEneH0203Serpent::Draw()
{
	//-----------------------------------------------------
	//描画
	//-----------------------------------------------------

	Sint32 sLine = pGame->GetMarineLine();
	if( sLine && m_Pos.y>sLine )
	{
		viiDraw::Sprite( &SprSeaSnakeShadow[0],m_Pos.x,sLine,PRIO_EFFECT,ATR_ALPHA_MINUS,0x40202020 );
		return;
	}

	viiDraw::Sprdmg( m_bDamage , &SprSeaSnake[0],m_Pos.x,m_Pos.y,PRIO_ENEMY,ATR_DFLT,ARGB_DFLT,1.0f,m_sRotation);

}


//--------------------------------------------------------------------------------
//
// Stage06:胴体
//
//--------------------------------------------------------------------------------

CEneH0203Body::CEneH0203Body( Sint32 sID )
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
	m_HitKurai.set_hp( HP_ARMORCLASS_B );

	m_HitAtari.set_hp(200);
	m_HitAtari.set_ap(1);
	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ay2 = enKuraiBottom;

	m_HitKurai.set_hp(enDefPointZakoM);
	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ay2 = enKuraiBottom;

	m_bHakai = gxFalse;

}


CEneH0203Body::~CEneH0203Body()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneH0203Body::SeqInit()
{
	//-----------------------------------------------------
	//初期化
	//-----------------------------------------------------
	SetActionSeq( enActionSeqMain );

}


void CEneH0203Body::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( m_bClear )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	if( m_bDestroy )
	{
		SetActionSeq( enActionSeqCrash );
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

	//m_bDamage = m_HitKurai.is_damage();

	//-----------------------------------
	//くらい判定登録
	//-----------------------------------
	if( !m_bHakai )
	{
		if( m_HitKurai.is_dead())
		{
			ScorePlus( 100 );
			viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
			m_bHakai = gxTrue;
		}
	}
	else
	{
		m_HitKurai.set_hp(1);
	}
	m_HitKurai.SetHantei(ID_ENEMY_DEF,&m_Pos );

	//-----------------------------------
	//あたり判定登録
	//-----------------------------------
	m_HitAtari.set_hp(1);
	m_HitAtari.set_ap(1);
	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
	m_HitAtari.SetHantei(ID_ENEMY_ATK,&m_Pos );


}


void CEneH0203Body::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	Atack();
}


void CEneH0203Body::Atack()
{
	//-----------------------------------------------------
	//攻撃
	//-----------------------------------------------------

//	if( m_sTimer%(16*8) < 16*4 )
//	{
//		if( m_sTimer%4==0 )
//		{
//			if( m_ID%2 )
//			{
//				CEne0001EneBullet *p;
//				p = new CEne0001EneBullet( m_Pos.x, m_Pos.y , m_sRotation , 200+m_sAtackSeq*40 );
//				p->SetDrop();
//			}
//			m_sAtackSeq ++;
//			m_sAtackSeq = m_sAtackSeq%8;
//		}
//	}

}


void CEneH0203Body::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	if( m_bHakai )
	{
		ScorePlus( 1000 );
	}

	viiMus::PlaySound( enSoundExplosionSmall );
	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 3 , 5 );
	SetActionSeq( enActionSeqEnd );

}


void CEneH0203Body::Draw()
{
	
	//-----------------------------------------------------
	//描画
	//-----------------------------------------------------
	int ptn = (viiSub::GameCounter()%12 <6)? 0 : 1;

	Sint32 sLine = pGame->GetMarineLine();
	if( sLine && m_Pos.y>sLine )
	{
		viiDraw::Sprite( &SprSeaSnakeShadow[0],m_Pos.x,sLine,PRIO_EFFECT,ATR_ALPHA_MINUS,0x40202020 );
		return;
	}

//	if(m_ID%3==0)
	{
		viiDraw::Sprdmg( m_bDamage , &SprSeaSnake[3+ptn],m_Pos.x,m_Pos.y,PRIO_ENEMY,ATR_DFLT,ARGB_DFLT,1.0f,m_sRotation);
		if( !m_bHakai )
		{
			viiDraw::Sprdmg( m_bDamage , &SprSeaSnake[1+ptn],m_Pos.x,m_Pos.y,PRIO_ENEMY,ATR_DFLT,ARGB_DFLT,1.0f,m_sRotation);
		}
	}

}


