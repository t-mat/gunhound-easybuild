//----------------------------------------------
//�ڐA�ς�
//----------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"

void CEffect::SandStorm()
{
//ENE	//����
//ENE
//ENE	if (is_init()) {
//ENE		Sint32 spd = 100+viiSub::Rand()%50;
//ENE		free.w[0] = 158-viiSub::Rand()%30;
//ENE		rot = viiSub::Rand()%360;//165+viiSub::Rand()%30;
//ENE		pos.x = x_;
//ENE		pos.y = y_;
//ENE		pos.mx = (viiMath::Cos100(rot)*spd)/100;
//ENE		pos.my = (viiMath::Sin100(rot)*spd)/100;
//ENE/*
//ENE		if(opt[0]){
//ENE			pos.mx = (viiMath::Cos100(rot)*spd)/100;
//ENE			pos.my = (viiMath::Sin100(rot)*spd)/100;
//ENE		}
//ENE*/
//ENE		pos.sx = 50+viiSub::Rand()%50;
//ENE	}
//ENE
//ENE	if(viiSub::IsScrollOut(x_,y_,3200,3200)){
//ENE		exist_ = gxFalse;
//ENE		return;
//ENE	}
//ENE	free.w[0] -= 2;
//ENE	if(free.w[0] <= 0){
//ENE		exist_ = gxFalse;
//ENE		return;
//ENE	}
//ENE
//ENE	pos.x += pos.mx;
//ENE	pos.y += pos.my;
//ENE	pos.sx += 1;
//ENE	rot += 2;
//ENE	viiSub::SetGravitMove(pos.x,pos.y);
//ENE
//ENE	Sint32 atr = ATR_DFLT;
//ENE
//ENE	viiDraw::Sprite(&spr_cloud[ 0 ],pos.x,pos.y,PRIO_EFFECT,atr,ARGB(free.w[0],0xff,0xff,0xff),pos.sx/100.0f);
//ENE
}


void CEffect::ArmyGun()
{
//ENE	//---------------------------------------
//ENE	//�_���A�[�~�[
//ENE	//---------------------------------------
//ENE	extern gxSprite spr_army01[];
//ENE	Sint32 atr = ATR_DFLT;
//ENE	Sint32 spr = free.w[3];
//ENE	Sint32 spd = 80;
//ENE	Sint32 joypad = 0;//Joy[0].psh;
//ENE
//ENE	if (is_init()) {
//ENE		m_HitKurai.SetDirectHitPosition( 1 );
//ENE		free.b[0] = 1;		//����
//ENE		free.b[1] = 0;		//���s�J�E���^
//ENE		free.b[2] = 0;		//�N�b�V�����J�E���^
//ENE		free.w[3] = 0;		//�X�v���C�g�p�^�[��
//ENE		free.w[4] = 0;		//�W�����v�J�E���^
//ENE		free.w[5] = 0;		//�E�F�C�g�J�E���^
//ENE		free.w[6] = 0;		//�U���J�E���^
//ENE		free.w[7] = 0;		//�s���X�e�[�^�X
//ENE		free.w[8] = 0;		//�s���E�F�C�g
//ENE		free.w[9] = -80;		//����^�C�}�[
//ENE		free.w[10] = 0;			//����t���O
//ENE		pObj = new CObj();
//ENE		pObj->pos.x = x_;
//ENE		pObj->pos.y = y_;
//ENE		pos.mx = 0;
//ENE		pos.my = 0;
//ENE		yy_ = 0;
//ENE		timer_ = 1+viiSub::Rand()%64;
//ENE	}
//ENE
//ENE	if(m_HitKurai.is_dead()){
//ENE		if(free.w[10] == 0) {
//ENE			free.w[10] = 1;
//ENE			viiMus::PlaySound(SE_TIBIYARARE);
//ENE		}
//ENE		free.w[9] +=8;
//ENE		Sint32 sd = free.w[9];
//ENE		if(sd >= 300) sd = 300;
//ENE		pObj->pos.x  += pObj->mov.x;
//ENE		pObj->pos.y  += sd;
//ENE		if(viiSub::IsScrollOut(pObj->pos.x,pObj->pos.y,1600,1600)){
//ENE			exist_ = gxFalse;
//ENE		}
//ENE
//ENE		atr = ATR_DFLT;
//ENE		if(free.b[0] == 1) atr |= ATR_FLIP_X;
//ENE		viiDraw::Sprite(&spr_army01[10],pObj->pos.x,pObj->pos.y,PRIO_ENEMY+5,atr);
//ENE		return;
//ENE	}
//ENE
//ENE	//�����^�]
//ENE	Sint32 tx = GetTargetPlayer()->x;
//ENE	Sint32 dst;
//ENE	switch(free.w[7]){
//ENE	case 0:
//ENE		//�������Ă��Ȃ�
//ENE		dst = get_valken_dist(pObj->pos.x,pObj->pos.y);
//ENE		if( dst <= 5000) {
//ENE			//�����p�^�[���ɓ���
//ENE			free.w[7] = 30;
//ENE			free.w[8] = 120+(viiSub::Rand()%8)*10;
//ENE		}else  if(dst >= 16000) {
//ENE			//�߂Â��p�^�[���ɓ���
//ENE			free.w[7] = 10;
//ENE			free.w[8] = 10+(viiSub::Rand()%24)*10;
//ENE		}else{
//ENE			if(viiSub::Rand()%80 == 0) {
//ENE				joypad = BTN_A;
//ENE			}
//ENE		}
//ENE		break;
//ENE	case 10:	//�߂Â�
//ENE		if(tx > pObj->pos.x +8000) {
//ENE			joypad = JOY_R;
//ENE		}else if(tx < pObj->pos.x-8000) {
//ENE			joypad = JOY_L;
//ENE		}
//ENE		free.w[8] --;
//ENE		if(free.w[8] > 0) free.w[8] --;	else free.w[7] = 0;
//ENE		break;
//ENE
//ENE	case 20:	//����
//ENE		dst = get_valken_dist(pObj->pos.x,pObj->pos.y);
//ENE		if( dst <= 5000) {
//ENE			//�����p�^�[���ɓ���
//ENE			free.w[7] = 30;
//ENE			free.w[8] = 120+(viiSub::Rand()%8)*10;
//ENE			break;
//ENE		}
//ENE		free.w[8] --;
//ENE		if(free.w[8] > 0) free.w[8] --;	else free.w[7] = 0;
//ENE		break;
//ENE	case 30:	//�ɂ���
//ENE		if(tx < pObj->pos.x) {
//ENE			joypad = JOY_R;
//ENE		}else{
//ENE			joypad = JOY_L;
//ENE		}
//ENE		
//ENE		if(free.w[8] > 0) free.w[8] --;
//ENE		else {
//ENE			free.w[7] = 20;
//ENE			free.w[8] = 30;
//ENE		}
//ENE		break;
//ENE	}
//ENE
//ENE
//ENE	//�U��
//ENE	if(joypad&BTN_A && free.w[6] == 0) {
//ENE		free.w[6] = 1;
//ENE	}
//ENE
//ENE	if(free.w[6] == 0) {
//ENE		if(joypad&JOY_L) {
//ENE			free.b[1] ++;
//ENE			free.b[0] = -1;
//ENE			pos.mx = -spd;
//ENE			free.w[5] = 0;
//ENE		}else if(joypad&JOY_R) {
//ENE			free.b[1] ++;
//ENE			free.b[0] = 1;
//ENE			pos.mx = spd;
//ENE			free.w[5] = 0;
//ENE		}else{
//ENE			pos.mx = 0;
//ENE			if(free.b[0] > 0) {
//ENE				if(free.b[1] != 9) {
//ENE					pos.mx = 10;
//ENE					free.b[1] ++;
//ENE				}
//ENE			}else if(free.b[0] < 0) {
//ENE				if(free.b[1] != 9) {
//ENE					pos.mx = -10;
//ENE					free.b[1] ++;
//ENE				}
//ENE			}
//ENE		}
//ENE	}
//ENE
//ENE	//----------------------------------------
//ENE	//��{�X�v���C�g��ݒ�
//ENE	//----------------------------------------
//ENE	Sint32 walk_tbl[] = {6,7,8,9};
//ENE	free.b[1] = ( (4*9)+free.b[1] )%(4*9);
//ENE	Sint32 walk = free.b[1]/9;
//ENE	walk = walk%4;
//ENE	spr = walk_tbl[walk];
//ENE
//ENE	if(pos.mx == 0) {
//ENE		//�Î~���
//ENE		spr = 5;
//ENE		free.w[5] ++;
//ENE		if(free.w[5] >= 30) {
//ENE			if(free.w[5] >= 120) {
//ENE				free.w[5] = 30;
//ENE			}
//ENE			spr = (free.w[5] -30 )/45;
//ENE		}
//ENE	}
//ENE
//ENE	const Sint32 atk_tbl[]={
//ENE		6,5,4,4,3,3,3,3,3,2,3,3,4,4,5,5,5,-1
//ENE	};
//ENE	if(free.w[6] > 0) {
//ENE		//�U��
//ENE		if(tx > pObj->pos.x) {
//ENE			free.b[0] = 1;
//ENE		}else if(tx < pObj->pos.x) {
//ENE			free.b[0] = -1;
//ENE		}
//ENE		dst = get_valken_dist(pObj->pos.x,pObj->pos.y);
//ENE
//ENE		free.w[6] ++;
//ENE		spr = atk_tbl[free.w[6]/8];
//ENE		if(free.w[6] == 10*8 && viiSub::Rand()%4==0) {
//ENE			free.w[6] = 33;
//ENE		}else if(free.w[6] == 16*8 && viiSub::Rand()%4==0) {
//ENE			free.w[6] = 8;
//ENE		}
//ENE		
//ENE		if(free.w[6] == 72) {
//ENE			viiEff::Set(EFF_MAZZLE    ,pObj->pos.x+800*free.b[0],pos.y-1800,NULL);
//ENE			viiEff::Set(EFF_ENE_BULLET,pObj->pos.x+800*free.b[0],pos.y-1600,argset(90-90*free.b[0],200));
//ENE		}
//ENE		
//ENE		if(spr == -1) {
//ENE			spr = 5;
//ENE			free.w[6] = 0;
//ENE		}
//ENE		if( dst <= 3200 && viiSub::Rand()%6==0) {
//ENE			//�����p�^�[���ɓ���
//ENE			free.w[6] = 0;
//ENE			free.w[7] = 30;
//ENE			free.w[8] = 120+(viiSub::Rand()%8)*10;
//ENE		}
//ENE	}
//ENE
//ENE
//ENE	if(viiSub::IsScrollOut(pObj->pos.x,pObj->pos.y,1600,1600)){
//ENE/*		exist_ = gxFalse;
//ENE		delete pObj;
//ENE		pObj = NULL;
//ENE*/
//ENE		return;
//ENE	}
//ENE
//ENE	pObj->mov.x = pos.mx;
//ENE	pObj->mov.y = pos.my+120;
//ENE
//ENE	if( pObj->easy_collision_check() ){
//ENE		if(free.w[4] > 0) {
//ENE			free.w[4] = 0;
//ENE			free.w[5] = 5;
//ENE		}
//ENE		pos.my = 0;
//ENE		pos.mx = 0;
//ENE	}else{
//ENE		pos.my += 10;
//ENE		//spr = 4;
//ENE	}
//ENE
//ENE	pObj->pos.x += pObj->mov.x;
//ENE	pObj->pos.y += pObj->mov.y;
//ENE
//ENE
//ENE	//----------------------------
//ENE	//�����̐���
//ENE	//----------------------------
//ENE	atr = ATR_DFLT;
//ENE	if(free.b[0] == 1) atr |= ATR_FLIP_X;
//ENE	viiDraw::Sprite(&spr_army01[spr],pObj->pos.x,pObj->pos.y,PRIO_ENEMY+5,atr);
//ENE	free.w[3] = spr;
//ENE
//ENE	//----------------------------
//ENE	//�����蔻��
//ENE	//----------------------------
//ENE	pos.x = pObj->pos.x;
//ENE	pos.y = pObj->pos.y;
//ENE	pos.kx1 =  -4;	//�����蔻��
//ENE	pos.ky1 =  -12;
//ENE	pos.kx2 =   4;
//ENE	pos.ky2 =   0;
//ENE	m_HitKurai.Set(ID_ENEMY_DEF,&pos);
//ENE	m_HitKurai.Activate();
//ENE
}

