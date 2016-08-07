gxSprite stSprBurn[]={
	//-------------------------------------
	//バーニア
	//-------------------------------------

	{enTexPageDmyHound, 0   ,192,32,64,24,40},		//0:アンテナ
	{enTexPageDmyHound, 0   ,144,16,16,14,9},		//1:ノズル
	{enTexPageDmyHound, 0   ,160,32,32,28,16},		//2:ブースター
};

gxSprite stSprSubWeapon[]={
	//-------------------------------------
	//ショルダーサブウェポン
	//-------------------------------------

	{enTexPageGunHoundBdy, 232   ,128,24,32,16,16},		//0:ショルダーポッド
	{enTexPageGunHoundBdy, 232   ,160,24,40,16,32},		//1:リニアカノン
	{enTexPageGunHoundBdy, 232   ,208,24,48,16,32},		//2:３連装
	{enTexPageGunHoundBdy, 232   ,32 ,24,40,16,32},		//3:ガトリング

	{enTexPageGunHoundBdy, 232   ,112+48*0,24,48,20,32},		//0:ショルダーポッド
	{enTexPageGunHoundBdy, 232   ,112+48*1,24,48,16,30},		//1:リニアカノン砲
	{enTexPageGunHoundBdy, 232   ,112+48*2,24,48,16,30},		//2:未使用
};


gxSprite stSprMainWeapon[]=
{
	//このテーブルは必ず最後尾に追加すること

	//-------------------------------------
	//ガトリングガン
	//-------------------------------------
	{enTexPageGunHoundArm, 192,16*0,64,16,20,13},	//00:ガトリングガン１
	{enTexPageGunHoundArm, 192,16*1,64,16,20,13},	//00:ガトリングガン２
	{enTexPageGunHoundArm, 192,16*2,64,16,20,13},	//00:ガトリングガン３
	{enTexPageGunHoundArm, 192,16*3,64,16,20,13},	//00:ガトリングガン４

	//------------------------------

	{enTexPageGunHoundArm, 192,48+24*0,64,24,20,22},	//04:バズーカ
	{enTexPageGunHoundArm, 192,48+24*1,64,24,20,22},	//05:ナパーム
	{enTexPageGunHoundArm, 192,48+24*2,64,24,20,22},	//06:ショットガン
	{enTexPageGunHoundArm, 192,48+24*3,64,24,20,22},	//07:３WAY

	//------------------------------

	{enTexPageGunHoundArm, 0,0,1,1,0,0},	//08:グレネード

};


gxSprite stSprMech[]=
{
	//-------------------------------------
	//基本ボディ（フレーム）
	//-------------------------------------
	{enTexPageGunHoundBdy, 0*32,0*32,32,32,16,16},	//00:アーム
	{enTexPageGunHoundBdy, 1*32,0*32,32,32,16,24},	//01:ボディ
	{enTexPageGunHoundBdy, 2*32,0*32,32,32,13,13},	//02:レッグ手前
	{enTexPageGunHoundBdy, 3*32,0*32,32,32,13,13},	//03:レッグ奥
	{enTexPageGunHoundBdy, 4*32,0*32,32,32,13,13},	//04:ショルダー
	{enTexPageGunHoundBdy, 5*32,0*32,32,32,13,13},	//05:パンチ
	{enTexPageGunHoundBdy, 6*32,0*32,32,32,16,16},	//06:太もも手前
	{enTexPageGunHoundBdy, 7*32,0*32,32,32,16,16},	//07:太もも奥

	//-------------------------------------------------------------

};

