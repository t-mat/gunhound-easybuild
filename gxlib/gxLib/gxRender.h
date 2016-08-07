enum {
	enCommandMax = MAX_ORDER_NUM*4,

	enVertexMax  = 65535*4,
	enIndexMax   = 65535*2,
};

typedef struct StCmdMake {
	Sint32 sType;
	Sint32 sStartVertex;
	Sint32 sLastVertex;
	Sint32 sStartIndex;
	Sint32 sLastIndex;
	Sint32 arg1,arg2,arg3,arg4;
	Sint32 x,y;
	void *pString;
}StCmdMake;

class CRender
{
public:
	CRender();
	~CRender();

	void Action();
	void Draw();

	CCommandList* GetCommandList(Sint32 n)
	{
		return &m_pCommand[n];
	}

	Sint32 GetCommandNum()
	{
		return m_sCommandNum;
	}

	StCustomVertex* GetVertex(int n)
	{
		return &m_pCutomVertex[n];
	}

#if INDEXBUFFER_BIT == 32
	Uint32* GetIndexBuffer(int n)
#else
	Uint16* GetIndexBuffer(int n)
#endif
	{
		return &m_pIndexBuffer[0];
	}

//	Uint32* GetIndexBuffer32(int n)
//	{
//		return &m_pIndexBuffer[0];
//	}

	Uint32 GetVertexNum()
	{
		return m_sVBufferNum;
	}

	Uint32 GetIndexNum()
	{
		return m_sIBufferNum;
	}

	void SetClearColor( Uint32 argb )
	{
		m_uBackGroundColor = argb;
		m_fBackGroundColor_A = 1.f*((argb & 0xff000000) >> 24) / 255.f;
		m_fBackGroundColor_R = 1.f*((argb&0x00ff0000)>>16)/255.f;
		m_fBackGroundColor_G = 1.f*((argb&0x0000ff00)>>8 )/255.f;
		m_fBackGroundColor_B = 1.f*((argb&0x000000ff)>>0 )/255.f;
	}

	Float32 getBgColor( Sint32 n )
	{
		switch(n){
		case 0:	//a
			return m_fBackGroundColor_A;
		case 1:	//r
			return m_fBackGroundColor_R;
		case 2:	//g
			return m_fBackGroundColor_G;
		case 3:	//b
			return m_fBackGroundColor_B;
		default:
			return 1.f;
		}
		return 1.f;
	}

	SINGLETON_DECLARE( CRender );

private:

	void ZSort(int* zsort, int left, int right);
	void makeCommand(Uint32 index);

	INLINE void setVertex( StCustomVertex *pVtx0 , StOrder* pOdr );
	INLINE void setTextureUV( Sint32 tpg , StCustomVertex *pVtx0 , StOrder* pOdr );
	INLINE Float32 getTetureOffsetX( Sint32 page , Sint32 x );
	INLINE Float32 getTetureOffsetY( Sint32 page , Sint32 y );

//	INLINE void addCommand( Uint32 sType , Uint16 a1,Uint16 a2,Uint16 a3,Uint16 a4, Sint16 x , Sint16 y , void* pString=NULL );
	INLINE void addCommand( StCmdMake *pCmd );

	INLINE void atacheTexture( Sint32 page );

	INLINE void drawSprite( StOrder* pOdr );
		INLINE void spriteRotationScale( StOrder* pOdr );
		INLINE void spriteScale( StOrder* pOdr );
		INLINE void spriteNormal( StOrder* pOdr );

	INLINE void drawLine( StOrder* pOdr );
	INLINE void drawFont( StOrder* pOdr );
	INLINE void drawPoint( StOrder* pOdr );
	INLINE void drawTriangle( StOrder* pOdr );
	INLINE void drawLineStrip( StOrder* pOdr );
	INLINE void drawTriangleSprite( StOrder* pOdr );

	INLINE void setAttribute( Uint32 sAtr);

	CCommandList*   m_pCommand;
	Sint32          m_sCommandNum;
	Uint32			m_sLastOdrType;

	StCustomVertex* m_pCutomVertex;
	Sint32          m_sVBufferNum;
	Sint32			m_sStartVertex;
	Sint32			m_sLastVertex;

#if INDEXBUFFER_BIT == 32
	Uint32*			m_pIndexBuffer;
#else
	Uint16*			m_pIndexBuffer;
#endif
	Uint32			m_sIBufferNum;

	Sint32			m_sStartIndex;
	Sint32			m_sLastIndex;
	Sint32			m_sIndexCnt;
	Sint32          m_sOldBankNum;
	Sint32          m_sOldAttribute;

	StCmdMake stCmd;
	StCmdMake stCmdOld;

	Float32 m_fTexturePageWidth;
	Float32 m_fTexturePageHeight;
	Sint32 m_sPageX;

	Uint32 m_uBackGroundColor;
	Float32 m_fBackGroundColor_A;
	Float32 m_fBackGroundColor_R;
	Float32 m_fBackGroundColor_G;
	Float32 m_fBackGroundColor_B;
};


