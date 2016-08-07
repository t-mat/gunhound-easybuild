//-------------------------------------------------------
//@
//@ あたりありエフェクト
//@
//-------------------------------------------------------
#include <gunvalken.h>
#include "EffectBase.h"


void CEffect::ArmyCannon()
{
	//---------------------------------------
	//バズーカアーミー
	//---------------------------------------
	extern gxSprite spr_army02[];
	Sint32 atr = ATR_DFLT;
	Sint32 spr = free.w[3];
	Sint32 spd = 80;
	Sint32 joypad = 0;//Joy[0].psh;

	if (is_init()) {
//ZANTEI		m_HitKurai.SetDirectHitPosition( 1 );
		free.b[0] = 1;		//向き
		free.b[1] = 0;		//歩行カウンタ
		free.b[2] = 0;		//クッションカウンタ
		free.w[3] = 0;		//スプライトパターン
		free.w[4] = 0;		//ジャンプカウンタ
		free.w[5] = 0;		//ウェイトカウンタ
		free.w[6] = 0;		//攻撃カウンタ
		free.w[7] = 0;		//行動ステータス
		free.w[8] = 0;		//行動ウェイト
		free.w[9] = -80;		//やられタイマー
		free.w[10] = 0;
		pObj = new CObj();
		pObj->pos.x = x_;
		pObj->pos.y = y_;
		pos.mx = 0;
		pos.my = 0;
		yy_ = 0;
		timer_ = 1+viiSub::Rand()%64;
	}

//ZANTEI	if(m_HitKurai.is_dead())
		{
		if(free.w[10] == 0) {
			free.w[10] = 1;
			viiMus::PlaySound(SE_TIBIYARARE);
		}
		free.w[9] +=8;
		Sint32 sd = free.w[9];
		if(sd >= 300) sd = 300;
		pObj->pos.x  += pObj->mov.x;
		pObj->pos.y  += sd;
		if(viiSub::IsScrollOut(pObj->pos.x,pObj->pos.y,1600,1600)){
			exist_ = gxFalse;
		}

		atr = ATR_DFLT;
		if(free.b[0] == 1) atr |= ATR_FLIP_X;
		viiDraw::Sprite(&spr_army02[10],pObj->pos.x,pObj->pos.y,PRIO_ENEMY+5,atr);
		return;
	}

	//自動運転
	Sint32 tx = GetTargetPlayer()->x;
	Sint32 dst;
	switch(free.w[7]){
	case 0:
		//何もしていない
		dst = get_valken_dist(pObj->pos.x,pObj->pos.y);
		if( dst <= 8000) {
			//逃げパターンに入る
			free.w[7] = 30;
			free.w[8] = 120+(viiSub::Rand()%8)*10;
		}else  if(dst >= 19000) {
			//近づきパターンに入る
			free.w[7] = 10;
			free.w[8] = 10+(viiSub::Rand()%24)*10;
		}else{
			if(viiSub::Rand()%80 == 0) {
				joypad = BTN_A;
			}
		}
		break;
	case 10:	//近づく
		if(tx > pObj->pos.x +11000) {
			joypad = JOY_R;
		}else if(tx < pObj->pos.x-11000) {
			joypad = JOY_L;
		}
		free.w[8] --;
		if(free.w[8] > 0) free.w[8] --;	else free.w[7] = 0;
		break;

	case 20:	//迷う
		dst = get_valken_dist(pObj->pos.x,pObj->pos.y);
		if( dst <= 11000) {
			//逃げパターンに入る
			free.w[7] = 30;
			free.w[8] = 120+(viiSub::Rand()%8)*10;
			break;
		}
		free.w[8] --;
		if(free.w[8] > 0) free.w[8] --;	else free.w[7] = 0;
		break;
	case 30:	//にげる
		if(tx < pObj->pos.x) {
			joypad = JOY_R;
		}else{
			joypad = JOY_L;
		}
		
		if(free.w[8] > 0) free.w[8] --;
		else {
			free.w[7] = 20;
			free.w[8] = 30;
		}
		break;
	}


	//攻撃
	if(joypad&BTN_A && free.w[6] == 0) {
		free.w[6] = 1;
	}

	if(free.w[6] == 0) {
		if(joypad&JOY_L) {
			free.b[1] ++;
			free.b[0] = -1;
			pos.mx = -spd;
			free.w[5] = 0;
		}else if(joypad&JOY_R) {
			free.b[1] ++;
			free.b[0] = 1;
			pos.mx = spd;
			free.w[5] = 0;
		}else{
			pos.mx = 0;
			if(free.b[0] > 0) {
				if(free.b[1] != 9) {
					pos.mx = 10;
					free.b[1] ++;
				}
			}else if(free.b[0] < 0) {
				if(free.b[1] != 9) {
					pos.mx = -10;
					free.b[1] ++;
				}
			}
		}
	}

	//----------------------------------------
	//基本スプライトを設定
	//----------------------------------------
	Sint32 walk_tbl[] = {6,7,8,9};
	free.b[1] = ( (4*9)+free.b[1] )%(4*9);
	Sint32 walk = free.b[1]/9;
	walk = walk%4;
	spr = walk_tbl[walk];

	if(pos.mx == 0) {
		//静止状態
		spr = 5;
		free.w[5] ++;
		if(free.w[5] >= 30) {
			if(free.w[5] >= 120) {
				free.w[5] = 30;
			}
			spr = (free.w[5] -30 )/45;
		}
	}

	const Sint32 atk_tbl[]={
		6,5,4,4,3,3,3,3,3,2,3,3,4,4,5,5,5,-1
	};
	if(free.w[6] > 0) {
		//攻撃
		if(tx > pObj->pos.x) {
			free.b[0] = 1;
		}else if(tx < pObj->pos.x) {
			free.b[0] = -1;
		}
		dst = get_valken_dist(pObj->pos.x,pObj->pos.y);

		free.w[6] ++;
		spr = atk_tbl[free.w[6]/8];
		if(free.w[6] == 10*8 && viiSub::Rand()%4==0) {
			free.w[6] = 33;
		}else if(free.w[6] == 16*8 && viiSub::Rand()%4==0) {
			free.w[6] = 8;
		}
		
		if(free.w[6] == 72) {
			viiEff::Set(EFF_MAZZLE    ,pObj->pos.x+800*free.b[0],pos.y-1800,NULL);
			viiEff::Set(EFF_ENE_BULLET2,pObj->pos.x+800*free.b[0],pos.y-2000,argset(90-120*free.b[0],250));
		}
		
		if(spr == -1) {
			spr = 5;
			free.w[6] = 0;
		}
		if( dst <= 3200 && viiSub::Rand()%6==0) {
			//逃げパターンに入る
			free.w[6] = 0;
			free.w[7] = 30;
			free.w[8] = 120+(viiSub::Rand()%8)*10;
		}
	}


	if(viiSub::IsScrollOut(pObj->pos.x,pObj->pos.y,1600,1600)){
/*		exist_ = gxFalse;
		delete pObj;
		pObj = NULL;
*/
		return;
	}

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
	//向きの制御
	//----------------------------
	atr = ATR_DFLT;
	if(free.b[0] == 1) atr |= ATR_FLIP_X;
	viiDraw::Sprite(&spr_army02[spr],pObj->pos.x,pObj->pos.y,PRIO_ENEMY+5,atr);
	free.w[3] = spr;

	//----------------------------
	//あたり判定
	//----------------------------
	pos.x = pObj->pos.x;
	pos.y = pObj->pos.y;
	pos.kx1 =  -4;	//当たり判定
	pos.ky1 =  -12;
	pos.kx2 =   4;
	pos.ky2 =   0;
//ZANTEI	m_HitKurai.Set(ID_ENEMY_DEF,&pos);
//ZANTEI	m_HitKurai.Activate();

}

void CEffect::engineer3()
{
	//-------------------------------------
	//ブランディッシュにやられるエンジニア
	//-------------------------------------
	static const Sint32 human_eng[][8]={
		{0+1*16,1+1*16,0+1*16,2+1*16},
	};
	Sint32 atr = ATR_DFLT;
	gxSprite spr;

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		free.w[0] = 1;
		free.w[1] = 0;
		free.w[2] = 0;
	}

//ZANTEI	if(m_HitAtari.IsImpacted())
	{
		exist_=gxFalse;
		viiEff::Set(EFF_HUMAN_DAMAGE ,pos.x,pos.y,argset(0,1));
		return;
	}

	if(free.w[0] == 1) {
		pos.mx = 40+viiSub::Rand()%40;
		if(viiSub::Rand()%2==0)pos.mx *= -1;
		free.w[0] = 0;
		free.w[2] = 0;
	}

	pos.x += pos.mx;
	free.w[1] += pos.mx;
	if(free.w[1] >= 300*100) {
		exist_ = gxFalse;
		return;
	}

	if(abs(pos.mx) > 0) {
		if(pos.mx > 0) {
			pos.mx --;
		}else if(pos.mx < 0) {
			pos.mx ++;
		}
		if( abs(pos.mx) <= 10 ) free.w[0] = 1;
	}

	Sint32 n = 0;
	if(timer_%8==0) free.w[2] ++;
	n = free.w[2]%4;

	Sint32 no = human_eng[ 0 ][ n ];

	spr.page = TPG_HUMAN;
	spr.u = no%16*16;
	spr.v = no/16*16;
	spr.w = 16;
	spr.h = 16;
	spr.cx = 8;
	spr.cy = 16;

	if(pos.mx > 0) atr |= ATR_FLIP_X;
	viiDraw::Sprite(&spr,pos.x,pos.y,PRIO_ENEMY,atr,ARGB_DFLT);//,1.0f,pos.rot);

	pos.kx1 =  -4;
	pos.kx2 =   4;
	pos.ky1 =  -4;
	pos.ky2 =   4;
//ZANTEI	m_HitKurai.Set(ID_ENEMY_DEF,&pos);
//ZANTEI	m_HitKurai.Activate();

}


