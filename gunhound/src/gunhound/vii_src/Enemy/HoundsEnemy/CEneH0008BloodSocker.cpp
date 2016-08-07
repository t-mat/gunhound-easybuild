//--------------------------------------------------------------------------------
//
// Stage00:ボルゾイ（ブラッドサッカー）
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
//#include "EnemyTbl/CEneH0008BloodSockerTBL.h"
#include "EnemyTbl/CEneH0008BloodSockerCustomTbl.h"
#include "../../effect/EffectBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -14,
	enKuraiTop   = -48,
	enKuraiRight =  14,
	enKuraiBottom=  0,

	enScore = 1200,

	enBackTime = 30,

	enShortBurrelSpeed = 480,
	enLongBurrelSpeed  = 480,
	enSolidShooterSpeed  = 640,

	enHardVorzoyHP = enDefPointZakoBS*4,
};


CEneH0008BloodSocker::CEneH0008BloodSocker( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Src.x = m_Pos.x = x;
	m_Src.y = m_Pos.y = y;
	m_Grd.x = m_Pos.x;
	m_Grd.y = m_Pos.y;

	m_HitKurai.set_hp( enDefPointZakoBS );
	m_HitGuard.set_hp( enAtkPointBazooka );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_HitGuard.ax1 = -12;
	m_HitGuard.ay1 = -12;
	m_HitGuard.ax2 = +12;
	m_HitGuard.ay2 = +12;

	m_bSpecialVersion = gxFalse;

	m_sDirection = DIR_LEFT;

	//-----------------------------------

	m_sPushControl = 0;
	m_sTrigControl = 0;
	m_sBackWait    = 0;
	m_sReboundWait = 0;
	m_sArmRotation = 0;

	//-----------------------------------

	m_sAtackCnt = 0;
	m_sAtackLag = 0;
	m_bGuard    = gxFalse;
	m_bArmCrash = gxFalse;

	for(Sint32 ii=0;ii<enWeaponMax; ii++)
	{
		//武器を初期化
		m_sWeapon[ ii ] = enWeaponNone;
	}

	//-----------------------------------

	m_sWeaponNum  = 0;
	m_sMainWeapon = 0;
	m_sTargetSeq  = 0;

	//-----------------------------------

	m_sBody = enBodyNormal;
	m_sLegs = enLegNormal;
	m_sArms = enArmShort;

	//-----------------------------------
	m_sCrashTimer = 0;

	SetInitialize();

	m_pLaser = NULL;

	m_bEscaped = gxFalse;
	SetCustomIndex(0);
}


CEneH0008BloodSocker::~CEneH0008BloodSocker()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------
	if(m_pLaser) delete m_pLaser;

}


void CEneH0008BloodSocker::SeqInit()
{
	//-----------------------------------------------------
	//初期化
	//-----------------------------------------------------
	SetRader( enTypeMarkerGroundObject );

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}

	Sint32 tx = GetTargetPlayer()->x;

	if( tx < m_Pos.x-16*100 )
	{
		m_Leynos.set_dir( DIR_LEFT );
	}
	else if( tx > m_Pos.x-16*100 )
	{
		m_Leynos.set_dir( DIR_RIGHT );
	}

	SetActionSeq( enActionSeqMain );
}


void CEneH0008BloodSocker::SetInitialize()
{
	//-----------------------------------------------------
	//とりあえずの初期化
	//-----------------------------------------------------

	m_Leynos.set_spr(EL_BASE ,NULL);

	m_Leynos.set_spr(EL_ARM_L,NULL);
	m_Leynos.set_spr(EL_BODY ,&SprCEneH0008BloodSockerBody[0]);
	m_Leynos.set_spr(EL_ARM_R,&SprCEneH0008BloodSockerArm[3]);
	m_Leynos.set_spr(EL_LEG_L,&SprCEneH0008BloodSockerLegOku[1]);
	m_Leynos.set_spr(EL_LEG_R,&SprCEneH0008BloodSockerLegTemae[2]);

	m_Leynos.parts[EL_BASE]->ofx = -3;
	m_Leynos.parts[EL_BASE]->ofy = -26;

	m_Leynos.parts[EL_BODY]->ofx = 2;
	m_Leynos.parts[EL_BODY]->ofy = -6;

	m_Leynos.parts[EL_ARM_R]->ofx = -4;
	m_Leynos.parts[EL_ARM_R]->ofy = -22;

	m_Leynos.parts[EL_ARM_L]->ofx = 0;
	m_Leynos.parts[EL_ARM_L]->ofy = -12;

	m_Leynos.parts[EL_LEG_L]->ofx = 4;
	m_Leynos.parts[EL_LEG_L]->ofy = -3;

	m_Leynos.parts[EL_LEG_R]->ofx = 4;
	m_Leynos.parts[EL_LEG_R]->ofy = -3;


	m_Leynos.parts[EL_BODY ]->sx = 40;
	m_Leynos.parts[EL_ARM_R]->sx = 30;
	m_Leynos.parts[EL_ARM_L]->sx = 25;
	m_Leynos.parts[EL_LEG_L]->sx = 25;
	m_Leynos.parts[EL_LEG_R]->sx = 25;


	m_Leynos.set_pos( m_Pos.x , m_Pos.y , PRIO_ENEMY );

	m_Leynos.set_spd( 50+viiSub::Rand()%80 );
	m_Leynos.set_walkcycle(40);
	m_Leynos.walk_width_ = 80;
	m_Leynos.walk_height_ = 100;
	m_Leynos.cpu_control = gxTrue;
	m_Leynos.set_spdadd(0);
	m_Leynos.setFirePos(5,10);

}

