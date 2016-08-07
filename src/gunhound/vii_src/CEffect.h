enum{
	//オブジェ番号
	OBJ_TEST=0,		//0:

	//攻撃
	EFF_VULCAN,
	EFF_PUNCH,
	EFF_MISSILE,
	EFF_LASER,
	EFF_NAPALM,
	EFF_LASER_PARTS,
	EFF_LEYNAPALM,
	EFF_GRENADE,
	EFF_D_D1,
	EFF_D_D2,
	EFF_GRENADE_EFFECT,
	EFF_MICROMISSILE,
	EFF_THEXDER,
	EFF_OPTIONBIT,
	EFF_SHORTLASER,
	EFF_MELONBOMB,
	EFF_HEATHAWK,

	//効果
//	EFF_BOMB,
	EFF_BOMB_PARTS,
	EFF_BIGBOMB,
	EFF_BOMB_SMALL,
	EFF_BIGBOMB_PARTS,
	EFF_LASERBOMB_PARTS,
	EFF_BURNIA_SMOKE,
	EFF_DASH_SMOKE,
	EFF_MISSILE_SMOKE,
	EFF_PARTS_BREAK,
	EFF_RING,
	EFF_BOMBBEAM,
	EFF_BURNIA,

	//敵弾など
//	EFF_ENE_BULLET,
	EFF_ENE_BULLET2,
//	EFF_ENE_BOMB,
	EFF_ENE_MISSILE,
	EFF_ENE_LASERCANNON,
	EFF_ENE_PHOTON,
	EFF_ENE_BOMB2,
//	EFF_AIRCANNON,
	ENE_PROPERA_MISSILE,
	EFF_BURNIA_DAMAGE,
	EFF_LASERCANNON,
	EFF_TWINCANNON,
	EFF_ENERGYBEAM,
	EFF_SPRINE_THUNDER,
	EFF_REFLECTBULLET,
	EFF_AIMINGLASERBIT,
	EFF_BIGCANNON,
	EFF_GATRINGBIT,

	EFF_BOOSTER_TOSS,
	EFF_BOOSTER_OFF,

	EFF_YAKKYOU,
	EFF_FIREFLOWER,
	EFF_MAZZLE,
	EFF_HITFLASH,
	EFF_HITFLASH_ENE,
	EFF_BOMBFLASH_ENE,

	EFF_SHUTTLE_BURNIA,
//	EFF_SHOT_CANNON,
	EFF_SHOT_BITLASER,

	EFF_DAMAGE_PARTS,
	EFF_RELEASE_PARTS,
	EFF_MAP_DAMAGE_PARTS,

	EFF_HUMAN,
	EFF_HUMAN_ENGINEER,
	EFF_HUMAN_ENGINEER2,
	EFF_HUMAN_ENGINEER3,
	EFF_HUMAN_DAMAGE,
	EFF_BREAK_THUNDER,
	ENESHIP_LAUNCH,

	EFF_MASATSU_A,
	EFF_MASATSU_B,
	EFF_CLOUD_A,
	EFF_CLOUD_B,
	EFF_CLOUD_C,
	EFF_BROKENPARTS,
	EFF_BROKENPARTSDELUXE,
	EFF_BROKENPARTS_COLONY,
	EFF_BROKEN_ROCK,
	EFF_ENERGYBALL,			//エネルギー充填中エフェクト
	EFF_COLONY_SHUTTER,		//コロニーのシャッター
	EFF_SPRINELASER,

//	EFF_SANDSTORM,
	EFF_SANDROCK,

	EFF_BRIZZARD,		//雪の天候エフェクト
	EFF_WEATHER_RAIN,	//雨の天候エフェクト
	EFF_SNOWSMOKE,		//雪煙
	EFF_DUNGEONROCK,	//ダンジョン内岩

	EFF_AIRBREATH,		//汎用エアー
	EFF_KUNOICHI,		//隠しキャラ
	EFF_ARMY_GUN,		//アーミー１
	EFF_ARMY_SNIPER,	//アーミー２
	EFF_ARMY_SARGENT,	//アーミー３
	EFF_ARMY_CANNON,	//アーミー４
	EFF_ARMY_JEEP,		//アーミー５
	EFF_ARMY_TANK,		//アーミー６

	//ＮＰＣキャラ
	EFF_RABBIT,
	EFF_FALL_A,
	EFF_FALL_B,
	EFF_FALL_C,
	EFF_VERSIS,
	EFF_POWER_CHIP,
	EFF_WEAPON_CHIP,
	EFF_HP_CHIP,
	EFF_LAUNCH_NPVALKEN,
	EFF_RUNNING_VERDARK,
	EFF_RUNNING_SEIJIKA,
	EFF_RUNNING_SHERMARK,
	EFF_KILLMYSELF_SHERMARK,
	EFF_ENDOFSOLDIER,

	EFF_OPENING_STRING,
	EFF_WARNINGBOX,

	//オープニング用
	EFF_MISSILE_SHIP,
	EFF_CRUISE_MISSILE,
	EFF_GRAHAMB,
};


class CEffect
{
private:
	gxBool is_init() {
		return (timer_ == 0)? gxTrue : gxFalse;
	}
	Sint32 dir,rot;
	Sint32 timer_;
	CObj *pObj;

//	tHitTest hit;
//	tHitTest kurai;

//ZANTEI	CHitCollision m_HitAtari;
//ZANTEI	CHitCollision m_HitKurai;