void CEffect::ArmySargent()
{
	//---------------------------------------
	//司令官
	//---------------------------------------
	extern gxSprite spr_sergent[];
	Sint32 atr = ATR_DFLT;
	Sint32 spr = free.w[3];
	Sint32 spd = 100;
	Sint32 joypad = 0;//Joy[0].psh;

	if (is_init()) {
//ZANTEI		m_HitKurai.SetDirectHitPosition( 1 );
		free.b[0] = 1;		//向き
		free.b[1] = 0;		//歩行カウンタ
		free.b[2] = 0;		//クッションカウンタ
		free.w[3] = 0;		//スプライトパターン
		free.w[4] = 0;		//ジャンプカウンタ
		free.w[5] = 0;		//ウェイトカウンタ
		free.w[6] = 0;		//攻撃カウンタ
		free.w[7] = 0;		//行動ステータス
		free.w[8] = 0;		//行動ウェイト
		free.w[9] = -80;		//やられタイマー
		free.w[10] = 0;
		pObj = new CObj();
		pObj->pos.x = x_;
		pObj->pos.y = y_;
		pos.mx = 0;
		pos.my = 0;
		yy_ = 0;
		timer_ = 1+viiSub::Rand()%64;
	}

//ZANTEI	if(m_HitKurai.is_dead())
	{
		if(free.w[10] == 0) {
			free.w[10] = 1;
			viiMus::PlaySound(SE_TIBIYARARE);
		}
		free.w[9] +=8;
		Sint32 sd = free.w[9];
		if(sd >= 300) sd = 300;
		pObj->pos.x  += pObj->mov.x;
		pObj->pos.y  += sd;
		if(viiSub::IsScrollOut(pObj->pos.x,pObj->pos.y,1600,1600)){
			exist_ = gxFalse;
		}

		atr = ATR_DFLT;
		if(free.b[0] == 1) atr |= ATR_FLIP_X;
		viiDraw::Sprite(&spr_sergent[8],pObj->pos.x,pObj->pos.y,PRIO_ENEMY+5,atr);
		return;
	}

	//自動運転
	Sint32 tx = GetTargetPlayer()->x;
	Sint32 dst;

	dst = abs(tx-pObj->pos.x);//get_valken_dist(pObj->pos.x,pObj->pos.y);

	switch(free.w[7]){
	case 0:	//判定
		if( dst <= 5000) {
			//逃げパターンに入る
			free.w[7] = 10;
			joypad = JOY_R;
		}else  if(dst <= 10000) {
			//退却指示パターンに入る
			joypad = BTN_A;
		}else  if(dst <= 24000) {
			//近づくパターンに入る
			joypad = JOY_L;
		}else{
			//通信モード
		}
		break;
	case 10:	//逃げモード
		if(dst >= 24000) {
			free.w[7] = 0;
		}
		joypad = JOY_R;
		break;
	}
	if(joypad&JOY_L) {
		free.b[1] ++;
		free.b[0] = 1;
		pos.mx = -spd;
	}else if(joypad&JOY_R) {
		free.b[1] ++;
		free.b[0] = -1;
		pos.mx = spd;
	}else{

	}

	//----------------------------------------
	//基本スプライトを設定
	//----------------------------------------

	//「逃げろ～」
	Sint32 walk_tbl[] = {1,2,3};
	free.b[1] = ( (3*9)+free.b[1] )%(3*9);
	Sint32 walk = free.b[1]/9;
	walk = walk%3;
	spr = walk_tbl[walk];

	//連絡中
	if(pos.mx == 0) {
		//静止状態
		spr = 0;
	}

	//「退却～！」
	if(joypad&BTN_A) {
		const Sint32 atk_tbl[]={
			4,4,4,4,4,5,5,6,6,7,7,7,-1
		};
		free.w[6] ++;
		spr = atk_tbl[free.w[6]/8];
		if(spr == -1) {
			spr = 5;
			free.w[6] = 40;
		}
	}


	if(viiSub::IsScrollOut(pObj->pos.x,pObj->pos.y,1600,1600)){
/*		exist_ = gxFalse;
		delete pObj;
		pObj = NULL;
*/
		return;
	}

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
	}

	pObj->pos.x += pObj->mov.x;
	pObj->pos.y += pObj->mov.y;


	//----------------------------
	//向きの制御
	//----------------------------
	atr = ATR_DFLT;
	if(free.b[0] == 1) atr |= ATR_FLIP_X;
	viiDraw::Sprite(&spr_sergent[spr],pObj->pos.x,pObj->pos.y,PRIO_ENEMY+5,atr);
	free.w[3] = spr;

	//----------------------------
	//あたり判定
	//----------------------------
	pos.x = pObj->pos.x;
	pos.y = pObj->pos.y;
	pos.kx1 =  -4;	//当たり判定
	pos.ky1 =  -12;
	pos.kx2 =   4;
	pos.ky2 =   0;
//ZANTEI	m_HitKurai.Set(ID_ENEMY_DEF,&pos);
//ZANTEI	m_HitKurai.Activate();

}

void CEffect::ene_bullet2()
{
	//------------------------------
	//敵バルカン弾（放物線軌道）
	//------------------------------

	if (is_init()) {
		rot = opt[0];		//初期方向
		xx_ = (viiMath::Cos100(rot)*opt[1])/100;
		yy_ = (viiMath::Sin100(rot)*opt[1])/100;

		rot = viiSub::Rand()%360;
//ZANTEI		m_HitAtari.set_ap(1);
//ZANTEI		m_HitAtari.set_hp(1);
		free.w[0] = 0;
	}

	Sint32 lv = 0;

	//存在チェック
//ZANTEI	if(m_HitAtari.IsImpacted() || viiSub::GetBoundBlock_Enemy(x_,y_) )
	{
		exist_ = gxFalse;
		_opt_[0] = 0;
		viiEff::SetBombEffect(x_,y_,3,3);
//		viiEff::Set(EFF_HITFLASH_ENE,x_,y_,o);
		return;
	}

	if(viiSub::IsScrollOut(x_,y_,0,0)){
		exist_ = gxFalse;
		return;
	}

	if(free.w[0] < 300) free.w[0] +=4;

	x_ += xx_;
	y_ += (yy_+free.w[0]);

	OBJ_POS_T ataripos;
	ataripos.x = x_;
	ataripos.y = y_;
	ataripos.ax1 = -4+(-lv*2);	//レベルに応じてあたり判定を大きくする
	ataripos.ay1 = -4+(-lv*2);
	ataripos.ax2 =  4+(+lv*2);
	ataripos.ay2 =  4+(+lv*2);

//ZANTEI	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
//ZANTEI	m_HitAtari.Set(ID_ENEMY_ATK,&ataripos);
//ZANTEI	m_HitAtari.Activate();

	rot += 8;
	viiDraw::Sprite(&spr_ene_bullet[0],x_,y_,PRIO_ENE_BULLET,ATR_DEFAULT,ARGB_DFLT,1.0f,rot);

}


void CEffect::ene_shot_photon()
{
	//宇宙魚雷
	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		pos.mx = 100;
		pos.my = 0;
		free.w[0] = 300;
		free.w[1] = 0;
		free.w[2] = 0;
//ZANTEI		m_HitAtari.set_ap(1);
//ZANTEI		m_HitKurai.set_hp(HP_ARMORCLASS_B);
	}

	Sint32 px = GetTargetPlayer()->x;
	Sint32 py = GetTargetPlayer()->y;

	switch(free.w[1]){
	case 0:	//壁エリアチェック中
		if(timer_ > 60){
			pos.mx = 300;
			if(viiSub::GetBoundBlock_Enemy(pos.x,pos.y) == 0){
				free.w[2] ++;
			}
			if(free.w[2] >= 30){
				free.w[1] = 1;
			}
		}
		break;
	case 1:	//壁エリア抜けた
		if(viiSub::GetBoundBlock_Enemy(pos.x,pos.y))	{
			//壁にあたった
			viiEff::SetBombEffect(pos.x,pos.y,3,3);
			exist_ = gxFalse;
		}
		break;
	}

	if(free.w[0] > 0){
		if(free.w[1] > 0){
			free.w[0] --;
		}
		pos.x += SHOOT_SCROLL_SPEED;
	}else{
		free.w[0] --;
		pos.x += free.w[0];
		if(viiSub::IsScrollOut(pos.x,pos.y,6400,6400)){
			exist_ = gxFalse;
			return;
		}
	}

	//ホーミング
	Sint32 spd = 4;
	if(px-8*100  < pos.x) pos.mx -=spd;
	if(px+8*100  > pos.x) pos.mx +=spd;

	if(px-8*100  < pos.x) pos.x -=20;
	if(px+8*100  > pos.x) pos.x +=20;

	if(pos.mx <= -200) pos.mx = -200;
	if(pos.mx >=  200) pos.mx =  200;

	if(free.w[1] > 0){
		if(py-32*100 < pos.y) pos.my -=spd;
		if(py-8*100  > pos.y) pos.my +=spd;

		if(py-32*100 < pos.y) pos.y -=20;
		if(py-8*100  > pos.y) pos.y +=20;

		if(pos.my <= -200) pos.my = -200;
		if(pos.my >=  200) pos.my =  200;
	}

	pos.x += pos.mx;
	pos.y += pos.my;

	if(free.w[1] == 0){
		//半透明にしておく
//ZANTEI		viiDraw::Sprdmg(m_HitKurai.is_damage(),&AsEneSpr02[14],pos.x,pos.y,PRIO_ENE_BULLET,ATR_DEFAULT|ATR_FLIP_X,0x80ffffff);
	}else{
//ZANTEI		viiDraw::Sprdmg(m_HitKurai.is_damage(),&AsEneSpr02[14],pos.x,pos.y,PRIO_ENE_BULLET,ATR_DEFAULT|ATR_FLIP_X,ARGB_DFLT);
	}
	if(free.w[1] > 0 && timer_%2==0)
	{
//ZANTEI		viiDraw::Spr_dmg_NoScrl(m_HitKurai.is_damage(),&AsEneSpr02[18],pos.x-2400,pos.y,PRIO_ENE_BULLET,ATR_ALPHA_PLUS|ATR_FLIP_X,ARGB_DFLT);
		viiDraw::Sprite(&AsEneSpr02[18],pos.x-2400-100+viiSub::Rand()%200,pos.y+viiSub::Rand()%200,PRIO_ENE_BULLET,ATR_ALPHA_PLUS|ATR_FLIP_X,ARGB_DFLT);
	}

	if(free.w[1] > 0)
	{
//ZANTEI		if(m_HitAtari.IsImpacted())
		{
			viiEff::SetBombEffect(pos.x,pos.y,5,8);
			exist_ = gxFalse;
			return;
		}
//ZANTEI		if(m_HitKurai.is_dead())
		{
			viiSub::AddScore(100);
			viiMus::PlaySound(SE_BOMB_SMALL);
			viiEff::SetBombEffect(pos.x,pos.y,3,5);
			exist_ = gxFalse;
			return;
		}

		OBJ_POS_T ataripos;
		ataripos.x = pos.x;
		ataripos.y = pos.y;
		//当たり用
		ataripos.ax1 = -12;
		ataripos.ay1 = -5;
		ataripos.ax2 =  12;
		ataripos.ay2 =  5;
		//くらい用
		ataripos.kx1 = -16;
		ataripos.ky1 = -5;
		ataripos.kx2 =  16;
		ataripos.ky2 =  5;
//ZANTEI		m_HitAtari.Set(ID_ENEMY_ATK,&ataripos);
//ZANTEI		m_HitAtari.Activate();

//ZANTEI		m_HitKurai.Set(ID_ENEMY_DEF,&ataripos);
//ZANTEI		m_HitKurai.Activate();

	}
}

