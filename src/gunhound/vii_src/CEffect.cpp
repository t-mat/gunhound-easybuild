//----------------------------------------------
//オブジェクト
//----------------------------------------------
#include<gunvalken.h>

gxSprite TitleSpr[]={
	{TPG_ETC2,0,128,112,32,56,12},		//PushStart
	{TPG_ETC2,0,144+16*0,144,16,72,8},	//PushButton
	{TPG_ETC2,0,144+16*1,144,16,72,8},	//MISSION START
	{TPG_ETC2,0,144+16*2,144,16,72,8},	//TRAINING
	{TPG_ETC2,0,144+16*3,144,16,72,8},	//AS SELECT
	{TPG_ETC2,0,144+16*4,144,16,72,8},	//OPTION

	{TPG_ETC2,0,256+0,144,16,72,8},	//MANUAL
	{TPG_ETC2,0,256+16,144,16,72,8},	//AUTO
};

gxSprite TitleSprCursor[]={
//	{TPG_ETC2,0,144+16*5,112,16,56,8},	//CURSOR
	{TPG_CLOUD,64,32,32,16,16,8},		//ピンク弾
	{TPG_ETC2,0,144+16*5,112,16,56,8},	//CURSOR
};

CEffect* CEffManager::set_effect(Sint32 type,Sint32 x,Sint32 y,Sint32* opt , gxSprite *pSpr )
{
	for(Sint32 i=0;i<EFF_MAX;i++)
	{
		if(pEff[i] == NULL)
		{
			pEff[i] = new CEffect();

//@HIT@			memset(pEff[i],0x00,sizeof(CEffect) );

//@HIT@			pEff[i]->init();

//			m_pEffectRescue->type_ = 
			pEff[i]->type_ = type;
//			m_pEffectRescue->x_ = 
			pEff[i]->x_ = x;
//			m_pEffectRescue->y_ = 
			pEff[i]->y_ = y;

			if(opt!=NULL)
			{
				for(Sint32 j=0;j<10;j++)
				{
//					m_pEffectRescue->opt[j] = 
					pEff[i]->opt[j] = opt[j];
				}
			}else{
				for(Sint32 j=0;j<10;j++)
				{
//					m_pEffectRescue->opt[j] = 
					pEff[i]->opt[j] = 0;
				}
			}

			if( pSpr )
			{
				pEff[i]->Sprite = *pSpr;
			}

			return pEff[i];
		}
	}

	//-------------------------------------------------------------------
	viiDbg::log("これ以上エフェクトは生成できません。");
	//-------------------------------------------------------------------

	return NULL;

}

//-------------------------------------------------------------
//-------------------------------------------------------------
CEffect::CEffect()
{
	init();
}

void CEffect::init()
{

	memset(&free,0x00,sizeof(char)*32);

	sw_ = 0;
	x_ = 0;
	y_ = 0;
	dir = 1;
	type_ = 0;
	seq_ = 0;
	exist_ = gxTrue;
	xx_ = yy_ =0;
	timer_ = 0;
	pObj = NULL;

}


CEffect::~CEffect()
{

	if(pObj) delete pObj;

}


