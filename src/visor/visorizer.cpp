//----------------------------------------------
//�o�`�k�l �r�g�n�n�s�d�q
//shooting.c
//
//----------------------------------------------

//----------------------------------------------
//�C���N���[�h
//----------------------------------------------
#include <gxLib.h>
#include "visorizer.h"

//----------------------------------------------
//�}�N��
//----------------------------------------------
#define enObjMax 		(90)		//�}�b�N�X�I�u�W�F�N�g��
#define enMyShipIndex 	(0)			//visoriser�̃I�u�W�F�N�g�i���o�[
#define MAX_MY_SHOT 	(25)		//�e�����
#define MAX_EN_SHOT 	(35)		//�G�̒e�����
#define MAX_ENEMY   	(15)		//�G�����
#define enTpgSpr    	(0)

#define PRIO_GROUND	(5)
#define PRIO_JIKI	(10)
#define PRIO_TEKI	(15)
#define PRIO_TAMA	(20)
#define PRIO_EFF	(25)
#define PRIO_SYS	(30)

void VisorizerInit(void);

static int GetAbs(int n);					//��Βl�����߂�
static int GetRand(void);					//�����_�������
static int GetStylus(int *x, int *y);
static void padControl(void);

#define GBATR_ALPHA_PLUS ATR_DFLT

//----------------------------------------------
//�^�錾
//----------------------------------------------
//�\�t�g�E�F�A�X�v���C�g��`�p�\����

//���z�p�b�h�������\����
typedef struct StGamePad {
	char psh;	//�������ςȂ�
	char trg;	//�����v�b�V��
	int x,y;
	int keydown;
} StGamePad;

//�I�u�W�F�N�g�\����
typedef struct StObj
{
	char flag;		//�����t���O
	short x,y;		//�w�A�x
	short vx,vy;	//�����l�Ȃ�
	char type;		//���
	short hp;		//�ϋv��
	//�ėp�̋��p��
	union FREE
	{
		Sint8 b[8];
		Sint16 w[4];
		Sint32 l[2];
	} free ;
} StObj;

//�Q�[���S�ʂ������\����
typedef struct StGame
{
	int x;
	char scene;
	int Score;			//�X�R�A
	//�I�u�W�F���L�^�p
	char my_shot_num;	//�e��
	char en_shot_num;	//�G�e��
	char enemy_num;		//�G��
	char bb_remain;
	int timer;
	char level;
	short option_x[35];
	short option_y[35];
	short option_pos;
	char bgm_flag;
	int r,g,b;
	int VsyncTimer;

} StGame;


//----------------------------------------------
//�O���[�o���ϐ�
//----------------------------------------------
static StObj SprObj[ enObjMax ];	//�I�u�W�F�N�g�Ǘ��p�z��
static StGame g_GameSystem;			//�Q�[���Ǘ��p
static StGamePad  g_GamePad;		//�p�b�h���͊Ǘ��p
static int g_sGameSeq = 0;
static int g_HighScore,Score_temp;

//--------------------------------
//�֐��v���g�^�C�v
//--------------------------------
static void ctrl_enMyShipIndex(void);
static void SprObj_control(void);
static void game_result(void);

static void enemy_set(void);
static void enemy_set_lv0(void);
static void enemy_set_lv1(void);
static void enemy_set_lv2(void);

static void shot_init(int n,int x,int y);
static void shot_main(int n);

static void enemy_main(int n);
static void enemy01_init(int n);
static void enemy01_main(int n);
static void enemy02_init(int n);
static void enemy02_main(int n);
static void enemy03_init(int n);
static void enemy03_main(int n);
static void enemy04_init(int n);
static void enemy04_main(int n);
static void enemy05_init(int n);
static void enemy05_main(int n);
static void enemy06_init(int n);
static void enemy06_main(int n);
static void enemy07_init(int n);
static void enemy07_main(int n);

static void ene_shot_init(int s,int type);
static void ene_shot_main(int s);
static void ene_shot_normal(int n);
static void ene_shot_laser01(int n);
static void ene_shot_bigblitz(int n);

static void bomb01_init(int s,int x, int y,int type);
static void bomb01_main(int n);
static void eff_star_init(int s,int x, int y);
static void eff_star_main(int n);
static void clear_enemy_shot(void);

static void background(void);
static void score_disp(int n,int x, int y);

static int hit_check(int n);
static int hit_check2(int n);
static int GetObjNum();
static void kill_obj(int n);

void cloud_init(void);
void cloud_main(int n);

extern StSpriteDef SprVisor[];
extern char sin_tbl[];	//�T�C���e�[�u��
extern char cos_tbl[];	//�R�T�C���e�[�u��

gxBool m_bWarning = gxFalse;	//��
Sint32 g_sFirst=0;
//---------------------------------------------------------
//�Q�[�����C��
//---------------------------------------------------------
void Visorizer(void)
{
	//-----------------------------
	//���C���R���g���[���[
	//-----------------------------
	int lev;

	if(g_sFirst==0)
	{
		//--------------------------------
		//�P�x�����ʂ鏉����
		//--------------------------------
		g_sFirst++;
		VisorizerInit();
	}

	padControl();

	if( g_GameSystem.scene >= 2 )
	{
		//�Q�[���I���i�^�C�g���j��
		game_result();
		return;
	}

	lev = g_GameSystem.level;

	background();			//�w�i
	ctrl_enMyShipIndex();	//���@
	enemy_set();			//�G�̏o�����Ԃ��Ǘ�
	SprObj_control();		//enObjMax�̃I�u�W�F�𓮂���

	if( lev != g_GameSystem.level )
	{
		//�Q�[�����x�����I�u�W�F�𓮂����O�ƁA
		//��ŕς���Ă���ʃN���A�Ƃ��āA�^�C�}�[�����Z�b�g

		g_GameSystem.timer=0;

		if(g_GameSystem.bgm_flag==0)
		{
			g_GameSystem.bgm_flag=1;
		}
	}

	g_GameSystem.VsyncTimer ++;

	gxLib::PutSprite( 0,0,200, enTpgSpr+8 ,0,0,320,320,0,0, ATR_DFLT , 0x80404000 );
	gxLib::PutSprite( 0,0,210, enTpgSpr+10,0,0,320,480,0,0, ATR_DFLT , 0xFFFFFFFF );
}


//---------------------------------------------------------
//������
//---------------------------------------------------------
void VisorizerInit(void)
{
	//�Q�[��������

	gxUtil::MemSet( SprObj,0,sizeof(StObj)*enObjMax);
	gxUtil::MemSet( &g_GameSystem,0,sizeof(StGame) );

	g_GameSystem.r = 0x00;
	g_GameSystem.g = 0x00;
	g_GameSystem.b = 0x00;

	ShootSub::SetBgColor(0xff010101);

	if( g_sGameSeq ==0 )
	{
		g_sGameSeq ++;
		g_HighScore=5100;
	}

	g_GameSystem.VsyncTimer=0;
	g_GameSystem.x=0;
	g_GameSystem.bb_remain=3;
	g_GameSystem.timer=0;
	g_GameSystem.level=0;
	g_GameSystem.scene=0;

	ShootSub::SoundLoad(0, "asset/visor/wav/bgm.ogg");
	ShootSub::SoundLoad(1, "asset/visor/wav/valcun.wav");
	ShootSub::SoundLoad(2, "asset/visor/wav/bomb01.wav");
	ShootSub::SoundLoad(3, "asset/visor/wav/bomb02.wav");
	ShootSub::SoundLoad(4, "asset/visor/wav/warning.wav");

	ShootSub::SoundPlay(0);

	gxLib::LoadTexture ( enTpgSpr   ,"asset/visor/shtimg.bmp" , 0xff004080 );
	gxLib::LoadTexture ( enTpgSpr+8 ,"asset/visor/wire.tga"   , 0xff00ff00 );
	gxLib::LoadTexture ( enTpgSpr+10,"asset/visor/gameboy.bmp", 0xff00ff00 );
	gxLib::UploadTexture();

}


static void game_result(void)
{
	//�Q�[�����ʁ��^�C�g��
	short px,py;

	//�n�C�X�R�A�X�V
	if(Score_temp >= g_HighScore)
	{
		g_HighScore = Score_temp;
	}

	px=g_GamePad.x;
	py=g_GamePad.y;

	if(g_GameSystem.scene==2)
	{
		if(g_GamePad.keydown==0)
		{
			g_GameSystem.scene=3;
		}
	}
	else if(g_GameSystem.scene==3)
	{
		if(g_GamePad.keydown)
		{
			VisorizerInit();
			return;
		}
	}

	ShootSub::PutSprite(32,50,66,PRIO_SYS,0);		//�^�C�g��

	ShootSub::PutSprite(48,80,67,PRIO_SYS,0);		//�n�C�X�R�A
	score_disp(g_HighScore,80,95);		//�n�C�X�R�A

	ShootSub::PutSprite(56,110,68,PRIO_SYS,0);	//�X�R�A
	score_disp(Score_temp,80,125);		//�X�R�A

	g_GameSystem.enemy_num=0;
	if(g_GameSystem.VsyncTimer%60==GetRand()%60)
	{
		enemy01_init(0);
	}

	SprObj_control();	//enObjMax�̃I�u�W�F�𓮂���
}


