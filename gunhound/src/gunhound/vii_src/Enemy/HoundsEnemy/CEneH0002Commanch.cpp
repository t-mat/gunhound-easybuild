//--------------------------------------------------------------------------------
//
// Stage00:コマンチ
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../Effect/EffectBase.h"
#include "EnemyTbl/CEneH0002CommanchTbl.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -30,
	enKuraiTop   = -34,
	enKuraiRight =  30,
	enKuraiBottom=  0,

	enMaxSpeed = 640,

	enScore = 800,
};



CEneH0002Commanch::CEneH0002Commanch( Sint32 x, Sint32 y ,gxBool bDeadWait )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_bClear = (bDeadWait)? gxFalse : gxTrue;

	m_HitKurai.set_hp( enDefPointZakoM );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_LEFT;
	m_sAtribute = ATR_DFLT;
	m_sRotation = 100*m_sDirection;
	m_sKatamuki = 0;

	m_bKaiten    = gxFalse;
	m_sKaitenDir = 0;

	m_Add.x = 0;
	m_Add.y = 0;

	m_bCrash = gxFalse;
	m_bDamage = gxFalse;
	m_sCrashTimer = 0;

	m_pEnemyAnime = new CEnemyAnime();

	Init();

	m_bEnd = gxFalse;
	m_pTargetPos = NULL;
}


CEneH0002Commanch::~CEneH0002Commanch()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

	delete m_pEnemyAnime;

}

void CEneH0002Commanch::Init()
{
	//-----------------------------------------------------
	//初期化
	//-----------------------------------------------------

	for(Sint32 ii=0; ii<enMax ;ii++)
	{
		m_pEnemyAnime->SetParts( ii , enMax , m_sParentCEneH0002Commanch[ii] , sPosCEneH0002Commanch[ii] );
	}

	m_pEnemyAnime->PosCalc();

	Float32 dx,dy;

	//プロペラ表示位置
	dx = m_pEnemyAnime->GetParts( enPUROPERA1 )->dx - m_pEnemyAnime->GetParts( enDOUTAI )->dx*1.0f;
	dy = m_pEnemyAnime->GetParts( enPUROPERA1 )->dy - m_pEnemyAnime->GetParts( enDOUTAI )->dy*1.0f;

	stPropera.dist = (Sint32)viiMath::Dist(dx,dy);
	stPropera.rot  = (Sint32)viiMath::Atan2d( dy , dx );

	//ローター表示位置
	dx = m_pEnemyAnime->GetParts( enUSIROPUROPERA1 )->dx - m_pEnemyAnime->GetParts( enDOUTAI )->dx*1.0f;
	dy = m_pEnemyAnime->GetParts( enUSIROPUROPERA1 )->dy - m_pEnemyAnime->GetParts( enDOUTAI )->dy*1.0f;

	stRotar.dist = (Sint32)viiMath::Dist(dx,dy);
	stRotar.rot  = (Sint32)viiMath::Atan2d( dy , dx );

	//ミサイルポッド
	dx = m_pEnemyAnime->GetParts( enMISAILPOD )->dx - m_pEnemyAnime->GetParts( enDOUTAI )->dx*1.0f;
	dy = m_pEnemyAnime->GetParts( enMISAILPOD )->dy - m_pEnemyAnime->GetParts( enDOUTAI )->dy*1.0f;

	stMissilePod.dist = (Sint32)viiMath::Dist(dx,dy);
	stMissilePod.rot  = (Sint32)viiMath::Atan2d( dy , dx );

}


void CEneH0002Commanch::SeqInit()
{

	if( m_pTargetPos == NULL && IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}

	m_Pos.x += 3600*100;

	SetActionSeq( enActionSeqMain );

}


