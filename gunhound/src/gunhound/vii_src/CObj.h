class CPOS
{
public:
	CPOS(){
		x = y = z = 0;
	}
	~CPOS(){
	}
	Sint32 x,y,z;
private:

};

class CObj
{
	//‚ ‚½‚è”»’è”»•Ê—p
public:
	CObj();
	~CObj();
	CPOS pos;
	CPOS mov;
//	CPOS tmp;
	CPOS atari1;
	CPOS atari2;
	gxBool m_bNoCheckHalfWall;
	void disp_debug(Sint32 n);
/*
	gxBool atari_hosei();
	gxBool atari_check(Sint32 type = 0);
	gxBool atari_check2();
	gxBool atari_check_xy(Sint32 xx,Sint32 yy,Sint32 type = 0);
	Sint32 atari_check_naname(Sint32 xx,Sint32 yy);
	gxBool chakuti_;	//’…’n”»’è—pƒtƒ‰ƒO
*/

	//------------------------------------------
	//‘æ‚S’e
	//------------------------------------------
	gxBool is_launding();
/*
	gxBool check_block();
	gxBool check_naname_block();
	gxBool check_naname_block2();
	gxBool check_naname_block3();
	Sint32 is_atari(Sint32 x,Sint32 y);
	gxBool is_block(Sint32 x,Sint32 y);
	gxBool is_naname_block(Sint32 x,Sint32 y);
	gxBool is_naname_block2(Sint32 x,Sint32 y,gxBool adjust_halfkabe=gxTrue);
*/
	//------------------------------------------
	//‘æ‚T’e
	//------------------------------------------
/*
	Sint32 get_kabe_inside(Sint32 x,Sint32 y);
	gxBool adjust_squareblock(Sint32 x,Sint32 y,Sint32 &xx,Sint32 &yy);
	gxBool is_square_block(Sint32 x,Sint32 y);
	Sint32 is_triangle_block(Sint32 x,Sint32 y);
	void adjust_pixel(Sint32 x,Sint32 y,Sint32 &xx,Sint32 &yy);
	gxBool is_kabe_inside(Sint32 x,Sint32 y);
	Sint32  Position_num_get();
	gxBool get_Position_xy(Sint32 n,Sint32 &x,Sint32 &y);
	Sint32  atari_check_all();
	Sint32  adust_all();
	Sint32  line_adjust();
*/

	//------------------------------------------
	//‘æ‚U’e
	//------------------------------------------

	Sint32 w_,h_;
	Sint32 adjust_move(Sint32 mode);
	Sint32  get_tile(Sint32 ax,Sint32 ay);
	Sint32  get_tile_xy(Sint32 ax,Sint32 ay);
	Sint32  is_hit(Sint32 x,Sint32 y);
	gxBool is_landnow();

	Sint32 move_mode_;
//	Sint32 hit_now_;
//	Sint32 check_through_;
//	gxBool area_hit_;
//	void move_env_shoot();
//	void move_env_space();
//	void move_env_gravit();
//	void adjust_block_area();
//	void adjust_kabe(Sint32 x,Sint32 y,Sint32 &xx,Sint32 &yy);
//	gxBool check_block(Sint32 px,Sint32 py,Sint32 mx,Sint32 my,Sint32 w,Sint32 h);
//	void adjust_kabe2(Sint32 x,Sint32 y,Sint32 &xx,Sint32 &yy);

	//------------------------------------------
	//‘æ‚V’e
	//------------------------------------------
//	void adjust_moveline(Sint32 x,Sint32 y,Sint32 &mx,Sint32 &my);

	//------------------------------------------
	//‘æ‚W’e
	//------------------------------------------
	Sint32 land_cnt;
//	gxBool adjust_gravit_env(Sint32 x,Sint32 y,Sint32 &mx,Sint32 &my);
//	gxBool final_umari_check(Sint32 x,Sint32 y,Sint32 &mx,Sint32 &my);

	//------------------------------------------
	//‘æ‚X’e
	//------------------------------------------
	Sint32 top_cnt;
	Sint32 KyoyouRange;
	void collision_check();
	gxBool check_allblock(Sint32 x1,Sint32 y1,Sint32 x2,Sint32 y2);
	gxBool check_under(Sint32 x,Sint32 y,Sint32 &mx,Sint32 &my);
	gxBool check_top(Sint32 x,Sint32 y,Sint32 &mx,Sint32 &my);
	gxBool check_side(Sint32 x,Sint32 y,Sint32 &mx,Sint32 &my);
	gxBool easy_collision_check();
	gxBool is_kabetype(Sint32 type);
	gxBool m_bAtariOff;
	gxBool Dummy;
private:

};


