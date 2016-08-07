//--------------------------------------------------------------------------------
//
// Stage01:ホバー戦車
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "EnemyTbl/CEneH0102NightSpiderTbl.h"
#include "../../Effect/EffectBase.h"

enum {
	enScrollOutWidth  = 256*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  =  64,
	enKuraiTop   = -120,
	enKuraiRight =  -64,
	enKuraiBottom=  -80,

	enScore = 80000,
	enMaxSpeed = 480,
	enSpeedAdd = 4,
};

Sint32 sShieldTbl[][4]=
{
	{180,-80,128,-16},					//前足
	{180-32,-80-16,128-32,-16-16},		//前足
	{180-64,-80-40,128-48,-16-48},		//前足

	{-32,-192,-148,-130},				//腹

	{-48,-100,-180,-64},				//後ろ足
	{-48-32,-100+32,-180-16,-64+32},	//後ろ足
	{64,-120,-64,-80},
	{-8,-8,8,8},
	{-8,-8,8,8},
	{-8,-8,8,8},
	{-8,-8,8,8},
	{-8,-8,8,8},
	{-8,-8,8,8},
	{-8,-8,8,8},
	{-8,-8,8,8},
	{-8,-8,8,8},
};

ghPos CEneH0102NightSpider::m_TgtPos;

CEneH0102NightSpider::CEneH0102NightSpider( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	Sint32 ii;

	m_bDisp = gxTrue;
	m_Pos.x = x;
	m_Pos.y = y;

	m_Base.x = x;
	m_Base.y = x;

	m_sDirection = DIR_LEFT;
	m_sAtribute  = ATR_DFLT;

	m_stObj.pos.x = m_Pos.x;
	m_stObj.pos.y = m_Pos.y;

	m_HitKurai.set_hp( 100 );
#ifdef _VII_DEBUG_
	m_HitKurai.set_hp( 1 );
#endif

	m_HitKurai.ax1 = enKuraiLeft*m_sDirection;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight*m_sDirection;
	m_HitKurai.ay2 = enKuraiBottom;

	m_pEffFire = NULL;
	m_sLogicSeq  = 0;
	m_sLogicWait = 0;

	m_bJump     = gxFalse;
	m_bDamage   = gxFalse;

	m_sTireAdd  = 0;
	m_sTireRot  = 0;

	m_bMaffler = gxTrue;
	m_bShield  = gxTrue;

	m_sMode = 0;

	m_pEnemyAnime = new CEnemyAnime();

	for(ii=0; ii<enMax ;ii++)
	{
		m_pEnemyAnime->SetParts( ii , enMax , m_sParentCEneH0102NightSpider[ii] , sPosCEneH0102NightSpider[ii] );
	}

	for(ii=0;ii<enCannonMax;ii++)
	{
		m_pSlideCannon[ii] = NULL;
	}

	m_pEnemyAnime->PosCalc();

	m_pEnemyAnime->GetParts( enDOUTAIMAE )->bNoDamage = gxTrue;
	m_pEnemyAnime->GetParts( enMAEASI )->bNoDamage = gxTrue;
	m_pEnemyAnime->GetParts( enUSIROASI1 )->bNoDamage = gxTrue;
	m_pEnemyAnime->GetParts( enUSIROASI2 )->bNoDamage = gxTrue;
//	m_pEnemyAnime->GetParts( enDOUTAIUSIRO )->bNoDamage = gxTrue;
//	m_pEnemyAnime->GetParts( enFSHIELD )->bNoDamage = gxTrue;
	m_pEnemyAnime->GetParts( enBSHIELD1 )->bNoDamage = gxTrue;
	m_pEnemyAnime->GetParts( enBSHIELD2 )->bNoDamage = gxTrue;
	m_pEnemyAnime->GetParts( enTIRE11 )->bNoDamage = gxTrue;
	m_pEnemyAnime->GetParts( enTIRE21 )->bNoDamage = gxTrue;
	m_pEnemyAnime->GetParts( enTIRE31 )->bNoDamage = gxTrue;

	m_Add.x = 0;
	m_Add.y = 0;

	m_bAbsorber = gxFalse;

	m_sAbsorber = 0;
	m_sRebound  = 0;
	m_sShieldHeight = 0;

	m_stRebound.x  = m_stRebound.y  = 0;

	m_Offset.x = 0;
	m_Offset.y = 0;

	//脚部開閉
	m_bLegOpen   = gxFalse;
	m_fLegKakudo = gxTrue;

	//キャノピー開閉
	m_bOpenCannopy = gxFalse;
	m_fCannopyKakudo = 0;

	//脚部コントロール初期化
	Float32 dx,dy,d1,d2;

	for(Sint32 jj=0;jj<3;jj++)
	{
		d1 = m_pEnemyAnime->GetParts( enSTER11+jj )->dx*1.0f;
		d2 = m_pEnemyAnime->GetParent( enSTER11+jj )->dx*1.0f;
		dx = d1 - d2;

		d1 = m_pEnemyAnime->GetParts( enSTER11+jj )->dy*1.0f;
		d2 = m_pEnemyAnime->GetParent( enSTER11+jj )->dy*1.0f;
		dy = d1 - d2;

		m_stTire[jj].dist = (Sint32)viiMath::Dist(dx,dy);
		m_stTire[jj].rot  = (Sint32)viiMath::Atan2d( dy , dx );

		m_pEnemyAnime->GetPartsDist( enSTER11+jj , &m_stTire[jj] );
	}

	m_sAtackSeq = 0;
	m_sAtackSec = 16;
	m_sMissileCnt   = 0;
	m_sMissileTimer = 200;
	m_sMissileSeq   = 0;
	m_sCrashSeq = 0;
	m_sAtackPattern = 0;

	m_CrashTgt.x = 0;
	m_CrashTgt.y = 0;
	m_bCrashTgt  = gxFalse;

	m_HitShield[0].set_hp( enHitPointMiddle );
	m_HitShield[1].set_hp( enHitPointMiddle );
	m_HitShield[2].set_hp( enHitPointMiddle );
	m_HitShield[3].set_hp( enHitPointMiddle );//1000 );

	m_sHitpointShield  = m_HitShield[0].m_sHP+m_HitShield[1].m_sHP+m_HitShield[2].m_sHP;
	m_sHitpointBody    = enHitPointBossSpider;//m_HitShield[3].m_sHP+m_HitKurai.m_sHP;
	m_sHitpointShield  = enHitPointHigh;
}


CEneH0102NightSpider::~CEneH0102NightSpider()
{
	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

	delete m_pEnemyAnime;

}