void CEneH0002Commanch::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( !IsScrollOut( WINDOW_W*100 , enScrollOutHeight ) )
	{
		viiMus::GetInstance()->PlayLoopSound( enSoundHeliRoter , 108 );
	}

	if( m_bCrash )
	{
		//-------------------------------------------
		//墜落中
		//-------------------------------------------
		if(m_sTimer%2==0)
		{
			new CEffBombSmoke(m_Pos.x , m_Pos.y-1600 , -90 , 120  );
		}

		//-------------------------------------------
		//激突
		//-------------------------------------------
		if( viiSub::IsGroundBlock(m_Pos.x,m_Pos.y) )
		{
			pGame->QuakeOn(16);
			SetActionSeq( enActionSeqCrash );
		}
	}
	else if( m_HitKurai.is_dead() )
	{
		//-------------------------------------------
		//空中でやられた
		//-------------------------------------------
		viiMus::PlaySound(enSoundExplosionSmall);
		viiEff::Set( EFF_RING , m_Pos.x , m_Pos.y-16*100 , argset(300) );
		EffectMakeBigBomb(m_Pos.x , m_Pos.y);
		m_bCrash = gxTrue;
		return;
	}
	else
	{
		SetRader( enTypeMarkerFlyingObject );
	}

	LogicAI();
	Gimmick();

	DirectionAdjust();

	m_bDamage = m_HitKurai.is_damage();

	//くらい判定登録
	if( !m_bCrash )
	{
		m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );
	}

}


void CEneH0002Commanch::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	Sint32 tx = GetTargetPlayer()->x ;

	if( m_pTargetPos )
	{
		tx = m_pTargetPos->x;
	}

	Sint32 dst = tx- m_Pos.x;
	Sint32 sControl = 0;

	if( gxAbs(dst) >= 128*100 )
	{
		//遠距離

		if( dst < 0 )
		{
			sControl = JOY_L;
		}
		else
		{
			sControl = JOY_R;
		}

		if( gxAbs(dst) >= 228*100 )
		{
			if( !m_bKaiten )
			{
				if( ( m_Pos.x < tx && m_sDirection == DIR_LEFT ) || ( m_Pos.x > tx && m_sDirection == DIR_RIGHT )  )
				{
					SetKaiten();
				}
			}
		}
	}
	else
	{
		sControl = 0;

		if( m_Add.x > 0)
		{
			m_Add.x -= 1;
		}
		else
		{
			m_Add.x += 1;
		}
	}

	//ふらふら
	if( m_bCrash )
	{
		if( m_Add.y <= 300 ) m_Add.y += 8;

		if( m_Add.x < 0 )
		{
			m_Add.x -= 8;
		}
		else
		{
			m_Add.x += 8;
		}
	}
	else
	{
		m_Add.y = viiMath::Cos100((m_sTimer*3)%360);
	}


	//コントロール
	if( sControl&JOY_L)
	{
		if( m_Add.x > 0 )
		{
			m_Add.x += -10;
		}
		else
		{
			m_Add.x += -10;
		}
	}
	else if( sControl&JOY_R)
	{
		if( m_Add.x < 0 )
		{
			m_Add.x += 10;
		}
		else
		{
			m_Add.x += 10;
		}
	}


	Sint32 sMaxSpeed = enMaxSpeed;
	if(m_bCrash) sMaxSpeed = 3*enMaxSpeed/5;

	if( m_Add.x < -sMaxSpeed ) m_Add.x = -sMaxSpeed;
	if( m_Add.x >  sMaxSpeed ) m_Add.x =  sMaxSpeed;

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	if( m_sTimer%48==0)
	{
		Atack();
	}

}


