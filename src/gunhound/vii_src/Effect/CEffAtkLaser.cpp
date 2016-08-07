//------------------------------------------------------------
//
//エフェクト：レーザー
//
//------------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

gxSprite SprLaser[] = {
	{TPG_CLOUD,0,236,256,20,16,10},
	{TPG_CLOUD,0,236-32,256,20,16,10},
};

enum {
	enLaseSpeed = 32,
	enLaseLengthMax = 640,
};

Float32 LaserWidthTbl[]=
{
	//レーザーの太さ管理
	0.1f,
	1.0f,
	2.5f,
};


CEffAtkLaser::CEffAtkLaser(Sint32 x , Sint32 y, Sint32 lv )
{
	//----------------------------------------------
	//コンストラクタ
	//----------------------------------------------

	m_bPowerIn = gxFalse;
	m_Pos.x = x;
	m_Pos.y = y;

	m_sRotation = 0;
	m_sLevel    = lv;
	m_sLength   = 0;

	m_sSpeed = enLaseSpeed;
	m_pSprite = NULL;
	m_bDisp = gxFalse;
	m_sLaserX = 0;
	m_sLaserY = 0;
	m_fLength = 0;

	m_sAtackType = ID_VERSIS_ATK;

	memset( &stLayer[0] ,0x00, sizeof(StLaserLayer) );
	memset( &stLayer[1] ,0x00, sizeof(StLaserLayer) );
	memset( &stLayer[2] ,0x00, sizeof(StLaserLayer) );

}


CEffAtkLaser::~CEffAtkLaser()
{
	//----------------------------------------------
	//デストラクタ
	//----------------------------------------------


}


void CEffAtkLaser::SeqInit()
{


	SetActionSeq( enActionSeqMain );

}


void CEffAtkLaser::SeqMain()
{
	//----------------------------------------------
	//メイン
	//----------------------------------------------
	Sint32 x,y;
	Sint32 sLength = 0;

	m_pSprite = &SprLaser[0];

	m_bDisp = gxTrue;

	m_sAdd_X = viiMath::Cos100( m_sRotation )*16;
	m_sAdd_Y = viiMath::Sin100( m_sRotation )*16;

	if( m_bPowerIn )
	{
		//エネルギーの注入があった

		m_sLength += m_sSpeed;
		if( m_sLength >= enLaseLengthMax ) m_sLength = enLaseLengthMax;

		m_bPowerIn = gxFalse;
	}
	else
	{
		//エネルギーの注入がなかった

		m_sLength -= m_sSpeed*2;
		if( m_sLength <= 0 ) m_sLength = 0;
	}

	Sint32 sLaserDist = m_sLength/32;

	for( Sint32 i=0;i<sLaserDist;i++ )
	{
		gxBool bHit;

		x = m_Pos.x + m_sAdd_X*i;
		y = m_Pos.y + m_sAdd_Y*i;
		bHit = viiSub::GetBoundBlock_Player( x,y )? gxTrue : gxFalse;

		if( bHit )
		{
			//-------------------------------------------------
			//壁に当たればそこで終了
			//-------------------------------------------------
			viiEff::Set(EFF_LASERBOMB_PARTS,x,y,argset(1,0) );
			break;
		}
		sLength ++;

		OBJ_POS_T stPos;
		stPos.x = x;
		stPos.y = y;
		m_HitAtari[i].ax1 = -4+(-m_sLevel*2);
		m_HitAtari[i].ay1 = -4+(-m_sLevel*2);
		m_HitAtari[i].ax2 =  4+(+m_sLevel*2);
		m_HitAtari[i].ay2 =  4+(+m_sLevel*2);
		m_HitAtari[i].SetHantei( m_sAtackType , &stPos );
		m_HitAtari[i].SetDirectAromorPosition( 50+m_sLevel*20 );//150+m_sLevel*20 );

	}

	//-------------------------------------------------
	//レーザーの長さ調節
	//-------------------------------------------------
	Float32 fLaserLen = m_sLength*1.0f;
	if( fLaserLen >= 24.0f+sLength*16.0f) fLaserLen = 24.0f+sLength*16.0f;

	//-------------------------------------------------
	//レーザーの長さがゼロ以下なら表示しない
	//-------------------------------------------------

	if( fLaserLen <= 0.f || sLength <= 0)
	{
		m_bDisp = gxFalse;
		return;
	}

	m_fLength = (fLaserLen-16.f)/256.0f;

	viiDbg::printf (100,200,"LASER %f.3",m_fLength);

	SetLaserEffect();
}


