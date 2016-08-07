//--------------------------------------------------------------------------------
//
// HOUNDプロトタイプ
//
//--------------------------------------------------------------------------------
/*
enum {
	enWeaponIconNone,
	enWeaponIconBullet,
	enWeaponIconPunch,
	enWeaponIconMissile,
	enWeaponIconRocket,
};

enum {
	enWeaponGaugeNone,
	enWeaponGaugeOne,
	enWeaponGaugeTwo,
	enWeaponGaugeThree,
	enWeaponGaugeFour,
	enWeaponGaugeMeter,
};
*/

void CGunHound::SetWeapon(Sint32 n , Sint32 sType)
{
	//-----------------------------------
	//武器の設定
	//-----------------------------------

	m_stAtack[n].sType = sType;

	switch(sType) {

	case enWeaponMachineGun:
	case enWeapon3Way:
		m_stAtack[n].sIcon      = enWeaponIconBullet;
		m_stAtack[n].sMaxRemain = 100;
		m_stAtack[n].sGaugeType = enWeaponGaugeMeter;
		break;

	case enWeaponArmHardBrow:
	case enWeaponArmGattling:
	case enWeaponArmStrikeCrow:
	case enWeaponArmZgokCrow:
	case enWeaponArmShieldArm:
		m_stAtack[n].sIcon = enWeaponIconPunch;
		m_stAtack[n].sMaxRemain = 1;
		m_stAtack[n].sGaugeType = enWeaponGaugeNone;
		break;

	case enWeaponBazooka:
		m_stAtack[n].sIcon = enWeaponIconMissile;
		m_stAtack[n].sMaxRemain = enBazookaMax;
		m_stAtack[n].sGaugeType = enWeaponGaugeTwo;
		break;

	case enWeaponNapalm:
		m_stAtack[n].sIcon = enWeaponIconMissile;
		m_stAtack[n].sMaxRemain = 100;
		m_stAtack[n].sGaugeType = enWeaponGaugeMeter;
		break;

	case enWeaponShotGun:
		m_stAtack[n].sIcon = enWeaponIconBullet;
		m_stAtack[n].sMaxRemain = 4;
		m_stAtack[n].sGaugeType = enWeaponGaugeFour;
		break;

	case enWeaponGrenade:
		m_stAtack[n].sIcon      = enWeaponIconRocket;
		m_stAtack[n].sMaxRemain = 2;
		m_stAtack[n].sGaugeType = enWeaponGaugeTwo;
		break;

	case enWeaponPodMissile:
		m_stAtack[n].sIcon = enWeaponIconRocket;
//		m_stAtack[n].sMaxRemain = 5;
//		m_stAtack[n].sGaugeType = enWeaponGaugeFour;
		m_stAtack[n].sMaxRemain = 100;
		m_stAtack[n].sGaugeType = enWeaponGaugeMeter;
		break;
	case enWeaponCannonLinar:
		m_stAtack[n].sIcon = enWeaponIconRocket;
		m_stAtack[n].sMaxRemain = 4;
		m_stAtack[n].sGaugeType = enWeaponGaugeFour;
		break;
	case enWeaponCannonGun:
		m_stAtack[n].sIcon      = enWeaponIconBullet;
		m_stAtack[n].sMaxRemain = 100;
		m_stAtack[n].sGaugeType = enWeaponGaugeMeter;
		break;
	}

	m_stAtack[n].Relord();

}


void CGunHound::ControlList( Sint32 sAtackType ,Sint32 n)
{
	//------------------------------------
	//コントロールリスト
	//------------------------------------
	if( m_bSwitchNow ) return;

	switch(sAtackType){
	case enWeaponMachineGun:	//実装（調整済み）//GAUGE
		MachineGun(n);
		break;

	case enWeapon3Way:			//実装（調整済み）//GAUGE
		ThreeWayShot(n);
		break;

	case enWeaponArmHardBrow:	//実装
		HardBrow(n);
		break;

	case enWeaponArmGattling:	//実装（調整済み）
		ArmGattling(n);
		break;

	case enWeaponArmStrikeCrow:	//実装
		StrikeCrow(n);
		break;

	case enWeaponArmZgokCrow:	//-----------------------------------------
		ZGokCrow(n);
		break;

	case enWeaponArmShieldArm:	//-----------------------------------------
		ShieldCrow(n);
		break;

	case enWeaponBazooka:		//実装（調整済み）//GAUGE
		Bazooka(n);
		break;

	case enWeaponGrenade:
		GrenadeCannon(n);
		break;

	case enWeaponShotGun:		//実装(調整済み)//GAUGE
		ShotGun(n);
		break;

	case enWeaponNapalm:		//-----------------------------------------
		Napalm(n);
		//ShotGun(n);
		break;

	case enWeaponPodMissile:	//実装（調整済み）//GAUGE
		MicroMisiile(n);
		break;

	case enWeaponCannonLinar:	//実装（調整済み）//GAUGE
		LinarCannon(n);
		break;

	case enWeaponCannonGun:		//jissou?
		GunCannon(n);
		break;

	default:
		break;
	}

}


