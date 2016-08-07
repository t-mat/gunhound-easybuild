//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../Effect/EffectBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -26,
	enKuraiTop   = -34,
	enKuraiRight =  26,
	enKuraiBottom=  0,

	enScore = 900,

	enBackTime = 120,

};

/*
gxSprite SprCEneH0202VorzoyWP[]=
{
	{enTexPageEnemyCommon01,0,192,64,64,32,64},//ボルゾイ
	{TPG_LEYNOS01,0,192,64,64,32,64},//ボルゾイ
};
*/

gxSprite SprCEneH0202VorzoyWP[]=
{
	{ enTexCEneH0202VorzoyWP,0*32,0*32,32,32,16,24},	//0:BODY
	{ enTexCEneH0202VorzoyWP,192,160,32,32,16,8},	//1:LEG奥
	{ enTexCEneH0202VorzoyWP,160,160,32,32,16,8},	//2:LEG手前
	{ enTexCEneH0202VorzoyWP,192,128,32,32,6,10},	//3:ARM
};

gxSprite SprCEneH0202VorzoyWPBody[]={
	{ enTexCEneH0202VorzoyWP,0*32,0*32,32,32,16,24},	//0:BODY0/ノーマル
	{ enTexCEneH0202VorzoyWP,1*32,0*32,32,32,16,24},	//1:BODY1/スラスター付
	{ enTexCEneH0202VorzoyWP,1*32,0*32,32,32,16,24},	//1:BODY2/シューター付
	{ enTexCEneH0202VorzoyWP,2*32,0*32,32,32,16,24},	//2:BODY3/軽量

	{ enTexCEneH0202VorzoyWP,0,192,32,32,16,24},		//2:BODY4/水中用
};

gxSprite SprCEneH0202VorzoyWPLegTemae[]={
	{ enTexCEneH0202VorzoyWP,0*32,1*32,32,32,16,8},	//0:LEG1
	{ enTexCEneH0202VorzoyWP,1*32,1*32,32,32,16,8},	//1:LEG2
	{ enTexCEneH0202VorzoyWP,2*32,1*32,32,32,16,8},	//2:LEG3
	{ enTexCEneH0202VorzoyWP,3*32,1*32,32,32,16,8},	//3:LEG4
	//
	{ enTexCEneH0202VorzoyWP,0,224,32,32,12,8},		//4:LEG5/WATER
};

gxSprite SprCEneH0202VorzoyWPLegOku[]={
	{ enTexCEneH0202VorzoyWP,0*32,2*32,32,32,16,8},	//0:LEG1/ノーマル
	{ enTexCEneH0202VorzoyWP,1*32,2*32,32,32,16,8},	//1:LEG2/アーマード
	{ enTexCEneH0202VorzoyWP,2*32,2*32,32,32,16,8},	//2:LEG3/シューター
	{ enTexCEneH0202VorzoyWP,3*32,2*32,32,32,16,8},	//3:LEG4/チキンレッグ
	//
	{ enTexCEneH0202VorzoyWP,0,224,32,32,12,8},		//4:LEG5/WATER
};

gxSprite SprCEneH0202VorzoyWPArm[]={
	{ enTexCEneH0202VorzoyWP,160,32*0,96,32  ,32,16},	//0:ARM0/ショートバレル
	{ enTexCEneH0202VorzoyWP,160,32*1,96,32  ,32,16},	//1:ARM0/ロングバレル
	{ enTexCEneH0202VorzoyWP,160,32*2,96,32  ,28,14},	//2:ARM0/ソリッドシューター
	{ enTexCEneH0202VorzoyWP,160,96  ,96,48  ,32,32},	//3:ARM0/ロケットキャノン
	{ enTexCEneH0202VorzoyWP,160,144 ,96,48  ,32,32},	//4:ARM0/メガ粒子砲
	{ enTexCEneH0202VorzoyWP,160,192 ,96,32  ,32,16},	//5:ARM0/メガ粒子砲（添え手）

	//水軍用
	{ enTexCEneH0202VorzoyWP,184,232,40,24  ,8,8},	//6:ショートバレル
};

enum {
	enBodyNormal,
	enBodySlaster,
	enBodyMini,