void CEffect::ArmyTank()
{
//	//---------------------------------------
//	//�������
//	//---------------------------------------
//	Sint32 atr = ATR_DFLT;
//	Sint32 spr = free.w[3];
//	Sint32 spd = 60;
//	Sint32 joypad = 0;//Joy[0].psh;
//
//	if (is_init()) {
//		m_HitKurai.SetDirectHitPosition( 1 );
//		m_HitAtari.set_hp(50);
//		rot = 0;
//		free.b[0] = -1;		//����
//		free.b[1] = 0;		//���s�J�E���^
//		free.b[2] = 0;		//�N�b�V�����J�E���^
//		free.w[3] = 0;		//�X�v���C�g�p�^�[��
//		free.w[4] = 0;		//�W�����v�J�E���^
//		free.w[5] = 0;		//�E�F�C�g�J�E���^
//		free.w[6] = 0;		//�U���J�E���^
//		free.w[7] = 0;		//�s���X�e�[�^�X
//		free.w[8] = 0;		//�s���E�F�C�g
//		free.w[9] = 0;		//�C�p�x
//		free.w[10] = 0;		//�C����
//		free.w[11] = 0;		//�C����i�U���Ԑ��j
//		free.w[12] = 0;		//�����^�C�}�[
//		free.w[13] = 0;		//�ӂ��ƂуJ�E���^�x
//		pObj = new CObj();
//		pObj->pos.x = x_;
//		pObj->pos.y = y_;
//		xx_ = 0;
//		yy_ = 0;
//		pos.mx = 0;
//		pos.my = 0;
//		timer_ = 1+viiSub::Rand()%64;
//	}
//
//	if(m_HitAtari.is_dead()){
//		spd = spd /4;
//		if(free.w[12] == 0) {
//			if(free.w[10] == 0){
//				free.w[10] ++;		//���m�I��
//			}
//			free.w[13] = -100;
//			yy_ = 0;
//		}
//		yy_ += free.w[13];
//		free.w[13] += 3;
//
//		if(viiSub::Rand()%8==0) viiEff::SetBombEffect(pObj->pos.x+(-32+viiSub::Rand()%64)*100,pObj->pos.y-(24+viiSub::Rand()%32)*100,1,2);
//		if(free.w[12]%8==0) {
//			rot ++;
//			viiMus::PlaySound(SE_BOMB_SMALL);
//		}
//		if(free.w[12] >= 60) {
//			viiEff::SetBombDeluxe(pObj->pos.x+(-32+viiSub::Rand()%64)*100,pObj->pos.y-(24+viiSub::Rand()%32)*100,1,2);
//			exist_ = gxFalse;
//			return;
//		}
//		free.w[12] ++;
//	}
//
//	//�����^�]
//	Sint32 tx = GetTargetPlayer()->x;
//	Sint32 dst;
//	switch(free.w[7]){
//	case 0:
//		//�������Ă��Ȃ�
//		dst = get_valken_dist(pObj->pos.x,pObj->pos.y);
//		if( dst <= 14000) {
//			free.w[11] += 2;
//			if(free.w[11] >= 24) {
//				free.w[11] = 24;
//			}
//		}else{
//			if( dst <= 12000) {
//				//�����p�^�[���ɓ���
//				free.w[7] = 30;
//				free.w[8] = 120+(viiSub::Rand()%8)*10;
//			}else  if(dst >= 16000) {
//				//�߂Â��p�^�[���ɓ���
//				free.w[7] = 10;
//				free.w[8] = 10+(viiSub::Rand()%24)*10;
//			}
//		}
//
//		break;
//	case 10:	//�߂Â�
//		if(tx > pObj->pos.x +8000) {
//			joypad = JOY_R;
//		}else if(tx < pObj->pos.x-8000) {
//			joypad = JOY_L;
//		}
//		free.w[8] --;
//		if(free.w[8] > 0) free.w[8] --;	else free.w[7] = 0;
//		break;
//
//	case 30:	//�ɂ���
//		if(tx < pObj->pos.x) {
//			joypad = JOY_R;
//		}else{
//			joypad = JOY_L;
//		}
//		
//		if(free.w[8] > 0) free.w[8] --;	else free.w[7] = 0;
//		break;
//	}
//
//	//------------------------------------------
//	//�ړ�
//	//------------------------------------------
//	if(joypad&JOY_L) {
//		xx_ -= 3;
//		if(xx_ <= -spd) xx_ = -spd;
//	}else if(joypad&JOY_R) {
//		xx_ += 3;
//		if(xx_ >= spd) xx_ = spd;
//	}else{
//		if(xx_ > 0) {
//			xx_ --;
//		}else if(xx_ < 0) {
//			xx_ ++;
//		}
//	}
//	pos.mx = xx_;
//	
//
//	//----------------------------------------
//	//��{�X�v���C�g��ݒ�
//	//----------------------------------------
//
//	if(viiSub::IsScrollOut(pObj->pos.x,pObj->pos.y,6400,6400)){
///*		exist_ = gxFalse;
//		delete pObj;
//		pObj = NULL;
//*/
//		return;
//	}
//
//	pObj->mov.x = pos.mx;
//	pObj->mov.y = pos.my+120;
//
//	if( pObj->easy_collision_check() ){
//		if(free.w[4] > 0) {
//			free.w[4] = 0;
//			free.w[5] = 5;
//		}
//		pos.my = 0;
//		pos.mx = 0;
//	}else{
//		pos.my += 10;
//		//spr = 4;
//	}
//
//	pObj->pos.x += pObj->mov.x;
//	pObj->pos.y += pObj->mov.y;
//
//	//----------------------------
//	//��Ԃ̕\��
//	//----------------------------
//	atr = ATR_DFLT;
//	if(free.b[0] == 1) atr |= ATR_FLIP_X;
//
//	extern gxSprite spr_armytank[];
//	viiDraw::Sprdmg(m_HitAtari.is_damage(),&spr_armytank[0],pObj->pos.x,pObj->pos.y,PRIO_ENEMY,atr);
//
//	//�C���
//	atr = ATR_DFLT;
//	if(tx > pObj->pos.x ) {
//		free.w[9]++;
//	}else if(tx < pObj->pos.x) {
//		free.w[9]--;
//	}
//
//	if(free.w[9] >= 40){
//		free.w[9] = 40;
//	}else if(free.w[9] <= 0)  {
//		free.w[9] = 0;
//	}else{
//		free.w[11] = 0;
//	}
//	Sint32 cannon = 0;
//
//	switch(free.w[9] / 4){
//	case 0:
//		cannon = 0;
//		break;
//	case 1:
//		cannon = 1;
//		break;
//	case 2:
//		cannon = 2;
//		break;
//	case 3:
//		cannon = 3;
//		break;
//	case 4:
//		atr = ATR_FLIP_X;
//		cannon = 3;
//		break;
//	case 5:
//		atr = ATR_FLIP_X;
//		cannon = 2;
//		break;
//	case 6:
//		atr = ATR_FLIP_X;
//		cannon = 1;
//		break;
//	default:
//		atr = ATR_FLIP_X;
//		cannon = 0;
//		break;
//	}
//	/*�C��\��*/
//	viiDraw::Sprdmg(m_HitAtari.is_damage(),&spr_armytank[1+cannon],pObj->pos.x,pObj->pos.y+yy_,PRIO_ENEMY,atr,ARGB_DFLT,1.0f,rot);
//
//	//---------------------------------
//	//�C����
//	//---------------------------------
////	if(joypad&BTN_X) {
////		free.w[10] = 1;
////	}
//	if(free.w[10] == 0 && m_HitKurai.IsImpacted()) {
//		free.w[10] = 1;
//		viiMus::PlaySound(SE_TIBIYARARE);
//	}
//
//	if(free.w[10] > 0){
//		//�G������
//		free.w[10] ++;
//		if(free.w[10] > 47){
//			free.w[10] = 47;
//		}else{
//			viiDraw::Sprite(&spr_armytank[5+free.w[10]/8],pObj->pos.x,pObj->pos.y+free.w[10]*10,PRIO_ENEMY+1,ATR_DFLT);
//		}
//	}else{
//		//����̂�������
//		if(free.w[11] > 0){
//			free.w[11] --;
//			if(free.w[11] <= 0) {
//				free.w[11] = 0;
//			}
//			viiDraw::Sprite(&spr_armytank[5],pObj->pos.x,pObj->pos.y+(800-(free.w[11]/3)*100),PRIO_ENEMY-1,ATR_DFLT);
//
//			//�}�V���K���U��
//			if(free.w[11] >= 20) {
//				Sint32 dr = (free.w[9]<20)? -1 : 1;
//				if(timer_%120 <= 60){
//					if(timer_%12 == 0) {
//						viiEff::Set(EFF_MAZZLE    ,pObj->pos.x+1600*dr,pos.y-3200,NULL);
//						viiEff::Set(EFF_ENE_BULLET,pObj->pos.x+1600*dr,pos.y-2800,argset(90-65*dr,200));
//					}
//				}
//			}
//
//			pos.x = pObj->pos.x;
//			pos.y = pObj->pos.y-3200;
//			pos.kx1 =   -4;	//�����蔻��
//			pos.ky1 =   -4;
//			pos.kx2 =   4;
//			pos.ky2 =   4;
//			m_HitKurai.Set(ID_ENEMY_DEF,&pos);
//			m_HitKurai.Activate();
//		}else{
//			if(timer_%120 == 60){
//				Sint32 dr = (free.w[9]<20)? -1 : 1;
//				viiEff::Set(EFF_MAZZLE    ,pObj->pos.x+5000*dr,pos.y-2200,NULL);
//				viiEff::Set(EFF_SHOT_CANNON,pObj->pos.x+5000*dr,pos.y-2200,argset(90-90*dr,400));
//			}
//		}
//	}
//
//	pos.x = pObj->pos.x;
//	pos.y = pObj->pos.y;
//	pos.kx1 =  -32;	//�����蔻��
//	pos.ky1 =  -24;
//	pos.kx2 =   32;
//	pos.ky2 =   0;
//	m_HitKurai.Set(ID_ENEMY_DEF,&pos);
//	m_HitKurai.Activate();
//	free.w[3] = spr;
//
}



