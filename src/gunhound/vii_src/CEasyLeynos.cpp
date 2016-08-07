//-----------------------------------
//
// 簡易アサルトクラス
//
//--------------------------
// 処理の軽いLeynosの処理
// 主に敵アサルト用
//-----------------------------------
#include <gunvalken.h>
#include "Effect/EffectBase.h"

#define ELEYNOS_MODE_SPACE (1)

gxSprite AsSpr[]={
	//GV用スプライト

	{TPG_VALKEN,32*0,32*0,32,32,8,8},	//0:ARM
	{TPG_VALKEN,32*1,32*0,32,32,16,24},	//1:BODY
	{TPG_VALKEN,32*3,32*0,32,32,16,8},	//2:LEG奥
	{TPG_VALKEN,32*2,32*0,32,32,16,8},	//3:LEG手前
	{TPG_VALKEN,32*4,32*0,32,32,16,16},	//4:Shield
	{TPG_VALKEN,32*6,32*0,64,16,22,4},	//5:gun
	{TPG_VALKEN,32*0,32*3,32,32,32,16},	//6:booster
	{TPG_VALKEN,32*3,32*1,16,32,8,32},	//7:Shield

	{TPG_VALKEN,32*0,32*1,32,32,16,8},	//8:leg1_jump2
	{TPG_VALKEN,32*1,32*1,32,32,16,0},	//9:leg1_jump1

	{TPG_VALKEN,32*6,16*0,64,16,22,4},	//10:gun
	{TPG_VALKEN,32*6,16*1,64,16,22,4},	//11:gun
	{TPG_VALKEN,32*6,16*2,64,16,22,4},	//12:gun
	{TPG_VALKEN,32*6,16*3,64,16,22,4},	//13:gun
	{TPG_VALKEN,32*6,16*4,64,16,22,4},	//14:gun
	{TPG_VALKEN,32*6,16*5,64,16,22,4},	//15:gun

	{TPG_VALKEN,160,0,32,32,8,16},		//16:punch
	{TPG_VALKEN,160,64,32,32,16,24},	//17:shield_body01
	{TPG_VALKEN,160,32,32,32,16,24},	//18:shield_body02
	{TPG_VALKEN,160,32,32,32,16,24},	//19:ターン用

//	{TPG_VALKEN,0,128,32,32,16,16},				//16:punch
};

enum {
	enWalkCntMax = 1600,
	enDamageOffsetY = 400,
};

const Sint32 el_leg_walktbl[][2]={
	{-400,0},
	{-300,-200},

	{-200,-400},
	{   0,-500},

	{200,-600},
	{350,-500},

	{500,-400},
	{700,-200},

	{800,0},
	{700,0},

	{500,0},
	{350,0},

	{200,0},
	{  0,0},

	{-200,0},
	{-300,0},
};

const Sint32 el_body_walktbl[][2]={
	{0,0},
	{0,-50},

	{0,-100},
	{0,-150},

	{0,-200},
	{0,-150},

	{0,-100},
	{0,-50},

	{0,0},
	{0,-50},

	{0,-100},
	{0,-150},

	{0,-200},
	{0,-150},

	{0,-100},
	{0,-50},
};

const Sint32 el_arm_walktbl[][2]={
	{-200,0},
	{-150,50},

	{-100,100},
	{-50,150},

	{ 0,200},
	{ 5,150},

	{ 100,100},
	{ 150,50},

	{ 200,0},
	{ 150,50},

	{ 100,100},
	{ 50,150},

	{ 0,200},
	{ -50,150},

	{-100,100},
	{-150,50},
};

CEasyLeynos::CEasyLeynos()
{
	//--------------------------
	//簡易レイノス
	//--------------------------

	for(Sint32 i=0;i<EL_MAX;i++){
		parts[i] = new elparts_t;
		memset(parts[i],0x00,sizeof(elparts_t));
		parts[i]->spr = NULL;
	}
	obj = new CObj();

	cpu_control = gxTrue;
	atari_flag_ = gxTrue;
	spd_ = 1;
	walk_spd_ = 1;
	walk_width_ = 1;
	walk_height_ = 1;
	walk_cnt = 0;
	arm_shock_ = 0;

	m_bNoShock = gxFalse;
	shock_cnt_ = 0;
	knock_back_ = 0;
	absorber_ = 0;
	gv_ = 0;
	prio_ = PRIO_ENEMY;
	dir_ = 1;
	arm_rot = 0;
	kansei.x = kansei.y = 0;
	m_bDeadEnd = m_bDead = gxFalse;
	spd_add_ = 0;
	atack_rot = 0;
	atack_dst = 0;
	chakuti_ = gxFalse;
	dmg.x = 0;
	dmg.y = 0;

	m_bDashMode = gxFalse;

	m_sNokezori = 0;

	m_uARGB = ARGB_DFLT;
	m_bCarryOn = gxFalse;	//運ばれている
	mode_ = 0;				//自由移動モード
}

