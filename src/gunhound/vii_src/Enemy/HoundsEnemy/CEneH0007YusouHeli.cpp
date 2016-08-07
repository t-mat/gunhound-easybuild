//--------------------------------------------------------------------------------
//
// Stage00:輸送ヘリ
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../effect/EffectBase.h"
#include "EnemyTbl/CEneH0007YusouHeliTbl.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -16,
	enKuraiTop   = -34,
	enKuraiRight =  96,
	enKuraiBottom=  0,

	enScore = 300,
	enMaxSpeed = 240,
};

/*
gxSprite SprCEneH0007YusouHeli[]=
{
	{enTexPageEnemyCommon01,128,48,128,56,64,56},//味方ヘリ(特殊部隊入り）
	{TPG_LEYNOS01,144,48,112,48,56,48},	//輸送ヘリ
	{TPG_LEYNOS01,0,56,72,40,36,40},
};
*/

enum {
	enSeqCNone,
	enSeqItemDrop,
	enSeqWaitSoldier,
	enSeqEscape,
};

CEneH0007YusouHeli::CEneH0007YusouHeli( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_HitKurai.set_hp( enHitPointYusouHeli );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_RIGHT;
	m_sAtribute = ATR_DFLT;

	m_sTargetPosX = x;//5300*100;
	m_sHigh = 0;
	m_sHighAdd = 0;
	m_sChakurikuSeq = enSeqCNone;
	m_sDropSeq      = 0;
	m_sDoorOpenX    = 0;
	m_bTakeOff = gxFalse;

	m_pEnemyAnime = new CEnemyAnime();

	for(Sint32 ii=0; ii<enMax ;ii++)
	{
		m_pEnemyAnime->SetParts( ii , enMax , m_sParentCEneH0007YusouHeli[ii] , sPosCEneH0007YusouHeli[ii] );
	}

	for(Sint32 jj=0;jj<enSoldierMax;jj++)
	{
		m_pSoldier[jj] = NULL;
	}

	m_sCrashSeq = 0;

	m_bForceChakuriku = gxFalse;
	m_bLanding        = gxFalse;

	m_sSoldierNum     = 0;
	m_sSoldierLeader  = enNoneLeader;
	m_bRideOff        = gxFalse;
	m_bDamage         = gxFalse;
	m_bWarning        = gxFalse;

}


CEneH0007YusouHeli::~CEneH0007YusouHeli()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

	delete m_pEnemyAnime;

}


void CEneH0007YusouHeli::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( m_HitKurai.is_dead() )
	{
		m_bDead = gxTrue;
	}

	if( m_HitKurai.get_hp() < enHitPointYusouHeli/2 )
	{
		if( viiSub::Rand()%4==0 )
		{
			new CEffectBlackFire( m_Pos.x+(-8+viiSub::Rand()%16)*100 , m_Pos.y+(-32+viiSub::Rand()%16)*100 , NULL );
		}
		m_bWarning = gxTrue;
	}

	if( m_bClear )
	{
		SetActionSeq( enActionSeqCrash );
	}
/*
	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}
*/

	if( !IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		viiMus::GetInstance()->PlayLoopSound( enSoundHeliRoter , 108 );
	}

	m_pEnemyAnime->PosCalc();
	LogicAI();

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

	SetRader( enTypeMarkerFlyingPartner );
	DirectionAdjust();


}