	enBodyChicken,
	enBodyWater,
};

enum {
	enArmShort,
	enArmLong,
	enArmSolid,
	enArmShoulder,
	enArmCannon,
	enArmCannonR,

	enArmShortWater,
};

enum {
	enLegNormal,
	enLegArmored,
	enLegPod,
	enLegChicken,
};

extern gxSprite SprCEneH0202VorzoyWP[];


CEneH0202VorzoyWP::CEneH0202VorzoyWP( Sint32 x, Sint32 y ,gxBool bDeadWait)
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_Base.x = x;
	m_Base.y = y;

	m_bDeadWait = bDeadWait;
	m_HitKurai.set_hp( enDefPointZakoM );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_LEFT;
	m_sAtribute = ATR_DFLT;

	m_sCrashTimer = 0;

	//-----------------------------------

	m_sJumpWait = 0;
	m_sPadControl = 0;
	m_sBackWait = 0;
	m_sLogicSeq = 0;
	m_sAtackWait = 60;
	m_sAtackLag = 0;
	m_sAtackCnt = 0;

	m_sLogicTimer = 0;
	m_sLogicPattern = 0;
	m_bDashMode  = gxFalse;

	m_Leynos.set_spr(EL_BASE ,NULL);

	m_Leynos.set_spr(EL_ARM_L,NULL);
	m_Leynos.set_spr(EL_BODY ,&SprCEneH0202VorzoyWPBody[0]);
	m_Leynos.set_spr(EL_ARM_R,&SprCEneH0202VorzoyWPArm[3]);
	m_Leynos.set_spr(EL_LEG_L,&SprCEneH0202VorzoyWPLegOku[1]);
	m_Leynos.set_spr(EL_LEG_R,&SprCEneH0202VorzoyWPLegTemae[2]);

	m_Leynos.parts[EL_BASE]->ofx = -3;
	m_Leynos.parts[EL_BASE]->ofy = -21;

	m_Leynos.parts[EL_BODY]->ofx = 2;
	m_Leynos.parts[EL_BODY]->ofy = -10;

	m_Leynos.parts[EL_ARM_R]->ofx = 4;
	m_Leynos.parts[EL_ARM_R]->ofy = -20;

	m_Leynos.parts[EL_ARM_L]->ofx = 4;
	m_Leynos.parts[EL_ARM_L]->ofy = -20;

	m_Leynos.parts[EL_LEG_L]->ofx = 4;
	m_Leynos.parts[EL_LEG_L]->ofy = -3;

	m_Leynos.parts[EL_LEG_R]->ofx = 4;
	m_Leynos.parts[EL_LEG_R]->ofy = -3;

	m_Leynos.set_pos( m_Pos.x , m_Pos.y , PRIO_ENEMY );


	m_Leynos.set_spd( 360 );
	m_Leynos.set_walkcycle(16);
	m_Leynos.walk_width_ = 120;
	m_Leynos.walk_height_ = 100;
//	pEleynos->cpu_control = gxFalse;
	m_Leynos.set_spdadd(8);
	m_Leynos.setFirePos(5,10);

	m_Add.x = 0;
	m_Add.y = 0;

	m_bNoneScrollOut = gxFalse;//gxTrue;

	SeqCustomize( 4,0,4);
	m_bDashMode = gxTrue;
	m_bHidden = gxTrue;
	m_bLandingMuteki = gxFalse;

	m_sHigh = 0;
}


CEneH0202VorzoyWP::~CEneH0202VorzoyWP()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneH0202VorzoyWP::SeqInit()
{
	//-----------------------------------------------------
	//初期化
	//-----------------------------------------------------
	SetRader( enTypeMarkerGroundObject );

	if( !m_bNoneScrollOut &&  IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}

	m_sBackWait = 8;

	if( GetTargetPlayer()->x > m_Pos.x )
	{
		m_sPadControl = JOY_R;
		m_Leynos.set_dir(DIR_RIGHT);
	}
	else
	{
		m_sPadControl = JOY_L;
		m_Leynos.set_dir(DIR_LEFT);
	}

	SetActionSeq( enActionSeqMain );
}