void CGunHound::MachineGun(Sint32 n)
{
	//------------------------------------
	//マシンガンコントロール
	//------------------------------------

	if( m_stAtack[n].IsEmpty() )
	{
		return;
	}

	if( m_stAtack[n].sTimeLag > 0)
	{
		return;
	}

	ghPos stPos;
	Sint32 sDirection = (m_sDirection==DIR_RIGHT)? m_sArmRotation : (m_sArmRotation*-1+180);

	m_stAtack[n].sRotSpd = 100;

	m_stAtack[n].sTimeLag = 4;

	m_pMech->SetBodyAbsorber( 4 );
	m_pMech->SetArmRebound( 8 );


	m_pMech->SetPartsPos( enPartsMainShot , 36,-6 );
	m_pMech->getPosition( enPartsMainShot , &stPos );

	for(Sint32 ii=0;ii<2;ii++)
	{
		Sint32 sRot = sDirection+(-2+viiSub::Rand()%4)*4;
		viiMus::PlaySound( enSoundGun01 , m_bOutofScreen );
		viiEff::Set( EFF_MAZZLE , stPos.x , stPos.y , NULL );

		viiEff::Set(EFF_FIREFLOWER , stPos.x , stPos.y , sRot+-5+argset( viiSub::Rand()%10) );

		CEffGattlingGun *p;
		p = new CEffGattlingGun( stPos.x , stPos.y , sRot , 1250 );
		p->SetAtackPower( enAtkPointMachineGun+2 );
	}

	ghPos stPos1;
	m_pMech->getPosition( enPartsWeapon , &stPos1 );

	viiEff::Set(EFF_YAKKYOU , stPos1.x , stPos1.y , argset( (m_sDirection==DIR_LEFT)? -45 : 45*m_sDirection+180 ) );
	m_pPenduram->Set( 20 );

	m_stAtack[n].sRemain -= enMachingunOne;

	if( m_stAtack[n].IsEmpty() )
	{
		m_stAtack[n].sRemain = 0;
		m_stAtack[n].sRelordTime = 32;
	}

	m_stAtack[n].sRemainBullet = m_stAtack[n].sRemain;

}


void CGunHound::ThreeWayShot(Sint32 n)
{
	//------------------------------------
	//３ＷＡＹコントロール
	//------------------------------------

	if( m_stAtack[n].IsEmpty() )
	{
		return;
	}

	if( m_stAtack[n].sTimeLag > 0)
	{
		return;
	}

	ghPos stPos;
	Sint32 sDirection = (m_sDirection==DIR_RIGHT)? m_sArmRotation : (m_sArmRotation*-1+180);

	m_stAtack[n].sRotSpd = 100;

	m_stAtack[n].sTimeLag = 5;

	m_pMech->SetBodyAbsorber( 4 );
	m_pMech->SetArmRebound( 8 );

	m_pMech->SetPartsPos( enPartsMainShot , 36,-10 );
	m_pMech->getPosition( enPartsMainShot , &stPos );

	for(Sint32 ii=0;ii<4;ii++)
	{
		Sint32 sRot = sDirection+ii*10-15;//sDirection+(-20+viiSub::Rand()%40);

		viiMus::PlaySound( enSoundGun01 , m_bOutofScreen );
		viiEff::Set( EFF_MAZZLE , stPos.x , stPos.y , NULL );

		CEffGattlingGun *p;
		p = new CEffGattlingGun( stPos.x , stPos.y , sRot, 1450 );

#ifdef _VII_DEBUG_
		p->SetAtackPower( enAtkPointMachineGun*200 );
#endif

	}

	viiEff::Set(EFF_FIREFLOWER , stPos.x , stPos.y , /*sRot+-5+*/argset( viiSub::Rand()%10) );

	ghPos stPos1;
	m_pMech->getPosition( enPartsWeapon , &stPos1 );

	viiEff::Set(EFF_YAKKYOU , stPos1.x , stPos1.y , argset( (m_sDirection==DIR_LEFT)? -45 : 45*m_sDirection+180 ) );
	m_pPenduram->Set( 20 );

	m_stAtack[n].sRemain -= enThreeWay;

	if( m_stAtack[n].IsEmpty() )
	{
		m_stAtack[n].sRemain = 0;
		m_stAtack[n].sRelordTime = 32;
	}

	m_stAtack[n].sRemainBullet = m_stAtack[n].sRemain;
}


void CGunHound::Bazooka(Sint32 n)
{
	//------------------------------------
	//バズーカコントロール
	//------------------------------------

	if( m_stAtack[n].IsEmpty() )
	{
		return;
	}

	if( m_stAtack[n].sTimeLag > 0)
	{
		return;
	}
	if( m_stAtack[n].sRemain < enBazookaOne )
	{
		return;
	}

	ghPos stPos;
	Sint32 sDirection = (m_sDirection==DIR_RIGHT)? m_sArmRotation : (m_sArmRotation*-1+180);

	m_pMech->SetPartsPos( enPartsMainShot , 36,-6 );
	m_pMech->getPosition( enPartsMainShot , &stPos );

	m_pMech->SetBodyAbsorber( 4 );
	m_pMech->SetArmRebound( 8 );
	m_pPenduram->Set( -40 );

	//バズーカ
	viiEff::Set( EFF_MAZZLE , stPos.x , stPos.y , NULL );
	new CAtkH0001Bazooka(stPos.x , stPos.y , sDirection ,  24*100);
	viiMus::PlaySound( enSoundBazooka , m_bOutofScreen );

	m_stAtack[n].sRemain -= enBazookaOne;

	if( m_stAtack[n].IsEmpty() )
	{
		m_stAtack[n].sRemain = 0;
		m_stAtack[n].sRelordTime = 48;
	}

	m_stAtack[n].sTimeLag = 8; 

	m_stAtack[n].sRemainBullet = m_stAtack[n].sRemain;

}


void CGunHound::ShotGun(Sint32 n)
{
	//-----------------------------------------
	//ショットガン
	//-----------------------------------------

	if( m_stAtack[n].IsEmpty() )
	{
		return;
	}

	if( m_stAtack[n].sTimeLag > 0)
	{
		return;
	}
	if( m_stAtack[n].sRemain < enBazookaOne )
	{
		return;
	}

	ghPos stPos;
	Sint32 sDirection = (m_sDirection==DIR_RIGHT)? m_sArmRotation : (m_sArmRotation*-1+180);

	m_pMech->SetPartsPos( enPartsMainShot , 36,-6 );
	m_pMech->getPosition( enPartsMainShot , &stPos );

	m_pMech->SetBodyAbsorber( 4 );
	m_pMech->SetArmRebound( 8 );
	m_pPenduram->Set( -40 );


	for(Sint32 ii=0;ii<10;ii++)
	{
		viiEff::Set( EFF_MAZZLE , stPos.x-10+viiSub::Rand()%20 , stPos.y-10+viiSub::Rand()%20 , NULL );
		CEffShotGun *p;

		p = new CEffShotGun( stPos.x , stPos.y , sDirection+(-5+ii)*10 , 2050 +viiSub::Rand()%400);
		p->SetKantsu(gxTrue);
//		p->SetAtackPower( enAtkPointMachineGun*2 );

/*
		p = new CEffGattlingGun( stPos.x , stPos.y , sDirection+-20+viiSub::Rand()%40 , 2050 +viiSub::Rand()%400);
		p->SetAtackPower( enAtkPointMachineGun*2 );
		if( ii<3 )
		{
			p = new CEffGattlingGun( stPos.x , stPos.y , sDirection+-30+viiSub::Rand()%60 , 1850 +viiSub::Rand()%400);
			p->SetAtackPower( enAtkPointMachineGun*2 );
		}
*/

	}

	m_stAtack[n].sRemain -= enBazookaOne;

	if( m_stAtack[n].IsEmpty() )
	{
		m_stAtack[n].sRemain = 0;
		m_stAtack[n].sRelordTime = 48;
		for(Sint32 ii=0;ii<10;ii++)
		{
			viiEff::Set(EFF_YAKKYOU , m_Pos.x , m_Pos.y-2400 , argset( viiSub::SetKakudo( m_sDirection , 220+viiSub::Rand()%40) ) );
		}
	}

	m_stAtack[n].sTimeLag = 8; 

	m_stAtack[n].sRemainBullet = m_stAtack[n].sRemain;

}


