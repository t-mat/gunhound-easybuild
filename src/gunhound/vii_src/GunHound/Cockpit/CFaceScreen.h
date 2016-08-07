class CFaceScreen
{
public:
	CFaceScreen();
	~CFaceScreen();

	void Action();
	void Draw(Sint32 bx,Sint32 by,Sint32 sPercent=100);
	void SetKuchiPaku(gxBool bFlag);
	void SetFace(Sint32 sIndex)
	{
		m_sFaceIndex = sIndex;
	}


private:

	Float32 m_fTimer;
	gxBool m_bKuchiPaku;

	Sint32 m_sFaceIndex;
};