void CEneH0202VorzoyWP::SeqCustomize( Sint32 sBody ,Sint32 sArm , Sint32 sLeg)
{
	m_Leynos.set_spr(EL_BODY ,&SprCEneH0202VorzoyWPBody[sBody]);

	if( sArm == enArmCannon )
	{
		
		m_Leynos.set_spr(EL_ARM_R,&SprCEneH0202VorzoyWPArm[enArmCannonR]);
		m_Leynos.set_spr(EL_ARM_L,&SprCEneH0202VorzoyWPArm[enArmCannon]);
	}
	else
	{
		m_Leynos.set_spr(EL_ARM_R,&SprCEneH0202VorzoyWPArm[sArm]);
//		m_Leynos.set_spr(EL_ARM_L,&SprCEneH0202VorzoyWPArm[enArmCannonR]);
		m_Leynos.set_spr(EL_ARM_L,NULL);
	}

	m_Leynos.set_spr(EL_LEG_L,&SprCEneH0202VorzoyWPLegOku[sLeg]);
	m_Leynos.set_spr(EL_LEG_R,&SprCEneH0202VorzoyWPLegTemae[sLeg]);
}

void CEneH0202VorzoyWP::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------
	OBJ_POS_T Old = m_Pos;
	SetRader( enTypeMarkerGroundObject );

	m_Leynos.parts[EL_BASE]->ofy = -21+m_sHigh/100;

	if( m_HitKurai.is_dead() )
	{
		viiMus::PlaySound(enSoundExplosionSmall );
		viiEff::SetBombSimple( m_Pos.x , m_Pos.y - 3200 , 3 ,1 );
		viiEff::Set( EFF_RING , m_Pos.x , m_Pos.y-24*100 , argset(400) );

		//if( viiSub::Rand()%5 == 0 ) m_sCrashTimer = 70;
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( m_HitKurai.IsImpacted() && m_HitKurai.IsAtackType( ATK_TYPE_CANNON ) )
	{
		m_HitKurai.SetOffImpact();
	}

	LogicAI();

//	if( m_bDashMode )
//	{
//		m_Leynos.set_spd( 320 );
//		GetLeynos()->SetDashMode();
//	}
//	else
//	{
//		m_Leynos.set_spd( 180 );
//	}
	GetLeynos()->SetDashMode();
	m_Leynos.set_spdadd(6);
	m_Leynos.set_spd( 240 );
	m_Leynos.action();

	DirectionAdjust();

	m_Pos.x = m_Leynos.obj->pos.x;
	m_Pos.y = m_Leynos.obj->pos.y;

	if( m_HitKurai.is_damage() )
	{
		m_bHidden = gxFalse;
		if( m_sBackWait == 0) m_sBackWait = enBackTime;
		m_Leynos.set_damageflag();
	}

	//----------------------------------------
	//水関連のエフェクト
	//----------------------------------------
	if( pGame->GetMarineLine() && m_Pos.y >= pGame->GetMarineLine() )
	{
		//水流
		new CEffWaterStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , pGame->GetMarineLine(), 180 , 180 );
		new CEffWaterStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , pGame->GetMarineLine(), 180 , 180 );
		new CEffWaterStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , pGame->GetMarineLine(), 180 , 180 );
	}

	if( Old.y < pGame->GetMarineLine() )
	{
		//----------------------------------------
		//水に浸かった時
		//----------------------------------------
		if( m_Pos.y > pGame->GetMarineLine() )
		{
			//viiMus::PlaySound( enSoundSplashS );
			new CEffWater( m_Pos.x , m_Pos.y , enTypeSplashLarge , 0);

			for(Sint32 ii=0;ii<6;ii++)
			{
				CEffWaterSpark *p;
				p = new CEffWaterSpark( m_Pos.x+(-8+viiSub::Rand()%16)*700 , m_Pos.y-400 ,-90+(-15+viiSub::Rand()%30) , (50+viiSub::Rand()%100) );
				p->SetType(enSparkTypeFog);
			}

		}
	}
	else if( Old.y > pGame->GetMarineLine() )
	{
		//----------------------------------------
		//水から上がったとき
		//----------------------------------------
		if( m_Pos.y < pGame->GetMarineLine() )
		{
			//viiMus::PlaySound( enSoundSplashS );
			new CEffWater( m_Pos.x     , m_Pos.y, enTypeSplashPillar , 0);
			new CEffWater( m_Pos.x-800 , m_Pos.y, enTypeSplashPillar , 0);
			new CEffWater( m_Pos.x+800 , m_Pos.y, enTypeSplashPillar , 0);
			for(Sint32 ii=0;ii<5;ii++)
			{
				CEffWaterSpark *p;
				p = new CEffWaterSpark( m_Pos.x+(-8+viiSub::Rand()%16)*100 , m_Pos.y-2400 ,-90+(-15+viiSub::Rand()%30) , (50+viiSub::Rand()%100) );
				p->SetType(enSparkTypeFog);
			}
		}
	}

	//くらい判定登録
	if( !m_Leynos.IsCarry() )
	{
		if( !m_bHidden )
		{
			if( !m_bLandingMuteki )
			{
				m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );
			}
		}
	}


	if( m_bHidden )
	{
		m_sHigh += (3200-m_sHigh)/10;
	}
	else
	{
		m_sHigh += (0-m_sHigh)/10;
	}
}


