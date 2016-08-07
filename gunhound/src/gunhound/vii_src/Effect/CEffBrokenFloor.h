class CEffBrokenFloor : public CEffectBase
{
public:
	CEffBrokenFloor(Sint32 x,Sint32 y );
	~CEffBrokenFloor();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

private:



};


class CEffBrokenFloorBlock : public CEffectBase
{
public:
	CEffBrokenFloorBlock(Sint32 x,Sint32 y );
	~CEffBrokenFloorBlock();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

private:

	Float32 m_fScale;
	Sint32 m_sRotation;
	Sint32 m_sAddY;
	Sint32 m_sAddX;

};

