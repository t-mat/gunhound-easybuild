//--------------------------------------------------------------------------------
//
// Stage00:ダミーハウンド
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../Effect/EffectBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -30,
	enKuraiTop   = -34,
	enKuraiRight =  30,
	enKuraiBottom=  0,

	enScore = 300,

	enBackTime = 60,

};


gxSprite SprCEneH0408Powerdsuit[]=
{
	{ enTexPageDmyHound,0*32,0*32,32,32,16,24},	//0:BODY
	{ enTexPageDmyHound,192,160,32,32,16,8},	//1:LEG奥
	{ enTexPageDmyHound,160,160,32,32,16,8},	//2:LEG手前
	{ enTexPageDmyHound,16,192,64,24,16,8},		//3:ARM
//	{ enTexPageDmyHound,192,128,32,32,6,10},	//3:ARM

	{ enTexPageDmyHound,0,192-8,16,72,8,40},		//4:DIVER ANTENNA
};

gxSprite SprCEneH0408PowerdsuitBody[]={
	{ enTexPageDmyHound,0*32,0*32,32,32,16,24},	//0:BODY0/ノーマル
	{ enTexPageDmyHound,1*32,0*32,32,32,16,24},	//1:BODY1/スラスター付
	{ enTexPageDmyHound,1*32,0*32,32,32,16,24},	//1:BODY2/シューター付
	{ enTexPageDmyHound,2*32,0*32,32,32,16,24},	//2:BODY3/軽量
};

gxSprite SprCEneH0408PowerdsuitLegTemae[]={
	{ enTexPageDmyHound,0*32,1*32,32,32,16,8},	//0:LEG1
	{ enTexPageDmyHound,1*32,1*32,32,32,16,8},	//1:LEG2
	{ enTexPageDmyHound,2*32,1*32,32,32,16,8},	//2:LEG3
	{ enTexPageDmyHound,3*32,1*32,32,32,16,8},	//3:LEG4
};

gxSprite SprCEneH0408PowerdsuitLegOku[]={
	{ enTexPageDmyHound,0*32,2*32,32,32,16,8},	//0:LEG1/ノーマル
	{ enTexPageDmyHound,1*32,2*32,32,32,16,8},	//1:LEG2/アーマード
	{ enTexPageDmyHound,2*32,2*32,32,32,16,8},	//2:LEG3/シューター
	{ enTexPageDmyHound,3*32,2*32,32,32,16,8},	//3:LEG4/チキンレッグ
};

gxSprite SprCEneH0408PowerdsuitArm[]={
	{ enTexPageDmyHound,16,192,64,24,16,8},		//3:ARM
	{ enTexPageDmyHound,16,192,64,24,16,8},		//3:ARM
	{ enTexPageDmyHound,16,192,64,24,16,8},		//3:ARM
	{ enTexPageDmyHound,16,192,64,24,16,8},		//3:ARM
	{ enTexPageDmyHound,16,192,64,24,16,8},		//3:ARM
	{ enTexPageDmyHound,16,192,64,24,16,8},		//3:ARM

	{ enTexPageDmyHound,160,32*0,96,32  ,32,16},	//0:ARM0/ショートバレル
	{ enTexPageDmyHound,160,32*1,96,32  ,32,16},	//1:ARM0/ロングバレル
	{ enTexPageDmyHound,160,32*2,96,32  ,28,14},	//2:ARM0/ソリッドシューター
	{ enTexPageDmyHound,160,96  ,96,48  ,32,32},	//3:ARM0/ロケットキャノン
	{ enTexPageDmyHound,160,144 ,96,48  ,32,32},	//4:ARM0/メガ粒子砲
	{ enTexPageDmyHound,160,192 ,96,32  ,32,16},	//5:ARM0/メガ粒子砲（添え手）
};

enum {
	enBodyNormal,
	enBodySlaster,
	enBodyMini,
};
enum {
	enArmShort,
	enArmLong,
	enArmSolid,
	enArmShoulder,
	enArmCannon,
	enArmCannonR,
};

enum {
	enLegNormal,
	enLegArmored,
	enLegPod,
	enLegChicken,
};