static void SprObj_control(void)
{
	//-----------------------------
	//�I�u�W�F�N�g�R���g���[��
	//-----------------------------
	int i;
	int e_sht,m_sht,ene;

	e_sht=m_sht=ene=0;

	for(i=0;i<enObjMax;i++){
		switch(SprObj[i].flag){
		case 0:	//�Ȃɂ��Ȃ�
			break;
		case 1:	//����
			break;
		case 2:	//����
			m_sht++;
			shot_main(i);
			break;
		case 3:	//�Ă�
			ene++;
			enemy_main(i);
			break;
		case 4:	//����
			bomb01_main(i);
			break;
		case 5:	//�G�̒e
			e_sht++;
			ene_shot_main(i);
			break;
		case 6:	//����
			e_sht++;
			eff_star_main(i);
			break;
		case 7:	//�_
			e_sht++;
			cloud_main(i);
			break;
		default:
			break;
		}
	}

	g_GameSystem.en_shot_num= e_sht;
	g_GameSystem.my_shot_num= m_sht;
	g_GameSystem.enemy_num  = ene;

}


static void enemy_set(void)
{
	//---------
	//�ʐ���
	//---------
	switch(g_GameSystem.level){
	case 0:
		enemy_set_lv0();
		break;
	case 1:
		enemy_set_lv1();
		break;
	case 2:
		enemy_set_lv2();
		break;
	case 3:
		//�N���A
		break;
	default:
		break;
	}
}

static void enemy_set_lv0(void)
{
	//-------------
	//�P��
	//-------------
	int timer;

	timer=g_GameSystem.timer;
	if(g_GameSystem.VsyncTimer%60==0){
		g_GameSystem.timer++;
	}

	//zako
	if(timer>1&&timer<=10){
		if(g_GameSystem.VsyncTimer%300==225){
			//���X�f�X�C���O
			enemy05_init(0);
		}
		if(g_GameSystem.VsyncTimer%10==0){
			enemy01_init(0);
		}
	}

	//�f�X�C���O
	if(timer>10&&timer<=20){
		if(g_GameSystem.VsyncTimer%40==0){
			enemy05_init(0);
		}
	}

	//�f�X�C���O��zako
	if(timer>20&&timer<=25){
		if(g_GameSystem.VsyncTimer%5==0){
			enemy01_init(0);
		}
		if(g_GameSystem.VsyncTimer%60==0){
			enemy05_init(0);
		}
	}

	//�{�X
	if(timer==30){
 		if(g_GameSystem.VsyncTimer%60==10){
			enemy02_init(0);
		}
	}

}


static void enemy_set_lv1(void)
{
	//-------------
	//�Q��
	//-------------
	int timer;

	if(g_GameSystem.VsyncTimer%60==0){
		g_GameSystem.timer++;
	}
	timer=g_GameSystem.timer;

	//ice block
	if(timer>1&&timer<=10){
		if(g_GameSystem.VsyncTimer%30==0){
			enemy06_init(0);
		}
	}

	//�f�X�C���O��zako
	if(timer>10&&timer<=25){
		if(g_GameSystem.VsyncTimer%15==60){
			enemy01_init(0);
		}
		if(g_GameSystem.VsyncTimer%45==0){
			enemy06_init(0);
		}
	}

	//zako��ice block
	if(timer>25&&timer<=28){
		if(g_GameSystem.VsyncTimer%20==0){
			enemy01_init(0);
		}
		if(g_GameSystem.VsyncTimer%60==15){
			enemy05_init(0);
		}
		if(g_GameSystem.VsyncTimer%120==0){
			enemy06_init(0);
		}
	}

	//�{�X
	if(timer==30){
		if(g_GameSystem.VsyncTimer%60==10){
			enemy03_init(0);
		}
	}

}


static void enemy_set_lv2(void)
{
	//-------------
	//�R��
	//-------------
	int timer;

	if(g_GameSystem.VsyncTimer%60==0){
		g_GameSystem.timer++;
	}
	timer=g_GameSystem.timer;

	//�f�X�C���O��ice block��zako
	if(timer>1&&timer<=10){
		if(g_GameSystem.VsyncTimer%20==0){
			enemy01_init(0);
		}
		if(g_GameSystem.VsyncTimer%120==60){
			enemy05_init(0);
		}
		if(g_GameSystem.VsyncTimer%120==30){
			enemy06_init(0);
		}
	}

	//�f�X�C���O��zako
	if(timer>10&&timer<=28){
		if(g_GameSystem.VsyncTimer%10==0){
			enemy01_init(0);
		}
		if(g_GameSystem.VsyncTimer%120==0){
			enemy05_init(0);
		}
	}

	//�{�X
	if(timer==30){
		if(g_GameSystem.VsyncTimer%60==10){
			enemy04_init(0);
		}
	}

}


static void background(void)
{
	//-----------------------------
	//�w�i�F�X�N���[��
	//-----------------------------
	int i,py;
	int r,g,b;
	static int y=0;

	if(g_GameSystem.VsyncTimer%2==0){
		y+=1;
	}
	y=y%168;
	for(i=0;i<21;i++){
		py=y+i*8;
		py=py%168;
		ShootSub::PutSprite(160-8*5+g_GameSystem.x,py-8,10+i,PRIO_GROUND,0);
	}

	if(g_GameSystem.timer==28)
	{
		if(g_GameSystem.VsyncTimer%60==5)
		{
			ShootSub::SoundPlay(4);
		}
	}

//	if(g_GameSystem.timer<=25)
//	{
//		if(g_GameSystem.VsyncTimer%7==0) cloud_init();
//	}

	//�X�R�A���c�@
	score_disp(g_GameSystem.Score,40,0);
	for(i=0;i<g_GameSystem.bb_remain;i++){
		ShootSub::PutSprite(112+i*9,2,54,PRIO_SYS,0);
	}

	switch(g_GameSystem.level){
	case 0:
		r=0x00; g=0x40; b=0x80;
		break;
	case 1:
		r=0x80; g=0x40; b=0x00;
		break;
	case 2:
		r=0xc0; g=0xa0; b=0xf0;
		r=0x80; g=0x00; b=0x80;
		break;
	default:
		r=0xff; g=0xff; b=0xff;
		break;
	}

//	r=224; g=239; b=48;
	r=8; g=24; b=32;

	//�a�f�J���[�̕ύX
	if(g_GameSystem.r< r) g_GameSystem.r++;
	if(g_GameSystem.r> r) g_GameSystem.r--;
	if(g_GameSystem.g< g) g_GameSystem.g++;
	if(g_GameSystem.g> g) g_GameSystem.g--;
	if(g_GameSystem.b< b) g_GameSystem.b++;
	if(g_GameSystem.b> b) g_GameSystem.b--;

	ShootSub::SetBgColor(0xff000000|(g_GameSystem.r<<16)|(g_GameSystem.g<<8)|(g_GameSystem.b));

}


static void score_disp(int n,int x,int y)
{
	//-----------------
	//�X�R�A�\��
	//-----------------
	int i,score;

	ShootSub::PutSprite(x+8,y,52,PRIO_SYS,0);
	if(n==0) return;

	i=0;
	while(1){
		score=n%10;
		ShootSub::PutSprite(x-8*i,y,42+score,PRIO_SYS,0);
		n=n/10;
		if(n==0) return;
		i++;
	}

}