void CEffect::action()
{
	//オブジェクト実行振り分け

	switch(type_){
	case EFF_VULCAN:
		vulcan();
		break;
	case EFF_PUNCH:
		punch();
		break;
	case EFF_MISSILE:
		missile();
		break;
	case EFF_LASER:
		laser();
		break;
	case EFF_LASER_PARTS:
		laser_parts();
		break;
	case EFF_NAPALM:
		napalm();
		break;
	case EFF_LEYNAPALM:
		ley_napalm();
		break;
	case EFF_MICROMISSILE:
		MicroMissile();
		break;
	case EFF_THEXDER:
		thexder();
		break;
	case EFF_MELONBOMB:
		melonbomb();
		break;
	case EFF_HEATHAWK:
		heathawk();
		break;

	case EFF_MISSILE_SMOKE:
		missile_smoke();
		break;

	case OBJ_TEST:
		test_obj();
		break;
//	case EFF_BOMB:
//		bomb();
		break;
	case EFF_RING:
		ring();
		break;
	case EFF_BURNIA:
		eff_burnia();
		break;
	case EFF_DASH_SMOKE:
		roller_dashsmoke();
		break;
	case EFF_BOMB_PARTS:
		bomb_parts();
		break;
	case EFF_BOMB_SMALL:
		bomb_small();
		break;
	case EFF_BIGBOMB:
		bigbomb();
		break;
	case EFF_BIGBOMB_PARTS:
		bigbomb_parts();
		break;
	case EFF_LASERBOMB_PARTS:
		laser_bomb_parts();
		break;
	case EFF_BURNIA_SMOKE:
		burnia_smoke();
		break;
	case EFF_BOMBBEAM:
		bombbeam();
		break;
	case EFF_VERSIS:
		versis_action();
		break;
	case EFF_MISSILE_SHIP:
		op_missile_ship();
		break;
	case EFF_CRUISE_MISSILE:
		op_cruise_missile();
		break;
	case EFF_GRAHAMB:
		op_grahamb_ship();
		break;

	case EFF_OPTIONBIT:
		OptionBit();
		break;

	case EFF_SHORTLASER:
		ShortLaser();
		break;

	case EFF_PARTS_BREAK:
		parts_break();
		break;
	//敵弾関連
//	case EFF_ENE_BULLET:
//		ene_bullet();
//		break;
	case EFF_ENE_BULLET2:
		ene_bullet2();
		break;
//	case EFF_ENE_BOMB:
//		ene_bomb();
//		break;
//	case EFF_ENE_MISSILE:
//		ene_missile();
//		break;
	case EFF_ENE_BOMB2:
		ene_bomb2();
		break;
//	case EFF_AIRCANNON:
//		ene_aircannon();
//		break;
	case EFF_ENE_PHOTON:
		ene_shot_photon();
		break;
	case EFF_BURNIA_DAMAGE:
		ene_burnia_damage();
		break;
	case EFF_MAP_DAMAGE_PARTS:
		map_breakparts();
		break;
	case EFF_SHUTTLE_BURNIA:
		shuttle_burnia();
		break;
	case EFF_BOOSTER_TOSS:
		booster_toss();
		break;

	case EFF_ENE_LASERCANNON:
		ene_laser();
		break;
	case EFF_ENERGYBEAM:
		energybeam();
		break;

	case EFF_REFLECTBULLET:
		eneReflectBullet();
		break;
	case EFF_AIMINGLASERBIT:
		eneAimingLaserBit();
		break;
	case EFF_BIGCANNON:
		eneBigCannon();
		break;
	case EFF_GATRINGBIT:
		eneGatringBit();
		break;

	case EFF_POWER_CHIP:
	case EFF_WEAPON_CHIP:
	case EFF_HP_CHIP:
		//@ACT@item_chip();
		break;
	case EFF_YAKKYOU:
		yakkyou();
		break;
	case EFF_FIREFLOWER:
		FireFlower();
		break;
	case EFF_MAZZLE:
		vulcan_mazzleflash();
		break;
	case EFF_HITFLASH:
		vulcan_hitflash();
		break;
	case EFF_HITFLASH_ENE:
		tama_hitflash_ene();
		break;
	case EFF_BOMBFLASH_ENE:
		bomb_hitflash_ene();
		break;
//	case EFF_SHOT_CANNON:
//		ene_shot_cannon();
//		break;
	case EFF_SHOT_BITLASER:
		ene_bitlaser();
		break;
	case EFF_DAMAGE_PARTS:
		damage_parts();
		break;

	case EFF_RELEASE_PARTS:
		release_parts();
		break;

	case EFF_HUMAN:
		human();
		break;
	case EFF_HUMAN_DAMAGE:
		human_damage();
		break;

	case EFF_HUMAN_ENGINEER:
		engineer();
		break;
	case EFF_HUMAN_ENGINEER2:
		engineer2();
		break;

	case EFF_HUMAN_ENGINEER3:
		engineer3();
		break;

	case EFF_BOOSTER_OFF:
		booster_off();
		break;

	case EFF_BREAK_THUNDER:
		break_thunder();
		break;

	case ENESHIP_LAUNCH:
		eneship_aslaunch();
		break;

	case ENE_PROPERA_MISSILE:
		ene_propera_missile();
		break;

	case EFF_LASERCANNON:
		lasercannon();
		break;
	case EFF_TWINCANNON:
		twin_cannon();
		break;

	case EFF_MASATSU_A:
		masatsu_large();
		break;

	case EFF_MASATSU_B:
		masatsu_small();
		break;
	case EFF_CLOUD_A:
		cloud_yoko();
		break;
	case EFF_CLOUD_B:
		cloud_tate();
		break;
	case EFF_CLOUD_C:
		cloud_shuttle();
		break;
	case EFF_BROKENPARTS:
		broken_parts();
		break;
	case EFF_BROKENPARTSDELUXE:
		broken_parts_deluxe();
		break;
	case EFF_BROKENPARTS_COLONY:
		broken_parts_colony();
		break;
	case EFF_BROKEN_ROCK:
		broken_rock();
		break;
	case EFF_GRENADE:
		grenade();
		break;
	case EFF_D_D1:
		defence_discharger();
		break;
	case EFF_D_D2:
		defence_discharger2();
		break;
	case EFF_GRENADE_EFFECT:
		grenade_effect();
		break;
	case EFF_ENERGYBALL:
		energyball();
		break;
	case EFF_COLONY_SHUTTER:
		colony_shutter();
		break;

	case EFF_SPRINELASER:
		sprine_laser();
		break;

//	case EFF_SANDSTORM:
//		SandStorm();
//		break;
	case EFF_SANDROCK:
		SandRock();
		break;

	//ステージ５エフェクト
	case EFF_BRIZZARD:		//雪の天候エフェクト
		Brizzard();
		break;
	case EFF_WEATHER_RAIN:	//雨の天候エフェクト
		WeatherRain();
		break;
	case EFF_SNOWSMOKE:		//雪煙
		SnowSmoke();
		break;
	case EFF_DUNGEONROCK:	//ダンジョン内岩
		DungeonRock();
		break;
	case EFF_AIRBREATH:		//汎用エアー
		AirBreath();
		break;

	case EFF_FALL_A:
		FallEffectA();
		break;
	case EFF_FALL_B:
		FallEffectB();
		break;
	case EFF_FALL_C:
		FallEffectC();
		break;
	case EFF_RABBIT:
		Rabbit();
		break;
	case EFF_KUNOICHI:
		Kunoichi();
		break;
	case EFF_ARMY_GUN:		//アーミー１
		ArmyGun();
		break;
	case EFF_ARMY_SNIPER:	//アーミー２
		ArmySniper();
		break;
	case EFF_ARMY_SARGENT:	//アーミー３
		ArmySargent();
		break;
	case EFF_ARMY_CANNON:	//アーミー４
		ArmyCannon();
		break;
	case EFF_ARMY_JEEP:		//アーミー５
		ArmyJeep();
		break;
	case EFF_ARMY_TANK:		//アーミー６
		ArmyTank();
		break;

	case EFF_LAUNCH_NPVALKEN:	//２面で出撃するその他のＡＳ
		launch_other_as();
		break;

	case EFF_SPRINE_THUNDER:
		sprine_thunder();
		break;

	case EFF_RUNNING_VERDARK:
		RunningVerdark();
		break;
	case EFF_RUNNING_SEIJIKA:
		RunningSeijika();
		break;
	case EFF_RUNNING_SHERMARK:
		RunningShermark();
		break;
	case EFF_KILLMYSELF_SHERMARK:
		KillMyselfShelmark();
		break;

	case EFF_OPENING_STRING:
		OpeningString();
		break;

	case EFF_ENDOFSOLDIER:
		EndOfSoldier();
		break;

	case EFF_WARNINGBOX:
		warning_box();
		break;

	}

	timer_ ++;
	if(timer_ >=0x70000000){
		//絶対にタイマーをゼロにしない
		timer_ = 1;
	}
}