void CEneH0008BloodSocker::SetCustomIndex(Sint32 Index)
{
	//-------------------------------------------------------
	//インデックスによるカスタマイズ
	//-------------------------------------------------------

	Sint32 n = g_sCustomizeTbl[Index];
	Sint32 sBody = 0;
	Sint32 sLegs = 0;
	Sint32 sArms = 0;

	ASSERT(Index>=0 && Index<=20);

	if( n == 999 )
	{
		//SPECIAL VORZOY
		m_sBody = sBody = 1;
		m_sLegs = sLegs = 2;
		m_sArms = sArms = 2;
		m_bSpecialVersion = gxTrue;
		//HITPOINT高め
		m_HitKurai.set_hp( enHardVorzoyHP );
		m_HitGuard.set_hp( enAtkPointBazooka );
	}
	else
	{
		m_sBody = sBody = (n/100)%10;
		m_sLegs = sLegs = (n/10)%10;
		m_sArms = sArms = n%10;
	}

	SetCustomize( sBody , sArms, sLegs );

	//武器の設定
	SetArms();

}


void CEneH0008BloodSocker::SetArms()
{
	//-------------------------------------------------------
	//武器設定
	//-------------------------------------------------------

	m_sWeaponNum = 0;

	//アーム武器

	switch( m_sArms ) {
	case enArmShort:
		m_sWeapon[m_sWeaponNum] = enWeaponShort;
		m_sWeaponNum ++;
		break;
	case enArmLong:
		m_sWeapon[m_sWeaponNum] = enWeaponLong;
		m_sWeaponNum ++;
		break;
	case enArmSolid:
		m_sWeapon[m_sWeaponNum] = enWeaponSolid;
		m_sWeaponNum ++;
		break;
	case enArmShoulder:
		m_sWeapon[m_sWeaponNum] = enWeaponLong;
		m_sWeaponNum ++;
		m_sWeapon[m_sWeaponNum] = enWeaponSPod;
		m_sWeaponNum ++;
		break;
	case enArmCannon:
	case enArmCannonR:
		m_sWeapon[m_sWeaponNum] = enWeaponLogGun;
		m_sWeaponNum ++;
		break;
	default:
		break;
	}

	//体パーツ

	switch( m_sBody ) {
	case enBodyHowk:
		m_sWeapon[m_sWeaponNum] = enWeaponBCrasher;
		m_sWeaponNum ++;
		break;
	case enBodySlaster:
		m_bGuard = gxTrue;
		break;
	case enBodyNormal:
	case enBodyMini:
	default:
		break;
	}

	//脚パーツ

	switch( m_sLegs ) {
	case enLegPod:
		m_sWeapon[m_sWeaponNum] = enWeaponLPod;
		m_sWeaponNum ++;
		break;
	case enLegChicken:
	case enLegNormal:
	case enLegArmored:
	default:
		break;
	}
}


