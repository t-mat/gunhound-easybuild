//--------------------------------------------------------------------------------
//
// Stage00:自爆ジャンパー
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../Effect/EffectBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -16,
	enKuraiTop   = -16,
	enKuraiRight =  16,
	enKuraiBottom=  0,

	enScore = 80,
};

gxSprite SprCEneH0107Walker[] =
{
	{ enTexCEneH0107Walker,0,96,40,24,16,16},	//ボディ
	{ enTexCEneH0107Walker,40,96,16,16,8,2},	//レッグ
	{ enTexCEneH0107Walker,64,96,24,16,16,4},	//フット

//	{ enTexCEneH0107Walker,0*32,96+0*32,32,32,16,32},
//	{ enTexCEneH0107Walker,1*32,96+0*32,32,32,16,32},
//	{ enTexCEneH0107Walker,2*32,96+0*32,32,32,16,32},
//	{ enTexCEneH0107Walker,3*32,96+0*32,32,32,16,32},
//	{ enTexCEneH0107Walker,4*32,96+0*32,32,32,16,32},
//	{ enTexCEneH0107Walker,5*32,96+0*32,32,32,16,32},
//	{ enTexCEneH0107Walker,0*32,96+1*32,32,32,16,32},

};

enum {
	enSeqJumpInit,
	enSeqJumpMain,
	enSeqJumpEnd,
};

CEneH0107Walker::CEneH0107Walker( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_Add.x = 0;
	m_Add.y = 0;

	m_HitKurai.set_hp( enDefPointMinimum );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_HitAtari.set_hp( 1 );
	m_HitAtari.set_ap( 0 );
	m_HitAtari.ax1 = enKuraiLeft;
	m_HitAtari.ay1 = enKuraiTop;
	m_HitAtari.ax2 = enKuraiRight;
	m_HitAtari.ay2 = enKuraiBottom;


	m_sDirection = DIR_LEFT;
	m_sAtribute  = ATR_DFLT;
	m_bDamage    = gxFalse;
	m_sSequence  = enSeqJumpMain;//enSeqJumpInit;

	m_Obj.pos.x = m_Pos.x;
	m_Obj.pos.y = m_Pos.y;

	m_Obj.mov.x = 0;
	m_Obj.mov.y = 0;

	m_sLegRotation = 0;

	m_sSprite  = 0;
	m_bChakuti = gxFalse;
	m_sJumpWait= 0;

	m_bJibaku = gxFalse;
	m_sJibakuCnt = 0;
	m_sBombWait = 2;
}


CEneH0107Walker::~CEneH0107Walker()
{
	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------


}


void CEneH0107Walker::SeqInit()
{
	SetRader( enTypeMarkerGroundObject );

	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}

	SetActionSeq( enActionSeqMain );

}


void CEneH0107Walker::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( m_HitKurai.is_dead() )
	{
		viiMus::PlaySound(enSoundExplosionMiddle);
		ScorePlus( enScore );
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
		SetActionSeq( enActionSeqCrash );
		return;
	}

	LogicAI();

	m_Pos.x = m_Obj.pos.x;
	m_Pos.y = m_Obj.pos.y;

	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	m_bDamage = m_HitKurai.is_damage();

	if( m_bJibaku )
	{
		m_sJibakuCnt --;
		if(m_sJibakuCnt==0)
		{
			viiMus::PlaySound(enSoundExplosionMiddle);
			EffectMakeBigBomb(m_Pos.x , m_Pos.y);
			SetActionSeq( enActionSeqCrash );
		}
	}
	else
	{
		if( m_HitAtari.IsImpacted() )
		{
			if( !m_bJibaku )
			{
				viiEff::Set(EFF_RING,m_Pos.x, m_Pos.y,argset(250));
				m_bJibaku = gxTrue;
				m_sJibakuCnt = 180;
				m_HitKurai.set_hp( enDefPointZakoM );
			}
		}
		else
		{
			m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );
		}
	}

	//くらい判定登録
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );
}


void CEneH0107Walker::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

/*
	if( Joy[0].psh&BTN_A ) m_sLegRotation --;
	if( Joy[0].psh&BTN_B ) m_sLegRotation ++;
	if( Joy[0].psh&JOY_D )
	{
		if( m_sLegRotation > 0 ) m_sLegRotation --;
		if( m_sLegRotation < 0 ) m_sLegRotation ++;
	}
	if( m_sLegRotation < -100 ) m_sLegRotation = -100;
	if( m_sLegRotation >  100 ) m_sLegRotation =  100;
	return;
*/

	SeqJump();

	m_Obj.mov.x = m_Add.x;
	m_Obj.mov.y = m_Add.y;

	if( m_Obj.easy_collision_check() )
	{
		m_Add.x = 0;
		m_Add.y = 0;
		m_bChakuti = gxTrue;
	}
	else
	{
		m_Add.y += 10;
		m_bChakuti = gxFalse;
	}

	m_Obj.pos.x += m_Obj.mov.x;
	m_Obj.pos.y += m_Obj.mov.y;

}