void CGunHound::Napalm(Sint32 n)
{
	//------------------------------------
	//ナパームスロワー
	//------------------------------------

	if( m_stAtack[n].IsEmpty() )
	{
		return;
	}

	if( m_stAtack[n].sTimeLag > 0)
	{
		return;
	}

	ghPos stPos;
	Sint32 sDirection = (m_sDirection==DIR_RIGHT)? m_sArmRotation : (m_sArmRotation*-1+180);

	m_stAtack[n].sRotSpd = 100;

	m_stAtack[n].sTimeLag = 2;

	m_pMech->SetBodyAbsorber( 4 );
//	m_pMech->SetArmRebound( 8 );

	m_pMech->SetPartsPos( enPartsMainShot , 52,-8 );
	m_pMech->getPosition( enPartsMainShot , &stPos );

	for(Sint32 ii=0;ii<2;ii++)
	{
		Sint32 sRot = sDirection+(-3+viiSub::Rand()%6)*3;
//		viiMus::PlaySound( enSoundGun01 , m_bOutofScreen );
		CEffNapalmGun *p;
		p = new CEffNapalmGun( stPos.x , stPos.y , sRot , 800+viiSub::Rand()%400 );
	}

	m_pPenduram->Set( 20 );

	m_stAtack[n].sRemain -= 2;

	if( m_stAtack[n].IsEmpty() )
	{
		m_stAtack[n].sRemain = 0;
		m_stAtack[n].sRelordTime = 64;
	}

	m_stAtack[n].sRemainBullet = m_stAtack[n].sRemain;

}


void CGunHound::MicroMisiile(Sint32 n)
{
	//------------------------------------
	//ミサイルコントロール
	//------------------------------------

	if( m_stAtack[n].sTimeLag > 0) return;
	if( m_stAtack[n].IsEmpty()   ) return;

	ghPos stPos;

	Sint32 sDirection;

	sDirection = m_sArmRotation/4-36+(viiSub::Rand()%3)*9;
	sDirection = (m_sDirection==DIR_RIGHT)? sDirection : (sDirection*-1+180);

	m_pMech->SetPartsPos( enPartsShot1 , -6,-16 );
	m_pMech->getPosition( enPartsShot1 , &stPos );

	viiEff::Set( EFF_MAZZLE , stPos.x , stPos.y , NULL );

	CAtkH0001Missile *pMissile;
	pMissile = new CAtkH0001Missile( stPos.x , stPos.y+(-1+viiSub::Rand()%3)*300 ,sDirection,100);
	pMissile->SetHomingLevel( 2 );
	pMissile->SetWait( 64 );

	m_pPenduram->Set( -20 );
	m_pMech->SetBodyAbsorber( 4 );

	viiMus::PlaySound( enSoundMissile , m_bOutofScreen );

	m_stAtack[n].sTimeLag = 6;

	m_stAtack[n].sRemain -= 20;

	if( m_stAtack[n].IsEmpty() )
	{
		m_stAtack[n].sRemain = 0;
		m_stAtack[n].sRelordTime = 60;
	}

	m_stAtack[n].sRemainBullet = m_stAtack[n].sRemain;

}


void CGunHound::GrenadeCannon(Sint32 n)
{
	//------------------------------------
	//グレネードキャノンコントロール
	//------------------------------------

	if( m_stAtack[n].sTimeLag > 0) return;
	if( m_stAtack[n].IsEmpty()   ) return;

	Sint32 ax,ay;

	Sint32 sDirection;
	sDirection = -55;//52-5+(viiSub::Rand()%10);//m_sArmRotation/4;
	sDirection = (m_sDirection==DIR_RIGHT)? sDirection : (sDirection*-1+180);

//	ghPos stPos;
//	m_pMech->SetPartsPos( enPartsShot1 , -12+m_stAtack[n].sRemain*3 , -32 );
//	m_pMech->getPosition( enPartsShot1 , &stPos );
//	viiEff::Set( EFF_MAZZLE , stPos.x , stPos.y , NULL );

	//----------------------------------------------------
//	ax = stPos.x;
//	ay = stPos.y+(-viiSub::Rand()%3)*100;

	ax = m_Pos.x;
	ay = m_Pos.y-3200;

	//グレネード弾
	viiEff::Set( EFF_MAZZLE , ax , ay , NULL );
	CAtkH0001Grenade *p;
	p = new CAtkH0001Grenade(  ax , ay , sDirection , 820/*+viiSub::Rand()%100*/ );

	//加速度をつける

	Sint32 sAdd = m_Add.x;
	if( m_sDirection == DIR_RIGHT && sAdd < 0 )
	{
		sAdd=sAdd/4;
	}
	else if( m_sDirection == DIR_LEFT && sAdd > 0 )
	{
		sAdd=sAdd/4;
	}

	p->AddSpeed( sAdd , 0);

	viiMus::PlaySound( enSoundBazooka , m_bOutofScreen );

	m_pPenduram->Set( -20 );
	m_pMech->SetBodyAbsorber( 4 );

	viiMus::PlaySound( enSoundMissile , m_bOutofScreen );

	//----------------------------------------------------

	m_stAtack[n].sTimeLag = 8;

	m_stAtack[n].sRemain --;

	if( m_stAtack[n].IsEmpty() )
	{
		m_stAtack[n].sRemain = 0;
		m_stAtack[n].sRelordTime = 90;
	}

	m_stAtack[n].sRemainBullet = m_stAtack[n].sRemain;
}


