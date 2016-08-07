//--------------------------------------------------------------------------------
//
// Stage00:ƒTƒ“ƒvƒ‹ƒGƒlƒ~[
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
//#include "EnemyTbl/CEneH0201VorzoyFlyTbl.h"
#include "../../CPenduram.h"


enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -30,
	enKuraiTop   = -16,
	enKuraiRight =  30,
	enKuraiBottom=  16,

	enScore = 300,
	enSpeedMax = 320,
};


gxSprite SprCEneH0201VorzoyFly[]=
{
	{enTexCEneH0201VorzoyFly,128,96,96,32,48,8},	//ƒ{ƒ‹ƒ]ƒC—A‘—ƒwƒŠ
	{enTexCEneH0201VorzoyFly,128,64,96,32,48,32},	//ƒ{ƒ‹ƒ]ƒC—A‘—ƒwƒŠ(Prop)
	{enTexCEneH0201VorzoyFly,128,224,96,32,48,32},	//ƒ{ƒ‹ƒ]ƒC—A‘—ƒwƒŠ(Prop)
};


CEneH0201VorzoyFly::CEneH0201VorzoyFly( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;
	m_Pos.z = PRIO_ENEMY;

	m_Add.x = 0;
	m_Add.y = 0;

	m_Tgt.x = 0;
	m_Tgt.y = 0;

	m_HitKurai.set_hp( enDefPointZakoM );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sDirection = DIR_LEFT;
	m_sAtribute  = ATR_DFLT;
	m_sControl   = 0;

	m_bDamage = gxFalse;

	m_pEasyLeynos = NULL;
	m_sLogicSeq  = 0;
	m_sLogicWait = 0;
	m_sPropTimer = 0;

	m_pPenduram = new CPenduram();

	m_pPenduram->SetWeight(100);
	m_pPenduram->Set( 10 );
	m_pPenduram->SetInfinity();

	CEneH0202VorzoyWP* pVor;
	pVor = new CEneH0202VorzoyWP( m_Pos.x , m_Pos.y );
	pVor->SetLogic( CEneH0003Vorzoy::enLogicCarry );
//	pVor->SeqCustomize(4,3,4);

	m_pTargetPos = NULL;
	m_bTettai    = gxFalse;

	CarryOn( pVor->GetLeynos() );

	m_sOffset = 0;
}


CEneH0201VorzoyFly::~CEneH0201VorzoyFly()
{

	//-----------------------------------------------------
	//ƒfƒXƒgƒ‰ƒNƒ^
	//-----------------------------------------------------
	if(m_pPenduram)
	{
		delete m_pPenduram;
	}
	else
	{
		ASSERT(m_pPenduram==NULL);
	}
}


void CEneH0201VorzoyFly::SeqInit()
{
	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}

	SetActionSeq( enActionSeqMain );
}

void CEneH0201VorzoyFly::SeqMain()
{
	//-----------------------------------------------------
	//ƒƒCƒ“
	//-----------------------------------------------------

	if( m_HitKurai.is_dead() )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

	if( m_bTettai )
	{
		//“P‘Þ’†‚ÉƒXƒNƒ[ƒ‹ƒAƒEƒg‚·‚éê‡‚É‚ÍI—¹
		if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
		{
			SetActionSeq( enActionSeqEnd );
		}
	}

	LogicAI();

	SetRader( enTypeMarkerFlyingObject );
	DirectionAdjust();

	m_pPenduram->Action();

	if( m_pEasyLeynos )
	{
		Sint32 x,y,r;

		r = (Sint32)m_pPenduram->GetRotation()+90;
		x = viiMath::Cos100( r )*16;
		y = viiMath::Sin100( r )*32;

		m_pEasyLeynos->set_pos( m_Pos.x +x , 3200+m_Pos.y + y , m_Pos.z );
		m_pEasyLeynos->set_dir(m_sDirection);
	}

	m_bDamage = m_HitKurai.is_damage();

	//‚­‚ç‚¢”»’è“o˜^
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

	m_sPropTimer ++;

}


