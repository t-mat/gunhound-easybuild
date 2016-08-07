//--------------------------------------------------
//
// gxOrderManager.h
// リクエストされた描画をオーダリングテーブルに格納します
//
//--------------------------------------------------

#include <gxLib.h>
#include "gx.h"
#include "gxOrderManager.h"

SINGLETON_DECLARE_INSTANCE( COrderManager )

#define SORT_BSORT (0)
#define SORT_QSORT (1)
#define SORT_RSORT (0)

COrderManager::COrderManager()
{
	m_pOrder = new StOrder[ enOrderMax ];
	m_sCount = 0;

	s_pInstance = this;

	m_uSubOrderCnt = 0;

	for(Sint32 ii=0;ii<MAX_TEXT_NUM;ii++)
	{
		m_pStringBuffer[ii] = NULL;
	}

	m_sStringCnt = 0;

	m_pSubOrder = new StOrder[ 300000 ];//enOrderMax ];
	m_pLastOrder = NULL;
}


COrderManager::~COrderManager()
{
	delete[] m_pOrder;
	delete[] m_pSubOrder;


	s_pInstance = NULL;
}


Sint32 COrderManager::set(StOrder* pOrder)
{
	if( m_sCount >= MAX_ORDER_NUM )
	{
		return -1;
	}

	pOrder->m_pNextOrder = NULL;

	if( pOrder->sType == enOrderTypeFont && m_sStringCnt < MAX_TEXT_NUM )
	{
		gxChar *p = (gxChar*)pOrder->pString;
		Uint32 len;

		len = strlen(p)+1;

		m_pStringBuffer[ m_sStringCnt ] = new gxChar[len];
		memcpy( m_pStringBuffer[ m_sStringCnt ], pOrder->pString , len-1 );
		m_pStringBuffer[ m_sStringCnt ][len-1] = 0x00; 

		pOrder->pString = (void*)m_pStringBuffer[ m_sStringCnt ];
		m_sStringCnt ++;
	}
	else if( m_pLastOrder != NULL  )
	{
		//


		if( m_pLastOrder->sType == pOrder->sType )
		{
			if( m_pLastOrder->pr == pOrder->pr )
			{
				if( m_pLastOrder->sAtribute == pOrder->sAtribute )
				{
					if( m_pLastOrder->pg == pOrder->pg )
					{
						//前と同じタイプのオーダーだった
						//反転を意識させるか？
						m_pSubOrder[ m_uSubOrderCnt ] = *pOrder;
						m_pLastOrder->m_pNextOrder = &m_pSubOrder[ m_uSubOrderCnt ];

						m_pLastOrder = &m_pSubOrder[ m_uSubOrderCnt ];

						m_uSubOrderCnt ++;
						return m_sCount;
					}
				}
			}
		}

	}

	//------------------------

	m_pOrder[m_sCount] = *pOrder;

	m_pLastOrder = &m_pOrder[m_sCount];

	m_sCount ++;

	return m_sCount-1;
}


void COrderManager::Reset()
{
	//オーダリングテーブルの中身をリセットする

	for(Sint32 ii=0;ii<MAX_TEXT_NUM;ii++)
	{
		if( m_pStringBuffer[ii] )
		{
			delete[] m_pStringBuffer[ii];
			m_pStringBuffer[ii] = NULL;
		}
	}

	m_sStringCnt = 0;

	m_sCount = 0;

	m_uSubOrderCnt = 0;

	m_pLastOrder = NULL;

}


void COrderManager::ZSort()
{
	//Zソートを行う

#if SORT_QSORT
	for(Uint32 ii=0; ii<(Uint32)m_sCount; ii++)
	{
		//m_zSort[ii]  = m_pOrder[ii].pr+ii/10000.f;	//あくまで描画順にあわせるためにリクエストIDを足しこんでおく
		m_zSort[ii] = (m_pOrder[ii].pr<<16) + ii;
		m_zIndex[ii] = ii;
	}
	if(m_sCount>0)QSort( m_zSort , m_zIndex , 0 , m_sCount -1 );

#endif

#if SORT_BSORT
	for(Uint32 ii=0; ii<(Uint32)m_sCount; ii++)
	{
		m_zSort[ii]  = m_pOrder[ii].pr+ii/10000.f;	//あくまで描画順にあわせるためにリクエストIDを足しこんでおく
		m_zIndex[ii] = ii;
	}
	if(m_sCount>0) BSort( m_zSort , m_zIndex , 0 , m_sCount -1 );

#endif


#if SORT_RSORT

	for(Uint32 ii=0; ii<(Uint32)m_sCount; ii++)
	{
		m_zSort[ii]  = m_pOrder[ii].pr*100+ii;	//あくまで描画順にあわせるためにリクエストIDを足しこんでおく
		m_zIndex[ii] = ii;
	}

	// 基数ソートを使ってみる
	RSort( m_zSort, m_zIndex, m_sCount );
#endif

}





