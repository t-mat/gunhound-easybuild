//--------------------------------------------------------------------------------
//
//弾幕用の弾クラス
//
//--------------------------------------------------------------------------------

#include "HoundsEnemy.h"
//#include "EnemyTbl/CEneH0205JourikusenTBL.h"

enum {
	enScrollOutWidth  = WINDOW_W*100,
	enScrollOutHeight = WINDOW_H*100,

	enKuraiLeft  = -30,
	enKuraiTop   = -34,
	enKuraiRight =  30,
	enKuraiBottom=  0,

	enScore = 300,
};


gxSprite SprDanmaku[] = {
	//弾幕用弾丸
	{ enTexPageEffectCommon02,32+16,136-8  ,16,16,8,8 },	//可変
	{ enTexPageEffectCommon02,32,136-8,14,8,7,5 },			//直進
	{ enTexPageEffectCommon02,32,136  ,10,10,5,5 },			//通常
};

enum {
	enBulletCurve,
	enBulletGensokuStraight,
	enBulletKasoku,
	enBulletGattling,
	enBulletNormal,
	enBulletFall,
	enBulletDropCandy,
};

//-------------------------------------------------------
//弾幕根元
//-------------------------------------------------------
CDanmakuRoot::CDanmakuRoot( Sint32 sType , Sint32 x , Sint32 y , Sint32 sRot , Sint32 sSpd )
{
	m_Pos.x = x;
	m_Pos.y = y;

	m_sPattern = sType;
	m_fRot = (Float32)sRot;
	m_sSpd = sSpd;
	m_fRotSpd = ZERO;
}

void CDanmakuRoot::SeqMain()
{

	switch( m_sPattern ){
	case enDanmakuCandyShot:
		MakeCandyShot();
		break;
	case enDanmakuDropShot:
		//ドロップショット
		MakeDropShot();
		break;

	case enDanmakuAirCannon:
		//対空ドロップレーザー
		MakeAirCannon();
		break;

	case enDanmakuSpiralGat:
		//ガトリング（ねじり）速い
		MakeGattlingSpiral();
		break;

	case enDanmakuGattling:
		//ガトリング（ストレート）
		MakeGattlingStraight();
		break;

	case enDanmakuWaveShot:
		//ウェーブ
		MakeWaveShot();
		break;

	case enDanmakuTyphoonShot:
		//台風
		MakeTyphoonShot();
		break;

	case enDanmakuDonutsShot:
		//イオンリング
		MakeDonutsShot();
		break;

	case enDanmakuPileBankerShot:
		//ランサーショット
		MakePileBankerShot();
		break;
	}

}


void CDanmakuRoot::MakeCandyShot()
{
	CDanmakuBullet *pBlt;

	for(Sint32 ii=0;ii<4;ii++)
	{
		Sint32 x,y;
		x = viiMath::Cos100((Sint32)m_fRot);
		y = viiMath::Sin100((Sint32)m_fRot);

		pBlt = new CDanmakuBullet( enBulletDropCandy , m_Pos.x, m_Pos.y  );
		pBlt->m_Add.x = 0;
		pBlt->m_Add.y = 0;
		pBlt->m_fRot  = m_fRot+(-4+viiSub::Rand()%8)*4;
		pBlt->m_sAddSpd = 24;
		pBlt->m_sMaxSpd = m_sSpd+(-80+viiSub::Rand()%80);
		pBlt->m_sWait = viiSub::Rand()%8;//ii*2;
		pBlt->m_sSprite = 0;
		pBlt->m_sBulKaku = (Sint32)m_fRot;
		pBlt->m_fRotSpd = m_fRotSpd+1.5f+(-50+viiSub::Rand()%50)/100.f;
		pBlt->m_fWeightAdd = 10.5f;
		pBlt->m_sMaxSpd = m_sSpd+(-80+viiSub::Rand()%80);
		pBlt->m_bScrollOut = gxFalse;
		pBlt->SetBulletIndex( m_sBulletIndex );
	}


	SetActionSeq( enActionSeqEnd );
	return;
}


