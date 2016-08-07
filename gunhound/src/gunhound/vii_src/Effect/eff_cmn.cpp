//----------------------------------------------
//@
//@ ������Ȃ�
//@
//----------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

/*��*/Sint32 _opt_[10];

extern gxSprite Spr07AirCraft[];
extern gxSprite Spr07VermarkEscape[];
extern gxSprite spr_shermark[];

gxSprite stSprMazzleFlash[]={
	//HOUND�p
	{TPG_CLOUD,96,0,32,32,16,16},
	{enTexPageEffectCommon02,144,144,16,16,8,8},
};
gxSprite stSprHitFlash[]={
	//HOUND�p
	{TPG_CLOUD,96,0,32,32,16,16},
	{enTexPageEffectCommon02,160,128,32,32,16,16},
};

gxSprite SprHitFlash[]={
	{ TPG_EFFECT , 0*16,224*1,16,16,8,8 },
	{ TPG_EFFECT , 1*16,224*1,16,16,8,8 },
	{ TPG_EFFECT , 2*16,224*1,16,16,8,8 },
	{ TPG_EFFECT , 3*16,224*1,16,16,8,8 },
	{ TPG_EFFECT , 4*16,224*1,16,16,8,8 },
	{ TPG_EFFECT , 5*16,224*1,16,16,8,8 },
	{ TPG_EFFECT , 6*16,224*1,16,16,8,8 },
	{ TPG_EFFECT , 7*16,224*1,16,16,8,8 },

	{ TPG_EFFECT , 0*16,224*1+16,16,16,8,8 },
};

gxSprite sprCloudAlpha[]={
	{TPG_CLOUD,128,0,16,16,8,8},
};

gxSprite other_as[]={
	{TPG_HUMAN,128,224,32,32,0,32},	//0:as01
	{TPG_HUMAN,164,224,32,32,0,32},	//1:as02
	{TPG_HUMAN,64,224,32,32,0,32},	//2:burner0
	{TPG_HUMAN,96,224,32,32,0,32},	//3:burner1
};

gxSprite spr_brokenDeluxe[]={
	{TPG_EFFECT,0*32,256,32,32,16,16},
	{TPG_EFFECT,1*32,256,32,32,16,16},
	{TPG_EFFECT,2*32,256,32,32,16,16},
	{TPG_EFFECT,3*32,256,32,32,16,16},
	{TPG_EFFECT,4*32,256,32,32,16,16},
	{TPG_EFFECT,5*32,256,32,32,16,16},
	{TPG_EFFECT,0*32,256+32,32,32,16,16},
	{TPG_EFFECT,1*32,256+32,32,32,16,16},
	{TPG_EFFECT,2*32,256+32,32,32,16,16},
	{TPG_EFFECT,3*32,256+32,32,32,16,16},
	{TPG_EFFECT,4*32,256+32,32,32,16,16},
	{TPG_EFFECT,5*32,256+32,32,32,16,16},
};


void CEffect::missile_smoke()
{
	//-------------------------------------
	//�~�T�C���̉�
	//-------------------------------------

	if (is_init()) {
		rot = viiSub::Rand()%360;
		free.w[0] = 80+viiSub::Rand()%20;		//�F
		free.w[1] = 100+viiSub::Rand()%20;		//�傫��
	}

	if(free.w[0] > 0) {
		free.w[0] -= 2;
		free.w[1] += 2;
	}

	if(free.w[0] <=0){
		//���ߓx�[���p�[�Z���g�ŏ���
		exist_ = gxFalse;
		return;
	}

	Sint32 col   = free.w[0]*2;
	float scl = free.w[1]/200.0f;

	rot += 2;
	if(col >= 255) col = 255;

	Sint32 x,y;
	x = x_;
	y = y_;

	viiDraw::Sprite(&spr_custom[SPR_MISSILE_SMOKE],x,y,PRIO_EFFECT+1,ATR_ALPHA_PLUS,ARGB(col/2,0xff,0xff,0xff),scl,-rot);

}


gxSprite SprBooster[]={
	{TPG_VALKEN,32*0,32*3,32,32,16,16},	//6:booster
};

void CEffect::booster_toss()
{

}


void CEffect::OptionBit()
{
	//�I�v�V�����p�̃r�b�g
}


void CEffect::bomb_small()
{
	//�V���g���j�󎞂̏�����
	if(is_init()){
		x_ += (-3+viiSub::Rand()%6)*100;
		free.w[0] = 128+viiSub::Rand()%20;		//�F
		free.w[1] = 100+viiSub::Rand()%20;		//�傫��
		pos.mx = opt[0];	//�ړ������w
		pos.my = opt[1];	//�ړ������x
	}

	if(free.w[0] > 0) {
		free.w[0] -= 3;
		free.w[1] += 3;
	}

	if(free.w[0] <=0){
		//���ߓx�[���p�[�Z���g�ŏ���
		exist_ = gxFalse;
		return;
	}

	Sint32 col   = free.w[0]*2;
	float scl = free.w[1]/100.0f;

	if(col >= 255) col = 255;

//	viiSub::SetGravitMove(x_,y_);

	x_ += pos.mx;
	y_ += pos.my;

	Sint32 x,y;
	x = x_;
	y = y_;

	if(timer_== 0){
		viiDraw::Sprite(&spr_bomb[11],x,y,PRIO_EFFECT,ATR_DFLT,ARGB(col/2,0xff,0xff,0xff),scl);
	}
	viiDraw::Sprite(&spr_bomb[11],x,y,PRIO_EFFECT,ATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff),scl);
}


void CEffect::laser_bomb_parts()
{
	//���[�U�[�������p�[�c

	if(is_init()){
		rot = viiSub::Rand()%360;
		free.w[0] = 128+viiSub::Rand()%20;		//�F
		free.w[1] = 100+viiSub::Rand()%20;		//�傫��
		xx_ = viiMath::Cos100(opt[0])*opt[1];	//�p�xx�X�s�[�h
		yy_ = viiMath::Sin100(opt[0])*opt[1];	//�p�xx�X�s�[�h
	}

	if(free.w[0] > 0) {
		free.w[0] -= 6;
		free.w[1] += 1;
	}

	if(free.w[0] <=0){
		//���ߓx�[���p�[�Z���g�ŏ���
		exist_ = gxFalse;
		return;
	}

	Sint32 col   = free.w[0];
	float scl = free.w[1]/100.0f;

	rot += 2;
	if(col >= 255) col = 255;
	if(col <= 0) col = 0;

	x_ += xx_;
	y_ += yy_;

	xx_ += -xx_/10;
	yy_ += -yy_/10;


	Sint32 x,y;
	x = x_;
	y = y_;

	viiDraw::Sprite(&spr_bomb[2],x,y,PRIO_EFFECT,ATR_DEFAULT,ARGB(col/2,0xff,0xff,0xff),1.0f,rot);
	viiDraw::Sprite(&spr_bomb[2],x,y,PRIO_EFFECT+1,ATR_ALPHA_PLUS,ARGB(col/2,0xff,0xff,0xff),scl,-rot);

}

void CEffect::burnia_smoke()
{
	//�o�[�j�A����r�o����鉌
	//opt[0] = ���� -1 : 1

	if(is_init()){
		free.w[0] = 128+viiSub::Rand()%20;					//�F
		free.w[1] = 50+viiSub::Rand()%50;					//�傫��
		free.w[2] = (400+viiSub::Rand()%100)*(-opt[0]);		//����
		pObj = new CObj();
		if(opt[1]){
			free.w[2] = opt[1] * opt[0];			//���x�ݒ�ς݂̏ꍇ�͂�����
		}
	}

	if(free.w[0] > 0) {
		free.w[0] -= 4;
		free.w[1] +=4;
	}

	if(free.w[0] <=0){
		//���ߓx�[���p�[�Z���g�ŏ���
		exist_ = gxFalse;
		delete pObj;
		pObj = NULL;
		return;
	}

	Sint32 col   = free.w[0]*2;
	if(col>=255) col = 255;

	float scl = free.w[1]/100.0f;
	pObj->pos.x = x_;
	pObj->pos.y = y_;
	pObj->mov.x = free.w[2];
	pObj->mov.y =  500;

	if(pObj->easy_collision_check()){
		Sint32 atr = ATR_ALPHA_PLUS;
		if(opt[0] < 0) atr = ATR_ALPHA_PLUS|ATR_FLIP_X;
		viiDraw::Sprite(&spr_bomb[9],x_,y_,PRIO_EFFECT,atr,ARGB(col,0xff,0xff,0xff),scl);
	}

	x_ += pObj->mov.x;
	y_ += pObj->mov.y;
}

void CEffect::roller_dashsmoke()
{
	//���[���[�_�b�V�����̉Ή�
	//opt[0] = ���� -1 : 1

	if(is_init()) {
		free.w[0] = 128+viiSub::Rand()%20;					//�F
		free.w[1] = 100+viiSub::Rand()%50;					//�傫��
		free.w[2] = (250+viiSub::Rand()%150)*(-opt[0]);		//����
	}

	if(free.w[0] > 0) {
		free.w[0] -= 16;
		free.w[1] +=5;
	}

	if(free.w[0] <=0){
		//���ߓx�[���p�[�Z���g�ŏ���
		exist_ = gxFalse;
		return;
	}

	x_ += free.w[2];

	Sint32 col   = free.w[0]*2;
	if(col>=255) col = 255;

	float scl = free.w[1]/100.0f;

	Sint32 r = (viiSub::Rand()%2)*240;
	Sint32 g = (viiSub::Rand()%2)*240;
	Sint32 b = (viiSub::Rand()%2)*240;
	if(r+g+b==0) {
		r = g = b = 128;
	}

	Sint32 atr = ATR_ALPHA_PLUS;
	if(opt[0] < 0) atr = ATR_ALPHA_PLUS|ATR_FLIP_X;

	viiDraw::Sprite(&spr_bomb[10],x_,y_,PRIO_BG3-1,atr,ARGB(col,r,g,b),scl);

}

void CEffect::bigbomb()
{
	//�唚������p
	//�����p�^�[������I�u�W�F
//	Sint32 start_rot = viiSub::Rand()%360;

	if(opt[0] <= 0) opt[0] = 1;

	if(is_init()){
//		for(Sint32 i=0;i<1;i++){
			viiEff::Set( EFF_BIGBOMB_PARTS,x_,y_,opt);
//		}
	}
	exist_ = gxFalse;
}
extern gxSprite stSprMidiamBomb[];
extern gxSprite stSprExplosion[];
void CEffect::bigbomb_parts()
{
	//�唚���p�[�c
	float scl_base = 1.0f;
	if (is_init())
	{
		rot = viiSub::Rand()%360;
		free.w[0] = 128+viiSub::Rand()%20;		//�F
		free.w[1] = 100+viiSub::Rand()%20;		//�傫��
		free.w[2] = 3;//�X�v���C�g
		xx_ = 0;
		yy_ = 0;
	}

	scl_base *= opt[0];

	if(free.w[0] > 0)
	{
		//���ߓx�X�P�[������
		free.w[0] -= 2;
		free.w[1] += 1;
		free.w[2] += 1;
		if(free.w[2] >=39){
			free.w[2] = 39;
		}
	}

	if(free.w[0] <=0)
	{
		//���ߓx�[���p�[�Z���g�ŏ���
		exist_ = gxFalse;
		return;
	}

	Sint32 col   = free.w[0]*2;
	float scl = free.w[1]/100.0f;
	Sint32 spr = 3+free.w[2]/10;

	rot += 2;
	if(col >= 255) col = 255;

	x_ += xx_;
	y_ += yy_;

	xx_ += -xx_/10;
	yy_ += -yy_/10;

//	viiSub::SetGravitMove(x_,y_);

	Sint32 x,y;
	x = x_;
	y = y_;

	viiDraw::Sprite(&stSprMidiamBomb[1],x,y,PRIO_EFFECT,ATR_DEFAULT,ARGB(col,0xff,0xff,0xff),scl_base*1,rot*2);
	viiDraw::Sprite(&stSprMidiamBomb[1],x,y,PRIO_EFFECT+1,ATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff),scl_base*scl,-rot*2);

	viiDraw::Sprite(&stSprMidiamBomb[1],x,y,PRIO_EFFECT,ATR_DEFAULT,ARGB(col,0xff,0xff,0xff),scl_base*1,rot);
	viiDraw::Sprite(&stSprMidiamBomb[1],x,y,PRIO_EFFECT+1,ATR_ALPHA_PLUS,ARGB(col/2,0xff,0xff,0xff),scl_base*scl,-rot);

	viiDraw::Sprite(&stSprExplosion[1],x,y,PRIO_EFFECT,ATR_DEFAULT,ARGB(col,0xff,0xff,0xff),scl_base,rot);
	viiDraw::Sprite(&stSprExplosion[1],x,y,PRIO_EFFECT,ATR_DEFAULT,ARGB(col/2,0xff,0xff,0xff),scl_base,-rot);

	viiDraw::Sprite(&spr_cloud[1],x,y,PRIO_EFFECT+1,ATR_ALPHA_PLUS,ARGB(col/3,0xff,0xff,0xff),scl_base*scl*1,-rot*1);
	viiDraw::Sprite(&spr_cloud[1],x,y,PRIO_EFFECT+1,ATR_ALPHA_PLUS,ARGB(col/2,0xff,0xff,0xff),scl_base*scl*1,rot*1);

}