	void *pMemory[10];
public:
	Sint32 x_,y_;
	Sint32 xx_,yy_;
	Sint32 type_;
	OBJ_POS_T pos;
	CEffect();
	~CEffect();
	void action();
	Sint32 opt[10];
	Sint32 seq_;
	gxBool exist_;
	Sint32 sw_;
	gxSprite Sprite;
	union free {
		char  b[32];
		short w[16];
		Sint32   l[8];
	}free;

	gxBool get_sw(Sint32 n){
		return (sw_&(0x01<<n))? gxTrue : gxFalse;
	}

	void sw_on(Sint32 n){
		sw_ |= (0x01<<n);
	}

	void sw_off(Sint32 n){
		sw_ = sw_&~(0x01<<n);
	}

	//-----------------------------
	void test_obj();

	//-----------------------------
	//共通
	//-----------------------------
	void bomb();
	void ring();
	void bomb_parts();
	void laser_bomb_parts();
	void burnia_smoke();
	void versis_action();
	void vulcan();
	void punch();
	void missile();
		void missileLv1();
		void missileLv2();
		void missileLv3();
	void laser();
	void napalm();
	void missile_smoke();
	void laser_parts();
	void ley_napalm();
	void MicroMissile();
	void thexder();
	void OptionBit();
	void ShortLaser();
	void melonbomb();
	void heathawk();

	void grenade();
	void grenade_effect();
	void defence_discharger();
	void defence_discharger2();

	void bigbomb();
	void bigbomb_parts();

	void eff_burnia();
	void roller_dashsmoke();
	void ene_bullet();
	void ene_bullet2();
	void eneReflectBullet();
	void eneAimingLaserBit();
	void eneBigCannon();
	void eneGatringBit();
	void ene_bomb();
	void ene_missile();
	void ene_bomb2();
//	void ene_aircannon();
	void ene_burnia_damage();
	void parts_break();
	void shuttle_burnia();
	void booster_toss();
	void lasercannon();
	void twin_cannon();
	void ene_laser();
	void energybeam();
	void item_chip();
	void sprine_thunder();

	void bomb_hitflash_ene();
	void tama_hitflash_ene();

	void ene_bitlaser();
	void ene_shot_cannon();
	void ene_shot_photon();

	void damage_parts();
	void release_parts();
	void map_breakparts();

	void human();
	void human_damage();
	void engineer();
	void engineer2();
	void engineer3();
	void break_thunder();
	void bomb_small();
	void eneship_aslaunch();
	void ene_propera_missile();

	void masatsu_large();
	void masatsu_small();
	void cloud_yoko();
	void cloud_tate();
	void cloud_shuttle();
	void broken_parts();
	void broken_parts_deluxe();
	void broken_parts_colony();
	void broken_rock();
	void energyball();
	void colony_shutter();
	void sprine_laser();

	void SandRock();
	void SandStorm();

	void Brizzard();
	void WeatherRain();
	void SnowSmoke();
	void DungeonRock();
	void AirBreath();

	void FallEffectA();
	void FallEffectB();
	void FallEffectC();
	void Rabbit();
	void OpeningString();
	void Kunoichi();
	void ArmyGun();
	void ArmySniper();
	void ArmySargent();
	void ArmyCannon();
	void ArmyJeep();
	void ArmyTank();

	void bombbeam();

	void RunningVerdark();
	void RunningSeijika();
	void RunningShermark();
	void KillMyselfShelmark();
	void EndOfSoldier();
	//-----------------------------
	//VII専用
	//-----------------------------
	void yakkyou();
	void FireFlower();
	void vulcan_mazzleflash();
	void vulcan_hitflash();
	void booster_off();
	void launch_other_as();

	void warning_box();
	void StageClear();

	//-----------------------------
	//オープニング専用
	//-----------------------------
	void op_missile_ship();
	void op_cruise_missile();
	void op_grahamb_ship();

	void init();

};

class CEffManager
{
private:
	enum{
		EFF_MAX = 1024,
	};
	CEffect *pEff[EFF_MAX];
//	CEffect *m_pEffectRescue;	//エフェクト生成数ＭＡＸを超えた場合の特別

public:

	CEffManager()
	{
		for(Sint32 i=0;i<EFF_MAX;i++){
			pEff[i] = NULL;
		}
	}
	~CEffManager()
	{
		for(Sint32 i=0;i<EFF_MAX;i++){
			if(pEff[i]) delete pEff[i];
			pEff[i] = NULL;
		}
	}

	void action(){
		Sint32 cnt = 0;
		Sint32 i;
		for(i=0;i<EFF_MAX;i++){
			if(pEff[i]) {
				cnt ++;
				pEff[i]->action();
//				if(pEff[i]->exist_ == gxFalse){
//					delete pEff[i];
//					pEff[i] = NULL;
//				}
			}
		}

		for(i=0;i<EFF_MAX;i++){
			if(pEff[i]) {
				if(pEff[i]->exist_ == gxFalse){
					delete pEff[i];
					pEff[i] = NULL;
				}
			}
		}
	}

	void clear_all()
	{
		//すべての敵を消去する
		for(Sint32 i=0;i<EFF_MAX;i++) {
			if( pEff[i] ) delete pEff[i];
			pEff[i] = NULL;
		}
		//XMAS_VERSION_SNOW_EFFECT_REQUEST
	}

	CEffect* set_effect(Sint32 type,Sint32 x,Sint32 y,Sint32 *opt=NULL , gxSprite *pSpr = NULL );

};

//弾丸の跳ね返り方向を制御できる
gxBool bullet_reflect(CEffect *p);

//もっとも近い敵をサーチする
gxBool get_nearly_enemypos(Sint32 x,Sint32 y,Sint32 &px,Sint32 &py);