static void ctrl_enMyShipIndex(void)
{
	//-----------------------------
	//�}�C�V�b�v�R���g���[��
	//-----------------------------
	Sint16 px,py;
	int x,y,opt,i;
	int dir;

	x=SprObj[enMyShipIndex].x;
	y=SprObj[enMyShipIndex].y;

	if(SprObj[enMyShipIndex].flag==0){
		y=SprObj[enMyShipIndex].y=200;
		SprObj[enMyShipIndex].type=0;
		if(g_GameSystem.bb_remain>0)
		{
			//�c�@�����
			g_GameSystem.bb_remain--;
			SprObj[enMyShipIndex].flag=1;
			SprObj[enMyShipIndex].type=1;
			x=SprObj[enMyShipIndex].x=80;
			for(i=0;i<30;i++)
			{
				//�I�v�V�����ʒu������
				g_GameSystem.option_x[i]=80;
				g_GameSystem.option_x[i]=180;
				g_GameSystem.option_pos=0;
			}
		}
	}

	if(SprObj[enMyShipIndex].type==1){
		if(SprObj[enMyShipIndex].y>140)
		{
			SprObj[enMyShipIndex].y--;
		}else{
			SprObj[enMyShipIndex].type=2;
		}
		x=SprObj[enMyShipIndex].x;
		y=SprObj[enMyShipIndex].y;
		SprObj[enMyShipIndex].free.b[0]=10;
	}
	px=g_GamePad.x;
	py=g_GamePad.y;
	if((SprObj[enMyShipIndex].type>=2)&&(SprObj[enMyShipIndex].type<4))
	{
		if(g_GamePad.keydown)
		{
/*
			if((GetAbs(x-px)>=16)||(GetAbs(y-py)>=16)){
				//���[�v���Ȃ����߂̕␳
				if(x<px) x+=3;
				if(x>px) x-=3;
				if(y<py) y+=3;
				if(y>py) y-=3;
			}
			else
*/
			{
				x = px;
				y = py;
			}
		}
	}

	//�p�b�h����
	dir=-1;
	int j=gxLib::Joy(0)->psh&(JOY_U|JOY_L|JOY_R|JOY_D);
	switch(j){
	case JOY_R:
		dir = 0;
		break;
	case JOY_D:
		dir = 90;
		break;
	case JOY_L:
		dir = 180;
		break;
	case JOY_U:
		dir = 270;
		break;

	case JOY_R|JOY_D:
		dir = 45;
		break;
	case JOY_D|JOY_L:
		dir = 135;
		break;
	case JOY_L|JOY_U:
		dir = 225;
		break;
	case JOY_U|JOY_R:
		dir = 315;
		break;
	}

	if(dir!=-1)
	{
		 x += cos_tbl[dir]/35;
		 y += sin_tbl[dir]/35;
	}

	if(SprObj[enMyShipIndex].type==0){
		if(g_GameSystem.bb_remain==0){
			//�Q�[���I�[�o�[�\��
			ShootSub::PutSprite(40,80,65,PRIO_SYS,0);
			if(g_GameSystem.scene==0){
				if(g_GamePad.keydown==0){
					g_GameSystem.scene=1;
				}
			}else if(g_GameSystem.scene==1){
				if(g_GamePad.keydown){
					Score_temp=g_GameSystem.Score;
					VisorizerInit();
					g_GameSystem.scene=2;
				}
			}
		}
		return;
	}

	if(SprObj[enMyShipIndex].free.b[0]>0){
		//���G����
		SprObj[enMyShipIndex].free.b[0]--;
		if(SprObj[enMyShipIndex].free.b[0]==5){
			clear_enemy_shot();
		}
	}else{
		SprObj[enMyShipIndex].type=3;
	}

	//��ʊO�␳
	if(x<=0) x=0;
	if(x>=160) x=160;
	if(y<=0) y=0;
	if( !SprObj[enMyShipIndex].type != 1 )
	{
		if(y>=160) y=160;
	}
	if(SprObj[enMyShipIndex].flag==0){
		if(y>=160-16) y=160-16;
	}

	if(g_GameSystem.level==3){
		//�N���A
		if(SprObj[enMyShipIndex].y>-100){
			SprObj[enMyShipIndex].y-=4;
		}else{
			Score_temp=g_GameSystem.Score;
			VisorizerInit();
			g_GameSystem.scene=2;
			return;
		}
		x=SprObj[enMyShipIndex].x;
		y=SprObj[enMyShipIndex].y;
	}
	ShootSub::PutSprite(x,y,0,PRIO_JIKI,0);

	g_GamePad.x = x;
	g_GamePad.y = y;

	SprObj[enMyShipIndex].x=x;
	SprObj[enMyShipIndex].y=y;

	//------------
	//�V���b�g
	//------------
	if(g_GameSystem.VsyncTimer%4==0){
		ShootSub::SoundPlay(1);
		shot_init(enMyShipIndex,-5,-1);
		shot_init(enMyShipIndex,6,-1);
	}

	//------------
	//�I�v�V����
	//------------
	if((g_GameSystem.option_x[g_GameSystem.option_pos]!=x)||(g_GameSystem.option_y[g_GameSystem.option_pos]!=y)){
		g_GameSystem.option_pos++;
		g_GameSystem.option_pos= g_GameSystem.option_pos%30;
		g_GameSystem.option_x[g_GameSystem.option_pos]=x;
		g_GameSystem.option_y[g_GameSystem.option_pos]=y;
	}
	opt=(g_GameSystem.option_pos+20)%30;
	ShootSub::PutSprite(g_GameSystem.option_x[opt],g_GameSystem.option_y[opt],9,PRIO_JIKI-1,0);

	//�I�v�V�����V���b�g
	if(g_GameSystem.VsyncTimer%4==1){
		shot_init(-1,g_GameSystem.option_x[opt]-x+1,g_GameSystem.option_y[opt]-y+2);
	}

}


void cloud_init(void)
{
	int n;

	n=GetObjNum();
	if(n==-1) return;

	SprObj[n].free.b[0]=2+rand()%4;
	SprObj[n].flag = 7;
	SprObj[n].x = -32+(rand()%20)*6;
	SprObj[n].y = -24*(SprObj[n].free.b[0])*2;
	SprObj[n].vy = (3+rand()%2);//*SprObj[n].free.b[0]/2;
	SprObj[n].vx = 40+rand()%80;

}

void cloud_main(int n)
{
	SprObj[n].y+=SprObj[n].vy;

	ShootSub::PutSpriteVisor(SprObj[n].x,SprObj[n].y,69,20+rand()%10,ATR_ALPHA_NML,ARGB(0x40,0xff,0xff,0xff),SprObj[n].free.b[0]);

	if((SprObj[n].x<=-32)||(SprObj[n].x>=160+32)||(SprObj[n].y>=160+32)){
		kill_obj(n);
		return;
	}

}


//---------------------------------------------------------
//  �V���b�g
//---------------------------------------------------------
static void shot_init(int s,int x, int y)
{
	int n;

	if(s==-1){
		s=enMyShipIndex;
	}else{
		if(g_GameSystem.my_shot_num>=MAX_MY_SHOT) return;
	}

	n=GetObjNum();
	if(n==-1) return;

	SprObj[n].flag = 2;
	SprObj[n].x = SprObj[s].x+x;
	SprObj[n].y = SprObj[s].y+y;

}


static void shot_main(int n)
{
	//-----------------------------
	//�V���b�g���C��
	//-----------------------------

	SprObj[n].y -= 7;

	ShootSub::PutSprite(SprObj[n].x,SprObj[n].y,1,PRIO_TAMA,0);

	if(hit_check(n)){
		kill_obj(n);
	}
	if(SprObj[n].y <= -16){
		kill_obj(n);
	}

}


//-------------
//�G�V���b�g
//-------------
static void ene_shot_init(int s,int type)
{
	//-----------------------------
	//�G�V���b�g�F������
	//-----------------------------
	int n,kaku,i;

	if(g_GameSystem.en_shot_num>=MAX_EN_SHOT) return;

	n=GetObjNum();
	if(n==-1) return;

	SprObj[n].flag = 5;
	SprObj[n].type = type;
	SprObj[n].vx = (SprObj[s].x*100);
	SprObj[n].vy = (SprObj[s].y*100);

	switch(type){
	case 1:		//�͂����݃��[�U�[�e1
		SprObj[n].free.w[0] = 80;
		SprObj[n].free.w[1] = -100;
		break;
	case 2:		//�͂����݃��[�U�[�e2
		SprObj[n].type = 1;
		SprObj[n].free.w[0] = -80;
		SprObj[n].free.w[1] = -100;
		break;
	case 3:		//�W�̎�1
	case 4:		//�W�̎�2
		SprObj[n].flag = 0;
		for(i=0;i<360;i+=18){
			n=GetObjNum();
			if(n==-1) return;
			SprObj[n].flag = 5;
			SprObj[n].type = 0;	/*�ʏ�e������*/
			SprObj[n].vx = (SprObj[s].x*100)+cos_tbl[i]*30;
			SprObj[n].vy = (SprObj[s].y*100)+sin_tbl[i]*30;
			if(type==3){
				kaku=(i+180)%360;
			}else{
				kaku=(i)%360;
			}
			SprObj[n].free.w[0] = sin_tbl[kaku]*2;
			SprObj[n].free.w[1] = cos_tbl[kaku]*2;
		}
		break;
	case 5:		//�ŋ��e
		SprObj[n].vx = (SprObj[s].x*100);
		SprObj[n].vy = (SprObj[s].y*100)+16*100;
		SprObj[n].free.w[0] = 0;
		SprObj[n].free.w[1] = 300;
		break;
	default:	//�m�[�}���e
		if(SprObj[enMyShipIndex].x<SprObj[s].x) kaku = 0;
		if(SprObj[enMyShipIndex].x>=SprObj[s].x) kaku = -45;

		SprObj[n].free.w[0] = cos_tbl[90+kaku+GetRand()%45]*2/3;
		SprObj[n].free.w[1] = sin_tbl[90+kaku+GetRand()%45]*2/3;
		break;
	}
}