void CGunHound::LinarCannon(Sint32 n)
{
	//------------------------------------
	//ライナーキャノンコントロール
	//------------------------------------

	if( m_stAtack[n].sTimeLag > 0) return;
	if( m_stAtack[n].IsEmpty()   ) return;

	Sint32 ax,ay;
	ghPos stPos;

	Sint32 sDirection;
	sDirection = m_sArmRotation/4-25+(viiSub::Rand()%3)*6;
	sDirection = (m_sDirection==DIR_RIGHT)? sDirection : (sDirection*-1+180);

	m_pMech->SetPartsPos( enPartsShot1 , -12+m_stAtack[n].sRemain*3 , -32 );
	m_pMech->getPosition( enPartsShot1 , &stPos );

	viiEff::Set( EFF_MAZZLE , stPos.x , stPos.y , NULL );

	//----------------------------------------------------
	ax = stPos.x;
	ay = stPos.y+(-viiSub::Rand()%3)*100;

	//バズーカ
	viiEff::Set( EFF_MAZZLE , stPos.x , stPos.y , NULL );
	new CAtkH0001Bazooka(stPos.x , stPos.y , sDirection ,  24*100);
	viiMus::PlaySound( enSoundBazooka , m_bOutofScreen );

	m_pPenduram->Set( -20 );
	m_pMech->SetBodyAbsorber( 4 );

	viiMus::PlaySound( enSoundMissile , m_bOutofScreen );

	//----------------------------------------------------


	m_stAtack[n].sTimeLag = 8;

	m_stAtack[n].sRemain --;

	if( m_stAtack[n].IsEmpty() )
	{
		m_stAtack[n].sRemain = 0;
		m_stAtack[n].sRelordTime = 90;
	}

	m_stAtack[n].sRemainBullet = m_stAtack[n].sRemain;
}


void CGunHound::GunCannon(Sint32 n)
{
	//------------------------------------
	//対空ガトリングガンコントロール
	//------------------------------------

	if( m_stAtack[n].IsEmpty() )
	{
		return;
	}

	if( m_stAtack[n].sTimeLag > 0)
	{
		return;
	}

	Sint32 sRot = -25;
	ghPos stPos;
	Sint32 sDirection = (m_sDirection==DIR_RIGHT)? sRot : (sRot*-1+180);

	m_stAtack[n].sRotSpd = 100;

	m_stAtack[n].sTimeLag = 2;

	m_pMech->SetBodyAbsorber( 4 );
//	m_pMech->SetArmRebound( 8 );

	m_pMech->SetPartsPos( enPartsShot1 , -10,-34 );
	m_pMech->getPosition( enPartsShot1 , &stPos );

	Sint32 sRand = 0;
	for(Sint32 ii=0;ii<2;ii++)
	{
		sRand = viiSub::Rand();
		Sint32 sRot = sDirection+(-5+sRand%10)*3;
		viiMus::PlaySound( enSoundGun01 , m_bOutofScreen );
		viiEff::Set( EFF_MAZZLE , stPos.x , stPos.y , NULL );

		viiEff::Set(EFF_FIREFLOWER , stPos.x , stPos.y , sRot+-10+argset( sRand%20) );

		CEffGattlingGun *p;
 		p = new CEffGattlingGun( stPos.x , stPos.y , sRot , 1450 );
		//p->SetAtackPower( enAtkPointMachineGun );
	}

	ghPos stPos1;
	m_pMech->getPosition( enPartsWeapon , &stPos1 );
	viiEff::Set(EFF_YAKKYOU , stPos1.x , stPos1.y-16*100 , argset( (m_sDirection==DIR_LEFT)? 25 : -25*m_sDirection+180 ) );

	m_pPenduram->Set( 20 );

	m_stAtack[n].sRemain -= enMachingunOne;

	if( m_stAtack[n].IsEmpty() )
	{
		m_stAtack[n].sRemain = 0;
		m_stAtack[n].sRelordTime = 12;
	}

	m_stAtack[n].sRemainBullet = m_stAtack[n].sRemain;


}