void CEffect::parts_break()
{
	//---------------------------------------------------------------------
	//�j�󂳂ꂽ�p�[�c(���g�p�H�H)
	//gxSprite*�������ɂ��炤�̂Ń��[�J���ϐ��A�h���X��n���Ȃ��悤����
	//---------------------------------------------------------------------

	if (is_init()) {
		//opt[0];		//�X�v���C�g�e�[�u���ւ̃|�C���^
		xx_ = opt[1];
		yy_ = opt[2];
		rot = viiSub::Rand()%360;
		free.w[0] = 0;		//�o�E���h�񐔐���
		free.w[1] = opt[3];	//��]���x
	}

	gxSprite *p = &Sprite;// (gxSprite*)opt[0]; 64bit�Ή��@�������g���ĂȂ����ۂ�

	if(viiSub::GetBoundBlock_Enemy(x_,y_)){
		if(free.w[0] == 0){
			free.w[0] = 1;
			yy_ = -yy_/2;
			free.w[1] = -free.w[1];
		}else{
			exist_ = gxFalse;
			return;
		}
	}
	if(viiSub::IsScrollOut(x_,y_,0,0)){
		exist_ = gxFalse;
		return;
	}

	//����
	yy_+=32;
	if(yy_>=600){
		yy_ = 600;
	}
	//X�͌���
	xx_ += -xx_/100;

	x_ += xx_;
	y_ += yy_;

	rot += free.w[1];
	viiDraw::Sprite(p,x_,y_,PRIO_EFFECT,ATR_DEFAULT,ARGB_DFLT,1.0f,rot);

}


void CEffect::shuttle_burnia()
{
	//�V���g���o�[�j�A

	Sint32 type;
	if (is_init()) {
		rot = viiSub::Rand()%360;
		free.w[0] = 128+viiSub::Rand()%20;			//�F
		free.w[1] = opt[2]+viiSub::Rand()%20;		//�傫��
		xx_ = (viiMath::Cos100(opt[0])*opt[1])/100;	//�p�xx�X�s�[�h
		yy_ = (viiMath::Sin100(opt[0])*opt[1])/100;	//�p�xx�X�s�[�h
		free.w[2] = 0;						//�q���쐬�t���O
	}

	type = opt[3];	//�P�Ȃ牌

	if(free.w[0] > 0) {
		switch(type){
		case 1:
			//������
			if(timer_ %2 == 0) free.w[0] -= 1;
			break;
		case 2:
			//�{�X���̉�
			free.w[0] -= 4;
//			if(viiSub::IsScrollOut(x_,y_,128*100,128*100))	exist_ = gxFalse;
			break;
		case 0:
			//�ˏo���̉�(TYPE 0)
			free.w[0] -= 1;
			break;
		}
		free.w[1] += 2;
	}

	if(free.w[0] <=0){
		//���ߓx�[���p�[�Z���g�ŏ���
		exist_ = gxFalse;
		return;
	}

	Sint32 col   = free.w[0]*2;
	float scl = free.w[1]/100.0f;

	rot += 2;
	if(col >= 255) col = 255;

	x_ += xx_;
	y_ += yy_;

	xx_ += -xx_/50;

	Sint32 x,y;
	x = x_;
	y = y_;

	if(type == 0 || type == 2 ){
		viiDraw::Sprite(&spr_bomb[0],x,y,PRIO_ENEMY-1,ATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff),1.0f,rot);
		viiDraw::Sprite(&spr_bomb[0],x,y,PRIO_ENEMY-1,ATR_ALPHA_PLUS,ARGB(col/2,0xff,0xff,0xff),scl,-rot);
	}else{
		viiDraw::Sprite(&spr_bomb[8],x,y,PRIO_ENEMY-1,ATR_ALPHA_PLUS,ARGB(col,0x40,0x40,0x40),scl/2,rot);
		viiDraw::Sprite(&spr_bomb[8],x,y,PRIO_ENEMY-1,ATR_ALPHA_PLUS,ARGB(col/2,0x40,0x40,0x40),scl,-rot);
	}
	if((type == 0 || type == 2) && free.w[2] == 0 && col <= 0xa0){
		//�r���ŉ��𐶐�����
		_opt_[0] = 90;	//�p�x
		_opt_[1] = 600;	//�X�s�[�h
		_opt_[2] = 290;	//�傫��
		_opt_[3] = 1;	//��
		free.w[2] = 1;
		viiEff::Set(EFF_SHUTTLE_BURNIA,x,y, _opt_);
	}
}


void CEffect::vulcan_hitflash()
{
	//�o���J�����e�G�t�F�N�g

	if (is_init())
	{
		//opt[0]	//�ǂɒ��e�i0�j/ �G�ɒ��e(1)
		pos.x = x_;
		pos.y = y_;
		pos.rot = viiSub::Rand()%360;
		pos.sx = 100+viiSub::Rand()%100;
		free.w[0] = 200+viiSub::Rand()%50;	//�F����
		free.w[1] = 0;
	}

	if(viiSub::IsScrollOut(x_,y_,32*100,32*100))
	{
		exist_ = gxFalse;
		return;		//��ʊO�ł͕`�悵�Ȃ�
	}

	Sint32 col;

	if(free.w[0] > 0)
	{
		free.w[0] -= 8;
		if(free.w[0] <= 0)
		{
			free.w[0] = 0;
			exist_ = gxFalse;
		}
		pos.sx += 15;
	}

	col = free.w[0];

	float scl = pos.sx / 100.0f;
	if(scl >4.0f) scl = 4.f;

/*
	Sint32 spr = 1;
	if(opt[0] == 1) spr = 4;

	viiDraw::Sprite(&spr_hitflash[spr],x_,y_,PRIO_EFFECT,ATR_DFLT,ARGB(col,0xff,0xff,0xff),scl,0);
	viiDraw::Sprite(&spr_hitflash[spr],x_,y_,PRIO_EFFECT,ATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff),scl*1.5f,pos.rot);
	viiDraw::Sprite(&spr_hitflash[0],x_,y_,PRIO_EFFECT,ATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff),scl*1.2f,-pos.rot);
*/

	free.w[1] ++;
	Sint32 spr = free.w[1]/2;
	if(spr >= 7) spr = 7;

#ifdef GUNHOUND
	if( opt[0] == 1 )
	{
		//�G�ɒ��e�i���΁j
		pos.rot += 8;
		viiDraw::Sprite( &stSprHitFlash[0] , x_ , y_ , PRIO_EFFECT , ATR_ALPHA_PLUS       ,ARGB(col,0xff,0xff,0xff)  ,scl  ,-pos.rot);
		viiDraw::Sprite( &stSprHitFlash[1] , x_ , y_ , PRIO_EFFECT , ATR_ALPHA_PLUS , ARGB(col,0xff,0xff,0xff) ,scl/4,pos.rot);
	}
	else
	{
		viiDraw::Sprite( &stSprHitFlash[0] , x_ , y_ , PRIO_EFFECT , ATR_ALPHA_PLUS , ARGB(col,0xff,0xff,0x00)   , scl/5 , -pos.rot);
		viiDraw::Sprite( &stSprHitFlash[1] , x_ , y_ , PRIO_EFFECT , ATR_ALPHA_PLUS , ARGB(col/2,0xff,0xff,0xff) , scl/5 , pos.rot );
	}
#else
	if( opt[0] == 1 )
	{
		//�G�ɒ��e�i���΁j
		viiDraw::Sprite( &SprHitFlash[spr] , x_ , y_ , PRIO_EFFECT , ATR_DFLT,ARGB(col,0xff,0xff,0xff),scl*1.0f,-pos.rot);
		viiDraw::Sprite( &SprHitFlash[0]   , x_ , y_ , PRIO_EFFECT , ATR_ALPHA_PLUS , ARGB(col,0xff,0xff,0xff),scl,pos.rot);
	}
	else
	{
		viiDraw::Sprite( &SprHitFlash[spr] , x_ , y_ , PRIO_EFFECT , ATR_DFLT , ARGB(col,0xff,0xff,0x00)   , scl/2 , -pos.rot);
		viiDraw::Sprite( &SprHitFlash[ 8 ] , x_ , y_ , PRIO_EFFECT , ATR_DFLT , ARGB(col/2,0xff,0xff,0xff) , scl/2 , pos.rot );
	}
#endif
}


gxSprite SprCEne0001MazzleNew[]=
{
	{ enTexPageEffect4,128,0,128,128,64,64},		//0:�n�C�p�[�{�[��
};

void CEffect::vulcan_mazzleflash()
{
	//�}�Y���t���b�V��

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		pos.rot = viiSub::Rand()%360;
		pos.sx = 100;
		free.w[0] = 200+viiSub::Rand()%50;	//�F����
	}

	if(viiSub::IsScrollOut(x_,y_,32*100,32*100)){
		exist_ = gxFalse;
		return;
	}

	Sint32 col;
	if(free.w[0] > 0){
		free.w[0] -= 32;
		if(free.w[0] <= 0){
			free.w[0] = 0;
			exist_ = gxFalse;
			return;
		}
		pos.sx += 4;
		pos.rot += 4;
	}
	col = free.w[0];

	float scl = pos.sx / 100.0f;

//	viiDraw::Sprite(&sprCloudAlpha[0] , pos.x,pos.y,PRIO_EFFECT,ATR_DFLT,ARGB(col/2,0xff,0xff,0xff),scl*1.2f,-pos.rot);
//	viiDraw::Sprite(&sprCloudAlpha[0] , pos.x,pos.y,PRIO_EFFECT,ATR_ALPHA_PLUS,ARGB(col/2,0xff,0xff,0xff),scl*1.5f, pos.rot+45);

#ifdef GUNHOUND
	viiDraw::Sprite(&stSprMazzleFlash[1],pos.x,pos.y,PRIO_EFFECT,ATR_DFLT,ARGB(col,0xff,0xff,0xff),scl*1.0f,pos.rot);
	viiDraw::Sprite(&stSprMazzleFlash[0],pos.x,pos.y,PRIO_EFFECT,ATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff),scl*1.5f,-pos.rot);
	viiDraw::Sprite(&SprCEne0001MazzleNew[0],pos.x,pos.y,PRIO_EFFECT,ATR_ALPHA_PLUS,ARGB(col/32,0xff,0xff,0xff),1.55f);

#else
	viiDraw::Sprite(&spr_hitflash[2],pos.x,pos.y,PRIO_EFFECT,ATR_DFLT,ARGB(col,0xff,0xff,0xff),scl*1.0f,pos.rot);
	viiDraw::Sprite(&spr_hitflash[2],pos.x,pos.y,PRIO_EFFECT,ATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff),scl*1.2f,-pos.rot);
#endif

}


void CEffect::yakkyou()
{
	//-------------------------------------
	//�򂫂傤
	//-------------------------------------

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		pos.rot = viiSub::Rand()%360;
		pos.sx = 100;
		free.w[0] = 8+viiSub::Rand()%8;	//��]���x
		free.w[1] = 2+viiSub::Rand()%2;			//�Ԃ���J�E���^�[
		if(viiSub::Rand()%2==0) free.w[0] *= -1;
		Sint32 spd = (250+viiSub::Rand()%200);
		pos.mx = (viiMath::Cos100(opt[0])*spd)/50;
		pos.my = (viiMath::Sin100(opt[0])*spd)/100;
		yy_ = 16;
	}

	gxBool hit_kabe = gxFalse;
	if(pGame->is_gravit()){
		//�d�͉�
		yy_ += 16;
		if(viiSub::GetBoundBlock_Enemy(pos.x,pos.y)){
			hit_kabe = gxTrue;
			yy_ = (yy_+pos.my)*-1;
			yy_ = 1*yy_/5;
			pos.y += pos.my+yy_;
			pos.my = 0;
			pos.mx = 1*pos.mx/3;
			free.w[0] *= -1;
		}else{
			pos.mx = 95*pos.mx/100;
		}
	}else{
		if(viiSub::GetBoundBlock_Enemy(pos.x+pos.mx,pos.y+pos.my)){
/*
			//���ˏ���
			if(viiSub::GetBoundBlock_Enemy(pos.x+pos.mx,pos.y)){
				hit_kabe = gxTrue;
				pos.mx *= -1;
			}else if(viiSub::GetBoundBlock_Enemy(pos.x,pos.y+pos.my)){
				hit_kabe = gxTrue;
				pos.my *= -1;
			}else{
				hit_kabe = gxTrue;
				pos.my *= -1;
				pos.mx *= -1;
			}
*/
			hit_kabe = gxTrue;
			free.w[1] = 1;
		}
	}

	if(hit_kabe){
		//�ǃq�b�g�J�E���g����
		free.w[1]--;
		if(free.w[1]<=0){
			exist_=gxFalse;
			return;
		}
	}

	Sint32 mx,my;
	mx = pos.mx;
	my = pos.my+yy_;
	viiSub::SetGravitMove(mx,my);

	pos.x += mx;
	pos.y += my;


	if(viiSub::IsScrollOut(x_,y_,8*100,8*100)){
		exist_ = gxFalse;
		return;
	}

	pos.rot += free.w[0];

	viiDraw::Sprite(&spr_hitflash[3],pos.x,pos.y,PRIO_PLAYER-1,ATR_DFLT,ARGB_DFLT,1.0f,pos.rot);

}