void CDanmakuRoot::MakeDropShot()
{
	//---------------------------------------
	//ドロップショット
	//---------------------------------------
	CDanmakuBullet *pBlt;

	for(Sint32 ii=0;ii<4;ii++)
	{
		Sint32 x,y;
		x = viiMath::Cos100((Sint32)m_fRot);
		y = viiMath::Sin100((Sint32)m_fRot);

		pBlt = new CDanmakuBullet( enBulletFall , m_Pos.x, m_Pos.y  );
		pBlt->m_Add.x = 0;
		pBlt->m_Add.y = 0;
		pBlt->m_fRot  = m_fRot-4+viiSub::Rand()%8;
		pBlt->m_sAddSpd = 24;
		pBlt->m_sMaxSpd = m_sSpd+(-80+viiSub::Rand()%80);
		pBlt->m_sWait = viiSub::Rand()%8;//ii*2;
		pBlt->m_sSprite = 0;
		pBlt->m_sBulKaku = (Sint32)m_fRot;
		pBlt->m_fRotSpd = m_fRotSpd+1.5f+(-50+viiSub::Rand()%50)/100.f;
		pBlt->SetBulletIndex( m_sBulletIndex );
	}

	SetActionSeq( enActionSeqEnd );
}


void CDanmakuRoot::MakeAirCannon()
{
	//---------------------------------------
	//高射砲ショット
	//---------------------------------------
	CDanmakuBullet *pBlt;

	for(Sint32 ii=0;ii<12;ii++)
	{
		Sint32 x,y;
		x = viiMath::Cos100((Sint32)m_fRot);
		y = viiMath::Sin100((Sint32)m_fRot);

		pBlt = new CDanmakuBullet( enBulletFall , m_Pos.x, m_Pos.y  );
		pBlt->m_Add.x = 0;
		pBlt->m_Add.y = 0;
		pBlt->m_fRot  = m_fRot;
		pBlt->m_sAddSpd = 24;
		pBlt->m_sMaxSpd = 480;
		pBlt->m_sWait = ii*2;
		pBlt->m_sSprite = 1;
		pBlt->m_sBulKaku = (Sint32)m_fRot;
		pBlt->m_fRotSpd = 1;
		pBlt->SetBulletIndex( m_sBulletIndex );
	}

	SetActionSeq( enActionSeqEnd );
}

void CDanmakuRoot::MakeGattlingSpiral()
{
	//---------------------------------------
	//ガトリング砲(スパイラル)
	//---------------------------------------

	CDanmakuBullet *pBlt;

	m_fRot -= 270.f;

	for(Sint32 ii=0;ii<8;ii++)
	{
		Sint32 x,y;
		x = viiMath::Cos100((Sint32)m_fRot);
		y = viiMath::Sin100((Sint32)m_fRot);

		pBlt = new CDanmakuBullet( enBulletKasoku , m_Pos.x, m_Pos.y  );
		pBlt->m_Add.x = x;
		pBlt->m_Add.y = y;
		pBlt->m_fRot  = m_fRot+ii;
		pBlt->m_sAddSpd = 32;
		pBlt->m_sMaxSpd = 880;
		pBlt->m_sWait = ii*2;
		pBlt->m_sSprite = 1;
		pBlt->m_sBulKaku = (Sint32)m_fRot;
		pBlt->SetBulletIndex( m_sBulletIndex );
	}

	SetActionSeq( enActionSeqEnd );
}


void CDanmakuRoot::MakeGattlingStraight()
{
	//---------------------------------------
	//ガトリング砲
	//---------------------------------------

	CDanmakuBullet *pBlt;

//	m_fRot -= 270.f;

	for(Sint32 ii=0;ii<4;ii++)
	{
		Sint32 x,y;
		x = viiMath::Cos100((Sint32)m_fRot);
		y = viiMath::Sin100((Sint32)m_fRot);

		pBlt = new CDanmakuBullet( enBulletGattling , m_Pos.x +x, m_Pos.y+y  );
		pBlt->m_Add.x = x;
		pBlt->m_Add.y = y;
		pBlt->m_fRot  = m_fRot+ii;
		pBlt->m_sAddSpd = 24;
		pBlt->m_sMaxSpd = 360*2;
		pBlt->m_sWait = ii*6;
		pBlt->m_sSprite = 1;
		pBlt->m_sBulKaku = (Sint32)m_fRot;
		pBlt->SetBulletIndex( m_sBulletIndex );
	}

	SetActionSeq( enActionSeqEnd );
}


