//--------------------------------------------------------------------------------
//
// Stage00:ƒŒ[ƒ‹’e–‹ƒLƒƒƒmƒ“
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../effect/EffectBase.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -30,
	enKuraiTop   = -34,
	enKuraiRight =  30,
	enKuraiBottom=  0,

	enScore = 300,
	enSpeed = 240,
	enRotSpeed = 1,
};


gxSprite SprCEneH0409DanmakuCannon[]=
{
	{enTexCEneH0409SlideCannon, 208 , 32*0 , 48 , 32 , 16 , 14 },	//‚R˜A’¼i’e–‹ƒLƒƒƒmƒ“
	{enTexCEneH0409SlideCannon, 208 , 32*1 , 48 , 32 , 16 , 14 },	//‚R˜A’¼i’e–‹ƒLƒƒƒmƒ“
	{enTexCEneH0409SlideCannon, 208 , 32*2 , 48 , 32 , 16 , 14 },	//‚R˜A’¼i’e–‹ƒLƒƒƒmƒ“

	{enTexCEneH0409SlideCannon, 208 , 96+48*0 , 48 , 48 , 16 , 20 },	//‚R˜A’¼i’e–‹ƒLƒƒƒmƒ“
	{enTexCEneH0409SlideCannon, 208 , 96+48*1 , 48 , 48 , 16 , 20 },	//‚R˜A’¼i’e–‹ƒLƒƒƒmƒ“
	{enTexCEneH0409SlideCannon, 208 , 96+48*2 , 48 , 48 , 16 , 20 },	//‚R˜A’¼i’e–‹ƒLƒƒƒmƒ“


	{enTexCEneH0409SlideCannon, 208 , 0 , 48 , 32 , 16 , 16 },	//‚R˜A’¼i’e–‹ƒLƒƒƒmƒ“

	{enTexCEneH0409SlideCannon, 208-48*0 , 32 , 48 , 48 , 16 , 24 },	//‚R˜A’¼iŠgUƒLƒƒƒmƒ“
	{enTexCEneH0409SlideCannon, 208-48*1 , 32 , 48 , 48 , 16 , 24 },	//‚R˜A’¼iŠgUƒLƒƒƒmƒ“
	{enTexCEneH0409SlideCannon, 208-48*2 , 32 , 48 , 48 , 16 , 24 },	//‚R˜A’¼iŠgUƒLƒƒƒmƒ“
};


void MakeSetBullet( Sint32 sType , Sint32 x , Sint32 y , Sint32 r1 ,Sint32 dst, Sint32 r2 , Sint32 spd)
{
	//’Êí’e

	x += (viiMath::Cos100(r1)*dst)/100;
	y += (viiMath::Sin100(r1)*dst)/100;
	viiEff::Set(EFF_MAZZLE,x,y,NULL);
	new CEne0001EneBullet( x, y , r2 , spd ,0);

}



CEneH0409SlideCannon::CEneH0409SlideCannon( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_Tgt.x = x;
	m_Tgt.y = y;

	m_bAtack     = gxFalse;
	m_sAtackType = 0;
	m_sRotation    = 90;
	m_sTgtRotation = 90;
	m_bArrive      = gxFalse;
	m_sSprite      = 0;
	m_sAtackTimer  = 0;

	m_sShotWait= 0;
	m_sShotCnt = 0;
	m_sShotlag = 0;

	m_HitKurai.set_hp( enHitPointHigh );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sShotTotalCnt = 0;
	m_sDirection = DIR_LEFT;
	m_sAtribute  = ATR_DFLT;

	m_bDamage = gxFalse;

}


CEneH0409SlideCannon::~CEneH0409SlideCannon()
{

	//-----------------------------------------------------
	//ƒfƒXƒgƒ‰ƒNƒ^
	//-----------------------------------------------------

}


void CEneH0409SlideCannon::SeqMain()
{
	//-----------------------------------------------------
	//ƒƒCƒ“
	//-----------------------------------------------------
	m_sAtackTimer ++;

	if( m_bClear || m_HitKurai.is_dead() )
	{
		SetActionSeq( enActionSeqCrash );
		return;
	}

/*
	if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}
*/

	LogicAI();

	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	m_bDamage = m_HitKurai.is_damage();

	//‚­‚ç‚¢”»’è“o˜^
	if( m_bAtack )
	{
		m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );
	}

}