static void ene_shot_main(int n)
{
	//-----------------------------
	//�G�V���b�g���C�����ނ킯
	//-----------------------------

	switch(SprObj[n].type){
	case 1:		//�͂����݃��[�U�[
		ene_shot_laser01(n);
		break;
	case 2:		//�͂����݃��[�U�[
		ene_shot_laser01(n);
		break;
	case 5:		//�ŋ��e
		ene_shot_bigblitz(n);
		break;
	default:	//�m�[�}���V���b�g
		ene_shot_normal(n);
		break;
	}

	if(hit_check2(n)){
		//kill_obj(n);
	}

	if( (SprObj[n].x >= 165)||(SprObj[n].x <= -5)
	 ||(SprObj[n].y >= 165)||(SprObj[n].y <= -5) ){
		kill_obj(n);
	}

}

static void ene_shot_normal(int n)
{
	//-----------------------------
	//�m�[�}���e
	//-----------------------------

	SprObj[n].vx += SprObj[n].free.w[0];
	SprObj[n].vy += SprObj[n].free.w[1];

	SprObj[n].x = SprObj[n].vx/100;
	SprObj[n].y = SprObj[n].vy/100;

	ShootSub::PutSprite(SprObj[n].x,SprObj[n].y,6,PRIO_TAMA,0);

}


static void ene_shot_laser01(int n)
{
	//-----------------------------
	//���[�U�[�e
	//-----------------------------

	if(SprObj[n].free.w[0] >0) SprObj[n].free.w[0]-=2;
	if(SprObj[n].free.w[0] <0) SprObj[n].free.w[0]+=2;
	if(SprObj[n].free.w[1] <320) SprObj[n].free.w[1]+=10;

	SprObj[n].vx += SprObj[n].free.w[0];
	SprObj[n].vy += SprObj[n].free.w[1];

	SprObj[n].x = SprObj[n].vx/100;
	SprObj[n].y = SprObj[n].vy/100;

	ShootSub::PutSprite(SprObj[n].x,SprObj[n].y,36,PRIO_TAMA+1,0);

}


static void ene_shot_bigblitz(int n)
{
	//-----------------------------
	//�ŋ��e
	//-----------------------------

	
	if(SprObj[n].free.w[1]>120)
		SprObj[n].free.w[1]-=5;

	SprObj[n].vx += SprObj[n].free.w[0];
	SprObj[n].vy += SprObj[n].free.w[1];

	SprObj[n].x = SprObj[n].vx/100;
	SprObj[n].y = SprObj[n].vy/100;

	ShootSub::PutSprite(SprObj[n].x,SprObj[n].y,37,PRIO_TAMA+2,0);

}


//---------------------------------------------------------
//  �G
//---------------------------------------------------------
static void enemy_main(int n)
{
	//-----------------------------
	//�G���ނ킯
	//-----------------------------

	switch(SprObj[n].type){
	case 0:		//zako
		enemy01_main(n);
		break;
	case 1:		//big_boss�P
		enemy02_main(n);
		break;
	case 2:		//big_boss�Q
		enemy03_main(n);
		break;
	case 3:		//big_boss�R
		enemy04_main(n);
		break;
	case 4:		//�f�X�C���O
		enemy05_main(n);
		break;
	case 5:		//ice block
		enemy06_main(n);
		break;
	case 6:		//zako�S
		enemy04_main(n);
		break;
		break;
	}
}


//--------------
//zako
//--------------
static void enemy01_init(int s)
{
	//-----------------------------
	//zako�F������
	//-----------------------------
	int n;

	if(g_GameSystem.enemy_num>=MAX_ENEMY) return;

	n=GetObjNum();
	if(n==-1) return;

	SprObj[n].hp=3;
	SprObj[n].type=0;
	SprObj[n].flag=3;
	SprObj[n].x=80;
	SprObj[n].vy=-16*10;

	if(SprObj[enMyShipIndex].x<80){
		SprObj[n].vx=50;
		SprObj[n].free.b[0]=-2;
	}else{
		SprObj[n].vx=-50;
		SprObj[n].free.b[0]=2;
	}

}


static void enemy01_main(int n)
{
	//-----------------------------
	//zako�F���C��
	//-----------------------------
	int shoot_timing[]=
	{
		180,90,60,30,240
	};

	SprObj[n].x += SprObj[n].vx/10;
	SprObj[n].vx+=SprObj[n].free.b[0];

	SprObj[n].vy+=12;
	if(GetAbs(SprObj[n].vx)>=50)
		SprObj[n].free.b[0]*=-1;

	SprObj[n].x = SprObj[n].x;
	SprObj[n].y = SprObj[n].vy/10;

	ShootSub::PutSprite(SprObj[n].x,SprObj[n].y,31,PRIO_TEKI,0);

	if(GetRand()%shoot_timing[g_GameSystem.level]==g_GameSystem.VsyncTimer%shoot_timing[g_GameSystem.level]){
		ene_shot_init(n,0);
	}

	if(SprObj[n].hp==0){
		//bomb01_init(n,0,0,0);
		kill_obj(n);
		return;
	}
	if(SprObj[n].y>=176){
		kill_obj(n);
		return;
	}

}


//--------------
//big_boss�P
//--------------
static void enemy02_init(int s)
{
	//-----------------------------
	//big_boss�P�F������
	//-----------------------------
	int n;

	if(g_GameSystem.enemy_num>=MAX_ENEMY) return;

	n=GetObjNum();
	if(n==-1) return;
	SprObj[n].hp=160;
	SprObj[n].type=1;
	SprObj[n].flag=3;
	SprObj[n].x=80;
	SprObj[n].vy=-16*10;
	SprObj[n].free.w[1]=0;
	SprObj[n].free.w[2]=0;

	if(SprObj[enMyShipIndex].x<80){
		SprObj[n].x=110;
		SprObj[n].vx=25;
		SprObj[n].free.b[0]=-1;
	}else{
		SprObj[n].x=50;
		SprObj[n].vx=-25;
		SprObj[n].free.b[0]=1;
	}

}


static void enemy02_main(int n)
{
	//-----------------------------
	//big_boss�P�F���C��
	//-----------------------------

	if(SprObj[n].free.w[2]==0){
		if(SprObj[n].hp==80){
			bomb01_init(n, -18,-18,1);
			bomb01_init(n, -18 ,18,1);
			bomb01_init(n,  18,-18,1);
			bomb01_init(n,  18 ,18,1);
			SprObj[n].free.w[2]=-45;
		}
	}

	if(SprObj[n].y<48){
		//���ɉ���Ă���
		SprObj[n].vy+=4;
	}else{
		//���E�ɐU��
		if(SprObj[n].free.w[2]<-1){
			SprObj[n].free.w[2]++;
		}else{
			SprObj[n].y=48;
			SprObj[n].x += SprObj[n].vx/10;
			SprObj[n].vx+=SprObj[n].free.b[0];
		}
	}

	if(GetAbs(SprObj[n].vx)>=25)
		SprObj[n].free.b[0]*=-1;

	SprObj[n].x = SprObj[n].x;
	SprObj[n].y = SprObj[n].vy/10;

	ShootSub::PutSpriteVisor(SprObj[n].x,SprObj[n].y,32,PRIO_TEKI+2,0,ARGB_DFLT,1);

	SprObj[n].free.w[1]++;
	SprObj[n].free.w[1]=SprObj[n].free.w[1]%280;

	//�U���p�^�[��
	if(SprObj[n].free.w[1]<100){
		//�΂�T���e
		if(g_GameSystem.VsyncTimer%4==0){
			ene_shot_init(n,0);
		}
	}else if(SprObj[n].free.w[1]==200){
		//�W�̎��e1
		ene_shot_init(n,3);
	}else if(SprObj[n].free.w[1]==250){
		//�W�̎��e2
		ene_shot_init(n,4);
	}
	if(g_GameSystem.VsyncTimer%120==0){
		enemy01_init(0);
	}
	if(SprObj[n].hp==0){
		bomb01_init(n, 18 ,0  ,1);
		bomb01_init(n,-18 ,0  ,1);
		bomb01_init(n, 0  ,-18,1);
		bomb01_init(n, 0  , 18,1);
		bomb01_init(n, -18,-18,1);
		bomb01_init(n, -18 ,18,1);
		bomb01_init(n,  18,-18,1);
		bomb01_init(n,  18 ,18,1);
		clear_enemy_shot();
		kill_obj(n);
		g_GameSystem.level++;
		g_GameSystem.Score+=500;
		return;
	}
	if(SprObj[n].y>=176){
		kill_obj(n);
	}

}