void CDanmakuRoot::MakePileBankerShot()
{
	//------------------------------------
	//パイルバンカー
	//------------------------------------

	CDanmakuBullet *pBlt;

	m_fRot -= 270.f;

	Sint32 xx,yy;
	xx = viiMath::Cos100((Sint32)m_fRot);
	yy = viiMath::Sin100((Sint32)m_fRot);
	Sint32 dstAdd = 0;

	for(Sint32 ii=0;ii<360*2-90;ii+=12)
	{
		Sint32 x,y,dst,r;
		//楕円を生成
		x = (viiMath::Cos100(ii)*32)/100;
		y = (viiMath::Sin100(ii)*32 )/100;
		dst = 0;//viiMath::Dist(0,0,x,y);
		r   = viiMath::Atan2_100(y,x)/100;
		dstAdd += 1;
		//楕円の角度を変更
		x = viiMath::Cos100(r+(Sint32)m_fRot)*(dst+dstAdd/3);
		y = viiMath::Sin100(r+(Sint32)m_fRot)*(dst+dstAdd/3);

		pBlt = new CDanmakuBullet( enBulletKasoku , m_Pos.x +x, m_Pos.y+y  );
		pBlt->m_Add.x = 0;//;xx*8;
		pBlt->m_Add.y = 0;//;yy*8;
		pBlt->m_fRot  = m_fRot-90;
		pBlt->m_sAddSpd = 32;
		pBlt->m_sMaxSpd = 300+viiMath::Sin100(ii);//480+ii*10;
		pBlt->SetBulletIndex( m_sBulletIndex );
	}

	SetActionSeq( enActionSeqEnd );

	return;


}

void CDanmakuRoot::MakeDonutsShot()
{
	//------------------------------------
	//ドーナツ砲
	//------------------------------------

	CDanmakuBullet *pBlt;

	m_fRot -= 270.f;

	Sint32 xx,yy;
	xx = viiMath::Cos100((Sint32)m_fRot);
	yy = viiMath::Sin100((Sint32)m_fRot);

	for(Sint32 ii=0;ii<360;ii+=16)
	{
		Sint32 x,y,dst,r;
		//楕円を生成
		x = (viiMath::Cos100(ii)*30)/100;
		y = (viiMath::Sin100(ii)*10 )/100;
		dst = viiMath::Dist(0,0,x,y);
		r   = viiMath::Atan2_100(y,x)/100;

		//楕円の角度を変更
		x = viiMath::Cos100(r+(Sint32)m_fRot)*dst;
		y = viiMath::Sin100(r+(Sint32)m_fRot)*dst;

		pBlt = new CDanmakuBullet( enBulletKasoku , m_Pos.x +x, m_Pos.y+y  );
		pBlt->m_Add.x = 0;
		pBlt->m_Add.y = 0;
		pBlt->m_fRot  = m_fRot-90;
		pBlt->m_sAddSpd = 32;//8
		pBlt->m_sMaxSpd = 480;
		pBlt->SetBulletIndex( m_sBulletIndex );
	}

	SetActionSeq( enActionSeqEnd );

	return;
}


void CDanmakuRoot::MakeTyphoonShot()
{
	//------------------------------------
	//タイフーンショット
	//------------------------------------

	CDanmakuBullet *pBlt;
	Sint32 sWait = 0;
	for(Sint32 ii=-135;ii<90;ii+=12)
	{
		m_Pos.x -= 100;
		pBlt = new CDanmakuBullet( enBulletCurve , m_Pos.x , m_Pos.y  );
		pBlt->m_Add.x = (viiMath::Cos100((Sint32)m_fRot+ii));
		pBlt->m_Add.y = (viiMath::Sin100((Sint32)m_fRot+ii));
		pBlt->m_fRot    = m_fRot+ii;
		pBlt->m_sMaxSpd = 60;
		pBlt->m_sSpd    = 0;
		pBlt->m_sWait = sWait*1;
		pBlt->SetBulletIndex( m_sBulletIndex );
		sWait ++;
	}

	SetActionSeq( enActionSeqEnd );
}