void CGunHound::HardBrow(Sint32 n)
{
	//------------------------------------
	//パンチコントロール
	//------------------------------------
	ghPos stPos;
	OBJ_POS_T PunchPos;
	Sint32 sSeq = m_stPunch.timelag;

	m_pMech->getPosition( enPartsArmL , &stPos );

	PunchPos.x = stPos.x;
	PunchPos.y = stPos.y;

	//------------------------------------------------
	//弾丸イレーサー処理
	//------------------------------------------------
	if( sSeq <= 25 )
	{
		m_HitBarrier.ax1 = -8*m_sDirection;
		m_HitBarrier.ax2 =  32*m_sDirection;
		m_HitBarrier.ay1 = -24;
		m_HitBarrier.ay2 =  32;

		if( m_HitBarrier.m_sHP!=100*100)
		{
			//弾丸けし成功
			viiEff::Set(EFF_RING,stPos.x, stPos.y,argset(250));
		}
		m_HitBarrier.set_hp(100);
		m_HitBarrier.SetHantei( ID_VERSIS_DEF , &PunchPos );

/*
		//アームローテーション制御
		if( m_sArmRotation < 45 ) m_sArmRotation += (45-m_sArmRotation)/3;
*/

	}

	if( sSeq < 10 )
	{
		//薬きょうエフェクト
		new CEffDashStorm( stPos.x+1600*m_sDirection , stPos.y, viiSub::SetKakudo(m_sDirection,180) , 60 );
	}

	switch(sSeq){
	//------------------------------------------------
	//パンチ初期化
	//------------------------------------------------
	case 1:
		m_bOffenceControll = gxFalse;
		//viiEff::Set(EFF_RING,stPos.x, stPos.y,argset(250) );
		m_HitBarrier.SetOffImpact();
		m_pMech->SetPunchMotion( gxTrue , 0 );
		break;
	//------------------------------------------------
	//パンチ判定出現
	//------------------------------------------------
	case 5:
		{

			Sint32 sDamage = 3*(enAtkPointGun*1)/2;
			sDamage = sDamage + (enAtkPointGun*m_stPunch.sAtackCnt*3);
			sDamage = (sDamage >= 99)? 99 : sDamage;
			m_stPunch.m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
			m_stPunch.m_HitAtari.set_hp(100);
			m_stPunch.m_HitAtari.set_ap( sDamage);
		}
		m_stPunch.m_HitAtari.ax1 = -32;
		m_stPunch.m_HitAtari.ax2 =  28;
		m_stPunch.m_HitAtari.ay1 = -16;
		m_stPunch.m_HitAtari.ay2 =  16;
		m_pMech->getPosition( enPartsArmL , &stPos );
		viiEff::Set(EFF_YAKKYOU , stPos.x , stPos.y , argset( viiSub::SetKakudo(m_sDirection,180+45) ) );
		m_pPenduram->Set( -40 );
		m_stPunch.m_HitAtari.m_bImpact = gxFalse;
		m_stPunch.m_HitAtari.SetHantei( ID_VERSIS_ATK , &PunchPos );
		break;
	//------------------------------------------------
	//パンチ判定
	//------------------------------------------------
	case 6:
		if( viiSub::GetBoundBlock_Player(stPos.x,stPos.y) )
		{
			m_Add.x = -120*m_sDirection;
			viiEff::Set( EFF_HITFLASH , stPos.x , stPos.y , argset(1) );
			m_stPunch.m_HitAtari.m_bImpact = gxTrue;
			viiMus::PlaySound( enSoundPunch , m_bOutofScreen );
		}
		else if( m_stPunch.m_HitAtari.IsImpacted())
		{
			CCockpit::GetInstance()->SetHitStop(4);
			m_Add.x += -120*m_sDirection;
//			new CEffectDmgprt( stPos.x , stPos.y, viiSub::SetKakudo(m_sDirection,180+45) , 200 );
//			new CEffectDmgprt( stPos.x , stPos.y, viiSub::SetKakudo(m_sDirection,180+35) , 200 );
			for(Sint32 nnn=0;nnn<18;nnn++)
			{
				viiEff::Set(EFF_FIREFLOWER , stPos.x , stPos.y , argset( viiSub::Rand()%10) );
			}
			viiMus::PlaySound( enSoundGachan , m_bOutofScreen );
		}
		else
		{
			viiMus::PlaySound( enSoundPunch , m_bOutofScreen );
		}
		if( m_HitBarrier.IsImpacted() )
		{
			viiMus::PlaySound( enSoundShotErase , m_bOutofScreen );
		}
		break;
	//------------------------------------------------
	//パンチキャンセル受付
	//------------------------------------------------
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
		if((m_HitBarrier.IsImpacted() || m_stPunch.m_HitAtari.m_bImpact) && m_sControlTrig&BTN_Y)
		{
			//フラッシュピストンパンチ
			m_stPunch.m_HitAtari.SetOffImpact();
			m_stPunch.timelag = 0;
			m_pMech->SetPunchMotion( gxTrue , 100 );
			if( m_stPunch.sAtackCnt < 10 )
			{
				//攻撃力追加
				m_stPunch.sAtackCnt ++;
			}
		}
		break;

	case 25:
		m_bOffenceControll = gxTrue;
		break;
	case 40:
		//パンチ終了
		m_stPunch.timelag = 0;
		return;
	}

	m_stPunch.timelag ++;

}