void CEffect::test_obj()
{

}


//----------------------------------------
//オープニング用
//----------------------------------------
void CEffect::op_missile_ship()
{
	//ミサイル艦
	if(timer_ == 0){
		free.w[0] = 0;
		free.w[1] = 0;
		rot = -45;
		x_*=100;
		y_*=100;
	}


	if(timer_ >= 200){
		if(rot<90){
			rot += (90-rot)/30;
		}
	}

	if(timer_ >= 220){
		free.w[1] += 6;
		y_ -= free.w[1];
	}else{
		y_ += (12000-y_)/100;
	}

	viiDraw::Spr_NoScrl(&AsOpening[0],x_,y_,PRIO_SYSTEM-1,ATR_DFLT,ARGB_DFLT);
	viiDraw::Spr_NoScrl(&AsOpening[1],x_+30*100,y_+30*100,PRIO_SYSTEM-1,ATR_DFLT,ARGB_DFLT,1.0f,-rot);
	viiDraw::Spr_NoScrl(&AsOpening[1],x_-30*100,y_+30*100,PRIO_SYSTEM-1,ATR_DFLT|ATR_FLIP_X,ARGB_DFLT,1.0f,-rot);
	if(free.w[0] == 0){
		viiDraw::Spr_NoScrl(&AsOpening[2],x_+15*100,y_+35*100,PRIO_SYSTEM-1,ATR_DFLT,ARGB_DFLT,1.0f);
		viiDraw::Spr_NoScrl(&AsOpening[2],x_-15*100,y_+35*100,PRIO_SYSTEM-1,ATR_DFLT|ATR_FLIP_X,ARGB_DFLT,1.0f);
	}

	if(timer_ == 230){
		viiEff::Set(EFF_CRUISE_MISSILE,x_+15*100,y_+35*100,NULL);
		viiEff::Set(EFF_CRUISE_MISSILE,x_-15*100,y_+35*100,NULL);

		for(Sint32 i=0;i<10;i++){
			viiEff::Set(EFF_CRUISE_MISSILE,16000+(200+30*i)*100,y_+viiSub::Rand()%35*100,NULL);
			viiEff::Set(EFF_CRUISE_MISSILE,16000-(200+30*i)*100,y_+viiSub::Rand()%35*100,NULL);
		}
		free.w[0] = 1;
	}

	if(y_ <= -150*100){
		exist_ = gxFalse;
	}

}