void CEneH0102NightSpider::SeqInit()
{

/*
	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}
*/
	SetActionSeq( enActionSeqMain );

}


void CEneH0102NightSpider::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	for(Sint32 ii=0;ii<4;ii++)
	{
		m_HitShield[ii].ax1 = sShieldTbl[ii][0]*m_sDirection;
		m_HitShield[ii].ay1 = sShieldTbl[ii][1];
		m_HitShield[ii].ax2 = sShieldTbl[ii][2]*m_sDirection;
		m_HitShield[ii].ay2 = sShieldTbl[ii][3];
		if( m_sHitpointShield )
		{
			//m_HitShield[ii].SetHantei( ID_ENEMY_DEF , &m_Pos );
		}
	}
	m_HitShield[3].SetHantei( ID_ENEMY_DEF , &m_Pos );

	//-------------------------------
	m_sHitpointShield -= m_HitShield[0].GetTotalDamage();
	m_sHitpointShield -= m_HitShield[1].GetTotalDamage();
	m_sHitpointShield -= m_HitShield[2].GetTotalDamage();

	m_HitShield[0].set_hp(100);
	m_HitShield[1].set_hp(100);
	m_HitShield[2].set_hp(100);

	//----------------------------
	m_sHitpointBody   -= m_HitShield[3].GetTotalDamage();
	m_sHitpointBody   -= m_HitKurai.GetTotalDamage();
	//----------------------------

	m_HitShield[3].set_hp(100);
	m_HitKurai.set_hp(100);

	if( 0 )//m_sHitpointShield <= 0 )
	{
		if( m_bShield )
		{
			//----------------------------------------------------------
			//シールド破壊
			//----------------------------------------------------------
			StPartsInfo *p = m_pEnemyAnime->GetParts( enFSHIELD );

			CEffectDmgprt *pDmg;
			pDmg = new CEffectDmgprt( m_Pos.x+p->dx*100*m_sDirection , m_Pos.y + p->dy*100 +m_Offset.y , -90 , 100 );
			pDmg->SetSprite( &SprCEneH0102NightSpider[ p->sDispSprite ] );
			pDmg->SetHeavy();
			pDmg->SetRotation( p->dr );
			pDmg->SetAtribute( (m_sDirection == DIR_LEFT)? ATR_FLIP_X : 0 );

			m_bShield = gxFalse;
		}
		m_sHitpointShield = 0;
	}

	if( m_sHitpointBody <= 0 )
	{
		if( m_sMode == enLogicModeDefault )
		{
			SetActionSeq( enActionSeqCrash );
		}
		else
		{
			m_bDead = gxTrue;
		}
		//return;
	}

	m_pEnemyAnime->PosCalc();
	m_pEnemyAnime->GetParts( enFSHIELD )->bDisp = gxFalse;

	LogicAI();
	Gimmick();

	SetRader( enTypeMarkerMiddleBoss );
	DirectionAdjust();

	m_bDamage  = m_HitKurai.is_damage();
	if( m_HitShield[3].is_damage() ) m_bDamage = gxTrue;

	m_bDamageShield = gxFalse;
	if( m_HitShield[0].is_damage() ) m_bDamageShield = gxTrue;
	if( m_HitShield[1].is_damage() ) m_bDamageShield = gxTrue;
	if( m_HitShield[2].is_damage() ) m_bDamageShield = gxTrue;

	m_HitKurai.SetHantei( ID_ENEMY_DEF , &m_Pos );

}

void CEneH0102NightSpider::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

//	if( Joy[0].trg&BTN_L1 )
//	{
//		SetActionSeq( enActionSeqCrash );
//	}

	if( m_sTimer%40==0)
	{
		m_bLegOpen = !m_bLegOpen;
	}

	switch(m_sMode){
	case enLogicModeDefault:
		LogicNormalMode();
		break;

	case enLogicModeKousoku:
		LogicKousokuScroll();
		break;

	case enLogicModeCrash:
		LogicCrashMode();
		break;
	}

	if( !m_bJump )
	{
		m_Add.y = 0;
	}
	else
	{
		m_Add.y += 10;
	}

	if( m_sControl&JOY_L )
	{
		if( m_Add.x > -enMaxSpeed ) m_Add.x -= enSpeedAdd;
	}
	else if( m_sControl&JOY_R )
	{
		if( m_Add.x < enMaxSpeed ) m_Add.x += enSpeedAdd;
	}
	else
	{
		m_Add.x += -m_Add.x/30;
	}

	m_stObj.mov.x = m_Add.x;
	m_stObj.mov.y = m_Add.y+120;

	if( m_stObj.easy_collision_check() )
	{
		m_bJump = gxFalse;
		m_Add.y = 0;
	}

	m_stObj.pos.x += m_stObj.mov.x;
	m_stObj.pos.y += m_stObj.mov.y;

	m_Pos.x = m_stObj.pos.x;
	m_Pos.y = m_stObj.pos.y;

	//--------------------------------------------------------
	//地上の検出
	//--------------------------------------------------------
	m_sGround = m_Pos.y;

	while( viiSub::IsGroundBlock( m_Pos.x , m_sGround ) )
	{
		m_sGround -= 100;
	}

}

