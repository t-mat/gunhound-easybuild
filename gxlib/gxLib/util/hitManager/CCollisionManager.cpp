//--------------------------------------------------
//
// CCollisionManager.cpp
// 当たり判定管理
//
//--------------------------------------------------

#include <gxLib.h>
#include "CCollisionManager.h"

//CCollisionManager *g_pCollisionManager = NULL;
SINGLETON_DECLARE_INSTANCE(CCollisionManager);

CCollisionManager::CCollisionManager()
{
	if (s_pInstance) delete s_pInstance;

	s_pInstance = this;

	//全判定を初期化

	for (int i = 0; i<MaxCollisionNum; i++)
	{
		m_pCollision[i] = NULL;
		m_pStCollisionBackup[i] = NULL;
	}
	//s_pInstance = this;
	m_sMaxCollision = 0;
	m_sCollisionNum = 0;

}

Sint32 CCollisionManager::SetRegist(CCollision* p)
{
	//当たり判定を登録する
	Sint32 i;

	for( i=0;i<MaxCollisionNum;i++)
	{
		if( m_pCollision[i] == NULL)
		{
			m_pCollision[i] = p;

			m_sCollisionNum ++;
			return i;
		}
	}

//	ASSERT( i<MaxCollisionNum );
#ifdef GX_DEBUG
	gxLib::DebugLog( "あたり判定に充分な空きがない" );
#endif
	return 0;
}

CCollisionManager::~CCollisionManager()
{
	//全判定を解放
	for (int i = 0; i<MaxCollisionNum; i++)
	{
		if (m_pCollision[i]) delete m_pCollision[i];
		m_pCollision[i] = NULL;
		//if( m_pStCollisionBackup[i] ) delete m_pStCollisionBackup[i];
	}

	s_pInstance = NULL;
};

void CCollisionManager::UnRegist(Sint32 id)
{
	//当たり判定の登録を解除する
	
	if( m_pCollision[id] )
	{
		m_sCollisionNum --;
		m_pCollision[id] = NULL;
	}
}

void CCollisionManager::Action()
{
	int i;

	EnCollisionID id1,id2;

	for(int n=0;n<2;n++)
	{
		switch(n){
		case 0:
			id1 = enCollisionID_A_Atack;
			id2 = enCollisionID_B_Defence;
			break;
		case 1:
			id1 = enCollisionID_B_Atack;
			id2 = enCollisionID_A_Defence;
			break;
		}

		for(i=0;i<MaxCollisionNum;i++)
		{

			if( m_pCollision[i] == NULL ) continue;

			if( m_pCollision[i]->m_sTime > 0) m_pCollision[i]->m_sTime --;

			if( m_pCollision[i]->stHitRect.sType != id1 ) continue;
			if( m_pCollision[i]->m_sFlag == gxFalse) continue;

			m_pCollision[i]->m_bHit = gxFalse;


			for(int j=0;j<MaxCollisionNum;j++)
			{
				if( m_pCollision[j] == NULL ) continue;
				if( m_pCollision[j]->stHitRect.sType != id2 ) continue;
				if( m_pCollision[j]->m_sFlag == gxFalse) continue;
				if( ( m_pCollision[j]->m_sHitType&m_pCollision[i]->m_sHitType) == 0x00 ) continue;
				if( (m_pCollision[j]->m_sAtr&m_pCollision[i]->m_sAtr) == 0 )             continue;	//属性の違うものには判定を行わない

				if( judgeRect(i,j) )
				{
					m_pCollision[i]->m_bHit = gxTrue;
					m_pCollision[i]->m_sTime = 5;

					m_pCollision[j]->m_bHit = gxTrue;
					m_pCollision[j]->m_sTime = 5;
					m_pCollision[j]->m_sHp -= m_pCollision[i]->m_sAp;
					if( m_pCollision[j]->m_sHp <= 0 )
					{
						m_pCollision[j]->m_sHp = 0;
					}

//ダメージ計算
//					int atk = m_pCollision[i]->m_sAtk; //(0～100)
//					int def = m_pCollision[i]->m_sDef; //(0～100)
//					Float32 dmg = m_pCollision[i]->dmg*(def - atk)/100.0f;
//					m_pCollision[j]->m_sHp -= (int)dmg;
//					if( m_pCollision[j]->m_sHp < 0 ) m_pCollision[j]->m_sHp = 0;

				}

			}
		}
	}

	//当たり判定のバックアップを取る
	m_sMaxCollision = 0;

	for(i=0;i<MaxCollisionNum;i++)
	{
		if( m_pCollision[i] == NULL ) continue;
		if( m_pCollision[i]->m_sFlag == gxFalse) continue;
		m_pStCollisionBackup[ m_sMaxCollision ] = m_pCollision[i];
		m_pCollision[i]->m_sFlag = 0;
		m_sMaxCollision ++;
	}

}


bool CCollisionManager::judgeRect(int a1,int b1)
{
	//-----------------------------------------------
	//矩形での当たり判定を取る
	//-----------------------------------------------

	int x1[2],y1[2],x2[2],y2[2];

	x1[0] = m_pCollision[a1]->stHitRect.sX;
	y1[0] = m_pCollision[a1]->stHitRect.sY;
	x2[0] = x1[0] + m_pCollision[a1]->stHitRect.sW;
	y2[0] = y1[0] + m_pCollision[a1]->stHitRect.sH;

	x1[1] = m_pCollision[b1]->stHitRect.sX;
	y1[1] = m_pCollision[b1]->stHitRect.sY;
	x2[1] = x1[1] + m_pCollision[b1]->stHitRect.sW;
	y2[1] = y1[1] + m_pCollision[b1]->stHitRect.sH;

	if(x2[1] >= x1[0] && x1[1] <= x2[0])
	{
		if(y2[1] >= y1[0] && y1[1] <= y2[0])
		{
			return true;
		}
	}

	return false;
}


void CCollisionManager::Draw()
{
	//デバッグ表示
	int i;

	for(i=0;i<MaxCollisionNum;i++)
	{
		if( m_pCollision[i])
		{
			if( m_pCollision[i]->m_sFlag == gxFalse) continue;
			int x,y,w,h;
			x = m_pCollision[i]->stHitRect.sX;//+WINDOW_W/2;
			y = m_pCollision[i]->stHitRect.sY;//+WINDOW_H/2;
			w = m_pCollision[i]->stHitRect.sW;
			h = m_pCollision[i]->stHitRect.sH;
			//
			gxLib::DrawBox(x,y,x+w,y+h,255,gxTrue,ATR_DFLT ,0x80ff0000);

		}
	}
}

//------------------------------------------------------------------------

CCollision::CCollision()
{

	m_sIndex   = CCollisionManager::GetInstance()->SetRegist(this);
	m_sFlag    = m_sTime = 0;
	m_bHit     = gxFalse;
	m_sHp      = 1;
	m_sAp      = 1;
	m_sHitType = enCollisionTypeA;
	m_sAtr     = enCollisionAtrGnd;

};

CCollision::~CCollision()
{
	CCollisionManager::GetInstance()->UnRegist(m_sIndex);
}