#if 0
	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		pos.rot = viiSub::Rand()%360;
		pos.sx = 100;
		free.w[0] = 8+viiSub::Rand()%8;		//��]���x
		free.w[1] = 2+viiSub::Rand()%2;		//�Ԃ���J�E���^�[
		free.w[2] = viiSub::Rand()%(13-2);	//�}�b�v�p�[�c�ԍ��i���Q�͊�Ȃ̂ŏ��O����j
		free.w[3] = 50+viiSub::Rand()%50;	//�傫��
		free.w[4] = (viiSub::Rand()%4);		//����
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

	if(viiSub::IsScrollOut(pos.x,pos.y,8*100,8*100)){
		exist_ = gxFalse;
		return;
	}

	pos.rot += free.w[0];
	float scl = free.w[3]/100.0f;

	viiDraw::Sprite(&spr_broken[ free.w[2] ],pos.x,pos.y,PRIO_BG2+2,ATR_DFLT,ARGB(0xff,0xff,0xff,0xff),scl,pos.rot);
	if(free.w[4] == 0){
		viiDraw::Sprite(&spr_bomb[0],pos.x,pos.y,PRIO_EFFECT,ATR_ALPHA_PLUS,ARGB(0x80,0xff,0xff,0xff),0.2f,pos.rot);
	}
#endif

void CEffect::bomb_parts()
{
//	//�����p�[�c
//	if(is_init()){
//		rot = viiSub::Rand()%360;
//		free.w[0] = 128+viiSub::Rand()%20;		//�F
//		free.w[1] = 100+viiSub::Rand()%20;		//�傫��
//		free.w[2] = viiSub::Rand()%10;
//		if(!pGame->is_gravit()){
//			free.w[2] = 0;	//�F����Ԃł͕s���S�R�ăi�V
//		}
//		xx_ = viiMath::Cos100(opt[0])*opt[1];	//�p�xx�X�s�[�h
//		yy_ = viiMath::Sin100(opt[0])*opt[1];	//�p�xx�X�s�[�h
//	}
//
//	if(opt[2] > 0) {
//		opt[2] --;
//		return;
//	}
//
//	if(free.w[0] > 0) {
//		free.w[0] -= 2;
//		free.w[1] += 1;
//	}
//
//	if(free.w[0] <=0){
//		//���ߓx�[���p�[�Z���g�ŏ���
//		exist_ = gxFalse;
//		return;
//	}
//
//	Sint32 col   = free.w[0]*2;
//	float scl = free.w[1]/100.0f;
//
//	rot += 2;
//	if(col >= 255) col = 255;
//
//	x_ += xx_;
//	y_ += yy_;
//
//	viiSub::SetGravitMove(x_,y_);
//
//	xx_ += -xx_/10;
//	yy_ += -yy_/10;
//
//	Sint32 x,y;
//	x = x_;
//	y = y_;
//
//	if(free.w[2] == 8){
//		viiDraw::Sprite(&spr_cloud[ 0 ],x,y,PRIO_EFFECT,ATR_DEFAULT,ARGB(col,0x80,0x80,0x80),1.0f,rot);
//
//	}else if(free.w[2] == 3){
//		//�s���S�R��
//		viiDraw::Sprite(&spr_cloud[ 0 ],x,y,PRIO_EFFECT,ATR_ALPHA_MINUS,ARGB(col,0xff,0xff,0xff),1.0f,rot);
//
//	}else{
//		viiDraw::Sprite(&spr_bomb[0],x,y,PRIO_EFFECT,ATR_DEFAULT,ARGB(col,0xff,0xff,0xff),1.0f,rot);
//		viiDraw::Sprite(&spr_bomb[0],x,y,PRIO_EFFECT+1,ATR_ALPHA_PLUS,ARGB(col/2,0xff,0xff,0xff),scl,-rot);
//	}
//
}

void CEffect::bomb()
{
//�i�g���ĂȂ��j
//	//�����p�^�[������I�u�W�F
//	Sint32 start_rot = viiSub::Rand()%360;
////	opt[0] = 8;	//������
////	opt[1] = 1;	//�����̑傫��
//
//	if(opt[1] < 0)	opt[1] = 1;	//���S�\�t�g
//
//	Sint32 n = 360/opt[0];
//	if(is_init()){
//		for(Sint32 i=0;i<opt[0];i++){
//			o[0] = start_rot + n*i;
//			o[1] = 1+viiSub::Rand()%opt[1];
//			o[2] = i*4;
//			new CEffBomb( x_ , y_ , o[0] , o[1] );// , o[2] );
////			viiEff::Set(EFF_BOMB_PARTS,x_,y_,o);
//		}
//	}
//	exist_ = gxFalse;
}

