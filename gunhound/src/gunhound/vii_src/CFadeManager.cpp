//--------------------------------------------------------------------------------
//
//フェードマネージャ
//
//--------------------------------------------------------------------------------
#include <gunvalken.h>

CFadeManager* CFadeManager::g_pCFadeManager = NULL;

CFadeManager::CFadeManager()
{
	fade_add_ = 0;
	fade_timer_ = 0;
	fade_spd_ = 8;
	fade_col_ = 0x00010101;
	fade_now_ = gxFalse;
	fade_block_ = 0;

	m_sAtribute = ATR_DFLT;
	g_pCFadeManager = this;
}

CFadeManager::~CFadeManager()
{
	g_pCFadeManager = NULL;
}

CFadeManager* CFadeManager::GetInstance()
{
	if(g_pCFadeManager== NULL)
	{
		g_pCFadeManager = new CFadeManager();
	} 
	return g_pCFadeManager;
}

void CFadeManager::Destroy()
{
	if( g_pCFadeManager )
	{
		delete g_pCFadeManager;
	}
}

void CFadeManager::action()
{
	//フェードメイン

	if(fade_add_ < 0){
		//フェードイン中
		fade_now_ = gxTrue;
		fade_timer_ += fade_add_;
		if(fade_timer_ <= 0){
			fade_timer_ = 0;
			if(fade_block_ < 3) fade_block_ ++;
		}
	}else if(fade_add_ > 0){
		//フェードアウト中
		fade_now_ = gxTrue;
		fade_timer_ += fade_add_;
		if(fade_timer_ >=255){
			fade_timer_ = 255;
			if(fade_block_ < 3) fade_block_ ++;
		}
	}

	if(fade_timer_ <= 0) 	fade_timer_ = 0;
	if(fade_timer_ >= 255)	fade_timer_ = 255;

	if(	fade_block_ == 3){
		fade_now_ = gxFalse;
	}

	//フェードスクリーン表示
	if(fade_timer_){
		Sint32 alpha = fade_timer_*((fade_col_&0xFF000000)>>24)/255;
		 alpha = alpha<<24;//fade_timer_<<24;
		Sint32 col = fade_col_&0x00ffffff;
		col = alpha | col;
		viiDraw::Box(0,0,WINDOW_W,WINDOW_H,PRIO_FADE,gxTrue,col,m_sAtribute);
	}
}

void CFadeManager::set_fadein(Sint32 spd  ,Sint32 c , Sint32 sAtribute)
{
	fade_timer_ = 255;
	fade_now_ = gxTrue;
	fade_add_ = -spd;
	fade_block_ = 0;
	m_sAtribute = sAtribute;
	set_fade_color(c);
}

void CFadeManager::set_fadeout(Sint32 spd,Sint32 c)
{
	fade_timer_ = 0;
	fade_now_ = gxTrue;
	fade_add_ = spd;
	fade_block_ = 0;
	set_fade_color(c);
}

void CFadeManager::fade_off()
{
	fade_add_ = -255;
	fade_timer_ = 0;
	fade_block_ = 3;
	fade_now_ = gxFalse;
}

gxBool CFadeManager::is_fade()
{
	return fade_now_;
}

void CFadeManager::set_fade_color(Sint32 col)
{
	fade_col_ = col;
}
