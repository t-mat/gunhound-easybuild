//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../Effect/EffectBase.h"

enum {
	enScrollOutWidth  = 32*100,
	enScrollOutHeight = 128*100,

	enKuraiLeft  = -30,
	enKuraiTop   = -34,
	enKuraiRight =  30,
	enKuraiBottom=  32,

	enScore = 300,
	enSpeed = 320,

	enTimeUpMax = 30*60,
};

gxSprite SprCEneH0108Hovered[]=
{
	{enTexCEneH0108Hovered,96  ,160,72,40,36,32},		//0:本体
	{enTexCEneH0108Hovered,169 ,160,16,16,4,2},			//1:太もも
	{enTexCEneH0108Hovered,192 ,152,32,48,12,12},		//2:脚部

	//ジオング時代

	{enTexCEneH0108Hovered,64,96+64,32,32,12,12},	//1:肩
	{enTexCEneH0108Hovered,64,96+96,32,16,24,8},	//2:腕

	{enTexCEneH0108Hovered,0 ,96+64,64,64,32,32},	//0:本体
};

CEneH0108Hovered::CEneH0108Hovered( Sint32 x, Sint32 y ,gxBool bForceStart)
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Base.x = x;
	m_Base.y = y;
	m_Base.z = 27000;

	m_HitKurai.set_hp( enDefPointBazooka );

	m_Obj.pos.x = m_Pos.x = x;
	m_Obj.pos.y = m_Pos.y = y;

	m_Add.x     = m_Add.y = 0;

	m_sAtribute = ATR_FLIP_X;
	m_sKasokuX  = 0;
	m_sKasokuY  = 0;
	m_sHigh     = 4800;		//ホバリングの高さ
	m_sWait     = 0;		//ウェイト時間
	m_bDamage   = gxFalse;
	m_bDeadable = gxFalse;
	m_sRot      = 0;
	m_bDisp     = gxFalse;
	m_sBombTimer = 0;
	m_sDirection = DIR_LEFT;
	m_sLegRot = 0;
	m_sWaitTime = 0;
	m_bReverse = gxFalse;

	m_sAtackWait = 16;
	m_sAtackCnt  = 0;
	m_sAtackLag  = 0;

	m_sTimeUpCnt = enTimeUpMax;

	m_bForceStart = bForceStart;

	m_bChicken = gxFalse;

	if( m_bForceStart )
	{
		//ワラワラゲームで使用するため、自滅させない
		m_bClear   = gxFalse;

		//初期形態はチキン形態で
		m_bChicken = gxTrue;
	}
	else
	{
		//スクロールシューティングで出てくるため自滅していい
		m_bClear = gxTrue;
	}

	m_sChangeTime = 0;
}


CEneH0108Hovered::~CEneH0108Hovered()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------


}


void CEneH0108Hovered::SeqInit()
{
	//-----------------------------------------------------
	//初期化
	//-----------------------------------------------------

	if( !m_bForceStart && IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	SetActionSeq( enActionSeqMain );

}


void CEneH0108Hovered::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------
	m_bDisp = gxTrue;

	if (m_HitKurai.is_dead())
	{
		if( 0 )	//!m_bChicken ) 
		{
			viiMus::PlaySound(enSoundExplosionMiddle);
			viiEff::Set( EFF_RING , m_Pos.x , m_Pos.y , argset(500) );
			viiEff::SetBombSimple( m_Pos.x , m_Pos.y , 5 , 3 );
			m_sChangeTime = 32;
			m_bChicken  =gxTrue;
			m_HitKurai.set_hp( enDefPointBazooka );
		}
		else
		{
			viiEff::Set( EFF_RING , m_Pos.x , m_Pos.y , argset(500) );
			SetActionSeq( enActionSeqCrash );
		}
		return;
	}

	LogicAI();
	Gimmick();

	SetRader( enTypeMarkerGroundObject );
	DirectionAdjust();

	if( m_bChicken )
	{
		AtackChicken();
	}
	else
	{
		if( m_sWaitTime >= 90 )
		{
			AtackNormal();
		}
	}

	m_Pos.x = m_Obj.pos.x;
	m_Pos.y = m_Obj.pos.y + viiMath::Cos100((m_sTimer*8)%360)*5-m_sHigh;

//	new CEffDashStorm( m_Pos.x+(-48+viiSub::Rand()%96)*100 , m_Obj.pos.y, 180 , 30 );

	m_bDamage = m_HitKurai.is_damage();

	if( m_sChangeTime == 0 )
	{
		m_HitKurai.SetHantei( ID_ENEMY_DEF , &m_Pos );
	}

}