//--------------
//big_boss�Q
//--------------
static void enemy03_init(int s)
{
	//-----------------------------
	//big_boss�Q�F������
	//-----------------------------
	int n;

	if(g_GameSystem.enemy_num>=MAX_ENEMY) return;

	n=GetObjNum();
	if(n==-1) return;
	SprObj[n].hp=210;
	SprObj[n].type=2;
	SprObj[n].flag=3;
	SprObj[n].x=80;
	SprObj[n].vy=-16*10;

	SprObj[n].x=80;
	SprObj[n].vx=25;
	SprObj[n].free.b[0]=-1;

}


static void enemy03_main(int n)
{
	//-----------------------------
	//big_boss�Q�F���C��
	//-----------------------------

	if(SprObj[n].y<32){
		//���ɉ���Ă���
		SprObj[n].vy+=4;
	}else{
		//���E�ɐU��
		SprObj[n].x += SprObj[n].vx/10;
		SprObj[n].vx+=SprObj[n].free.b[0];
	}

	if(GetAbs(SprObj[n].vx)>=28)
		SprObj[n].free.b[0]*=-1;

	SprObj[n].x = SprObj[n].x;
	SprObj[n].y = SprObj[n].vy/10;

	ShootSub::PutSprite(SprObj[n].x,SprObj[n].y,32,PRIO_TEKI,0);
	ShootSub::PutSprite(SprObj[n].x-24,SprObj[n].y+8,33,PRIO_TEKI+1,0);	//�E�C���O
	ShootSub::PutSprite(SprObj[n].x+32,SprObj[n].y+8,34,PRIO_TEKI+1,0);	//�E�C���O

	//�͂����݃��[�U�[
	if(g_GameSystem.VsyncTimer%30<15){
		if(g_GameSystem.VsyncTimer%5==0){
			ene_shot_init(n,1);
			ene_shot_init(n,2);
		}
	}
	if(g_GameSystem.VsyncTimer%120==0){
		enemy01_init(0);
	}

	if(SprObj[n].hp==0){
		bomb01_init(n,8,-16,1);
		bomb01_init(n,-8,-16,1);
		bomb01_init(n,0,-16,1);
		bomb01_init(n,0, 16,1);
		bomb01_init(n, -18,-18,1);
		bomb01_init(n, -18 ,18,1);
		bomb01_init(n,  18,-18,1);
		bomb01_init(n,  18 ,18,1);
		g_GameSystem.Score+=1000;
		clear_enemy_shot();
		kill_obj(n);
		g_GameSystem.level++;
		return;
	}
	if(SprObj[n].y>=176){
		kill_obj(n);
	}

}


//--------------
//big_boss�R
//--------------
static void enemy04_init(int s)
{
	//-----------------------------
	//big_boss�R�F������
	//-----------------------------
	int n;

	if(g_GameSystem.enemy_num>=MAX_ENEMY) return;

	n=GetObjNum();
	if(n==-1) return;
	SprObj[n].hp=400;
	SprObj[n].type=3;
	SprObj[n].flag=3;
	SprObj[n].x=80;
	SprObj[n].vy=-16*10;

	SprObj[n].x=80;
	SprObj[n].vx=25;
	SprObj[n].free.b[0]=-1;
	SprObj[n].free.w[1]=0;
	SprObj[n].free.w[2]=0;

}


static void enemy04_main(int n)
{
	//-----------------------------
	//big_boss�R�F���C��
	//-----------------------------

	if(SprObj[n].y<32){
		//���ɉ���Ă���
		SprObj[n].vy+=4;
	}else{
		//���E�ɐU��
		SprObj[n].x += SprObj[n].vx/10;
		SprObj[n].vx+=SprObj[n].free.b[0];
	}

	if(GetAbs(SprObj[n].vx)>=27)
		SprObj[n].free.b[0]*=-1;

	SprObj[n].x = SprObj[n].x;
	SprObj[n].y = SprObj[n].vy/10;

	if((SprObj[n].hp<=350)&&(SprObj[n].free.w[2]==0)){
		bomb01_init(n,0,16,1);
		bomb01_init(n, -18 ,18,1);
		bomb01_init(n,  18,-18,1);
		SprObj[n].free.w[2]=1;
	}else if((SprObj[n].hp<=200)&&(SprObj[n].free.w[2]==1)){
		bomb01_init(n,-24,8,1);
		bomb01_init(n, 32,8,1);
		bomb01_init(n, -18,-18,1);
		bomb01_init(n,  18 ,18,1);
		SprObj[n].free.w[2]=2;
	}
	if(g_GameSystem.VsyncTimer%60==0){
		enemy01_init(0);
	}

	//�_���[�W�ɉ������`�ŕ\��
	ShootSub::PutSprite(SprObj[n].x,SprObj[n].y,32,PRIO_TEKI,0);
	if(SprObj[n].free.w[2]<=1){
		ShootSub::PutSprite(SprObj[n].x-24,SprObj[n].y+8,33,PRIO_TEKI+1,0);	//�E�C���O��
		ShootSub::PutSprite(SprObj[n].x+32,SprObj[n].y+8,34,PRIO_TEKI+1,0);	//�E�C���O�E
	}
	if(SprObj[n].free.w[2]<=0){
		ShootSub::PutSprite(SprObj[n].x,SprObj[n].y+16,35,PRIO_TAMA+2,0);	//�L���m��
	}


	SprObj[n].free.w[1]++;
	SprObj[n].free.w[1]=SprObj[n].free.w[1]%100;

	switch(SprObj[n].free.w[2]){
	case 0:		//�U���p�^�[��1
		if(SprObj[n].free.w[1]%100==70){
			ene_shot_init(n,3);			//�W�̎�
		}
		if(SprObj[n].free.w[1]%100<60){
			if(SprObj[n].free.w[1]%15==0){
				ene_shot_init(n,5);		//�ŋ��e
			}
		}
		break;
	case 1:		//�U���p�^�[��2
		if(g_GameSystem.VsyncTimer%35==20){
			ene_shot_init(n,5);			//�ŋ��e
		}
		if(g_GameSystem.VsyncTimer%35<20){
			if(g_GameSystem.VsyncTimer%5==0){
				ene_shot_init(n,1);		//�͂����݃��[�U�[
				ene_shot_init(n,2);		//�͂����݃��[�U�[
			}
		}
		break;
	case 2:		//�U���p�^�[��3
		if(SprObj[n].free.w[1]==0){
			ene_shot_init(n,3);			//�W�̎�
		}else if(SprObj[n].free.w[1]==50){
			ene_shot_init(n,4);			//�W�̎�
		}
		break;
	default:
		break;
	}

	if(SprObj[n].hp==0){
		bomb01_init(n,8,-16,1);
		bomb01_init(n,-8,-16,1);
		bomb01_init(n,0,-16,1);
		bomb01_init(n,0, 16,1);
		bomb01_init(n, -18,-18,1);
		bomb01_init(n, -18 ,18,1);
		bomb01_init(n,  18,-18,1);
		bomb01_init(n,  18 ,18,1);
		kill_obj(n);
		clear_enemy_shot();
		g_GameSystem.level++;
		g_GameSystem.Score+=2000;
		return;
	}
	if(SprObj[n].y>=176){
		kill_obj(n);
	}

}


static void enemy05_init(int s)
{
	//-----------------------------
	//�f�X�C���O�F������
	//-----------------------------
	int n;

	if(g_GameSystem.enemy_num>=MAX_ENEMY) return;

	n=GetObjNum();
	if(n==-1) return;

	SprObj[n].hp=10;
	SprObj[n].type=4;
	SprObj[n].flag=3;
	SprObj[n].vy=-160;

	SprObj[n].vx=0;
	if(SprObj[enMyShipIndex].x<80){
		SprObj[n].x=80+10+GetRand()%30;
		SprObj[n].free.w[0]=-2;
		SprObj[n].free.w[1]=130+GetRand()%3*10;
	}else{
		SprObj[n].x=80-10-GetRand()%30;
		SprObj[n].free.w[0]=2;
		SprObj[n].free.w[1]=130+GetRand()%30;
	}

}


static void enemy05_main(int n)
{
	//-----------------------------
	//�f�X�C���O�F���C��
	//-----------------------------
	int shoot_timing[]={
		50,30,20,240,30,10,
	};

	if(SprObj[n].free.w[1]>0){
		SprObj[n].free.w[1]-=1;
	}else{
		if(GetAbs(SprObj[n].free.w[1])<=50){
			SprObj[n].vx += SprObj[n].free.w[0];
		}
	}

	SprObj[n].vy += SprObj[n].free.w[1];

	SprObj[n].x += SprObj[n].vx/10;
	SprObj[n].y = SprObj[n].vy/100;
	ShootSub::PutSprite( SprObj[n].x , SprObj[n].y , 53,PRIO_TEKI , 0 );

	if(GetRand()%shoot_timing[g_GameSystem.level]==g_GameSystem.VsyncTimer%shoot_timing[g_GameSystem.level]){
		ene_shot_init(n,0);
	}

	if(SprObj[n].hp==0){
		bomb01_init(n,-16,0,1);
		bomb01_init(n,0,-16,1);
		bomb01_init(n,0,16,1);
		bomb01_init(n,16,0,1);
		kill_obj(n);
		return;
	}
	if((SprObj[n].x<=-32)||(SprObj[n].x>=160+32)){
		kill_obj(n);
		return;
	}

}