void CEneH0102NightSpider::LogicNormal()
{
	//-----------------------------------------------------
	//対ハウンドガトリング砲攻撃
	//-----------------------------------------------------
	m_bLegOpen = gxTrue;

	Sint32 tx  = GetTargetPlayer()->x;

	m_sLogicWait ++;

	switch(m_sLogicSeq){
	case 0:	//初期化
		m_sControl   = 0;
		m_sLogicWait = 0;
		m_sLogicSeq  = 100;
		m_bMaffler = gxFalse;
		m_sAtackSec = (m_sAtackSeq%2)? 8 : 6;
		m_sAtackSeq++;
		break;

	case 100:	//ブルン・ブルルン
		m_sLogicWait ++;
		if(m_sLogicWait%60<30)
		{
			m_bMaffler = gxTrue;
		}
		else
		{
			m_bMaffler = gxFalse;
		}
		if(m_sLogicWait < 60)
		{
			if(m_sDirection==DIR_LEFT) 	m_sControl = JOY_R;
			if(m_sDirection==DIR_RIGHT)	m_sControl = JOY_L;
		}
		else
		{
			m_sControl = 0;
		}
		if(m_sLogicWait > 60*2)
		{
			m_bMaffler = gxTrue;
			m_sLogicWait = 0;
			m_sLogicSeq  = 200;
		}
		else
		{
			if( m_sLogicWait%30 == 0 )
			{
				CAtkH0002GMissile *p;
				Sint32 ax,ay;
				ax = m_Pos.x;
				ay = m_Pos.y-6400;
				p = new CAtkH0002GMissile( ax , ay , 100, 480 );
				p->SetForce( CAtkH0001Missile::enForceID_Enemy );
				p->m_Pos.z = PRIO_ENEMY -1 ;
				p->SetEndless(60*8);

				viiEff::Set( EFF_MAZZLE , ax , ay , NULL );
				viiEff::SetBombSimple(    ax , ay , 1 , 1+viiSub::Rand()%2 );
				viiMus::PlaySound(enSoundEneShotLaser);
			}
		}
		break;

	case 200:	//グイイイーン(攻撃アリ)
		if(	m_sLogicWait <= 60*2 )
		{
			if(m_sDirection==DIR_LEFT) 	m_sControl = JOY_L;
			if(m_sDirection==DIR_RIGHT)	m_sControl = JOY_R;
		}
		if(	m_sLogicWait >= 60*m_sAtackSec )
		{
			m_sLogicWait = 0;
			m_sLogicSeq  = 999;
		}

		if( tx < m_Pos.x )
		{
			m_sControl = JOY_L;
			if( m_sTireAdd > -6400 ) m_sTireAdd -= 120;
		}
		else if( tx > m_Pos.x )
		{
			m_sControl = JOY_R;
			if( m_sTireAdd < 6400 ) m_sTireAdd += 120;
		}
		Atack();
		break;

	case 300:	//グイイイーン（攻撃なし）
		if(	m_sLogicWait >= 60*3 )
		{
			m_sLogicWait = 0;
			m_sLogicSeq  = 400;
		}
		if( tx < m_Pos.x )
		{
			m_sControl = JOY_L;
			if( m_sTireAdd > -6400 ) m_sTireAdd -= 120;
		}
		else if( tx > m_Pos.x )
		{
			m_sControl = JOY_R;
			if( m_sTireAdd < 6400 ) m_sTireAdd += 120;
		}

/*
		if( m_sLogicWait%12 == 0 && m_sLogicWait%84< 36)
		{
			CAtkH0002GMissile *p;
			p = new CAtkH0002GMissile( m_Pos.x-6400 , m_Pos.y-12800 , 100, 480 );
			p->SetForce( CAtkH0001Missile::enForceID_Enemy );
			viiMus::PlaySound(enSoundEneShotLaser);
			m_sMissileCnt ++;
		}
*/
		break;

	case 400:	//終了
		m_sControl   = 0;
		m_sLogicWait = 0;
		m_sLogicSeq  = 0;
		break;

	case 999:	//終了
		break;
	}

}


void CEneH0102NightSpider::LogicCrashMode()
{
	//---------------------------------------------
	//クラッシュモード
	//---------------------------------------------

	switch( m_sLogicSeq ){
	case 0:
		//-----------------------------
		//位置あわせ
		//-----------------------------
		if( m_bCrashTgt )
		{
			m_Add.x = (m_CrashTgt.x - m_Pos.x)/100;
			if( gxAbs( m_Add.x ) <= 16 )
			{
				m_sLogicSeq = 125;
			}
		}
		break;

	case 125:
		//-----------------------------
		//微調整
		//-----------------------------
		if(m_Add.x < -8 )
		{
			m_Add.x += 8; 
		}
		else if(m_Add.x > 8 )
		{
			m_Add.x -= 8; 
		}
		else
		{
			m_Add.x = 0; 
			m_sLogicSeq = 150;
		}
		break;
	case 150:
		//-----------------------------
		//移動準備待ち
		//-----------------------------
		m_Add.x = 0; 
		break;

	case 200:
		m_Add.x = 800; 
		break;

	case 500:
		//-----------------------------
		//ごっつんこ
		//-----------------------------
		m_Add.x = -200;
		m_sLogicSeq = 600;
		{
			CEffectDmgprt *p;
			p = new CEffectDmgprt( m_Pos.x+128*100, m_Pos.y-120*100 ,-45, 800 );
			p = new CEffectDmgprt( m_Pos.x+128*100, m_Pos.y-130*100, -25, 800 );
			p = new CEffectDmgprt( m_Pos.x+128*100, m_Pos.y-140*100, 15 , 800 );
			p = new CEffectDmgprt( m_Pos.x+128*100, m_Pos.y-150*100, 20 , 800 );
			EffectMakeBigBomb( m_Pos.x+128*100 , m_Pos.y-128*100 );
			pGame->QuakeOn(16);
		}
		break;

	case 600:
		if(m_Add.x < 830 )
		{
			m_Add.x += 32;
		}
		break;

	case 800:
		//-----------------------------
		//ごっつんこ
		//-----------------------------
		m_bDead = gxFalse;
		SetActionSeq( enActionSeqCrash );
		break;

	}


}


