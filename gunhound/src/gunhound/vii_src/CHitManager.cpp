//------------------------------------------------------------------
//
// CHitManager
//
//------------------------------------------------------------------

#include <gunvalken.h>
#define DAMAGE_TIME (2)	//ダメージ硬直時間

void CHitManager::Action()
{
	//当たりを判定
	Sint32 i;

	EnCollisionID id1,id2;

	gxBool bExistFlag;
	CHitCollision *pHitCollisionA,*pHitCollisionB;

	for(Sint32 n=0;n<2;n++)
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
			pHitCollisionA = dynamic_cast<CHitCollision*>(m_pCollision[i]);

			if( pHitCollisionA == NULL ) continue;
			if( pHitCollisionA->m_sTime > 0) pHitCollisionA->m_sTime --;
			if( pHitCollisionA->m_bJudge == gxFalse) continue;
			if( pHitCollisionA->m_bMuteki ) continue;

			DrawDebug( i );

			if( pHitCollisionA->m_sForceID != id1 ) continue;

			pHitCollisionA->m_bHit   = gxFalse;

			for(Sint32 j=0;j<MaxCollisionNum;j++)
			{
				pHitCollisionB = dynamic_cast<CHitCollision*>( m_pCollision[j] );

				if( pHitCollisionB == NULL ) continue;
				if( pHitCollisionB->m_sForceID != id2 ) continue;
				if( pHitCollisionB->m_bJudge == gxFalse) continue;
				if( pHitCollisionA->m_bMuteki ) continue;

				bExistFlag = gxFalse;
				pHitCollisionB->m_bFisrtDead = gxFalse;

				if( HitCheck(i,j) )
				{
					pHitCollisionA->m_bHit = gxTrue;
					pHitCollisionA->m_sTime = DAMAGE_TIME;

					if(pHitCollisionA->m_sAP > 0) pHitCollisionB->m_bHit = gxTrue;
					if(pHitCollisionA->m_sAP > 0) pHitCollisionB->m_sTime = DAMAGE_TIME;

					if( pHitCollisionB->m_sHP > 0)
					{
						bExistFlag = gxTrue;
					}

					pHitCollisionB->m_sHP -= pHitCollisionA->m_sAP;

					if(pHitCollisionA->m_sAP > 0) pHitCollisionB->m_sDamageCount = DAMAGE_TIME;	//攻撃力のないアイテム（パワーチップなどではダメージ受けない）

					pHitCollisionA->m_bImpact = gxTrue;
					if(pHitCollisionA->m_sAP > 0) pHitCollisionB->m_bImpact = gxTrue;

					pHitCollisionA->m_bFlag   = gxTrue;
					if(pHitCollisionA->m_sAP > 0) pHitCollisionB->m_bFlag   = gxTrue;

					pHitCollisionB->m_sAtackType = pHitCollisionA->m_sAtackType;

					if( pHitCollisionB->m_sHP <= 0)
					{
						if( bExistFlag )
						{
							pHitCollisionB->m_bFisrtDead = gxTrue;
						}
						pHitCollisionB->m_sHP = 0;
					}
				}

			}
		}
	}

	//-------------------------------------------------------------------------
	//当たり判定のバックアップを取る
	//-------------------------------------------------------------------------

	Sint32 sCnt = 0;
	CHitCollision *pHitCollision;

	for(i=0;i<MaxCollisionNum;i++)
	{
		//敵のくらい判定をホーミング用の過去バッファに入れる
		pHitCollisionB = dynamic_cast<CHitCollision*>( m_pCollision[i] );

		if( pHitCollisionB == NULL ) continue;
		if(!( pHitCollisionB->m_sForceID == enCollisionID_B_Defence || pHitCollisionB->m_sForceID == enCollisionID_B_Atack )) continue;
		if( pHitCollisionB->m_bJudge == gxFalse) continue;
		if( !pHitCollisionB->m_bHomingable ) continue;


		pHitCollision = dynamic_cast<CHitCollision*>(m_pCollision[i]);

		m_stHit[sCnt].x = (pHitCollisionB->x1+pHitCollisionB->x2) / 2;
		m_stHit[sCnt].y = (pHitCollisionB->y1+pHitCollisionB->y2) / 2;
		m_stHit[sCnt].id = pHitCollisionB->m_sForceID;

		sCnt ++;
	}

	m_sOldHitMax = sCnt;
	Sint32 HitCnt = 0;
	for(i=0;i<MaxCollisionNum;i++)
	{
		CHitCollision *pHitCollision;
		pHitCollision = dynamic_cast<CHitCollision*>( m_pCollision[i] );

		if( pHitCollision )
		{
			pHitCollision->m_bJudge = gxFalse;
			pHitCollision->m_bMuteki = gxFalse;	//毎フレーム初期化される
			HitCnt ++;
		}
	}


	//HITカウントを初期化（超重要）
	ClearHitCount();
}