void CEffAtkLaser::SetLaserEffect()
{
	//-------------------------------------------------
	//レーザーのエフェクト調節
	//-------------------------------------------------
	Sint32 sx,sy;
	Float32 fy;

	Float32 fmax = LaserWidthTbl[m_sLevel];
	viiSub::GetScroll_s( sx,sy );

	fy = m_sLength/300.0f;

	if( fy >= fmax ) fy = fmax;

	stLayer[0].x  = m_Pos.x/100 - sx;
	stLayer[0].y  = m_Pos.y/100 - sy;
	stLayer[0].fy = fy;

	stLayer[1].x  = m_Pos.x/100 - sx;
	stLayer[1].y  = m_Pos.y/100 - sy;
	stLayer[1].fy = 1.0f+(viiSub::Rand()%20)/100.0f;

	stLayer[2].x  = m_Pos.x/100 - sx;
	stLayer[2].y  = m_Pos.y/100 - sy;
	stLayer[2].fy = fy*LaserWidthTbl[m_sLevel]/2;

}


Sint32 CEffAtkLaser::GetLaserColor()
{
	//----------------------------------------------
	//レーザーの色を変更
	//----------------------------------------------

	Sint32 sARGB = ARGB_DFLT;

	if(m_sLevel == 0)
	{
		//色がえ
		Sint32 r = (viiSub::Rand()%2)*240;
		Sint32 g = (viiSub::Rand()%2)*240;
		Sint32 b = (viiSub::Rand()%2)*240;
		if(r+g+b==0)
		{
			r = g = b = 128;
		}
		sARGB = ARGB(0x80,(r),(g),(b));
	}

	return sARGB;
}


void CEffAtkLaser::SeqCrash()
{
	//----------------------------------------------
	//終了
	//----------------------------------------------

//	viiEff::Set( EFF_HITFLASH , m_Pos.x , m_Pos.y , argset(1) );

	SetActionSeq( enActionSeqEnd );

}


void CEffAtkLaser::SeqEnd()
{
	
	
}


void CEffAtkLaser::Draw()
{
	//------------------------------------------------
	//描画
	//------------------------------------------------

	if( !m_bDisp ) return;

	PutSpriteDX(
		stLayer[0].x,stLayer[0].y,PRIO_ATACK,
		m_pSprite->page,
		m_pSprite->u,
		m_pSprite->v,
		m_pSprite->w,
		m_pSprite->h,
		m_pSprite->cx,
		m_pSprite->cy,
		ATR_ALPHA_PLUS,0x40ffffff,m_sRotation*1.0f,
		m_fLength,stLayer[0].fy);

	PutSpriteDX(
		stLayer[1].x,stLayer[1].y,PRIO_ATACK,
		m_pSprite->page,
		m_pSprite->u,
		m_pSprite->v,
		m_pSprite->w,
		m_pSprite->h,
		m_pSprite->cx,
		m_pSprite->cy,
		ATR_ALPHA_PLUS,0x40ffffff,m_sRotation*1.0f,
		m_fLength,stLayer[1].fy );

	PutSpriteDX(
		stLayer[2].x,stLayer[2].y,PRIO_ATACK,
		m_pSprite->page,
		m_pSprite->u,
		m_pSprite->v,
		m_pSprite->w,
		m_pSprite->h,
		m_pSprite->cx,
		m_pSprite->cy,
		ATR_ALPHA_PLUS,0x40ffffff,m_sRotation*1.0f,
		m_fLength,stLayer[2].fy );

}