void CEneH0102NightSpider::LogicKousokuScroll()
{
	//----------------------------------------------
	//ステージ４ボスロジック
	//----------------------------------------------
	Sint32 sPosTbl[]={
		3000+2300,3000+2300,3900+2300,3700+2300,
	};

	Sint32 sMslTbl[]={
		5200,630,15,60,
		5200,650,15,60,
		5200,670,15,60,
		5200,790,0  ,60,

		5200,630,15,20,
		5200,650,15,20,
		5200,670,15,20,
		5200,790,0 ,20,

		5200,630,15,60,
		5200,650,15,60,
		5200,670,15,60,
		5200,790,0  ,60,

		5200,630,15,20,
		5200,650,15,20,
		5200,670,15,20,
		5200,790,0 ,20,

	};

	switch( m_sLogicSeq ){
	case 0:
		//---------------------------------
		//ガトリング攻撃
		//---------------------------------
		m_bMaffler = gxTrue;
		m_sControl   = 0;
		m_sLogicWait = 0;
		m_sLogicSeq  = 100;
		m_sAtackSec = 0;

		m_sMissileCnt   = 0;
		m_sMissileSeq   =  0;
		m_sMissileTimer = 0;
		m_sLogicSeq = 100;
		break;

	case 100:
		//出現
		m_bOpenCannopy = gxFalse;
		m_bMaffler = gxTrue;
		m_bLegOpen = gxTrue;
		m_sTireAdd += 120;
		if( m_Pos.x  < sPosTbl[2]*100 )
		{
			m_sControl = JOY_R;
		}
		else
		{
			m_sLogicWait = 0;
			m_sLogicSeq = 150;
		}
		Atack();
		break;

	case 150:
		//----------------------------------------
		//グラミサイル
		//----------------------------------------
		if( m_Pos.x  < (sPosTbl[3]-50)*100 )
		{
			m_sControl = JOY_R;
		}
		else if( m_Pos.x  > (sPosTbl[3]+50)*100 )
		{
			m_sControl = JOY_L;
		}
		else
		{
			m_sControl = 0;

			m_sMissileTimer ++;

//			if( m_sMissileTimer%48 == 0 )
//			{
//				CAtkH0002GMissile *p;
//				p = new CAtkH0002GMissile( m_Pos.x , m_Pos.y-3200 , 180, 240 );
//				p->SetForce( CAtkH0001Missile::enForceID_Enemy );
//				viiMus::PlaySound(enSoundEneShotLaser);
//				m_sMissileCnt ++;
//			}
			if( m_sMissileTimer%12 == 0 && m_sMissileTimer%84< 36)
			{
				CAtkH0002GMissile *p;
				p = new CAtkH0002GMissile( m_Pos.x+6400 , m_Pos.y-12800 , 100, 480 );
				p->SetForce( CAtkH0001Missile::enForceID_Enemy );
				viiMus::PlaySound(enSoundEneShotLaser);
				m_sMissileCnt ++;
			}

			if(m_sMissileCnt >= 18 )
			{
				m_sLogicSeq = 200;
			}
		}
		break;

	case 200:
		//後退する
		m_bMaffler = gxFalse;
		m_bLegOpen = gxTrue;
		m_sTireAdd += 100;
		if( m_Pos.x  > sPosTbl[0]*100 )
		{
			m_sControl = JOY_L;
		}
		else
		{
			m_sControl = 0;
			m_sLogicWait = 0;
			m_sLogicSeq = 300;
		}
//		Atack();
		break;

	case 300:
		//---------------------------------
		//ミサイル攻撃初期化
		//---------------------------------
		m_sLogicWait ++;
		if( m_sLogicWait > 60*2 )
		{
			m_sMissileCnt   = 0;
			m_sMissileTimer = 0;
			m_sLogicWait = 0;
			m_sLogicSeq = 310;
		}
		break;

	case 310:
		if(m_sMissileTimer%24==0)
		{
			for(Sint32 ii=0;ii<1;ii++)
			{
				Sint32 n = m_sMissileCnt%8;
				CAtkH0001Missile *p;
				p = new CAtkH0001Missile( sMslTbl[n*4+0]*100 , sMslTbl[n*4+1]*100 , viiSub::SetKakudo( m_sDirection,sMslTbl[n*4+2]) , 320 );
				p->SetForce( CAtkH0001Missile::enForceID_Enemy );
				p->SetHomingLevel(10);
				p->SetEndless();
				p->SetWait( sMslTbl[n*4+3] );
				m_sMissileCnt ++;
				viiMus::PlaySound( enSoundMissile );
				if( m_sMissileCnt >= 16 )
				{
					m_sLogicSeq = 320;
				}
			}
		}
		m_sMissileTimer ++;
		break;

	case 320:
		m_sLogicWait ++;
		if( m_sLogicWait > 60*2 )
		{
			m_sMissileCnt   = 0;
			m_sMissileTimer = 0;
			m_sLogicSeq     = 400;
		}
		break;

	case 400:
		//----------------------------------------
		//弾幕攻撃
		//----------------------------------------
		if( WaraWaraDanmaku() )
		{
			m_sLogicSeq     = 999;
		}
		if( m_Pos.x  < (sPosTbl[1]-50)*100 )
		{
			m_sControl = JOY_R;
		}
		else if( m_Pos.x  > (sPosTbl[1]+50)*100 )
		{
			m_sControl = JOY_L;
		}
		else
		{
			m_sControl = 0;
		}
		break;

	case 999:
		m_sControl = 0;
		m_sLogicWait ++;
		if( m_sLogicWait > 60*3 )
		{
			m_sLogicWait = 0;
			m_sLogicSeq = 100;
		}
		break;
	}

}

void CEneH0102NightSpider::LogicNormalMode()
{
	//----------------------------------------------
	//ステージ１ボスロジック
	//----------------------------------------------

	switch(m_sAtackPattern){
	case enAtackModeNormal:	//ガトリングガン攻撃
		m_bLegOpen = gxFalse;
		LogicNormal();
		if( m_sLogicSeq == 999 )
		{
			m_sMissileSeq = 0;
			m_sLogicSeq   = 0;
			m_sAtackPattern ++;
			m_sAtackPattern = m_sAtackPattern%3;
		}
		break;

	case enAtackModeMissile:
		m_bLegOpen = gxTrue;
		LogicMissile();
		if( m_sMissileSeq == 999 )
		{
			m_sMissileSeq = 0;
			m_sLogicSeq   = 0;
			m_sAtackPattern ++;
			m_sAtackPattern = m_sAtackPattern%3;
			m_sMissileTimer = 0;
		}
		break;

	default:
		if(m_sMissileTimer<=240)
		{
			m_sMissileTimer ++;
		}
		else
		{
			m_sMissileSeq = 0;
			m_sLogicSeq   = 0;
			m_sAtackPattern ++;
			m_sAtackPattern = m_sAtackPattern%3;
		}
		if( m_Base.x+3200 < m_Pos.x )
		{
			m_sControl = JOY_L;
		}
		else if( m_Base.x-3200 > m_Pos.x )
		{
			m_sControl = JOY_R;
		}
		else
		{
			m_sControl = 0;
		}
		break;
	}
}