CEasyLeynos::~CEasyLeynos()
{
	for(Sint32 i=0;i<EL_MAX;i++){
		delete parts[i];
	}

	delete obj;
}

void CEasyLeynos::action()
{
	//--------------------------
	//イージーレイノス基本行動
	//--------------------------

	Sint32 gravity = 120;
	Sint32 old_land = obj->is_landnow();

	if(IsCarry())
	{
		arm_rot = 30;
		gv_ = 0;
		obj->mov.x = 0;
		obj->mov.y = 0;
		pad_ = 0;//Joy[0].psh;
	}

	if(!cpu_control)
	{
		pad_ = Joy[0].psh;
	}

	if(m_bDeadEnd) return;

	control();
	absorber_proc();

	//-----------------------
	//ダメージ用の慣性移動
	//-----------------------
	if(dmg.x < 0) {
		dmg.x += 2;
		if(dmg.x > 0) dmg.x = 0;

	}else if(dmg.x > 0) {
		dmg.x -= 2;
		if(dmg.x < 0) dmg.x = 0;
	}

	obj->mov.x += (kansei.x+dmg.x);
	obj->mov.y += (kansei.y+dmg.y);

	if(mode_ == ELEYNOS_MODE_SPACE){
		gravity = 0;

	}else{
		if(pGame->is_gravit()){
			obj->mov.y += gv_+gravity;
		}

		if(!old_land && pGame->is_gravit() ){
			gv_ += 12;
			if(gv_ >= 700) gv_ = 700;
		}
	}

	if(obj->mov.y >= 700) obj->mov.y = 700;

	//慣性制御
	if(old_land)
	{
		//着地中のみ慣性減衰
/*
		if(kansei.x > 50){
			kansei.x += -kansei.x/10;
		}else if(kansei.x < -50){
			kansei.x += -kansei.x/10;
		}else{
			kansei.x = 0;
		}
*/
	}

	Sint32 direction = 1;

	if(dir_ == 1 && obj->mov.x < 0)
	{
		direction = -1;
	}

	if(dir_ == -1 && obj->mov.x > 0)
	{
		direction = -1;
	}

	if( gv_ < 0 ) m_bDashMode = gxFalse;

	if( m_bDashMode )
	{
		//ダッシュ時
		if( shock_cnt_ < 50 ) shock_cnt_ +=10;

		if(walk_cnt==0)
		{
			//何もしない
		}
		else
		{
			//脚をそろえるように調整
			Sint32 sAdd = direction * (walk_spd_*abs(obj->mov.x))/100;

			walk_cnt += sAdd;

			if( ( sAdd > 0 && walk_cnt >= enWalkCntMax) || ( sAdd < 0 && walk_cnt <= 0 ) )
			{
				walk_cnt = 0;
			}
		}

		m_bDashMode = gxFalse;
	}
	else
	{
		walk_cnt += direction * (walk_spd_*abs(obj->mov.x))/100;
	}

	if(atari_flag_)	obj->easy_collision_check();

	obj->pos.x += obj->mov.x;
	obj->pos.y += obj->mov.y;

	chakuti_ = gxFalse;

	if(atari_flag_)
	{
		if(obj->is_landnow())
		{
			chakuti_ = gxTrue;

			gv_ = 0;

			if(old_land == gxFalse)
			{
				//----------------------------
				//着地時の煙
				//----------------------------

				new CEffDashStorm( obj->pos.x+(-15+viiSub::Rand()%30)*100 , obj->pos.y, 0 , 0 );
				new CEffDashStorm( obj->pos.x+(-15+viiSub::Rand()%30)*100 , obj->pos.y, 0 , 0 );
				new CEffDashStorm( obj->pos.x+(-15+viiSub::Rand()%30)*100 , obj->pos.y, 0 , 0 );

				walk_cnt = 0;

				if( !m_bNoShock ) set_shock();
			}
		}
	}

	if(arm_shock_ > 0) arm_shock_ --;

	calc_parts(EL_BASE);
	calc_parts(EL_BODY);
	calc_parts(EL_ARM_L);
	calc_parts(EL_ARM_R);
	calc_parts(EL_LEG_L);
	calc_parts(EL_LEG_R);

	DamageMotion();

	parts[EL_BODY]->rot = arm_rot/5+parts[EL_BODY]->orz;
	parts[EL_ARM_R]->rot = arm_rot;

	pad_ = 0;

	obj->mov.x = obj->mov.y = 0;

	if(m_bDead)
	{
		m_bDeadEnd = gxTrue;
	}

	m_bDamage = gxFalse;

	if(obj->pos.x <= 500) obj->pos.x = 500;
}


