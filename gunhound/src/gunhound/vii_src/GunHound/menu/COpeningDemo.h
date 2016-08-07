class COpeningDemo
{
public:
	enum {
		enOperationNone,
		enOperationInit,
		enOperationMain,
		enOperationEnd,
	};

	COpeningDemo();
	~COpeningDemo();

	void Action();
	void Draw();

	gxBool IsEnd()
	{
		return m_bEnd;
	}

	Sint32 GetOperation()
	{
		return m_sOperationIndex;
	}

	void SetStatusGameOver();

private:

	Sint32 m_sTimer;
	Sint32 m_sAlpha;

	Sint32 m_sSequence;
	Sint32 m_sOperationIndex;

	Sint32 m_sJoyTrig;
	Sint32 m_sJoyRep;

	gxBool m_bEnd;
};