void CEneH0102NightSpider::LogicMissile()
{
	//-----------------------------------------------------
	//ミサイル攻撃
	//-----------------------------------------------------
	ghPos s_Pos;

	switch( m_sMissileSeq ){
	case 0:	//初期化
		m_sControl = 0;
		m_sMissileCnt = 0;
		m_sMissileSeq = 100;
		m_sMissileTimer = 0;
		break;

	case 100:	//キャノピーオープン
		m_bOpenCannopy = gxTrue;
		if(m_sMissileTimer>=120)
		{
			m_sMissileTimer = 0;
			m_sMissileSeq = 200;
		}
		break;

	case 200:	//ミサイル発射

		if(	m_sMissileTimer >= 4 )
		{
			Sint32 ax,ay;
			s_Pos.x = m_Pos.x+m_Offset.x;
			s_Pos.y = m_Pos.y+m_Offset.y;
			m_pEnemyAnime->GetPos( &s_Pos , enSTER2+m_sMissileCnt%5,  m_sDirection );

//			CAtkH0001Missile *p;
//			p = new CAtkH0001Missile( s_Pos.x , s_Pos.y , viiSub::SetKakudo( m_sDirection,180+25) , 320 );
//			p->SetForce( CAtkH0001Missile::enForceID_Enemy );
//			p->SetHomingLevel(18);
//			p->SetEndless();

/*
			CEne0001EneSuperBall *p;
			p = new CEne0001EneSuperBall( s_Pos.x , s_Pos.y , viiSub::SetKakudo( m_sDirection,180+45+m_sMissileCnt) , 240 );
			p->SetPattern( CEne0001EneSuperBall::enPatternMicroMissile );
			p->SetMaxSpeed( 720 );
			p->SetLevel(20);
			p->SetEndless();
			m_sMissileSeq = 300;
*/
			ax = s_Pos.x;
			ay = s_Pos.y;

			CEne0001EneSuperBall *p;
			Sint32 sRotation = get_valken_rot( ax , ay+2400 )+180;
			p = new CEne0001EneSuperBall( ax , ay , sRotation-30+viiSub::Rand()%60 ,300 );
			p->SetPattern( CEne0001EneSuperBall::enPatternMicroMissile );
			p->SetMaxSpeed( 720 );
			p->SetScrollOutRange( WINDOW_W*200 );
			p->SetScale( 1.0f);
			p->SetLevel(1);
			p->SetEndless();
			viiMus::PlaySound(enSoundEneShotLaser);

			viiEff::Set( EFF_MAZZLE , ax , ay , NULL );
			viiEff::SetBombSimple(    ax , ay , 1 , 1+viiSub::Rand()%2 );

			m_sMissileSeq = 300;
		}
		break;

	case 300:	//ミサイル発射ループポイント
		if(m_sMissileCnt < 18 )
		{
			m_sMissileTimer = 0;
			m_sMissileCnt ++;
			m_sMissileSeq = 200;
		}
		else
		{
			m_sMissileSeq = 400;
		}
		break;

	case 400:	//ミサイル発射ループポイント
		m_bOpenCannopy = gxFalse;
		m_sMissileSeq = 999;
		break;

	case 999:
		break;
	}

	m_sMissileTimer ++;


}


void CEneH0102NightSpider::Gimmick()
{
	//-----------------------------------------------------
	//ギミック制御
	//-----------------------------------------------------
	GimmickRebound();
	GimmickLegOpen();
	GimmickCannopyOpen();
	GimmickMaffler();

	m_sShieldHeight = (viiMath::Cos100((m_sTimer*32)%360)*m_Add.x)/100;

	//--------------------------------------
	//プライオリティ制御
	//--------------------------------------
	m_pEnemyAnime->GetParts( enUSIROASI1 )->dz = (PRIO_PLAYER ) - PRIO_ENEMY;
	m_pEnemyAnime->GetParts( enUSIROASI2 )->dz = (PRIO_PLAYER ) - PRIO_ENEMY;
	m_pEnemyAnime->GetParts( enTIRE21    )->dz = (PRIO_PLAYER ) - PRIO_ENEMY+1;
	m_pEnemyAnime->GetParts( enTIRE31    )->dz = (PRIO_PLAYER ) - PRIO_ENEMY+1;
	m_pEnemyAnime->GetParts( enBSHIELD1  )->dz = (PRIO_PLAYER ) - PRIO_ENEMY+5;
	m_pEnemyAnime->GetParts( enBSHIELD2  )->dz = (PRIO_PLAYER ) - PRIO_ENEMY+4;


	//----------------------------------
	//胴体の角度あわせ
	//----------------------------------

	m_pEnemyAnime->GetParts( enFSHIELD    )->dr += m_pEnemyAnime->GetParent( enFSHIELD  )->dr;
	m_pEnemyAnime->GetParts( enBSHIELD1   )->dr += m_pEnemyAnime->GetParent( enBSHIELD1 )->dr;
	m_pEnemyAnime->GetParts( enBSHIELD2   )->dr += m_pEnemyAnime->GetParent( enBSHIELD2 )->dr;

	m_pEnemyAnime->GetParts( enFSHIELD    )->dy += m_sShieldHeight/100;
	m_pEnemyAnime->GetParts( enBSHIELD1   )->dy += m_sShieldHeight/100;
	m_pEnemyAnime->GetParts( enBSHIELD2   )->dy += m_sShieldHeight/200;


	m_pEnemyAnime->GetParts( enTIRE11   )->dr += m_pEnemyAnime->GetParent( enTIRE11 )->dr;
	m_pEnemyAnime->GetParts( enTIRE21   )->dr += m_pEnemyAnime->GetParent( enTIRE21 )->dr;
	m_pEnemyAnime->GetParts( enTIRE31   )->dr += m_pEnemyAnime->GetParent( enTIRE31 )->dr;

	m_pEnemyAnime->GetParts( enMISSILEPOD   )->dr += m_pEnemyAnime->GetParent( enMISSILEPOD )->dr;
	m_pEnemyAnime->GetParts( enLASERTANK    )->dr += m_pEnemyAnime->GetParent( enLASERTANK  )->dr;

//	15


	//----------------------------------
	//リバウンド処理
	//----------------------------------
	m_pEnemyAnime->GetParts( enDOUTAIMAE   )->dx += m_stRebound.x/3;

	m_pEnemyAnime->GetParts( enDOUTAIUSIRO )->dx += m_stRebound.x;
	m_pEnemyAnime->GetParts( enDOUTAIUSIRO )->dy += m_stRebound.y;

	m_pEnemyAnime->GetParts( enMISSILEPOD  )->dx += m_stRebound.x;
	m_pEnemyAnime->GetParts( enMISSILEPOD  )->dy += m_stRebound.y;

	m_pEnemyAnime->GetParts( enLASERTANK   )->dx += m_stRebound.x;
	m_pEnemyAnime->GetParts( enLASERTANK   )->dy += m_stRebound.y;

	m_pEnemyAnime->GetParts( enGATLINGGUN1 )->dx += m_stRebound.x;
	m_pEnemyAnime->GetParts( enGATLINGGUN1 )->dy += m_stRebound.y;


	//----------------------------------
	//ガトリングガン(2,3)を消す
	//----------------------------------
	m_pEnemyAnime->GetParts( enGATLINGGUN2    )->bDisp = gxFalse;
	m_pEnemyAnime->GetParts( enGATLINGGUN3    )->bDisp = gxFalse;

	//----------------------------------
	//☆を消す
	//----------------------------------
	for(Sint32 ii=enSTER1; ii<enMax; ii++)
	{
		m_pEnemyAnime->GetParts( ii    )->bDisp = gxFalse;
	}

}


