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

	enBackTime = 60,

//	enDamageOffsetY = 400,
};

/*
gxSprite SprCEneH0003Vorzoy[]=
{
	{enTexPageEnemyCommon01,0,192,64,64,32,64},//ボルゾイ
	{TPG_LEYNOS01,0,192,64,64,32,64},//ボルゾイ
};
*/

gxSprite SprCEneH0003Vorzoy[]=
{
	{ enTexCEneH0003Vorzoy,0*32,0*32,32,32,16,24},	//0:BODY
	{ enTexCEneH0003Vorzoy,192,160,32,32,16,8},	//1:LEG奥
	{ enTexCEneH0003Vorzoy,160,160,32,32,16,8},	//2:LEG手前
	{ enTexCEneH0003Vorzoy,192,128,32,32,6,10},	//3:ARM
};

gxSprite SprCEneH0003VorzoyBody[]={
	{ enTexCEneH0003Vorzoy,0*32,0*32,32,32,16,24},	//0:BODY0/ノーマル
	{ enTexCEneH0003Vorzoy,1*32,0*32,32,32,16,24},	//1:BODY1/スラスター付
	{ enTexCEneH0003Vorzoy,1*32,0*32,32,32,16,24},	//1:BODY2/シューター付
	{ enTexCEneH0003Vorzoy,2*32,0*32,32,32,16,24},	//2:BODY3/軽量

	{ enTexCEneH0003Vorzoy,0,192,32,32,16,24},		//2:BODY4/水中用
};

gxSprite SprCEneH0003VorzoyLegTemae[]={
	{ enTexCEneH0003Vorzoy,0*32,1*32,32,32,16,8},	//0:LEG1
	{ enTexCEneH0003Vorzoy,1*32,1*32,32,32,16,8},	//1:LEG2
	{ enTexCEneH0003Vorzoy,2*32,1*32,32,32,16,8},	//2:LEG3
	{ enTexCEneH0003Vorzoy,3*32,1*32,32,32,16,8},	//3:LEG4
	//
	{ enTexCEneH0003Vorzoy,0,224,32,32,12,8},		//4:LEG5/WATER
};

gxSprite SprCEneH0003VorzoyLegOku[]={
	{ enTexCEneH0003Vorzoy,0*32,2*32,32,32,16,8},	//0:LEG1/ノーマル
	{ enTexCEneH0003Vorzoy,1*32,2*32,32,32,16,8},	//1:LEG2/アーマード
	{ enTexCEneH0003Vorzoy,2*32,2*32,32,32,16,8},	//2:LEG3/シューター
	{ enTexCEneH0003Vorzoy,3*32,2*32,32,32,16,8},	//3:LEG4/チキンレッグ
	//
	{ enTexCEneH0003Vorzoy,0,224,32,32,12,8},		//4:LEG5/WATER
};

gxSprite SprCEneH0003VorzoyArm[]={
	{ enTexCEneH0003Vorzoy,160,32*0,96,32  ,32,16},	//0:ARM0/ショートバレル
	{ enTexCEneH0003Vorzoy,160,32*1,96,32  ,32,16},	//1:ARM0/ロングバレル
	{ enTexCEneH0003Vorzoy,160,32*2,96,32  ,28,14},	//2:ARM0/ソリッドシューター
	{ enTexCEneH0003Vorzoy,160,96  ,96,48  ,32,32},	//3:ARM0/ロケットキャノン
	{ enTexCEneH0003Vorzoy,160,144 ,96,48  ,32,32},	//4:ARM0/メガ粒子砲
	{ enTexCEneH0003Vorzoy,160,192 ,96,32  ,32,16},	//5:ARM0/メガ粒子砲（添え手）

	//水軍用
	{ enTexCEneH0003Vorzoy,184,232,40,24  ,8,8},	//6:ショートバレル
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

extern gxSprite SprCEneH0003Vorzoy[];


CEneH0003Vorzoy::CEneH0003Vorzoy( Sint32 x, Sint32 y ,gxBool bDeadWait)
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
	m_Leynos.set_spr(EL_BODY ,&SprCEneH0003VorzoyBody[0]);
	m_Leynos.set_spr(EL_ARM_R,&SprCEneH0003VorzoyArm[3]);
	m_Leynos.set_spr(EL_LEG_L,&SprCEneH0003VorzoyLegOku[1]);
	m_Leynos.set_spr(EL_LEG_R,&SprCEneH0003VorzoyLegTemae[2]);

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


	m_Leynos.set_spd( 180+viiSub::Rand()%80 );
	m_Leynos.set_walkcycle(16);
	m_Leynos.walk_width_ = 120;
	m_Leynos.walk_height_ = 100;
//	pEleynos->cpu_control = gxFalse;
	m_Leynos.set_spdadd(50);
	m_Leynos.setFirePos(5,10);

	m_Add.x = 0;
	m_Add.y = 0;

	m_bNoneScrollOut = gxFalse;

//	m_sDamgeWait = 0;

	SeqCustomize( 2,1,3);

}