void CEneH0202VorzoyWP::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	if( m_sAtackLag > 0 )
	{
		m_sAtackLag --;
	}

	switch(m_sLogicPattern){
	case enLogicNormal:
		LogicNormal();
		break;
	case enLogicWaraWara:
		//LogicWaraWara();
		break;
	case enLogicCarry:
		m_bHidden = gxFalse;
		m_bLandingMuteki = gxTrue;
		LogicCarryAtack();
		break;
	case enLogicFlying:
		m_bHidden = gxFalse;
		LogicFlyingAtack();
		break;
	}

	m_Leynos.set_control( m_sPadControl | BTN_L1 );

}


void CEneH0202VorzoyWP::LogicNormal()
{
	Sint32 vx = GetTargetPlayer()->x;
	Sint32 sRange = 320*100;
	Sint32 sMaxRange = sRange;

	if( m_sLogicSeq == 0 && m_sBackWait )
	{
		if( m_sBackWait == enBackTime )
		{
			if( vx < m_Pos.x )
			{
				m_sPadControl = JOY_R;
				m_Leynos.set_dir(DIR_RIGHT);
			}
			else
			{
				m_sPadControl = JOY_L;
				m_Leynos.set_dir(DIR_LEFT);
			}
		}

		if( m_Leynos.obj->is_landnow() )
		{
			m_bLandingMuteki = gxFalse;
			if(!m_bHidden)
			{
				new CEffWater( m_Pos.x , pGame->GetMarineLine() , enTypeSplashLarge , 0);
			}
			m_bHidden = gxTrue;
			m_sBackWait --;
		}

		m_sLogicSeq = 0;

		if( m_sBackWait == 0)
		{
			m_sPadControl = 0;
		}
	}
	else
	{
		Sint32 dst = gxAbs(vx-m_Pos.x);
		Sint32 vdir = pGame->GetHound()->GetDirection();
		Sint32 bHide = gxFalse;

		switch(m_sLogicSeq){
		case 0:
			//-------------------------------
			//攻撃範囲内に入るまで待つ
			//-------------------------------
			m_Leynos.arm_rot += (0-m_Leynos.arm_rot)/5;
			m_sLogicTimer = 0;
			m_sPadControl = 0;


			if( vx < m_Pos.x )
			{
				//ハウンドが右に居る
				if( vdir == DIR_LEFT )
				{
					//背後確定
					sRange = sRange/2;
					bHide = gxTrue;
				}
			}
			else
			{
				//ハウンドが左に居る
				if( vdir == DIR_RIGHT )
				{
					//背後確定
					sRange = 2*sRange/3;
					bHide = gxTrue;
				}
			}

			if( vx > m_Pos.x+sRange )
			{
				m_sPadControl = JOY_R;
				m_Leynos.set_dir( DIR_RIGHT );
			}
			else if( vx < m_Pos.x-sRange )
			{
				m_sPadControl = JOY_L;
				m_Leynos.set_dir( DIR_LEFT );
			}

			if( dst < sRange )
			{
				if( vx < m_Pos.x )
				{
					m_Leynos.set_dir( DIR_LEFT );
					m_sPadControl = JOY_R;
						m_Leynos.obj->mov.y = -800;
						m_sJumpWait = 25;
						m_sLogicSeq = 100;
				}
				else
				{
					m_Leynos.set_dir( DIR_RIGHT );
					m_sPadControl = JOY_L;
						m_Leynos.obj->mov.y = -800;
						m_sJumpWait = 25;
						m_sLogicSeq = 100;
				}

				
				if( dst < sRange/2 )
				{
					if( vx < m_Pos.x )
					{
						m_Leynos.set_dir( DIR_LEFT );
						m_sPadControl = JOY_R;
						//射程範囲内
						m_Leynos.obj->mov.y = -800;
						m_sJumpWait = 25;
						m_sLogicSeq = 100;
					}
					else
					{
						m_Leynos.set_dir( DIR_RIGHT );
						m_sPadControl = JOY_L;
						//射程範囲内
						m_Leynos.obj->mov.y = -800;
						m_sJumpWait = 25;
						m_sLogicSeq = 100;
					}
				}
				
			}
			break;

		case 100:
			//-------------------------------
			//垂直ジャンプ攻撃
			//-------------------------------
			m_bHidden = gxFalse;
			if(m_sJumpWait > 0)
			{
				m_sPadControl |= BTN_A;

				//----------------------------------
				//腕の角度をあわせる
				//----------------------------------
				m_Leynos.arm_rot += (45-m_Leynos.arm_rot)/10;

				m_sPadControl |= BTN_A;
				m_sJumpWait --;
			}
			else
			{
				m_sLogicSeq = 200;
			}
			break;

		case 200:
			//-------------------------------
			//着地するまで攻撃を行う
			//-------------------------------
			m_bHidden = gxFalse;
			m_sPadControl &= ~BTN_A;
			m_sJumpWait ++;
			if( m_sJumpWait < 30 && m_sJumpWait == 10)
			{
				Atack2();
			}
			if( m_Leynos.obj->is_landnow() )
			{
				m_sLogicSeq = 300;
			}
			break;

		case 300:
			m_bHidden = gxFalse;
			if( m_sLogicTimer < 20 )
			{
				m_sLogicTimer ++;
			}
			else
			{
				m_sBackWait = enBackTime;
				m_sLogicSeq = 0;
			}
			break;
		}

	}

}