void CEneH0002Commanch::Gimmick()
{
	//-------------------------------------------
	//動作ギミック
	//-------------------------------------------

	Sint32 sRoterType = m_sTimer%(2*4)/2;
	Sint32 sSprite = 1;
	Sint32 sControl = Joy[0].psh;

	m_pEnemyAnime->PosCalc();

	if( m_Add.x < 0 )
	{
		if( m_sDirection == DIR_LEFT )
		{
			sControl = JOY_R;
		}
		else
		{
			sControl = JOY_L;
		}
	}
	else if( m_Add.x > 0 )
	{
		if( m_sDirection == DIR_LEFT )
		{
			sControl = JOY_L;
		}
		else
		{
			sControl = JOY_R;
		}
	}

	if( m_bKaiten )
	{
		//--------------------------------------------------
		//ボディ回転制御
		//--------------------------------------------------

		if( m_sKaitenDir==DIR_LEFT)
		{
			if( m_sRotation > -100  ) m_sRotation -= 3;
			if( m_sRotation <= -100 )
			{
				m_sRotation = -100;
				m_bKaiten = gxFalse;
			}
		}
		else if( m_sKaitenDir==DIR_RIGHT)
		{
			if( m_sRotation <  100 ) m_sRotation += 3;
			if( m_sRotation >= 100 )
			{
				m_bKaiten = gxFalse;
				m_sRotation = 100;
			}
		}

		if( m_sRotation < 0 )
		{
			m_sDirection = DIR_LEFT;
		}
		else
		{
			m_sDirection = DIR_RIGHT;
		}
		m_sKatamuki = 0;
	}

	//-----------------------------------------
	//左右の傾き制御
	//-----------------------------------------

	if( gxAbs(m_sRotation) == 100 )
	{
		if( sControl&JOY_L)
		{
			m_sKatamuki += (-100-m_sKatamuki)/30;
		}
		else if( sControl&JOY_R)
		{
			m_sKatamuki += (100-m_sKatamuki)/30;
		}
		else
		{
			Sint32 sAdd = -m_sKatamuki/30;
			if( sAdd <= 30 )
			{
				if( m_sKatamuki < 0 ) sAdd = 1;
				if( m_sKatamuki > 0 ) sAdd = -1;
			}
			m_sKatamuki += sAdd;

		}
		if( m_sKatamuki <= -100 ) m_sKatamuki = -100;
		if( m_sKatamuki >=  100 ) m_sKatamuki =  100;

		m_pEnemyAnime->GetParts( enDOUTAI )->dr = (20*m_sKatamuki)/100;
	}

	//-------------------------------------------------------------
	//ローターを回転させる
	//-------------------------------------------------------------

	switch(sRoterType){
	case 0:
		m_pEnemyAnime->GetParts( enPUROPERA1 )->sDispSprite = m_pEnemyAnime->GetParts( enPUROPERA1 )->sSprite;
		break;
	case 1:
	case 3:
		m_pEnemyAnime->GetParts( enPUROPERA1 )->sDispSprite = m_pEnemyAnime->GetParts( enPUROPERA2 )->sSprite;
		break;
	case 2:
		m_pEnemyAnime->GetParts( enPUROPERA1 )->sDispSprite = m_pEnemyAnime->GetParts( enPUROPERA3 )->sSprite;
		break;
	}

	//-------------------------------------------------------------
	//胴体の回転処理
	//-------------------------------------------------------------

	if( gxAbs(m_sRotation) < 10 )
	{
		sSprite = 4;
		m_pEnemyAnime->GetParts( enUSIROPUROPERA1 )->bDisp = gxFalse;
		m_pEnemyAnime->GetParts( enMISAILPOD      )->bDisp = gxFalse;
	}
	else if( gxAbs(m_sRotation) < 30 )
	{
		sSprite = 3;
		m_pEnemyAnime->GetParts( enUSIROPUROPERA1 )->bDisp = gxFalse;
		m_pEnemyAnime->GetParts( enMISAILPOD      )->bDisp = gxFalse;
	}
	else if( gxAbs(m_sRotation) < 60 )
	{
		sSprite = 2;
		m_pEnemyAnime->GetParts( enUSIROPUROPERA1 )->bDisp = gxFalse;
		m_pEnemyAnime->GetParts( enMISAILPOD      )->bDisp = gxFalse;
	}
	else
	{
		sSprite = 1;
		m_pEnemyAnime->GetParts( enUSIROPUROPERA1 )->bDisp = gxTrue;
		m_pEnemyAnime->GetParts( enMISAILPOD      )->bDisp = gxTrue;
	}

	//胴体スプライトの確定
	m_pEnemyAnime->GetParts( enDOUTAI )->sDispSprite = sSprite;

	//後ろのローターを回転させる
	m_pEnemyAnime->GetParts( enUSIROPUROPERA1 )->dr = (m_sTimer*51)%360;

	//他のプロペラは消しておく
	m_pEnemyAnime->GetParts( enPUROPERA2 )->bDisp = gxFalse;
	m_pEnemyAnime->GetParts( enPUROPERA3 )->bDisp = gxFalse;

	Sint32 sRot = stPropera.rot + m_pEnemyAnime->GetParts( enDOUTAI )->dr;

	//プロペラ傾倒
	m_pEnemyAnime->GetParts( enPUROPERA1 )->dx = m_pEnemyAnime->GetParts( m_pEnemyAnime->GetParts( enPUROPERA1 )->sParent )->ox + ( viiMath::Cos100( sRot )*stPropera.dist )/100;
	m_pEnemyAnime->GetParts( enPUROPERA1 )->dy = m_pEnemyAnime->GetParts( m_pEnemyAnime->GetParts( enPUROPERA1 )->sParent )->oy + ( viiMath::Sin100( sRot )*stPropera.dist )/100;
	m_pEnemyAnime->GetParts( enPUROPERA1 )->dr = m_pEnemyAnime->GetParts( enDOUTAI )->dr;

	//ローター傾倒
	sRot = stRotar.rot + m_pEnemyAnime->GetParts( enDOUTAI )->dr;
	m_pEnemyAnime->GetParts( enUSIROPUROPERA1 )->dx = m_pEnemyAnime->GetParts( m_pEnemyAnime->GetParts( enUSIROPUROPERA1 )->sParent )->ox + ( viiMath::Cos100( sRot )*stRotar.dist )/100;
	m_pEnemyAnime->GetParts( enUSIROPUROPERA1 )->dy = m_pEnemyAnime->GetParts( m_pEnemyAnime->GetParts( enUSIROPUROPERA1 )->sParent )->oy + ( viiMath::Sin100( sRot )*stRotar.dist )/100;
//	m_pEnemyAnime->GetParts( enUSIROPUROPERA1 )->dr = m_pEnemyAnime->GetParts( enDOUTAI )->dr;

	//ミサイルポッド傾倒
	sRot = stMissilePod.rot + m_pEnemyAnime->GetParts( enDOUTAI )->dr;
	m_pEnemyAnime->GetParts( enMISAILPOD )->dx = m_pEnemyAnime->GetParts( m_pEnemyAnime->GetParts( enMISAILPOD )->sParent )->ox + ( viiMath::Cos100( sRot )*stMissilePod.dist )/100;
	m_pEnemyAnime->GetParts( enMISAILPOD )->dy = m_pEnemyAnime->GetParts( m_pEnemyAnime->GetParts( enMISAILPOD )->sParent )->oy + ( viiMath::Sin100( sRot )*stMissilePod.dist )/100;
	m_pEnemyAnime->GetParts( enMISAILPOD )->dr = m_pEnemyAnime->GetParts( enDOUTAI )->dr;

}