void CEffect::ene_bitlaser()
{
	//ビットレーザー

	if (is_init()) {
		free.w[0]=200;
		rot = opt[0];

		xx_ = (viiMath::Cos100(rot)*200)/100;
		yy_ = (viiMath::Sin100(rot)*200)/100;

		if(opt[1]){
			//消える速度
			xx_ = yy_ = 0;
			free.w[0]=128;
		}
		//opt[2]	ホーミング属性

//ZANTEI		m_HitAtari.set_ap(1);
//ZANTEI		m_HitAtari.set_hp(1);
		free.w[1] = 0;	//回転タイマー
		pos.rot = 0;
	}

	//存在チェック
//ZANTEI	if(m_HitAtari.IsImpacted() || (viiSub::GetBoundBlock_Enemy( x_,y_ ) && opt[1]==0) )
	{
		exist_ = gxFalse;
		viiEff::SetBombEffect(x_,y_,3,5);
		viiMus::PlaySound(SE_BOMB_SMALL);
		return;
	}

	if(viiSub::IsScrollOut(x_,y_,20*100,20*100)){
		exist_ = gxFalse;
		return;
	}

	if(opt[1] >= 1){
		if(free.w[0] > 0){
			free.w[0] -= 4/opt[1];
			if(free.w[0] <= 0){
				free.w[0] = 0;
				exist_ = gxFalse;
				return;
			}
		}
	}else{
		OBJ_POS_T ataripos;
		ataripos.x = x_;
		ataripos.y = y_;
		ataripos.ax1 = -4;
		ataripos.ay1 = -4;
		ataripos.ax2 =  4;
		ataripos.ay2 =  4;
//ZANTEI		m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
//ZANTEI		m_HitAtari.Set(ID_ENEMY_ATK,&ataripos);
//ZANTEI		m_HitAtari.Activate();
	}
//	Sint32 col = free.w[0];
	//------------------------
	//移動処理
	//------------------------

	if(opt[2] == 1) {
		//ホーミング
		if(timer_ < 90){
			Sint32 r = get_valken_rot(x_,y_+2400);
			Sint32 ar = viiMath::GetTargetAngle(rot,r);
			pos.rot = ar;
		}else{
			pos.rot = 0;
		}
		if(pos.rot < -5){
			rot -= 2;
		}else if(pos.rot > 5){
			rot += 2;
		}
		x_ += (viiMath::Cos100(rot)*200)/100;
		y_ += (viiMath::Sin100(rot)*200)/100;
	}else{
		x_ += xx_;
		y_ += yy_;
	}
	x_ -= pGame->power_x;

	OBJ_POS_T ataripos;
	ataripos.x = x_;
	ataripos.y = y_;
	ataripos.ax1 = -4;
	ataripos.ay1 = -4;
	ataripos.ax2 =  4;
	ataripos.ay2 =  4;

	if (opt[1] >= 1) {
//		viiDraw::Sprite(&spr_sceneobj[8],x_,y_,PRIO_ENE_BULLET,ATR_ALPHA_PLUS|ATR_FLIP_X,ARGB(free.w[0]*2,0xff,0xff,0xff),0.8f,rot);
		viiDraw::Sprite(&spr_cloud[1],x_,y_,PRIO_ENE_BULLET,ATR_ALPHA_PLUS|ATR_FLIP_X,ARGB(free.w[0]*2,0xff,0xff,0xff),0.2f,rot);

	}else{
		if(timer_%4==1) viiEff::Set(EFF_SHOT_BITLASER,x_,y_,argset(rot,1));
//		if(timer_%2==0) viiDraw::Sprite(&spr_sceneobj[8],x_,y_,PRIO_ENE_BULLET+1,ATR_ALPHA_PLUS,ARGB(free.w[0]/2,0xff,0xff,0xff),1.0f,-rot);
		if(timer_%2==0) viiDraw::Sprite(&spr_cloud[1],x_,y_,PRIO_ENE_BULLET+1,ATR_ALPHA_PLUS,ARGB(free.w[0]/2,0xff,0xff,0xff),1.0f,-rot);

		viiDraw::Sprite(&spr_cloud[1],x_,y_,PRIO_ENE_BULLET,ATR_DFLT,ARGB(free.w[0],0xff,0xff,0xff),0.5f,rot);
//		viiDraw::Sprite(&spr_sceneobj[8],x_,y_,PRIO_ENE_BULLET,ATR_DFLT,ARGB(free.w[0],0xff,0xff,0xff),0.5f,rot);
//		viiDraw::Sprite(&spr_sceneobj[8],x_,y_,PRIO_ENE_BULLET+1,ATR_ALPHA_PLUS,ARGB(free.w[0]/2,0xff,0xff,0xff),1.0f,-rot);
	}

}


void CEffect::ene_bomb2()
{
	//---------------------
	//敵爆弾2(爆撃機より)
	//---------------------

	if (is_init()) {
		rot = 0;		//初期方向
		xx_ = opt[0];
		yy_ = 0;

		rot = viiSub::Rand()%360;
//ZANTEI		m_HitAtari.set_ap(1);
//ZANTEI		m_HitAtari.set_hp(1);
	}

	//存在チェック
//ZANTEI	if(m_HitAtari.IsImpacted())
	{
		viiMus::PlaySound(SE_BOMB_SMALL);
		viiEff::SetBombEffect(x_,y_,3,3);
		viiEff::Set(EFF_BOMBFLASH_ENE,x_,y_);
		exist_ = gxFalse;
		return;
	}
	if(viiSub::GetBoundBlock_Enemy(x_,y_)){
		viiMus::PlaySound(SE_BOMB_SMALL);
		viiEff::Set(EFF_BOMBFLASH_ENE,x_,y_);
		exist_ = gxFalse;
		return;
	}
	if(viiSub::IsScrollOut(x_,y_,0,0)){
		exist_ = gxFalse;
		return;
	}

	//加速
	yy_+=12;
	if(yy_>=700){
		yy_ = 700;
	}
	//Xは減速
	xx_ += -xx_/20;

	x_ += xx_;
	y_ += yy_;

//	Sint32 lv = 0;
	OBJ_POS_T ataripos;
	ataripos.x = x_;
	ataripos.y = y_;
	ataripos.ax1 = -4;
	ataripos.ay1 = -4;
	ataripos.ax2 =  4;
	ataripos.ay2 =  4;

//ZANTEI	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
//ZANTEI	m_HitAtari.Set(ID_ENEMY_ATK,&ataripos);
//ZANTEI	m_HitAtari.Activate();

	rot += 2;
	viiDraw::Sprite(&spr_ene_bullet[1],x_,y_,PRIO_ENE_BULLET,ATR_DEFAULT,ARGB_DFLT,1.0f,rot);

}


void CEffect::ene_propera_missile()
{
	//---------------------
	//敵プロペラミサイル
	//---------------------

	if (is_init()) {
		rot = -45;
		if(opt[0] < 0){
			rot = 270-45;
		}
		xx_ = (viiMath::Cos100(rot)*opt[1])/100;
		yy_ = (viiMath::Sin100(rot)*opt[1])/200;

//ZANTEI		m_HitAtari.set_ap(1);
//ZANTEI		m_HitAtari.set_hp(1);
//ZANTEI		m_HitKurai.set_hp(HP_ARMORCLASS_B);
		pos.ax1 = -4;
		pos.ay1 = -4;
		pos.ax2 =  4;
		pos.ay2 =  4;

		pos.kx1 = -6;
		pos.ky1 = -6;
		pos.kx2 =  6;
		pos.ky2 =  6;
	}

	//存在チェック
//ZANTEI	if(m_HitAtari.IsImpacted() )
	{
		exist_ = gxFalse;
		viiEff::SetBombEffect(x_,y_,3,5);
		viiEff::Set(EFF_BOMBFLASH_ENE,x_,y_);
		viiMus::PlaySound(SE_BOMB_SMALL);
		return;
	}

	if(viiSub::IsScrollOut(x_,y_,128*100,128*100)){
		exist_ = gxFalse;
		return;
	}

	//------------------------
	//移動処理
	//------------------------
	if(xx_ > 0){
		xx_ -= 4;
		if(xx_ < 0){
			xx_ = 0;
		}
	}
	if(xx_ < 0){
		xx_ += 4;
		if(xx_ > 0){
			xx_ = 0;
		}
	}
	yy_ += 4;
	if(yy_ >= 200){
		yy_ = 200;
	}
	x_ += xx_;
	y_ += yy_;

	Sint32 r = viiMath::Atan2_100(y_-(y_-yy_),x_-(x_-xx_))/100;
	rot = r;

	pos.x = x_;
	pos.y = y_;

	if(r == 90)
	{
		if(viiSub::GetBoundBlock_Enemy(x_,y_)) {
			exist_ = gxFalse;
			viiEff::SetBombEffect(x_,y_,3,5);
			viiEff::Set(EFF_BOMBFLASH_ENE,x_,y_);
			viiMus::PlaySound(SE_BOMB_SMALL);
			return;
		}
//ZANTEI		m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
//ZANTEI		m_HitAtari.Set(ID_ENEMY_ATK,&pos);
//ZANTEI		m_HitAtari.Activate();

/*
//ZANTEI		m_HitKurai.Set(ID_ENEMY_DEF,&pos);
//ZANTEI		m_HitKurai.Activate();
*/
		if((timer_%8)<4){
			viiDraw::Sprite(&spr_ene_bullet[4],x_,y_,PRIO_ENEMY-1,ATR_DEFAULT,ARGB_DFLT,1.0f,0);
		}else{
			viiDraw::Sprite(&spr_ene_bullet[5],x_,y_,PRIO_ENEMY-1,ATR_DEFAULT,ARGB_DFLT,1.0f,0);
		}
	}else{
		viiDraw::Sprite(&spr_ene_bullet[3],x_,y_,PRIO_ENEMY-1,ATR_DEFAULT,ARGB_DFLT,1.0f,rot);
	}
}


void CEffect::lasercannon()
{
	//---------------------
	//敵シャトルレーザーキャノン
	//---------------------

	if (is_init()) {
		free.w[0]=200;
		rot = opt[0];

		xx_ = (viiMath::Cos100(rot)*300)/100;
		yy_ = (viiMath::Sin100(rot)*300)/100;
		if(opt[1]){
			xx_ = yy_ = 0;
			free.w[0]=128;
		}
//ZANTEI		m_HitAtari.set_ap(1);
//ZANTEI		m_HitAtari.set_hp(1);
	}

	//存在チェック
//ZANTEI	if(m_HitAtari.IsImpacted() )
	{
		exist_ = gxFalse;
		viiEff::SetBombEffect(x_,y_,3,5);
		viiMus::PlaySound(SE_BOMB_SMALL);
		return;
	}

	if(viiSub::IsScrollOut(x_,y_,20*100,20*100)){
		exist_ = gxFalse;
		return;
	}

	if(opt[1] == 1){
		if(free.w[0] > 0){
			free.w[0] -= 16;
			if(free.w[0] <= 0){
				free.w[0] = 0;
				exist_ = gxFalse;
				return;
			}
		}
	}else{
		OBJ_POS_T ataripos;
		ataripos.x = x_;
		ataripos.y = y_;
		ataripos.ax1 = -4;
		ataripos.ay1 = -4;
		ataripos.ax2 =  4;
		ataripos.ay2 =  4;
//ZANTEI		m_HitAtari.Set(ID_ENEMY_ATK,&ataripos);
//ZANTEI		m_HitAtari.Activate();
	}
//	Sint32 col = free.w[0];
	//------------------------
	//移動処理
	//------------------------
	x_ += xx_;
	y_ += yy_;

	OBJ_POS_T ataripos;
	ataripos.x = x_;
	ataripos.y = y_;
	ataripos.ax1 = -4;
	ataripos.ay1 = -4;
	ataripos.ax2 =  4;
	ataripos.ay2 =  4;

	if(opt[1] == 1){
		viiDraw::Sprite(&spr_ene_bullet[6],x_,y_,PRIO_ENE_BULLET,ATR_ALPHA_PLUS,ARGB(free.w[0],0xff,0xff,0xff),1.0f,rot);
	}else{
		if(timer_%2==0) viiEff::Set(EFF_LASERCANNON,x_,y_,argset(rot,1));
		viiDraw::Sprite(&spr_ene_bullet[6],x_,y_,PRIO_ENE_BULLET,ATR_DEFAULT,ARGB(free.w[0],0xff,0xff,0xff),1.0f,rot);
	}
}