void CEffect::FireFlower()
{
	//-------------------------------------
	//�Ή�
	//-------------------------------------
	Sint32 ox,oy;

	if (is_init())
	{
		pos.x = x_;
		pos.y = y_;
		pos.rot = 0;
		pos.sx = 100;
		free.w[0] = 8+viiSub::Rand()%8;	//��]���x
		free.w[1] = 2+viiSub::Rand()%2;			//�Ԃ���J�E���^�[
		free.w[3] = 12+viiSub::Rand()%20;		//�c���J�E���^�[
		if(viiSub::Rand()%2==0) free.w[0] *= -1;
		Sint32 spd = (60+viiSub::Rand()%60);
		pos.mx = (viiMath::Cos100(opt[0])*spd)/50;
		pos.my = (viiMath::Sin100(opt[0])*spd)/100;
		yy_ = 16;
	}

	gxBool hit_kabe = gxFalse;

	if(pGame->is_gravit())
	{
		//�d�͉�
		yy_ += 2;
		if(viiSub::GetBoundBlock_Enemy(pos.x,pos.y)){
			hit_kabe = gxTrue;
			yy_ = (yy_+pos.my)*-1;
			yy_ = 1*yy_/5;
			pos.y += pos.my+yy_;
			pos.my = 0;
			pos.mx = 1*pos.mx/3;
			free.w[0] *= -1;
		}else{
			pos.mx = 95*pos.mx/100;
		}
	}else{
		if(viiSub::GetBoundBlock_Enemy(pos.x+pos.mx,pos.y+pos.my))
		{
			hit_kabe = gxTrue;
			free.w[1] = 1;
		}
	}

	if(hit_kabe)
	{
		//�ǃq�b�g�J�E���g����
		free.w[1]--;
		if(free.w[1]<=0){
			exist_=gxFalse;
			return;
		}
	}

	ox = pos.x;
	oy = pos.y;

	Sint32 mx,my;
	mx = pos.mx;
	my = pos.my+yy_;
	viiSub::SetGravitMove(mx,my);

	pos.x += mx;
	pos.y += my;

	if(viiSub::IsScrollOut(x_,y_,8*100,8*100))
	{
		exist_ = gxFalse;
		return;
	}

	if( free.w[3] > 0 )
	{
		free.w[3] --;
	}
	else
	{
		exist_ = gxFalse;
		return;
	}

	pos.rot += 16;

	viiDraw::Sprite( &spr_hitflash[7],ox,oy,PRIO_PLAYER-1,ATR_ALPHA_PLUS,0x80ffffff,1.0f,pos.rot);
	viiDraw::Sprite( &spr_hitflash[7],pos.x,pos.y,PRIO_PLAYER-1,ATR_ALPHA_PLUS,0xA0ffffff,1.0f,pos.rot);

}


void CEffect::ring()
{
	//-------------------------------------
	//�唚�����O�̃����O
	//-------------------------------------

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		pos.rot = viiSub::Rand()%360;
		pos.sx = 0;
		free.w[0] = 200+viiSub::Rand()%50;	//�F����
		//opt[0] MAX�̑傫��(300=�R�{)
	}

	Sint32 col;
	if(free.w[0] > 0){
		free.w[0] -= 26;
		if(free.w[0] <= 0){
			free.w[0] = 0;
			exist_ = gxFalse;
			return;
		}
		pos.sx += (opt[0]-pos.sx)/8;
		pos.rot += 4;
	}
	col = free.w[0];

	float scl = pos.sx / 100.0f;

	viiDraw::Sprite(&spr_bomb[7],pos.x,pos.y,PRIO_EFFECT,ATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff),scl,pos.rot);
}


void CEffect::eff_burnia()
{
	//-------------------------------------
	//�o�[�j�A�G�t�F�N�g
	//-------------------------------------

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		if(opt[1] == 0){
			//�u�[�X�^�[�Ȃ�
			pos.sx = 60+viiSub::Rand()%40;
			free.w[1] = 8;	//�o�[�j�A�T�C�Y
		}else{
			//�u�[�X�^�[����
			pos.sx = 80+viiSub::Rand()%50;
			free.w[1] = 10;	//�o�[�j�A�T�C�Y
		}
		free.w[0] = 150+viiSub::Rand()%100;	//�F����
	}

	Sint32 col;
	if(free.w[0] > 0){
		free.w[0] -= 26;
		if(free.w[0] <= 0){
			free.w[0] = 0;
			exist_ = gxFalse;
			return;
		}
		pos.sx += free.w[1];
	}
	col = free.w[0];

	float scl = pos.sx / 100.0f;
	Sint32 atr = ATR_ALPHA_PLUS;
	if(opt[0] == -1){
		atr |= ATR_FLIP_X;
	}

	viiDraw::Sprite(&spr_custom[5],pos.x,pos.y,PRIO_ATACK,atr,ARGB(col,0xff,0xff,0xff) ,scl);//,scl,pos.rot);
}


void CEffect::damage_parts()
{
	//-------------------------------------
	//�_���[�W���󂯂Ĕj�ЂƂȂ�
	//-------------------------------------

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		//opt[0]	//gxSprite *
		//opt[1]	//atr
		//opt[2]	//color
		//opt[3]	//rot
		//opt[4]	//scl
		//opt[5]	//�I�[�o�[�ȕ\���t���O
		rot = opt[3];
		free.w[0] = opt[4];	//�X�P�[��
		free.w[1] = (viiSub::Rand()%2)? 2 : -2;	//��]���x
		free.w[2] = 20+viiSub::Rand()%30;		//�����^�C�}�[
		pos.my = -100+viiSub::Rand()%50;
		pos.mx = -50+viiSub::Rand()%100;
		if(opt[5]){
			free.w[2] *= opt[5];
			pos.my *= opt[5];
			pos.mx *= opt[5];
		}
		if(!pGame->is_gravit()){
			if(viiSub::Rand()%2==0) pos.my *= -1;
		}
	}

	gxSprite *pSpr = &Sprite;//(gxSprite*)opt[0]; 64bit�Ή�
	Sint32 atr = opt[1];
	Sint32 col = opt[2];

	if( pSpr == NULL )
	{
		exist_ = gxFalse;
		return;
	}


	float scl = free.w[0]/100.0f;
	rot += free.w[1];


	if(pGame->is_gravit()){
		pos.my += 8;
		if(pos.my >= 300)	pos.my = 300;
	}

	pos.x += pos.mx;
	pos.y += pos.my;

	viiDraw::Sprite(pSpr,pos.x,pos.y,PRIO_EFFECT+3,atr,col,scl,rot);
//	viiDraw::Sprite(pSpr,pos.x,pos.y,PRIO_EFFECT+5,atr|ATR_ALPHA_PLUS,ARGB(0xf0,0xff,0xff,0xff),scl,rot);

	free.w[2] --;
	if((viiSub::GetBoundBlock_Enemy(pos.x,pos.y) && timer_>20 && opt[5]==0) || free.w[2]==0 ){
		_opt_[0] = 0;
		_opt_[1] = 0;
		//viiEff::Set(EFF_BOMB_PARTS,pos.x,pos.y,o);
		new CEffBomb( x_ , y_ , _opt_[0] , _opt_[1] );
		exist_ = gxFalse;
	}

}


void CEffect::release_parts()
{
	//-------------------------------------
	//�_���[�W���󂯂Ĕj�ЂƂȂ�i�������Ȃ��j
	//-------------------------------------
	gxSprite *pSpr = &Sprite;	//(gxSprite*)opt[0];	64bit�Ή�

	Sint32 atr = opt[1];
	Sint32 col = opt[2];

	if (is_init()) 
	{
		pos.x = x_;
		pos.y = y_;
		//opt[0]	//gxSprite *
		//opt[1]	//atr
		//opt[2]	//col
		free.w[0] = opt[3]; //rot
		free.w[1] = opt[4];	//spd

		pos.mx = (viiMath::Cos100( free.w[0] )*free.w[1])/100;
		pos.my = (viiMath::Sin100( free.w[0] )*free.w[1])/100;
	}


	float scl = 1.f;

	if(pGame->is_gravit() && free.w[1] == 0 )
	{
		pos.my += 8;

		if(pos.my >= 300)
		{
			pos.my = 300;
		}
	}

	Sint32 spd = free.w[1]/2;

	if( spd <= 0 )	spd = 1;

	free.w[1] -= spd;

	if( free.w[1] <= 0 )
	{
		free.w[1] = 0;
		if( pos.my < 300 ) pos.my += 8;
	}
	else
	{
		pos.mx = (viiMath::Cos100( free.w[0] )*free.w[1])/100;
		pos.my = (viiMath::Sin100( free.w[0] )*free.w[1])/100;
	}

	pos.x += pos.mx;
	pos.y += pos.my;

	viiDraw::Sprite( pSpr , pos.x , pos.y , PRIO_EFFECT+3 , atr , col , scl , 0 );

	if( viiSub::GetBoundBlock_Enemy( pos.x , pos.y ) )//&& timer_>20 )
	{
		exist_ = gxFalse;
	}

}


void CEffect::human()
{
	//-------------------------------------
	//�l:_opt_[0] = �p�^�[���w:�p�^�[���x
	//-------------------------------------
	static const Sint32 human[][8]=
	{
		{0+0*16,1+0*16,0+0*16,2+0*16},
		{0+1*16,1+1*16,0+1*16,2+1*16},
		{0+2*16,1+2*16,0+2*16,2+2*16},	
		{0+3*16,1+3*16,0+3*16,2+3*16},
		{0+4*16,1+4*16,0+4*16,2+4*16},
		{0+5*16,1+5*16,0+5*16,2+5*16},	
		{0+6*16,1+6*16,0+6*16,2+6*16},
		{0+7*16,1+7*16,0+7*16,2+7*16},
		{0+8*16,1+8*16,0+8*16,2+8*16},	
		{0+9*16,1+9*16,0+9*16,2+9*16},	
	};
	Sint32 atr = ATR_DFLT;
	gxSprite spr;

	if (is_init()) {
		free.b[0] = 0;//����
		free.b[1] = 0;//���s�J�E���^
		free.w[1] = -305;//�d��
		free.w[2] = 60+viiSub::Rand()%30;//���x
		pObj = new CObj();
		pObj->pos.x = x_;
		pObj->pos.y = y_;
	}
	if(viiSub::IsScrollOut(pObj->pos.x,pObj->pos.y,1600,1600)){
		exist_ = gxFalse;
		delete pObj;
		pObj = NULL;
		return;
	}

	if(timer_%8==0)
	{
		free.b[1]++;
		free.b[1] = free.b[1]%4;
	}
	if(pObj->is_landnow()){
		free.w[1] = 0;
	}

	pObj->mov.x += free.b[0]*4;
	pObj->mov.y = free.w[1]+120;
	if(pObj->mov.x <= -free.w[2]) pObj->mov.x = -free.w[2];
	if(pObj->mov.x >=  free.w[2]) pObj->mov.x = free.w[2];

	if(pObj->easy_collision_check() || free.b[0] == 0){
		if(GetTargetPlayer()->x < pObj->pos.x){
			free.b[0] = 1;
		}else{
			free.b[0] = -1;
		}
	}else{
		free.b[1] = 1;
		if(free.w[1] == 0){
			free.w[1] = -310;//�d�͂Ŋ���؂�Ă̓_��
		}else{
			free.w[1] += 12;
		}
	}

	pObj->pos.x += pObj->mov.x;
	pObj->pos.y += pObj->mov.y;

	Sint32 dr = 1;
	if(pObj->mov.x < 0) dr = -1;

	Sint32 no = human[ opt[0] ][ free.b[1] ];

	spr.page = TPG_HUMAN;
	spr.u = no%16*16;
	spr.v = no/16*16;
	spr.w = 16;
	spr.h = 16;
	spr.cx = 8;
	spr.cy = 16;

	switch(opt[0]){
	case 1:
		dr = dr*-1;
		break;
	}
	if(dr < 0) atr |= ATR_FLIP_X;

	viiDraw::Sprite(&spr,pObj->pos.x,pObj->pos.y,PRIO_ENEMY,atr);

}

