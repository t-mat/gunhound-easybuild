class CBellowsSprite
{
public:
	enum {
		enBellowNum = 64,
		enBellowMax = enBellowNum*2+1,
	};

	CBellowsSprite();
	~CBellowsSprite();

	void Action();
	void Draw();

	void AddPosition( Sint32 x1, Sint32 y1 ,Sint32 x2, Sint32 y2 );
	void SetColor( Uint32 uSrc , Uint32 uDst );
	void SetLength( Float32 m_fLength );

	ghPos m_Pos;

private:

	ghPos     m_PosSrc[enBellowMax];
	ghPos     m_PosMid[enBellowMax];
	GRA_POS_T m_PosDst[enBellowMax*4];

	Uint32 uColor[2];

	Float32 m_fLength;
	Float32 m_fMaxLength[2];
	Sint32 m_sMax;

};


