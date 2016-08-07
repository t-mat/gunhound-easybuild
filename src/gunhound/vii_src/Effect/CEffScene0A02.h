class CSoldierBase;
class CEffScene0A02 : public CEffectBase
{
public:

	CEffScene0A02(Sint32 x,Sint32 y ,Sint32 z ,Sint32 sStat = 0);
	~CEffScene0A02();

	void SeqInit();
	void SeqMain();
	void SeqCrash();
	void SeqEnd();

	void Draw();
	void SetRain()
	{
		m_bRain = gxTrue;
	}
	gxBool IsEnd()
	{
		return m_bFinish;
	}
	void SetRainPrio( Sint32 n)
	{
		m_sTargetPrio = n;
	}


private:

	typedef struct StBrizzard {
		Sint32 x;
		Sint32 y;
		Sint32 z;
		Sint32 mx;
		Sint32 my;
		Sint32 type;
	}StBrizzard;

	enum
	{
		enBRIZZARD_MAX = (160),
	};

	Sint32 m_sPrio;
	Sint32 m_sSunStatus;

	Sint32 m_sColor;
	Sint32 m_sColorAdd;
	Sint32 m_sTargetPrio;


	StBrizzard *m_pBrizzard;
	gxBool m_bFinish;
	gxBool m_bRain;

	void RainEffect();
	void DrawSunRise();
	void DrawDayTime();
	void DrawRain();
};