void CEneH0108Hovered::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	//タイムアップ時
	if( m_sTimeUpCnt > 0 )
	{
		m_sTimeUpCnt --;
	}
	if( m_sTimeUpCnt == 0 )
	{
		if( !m_bForceStart )
		{
			m_Obj.pos.x += 200;
			if( IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
			{
				SetActionSeq( enActionSeqEnd );
			}
			return;
		}
	}


	if( m_bChicken )
	{
		LogicChicken();
	}
	else
	{
		LogicNormal();

		if( m_sWaitTime >= 60*5 ) 
		{
			m_bChicken = gxTrue;
		}
		m_sWaitTime ++;


	}

}


void CEneH0108Hovered::LogicNormal()
{
	//-----------------------------------------------------
	//標準
	//-----------------------------------------------------

	if( m_bReverse )
	{
		m_Base.z += ((270*100)-m_Base.z)/60;
	}
	else
	{
		m_Base.z += ((90*100)-m_Base.z)/60;
	}
	m_Pos.x = m_Base.x+viiMath::Cos100(m_Base.z/100)*128;
	m_Pos.y = m_Base.y+viiMath::Sin100(m_Base.z/100)*96;

	m_Obj.pos.x = m_Pos.x;
	m_Obj.pos.y = m_Pos.y;

}


void CEneH0108Hovered::LogicChicken()
{
	//-----------------------------------------------------
	//チキンレッグ
	//-----------------------------------------------------
	Sint32 tx,ty,vx,vy;

	vx = tx = GetTargetPlayer()->x;
	vy = ty = GetTargetPlayer()->y;

	if( tx < m_Pos.x )
	{
		tx += 220*100;
	}
	else if( tx > m_Pos.x )
	{
		tx -= 220*100; 
	}

	if(m_sWait==0)
	{
		m_sWait = 3;
		if(m_Obj.pos.x < tx)
		{
			m_sKasokuX = 10;
		}
		else
		{
			m_sKasokuX =-10;
		}

		if(m_Obj.pos.y < ty)
		{
			m_sKasokuY = 16;
		}
		else
		{
			m_sKasokuY =-16;
		}
	}
	else
	{
		m_sWait --;
	}

	m_Add.x += m_sKasokuX;
	m_Add.y += m_sKasokuY;

	if(m_Add.x <= -enSpeed) m_Add.x = -enSpeed;
	if(m_Add.x >=  enSpeed) m_Add.x =  enSpeed;
	if(m_Add.y <= -enSpeed) m_Add.y = -enSpeed;
	if(m_Add.y >=  enSpeed) m_Add.y =  enSpeed;


	if( m_sChangeTime > 0 )
	{
		m_Obj.mov.x = m_Add.x;
		m_sChangeTime --;
	}
	else
	{
		m_Obj.mov.x = m_Add.x;
		m_Obj.mov.y = 120;

		if( m_Obj.easy_collision_check() )
		{
			m_Obj.pos.y -= 180;
		}
		else if( pGame->GetMarineLine() && m_Obj.pos.y > pGame->GetMarineLine() )
		{
			m_Obj.pos.y -= 180;
		}

	}

	//---------------------------------------
	//スモークを出す
	//---------------------------------------
	if( m_sTimer%4==0)
	{
		if( m_sDirection ==  DIR_LEFT )
		{
			new CEffectGroundSmoke( m_Pos.x+viiSub::Rand()%1600 , m_Pos.y, DIR_LEFT );
		}
		else
		{
			new CEffectGroundSmoke( m_Pos.x+viiSub::Rand()%1600 , m_Pos.y , DIR_RIGHT );
		}
	}

	m_Obj.pos.x += m_Obj.mov.x;
	m_Obj.pos.y += m_Obj.mov.y;

	if( m_Obj.mov.x < -80 && vx<m_Pos.x) m_sDirection = DIR_LEFT;
	if( m_Obj.mov.x >  80 && vx>m_Pos.x) m_sDirection = DIR_RIGHT;

}