void CEffect::ene_laser()
{
	//ＡＳから放たれるレーザーキャノン
	//EFF_ENE_LASERCANNON,

	if (is_init()) {
		free.w[0]=200;
		rot = opt[0];

		xx_ = (viiMath::Cos100(rot)*200)/100;
		yy_ = (viiMath::Sin100(rot)*200)/100;
		if(opt[1]){
			xx_ = yy_ = 0;
			free.w[0]=128;
		}
//ZANTEI		m_HitAtari.set_ap(1);
//ZANTEI		m_HitAtari.set_hp(1);
	}

	//存在チェック
//ZANTEI	if(m_HitAtari.IsImpacted() || (viiSub::GetBoundBlock_Enemy( x_,y_ ) && opt[1]==0) )
	{
		exist_ = gxFalse;
		viiEff::SetBombEffect(x_,y_,3,5);
		viiMus::PlaySound(SE_BOMB_SMALL);
		return;
	}

	if(viiSub::IsScrollOut(x_,y_,20*100,20*100)){
		exist_ = gxFalse;
		return;
	}

	if(opt[1] >= 1){
		if(free.w[0] > 0){
			free.w[0] -= 4/opt[1];
			if(free.w[0] <= 0){
				free.w[0] = 0;
				exist_ = gxFalse;
				return;
			}
		}
	}else{
		OBJ_POS_T ataripos;
		ataripos.x = x_;
		ataripos.y = y_;
		ataripos.ax1 = -4;
		ataripos.ay1 = -4;
		ataripos.ax2 =  4;
		ataripos.ay2 =  4;
//ZANTEI		m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
//ZANTEI		m_HitAtari.Set(ID_ENEMY_ATK,&ataripos);
//ZANTEI		m_HitAtari.Activate();
	}
//	Sint32 col = free.w[0];
	//------------------------
	//移動処理
	//------------------------
	x_ += xx_;
	y_ += yy_;

	OBJ_POS_T ataripos;
	ataripos.x = x_;
	ataripos.y = y_;
	ataripos.ax1 = -4;
	ataripos.ay1 = -4;
	ataripos.ax2 =  4;
	ataripos.ay2 =  4;

	if (opt[1] >= 1) {
		viiDraw::Sprite(&spr_ene_bullet[8],x_,y_,PRIO_ENE_BULLET,ATR_ALPHA_PLUS,ARGB(free.w[0],0xff,0xff,0xff),1.0f,rot);
	}else{
		if(timer_%2==1) viiEff::Set(EFF_ENE_LASERCANNON,x_,y_,argset(rot,1));
		viiDraw::Sprite(&spr_ene_bullet[8],x_,y_,PRIO_ENE_BULLET,ATR_ALPHA_PLUS,ARGB(free.w[0],0xff,0xff,0xff),1.0f,rot);
	}

}


void CEffect::energybeam()
{
	//据え置きのエネルギービーム砲
	//EFF_ENERGYBEAM

	if (is_init()) {
		free.w[0]=200;
		free.w[1]=0;
		rot = opt[0];

		xx_ = (viiMath::Cos100(rot)*400)/100;
		yy_ = (viiMath::Sin100(rot)*400)/100;
		if(opt[1]){
			//消える速度
			xx_ = yy_ = 0;
			free.w[0]=128;
		}
//ZANTEI		m_HitAtari.set_ap(1);
//ZANTEI		m_HitAtari.set_hp(1);
	}

	//存在チェック
//ZANTEI	if(m_HitAtari.IsImpacted() || (viiSub::GetBoundBlock_Enemy( x_,y_ ) && opt[1]==0) )
	{
		exist_ = gxFalse;
		viiEff::SetBombEffect(x_,y_,3,5);
		viiMus::PlaySound(SE_BOMB_SMALL);
		return;
	}

	if(viiSub::IsScrollOut(x_,y_,20*100,20*100)){
		exist_ = gxFalse;
		return;
	}

	if(opt[1] >= 1){
		if(free.w[0] > 0){
			free.w[0] -= 4/opt[1];
			if(free.w[0] <= 0){
				free.w[0] = 0;
				exist_ = gxFalse;
				return;
			}
		}
	}else{
		OBJ_POS_T ataripos;
		ataripos.x = x_;
		ataripos.y = y_;
		ataripos.ax1 = -4;
		ataripos.ay1 = -4;
		ataripos.ax2 =  4;
		ataripos.ay2 =  4;
//ZANTEI		m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
//ZANTEI		m_HitAtari.Set(ID_ENEMY_ATK,&ataripos);
//ZANTEI		m_HitAtari.Activate();
	}
//	Sint32 col = free.w[0];
	//------------------------
	//移動処理
	//------------------------
	x_ += xx_;
	y_ += yy_;

	OBJ_POS_T ataripos;
	ataripos.x = x_;
	ataripos.y = y_;
	ataripos.ax1 = -4;
	ataripos.ay1 = -4;
	ataripos.ax2 =  4;
	ataripos.ay2 =  4;

	free.w[1] += 4;

	if (opt[1] >= 1) {
		viiDraw::Sprite(&spr_cloud[4],x_,y_,PRIO_ENE_BULLET,ATR_ALPHA_PLUS|ATR_FLIP_X,ARGB(free.w[0],0xff,0xff,0xff),2.0f);//,rot);
	}else{
		if(timer_%2==1) viiEff::Set(EFF_ENERGYBEAM,x_,y_,argset(rot,1));
		viiDraw::Sprite(&spr_cloud[4],x_,y_,PRIO_ENE_BULLET,ATR_DFLT|ATR_FLIP_X,ARGB(free.w[0],0xff,0xff,0xff),2.0f);//,free.w[1]);
		viiDraw::Sprite(&spr_cloud[4],x_,y_,PRIO_ENE_BULLET,ATR_ALPHA_PLUS|ATR_FLIP_X,ARGB(free.w[0],0xff,0xff,0xff),2.0f);//,1.0f,-free.w[1]);
	}

}
#define LASER_ROT_SPEED (10)
#define LASER_ROT_TIME  (6)

void CEffect::sprine_laser()
{
	//----------------------------------
	//屈折レーザー
	//----------------------------------

	if (is_init()) {
		rot = opt[1];
		pos.x = x_;
		pos.y = y_;
		free.w[0] = 0;
		free.w[1] = 5;
		free.w[2] = 0;
		if(opt[0]){
			//消える速度
			xx_ = yy_ = 0;
			free.w[0]=128;
			rot = opt[1];
		}

//ZANTEI		m_HitAtari.set_ap(1);
//ZANTEI		m_HitAtari.set_hp(1);
	}

	if(opt[0]){
		//残像処理
		if(viiSub::GameCounter()%1==0)free.w[0] -= 1;
		if(free.w[0] <= 0){
			exist_ = gxFalse;
		}else{
			viiDraw::Sprite(&spr_ene_bullet[9],pos.x,pos.y,PRIO_ENE_BULLET,ATR_DFLT,ARGB(free.w[0],0xff,0xff,0xff),1.0f,rot);
			viiDraw::Sprite(&spr_ene_bullet[9],pos.x,pos.y,PRIO_ENE_BULLET,ATR_ALPHA_PLUS,ARGB(free.w[0],0x00,0xff,0xff),1.5f,rot);
		}
		return;
	}

//ZANTEI	if(viiSub::GetBoundBlock_Enemy(x_,y_) || m_HitAtari.IsImpacted())
	{
		viiEff::SetBombEffect(x_,y_,5,2);
		exist_ = gxFalse;
		return;
	}

	if(free.w[0] == 0){
		Sint32 r = get_valken_rot(x_,y_+2400);
		if(timer_ > 60*3)	r = rot;

		Sint32 rr = viiMath::GetTargetAngle(rot,r);
		if(rr >=  15) rr =  LASER_ROT_SPEED;
		if(rr <= -15) rr = -LASER_ROT_SPEED;
		pos.x = x_;
		pos.y = y_;
		if(free.w[1] == 0) {
			free.w[2] = rr / LASER_ROT_TIME;
//			rot += rr;
		}else{
			free.w[1] --;
		}
		viiEff::Set(EFF_SPRINELASER,pos.x,pos.y,argset(1,rot));
		xx_ = viiMath::Cos100(rot)*12/LASER_ROT_TIME;
		yy_ = viiMath::Sin100(rot)*12/LASER_ROT_TIME;
		free.w[0] = LASER_ROT_TIME;

	}else{
		free.w[0] --;
	}

	rot += free.w[2];
	x_ += xx_;
	y_ += yy_;

	viiDraw::Sprite(&spr_ene_bullet[9],x_,y_,PRIO_ENE_BULLET,ATR_DFLT,ARGB(0x80,0xff,0xff,0xff),1.0f,rot);
	viiDraw::Sprite(&spr_ene_bullet[9],x_,y_,PRIO_ENE_BULLET,ATR_ALPHA_PLUS,ARGB(0x80,0x00,0xff,0xff),1.5f,rot);

//	m_HitAtari.type = ATK_TYPE_CANNON;
	pos.x = x_;
	pos.y = y_;
	pos.ax1 = -4;
	pos.ay1 = -4;
	pos.ax2 =  4;
	pos.ay2 =  4;
//ZANTEI	m_HitAtari.Set(ID_ENEMY_ATK,&pos);
//ZANTEI	m_HitAtari.Activate();

	if(viiSub::IsScrollOut(x_,y_,60*100,60*100)){
		exist_ = gxFalse;
	}
}