//-------------------------------------------------------------
//���P�̓C�[�W�X�֘A
//-------------------------------------------------------------
void CEffect::versis_action()
{
//	//�C�[�W�X
//	Sint32 sx,sy,x1,y1,x2,y2,ay=0;
//
//	if (is_init()) {
//		free.w[0] = 0;	//�_�ŗp�̃J���[
////		free.w[1] = 0;	//�C�[�W�X�̃v���C�I���e�B
//	}
//
//	if(get_sw(0)){
//		if(viiSub::Rand()%2) ay = ((timer_%6)<3)? 200 : 0;
//	}
//	if(get_sw(1)){
//		if(viiSub::Rand()%2) ay = ((timer_%12)<6)? 100 : 0;
//		else		 ay = 0;
//	}
//
//	viiSub::GetScroll_s( sx,sy );
//
//	x1 = x_/100 -10 - sx;
//	y1 = ay+y_/100 -115 - sy;
//	x2 = x_/100 +220 - sx;
//	y2 = ay+y_/100 -65 - sy;
//
//	free.w[0] += 2;
//	free.w[0] = free.w[0]%250;
//
//	Sint32 col;
//	col = abs(free.w[0]-125);
//
//	viiDraw::Box(x1,y1,x2,y2,PRIO_VERSIS_B,gxTrue,ARGB(0xff,col*2,0x00,0x00));
//
//	x1 = x_/100 -220 - sx;
//	y1 = y_/100 -82 - sy;
//	x2 = x_/100 -60 - sx;
//	y2 = y_/100 -33 - sy;
//	viiDraw::Box(x1,y1,x2,y2,PRIO_VERSIS_B,gxTrue,ARGB(0x80,col*2,0x00,0x00));
//
///*
//	Sint32 prio = PRIO_VERSIS_F;
//	if(free.w[1]){
//		prio = PRIO_PLAYER
//	}
//*/
//	viiDraw::Sprite(&spr_versis[0],x_,ay+y_,PRIO_VERSIS_B,ATR_DFLT,ARGB(0xff,0xff,0xff,0xff));
//
}


void CEffect::item_chip()
{
//	//�A�C�e���`�b�v(�����`�b�v��)
//	Sint32 type;
//	if ( is_init() )
//	{
//		free.b[0] = 0;	//�A�C�e���o���O�i�a�n�w��ԁj
//		if(opt[1] == 1)
//		{
//			free.b[0] = 1;	//�A�C�e���o����
//		}
//		if(opt[0] < 0 || opt[0]>=3) opt[0] = 0;	//�A�C�e���^�C�v
//		pos.ax1 = -8;
//		pos.ay1 = -8;
//		pos.ax2 =  8;
//		pos.ay2 =  8;
//
//		pos.kx1 = -8;
//		pos.ky1 = -8;
//		pos.kx2 =  8;
//		pos.ky2 =  8;
//
//		m_HitAtari.SetDirectAromorPosition( 0 );
//		pos.x = x_;
//		pos.y = y_;
//		pos.my = 0;
//
//	}
//	type = opt[0];
//
//	if(viiSub::IsScrollOut(x_,y_,32*100,32*100))	return;		//��ʊO�ł͕`�悵�Ȃ�
//
//	if(free.b[0] == 0)
//	{
//		//---------------------------------------
//		//�{�b�N�X���
//		//---------------------------------------
//		if(m_HitKurai.is_damage())
//		{
//			//�j�󂳂ꂽ
//			free.b[0] = 1;	//�A�C�e����Ԃ�
//			free.b[1] = 0;	//�����I���t���O
//			//m_HitKurai.SetOffImpact();
//			viiEff::SetBombSimple(pos.x,pos.y,1,1);
//			if(pGame->is_gravit())	pos.my = -300;
//		}else{
//			m_HitKurai.Set(ID_ENEMY_DEF,&pos);
//			m_HitKurai.Activate();	//&hit
//		}
//		viiDraw::Sprite(&spr_item[4],pos.x,pos.y,PRIO_EFFECT,ATR_DFLT,ARGB(0xf0,0xff,0xff,0xff));
//
//	}
//	else if(free.b[0] == 1)
//	{
//		//---------------------------------------
//		//�A�C�e�����
//		//---------------------------------------
//		if(get_valken_dist(pos.x,pos.y+1200) <= 18*100)
//		{
//			//��悳�ꂽ
//			switch(type){
//			case 0:	//POW
//			case 1:	//WEAPON
//			case 2:	//HP
//				get_item(type);
//				break;
//			}
//			exist_ = gxFalse;
//			return;
//		}
//
//		if(pGame->is_gravit() && free.b[1] == 0){
//			//����
//			pos.y += pos.my;
//			CObj *p = new CObj();
//			if(p->is_hit(pos.x,pos.y+800) && pos.my>=300){
//				while(p->is_hit(pos.x,pos.y+800)){
//					pos.y -= 100;
//				}
//				free.b[1] = 1;
//			}else{
//				if(pos.my<=300){
//					pos.my += 16;
//				}
//			}
//			delete p;
//		}
//
//		if(!pGame->is_gravit() || (pGame->is_gravit() && pos.my>100)){
//			m_HitAtari.Set(ID_ENEMY_ATK,&pos);
//			m_HitAtari.Activate();
//		}
//
//		viiDraw::Sprite(&spr_item[3],pos.x,pos.y,PRIO_EFFECT,ATR_DFLT,ARGB(0xf0,0xff,0xff,0xff));
//		viiDraw::Sprite(&spr_item[type],pos.x,pos.y,PRIO_EFFECT,ATR_DFLT,ARGB(0xf0,0xff,0xff,0xff));
//
//		Sint32 col = timer_%100;
//		col = abs(col -50);
//		viiDraw::Sprite(&spr_item[type],pos.x,pos.y,PRIO_EFFECT,ATR_ALPHA_PLUS,ARGB((col*3),0xff,0xff,0xff),(col*2.5f)/100.0f);
//	}
//
}


void CEffect::vulcan()
{
//	//---------------------
//	//�o���J���e
//	//---------------------
//
//	if (is_init()) {
//		rot = opt[0];		//��������
//		free.b[0] = opt[1];	//�e�ۃ��x��
//		free.b[1] = opt[2];	//���e��
//
//		m_HitAtari.set_hp(1);
//		xx_ = viiMath::Cos100(rot)*10;
//		yy_ = viiMath::Sin100(rot)*10;
//	}
//
//	Sint32 lv = free.b[0];
//
//	if(m_HitAtari.IsImpacted() || viiSub::GetBoundBlock_Player( x_,y_ )){
//		o[0] = 1;
//		viiEff::Set(EFF_HITFLASH,x_,y_,o);
//		exist_ = gxFalse;
//		return;
//	}
//
////	exist_ = gxFalse;
////	return;
///*
//	//�f�o�b�O�\��
//	Sint32 x1 = x_/100 -3;
//	Sint32 y1 = y_/100 -3;
//	Sint32 x2 = x_/100 +3;
//	Sint32 y2 = y_/100 +3;
//
//	Sint32 sx,sy,col;
//
//	viiSub::GetScroll_s( sx,sy );
//
//	x1 = x1-sx;
//	y1 = y1-sy;
//	x2 = x2-sx;
//	y2 = y2-sy;
//	col = 0xFFFF0000;
//	viiDraw::Box(x1,y1,x2,y2,PRIO_DEBUG,gxTrue,col,ATR_DFLT);
//*/
//
//	if(bullet_reflect(this)){
//		o[0] = 0;
//		viiEff::Set(EFF_HITFLASH,x_,y_,o);
//		free.b[1] --;
//		if(free.b[1]==0){
//			exist_ = gxFalse;
//		}
//	}
//
//	//���݃`�F�b�N
//	if(viiSub::IsScrollOut(x_,y_,0,0)){
//		exist_ = gxFalse;
//	}
//
//	OBJ_POS_T ataripos;
//	ataripos.x = x_;
//	ataripos.y = y_;
//	ataripos.ax1 = -4+(-lv*2);	//���x���ɉ����Ă����蔻���傫������
//	ataripos.ay1 = -4+(-lv*2);
//	ataripos.ax2 =  4+(+lv*2);
//	ataripos.ay2 =  4+(+lv*2);
////	m_HitAtari.type = ATK_TYPE_CANNON;
//	m_HitAtari.SetDirectAromorPosition( 150+lv*20 );//set_ap(lv+1);
//
////#ifdef TEST__HEAP
//	m_HitAtari.Set(ID_VERSIS_ATK,&ataripos);
//	m_HitAtari.Activate();//&hit
////#endif
//
//	float scl = (100+(lv*50))/100.0f;
//	viiDraw::Sprite(&spr_custom[SPR_BULLET],x_,y_,PRIO_ATACK,ATR_DEFAULT,ARGB_DFLT,scl,0);
//
}

gxBool bullet_reflect(CEffect *p)
{
//	//------------------------------
//	//���e����
//	//x_,y_,xx_,yy_��ύX���܂�
//	//------------------------------
//	Sint32 ox,oy;
//	Sint32 x_ = p->x_;
//	Sint32 y_ = p->y_;
//	Sint32 xx_ = p->xx_;
//	Sint32 yy_ = p->yy_;
//	gxBool flag = gxFalse;
//	ox = x_;
//	oy = y_;
//
//	x_ = x_ + xx_;
//	y_ = y_ + yy_;
//
//	//���B�_���ǂ��ǂ����𒲂ׂ�
//	Sint32 type = viiSub::GetBoundBlock_Player( x_,y_ );
//
//	if(type) {
//		//�ǂɂ�������
//		Sint32 px,py,length;
//		Sint32 qx=ox/100,qy=oy/100;
//		flag = gxTrue;
//		length = viiMath::LineLogic(ox/100,oy/100,x_/100,y_/100,-1,px,py);
//
//		for(Sint32 i=0;i<=length;i++){
//			//�ŏ��ɂ��������ꏊ����������
//			viiMath::LineLogic(ox/100,oy/100,x_/100,y_/100,i,px,py);
//			type = viiSub::GetBoundBlock_Player( px*100,py*100 );
//			if(type){
//				break;
//			}
//			qx = px;
//			qy = py;
//		}
//
//		//�o�E���f�B���O���������肷��
//		Sint32 zz;
//		switch(type){
//		case 1:
//			if(viiSub::GetBoundBlock_Player( x_,oy )){
//				//���ł�������
//				xx_ *= -1;
//			}else if(viiSub::GetBoundBlock_Player( ox,y_ )){
//				//�c�ł�������
//				yy_ *= -1;
//			}else{
//				//�΂߂ł�������
//				xx_ *= -1;
//				yy_ *= -1;
//			}
//			x_ = qx*100;
//			y_ = qy*100;
//			break;
//		case 12:
//		case 14:
//			x_ = qx*100;
//			y_ = qy*100;
//			zz  =  xx_;
//			xx_ = -yy_;
//			yy_ = -zz;
//			break;
//		case 13:
//		case 15:
//			x_ = qx*100;
//			y_ = qy*100;
//			zz  = xx_;
//			xx_ = yy_;
//			yy_ = zz;
//			break;
//		}
//	}
//
//	p->x_ = x_;
//	p->y_ = y_;
//	p->xx_ = xx_;
//	p->yy_ = yy_;
//
//	return flag;
//
	return gxTrue;
}