gxSprite stSprBody[]=
{
	//-------------------------------------
	//基本ボディ（スキン）
	//-------------------------------------
	{enTexPageGunHoundBdy, 0*32,1*32,32,32,16,16},	//00:アーム
	{enTexPageGunHoundBdy, 1*32,1*32,32,32,16,24},	//01:ボディ
	{enTexPageGunHoundBdy, 2*32,1*32,32,32,13,13},	//02:レッグ手前
	{enTexPageGunHoundBdy, 3*32,1*32,32,32,13,13},	//03:レッグ奥
	{enTexPageGunHoundBdy, 4*32,1*32,32,32,16,13},	//04:ショルダー

	//-------------------------------------------------------------

	{enTexPageGunHoundBdy, 0*32,2*32,32,32,16,16},	//00:アーム
	{enTexPageGunHoundBdy, 1*32,2*32,32,32,16,24},	//01:ボディ
	{enTexPageGunHoundBdy, 2*32,2*32,32,32,13,13},	//02:レッグ手前
	{enTexPageGunHoundBdy, 3*32,2*32,32,32,13,13},	//03:レッグ奥
	{enTexPageGunHoundBdy, 4*32,2*32,32,32,16,13},	//04:ショルダー

	//-------------------------------------------------------------

	{enTexPageGunHoundBdy, 0*32,3*32,32,32,16,16},	//00:アーム
	{enTexPageGunHoundBdy, 1*32,3*32,32,32,16,24},	//01:ボディ
	{enTexPageGunHoundBdy, 2*32,3*32,32,32,13,13},	//02:レッグ手前
	{enTexPageGunHoundBdy, 3*32,3*32,32,32,13,13},	//03:レッグ奥
	{enTexPageGunHoundBdy, 4*32,3*32,32,32,16,13},	//04:ショルダー

	//-------------------------------------------------------------

	{enTexPageGunHoundBdy, 0*32,4*32,32,32,16,16},	//00:アーム
	{enTexPageGunHoundBdy, 1*32,4*32,32,32,16,24},	//01:ボディ
	{enTexPageGunHoundBdy, 2*32,4*32,32,32,13,13},	//02:レッグ手前
	{enTexPageGunHoundBdy, 3*32,4*32,32,32,13,13},	//03:レッグ奥
	{enTexPageGunHoundBdy, 4*32,4*32,32,32,16,13},	//04:ショルダー

	//-------------------------------------------------------------

	{enTexPageGunHoundBdy, 0*32,5*32,32,32,16,16},	//00:アーム
	{enTexPageGunHoundBdy, 1*32,5*32,32,32,16,24},	//01:ボディ
	{enTexPageGunHoundBdy, 2*32,5*32,32,32,13,13},	//02:レッグ手前
	{enTexPageGunHoundBdy, 3*32,5*32,32,32,13,13},	//03:レッグ奥
	{enTexPageGunHoundBdy, 4*32,5*32,32,32,16,13},	//04:ショルダー

	//-------------------------------------------------------------

	{enTexPageGunHoundBdy, 0*32,6*32,32,32,16,16},	//00:アーム
	{enTexPageGunHoundBdy, 1*32,6*32,32,32,16,24},	//01:ボディ
	{enTexPageGunHoundBdy, 2*32,6*32,32,32,13,13},	//02:レッグ手前
	{enTexPageGunHoundBdy, 3*32,6*32,32,32,13,13},	//03:レッグ奥
	{enTexPageGunHoundBdy, 4*32,6*32,32,32,16,13},	//04:ショルダー


	//BLACK 2010.05.23-------------------------------------------------------------

	{enTexPageGunHoundBdy, 0*32,7*32,32,32,16,16},	//00:アーム
	{enTexPageGunHoundBdy, 1*32,7*32,32,32,16,24},	//01:ボディ
	{enTexPageGunHoundBdy, 2*32,7*32,32,32,13,13},	//02:レッグ手前
	{enTexPageGunHoundBdy, 3*32,7*32,32,32,13,13},	//03:レッグ奥
	{enTexPageGunHoundBdy, 4*32,7*32,32,32,16,13},	//04:ショルダー
};