static void enemy06_init(int s)
{
	//-----------------------------
	//ice block�F������
	//-----------------------------
	int n,kaku;

	if(g_GameSystem.enemy_num>=MAX_ENEMY) return;

	n=GetObjNum();
	if(n==-1) return;

	SprObj[n].type=5;
	SprObj[n].flag=3;

	if((SprObj[s].flag==3)&&(SprObj[s].free.w[2])){
		//����쐬
		if(SprObj[s].free.w[2]==1){
			//��Q�i�K
			kaku=30+(GetRand()%4)*30;
			SprObj[n].hp=3;
			SprObj[n].free.w[0]=cos_tbl[kaku];
			SprObj[n].free.w[1]=sin_tbl[kaku];
		}else if(SprObj[s].free.w[2]==2){
			//��R�i�K
			kaku=15+GetRand()%150;
			SprObj[n].hp=3;
			SprObj[n].free.w[0]=cos_tbl[kaku]*2/3;
			SprObj[n].free.w[1]=sin_tbl[kaku]*2/3;
		}
		SprObj[n].vx=SprObj[s].x*100;
		SprObj[n].vy=SprObj[s].y*100;
		SprObj[n].free.w[2]=SprObj[s].free.w[2];
		SprObj[n].free.w[3]=GetRand()%2;
	}else{
		//��P�i�K
		SprObj[n].free.w[2]=0;
		SprObj[n].hp=5;
		SprObj[n].vy=-180;
		SprObj[n].vx=(40+GetRand()%80)*100;
		kaku=GetRand()%30;
		if(SprObj[n].vx<80){
			SprObj[n].free.w[0]=cos_tbl[90-kaku]*2/3;
			SprObj[n].free.w[1]=sin_tbl[90-kaku]*2/3;
		}else{
			SprObj[n].free.w[0]=cos_tbl[90+kaku]*2/3;
			SprObj[n].free.w[1]=sin_tbl[90+kaku]*2/3;
		}
	}

}


static void enemy06_main(int n)
{
	//-----------------------------
	//ice block�F���C��
	//-----------------------------
	int i,ptn;

	SprObj[n].vx+=SprObj[n].free.w[0];
	SprObj[n].vy+=SprObj[n].free.w[1];

	SprObj[n].x = SprObj[n].vx/100;
	SprObj[n].y = SprObj[n].vy/100;

	SprObj[n].free.w[3]+=GetRand()%3;
	if(SprObj[n].free.w[3]%20<10){
		ptn=0;
	}else{
		ptn=1;
	}

	if(SprObj[n].free.w[2]==0){
		ShootSub::PutSprite(SprObj[n].x,SprObj[n].y,60,PRIO_TEKI+4,0);
	}else if(SprObj[n].free.w[2]==1){
		ShootSub::PutSprite(SprObj[n].x,SprObj[n].y,61+ptn,PRIO_TEKI+4,0);
	}else{
		ShootSub::PutSprite(SprObj[n].x,SprObj[n].y,63+ptn,PRIO_TEKI+4,0);
	}

	if(SprObj[n].hp<=0){
		if(SprObj[n].free.w[2]==2){
			bomb01_init(n,0,0,0);
			kill_obj(n);
			return;
		}
		SprObj[n].free.w[2]++;
		for(i=0;i<SprObj[n].free.w[2]*2;i++){
			enemy06_init(n);
		}
		kill_obj(n);
		ShootSub::SoundPlay(3);
		return;
	}

	if((SprObj[n].x<=-32)||(SprObj[n].x>=160+32)||(SprObj[n].y>=160+32)){
		kill_obj(n);
		return;
	}

}


//---------------------------------------------------------
//  ����
//---------------------------------------------------------
static void clear_enemy_shot(void)
{
	//--------------
	//�G�̒e�S����
	//--------------
	int i;

	for(i=0;i<enObjMax;i++){
		if(SprObj[i].flag==5){
			eff_star_init(i,0,0);
			kill_obj(i);
		}
	}


}


static void eff_star_init(int s,int x, int y)
{
	//--------------
	//���炫��F������
	//--------------
	int n;

	n=GetObjNum();
	if(n==-1) return;

	SprObj[n].flag=6;

	SprObj[n].free.b[0]=0;
	SprObj[n].x = SprObj[s].x+x;
	SprObj[n].y = SprObj[s].y+y;

}


static void eff_star_main(int n)
{
	//--------------
	//���炫��F���C��
	//--------------
	int spr=55;

	SprObj[n].free.b[0]++;

	spr+=(SprObj[n].free.b[0]/3);
	if(spr>=60){
		kill_obj(n);
		return;
	}

	SprObj[n].y--;
	ShootSub::PutSpriteVisor(SprObj[n].x,SprObj[n].y,spr,PRIO_EFF,GBATR_ALPHA_PLUS);
}

static void bomb01_init(int s,int x, int y,int type)
{
	//--------------
	//�����F������
	//--------------
	int n;

	if(s==-1){
		//�����I�ɔ����p�^�[�����o���B
		n=1;
		s=enMyShipIndex;
		gxUtil::MemSet(&SprObj[1],0,sizeof(StObj));
	}else{
		//if(g_GameSystem.en_shot_num>=MAX_EN_SHOT) return;
		n=GetObjNum();
		if(n==-1) return;
	}

	SprObj[n].flag=4;
	SprObj[n].free.b[0]=0;
	SprObj[n].type = type;
	SprObj[n].x = SprObj[s].x+x;
	SprObj[n].y = SprObj[s].y+y;
	if(type==0)
	{
		ShootSub::SoundPlay(2);
	}
	else
	{
		ShootSub::SoundPlay(3);
	}

}


static void bomb01_main(int n)
{
	//--------------
	//�����F���C��
	//--------------
	int s,spr=5;
	int col;
	s=SprObj[n].free.b[0]++;

	if(SprObj[n].type==0){
		//����A
		if(s<2){
			spr = 38;
		}else if(s<4){
			spr = 39;
		}else if(s<6){
			spr = 40;
		}else if(s<8){
			spr = 2;
		} else if(s<16){
			spr = 3;
		} else if(s<24){
			spr = 4;
		}else{
			kill_obj(n);
			return;
		}
		ShootSub::PutSpriteVisor(SprObj[n].x+(-1+GetRand()%2),SprObj[n].y+(-1+GetRand()%2)-s,spr,PRIO_EFF,GBATR_ALPHA_PLUS,0x80ffffff);
	}else{
		//����B
		col=0xff*(25-s)/25;
		if(s<6){
			spr = 38;
			ShootSub::PutSpriteVisor(SprObj[n].x,SprObj[n].y,spr,PRIO_EFF,GBATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff));
		} else if(s<12){
			spr = 39;
			ShootSub::PutSpriteVisor(SprObj[n].x,SprObj[n].y,spr,PRIO_EFF,GBATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff));
		} else if(s<18){
			spr = 40;
			ShootSub::PutSpriteVisor(SprObj[n].x,SprObj[n].y,spr,PRIO_EFF,GBATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff));
		} else if(s<24){
			spr = 41;
			ShootSub::PutSpriteVisor(SprObj[n].x,SprObj[n].y,spr,PRIO_EFF,GBATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff));
		}else{
			kill_obj(n);
			return;
		}
	}

}


//---------------------------------
//�Q�[���ŗL�̃T�u���[�`��
//---------------------------------
static int GetObjNum()
{
	//------------------------------------
	//�����Ă���I�u�W�F�N�g�i���o�[��Ԃ�
	//------------------------------------
	int i;

	for(i=2;i<enObjMax;i++){
		if(SprObj[i].flag==0){
			gxUtil::MemSet(&SprObj[i],0,sizeof(StObj));
			return i;
		}
	}

	return -1;

}


static void kill_obj(int n)
{
	//-----------------
	//�I�u�W�F�N�g�폜
	//-----------------

	SprObj[n].flag=0;
}