void CEneH0201VorzoyFly::LogicAI()
{
	//-----------------------------------------------------
	//ˆÚ“®ƒƒWƒbƒN
	//-----------------------------------------------------
	m_sControl = 0;//Joy[0].psh;


	switch( m_sLogicSeq ){
	case 0:
		//--------------------------------
		//“oê
		//--------------------------------
		if( m_pTargetPos )
		{
			m_sControl = JOY_L;
			if( (*m_pTargetPos+m_sOffset*100) > m_Pos.x )
			{
				CarryOff( );
				m_sLogicWait = 0;
				m_sLogicSeq  = 20;
			}
		}
		else
		{
			if( m_sLogicWait < 120 )
			{
				m_sControl = JOY_L;
				m_sLogicWait ++;
			}
			else
			{
				m_sControl   = 0;
				m_sLogicWait = 0;
				m_sLogicSeq  = 10;
			}
		}
		break;

	case 10:
		//--------------------------------
		//‘Ò‹@
		//--------------------------------
		m_sControl   = 0;

		if( m_pTargetPos )
		{
			if( (*m_pTargetPos+m_sOffset) > m_Pos.x )
			{
				CarryOff( );
				m_sLogicWait = 0;
				m_sLogicSeq  = 20;
			}
		}
		else
		{
			m_sLogicWait ++;
			if( m_sLogicWait > 600 )
			{
				CarryOff( );
				m_sLogicWait = 0;
				m_sLogicSeq  = 20;
			}
		}
		break;

	case 20:
		m_bTettai  =gxTrue;
		m_sControl = JOY_R;
		break;
	}


/*
	return;

	Sint32 tx = GetTargetPlayer()->x;

	m_Tgt.x = GetTargetPlayer()->x;

	if( tx < m_Pos.x )
	{
		m_sControl = JOY_L;
	}
	else
	{
		m_sControl = JOY_R;
	}
*/

	if( m_sControl&JOY_L )
	{
		if( m_Add.x >= -enSpeedMax )
		{
			m_Add.x -= 8;
		}
	}
	else if( m_sControl&JOY_R )
	{
		if( m_Add.x <= enSpeedMax )
		{
			m_Add.x += 8;
		}
	}
	else
	{
		if( m_Add.x > 0 )
		{
			m_Add.x --;
		}
		else if( m_Add.x < 0 )
		{
			m_Add.x ++;
		}
	}

	if( m_bTettai )
	{
		if( m_Add.y > -200 ) m_Add.y -= 2;
	}

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;

/*
	if( m_pEasyLeynos )
	{
		if( m_pEasyLeynos->IsCarry() )
		{
			if( m_Pos.x < tx ) CarryOff();	//	m_pEasyLeynos->
		}
	}
*/

}


void CEneH0201VorzoyFly::CarryOff( )
{
	//--------------------------------------
	//ƒ{ƒ‹ƒ]ƒC‚ðØ‚è—£‚·
	//--------------------------------------

	if( m_pEasyLeynos == NULL ) return;

	m_pEasyLeynos->CarryOff();
	//m_pEasyLeynos->SetLogic( CEneH0003Vorzoy::enLogicNormal );

	m_pEasyLeynos = NULL;


}

void CEneH0201VorzoyFly::Atack()
{
	//-----------------------------------------------------
	//UŒ‚‰Â”\‚È‚çUŒ‚‚·‚é
	//-----------------------------------------------------

}


void CEneH0201VorzoyFly::SeqCrash()
{
	//-----------------------------------------------------
	//”j‰ó‚³‚ê‚½
	//-----------------------------------------------------

	if( m_pEasyLeynos )
	{
		if( m_pEasyLeynos->IsCarry() )
		{
			CarryOff();
		}
	}

	viiMus::PlaySound(enSoundExplosionMiddle);

	ScorePlus( enScore );

	viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );

	SetActionSeq( enActionSeqEnd );

}

void CEneH0201VorzoyFly::Draw()
{

	if( m_sPropTimer%4 < 2 )
	{
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0201VorzoyFly[ 1 ] , m_Pos.x , m_Pos.y , m_Pos.z-1 , m_sAtribute|m_sAtributeDir , ARGB_DFLT );
		if( m_sPropTimer%2 ) viiDraw::Sprdmg( m_bDamage , &SprCEneH0201VorzoyFly[ 2 ] , m_Pos.x , m_Pos.y+100 , m_Pos.z-1 , ATR_ALPHA_PLUS , ARGB_DFLT );
	}
	else
	{
		viiDraw::Sprdmg( m_bDamage , &SprCEneH0201VorzoyFly[ 2 ] , m_Pos.x , m_Pos.y , m_Pos.z-1 , m_sAtribute|m_sAtributeDir , ARGB_DFLT );
		if( m_sPropTimer%2 ) viiDraw::Sprdmg( m_bDamage , &SprCEneH0201VorzoyFly[ 1 ] , m_Pos.x , m_Pos.y+100 , m_Pos.z-1 , ATR_ALPHA_PLUS , ARGB_DFLT );
	}

	viiDraw::Sprdmg( m_bDamage , &SprCEneH0201VorzoyFly[ 0 ] , m_Pos.x , m_Pos.y , m_Pos.z , m_sAtribute|m_sAtributeDir , ARGB_DFLT );
}