void CEneH0008BloodSocker::SetCustomize( Sint32 sBody ,Sint32 sArm , Sint32 sLeg)
{
	//----------------------------------------------
	//武器ごとのグラフィックのカスタマイズ
	//----------------------------------------------

	m_Leynos.set_spr(EL_BODY ,&SprCEneH0008BloodSockerBody[sBody]);

	if( sArm == enArmCannon )
	{
		m_Leynos.set_spr(EL_ARM_R,&SprCEneH0008BloodSockerArm[enArmCannon]);
		m_Leynos.set_spr(EL_ARM_L,&SprCEneH0008BloodSockerArm[enArmCannonR]);
	}
	else if( sArm == enArmNone )
	{
		m_Leynos.set_spr(EL_ARM_R,&SprCEneH0008BloodSockerArm[enArmCannonR]);
		m_Leynos.set_spr(EL_ARM_L,NULL);
	}
	else
	{
		m_Leynos.set_spr(EL_ARM_R,&SprCEneH0008BloodSockerArm[sArm]);
//		m_Leynos.set_spr(EL_ARM_L,&SprCEneH0008BloodSockerArm[enArmCannonR]);
		m_Leynos.set_spr(EL_ARM_L,NULL);
	}

	m_Leynos.set_spr(EL_LEG_L,&SprCEneH0008BloodSockerLegOku[sLeg]);
	m_Leynos.set_spr(EL_LEG_R,&SprCEneH0008BloodSockerLegTemae[sLeg]);


}

void CEneH0008BloodSocker::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------
	SetRader( enTypeMarkerGroundObject );

//	SetCustomize( 2 , 2, 3 );

	m_bLaser = gxFalse;

	if( m_bGuard && m_HitGuard.is_dead() )
	{
		//武器破壊
		Sint32 ax,ay;

		m_bGuard    = gxFalse;
		m_bArmCrash = gxTrue;
		SetCustomize( m_sBody , enArmNone , m_sLegs );

		//m_sWeapon[0] = enWeaponNone;	//武器をなくしてしまう
		for(Sint32 ii=0;ii<m_sWeaponNum;ii++)
		{
			//武器をすべてなくしてしまう
			m_sWeapon[ii] = enWeaponNone;
		}

		m_Leynos.setFirePos( 16 , 8 );
		m_Leynos.getFirePos( ax , ay );

		//ソルジャー逃げる
		{
			CSoldierBase *p = NULL;
			p = new CSoldierBase( );

			p->SetPosition( 0 , m_Pos.x  , m_Pos.y-16*100 , NULL);
			p->SetForce(enForceEnemy);
			p->Command( enSoldierCommandFuttobi );
			m_bEscaped = gxTrue;
		}

		viiMus::PlaySound(enSoundExplosionMiddle);
		viiEff::SetBombSimple( ax , ay , 1 , 1+viiSub::Rand()%2 );
	}

	if( m_HitKurai.is_dead() )
	{
		viiEff::SetBombSimple( m_Pos.x , m_Pos.y - 3200 , 3 ,1 );
		viiEff::Set( EFF_RING , m_Pos.x , m_Pos.y-24*100 , argset(400) );
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
	}
	else
	{
		ActionMain();
	}

	m_Leynos.action();
	m_Pos.x = m_Leynos.obj->pos.x;
	m_Pos.y = m_Leynos.obj->pos.y;

	DirectionAdjust();

	if( m_HitKurai.is_damage() )
	{
		if( m_HitKurai.m_sAtackType == ATK_TYPE_CANNON )
		{
			m_Leynos.SetDamageAnime( m_HitKurai.dx );
		}
		m_sBackWait = enBackTime;
		m_sArmRotation = (viiSub::Rand()%2)? -60 : 60;
		m_Leynos.set_damageflag();
	}

	//くらい判定登録
	m_HitKurai.SetHantei( ID_ENEMY_DEF , &m_Pos );

}