void CEneH0108Hovered::Gimmick()
{
	//-----------------------------------------------------
	//ギミック
	//-----------------------------------------------------
	Sint32 sLegDir = 0;

	if( m_Obj.mov.x > 0 )
	{
		if(m_sDirection == DIR_RIGHT )
		{
			sLegDir = -1;
		}
		else
		{
			sLegDir = 1;
		}
	}
	else if( m_Obj.mov.x < 0 )
	{
		if(m_sDirection == DIR_RIGHT )
		{
			sLegDir = 1;
		}
		else
		{
			sLegDir = -1;
		}
	}

	if( !m_bChicken )
	{
		//飛行形態
		m_sLegRot += (-180-m_sLegRot)/10;
		if( m_sRot < 90 || m_sRot > 100 ) m_sRot +=2;
		m_sRot = m_sRot%180;
	}
	else
	{
		//チキン形態
		if( sLegDir < 0 )
		{
			if( m_sLegRot >= -30 ) m_sLegRot -= 2;
		}
		else
		{
			m_sLegRot += (0-m_sLegRot)/10;
		}

		m_sRot +=2;
		m_sRot = m_sRot%180;
	}

}


gxBool CEneH0108Hovered::AtackChicken()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------

	if( m_sAtackWait > 0)
	{
		m_sAtackWait --;
		return gxTrue;
	}
	if( m_sAtackLag > 0)
	{
		m_sAtackLag --;
		return gxTrue;
	}

	Sint32 ax,ay;

	ax = m_Pos.x;
	ay = m_Pos.y-1600;
//	CAtkH0001Ryudan *p;
	viiEff::Set(EFF_MAZZLE,ax,ay,NULL);
//	p = new CAtkH0001Ryudan( ax , ay , viiSub::SetKakudo(m_sDirection,-35), 480 );
//	p->SetForce(CAtkH0001Ryudan::enForceID_Enemy);

	CEne0001EneSuperBall *q;
	q = new CEne0001EneSuperBall( ax,ay ,viiSub::SetKakudo(m_sDirection,-35), 480  );
	q->SetPattern( CEne0001EneSuperBall::enPatternMicroMissile );
	q->SetMaxSpeed( 720 );
	q->SetScrollOutRange( 128 );
	q->SetLevel(50);
	q->SetScale( 0.5f );


	m_sAtackCnt ++;
	m_sAtackLag = 16;

	if(m_sAtackCnt >= 2 )
	{
		m_sAtackWait = 180;
		m_sAtackCnt = 0;
	}
	return gxFalse;
}


gxBool CEneH0108Hovered::AtackNormal()
{
	//-----------------------------------------------------
	//ノーマル攻撃
	//-----------------------------------------------------

	Sint32 ax,ay,x,y,r=get_valken_rot( m_Pos.x , m_Pos.y);

	ax = m_Pos.x;
	ay = m_Pos.y-6400;

	if( m_sAtackWait > 0)
	{
		m_sAtackWait --;
		return gxTrue;
	}
	if( m_sAtackLag > 0)
	{
		m_sAtackLag --;
		return gxTrue;
	}

	{
		x = viiMath::Cos100(-90)*8;
		y = viiMath::Sin100(-90)*8;

/*
		viiEff::Set( EFF_MAZZLE , ax+x , ay+y , NULL );
		new CEne0001EneBullet( ax+x , ay+y, r , 320+m_sAtackCnt*50 ,1);

		x = viiMath::Cos100(90)*8;
		y = viiMath::Sin100(90)*8;

		viiEff::Set( EFF_MAZZLE , ax+x , ay+y , NULL );
		new CEne0001EneBullet( ax+x , ay+y, r , 320+m_sAtackCnt*50 ,1);
*/
	}


	CAtkH0002GMissile *p;
	p = new CAtkH0002GMissile(ax,ay , 180+20 , 130);
	p->SetForce( CAtkH0001Missile::enForceID_Enemy );
	p->SetRotationFix(180+20);

	m_sAtackCnt ++;

	if( m_sAtackCnt  >= 1 )
	{
		m_sAtackCnt = 0;
		m_sAtackWait = 90;
	}

	m_sAtackLag  = 2;

	return gxTrue;
}


