#ifndef _DEFTEXTURE_H_
#define _DEFTEXTURE_H_

enum {
	enTexturePage00=16*0,//常駐256
	enTexturePage01=16*1,//常駐512
	enTexturePage02=16*2,//フリー256
	enTexturePage03=16*3,//フリー256
	enTexturePage04=16*4,//フリー256/512
	enTexturePage05=16*5,//フリー256/512
	enTexturePage06=16*6,//フリー256/512
	enTexturePage07=16*7,//フリー256/512
};

enum {
enCEneH0413Fannel=101,
enCEneH0002Commanch=102,
enCEneH0003Vorzoy=103,
enCEneH0004Tachikoma=104,
enCEneH0005TransHeli=105,
enCEneH0006UGTortika=106,
enCEneH0007YusouHeli=107,
enCEneH0101HeavyTank=132,
enCEneH0102NightSpider=133,
enCEneH0103ClubGunner=134,
enCEneH0104LightTank=135,
enCEneH0105TankGuard=136,
enCEneH0106Yagura=137,
enCEneH0107Walker=138,
enCEneH0108Hovered=139,
enCEneH0201VorzoyFly=140,
enCEneH0202VorzoyWP=141,
enCEneH0203Serpent=142,
enCEneH0204Grazzi=143,
enCEneH0205Jourikusen=144,
enCEneH0206Jourikusensya=145,
enCEneH0207TransShips=146,
enCEneH0301Gunpelly=147,
enCEneH0008GunTank=108,
enCEneH0303Valkylie=149,
enCEneH0306CapselMissile=166,
enCEneH0304Bee=150,
enCEneH0305Missile=151,
enCEneH0401MovingCannon=152,
enCEneH0008BloodSocker=153,
enCEneH0403Tarantura=154,
enCEneH0404GravityStar=155,
enCEneH0405Nancy=156,
enCEneH0406Carrymissile=157,
enCEneH0407Nautilus=158,
enCEneH0408Powerdsuit=159,
enCEneH0409SlideCannon=160,
enCEneH0410Wall=161,
enCEneH0411Stunbarrier=162,
enCEneH0501PlasmaMachine=163,
enCEneH0422StunBarrierTate=164,
enCEneH0503HoundFly=165,
enCEneH0504CrasterBom=166,
enCEneH0505CroudBom=167,
enCEneH0506EraserGun=168,
enCEneH0412nuclearengine=169,
enCEneH0006UGTortika2=170,
enCEneH0201VorzoyFlyAfter=171,

enCEneEx0001Cannon = 300,
enCEneEx0002Laser = 301,
enCEneEx0003ShotRight = 302,
enCEneEx0004Missile = 303,
enCEneEx0005Baz = 304,
enCEneEx0006Tank = 305,
enCEneEx0007Nozzle = 306,
enCEneEx0008BombRight = 307,
enCEneEx0008BombLeft  = 308,
enCEneEx0009Body      = 309,
enCEneEx0010Suihei    = 310,
enCEneEx0010SuiheiRight  = 311,
enCEneEx0003ShotLeft = 320,

};


enum enTexPage {

	//----------------------------------------
	//システム関連(page00)
	//----------------------------------------
	enTexPageDashBoard    = enTexturePage00+0,
	enTexPageSystemUI     = enTexturePage00+2,
	enTexPageSystemUI3    = enTexturePage00+3,

	enTexPageGunHoundBdy  = enTexturePage00+4,
	enTexPageGunHoundArm  = enTexturePage00+5,
	enTexPageDmyHound     = enTexturePage00+6,
	enTexPageLineHound    = enTexturePage00+7,

	enTexPageFace      =enTexturePage00+8,
	enTexPageFace00    =enTexturePage00+8,
	enTexPageFace01    =enTexturePage00+9,
	enTexPageString    =enTexturePage00+10,
	enTexPageFace03    =enTexturePage00+11,

	enTexPageEffect1   =enTexturePage00+12,
	enTexPageEffect2   =enTexturePage00+13,
	enTexPageEffect3   =enTexturePage00+14,
	enTexPageEffect4   =enTexturePage00+15,