void CEffect::sprine_thunder()
{
	//----------------------------------
	//屈折雷レーザー
	//----------------------------------
	Sint32 vx,vy;
	vx = GetTargetPlayer()->x;
	vy = GetTargetPlayer()->y;

	if (is_init()) {
		rot = opt[1];
		free.w[0] = 0;
		free.w[1] = 5;
		free.w[2] = 0;
		if(opt[0]){
			//消える速度
			xx_ = yy_ = 0;
			free.w[0]=128;
			rot = opt[1];
			free.w[3] = 0;
		}
		pos.x = x_+vx;
		pos.y = y_+vy;
//ZANTEI		m_HitAtari.set_ap(1);
//ZANTEI		m_HitAtari.set_hp(1);
	}

	pos.x = x_ + vx;
	pos.y = y_ + vy;

	if(opt[0]){
		//残像処理
		if(viiSub::GameCounter()%1==0)free.w[0] -= 1;
		if(viiSub::Rand()%4==0) free.w[3] = 1-free.w[3];
		if(free.w[0] <= 0){
			exist_ = gxFalse;
		}else{
			viiDraw::Sprite(&spr_ene_bullet[10+free.w[3]],pos.x,pos.y,PRIO_ENE_BULLET,ATR_DFLT,ARGB(free.w[0],0xff,0xff,0xff),1.0f,rot);
			//viiDraw::Sprite(&spr_ene_bullet[10],pos.x,pos.y,PRIO_ENE_BULLET,ATR_ALPHA_PLUS,ARGB(free.w[0],0x00,0xff,0xff),1.5f,rot);
		}

		if(free.w[0] >= 64)
		{
//ZANTEI			m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
			pos.ax1 = -4;
			pos.ay1 = -4;
			pos.ax2 =  4;
			pos.ay2 =  4;
//ZANTEI			m_HitAtari.Set(ID_ENEMY_ATK,&pos);
	//		m_HitAtari.Activate();
		}
		return;
	}

//ZANTEI	if(viiSub::GetBoundBlock_Enemy(pos.x,pos.y) || m_HitAtari.IsImpacted())
	{
		viiEff::SetBombEffect(pos.x,pos.y,5,2);
		exist_ = gxFalse;
		return;
	}

	if(free.w[0] == 0){
		Sint32 r = get_valken_rot(pos.x,pos.y+2400);
		if(timer_ > 60*3)	r = rot;

		Sint32 rr = viiMath::GetTargetAngle(rot,r);
		if(rr >=  15) rr =  LASER_ROT_SPEED;
		if(rr <= -15) rr = -LASER_ROT_SPEED;
		if(free.w[1] == 0) {
			free.w[2] = rr / (LASER_ROT_TIME/2);
//			rot += rr;
		}else{
			free.w[1] --;
		}
		viiEff::Set(EFF_SPRINE_THUNDER,x_,y_,argset(1,rot));
		xx_ = viiMath::Cos100(rot)*12/LASER_ROT_TIME;
		yy_ = viiMath::Sin100(rot)*12/LASER_ROT_TIME;
		free.w[0] = LASER_ROT_TIME;

	}else{
		free.w[0] --;
	}

	rot += free.w[2];
	x_ += xx_;
	y_ += yy_;

	viiDraw::Sprite(&spr_ene_bullet[12],pos.x,pos.y,PRIO_ENE_BULLET,ATR_DFLT,ARGB(0x80,0xff,0xff,0xff),1.0f,rot);
//	viiDraw::Sprite(&spr_ene_bullet[10],x_,y_,PRIO_ENE_BULLET,ATR_ALPHA_PLUS,ARGB(0x80,0x00,0xff,0xff),1.5f,rot);

//ZANTEI	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
	pos.x = x_;
	pos.y = y_;
	pos.ax1 = -4;
	pos.ay1 = -4;
	pos.ax2 =  4;
	pos.ay2 =  4;
//ZANTEI	m_HitAtari.Set(ID_ENEMY_ATK,&pos);
//	m_HitAtari.Activate();

	if(viiSub::IsScrollOut(pos.x,pos.y,60*100,60*100)){
		exist_ = gxFalse;
	}
}


void CEffect::eneReflectBullet()
{
	//---------------------------------------------
	//反射弾
	//---------------------------------------------

	if (is_init()) {

//ZANTEI		m_HitAtari.set_ap(1);
//ZANTEI		m_HitAtari.set_hp(1);
		pos.ax1 = -4;
		pos.ay1 = -4;
		pos.ax2 =  4;
		pos.ay2 =  4;

		pos.rot = opt[0];
		pos.mx = (viiMath::Cos100(opt[0])*400)/100;
		pos.my = (viiMath::Sin100(opt[0])*400)/100;
		pos.x = x_;
		pos.y = y_;
	}


//ZANTEI	if(m_HitAtari.IsImpacted())
	{
		viiEff::SetBombEffect(pos.x,pos.y,1,2);
		exist_ = gxFalse;
		return;
	}

//ZANTEI	m_HitAtari.Set(ID_ENEMY_ATK,&pos);
//ZANTEI	m_HitAtari.Activate();

	pos.x += pos.mx;
	pos.y += pos.my;

	if(pos.x <= 10*100) {
		pos.mx *= -1;
	}

	viiDraw::Sprite(&spr_ene_bullet[13],pos.x,pos.y,PRIO_ENE_BULLET,ATR_DFLT,ARGB(0xff,0xff,0xff,0xff) );

	if(viiSub::IsScrollOut(pos.x,pos.y,60*100,60*100)){
		exist_ = gxFalse;
	}

}

void CEffect::eneAimingLaserBit()
{
	//---------------------------------------------------
	//LASET BIT(rot,dummy,zanzoflag)
	//---------------------------------------------------

	if (is_init()) {
//ZANTEI		m_HitAtari.set_ap(1);
//ZANTEI		m_HitAtari.set_hp(1);
		pos.ax1 = -4;
		pos.ay1 = -4;
		pos.ax2 =  4;
		pos.ay2 =  4;

		pos.rot = opt[0];
		pos.x = x_;
		pos.y = y_;
		free.w[0] = 0;
		free.w[1] = 0x80;
	}

	//---------------------------
	//残像分
	//---------------------------
	if(opt[2]) {
		free.w[1] -= 0x04;
		if(free.w[1] < 0) {
			exist_ = gxFalse;
			return;
		}
		viiDraw::Sprite(&spr_ene_bullet[14],pos.x,pos.y,PRIO_ENE_BULLET,ATR_ALPHA_PLUS,ARGB(free.w[1],0xff,0xff,0xff) ,1.0f,pos.rot);
		return;
	}

//ZANTEI	if(m_HitAtari.IsImpacted())
	{
		viiEff::SetBombEffect(pos.x,pos.y,1,2);
		exist_ = gxFalse;
		return;
	}

//ZANTEI	m_HitAtari.Set(ID_ENEMY_ATK,&pos);
//ZANTEI	m_HitAtari.Activate();

	Sint32 rot_spd;
	if(timer_ < 120) {
		rot_spd = 2;
		free.w[0] += 3;
		if(free.w[0] >= 100) free.w[0] = 50;
	}else{
		if(timer_ == 120) viiMus::PlaySound(SE_ENEBEAM);
		viiEff::Set(EFF_AIMINGLASERBIT,pos.x,pos.y,argset(pos.rot,0,1));
		rot_spd = 1;
		if(free.w[0] <= 800) free.w[0] += 20;
		if(timer_ >= 120) {
			if(viiSub::IsScrollOut(pos.x,pos.y,60*100,60*100)){
				exist_ = gxFalse;
				return;
			}
		}
	}

	Sint32 r = get_valken_rot(pos.x,pos.y);
	if(viiMath::GetTargetAngle(pos.rot,r) <= -10) pos.rot -= rot_spd;
	if(viiMath::GetTargetAngle(pos.rot,r) >=  10) pos.rot += rot_spd;

	pos.mx = (viiMath::Cos100(pos.rot)*free.w[0])/100;
	pos.my = (viiMath::Sin100(pos.rot)*free.w[0])/100;

	pos.x += pos.mx;
	pos.y += pos.my;

	viiDraw::Sprite(&spr_ene_bullet[14],pos.x,pos.y,PRIO_ENE_BULLET,ATR_ALPHA_PLUS,viiSub::GetRandColor(0xff) ,1.0f,pos.rot);

}


void CEffect::eneBigCannon()
{
	if (is_init())
	{
//ZANTEI		m_HitAtari.set_ap(1);
//ZANTEI		m_HitAtari.set_hp(1);
		pos.ax1 = -16;
		pos.ay1 = -16;
		pos.ax2 =  16;
		pos.ay2 =  16;

		pos.rot = opt[0];
		pos.x = x_;
		pos.y = y_;
		pos.sx = 100;
		free.w[0] = 0;
		free.w[1] = 0x80;
	}

	//---------------------------
	//残像分
	//---------------------------
	if(opt[2]) {
		free.w[1] -= 0x04;
		pos.sx +=4;
		if(free.w[1] < 0) {
			exist_ = gxFalse;
			return;
		}
		viiDraw::Sprite(&spr_ene_bullet[15],pos.x,pos.y,PRIO_ENE_BULLET,ATR_ALPHA_PLUS,ARGB(free.w[1],0xff,0xff,0xff) ,pos.sx/100.0f,pos.rot);
		return;
	}

//ZANTEI	if(m_HitAtari.IsImpacted())
	{
		viiEff::SetBombEffect(pos.x,pos.y,1,2);
		exist_ = gxFalse;
		return;
	}

//ZANTEI	m_HitAtari.Set(ID_ENEMY_ATK,&pos);
//ZANTEI	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
//ZANTEI	m_HitAtari.Activate();

	Sint32 rot_spd=0;
	if(timer_ <= 120) {
		if(free.w[0] <= 800) free.w[0] += 20;
		if(timer_ >= 120) {
			if(viiSub::IsScrollOut(pos.x,pos.y,60*100,60*100)){
				exist_ = gxFalse;
				return;
			}
		}
	}

//	Sint32 r = get_valken_rot(pos.x,pos.y);
//	if(viiMath::GetTargetAngle(pos.rot,r) <= -10) pos.rot -= rot_spd;
//	if(viiMath::GetTargetAngle(pos.rot,r) >=  10) pos.rot += rot_spd;

	pos.mx = (viiMath::Cos100(pos.rot)*free.w[0])/100;
	pos.my = (viiMath::Sin100(pos.rot)*free.w[0])/100;

	pos.x += pos.mx;
	pos.y += pos.my;

	if(pos.x <= 10*100) {
		viiEff::SetBombEffect(pos.x,pos.y,1,1);
		exist_ = gxFalse;
	}

	viiDraw::Sprite(&spr_ene_bullet[15],pos.x,pos.y,PRIO_ENE_BULLET,ATR_ALPHA_PLUS,ARGB(0xf0,0xff,0xff,0xff) ,1.0f,pos.rot);

}