void CEffect::op_cruise_missile()
{
	//巡航ミサイル

	if(timer_ == 0){
		free.w[0] = 10000;
		rot = 0;
		free.w[1] = viiSub::Rand()%5;
		free.w[2] = 255;
		xx_ = 0;
		free.w[3] = (16000-x_)/150;
	}

	if(free.w[0] > 0){
		free.w[0] -= 200;
		if(free.w[0] <= 1000){
			free.w[0] = 1000;
			free.w[2] -= 4;
			if(free.w[2] <= 0) free.w[2] = 0;
		}
	}

	xx_ += free.w[3];
	if(free.w[1] >0){
		free.w[1] --;
	}else{
		x_ += (16000-x_+xx_)/100;
		y_ += (8000-y_)/100;
	}

	Sint32 col = free.w[2];
	float scl = free.w[0]/10000.0f;
	rot +=2;
	viiDraw::Spr_NoScrl(&AsOpening[2],x_,y_,PRIO_SYSTEM-2,ATR_DFLT,ARGB(col,0xff,0xff,0xff),scl,rot);

	if(col==0){
		exist_ = gxFalse;
	}

}

void CEffect::op_grahamb_ship()
{
	//------------------------------
	//グラハム号
	//------------------------------

	if(timer_ == 0){
		x_*=100;
		y_*=100;
		xx_ = 3+viiSub::Rand()%10;
	}

	x_ += xx_;
	y_ += (9000-y_)/100;

	viiDraw::Spr_NoScrl(&AsOpening[3],x_,y_,PRIO_ENEMY,ATR_DFLT,ARGB_DFLT);

	if(timer_ >= 480){
		exist_ = gxFalse;
	}
}