void CEneH0008BloodSocker::ActionMain()
{
	//-----------------------------------------------------
	//メインアクション（毎フレーム通過）
	//-----------------------------------------------------

	gxBool bNoneControl = gxFalse;

	if( !m_Leynos.cpu_control )
	{
		m_sPushControl = Joy[0].psh;
		m_sTrigControl = Joy[0].trg;
	}
	else
	{
		m_sPushControl = 0;
		m_sTrigControl = 0;
		LogicAI();
	}

	if( m_sBackWait )
	{
		//チキンレッグの特殊効果
		if( m_sLegs == enLegChicken || m_bSpecialVersion)
		{
			//ノックバックをキャンセルする
			m_sBackWait = 0;
		}
	}

	if( m_sBackWait > 0 )
	{
		//くらいモーション制御

		if (m_Leynos.get_dir() == DIR_RIGHT)
		{
			m_sPushControl = JOY_L|BTN_L1;
		}
		else
		{
			m_sPushControl = JOY_R|BTN_L1;
		}

		//被弾モーション
		if( m_sBackWait > enBackTime-10 )
		{
			m_Leynos.arm_rot += (m_sArmRotation-m_Leynos.arm_rot)/10;
		}
		else
		{
			m_Leynos.arm_rot += (-m_sArmRotation-m_Leynos.arm_rot)/10;
		}

		if( m_sBody == enBodyHowk )
		{
			//ホーク搭載ならチャフ弾を打たせてガードさせる

			m_sTargetSeq = -1;
			AtackBodyCrasher();
		}

		//ガードさせる
		if( m_bGuard )
		{
			Sint32 ax,ay;

			m_Leynos.setFirePos( 16 , 8 );
			m_Leynos.getFirePos( ax , ay );
			m_Grd.x = ax;
			m_Grd.y = ay;

			m_HitGuard.SetHantei( ID_ENEMY_DEF , &m_Grd );
		}

		m_sBackWait --;

		bNoneControl = gxTrue;

		if( m_sBackWait == 0)
		{
//			LogicAI();
//			WeaponReset();
			bNoneControl = gxFalse;
		}

	}

	if( m_sReboundWait > 0)
	{
		//腕のリバウンド制御

		m_sReboundWait --;
		m_Leynos.arm_rot += ( -90 - m_Leynos.arm_rot )/10;

		bNoneControl = gxTrue;
	}

	if( !bNoneControl )
	{
		PadControl(0);
	}

	if( m_sAtackLag > 0 ) m_sAtackLag --;

	m_Leynos.set_control( m_sPushControl );

}


void CEneH0008BloodSocker::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	if( m_Leynos.IsNokezori() )
	{
		return;
	}

	if( IsWeaponChangeChance() )
	{
		Sint32 tx = GetTargetPlayer()->x;
		if( tx < m_Pos.x-16*100 )
		{
			m_Leynos.set_dir( DIR_LEFT );
		}
		else if( tx > m_Pos.x-16*100 )
		{
			m_Leynos.set_dir( DIR_RIGHT );
		}

		m_sTrigControl |= BTN_Y;
	}

	m_sPushControl |= BTN_X;

}


void CEneH0008BloodSocker::PadControl(Sint32 n)
{
	//-----------------------------------------------------
	//コントロール
	//ifの順番は行動の優先順位になるので注意！！
	//例）Ｙボタン押しっぱなしだと攻撃できない、など
	//-----------------------------------------------------

	if( m_sTrigControl&BTN_Y )
	{
		m_sMainWeapon ++;
		WeaponReset();
	}
	else if( m_sPushControl&BTN_X )
	{
		Atack( m_sWeapon[m_sMainWeapon] );
	}

}


void CEneH0008BloodSocker::Atack(Sint32 n)
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

	switch( n ){
	case enWeaponShort:
		AtackShortBurrel();
		break;
	case enWeaponLong:
		AtackLongBurrel();	
		break;
	case enWeaponSolid:
		AtackSolidShooter();
		break;
	case enWeaponSPod:
		AtackShoulderMissilePod();
		break;
	case enWeaponLogGun:
		AtackLogGun();
		break;
	case enWeaponLPod:
		AtackLegPod();
		break;
	case enWeaponBCrasher:
		AtackBodyCrasher();
		break;
	default:
		break;
	}

}


void CEneH0008BloodSocker::WeaponReset()
{
	m_sMainWeapon = m_sMainWeapon%m_sWeaponNum;
	m_sAtackCnt  = 0;
	m_sAtackLag  = 0;
	m_sTargetSeq = 0;
}


gxBool CEneH0008BloodSocker::IsWeaponChangeChance()
{
	//------------------------------------------------
	//攻撃のチャンスか？
	//------------------------------------------------

//	if( m_sTargetSeq == 0 )
	if( m_sAtackCnt && m_sTargetSeq == 0 )
	{
		return gxTrue;
	}
	return gxFalse;
}


void CEneH0008BloodSocker::AdjustArmAngle( Sint32 ax , Sint32 ay ,Sint32 sOffset)
{
	//------------------------------------------------
	//腕の角度をあわせる
	//------------------------------------------------

	Sint32 r = get_valken_rot( ax , ay+2400);

//	r = r/20;
//	r = r*20;

	if(r >90 && r<270)
	{
		r = 90-(r-90);
	}

	if(r >270)	r -= 360;

	Sint32 sKaku = 0;

	sKaku = viiMath::GetTargetAngle( m_Leynos.arm_rot , r+sOffset );
	if( gxAbs( sKaku/10 ) == 0 )
	{
		if( sKaku < -1 ) sKaku = -1;
		if( sKaku > +1 ) sKaku = +1;
	}
	else
	{
		sKaku = sKaku/10;
	}
	m_Leynos.arm_rot += sKaku;

}


