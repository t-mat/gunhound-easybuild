//--------------------------------------------------------------------------------
//
// Stage00:サンプルエネミー
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
#include "../../Effect/EffectBase.h"
//#include "EnemyTbl/CEneH0004TachikomaTbl.h"

enum {
	enScrollOutWidth  = 60*100,
	enScrollOutHeight = 60*100,

	enKuraiLeft  = -30,
	enKuraiTop   = -34,
	enKuraiRight =  30,
	enKuraiBottom=  0,

	enScore = 500,
};

gxSprite SprCEneH0004Tachikoma[]=
{
	{enTexCEneH0107Walker,0      ,96+224,24,32,5,16},	//0:ARM
	{enTexCEneH0107Walker,0      ,96+32,64,32,28,32},	//1:BODY
	{enTexCEneH0107Walker,64+48*0,96+32,48,32,24,12},	//2:LEG1
	{enTexCEneH0107Walker,64+48*1,96+32,48,32,24,12},	//2:LEG1

};


CEneH0004Tachikoma::CEneH0004Tachikoma( Sint32 x, Sint32 y )
{
	//-----------------------------------------------------
	//コンストラクタ
	//-----------------------------------------------------
	m_Pos.x = x;
	m_Pos.y = y;

	m_Tgt.x = m_Pos.x;
	m_Tgt.y = m_Pos.y;

	m_HitKurai.set_hp( enDefPointZakoM );

	m_HitKurai.ax1 = enKuraiLeft;
	m_HitKurai.ay1 = enKuraiTop;
	m_HitKurai.ax2 = enKuraiRight;
	m_HitKurai.ay2 = enKuraiBottom;

	m_sJumpCnt = 0;
	m_sControl   = 0;	//PAD
	m_sLogicWait = 0;	//思考時間
	m_sJumpWait  = 0;	//ジャンプ待機時間
	m_sAtackLag = 0;
	m_sAtackWait = 0;

	m_bWaraWara = gxFalse;
}


CEneH0004Tachikoma::~CEneH0004Tachikoma()
{

	//-----------------------------------------------------
	//デストラクタ
	//-----------------------------------------------------

}


void CEneH0004Tachikoma::SeqInit()
{
	//-----------------------------------------------------
	//初期化
	//-----------------------------------------------------

	if( !m_bWaraWara && IsScrollOut( enScrollOutWidth , enScrollOutHeight ) )
	{
		return;
	}

	m_Leynos.set_spr(EL_BASE ,NULL);

	m_Leynos.set_spr( EL_BODY ,&SprCEneH0004Tachikoma[1] );
	m_Leynos.set_spr( EL_ARM_L,NULL );
	m_Leynos.set_spr( EL_ARM_R,NULL);//&SprCEneH0004Tachikoma[0] );
	m_Leynos.set_spr( EL_LEG_L,&SprCEneH0004Tachikoma[3] );
	m_Leynos.set_spr( EL_LEG_R,&SprCEneH0004Tachikoma[2] );
	m_Leynos.set_pos( m_Pos.x , m_Pos.y , PRIO_ENEMY );

	m_Leynos.parts[EL_BASE]->ofy  = -12;
	m_Leynos.parts[EL_BODY]->ofy  = 0;
	m_Leynos.parts[EL_ARM_R]->ofx = 0;
	m_Leynos.parts[EL_ARM_R]->ofy = 0;
	m_Leynos.parts[EL_LEG_L]->ofx = 5;
	m_Leynos.parts[EL_LEG_L]->ofy = -5;
	m_Leynos.parts[EL_LEG_R]->ofx = -5;
	m_Leynos.parts[EL_LEG_R]->ofy = -5;
	m_Leynos.parts[EL_ARM_R]->pdist = 13;
	m_Leynos.parts[EL_ARM_R]->prot = -20;
	m_Leynos.set_spd(320);
	m_Leynos.walk_width_  = 170;
	m_Leynos.walk_height_ = 160;
	m_Leynos.set_walkcycle(20);
//	m_Leynos.cpu_control = gxFalse;
	m_Leynos.set_spdadd(8);
	m_Leynos.setFirePos(22,-14);	//武器発射位置設定
	m_Leynos.m_bNoShock = gxTrue;

	UpdateTargetPos();

	SetActionSeq( enActionSeqMain );

}


void CEneH0004Tachikoma::SeqMain()
{
	//-----------------------------------------------------
	//メイン
	//-----------------------------------------------------

	if( m_HitKurai.is_dead() )
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

	m_Leynos.action();

	m_Pos.x = m_Leynos.obj->pos.x;
	m_Pos.y = m_Leynos.obj->pos.y;

	if( m_HitKurai.is_damage() )
	{
		m_Leynos.set_damageflag( m_HitKurai.GetDamageDir()*2 );
	}

	//くらい判定登録
	m_HitKurai.SetHantei(ID_ENEMY_DEF , &m_Pos );

}