void CEneH0102NightSpider::GimmickLegOpen()
{
	//----------------------------------------------------
	// 脚部制御
	//----------------------------------------------------

	if( m_bLegOpen )
	{
		if( m_fLegKakudo < 100 )
		{
			m_fLegKakudo += (100-m_fLegKakudo)/10;
			if( m_fLegKakudo > 100 )
			{
				m_fLegKakudo = 100;
			}
		}
	}
	else
	{
		if( m_fLegKakudo > 0 )
		{
			m_fLegKakudo += (0-m_fLegKakudo)/10;
			if( m_fLegKakudo < 0 )
			{
				m_fLegKakudo = 0;
			}
		}
	}

	Sint32 sOpenKaku[3];

	//前足
	sOpenKaku[0] = s_cast<Sint32>(-10*m_fLegKakudo/100);
	//真ん中
	sOpenKaku[1] = s_cast<Sint32>(5+10*m_fLegKakudo/100);
	//後ろ
	sOpenKaku[2] = s_cast<Sint32>(10+10*m_fLegKakudo/100);

	m_pEnemyAnime->GetParts( enMAEASI    )->dr += sOpenKaku[0];
	m_pEnemyAnime->GetParts( enUSIROASI1 )->dr += sOpenKaku[1];
	m_pEnemyAnime->GetParts( enUSIROASI2 )->dr += sOpenKaku[2];

	Sint32 sRot;
	Sint32 sDst;
	Sint32 dx,dy;
	Sint32 ox,oy;
	Sint32 sObj;

	for(Sint32 jj=0;jj<3;jj++)
	{
		sRot = m_stTire[jj].rot+sOpenKaku[jj];
		sDst = m_stTire[jj].dist;
		sObj = enSTER11+jj;

		//親の表示座標
		ox = m_pEnemyAnime->GetParent( sObj )->dx;
		oy = m_pEnemyAnime->GetParent( sObj )->dy;

		//子の表示座標を計算
		dx = ( viiMath::Cos100( sRot )*sDst )/100;
		dy = ( viiMath::Sin100( sRot )*sDst )/100;

		m_pEnemyAnime->GetParts( sObj )->dx = ox+dx;
		m_pEnemyAnime->GetParts( sObj )->dy = oy+dy;

		m_PosTire[jj].x = m_Pos.x+m_Offset.x;
		m_PosTire[jj].y = m_Pos.y+m_Offset.y;

		m_pEnemyAnime->GetPos( &m_PosTire[jj] , enSTER11+jj , m_sDirection );
	}

	m_Offset.y = s_cast<Sint32>( (viiSub::Rand()%4+-128+(16*m_fLegKakudo/100))*100 );

	m_sTireRot += m_sTireAdd;
	m_sTireRot = (36000+m_sTireRot)%36000;

	if( ( m_Add.x > -220 && m_sControl&JOY_L) || ( m_Add.x < 220 && m_sControl&JOY_R))
	{
		for(Sint32 ii=0; ii<3; ii++ )
		{
			new CEffDashStorm( m_PosTire[ii].x+(-32+viiSub::Rand()%64)*100 , m_sGround, 180 , 60 );
		}
		viiEff::Set(EFF_FIREFLOWER , m_PosTire[0].x , m_PosTire[0].y , argset( viiSub::Rand()%360) );
		viiEff::Set(EFF_FIREFLOWER , m_PosTire[1].x , m_PosTire[1].y , argset( viiSub::Rand()%360) );
		viiEff::Set(EFF_FIREFLOWER , m_PosTire[2].x , m_PosTire[2].y , argset( viiSub::Rand()%360) );
	}

}



void CEneH0102NightSpider::GimmickRebound()
{
	//----------------------------------------------------
	// リバウンド
	//----------------------------------------------------

	if( m_bAbsorber )
	{
		Sint32 sAdd = ( (100*100)-m_sAbsorber )/5;
		m_sAbsorber += sAdd;
		if(sAdd == 0)
		{
			m_bAbsorber = gxFalse;
		}
	}
	else
	{
		if( m_sAbsorber > 0 ) m_sAbsorber -= 300;

		if( m_sAbsorber <= 0 )
		{
			m_bAbsorber = gxFalse;
			m_sAbsorber = 0;
		}
	}

	m_stRebound.y = 16*m_sAbsorber/(100*100);

	if( m_sRebound > 0)
	{
		Sint32 sAdd = (0-m_sRebound)/10;

		if( gxAbs( sAdd ) < 10 )
		{
			sAdd = -10;
		}

		m_sRebound += sAdd;

		if(m_sRebound < 0) m_sRebound = 0;
	}

	m_stRebound.x = 8*m_sRebound/100;

}


void CEneH0102NightSpider::GimmickCannopyOpen()
{
	//----------------------------------------------------
	// キャノピー制御
	//----------------------------------------------------
	Float32 r=12;

	if( m_bOpenCannopy )
	{
		if( m_fCannopyKakudo < 100 )
		{
			m_fCannopyKakudo += (100-m_fCannopyKakudo)/10;
			if( m_fCannopyKakudo > 100 )
			{
				m_fCannopyKakudo = 100;
			}
		}
	}
	else
	{
		if( m_fCannopyKakudo > 0 )
		{
			m_fCannopyKakudo += (0-m_fCannopyKakudo)/10;
			if( m_fCannopyKakudo < 0 )
			{
				m_fCannopyKakudo = 0;
			}
		}
	}

	Sint32 dr = s_cast<Sint32>(120*m_fCannopyKakudo/100);

//	Float32 dx,dy,dst,r=12;
//	Sint32 rr = s_cast<Sint32>(r);
//	dst = (32*m_fCannopyKakudo)/100;
//	dx = (viiMath::Cos100(rr)*dst)/100;
//	dy = (viiMath::Sin100(rr)*dst)/100;

	m_pEnemyAnime->GetParts( enMISSILEPOD )->dr += dr;
}

