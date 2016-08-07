//----------------------------------------------
//ＰＡＬＭ ＳＨＯＯＴＥＲ
//shooting.c
//
//----------------------------------------------

//----------------------------------------------
//インクルード
//----------------------------------------------
#include <gxLib.h>
#include "visorizer.h"

//----------------------------------------------
//マクロ
//----------------------------------------------
#define enObjMax 		(90)		//マックスオブジェクト数
#define enMyShipIndex 	(0)			//visoriserのオブジェクトナンバー
#define MAX_MY_SHOT 	(25)		//弾数上限
#define MAX_EN_SHOT 	(35)		//敵の弾数上限
#define MAX_ENEMY   	(15)		//敵数上限
#define enTpgSpr    	(0)

#define PRIO_GROUND	(5)
#define PRIO_JIKI	(10)
#define PRIO_TEKI	(15)
#define PRIO_TAMA	(20)
#define PRIO_EFF	(25)
#define PRIO_SYS	(30)

void VisorizerInit(void);

static int GetAbs(int n);					//絶対値を求める
static int GetRand(void);					//ランダムを作る
static int GetStylus(int *x, int *y);
static void padControl(void);

#define GBATR_ALPHA_PLUS ATR_DFLT

//----------------------------------------------
//型宣言
//----------------------------------------------
//ソフトウェアスプライト定義用構造体

//仮想パッドを扱う構造体
typedef struct StGamePad {
	char psh;	//押しっぱなし
	char trg;	//ワンプッシュ
	int x,y;
	int keydown;
} StGamePad;

//オブジェクト構造体
typedef struct StObj
{
	char flag;		//生存フラグ
	short x,y;		//Ｘ、Ｙ
	short vx,vy;	//増分値など
	char type;		//種類
	short hp;		//耐久力
	//汎用の共用体
	union FREE
	{
		Sint8 b[8];
		Sint16 w[4];
		Sint32 l[2];
	} free ;
} StObj;