#define LASER_ATARI_MAX (64)

void CEffect::laser()
{
//ACT_END	//-------------------------------------
//ACT_END	//���[�U�[
//ACT_END	//-------------------------------------
//ACT_END	Sint32 i;
//ACT_END	if (is_init()) {
//ACT_END		rot = opt[0];	//�p�x
//ACT_END		free.b[0] = opt[1];	//���x��
//ACT_END		free.w[1] = opt[2]/100;	//����
//ACT_END
//ACT_END		//�����̂����蔻��p
//ACT_END//		free.l[2] = (Sint32)(pGame->vmem->vmem_alloc(sizeof(tHitTest)*LASER_ATARI_MAX));
//ACT_END		/*@HIT@*///free.l[2] = (Sint32)(pGame->vmem->vmem_alloc(sizeof(tHitTest)*LASER_ATARI_MAX));
//ACT_END	}
//ACT_END
//ACT_END/*
//ACT_END@HIT@
//ACT_END@HIT@	//���������蔻���������
//ACT_END@HIT@	tHitTest *phits;
//ACT_END@HIT@	phits = (tHitTest*)free.l[2];
//ACT_END@HIT@	if (is_init()) {
//ACT_END@HIT@		for(i=0;i<LASER_ATARI_MAX;i++){
//ACT_END@HIT@			phits[i].init();
//ACT_END@HIT@		}
//ACT_END@HIT@	}
//ACT_END@HIT@
//ACT_END@HIT@	if (timer_ > 0) {
//ACT_END@HIT@		//�Q�T�ڂ݂̂��������`�F�b�N���s��
//ACT_END@HIT@		for(i=0;i<LASER_ATARI_MAX;i++){
//ACT_END@HIT@			o[0] = 1;
//ACT_END@HIT@			o[1] = 0;
//ACT_END@HIT@			if(phits[i].IsImpacted()){
//ACT_END@HIT@				Sint32 xx,yy;
//ACT_END@HIT@				xx = (phits[i].x1+phits[i].x2)/2;
//ACT_END@HIT@				yy = (phits[i].y1+phits[i].y2)/2;
//ACT_END@HIT@				viiEff::Set(EFF_LASERBOMB_PARTS,xx*100,yy*100,o);
//ACT_END@HIT@			}
//ACT_END@HIT@		}
//ACT_END@HIT@		exist_ = gxFalse;
//ACT_END@HIT@		pGame->vmem->vmem_free((void*)phits);
//ACT_END@HIT@		return;
//ACT_END@HIT@	}
//ACT_END*/
//ACT_END
//ACT_END
//ACT_END
//ACT_END	Sint32 lv = free.b[0];			//���x��
//ACT_END	Sint32 dist = free.w[1]/32;	//����
//ACT_END
//ACT_END	//���[�U�[�̂t�u
//ACT_END	gxSprite spr1 = {TPG_VALKEN,0,184,16,8,0,4};
//ACT_END	gxSprite spr2 = {TPG_VALKEN,0,184,16,8,0,4};
//ACT_END	gxSprite spr3 = {TPG_VALKEN,16*2+8*4,160,8,8,4,4};	//���[�U�[��[
//ACT_END
//ACT_END	Sint32 x,y;
//ACT_END
//ACT_END	spr1.u = lv*32;
//ACT_END	spr2.u = lv*32;
//ACT_END
//ACT_END	spr2.u += (15-(viiSub::GameCounter()%16));
//ACT_END
//ACT_END	Sint32 col = ARGB(0x80,0xff,0xff,0xff);
//ACT_END	if(lv == 0) {
//ACT_END		//�F����
//ACT_END		Sint32 r = (viiSub::Rand()%2)*240;
//ACT_END		Sint32 g = (viiSub::Rand()%2)*240;
//ACT_END		Sint32 b = (viiSub::Rand()%2)*240;
//ACT_END		if(r+g+b==0) {
//ACT_END			r = g = b = 128;
//ACT_END		}
//ACT_END		col = ARGB(0x80,(r),(g),(b));
//ACT_END	}
//ACT_END	for(i=0;i<dist;i++) {
//ACT_END
//ACT_END		x = x_ + viiMath::Cos100(rot)*16*i;
//ACT_END		y = y_ + viiMath::Sin100(rot)*16*i;
//ACT_END
//ACT_END	if (is_init()) {
//ACT_END			//�����蔻���ݒ�
//ACT_END			if(!viiSub::IsScrollOut(x,y,16,16)){	//��ʔ͈͊O�̃��c�͑���ɂ��Ȃ�
//ACT_END				OBJ_POS_T ataripos;
//ACT_END				ataripos.x = x;
//ACT_END				ataripos.y = y;
//ACT_END				ataripos.ax1 = -1+(-lv*2);	//���x���ɉ����Ă����蔻���傫������
//ACT_END				ataripos.ay1 = -1+(-lv*2);
//ACT_END				ataripos.ax2 =  1+(+lv*2);
//ACT_END				ataripos.ay2 =  1+(+lv*2);
//ACT_END//@HIT@				m_HitAtari.Set(ID_VERSIS_ATK,&phits[i],&ataripos);
//ACT_END//@HIT@				phits[i].SetDirectAromorPosition( (5+lv)*3 );
//ACT_END//@HIT@				m_HitXXXXX.Activate(&phits[i]);
//ACT_END			}
//ACT_END		}
//ACT_END
//ACT_END		if(viiSub::GetBoundBlock_Player( x,y )){
//ACT_END			//�ǂɂ������Ă���΂���ȏ�s���Ȃ�
//ACT_END			o[0] = 1;
//ACT_END			o[1] = 0;
//ACT_END			viiEff::Set(EFF_LASERBOMB_PARTS,x,y,o);
//ACT_END			break;
//ACT_END		}
//ACT_END
//ACT_END		viiDraw::Sprite(&spr2,x,y,PRIO_ATACK,ATR_DFLT,0x40ffffff,1.0f,rot);
//ACT_END		viiDraw::Sprite(&spr2,x,y,PRIO_ATACK,ATR_ALPHA_PLUS,col,1.0f,rot);
//ACT_END
//ACT_END		if(i == dist -1 && timer_ == 0) {
//ACT_END			x = x_ + viiMath::Cos100(rot)*16*(i+1);
//ACT_END			y = y_ + viiMath::Sin100(rot)*16*(i+1);
//ACT_END			viiDraw::Sprite(&spr3,x,y,PRIO_ATACK,ATR_ALPHA_PLUS,ARGB(0x80,0xff,0xff,0xff),1.0f);
//ACT_END			viiDraw::Sprite(&spr3,x,y,PRIO_ATACK,ATR_ALPHA_PLUS,ARGB(0x80,0xff,0xff,0xff),1.5f);
//ACT_END		}
//ACT_END	}
//ACT_END
}

void CEffect::laser_parts()
{
/*
	//���[�U�[��[
	if(timer_==0){
		rot = viiSub::Rand()%360;
		free.w[0] = 255-viiSub::Rand()%40;		//�F
		free.w[1] = 100+viiSub::Rand()%20;		//�傫��
		xx_ = viiMath::Cos100(opt[0])*opt[1];	//�p�xx�X�s�[�h
		yy_ = viiMath::Sin100(opt[0])*opt[1];	//�p�xx�X�s�[�h
	}

	if(free.w[0] > 0) {
		free.w[0] -= 8;
		free.w[1] += 2;
	}

	Sint32 col   = free.w[0];
	float scl = free.w[1]/100.0f;

	if(col >= 255) col = 255;
	if(col <= 0)   col = 0;

	if(col <=0){
		//���ߓx�[���p�[�Z���g�ŏ���
		exist_ = gxFalse;
		return;
	}


//	x_ += xx_;
//	y_ += yy_;

//	xx_ += -xx_/10;
//	yy_ += -yy_/10;


	Sint32 x,y;
	x = x_;
	y = y_;

	gxSprite spr = {TPG_VALKEN,16*2+8*4,160,8,8,4,4};	//���[�U�[��[
	viiDraw::Sprite(&spr,x,y,PRIO_EFFECT+1,ATR_ALPHA_PLUS,ARGB(col/2,0xff,0xff,0xff),scl);
*/

}