gxSprite stSprOption[]=
{
	//-------------------------------------
	//基本ボディ（アーマー）未使用
	//-------------------------------------
	{enTexPageGunHoundBdy, 0*32,2*32,32,32,16,16},	//00:アーム
	{enTexPageGunHoundBdy, 1*32,2*32,32,32,16,24},	//01:ボディ
	{enTexPageGunHoundBdy, 2*32,2*32,32,32,13,13},	//02:レッグ手前
	{enTexPageGunHoundBdy, 3*32,2*32,32,32,13,13},	//03:レッグ奥
	{enTexPageGunHoundBdy, 4*32,2*32,32,32,16,13},	//04:ショルダー

	//-------------------------------------------------------------


};


//------------------------------------
//鎧関連
//------------------------------------

gxSprite stSprArmorArm[]=
{
	{enTexPageGunHoundArm, 0*32,0*32,32,32,16,16},	//01:アーム
	{enTexPageGunHoundArm, 0*32,1*32,32,32,16,16},	//01:アーム
	{enTexPageGunHoundArm, 0*32,2*32,32,32,16,16},	//01:アーム
	{enTexPageGunHoundArm, 0*32,3*32,32,32,16,16},	//01:アーム
	{enTexPageGunHoundArm, 0*32,4*32,32,32,16,16},	//01:アーム
	{enTexPageGunHoundArm, 0*32,5*32,32,32,16,16},	//01:アーム

	//BLACK
	{enTexPageGunHoundArm, 0*32,7*32,32,32,16,16},	//01:アーム
};

gxSprite stSprArmorBody[]=
{
	{enTexPageGunHoundArm, 1*32,0*32,32,32,16,24},	//02:ボディ
	{enTexPageGunHoundArm, 1*32,1*32,32,32,16,24},	//02:ボディ
	{enTexPageGunHoundArm, 1*32,2*32,32,32,16,24},	//02:ボディ
	{enTexPageGunHoundArm, 1*32,3*32,32,32,16,24},	//02:ボディ
	{enTexPageGunHoundArm, 1*32,4*32,32,32,16,24},	//02:ボディ
	{enTexPageGunHoundArm, 1*32,5*32,32,32,16,24},	//02:ボディ

	//BLACK
	{enTexPageGunHoundArm, 1*32,7*32,32,32,16,24},	//02:ボディ
};

gxSprite stSprArmorLeg1[]=
{
	{enTexPageGunHoundArm, 2*32,0*32,32,32,13,13},	//02:レッグ手前
	{enTexPageGunHoundArm, 2*32,1*32,32,32,13,13},	//02:レッグ手前
	{enTexPageGunHoundArm, 2*32,2*32,32,32,13,13},	//02:レッグ手前
	{enTexPageGunHoundArm, 2*32,3*32,32,32,13,13},	//02:レッグ手前
	{enTexPageGunHoundArm, 2*32,4*32,32,32,13,13},	//02:レッグ手前
	{enTexPageGunHoundArm, 2*32,5*32,32,32,13,13},	//02:レッグ手前

	{enTexPageGunHoundArm, 2*32,7*32,32,32,13,13},	//02:レッグ手前
};

gxSprite stSprArmorLeg2[]=
{
	{enTexPageGunHoundArm, 3*32,0*32,32,32,13,13},	//03:レッグ奥
	{enTexPageGunHoundArm, 3*32,1*32,32,32,13,13},	//03:レッグ奥
	{enTexPageGunHoundArm, 3*32,2*32,32,32,13,13},	//03:レッグ奥
	{enTexPageGunHoundArm, 3*32,3*32,32,32,13,13},	//03:レッグ奥
	{enTexPageGunHoundArm, 3*32,4*32,32,32,13,13},	//03:レッグ奥
	{enTexPageGunHoundArm, 3*32,5*32,32,32,13,13},	//03:レッグ奥

	{enTexPageGunHoundArm, 3*32,7*32,32,32,13,13},	//03:レッグ奥
};