void COrderManager::BSort(Float32* zsort,Uint32* zIndex ,int left, int right)
{
	//-----------------------------------------------------------------------
	//とりあえずバブルソート
	//-----------------------------------------------------------------------

#if 1

	Float32 fmin = -1;

	for(Sint32 ii=0;ii<m_sCount;ii++)
	{
		fmin = zsort[ii];

		for(Sint32 jj=ii+1;jj<m_sCount;jj++)
		{
			if( zsort[jj] < zsort[ii] || fmin == -1 )
			{
				fmin = zsort[jj];
				SWAP(zsort[ii], zsort[jj]);			 /* x[i] と x[j]を交換 */
				SWAP(zIndex[ii], zIndex[jj]);			 /* x[i] と x[j]を交換 */
			}
		}
	}

	return;
#endif
}


void COrderManager::QSort(Sint32* zsort,Uint32* zIndex ,int left, int right)
{
	//クイックソート

	int i, j;
	Sint32 pivot;

	i = left;								/* ソートする配列の一番小さい要素の添字 */
	j = right;								/* ソートする配列の一番大きい要素の添字 */

	pivot = zsort[(left + right) / 2];		/* 基準値を配列の中央付近にとる */

	while (1) {								/* 無限ループ */

		while (zsort[i] < pivot)			/* pivot より大きい値が */
			i++;							/* 出るまで i を増加させる */

		while (pivot < zsort[j])			/* pivot より小さい値が */
			j--;							/*  出るまで j を減少させる */

		if (i >= j)							/* i >= j なら */
			break;							/* 無限ループから抜ける */
		SWAP(zsort[i], zsort[j]);			/* x[i] と x[j]を交換 */
		SWAP(zIndex[i], zIndex[j]);			/* x[i] と x[j]を交換 */
		i++;								/* 次のデータ */
		j--;
	}

/*
		for(Sint32 yy=0;yy<9;yy++)
		{
			gxLib::DebugLog( "%d (%d),",(zsort[yy]>>16)&0x0000ffff , zsort[yy]&0x0000ffff);
		}
		gxLib::DebugLog( "----------------%d  %d" ,left , right );
*/
	if (left < i - 1)						/* 基準値の左に 2 以上要素があれば */
		QSort(zsort, zIndex,left, i - 1);	/* 左の配列を Q ソートする */
	if (j + 1 <  right)						/* 基準値の右に 2 以上要素があれば */
		QSort(zsort, zIndex,j + 1, right);	/* 右の配列を Q ソートする */
}


#define MAX_RADIX (10000)

void COrderManager::RSort(Float32* zsort, Uint32* zIndex, Uint32 size)
{
	//基数ソート
	
	// テンポラリ配列生成
	Float32* fTmp = new Float32[size];
	Uint32*	uTmp = new Uint32[size];
	Uint32*	uRad = new Uint32[size];
	Sint32	m = 1;

	// 基数の取り出し
	while( m <= MAX_RADIX ) {
		for( Uint32 i = 0;i < size;i++ )
		{
			uRad[i] = ( (Uint32)( zsort[i]/m ) % 10 );
		}

		int k = 0;

		for( Uint32 i = 0;i <= 9;i++ ) {
			for( Uint32 j = 0;j < size;j++ ) {
				if( uRad[j] == i ) {
					fTmp[k] = zsort[j];
					uTmp[k] = zIndex[j];
					k++;
				}
			}
		}
		
		for( Uint32 i = 0;i < size;i++ ) {
			zsort[i] = fTmp[i];
			zIndex[i] = uTmp[i];
		}
		
		m *= 10;
	}

	delete[] fTmp;
	delete[] uTmp;
	delete[] uRad;
}




