//--------------------------------------------------
//
// gxOrderManager.h
//
//--------------------------------------------------

#ifndef _GGXORDER_MANAGER_H_
#define _GGXORDER_MANAGER_H_

 enum
{
	enOrderMax = MAX_ORDER_NUM,
};

enum
{
	enOrderTypePoint,
	enOrderTypeLine,
	enOrderTypeTriangle,
	enOrderTypWired,
	enOrderTypeNoneTexPolygon,
	enOrderTypeTexturePolygon,
	enOrderTypeTextureTriangle,
	enOrderTypeFont,
};

typedef struct StOrder {
	Sint32  sType;			// 0	//オーダータイプ
	Sint32  sAtribute;		// 4	//アトリビュート
	Sint32  x,y,pr;			// 12	//表示位置
	Float32 sx,sy,sz;		// 12	//拡大率
	Float32 rx,ry,rz;		// 12	//回転
	Sint32  pg;				// 4	テクスチャページ
	Sint32  u,v,w,h,cx,cy;	// 16	//テクスチャ座標＆中心位置

	Uint32 color[4];		// 16
	void    *pString;

	Uint32 opt;				//オプション

	Float32 x1[4],y1[4],z1[4],u1[4],v1[4];

	StOrder *m_pNextOrder;	//同一プリミティブがある場合の処理

} StOrder;

class COrderManager
{

public:
	COrderManager();
	~COrderManager();

	StOrder* get( Sint32 n )
	{
		return &m_pOrder[n];
	}

	Sint32 set(StOrder* pOrder);
	void Reset();

	void ZSort();

	Uint32* getZsortBuf()
	{
		return m_zIndex;
	}

	Sint32 GetOrderNum()
	{
		return m_sCount;
	}

	Sint32 GetSubOrderNum()
	{
		return m_uSubOrderCnt;
	}

private:

	SINGLETON_DECLARE( COrderManager );

	void QSort(Sint32* zsort,Uint32* zIndex, int left, int right);
	void RSort(Float32* zsort, Uint32* zIndex, Uint32 size);
	void BSort(Float32* zsort,Uint32* zIndex ,int left, int right);

	StOrder* m_pOrder;
	Sint32   m_sCount;
	gxChar*  m_pStringBuffer[MAX_TEXT_NUM];
	Sint32   m_sStringCnt;
//	Float32  m_zSort[MAX_ORDER_NUM];
	Sint32   m_zSort[MAX_ORDER_NUM];
	Uint32   m_zIndex[MAX_ORDER_NUM];


	Uint32 m_uSubOrderCnt;
	StOrder* m_pSubOrder;
	StOrder* m_pLastOrder;


};


#endif