CEneH0003Vorzoy::~CEneH0003Vorzoy()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneH0003Vorzoy::SeqInit()
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

void CEneH0003Vorzoy::SeqCustomize( Sint32 sBody ,Sint32 sArm , Sint32 sLeg)
{
	m_Leynos.set_spr(EL_BODY ,&SprCEneH0003VorzoyBody[sBody]);

	if( sArm == enArmCannon )
	{
		
		m_Leynos.set_spr(EL_ARM_R,&SprCEneH0003VorzoyArm[enArmCannonR]);
		m_Leynos.set_spr(EL_ARM_L,&SprCEneH0003VorzoyArm[enArmCannon]);
	}
	else
	{
		m_Leynos.set_spr(EL_ARM_R,&SprCEneH0003VorzoyArm[sArm]);
//		m_Leynos.set_spr(EL_ARM_L,&SprCEneH0003VorzoyArm[enArmCannonR]);
		m_Leynos.set_spr(EL_ARM_L,NULL);
	}

	m_Leynos.set_spr(EL_LEG_L,&SprCEneH0003VorzoyLegOku[sLeg]);
	m_Leynos.set_spr(EL_LEG_R,&SprCEneH0003VorzoyLegTemae[sLeg]);
}

void CEneH0003Vorzoy::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------
	SetRader( enTypeMarkerGroundObject );
/*
	static Sint32 sCustomize1 = 0;
	static Sint32 sCustomize2 = 0;

	if(Joy[0].trg&JOY_L)
	{
		sCustomize1 --;
	}
	if(Joy[0].trg&JOY_R)
	{
		sCustomize1 ++;
	}
	sCustomize1 = (5+sCustomize1)%5;

	if(Joy[0].trg&JOY_U)
	{
		sCustomize2 --;
	}
	if(Joy[0].trg&JOY_D)
	{
		sCustomize2 ++;
	}
	sCustomize2 = (4+sCustomize2)%4;

	SeqCustomize( 3 ,sCustomize1 , sCustomize2);
*/

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
		//m_sBackWait = 60;
	}

/*
	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}
*/

	LogicAI();

	if( m_bDashMode )
	{
		m_Leynos.set_spd( 320 );
		GetLeynos()->SetDashMode();
	}
	else
	{
//		m_Leynos.set_spd( 180 );
	}
	m_Leynos.action();

	DirectionAdjust();

	m_Pos.x = m_Leynos.obj->pos.x;
	m_Pos.y = m_Leynos.obj->pos.y;

	if( m_HitKurai.is_damage() )
	{
		if( m_HitKurai.m_sAtackType == ATK_TYPE_CANNON )
		{
			m_Leynos.SetDamageAnime( m_HitKurai.dx );
		}

		if( m_sBackWait == 0) m_sBackWait = enBackTime;

		m_Leynos.set_damageflag();
	}

	if( pGame->GetMarineLine() && m_Pos.y >= pGame->GetMarineLine() )
	{
		//水流
		new CEffWaterStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , pGame->GetMarineLine(), 180 , 180 );
		new CEffWaterStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , pGame->GetMarineLine(), 180 , 180 );
		new CEffWaterStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , pGame->GetMarineLine(), 180 , 180 );
	}

	//くらい判定登録
	if( !m_Leynos.IsCarry() )
	{
		m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );
	}
}