static int hit_check(int n)
{
	//-----------------
	//�����蔻�茟�o
	//-----------------
	int i;

	//��ʊO
	if(SprObj[n].y<=8) return 0;

	for(i=0;i<enObjMax;i++){
		if(SprObj[i].flag==0) continue;
		if(i==n) continue;
		if(SprObj[i].flag!=3) continue;
		if(GetAbs(SprObj[i].x-SprObj[n].x)<=16){
			if(GetAbs(SprObj[i].y-SprObj[n].y)<=16){
				//HIT���o
				if((SprObj[i].type>=1)&&(SprObj[i].type<=3)){
					bomb01_init(n,0,0,0);
				}else if(SprObj[i].type==5){
					eff_star_init(n,0,0);
				}else{
					bomb01_init(i,0,0,0);
				}
				SprObj[i].hp--;
				g_GameSystem.Score+=1;
				kill_obj(n);
				if(SprObj[i].hp<=0){
					SprObj[i].hp=0;
				}
				return 1;
			}
		}
	}

	return 0;
}


static int hit_check2(int n)
{
	//-----------------------------
	//�G�e���������̂����蔻�茟�o
	//-----------------------------
	int i;

	i = enMyShipIndex;

	if(SprObj[i].type!=3) return 0;

	if(GetAbs(SprObj[i].x-SprObj[n].x)<=2)
	{
		if(GetAbs(SprObj[i].y-SprObj[n].y)<=3)
		{
			//HIT���o
			//clear_enemy_shot();
			bomb01_init(-1,0,0 ,1);
			kill_obj(i);
			return 1;
		}
	}

	return 0;
}


static int GetStylus(int *x, int *y)
{
	//-----------------------------
	//�X�^�C���X�̏��𓾂�
	//-----------------------------
	static gxPos m_Old,m_Joy;
	Sint32 mx,my;

	mx = gxLib::Joy(0)->mx;
	my = gxLib::Joy(0)->my;

	if( gxLib::Joy(0)->trg&MOUSE_L )
	{
		m_Old.x = *x;
		m_Old.y = *y;
		m_Joy.x = mx;
		m_Joy.y = my;
	}

	if( gxLib::Joy(0)->psh&MOUSE_L )
	{
		Sint32 ax,ay;
		ax = (mx - m_Joy.x)/2;
		ay = (my - m_Joy.y)/2;

		ax = 5*ax/4;
		ay = 5*ay/4;
		*x = (m_Old.x + ax);
		*y = (m_Old.y + ay);
	}

	if( gxLib::Joy(0)->psh&MOUSE_L )
	{
		return 1;
	}

	return 0;

}


static int GetAbs(int n)
{
	//-----------------------------
	//��Βl�����߂�
	//-----------------------------
	return  (n<0)? n*-1 : n;
}


int GetRand(void)
{
	//-----------------------------
	//�����_���Ȑ��������o���B
	//-----------------------------

	return rand()%10000;

}


static void padControl( void )
{
	//-----------------------------
	//���t���[���̃p�b�h�����擾
	//-----------------------------

	g_GamePad.keydown = GetStylus( &g_GamePad.x , &g_GamePad.y );

}


//-------------------------------
//SIN�e�[�u��
//�E���O�x�Ƃ��ĉE���ɂR�U�O��
//-------------------------------
char sin_tbl[]={
	   0,	   1,	   3,	   5,	   6,	   8,	  10,	  12,	  13,	  15,
	  17,	  19,	  20,	  22,	  24,	  25,	  27,	  29,	  30,	  32,
	  34,	  35,	  37,	  39,	  40,	  42,	  43,	  45,	  46,	  48,
	  50,	  51,	  52,	  54,	  55,	  57,	  58,	  60,	  61,	  62,
	  64,	  65,	  66,	  68,	  69,	  70,	  71,	  73,	  74,	  75,
	  76,	  77,	  78,	  79,	  80,	  81,	  82,	  83,	  84,	  85,
	  86,	  87,	  88,	  89,	  89,	  90,	  91,	  92,	  92,	  93,
	  93,	  94,	  95,	  95,	  96,	  96,	  97,	  97,	  97,	  98,
	  98,	  98,	  99,	  99,	  99,	  99,	  99,	  99,	  99,	  99,
	  99,	  99,	  99,	  99,	  99,	  99,	  99,	  99,	  99,	  98,
	  98,	  98,	  97,	  97,	  97,	  96,	  96,	  95,	  95,	  94,
	  93,	  93,	  92,	  92,	  91,	  90,	  89,	  89,	  88,	  87,
	  86,	  85,	  84,	  83,	  82,	  81,	  80,	  79,	  78,	  77,
	  76,	  75,	  74,	  73,	  71,	  70,	  69,	  68,	  66,	  65,
	  64,	  62,	  61,	  60,	  58,	  57,	  55,	  54,	  52,	  51,
	  49,	  48,	  46,	  45,	  43,	  42,	  40,	  39,	  37,	  35,
	  34,	  32,	  30,	  29,	  27,	  25,	  24,	  22,	  20,	  19,
	  17,	  15,	  13,	  12,	  10,	   8,	   6,	   5,	   3,	   1,
	   0,	  -1,	  -3,	  -5,	  -6,	  -8,	 -10,	 -12,	 -13,	 -15,
	 -17,	 -19,	 -20,	 -22,	 -24,	 -25,	 -27,	 -29,	 -30,	 -32,
	 -34,	 -35,	 -37,	 -39,	 -40,	 -42,	 -43,	 -45,	 -46,	 -48,
	 -50,	 -51,	 -52,	 -54,	 -55,	 -57,	 -58,	 -60,	 -61,	 -62,
	 -64,	 -65,	 -66,	 -68,	 -69,	 -70,	 -71,	 -73,	 -74,	 -75,
	 -76,	 -77,	 -78,	 -79,	 -80,	 -81,	 -82,	 -83,	 -84,	 -85,
	 -86,	 -87,	 -88,	 -89,	 -89,	 -90,	 -91,	 -92,	 -92,	 -93,
	 -93,	 -94,	 -95,	 -95,	 -96,	 -96,	 -97,	 -97,	 -97,	 -98,
	 -98,	 -98,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,
	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -98,
	 -98,	 -98,	 -97,	 -97,	 -97,	 -96,	 -96,	 -95,	 -95,	 -94,
	 -93,	 -93,	 -92,	 -92,	 -91,	 -90,	 -89,	 -89,	 -88,	 -87,
	 -86,	 -85,	 -84,	 -83,	 -82,	 -81,	 -80,	 -79,	 -78,	 -77,
	 -76,	 -75,	 -74,	 -73,	 -71,	 -70,	 -69,	 -68,	 -66,	 -65,
	 -64,	 -62,	 -61,	 -60,	 -58,	 -57,	 -55,	 -54,	 -52,	 -51,
	 -49,	 -48,	 -46,	 -45,	 -43,	 -42,	 -40,	 -39,	 -37,	 -35,
	 -34,	 -32,	 -30,	 -29,	 -27,	 -25,	 -24,	 -22,	 -20,	 -19,
	 -17,	 -15,	 -13,	 -12,	 -10,	  -8,	  -6,	  -5,	  -3,	  -1,
};