void CEffect::ene_bullet()
{
//	//---------------------
//	//�G�o���J���e
//	//---------------------
//
//	if (is_init()) {
//		rot = opt[0];		//��������
//		xx_ = (viiMath::Cos100(rot)*opt[1])/100;
//		yy_ = (viiMath::Sin100(rot)*opt[1])/100;
//
//		rot = viiSub::Rand()%360;
////ZANTEI		m_HitAtari.set_ap(1);
////ZANTEI		m_HitAtari.set_hp(1);
//	}
//
//	Sint32 lv = 0;
//
//	//���݃`�F�b�N
////ZANTEI	if(m_HitAtari.IsImpacted() || viiSub::GetBoundBlock_Enemy(x_,y_) )
//	{
//		exist_ = gxFalse;
//		o[0] = 0;
//		viiEff::Set(EFF_HITFLASH_ENE,x_,y_,o);
//		return;
//	}
//
//	if(viiSub::IsScrollOut(x_,y_,0,0)){
//		exist_ = gxFalse;
//		return;
//	}
//
//	x_ += xx_;
//	y_ += yy_;
//
//	OBJ_POS_T ataripos;
//	ataripos.x = x_;
//	ataripos.y = y_;
//	ataripos.ax1 = -4+(-lv*2);	//���x���ɉ����Ă����蔻���傫������
//	ataripos.ay1 = -4+(-lv*2);
//	ataripos.ax2 =  4+(+lv*2);
//	ataripos.ay2 =  4+(+lv*2);
//
////ZANTEI	m_HitAtari.Set(ID_ENEMY_ATK,&ataripos);
////ZANTEI	m_HitAtari.Activate();
//
//	rot += 8;
//	viiDraw::Sprite(&spr_ene_bullet[0],x_,y_,PRIO_ENE_BULLET,ATR_DEFAULT,ARGB_DFLT,1.0f,rot);
//
}

void CEffect::ene_bomb()
{
//	//---------------------
//	//�G���e
//	//---------------------
//
//	if (is_init()) {
//		rot = opt[0];		//��������
//		xx_ = opt[0];
//		yy_ = -200;
//
//		rot = viiSub::Rand()%360;
////ZANTEI		m_HitAtari.set_ap(1);
////ZANTEI		m_HitAtari.set_hp(1);
//	}
//
//	//���݃`�F�b�N
////ZANTEI	if(m_HitAtari.IsImpacted())
//	{
//		viiMus::PlaySound(SE_BOMB_SMALL);
//		viiEff::SetBombEffect(x_,y_,3,3);
//		viiEff::Set(EFF_BOMBFLASH_ENE,x_,y_);
//		exist_ = gxFalse;
//		return;
//	}
//	if(viiSub::GetBoundBlock_Enemy(x_,y_)){
//		viiMus::PlaySound(SE_BOMB_SMALL);
//		viiEff::Set(EFF_BOMBFLASH_ENE,x_,y_);
//		exist_ = gxFalse;
//		return;
//	}
//	if(viiSub::IsScrollOut(x_,y_,0,0)){
//		exist_ = gxFalse;
//		return;
//	}
//
//	//����
//	yy_+=8;
//	if(yy_>=700){
//		yy_ = 700;
//	}
//	//X�͌���
//	xx_ += -xx_/100;
//
//	x_ += xx_;
//	y_ += yy_;
//
//	Sint32 lv = 0;
//	OBJ_POS_T ataripos;
//	ataripos.x = x_;
//	ataripos.y = y_;
//	ataripos.ax1 = -4+(-lv*2);	//���x���ɉ����Ă����蔻���傫������
//	ataripos.ay1 = -4+(-lv*2);
//	ataripos.ax2 =  4+(+lv*2);
//	ataripos.ay2 =  4+(+lv*2);
//
////ZANTEI	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
////ZANTEI	m_HitAtari.Set(ID_ENEMY_ATK,&ataripos);
////ZANTEI	m_HitAtari.Activate();
//
//	rot += 8;
//	viiDraw::Sprite(&spr_ene_bullet[1],x_,y_,PRIO_ENE_BULLET,ATR_DEFAULT,ARGB_DFLT,1.0f,rot);
//
}


void CEffect::ene_shot_cannon()
{
//	//�G�L���m���C
//
//	if (is_init()) {
//		rot = opt[0];		//��������
//		if(opt[2]==0){
//			xx_ = (viiMath::Cos100(rot)*opt[1])/100;
//			yy_ = (viiMath::Sin100(rot)*opt[1])/100;
//			free.w[0] = 200-viiSub::Rand()%100;
//			free.w[1] = 200-viiSub::Rand()%100;
//		}else{
//			//����
//			free.w[0] = 200-viiSub::Rand()%50;
//			free.w[1] = 200-viiSub::Rand()%100;
//		}
//		//rot = viiSub::Rand()%360;
////ZANTEI		m_HitAtari.set_ap(1);
////ZANTEI		m_HitAtari.set_hp(1);
//	}
//
//	if(opt[2]){
//		float scl = free.w[0]/100.0f;
//		Sint32 col = ARGB((free.w[1]),0xff,0xa0,0xa0);
//
//		free.w[1] -= 8;
//		if(free.w[0] <=0 || free.w[1] <= 0) {
//			exist_ = gxFalse;
//		}else{
//			viiDraw::Sprite(&spr_cloud[3],x_,y_,PRIO_ENE_BULLET,ATR_ALPHA_PLUS,col,scl,rot);
//		}
//		return;
//	}
//
////	Sint32 lv = 0;
//
//	//���݃`�F�b�N
////ZANTEI	if(m_HitAtari.IsImpacted() || viiSub::GetBoundBlock_Enemy(x_,y_) )
//	{
//		exist_ = gxFalse;
//		o[0] = 0;
//		viiEff::SetBombEffect(x_,y_,3,4);
//		return;
//	}
//
//	if(viiSub::IsScrollOut(x_,y_,6400,6400)){
//		exist_ = gxFalse;
//		return;
//	}
//
//	x_ += xx_;
//	y_ += yy_;
//
//	OBJ_POS_T ataripos;
//	ataripos.x = x_;
//	ataripos.y = y_;
//	ataripos.ax1 = -4+(-1*2);	//���x���ɉ����Ă����蔻���傫������
//	ataripos.ay1 = -4+(-1*2);
//	ataripos.ax2 =  4+(+1*2);
//	ataripos.ay2 =  4+(+1*2);
//
////ZANTEI	m_HitAtari.Set(ID_ENEMY_ATK,&ataripos);
////ZANTEI	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
////ZANTEI	m_HitAtari.Activate();
//
////	rot += 64;
//
//	viiDraw::Sprite(&spr_cloud[2],x_,y_,PRIO_ENE_BULLET+1,ATR_ALPHA_PLUS,0xf0ffffff,1.5f,rot);
//
//	if(timer_%2==0) viiEff::Set(EFF_SHOT_CANNON,x_,y_,argset(rot,0,1));
//
}


void CEffect::ene_missile()
{
//	//-------------------------------------
//	//�G�U���~�T�C��
//	//-------------------------------------
//	if (is_init()) {
//		pos.x = x_;
//		pos.y = y_;
//		pos.mx = -100;
//		pos.my = 30;
//		pos.ax1 = -4;
//		pos.ay1 = -4;
//		pos.ax2 =  4;
//		pos.ay2 =  4;
////ZANTEI		m_HitAtari.SetDirectHitPosition( 1 );
////ZANTEI		m_HitAtari.set_ap(2);
//	}
//
//	if(GetTargetPlayer()->y-3200 > pos.y){
//		pos.my +=2;
//	}else if(GetTargetPlayer()->y-1200 < pos.y){
//		pos.my -=2;
//	}
//	pos.mx += 10;
//	pos.y += pos.my;
//	pos.x -= pos.mx;
//
//	if(pos.mx > 100){
//		viiDraw::Sprite(&ene_homing_missile[1],pos.x+1600,pos.y+viiSub::Rand()%200,PRIO_ENEMY,ATR_ALPHA_PLUS,0x80ffffff);
//		viiDraw::Sprite(&ene_homing_missile[1],pos.x+1400+viiSub::Rand()%200,pos.y,PRIO_ENEMY,ATR_ALPHA_PLUS,0x80ffffff);
//		viiEff::Set(EFF_MISSILE_SMOKE,pos.x+1600,pos.y,opt);
//	}
//
//	viiDraw::Sprite(&ene_homing_missile[0],pos.x,pos.y,PRIO_ENEMY,ATR_DFLT,ARGB_DFLT);
//
//	//�X�N���[���A�E�g
//	Sint32 sx,sy;
//	viiSub::GetScroll_s( sx,sy );
//	if(pos.x < (sx-64)*100){
//		exist_ = gxFalse;
//		return;
//	}
//
////ZANTEI	if(m_HitAtari.IsImpacted())
//	{
//		viiEff::SetBombDeluxe(pos.x,pos.y,8,5);
//		exist_ = gxFalse;
//		return;
//	}
//
////ZANTEI	m_HitAtari.Set(ID_ENEMY_ATK,&pos);
////ZANTEI	m_HitAtari.Activate();
}

