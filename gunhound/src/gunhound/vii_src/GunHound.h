#ifndef _GUNHOUND_H_
#define _GUNHOUND_H_

#define GUNHOUND	//ガンハウンド用

//#define USE_AMS

#ifdef GX_RELEASE

	#ifdef _TRIAL_VERSION_
	#define MUTEKI_ON
	#endif

#else

//	#define MUTEKI_ON

#endif	//_MASTER_VERSION_




#include "../DefSound.h"
#include "../DefTexture.h"


enum {

	enHitPointHard   = 200,	//2016 300->200に
	enHitPointHigh   = 150, //2016 200->150に
	enHitPointMiddle = 100,

	enHitPointBossSpider  = enHitPointHigh*12,
	enHitPointBossBombShip= enHitPointHigh*2,
	enHitPointBossSnakeHead   = enHitPointHigh*3,
	enHitPointBossSnakeBody   = enHitPointHigh*3,
	enHitPointBossChicken = enHitPointHigh*8,
	enHitPointBossOrganiz = enHitPointHigh*5,

	enHitPointShipPolymerCannonMain = enHitPointHigh*3,	//ニュートロンポリマー砲

	enHitPointShipSmallCannon 		= enHitPointMiddle*2,//2,	//でかほうだい
	enHitPointShipSubCannon 		= enHitPointHigh*2,//4,	//でかほうだい
	enHitPointShipPanel				= enHitPointHigh*5,//5,	//パネル
	enHitPointShipSubCoreBefore		= enHitPointHigh*2,//4,	//サブコアA
	enHitPointShipSubCoreAfter		= enHitPointHigh*2,//3,	//サブコアB
	enHitPointShipPolymerCannonCore = enHitPointHigh*14,//24,//ファイナルコア

	enHitPointDefault  = 1,

	enAtkPointBazooka    = 35,	//バズーカ級
	enAtkPointBomb       = 25,	//爆弾級
	enAtkPointMissile    = 20,	//ミサイル級
	enAtkPointRyudan     = 15,	//榴弾
	enAtkPointGun        = 5,	//マシンガン
	enAtkPointMachineGun = 3,	//マシンガン

	enAtkMicroMissile   = 30,	//マイクロミサイル

	enDefPointBazooka   = enAtkPointBazooka,	//バズーカ級
	enDefPointMissile   = enAtkPointMissile,	//ミサイル級
	enDefPointBomb      = enAtkPointBomb,	//爆弾級
	enDefPointRyudan    = enAtkPointRyudan,
	enDefPointGun       = enAtkPointGun,	//マシンガン５発

	enHitPointYusouHeli = enAtkPointRyudan*5,

	enDefPointZakoBS     = enAtkPointGun*16,//マシンガン16発
	enDefPointZakoL      = enAtkPointGun*8,	//マシンガン８発
	enDefPointZakoM      = enAtkPointGun*5,	//マシンガン５発
	enDefPointZakoS      = enAtkPointGun*2,	//マシンガン２発
	enDefPointMinimum    = enAtkPointGun,	//マシンガン１発

	HP_ARMORCLASS_BOSS = 250,
	HP_ARMORCLASS_ENEUNIT   = 120,
	HP_ARMORCLASS_ENEPISTON = 50,
	HP_ARMORCLASS_ZOAFRAME = 220,
	HP_ARMORCLASS_BUILDBORG = 200,
	HP_ARMORCLASS_SHUMERTZ_BOSS = 200,
	HP_ARMORCLASS_TWEI = 250,
	HP_ARMORCLASS_GAROISH_A = 100,
	HP_ARMORCLASS_GAROISH_B = 150,
	HP_ARMORCLASS_BRANDISH      = 150,
	HP_ARMORCLASS_BRANDISH_ARM  = 80,
	HP_ARMORCLASS_BOSS_CONTROLROOM = 200,
	HP_ARMORCLASS_BOSS_C = 100,
	HP_ARMORCLASS_MARSENGINE = 150,
	HP_ARMORCLASS_REBENDICK  = 32,
	HP_ARMORCLASS_MIDDLE_BOSS_A = 42,
	HP_ARMORCLASS_MIDDLE_BOSS_B = 62,
	HP_ARMORCLASS_MIDDLE_BOSS_C = 82,
	HP_ARMORCLASS_S = 32,
	HP_ARMORCLASS_AA = 24,
	HP_ARMORCLASS_A = 16,
	HP_ARMORCLASS_B =  8,
	HP_ARMORCLASS_BC = 4,
	HP_ARMORCLASS_C =  1,
};
enum {
	enLevelNumGocho = 0,
	enLevelNumGunso,
	enLevelNumJokyuGunso,
	enLevelNumSyoui,
	enLevelNumChui,
	enLevelNumTaii,
	enLevelNumSyosa,
	enLevelNumChusa,
	enLevelNumTaisa,
	enLevelNumMax,
};

enum {
	//システム変更系

	enHiddenItemStageSelect,	//シナリオ中で自由にステージセレクトできる機能

	enHiddenItemMission1,		//ステージ１クリア（実装）
	enHiddenItemMission2,		//ステージ１クリア（実装）
	enHiddenItemMission3,		//ステージ２クリア（実装）
	enHiddenItemMission4,		//ステージ３クリア（実装）
	enHiddenItemMission5,		//ステージ４クリア（実装）
	enHiddenItemViewEnding,		//エンディングモード（実装）	→全ステージクリア後オープン