void CEasyLeynos::Draw()
{
	draw_parts(EL_BASE);
	draw_parts(EL_LEG_L);
	draw_parts(EL_ARM_L);
	draw_parts(EL_BODY);
	draw_parts(EL_LEG_R);
	draw_parts(EL_ARM_R);
}


void CEasyLeynos::control()
{
	//-----------------------
	//操作
	//-----------------------

	Sint32 landnow = obj->is_landnow();
	Sint32 direction = 1;

	if(spd_add_ == 0) spd_add_ = 1000;	//即座に移動できる（慣性をかけない）

	flying_y = 0;

	if(landnow)
	{
		kansei.y = 0;

		if(pad_&JOY_L)
		{
			kansei.x += -spd_add_;

			if(kansei.x <= -spd_) kansei.x = -spd_;

			if((pad_&BTN_L1)==0) dir_ = -1;

			if(dir_ == 1)
			{
				direction = -1;
			}

		}
		else if(pad_&JOY_R)
		{
			kansei.x += spd_add_;
			if(kansei.x >= spd_) kansei.x = spd_;
			if((pad_&BTN_L1)==0) dir_ = 1;
			if(dir_ == -1){
				direction = -1;
			}
		}
		else
		{
			//慣性減衰
			if(kansei.x > 0)
			{
				kansei.x -= spd_add_;
				if(kansei.x <= 0)
				{
					kansei.x = 0;
				}
			}
			else if(kansei.x < 0)
			{
				kansei.x += spd_add_;

				if(kansei.x >= 0)
				{
					kansei.x = 0;
				}
			}
		}

	}
	else
	{
		//---------------------------------------
		//空中時
		//---------------------------------------

		if(mode_ == ELEYNOS_MODE_SPACE)
		{
			flying_y = viiMath::Cos100((viiSub::GameCounter()*8)%360)*2;

			if(pad_&JOY_L)
			{
				gv_ = -1;

				kansei.x += -spd_add_;

				if(kansei.x <= -spd_) kansei.x = -spd_;

				if((pad_&BTN_L1)==0) dir_ = -1;

				if(dir_ == 1)
				{
					gv_ *= -1;
					direction = -1;
				}

			}
			else if(pad_&JOY_R)
			{
				gv_ = 1;

				kansei.x += spd_add_;

				if(kansei.x >= spd_) kansei.x = spd_;

				if((pad_&BTN_L1)==0) dir_ = 1;

				if(dir_ == -1)
				{
					gv_ *= -1;
					direction = -1;
				}
			}

			if(pad_&JOY_U)
			{
				gv_ = -1;
				kansei.y += -spd_add_;
				if(kansei.y <= -spd_) kansei.y = -spd_;
			}
			else if(pad_&JOY_D)
			{
				gv_ = 1;

				kansei.y += spd_add_;

				if(kansei.y >= spd_) kansei.y = spd_;
			}

			//-----------------------------------
			//慣性減衰
			//-----------------------------------

			if(kansei.x > 0)
			{
				kansei.x -= spd_add_/2;

				if(kansei.x <= 0)
				{
					kansei.x = 0;
				}
			}
			else if(kansei.x < 0)
			{
				kansei.x += spd_add_/2;

				if(kansei.x >= 0)
				{
					kansei.x = 0;
				}
			}

			if(kansei.y > 0)
			{
				kansei.y -= spd_add_/2;

				if(kansei.y <= 0)
				{
					kansei.y = 0;
				}
			}
			else if(kansei.y < 0)
			{
				kansei.y += spd_add_/2;

				if(kansei.y >= 0)
				{
					kansei.y = 0;
				}
			}
		}
	}

	if((pad_&BTN_L1) == 0)
	{
		if(pad_&JOY_U) arm_rot --;
		if(pad_&JOY_D) arm_rot ++;
		if(arm_rot < -90) arm_rot = -90;
		if(arm_rot >  90) arm_rot = 90;
	}

	if(pad_&BTN_A)
	{
		//ジャンプ処理
		gv_ = -300;
	}

	walk_cnt = (enWalkCntMax+walk_cnt)%enWalkCntMax;	//16パターン

	if(landnow)
	{
		parts[EL_LEG_L]->rot = 0;
		parts[EL_LEG_R]->rot = 0;
	}
	else if(gv_ < 0)
	{
		walk_cnt = 0;
		if(parts[EL_LEG_L]->rot < 25) parts[EL_LEG_L]->rot += (25-parts[EL_LEG_L]->rot)/10;
		if(parts[EL_LEG_R]->rot < 40) parts[EL_LEG_R]->rot += (40-parts[EL_LEG_R]->rot)/10;
	}
	else if(gv_ > 0)
	{
		walk_cnt = 0;
		if(parts[EL_LEG_L]->rot > -40) parts[EL_LEG_L]->rot += (-40-parts[EL_LEG_L]->rot)/10;
		if(parts[EL_LEG_R]->rot > -25) parts[EL_LEG_R]->rot += (-25-parts[EL_LEG_R]->rot)/10;
	}
}