void CEffect::human_damage()
{
	//-------------------------------------
	//�g�t�l�`�m�A�E�g
	//-------------------------------------
	Sint32 atr = ATR_DFLT;
	gxSprite spr;

	if (is_init()) {
		rot = 0;
		xx_ = 0;
		yy_ = -120+viiSub::Rand()%100;
	}
	if(viiSub::IsScrollOut(x_,y_,1600,1600)){
		exist_ = gxFalse;
		return;
	}

	if(yy_ <= 420){
		yy_ += 5;
	}

	x_ += xx_;
	y_ += yy_;

	spr.page = TPG_HUMAN;
	spr.u = 6*16;
	spr.v = 1*16;
	spr.w = 16;
	spr.h = 16;
	spr.cx = 8;
	spr.cy = 16;

	if(opt[1] < 0) atr |= ATR_FLIP_X;

	viiDraw::Sprite(&spr,x_,y_,PRIO_EFFECT,atr,ARGB_DFLT,1.0f,rot);
	rot ++;
}

void CEffect::engineer()
{
	//-------------------------------------
	//�G���W�j�A
	//-------------------------------------
	static const Sint32 human_eng[][8]={
		{0+0*16,1+0*16,0+0*16,2+0*16},
	};
	Sint32 atr = ATR_DFLT;
	gxSprite spr;

	if (is_init()) {
		free.b[0] = -1;//�����i���Œ�j
		free.b[1] = 0;//���s�J�E���^
		free.w[1] = 0;//�d��
		free.w[2] = 90*100;//�ړ�����
		pos.rot = 0;
		pObj = new CObj();
		pObj->pos.x = x_;
		pObj->pos.y = y_;
		pObj->mov.x = -(40+viiSub::Rand()%40);
		if(opt[1] == 1){
			pObj->mov.x = -40;
		}
	}


	if(free.w[2] <= -20*100){
		exist_ = gxFalse;
		delete pObj;
		pObj = NULL;
		return;
	}

	if(timer_%8==0)
	{
		free.b[1]++;
		free.b[1] = free.b[1]%4;
	}

	pObj->mov.y = free.w[1]+120;

	if(pObj->easy_collision_check()){
	}else{
		free.w[1] += 12;
		pos.rot += 3;
	}
	if(pObj->is_landnow()){
		free.w[1] = 0;
	}

	pObj->pos.x += pObj->mov.x;
	pObj->pos.y += pObj->mov.y;
	free.w[2] += pObj->mov.x;

	//�W�����v���ăV���g���ɏ�荞��
	if(opt[1] == 1 && free.w[2]<=5*100 && free.w[1] == 0) {
		free.w[1] = -270;//�d�͂Ŋ���؂�Ă̓_��
		pObj->mov.x = -70;
	}

	if(pObj->mov.x < 0) atr |= ATR_FLIP_X;

	Sint32 no = human_eng[ 0 ][ free.b[1] ];

	spr.page = TPG_HUMAN;
	spr.u = no%16*16;
	spr.v = no/16*16;
	spr.w = 16;
	spr.h = 16;
	spr.cx = 8;
	spr.cy = 16;

	viiDraw::Sprite(&spr,pObj->pos.x,pObj->pos.y,PRIO_ENEMY,atr,ARGB_DFLT,1.0f,pos.rot);

}

void CEffect::engineer2()
{
	//-------------------------------------
	//�i�ߎ��̃G���W�j�A
	//-------------------------------------
	static const Sint32 human_eng[][8]={
		{0+3*16,1+3*16,2+3*16,3+3*16},
	};
	Sint32 atr = ATR_DFLT;
	gxSprite spr;

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		free.w[0] = 1;
		free.w[1] = 0;
	}


	if(free.w[0] == 1) {
		pos.mx = 80+viiSub::Rand()%80;
		free.w[0] = 0;
	}

	pos.x += pos.mx;
	free.w[1] += pos.mx;
	if(free.w[1] >= 300*100) {
		exist_ = gxFalse;
		return;
	}

	if(pos.mx > 0) {
		pos.mx --;
		if(pos.mx <= 10) free.w[0] = 1;
	}

	Sint32 n = 0;
	if(pos.mx < 80) n = 1;
	if(pos.mx < 40) n = 2;
	if(pos.mx < 20) n = 3;

	Sint32 no = human_eng[ 0 ][ n ];

	spr.page = TPG_HUMAN;
	spr.u = no%16*16;
	spr.v = no/16*16;
	spr.w = 16;
	spr.h = 16;
	spr.cx = 8;
	spr.cy = 16;

	viiDraw::Sprite(&spr,pos.x,pos.y,PRIO_ENEMY,atr,ARGB_DFLT);//,1.0f,pos.rot);

}


void CEffect::ene_burnia_damage()
{
	//�G���W���g���u���i�j�ЂɕύX�j

	if(is_init()){
		x_ += (-3+viiSub::Rand()%6)*100;
		free.w[0] = 0;
		free.w[1] = 50+viiSub::Rand()%50;		//�傫��
		free.w[2] = viiSub::Rand()%11;
		pos.mx = opt[0];	//�ړ������w
		pos.my = opt[1];	//�ړ������x
		pos.rot = viiSub::Rand()%360;
		if( opt[2] == -1 )
		{
			free.w[1] = 50+viiSub::Rand()%100;		//�傫��
		}
	}

	if(viiSub::IsScrollOut(x_,y_,1600,12800)){
		exist_ = gxFalse;
		return;
	}

//	Sint32 col   = 0xff;
	float scl = free.w[1]/100.0f;
	pos.rot += 4;

//	viiSub::SetGravitMove(x_,y_);

	x_ += pos.mx;
	y_ += pos.my;

	Sint32 x,y;
	x = x_;
	y = y_;

	viiDraw::Sprite(&spr_broken[ free.w[2] ],x,y,PRIO_EFFECT,ATR_DFLT,ARGB(0xff,0xff,0xff,0xff),scl,pos.rot);

}


void CEffect::map_breakparts()
{
	//�}�b�v�`�b�v�j��

	if(is_init()){
		x_ += (-3+viiSub::Rand()%6)*100;
		free.w[0] = 0;
		free.w[1] = 50+viiSub::Rand()%50;		//�傫��
		free.w[2] = viiSub::Rand()%(13-2);		//�}�b�v�p�[�c�ԍ��i���Q�͊�Ȃ̂ŏ��O����j
		pos.mx = opt[0];	//�ړ������w
		pos.my = opt[1];	//�ړ������x
		pos.rot = viiSub::Rand()%360;
	}

	if(viiSub::IsScrollOut(x_,y_,3200,12800)){
		exist_ = gxFalse;
		return;
	}

//	Sint32 col   = 0xff;
	float scl = free.w[1]/100.0f;
	pos.rot += 4;

//	viiSub::SetGravitMove(x_,y_);

	x_ += pos.mx;
	y_ += pos.my;

	Sint32 x,y;
	x = x_;
	y = y_;

	viiDraw::Sprite(&spr_broken[ free.w[2] ],x,y,PRIO_EFFECT,ATR_DFLT,ARGB(0xff,0xff,0xff,0xff),scl,pos.rot);

}


void CEffect::booster_off()
{
	//�u�[�X�^�[�͂���

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		pos.mx = opt[1];
		pos.my = opt[2];
		if(pos.mx < -200) pos.mx = -200;
		if(pos.mx >  200) pos.mx =  200;
		if(pos.my < -200) pos.my = -200;
		if(pos.my >  200) pos.my =  200;

/*
		if(pGame->is_gravit()){
			pos.mx = pos.my = 0;
		}
*/
		free.w[3] = ATR_DFLT;
	}

	Sint32 dir = opt[0];	//����

	Sint32 atr = ATR_DFLT;
	if(dir == -1){
		atr |= ATR_FLIP_X;
	}

	if(timer_%40 <=20){
		if(timer_%3==0){
			free.w[3] = ATR_ALPHA_PLUS;
			switch(viiSub::Rand()%3){
			case 0:
				free.w[3] |= ATR_FLIP_X;
			case 1:
				free.w[3] |= ATR_FLIP_Y;
				break;
			default:
				break;
			}
			free.w[4]  = (viiSub::Rand()%6)*100;
		}
		Sint32 col = viiSub::GetRandColor(0xf0);
		viiDraw::Sprite(&spr_thunder[ 0 ],pos.x-(dir*1400),pos.y+(-4+viiSub::Rand()%8)*100+free.w[4],PRIO_EFFECT,free.w[3],col,1.0f);
	}
//	viiEff::Set(EFF_BREAK_THUNDER,pos.x+(-8+viiSub::Rand()%16)*100-(dir*1400),pos.y+(-6+viiSub::Rand()%12)*100);

	if(timer_ >= 12){
		if(viiSub::Rand()%8==0){
			viiEff::Set(EFF_HITFLASH,
				pos.x+(-8+viiSub::Rand()%16)*100-(dir*1400),pos.y+(-6+viiSub::Rand()%12)*100,
				argset(0)	);
		}
	}


	if(pGame->is_gravit()){
		pos.my += 12;
		if(pos.my >= 300)	pos.my = 300;
	}

	//���������Ȃ��̂Ō��������邱�Ƃɂ���

	if(pos.mx > 0){
		pos.mx --;
	}else if(pos.mx < 0){
		pos.mx ++;
	}
	if(pos.my > 0){
		pos.my --;
	}else if(pos.my < 0){
		pos.my ++;
	}

	pos.x += pos.mx;
	pos.y += pos.my;

	if(timer_ >= 240) {
		viiEff::SetBombEffect(pos.x-(dir*1400),pos.y,8,5);
		exist_ = gxFalse;
	}

	viiDraw::Sprite(&AsSpr[6],pos.x,pos.y,PRIO_ENEMY,atr,ARGB_DFLT);

}


void CEffect::break_thunder()
{
	//���G�t�F�N�g

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		timer_ = viiSub::Rand()%30;
		free.w[1] = 0;
	}

	if(timer_%40 <=20){
		if(timer_%3==0) {
			free.w[0] = ATR_ALPHA_PLUS;
			free.w[1] = -20+viiSub::Rand()%40;
			switch(viiSub::Rand()%3){
			case 0:
				free.w[0] |= ATR_FLIP_X;
			case 1:
				free.w[0] |= ATR_FLIP_Y;
				break;
			default:
				break;
			}
		}
		Sint32 col = viiSub::GetRandColor(0xf0);
		viiDraw::Sprite(&spr_thunder[ 0 ],pos.x,pos.y,PRIO_EFFECT,free.w[0],col,1.0f,free.w[1]);
	}

	if(timer_ >= 120){
		exist_ = gxFalse;
	}
}


void CEffect::eneship_aslaunch()
{
	//�A���D���`�r�G�t�F�N�g
	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		free.w[0] = 128-viiSub::Rand()%30;
		pos.rot = viiSub::Rand()%360;
	}

	switch(opt[0]){
	case 0:	//�A���D
		if(timer_ <= 1400){
			pos.x += 5;
			pos.y += 7;
		}
		if(timer_ >= 2000){
			pos.y -= 7;
			pos.x += 10;
		}
		if(timer_ == 1500)	viiEff::Set(ENESHIP_LAUNCH,pos.x,pos.y,argset(10));
		if(timer_ == 1530)	viiEff::Set(ENESHIP_LAUNCH,pos.x,pos.y,argset(10));
		if(timer_ == 1560)	viiEff::Set(ENESHIP_LAUNCH,pos.x,pos.y,argset(10));

		viiDraw::Spr_NoScrl(&spr_sceneobj[ 1 ],pos.x,pos.y,PRIO_BG2+2,ATR_DFLT,0x80ffffff);
		if( pos.x >= 600*100) {
			exist_ = gxFalse;
		}
		break;
	case 10:	//�`�r
		pos.x += 110;
		pos.y -= 25;
		pos.rot += 5;
		viiDraw::Spr_NoScrl(&spr_sceneobj[ 2 ],pos.x,pos.y,PRIO_BG2,ATR_DFLT,ARGB(0x80,0xff,0xff,0xff),1.0f,pos.rot);
		viiDraw::Spr_NoScrl(&spr_sceneobj[ 2 ],pos.x,pos.y,PRIO_BG2,ATR_DFLT,ARGB(0x80,0xff,0xff,0xff),1.5f,-pos.rot);
		if(timer_%1==0){
			viiEff::Set(ENESHIP_LAUNCH,pos.x,pos.y,argset(20));
		}
		if( pos.x >= 600*100) {
			exist_ = gxFalse;
		}
		break;
	case 20:	//�`�r��
		free.w[0] -= 8;
		if(free.w[0]<=0) free.w[0] = 0;
		pos.rot += 8;
		viiDraw::Spr_NoScrl(&spr_sceneobj[ 2 ],pos.x,pos.y,PRIO_BG2+1,ATR_ALPHA_PLUS,ARGB(free.w[0],0xff,0xff,0xff),1.0f,pos.rot);
		viiDraw::Spr_NoScrl(&spr_sceneobj[ 2 ],pos.x,pos.y,PRIO_BG2+1,ATR_ALPHA_PLUS,ARGB(free.w[0],0xff,0xff,0xff),1.0f,-pos.rot);
		break;
	}

	if( free.w[0] == 0 ) {
		exist_ = gxFalse;
	}
}

