class CEffScene02 : public CEffectBase
{
public:

	CEffScene02(Sint32 x,Sint32 y ,Sint32 z );
	~CEffScene02();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

	CEneH0406Carrymissile* GetTorpedo()
	{
		return m_pTorpedo;
	}

	void Set(Sint32 sSeq)
	{
		m_sSeq = sSeq;
	}

	void SetControl(Sint32 sControl)
	{
		m_sControl = sControl;
	}

private:
	enum {
		enNancyLarge,
		enNancyMiddle,
		enNancySmall,
		enNancyMax,
	};

	Sint32 m_sSeq;

	CEneH0406Carrymissile  *m_pTorpedo;						//‹›—‹
	CEneH0405Nancy *m_pNacy[enNancyMax];
	CEneH0407Nautilus *m_pNautilus;

	ghPos m_ScenePos;
	ghPos m_Add;
	Sint32 m_sEnsyutuTimer;
	Sint32 m_sControl;
};