void CEasyLeynos::absorber_proc()
{
	//ショックアブソーバー処理

	Sint32 cnt = 0;

	if( knock_back_ > 0)
	{
		knock_back_ -= 100;

		if(knock_back_<0)
		{
			knock_back_ = 0;
		}
	}

	if(shock_cnt_ > 0)
	{
		shock_cnt_ -= 4;
		if(shock_cnt_ <= 0)
		{
			shock_cnt_ = 0;
		}
		else if(!m_bDashMode)
		{
			kansei.x = kansei.x*2/5;
			walk_cnt = 0;
		}
	}

	cnt = 50 - abs(shock_cnt_-50);

	absorber_ = cnt*10;
}

void CEasyLeynos::calc_parts(Sint32 n)
{
	//-----------------------
	//パーツ単品の位置計算
	//-----------------------
	Sint32 x,y;

	//ベース座標を足しこむ
	x = parts[n]->ofx;
	y = parts[n]->ofy;

	Sint32 wpos = walk_cnt/100;

	Sint32 xx;
	Sint32 yy;
	switch(n){
	case EL_BODY:
		if(absorber_){
			y += (absorber_*walk_height_)/10000;
		}else{
			x += el_body_walktbl[wpos][0]/100;
			y += el_body_walktbl[wpos][1]/100;
		}
		break;
	case EL_ARM_L:
		if(absorber_){
			y += (absorber_*walk_height_)/10000;
		}else{
			x += el_arm_walktbl[wpos][0]/100;
			y += el_arm_walktbl[wpos][1]/100;
		}
		break;
	case EL_ARM_R:
		xx = viiMath::Cos100(parts[EL_BODY]->rot+parts[n]->prot);
		yy = viiMath::Sin100(parts[EL_BODY]->rot+parts[n]->prot);
		x += (xx*parts[n]->pdist)/100;
		y += (yy*parts[n]->pdist)/100;
		x += (viiMath::Cos100(arm_rot+atack_rot) * (knock_back_*-1))/10000;
		y += (viiMath::Sin100(arm_rot+atack_rot) * (knock_back_*-1))/10000;
		wpos = (wpos+8)%16;
		if(absorber_){
			y += (absorber_*walk_height_)/10000;
		}else{
			x += el_arm_walktbl[wpos][0]/100;
			y += el_arm_walktbl[wpos][1]/100;
		}
		break;
	case EL_LEG_L:
		x += walk_width_ *el_leg_walktbl[wpos][0]/10000;
		y += walk_height_*el_leg_walktbl[wpos][1]/10000;
		break;
	case EL_LEG_R:
		wpos = (wpos+8)%16;
		x += walk_width_ *el_leg_walktbl[wpos][0]/10000;
		y += walk_height_*el_leg_walktbl[wpos][1]/10000;
		break;
	}

	parts[n]->x = x;
	parts[n]->y = y;
}


void CEasyLeynos::draw_parts(Sint32 n)
{
	//-----------------------
	//パーツ表示
	//-----------------------
	Sint32 direction = dir_;

	if(parts[n]->spr == NULL) return;

	Sint32 x,y,z;

	x = obj->pos.x;
	y = obj->pos.y;
	z = parts[n]->ofz;

	//ベース座標を足しこむ
	x += parts[EL_BASE]->x*100*direction;
	y += parts[EL_BASE]->y*100;

	//自分座標とオフセット座標を足しこむ
	x += (parts[n]->x)*100*direction;
	y += (parts[n]->y)*100;
	y += flying_y;

	Sint32 atr = ATR_DFLT,col=m_uARGB;

	if(dir_==-1){
		atr |= ATR_FLIP_X;
	}

	//最終的な座標を保存しておく
	parts[n]->dx = x;
	parts[n]->dy = y;

	if(m_bDead){

		_opt_[0] = 0;	//(Sint32)parts[n]->spr; ポインタが64bitになってつじつまが合わなくなった
		_opt_[1] = atr;
		_opt_[2] = col;
		_opt_[3] = parts[n]->rot;
		_opt_[4] = 100;
		_opt_[5] = 0;

		viiEff::Set(EFF_DAMAGE_PARTS,x,y, _opt_, parts[n]->spr );


	}
	else
	{
		viiDraw::Sprdmg( m_bDamage ,parts[n]->spr,x,y,prio_+z,atr,col,(parts[n]->sx+100)/100.0f,parts[n]->rot);
	}

}