//ゲーム全般を扱う構造体
typedef struct StGame
{
	int x;
	char scene;
	int Score;			//スコア
	//オブジェ数記録用
	char my_shot_num;	//弾数
	char en_shot_num;	//敵弾数
	char enemy_num;		//敵数
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
//グローバル変数
//----------------------------------------------
static StObj SprObj[ enObjMax ];	//オブジェクト管理用配列
static StGame g_GameSystem;			//ゲーム管理用
static StGamePad  g_GamePad;		//パッド入力管理用
static int g_sGameSeq = 0;
static int g_HighScore,Score_temp;

//--------------------------------
//関数プロトタイプ
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
extern char sin_tbl[];	//サインテーブル
extern char cos_tbl[];	//コサインテーブル

gxBool m_bWarning = gxFalse;	//毒
Sint32 g_sFirst=0;
//---------------------------------------------------------
//ゲームメイン
//---------------------------------------------------------
void Visorizer(void)
{
	//-----------------------------
	//メインコントローラー
	//-----------------------------
	int lev;

	if(g_sFirst==0)
	{
		//--------------------------------
		//１度だけ通る初期化
		//--------------------------------
		g_sFirst++;
		VisorizerInit();
	}

	padControl();

	if( g_GameSystem.scene >= 2 )
	{
		//ゲーム終了（タイトル）へ
		game_result();
		return;
	}

	lev = g_GameSystem.level;

	background();			//背景
	ctrl_enMyShipIndex();	//自機
	enemy_set();			//敵の出現順番を管理
	SprObj_control();		//enObjMax個のオブジェを動かす

	if( lev != g_GameSystem.level )
	{
		//ゲームレベルがオブジェを動かす前と、
		//後で変わってたら面クリアとして、タイマーをリセット

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
//初期化
//---------------------------------------------------------
void VisorizerInit(void)
{
	//ゲーム初期化

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
	//ゲーム結果＆タイトル
	short px,py;

	//ハイスコア更新
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

	ShootSub::PutSprite(32,50,66,PRIO_SYS,0);		//タイトル

	ShootSub::PutSprite(48,80,67,PRIO_SYS,0);		//ハイスコア
	score_disp(g_HighScore,80,95);		//ハイスコア

	ShootSub::PutSprite(56,110,68,PRIO_SYS,0);	//スコア
	score_disp(Score_temp,80,125);		//スコア

	g_GameSystem.enemy_num=0;
	if(g_GameSystem.VsyncTimer%60==GetRand()%60)
	{
		enemy01_init(0);
	}

	SprObj_control();	//enObjMax個のオブジェを動かす
}


static void SprObj_control(void)
{
	//-----------------------------
	//オブジェクトコントロール
	//-----------------------------
	int i;
	int e_sht,m_sht,ene;

	e_sht=m_sht=ene=0;

	for(i=0;i<enObjMax;i++){
		switch(SprObj[i].flag){
		case 0:	//なにもなし
			break;
		case 1:	//自分
			break;
		case 2:	//たま
			m_sht++;
			shot_main(i);
			break;
		case 3:	//てき
			ene++;
			enemy_main(i);
			break;
		case 4:	//爆発
			bomb01_main(i);
			break;
		case 5:	//敵の弾
			e_sht++;
			ene_shot_main(i);
			break;
		case 6:	//効果
			e_sht++;
			eff_star_main(i);
			break;
		case 7:	//雲
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
	//面制御
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
		//クリア
		break;
	default:
		break;
	}
}

static void enemy_set_lv0(void)
{
	//-------------
	//１面
	//-------------
	int timer;

	timer=g_GameSystem.timer;
	if(g_GameSystem.VsyncTimer%60==0){
		g_GameSystem.timer++;
	}

	//zako
	if(timer>1&&timer<=10){
		if(g_GameSystem.VsyncTimer%300==225){
			//時々Ｇスイング
			enemy05_init(0);
		}
		if(g_GameSystem.VsyncTimer%10==0){
			enemy01_init(0);
		}
	}

	//Ｇスイング
	if(timer>10&&timer<=20){
		if(g_GameSystem.VsyncTimer%40==0){
			enemy05_init(0);
		}
	}

	//Ｇスイング＆zako
	if(timer>20&&timer<=25){
		if(g_GameSystem.VsyncTimer%5==0){
			enemy01_init(0);
		}
		if(g_GameSystem.VsyncTimer%60==0){
			enemy05_init(0);
		}
	}

	//ボス
	if(timer==30){
 		if(g_GameSystem.VsyncTimer%60==10){
			enemy02_init(0);
		}
	}

}


static void enemy_set_lv1(void)
{
	//-------------
	//２面
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

	//Ｇスイング＆zako
	if(timer>10&&timer<=25){
		if(g_GameSystem.VsyncTimer%15==60){
			enemy01_init(0);
		}
		if(g_GameSystem.VsyncTimer%45==0){
			enemy06_init(0);
		}
	}

	//zako＆ice block
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

	//ボス
	if(timer==30){
		if(g_GameSystem.VsyncTimer%60==10){
			enemy03_init(0);
		}
	}

}


static void enemy_set_lv2(void)
{
	//-------------
	//３面
	//-------------
	int timer;

	if(g_GameSystem.VsyncTimer%60==0){
		g_GameSystem.timer++;
	}
	timer=g_GameSystem.timer;

	//Ｇスイング＆ice block＆zako
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

	//Ｇスイング＆zako
	if(timer>10&&timer<=28){
		if(g_GameSystem.VsyncTimer%10==0){
			enemy01_init(0);
		}
		if(g_GameSystem.VsyncTimer%120==0){
			enemy05_init(0);
		}
	}

	//ボス
	if(timer==30){
		if(g_GameSystem.VsyncTimer%60==10){
			enemy04_init(0);
		}
	}

}


static void background(void)
{
	//-----------------------------
	//背景：スクロール
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

	//スコア＆残機
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

	//ＢＧカラーの変更
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
	//スコア表示
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
	//マイシップコントロール
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
			//残機あれば
			g_GameSystem.bb_remain--;
			SprObj[enMyShipIndex].flag=1;
			SprObj[enMyShipIndex].type=1;
			x=SprObj[enMyShipIndex].x=80;
			for(i=0;i<30;i++)
			{
				//オプション位置初期化
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
				//ワープしないための補正
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

	//パッド操作
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
			//ゲームオーバー表示
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
		//無敵時間
		SprObj[enMyShipIndex].free.b[0]--;
		if(SprObj[enMyShipIndex].free.b[0]==5){
			clear_enemy_shot();
		}
	}else{
		SprObj[enMyShipIndex].type=3;
	}

	//画面外補正
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
		//クリア
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
	//ショット
	//------------
	if(g_GameSystem.VsyncTimer%4==0){
		ShootSub::SoundPlay(1);
		shot_init(enMyShipIndex,-5,-1);
		shot_init(enMyShipIndex,6,-1);
	}

	//------------
	//オプション
	//------------
	if((g_GameSystem.option_x[g_GameSystem.option_pos]!=x)||(g_GameSystem.option_y[g_GameSystem.option_pos]!=y)){
		g_GameSystem.option_pos++;
		g_GameSystem.option_pos= g_GameSystem.option_pos%30;
		g_GameSystem.option_x[g_GameSystem.option_pos]=x;
		g_GameSystem.option_y[g_GameSystem.option_pos]=y;
	}
	opt=(g_GameSystem.option_pos+20)%30;
	ShootSub::PutSprite(g_GameSystem.option_x[opt],g_GameSystem.option_y[opt],9,PRIO_JIKI-1,0);

	//オプションショット
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
//  ショット
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
	//ショットメイン
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
//敵ショット
//-------------
static void ene_shot_init(int s,int type)
{
	//-----------------------------
	//敵ショット：初期化
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
	case 1:		//囲い込みレーザー弾1
		SprObj[n].free.w[0] = 80;
		SprObj[n].free.w[1] = -100;
		break;
	case 2:		//囲い込みレーザー弾2
		SprObj[n].type = 1;
		SprObj[n].free.w[0] = -80;
		SprObj[n].free.w[1] = -100;
		break;
	case 3:		//８の字1
	case 4:		//８の字2
		SprObj[n].flag = 0;
		for(i=0;i<360;i+=18){
			n=GetObjNum();
			if(n==-1) return;
			SprObj[n].flag = 5;
			SprObj[n].type = 0;	/*通常弾あつかい*/
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
	case 5:		//最強弾
		SprObj[n].vx = (SprObj[s].x*100);
		SprObj[n].vy = (SprObj[s].y*100)+16*100;
		SprObj[n].free.w[0] = 0;
		SprObj[n].free.w[1] = 300;
		break;
	default:	//ノーマル弾
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
	//敵ショットメイン分類わけ
	//-----------------------------

	switch(SprObj[n].type){
	case 1:		//囲い込みレーザー
		ene_shot_laser01(n);
		break;
	case 2:		//囲い込みレーザー
		ene_shot_laser01(n);
		break;
	case 5:		//最強弾
		ene_shot_bigblitz(n);
		break;
	default:	//ノーマルショット
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
	//ノーマル弾
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
	//レーザー弾
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
	//最強弾
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
//  敵
//---------------------------------------------------------
static void enemy_main(int n)
{
	//-----------------------------
	//敵分類わけ
	//-----------------------------

	switch(SprObj[n].type){
	case 0:		//zako
		enemy01_main(n);
		break;
	case 1:		//big_boss１
		enemy02_main(n);
		break;
	case 2:		//big_boss２
		enemy03_main(n);
		break;
	case 3:		//big_boss３
		enemy04_main(n);
		break;
	case 4:		//Ｇスイング
		enemy05_main(n);
		break;
	case 5:		//ice block
		enemy06_main(n);
		break;
	case 6:		//zako４
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
	//zako：初期化
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
	//zako：メイン
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
//big_boss１
//--------------
static void enemy02_init(int s)
{
	//-----------------------------
	//big_boss１：初期化
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
	//big_boss１：メイン
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
		//下に下りてくる
		SprObj[n].vy+=4;
	}else{
		//左右に振る
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

	//攻撃パターン
	if(SprObj[n].free.w[1]<100){
		//ばら撒き弾
		if(g_GameSystem.VsyncTimer%4==0){
			ene_shot_init(n,0);
		}
	}else if(SprObj[n].free.w[1]==200){
		//８の字弾1
		ene_shot_init(n,3);
	}else if(SprObj[n].free.w[1]==250){
		//８の字弾2
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
//big_boss２
//--------------
static void enemy03_init(int s)
{
	//-----------------------------
	//big_boss２：初期化
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
	//big_boss２：メイン
	//-----------------------------

	if(SprObj[n].y<32){
		//下に下りてくる
		SprObj[n].vy+=4;
	}else{
		//左右に振る
		SprObj[n].x += SprObj[n].vx/10;
		SprObj[n].vx+=SprObj[n].free.b[0];
	}

	if(GetAbs(SprObj[n].vx)>=28)
		SprObj[n].free.b[0]*=-1;

	SprObj[n].x = SprObj[n].x;
	SprObj[n].y = SprObj[n].vy/10;

	ShootSub::PutSprite(SprObj[n].x,SprObj[n].y,32,PRIO_TEKI,0);
	ShootSub::PutSprite(SprObj[n].x-24,SprObj[n].y+8,33,PRIO_TEKI+1,0);	//ウイング
	ShootSub::PutSprite(SprObj[n].x+32,SprObj[n].y+8,34,PRIO_TEKI+1,0);	//ウイング

	//囲い込みレーザー
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
//big_boss３
//--------------
static void enemy04_init(int s)
{
	//-----------------------------
	//big_boss３：初期化
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
	//big_boss３：メイン
	//-----------------------------

	if(SprObj[n].y<32){
		//下に下りてくる
		SprObj[n].vy+=4;
	}else{
		//左右に振る
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

	//ダメージに応じた形で表示
	ShootSub::PutSprite(SprObj[n].x,SprObj[n].y,32,PRIO_TEKI,0);
	if(SprObj[n].free.w[2]<=1){
		ShootSub::PutSprite(SprObj[n].x-24,SprObj[n].y+8,33,PRIO_TEKI+1,0);	//ウイング左
		ShootSub::PutSprite(SprObj[n].x+32,SprObj[n].y+8,34,PRIO_TEKI+1,0);	//ウイング右
	}
	if(SprObj[n].free.w[2]<=0){
		ShootSub::PutSprite(SprObj[n].x,SprObj[n].y+16,35,PRIO_TAMA+2,0);	//キャノン
	}


	SprObj[n].free.w[1]++;
	SprObj[n].free.w[1]=SprObj[n].free.w[1]%100;

	switch(SprObj[n].free.w[2]){
	case 0:		//攻撃パターン1
		if(SprObj[n].free.w[1]%100==70){
			ene_shot_init(n,3);			//８の字
		}
		if(SprObj[n].free.w[1]%100<60){
			if(SprObj[n].free.w[1]%15==0){
				ene_shot_init(n,5);		//最強弾
			}
		}
		break;
	case 1:		//攻撃パターン2
		if(g_GameSystem.VsyncTimer%35==20){
			ene_shot_init(n,5);			//最強弾
		}
		if(g_GameSystem.VsyncTimer%35<20){
			if(g_GameSystem.VsyncTimer%5==0){
				ene_shot_init(n,1);		//囲い込みレーザー
				ene_shot_init(n,2);		//囲い込みレーザー
			}
		}
		break;
	case 2:		//攻撃パターン3
		if(SprObj[n].free.w[1]==0){
			ene_shot_init(n,3);			//８の字
		}else if(SprObj[n].free.w[1]==50){
			ene_shot_init(n,4);			//８の字
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
	//Ｇスイング：初期化
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
	//Ｇスイング：メイン
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
	//ice block：初期化
	//-----------------------------
	int n,kaku;

	if(g_GameSystem.enemy_num>=MAX_ENEMY) return;

	n=GetObjNum();
	if(n==-1) return;

	SprObj[n].type=5;
	SprObj[n].flag=3;

	if((SprObj[s].flag==3)&&(SprObj[s].free.w[2])){
		//分裂作成
		if(SprObj[s].free.w[2]==1){
			//第２段階
			kaku=30+(GetRand()%4)*30;
			SprObj[n].hp=3;
			SprObj[n].free.w[0]=cos_tbl[kaku];
			SprObj[n].free.w[1]=sin_tbl[kaku];
		}else if(SprObj[s].free.w[2]==2){
			//第３段階
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
		//第１段階
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
	//ice block：メイン
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
//  効果
//---------------------------------------------------------
static void clear_enemy_shot(void)
{
	//--------------
	//敵の弾全消し
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
	//きらきら：初期化
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
	//きらきら：メイン
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
	//爆発：初期化
	//--------------
	int n;

	if(s==-1){
		//強制的に爆発パターンを出す。
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
	//爆発：メイン
	//--------------
	int s,spr=5;
	int col;
	s=SprObj[n].free.b[0]++;

	if(SprObj[n].type==0){
		//爆発A
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
		//爆発B
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
//ゲーム固有のサブルーチン
//---------------------------------
static int GetObjNum()
{
	//------------------------------------
	//あいているオブジェクトナンバーを返す
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
	//オブジェクト削除
	//-----------------

	SprObj[n].flag=0;
}


static int hit_check(int n)
{
	//-----------------
	//あたり判定検出
	//-----------------
	int i;

	//画面外
	if(SprObj[n].y<=8) return 0;

	for(i=0;i<enObjMax;i++){
		if(SprObj[i].flag==0) continue;
		if(i==n) continue;
		if(SprObj[i].flag!=3) continue;
		if(GetAbs(SprObj[i].x-SprObj[n].x)<=16){
			if(GetAbs(SprObj[i].y-SprObj[n].y)<=16){
				//HIT検出
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
	//敵弾ｖｓ自分のあたり判定検出
	//-----------------------------
	int i;

	i = enMyShipIndex;

	if(SprObj[i].type!=3) return 0;

	if(GetAbs(SprObj[i].x-SprObj[n].x)<=2)
	{
		if(GetAbs(SprObj[i].y-SprObj[n].y)<=3)
		{
			//HIT検出
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
	//スタイラスの情報を得る
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
	//絶対値を求める
	//-----------------------------
	return  (n<0)? n*-1 : n;
}


int GetRand(void)
{
	//-----------------------------
	//ランダムな数字を作り出す。
	//-----------------------------

	return rand()%10000;

}


static void padControl( void )
{
	//-----------------------------
	//毎フレームのパッド情報を取得
	//-----------------------------

	g_GamePad.keydown = GetStylus( &g_GamePad.x , &g_GamePad.y );

}


//-------------------------------
//SINテーブル
//右を０度として右回りに３６０°
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
//COSテーブル
//右を０度として右回りに３６０°
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
//スプライト定義テーブル
//-------------------------
StSpriteDef SprVisor[] = {
	//スプライト定義テーブル
	{enTpgSpr,0,0,2,2, 8,8},	//0 visoriser
	{enTpgSpr,2,0,1,1, 4,4},	//1 visoriserショット
	{enTpgSpr,4,0,2,2, 8,8},	//2 爆発A_0
	{enTpgSpr,6,0,2,2, 8,8},	//3 爆発A_1
	{enTpgSpr,8,0,2,2, 8,8},	//4 爆発A_2
	{enTpgSpr,3,0,1,2, 4,0},	//5 visoriserレーザー

	{enTpgSpr,2,1,1,1, 4,4},	//6 敵ショット
	{enTpgSpr,2,3,1,1, 4,4},	//7 visoriserレーザーヒット1
	{enTpgSpr,3,3,1,1, 4,4},	//8 visoriserレーザーヒット2

	{enTpgSpr,0,2,2,2, 8,8},	//9 option

	//地形
	{enTpgSpr,15,0,5,1, 0,0},	//10 バックグラウンド0
	{enTpgSpr,15,1,5,1, 0,0},	//11 バックグラウンド1
	{enTpgSpr,15,2,5,1, 0,0},	//12 バックグラウンド2
	{enTpgSpr,15,3,5,1, 0,0},	//13 バックグラウンド3
	{enTpgSpr,15,4,5,1, 0,0},	//14 バックグラウンド4
	{enTpgSpr,15,5,5,1, 0,0},	//15 バックグラウンド5
	{enTpgSpr,15,6,5,1, 0,0},	//16 バックグラウンド6
	{enTpgSpr,15,7,5,1, 0,0},	//17 バックグラウンド7
	{enTpgSpr,15,8,5,1, 0,0},	//18 バックグラウンド8
	{enTpgSpr,15,9,5,1, 0,0},	//19 バックグラウンド9
	{enTpgSpr,15,10,5,1, 0,0},	//20 バックグラウンド10
	{enTpgSpr,15,11,5,1, 0,0},	//21 バックグラウンド11
	{enTpgSpr,15,12,5,1, 0,0},	//22 バックグラウンド12
	{enTpgSpr,15,13,5,1, 0,0},	//23 バックグラウンド13
	{enTpgSpr,15,14,5,1, 0,0},	//24 バックグラウンド14
	{enTpgSpr,15,15,5,1, 0,0},	//25 バックグラウンド15
	{enTpgSpr,15,16,5,1, 0,0},	//26 バックグラウンド16
	{enTpgSpr,15,17,5,1, 0,0},	//27 バックグラウンド17
	{enTpgSpr,15,18,5,1, 0,0},	//28 バックグラウンド18
	{enTpgSpr,15,19,5,1, 0,0},	//29 バックグラウンド19
	{enTpgSpr,10,0,5,1, 0,0},	//30 バックグラウンド20

	{enTpgSpr,4,4,2,2, 8,8},	//31 敵ザコ zako
	{enTpgSpr,0,4,4,4, 16,16},	//32 敵ボス big_boss

	{enTpgSpr,6,4,3,2, 16,16},	//33 敵ボス big_boss用オプションウイング左
	{enTpgSpr,6,6,3,2, 16,16},	//34 敵ボス big_boss用オプションウイング右
	{enTpgSpr,4,6,2,2, 8,8},	//35 敵ボス big_boss用オプションキャノン

	{enTpgSpr,9,4,1,2, 4,4},	//36 敵   レーザー弾
	{enTpgSpr,9,6,2,2, 8,8},	//37 敵   最強弾

	{enTpgSpr,0 ,8,4,4, 16,16},	//38 爆発B_0
	{enTpgSpr,4 ,8,4,4, 16,16},	//39 爆発B_1
	{enTpgSpr,8 ,8,4,4, 16,16},	//40 爆発B_2
	{enTpgSpr,11,4,4,4, 16,16},	//41 爆発B_3

	{enTpgSpr,2,2,1,1, 0,0},	//42 数字０
	{enTpgSpr,3,2,1,1, 0,0},	//43 数字１
	{enTpgSpr,4,2,1,1, 0,0},	//44 数字２
	{enTpgSpr,5,2,1,1, 0,0},	//45 数字３
	{enTpgSpr,6,2,1,1, 0,0},	//46 数字４
	{enTpgSpr,7,2,1,1, 0,0},	//47 数字５
	{enTpgSpr,8,2,1,1, 0,0},	//48 数字６
	{enTpgSpr,9,2,1,1, 0,0},	//49 数字７
	{enTpgSpr,10,2,1,1, 0,0},	//50 数字８
	{enTpgSpr,11,2,1,1, 0,0},	//51 数字９
	{enTpgSpr,12,2,2,1, 0,0},	//52 数字00

	{enTpgSpr,12,8,3,3, 12,12},	//53 mid Boss
	{enTpgSpr,4,3,1,1, 0,0},	//54 残機表示

	{enTpgSpr,5,14,1,1, 0,0},	//55 きらきら
	{enTpgSpr,6,14,1,1, 0,0},	//56 きらきら
	{enTpgSpr,7,14,1,1, 0,0},	//57 きらきら
	{enTpgSpr,8,14,1,1, 0,0},	//58 きらきら
	{enTpgSpr,9,14,1,1, 0,0},	//59 きらきら

	{enTpgSpr,0,12,3,3, 12,12},	//60 ice block
	{enTpgSpr,3,12,2,2, 8,8},	//61 ice block（中１）
	{enTpgSpr,5,12,2,2, 8,8},	//62 ice block（中２）
	{enTpgSpr,3,14,1,1, 4,4},	//63 ice block（小１）
	{enTpgSpr,4,14,1,1, 4,4},	//64 ice block（小２）

	{enTpgSpr,0,15,10,1, 0,0},	//65 ゲームオーバー
	{enTpgSpr,0,17,11,2, 0,0},	//66 VISORIZER

	{enTpgSpr,0,16,9,1, 0,0},	//67 HISCORE
	{enTpgSpr,2,16,7,1, 0,0},	//68 SCORE

	{enTpgSpr,11,12,4,2, 0,0},	//69 SCORE

};