void CEneH0202VorzoyWP::LogicWaraWara()
{

}


void CEneH0202VorzoyWP::LogicCarryAtack()
{
	//---------------------------------------------
	//キャリー攻撃
	//---------------------------------------------
//	Sint32 ax,ay,rot,spd;

	if( !m_Leynos.IsCarry() )
	{
		//切り離された
		m_sAtackCnt  = 0;
		m_sAtackLag  = 0;
		m_sAtackWait = 0;
		m_sLogicPattern = enLogicNormal;
		m_sBackWait = 30;//enBackTime/2;
		return;
	}

	if( m_sAtackWait > 0 )
	{
		m_sAtackWait --;
		return;
	}
	if( m_sAtackLag > 0 )
	{
		m_sAtackLag --;
		return;
	}

//	m_Leynos.setFirePos( 45 , 0 );	//武器発射位置設定
//	m_Leynos.getFirePos( ax , ay );
//
//	viiEff::Set( EFF_MAZZLE,ax,ay , NULL );
//
//	rot = viiSub::SetKakudo( m_Leynos.get_dir(),15 );
//	spd = 220+m_sAtackCnt*40;
//
//	viiMus::PlaySound(enSoundEneShotSmall);
//	new CEne0001EneBullet( ax , ay , rot     , spd );
//	new CEne0001EneBullet( ax , ay , rot -30 , spd );
//	new CEne0001EneBullet( ax , ay , rot +30 , spd );
//	m_sAtackLag = 8;
//
//	m_sAtackCnt ++;
//
//	if( m_sAtackCnt >= 6 ) 
//	{
//		m_sAtackCnt = 0;
//		m_sAtackWait = 220;
//	}
}