void CEffect::eneGatringBit()
{
	//---------------------------------------------
	//ビットバルカン（EFF_GATRINGBIT）
	//---------------------------------------------

	if (is_init())
	{
//ZANTEI		m_HitAtari.set_ap(1);
//ZANTEI		m_HitAtari.set_hp(1);

		//味方も巻き添えにする
//ZANTEI		m_HitKurai.SetDirectAromorPosition( 50 );
//ZANTEI		m_HitKurai.set_hp(1);
		pos.ax1 = -4;
		pos.ay1 = -4;
		pos.ax2 =  4;
		pos.ay2 =  4;

		pos.rot = opt[0];
		pos.mx = (viiMath::Cos100(opt[0])*800)/100;
		pos.my = (viiMath::Sin100(opt[0])*800)/100;
		pos.x = x_;
		pos.y = y_;
	}


//ZANTEI	if(m_HitAtari.IsImpacted())
	{
		viiEff::Set(EFF_HITFLASH_ENE,pos.x,pos.y,argset(0));
		exist_ = gxFalse;
		return;
	}

//ZANTEI	m_HitAtari.Set(ID_ENEMY_ATK,&pos);
//ZANTEI	m_HitAtari.Activate();

	/*@HIT@*/
//ZANTEI	m_HitKurai.Set(ID_VERSIS_ATK,&pos);
//ZANTEI	m_HitKurai.Activate();
	/*@HIT@*/
	pos.x += pos.mx;
	pos.y += pos.my;

	if(pos.x <= 10*100 || pos.x >= 350*100) {
		viiEff::Set(EFF_HITFLASH_ENE,pos.x,pos.y,argset(0));
		exist_ = gxFalse;
		return;
	}

	viiDraw::Sprite(&spr_ene_bullet[14],pos.x,pos.y,PRIO_ENE_BULLET,ATR_ALPHA_PLUS,viiSub::GetRandColor(0xff) ,1.0f,pos.rot);

}


void CEffect::thexder()
{
	//-------------------------------------
	//テグザーレーザー
	//opt[0];	//角度
	//opt[1];	//長さ
	//opt[2];	//レベル
	//-------------------------------------
	Sint32 i;
	if (is_init()) {
		rot = opt[0];	//角度
	}
	if(timer_){
		viiEff::Set(EFF_LASERBOMB_PARTS,pos.x,pos.y,argset(0));
		exist_ = gxFalse;
		return;
	}

	Sint32 lv = opt[2];		//レベル
	Sint32 dist = opt[1]/1600;	//距離

	//レーザーのＵＶ
	gxSprite spr1 = {TPG_VALKEN,0,184,16,8,0,4};
	gxSprite spr2 = {TPG_VALKEN,0,184,16,8,0,4};
//	gxSprite spr3 = {TPG_VALKEN,16*2+8*4,160,8,8,4,4};	//レーザー先端

	Sint32 x,y;

	spr1.u = 0*32;
	spr2.u = 0*32;
	spr2.u += (15-(viiSub::GameCounter()%16));

	Sint32 col = ARGB(0x80,0xff,0xff,0xff);
	//色がえ
	Sint32 r = (viiSub::Rand()%2)*240;
	Sint32 g = (viiSub::Rand()%2)*240;
	Sint32 b = (viiSub::Rand()%2)*240;
	if(r+g+b==0) {
		r = g = b = 128;
	}
	col = ARGB(0x80,(r),(g),(b));

	Sint32 c=viiMath::Cos100(rot),s=viiMath::Sin100(rot);
	for(i=0;i<dist;i++) {

		x = x_ + c*16*i;
		y = y_ + s*16*i;

		if(viiSub::GetBoundBlock_Player( x,y )){
			//壁にあたっていればそれ以上行かない
			viiEff::Set(EFF_LASERBOMB_PARTS,x,y,argset(0));
			break;
		}

		viiDraw::Sprite(&spr2,x,y,PRIO_ATACK,ATR_ALPHA_PLUS,col,1.0f,rot);

		if(i==dist-1){
			pos.x = x+c*24;
			pos.y = y+s*24;
			pos.ax1 = -2;
			pos.ay1 = -2;
			pos.ax2 =  2;
			pos.ay2 =  2;
//ZANTEI			m_HitAtari.SetDirectAromorPosition( 15*lv );
//ZANTEI			m_HitAtari.Set(ID_VERSIS_ATK,&pos);
//ZANTEI			m_HitAtari.Activate();	//&hit
		}
	}

}


void CEffect::napalm()
{
	//-------------------------------------
	//ナパーム
	//-------------------------------------
	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		pos.rot = viiSub::Rand()%360;
		Sint32 spd = 500;
		pos.mx = (viiMath::Cos100(opt[0])*spd)/100;
		pos.my = (viiMath::Sin100(opt[0])*spd)/100;
		free.w[0] = 0;	//色
		if(opt[1] == 1){
			//帯
			pos.mx = pos.my = 0;
			free.w[0] = 128+viiSub::Rand()%100;
		}
	}

	//α値減らす
	if(free.w[0] > 0){
		//帯のとき専用処理
		free.w[0] -= 8;
		if(free.w[0] <= 0){
			free.w[0] = 0;
			exist_ = gxFalse;
		}
		viiDraw::Sprite(&spr_bomb[0],pos.x,pos.y,PRIO_EFFECT,ATR_ALPHA_PLUS ,ARGB(free.w[0],0xff,0xff,0xff),1.0f, pos.rot);
		return;
	}

	if(pGame->is_gravit()){
		//重力下
		pos.my += -2+viiSub::Rand()%5;
		pos.my += 5;
	}

	if(viiSub::GetBoundBlock_Player(pos.x,pos.y)){
		//壁ヒットカウント調査
		viiEff::SetBombEffect(pos.x,pos.y,5,5);
		exist_=gxFalse;
		return;
	}

//ZANTEI	if(m_HitAtari.IsImpacted())
	{
		viiEff::SetBombEffect(pos.x,pos.y,5,5);
		exist_=gxFalse;
		return;
	}

	pos.x += pos.mx;
	pos.y += pos.my;

	if(viiSub::IsScrollOut(x_,y_,32*100,32*100)){
		exist_ = gxFalse;
		return;
	}

	pos.rot += 8;

	viiDraw::Sprite(&spr_bomb[0],pos.x,pos.y,PRIO_EFFECT,ATR_ALPHA_PLUS ,0xffffffff,0.8f,0);
	viiDraw::Sprite(&spr_bomb[0],pos.x,pos.y,PRIO_EFFECT,ATR_ALPHA_PLUS ,0x80ffffff,1.0f,-pos.rot);
	viiDraw::Sprite(&spr_bomb[0],pos.x,pos.y,PRIO_EFFECT,ATR_ALPHA_PLUS ,0x80ffffff,1.2f, pos.rot);

	pos.ax1 = -16;
	pos.ay1 = -16;
	pos.ax2 =  16;
	pos.ay2 =  16;
//ZANTEI	m_HitAtari.set_ap(50);

//ZANTEI	m_HitAtari.Set(ID_VERSIS_ATK,&pos);
//ZANTEI	m_HitAtari.Activate();	//&hit

	if(viiSub::GameCounter()%2==0){
		viiEff::Set(EFF_NAPALM,pos.x,pos.y,argset(0,1));
	}
}

void CEffect::ley_napalm()
{
	//-------------------------------------
	//火炎放射器
	//-------------------------------------

	if (is_init()) {

		rot = viiSub::Rand()%360;
		xx_ = (viiMath::Cos100(opt[0])*opt[1])/100;
		yy_ = (viiMath::Sin100(opt[0])*opt[1])/100;
		free.w[0] = 255;
		free.w[1] = 50+viiSub::Rand()%20;
		if(viiSub::Rand()%2==0){
			free.w[2] = 2;
		}else{
			free.w[2] = -2;
		}
	}

	if(free.w[0] > 0) {
		free.w[0] -= 4;
		free.w[1] += 1;
		if(opt[2]==1){
			if(free.w[1] >= 30) free.w[1] = 30;	//残りかすは大きくしない
		}
		if(opt[2]==2){
			if(free.w[1] >= 10) free.w[1] = 10;	//残りかすは大きくしない
		}
	}

	if(free.w[0] <=0){
		//透過度ゼロパーセントで消滅
		exist_ = gxFalse;
		return;
	}

	Sint32 col   = free.w[0];
	float scl = free.w[1]/100.0f;

	rot += free.w[2];
	if(col >= 255) col = 255;

	yy_ -= 1;
	xx_ -= xx_/50;

	x_ += xx_;
	y_ += yy_;

	Sint32 x,y;
	x = x_;
	y = y_;

	viiDraw::Sprite(&spr_bomb[0],x,y,PRIO_EFFECT,ATR_ALPHA_PLUS,ARGB(col,0xff,0xff,0xff),scl*0.7f,-rot);

	OBJ_POS_T ataripos;
	ataripos.x = x_;
	ataripos.y = y_;
	ataripos.ax1 = (Sint32)(-12*scl);
	ataripos.ay1 = (Sint32)(-12*scl);
	ataripos.ax2 = (Sint32)(12*scl);
	ataripos.ay2 = (Sint32)(12*scl);

//ZANTEI	m_HitAtari.SetDirectAromorPosition( (Sint32)(5+1*scl) );
//ZANTEI	m_HitAtari.Set(ID_VERSIS_ATK,&ataripos);//,&hit
//ZANTEI	m_HitAtari.Activate();	//&hit

}



void CEffect::grenade()
{
	//グレネード弾

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		pos.rot = viiSub::Rand()%360;
		pos.sx = 100;
		free.w[0] = 8+viiSub::Rand()%8;		//回転速度
		free.w[1] = 3+viiSub::Rand()%2;		//ぶつかりカウンター
		free.w[3] = 0;				//爆風タイム
		if(viiSub::Rand()%2==0) free.w[0] *= -1;
		Sint32 spd = opt[1]+viiSub::Rand()%100;
		pos.mx = (viiMath::Cos100(opt[0])*spd)/100;
		pos.my = (viiMath::Sin100(opt[0])*spd)/100;
		yy_ = 16;
		//opt[2]	//ダメージ
	}

	gxBool hit_kabe = gxFalse;
	if(pGame->is_gravit()){
		//重力下
		yy_ += 16;
		if(viiSub::GetBoundBlock_Player(pos.x,pos.y)){
			hit_kabe = gxTrue;
			yy_ = (yy_+pos.my)*-1;
			yy_ = 1*yy_/5;
			pos.y += pos.my+yy_;
			pos.my = 0;
			pos.mx = 1*pos.mx/5;
			free.w[0] *= -1;
		}
	}else{
		if(viiSub::GetBoundBlock_Enemy(pos.x+pos.mx,pos.y+pos.my)){
			hit_kabe = gxTrue;
			free.w[1] = 1;
		}
	}

	gxBool bomb = gxFalse;