void CEffect::masatsu_large()
{
	//���C�M�i��j

	if (is_init()) {
		free.w[0] = 128-viiSub::Rand()%30;
	}

	free.w[0] -= 8;
	if(free.w[0]<=0){
		exist_ = gxFalse;
		return;
	}

	Sint32 atr = ATR_ALPHA_PLUS;
	if(opt[0] < 0) atr |= ATR_FLIP_X;

	y_ -= 16;
	viiDraw::Sprite(&spr_sceneobj[ 3 ],x_,y_-1200,PRIO_EFFECT,atr,ARGB(free.w[0],0xff,0xff,0xff));
}

void CEffect::masatsu_small()
{
	//���C�M�i���j�@���x

	if (is_init()) {
		free.w[0] = 128-viiSub::Rand()%30;
	}

	Sint32 atr = ATR_ALPHA_PLUS;
	if(opt[0] < 0) atr |= ATR_FLIP_X;

	if(opt[0]){
		free.w[0] -= 4;
		if(free.w[0]<=0){
			exist_ = gxFalse;
			return;
		}
		y_ -= opt[0];
		viiDraw::Sprite(&spr_sceneobj[ 4 ],x_,y_-1200,PRIO_EFFECT,ATR_DFLT,ARGB(free.w[0],0xff,0xff,0xff));
		viiDraw::Sprite(&spr_sceneobj[ 4 ],x_,y_-1200-2400,PRIO_EFFECT,ATR_DFLT,ARGB(free.w[0],0xff,0xff,0xff));
	}else{
		free.w[0] -= 8;
		if(free.w[0]<=0){
			exist_ = gxFalse;
			return;
		}
		viiSub::SetGravitMove(x_,y_);
		y_ -= 224;
		viiDraw::Sprite(&spr_sceneobj[ 4 ],x_,y_-1200,PRIO_EFFECT,atr,ARGB(free.w[0],0xff,0xff,0xff));
	}
}

void CEffect::cloud_yoko()
{
	//�V���[�e�B���O�G���A�̉_
	if (is_init()) {
		pos.sx  = opt[0];	//�X�P�[��
		pos.rot = viiSub::Rand()%360;
		free.w[0] = 48;				//��
		free.w[1] = -1+2*viiSub::Rand()%2;	//��]����
		free.w[2] = viiSub::Rand()%3;		//�`
		free.w[3] = 600+viiSub::Rand()%300;	//�X�s�[�h
	}

	Sint32 atr = ATR_ALPHA_PLUS;
	if(free.w[2] == 1) atr|=ATR_FLIP_X;
	if(free.w[2] == 2) atr|=ATR_FLIP_Y;
	if(free.w[2] == 3) atr|=(ATR_FLIP_Y|ATR_FLIP_X);

	pos.rot += free.w[1]*free.w[2];

	x_ -= free.w[3];

	atr = ATR_ALPHA_PLUS;
	viiDraw::Sprite(&spr_cloud[ 0 ],x_,y_,PRIO_BG2,atr,ARGB(free.w[0],0xff,0xff,0xff),pos.sx/100.0f,pos.rot);
	viiDraw::Sprite(&spr_cloud[ 0 ],x_,y_,PRIO_BG2,atr,ARGB(free.w[0],0xff,0xff,0xff),pos.sx/100.0f,-pos.rot);

	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );
	if(x_ < (sx-320)*100){
		exist_ = gxFalse;
	}
}

void CEffect::cloud_tate()
{
	//�V���g���ǌ����̉_

	if (is_init()) {
		pos.sx  = opt[0];	//�X�P�[��
		pos.rot = viiSub::Rand()%360;
		free.w[0] = 82;				//��
		free.w[2] = viiSub::Rand()%3;		//�`
		free.w[3] = 800+viiSub::Rand()%300;	//�X�s�[�h
		free.w[4] = PRIO_BG2;

		if(opt[1]){
			//���z
			pos.sx = 50+viiSub::Rand()%10;
			free.w[1] = -1+2*viiSub::Rand()%2;	//��]����
			free.w[2] = 48;
			free.w[3] = 0;
		}
	}

	Sint32 atr = ATR_ALPHA_PLUS;
	if(free.w[2] == 1) atr|=ATR_FLIP_X;
	if(free.w[2] == 2) atr|=ATR_FLIP_Y;
	if(free.w[2] == 3) atr|=(ATR_FLIP_Y|ATR_FLIP_X);

	pos.rot += free.w[1]*free.w[2];

	y_ += free.w[3];

	atr = ATR_ALPHA_PLUS;
	if(opt[1]==1) {
		viiDraw::Spr_NoScrl(&spr_cloud[ 0 ],x_,y_,free.w[4],atr,ARGB(free.w[0],0xff,0x00,0x00),pos.sx/100.0f,pos.rot);
		viiDraw::Spr_NoScrl(&spr_cloud[ 0 ],x_,y_,free.w[4],atr,ARGB(free.w[0],0xff,0xff,0x00),pos.sx/100.0f,-pos.rot);
		viiDraw::Spr_NoScrl(&spr_cloud[ 0 ],x_,y_,free.w[4],atr,ARGB(free.w[0],0x40,0x40,0x40),1.5f,0);
	}else{
		viiDraw::Sprite(&spr_cloud[ 0 ],x_,y_,free.w[4],atr,ARGB(free.w[0],0xff,0xff,0xff),pos.sx/100.0f,pos.rot);
		viiDraw::Sprite(&spr_cloud[ 0 ],x_,y_,free.w[4],atr,ARGB(free.w[0],0xff,0xff,0xff),pos.sx/100.0f,-pos.rot);
	}
	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );
	if(y_ > (sy+640)*100 || (opt[1]==1 && timer_ > 5) ){
		exist_ = gxFalse;
	}
}

void CEffect::cloud_shuttle()
{
	//�V���g�����ˎ��̉_
	
}

void CEffect::broken_parts()
{
	//�G�j�󎞂̉��p�[�c(�p�x�A�X�s�[�h�A�ǔ���Ȃ�(1) )

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		pos.sx = 30+viiSub::Rand()%10;
		pos.mx = viiMath::Cos100(opt[0]);
		pos.my = viiMath::Sin100(opt[0]);
		free.w[0] = opt[1]+viiSub::Rand()%100;		//�X�s�[�h
		free.w[1] = 180+viiSub::Rand()%20;		//�}�b�v�p�[�c�ԍ��i���Q�͊�Ȃ̂ŏ��O����j
		free.w[2] = (viiSub::Rand()%4)*4;		//�^�C�}�[
		yy_ = 0;
		free.w[4] = viiSub::Rand()%16;

		if(opt[2] == 1) {
			//�c�����鉌
			pos.x = x_;
			pos.y = y_;
			pos.sx = opt[3]/2;
			pos.mx = viiMath::Cos100(opt[0]);
			pos.my = viiMath::Sin100(opt[0]);
			free.w[0] = opt[1]/8;
			free.w[1] = 180+viiSub::Rand()%20;	//�}�b�v�p�[�c�ԍ��i���Q�͊�Ȃ̂ŏ��O����j
			free.w[4] = viiSub::Rand()%30;
		}
	}

	if( free.w[4] > 0 )
	{
		free.w[4] --;
		return;
	}

	if(pGame->is_gravit())
	{
		//�d�͉�
		if(opt[2]==0)	yy_ += 16;
	}

	if(opt[2]==0) {
		if(free.w[2] > 0){
			free.w[2] --;
			return;
		}
	}

	if(opt[2]==0) {
		free.w[0] -= free.w[0]/32;	//�X�s�[�h
		pos.sx -=1;
		free.w[1] -= 4;				//�A���t�@
	}else{
		//free.w[0] -= free.w[0]/64;	//�X�s�[�h
		free.w[1] -= 2;				//�A���t�@
	}

	Sint32 mx,my,spd;

	spd = free.w[0];
	mx = (pos.mx*spd)/100;
	my = (pos.my*spd)/100+yy_;

	viiSub::SetGravitMove(mx,my);	//���ŉ����p���[

	pos.x += mx;
	pos.y += my;
	pos.rot += 4;

	Sint32 alpha = free.w[1];

	if(viiSub::IsScrollOut(pos.x,pos.y,8*100,8*100) || free.w[0] <= 0 || free.w[1] <= 0 || pos.sx<=0){
		exist_ = gxFalse;
		return;
	}

	float scl = pos.sx/100.0f;

	viiDraw::Sprite(&spr_cloud[ 0 ],pos.x,pos.y,PRIO_BG2+2,ATR_DFLT,ARGB(alpha,0xB0,0xB0,0xB0),scl,pos.rot);
//	viiDraw::Sprite(&spr_cloud[ 0 ],pos.x,pos.y,PRIO_BG2+2,ATR_ALPHA_PLUS,ARGB(alpha/2,0x80,0x80,0x80),scl,pos.rot);
//	viiDraw::Sprite(&spr_cloud[ 0 ],pos.x,pos.y,PRIO_BG2+2,ATR_ALPHA_MINUS,ARGB(alpha,0xff,0xff,0xff),scl,pos.rot);

	if(opt[2]) {
		return;
	}
	viiEff::Set(EFF_BROKENPARTS,pos.x,pos.y,argset(opt[0],free.w[0]/2,1,pos.sx));
}
#if 0
void CEffect::brokenCloud()
{
	//�G�j�󎞂̉�

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		pos.sx = 80+viiSub::Rand()%40;
		pos.mx = viiMath::Cos100(opt[0]);
		pos.my = viiMath::Sin100(opt[0]);

		free.w[0] = viiSub::Rand()%(13-2);	//�}�b�v�p�[�c�ԍ��i���Q�͊�Ȃ̂ŏ��O����j
		yy_ = 0;
	}

	if(pGame->is_gravit()){
		//�d�͉�
		yy_ += 3;
	}

	Sint32 mx,my,spd;
	spd = pos.sx;
	mx = (pos.mx*spd)/100;
	my = (pos.my*spd)/100;

	viiSub::SetGravitMove(mx,my);	//���ŉ����p���[

	pos.x += mx;
	pos.y += my;
	pos.rot += 4;
	pos.sx --;

	Sint32 alpha = pos.sx;

	if(viiSub::IsScrollOut(pos.x,pos.y,8*100,8*100) || pos.sx <= 0){
		exist_ = gxFalse;
		return;
	}

	float scl = pos.sx/100.0f;

	viiDraw::Sprite(&spr_cloud[ 0 ],pos.x,pos.y,PRIO_BG2+2,ATR_DFLT,ARGB(alpha,0xff,0xff,0xff),scl,pos.rot);
}
#endif

void CEffect::broken_parts_deluxe()
{
	//�G�j�󎞂̍��؂ȉ�]�̉��p�[�c(kaku,spd)

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		pos.rot = viiSub::Rand()%360;
		free.w[0] = viiSub::Rand()%1200;		//��]���x
		free.w[1] = 30;//+viiSub::Rand()%50;	//��]�ʒu
		pos.mx = (viiMath::Cos100(opt[0])*opt[1])/100;
		pos.my = (viiMath::Sin100(opt[0])*opt[1])/100;
		pos.sx = opt[2];
		yy_ = 16;
		if(opt[3] == 0) {
			opt[3] = 0xFFFFFFFF;
		}
	}

	gxBool hit_kabe = gxFalse;

	if(pGame->is_gravit()){
		//�d�͉�
		yy_ += 16;
		if(yy_ >= 400) yy_ = 400;
	}

	pos.x += pos.mx;
	pos.y += pos.my;

	if(viiSub::IsScrollOut(pos.x,pos.y,32*100,32*100)){
		exist_ = gxFalse;
		return;
	}

	float scl = pos.sx/100.0f;

	free.w[0] += free.w[1];
	free.w[0] = free.w[0]%1200;

	Sint32 spr = free.w[0]/100;

	viiDraw::Sprite(&spr_brokenDeluxe[ spr ],pos.x,pos.y,PRIO_BG2+2,ATR_DFLT,opt[3],scl,pos.rot);

}

void CEffect::broken_parts_colony()
{
	//�X�e�[�W�P�Ō�ɃR���j�[�����яo���j��(���E����)

	if(is_init()){
		x_ += (-3+viiSub::Rand()%6)*100;
		free.w[0] = 0;
		free.w[1] = 50+viiSub::Rand()%50;		//�傫��
		free.w[2] = viiSub::Rand()%(13-2);		//�}�b�v�p�[�c�ԍ��i���Q�͊�Ȃ̂ŏ��O����j
		pos.mx = (300+viiSub::Rand()%50)*opt[0];
		pos.my = 0;
		pos.rot = viiSub::Rand()%360;
	}

/*
	if(viiSub::IsScrollOut(x_,y_,3200,12800)){
		exist_ = gxFalse;
		return;
	}
*/
//	Sint32 col   = 0xff;
	float scl = free.w[1]/100.0f;
	pos.rot += 4;

	x_ += pos.mx;
	y_ += pos.my;

	free.w[0] ++;
	if(pos.mx > 0) {
		pos.mx -= 2;
		if(pos.mx < 0) pos.mx = 0;
	}
	if(pos.mx < 0) {
		pos.mx += 2;
		if(pos.mx > 0) pos.mx = 0;
	}
	if(free.w[0] >= 60){
		pos.my += 8;
		if(pos.my >= 600){
			pos.my = 600;
		}
	}else if(free.w[0] >= 600){
		exist_ = gxFalse;
		return;
	}

	Sint32 x,y;
	x = x_;
	y = y_;

	viiDraw::Sprite(&spr_broken[ free.w[2] ],x,y,PRIO_BG2+2,ATR_DFLT,ARGB(0xff,0xff,0xff,0xff),scl,pos.rot);



}