void CGunHound::ArmGattling(Sint32 n)
{
	//------------------------------------------------------
	//ガトリングパンチコントロール(ガトリングガンを装備)
	//------------------------------------------------------
	ghPos stPos;
	OBJ_POS_T PunchPos;
	Sint32 sSeq = m_stPunch.timelag;

	m_pMech->getPosition( enPartsArmL , &stPos );

	PunchPos.x = stPos.x;
	PunchPos.y = stPos.y;

	//------------------------------------------------
	//弾丸イレーサー処理
	//------------------------------------------------
	if( sSeq <= 25 )
	{
		m_HitBarrier.ax1 = -8*m_sDirection;
		m_HitBarrier.ax2 =  32*m_sDirection;
		m_HitBarrier.ay1 = -24;
		m_HitBarrier.ay2 =  32;

		if( m_HitBarrier.m_sHP!=100*100)
		{
			//弾丸けし成功
			viiEff::Set(EFF_RING,stPos.x, stPos.y,argset(250));
		}
		m_HitBarrier.set_hp(100);
		m_HitBarrier.SetHantei( ID_VERSIS_DEF , &PunchPos );

/*
		//アームローテーション制御
		if( m_sArmRotation < 45 ) m_sArmRotation += (45-m_sArmRotation)/3;
*/
	}

	switch(sSeq){
	//------------------------------------------------
	//パンチ初期化
	//------------------------------------------------
	case 1:
		viiMus::PlaySound( enSoundPunch , m_bOutofScreen );
		m_HitBarrier.SetOffImpact();
		m_pMech->SetPunchMotion( gxTrue , 0 );
		break;

	//------------------------------------------------
	//パンチ判定出現
	//------------------------------------------------
	case 5:
		{
			Sint32 sDamage = enAtkPointGun*2;
			sDamage = sDamage + (enAtkPointGun*m_stPunch.sAtackCnt);
			sDamage = (sDamage >= 99)? 99 : sDamage;
			m_stPunch.m_HitAtari.set_hp(100);
			m_stPunch.m_HitAtari.set_ap( sDamage);
		}
		m_stPunch.m_HitAtari.ax1 = -20;
		m_stPunch.m_HitAtari.ax2 =  28;
		m_stPunch.m_HitAtari.ay1 = -16;
		m_stPunch.m_HitAtari.ay2 =  16;

		m_pMech->getPosition( enPartsArmL , &stPos );
		viiEff::Set(EFF_YAKKYOU , stPos.x , stPos.y , argset( viiSub::SetKakudo(m_sDirection,180+45) ) );
		m_pPenduram->Set( -40 );
		m_stPunch.m_HitAtari.m_bImpact = gxFalse;
		m_stPunch.m_HitAtari.SetHantei( ID_VERSIS_ATK , &PunchPos );
		break;

	//------------------------------------------------
	//パンチ判定
	//------------------------------------------------
	case 6:
		if( viiSub::GetBoundBlock_Player(stPos.x,stPos.y) )
		{
			m_Add.x = -180*m_sDirection;
			viiEff::Set( EFF_HITFLASH , stPos.x , stPos.y , argset(1) );
			m_stPunch.m_HitAtari.m_bImpact = gxTrue;
			viiMus::PlaySound( enSoundPunch , m_bOutofScreen );
		}
		else if( m_stPunch.m_HitAtari.IsImpacted())
		{
			CCockpit::GetInstance()->SetHitStop(3);
			m_Add.x += -180*m_sDirection;
			viiEff::Set( EFF_HITFLASH , stPos.x , stPos.y , argset(1) );
			viiMus::PlaySound( enSoundPunch , m_bOutofScreen );
		}
		if( m_HitBarrier.IsImpacted() )
		{
			viiMus::PlaySound( enSoundShotErase , m_bOutofScreen );
		}
		break;
	//------------------------------------------------
	//パンチキャンセル受付
	//------------------------------------------------
	case 20:
		m_stPunch.m_HitAtari.m_bImpact = gxFalse;
		m_stPunch.m_HitAtari.SetHantei( ID_VERSIS_ATK , &PunchPos );
		m_stPunch.m_HitAtari.set_ap( 12 );

	case 21:
	case 22:
	case 23:
		//if((m_HitBarrier.IsImpacted()||m_stPunch.m_HitAtari.m_bImpact) && Joy[0].psh&BTN_Y)

		if( m_sControlPush&BTN_Y && m_stPunch.sAtackCnt<32 )
		{
			//フラッシュピストンパンチ
			Sint32 ax,ay,ax2;

			ax  = stPos.x+1600*m_sDirection;
			ax2 = stPos.x+2400*m_sDirection;
			ay  = stPos.y + (-2+m_stPunch.sAtackCnt%4)*300;

			m_stPunch.m_HitAtari.SetOffImpact();
			m_stPunch.timelag = 16;
			m_pMech->SetPunchMotion( gxTrue , 100 );
			new CEffDashStorm  ( ax , stPos.y , 180 , 60 );
			CEffShotGun *p;
			p = new CEffShotGun( ax , ay , viiSub::SetKakudo( m_sDirection , (-2+m_stPunch.sAtackCnt%4)*4 )   , 1250 );
			//p->SetAtackPower( enAtkPointMachineGun );
			p->SetKantsu(gxTrue);
			viiEff::Set(EFF_YAKKYOU , stPos.x , stPos.y , argset( (m_sDirection==DIR_LEFT)? -45 : 45*m_sDirection+180 ) );
			viiEff::Set( EFF_MAZZLE , ax2 , ay , NULL );
			viiMus::PlaySound( enSoundGun01 , m_bOutofScreen );
			m_stPunch.sAtackCnt ++;
		}
		break;

	case 64:
		//パンチ終了
		m_stPunch.timelag = 0;
		return;
	}

	m_stPunch.timelag ++;

}