void CEneH0202VorzoyWP::LogicFlyingAtack()
{
	//-----------------------------------------------------
	//フライング攻撃
	//-----------------------------------------------------

	switch(m_sLogicSeq){
	case 0:		//出現
		SetNoneScrollOut();
		m_Leynos.atari_off();
		m_Add.x = 0;
		m_Add.y = -400;
		m_sLogicTimer = 0;
		m_sLogicSeq = 100;
		break;

	case 100:	//上昇
		if( m_Add.y < 0 )
		{
			m_Add.y += 2;
		}
		else
		{
			m_Add.y = 0;
			m_sLogicTimer = 0;
			m_sLogicSeq = 200;
		}
		break;

	case 200:	//待機
		if( m_sLogicTimer <= 30 )
		{

		}
		else
		{
			m_sLogicTimer = 0;
			m_sLogicSeq = 300;
		}
		break;

	case 300:	//急降下
		m_Base.x += (viiMath::Sin100( (m_sLogicTimer*1)%360 )*240)/50;
		m_Base.y += (viiMath::Cos100( (m_sLogicTimer*2)%360 )*320)/60;
		if(	GetTargetPlayer()->x > m_Base.x )
		{
			if( m_Add.x < 220 ) m_Add.x += 8;
		}
		else if(	GetTargetPlayer()->x < m_Base.x )
		{
			if( m_Add.x > -220 ) m_Add.x -= 8;
		}

		if( viiSub::GameCounter()%32 == 0 && m_Pos.y<GetTargetPlayer()->y-10000 )
		{
			int rot = get_valken_rot( m_Base.x , m_Base.y );

			new CEne0001EneBullet( m_Pos.x , m_Pos.y , rot+2 , 460 ,2 );
			new CEne0001EneBullet( m_Pos.x , m_Pos.y , rot-2 , 460 ,2 );

			new CEne0001EneBullet( m_Pos.x , m_Pos.y , rot , 400 ,0 );
			new CEne0001EneBullet( m_Pos.x , m_Pos.y , rot , 420 ,0 );
			new CEne0001EneBullet( m_Pos.x , m_Pos.y , rot , 480 ,0 );
		}
		break;

	}

	m_Base.x += m_Add.x;
	m_Base.y += m_Add.y;

	m_Base.y += (viiMath::Cos100( ( m_sTimer*2 )%360 )*1)/2;

	m_Leynos.set_pos( m_Base.x , m_Base.y , PRIO_ENEMY );
//	m_pEasyLeynos->set_dir(m_sDirection);

	m_Leynos.ResetGravity();
	m_sCrashTimer = 15;
	m_sLogicTimer ++;

}


void CEneH0202VorzoyWP::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

	Sint32 ax,ay,rot,spd;

	if( m_sAtackLag ) return;

	m_Leynos.setFirePos(30,10);	//武器発射位置設定
	m_Leynos.getFirePos(ax,ay);

	rot = (m_Leynos.get_dir() == DIR_RIGHT)? m_Leynos.arm_rot : (m_Leynos.arm_rot*-1+180);
	spd = 200;

	viiMus::PlaySound(enSoundEneShotSmall);

	if( m_sAtackCnt !=1 )
	{
		spd = 180;
		new CEne0001EneBullet( ax , ay , rot , spd );
	}

	viiEff::Set(EFF_MAZZLE,ax,ay-400,NULL);

	m_sAtackCnt ++;

	if( m_sAtackCnt >= 4 )
	{
		m_sAtackLag = 60;
		m_sAtackCnt = 0;
	}
}


void CEneH0202VorzoyWP::Atack2()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

	Sint32 ax,ay,rot,spd;

//	if( m_sAtackLag ) return;

	m_Leynos.setFirePos(30,10);	//武器発射位置設定
	m_Leynos.getFirePos(ax,ay);

	rot = (m_Leynos.get_dir() == DIR_RIGHT)? m_Leynos.arm_rot : (m_Leynos.arm_rot*-1+180);
	spd = 200;

	viiMus::PlaySound(enSoundEneShotLaser);

	viiEff::Set(EFF_MAZZLE,ax,ay-400,NULL);

	new CEne0001EneBullet( ax , ay, rot , 240+80*1 ,0);
	new CEne0001EneBullet( ax , ay, rot , 240+80*2 ,0);
	new CEne0001EneBullet( ax , ay, rot , 240+80*3 ,0);