//ZANTEI	if(m_HitAtari.IsImpacted())  bomb = gxTrue;

	if(hit_kabe){
		//壁ヒットカウント調査
		free.w[1]--;
		if(free.w[1]<=0){
			bomb = gxTrue;
		}
	}

	if(free.w[3]) bomb = gxTrue;

	if(bomb){
		if(free.w[3] == 0){
			viiMus::PlaySound(SE_BOMB_SMALL);
			viiEff::SetBombEffect(pos.x,pos.y,5,3);
		}
		free.w[3] ++;
		if(free.w[3] >= 10) exist_=gxFalse;

		viiEff::Set(EFF_GRENADE_EFFECT,pos.x,pos.y,
				argset(
					10,//１０フレーム
					-40,
					-40,
					 40,
					 40,
					 opt[2])//ダメージ
					 );
		return;
	}

	Sint32 mx,my;
	mx = pos.mx;
	my = pos.my+yy_;
	//viiSub::SetGravitMove(mx,my);

	pos.x += mx;
	pos.y += my;


	if(viiSub::IsScrollOut(x_,y_,8*100,8*100)){
		exist_ = gxFalse;
		return;
	}

	pos.rot += free.w[0];

	viiDraw::Sprite(&spr_ene_bullet[1],pos.x,pos.y,PRIO_EFFECT,ATR_DEFAULT,ARGB_DFLT,1.0f,pos.rot);

	pos.ax1 = -4;
	pos.ay1 = -4;
	pos.ax2 =  4;
	pos.ay2 =  4;
//ZANTEI	m_HitAtari.set_ap(1);

//ZANTEI	m_HitAtari.Set(ID_VERSIS_ATK,&pos);	//,&hit
//ZANTEI	m_HitAtari.Activate();	//&hit

}

void CEffect::grenade_effect()
{
	//グレネード爆発エフェクト
	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		
	}

	Sint32 time = opt[0];	//持続時間
	pos.ax1 =   opt[1];	//当たり判定
	pos.ay1 =   opt[2];
	pos.ax2 =   opt[3];
	pos.ay2 =   opt[4];
//ZANTEI	m_HitAtari.SetDirectAromorPosition( opt[5] );	//ダメージ

//ZANTEI	m_HitAtari.set_hp(10);
//ZANTEI	m_HitAtari.Set(ID_VERSIS_ATK,&pos);	//,&hit
//ZANTEI	m_HitAtari.Activate();	//&hit

	if(timer_ > time){
		exist_ = gxFalse;
	}
}


void CEffect::defence_discharger()
{
	//Defence DisChager

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		pos.rot = viiSub::Rand()%360;
		pos.sx = 100;
		free.w[0] = 8+viiSub::Rand()%8;		//回転速度
		free.w[1] = 3+viiSub::Rand()%2;		//ぶつかりカウンター
		if(viiSub::Rand()%2==0) free.w[0] *= -1;
		Sint32 spd = opt[1];
		pos.mx = (viiMath::Cos100(opt[0])*spd)/100;
		pos.my = (viiMath::Sin100(opt[0])*spd)/100;
		yy_ = 16;
	}
	Sint32 lv = opt[2];

	gxBool hit_kabe = gxFalse;
	if(pGame->is_gravit()){
		//重力下
		yy_ += 8;
		if(viiSub::GetBoundBlock_Player(pos.x,pos.y)){
			hit_kabe = gxTrue;
		}
	}else{
		if(viiSub::GetBoundBlock_Enemy(pos.x+pos.mx,pos.y+pos.my)){
			hit_kabe = gxTrue;
		}
	}

	if(hit_kabe || timer_ >= 60){
		//壁ヒットカウント調査
		for(Sint32 i=0;i<20+lv*5;i++) {
			viiEff::Set(EFF_D_D2,pos.x,pos.y,argset(lv));
		}
		viiMus::PlaySound(SE_KUSUDAMA2);
		viiEff::SetBombDeluxe(pos.x,pos.y,3,3);
		exist_=gxFalse;
		return;
	}


	Sint32 mx,my;
	mx = pos.mx;
	my = pos.my+yy_;

//	viiSub::SetGravitMove(mx,my);

	pos.x += mx;
	pos.y += my;


	if(viiSub::IsScrollOut(x_,y_,8*100,8*100)){
		exist_ = gxFalse;
		return;
	}

	pos.rot += free.w[0];

	viiDraw::Sprite(&spr_custom[6],pos.x,pos.y,PRIO_EFFECT,ATR_DEFAULT,ARGB_DFLT,1.0f,pos.rot);
}

void CEffect::defence_discharger2()
{
	//D-D破裂後
	Sint32 lv;

	if (is_init()) {
		lv = opt[0];
		Sint32 spd = 10+viiSub::Rand()%(10+lv*5);	//レベル上がるほど横に広がる
		pos.x = x_;
		pos.y = y_;
		pos.rot = viiSub::Rand()%360;
		pos.mx = (viiMath::Cos100(pos.rot)*spd)/50;
		pos.my = (viiMath::Sin100(pos.rot)*spd)/100;
		free.w[0] = (lv*30) + 90+viiSub::Rand()%60;	//残存タイマー
		free.w[1] = 1+viiSub::Rand()%2;
		free.w[2] = viiSub::Rand()%3;
	}

	lv = opt[0];

	if(timer_%4==0){
		if(pGame->is_gravit())	pos.my += free.w[1];	//重力下
	}
	pos.x += pos.mx;
	pos.y += pos.my;
//	viiSub::SetGravitMove(pos.x,pos.y);

	if(viiSub::Rand()%4==0){
		free.w[2] ++;
		free.w[2] = free.w[2]%3;
	}

	viiDraw::Sprite(&spr_custom[ 7 + free.w[2]],pos.x,pos.y,PRIO_EFFECT,ATR_DEFAULT,ARGB_DFLT,1.0f,pos.rot);

	pos.ax1 = pos.kx1 =   -4;
	pos.ay1 = pos.ky1 =   -4;
	pos.ax2 = pos.kx2 =   +4;
	pos.ay2 = pos.ky2 =   +4;
//ZANTEI	m_HitAtari.set_hp(1);
//ZANTEI	m_HitAtari.SetDirectAromorPosition( lv+1 );
//ZANTEI	m_HitAtari.Set(ID_VERSIS_ATK,&pos);	//,&hit
//ZANTEI	m_HitAtari.Activate();	//&hit

//ZANTEI	m_HitKurai.set_hp(1);
//ZANTEI	m_HitKurai.Set(ID_VERSIS_DEF,&pos);	//&kurai,
//ZANTEI	m_HitKurai.Activate();	//&kurai

	free.w[0]--;
	if(free.w[0] < 0){
		exist_ = gxFalse;
	}
	if(viiSub::IsScrollOut(x_,y_,8*100,8*100)){
		exist_ = gxFalse;
		return;
	}

}

#define MMISSILE_ROT_SPEED (15)
#define MMISSILE_ROT_TIME  (4)

void CEffect::MicroMissile()
{
	//----------------------------------
	//マイクロミサイル
	//----------------------------------

	if (is_init()) {
		rot = opt[1];
		pos.x = x_;
		pos.y = y_;
		xx_ = viiMath::Cos100(rot)*(12+viiSub::Rand()%5)/MMISSILE_ROT_TIME;
		yy_ = viiMath::Sin100(rot)*(12+viiSub::Rand()%5)/MMISSILE_ROT_TIME;
		free.w[0] = 0;
		free.w[1] = 5+viiSub::Rand()%10;
		free.w[2] = 0;
		if(opt[0]){
			//消える速度
			xx_ = yy_ = 0;
			free.w[0]=128;
			rot = opt[1];
		}

//ZANTEI		m_HitAtari.set_ap(5);
//ZANTEI		m_HitAtari.set_hp(1);

		free.w[3] = 1+viiSub::Rand()%15;//20+viiSub::Rand()%20;
		free.w[4] = 0;
	}

	if(opt[0]){
		//残像処理
		free.w[0] -= 1;
		if(free.w[0] <= 0){
			exist_ = gxFalse;
		}else{
			viiDraw::Sprite(&spr_ene_bullet[9],pos.x,pos.y,PRIO_EFFECT,ATR_DFLT,ARGB(free.w[0],0xff,0xff,0xff),1.0f,rot);
		}
		return;
	}

//	if(viiSub::GetBoundBlock_Enemy(x_,y_) || m_HitAtari.IsImpacted()){
//ZANTEI	if(m_HitAtari.IsImpacted())
	{
		viiMus::PlaySound(SE_BOMB_SMALL);
		viiEff::SetBombEffect(x_,y_,5,2);
		exist_ = gxFalse;
		return;
	}

	if(free.w[3] > 0){
		free.w[3] --;
//		xx_ = 90*xx_/100;
//		yy_ = 90*yy_/100;
		if(free.w[3] == 0){
			viiEff::Set(EFF_MAZZLE,pos.x,pos.y,NULL);
		}
		return;
	}else if(free.w[0] == 0){
		Sint32 r = rot,px=0,py=0;
		if( get_nearly_enemypos(x_/100,y_/100,px,py) ) {
			r = viiMath::Atan2_100(py*100-y_,px*100-x_);
		}

		Sint32 rr = viiMath::GetTargetAngle(rot,r/100);
		if(rr >=  15) rr =  MMISSILE_ROT_SPEED;
		if(rr <= -15) rr = -MMISSILE_ROT_SPEED;
		pos.x = x_;
		pos.y = y_;
		if(free.w[1] == 0) {
			free.w[2] = rr / MMISSILE_ROT_TIME;
			free.w[1] = viiSub::Rand()%3;
		}else{
			free.w[1] --;
		}
		free.w[4] ++;
		if(	free.w[4] > 2) viiEff::Set(EFF_MICROMISSILE,pos.x,pos.y,argset(1,rot));
		xx_ = viiMath::Cos100(rot)*12/MMISSILE_ROT_TIME;
		yy_ = viiMath::Sin100(rot)*12/MMISSILE_ROT_TIME;
		free.w[0] = MMISSILE_ROT_TIME;

	}else{
		free.w[0] --;
	}

	rot += free.w[2];
	x_ += xx_;
	y_ += yy_;

//	viiDraw::Sprite(&spr_ene_bullet[9],x_,y_,PRIO_EFFECT,ATR_DFLT,ARGB(0x80,0xff,0xff,0xff),1.0f,rot);
	viiDraw::Sprite(&spr_custom[SPR_MISSILE]       ,x_,y_,PRIO_ATACK,ATR_DEFAULT,ARGB_DFLT,1.0f,rot);
	viiDraw::Sprite(&spr_custom[SPR_MISSILE_BURNIA],x_,y_,PRIO_ATACK,ATR_DEFAULT,ARGB_DFLT,1.0f,rot);

//ZANTEI	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
	pos.x = x_;
	pos.y = y_;
	pos.ax1 = -4;
	pos.ay1 = -4;
	pos.ax2 =  4;
	pos.ay2 =  4;
//ZANTEI	m_HitAtari.Set(ID_VERSIS_ATK,&pos);//,&hit
//ZANTEI	m_HitAtari.Activate();	//&hit


	if(viiSub::IsScrollOut(x_,y_,120*100,120*100)){
		exist_ = gxFalse;
	}

}