void CEneH0102NightSpider::GimmickMaffler()
{
	//-----------------------------------------------------
	//マフラー制御
	//-----------------------------------------------------
	
	if( !m_bMaffler ) return;

	ghPos s_Pos;

	m_TgtPos.x = m_Pos.x;
	m_TgtPos.y = m_Pos.y;

	if( viiSub::Rand()%3==0)
	{
		s_Pos.x = m_Pos.x+m_Offset.x+1200*m_sDirection;
		s_Pos.y = m_Pos.y+m_Offset.y;
		m_pEnemyAnime->GetPos( &s_Pos , enSTER9 , m_sDirection );

		CEffBombSmoke *p;
		Sint32 sKaku = 180+25+viiSub::Rand()%10;
		p = new CEffBombSmoke( s_Pos.x-m_Pos.x , s_Pos.y-m_Pos.y ,viiSub::SetKakudo( m_sDirection , sKaku ) , 100+viiSub::Rand()%580 );
		p->RotFix();
		p->SetTgt(&m_TgtPos);
		p->SetType(1);

		//-------------------

		s_Pos.x = m_Pos.x+m_Offset.x+1200*m_sDirection;
		s_Pos.y = m_Pos.y+m_Offset.y;
		m_pEnemyAnime->GetPos( &s_Pos , enSTER10 , m_sDirection );

		sKaku = 180+15+viiSub::Rand()%10;
		p = new CEffBombSmoke( s_Pos.x-m_Pos.x , s_Pos.y-m_Pos.y ,viiSub::SetKakudo( m_sDirection , sKaku ) , 100+viiSub::Rand()%580 );
		p->RotFix();
		p->SetTgt(&m_TgtPos);
		p->SetType(1);

	}

}


void CEneH0102NightSpider::Atack()
{
	//-----------------------------------------------------
	//ガトリングガン攻撃
	//-----------------------------------------------------
	Sint32 sSprite =(m_sTimer%(4*3))/4;
	m_pEnemyAnime->GetParts( enGATLINGGUN1    )->sDispSprite = m_pEnemyAnime->GetParts( enGATLINGGUN1+sSprite    )->sSprite;

	if( viiSub::GameCounter()%6 == 0 )
	{
		m_sRebound  = 100;
		ghPos s_Pos;

		s_Pos.x = m_Pos.x+m_Offset.x+1200*m_sDirection;
		s_Pos.y = m_Pos.y+m_Offset.y-2000;

		viiMus::PlaySound( enSoundEneShotLaser );

		m_pEnemyAnime->GetPos( &s_Pos , enSTER1 , m_sDirection );
		new CEffEneTwinGun( s_Pos.x, s_Pos.y ,viiSub::SetKakudo( m_sDirection , 90-60 ) ,800);
		viiEff::Set(EFF_MAZZLE , s_Pos.x, s_Pos.y , NULL );

	//	viiMus::PlaySound( enSoundGun01 );
	}

}


void CEneH0102NightSpider::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	Sint32 ax,ay;

	if( m_bDead )
	{
		m_bDisp = gxFalse;
	}

	if( m_sCrashSeq == 0 )
	{
		m_pEffFire = new CEffFire( m_Pos.x , m_Pos.y-8*100 , PRIO_BG2+1);
		viiMus::PlaySound( enSoundExplosionMiddle );
		viiMus::PlaySound( enSoundExplosionLarge );
	}

	if( m_pEffFire )
	{
		m_pEffFire->m_Pos.x = m_Pos.x;
		m_pEffFire->m_Pos.y = m_Pos.y;
		m_pEffFire->m_Pos.z = PRIO_ENEMY;
	}

	if( m_sCrashSeq <= 96 )
	{
		new CEffectBlackFire ( m_Pos.x + (-64+viiSub::Rand()%128)*100, m_Pos.y  + (-32+viiSub::Rand()%64)*100, NULL );

		m_pEnemyAnime->PosCalc();
		Gimmick();

		m_stObj.mov.x += (0-m_stObj.mov.x)/30;

		m_stObj.pos.x += m_stObj.mov.x;
		m_stObj.pos.y += m_stObj.mov.y;

		m_Pos.x = m_stObj.pos.x;
		m_Pos.y = m_stObj.pos.y;

		m_HitShield[3].SetHantei( ID_ENEMY_DEF , &m_Pos );
		m_HitKurai.SetHantei( ID_ENEMY_DEF , &m_Pos );

		if(m_sCrashSeq%8==0)
		{
			ax = m_Pos.x        +(-8+viiSub::Rand()%16)*1400;
			ay = m_Pos.y-128*100 +(-8+viiSub::Rand()%16)*800;

			for(Sint32 n=0; n<4; n++ )
			{
				viiEff::SetBombDeluxe( ax , ay , 1 , 1+viiSub::Rand()%2 );
				//EffectMakeBigBomb( ax+(-32+viiSub::Rand()%64)*100, ay+(-32+viiSub::Rand()%64)*100 );
			}
			viiMus::PlaySound( enSoundExplosionMiddle );
		}
	}

	if( m_sCrashSeq == 96 )
	{
		//どかーーーん

		if( !m_bDead )
		{
			ax = m_Pos.x;
			ay = m_Pos.y-128*100;

			viiEff::Set(EFF_BIGBOMB , m_Pos.x, m_Pos.y , argset(5));

			for(Sint32 ii=0; ii<enMax ;ii++)
			{
				StPartsInfo *p = m_pEnemyAnime->GetParts( ii );

				_opt_[0] = 0;//(Sint32)&SprCEneH0102NightSpider[ p->sDispSprite ];	//64bitポインタ対応
				_opt_[1] = (m_sDirection==DIR_LEFT)? ATR_FLIP_X : ATR_DFLT;
				_opt_[2] = ARGB_DFLT;
				_opt_[3] = p->dr;
				_opt_[4] = 100;
				_opt_[5] = 1;

				viiEff::Set( EFF_DAMAGE_PARTS,m_Pos.x+p->dx*100*m_sDirection , m_Pos.y + p->dy*100 +m_Offset.y , _opt_ ,&SprCEneH0102NightSpider[ p->sDispSprite ] );
				viiEff::SetBombSimple( m_Pos.x+p->dx*100*m_sDirection , m_Pos.y + p->dy*100 +m_Offset.y , 1 , 9 );
			}

			viiEff::SetBombDeluxe( ax , ay , 1 , 1+viiSub::Rand()%2 );

			CEffectDmgprt *p;
			p = new CEffectDmgprt( ax, ay -1600, 180+15 , 800 );
			p = new CEffectDmgprt( ax, ay -1600, 180+65 , 600 );
			p = new CEffectDmgprt( ax, ay -1600, -60    , 800 );
			p = new CEffectDmgprt( ax, ay -1600, -20    , 700 );

			p = new CEffectDmgprt( ax, ay -1600, 180-15 , 800/2 );
			p = new CEffectDmgprt( ax, ay -1600, 180-65 , 600/2 );
			p = new CEffectDmgprt( ax, ay -1600, 60    , 800/2 );
			p = new CEffectDmgprt( ax, ay -1600, 20    , 700/2 );

			delete m_pEffFire;
			m_pEffFire = NULL;
			pGame->QuakeOn( 32 );
			viiMus::PlaySound( enSoundCrashA );
			ScorePlus( enScore );
		}

		m_bDead = gxTrue;
	}

	if( m_bClear )
	{
		SetActionSeq( enActionSeqEnd );
	}

	m_sCrashSeq ++;
}