void CEneH0002Commanch::SetKaiten()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

	if( m_sDirection == DIR_LEFT )
	{
		m_sKaitenDir = DIR_RIGHT;
		m_sRotation = -100;
	}
	else if( m_sDirection == DIR_RIGHT )
	{
		m_sKaitenDir = DIR_LEFT;
		m_sRotation = 100;
	}
	
	m_bKaiten = gxTrue;

}


void CEneH0002Commanch::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------
	Sint32 sRot = m_pEnemyAnime->GetParts( enDOUTAI )->dr;

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight*2 ) ) return;

	if( m_bCrash ) return;

//	CAtkH0001Ryudan *p;

	if( sRot >= 10 )
	{
		if( m_sDirection == DIR_RIGHT)
		{
			sRot = sRot+5;
			viiEff::Set( EFF_MAZZLE , m_Pos.x , m_Pos.y , NULL );
//			p = new CAtkH0001Ryudan( m_Pos.x , m_Pos.y , sRot, 480 );
//			p->SetForce(CAtkH0001Ryudan::enForceID_Enemy);

			CEne0001EneSuperBall *q;
			q = new CEne0001EneSuperBall( m_Pos.x , m_Pos.y-16*100 , sRot, 280  );
			q->SetPattern( CEne0001EneSuperBall::enPatternDrop );
			q->SetScale( 0.75f );
//			q->SetDropGensui(gxTrue,500);

			viiMus::PlaySound(enSoundEneShotLaser);
		}
		else if( m_sDirection == DIR_LEFT)
		{
			sRot = 180-sRot-5;
			viiEff::Set( EFF_MAZZLE , m_Pos.x , m_Pos.y , NULL );

//			p = new CAtkH0001Ryudan( m_Pos.x , m_Pos.y , sRot, 480 );
//			p->SetForce(CAtkH0001Ryudan::enForceID_Enemy);

			CEne0001EneSuperBall *q;
			q = new CEne0001EneSuperBall( m_Pos.x , m_Pos.y-16*100 , sRot, 280  );
			q->SetPattern( CEne0001EneSuperBall::enPatternDrop );
			q->SetScale( 0.75f );
//			q->SetDropGensui(gxTrue,500);

			viiMus::PlaySound(enSoundEneShotLaser);
		}
	}



}