void CEffect::ShortLaser()
{
	//---------------------
	//短いレーザー弾
	//---------------------

	if (is_init()) {
		rot = opt[0];		//初期方向
		xx_ = (viiMath::Cos100(rot)*opt[1])/100;
		yy_ = (viiMath::Sin100(rot)*opt[1])/100;

//ZANTEI		m_HitAtari.SetDirectAromorPosition( 80 );
//ZANTEI		m_HitAtari.set_hp(1);
		if(opt[2] == 1){
			free.w[0] = 128;
		}
	}

	if(opt[2]){
		if(free.w[0] > 0){
			free.w[0] -= 8;
			if(free.w[0] < 0) free.w[0] = 0;
			viiDraw::Sprite(&spr_custom[SPR_SHORTLASER],x_,y_,PRIO_ENE_BULLET,ATR_DEFAULT,ARGB(free.w[0],0xff,0xff,0xff),1.0f,rot);
		}else{
			exist_ = gxFalse;
		}
		return;
	}


	Sint32 lv = 0;

	//存在チェック
	if(viiSub::GetBoundBlock_Player(x_,y_) ){
		exist_ = gxFalse;
		_opt_[0] = 0;
		viiEff::Set(EFF_HITFLASH_ENE,x_,y_, _opt_);
		return;
	}

	if(viiSub::IsScrollOut(x_,y_,0,0)){
		exist_ = gxFalse;
		return;
	}

	x_ += xx_;
	y_ += yy_;

	OBJ_POS_T ataripos;
	ataripos.x = x_;
	ataripos.y = y_;
	ataripos.ax1 = -4+(-lv*2);	//レベルに応じてあたり判定を大きくする
	ataripos.ay1 = -4+(-lv*2);
	ataripos.ax2 =  4+(+lv*2);
	ataripos.ay2 =  4+(+lv*2);

//ZANTEI	m_HitAtari.Set(ID_VERSIS_ATK,&ataripos);
//ZANTEI	m_HitAtari.Activate();	//&hit

	viiDraw::Sprite(&spr_custom[SPR_SHORTLASER],x_,y_,PRIO_ENE_BULLET,ATR_DEFAULT,ARGB_DFLT,1.0f,rot);

	if(timer_%2==0) {
		viiEff::Set(EFF_SHORTLASER,x_,y_,argset(rot,0,1));
	}

}

void CEffect::melonbomb()
{
	//---------------------
	//メロン爆弾
	//---------------------

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		pos.rot = viiSub::Rand()%360;
		pos.sx = 100;
		free.w[0] = 8+viiSub::Rand()%8;		//回転速度
		free.w[1] = 3+viiSub::Rand()%2;		//ぶつかりカウンター
		free.w[3] = 0;				//爆風タイム
		free.w[4] = 120;			//時限カウンター
		if(viiSub::Rand()%2==0) free.w[0] *= -1;
//		Sint32 spd = opt[1]+viiSub::Rand()%100;
		pos.mx = 0;
		pos.my = 0;
		yy_ = 16;
		//opt[2]	//ダメージ
	}

	gxBool bomb = gxFalse;
//ZANTEI	if(m_HitAtari.IsImpacted())		bomb = gxTrue;
	if(free.w[4]>0)
	{
		free.w[4] --;
	}

	if(free.w[4]==0)
	{
		bomb = gxTrue;
	}

	if(bomb){
		if(free.w[3] == 0){
			viiMus::PlaySound(SE_BOMB_SMALL);
			viiEff::SetBombEffect(pos.x,pos.y,5,3);
		}
		free.w[3] ++;
		if(free.w[3] >= 10){
			exist_=gxFalse;
			return;
		}

		viiEff::Set(EFF_GRENADE_EFFECT,pos.x,pos.y,
				argset(
					10,//１０フレーム
					-40,
					-40,
					 40,
					 40,
					 opt[2])//ダメージ
					 );
		return;
	}

	Sint32 mx,my;
	mx = pos.mx;
	my = pos.my;

	pos.x += mx;
	pos.y += my;

	if(viiSub::IsScrollOut(x_,y_,8*100,8*100)){
		exist_ = gxFalse;
		return;
	}

	pos.rot += free.w[0];

	viiDraw::Sprite(&spr_ene_bullet[1],pos.x,pos.y,PRIO_EFFECT,ATR_DEFAULT,ARGB_DFLT,1.0f,pos.rot);

	pos.kx1 = pos.ax1 = -4;
	pos.ky1 = pos.ay1 = -4;
	pos.kx2 = pos.ax2 =  4;
	pos.ky2 = pos.ay2 =  4;
//ZANTEI	m_HitAtari.set_ap(1);

//ZANTEI	m_HitAtari.Set(ID_VERSIS_ATK,&pos);
//ZANTEI	m_HitAtari.Activate();	//&hit

}


void CEffect::heathawk()
{
	//-------------------------------------------
	//ヒートホーク（エネルギーカッター） (方向,速度,大きさ(%))
	//-------------------------------------------
	Sint32 sDirection = DIR_RIGHT;

	if (is_init()) {
		pos.x = x_;
		pos.y = y_;
		viiSub::GetScroll_s( x_,y_ );
		x_ *= 100;
		y_ *= 100;
		x_ = GetTargetPlayer()->x;
		y_ = GetTargetPlayer()->y;

		pos.x -= x_;
		pos.y -= y_;
		pos.mx = 3200;
		pos.my = 0;
		pos.rot = -180;
		pos.sx = opt[2];			//大きさ
		free.w[0] = 255;	//色合い
		free.w[1] = 0;		//一度だけあたったフラグ
	}

	if(opt[0]<0) sDirection = DIR_LEFT;

	//----------------------------------------
	//移動
	//----------------------------------------
	pos.x = (viiMath::Cos100(pos.rot)*(pos.mx*opt[0]))/100;;
	pos.y = (viiMath::Sin100(pos.rot)*(pos.mx*1 ))/100-2400;
	pos.rot += 20;

	//----------------------------------------
	//色
	//----------------------------------------
	Sint32 r = (viiSub::Rand()%2)*240;
	Sint32 g = (viiSub::Rand()%2)*240;
	Sint32 b = (viiSub::Rand()%2)*240;
	if(r+g+b==0) {
		r = g = b = 128;
	}

	Sint32 col;

	if(free.w[0] > 0){
		pos.mx = 98*pos.mx/100;
		free.w[0] -= 16;
//		pos.sx += 1;
		if(free.w[0] <= 0){
			free.w[0] = 0;
			exist_ = gxFalse;
			return;
		}
	}
	col = free.w[0];

	float scl = (pos.sx) / 70.0f;

	//----------------------------------------
	//GVにあわせて動く
	//----------------------------------------
	Sint32 x,y;
#if PUNCH_MOVE_WITH_SCROLL
	viiSub::GetScroll_s( x_,y_ );
	x  = x_*100 + pos.x;
	y  = y_*100 + pos.y;
	x = GetTargetPlayer()->x+pos.x;
	y = GetTargetPlayer()->y+pos.y;

#else
	x  = pos.x;
	y  = pos.y;
#endif


	Sint32 atr = ATR_DFLT;
	if( sDirection == DIR_LEFT ) atr |= ATR_FLIP_X;

	viiDraw::Sprite(&spr_custom[4],x,y,PRIO_ATACK,atr,ARGB((col/2),r,g,b),scl,pos.rot);
	viiDraw::Sprite(&spr_custom[4],x,y,PRIO_ATACK,atr|ATR_ALPHA_PLUS,ARGB((col/2),r,g,b),scl,pos.rot-8);
	viiDraw::Sprite(&spr_custom[4],x,y,PRIO_ATACK,atr,ARGB((col/2),r,g,b),scl,pos.rot-16);

//ZANTEI	if(m_HitAtari.IsImpacted() && free.w[1] == 0)
	{
		//----------------------------------------
		//あたっていれば一度だけ破片を出す
		//----------------------------------------
		Sint32 rot = 0;
		if(pos.mx > 0) rot += 180;
		viiEff::Set(EFF_BROKENPARTS,x, y,argset(-45+90*0+rot,300,1));
		viiEff::Set(EFF_BROKENPARTS,x, y,argset( 45+90*0+rot,300,1));
		free.w[1] = 1;
	}

	//----------------------------------------
	//当たり判定登録
	//----------------------------------------
	OBJ_POS_T ataripos;
	ataripos.x = x;
	ataripos.y = y;
/*
	ataripos.ax1 = opt[0]*pos.sx/100;	//レベルに応じてあたり判定を大きくする
	ataripos.ax2 = opt[0]*16*pos.sx/100;
	ataripos.ay1 =  -8-10*pos.sx/100;
	ataripos.ay2 =  20;
//ZANTEI	m_HitAtari.SetDirectAromorPosition( 1+pos.sx );
//ZANTEI	m_HitAtari.SetAtackType( ATK_TYPE_CANNON );
//ZANTEI	m_HitAtari.Set(ID_VERSIS_ATK,&ataripos);//,&hit
*/

//ZANTEI	m_HitAtari.ax1 = sDirection*pos.sx/100;	//レベルに応じてあたり判定を大きくする
//ZANTEI	m_HitAtari.ax2 = sDirection*20*pos.sx/100;
//ZANTEI	m_HitAtari.ay1 = -8-10*pos.sx/100;
//ZANTEI	m_HitAtari.ay2 = 20;
//ZANTEI	m_HitAtari.SetDirectAromorPosition( 1+pos.sx );
//ZANTEI	m_HitAtari.SetHantei(ID_VERSIS_ATK,&ataripos);//,&hit

	//----------------------------------------
	//壁への当たり判定
	//----------------------------------------
	switch(pGame->pBg->get_kabetype( x/100,y/100)){
	case 1:
	case 12:
	case 13:
	case 14:
	case 15:
		_opt_[0] = 0;	//角度ゼロ
		_opt_[1] = 0;	//スピードゼロ
//		viiEff::Set(EFF_BOMB_PARTS,x,y,o);
//		exist_ = gxFalse;
		break;
	}
}



gxBool get_nearly_enemypos(Sint32 x,Sint32 y,Sint32 &px,Sint32 &py)
{
	//-------------------------------------
	//ロックオンロジック
	//最も近い敵の座標を得る
	//該当の敵がいない場合にはgxFalseを返す
	//-------------------------------------
	double kyori,min_dist = SCR_W*SCR_H;
	Sint32 min=-1;
	Sint32 x1,y1;

	//座標は1/100サイズ

	for(Sint32 i=0;i<pGame->pHitManager->GetOldMax();i++) {

		if(pGame->pHitManager->GetOldID(i) == ID_ENEMY_DEF){
			x1 = pGame->pHitManager->GetOldX(i);
			y1 = pGame->pHitManager->GetOldY(i);
			kyori = viiMath::Dist( x1-x , y1-y );
			if(kyori < min_dist){
				if(!viiSub::IsScrollOut(x1*100,y1*100,0,0)){
					//画面範囲外のヤツは相手にしない
					min_dist = kyori;
					min = i;
				}
			}
		}
	}

	if(min==-1) return gxFalse;	//見つからなかった

	px = pGame->pHitManager->GetOldX(min);
	py = pGame->pHitManager->GetOldY(min);

	return gxTrue;
}