//void CEffect::ene_aircannon()
//{
//	//���˖C�G�t�F�N�g
//
//	if (is_init()) {
//		pos.x = x_;
//		pos.y = y_;
//		pos.mx = -1200;
//		pos.my = -1200;
//		pos.ax1 = -4;	//���x���ɉ����Ă����蔻���傫������
//		pos.ay1 = -4;
//		pos.ax2 =  4;
//		pos.ay2 =  4;
//		free.w[0] = 128-viiSub::Rand()%30;
//	}
//
//	switch(opt[0]){
//	case 0:	//��ԉ���
//		pos.x += pos.mx/3;
//		pos.y += pos.my/3;
//		viiDraw::Sprite(&spr_sceneobj[ 0 ],pos.x,pos.y,PRIO_BG2,ATR_DFLT,0x80ffffff,0.5f);
//		break;
//	case 1:	//�^��
//		pos.x += pos.mx/2;
//		pos.y += pos.my/2;
//		viiDraw::Sprite(&spr_sceneobj[ 0 ],pos.x,pos.y,PRIO_BG2,ATR_DFLT,0x80ffffff,1.0f);
//		break;
//	case 2:	//��ԋ߂�
//		pos.x += pos.mx/1;
//		pos.y += pos.my/1;
//		viiDraw::Sprite(&spr_sceneobj[ 0 ],pos.x,pos.y,PRIO_EFFECT,ATR_DFLT,0x80ffffff,1.5f);
//		viiEff::Set(EFF_AIRCANNON,pos.x,pos.y,argset(10));
//		break;
//	case 3:	//���U���p
//		pos.x += pos.mx/1;
//		pos.y += pos.my/1;
//		viiDraw::Sprite(&spr_sceneobj[ 0 ],pos.x,pos.y,PRIO_BG3,ATR_DFLT,0x80ffffff,1.5f);
//		viiEff::Set(EFF_AIRCANNON,pos.x,pos.y,argset(10));
////ZANTEI		if(m_HitAtari.IsImpacted())
//		{
//			viiEff::SetBombEffect(pos.x,pos.y,8,5);
//			exist_ = gxFalse;
//			return;
//		}
////ZANTEI		m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
////ZANTEI		m_HitAtari.Set(ID_ENEMY_ATK,&pos);
////ZANTEI		m_HitAtari.Activate();
//		break;
//	case 10:	//��
//		free.w[0] -= 8;
//		if(free.w[0]<=0) free.w[0] = 0;
//		viiDraw::Sprite(&spr_sceneobj[ 0 ],pos.x,pos.y,PRIO_EFFECT,ATR_DFLT,ARGB(free.w[0],0xff,0xff,0xff),1.5f);
//		break;
//	}
//
//	Sint32 sx,sy;
//	viiSub::GetScroll_s( sx,sy );
//	if( (pos.y < (sy-32)*100) || (pos.x < (sx-32)*100) || (free.w[0] == 0) ) {
//		exist_ = gxFalse;
//	}
//}

#define PUNCH_MOVE_WITH_SCROLL (1)

void CEffect::punch()
{
//	//-------------------------------------------
//	//�p���`�i�G�l���M�[���j (�p�x,���x,�傫��(%))
//	//-------------------------------------------
//
//	if (is_init()) {
//		pos.x = x_;
//		pos.y = y_;
//		viiSub::GetScroll_s( x_,y_ );
//		x_ *= 100;
//		y_ *= 100;
//		x_ = GetTargetPlayer()->x;
//		y_ = GetTargetPlayer()->y;
//#if PUNCH_MOVE_WITH_SCROLL
//		pos.x -= x_;
//		pos.y -= y_;
//#endif
//		pos.mx = (viiMath::Cos100(opt[0])*opt[1])/100;
//		pos.my = (viiMath::Sin100(opt[0])*opt[1])/100;
//		pos.rot = viiSub::Rand()%360;
//		pos.sx = opt[2];			//�傫��
//		free.w[0] = 255;	//�F����
//		free.w[1] = 0;		//��x�������������t���O
//	}
//
//	//----------------------------------------
//	//�ړ�
//	//----------------------------------------
//	pos.x +=pos.mx;
//	pos.y +=pos.my;
//
//	//----------------------------------------
//	//�F
//	//----------------------------------------
//	Sint32 r = (viiSub::Rand()%2)*240;
//	Sint32 g = (viiSub::Rand()%2)*240;
//	Sint32 b = (viiSub::Rand()%2)*240;
//	if(r+g+b==0) {
//		r = g = b = 128;
//	}
//
//	Sint32 col;
//
//	if(free.w[0] > 0){
//		pos.mx = 95*pos.mx/100;
//		free.w[0] -= 16;
////		pos.sx += 1;
//		if(free.w[0] <= 0){
//			free.w[0] = 0;
//			exist_ = gxFalse;
//			return;
//		}
//	}
//	col = free.w[0];
//
//	float scl = pos.sx / 150.0f;
//
//	//----------------------------------------
//	//GV�ɂ��킹�ē���
//	//----------------------------------------
//	Sint32 x,y;
//#if PUNCH_MOVE_WITH_SCROLL
//	viiSub::GetScroll_s( x_,y_ );
//	x  = x_*100 + pos.x;
//	y  = y_*100 + pos.y;
//	x = GetTargetPlayer()->x+pos.x;
//	y = GetTargetPlayer()->y+pos.y;
//#else
//	x  = pos.x;
//	y  = pos.y;
//#endif
//	viiDraw::Sprite(&spr_custom[4],x,y,PRIO_ATACK,ATR_DFLT,ARGB(0xff,r,g,b),scl,0);
//	viiDraw::Sprite(&spr_custom[4],x,y,PRIO_ATACK,ATR_ALPHA_PLUS,ARGB(col*2,r,g,b),scl,timer_*4);
//	viiDraw::Sprite(&spr_custom[4],x,y,PRIO_ATACK,ATR_ALPHA_PLUS,ARGB(col,r,g,b),scl,-timer_*8);
//
////ZANTEI	if(m_HitAtari.IsImpacted() && free.w[1] == 0)
//	{
//		//----------------------------------------
//		//�������Ă���Έ�x�����j�Ђ��o��
//		//----------------------------------------
//		Sint32 rot = 0;
//		if(pos.mx > 0) rot += 180;
//		viiEff::Set(EFF_BROKENPARTS,x, y,argset(-45+90*0+rot,300,1));
//		viiEff::Set(EFF_BROKENPARTS,x, y,argset( 45+90*0+rot,300,1));
//		free.w[1] = 1;
//		viiMus::PlaySound(SE_PUNCHHIT);
//		viiSub::SetRumble(20,100);
//	}
//
//	//----------------------------------------
//	//�����蔻��o�^
//	//----------------------------------------
//	OBJ_POS_T ataripos;
//	ataripos.x = x;
//	ataripos.y = y;
//	ataripos.ax1 = -10*pos.sx/100;	//���x���ɉ����Ă����蔻���傫������
//	ataripos.ax2 =  10*pos.sx/100;
//	ataripos.ay1 =  -8-10*pos.sx/100;
//	ataripos.ay2 =  20;
////ZANTEI	m_HitAtari.SetDirectAromorPosition( 1+pos.sx );
////ZANTEI	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
////ZANTEI	m_HitAtari.Set(ID_VERSIS_ATK,&ataripos);
////ZANTEI	m_HitAtari.Activate();	//&hit
//
//	//----------------------------------------
//	//�ǂւ̓����蔻��
//	//----------------------------------------
//	switch(pGame->pBg->get_kabetype( x/100,y/100)){
//	case 1:
//	case 12:
//	case 13:
//	case 14:
//	case 15:
//		o[0] = 0;	//�p�x�[��
//		o[1] = 0;	//�X�s�[�h�[��
////		viiEff::Set(EFF_BOMB_PARTS,x,y,o);
//		exist_ = gxFalse;
//		break;
//	}
}


void CEffect::missile()
{
//	//-------------------------------------
//	//�~�T�C��
//	//-------------------------------------
//
//	if (is_init()) {
//		rot = opt[0];					//��������
//		free.b[0] = opt[1];				//�z�[�~���O���x��
//		free.b[1] = 0;
//		free.b[2] = 30+viiSub::Rand()%20;	//�z�[�~���O�J�n����
//		free.b[3] = -3+viiSub::Rand()%7;		//�O���C���i���炵�j���x��
//		free.w[2] = 300;				//�X�s�[�h
//		free.w[3] = 60*3;				//Lv3���؎���
//		free.w[4] = 0;					//�^�[�Q�b�g�ʒu
//		free.w[5] = 0;					//�^�[�Q�b�g�ʒu
////ZANTEI		m_HitAtari.set_hp(1);
//
//		xx_ = (viiMath::Cos100(rot)*free.w[2])/100;
//		yy_ = (viiMath::Sin100(rot)*free.w[2])/100;
//
//		pos.ax1 = -6;
//		pos.ay1 = -6;
//		pos.ax2 =  6;
//		pos.ay2 =  6;
//	}
//
//	Sint32 lv = free.b[0];
//
//	//�z�[�~���O
//	gxBool homing_ok = gxFalse;
//	if(free.b[2]>0){
//		free.b[2]--;
//	}
//	Sint32 out_range = 0;	//��ʊO���e�͈�
//
//	if(lv == 0)
//	{
//		//�z�[�~���O���\�i�V
////ZANTEI		m_HitAtari.set_ap(5);
//		missileLv1();
//		out_range = 0;
//
//	}else if(lv == 1){
//		//�k�u�Q�ȈՃz�[�~���O�i�ǔ���Ȃ��j
////ZANTEI		m_HitAtari.set_ap(8);
//		out_range = 32;
//		missileLv2();
//
//	}else{
//		//�k���R�z�[�~���O�i�ǔ���Ȃ��j
////ZANTEI		m_HitAtari.set_ap(10);
//		out_range = 64;
//		missileLv3();
//	}
//
//	x_ += xx_;
//	y_ += yy_;
//
////ZANTEI	if(m_HitAtari.IsImpacted())
//	{
//		viiEff::SetBombEffect(pos.x,pos.y,3,5);
//		exist_ = gxFalse;
//		return;
//	}
//
//	//���݃`�F�b�N
//	if(viiSub::IsScrollOut(x_,y_,out_range*100,out_range*100)){
//		exist_ = gxFalse;
//	}
//
//	pos.x = x_;
//	pos.y = y_;
//
////ZANTEI	m_HitAtari.Set(ID_VERSIS_ATK,&pos);
////ZANTEI	m_HitAtari.Activate();//&hit
//
//	float scl = 1.0f;
//	viiDraw::Sprite(&spr_custom[SPR_MISSILE]       ,x_,y_,PRIO_ATACK,ATR_DEFAULT,ARGB_DFLT,scl,rot);
//	viiDraw::Sprite(&spr_custom[SPR_MISSILE_BURNIA],x_,y_,PRIO_ATACK,ATR_DEFAULT,ARGB_DFLT,scl,rot);
//
}