/*
	CAtkH0001Ryudan *p;
	p = new CAtkH0001Ryudan( ax , ay , -90, 80 );
	p->SetForce( CAtkH0001Missile::enForceID_Enemy );
	p->SetMoveWait( 0 );
*/

/*
	CAtkH0002GMissile *p;
	p = new CAtkH0002GMissile(ax,ay , (m_Leynos.get_dir() == DIR_RIGHT)? 0 : 180 , 30);
	p->SetForce( CAtkH0001Missile::enForceID_Enemy );
*/

}


void CEneH0202VorzoyWP::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	m_bHidden = gxFalse;
	m_bDead = gxTrue;

	if( m_bDeadWait )
	{
		//破壊判定待ち
		if( !m_bClear ) return;
	}

	m_Leynos.action();
	m_Pos.x = m_Leynos.obj->pos.x;
	m_Pos.y = m_Leynos.obj->pos.y;

	m_sCrashTimer ++;

	if(m_sCrashTimer==30 && m_Leynos.obj->is_landnow() )
	{
		new CEffBombSmoke(m_Pos.x , m_Pos.y , -90 , 120  );
		viiMus::PlaySound( enSoundEneShotBiriBiri );
	}

	if(m_sCrashTimer%10 == 0  && m_Leynos.obj->is_landnow() && m_sCrashTimer <= 40)
	{
		//ビリビリ
		viiEff::Set( EFF_BREAK_THUNDER , m_Pos.x+(-8+viiSub::Rand()%16)*100 , m_Pos.y+(-48+viiSub::Rand()%32)*100 );
	}

	if( m_sCrashTimer <= 75 )
	{
		if( m_HitKurai.is_damage() )
		{
			m_Leynos.set_damageflag();
		}
		m_HitKurai.set_hp( enDefPointZakoM );
		m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );
		return;
	}

	//---------------------------------
	//---------------------------------


	if( viiSub::Rand()%4 )
	{
		//敵が逃げ出す

		CSoldierBase *p = NULL;
		p = new CSoldierBase( );

		p->SetPosition( 0 , m_Pos.x  , m_Pos.y-16*100 , NULL);
		p->SetForce(enForceEnemy);
		p->Command( enSoldierCommandFuttobi );

		if( m_bDeadWait )
		{
			//ステージ３わらわら専用
			p->SetTargetPosition( 0 );	//左方向に逃げる
		}
	}

	if( pGame->GetMarineLine() && m_Leynos.obj->is_landnow() )
	{
		//水中であれば

		m_Leynos.m_bDead = gxTrue;
		EffectMakeBigBomb(m_Pos.x , m_Pos.y);
		MakeSplash( m_Pos.x , m_Pos.y );
		viiMus::PlaySound( enSoundSplashM );
	}
	else
	{
		//水中でなければ

		viiMus::PlaySound( enSoundExplosionMiddle );

		EffectMakeBigBomb(m_Pos.x , m_Pos.y);
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

		m_Leynos.m_bDead = gxTrue;

		CEffectDmgprt *pDmg;
		pDmg = new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+15*1 , 300 );
		pDmg->SetSprite( m_Leynos.parts[EL_BODY]->spr );

		pDmg = new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+90-15 , 250 );
		pDmg->SetSprite( m_Leynos.parts[EL_LEG_R]->spr );

		pDmg = new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+90+25 ,300 );
		pDmg->SetSprite( m_Leynos.parts[EL_LEG_L]->spr );

		pDmg = new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+165 , 450 );
		pDmg->SetSprite( m_Leynos.parts[EL_ARM_R]->spr );
	}

	ScorePlus( enScore );

	SetActionSeq( enActionSeqEnd );

}


void CEneH0202VorzoyWP::Draw()
{
//	if( m_bHidden ) return;

	m_Leynos.Draw();

//	viiDraw::Sprdmg( m_HitKurai.is_damage() , &SprCEneH0202VorzoyWP[ 0 ] , m_Pos.x , m_Pos.y,PRIO_ENEMY , m_sAtribute|m_sAtributeDir , ARGB_DFLT );

}