void CEneH0107Walker::SeqJump()
{
	//-----------------------------------------------------
	//ジャンプ
	//-----------------------------------------------------

	switch( m_sSequence ){
	case enSeqJumpInit:
		//------------------------------------------
		//初期化
		//------------------------------------------
		if(m_bJibaku)
		{
			//自爆を決めたら動かない
			m_sLegRotation += (90-m_sLegRotation)/10;
			break;
		}

		if( m_sJumpWait > 30 )
		{
			m_Add.y = -450;

			if(GetTargetPlayer()->x < m_Pos.x )
			{
				m_Add.x = -100;
				m_sDirection = DIR_RIGHT;
			}
			else
			{
				m_Add.x = 100;
				m_sDirection = DIR_LEFT;
			}
			m_sSequence = enSeqJumpMain;
		}

		m_sJumpWait ++;
		m_sLegRotation += (0-m_sLegRotation)/10;
		break;

	case enSeqJumpMain:
		//------------------------------------------
		//ジャンプ中
		//------------------------------------------
		if( m_Add.y > 100 )
		{
			m_sLegRotation += (0-m_sLegRotation)/10;
		}
		else
		{
			m_sLegRotation += (-100-m_sLegRotation)/10;
		}

		if( m_bChakuti )
		{
			m_sLegRotation = 0;
			m_sJumpWait = 0;
			m_sSequence = enSeqJumpEnd;
		}
		break;

	case enSeqJumpEnd:
		//------------------------------------------
		//着地完了
		//------------------------------------------
		m_sLegRotation += (100-m_sLegRotation)/10;
		m_sJumpWait ++;
		if( m_sJumpWait > 30 )
		{
			m_sJumpWait = 0;
			m_sSequence = enSeqJumpInit;
		}
		break;
	default:
		break;
	}

}


void CEneH0107Walker::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

}


void CEneH0107Walker::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	if( m_bJibaku && m_sBombWait )
	{
		m_sBombWait --;
		m_HitAtari.ax1 = -32;
		m_HitAtari.ay1 = -32;
		m_HitAtari.ax2 = 32;
		m_HitAtari.ay2 = 32;
		m_HitAtari.set_ap( 10 );
		m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );
		return;
	}

	SetActionSeq( enActionSeqEnd );

}


void CEneH0107Walker::Draw()
{
	//-----------------------------------------------------
	//描画
	//-----------------------------------------------------
	Sint32 ax,ay,bx,by;
	Sint32 sRot1 = m_sLegRotation;
	Sint32 sRot2 = m_sLegRotation;
	Sint32 sDir = m_sDirection*-1;
	Sint32 sHeight = 18*100;

	if( m_sLegRotation > 0 )
	{
		//------------------------------------------
		//リバウンド中
		//------------------------------------------
		ax = m_Pos.x + viiMath::Cos100(90)*10*sDir;
		ay = m_Pos.y + viiMath::Sin100(90)*10;
		bx = ax + viiMath::Cos100(m_sLegRotation-90)*12*sDir;
		by = ay + viiMath::Sin100(m_sLegRotation-90)*10;
		sRot1 = -m_sLegRotation;
		sRot2 = 0;
	}
	else if( m_sLegRotation <= 0 )
	{
		//------------------------------------------
		//ジャンプ中
		//------------------------------------------
		sRot1 = m_sLegRotation/2;
		ax = m_Pos.x + viiMath::Cos100(-sRot1+90)*10*sDir;
		ay = m_Pos.y + viiMath::Sin100(-sRot1+90)*10;
		bx = m_Pos.x;
		by = m_Pos.y;
		sRot2 = 3*m_sLegRotation/4;
	}

	viiDraw::Sprdmg( m_bDamage , &SprCEneH0107Walker[ 0 ] , bx , by-sHeight , PRIO_ENEMY , m_sAtribute|m_sAtributeDir , ARGB_DFLT );
	viiDraw::Sprdmg( m_bDamage , &SprCEneH0107Walker[ 1 ] , bx , by-sHeight , PRIO_ENEMY , m_sAtribute|m_sAtributeDir , ARGB_DFLT , 1.0f, sRot1 );
	viiDraw::Sprdmg( m_bDamage , &SprCEneH0107Walker[ 2 ] , ax , ay-sHeight , PRIO_ENEMY , m_sAtribute|m_sAtributeDir , ARGB_DFLT , 1.0f, sRot2 );

	if( m_bJibaku )
	{
		//------------------------------------------
		//自爆直前の点滅
		//------------------------------------------

		Sint32 sCol = 128+(120*viiMath::Cos100(m_sJibakuCnt*8%360))/100;
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0107Walker[ 0 ] , bx , by-sHeight , PRIO_ENEMY , ATR_ALPHA_PLUS|m_sAtribute|m_sAtributeDir , ARGB(sCol,0xff,0xff,0x00) );
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0107Walker[ 1 ] , bx , by-sHeight , PRIO_ENEMY , ATR_ALPHA_PLUS|m_sAtribute|m_sAtributeDir , ARGB(sCol,0xff,0xff,0x00) , 1.0f, sRot1 );
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0107Walker[ 2 ] , ax , ay-sHeight , PRIO_ENEMY , ATR_ALPHA_PLUS|m_sAtribute|m_sAtributeDir , ARGB(sCol,0xff,0xff,0x00) , 1.0f, sRot2 );
	}

	return;

}

