//-----------------------------------------------------
//振り子（っぽい）シミュレーション
//-----------------------------------------------------
#include <gunvalken.h>
#include "CPenduram.h"

CPenduram::CPenduram()
{
	//振り子っぽい動きを作る
	m_fRotation    = 0;
	m_fRotationAdd = ZERO;
	m_fMasatsu     = 1.0f;
	m_fWeight      = enPenduramWeight*1.f;
	m_fScale       = 1.f;
	SetWeight(50);
}


CPenduram::~CPenduram()
{
	//デストラクタ

	//特に解放するものはない

}


void CPenduram::SetWeight(Sint32 per)
{
	//0～100%の間で重さの設定を行う
	if( per < 0   ) per = 0;
	if( per > 100 ) per = 100;

	m_fWeight = enPenduramWeight+10000.f*per/100;
	m_bInfinity = gxFalse;
}


void CPenduram::Set(Sint32 n)
{
	//-90 ～ +90の間で振り子の初期角度を設定する

	m_fRotation    = (Float32)-n;
	m_fRotationAdd = ZERO;
	m_fScale       = 1.0f;
	m_fMasatsu     = 1.f;
	m_fWeight      = enPenduramWeight*1.f;
}

void CPenduram::Add(Sint32 n)
{
	//-90 ～ +90の間で振り子の初期角度を設定する
	Float32 fTgt = (Float32)-n;
	m_fRotation    += (fTgt-m_fRotation)/10.f;
	m_fRotationAdd = ZERO;
	m_fScale       = 1.0f;
	m_fMasatsu     = 1.f;
	m_fWeight      = enPenduramWeight*1.f;
}


Float32 CPenduram::GetRotation()
{
	//角度を得る
	
	return m_fRotation;
}


void CPenduram::Action()
{
	//毎フレームのアクション
	Float32 fRot;

	fRot = -DEG2RAD(m_fRotation);

	m_fRotationAdd += fRot*m_fScale;
	m_fRotation    += m_fRotationAdd;

	if( !m_bInfinity )
	{
		m_fMasatsu += (0.f - m_fMasatsu) / m_fWeight;
	}

	m_fRotation *= m_fMasatsu;

}


void CPenduram::Draw()
{
	//デバッグ用描画
	Sint32 x = viiMath::Cos100( (Sint32)(m_fRotation)+90 )*80;
	Sint32 y = viiMath::Sin100( (Sint32)(m_fRotation)+90 )*80;

	viiDbg::printf( 32,128    , "KAKU = %f" , DEG2RAD(m_fRotation) );
	viiDbg::printf( 32,128+16 , "rot = %f" , m_fRotation );
	viiDbg::printf( 32,128+32 , "mas = %f" , m_fMasatsu );

	GGX_DrawLine( 160,120,160+x,120+y,32 );

}