void CEneH0002Commanch::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	if( m_sCrashTimer == 0 )
	{
		m_bEnd = gxTrue;

		ScorePlus( enScore );

		if( pGame->GetMarineLine() )
		{
			MakeSplash( m_Pos.x, pGame->GetMarineLine() );
			viiMus::PlaySound( enSoundSplashM );
			m_bDead = gxTrue;
		}
		else
		{
			//m_pEffFire = 
			new CEffFire( m_Pos.x , m_Pos.y-8*100 , PRIO_EFFECT , 300 );

			CEffectDmgprt *p;
			p = new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, 180+15 , 800 );
			p->SetSprite( &SprCEneH0002Commanch[ m_pEnemyAnime->GetParts( enPUROPERA1 )->sSprite ] );

			p=new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, 180+65 , 600 );
			p->SetSprite( &SprCEneH0002Commanch[ m_pEnemyAnime->GetParts( enPUROPERA2 )->sSprite ] );

			p=new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, -60    , 800 );
			p->SetSprite( &SprCEneH0002Commanch[ m_pEnemyAnime->GetParts( enUSIROPUROPERA1 )->sSprite ] );

			p=new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, -20    , 700 );
			p->SetSprite( &SprCEneH0002Commanch[ m_pEnemyAnime->GetParts( enMISAILPOD )->sSprite ] );

			EffectMakeBigBomb(m_Pos.x , m_Pos.y);
			viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 3 , 1+viiSub::Rand()%5 );
			viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 3 , 1+viiSub::Rand()%5 );
			viiMus::PlaySound(enSoundExplosionMiddle);
		}
	}

	if( m_sCrashTimer == 30 )
	{
		m_bDead = gxTrue;
	}

	m_sCrashTimer ++;

	if( m_bDead )
	{
		if( m_bClear )
		{
			SetActionSeq( enActionSeqEnd );
		}
	}
}

void CEneH0002Commanch::Draw()
{

	if( m_bEnd ) return;

	m_pEnemyAnime->Draw( m_bDamage , m_Pos.x , m_Pos.y , ( m_sDirection == DIR_LEFT )? gxTrue : gxFalse , &SprCEneH0002Commanch[0] );

//	viiDraw::Sprdmg( m_HitKurai.is_damage() , &SprCEneH0002Commanch[ 0 ] , m_Pos.x , m_Pos.y,PRIO_ENEMY , m_sAtribute|m_sAtributeDir , ARGB_DFLT );

}