void CEffect::missileLv1()
{
//	//-------------------------------------------
//	//�~�T�C�����W�b�NLV1�i�z�[�~���O���\�i�V�j
//	//-------------------------------------------
//	Sint32 spd = free.w[2];
//
//	if(free.b[2] >= 15) free.b[2] = 10+viiSub::Rand()%5;
//	if(free.b[2] == 1)  spd = 800;
//	if(free.b[2]==0) {
//		Sint32 px,py;
//		if(get_nearly_enemypos(x_/100,y_/100,px,py)){
//			px *= 100;
//			py *= 100;
//
//			if(px < x_) xx_ -= 4;
//			if(px > x_) xx_ += 4;
//			if(py < y_) yy_ -= 4;
//			if(py > y_) yy_ += 4;
//		}
//		rot = (Sint32)(RAD2DEG(gxAtan(yy_,xx_)));
//	}else{
//		if(timer_%32 == 0) rot += free.b[3];
//
//		if(spd<=800){
//			spd += 8;
//		}
//
//		xx_ = (viiMath::Cos100(rot)*spd)/100;
//		yy_ = (viiMath::Sin100(rot)*spd)/100;
//	}
//
//	if(viiSub::GetBoundBlock_Player( x_,y_ )){
//		//�ǂɂ�������
//		exist_ = gxFalse;
////		viiEff::Set(EFF_BOMB_PARTS,x_,y_,argset(1,0));
//		new CEffBomb( x_ , y_ , 1 , 0 );
//		return;
//	}
//
//	if(free.b[2] == 0) {
//		viiEff::Set(EFF_MISSILE_SMOKE,x_,y_,NULL);
//		viiEff::Set(EFF_MISSILE_SMOKE,x_+xx_/2,y_+yy_/2,NULL);
//	}
//
//	free.w[2] = spd;
//
}

void CEffect::missileLv2()
{
//	//-------------------------------------
//	//�~�T�C�����W�b�NLV2
//	//-------------------------------------
//	Sint32 px,py;
//
//	if(free.b[2] > 0) {
//		missileLv1();
//		return;
//	}
//
//	if(get_nearly_enemypos(x_/100,y_/100,px,py)){
//		px *= 100;
//		py *= 100;
//
//		if(px < x_) xx_ -= 32;
//		if(px > x_) xx_ += 32;
//		if(py < y_) yy_ -= 32;
//		if(py > y_) yy_ += 32;
//	}
//
//	rot = (Sint32)(RAD2DEG(gxAtan(yy_,xx_)));
//
//	viiEff::Set(EFF_MISSILE_SMOKE,x_,y_,NULL);
//	viiEff::Set(EFF_MISSILE_SMOKE,x_+xx_/2,y_+yy_/2,NULL);
//
//	if(viiSub::GetBoundBlock_Player( x_,y_ )){
//		//�ǂɂ�������
////		viiEff::Set(EFF_BOMB_PARTS,x_,y_,argset(1,0));
//		new CEffBomb( x_ , y_ , 1 , 0 );
//		exist_ = gxFalse;
//		return;
//	}
//
}

void CEffect::missileLv3()
{
//	//-------------------------------------
//	//�~�T�C�����W�b�NLV3
//	//-------------------------------------
//	Sint32 px,py;
//
//	if(free.b[2] > 0) {
//		missileLv1();
//		return;
//	}
//
//	if(get_nearly_enemypos(x_/100,y_/100,px,py)){
//		px *= 100;
//		py *= 100;
//
//		if(px < x_) xx_ -= 32;
//		if(px > x_) xx_ += 32;
//		if(py < y_) yy_ -= 32;
//		if(py > y_) yy_ += 32;
//	}
//
//	rot = (Sint32)(RAD2DEG(gxAtan(yy_,xx_)));
//
//	viiEff::Set(EFF_MISSILE_SMOKE,x_,y_,NULL);
//	viiEff::Set(EFF_MISSILE_SMOKE,x_+xx_/2,y_+yy_/2,NULL);
//
//	if(viiSub::GetBoundBlock_Player( x_,y_ )){
//		//�ǂɂ�������
////		viiEff::Set(EFF_BOMB_PARTS,x_,y_,argset(1,0));
//		new CEffBomb( x_ , y_ , 1 , 0 );
//		exist_ = gxFalse;
//		return;
//	}
//
//#if 0
//	if(!homing_ok){
//			//���΂炭�Ԃ��
//			if(viiSub::Rand()%2 == 0) rot += free.b[3];
//			xx_ = (viiMath::Cos100(rot)*free.w[2])/100;
//			yy_ = (viiMath::Sin100(rot)*free.w[2])/100;
//		}
//		if(get_nearly_enemypos(x_/100,y_/100,px,py) && homing_ok){
//			//�^�[�Q�b�g����
//			px *= 100;
//			py *= 100;
//			double kyori = sqrt(POW((px-x_)/100)+POW((py-y_)/100));
//			Sint32 add = 20;
//			if(kyori <= 160*100){
//				add = 80;
//			}
//			if(px<x_){
//				xx_ -= add;
//			}else{
//				xx_ += add;
//			}
//			if(py<y_){
//				yy_ -= add;
//			}else{
//				yy_ += add;
//			}
//			if(xx_<=-400) xx_ =-400;
//			if(yy_<=-400) yy_ =-400;
//			if(xx_>= 400) xx_ = 400;
//			if(yy_>= 400) yy_ = 400;
//		}
//		rot = (Sint32)RAD2DEG(atan2(yy_,xx_));
//#endif
//
}


void CEffect::twin_cannon()
{
//	//---------------------
//	//�G�c�C���C
//	//---------------------
//
//	if (is_init()) {
//		free.w[0]=200;
//		rot = opt[0];
//
//		xx_ = (viiMath::Cos100(rot)*(200+opt[2]))/100;
//		yy_ = (viiMath::Sin100(rot)*(200+opt[2]))/100;
//		if(opt[1]){
//			xx_ = yy_ = 0;
//			free.w[0]=128;
//		}
////ZANTEI		m_HitAtari.set_ap(1);
////ZANTEI		m_HitAtari.set_hp(1);
//	}
//
//	//���݃`�F�b�N
////ZANTEI	if(m_HitAtari.IsImpacted() || viiSub::GetBoundBlock_Enemy( x_,y_ ))
//	{
//		exist_ = gxFalse;
//		viiEff::SetBombEffect(x_,y_,3,5);
//		viiMus::PlaySound(SE_BOMB_SMALL);
//		return;
//	}
//
//	if(viiSub::IsScrollOut(x_,y_,20*100,20*100)){
//		exist_ = gxFalse;
//		return;
//	}
//
//	if(opt[1] == 1){
//		if(free.w[0] > 0){
//			free.w[0] -= 16;
//			if(free.w[0] <= 0){
//				free.w[0] = 0;
//				exist_ = gxFalse;
//				return;
//			}
//		}
//	}else{
//		OBJ_POS_T ataripos;
//		ataripos.x = x_;
//		ataripos.y = y_;
//		ataripos.ax1 = -4;
//		ataripos.ay1 = -4;
//		ataripos.ax2 =  4;
//		ataripos.ay2 =  4;
////ZANTEI		m_HitAtari.Set(ID_ENEMY_ATK,&ataripos);
////ZANTEI		m_HitAtari.Activate();
//	}
////	Sint32 col = free.w[0];
//	//------------------------
//	//�ړ�����
//	//------------------------
//	x_ += xx_;
//	y_ += yy_;
//
//	OBJ_POS_T ataripos;
//	ataripos.x = x_;
//	ataripos.y = y_;
//	ataripos.ax1 = -4;
//	ataripos.ay1 = -4;
//	ataripos.ax2 =  4;
//	ataripos.ay2 =  4;
//
//	if(opt[1] == 1){
//		viiDraw::Sprite(&spr_ene_bullet[7],x_,y_,PRIO_ENE_BULLET,ATR_ALPHA_PLUS,ARGB(free.w[0],0xff,0xff,0xff),1.0f,rot);
//	}else{
//		if(timer_%2==0) viiEff::Set(EFF_TWINCANNON,x_,y_,argset(rot,1));
//		viiDraw::Sprite(&spr_ene_bullet[7],x_,y_,PRIO_ENE_BULLET,ATR_ALPHA_PLUS,ARGB(free.w[0],0xff,0xff,0xff),1.0f,rot);
//	}
}