void CEneH0004Tachikoma::LogicAI()
{
	//-----------------------------------------------------
	//移動ロジック
	//-----------------------------------------------------

	//ロジック
	gxBool bAtack = gxFalse;

	if( m_Leynos.cpu_control )
	{
		Sint32 tx = m_Tgt.x;

		if(tx < m_Pos.x)
		{
			m_sControl = JOY_L;
			m_sLogicWait = 20;
		}
		else if(tx > m_Pos.x)
		{
			m_sControl = JOY_R;
			m_sLogicWait = 20;
		}

		if( GetTargetPlayer()->x > m_Pos.x && m_Leynos.get_dir()==DIR_RIGHT )
		{
			m_sControl |= BTN_L1;
		}
		if( GetTargetPlayer()->x < m_Pos.x && m_Leynos.get_dir()==DIR_LEFT )
		{
			m_sControl |= BTN_L1;
		}

	}

	UpdateTargetPos();

	if( gxAbs(m_Tgt.x-m_Pos.x)<=32*100 )
	{
		m_sControl = 0;
		if( GetTargetPlayer()->x > m_Pos.x && m_Leynos.get_dir()==DIR_LEFT )
		{
			m_sControl = JOY_L;
		}
		if( GetTargetPlayer()->x < m_Pos.x && m_Leynos.get_dir()==DIR_RIGHT )
		{
			m_sControl = JOY_R;
		}

	}

	if( m_sControl == 0 )
	{
		if(m_Leynos.chakuti() && gxAbs(m_Leynos.obj->mov.x)==0 )
		{
			if( m_sTimer%16==0)
			{
				bAtack = gxTrue;
//				viiEff::Set(EFF_BURNIA_SMOKE,m_Pos.x+m_Leynos.get_dir()*-1500,m_Pos.y,argset(0));
				new CEffDashStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , m_Pos.y, 0 , 0 );
				new CEffDashStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , m_Pos.y, 0 , 0 );
				new CEffDashStorm( m_Pos.x+(-15+viiSub::Rand()%30)*100 , m_Pos.y, 0 , 0 );
			}
		}
	}

	m_Leynos.set_control( m_sControl );

	if( m_sAtackWait > 0)
	{
		m_sAtackWait --;
	}

	if( m_sAtackLag > 0)
	{
		m_sAtackLag --;
	}

	if( bAtack )
	{
		if(GetTargetPlayer()->x > m_Pos.x && m_Leynos.get_dir()==DIR_RIGHT)
		{
			Atack();
		}
		else if(GetTargetPlayer()->x < m_Pos.x && m_Leynos.get_dir()==DIR_LEFT)
		{
			Atack();
		}
	}

}

void CEneH0004Tachikoma::UpdateTargetPos()
{
	Sint32 tx = GetTargetPlayer()->x;

	if(m_Pos.x > tx ) m_Tgt.x = tx+280*100;
	if(m_Pos.x < tx ) m_Tgt.x = tx-280*100;

}


void CEneH0004Tachikoma::Atack()
{
	//-----------------------------------------------------
	//攻撃可能なら攻撃する
	//-----------------------------------------------------
	Sint32 ax,ay;

	if( m_sAtackLag ) return;
	if( m_sAtackWait ) return;

	m_Leynos.getFirePos(ax,ay);

	viiMus::PlaySound(enSoundEneShotLaser);

	m_Leynos.arm_rot = 0;

	m_Leynos.setFirePos( 0 , -35 );
	m_Leynos.getFirePos( ax , ay );

	ax += m_Pos.x;
	ay += m_Pos.y;

//	CAtkH0001Ryudan *p;
	viiEff::Set(EFF_MAZZLE,ax,ay,NULL);

//	p = new CAtkH0001Ryudan( ax , ay , viiSub::SetKakudo(m_Leynos.get_dir(),-55), 480 );
//	p->SetForce(CAtkH0001Ryudan::enForceID_Enemy);

	CEne0001EneSuperBall *q;
	q = new CEne0001EneSuperBall( ax , ay , viiSub::SetKakudo(m_Leynos.get_dir(),-75), 480  );
	q->SetPattern( CEne0001EneSuperBall::enPatternDrop );
	q->SetScale( 0.5f );


	m_sJumpCnt ++;

	m_sAtackLag = 16;

	if( m_sJumpCnt >= 2 )
	{
		m_sJumpCnt = 0;
		m_sAtackWait = 80;
	}
}


void CEneH0004Tachikoma::SeqCrash()
{
	//-----------------------------------------------------
	//破壊された
	//-----------------------------------------------------
	if( !m_bDead )
	{
		m_bDead = gxTrue;

		m_Leynos.m_bDead = gxTrue;
		m_Leynos.action();
		viiEff::SetBombDeluxe(m_Pos.x,m_Pos.y-30*100,5,3);
		viiMus::PlaySound(enSoundExplosionMiddle);
		ScorePlus( enScore );

		if( !m_bWaraWara )
		{
			m_bClear = gxTrue;
		}
	}

	if(m_bClear)
	{
		SetActionSeq( enActionSeqEnd );
	}

}


void CEneH0004Tachikoma::Draw()
{
	m_Leynos.Draw();
//	viiDraw::Sprdmg( m_HitKurai.is_damage() , &SprCEneH0004Tachikoma[ 0 ] , m_Pos.x , m_Pos.y,PRIO_ENEMY , ATR_DEFAULT , ARGB_DFLT );

}