void CEneH0008BloodSocker::AtackShortBurrel()
{
	//------------------------------------------------
	//ショートバレルを乱射
	//------------------------------------------------

	Sint32 ax,ay,sRot;
	m_Leynos.setFirePos( 30 , 5 );

	sRot = (m_Leynos.get_dir() == DIR_RIGHT)? m_Leynos.arm_rot : (m_Leynos.arm_rot*-1+180);

	if( m_sTargetSeq != -1 )
	{
		m_sTargetSeq ++;

		m_Leynos.getFirePos( ax , ay );

		if( m_sTargetSeq < 60)
		{

		}
		else if( m_sTargetSeq < 90)
		{
			//腕合わせ
			m_sPushControl = BTN_A;	//ジャンプしながら位置あわせ
			AdjustArmAngle( ax ,ay );
		}
		else
		{
			m_sArmRotation = m_Leynos.arm_rot;
			m_sTargetSeq = -1;
		}

	}
	else if( m_sAtackLag == 0 )
	{
		//攻撃タイミング

		m_Leynos.arm_rot = m_sArmRotation+ ( viiMath::Cos100( (m_sTimer*16)%360 )*16)/100;

		m_Leynos.getFirePos( ax , ay );
		m_Leynos.set_knockback(10);
	
		viiMus::PlaySound(enSoundEneShotSmall);

		new CEne0001EneBullet( ax , ay , sRot-8+viiSub::Rand()%16 , enShortBurrelSpeed );
		viiEff::Set(EFF_MAZZLE,ax,ay,NULL);

		viiEff::Set(EFF_YAKKYOU , m_Pos.x , m_Pos.y-2400 ,argset(	(m_Leynos.get_dir()==DIR_LEFT)? -65 : 65*1+180 ) );

		m_sAtackLag = 4;
		m_sAtackCnt ++;

		if( m_sAtackCnt%8 == 0 )
		{
			//8発撃ったら角度を補正
			m_sTargetSeq = 0;
		}

	}

}


void CEneH0008BloodSocker::AtackLongBurrel()
{
	//------------------------------------------------
	//実弾を狙い撃ちしてくる
	//------------------------------------------------

	Sint32 ax,ay,sRot;

	m_Leynos.setFirePos( 50 , 2 );
	m_Leynos.getFirePos( ax , ay );

	sRot = (m_Leynos.get_dir() == DIR_RIGHT)? m_Leynos.arm_rot : (m_Leynos.arm_rot*-1+180);

	if( m_sTargetSeq != -1 )
	{
		m_sTargetSeq ++;

		if( m_sTargetSeq < 46 )
		{
			AdjustArmAngle( ax ,ay );
		}
		else
		{
			m_sTargetSeq = -1;
		}
	}
	else  if( m_sAtackLag == 0 )
	{
		//攻撃タイミング

		m_Leynos.set_knockback(10);
	
		viiMus::PlaySound(enSoundEneShotSmall);

		new CEne0001EneBullet( ax , ay , sRot-2+viiSub::Rand()%4 , enLongBurrelSpeed );
		viiEff::Set(EFF_MAZZLE,ax,ay,NULL);

		viiEff::Set(EFF_YAKKYOU , m_Pos.x , m_Pos.y-2400 ,argset(	(m_Leynos.get_dir()==DIR_LEFT)? -65 : 65*1+180 ) );

		m_sAtackLag = 4;
		m_sAtackCnt ++;

		if( m_sAtackCnt%4 == 0 )
		{
			//4発撃ったら角度を補正
			m_sTargetSeq = 0;
		}
	}

}