gxBool CHitManager::HitCheck( Sint32 a1,Sint32 b1 )
{
	//-----------------------------------------------
	//矩形での当たり判定を取る
	//-----------------------------------------------

	Sint32 ax1,ay1,ax2,ay2;
	Sint32 bx1,by1,bx2,by2;

	CHitCollision *pHitCollisionA,*pHitCollisionB;
	pHitCollisionA = dynamic_cast<CHitCollision*>(m_pCollision[a1]);
	pHitCollisionB = dynamic_cast<CHitCollision*>(m_pCollision[b1]);

	ax1 = pHitCollisionA->x1;
	ay1 = pHitCollisionA->y1;
	ax2 = pHitCollisionA->x2;
	ay2 = pHitCollisionA->y2;

	bx1 = pHitCollisionB->x1;
	by1 = pHitCollisionB->y1;
	bx2 = pHitCollisionB->x2;
	by2 = pHitCollisionB->y2;

	if(bx2 >= ax1 && bx1 <= ax2)
	{
		if(by2 >= ay1 && by1 <= ay2)
		{
			pHitCollisionB->dx = (ax1+ax2)/2-(bx1+bx2)/2;
			pHitCollisionB->dy = (ay1+ay2)/2-(by1+by2)/2;
			return true;
		}
	}

	return false;

}


void CHitManager::DrawDebug(Sint32 n)
{
#ifndef _VII_DEBUG_
//	return;
#endif

	if( pGame->GetRaderScope() != enScopeTypeSnipe )
	{
		return;
	}

	//デバッグ用あたり判定表示
	Sint32 x1,y1,x2,y2;
	gxBool bBoxFill=gxTrue;

	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );

	CHitCollision *pHitCollision;

	pHitCollision = dynamic_cast<CHitCollision*>(m_pCollision[n]);

	x1 = pHitCollision->x1-sx;
	y1 = pHitCollision->y1-sy;
	x2 = pHitCollision->x2-sx;
	y2 = pHitCollision->y2-sy;

	Sint32 col=0;
	if(pHitCollision->m_sForceID&ID_VERSIS_ATK) {col = 0xFFFF0000;	bBoxFill = gxFalse;}
	if(pHitCollision->m_sForceID&ID_VERSIS_DEF) {col = 0x20FFFF00;	bBoxFill = gxTrue;}
	if(pHitCollision->m_sForceID&ID_ENEMY_ATK)  {col = 0xFF0000FF;	bBoxFill = gxFalse;}
	if(pHitCollision->m_sForceID&ID_ENEMY_DEF)  {col = 0x20FF00FF;	bBoxFill = gxTrue;}


	//デバッグ用
	if(DEBUG_ATARIOBJ)	viiDraw::Box(x1,y1,x2,y2,PRIO_DEBUG,bBoxFill,col,ATR_DFLT);
}