gxSprite stSprArmorShoulder[]=
{
	{enTexPageGunHoundArm, 4*32,0*32,32,32,16,13},	//05:ショルダー
	{enTexPageGunHoundArm, 4*32,1*32,32,32,16,13},	//05:ショルダー
	{enTexPageGunHoundArm, 4*32,2*32,32,32,16,13},	//05:ショルダー
	{enTexPageGunHoundArm, 4*32,3*32,32,32,16,13},	//05:ショルダー
	{enTexPageGunHoundArm, 4*32,4*32,32,32,16,13},	//05:ショルダー
	{enTexPageGunHoundArm, 4*32,5*32,32,32,16,13},	//05:ショルダー

	{enTexPageGunHoundArm, 4*32,7*32,32,32,16,13},	//05:ショルダー
};

gxSprite stSprRedLight[]={
	{enTexPageGunHoundArm, 0*32,6*32,32,32,16,16},	//01:アーム
	{enTexPageGunHoundArm, 1*32,6*32,32,32,16,24},	//02:ボディ
	{enTexPageGunHoundArm, 2*32,6*32,32,32,13,13},	//02:レッグ手前
	{enTexPageGunHoundArm, 3*32,6*32,32,32,13,13},	//03:レッグ奥
	{enTexPageGunHoundArm, 4*32,6*32,32,32,16,13},	//05:ショルダー
};

gxSprite stSprShot[]=
{
	{enTexPageGunHoundBdy, 0*0,240,16,16,8,8},	//HIT
};

gxSprite stSprPunch[]=
{
	//-------------------------------------
	//パンチ
	//-------------------------------------
	{enTexPageGunHoundBdy, 160,32+32*0,32,32,8,16},//0:Punch
	{enTexPageGunHoundBdy, 160,32+32*1,32,32,8,16},//1:Punch
	{enTexPageGunHoundBdy, 160,32+32*2,48,32,4+8,16},//2:Punch（ブルーティッシュ）

	{enTexPageGunHoundBdy, 160,32+32*3,48,32,4+8,16},//3:Punch（緑）
	{enTexPageGunHoundBdy, 160,32+32*4,48,32,4+8,16},//4:Punch（ストライククロー）
	{enTexPageGunHoundBdy, 160,32+32*5,48,32,4+8,16},//5:Punch（ズゴック）

	{enTexPageGunHoundBdy, 224,72,32,56,8,24},//6:Punch（シールド）

	{enTexPageGunHoundBdy, 160,32+32*6,48,32,8,16},//Punch
	{enTexPageGunHoundBdy, 160,32+32*7,48,32,8,16},//Punch
};

gxSprite stSprApogy[]=
{
	{enTexPageGunHoundBdy, 0*32,176,16,4,0,3},	//00:アポジモーター
};