	//武器追加系

	enHiddenItemAddWeapon3Way,

	enHiddenItemAddWeaponShotGun,
	enHiddenItemAddWeaponNapalmGun,

	enHiddenItemAddWeaponGattling,
	enHiddenItemAddWeaponIronCrow,
	enHiddenItemAddWeaponZgokCrow,
	enHiddenItemAddWeaponShieldArm,

	enHiddenItemAddWeaponLinearCannon,
	enHiddenItemAddWeaponAirGun,

	enHiddenItemAddWeaponDiverAntenna,	//ダイバーアンテナ
	enHiddenItemAddWeaponGrenade,		//グレネード

};

//---------------------------------------
//セーブデータ構造体
//---------------------------------------
enum {
	enWeaponNone=-1,

	enWeaponMachineGun=0,
	enWeapon3Way,

	enWeaponArmHardBrow,
	enWeaponArmGattling,
	enWeaponArmZgokCrow,
	enWeaponArmStrikeCrow,
	enWeaponArmShieldArm,

	enWeaponBazooka,
	enWeaponShotGun,
	enWeaponNapalm,

	enWeaponPodMissile,
	enWeaponCannonLinar,
	enWeaponCannonGun,

	enWeaponGrenade,	//ver1.05で追加

	enWeaponMax,
};


typedef struct StHoundConfig
{
	enum {
		enOptionMax = 128
	};
	StHoundConfig()
	{
		//初期化
		sMasterMainShot = enWeaponMachineGun;
		sMasterSubShot  = enWeaponArmHardBrow;
		sSlaveMainShot  = enWeaponBazooka;
		sSlaveSubShot   = enWeaponPodMissile;
		sBaseColor = 0;
		sArmorColor = 0;
		sNoArmor = 0;
		for(Sint32 ii=0;ii<enOptionMax;ii++)
		{
			sDummy[ii] = 0;
		}
	}

	//ショット関連
	Sint32 sMasterMainShot;
	Sint32 sMasterSubShot;
	Sint32 sSlaveMainShot;
	Sint32 sSlaveSubShot;

	//ボディカラー
	Sint32 sBaseColor;
	Sint32 sArmorColor;
	Sint32 sNoArmor;

	Sint32 sDummy[enOptionMax-1];

}StHoundConfig;

class CHoundSaveData
{
enum {
	enHiddenSize  =4,
};
public:
	Sint32 m_sClear[32];
/**/	Sint32 sRandData1;
	Uint32 m_uTotalScore;
/**/	Sint32 sRandData2;
	Uint32 m_uLevel;
/**/	Sint32 sRandData3;
	Uint32 m_uCrc;
/**/	Sint32 sRandData4;
/**/	Sint32 sRandData5;
	Sint32 m_sDummy[32];
	Sint32 m_sHidden[enHiddenSize];	//32アイテム*4=128

	//ハウンドのコンフィグデータ
	StHoundConfig m_StConfig;

	CHoundSaveData()
	{
		for(Sint32 ii=0;ii<8;ii++)
		{
			m_sClear[ii] = 0;
		}
		m_uLevel = 0;
		m_uTotalScore = 0;
		m_uCrc = 0;

		//隠しフラグ制御

		for(Sint32 jj=0;jj<enHiddenSize;jj++)
		{
			m_sHidden[jj] = 0x00000000;
		}
	}

	~CHoundSaveData()
	{

	}

	void Save();
	void Load();
	void UpdateHighScore(Uint32 score);

	gxBool IsHiddenOpen(Sint32 n)
	{
		Sint32 j=n/32;
		n = n%32;

		return (m_sHidden[j]&(1<<n))? gxTrue : gxFalse;
	}
	void SetHiddenOpen(Sint32 n)
	{
		Sint32 j=n/32;
		n = n%32;

		m_sHidden[j] |= (1<<n);
	}

	void AllHiddenOpen()
	{
		for(Sint32 ii=0;ii<enHiddenSize;ii++)
		{
			m_sHidden[ii] = 0xffffffff;
		}
	}

private:

};

extern CHoundSaveData g_CHoundSaveData;

/*
enum {
	//プライオリティ設定
	PRIO_BG0 = 100,	//★
	PRIO_BG1 = 110,		//空など
	PRIO_BG2 = 120,		//壁後
	PRIO_ENEMY =130 ,
	PRIO_ENE_B =130,
	PRIO_VERSIS_B =140,
	PRIO_PLAYER =150,
	PRIO_ENE_F =160,
	PRIO_VERSIS_F =170,
	PRIO_BG3 =180,		//壁前
	PRIO_EFFECT =190,
	PRIO_ENE_BULLET =195,
	PRIO_BLACKTONE=200,
	PRIO_SYSTEM,
	PRIO_SYSTEMWINDOW,
	PRIO_FADE,
	PRIO_FADEOVER,
	PRIO_DEBUG_TIP,
	//PRIO_MAX -10 //ダッシュボード
	//PRIO_MAX -5  //HD-MASK
	PRIO_DEBUG = (PRIO_MAX-2),
};
*/

#include "Gunhound/CockPit/CCockpit.h"

#endif