void CEneH0007YusouHeli::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------
	m_CEneCmnSignal.Off();

	//プロペラ処理
	m_pEnemyAnime->GetParts( enPUROPERAMAE2 )->bDisp = gxFalse;
	m_pEnemyAnime->GetParts( enPUROPERAMAE3 )->bDisp = gxFalse;
	m_pEnemyAnime->GetParts( enPUROPERAUSIRO2 )->bDisp = gxFalse;
	m_pEnemyAnime->GetParts( enPUROPERAUSIRO3 )->bDisp = gxFalse;

	Sint32 sRoterType = m_sTimer%(2*4)/2;

	switch(sRoterType){
	case 0:
		m_pEnemyAnime->GetParts( enPUROPERAMAE1   )->sDispSprite = m_pEnemyAnime->GetParts( enPUROPERAMAE1 )->sSprite;
		m_pEnemyAnime->GetParts( enPUROPERAUSIRO1 )->sDispSprite = m_pEnemyAnime->GetParts( enPUROPERAUSIRO3 )->sSprite;
		break;
	case 1:
		m_pEnemyAnime->GetParts( enPUROPERAMAE1   )->sDispSprite = m_pEnemyAnime->GetParts( enPUROPERAMAE2 )->sSprite;
		m_pEnemyAnime->GetParts( enPUROPERAUSIRO1 )->sDispSprite = m_pEnemyAnime->GetParts( enPUROPERAUSIRO2 )->sSprite;
		break;
	case 2:
		m_pEnemyAnime->GetParts( enPUROPERAMAE1   )->sDispSprite = m_pEnemyAnime->GetParts( enPUROPERAMAE3 )->sSprite;
		m_pEnemyAnime->GetParts( enPUROPERAUSIRO1 )->sDispSprite = m_pEnemyAnime->GetParts( enPUROPERAUSIRO1 )->sSprite;
		break;
	case 3:
		m_pEnemyAnime->GetParts( enPUROPERAMAE1   )->sDispSprite = m_pEnemyAnime->GetParts( enPUROPERAMAE2 )->sSprite;
		m_pEnemyAnime->GetParts( enPUROPERAUSIRO1 )->sDispSprite = m_pEnemyAnime->GetParts( enPUROPERAUSIRO2 )->sSprite;
		break;
	}

	m_pEnemyAnime->GetParts( enDOUTAIDOA   )->dx += m_sDoorOpenX/100;

	gxBool bHantei = gxFalse;
	switch( m_sChakurikuSeq ){
	case enSeqCNone:
		LogicMove();
		break;

	case enSeqItemDrop:
		bHantei = gxTrue;
		LogicDropItem();
		ProperaWind();
		break;

	case enSeqWaitSoldier:
		bHantei = gxTrue;
		ProperaWind();
		if( m_bTakeOff )
		{
			m_sChakurikuSeq = enSeqEscape;
		}
		break;
	case enSeqEscape:
		LogicEscape();
		break;
	}

	if( bHantei )
	{
		if( m_HitKurai.is_damage() )
		{
			CFadeManager::GetInstance()->set_fadein(16,0x40F08020);
		}
		//くらい判定登録
		m_bDamage = m_HitKurai.is_damage();
		m_HitKurai.SetHantei(ID_VERSIS_DEF , &m_Pos );
	}
	else
	{
		m_bDamage = gxFalse;
	}

	if( m_sChakurikuSeq == enSeqItemDrop ) return;

}


void CEneH0007YusouHeli::LogicMove()
{
	
	Sint32 tx = m_sTargetPosX;
	Sint32 vx = GetTargetPlayer()->x;
	Sint32 dst = tx- m_Pos.x;
	Sint32 sControl = 0;
	gxBool bChakuriku = gxFalse;

	if( gxAbs(dst) >= 80*100 )
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

		//コントロール
		if( sControl&JOY_L) m_Add.x += -10;
		if( sControl&JOY_R) m_Add.x +=  10;

		if( m_Add.x < -enMaxSpeed ) m_Add.x = -enMaxSpeed;
		if( m_Add.x >  enMaxSpeed ) m_Add.x =  enMaxSpeed;
	}
	else
	{
		//目的地付近
		m_CEneCmnSignal.SetSignal( m_Pos.x+9600,m_Pos.y-1200);
		m_CEneCmnSignal.On();

		sControl = 0;

		if( dst < 100 )
		{
			if( m_Add.x > -30 ) m_Add.x -= 2;
		}
		else if( dst > 100 )
		{
			if( m_Add.x < 30 ) m_Add.x += 2;
		}
		else
		{
			m_Add.x += (0-m_Add.x)/30;
		}

		if( gxAbs(dst) <= 300  && gxAbs(m_Add.x) < 100)
		{
			//着陸準備
			if( gxAbs(vx-(m_Pos.x+32*100)) <= 128*100 )
			{
				bChakuriku = gxTrue;
			}
		}
		else
		{
				bChakuriku = gxFalse;
		
		}
	}

	//ふらふら
	m_Add.y = viiMath::Cos100((m_sTimer*3)%360)/4;

	if( bChakuriku || m_bForceChakuriku )
	{
		m_Add.x += (0-m_Add.x)/30;
		m_Pos.x += (tx-m_Pos.x)/30;
		if( !viiSub::IsGroundBlock(m_Pos.x,m_Pos.y) )
		{
			if( m_sHighAdd < 50*100) m_sHighAdd += 10;
			m_Pos.y += m_sHighAdd/40;
			m_sHigh += m_sHighAdd/40;
			pGame->GetHound()->SetSignal();
		}
		else
		{
			//着陸完了
			pGame->QuakeOn(16);
			viiMus::PlaySound( enSoundLanding );
			m_sChakurikuSeq = enSeqItemDrop;
			m_bLanding = gxTrue;
			m_Add.y = 0;
			m_Add.x = 0;
			m_sHighAdd = 0;
		}
	}
	else
	{
		if( m_sHigh > 0)
		{
			if( m_sHighAdd < 50*100) m_sHighAdd += 10;
			m_Pos.y -= m_sHighAdd/100;
			m_sHigh -= m_sHighAdd/100;
		}
	}

	ProperaWind();

}