CEneH0408Powerdsuit::CEneH0408Powerdsuit( Sint32 x, Sint32 y ,gxBool bDeadWait)
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_BG2-1;

	m_bDeadWait = bDeadWait;
	m_HitKurai.set_hp( 1000 );

	m_bLeader = gxFalse;

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_LEFT;
	m_sAtribute = ATR_DFLT;

	//-----------------------------------

	m_sJumpWait = 0;
	m_sPadControl = 0;
	m_sBackWait = 0;
	m_sLogicSeq = 0;
	m_sAtackWait = 60;
	m_sAtackLag = 0;
	m_sAtackCnt = 0;

	m_sAtackSeq = 0;
	m_sAtackTimer = viiSub::Rand()%128;

	m_sLogicPattern = enLogicPrev;

	m_pTgt = NULL;
	m_Offset.x = 0;
	m_Offset.y = 0;

	m_Offset2 = m_Offset;

	m_bDashMode  = gxFalse;
	m_bRealAtack = gxFalse;

	m_sArmRotRange = -50;

	m_pAtkTgt = NULL;

	m_Leynos.set_pos( m_Pos.x , m_Pos.y , m_Pos.z );
}


CEneH0408Powerdsuit::~CEneH0408Powerdsuit()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneH0408Powerdsuit::SeqInit()
{
	//-----------------------------------------------------
	//初期化
	//-----------------------------------------------------
	m_Leynos.set_spr(EL_BASE ,NULL);

	m_Leynos.set_spr(EL_ARM_L,&SprCEneH0408Powerdsuit[4]);
	m_Leynos.set_spr(EL_BODY ,&SprCEneH0408PowerdsuitBody[0]);
	m_Leynos.set_spr(EL_ARM_R,&SprCEneH0408PowerdsuitArm[3]);
	m_Leynos.set_spr(EL_LEG_L,&SprCEneH0408PowerdsuitLegOku[1]);
	m_Leynos.set_spr(EL_LEG_R,&SprCEneH0408PowerdsuitLegTemae[2]);

	m_Leynos.parts[EL_BASE]->ofx = -3;
	m_Leynos.parts[EL_BASE]->ofy = -18;

	m_Leynos.parts[EL_BODY]->ofx = 2;
	m_Leynos.parts[EL_BODY]->ofy = -6;

	m_Leynos.parts[EL_ARM_L]->ofx = -8;
	m_Leynos.parts[EL_ARM_L]->ofy = -18;

	m_Leynos.parts[EL_ARM_R]->ofx = 0;
	m_Leynos.parts[EL_ARM_R]->ofy = -22;

	m_Leynos.parts[EL_LEG_L]->ofx = 4;
	m_Leynos.parts[EL_LEG_L]->ofy = -3;

	m_Leynos.parts[EL_LEG_R]->ofx = 4;
	m_Leynos.parts[EL_LEG_R]->ofy = -3;

	m_Leynos.set_spd( 120 );//115 );
	m_Leynos.set_walkcycle(35);
	m_Leynos.walk_width_ = 140;
	m_Leynos.walk_height_ = 100;
//	pEleynos->cpu_control = gxFalse;
	m_Leynos.set_spdadd(8);//50);
	m_Leynos.setFirePos(48,0);	//武器発射位置設定

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

	SeqCustomize(3,0,2);

	SetActionSeq( enActionSeqMain );
}

void CEneH0408Powerdsuit::SeqCustomize( Sint32 sBody ,Sint32 sArm , Sint32 sLeg)
{
	//-----------------------------------------------------
	//カスタマイズ
	//-----------------------------------------------------

	m_Leynos.set_spr(EL_BODY ,&SprCEneH0408PowerdsuitBody[sBody]);

	if( sArm == enArmCannon )
	{
		m_Leynos.set_spr(EL_ARM_R,&SprCEneH0408PowerdsuitArm[enArmCannonR]);
		m_Leynos.set_spr(EL_ARM_L,&SprCEneH0408PowerdsuitArm[enArmCannon]);
	}
	else
	{
		m_Leynos.set_spr(EL_ARM_R,&SprCEneH0408PowerdsuitArm[sArm]);
//		m_Leynos.set_spr(EL_ARM_L,&SprCEneH0408PowerdsuitArm[enArmCannonR]);
		if( !m_bLeader ) m_Leynos.set_spr(EL_ARM_L,NULL);
	}

	m_Leynos.set_spr(EL_LEG_L,&SprCEneH0408PowerdsuitLegOku[sLeg]);
	m_Leynos.set_spr(EL_LEG_R,&SprCEneH0408PowerdsuitLegTemae[sLeg]);
}