void CEffect::broken_rock()
{
	//�����

	if (is_init()) {
		Sint32 spd = 100+viiSub::Rand()%50;
		free.w[0] = 128-viiSub::Rand()%30;
		rot = viiSub::Rand()%360;//165+viiSub::Rand()%30;
		pos.x = x_;
		pos.y = y_;
		pos.mx = (viiMath::Cos100(rot)*spd)/100;
		pos.my = (viiMath::Sin100(rot)*spd)/100;
		pos.sx = 50+viiSub::Rand()%50;
	}

	if(viiSub::IsScrollOut(x_,y_,3200,3200)){
		exist_ = gxFalse;
		return;
	}

	pos.x += pos.mx;
	pos.y += pos.my;

	viiSub::SetGravitMove(pos.x,pos.y);

	Sint32 atr = ATR_DFLT;

	viiDraw::Sprite(&spr_sceneobj[ 7 ],pos.x,pos.y,PRIO_BG2,atr,ARGB_DFLT,pos.sx/100.0f);

}

void CEffect::energyball()
{
	//�G�l���M�[��
	Sint32 kaku;

	if(is_init()){
		rot = 0;
		free.w[0] = 0;					//�F
		free.w[1] = opt[0]+viiSub::Rand()%20;	//�傫��
		free.w[2] = 0;
		kaku = viiSub::Rand()%360;
		pos.x = x_;
		pos.y = y_;
		x_ = pos.x + viiMath::Cos100(kaku)*opt[1];	//����
		y_ = pos.y + viiMath::Sin100(kaku)*opt[1];	//����
		if(opt[2]) {
			//���v���C���[���W
			pos.mx = GetTargetPlayer()->x;	//X
			pos.my = GetTargetPlayer()->y;	//Y
		}
		if(opt[3] <= 0){
			opt[3] = 20;
		}
	}

	if(free.w[0] < 0x80) {
		free.w[0] += 6;
	}

	if(viiSub::IsScrollOut(x_,y_,3200,3200)){
		exist_ = gxFalse;
		return;
	}
	if(free.w[2] >= opt[3]){
		exist_ = gxFalse;
		return;
	}

	Sint32 col   = free.w[0];
	float scl = free.w[1]/100.0f;

	if(col >= 255) col = 255;
	if(col <= 0) col = 0;

	Sint32 xx_ = (pos.x-x_)/30;
	Sint32 yy_ = (pos.y-y_)/30;
	x_ += xx_;
	y_ += yy_;

	if( abs(xx_)<100 && abs(yy_)<100 ){
		free.w[2] ++;
	}

	Sint32 ax=0,ay=0;
	if(opt[2]){
		ax = GetTargetPlayer()->x - pos.mx;
		ay = GetTargetPlayer()->y - pos.my;
	}

	rot += 4;
//	viiDraw::Sprite(&spr_sceneobj[8],x_+ax,y_+ay,PRIO_EFFECT,ATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff),scl);
	viiDraw::Sprite(&spr_cloud[1],x_+ax,y_+ay,PRIO_EFFECT,ATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff),scl/5,rot);
}

void CEffect::colony_shutter()
{
	//�R���j�[�̃V���b�^�[

	if (is_init()) {
//		Sint32 spd = 100;
		free.w[0] = 0;
		pos.sx = 100;
		pos.x = x_;
		pos.y = y_;
		pos.mx = 0;
		pos.my = 0;
		rot = 0;
		if(opt[0] == 1){
			rot = viiSub::Rand()%360;
			pos.mx = 80+viiSub::Rand()%100;
			pos.my = -100+viiSub::Rand()%200;
			pos.sx = 60+viiSub::Rand()%40;
			free.w[1] = (100+viiSub::Rand()%100)*2;
			free.w[2] = 6+viiSub::Rand()%6;
		}
	}

	if(viiSub::IsScrollOut(x_,y_,3200,3200) && free.w[0] == 1){
		exist_ = gxFalse;
		return;
	}

	if(opt[0] == 1){
		//�j�Ђ̎�
		if(pos.mx >= -free.w[1]){
			pos.mx -=3;
		}
		pos.my -= pos.my/10;
		pos.x += pos.mx;
		pos.y += pos.my;
		rot += free.w[2];
		viiDraw::Sprite(&spr_sceneobj[ 9 ],pos.x,pos.y,PRIO_EFFECT,ATR_DFLT,ARGB_DFLT,pos.sx/100.0f,rot);
		if(viiSub::IsScrollOut(x_,y_,3200,3200))	exist_ = gxFalse;
		return;
	}

	pos.x += pos.mx;
	pos.y += pos.my;
/*
	if(timer_ >= 600){
		free.w[0] = 100;
	}
*/

	for(Sint32 i=0;i<10;i++){
		viiDraw::Sprite(&spr_sceneobj[ 9 ],pos.x,pos.y+(i*16)*100,PRIO_EFFECT,ATR_DFLT,ARGB_DFLT,pos.sx/100.0f,rot);
		if(opt[0] == 0 && get_sw(0)){
//			viiEff::Set(EFF_BURNIA_DAMAGE,pos.x+(-10+viiSub::Rand()%20)*100,pos.y+(i*16)*100,argset(-250+viiSub::Rand()%60,0) );
			viiEff::SetBombEffect(pos.x,pos.y+(i*16)*100,3,5);
			viiEff::Set(EFF_COLONY_SHUTTER,pos.x+(-10+viiSub::Rand()%20)*100,pos.y+(i*16)*100,argset(1));
			exist_ = gxFalse;
		}
	}


}

void CEffect::SandRock()
{
	//������

	if (is_init()) {
		Sint32 spd = 100+viiSub::Rand()%50;
		free.w[0] = 158-viiSub::Rand()%30;
		rot = viiSub::Rand()%360;//165+viiSub::Rand()%30;
		pos.x = x_;
		pos.y = y_;
		pos.mx = (viiMath::Cos100(rot)*spd)/100;
		pos.my = (viiMath::Sin100(rot)*spd)/100;
		pos.sx = 50+viiSub::Rand()%50;
	}

	if(viiSub::IsScrollOut(x_,y_,3200,3200)){
		exist_ = gxFalse;
		return;
	}
	free.w[0] -= 2;
	if(free.w[0] <= 0){
		exist_ = gxFalse;
		return;
	}

	pos.x += pos.mx;
	pos.y += pos.my;
	pos.sx -= 1;
	rot += 2;
	viiSub::SetGravitMove(pos.x,pos.y);

	Sint32 atr = ATR_DFLT;

	viiDraw::Sprite(&spr_sceneobj[ 7 ],pos.x,pos.y,PRIO_EFFECT,atr,0xc0ffffff,pos.sx/100.0f);

}


/*
#define BRIZZARD_MAX (300)
typedef struct brizzard_t {
	Sint32 x;
	Sint32 y;
	Sint32 mx;
	Sint32 my;
	Sint32 type;
}brizzard_t;
*/

void CEffect::Brizzard()
{
	//------------------
	//�V���G�t�F�N�g
	//------------------
/*
	brizzard_t *p;

	if (is_init()) {
		pMemory[0] = pGame->vmem->vmem_alloc(sizeof(brizzard_t)*BRIZZARD_MAX);
		p = (brizzard_t*)pMemory[0];
		for(Sint32 i=0;i<BRIZZARD_MAX;i++) {
			p[i].x = (viiSub::Rand()%360)*100;
			p[i].y = (viiSub::Rand()%240)*100;
			Sint32 spd = 100+viiSub::Rand()%260;
			p[i].mx = -spd;
			p[i].my = +spd+viiSub::Rand()%50;
			p[i].type = viiSub::Rand()%2;
		}
	}

	p = (brizzard_t*)pMemory[0];

	for(Sint32 i=0;i<BRIZZARD_MAX;i++)
	{
		p[i].x += p[i].mx;
		p[i].y += p[i].my;
		if(p[i].x <     0) p[i].x += 320*100;
		if(p[i].y > 24000) p[i].y -= 240*100;
		viiDraw::Spr_NoScrl(&spr_sceneobj[10+p[i].type],p[i].x,p[i].y,PRIO_EFFECT,ATR_ALPHA_PLUS ,0x80ffffff);
	}

	if(get_sw(0))
	{
		pGame->vmem->vmem_free(pMemory[0]);
		exist_ = gxFalse;
	}
*/

}

void CEffect::WeatherRain()
{
/*
	//------------------
	//�V��J�G�t�F�N�g
	//------------------
	brizzard_t *p;

	if (is_init())
	{
		pMemory[0] = pGame->vmem->vmem_alloc(sizeof(brizzard_t)*BRIZZARD_MAX);
		p = (brizzard_t*)pMemory[0];

		for(Sint32 i=0;i<BRIZZARD_MAX;i++)
		{
			p[i].x = (viiSub::Rand()%360)*100;
			p[i].y = (viiSub::Rand()%240)*100;
			Sint32 spd = 300+viiSub::Rand()%260;
			p[i].mx = -spd;
			p[i].my = +spd+viiSub::Rand()%50;
			p[i].type = viiSub::Rand()%2;
		}
	}

	p = (brizzard_t*)pMemory[0];

	for(Sint32 i=0;i<BRIZZARD_MAX;i++)
	{
		p[i].x += p[i].mx;
		p[i].y += p[i].my;
		if(p[i].x <     0) p[i].x += 320*100;
		if(p[i].y > 24000) p[i].y -= 240*100;
		viiDraw::Spr_NoScrl(&spr_sceneobj[15+p[i].type],p[i].x,p[i].y,PRIO_EFFECT,ATR_DFLT ,0xB0808080);
	}

	if(get_sw(0))
	{
		pGame->vmem->vmem_free(pMemory[0]);
		exist_ = gxFalse;
	}
*/

}


void CEffect::SnowSmoke()
{
	//------------------
	//�ቌ
	//------------------
	if (is_init())
	{
		pos.x = x_;
		pos.y = y_;
		pos.rot = -10+viiSub::Rand()%20;
		free.w[0] = 128+viiSub::Rand()%32;
		free.w[1] = 100+viiSub::Rand()%50;
		free.w[2] = viiSub::Rand()%2;
		free.w[3] = 0;
	}

	Sint32 atr = ATR_ALPHA_PLUS;
	Sint32 dir = 1;
	if(opt[0])
	{
		atr |= ATR_FLIP_X;
		dir = -1;
	}

	pos.x -= 600*dir;

	free.w[0] -= (5+viiSub::Rand()%2 - opt[1]*3);
	free.w[1] += 1;

	if(opt[1] == 0 && free.w[0]<80 && free.w[3]==0)
	{
		if(viiSub::Rand()%2==0)
		{
			//opt[1] = 1;
			viiEff::Set(EFF_SNOWSMOKE,pos.x,pos.y,argset(opt[0],1));
		}
		free.w[3] = 1;
	}

	if(free.w[0]<0){
		if(opt[1] == 0 && viiSub::Rand()%3 == 0)
		{
//			viiEff::Set(EFF_SNOWSMOKE,pos.x,pos.y,argset(opt[0],1));
		}
		exist_ = gxFalse;
		return;
	}

	Sint32 col = free.w[0];
	float scl = free.w[1]/100.0f;

	if(viiSub::Rand()%2==0)
	{
//		atr |= ATR_FLIP_Y;
	}

	gxSprite *p = &spr_sceneobj[12];
	if(opt[1]==1)
	{
		p = &spr_cloud[ 0 ];
		scl = scl /2;
	}

	viiDraw::Sprite(p,pos.x,pos.y,PRIO_EFFECT,atr ,ARGB(col,0xff,0xff,0xff),scl,pos.rot);


}

void CEffect::AirBreath()
{
	//--------------------------------
	//�G�A�u���X(�p�x,����,�傫��,������)
	//--------------------------------
	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		pos.rot = viiSub::Rand()%360;
		pos.mx = (viiMath::Cos100(opt[0])*opt[1])/100;
		pos.my = (viiMath::Sin100(opt[0])*opt[1])/100;
		free.w[0] = 128+viiSub::Rand()%32;
		free.w[1] = opt[2] + viiSub::Rand()%(1+opt[2]);
	}

	Sint32 atr = opt[3];

	pos.x += pos.mx;
	pos.y += pos.my;

	pos.mx = 99*pos.mx/100;
	pos.my = 99*pos.my/100;


	//-------------------------------------
	//�J���[
	//-------------------------------------
	free.w[0] -= 6;
	if(free.w[0]<0){
		exist_ = gxFalse;
		return;
	}
	Sint32 col = free.w[0];

	//-------------------------------------
	//�X�P�[��
	//-------------------------------------
	float scl = free.w[1]/100.0f;
	free.w[1] += 2;

	pos.rot += 4;

	viiDraw::Sprite(&spr_cloud[ 0 ],pos.x,pos.y,PRIO_EFFECT,atr,ARGB(col,0xff,0xff,0xff),scl,pos.rot);
}