static StMotionMan stMotion[]={
		//キックモーション
		{
			5,	//かがみこみ
			{
				{ enPartsBase,	0,0,0,	},
				{ enPartsBody,	0,0,0,	},
				{ enPartsArmL,	0,0,0,	},
				{ enPartsArmR,	0,0,0,	},
				{ enPartsFootL,	0,0,0,	},
				{ enPartsFootR,	-10,10,0,},
				{ enPartsLegL,	0,0,0,	},
				{ enPartsLegR,	0,0,0,	},
			}
		},
		{
			10,//後ろ足出す
			{
				{ enPartsBase,	0,0,0,	},
				{ enPartsBody,	20,0,0,	},
				{ enPartsArmL,	0,0,0,	},
				{ enPartsArmR,	0,0,0,	},
				{ enPartsFootL,	30,0,0,	},
				{ enPartsFootR,	0,0,0,	},
				{ enPartsLegL,	50,0,0,	},
				{ enPartsLegR,	0,0,0,	},
			}
		},
		{
			20,//後ろ足だけさらに出す
			{
				{ enPartsBase,	0,0,0,	},
				{ enPartsBody,	20,0,0,	},
				{ enPartsArmL,	0,0,0,	},
				{ enPartsArmR,	-20,0,0,	},
				{ enPartsFootL,	30,0,0,	},
				{ enPartsFootR,	0,0,0,	},
				{ enPartsLegL,	90,0,0,	},
				{ enPartsLegR,	0,0,0,	},
			}
		},
/*
		{
			10,//けりだし
			{
				{ enPartsBase,	0,0,0,	},
				{ enPartsBody,	30,0,0,	},
				{ enPartsArmL,	0,0,0,	},
				{ enPartsArmR,	0,0,0,	},
				{ enPartsFootL,	-45,5,0,	},
				{ enPartsFootR,	0,0,0,	},
				{ enPartsLegL,	0,10,0,	},
				{ enPartsLegR,	0,0,0,	},
			}
		},
*/
		{
			16,//蹴り上げ
			{
				{ enPartsBase,	0,0,0,	},
				{ enPartsBody,	-10,0,0,	},
				{ enPartsArmL,	0,0,0,	},
				{ enPartsArmR,	90,0,0,	},
				{ enPartsFootL,	0,5,0,	},
				{ enPartsFootR,	0,0,0,	},
				{ enPartsLegL,	-90,16,-12,	},
				{ enPartsLegR,	0,0,0,	},
			}
		},
		{
			8,//押し出し
			{
				{ enPartsBase,	0,0,0,	},
				{ enPartsBody,	-10,0,0,	},
				{ enPartsArmL,	0,0,0,	},
				{ enPartsArmR,	90,0,0,	},
				{ enPartsFootL,	0,0,0,	},
				{ enPartsFootR,	0,0,0,	},
				{ enPartsLegL,	-90,10,-16,	},
				{ enPartsLegR,	0,0,0,	},
			}
		},
		{
			5,//押し出し
			{
				{ enPartsBase,	0,0,0,	},
				{ enPartsBody,	-30,0,0,	},
				{ enPartsArmL,	0,0,0,	},
				{ enPartsArmR,	90,0,0,	},
				{ enPartsFootL,	0,5,0,	},
				{ enPartsFootR,	0,0,0,	},
				{ enPartsLegL,	00000000-90,16,-16,	},
				{ enPartsLegR,	0,0,0,	},
			}
		},
		{
			15,//押し出し
			{
				{ enPartsBase,	0,0,0,	},
				{ enPartsBody,	-30,0,0,	},
				{ enPartsArmL,	0,0,0,	},
				{ enPartsArmR,	90,0,0,	},
				{ enPartsFootL,	0,5,0,	},
				{ enPartsFootR,	0,0,0,	},
				{ enPartsLegL,	-90,16,-16,	},
				{ enPartsLegR,	0,0,0,	},
			}
		},
		{
			10,//元に戻す
			{
				{ enPartsBase,	0,0,0,	},
				{ enPartsBody,	-10,0,0,	},
				{ enPartsArmL,	0,0,0,	},
				{ enPartsArmR,	90,0,0,	},
				{ enPartsFootL,	0,5,0,	},
				{ enPartsFootR,	0,0,0,	},
				{ enPartsLegL,	-90,16,-12,	},
				{ enPartsLegR,	0,0,0,	},
			}
		},
		{
			20,
			{
				{ enPartsBase,	0,0,0,	},
				{ enPartsBody,	0,0,0,	},
				{ enPartsArmL,	0,0,0,	},
				{ enPartsArmR,	0,0,0,	},
				{ enPartsFootL,	0,0,0,	},
				{ enPartsFootR,	0,0,0,	},
				{ enPartsLegL,	0,0,0,	},
				{ enPartsLegR,	0,0,0,},
			}
		},
		{
			-1,
		}
	//		{ enPartsEnd, },
};