void CEneH0409SlideCannon::LogicAI()
{
	//-----------------------------------------------------
	//ˆÚ“®ƒƒWƒbƒN
	//-----------------------------------------------------

	if( !m_bArrive)
	{
		if( m_Tgt.x < m_Pos.x )
		{
			m_Pos.x += -enSpeed;
			if( m_Pos.x < m_Tgt.x  )
			{
 				m_Pos.x = m_Tgt.x;
 			}
		}
		else if( m_Tgt.x > m_Pos.x )
		{
			m_Pos.x += enSpeed;
			if( m_Pos.x > m_Tgt.x  )
			{
 				m_Pos.x = m_Tgt.x;
 			}
		}
		else
		{
			m_bArrive = gxTrue;
		}
	}

	//------------------------------------------------
	//–C‘ä‰ñ“]
	//------------------------------------------------

	Sint32 sAdd = (m_sTgtRotation-m_sRotation)/30;

	if( sAdd == 0 )
	{
		if( m_sAtackTimer >= 0)
		{
			if( m_sTgtRotation < m_sRotation )
			{
				m_sRotation += -enRotSpeed;
				if( m_sRotation < m_sTgtRotation  )
				{
		 			m_sRotation = m_sTgtRotation;
		 		}
			}
			else if( m_sTgtRotation > m_sRotation )
			{
				m_sRotation += enRotSpeed;
				if( m_sRotation > m_sTgtRotation  )
				{
		 			m_sRotation = m_sTgtRotation;
		 		}
			}
		}
	}

	m_sRotation += sAdd;

	if( m_bAtack )
	{
		if(m_sAtackTimer>=0) Atack();
	}

}