void CEneH0008BloodSocker::AtackSolidShooter()
{
	//------------------------------------------------
	//狙い撃ちバズーカ
	//------------------------------------------------
	Sint32 ax,ay,sRot;
	gxBool bATack = gxFalse;

	sRot = viiSub::SetKakudo(m_Leynos.get_dir(),m_Leynos.arm_rot);

	m_Leynos.setFirePos( 60 , -8 );
	m_Leynos.getFirePos( ax , ay );

	if( m_sTargetSeq != -1 )
	{
		m_sTargetSeq ++;
		Sint32 sDist = get_valken_dist(ax,ay);
		Sint32 sKaku = -15;	

		if(sDist > 6400)
		{
			sKaku = -(sDist/100)/10;
		}
		AdjustArmAngle( ax ,ay , sKaku );//viiSub::SetKakudo(m_Leynos.get_dir(),-15) );

		if( m_sTargetSeq < 60 )
		{
			viiEff::Set(EFF_ENERGYBALL , ax , ay , argset(50+m_sAtackCnt,50,0,50));
		}
		else if( m_sTargetSeq == 150 )
		{
			bATack = gxTrue;
		}
		else if( m_sTargetSeq < 160 )
		{

		}
		else
		{
			m_sTargetSeq = -1;
		}
	}


	if( bATack )
	{
		//攻撃タイミング

		m_Leynos.set_knockback(10);
	
		viiEff::Set( EFF_MAZZLE , ax , ay , NULL );
		viiEff::SetBombSimple( ax , ay , 1 , 1+viiSub::Rand()%2 );
		new CEffBombSmoke( ax , ay ,-135 , 120 );
		new CEffBombSmoke( ax , ay , 135 , 120 );
		new CEffBombSmoke( ax , ay ,-180 , 120 );
		viiEff::Set( EFF_RING , ax , ay , argset(500) );

		viiMus::PlaySound(enSoundEneShotLaser);

		CEne0001EneCannon *pCEne0001EneCannon;
		pCEne0001EneCannon = new CEne0001EneCannon( ax , ay , sRot , 840 );//enSolidShooterSpeed );
		pCEne0001EneCannon->SetDrop();

		m_sAtackLag = 24;
		m_sBackWait = //
		m_sReboundWait = 32;
		m_sAtackCnt ++;

		m_sTargetSeq = 0;
//		if( m_sAtackCnt%1 == 0 )
//		{
//			//1発撃ったら角度を補正
//		}
	}

}


void CEneH0008BloodSocker::AtackShoulderMissilePod()
{
	//------------------------------------------------
	//追尾式ミサイル
	//------------------------------------------------

	Sint32 ax,ay,sRot;

	sRot = (m_Leynos.get_dir() == DIR_RIGHT)? m_Leynos.arm_rot : (m_Leynos.arm_rot*-1+180);

	m_Leynos.setFirePos( 12 , -18 );
	m_Leynos.getFirePos( ax , ay );

	if( m_sTargetSeq != -1 )
	{
		m_sTargetSeq ++;

		if( m_sTargetSeq < 30 )
		{
			AdjustArmAngle( ax ,ay );
		}
		else if( m_sTargetSeq < 120 )
		{

		}
		else
		{
			m_sTargetSeq = -1;
		}
	}
	else  if( m_sAtackLag == 0 )
	{
		//攻撃タイミング

		m_Leynos.set_knockback(10);
	
		viiMus::PlaySound(enSoundEneShotSmall);

		CAtkH0001Missile *p;
		p = new CAtkH0001Missile( ax , ay ,sRot,100);
		p->SetForce( CAtkH0001Missile::enForceID_Enemy );

		viiEff::Set(EFF_MAZZLE,ax,ay,NULL);
		m_sAtackLag = 8;
		m_sAtackCnt ++;

		if( m_sAtackCnt%3 == 0 )
		{
			//3発撃ったら角度を補正
			m_sTargetSeq = 0;
		}
	}

}