void CEasyLeynos::setFirePos(Sint32 ax,Sint32 ay)
{
	//攻撃場所を設定

	atack_rot = viiMath::Atan2_100(ay,ax)/100;
	atack_dst = viiMath::Dist(ax,ay);
}

void CEasyLeynos::getFirePos(Sint32 &ax,Sint32 &ay)
{
	//攻撃場所を得る

	ax = parts[EL_ARM_R]->dx + viiMath::Cos100(arm_rot+atack_rot)*atack_dst*dir_;
	ay = parts[EL_ARM_R]->dy + viiMath::Sin100(arm_rot+atack_rot)*atack_dst;

}

void CEasyLeynos::set_armrot_targetangle45(Sint32 x,Sint32 y)
{
	//-----------------------------------
	//腕角度を４５度単位で補正する
	//-----------------------------------
	Sint32 syougen = viiMath::GetSyougen(x,y,GetTargetPlayer()->x,GetTargetPlayer()->y);
	Sint32 kaku = 0;

	switch(syougen)	{
	case 0:	//横方向
	case 4:
		kaku = 0;
		break;
	case 1:	//斜め下
	case 3:
		kaku = 45;
		break;

	case 2:	//下
		kaku = -90;
		break;
	case 6:
		kaku = 90;
		break;

	case 5:	//斜め上
	case 7:
		kaku = -45;
		break;
	}

	Sint32 rotspd = (kaku - arm_rot)/20;

	if(rotspd == 0)
	{
		if(kaku < arm_rot) arm_rot --;
		if(kaku > arm_rot) arm_rot ++;
	}

	arm_rot += rotspd;
}


void CEasyLeynos::SetDamageAnime(Sint32 dx)
{
	Sint32 sDir = 1;

	if(dx < 0 )
	{
		//左から
		if( get_dir() == DIR_LEFT)
		{
			sDir = -1;//のけぞり
		}
		else
		{
			sDir = 1;//うつむき
		}
	}
	else
	{
		//右から
		if( get_dir() == DIR_LEFT)
		{
			sDir = 1;//うつむき
		}
		else
		{
			sDir = -1;//のけぞり
		}
	}

	obj->pos.y -= enDamageOffsetY;

	parts[EL_BODY]->orz = 60*sDir;

	m_sNokezori = 20;
}


void CEasyLeynos::DamageMotion()
{
	if( m_sNokezori > 0 )
	{
		m_sNokezori --;
		parts[EL_BODY]->orz += (0-parts[EL_BODY]->orz)/20;
	}
	else
	{
		parts[EL_BODY]->orz = 0;
	}

}


void EasyLeynosTest()
{
	if(Eleynos == NULL){
		Eleynos = new CEasyLeynos();

		Eleynos->set_spr(EL_BASE ,NULL);
		Eleynos->set_spr(EL_BODY ,&AsSpr[1]);
		Eleynos->set_spr(EL_ARM_L,NULL);
		Eleynos->set_spr(EL_ARM_R,&AsSpr[0]);
		Eleynos->set_spr(EL_LEG_L,&AsSpr[2]);
		Eleynos->set_spr(EL_LEG_R,&AsSpr[3]);
		Eleynos->set_pos(150*50,100*100,PRIO_ENEMY);
		Eleynos->parts[EL_BASE]->ofy = -24;
		Eleynos->parts[EL_ARM_L]->ofy = -12;
		Eleynos->parts[EL_ARM_R]->ofy = -12;
		Eleynos->parts[EL_ARM_R]->ofx = 6;
		Eleynos->set_spd(100);
		Eleynos->set_walkcycle(50);
		Eleynos->walk_width_ = 100;
		Eleynos->walk_height_ = 100;
	}

/*
	if(Joy[0].trg&BTN_L1) Eleynos->m_bDead = gxTrue;
	if(Joy[0].psh&BTN_R1){
		Eleynos->m_bDead = gxFalse;
		Eleynos->m_bDeadEnd  = gxFalse;
	}
*/
	Eleynos->action();

}


