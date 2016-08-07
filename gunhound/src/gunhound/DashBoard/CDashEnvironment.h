class CDashEnvironment
{
public:
	CDashEnvironment();
	~CDashEnvironment();
	void Action();
	void Draw();

	gxBool IsEnd();
private:
	enum {
		enEnvSelectSize,
		enEnvSelectWide,
		enEnvSelectFrame,
//		enEnvSelectWide,
		enEnvSelectDec,
		enEnvSelectMax,
	};

	Sint32 m_sCursor;
	Sint32 m_sSelect[enEnvSelectMax];

	gxBool m_bEnd;
	gxBool m_bDecision;
};
