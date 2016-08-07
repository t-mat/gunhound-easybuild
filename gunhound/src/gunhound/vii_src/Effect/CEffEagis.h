class CEffEagis : public CEffectBase
{
public:
	CEffEagis(Sint32 x , Sint32 y, Sint32 rot=0 , Sint32 Speed = 0 );
	~CEffEagis();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();

	void SetBurniaOn()
	{
		m_bBurnia = gxTrue;
	}

	void sw_on(Sint32 n)
	{
		switch(n){
		case 0:
			m_bShockA = gxTrue;
			break;
		case 1:
			m_bShockB = gxTrue;
			break;
		}
	}

	void Disp(gxBool bFlag)
	{
		m_bDisp = bFlag;
	}

	void sw_off(Sint32 n)
	{
		switch(n){
		case 0:
			m_bShockA = gxFalse;
			break;
		case 1:
			m_bShockB = gxFalse;
			break;
		}
	}

	void SetDebugControl()
	{
		m_bControl = gxTrue;
	}
private:

	void Control();

	gxBool m_bShockA;
	gxBool m_bShockB;
	gxBool m_bBurnia;
	gxBool m_bControl;

	Sint32 m_sColor;
	Sint32 m_sAdd_Y;
	Sint32 m_sTimer;
	gxBool m_bDisp;
};