void CEneH0108Hovered::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------

	m_sBombTimer ++;

	if( m_sBombTimer == 3 )
	{
		viiMus::PlaySound(enSoundExplosionMiddle);

		viiEff::SetBombDeluxe( m_Pos.x , m_Pos.y , 5 , 3 );
		EffectMakeBigBomb(m_Pos.x , m_Pos.y);

	}
	else if( m_sBombTimer == 8 )
	{
		m_bDisp = gxFalse;

		viiEff::Set( EFF_DAMAGE_PARTS,m_Pos.x-viiSub::Rand()%3200,m_Pos.y-viiSub::Rand()%3200,argset( 0,m_sAtribute,ARGB_DFLT,	0,	100,	0) ,&SprCEneH0108Hovered[1] );
		viiEff::Set( EFF_DAMAGE_PARTS,m_Pos.x+viiSub::Rand()%3200,m_Pos.y+viiSub::Rand()%3200,argset( 0,m_sAtribute,ARGB_DFLT,	0,	100,	0) ,&SprCEneH0108Hovered[2] );

		ScorePlus( enScore );
	}
	else if( m_sBombTimer == 24 )
	{
		CEffectDmgprt *p;
		p = new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, 180+15 , 400 );
		p->SetSprite( &SprCEneH0108Hovered[ 0 ] );

		p=new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, 180+65 , 320 );
		p->SetSprite( &SprCEneH0108Hovered[ 1 ] );

		p=new CEffectDmgprt( m_Pos.x, m_Pos.y -1600, -60    , 500 );
		p->SetSprite( &SprCEneH0108Hovered[ 2 ] );
	}
	else if( m_sBombTimer >= 60 )
	{
		if( !m_bDead )
		{
			m_bDead = gxTrue;
		}

		if( m_bClear )
		{
			m_bDeadable = gxTrue;
			SetActionSeq( enActionSeqEnd );
		}
	}


}


void CEneH0108Hovered::Draw()
{
	//---------------------------------------------
	//表示
	//---------------------------------------------

	Sint32 dir = m_sDirection*-1;
	Sint32 x,y,rot;
	m_sAtribute = 0;

	if( !m_bDisp ) return;

	if(	m_sDirection==DIR_RIGHT)
	{
		m_sAtribute = ATR_FLIP_X;
	}

	rot = 30+abs(-90 + m_sRot);
//	rot = 30+abs(0 + m_sRot);

	x = viiMath::Cos100( rot )*8;
	y = viiMath::Sin100( rot )*8;

	//ボディ
	viiDraw::Sprdmg(
		m_bDamage ,
		&SprCEneH0108Hovered[0],
		m_Pos.x,
		m_Pos.y,
		PRIO_ENEMY,
		m_sAtribute,
		ARGB_DFLT,1.f,
		-rot/6 );

	//太もも
	viiDraw::Sprdmg(
		m_bDamage ,
		&SprCEneH0108Hovered[1],
		m_Pos.x,
		m_Pos.y,
		PRIO_ENEMY,
		m_sAtribute,
		ARGB_DFLT,1.0f,rot-90);

	//レッグ
	viiDraw::Sprdmg(
		m_bDamage ,
		&SprCEneH0108Hovered[2],
		m_Pos.x-x*dir,
		m_Pos.y+y,
		PRIO_ENEMY,
		m_sAtribute,
		ARGB_DFLT,1.0f,m_sLegRot+50);

	x = viiMath::Cos100( rot/2 )*-12;
	y = viiMath::Sin100( rot/2 )*12;

	//腕
	viiDraw::Sprdmg(
		m_bDamage ,
		&SprCEneH0108Hovered[4],
		m_Pos.x-x*dir,
		m_Pos.y+y-200,
		PRIO_ENEMY,
		m_sAtribute,
		ARGB_DFLT,1.f,-25);

	//肩
	viiDraw::Sprdmg(
		m_bDamage ,
		&SprCEneH0108Hovered[3],
		m_Pos.x+800*dir,
		m_Pos.y-8*100,
		PRIO_ENEMY,
		m_sAtribute,
		ARGB_DFLT,1.0f,rot/2-45);

}