	//共通の敵

	enTexPageEnemyCommon01 = enTexturePage01+0,
	enTexPageEnemyCommon02 = enTexturePage01+1,
	enTexPageEnemyCommon03 = enTexturePage01+2,
	enTexPageEnemyCommon04 = enTexturePage01+3,

	enTexPageEnemyCommon05 = enTexturePage01+4,
	enTexPageEnemyCommon06 = enTexturePage01+5,
	enTexPageEnemyCommon07 = enTexturePage01+6,
	enTexPageEnemyCommon08 = enTexturePage01+7,

	//ステージ固有の敵

	enTexPageEnemyStage01 = enTexturePage01+8,
	enTexPageEnemyStage02 = enTexturePage01+9,
	enTexPageEnemyStage03 = enTexturePage01+10,
	enTexPageEnemyStage04 = enTexturePage01+11,
	enTexPageEnemyStage05 = enTexturePage01+12,
	enTexPageEnemyStage06 = enTexturePage01+13,
	enTexPageEnemyStage07 = enTexturePage01+14,
	enTexPageEnemyStage08 = enTexturePage01+15,

	//System2
	enTexPageFont         = enTexturePage02+0,
	enTexPageGrowMap      = enTexturePage02+2,
	enTexPageGrowObj      = enTexturePage02+6,
	enTexPageGrowUI       = enTexturePage02+7,

	enTexPageSetup        = enTexturePage05+0,

	//Stage Obj
	enTexPageBgObj1      = enTexturePage02+8,
	enTexPageBgObj2      = enTexturePage02+10,
	enTexPageBgObj3      = enTexturePage02+12,
	enTexPageBgObj4      = enTexturePage02+14,

	enTexPageBgRiver          = enTexPageBgObj1,
	enTexPageBgCloud          = enTexPageBgObj3,
	enTexPageSearchLight      = enTexPageBgObj4,

	//Logo
//	enTexPageSysLogo      = enTexturePage03+0,
//	enTexPageSysLogo      = enTexturePage03+1,
	enTexPageTitle        = enTexturePage03+2,
//	enTexPageTitle        = enTexturePage03+3,
	enTexPageScreenShot   = enTexturePage03+4,
//	enTexPageScreenShot   = enTexturePage03+5,

	enTexPageMainMenu     = enTexturePage03+8,
	enTexPageEyeCatch     = enTexturePage03+10,

	enTexPageBackGround = enTexturePage04,
};


enum {
	//旧バージョンとのつじつまあわせ用
	enTexPageEffectCommon01	=	enTexPageEffect1,
	enTexPageEffectCommon02	=	enTexPageEffect2,
	enTexPageEffectCommon03	=	enTexPageEffect3,
	enTexPageEffectCommon04	=	enTexPageEffect4,

	//ステージエフェクト	=	,
	enTexPageEffectStage01	=	enTexPageBgObj1,
	enTexPageEffectStage02	=	enTexPageBgObj2,
	enTexPageEffectStage03	=	enTexPageBgObj3,
	enTexPageEffectStage04	=	enTexPageBgObj4,

	//-----------------------------------------	=	,

	enTpgTitleLogo	=	enTexPageSysLogo,
	enTpgMainMenuA	=	enTexPageTitle,
	enTpgMainMenuB	=	enTexPageMainMenu,

	//-----------------------------------------	=	,
	//共通:売約済み	=	,
	//-----------------------------------------	=	,
	//0	=	,
/*	enTpgDashBoard	=	enTexPageDashBoard,*/
	enTpgFaceDisp	=	enTexPageFace00,
	//-----------------------------------------	=	,

	enTexPageStgEffWood	=	0,
	enTexPageSoldier	=	enTexPageEnemyCommon01,
	enTexPageEnemyGear01	=enTexPageEnemyStage02,