void CEneH0408Powerdsuit::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------
	m_Old.x = m_Pos.x;
	m_Old.y = m_Pos.y;

	if( m_HitKurai.is_dead() )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	LogicAI();

	m_Leynos.action();

	if( m_Leynos.GetShockCnt() == 100 )
	{
		viiMus::PlaySound( enSoundLanding );
	}

//	if( m_Leynos.obj->is_landnow() )
//	{
//		m_Leynos.obj->mov.y = -20;
//	}
//	else
//	{
//
//	}

	SetRader( enTypeMarkerAssault );
	DirectionAdjust();

	m_Pos.x = m_Leynos.obj->pos.x;
	m_Pos.y = m_Leynos.obj->pos.y;

	if( m_HitKurai.is_damage() )
	{
		if( m_sBackWait == 0) m_sBackWait = enBackTime;
		m_Leynos.set_damageflag();
	}

	GimmickWaterEffect();

	//くらい判定登録
/*
	if( !m_Leynos.IsCarry() )
	{
		m_HitKurai.SetHantei(ID_VERSIS_DEF , &m_Pos );
	}
*/

}

void CEneH0408Powerdsuit::GimmickWaterEffect()
{
	if( !pGame->GetMarineLine() )	return;

	if( m_Old.y < pGame->GetMarineLine() )
	{
		//水に浸かった時
		if( m_Pos.y > pGame->GetMarineLine() )
		{
			new CEffWater( m_Pos.x , m_Pos.y , enTypeSplashLarge , 0);
		}
	}
	else if( m_Old.y > pGame->GetMarineLine() )
	{
		//水から上がったとき
		if( m_Pos.y < pGame->GetMarineLine() )
		{
			new CEffWater( m_Pos.x     , m_Pos.y, enTypeSplashPillar , 0);
			new CEffWater( m_Pos.x-800 , m_Pos.y, enTypeSplashPillar , 0);
			new CEffWater( m_Pos.x+800 , m_Pos.y, enTypeSplashPillar , 0);
		}
	}

	if( m_Pos.y >= pGame->GetMarineLine() )
	{
		if( m_bDashMode && viiSub::GameCounter()%4==0)
		{
			if( m_Leynos.obj->mov.x < 0)
			{
				new CEffWater( m_Pos.x , m_Pos.y-400 , enTypeSplashWave , 0, DIR_RIGHT );
			}
			else
			{
				new CEffWater( m_Pos.x , m_Pos.y-400 , enTypeSplashWave , 0, DIR_LEFT );
			}

		}
		else
		{
			//水流
			new CEffWaterStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , pGame->GetMarineLine(), 180 , 180 );
			new CEffWaterStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , pGame->GetMarineLine(), 180 , 180 );
			new CEffWaterStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , pGame->GetMarineLine(), 180 , 180 );
		}
	}

//		if( pGame->GetMarineLine() && m_Pos.y >= pGame->GetMarineLine() )
//		{
//			//水流
//			new CEffWaterStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , pGame->GetMarineLine(), 180 , 180 );
//			new CEffWaterStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , pGame->GetMarineLine(), 180 , 180 );
//			new CEffWaterStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , pGame->GetMarineLine(), 180 , 180 );
//		}

}

void CEneH0408Powerdsuit::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	if( m_sAtackLag > 0 ) m_sAtackLag --;


	//m_sPadControl = Joy[0].psh;

	switch(m_sLogicPattern){
	case enLogicPrev:
		LogicNormal();
		break;
	case enLogicAssault:
		LogicAssault();
		break;
	case enLogicBattleShip:
		LogicBattleShip();
		break;
	case enLogicDefence:
		LogicDefence();
		break;
	case enLogicAtackTarget:
		LogicAtack();
		break;
	}

	if( m_bDashMode )
	{
		m_Leynos.set_spd( 320 );
		m_Leynos.SetDashMode();
	}
	else
	{
		m_Leynos.SetDashMode(gxFalse);
		m_Leynos.set_spd( 115 );
	}

	m_Leynos.set_control( m_sPadControl );

}