void CGunHound::StrikeCrow(Sint32 n)
{
	//--------------------------------------------------------
	//ストライククロウコントロール（パンチ＋バズーカ機能アリ）
	//--------------------------------------------------------
	ghPos stPos;
	OBJ_POS_T PunchPos;
	Sint32 sSeq = m_stPunch.timelag;

	m_pMech->getPosition( enPartsArmL , &stPos );

	PunchPos.x = stPos.x;
	PunchPos.y = stPos.y;


	//------------------------------------------------
	//弾丸イレーサー処理
	//------------------------------------------------
	if( sSeq <= 25 )
	{
		m_HitBarrier.ax1 = -8*m_sDirection;
		m_HitBarrier.ax2 =  32*m_sDirection;
		m_HitBarrier.ay1 = -24;
		m_HitBarrier.ay2 =  32;

		if( m_HitBarrier.m_sHP!=100*100)
		{
			//弾丸けし成功
			viiEff::Set(EFF_RING,stPos.x, stPos.y,argset(250));
		}
		m_HitBarrier.set_hp(100);
		m_HitBarrier.SetHantei( ID_VERSIS_DEF , &PunchPos );

/*
		//アームローテーション制御
		if( m_sArmRotation < 45 ) m_sArmRotation += (45-m_sArmRotation)/3;
*/
	}

	if( sSeq < 10 )
	{
		//薬きょうエフェクト
		new CEffDashStorm( stPos.x+1600*m_sDirection , stPos.y, viiSub::SetKakudo(m_sDirection,180) , 60 );
	}

	switch(sSeq){
	//------------------------------------------------
	//パンチ初期化
	//------------------------------------------------
	case 1:
		m_bOffenceControll = gxFalse;
		m_HitBarrier.SetOffImpact();
		m_pMech->SetPunchMotion( gxTrue , 0 );
		break;

	//------------------------------------------------
	//パンチ判定出現
	//------------------------------------------------
	case 5:
		{
			Sint32 sDamage = enAtkPointGun*4;
			sDamage = sDamage + (enAtkPointGun*m_stPunch.sAtackCnt);
			sDamage = (sDamage >= 99)? 99 : sDamage;
			m_stPunch.m_HitAtari.set_hp(100);
			m_stPunch.m_HitAtari.set_ap( sDamage);
		}
		m_stPunch.m_HitAtari.ax1 = -8;
		m_stPunch.m_HitAtari.ax2 =  28;
		m_stPunch.m_HitAtari.ay1 = -16;
		m_stPunch.m_HitAtari.ay2 =  16;
		m_pMech->getPosition( enPartsArmL , &stPos );
		viiEff::Set(EFF_YAKKYOU , stPos.x , stPos.y , argset( viiSub::SetKakudo(m_sDirection,180+45) ) );
		m_pPenduram->Set( -40 );
		m_stPunch.m_HitAtari.m_bImpact = gxFalse;
		m_stPunch.m_HitAtari.SetHantei( ID_VERSIS_ATK , &PunchPos );
		break;
	//------------------------------------------------
	//パンチ判定
	//------------------------------------------------
	case 6:
		if( viiSub::GetBoundBlock_Player(stPos.x,stPos.y) )
		{
			viiEff::Set( EFF_HITFLASH , stPos.x , stPos.y , argset(1) );
			m_stPunch.m_HitAtari.m_bImpact = gxTrue;
			m_Add.x = -180*m_sDirection;
			viiMus::PlaySound( enSoundPunch , m_bOutofScreen );
		}
		else if( m_stPunch.m_HitAtari.IsImpacted())
		{
			CCockpit::GetInstance()->SetHitStop(3);
			m_Add.x += -180*m_sDirection;
			viiEff::Set( EFF_HITFLASH , stPos.x , stPos.y , argset(1) );
			for(Sint32 nnn=0;nnn<18;nnn++)
			{
				viiEff::Set(EFF_FIREFLOWER , stPos.x , stPos.y , argset( viiSub::Rand()%10) );
			}
			viiMus::PlaySound( enSoundPunch , m_bOutofScreen );
		}
		if( m_HitBarrier.IsImpacted() )
		{
			viiMus::PlaySound( enSoundShotErase , m_bOutofScreen );
		}
		break;
	//------------------------------------------------
	//パンチキャンセル受付
	//------------------------------------------------
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
		if((m_HitBarrier.IsImpacted() || m_stPunch.m_HitAtari.m_bImpact ) && m_sControlTrig&BTN_Y)
		{
			//フラッシュピストンパンチ
			Sint32 ax,ay;

			ax  = stPos.x + 2400*m_sDirection;
			ay  = stPos.y + (-2+m_stPunch.sAtackCnt%4)*200;

			m_stPunch.m_HitAtari.SetOffImpact();
			m_stPunch.timelag = 0;
			m_pMech->SetPunchMotion( gxTrue , 100 );
			if( m_stPunch.sAtackCnt < 10 )
			{
				//攻撃力追加
				m_stPunch.sAtackCnt ++;
			}

			new CAtkH0001Bazooka( stPos.x , stPos.y , viiSub::SetKakudo( m_sDirection , (-2+m_stPunch.sAtackCnt%4)*2 ) ,  24*100);
		}
		break;

	case 25:
		m_bOffenceControll = gxTrue;
		break;
	case 40:
		//パンチ終了
		m_stPunch.timelag = 0;
		return;
	}

	m_stPunch.timelag ++;


}


void CGunHound::ShieldCrow(Sint32 n)
{
	//-----------------------------------------------------
	//シールドクロウコントロール(攻撃力なし/連続防御可能)
	//-----------------------------------------------------
	ghPos stPos;
	OBJ_POS_T PunchPos;
	Sint32 sSeq = m_stPunch.timelag;

	m_pMech->getPosition( enPartsArmL , &stPos );

	PunchPos.x = stPos.x;
	PunchPos.y = stPos.y;

	//------------------------------------------------
	//弾丸イレーサー処理
	//------------------------------------------------
	if( sSeq <= 30 )
	{
		m_HitBarrier.ax1 = -16;
		m_HitBarrier.ax2 =  32;
		m_HitBarrier.ay1 = -24;
		m_HitBarrier.ay2 =  32;

		if( m_HitBarrier.m_sHP!=100*100)
		{
			//弾丸けし成功
			viiEff::Set(EFF_RING,stPos.x, stPos.y,argset(250));
		}
		m_HitBarrier.set_hp(100);
		m_HitBarrier.SetHantei( ID_VERSIS_DEF , &PunchPos );

/*
		//アームローテーション制御
		if( m_sArmRotation < 45 ) m_sArmRotation += (45-m_sArmRotation)/3;
*/
	}


	switch(sSeq){
	//------------------------------------------------
	//パンチ初期化
	//------------------------------------------------
	case 1:
		m_bOffenceControll = gxFalse;
		m_HitBarrier.SetOffImpact();
		m_pMech->SetPunchMotion( gxTrue , 0 , 60 );
		break;

	//------------------------------------------------
	//パンチ判定出現
	//------------------------------------------------
	case 5:
		{
			Sint32 sDamage = (m_stPunch.sAtackCnt+2);
			sDamage = enAtkPointMissile+sDamage*sDamage;
			sDamage = (sDamage >= 99)? 99 : sDamage;
			sDamage = (sDamage < enAtkPointMissile )? enAtkPointMissile : sDamage;
			m_stPunch.m_HitAtari.set_hp(100);
			m_stPunch.m_HitAtari.set_ap( sDamage);
		}
		m_stPunch.m_HitAtari.ax1 = -8;
		m_stPunch.m_HitAtari.ax2 =  28;
		m_stPunch.m_HitAtari.ay1 = -16;
		m_stPunch.m_HitAtari.ay2 =  16;
		m_pMech->getPosition( enPartsArmL , &stPos);
		m_pPenduram->Set( -40 );
		m_stPunch.m_HitAtari.m_bImpact = gxFalse;
		m_stPunch.m_HitAtari.SetHantei( ID_VERSIS_ATK , &PunchPos );
		break;
	//------------------------------------------------
	//パンチ判定
	//------------------------------------------------
	//------------------------------------------------
	//パンチキャンセル受付
	//------------------------------------------------
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
		if( m_sControlPush&BTN_Y )
		{
			//フラッシュピストンパンチ
			Sint32 ax,ay;

			ax  = stPos.x + 2400*m_sDirection;
			ay  = stPos.y + (-2+m_stPunch.sAtackCnt%4)*200;

			m_stPunch.m_HitAtari.SetOffImpact();
			m_stPunch.timelag = 14;
			m_pMech->SetPunchMotion( gxTrue , 142 ,60);
			if( m_stPunch.sAtackCnt < 10 )
			{
				//攻撃力追加
				m_stPunch.sAtackCnt ++;
			}
		}
		break;

	case 30:
		m_bOffenceControll = gxTrue;
		break;
	case 40:
		//パンチ終了
		m_stPunch.timelag = 0;
		return;
	}

	m_stPunch.timelag ++;


}