	enTexCEneH0413Fannel	=	enTexPageEnemyStage02,
	enTexCEneH0002Commanch	=	enTexPageEnemyCommon02,
	enTexCEneH0003Vorzoy	=	enTexPageEnemyCommon03,
	enTexCEneH0004Tachikoma	=	enTexPageEnemyCommon05,
	enTexCEneH0005TransHeli	=	enTexPageEnemyCommon06,
	enTexCEneH0006UGTortika	=	enTexPageEnemyCommon02,
	enTexCEneH0007YusouHeli	=	enTexPageEnemyCommon04,
	enTexCEneH0101HeavyTank	=	enTexPageEnemyStage04,
	enTexCEneH0102NightSpider	=	enTexPageEnemyStage05,
	enTexCEneH0103ClubGunner	=	enTexPageEnemyStage05,
	enTexCEneH0104LightTank	=	enTexPageEnemyStage02,
	enTexCEneH0105TankGuard	=	enTexPageEnemyCommon06,
	enTexCEneH0106Yagura	=	enTexPageEnemyStage02,
	enTexCEneH0107Walker	=	enTexPageEnemyCommon05,
	enTexCEneH0108Hovered	=	enTexPageEnemyCommon05,
	enTexCEneH0201VorzoyFly	=	enTexPageEnemyCommon02,
	enTexCEneH0202VorzoyWP	=	enTexPageEnemyCommon03,
	enTexCEneH0203Serpent	=	enTexPageEnemyStage08,
	enTexCEneH0204Grazzi	=	enTexPageEnemyStage08,
	enTexCEneH0205Jourikusen	=	0,
	enTexCEneH0206Jourikusensya	=	0,
	enTexCEneH0207TransShips	=	enTexPageEnemyStage02,
	enTexCEneH0301Gunpelly	=	enTexPageEnemyCommon07,
	enTexCEneH0008GunTank	=	enTexPageEnemyCommon05,
	enTexCEneH0303Valkylie	=	enTexPageEnemyStage08,
	enTexCEneH0306CapselMissile	=	0,
	enTexCEneH0304Bee	=	enTexPageEnemyCommon05,
	enTexCEneH0305Missile	=	enTexPageEnemyStage05,
	enTexCEneH0401MovingCannon	=	enTexPageEnemyStage05,
	enTexCEneH0008BloodSocker	=	enTexPageEnemyCommon03,
	enTexCEneH0403Tarantura	=	enTexPageEnemyStage03,
	enTexCEneH0404GravityStar	=	enTexPageEnemyStage02,
	enTexCEneH0405Nancy			=	enTexPageEnemyStage07,
	enTexCEneH0406Carrymissile	=	enTexPageEnemyStage07,
	enTexCEneH0407Nautilus		=	enTexPageBgObj2,
	enTexCEneH0408Powerdsuit	=	0,
	enTexCEneH0409SlideCannon	=	enTexPageEnemyStage01,
	enTexCEneH0410Wall	=	enTexPageEnemyStage02,
	enTexCEneH0411Stunbarrier	=	enTexPageEnemyStage01,
	enTexCEneH0501PlasmaMachine	=	enTexPageEnemyStage04,
	enTexCEneH0502Battletotika	=	enTexPageEnemyStage01,
	enTexCEneH0503Tapache	=	0,
	enTexCEneH0504CrasterBom	=	0,
	enTexCEneH0505CroudBom	=	0,
	enTexCEneH0506EraserGun	=	0,
	enTexCEneH0412nuclearengine	=	enTexPageBgObj2,
	enTexCEneH0206ShipEater = enTexPageEnemyStage06,

	enTexCEneEx0002Laser = enTexPageEnemyStage01,
	enTexCEneEx0003Shot = enTexPageEnemyStage02,
	enTexCEneEx0001Cannon = enTexPageEnemyStage03,
	enTexCEneEx0007Nozzle = enTexPageEnemyStage05,
	enTexCEneEx0004Missile = enTexPageEnemyStage02,
	enTexCEneEx0005Baz = enTexPageEnemyStage05,
	enTexCEneEx0006Tank = enTexPageEnemyStage06,
	enTexCEneEx0009Body = enTexPageEnemyStage07,
	enTexCEneEx0010Suihei = enTexPageEnemyStage02,

	enTexCEneEx0003ShotRight = enTexPageEnemyStage02,

	enTexPageLastBoss = enTexPageSearchLight,

};

#endif