void CEneH0003Vorzoy::LogicAI()
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
		LogicWaraWara();
		break;
	case enLogicCarry:
		LogicCarryAtack();
		break;
	case enLogicFlying:
		LogicFlyingAtack();
		break;
	}

	m_Leynos.set_control( m_sPadControl | BTN_L1 );

}


void CEneH0003Vorzoy::LogicNormal()
{
	Sint32 vx = GetTargetPlayer()->x;

	m_Leynos.set_spd( 320 );
	m_Leynos.set_spdadd(10);

	if( m_Leynos.IsNokezori() )
	{
		return;
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}

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
		Sint32 dst = gxAbs(vx-m_Pos.x)/100;

		switch(m_sLogicSeq){
		case 0:
			//-------------------------------
			//攻撃範囲内に入るまで待つ
			//-------------------------------
			m_Leynos.arm_rot += (0-m_Leynos.arm_rot)/5;
			m_sLogicTimer = 0;
			if(gxAbs(m_Leynos.arm_rot) < 5)
			{
				m_sLogicSeq = 50;
			}
			break;

		case 50:
			if( GetTargetPlayer()->x > m_Pos.x )
			{
				m_Leynos.set_dir( DIR_RIGHT );
			}
			else
			{
				m_Leynos.set_dir( DIR_LEFT );
			}

			if(dst < 64)
			{
				//近づかれすぎ
				m_Leynos.obj->mov.y = -800;
				m_sJumpWait = 30;
				m_sLogicSeq = 100;
			}
			else if(dst < 320)
			{
				//射程範囲内
				m_Leynos.obj->mov.y = -800;
				m_sJumpWait = 30;
				m_sLogicSeq = 100;
			}
			break;

		case 100:
			//-------------------------------
			//垂直ジャンプ攻撃
			//-------------------------------
			m_sAtackCnt = 0;
			m_sAtackWait = 0;
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
			m_sPadControl &= ~BTN_A;
			m_sJumpWait ++;
			if( m_sJumpWait%8==0 )
			{
				Sint32 ax,ay,r,rot;
				m_Leynos.setFirePos(40,0);	//武器発射位置設定
				m_Leynos.getFirePos(ax,ay);
				r = get_valken_rot( ax , ay-2400);
				r =  viiMath::GetTargetAngle( rot = viiSub::SetKakudo( m_Leynos.get_dir(),45 ) , r );
				if(gxAbs( r ) < 20 || m_sAtackWait>0 )
				{
					m_sAtackWait = 1;
					if( m_sAtackCnt < 4 )
					{
						Atack2();
						m_sAtackCnt ++;
					}
				}
			}
			if( m_Leynos.obj->is_landnow() )
			{
				m_sLogicSeq = 300;
			}
			break;

		case 300:
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


void CEneH0003Vorzoy::LogicWaraWara()
{
#if 1
	Sint32 tx = 186*100;//;
	Sint32 vx = tx;
	Sint32 sDist = abs( vx - m_Pos.x );

	SeqCustomize( 2,0,3);

	if( m_sBackWait )
	{
		if( m_sBackWait == enBackTime )
		{
			if( vx > m_Pos.x )
			{
				m_sPadControl = JOY_L;
				m_Leynos.set_dir(DIR_RIGHT);
			}
			else
			{
				m_sPadControl = JOY_L;
				m_Leynos.set_dir(DIR_LEFT);
			}
		}
		m_sBackWait --;
		if( m_sBackWait == 0)
		{
			if( GetTargetPlayer()->x > m_Pos.x )
			{
				m_Leynos.set_dir(DIR_RIGHT);
			}
			else
			{
				m_Leynos.set_dir(DIR_LEFT);
			}
			m_sAtackWait = 0;
			m_sPadControl = 0;
		}
	}
	else
	{
		if( m_Pos.x < tx )//sDist >= 6400)
		{
			m_sPadControl = JOY_R;
		}
		else
		{
			m_sPadControl = 0;
		}
	}

	if( m_sAtackWait > 0) 
	{
		m_sAtackWait --;
	}
	else
	{
		if( GetTargetPlayer()->x > m_Pos.x )
		{
//			m_sPadControl = JOY_L;
			m_Leynos.set_dir(DIR_RIGHT);
		}
		else
		{
//			m_sPadControl = JOY_R;
			m_Leynos.set_dir(DIR_LEFT);
		}

		m_sAtackWait = 300;
		m_sPadControl = 0;
	}

	if( m_sAtackWait > 270 )
	{

	}
	else if( m_sAtackWait > 260 )
	{
		m_Leynos.arm_rot += (-45-m_Leynos.arm_rot)/20;
	}
	else if( m_sAtackWait > 240 )
	{
		//薬きょうを捨てる
		m_Leynos.arm_rot += (80-m_Leynos.arm_rot)/3;

		//位置あわせを行う
		if( sDist <= 64*100 )
		{
/*
			if( vx > m_Pos.x )
			{
				m_sPadControl = JOY_L;
			}
			else
			{
				m_sPadControl = JOY_R;
			}
*/
		}

		if( m_sAtackWait%4==0)
		{
			Sint32 ax,ay;
			m_Leynos.setFirePos(20,10);	//武器発射位置設定
			m_Leynos.getFirePos(ax,ay);
//			viiEff::Set(EFF_YAKKYOU,ax,ay,argset(	(m_Leynos.get_dir()==DIR_LEFT)? -45 : 45*1+180 ) );
			viiEff::Set(EFF_YAKKYOU,ax,ay,argset(	(m_Leynos.get_dir()==DIR_LEFT)? -65 : 65*1+180 ) );
		}

	}
	else if( m_sAtackWait > 220 )
	{
		//薬きょうを捨てて元のポジションへ
		m_Leynos.arm_rot += (0-m_Leynos.arm_rot)/10;
	}
	else if( m_sAtackWait > 200 )
	{
		//狙いを定めて
		m_sPadControl = 0;

		Sint32 r = get_valken_rot( m_Pos.x , m_Pos.y);
		if(r >90 && r<270)
		{
			r = 90-(r-90);
		}
		if(r >270)	r -= 360;

		if(m_Leynos.arm_rot < r)
		{
			m_Leynos.arm_rot += 3;
		}
		else if( m_Leynos.arm_rot > r)
		{
			m_Leynos.arm_rot -= 3;
		}
	}
	else  if( m_sAtackWait < 180 )
	{
		if( m_sBackWait )
		{
			m_sAtackWait = 100;
		}
		else
		{
			//攻撃中
			m_Leynos.arm_rot += viiMath::Cos100( m_sAtackWait%30*6)/40;

			if( m_sAtackWait%8==0 && m_sAtackWait >= 120 )
			{
				Atack();
				m_sAtackCnt = 0;
				m_sAtackLag = 2;

				Sint32 ax,ay,rot;
				m_Leynos.getFirePos(ax,ay);
				rot = (m_Leynos.get_dir() == DIR_RIGHT)? m_Leynos.arm_rot : (m_Leynos.arm_rot*-1+180);
				new CEne0001EneCrasher( ax,ay , rot+(-16+viiSub::Rand()%32)*4, 160 );
			}
		}

	}

	m_Leynos.set_control( m_sPadControl | BTN_L1 );

#endif

}


void CEneH0003Vorzoy::LogicCarryAtack()
{
	//---------------------------------------------
	//キャリー攻撃
	//---------------------------------------------
	Sint32 ax,ay,rot,spd;

	if( !m_Leynos.IsCarry() )
	{
		//切り離された
		m_sAtackCnt  = 0;
		m_sAtackLag  = 0;
		m_sAtackWait = 0;
		m_sLogicPattern = enLogicNormal;
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

	m_Leynos.setFirePos( 45 , 0 );	//武器発射位置設定
	m_Leynos.getFirePos( ax , ay );

	viiEff::Set( EFF_MAZZLE,ax,ay , NULL );

	rot = viiSub::SetKakudo( m_Leynos.get_dir(),15 );
	spd = 220+m_sAtackCnt*40;

	viiMus::PlaySound(enSoundEneShotSmall);
	new CEne0001EneBullet( ax , ay , rot     , spd );
	new CEne0001EneBullet( ax , ay , rot -30 , spd );
	new CEne0001EneBullet( ax , ay , rot +30 , spd );
	m_sAtackLag = 16;

	m_sAtackCnt ++;

	if( m_sAtackCnt >= 3 ) 
	{
		m_sAtackCnt = 0;
		m_sAtackWait = 220;
	}
}


void CEneH0003Vorzoy::LogicFlyingAtack()
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


void CEneH0003Vorzoy::Atack()
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


void CEneH0003Vorzoy::Atack2()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

	Sint32 ax,ay;

	m_Leynos.setFirePos(40,0);	//武器発射位置設定
	m_Leynos.getFirePos(ax,ay);

/*
	Sint32 r = get_valken_rot( ax , ay-2400);

	r = r/20;
	r = r*20;

	if(r >90 && r<270)
	{
		r = 90-(r-90);
	}

	if(r >270)	r -= 360;

	Sint32 sKaku = 0;

	sKaku = viiMath::GetTargetAngle( m_Leynos.arm_rot , r );
	m_Leynos.arm_rot += sKaku;
	if( gxAbs( sKaku/10 ) == 0 )
	{
		if( sKaku < -1 ) sKaku --;
		if( sKaku > +1 ) sKaku ++;
	}
	else
	{
		sKaku = sKaku/10;
	}
*/
	viiEff::Set(EFF_MAZZLE,ax,ay,NULL);
	new CEne0001EneBullet( ax , ay , viiSub::SetKakudo( m_Leynos.get_dir() , m_Leynos.arm_rot ) , 180 );

#if 0
	Sint32 ax,ay,rot,spd;

//	if( m_sAtackLag ) return;

	m_Leynos.setFirePos(30,10);	//武器発射位置設定
	m_Leynos.getFirePos(ax,ay);

	rot = (m_Leynos.get_dir() == DIR_RIGHT)? m_Leynos.arm_rot : (m_Leynos.arm_rot*-1+180);
	spd = 200;

	viiMus::PlaySound(enSoundEneShotSmall);

	viiEff::Set(EFF_MAZZLE,ax,ay-400,NULL);

	new CEne0001EneBullet( ax , ay , viiSub::SetKakudo(m_Leynos.get_dir(),45-15) , 480 ,1);
	new CEne0001EneBullet( ax , ay , viiSub::SetKakudo(m_Leynos.get_dir(),45-18) , 320 ,0);
	new CEne0001EneBullet( ax , ay , viiSub::SetKakudo(m_Leynos.get_dir(),45+18) , 320 ,0);
	new CEne0001EneBullet( ax , ay , viiSub::SetKakudo(m_Leynos.get_dir(),45+15) , 480 ,1);

//	if( m_sAtackCnt !=1 )
//	{
//		spd = 480;
//		new CDanmakuRoot( CDanmakuRoot::enDanmakuGattling , ax , ay, rot , spd );
//		new CEne0001EneBullet( ax , ay , rot-15 , spd );
//		new CEne0001EneBullet( ax , ay , rot    , spd );
//		new CEne0001EneBullet( ax , ay , rot+15 , spd );

//	}

//	m_sAtackCnt ++;
//
//	if( m_sAtackCnt >= 4 )
//	{
//		m_sAtackLag = 10;
//		m_sAtackCnt = 0;
//	}
#endif

}


void CEneH0003Vorzoy::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

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
			if( m_HitKurai.m_sAtackType == ATK_TYPE_CANNON )
			{
				m_Leynos.SetDamageAnime( m_HitKurai.dx );
			}
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


void CEneH0003Vorzoy::Draw()
{
	m_Leynos.Draw();

//	viiDraw::Sprdmg( m_HitKurai.is_damage() , &SprCEneH0003Vorzoy[ 0 ] , m_Pos.x , m_Pos.y,PRIO_ENEMY , m_sAtribute|m_sAtributeDir , ARGB_DFLT );

}