void CEneH0408Powerdsuit::LogicNormal()
{
	//-----------------------------------------------------
	//ディフェンス
	//-----------------------------------------------------

	if( m_pTgt )
	{
		Sint32 tx = m_pTgt->x+m_Offset.x;

		if( tx < m_Pos.x-32*100 )
		{
			m_sPadControl = JOY_L;
		}
		else if( tx > m_Pos.x+32*100 )
		{
			m_sPadControl = JOY_R;
		}
		else
		{
			m_sPadControl = 0;
		}
	}



}


void CEneH0408Powerdsuit::LogicAssault()
{
	//-----------------------------------------------------
	//オフェンス
	//-----------------------------------------------------
#ifdef _VII_DEBUG_
	m_sPadControl = Joy[0].psh;
#endif

	if( m_sPadControl&BTN_X )
	{
		Atack();
	}

}


void CEneH0408Powerdsuit::LogicBattleShip()
{
	switch( m_sAtackSeq ){
	case 0:	//位置取り

		break;
	case 100:	//攻撃してるフリ

		break;
	case 200:	//攻撃してるフリ

		break;
	}

	//足元に攻撃を食らう

	if( m_pTgt )
	{
		Sint32 tx = m_pTgt->x+m_Offset.x;

		if( tx < m_Pos.x-32*100 )
		{
			m_sPadControl = JOY_L|BTN_L1;
		}
		else if( tx > m_Pos.x+32*100 )
		{
			m_sPadControl = JOY_R|BTN_L1;
		}
		else
		{
			m_sPadControl = 0;
		}

		if( 5000*100 > m_Pos.x )
		{
			m_Leynos.set_dir(DIR_RIGHT);
		}
		else
		{
			m_Leynos.set_dir(DIR_LEFT);
		}

	}
	if( m_sAtackTimer%60==0)
	{
		m_Offset.x = (-128+viiSub::Rand()%256)*800;
	}

	if( m_sAtackTimer%160 < 80)
	{
		m_Leynos.arm_rot += (-50-m_Leynos.arm_rot)/10;	
		if( m_sAtackTimer%4==0)
		{
			Sint32 ax,ay;
			m_Leynos.setFirePos(48,0);	//武器発射位置設定
			m_Leynos.getFirePos(ax,ay);
			m_Leynos.set_knockback(10);
			viiEff::Set( EFF_MAZZLE , ax , ay , NULL );
		}
	}
	else
	{
		m_Leynos.arm_rot += (5-m_Leynos.arm_rot)/10;	
	}


	if( m_sAtackTimer%12==0)
	{
		switch( viiSub::Rand()%12 ){
		case 0:
			viiEff::SetBombDeluxe( m_Pos.x+(-64+(viiSub::Rand()%128))*100 , m_Pos.y , 1 , 1 );
			break;

		case 2:
			CEneH0305Missile( m_Pos.x+(-64+(viiSub::Rand()%128))*100 , m_Pos.y , m_Pos.z);
			break;
		default:
			viiEff::Set( EFF_HITFLASH , m_Pos.x , m_Pos.y-(viiSub::Rand()%16)*100 , argset(1) );
			break;
		}
	}

	m_sAtackTimer ++;
	
}