void CGunHound::ZGokCrow(Sint32 n)
{
	//----------------------------------------------------
	//ズゴッククロー（早い、押しっぱなしで連射可能）
	//----------------------------------------------------
	ghPos stPos;
	OBJ_POS_T PunchPos;
	Sint32 sSeq = m_stPunch.timelag;

	m_pMech->getPosition( enPartsArmL , &stPos );

	PunchPos.x = stPos.x;
	PunchPos.y = stPos.y;

	//------------------------------------------------
	//弾丸イレーサー処理
	//------------------------------------------------
	if( sSeq <= 25 )
	{
		m_HitBarrier.ax1 = -24*m_sDirection;
		m_HitBarrier.ax2 =  24*m_sDirection;
		m_HitBarrier.ay1 = -24;
		m_HitBarrier.ay2 =  24;

		if( m_HitBarrier.m_sHP!=100*100)
		{
			//弾丸けし成功
			viiEff::Set(EFF_RING,stPos.x, stPos.y,argset(250));
		}
		m_HitBarrier.set_hp(100);
		m_HitBarrier.SetHantei( ID_VERSIS_DEF , &PunchPos );

/*
		//アームローテーション制御
		if( m_sArmRotation < 45 ) m_sArmRotation += (45-m_sArmRotation)/3;
*/
	}

	if( sSeq < 10 )
	{
		//薬きょうエフェクト
		new CEffDashStorm( stPos.x+1600*m_sDirection , stPos.y, viiSub::SetKakudo(m_sDirection,180) , 60 );
	}

	switch(sSeq){
	//------------------------------------------------
	//パンチ初期化
	//------------------------------------------------
	case 1:
		m_bOffenceControll = gxTrue;
		viiEff::Set(EFF_RING,stPos.x, stPos.y,argset(250) );
		m_HitBarrier.SetOffImpact();
		m_pMech->SetPunchMotion( gxTrue , 0 );
		break;
	//------------------------------------------------
	//パンチ判定出現
	//------------------------------------------------
	case 5:
		m_bOffenceControll = gxFalse;
		{
			Sint32 sDamage = enAtkPointGun*2;
			sDamage = sDamage + (enAtkPointGun*m_stPunch.sAtackCnt);
			sDamage = (sDamage >= 99)? 99 : sDamage;
			m_stPunch.m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
			m_stPunch.m_HitAtari.set_hp(100);
			m_stPunch.m_HitAtari.set_ap( sDamage);
		}
		m_stPunch.m_HitAtari.ax1 = -32;
		m_stPunch.m_HitAtari.ax2 =  48;
		m_stPunch.m_HitAtari.ay1 = -16;
		m_stPunch.m_HitAtari.ay2 =  16;
		m_pMech->getPosition( enPartsArmL , &stPos );
		viiEff::Set(EFF_YAKKYOU , stPos.x , stPos.y , argset( viiSub::SetKakudo(m_sDirection,180+45) ) );
		m_pPenduram->Set( -40 );
		m_stPunch.m_HitAtari.m_bImpact = gxFalse;
		m_stPunch.m_HitAtari.SetHantei( ID_VERSIS_ATK , &PunchPos );
		break;
	//------------------------------------------------
	//パンチ判定
	//------------------------------------------------
	case 6:
		if( viiSub::GetBoundBlock_Player(stPos.x,stPos.y) )
		{
			viiEff::Set( EFF_HITFLASH , stPos.x , stPos.y , argset(1) );
			m_stPunch.m_HitAtari.m_bImpact = gxTrue;
			//特別にノックバックペナルティなし
			//m_Add.x = -180*m_sDirection;
			viiMus::PlaySound( enSoundPunch , m_bOutofScreen );
		}
		else if( m_stPunch.m_HitAtari.IsImpacted())
		{
//			new CEffectDmgprt( stPos.x , stPos.y, viiSub::SetKakudo(m_sDirection,180+45) , 200 );
//			new CEffectDmgprt( stPos.x , stPos.y, viiSub::SetKakudo(m_sDirection,180+35) , 200 );
			for(Sint32 nnn=0;nnn<18;nnn++)
			{
				viiEff::Set(EFF_FIREFLOWER , stPos.x , stPos.y , argset( viiSub::Rand()%10) );
			}

			CCockpit::GetInstance()->SetHitStop(3);
			//特別にノックバックペナルティなし
			//m_Add.x += -180*m_sDirection;
			viiEff::Set( EFF_HITFLASH , stPos.x , stPos.y , argset(1) );
			viiMus::PlaySound( enSoundGachan , m_bOutofScreen );
		}
		else
		{
			viiMus::PlaySound( enSoundPunch , m_bOutofScreen );
		}
		if( m_HitBarrier.IsImpacted() )
		{
			viiMus::PlaySound( enSoundShotErase , m_bOutofScreen );
		}
		break;
	//------------------------------------------------
	//パンチキャンセル受付
	//------------------------------------------------
	case 12:
		if( (m_HitBarrier.IsImpacted() || m_stPunch.m_HitAtari.m_bImpact) && (m_sControlPush&BTN_Y || m_sControlPush&BTN_X)  )
		{
			//フラッシュピストンパンチ
			m_stPunch.m_HitAtari.SetOffImpact();
			m_stPunch.timelag = 0;
			m_pMech->SetPunchMotion( gxTrue , 100 );
			if( m_stPunch.sAtackCnt < 10 )
			{
				//攻撃力追加
				m_stPunch.sAtackCnt ++;
			}
		}
		break;

	case 25:
		m_bOffenceControll = gxTrue;
		break;
	case 40:
		//パンチ終了
		m_stPunch.timelag = 0;
		return;
	}

	m_stPunch.timelag ++;

}