void CEneH0008BloodSocker::AtackLogGun()
{
	//------------------------------------------------
	//溜め打ち極太レーザー
	//------------------------------------------------
	Sint32 ax,ay,sRot;
	Sint32 tx = GetTargetPlayer()->x;
	Sint32 ty = GetTargetPlayer()->y-3200;

	sRot = (m_Leynos.get_dir() == DIR_RIGHT)? m_Leynos.arm_rot : (m_Leynos.arm_rot*-1+180);

	m_Leynos.setFirePos( 64 , -28 );
	m_Leynos.getFirePos( ax , ay );

	switch( m_sTargetSeq ){
	case 0:
		m_sAtackCnt = 0;
		m_sTargetSeq = 100;
		break;

	case 100:
		viiEff::Set(EFF_ENERGYBALL , ax , ay , argset(50+m_sAtackCnt,50,0,50));
		if( m_sAtackCnt < 120 )
		{
			if( m_sAtackCnt%8 == 0 ) AdjustArmAngle( ax ,ay );
		}
		else if( m_sAtackCnt < 240 )
		{
			if( m_sAtackCnt%4==0) m_Leynos.set_knockback(4);
		}
		else
		{
			CEne0001EneHyperBall *q;
			q = new CEne0001EneHyperBall( ax , ay , viiSub::SetKakudo(m_Leynos.get_dir(),m_Leynos.arm_rot), 120 );
			q->SetPattern( CEne0001EneHyperBall::enPatternTargetting );

			m_sAtackCnt = 0;
			m_sTargetSeq = 200;
		}
		break;

	case 200:
		if( m_sAtackCnt < 180 || m_sAtackCnt > 200 )
		{
			if( m_sAtackCnt%4==0) m_Leynos.set_knockback(4);
		}
		if( m_sAtackCnt == 180 )
		{
			m_sBackWait = //
			m_sReboundWait = 2;
			m_Leynos.set_knockback(12);
		}
		if( m_sAtackCnt > 320 )
		{
			m_sTargetSeq = 0;
		}
		break;
	default:
		break;
	}
	m_sAtackCnt ++;

#if 0

	if( m_sTargetSeq != -1 )
	{
		m_sTargetSeq ++;

		if( m_sTargetSeq < 60 )
		{
			AdjustArmAngle( ax ,ay );
		}
		else if( m_sTargetSeq < 220 )
		{
//			m_pLaser->EnergyIn();
		}
		else
		{
			m_sTargetSeq = -1;
			m_sAtackCnt = 0;
		}
	}
	else  if( m_sAtackLag == 0 )
	{
		//攻撃タイミング

		//if( m_pLaser == NULL )
		//{
		//	m_pLaser = new CAtkH0001Thunder(m_Pos.x,m_Pos.y,tx,ty);
		//}
		//m_pLaser->Set( ax , ay , tx,ty );

		m_bLaser = gxTrue;

		m_Leynos.set_knockback(10);

		viiMus::PlaySound(enSoundEneShotSmall);

//		m_pLaser->SetEnemyAtack();
//		m_pLaser->SetLaserLevel(3);
		m_sAtackLag = 0;
		m_sAtackCnt ++;

		if( m_sAtackCnt%180 == 0 )
		{
			//180秒撃ったら角度を補正
			//m_pLaser = NULL;

			CEne0001EneHyperBall *q;
			q = new CEne0001EneHyperBall( ax , ay , viiSub::SetKakudo(m_Leynos.get_dir(),m_Leynos.arm_rot), 120 );
			q->SetPattern( CEne0001EneHyperBall::enPatternTargetting );

			m_sTargetSeq = 0;
		}
	}
#endif

}


void CEneH0008BloodSocker::AtackLegPod()
{
	//------------------------------------------------
	//２連式巡航ミニミサイル
	//------------------------------------------------
	Sint32 ax,ay,sRot;

	sRot = -15;
	sRot = (m_Leynos.get_dir() == DIR_RIGHT)? sRot : (sRot*-1+180);

	if( m_sTargetSeq != -1 )
	{
		m_sTargetSeq ++;

		if( m_sTargetSeq < 10 )
		{
			m_Leynos.arm_rot += (-60-m_Leynos.arm_rot)/10;
		}
		else if( m_sTargetSeq < 50 )
		{
			m_Leynos.arm_rot += (45-m_Leynos.arm_rot)/5;
		}
		else if( m_sTargetSeq < 60 )
		{
			
		}
		else
		{
			m_sTargetSeq = -1;
		}
	}
	else  if( m_sAtackLag == 0 )
	{
		//攻撃タイミング

		m_Leynos.setFirePos( 12 , -18 );
		m_Leynos.getFirePos( ax , ay );
		ax = m_Pos.x;
		ay = m_Pos.y-800;
		m_Leynos.set_knockback(10);
	
		viiMus::PlaySound(enSoundEneShotSmall);

		CAtkH0002GMissile *p;
		p = new CAtkH0002GMissile(ax,ay , sRot, 160 );
		p->SetForce( CAtkH0001Missile::enForceID_Enemy );

		viiEff::Set( EFF_MAZZLE , ax , ay , NULL );
		viiEff::SetBombSimple( ax , ay , 1 , 1+viiSub::Rand()%2 );
		new CEffBombSmoke( ax , ay ,-135 , 120 );
		new CEffBombSmoke( ax , ay , 135 , 120 );
		new CEffBombSmoke( ax , ay ,-180 , 120 );
		viiEff::Set( EFF_RING , ax , ay , argset(100) );


		m_sAtackLag = 14;
		m_sAtackCnt ++;

		if( m_sAtackCnt%2 == 0 )
		{
			//2発撃ったら待つ
			m_sTargetSeq = 0;
		}
	}

}


