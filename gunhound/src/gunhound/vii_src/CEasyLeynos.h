//-----------------------------------
//�ȈՃA�T���g�N���X
//-----------------------------------
enum {
	EL_BASE,
	EL_BODY,
	EL_ARM_L,
	EL_ARM_R,
	EL_LEG_L,
	EL_LEG_R,
	EL_MAX,
};


typedef struct elparts_t {
	gxSprite *spr;
	Sint32 x,y;
	Sint32 ofx,ofy,ofz;
	Sint32 rot,dist;
	Sint32 prot,pdist;
	Sint32 dx,dy;
	Sint32 sx,orz;
}elparts_t;


class CEasyLeynos {

public:
	CEasyLeynos();
	~CEasyLeynos();

	gxBool m_bNoShock;
	gxBool chakuti_;
	gxBool cpu_control;	//��{��gxTrue
	Sint32 arm_rot;
	Sint32 pad_;	//�R���g���[���[
	Sint32 prio_;
	gxBool m_bDamage;
	gxBool m_bDead;
	gxBool m_bDeadEnd;
	OBJ_POS_T kansei;
	OBJ_POS_T dmg;
	CObj *obj;
	elparts_t *parts[EL_MAX];

	Sint32 walk_cnt;				//���s�J�E���^
	Sint32 walk_width_;			//���s�Ԋu�{��X
	Sint32 walk_height_;			//���s�Ԋu�{��Y
	void ResetGravity()
	{
		gv_ = 0;
	}

	void SetPrio(Sint32 n)
	{
		prio_ = n;
	}
	void SetDashMode(gxBool bFlag=gxTrue)
	{
		m_bDashMode = bFlag;
	}

	gxBool IsCarry(){
		return m_bCarryOn;
	}
	void CarryOff(){
		//�؂藣�����
		m_bCarryOn = gxFalse;
	}
	void CarryOn(){
		//���܂��
		m_bCarryOn = gxTrue;
	}
	void set_armrot_targetangle45(Sint32 x,Sint32 y);
	void absorber_proc();
	void set_pos(Sint32 x,Sint32 y,Sint32 z){
		obj->pos.x = x;
		obj->pos.y = y;
		prio_ = z;
	}

	void set_knockback(Sint32 n){
		//�r�̃m�b�N�o�b�N��ݒ�
		//shock_cnt_ = n;
		knock_back_ = n*100;
	}
	void set_mode(Sint32 n){
		//���R�ړ����[�h�^��s���[�h�ؑ�
		mode_ = n;
	}

	void set_control(Sint32 n){
		pad_ = n;
	}
	Sint32 getControl(){
		return pad_;
	}
	void set_spr(Sint32 n,gxSprite *p){
		parts[n]->spr = p;
	}
	void set_sprprio_offset(Sint32 n,Sint32 z){
		parts[n]->ofz = z;
	}
	void set_walkcycle(Sint32 n){
		walk_spd_ = n;			//���s���x�ݒ�
	}
	void set_spd(Sint32 n){
		spd_ = n;			//���s���x�ݒ�
	}
	void set_shock(){
		shock_cnt_ = 100;
	}
	void set_spdadd(Sint32 add){
		spd_add_ = add;
	}
	void set_dir(Sint32 dir){
		dir_ = dir;
	}
	Sint32 get_dir(){	return dir_;	}
	gxBool chakuti(){
		return chakuti_;
	}
	void action();
	void control();
	void set_damageflag(Sint32 dmgkansei = 0){
		m_bDamage = gxTrue;
		dmg.x     = dmgkansei;
	}
	void Draw();
	void setFirePos(Sint32 ax,Sint32 ay);
	void getFirePos(Sint32 &ax,Sint32 &ay);

	void atari_on(){
		atari_flag_ = gxTrue;
	}
	void atari_off(){
		atari_flag_ = gxFalse;
	}

	Sint32 GetShockCnt()
	{
		return shock_cnt_;
	}

	void SetARGB(Uint32 uArgb)
	{
		m_uARGB = uArgb;
	}
	void SetDamageAnime(Sint32 dx);

	gxBool IsNokezori()
	{
		if( m_sNokezori ) return gxTrue;
		return gxFalse;
	}

private:

	void DamageMotion();

	gxBool atari_flag_;
	Sint32 gv_;
	Sint32 spd_;					//���s���x
	Sint32 walk_spd_;				//���s�T�C�N�����x
	Sint32 spd_add_;
	Sint32 dir_;					//����
	Sint32 mode_;					//���R�ړ����[�h�^��s���[�h
	Sint32 atack_rot;
	Sint32 atack_dst;
	Sint32 flying_y;				//�ӂ�ӂ�I�t�Z�b�g
	Sint32 arm_shock_;
	gxBool m_bCarryOn;
	gxBool m_bDashMode;
	void calc_parts(Sint32 n);
	void draw_parts(Sint32 n);

	Sint32 absorber_;
	Sint32 shock_cnt_;
	Sint32 knock_back_;

	Sint32 m_sNokezori;

	Uint32 m_uARGB;
};


void EasyLeynosTest();
