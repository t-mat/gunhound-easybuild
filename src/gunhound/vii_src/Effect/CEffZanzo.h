class CEffZanzo : public CEffectBase
{
public:
	CEffZanzo( Sint32 x,Sint32 y );
	~CEffZanzo();

//	void SeqInit();
	void SeqMain();
	void SeqCrash();
//	void SeqEnd();

	void Draw();

private:

	Sint32 m_sScale;
	Sint32 m_sColor;

};

void EffectMakeBomb( Sint32 x, Sint32 y, Sint32 num , Sint32 spd );