void CEneH0409SlideCannon::Atack()
{
	//-----------------------------------------------------
	//UŒ‚‰Â”\‚È‚çUŒ‚‚·‚é
	//-----------------------------------------------------

	if( m_sAtackType == 0 )
	{
		//‚R˜A’e
		m_Add.x =viiMath::Cos100((m_sAtackTimer*2)%360)*4;
		m_Pos.x += m_Add.x;

		m_sSprite = 3+viiSub::GameCounter()%(5*3)/5;

		m_sRotation = 90;

		if( m_sShotWait > 0 )
		{
			m_sShotWait --;
			return;
		}
		if( m_sShotlag > 0 )
		{
			m_sShotlag --;
			return;
		}

		MakeSetBullet( 0, m_Pos.x , m_Pos.y , m_sRotation ,3200, m_sRotation+80 , 120*6 );
		MakeSetBullet( 0, m_Pos.x , m_Pos.y , m_sRotation ,3200, m_sRotation-80 , 120*6 );
		MakeSetBullet( 0, m_Pos.x , m_Pos.y , m_sRotation ,3200, m_sRotation-40 , 120*4 );
		MakeSetBullet( 0, m_Pos.x , m_Pos.y , m_sRotation ,3200, m_sRotation+40 , 120*4 );
		MakeSetBullet( 0, m_Pos.x , m_Pos.y , m_sRotation ,3200, m_sRotation-45 , 160*3 );
		MakeSetBullet( 0, m_Pos.x , m_Pos.y , m_sRotation ,3200, m_sRotation+45 , 160*3 );
		m_sShotCnt ++;
		m_sShotlag = 8;
		if( m_sShotCnt >= 16 )
		{
			m_sShotWait= 80;
			m_sShotCnt = 0;
		}

//		m_sSprite      = viiSub::GameCounter()%(2*3)/2;
	}

	if( m_sAtackType == 1 )
	{
		//‚R˜AŠgU’e
		Sint32 sAdd = 0;
		m_sSprite = 3+viiSub::GameCounter()%(5*3)/5;

		if( m_sShotTotalCnt%3 == 2)
		{
//			sAdd = 64*100;
		}

		Sint32 r = get_valken_rot( m_Pos.x+sAdd , m_Pos.y);
		m_sRotation = r;

		if( m_sShotWait > 0 )
		{
			m_Pos.x += (m_Add.x-m_Pos.x)/30;
			m_sShotWait --;
			return;
		}

		if( m_sShotlag > 0 )
		{
			m_sShotlag --;
			return;
		}

		MakeSetBullet( 0, m_Pos.x , m_Pos.y , m_sRotation-70 ,3200, m_sRotation-70 , 380-m_sShotCnt*10  );
		MakeSetBullet( 0, m_Pos.x , m_Pos.y , m_sRotation-50 ,3200, m_sRotation-50 , 380-m_sShotCnt*10  );
		MakeSetBullet( 0, m_Pos.x , m_Pos.y , m_sRotation-30 ,3200, m_sRotation-30 , 380-m_sShotCnt*10  );
		MakeSetBullet( 0, m_Pos.x , m_Pos.y , m_sRotation-10 ,3200, m_sRotation-10 , 380-m_sShotCnt*10  );
		MakeSetBullet( 0, m_Pos.x , m_Pos.y , m_sRotation+10 ,3200, m_sRotation+10 , 380-m_sShotCnt*10  );
		MakeSetBullet( 0, m_Pos.x , m_Pos.y , m_sRotation+30 ,3200, m_sRotation+30 , 380-m_sShotCnt*10  );
		MakeSetBullet( 0, m_Pos.x , m_Pos.y , m_sRotation+50 ,3200, m_sRotation+50 , 380-m_sShotCnt*10  );
		MakeSetBullet( 0, m_Pos.x , m_Pos.y , m_sRotation+70 ,3200, m_sRotation+70 , 380-m_sShotCnt*10  );
		m_sShotCnt ++;
		m_sShotlag = 4;
		if( m_sShotCnt >= 6 )
		{
			Sint32 sTbl[]={
				-124,-64,0,64,128,
			};
			m_Add.x = m_Tgt.x+sTbl[viiSub::Rand()%5]*100;
			m_sShotWait= 64;
			m_sShotCnt = 0;
			m_sShotTotalCnt ++;
		}
	}

	if( m_sAtackType == 2 )
	{
		//ƒŒ[ƒXƒQ[ƒ€’e–‹
		Sint32 spd,sWidth=35;
		sWidth = 25;
		m_sRotation = 90+(sWidth*viiMath::Cos100((m_sAtackTimer*3)%360))/100;
		spd = 250;
		if( m_sAtackTimer%6==0)
		{
			MakeSetBullet( 0, m_Pos.x , m_Pos.y , m_sRotation-15 ,3200, m_sRotation-80 , 800 );
			MakeSetBullet( 0, m_Pos.x , m_Pos.y , m_sRotation-15 ,3200, m_sRotation+80 , 800 );
			MakeSetBullet( 0, m_Pos.x , m_Pos.y , m_sRotation-15 ,3200, m_sRotation-8 , 250+spd );
			MakeSetBullet( 0, m_Pos.x , m_Pos.y , m_sRotation+15 ,3200, m_sRotation+8 , 250+spd );
			m_sSprite      = 0;
		}
	}

}

void CEneH0409SlideCannon::SeqCrash()
{
	//-----------------------------------------------------
	//”j‰ó‚³‚ê‚½
	//-----------------------------------------------------

	if( !m_bDead )
	{
		m_bDead = gxTrue;
		viiMus::PlaySound(enSoundExplosionMiddle);
		ScorePlus( enScore );
		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
	}

	if( m_sTimer%4 == 0 )
	{
		new CEffectBlackFire ( m_Pos.x , m_Pos.y , NULL );
		viiEff::SetBombSimple( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
	}

	if( m_bClear )
	{
		EffectMakeBigBomb(m_Pos.x , m_Pos.y);
		SetActionSeq( enActionSeqEnd );
	}
}

void CEneH0409SlideCannon::Draw()
{
	if( m_bDead ) return;
	viiDraw::Sprdmg( m_bDamage , &SprCEneH0409DanmakuCannon[ m_sSprite ] , m_Pos.x , m_Pos.y , PRIO_ENEMY , m_sAtribute , ARGB_DFLT ,1.f, m_sRotation);

}