StMotionMan stMotionCarreer[]={
	//キャリアモード
	{
		5,	//かがみこみ
		{
			{ enPartsBase,	0,0,0,	},
			{ enPartsBody,	-10,0,0,	},
			{ enPartsArmL,	30,0,0,	},
			{ enPartsArmR,	50,0,0,	},
			{ enPartsFootL,	0,0,0,	},
			{ enPartsFootR,	-30,10,0,},
			{ enPartsLegL,	20,0,0,	},
			{ enPartsLegR,	20,0,0,	},
		}
	},
	{
		5000,	//かがみこみ
		{
			{ enPartsBase,	0,0,0,	},
			{ enPartsBody,	-10,0,0,	},
			{ enPartsArmL,	30,0,0,	},
			{ enPartsArmR,	50,0,0,	},
			{ enPartsFootL,	0,0,0,	},
			{ enPartsFootR,	-30,10,0,},
			{ enPartsLegL,	20,0,0,	},
			{ enPartsLegR,	20,0,0,	},
		}
	},
	{
		-1,
	}
//		{ enPartsEnd, },
};

static StMotionMan stMotionPunch[]={
	//パンチモーション
	{
		2,
		{
			{ enPartsBase,	0,0,0,	},
			{ enPartsBody,	0,0,0,	},
			{ enPartsArmL,	0,0,0,	},
			{ enPartsArmR,	0,0,0,	},
			{ enPartsFootL,	0,0,0,	-1},
			{ enPartsFootR,	0,0,0,	-1},
			{ enPartsLegL,	0,0,0,	-1},
			{ enPartsLegR,	0,0,0,	-1},
		}
	},

	{
		4,
		{
			{ enPartsBase,	0,0,0,	},
			{ enPartsBody,	5,8,0,	},
			{ enPartsArmL,	0,24,0,	},
			{ enPartsArmR,	64,-12,0,},
			{ enPartsFootL,	0,0,0,	-1},
			{ enPartsFootR,	0,0,0,	-1},
			{ enPartsLegL,	0,0,0,	-1},
			{ enPartsLegR,	0,0,0,	-1},
		}
	},
	//---------------------------------------
	{
		2,
		{
			{ enPartsBase,	0,0,0,	},
			{ enPartsBody,	5,8,0,	},
			{ enPartsArmL,	0,22,0,	},
			{ enPartsArmR,	64,-12,0,},
			{ enPartsFootL,	0,0,0,	-1},
			{ enPartsFootR,	0,0,0,	-1},
			{ enPartsLegL,	0,0,0,	-1},
			{ enPartsLegR,	0,0,0,	-1},
		}
	},
	{
		2,
		{
			{ enPartsBase,	0,0,0,	},
			{ enPartsBody,	5,8,0,	},
			{ enPartsArmL,	0,24,0,	},
			{ enPartsArmR,	64,-12,0,},
			{ enPartsFootL,	0,0,0,	-1},
			{ enPartsFootR,	0,0,0,	-1},
			{ enPartsLegL,	0,0,0,	-1},
			{ enPartsLegR,	0,0,0,	-1},
		}
	},
	//---------------------------------------
	{
		2,
		{
			{ enPartsBase,	0,0,0,	},
			{ enPartsBody,	5,8,0,	},
			{ enPartsArmL,	0,22,0,	},
			{ enPartsArmR,	64,-12,0,},
			{ enPartsFootL,	0,0,0,	-1},
			{ enPartsFootR,	0,0,0,	-1},
			{ enPartsLegL,	0,0,0,	-1},
			{ enPartsLegR,	0,0,0,	-1},
		}
	},
	{
		2,
		{
			{ enPartsBase,	0,0,0,	},
			{ enPartsBody,	5,8,0,	},
			{ enPartsArmL,	0,24,0,	},
			{ enPartsArmR,	64,-12,0,},
			{ enPartsFootL,	0,0,0,	-1},
			{ enPartsFootR,	0,0,0,	-1},
			{ enPartsLegL,	0,0,0,	-1},
			{ enPartsLegR,	0,0,0,	-1},
		}
	},
	//---------------------------------------
	{
		2,
		{
			{ enPartsBase,	0,0,0,	},
			{ enPartsBody,	5,8,0,	},
			{ enPartsArmL,	0,22,0,	},
			{ enPartsArmR,	64,-12,0,},
			{ enPartsFootL,	0,0,0,	-1},
			{ enPartsFootR,	0,0,0,	-1},
			{ enPartsLegL,	0,0,0,	-1},
			{ enPartsLegR,	0,0,0,	-1},
		}
	},
	{
		2,
		{
			{ enPartsBase,	0,0,0,	},
			{ enPartsBody,	5,8,0,	},
			{ enPartsArmL,	0,24,0,	},
			{ enPartsArmR,	64,-12,0,},
			{ enPartsFootL,	0,0,0,	-1},
			{ enPartsFootR,	0,0,0,	-1},
			{ enPartsLegL,	0,0,0,	-1},
			{ enPartsLegR,	0,0,0,	-1},
		}
	},
	//----------------------------------------------
	{
		6,
		{
			{ enPartsBase,	0,0,0,	},
			{ enPartsBody,	0,0,0,	},
			{ enPartsArmL,	0,0,0,	},
			{ enPartsArmR,	0,0,0,	},
			{ enPartsFootL,	0,0,0,	-1},
			{ enPartsFootR,	0,0,0,	-1},
			{ enPartsLegL,	0,0,0,	-1},
			{ enPartsLegR,	0,0,0,	-1},
		}
	},
	{
		-1,
		{
			{ enPartsBase,	0,0,0,	},
			{ enPartsBody,	0,0,0,	},
			{ enPartsArmL,	0,96,0,	},
			{ enPartsArmR,	0,0,0,	},
			{ enPartsFootL,	0,0,0,	-1},
			{ enPartsFootR,	0,0,0,	-1},
			{ enPartsLegL,	0,0,0,	-1},
			{ enPartsLegR,	0,120,0,-1},
		}
	},
	{
		10,
		{
			{ enPartsBase,	0,0,0,	},
			{ enPartsBody,	0,0,0,	},
			{ enPartsArmL,	0,48,0,	},
			{ enPartsArmR,	0,0,0,	},
			{ enPartsFootL,	0,0,0,	},
			{ enPartsFootR,	0,0,0,	},
			{ enPartsLegL,	0,0,0,	},
			{ enPartsLegR,	0,0,0,	},
		}
	},
	{
		60,
		{
			{ enPartsBase,	0,0,0,	},
			{ enPartsBody,	0,0,0,	},
			{ enPartsArmL,	0,24,0,	},
			{ enPartsArmR,	0,0,0,	},
			{ enPartsFootL,	0,0,0,	},
			{ enPartsFootR,	0,0,0,	},
			{ enPartsLegL,	0,0,0,	},
			{ enPartsLegR,	0,0,0,	},
		}
	},
	{
		-1,
	}
//		{ enPartsEnd, },
};


typedef struct StBodyParts
{
	Sint32 bFlag;
	Sint32 hp;
	Sint32 x1;
	Sint32 y1;
	Sint32 x2;
	Sint32 y2;

} StBodyParts;


//ダメージ
StBodyParts stBodyParts[]=
{
	{gxTrue,HP_ARMORCLASS_B,-4 ,-12,4 ,12},	//BODY
	{gxTrue,HP_ARMORCLASS_B,-8,0  ,8,6},	//ARM
	{gxTrue,HP_ARMORCLASS_C,-3 ,0  ,3 ,12},	//LEG
	{gxTrue,HP_ARMORCLASS_C,-3 ,0  ,3 ,12},	//LEG
};