void CEneH0102NightSpider::Draw()
{
	if( !m_bDisp ) return;

	m_pEnemyAnime->Draw( m_bDamage , m_Pos.x+m_Offset.x , m_Pos.y+m_Offset.y , ( m_sDirection == DIR_LEFT )? gxTrue : gxFalse , &SprCEneH0102NightSpider[0] );

	for(Sint32 jj=0;jj<3;jj++)
	{
		Sint32 prio = m_pEnemyAnime->GetParts( enTIRE21  )->dz+PRIO_ENEMY;
		if( jj==0 )
		{
			prio = PRIO_ENEMY-1;
		}
		viiDraw::Sprite( &SprCEneH0102NightSpider[15] , m_PosTire[jj].x , m_PosTire[jj].y , prio , ATR_DEFAULT,ARGB_DFLT,1.f ,m_sTireRot/100 );
	}

	if( m_bShield )
	{
		StPartsInfo *p = m_pEnemyAnime->GetParts( enFSHIELD );
		viiDraw::Sprdmg( m_bDamageShield , &SprCEneH0102NightSpider[ p->sDispSprite ] , m_Pos.x+p->dx*100*m_sDirection , m_Pos.y + p->dy*100 +m_Offset.y , PRIO_ENEMY , ( m_sDirection == DIR_LEFT )? ATR_FLIP_X : ATR_DEFAULT ,ARGB_DFLT,1.f ,p->dr );
	}

}

enum {
	enSlideCannonY = 618*100,
};

gxBool CEneH0102NightSpider::WaraWaraDanmaku()
{
	//----------------------------------------------------
	//雷：弾幕ゲーム
	//----------------------------------------------------
	gxBool bZenmetsu = gxTrue;
	gxBool bTouchaku = gxTrue;

	//全滅チェック
	for(Sint32 ii=0;ii<enCannonMax;ii++)
	{
		if( m_pSlideCannon[ii] )
		{
			if( !m_pSlideCannon[ii]->IsArrive() )
			{
				bTouchaku = gxFalse;
			}

			if( m_pSlideCannon[ii]->IsDead() )
			{
				//m_pSlideCannon[ii]->Clear();
				//m_pSlideCannon[ii] = NULL;
			}
			else
			{
				bZenmetsu = gxFalse;
			}
		}
	}

	if( bZenmetsu )
	{
		for(Sint32 ii=0;ii<enCannonMax;ii++)
		{
			if( m_pSlideCannon[ii] )
			{
				m_pSlideCannon[ii]->Clear();
				m_pSlideCannon[ii] = NULL;
			}
		}
	}

	CEneH0409SlideCannon *p;
	Sint32 Center = 5832*100;

	if( m_sAtackPattern == 0)
	{
		//----------------------------------------
		//真下弾丸部隊
		//----------------------------------------
		switch( m_sMissileSeq ){
		case 0:
			m_sMissileSeq = 100;
			break;

		case 100:
			p = m_pSlideCannon[0] = new CEneH0409SlideCannon( Center-500*100 , enSlideCannonY );
			p->SetPosition(Center+120*100);
			p->SetAtack(gxFalse , 0 );

			p = m_pSlideCannon[1] = new CEneH0409SlideCannon( Center+500*100 , enSlideCannonY );
			p->SetPosition(Center-120*100);
			p->SetAtack(gxFalse , 0 );

			m_sMissileSeq = 110;
			break;

		case 110:
			if( bTouchaku )
			{
				m_pSlideCannon[0]->SetAtack(gxTrue);
				m_pSlideCannon[1]->SetAtack(gxTrue);
				m_sMissileSeq = 199;
			}
			break;
		case 199:	//待ち
			if( bZenmetsu )
			{
				m_sAtackPattern ++;
				m_sAtackPattern = m_sAtackPattern%3;
				m_sMissileSeq = 0;
			}
			break;
		}
	}
	else if( m_sAtackPattern == 1)
	{
		//----------------------------------------
		//レースゲー弾幕
		//----------------------------------------
		switch( m_sMissileSeq ){
		case 0:	
			p = m_pSlideCannon[0] = new CEneH0409SlideCannon( Center-500*100 , enSlideCannonY );
			p->SetPosition(Center+120*100);
			p->SetAtack(gxFalse , 2 );

			p = m_pSlideCannon[1] = new CEneH0409SlideCannon( Center+500*100 , enSlideCannonY );
			p->SetPosition(Center-120*100);
			p->SetAtack(gxFalse , 2 );

			m_sMissileSeq = 110;
			break;

		case 110:
			if( bTouchaku )
			{
				m_pSlideCannon[0]->SetAtack(gxTrue);
				m_pSlideCannon[1]->SetAtack(gxTrue);
				m_sMissileSeq = 199;
			}
			break;

		case 199:	//待ち
			if( bZenmetsu )
			{
				m_sAtackPattern ++;
				m_sAtackPattern = m_sAtackPattern%3;
				m_sMissileSeq = 0;
				return gxTrue;
			}
			break;

		}
	}
	else if( m_sAtackPattern == 2)
	{
		//----------------------------------------
		//３連弾
		//----------------------------------------
		switch( m_sMissileSeq ){
		case 0:
			p = m_pSlideCannon[0] = new CEneH0409SlideCannon( Center-500*100 , enSlideCannonY );
			p->SetPosition(Center+120*100);
			p->SetAtack(gxFalse , 1 );

			p = m_pSlideCannon[1] = new CEneH0409SlideCannon( Center+500*100 , enSlideCannonY );
			p->SetPosition(Center-120*100);
			p->SetAtack(gxFalse , 1 );

			m_sMissileSeq = 110;
			break;

		case 110:
			if( bTouchaku )
			{
				m_pSlideCannon[0]->SetAtack(gxTrue);
				m_pSlideCannon[1]->SetAtack(gxTrue);
				m_sMissileSeq = 199;
			}
			break;

		case 199:	//待ち
			if( bZenmetsu )
			{
				m_sAtackPattern ++;
				m_sAtackPattern = m_sAtackPattern%3;
				m_sMissileSeq = 0;
				return gxTrue;
			}
			break;
		}
	}

	return gxFalse;
}