void CEneH0408Powerdsuit::LogicDefence()
{
	switch( m_sAtackSeq ){
	case 0:	//位置取り

		break;
	case 100:	//攻撃してるフリ

		break;
	case 200:	//攻撃してるフリ

		break;
	}

	//足元に攻撃を食らう

	if( m_pTgt )
	{
		Sint32 tx = m_pTgt->x+m_Offset.x;

		if( tx < m_Pos.x-32*100 )
		{
			m_sPadControl = JOY_L|BTN_L1;
		}
		else if( tx > m_Pos.x+32*100 )
		{
			m_sPadControl = JOY_R|BTN_L1;
		}
		else
		{
			m_sPadControl = 0;
		}

		if( 5000*100 > m_Pos.x )
		{
			m_Leynos.set_dir(DIR_RIGHT);
		}
		else
		{
			m_Leynos.set_dir(DIR_LEFT);
		}

	}
	if( m_sAtackTimer%60==0)
	{
		m_Offset.x = (-128+viiSub::Rand()%256)*100;
	}

	if( m_sAtackTimer%160 < 80)
	{
		Sint32 sAdd = (m_sArmRotRange-m_Leynos.arm_rot)/10;
		m_Leynos.arm_rot += sAdd;
		if( m_sAtackTimer%4==0 && sAdd== 0)
		{
			Sint32 ax,ay;
			m_Leynos.setFirePos(48,6);	//武器発射位置設定
			m_Leynos.getFirePos(ax,ay);
			m_Leynos.set_knockback(10);
			viiEff::Set( EFF_MAZZLE , ax , ay , NULL );
			if( m_bRealAtack )
			{
				GattlingAtack();
			}
		}
	}
	else
	{
		m_Leynos.arm_rot += (5-m_Leynos.arm_rot)/10;	
	}

	m_sAtackTimer ++;
	
}


void CEneH0408Powerdsuit::LogicAtack()
{
	//-----------------------------------------------------
	//攻撃対象を攻撃する
	//-----------------------------------------------------
	Sint32 vx,vy,mx,my,tx = 0;
	Sint32 ax,ay;

	if( m_pAtkTgt == NULL )
	{
		LogicNormal();
		return;
	}
	else
	{
		if( m_sAtackTimer%180==0)
		{
			m_Offset2.x = m_Offset.x+(-128+viiSub::Rand()%256)*100;
		}
	}

	m_Leynos.getFirePos( ax , ay );

	vx = m_pAtkTgt->x+m_Offset2.x;
	vy = m_pAtkTgt->y+m_Offset2.y;

	mx = m_Pos.x ;
	my = m_Pos.y ;

	//方向を正す

	if( vx < mx )
	{
		//左を向く
		m_Leynos.set_dir(DIR_LEFT);
		tx = vx+200*100;
	}
	else if( vx > mx )
	{
		//右を向く
		m_Leynos.set_dir(DIR_RIGHT);
		tx = vx-200*100;
	}

	//-----------------------------------
	//ターゲットポジションへ移動
	//-----------------------------------

	if( tx < m_Pos.x-32*100 )
	{
		m_sPadControl = JOY_L|BTN_L1;
	}
	else if( tx > m_Pos.x+32*100 )
	{
		m_sPadControl = JOY_R|BTN_L1;
	}
	else
	{
		m_sPadControl = 0;
	}
	

	//腕の角度を調整
	AdjustArmAngle(  vx-ax  , vy-ay-32 );

	if( m_bRealAtack )
	{
		if( m_sAtackTimer%80 < 60 &&  m_sAtackTimer%4==0)
		{
			GattlingAtack();
		}
	}
	m_sAtackTimer ++;

}

void CEneH0408Powerdsuit::GattlingAtack()
{
	//------------------------------------------------
	//ガトリング攻撃
	//------------------------------------------------
	Sint32 ax,ay;

	m_Leynos.getFirePos( ax , ay );

	viiEff::Set(EFF_MAZZLE,ax,ay,NULL);
	new CEffGattlingGun( ax , ay , viiSub::SetKakudo(m_Leynos.get_dir() , m_Leynos.arm_rot+( -4+viiSub::Rand()%8)*3 ) , 1050 );

}

void CEneH0408Powerdsuit::AdjustArmAngle( Sint32 ax , Sint32 ay )
{
	//------------------------------------------------
	//腕の角度をあわせる
	//------------------------------------------------

	Sint32 r = viiMath::Atan2_100(ay,ax)/100;
	while(r<0)
	{
		r+=360;
	}
	if(r >90 && r<270)
	{
		r = 90-(r-90);
	}

	if(r >270)	r -= 360;

//	r = (m_Leynos.get_dir() == DIR_RIGHT)? r : -(180+r);

	Sint32 sKaku = 0;

	sKaku = viiMath::GetTargetAngle( m_Leynos.arm_rot , r );


	if( gxAbs( sKaku/10 ) == 0 )
	{
		sKaku = 0;
		if( sKaku < -1 ) sKaku --;
		if( sKaku > +1 ) sKaku ++;
	}
	else
	{
		sKaku = sKaku/10;
	}

	m_Leynos.arm_rot += sKaku;

}


