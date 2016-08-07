Sint32 g_sCustomizeTbl[]={	
	31,
	3,
	2,
	134,
	130,
	322,
	133,
	231,
	232,
	234,
	223,
	110,
	113,
	233,
	999,
	0,
	0,
	0,
	0,
	0,
	0,
};	

enum {
	//�{�f�B�p�[�c
	enBodyNormal,
	enBodySlaster,
	enBodyHowk,
	enBodyMini,
};


enum {
	//�r�p�[�c
	enLegNormal,
	enLegArmored,
	enLegPod,
	enLegChicken,
};


enum {
	//����p�[�c
	enArmNone = -1,
	enArmShort = 0,
	enArmLong,
	enArmSolid,
	enArmShoulder,
	enArmCannon,
	enArmCannonR,
};

enum {
	//����ԍ�
	enWeaponVorzoyNone = -1,
	enWeaponShort = 0,
	enWeaponLong,
	enWeaponSolid,
	enWeaponSPod,
	enWeaponLogGun,
	enWeaponLPod,
	enWeaponBCrasher,
};

gxSprite SprCEneH0008BloodSocker[]=
{
	{ enTexCEneH0008BloodSocker,0*32,96+0*32,32,32,16,24},	//0:BODY
	{ enTexCEneH0008BloodSocker,192, 96+160,32,32,16,8},		//1:LEG��
	{ enTexCEneH0008BloodSocker,160, 96+160,32,32,16,8},		//2:LEG��O
	{ enTexCEneH0008BloodSocker,192, 96+128,32,32,6,10},		//3:ARM
};

gxSprite SprCEneH0008BloodSockerBody[]={
	{ enTexCEneH0008BloodSocker,0*32,96+0*32,32,32,16,24},	//0:BODY0/�m�[�}��
	{ enTexCEneH0008BloodSocker,1*32,96+0*32,32,32,16,24},	//1:BODY1/�X���X�^�[�t
	{ enTexCEneH0008BloodSocker,2*32,96+0*32,32,32,16,24},	//2:BODY2/�V���[�^�[�t
	{ enTexCEneH0008BloodSocker,3*32,96+0*32,32,32,16,24},	//3:BODY3/�y��
};

gxSprite SprCEneH0008BloodSockerLegTemae[]={
	{ enTexCEneH0008BloodSocker,0*32,96+1*32,32,32,16,8},	//0:LEG1
	{ enTexCEneH0008BloodSocker,1*32,96+1*32,32,32,16,8},	//1:LEG2
	{ enTexCEneH0008BloodSocker,2*32,96+1*32,32,32,16,8},	//2:LEG3
	{ enTexCEneH0008BloodSocker,3*32,96+1*32,32,32,16,8},	//3:LEG4
};

gxSprite SprCEneH0008BloodSockerLegOku[]={
	{ enTexCEneH0008BloodSocker,0*32,96+2*32,32,32,16,8},	//0:LEG1/�m�[�}��
	{ enTexCEneH0008BloodSocker,1*32,96+2*32,32,32,16,8},	//1:LEG2/�A�[�}�[�h
	{ enTexCEneH0008BloodSocker,2*32,96+2*32,32,32,16,8},	//2:LEG3/�V���[�^�[
	{ enTexCEneH0008BloodSocker,3*32,96+2*32,32,32,16,8},	//3:LEG4/�`�L�����b�O
};

gxSprite SprCEneH0008BloodSockerArm[]={
	{ enTexCEneH0008BloodSocker,160,32*0,96,32  ,32,16},	//0:ARM0/�V���[�g�o����
	{ enTexCEneH0008BloodSocker,160,32*1,96,32  ,32,16},	//1:ARM0/�����O�o����
	{ enTexCEneH0008BloodSocker,160,32*2,96,32  ,28,14},	//2:ARM0/�\���b�h�V���[�^�[
	{ enTexCEneH0008BloodSocker,160,96  ,96,48  ,32,32},	//3:ARM0/���P�b�g�L���m��
	{ enTexCEneH0008BloodSocker,160,144 ,96,48  ,32,32},	//4:ARM0/���K���q�C
	{ enTexCEneH0008BloodSocker,160,192 ,96,32  ,32,16},	//5:ARM0/���K���q�C�i�Y����j
};

