//--------------------------------------------------------------------------------
//
//フェードマネージャ
//
//--------------------------------------------------------------------------------

class CFadeManager {

public:

	CFadeManager();
	~CFadeManager();

	static CFadeManager* GetInstance();
	void Destroy();
	void action();

	void set_fadein(Sint32 spd=8,Sint32 c=0xFF010101,Sint32 sAtribute=ATR_DFLT );
	void set_fadeout(Sint32 spd=8,Sint32 c=0xFF010101);
	void fade_off();
	gxBool is_fade();
	Sint32 GetFadeAlpha()
	{
		return fade_timer_;
	}
private:

	static CFadeManager *g_pCFadeManager;

	void set_fade_color(Sint32 col);

	Sint32 m_sAtribute;
	Sint32 fade_timer_;
	Sint32 fade_add_;
	Sint32 fade_spd_;
	Sint32 fade_col_;
	gxBool fade_now_;
	Sint32 fade_block_;
};