void CEneH0408Powerdsuit::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

	Sint32 ax,ay,rot,spd;

	if( m_sAtackLag ) return;

	m_Leynos.setFirePos(56,2);	//武器発射位置設定
	m_Leynos.getFirePos(ax,ay);

	rot = (m_Leynos.get_dir() == DIR_RIGHT)? m_Leynos.arm_rot : (m_Leynos.arm_rot*-1+180);
	spd = 200;

//	viiMus::PlaySound(enSoundShotErase);

	if( m_sAtackCnt !=1 )
	{
//		new CEne0001EneBullet( ax , ay , rot , spd );
	}

	viiEff::Set(EFF_MAZZLE,ax,ay-400,NULL);
	m_Leynos.set_knockback(10);

	if( viiSub::Rand()%8==0)
	{
		m_Leynos.setFirePos(16,2);	//武器発射位置設定
		m_Leynos.getFirePos(ax,ay);
		viiEff::Set(EFF_YAKKYOU , ax , ay ,argset(	(m_Leynos.get_dir()==DIR_LEFT)? -65 : 65*1+180 ) );
	}

	m_sAtackCnt ++;

	if( m_sAtackCnt >= 4 )
	{
		m_sAtackLag = 6;
		m_sAtackCnt = 0;
	}
}


void CEneH0408Powerdsuit::Atack2()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

//	Sint32 ax,ay,rot,spd;
//
//	if( m_sAtackLag ) return;
//
//	m_Leynos.setFirePos(30,10);	//武器発射位置設定
//	m_Leynos.getFirePos(ax,ay);
//
//	rot = (m_Leynos.get_dir() == DIR_RIGHT)? m_Leynos.arm_rot : (m_Leynos.arm_rot*-1+180);
//	spd = 200;
//
//	viiMus::PlaySound(enSoundShotErase);
//
//	if( m_sAtackCnt !=1 )
//	{
//		spd = 180;
//		new CDanmakuRoot( CDanmakuRoot::enDanmakuGattling , ax , ay, rot , spd );
////		new CEne0001EneBullet( ax , ay , rot , spd );
//	}
//
//	viiEff::Set(EFF_MAZZLE,ax,ay-400,NULL);
//
//	m_sAtackCnt ++;
//
//	if( m_sAtackCnt >= 4 )
//	{
//		m_sAtackLag = 10;
//		m_sAtackCnt = 0;
//	}
}


void CEneH0408Powerdsuit::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	m_bDead = gxTrue;

	if( m_bDeadWait )
	{
		//破壊判定待ち
		if(!m_bClear) return;
	}

	CSoldierBase *p = new CSoldierBase( );

	p->SetPosition( 0 , m_Pos.x  , m_Pos.y-16*100 , NULL);
	p->SetForce(enForceEnemy);
	p->Command( enSoldierCommandFuttobi );
	if( m_bDeadWait )
	{
		//ステージ３わらわら専用
		p->SetTargetPosition( 0 );	//左方向に逃げる
	}
//	p->SetTargetPosition(9600*100);
//	p->ChangeMoveType( enMoveTypeFlyDown );
//	p->SetFuttobi(120);
//	p->SetEnd();


	viiMus::PlaySound(enSoundExplosionMiddle);

	ScorePlus( enScore );

	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

	m_Leynos.m_bDead = gxTrue;

	m_Leynos.action();

	SetActionSeq( enActionSeqEnd );


}

void CEneH0408Powerdsuit::Draw()
{

	m_Leynos.Draw();
//	viiDraw::Sprdmg( m_HitKurai.is_damage() , &SprCEneH0408Powerdsuit[ 0 ] , m_Pos.x , m_Pos.y,PRIO_ENEMY , m_sAtribute|m_sAtributeDir , ARGB_DFLT );

}