void CEneH0008BloodSocker::AtackBodyCrasher()
{
	//------------------------------------------------
	//ボディからのクラッシャー
	//------------------------------------------------
	Sint32 ax,ay,sRot;

	sRot = 0;
	sRot = (m_Leynos.get_dir() == DIR_RIGHT)? sRot : (sRot*-1+180);

	if( m_sTargetSeq != -1 )
	{
		m_sTargetSeq ++;

		if( m_sTargetSeq < 10 )
		{
			m_Leynos.arm_rot += (30-m_Leynos.arm_rot)/10;
		}
		else if( m_sTargetSeq < 50 )
		{
			m_Leynos.arm_rot += (-45-m_Leynos.arm_rot)/5;
		}
		else if( m_sTargetSeq < 60 )
		{
			
		}
		else
		{
			m_sTargetSeq = -1;
		}
	}
	else  if( m_sAtackLag == 0 )
	{
		//攻撃タイミング

		m_Leynos.setFirePos( -2 , 8 );
		m_Leynos.getFirePos( ax , ay );
//		ax = m_Pos.x+((m_Leynos.get_dir() == DIR_RIGHT)? 1800 : -1800);
//		ay = m_Pos.y-2600;

//		m_Leynos.set_knockback(10);
	
		viiMus::PlaySound(enSoundEneShotSmall);

		new CEne0001EneCrasher(ax,ay , sRot+(-16+viiSub::Rand()%32)*4, 160 );
		viiEff::Set(EFF_MAZZLE,ax,ay,NULL);

//		viiEff::Set(EFF_YAKKYOU , m_Pos.x , m_Pos.y-2400 ,argset(	(m_Leynos.get_dir()==DIR_LEFT)? -65 : 65*1+180 ) );

		m_sAtackLag = 6;
		m_sAtackCnt ++;

		if( m_sAtackCnt%24 == 0 )
		{
			//16発撃ったら待つ
			m_sTargetSeq = 0;
		}
	}

}



void CEneH0008BloodSocker::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	if( !m_bDead )
	{
		m_bDead = gxTrue;
//		viiMus::PlaySound(enSoundExplosionSmall);
		viiMus::PlaySound( enSoundEneShotBiriBiri );
	}

	m_sCrashTimer ++;

	if( m_sCrashTimer%5 == 0 && m_sCrashTimer <= 30 )
	{
		//ビリビリ
		viiEff::Set( EFF_BREAK_THUNDER , m_Pos.x+(-8+viiSub::Rand()%16)*100 , m_Pos.y+(-64+viiSub::Rand()%48)*100 );
	}

	if( m_sCrashTimer < 50 )
	{
		m_Leynos.action();

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
		m_Leynos.m_bDead = gxTrue;

		viiMus::PlaySound( enSoundExplosionMiddle );

		pGame->QuakeOn(16);

		EffectMakeBigBomb(m_Pos.x , m_Pos.y);

		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 3 , 1+viiSub::Rand()%5 );
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 3 , 1+viiSub::Rand()%5 );

//		new CEneH0305Bomb( CEneH0305Bomb::enTypeFire , m_Pos.x , m_Pos.y , PRIO_ENEMY , -45 ,180 );

		CEffectDmgprt *pDmg;
		pDmg = new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+15*1 , 300 );
		pDmg->SetSprite( m_Leynos.parts[EL_BODY]->spr );

		pDmg = new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+90-15 , 450 );
		pDmg->SetSprite( m_Leynos.parts[EL_LEG_R]->spr );

		pDmg = new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+90+25 ,500 );
		pDmg->SetSprite( m_Leynos.parts[EL_LEG_L]->spr );

		pDmg = new CEffectDmgprt( m_Pos.x, m_Pos.y , 180+165 , 350 );
		pDmg->SetSprite( m_Leynos.parts[EL_ARM_R]->spr );
	}

	if( viiSub::Rand()%4 && !m_bEscaped )
	{
		CSoldierBase *p = NULL;
		p = new CSoldierBase( );

		p->SetPosition( 0 , m_Pos.x  , m_Pos.y-16*100 , NULL);
		p->SetForce(enForceEnemy);
		p->Command( enSoldierCommandFuttobi );
	}

	m_Leynos.action();

	ScorePlus( enScore );

	SetActionSeq( enActionSeqEnd );
}

void CEneH0008BloodSocker::Draw()
{
	m_Leynos.Draw();

	if(m_pLaser && m_bLaser)
	{
		m_pLaser->Draw();
	}

}