gxBool CEneH0007YusouHeli::IsSoldierRideOff()
{

	return m_bRideOff;

}


CSoldierBase* CEneH0007YusouHeli::GetSoldierLeader( )
{
	//ソルジャーのリーダーを教える

	if( m_sSoldierLeader != enNoneLeader )
	{
		return m_pSoldier[m_sSoldierLeader];
	}

	return NULL;

}

void CEneH0007YusouHeli::LogicDropItem()
{
	//-------------------------------------------------
	//アイテムのドロップ
	//-------------------------------------------------
	m_sDropSeq ++;

//	if( m_sDropSeq < 20 ) ProperaWind();

	if( m_sTimer%40==0 )
	{
		if( m_sSoldierNum < enSoldierMax)
		{
			m_pSoldier[m_sSoldierNum] = new CSoldierBase( );
			m_pSoldier[m_sSoldierNum]->SetPosition( 0 , m_Pos.x + m_pEnemyAnime->GetParts( enDOUTAIDOA )->dx+3200 , m_Pos.y-1200 , NULL);
			m_pSoldier[m_sSoldierNum]->SetTargetPosition( m_Pos.x+viiSub::Rand()%6400 );
			m_pSoldier[m_sSoldierNum]->ChangeMoveType( enMoveTypeRideOffHeli );

			if( m_sSoldierNum == 0 )
			{
				//リーダーに任命
				m_sSoldierLeader = m_sSoldierNum;
			}
			else
			{
				//リーダーの設定
				m_pSoldier[m_sSoldierNum]->SetLeader( m_pSoldier[ m_sSoldierLeader ] );
			}

			m_sSoldierNum ++;
			if( m_sSoldierNum == enSoldierMax ) m_bRideOff = gxTrue;
		}
	}

	switch(m_sDropSeq){
	case 60:
		new CItemChip( m_Pos.x +32*100, m_Pos.y, enItemEnergyChip);
		break;

	case 120:
		//new CItemChip( m_Pos.x +32*100, m_Pos.y, enItemPowerChip);
		break;

	case 600:	//Escapeへ
		m_sChakurikuSeq = enSeqWaitSoldier;
		break;
	}

	m_sDoorOpenX += (2000-m_sDoorOpenX)/40;

	m_Add.x = 0;
	m_Add.y = 0;

}

void CEneH0007YusouHeli::LogicEscape()
{
	//-------------------------------------------------
	//任務完了
	//-------------------------------------------------

	//ふらふら
	m_Add.y = viiMath::Cos100((m_sTimer*3)%360)/4;

	if( m_sHigh > -WINDOW_W*100)
	{
		if( m_sHighAdd < 50*100) m_sHighAdd += 10;

		m_Pos.y -= m_sHighAdd/100;
		m_sHigh -= m_sHighAdd/100;

		ProperaWind();

	}
	else
	{
		m_sChakurikuSeq = enSeqCNone;
		SetTargetPos(0);	//画面外へ消える
	}
}

void CEneH0007YusouHeli::ProperaWind()
{
	Sint32 y = 0;

	if( viiSub::GameCounter()%4 ) return;

	new CEffectGroundSmoke( 4800+m_Pos.x-6400+viiSub::Rand()%1600 , m_Pos.y , DIR_LEFT );
	new CEffectGroundSmoke( 4800+m_Pos.x+6400-viiSub::Rand()%1600 , m_Pos.y , DIR_RIGHT );

	return;
}

void CEneH0007YusouHeli::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CEneH0007YusouHeli::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	m_bDead = gxTrue;

	if( m_sCrashSeq == 0 )
	{
		viiMus::PlaySound( enSoundCrashA );
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
		EffectMakeBigBomb(m_Pos.x , m_Pos.y);
		m_sCrashSeq ++;
	}
	if( m_sCrashSeq >= 600 )
	{
		//１０秒もあれば誰も見てないはず
		SetActionSeq( enActionSeqEnd );
	}

}

void CEneH0007YusouHeli::Draw()
{

//	viiDraw::Sprdmg( m_HitKurai.is_damage() , &SprCEneH0007YusouHeli[ 0 ] , m_Pos.x , m_Pos.y,PRIO_ENEMY , m_sAtribute|m_sAtributeDir , ARGB_DFLT );
#ifdef _VII_DEBUG_
	viiDbg::printf( 32,64,"TransHeli HP = %d/%d",m_HitKurai.get_hp() , m_HitKurai.GetTotalDamage() );
#endif
	m_pEnemyAnime->Draw( m_bDamage , m_Pos.x , m_Pos.y , ( m_sDirection == DIR_LEFT )? gxTrue : gxFalse , &SprCEneH0007YusouHeli[0] );

	m_CEneCmnSignal.Draw();

}

