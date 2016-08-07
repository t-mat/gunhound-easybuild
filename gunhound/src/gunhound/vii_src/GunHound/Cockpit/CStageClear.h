//---------------------------------------------------
//ステージクリア画面
//---------------------------------------------------

class CStageClear
{
public:
	CStageClear();
	~CStageClear();

	void Action();
	void Draw();
	gxBool IsEnd()
	{
		return m_bEnd;
	}

private:

	Sint32 m_sScrollw;
	Sint32 m_sSeq;
	gxBool m_bEnd;
};