void CDanmakuRoot::MakeWaveShot()
{
	//------------------------------------
	//ウェーブショット
	//------------------------------------
	CDanmakuBullet *pBlt;

	m_fRot -= 270.f;

	Sint32 xx,yy;
	xx = viiMath::Cos100((Sint32)m_fRot);
	yy = viiMath::Sin100((Sint32)m_fRot);

	for(Sint32 ii=0;ii<360;ii+=16)
	{
		Sint32 x,y,dst,r;
		//楕円を生成
//		x = (viiMath::Cos100(ii)*30)/100;
//		y = (viiMath::Sin100(ii)*10 )/100;
		x = (viiMath::Cos100(ii)*10)/100;
		y = (viiMath::Sin100(ii)*3 )/100;
		dst = viiMath::Dist(0,0,x,y);
		r   = viiMath::Atan2_100(y,x)/100;

		//楕円の角度を変更
		x = viiMath::Cos100(r+(Sint32)m_fRot)*dst;
		y = viiMath::Sin100(r+(Sint32)m_fRot)*dst;

		pBlt = new CDanmakuBullet( enBulletKasoku , m_Pos.x +x, m_Pos.y+y  );
		pBlt->m_Add.x = 0;
		pBlt->m_Add.y = 0;
		pBlt->m_fRot  = m_fRot-90;
		pBlt->m_sAddSpd = 32;//8
		pBlt->m_sMaxSpd = 100+m_sSpd+gxAbs(viiMath::Sin100(ii));//480+ii*10;
		pBlt->SetBulletIndex( m_sBulletIndex );
	}

	SetActionSeq( enActionSeqEnd );
}

void CDanmakuRoot::Draw()
{
	
	
}

//-------------------------------------------------------


CDanmakuBullet::CDanmakuBullet( Sint32 sType , Sint32 x , Sint32 y )
{
	m_HitAtari.set_hp( 1 );
	m_HitAtari.set_ap( 1 );

	m_Pos.x = x;
	m_Pos.y = y;

	m_sPattern = sType;

	m_Add.x = 0;
	m_Add.y = 0;
	m_Kasoku.x = 0;
	m_Kasoku.y = 0;

	m_sWait   = 0;
	m_fRot    = 0;
	m_sSpd    = 0;
	m_sMaxSpd = 0;
	m_sAddSpd = 8;
	m_fRotSpd = 1;

	m_fWeight = ZERO;
	m_fWeightAdd = ZERO;

	m_bScrollOut = gxTrue;
	m_HitAtari.ax1 = -2;
	m_HitAtari.ay1 = -2;
	m_HitAtari.ax2 = 2;
	m_HitAtari.ay2 = 2;

	m_sSprite  = 2;
	m_sBulKaku = 0;//viiSub::Rand()%360;
}

CDanmakuBullet::~CDanmakuBullet()
{
	
	
}