void CEffect::DungeonRock()
{
	//------------------------------------------------------
	//�_���W�������̊�	(�p�x�A�X�s�[�h�A�ǔ���Ȃ�(1) )
	//------------------------------------------------------

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		pos.rot = viiSub::Rand()%360;
		pos.sx = 100;
		free.w[0] = 8+viiSub::Rand()%8;		//��]���x
		free.w[1] = 2+viiSub::Rand()%2;		//�Ԃ���J�E���^�[
//		free.w[2] = viiSub::Rand()%(13-2);	//�}�b�v�p�[�c�ԍ��i���Q�͊�Ȃ̂ŏ��O����j
		free.w[3] = 50+viiSub::Rand()%50;	//�傫��
		if(viiSub::Rand()%2==0) free.w[0] *= -1;
		Sint32 spd = (opt[1]+viiSub::Rand()%100);
		pos.mx = (viiMath::Cos100(opt[0])*spd)/100;
		pos.my = (viiMath::Sin100(opt[0])*spd)/100;
		yy_ = 16;
	}

	gxBool hit_kabe = gxFalse;

	if(pGame->is_gravit()){
		//�d�͉�
		yy_ += 16;
		if(opt[2]==0){
			if(viiSub::GetBoundBlock_Enemy(pos.x,pos.y)){
				hit_kabe = gxTrue;
				yy_ = (yy_+pos.my)*-1;
				yy_ = 1*yy_/5;
				pos.y += pos.my+yy_;
				pos.my = 0;
				pos.mx = 1*pos.mx/5;
				free.w[0] *= -1;
			}
		}
	}else{
		if(opt[2]==0){
			if(viiSub::GetBoundBlock_Player(pos.x+pos.mx,pos.y+pos.my)){
				hit_kabe = gxTrue;
				free.w[1] = 1;
			}
		}
	}

	if(hit_kabe){
		//�ǃq�b�g�J�E���g����
		free.w[1]--;
		if(free.w[1]<=0){
			exist_=gxFalse;
			return;
		}
	}

	Sint32 mx,my;
	mx = pos.mx;
	my = pos.my+yy_;
	viiSub::SetGravitMove(mx,my);

	pos.x += mx;
	pos.y += my;

	if(viiSub::IsScrollOut(pos.x,pos.y,32*100,32*100)){
		exist_ = gxFalse;
		return;
	}

	pos.rot += free.w[0];
	float scl = free.w[3]/100.0f;

	viiDraw::Sprite(&spr_sceneobj[ 7 ],pos.x,pos.y,PRIO_BG2+2,ATR_DFLT,ARGB(0xff,0xff,0xff,0xff),scl,pos.rot);
	
}

#define EFFECT_FALL_TIME (3)
void CEffect::FallEffectA()
{
	//-------------------------------------
	//��G�t�F�N�g�`�i�傫����jOPT[0] = ��̒���,opt[1] //�v���C�I���e�B
	//-------------------------------------

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		if(opt[1] == 0) opt[1] = PRIO_BG3-1;
	}

	//-------------------------------------
	//�J���[
	//-------------------------------------
/*
	free.w[0] -= 6;
	if(free.w[0]<0){
//		exist_ = gxFalse;
		return;
	}
*/

	Sint32 length = opt[0];


/*
	if(Joy[0].psh&BTN_L1){
		pGame->valken()->no_padcontrol();
		if(Joy[0].psh&JOY_U) pos.y -= 100;
		if(Joy[0].psh&JOY_D) pos.y += 100;
		if(Joy[0].psh&JOY_L) pos.x -= 100;
		if(Joy[0].psh&JOY_R) pos.x += 100;
		if(Joy[0].trg&BTN_A){
			opt[0] ++;
		}else if(Joy[0].trg&BTN_B){
			if(opt[0]>0) opt[0] --;
		}else if(Joy[0].trg&BTN_C){
			Sint32 sx,sy;
			viiSub::GetScroll_s( sx,sy );
			pos.x = sx*100;
			pos.y = sy*100;
		}

		msg_prSint32f (100,200,PRIO_DEBUG,"%d,%d,%d",pos.x/100,pos.y/100,length);
	}else{
		pGame->valken()->accept_padcontrol();
	}
*/

	Sint32 spr  = (viiSub::GameCounter()%(EFFECT_FALL_TIME*4))/EFFECT_FALL_TIME;
	Sint32 spr2 = (viiSub::GameCounter()%(EFFECT_FALL_TIME*2))/EFFECT_FALL_TIME;
	Sint32 spr3 = (viiSub::GameCounter()%(EFFECT_FALL_TIME*3))/EFFECT_FALL_TIME;
	Sint32 prio = PRIO_BG3+1;
	viiDraw::Sprite(&spr_scn_stage06[ 0+spr ],pos.x,pos.y,prio,ATR_DFLT,ARGB(0xa0,0xff,0xff,0xff));

	for(Sint32 i=0;i<length;i++){
		viiDraw::Sprite(&spr_scn_stage06[ 4+spr ] ,pos.x,pos.y+6400+3200*i,prio,ATR_DFLT,ARGB(0xa0,0xff,0xff,0xff));
	}

	prio = opt[1];
	viiDraw::Sprite(&spr_scn_stage06[ 8 ] ,pos.x,pos.y+6400+3200*length-1600,prio,ATR_DFLT,ARGB(0xa0,0xff,0xff,0xff));
	viiDraw::Sprite(&spr_scn_stage06[ 18 + spr2] ,pos.x,pos.y+6400+3200*(length)-600,prio,ATR_DFLT,ARGB(0xa0,0xff,0xff,0xff));
	viiDraw::Sprite(&spr_scn_stage06[ 20 + spr3] ,pos.x,pos.y+6400+3200*(length-1)-800,prio,ATR_DFLT,ARGB(0xa0,0xff,0xff,0xff));

	viiEff::Set(EFF_FALL_C,pos.x,pos.y+6400+3200*(length)-1600,NULL);

}

void CEffect::FallEffectB()
{
	//-------------------------------------
	//��G�t�F�N�g�a�i��������j(�����A����)
	//-------------------------------------

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
	}

	//-------------------------------------
	//�J���[
	//-------------------------------------

/*
	if(Joy[0].psh&BTN_L1){
		pGame->valken()->no_padcontrol();
		if(Joy[0].psh&JOY_U) pos.y -= 100;
		if(Joy[0].psh&JOY_D) pos.y += 100;
		if(Joy[0].psh&JOY_L) pos.x -= 100;
		if(Joy[0].psh&JOY_R) pos.x += 100;
		if(Joy[0].trg&BTN_A){
			opt[1] --;
		}else if(Joy[0].trg&BTN_B){
			opt[1] ++;
		}
		if(Joy[0].trg&BTN_Z){
			opt[0] = 1-opt[0];
		}
		if(Joy[0].trg&BTN_C){
			Sint32 sx,sy;
			viiSub::GetScroll_s( sx,sy );
			pos.x = sx*100;
			pos.y = sy*100;
		}

		msg_prSint32f (100,200,PRIO_DEBUG,"%d,%d,%d,%d",pos.x/100,pos.y/100,opt[0],opt[1]);
	}else{
		pGame->valken()->accept_padcontrol();
	}
*/

	Sint32 spr  = (viiSub::GameCounter()%(EFFECT_FALL_TIME*4))/EFFECT_FALL_TIME;
	spr = (4+(4-spr))%4;
	if(opt[0]){
		spr += 8;
	}

	Sint32 prio = PRIO_BG3;

	viiDraw::Sprite(&spr_scn_stage06[ 29+spr ],pos.x,pos.y,prio,ATR_DFLT,ARGB(0xff,0xff,0xff,0xff));

	for(Sint32 i=0;i<opt[1];i++) {
		viiDraw::Sprite(&spr_scn_stage06[ 33+spr ],pos.x,pos.y+(48+i*32)*100,prio,ATR_DFLT,ARGB(0xff,0xff,0xff,0xff));
	}



}



void CEffect::FallEffectC()
{
	//-------------------------------------
	//��G�t�F�N�g�b�i��̔򖗁j
	//-------------------------------------

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		pos.rot = viiSub::Rand()%360;
		pos.mx = -200+viiSub::Rand()%400;
		pos.my = -100+viiSub::Rand()%50;
		free.w[0] = 128+viiSub::Rand()%32;
		free.w[1] = opt[2] + viiSub::Rand()%(1+opt[2]);
	}

//	Sint32 atr = opt[3];

	pos.x += pos.mx;
	pos.y += pos.my;

	//-------------------------------------
	//�J���[
	//-------------------------------------
	free.w[0] -= 6;

	if(free.w[0]<0){
		exist_ = gxFalse;
		return;
	}

	Sint32 col = free.w[0];

	//-------------------------------------
	//�X�P�[��
	//-------------------------------------
//	float scl = free.w[1]/100.0f;
	free.w[1] += 2;

	pos.rot += 4;

	Sint32 prio = PRIO_BG3+1;
	viiDraw::Sprite(&spr_scn_stage06[ 23 ],pos.x,pos.y,prio,ATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff),1.0f,pos.rot);

}

void CEffect::Rabbit()
{
	//---------------------------------------
	//������
	//---------------------------------------
	Sint32 atr = ATR_DFLT;
//	Sint32 no = 0;

	if (is_init()) {
		free.b[0] = 1;		//����
		free.b[1] = 0;		//���s�J�E���^
		free.w[1] = 0;		//�d��
		free.w[2] = 0;		//�W�����v�t���O
		free.w[3] = 0;		//�X�v���C�g�p�^�[��
		pObj = new CObj();
		pObj->pos.x = x_;
		pObj->pos.y = y_;
		pos.mx = 0;
		pos.my = 0;
		timer_ = 1+viiSub::Rand()%64;
	}

	//----------------------------------------
	//��{�X�v���C�g��ݒ�
	//----------------------------------------
	if(timer_%4==0 && free.w[2] == 0){
		free.w[3] = 0;
		if(viiSub::Rand()%8==0) free.w[3] = 1;
		if(timer_%32==0){
			if(viiSub::Rand()%2==0) free.b[0] = -1;
			else			free.b[0] =  1;
		}
		if(viiSub::Rand()%44==32) {
			//if(Joy[0].trg&BTN_X) {
				pos.my = -(220+viiSub::Rand()%20);
				pos.mx = (-5+viiSub::Rand()%10)*10;
				if(pObj->pos.x < x_-30*100){
					pos.mx = (5+viiSub::Rand()%10)*10;
				}
				if(pObj->pos.x > x_+30*100){
					pos.mx = (-5+viiSub::Rand()%10)*10;
				}
				free.w[2] = 8;
			//}
		}
	}


	if(viiSub::IsScrollOut(pObj->pos.x,pObj->pos.y,1600,1600)){
/*
		exist_ = gxFalse;
		delete pObj;
		pObj = NULL;
*/
		return;
	}


	pObj->mov.x = pos.mx;
	pObj->mov.y = pos.my+120;

	if( pObj->easy_collision_check() ){
		if(free.w[2] > 0){
			free.w[2] --;
			free.w[3] = 3;
		}
		pos.my = 0;
		pos.mx = 0;
	}else{
		pos.my += 10;
		free.w[3] = 4;
	}
/*
	if(pObj->is_landnow()){
		free.w[1] = 0;	//�d�͒l���N���A
	}else{
		free.w[1] += 10;
	}
*/
	pObj->pos.x += pObj->mov.x;
	pObj->pos.y += pObj->mov.y;


	//----------------------------
	//�����̐���
	//----------------------------
	if(pObj->mov.x < 0) free.b[0] = -1;
	if(pObj->mov.x > 0) free.b[0] =  1;

	Sint32 dr = free.b[0];
	if(dr < 0) atr |= ATR_FLIP_X;

	viiDraw::Sprite(&spr_scn_stage06[24+free.w[3]],pObj->pos.x,pObj->pos.y,PRIO_ENEMY,atr);

}