void CEffect::OpeningString()
{
	//----------------------------------
	//文字表示
	//----------------------------------
	gxSprite *p;
	Sint32 col,add;

	if(timer_ == 0){
		switch(opt[0]){
		case 0:
			pos.sx = 100;
			free.w[0] = 0xff;
			break;
		case 1:
		case 2:
			pos.mx = 1;
			pos.sx = 100;
			pos.sy = 100;
			break;
		case 20:
		case 21:
		case 22:
		case 23:
			free.w[0] = 0xff;
			pos.mx = 2000;
			pos.sx = 100;
			break;

		case 30:
			free.w[0] = 0xff;
			pos.sx = 0;
			break;
		case 100:
			pos.sx = 100;
			break;
		}
	}
	col = free.w[0];
	if(col < 0) col = 0;

	Sint32 argb = 0xffffffff;
	switch(opt[0]){
	case 0:	//PUSH BUTTON
		argb = 0xffffffff;
		if(opt[1] == 0) argb = viiSub::GetRandColor(0xF0);//0xffffa000;
		GGX_Putsprite(&TitleSpr[1],x_,y_,PRIO_BG3+1,ATR_ALPHA_PLUS,argb);
		exist_ = gxFalse;
		break;

	case 1:	//MANUAL PLAY選択
		p = &TitleSpr[1];
		PutSpriteDX(x_,y_,PRIO_BG3+1,
			p->page,	p->u,	p->v,	p->w,	p->h,	p->cx,	p->cy,	ATR_DFLT,viiSub::GetRandColor(0xF0),//ARGB_DFLT,
			1.0f,pos.sx/75.0f,pos.sy/100.0f );
		pos.sx += pos.mx;//5;
		pos.sy -= 10;
		pos.mx +=2;
		if(pos.sy <= 0) exist_ = gxFalse;
		break;
	case 2:	//AUTOPLAY選択
		p = &TitleSpr[7];
		PutSpriteDX(x_,y_+24,PRIO_BG3+1,
			p->page,	p->u,	p->v,	p->w,	p->h,	p->cx,	p->cy,	ATR_DFLT,ARGB_DFLT,
			1.0f,pos.sx/75.0f,pos.sy/100.0f );
		pos.sx += pos.mx;//5;
		pos.sy -= 10;
		pos.mx +=2;
		if(pos.sy <= 0) exist_ = gxFalse;
		break;

	case 20:	//MISSION START
	case 21:	//TRAINING
	case 22:	//AS SELECT
	case 23:	//OPTION
		if(get_sw(1)) {
			GGX_Putsprite(&TitleSpr[opt[0]-20+2],x_-pos.mx/100,y_,PRIO_BG3+5,ATR_DFLT,0xffffffff ,pos.sx/100.0f);
			if(pos.sx <100) pos.sx += 2;
		}else{
			GGX_Putsprite(&TitleSpr[opt[0]-20+2],x_-pos.mx/100,y_,PRIO_BG3+5,ATR_ALPHA_PLUS,ARGB( (0xff-col),0xA0,0xA0,0xA0) ,pos.sx/100.0f);
			if(pos.sx >= 80) pos.sx -= 4;
		}
		add = pos.mx/10;
		if(add < 10) add = 0;
		pos.mx -= add;
		free.w[0] -= 8;
		if(free.w[0] <= 0x10) free.w[0] = 0x10;
		break;

	case 30:
		//-----------------------------
		//カーソル
		//-----------------------------
		p = &TitleSprCursor[0];

		PutSpriteDX(x_,y_+2,PRIO_BG3+4,
			p->page,	p->u,	p->v,	p->w,	p->h,	p->cx,	p->cy,	ATR_ALPHA_PLUS,ARGB( (20*col/100),0xff,0xff,0xff),
			0.0f,pos.sx/15.0f,1.5f );

		PutSpriteDX(x_,y_+2,PRIO_BG3+4,
			p->page,	p->u,	p->v,	p->w,	p->h,	p->cx,	p->cy,	ATR_ALPHA_PLUS,ARGB( (20*col/100),0xff,0xff,0xff),
			0.0f,(pos.sx-viiSub::Rand()%30)/15.0f,1.5f );

		PutSpriteDX(x_,y_+2,PRIO_BG3+4,
			p->page,	p->u,	p->v,	p->w,	p->h,	p->cx,	p->cy,	ATR_ALPHA_PLUS,ARGB( 0x40,0xff,0xff,0xff),
			0.0f,(pos.sx-viiSub::Rand()%30)/30.0f,1.5f );

		if(pos.sx <  100) pos.sx += 8;
		if(pos.sx >= 100) pos.sx = 100;

		if(get_sw(0)){
			if(col == 0){
				exist_ = gxFalse;
				return;
			}
			free.w[0] -= 16;
		}
		break;

	case 100:
		//-----------------------------
		//タイトル
		//-----------------------------
		PutSpriteDX(
			160,120,PRIO_SYSTEM-1,
			TPG_ETC,0,0,320,240,160,120,
			//オプション
			ATR_ALPHA_PLUS,0x20ffffff,opt[1]*1.0f,pos.sx/100.0f,pos.sx/100.0f);
		pos.sx += 8;
		break;

	case -1:
		GGX_Putsprite(&TitleSpr[1],x_,y_,PRIO_BG3+1,ATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff) ,pos.sx/100.0f);
		pos.sx +=4;
		free.w[0] -= 2;
		if(col == 0) exist_ = gxFalse;
		break;
	}

}


void CEffect::warning_box()
{
	//------------------------------------
	//警告用ワーニングBOX
	//------------------------------------
	if(timer_ == 0){
		free.w[0] = 0;
	}

	Sint32 dst = get_valken_dist(x_,y_);

	if(dst <= 92*100) {
		if(free.w[0] <= 30) {
			free.w[0] ++;
		}else{
			gStageValue [0] = 1;
		}
	}
}