void CDanmakuBullet::SeqMain()
{
	if( m_sWait > 0 )
	{
		m_sWait --;
		return;
	}

	if( IsClearBullet() )
	{
		viiEff::Set(EFF_HITFLASH_ENE , m_Pos.x , m_Pos.y , 0 );
		SetActionSeq( enActionSeqEnd );
		return;
	}

	if( m_HitAtari.is_dead() || m_HitAtari.IsImpacted() )
	{
		viiEff::Set( EFF_HITFLASH , m_Pos.x , m_Pos.y , argset(0) );
//		viiEff::SetBombSimple( m_Pos.x , m_Pos.y , 1 , 1+viiSub::Rand()%2 );
		SetActionSeq( enActionSeqEnd );
		return;
	}

	if( m_bScrollOut && viiSub::IsScrollOut( m_Pos.x , m_Pos.y , enScrollOutWidth , enScrollOutHeight ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

	if( viiSub::GetBoundBlock_Enemy( m_Pos.x , m_Pos.y ) )
	{
		viiEff::Set(EFF_HITFLASH_ENE , m_Pos.x , m_Pos.y , 0 );
		SetActionSeq( enActionSeqEnd );
		return;
	}

	m_sBulKaku +=8;
	m_sBulKaku = m_sBulKaku%360;

	switch( m_sPattern ){
	case enBulletDropCandy:
		PatternCandy();
		break;
	case enBulletFall:
//		m_sBulKaku = (Sint32)m_fRot;
		PatternFall();
		break;

	case enBulletGensokuStraight:
		PatternWaveShot();
		break;

	case enBulletCurve:
		PatternTyphoon();
		break;

	case enBulletKasoku:
		PatternKasoku();
		break;

	case enBulletNormal:
		PatternNormal();
		break;

	case enBulletGattling:
		m_sBulKaku = (Sint32)m_fRot;
		PatternKasoku();
		break;
	}

	if( m_sSpd < m_sMaxSpd )
	{
		m_sSpd += m_sAddSpd;
		if( m_sSpd >= m_sMaxSpd )
		{
			m_sSpd = m_sMaxSpd;
		}
	}

	m_HitAtari.SetHantei(ID_ENEMY_ATK , &m_Pos );

}


void CDanmakuBullet::PatternTyphoon()
{
	//回転軌道

	m_fRot +=1;
	m_Add.x = viiMath::Cos100((Sint32)m_fRot)*10;
	m_Add.y = viiMath::Sin100((Sint32)m_fRot)*3;

	m_Pos.x += m_Add.x+150;
	m_Pos.y += m_Add.y;

}


void CDanmakuBullet::PatternKasoku()
{
	//単純加速

	m_Add.x = (viiMath::Cos100((Sint32)m_fRot)*m_sSpd)/100;
	m_Add.y = (viiMath::Sin100((Sint32)m_fRot)*m_sSpd)/100;

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;
}


void CDanmakuBullet::PatternNormal()
{
	//単純加速

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;
}


void CDanmakuBullet::PatternFall()
{
	//落下軌道

	NORMALIZE( m_fRot );

/*
	while(m_fRot > 360.f)
	{
		m_fRot -= 360.f;
	}
	while(m_fRot < 0.f)
	{
		m_fRot += 360.f;
	}
*/
	if(m_fRot < 270 && m_fRot > 90)
	{
		m_fRot -= m_fRotSpd;
	} 
	else if(m_fRot > 270 && m_fRot < 360+90)
	{
		m_fRot += m_fRotSpd;
	} 
	else if(m_fRot < 90 && m_fRot > -90)
	{
		m_fRot += m_fRotSpd;
	} 

	m_Add.x = (viiMath::Cos100((Sint32)m_fRot)*m_sSpd)/100;
	m_Add.y = (viiMath::Sin100((Sint32)m_fRot)*m_sSpd)/100;

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;
}


void CDanmakuBullet::PatternWaveShot()
{
	Sint32 sSpdRev = 1;

	if( m_sSpd > m_sMaxSpd )
	{
		m_sSpd -= 16;
	}

	if(m_sSpd < 0 )
	{
		m_sSpd -= 12;
		m_Add.x = viiMath::Cos100((Sint32)m_fRot);
		m_Add.y = viiMath::Sin100((Sint32)m_fRot);
		sSpdRev = -1;
	}

	m_Pos.x += (m_Add.x*(m_sSpd*sSpdRev))/100;
	m_Pos.y += (m_Add.y*(m_sSpd*sSpdRev))/100;

}

void CDanmakuBullet::PatternCandy()
{
	//単純加速

	m_Add.x = (viiMath::Cos100((Sint32)m_fRot)*m_sSpd)/100;
	m_Add.y = (viiMath::Sin100((Sint32)m_fRot)*m_sSpd)/100;
	m_Add.y += m_fWeight;
	if(m_fWeight < 800.f) m_fWeight += m_fWeightAdd;

	m_Pos.x += m_Add.x;
	m_Pos.y += m_Add.y;


	if( viiSub::IsScrollOut( m_Pos.x , m_Pos.y , 32*100 , 128*100 ) )
	{
		SetActionSeq( enActionSeqEnd );
		return;
	}

}



void CDanmakuBullet::Draw()
{

	viiDraw::Sprite( &SprDanmaku[m_sSprite] , m_Pos.x , m_Pos.y , PRIO_ENE_BULLET ,ATR_DFLT , ARGB_DFLT , 1.f,m_sBulKaku);

}