void CEffect::Kunoichi()
{
	//---------------------------------------
	//�j�t�m�n�h�b�g�h
	//---------------------------------------
	Sint32 atr = ATR_DFLT;
	Sint32 spr = free.w[3];
	Sint32 spd = 40;
	Sint32 joypad = 0;//Joy[0].psh;

	if (is_init()) {
		free.b[0] = 1;		//����
		free.b[1] = 0;		//���s�J�E���^
		free.b[2] = 0;		//�N�b�V�����J�E���^
		free.w[3] = 0;		//�X�v���C�g�p�^�[��
		free.w[4] = 0;		//�W�����v�J�E���^
		pObj = new CObj();
		pObj->pos.x = x_;
		pObj->pos.y = y_;
		pos.mx = 0;
		pos.my = 0;
		timer_ = 1+viiSub::Rand()%64;
	}

	//�����^�]
	Sint32 tx = GetTargetPlayer()->x;
	if(tx > pObj->pos.x +2400) {
		joypad = JOY_R;
	}else if(tx < pObj->pos.x-6400) {
		joypad = JOY_U;
	}else if(tx < pObj->pos.x-2400) {
		joypad = JOY_L;
	}

	if(joypad&JOY_U && free.w[4]==0) {
		pos.my = -320;
		free.w[4] = 1;
	}

	if(free.w[4] == 0) {
		if(joypad&JOY_L) {
			free.b[1] --;
			free.b[0] = -1;
			pos.mx = -spd;
		}else if(joypad&JOY_R) {
			free.b[1] ++;
			free.b[0] = 1;
			pos.mx = spd;
		}else{
			pos.mx = 0;
			if(free.b[0] > 0) {
				if(free.b[1] != 0) {
					pos.mx = 10;
					free.b[1] ++;
				}
			}else if(free.b[0] < 0) {
				if(free.b[1] != 0) {
					pos.mx = -10;
					free.b[1] --;
				}
			}
		}
	}else{
		pos.mx = -80;
	}

	//----------------------------------------
	//��{�X�v���C�g��ݒ�
	//----------------------------------------
	Sint32 walk_tbl[] = {0,1,2,3};
	free.b[1] = ( (4*9)+free.b[1] )%(4*9);
	Sint32 walk = free.b[1]/9;
	walk = walk%4;
	spr = walk_tbl[walk];

	//�W�����v��
	if(free.w[4] > 0) {
		Sint32 jump_tbl[] = {4,4,4,5,6,7,8,9,10};
		walk = free.w[4] / 5;
		if(walk >= 8) walk = 8;
		spr = jump_tbl[walk];
		free.w[4] ++;
	}
	//���Ⴊ�ݒ�
	if(free.w[5] > 0) {
		free.w[5] --;
		spr = 11;
		free.b[1] = 0;
	}else{
		if(pos.mx == 0) {
			spr = 12;
		}
	}

/*
	if(viiSub::IsScrollOut(pObj->pos.x,pObj->pos.y,1600,1600)){
		exist_ = gxFalse;
		delete pObj;
		pObj = NULL;
		return;
	}
*/

	pObj->mov.x = pos.mx;
	pObj->mov.y = pos.my+120;

	if( pObj->easy_collision_check() ){
		if(free.w[4] > 0) {
			free.w[4] = 0;
			free.w[5] = 5;
		}
		pos.my = 0;
		pos.mx = 0;
	}else{
		pos.my += 10;
		//spr = 4;
	}

	pObj->pos.x += pObj->mov.x;
	pObj->pos.y += pObj->mov.y;


	//----------------------------
	//�����̐���
	//----------------------------

	viiDraw::Sprite(&spr_kunoichi[spr],pObj->pos.x,pObj->pos.y,PRIO_ENEMY,atr);
	free.w[3] = spr;

}

void CEffect::ArmySniper()
{
	
}

void CEffect::ArmyJeep()
{
	
}

void CEffect::launch_other_as()
{
	//---------------------------------------
	//���̃o�[�V�X����o������`�r
	//---------------------------------------
	Sint32 spr = 1;
	if (is_init()) {
		static Sint32 n = 0;
		pos.sx = opt[0];
		pos.x = x_;
		pos.y = y_;
		pos.mx = 0;
		free.w[0] = -1;
		free.w[1] = 10*(viiSub::Rand()%(n+4));
		free.w[2] = 0;
		n ++;
		n = n%4;
	}

	switch(free.w[0]){
	case -1:
		if(free.w[1]>0) free.w[1] --;
		else 			free.w[0] = 0;
		break;
	case 0:
		pos.mx += 8;
		if(pos.mx >= 400) {
			pos.mx = 400;
		}
		free.w[2] ++;
		if(free.w[2] >= 50){
			pos.my = -50;
			spr = 0;
		}
		if(free.w[2] >= 60){
			free.w[0] = 1;
		}
		break;
	case 1:
		free.w[2] ++;
		spr = 0;
		pos.my =  0;
		pos.mx -= 4;
		break;
	}

	pos.x += pos.sx*(pos.mx)/100+100;
	pos.y += pos.sx*(pos.my)/100;

	viiDraw::Sprite(&other_as[spr],pos.x,pos.y,PRIO_BG1+1,ATR_DFLT,ARGB_DFLT,pos.sx/100.0f);

	for(Sint32 i=0;i<5;i++) {
		viiDraw::Sprite(&other_as[2+viiSub::Rand()%2]  ,pos.x+(-1600*pos.sx/100),pos.y+(-1+viiSub::Rand()%2)*100,PRIO_BG1+1,ATR_ALPHA_PLUS,0x40ffffff,pos.sx/100.0f);
	}

	if(free.w[2] >= 60*10 && viiSub::IsScrollOut(pos.x,pos.y,6400,6400)){
		exist_ = gxFalse;
	}
}

extern gxSprite spr_bombbeam[];

void CEffect::bombbeam()
{
	//--------------------------------------
	//�������̌���
	//--------------------------------------
	static Sint32 rot_stat = 0;
	Sint32 alpha;
	if (is_init()) {
		rot_stat += 30;
		pos.x = x_;
		pos.y = y_;
//		pos.rot = (rot_stat+viiSub::Rand()%30)%360;
		pos.rot = (rot_stat+viiSub::Rand()%360)%360;
		pos.my  = (viiSub::Rand()%2)? 2 : 4;
		pos.sx = 100;
		pos.my  *= (viiSub::Rand()%2)? -1 : 1;
		free.w[0] = 128+viiSub::Rand()%64;
	}
	alpha = 0xFF*free.w[0]/255;
	if(alpha <= 0) {
		exist_ = gxFalse;
		return;
	}
	pos.rot += pos.my;
	viiDraw::Sprite(&spr_bombbeam[0],pos.x,pos.y,opt[0],ATR_ALPHA_PLUS,ARGB(alpha,0xFF,0xFF,0xFF),pos.sx/100.0f,pos.rot);

	if(free.w[0] > 0) {
		free.w[0] -=4;
	}

}

void CEffect::EndOfSoldier()
{
	//-------------------------------------
	//�N���t�g���痎���镺�m
	//-------------------------------------
	if (is_init()) {
		xx_ = 0;
		yy_ = -180;
		pos.rot = 0;
	}

	pos.rot ++;
	if(yy_ < 800) yy_ += 8;

	x_ += xx_;
	y_ += yy_;

	viiSub::SetGravitMove(x_,y_);

	viiDraw::Sprite(&Spr07AirCraft[2],x_,y_,PRIO_EFFECT,ATR_DFLT,ARGB_DFLT,1.0f,pos.rot);

	if(viiSub::IsScrollOut(x_,y_,6400,6400)){
		exist_ = gxFalse;
	}

}


void CEffect::RunningVerdark()
{
	//-------------------------------------
	//�x���}�[�N�E�o
	//-------------------------------------
	Sint32 atr = ATR_DFLT;
	Sint32 spr;
	
	if (is_init()) {
		free.b[0] = 1;		//����
		free.b[1] = 0;		//���s�J�E���^
		free.w[1] = -305;	//�d��
		free.w[2] = 80;//���x
		free.w[3] = 0;		//���n�`�F�b�N
		pObj = new CObj();
		pObj->pos.x = x_;
		pObj->pos.y = y_;
	}

	if(timer_ < 40) {
		spr = 0;
		goto DISP_VERDARK;
	}

	if(viiSub::IsScrollOut(pObj->pos.x,pObj->pos.y,1600,1600)){
		exist_ = gxFalse;
		delete pObj;
		pObj = NULL;
		return;
	}

	if(timer_%8==0)	{
		free.b[1]++;
		free.b[1] = free.b[1]%4;
	}
	if(pObj->is_landnow()){
		free.w[1] = 0;
	}

	pObj->mov.x += free.b[0]*4;
	pObj->mov.y = free.w[1]+120;
	if(pObj->mov.x <= -free.w[2]) pObj->mov.x = -free.w[2];
	if(pObj->mov.x >=  free.w[2]) pObj->mov.x = free.w[2];

	if(pObj->easy_collision_check()){
		//���n��
		free.b[0] = 1;		//�E�����Œ�
		spr = 3+free.b[1];	//�_�b�V���p�^�[��
		if(free.w[3] == 1) {
			free.w[3] = -8;
		}
	}else{
		//�W�����v��
		if(free.w[1]< 0) spr = 0; else spr = 1;
		free.b[1] = 1;

		if(free.w[1] == 0){
			free.w[1] = -310;//�d�͂Ŋ���؂�Ă̓_��
		}else{
			free.w[1] += 12;
		}
		free.w[3] = 1;
	}

	if(free.w[3] < 0) {
		free.w[3] ++;
		pObj->mov.x = pObj->mov.x/2;
		spr = 2;
	}

	pObj->pos.x += pObj->mov.x;
	pObj->pos.y += pObj->mov.y;

DISP_VERDARK:

	viiDraw::Sprite(&Spr07VermarkEscape[spr],pObj->pos.x,pObj->pos.y,PRIO_ENEMY,atr);
}

void CEffect::RunningSeijika()
{
	
}

void CEffect::RunningShermark()
{
	
}

void CEffect::KillMyselfShelmark()
{
	//----------------------------------
	//�V�F���}�[�N�哝�̏I��
	//----------------------------------
	static Sint32 spr_ptndat[]={
		0,0,0,0,0,0,0,0,1,2,2,3,4,4,3,2,2,5,5,5,5,5,5,5,5,5,6,6,7,8,9,10,-1
	};

	if (is_init()) {
		free.w[0] = 0;
		free.w[1] = 1;
		free.w[2] = 0;
	}

	//�A�j���J�E���^��i�߂�
	if(get_sw(0)) {
		free.w[0] += free.w[1];
	}

	Sint32 sprptn = 0;
	Sint32 cnt = free.w[0] / 8;

	sprptn = spr_ptndat[cnt];

	//�ŏI�p�^�[���܂ł����Ă���΃A�j���ԍ��P�O�ɌŒ�
	if(sprptn == -1) {
		free.w[1] = 0;
		sprptn = 10;
		free.w[2] ++;
		if(free.w[2] >= 180) {
			sw_on(1);
		}
	}

	viiDraw::Sprite(&spr_shermark[sprptn],x_,y_,opt[0]);
}


void CEffect::tama_hitflash_ene()
{
	//�G�o���J�����e�G�t�F�N�g

	if (is_init()) {
		//opt[0]	//�ǂɒ��e�i0�j/ �G�ɒ��e(1)
		pos.x = x_;
		pos.y = y_;
		pos.rot = viiSub::Rand()%360;
		pos.sx = 100;
		free.w[0] = 200+viiSub::Rand()%50;	//�F����
	}

	if(viiSub::IsScrollOut(x_,y_,32*100,32*100)){
		exist_ = gxFalse;
		return;		//��ʊO�ł͕`�悵�Ȃ�
	}

	Sint32 col;
	if(free.w[0] > 0){
		free.w[0] -= 16;
		if(free.w[0] <= 0){
			free.w[0] = 0;
			exist_ = gxFalse;
		}
		pos.sx += 8;
		pos.rot += 24;
	}
	col = free.w[0];

	float scl = pos.sx / 100.0f;

	Sint32 spr = 5;

	viiDraw::Sprite(&spr_hitflash[spr],x_,y_,PRIO_EFFECT,ATR_DFLT,ARGB(col,0xff,0xff,0xff),scl,0);
	viiDraw::Sprite(&spr_hitflash[spr],x_,y_,PRIO_EFFECT,ATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff),scl*1.5f,pos.rot);
	viiDraw::Sprite(&spr_hitflash[0],x_,y_,PRIO_EFFECT,ATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff),scl*1.2f,-pos.rot);

}



void CEffect::bomb_hitflash_ene()
{
	//�G�{�����e�G�t�F�N�g

	if (is_init()) {
		pos.y = y_;
		xx_ = 0;
		if(opt[0]==0){
			//viiEff::SetBombEffect(x_,y_,3,3);
			yy_ = -100;
		}else{
			//����
			xx_ = 0;
			yy_ = 0;
		}
		rot = viiSub::Rand()%360;
		free.w[0] = 200-viiSub::Rand()%50;	//�J���[
	}

	Sint32 col = ARGB((free.w[0]),0xff,0xff,0xff);
	free.w[0] -= 6;
	if(free.w[0] <= 0){
		exist_ = gxFalse;
		return;
	}

	if(viiSub::IsScrollOut(x_,y_,6400,6400)){
		exist_ = gxFalse;
		return;
	}

	yy_ -= 12;

	x_ += xx_;
	y_ += yy_;

	rot += 8;

	viiDraw::Sprite(&spr_bomb[0],x_,y_,PRIO_EFFECT,ATR_ALPHA_PLUS,col,0.6f,-rot);

	if(opt[0] == 0){
		viiEff::Set(EFF_BOMBFLASH_ENE,x_+(-4+viiSub::Rand()%8)*100,pos.y+(-4+viiSub::Rand()%8)*100,argset(1));
		viiDraw::Sprite(&spr_bomb[0],x_,pos.y,PRIO_EFFECT,ATR_ALPHA_PLUS,col,0.6f,-rot);
	}

}