//-------------------------------
//COS�e�[�u��
//�E���O�x�Ƃ��ĉE���ɂR�U�O��
//-------------------------------
char cos_tbl[]={
	 100,	  99,	  99,	  99,	  99,	  99,	  99,	  99,	  99,	  98,
	  98,	  98,	  97,	  97,	  97,	  96,	  96,	  95,	  95,	  94,
	  93,	  93,	  92,	  92,	  91,	  90,	  89,	  89,	  88,	  87,
	  86,	  85,	  84,	  83,	  82,	  81,	  80,	  79,	  78,	  77,
	  76,	  75,	  74,	  73,	  71,	  70,	  69,	  68,	  66,	  65,
	  64,	  62,	  61,	  60,	  58,	  57,	  55,	  54,	  52,	  51,
	  49,	  48,	  46,	  45,	  43,	  42,	  40,	  39,	  37,	  35,
	  34,	  32,	  30,	  29,	  27,	  25,	  24,	  22,	  20,	  19,
	  17,	  15,	  13,	  12,	  10,	   8,	   6,	   5,	   3,	   1,
	   0,	  -1,	  -3,	  -5,	  -6,	  -8,	 -10,	 -12,	 -13,	 -15,
	 -17,	 -19,	 -20,	 -22,	 -24,	 -25,	 -27,	 -29,	 -30,	 -32,
	 -34,	 -35,	 -37,	 -39,	 -40,	 -42,	 -43,	 -45,	 -46,	 -48,
	 -50,	 -51,	 -52,	 -54,	 -55,	 -57,	 -58,	 -60,	 -61,	 -62,
	 -64,	 -65,	 -66,	 -68,	 -69,	 -70,	 -71,	 -73,	 -74,	 -75,
	 -76,	 -77,	 -78,	 -79,	 -80,	 -81,	 -82,	 -83,	 -84,	 -85,
	 -86,	 -87,	 -88,	 -89,	 -89,	 -90,	 -91,	 -92,	 -92,	 -93,
	 -93,	 -94,	 -95,	 -95,	 -96,	 -96,	 -97,	 -97,	 -97,	 -98,
	 -98,	 -98,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,
	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -99,	 -98,
	 -98,	 -98,	 -97,	 -97,	 -97,	 -96,	 -96,	 -95,	 -95,	 -94,
	 -93,	 -93,	 -92,	 -92,	 -91,	 -90,	 -89,	 -89,	 -88,	 -87,
	 -86,	 -85,	 -84,	 -83,	 -82,	 -81,	 -80,	 -79,	 -78,	 -77,
	 -76,	 -75,	 -74,	 -73,	 -71,	 -70,	 -69,	 -68,	 -66,	 -65,
	 -64,	 -62,	 -61,	 -60,	 -58,	 -57,	 -55,	 -54,	 -52,	 -51,
	 -49,	 -48,	 -46,	 -45,	 -43,	 -42,	 -40,	 -39,	 -37,	 -35,
	 -34,	 -32,	 -30,	 -29,	 -27,	 -25,	 -24,	 -22,	 -20,	 -19,
	 -17,	 -15,	 -13,	 -12,	 -10,	  -8,	  -6,	  -5,	  -3,	  -1,
	   0,	   1,	   3,	   5,	   6,	   8,	  10,	  12,	  13,	  15,
	  17,	  19,	  20,	  22,	  24,	  25,	  27,	  29,	  30,	  32,
	  34,	  35,	  37,	  39,	  40,	  42,	  43,	  45,	  46,	  48,
	  50,	  51,	  52,	  54,	  55,	  57,	  58,	  60,	  61,	  62,
	  64,	  65,	  66,	  68,	  69,	  70,	  71,	  73,	  74,	  75,
	  76,	  77,	  78,	  79,	  80,	  81,	  82,	  83,	  84,	  85,
	  86,	  87,	  88,	  89,	  89,	  90,	  91,	  92,	  92,	  93,
	  93,	  94,	  95,	  95,	  96,	  96,	  97,	  97,	  97,	  98,
	  98,	  98,	  99,	  99,	  99,	  99,	  99,	  99,	  99,	  99,
};

//-------------------------
//�X�v���C�g��`�e�[�u��
//-------------------------
StSpriteDef SprVisor[] = {
	//�X�v���C�g��`�e�[�u��
	{enTpgSpr,0,0,2,2, 8,8},	//0 visoriser
	{enTpgSpr,2,0,1,1, 4,4},	//1 visoriser�V���b�g
	{enTpgSpr,4,0,2,2, 8,8},	//2 ����A_0
	{enTpgSpr,6,0,2,2, 8,8},	//3 ����A_1
	{enTpgSpr,8,0,2,2, 8,8},	//4 ����A_2
	{enTpgSpr,3,0,1,2, 4,0},	//5 visoriser���[�U�[

	{enTpgSpr,2,1,1,1, 4,4},	//6 �G�V���b�g
	{enTpgSpr,2,3,1,1, 4,4},	//7 visoriser���[�U�[�q�b�g1
	{enTpgSpr,3,3,1,1, 4,4},	//8 visoriser���[�U�[�q�b�g2

	{enTpgSpr,0,2,2,2, 8,8},	//9 option

	//�n�`
	{enTpgSpr,15,0,5,1, 0,0},	//10 �o�b�N�O���E���h0
	{enTpgSpr,15,1,5,1, 0,0},	//11 �o�b�N�O���E���h1
	{enTpgSpr,15,2,5,1, 0,0},	//12 �o�b�N�O���E���h2
	{enTpgSpr,15,3,5,1, 0,0},	//13 �o�b�N�O���E���h3
	{enTpgSpr,15,4,5,1, 0,0},	//14 �o�b�N�O���E���h4
	{enTpgSpr,15,5,5,1, 0,0},	//15 �o�b�N�O���E���h5
	{enTpgSpr,15,6,5,1, 0,0},	//16 �o�b�N�O���E���h6
	{enTpgSpr,15,7,5,1, 0,0},	//17 �o�b�N�O���E���h7
	{enTpgSpr,15,8,5,1, 0,0},	//18 �o�b�N�O���E���h8
	{enTpgSpr,15,9,5,1, 0,0},	//19 �o�b�N�O���E���h9
	{enTpgSpr,15,10,5,1, 0,0},	//20 �o�b�N�O���E���h10
	{enTpgSpr,15,11,5,1, 0,0},	//21 �o�b�N�O���E���h11
	{enTpgSpr,15,12,5,1, 0,0},	//22 �o�b�N�O���E���h12
	{enTpgSpr,15,13,5,1, 0,0},	//23 �o�b�N�O���E���h13
	{enTpgSpr,15,14,5,1, 0,0},	//24 �o�b�N�O���E���h14
	{enTpgSpr,15,15,5,1, 0,0},	//25 �o�b�N�O���E���h15
	{enTpgSpr,15,16,5,1, 0,0},	//26 �o�b�N�O���E���h16
	{enTpgSpr,15,17,5,1, 0,0},	//27 �o�b�N�O���E���h17
	{enTpgSpr,15,18,5,1, 0,0},	//28 �o�b�N�O���E���h18
	{enTpgSpr,15,19,5,1, 0,0},	//29 �o�b�N�O���E���h19
	{enTpgSpr,10,0,5,1, 0,0},	//30 �o�b�N�O���E���h20

	{enTpgSpr,4,4,2,2, 8,8},	//31 �G�U�R zako
	{enTpgSpr,0,4,4,4, 16,16},	//32 �G�{�X big_boss

	{enTpgSpr,6,4,3,2, 16,16},	//33 �G�{�X big_boss�p�I�v�V�����E�C���O��
	{enTpgSpr,6,6,3,2, 16,16},	//34 �G�{�X big_boss�p�I�v�V�����E�C���O�E
	{enTpgSpr,4,6,2,2, 8,8},	//35 �G�{�X big_boss�p�I�v�V�����L���m��

	{enTpgSpr,9,4,1,2, 4,4},	//36 �G   ���[�U�[�e
	{enTpgSpr,9,6,2,2, 8,8},	//37 �G   �ŋ��e

	{enTpgSpr,0 ,8,4,4, 16,16},	//38 ����B_0
	{enTpgSpr,4 ,8,4,4, 16,16},	//39 ����B_1
	{enTpgSpr,8 ,8,4,4, 16,16},	//40 ����B_2
	{enTpgSpr,11,4,4,4, 16,16},	//41 ����B_3

	{enTpgSpr,2,2,1,1, 0,0},	//42 �����O
	{enTpgSpr,3,2,1,1, 0,0},	//43 �����P
	{enTpgSpr,4,2,1,1, 0,0},	//44 �����Q
	{enTpgSpr,5,2,1,1, 0,0},	//45 �����R
	{enTpgSpr,6,2,1,1, 0,0},	//46 �����S
	{enTpgSpr,7,2,1,1, 0,0},	//47 �����T
	{enTpgSpr,8,2,1,1, 0,0},	//48 �����U
	{enTpgSpr,9,2,1,1, 0,0},	//49 �����V
	{enTpgSpr,10,2,1,1, 0,0},	//50 �����W
	{enTpgSpr,11,2,1,1, 0,0},	//51 �����X
	{enTpgSpr,12,2,2,1, 0,0},	//52 ����00

	{enTpgSpr,12,8,3,3, 12,12},	//53 mid Boss
	{enTpgSpr,4,3,1,1, 0,0},	//54 �c�@�\��

	{enTpgSpr,5,14,1,1, 0,0},	//55 ���炫��
	{enTpgSpr,6,14,1,1, 0,0},	//56 ���炫��
	{enTpgSpr,7,14,1,1, 0,0},	//57 ���炫��
	{enTpgSpr,8,14,1,1, 0,0},	//58 ���炫��
	{enTpgSpr,9,14,1,1, 0,0},	//59 ���炫��

	{enTpgSpr,0,12,3,3, 12,12},	//60 ice block
	{enTpgSpr,3,12,2,2, 8,8},	//61 ice block�i���P�j
	{enTpgSpr,5,12,2,2, 8,8},	//62 ice block�i���Q�j
	{enTpgSpr,3,14,1,1, 4,4},	//63 ice block�i���P�j
	{enTpgSpr,4,14,1,1, 4,4},	//64 ice block�i���Q�j

	{enTpgSpr,0,15,10,1, 0,0},	//65 �Q�[���I�[�o�[
	{enTpgSpr,0,17,11,2, 0,0},	//66 VISORIZER

	{enTpgSpr,0,16,9,1, 0,0},	//67 HISCORE
	{enTpgSpr,2,16,7,1, 0,0},	//68 SCORE

	{enTpgSpr,11,12,4,2, 0,0},	//69 SCORE

};

