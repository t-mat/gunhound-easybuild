//-------------------------------
//壁とのあたり判定オブジェクト
//-------------------------------

#include <gunvalken.h>

CObj::CObj()
{
	m_bNoCheckHalfWall = gxFalse;
	land_cnt = 0;
	top_cnt = 0;
	m_bAtariOff = gxFalse;
}

CObj::~CObj()
{
	
}

void CObj::disp_debug(Sint32 n)
{
	//デバッグ表示
	if(!DEBUG_ATARIMAP) return;

	Sint32 x1 = pos.x + atari1.x;
	Sint32 y1 = pos.y + atari1.y;
	Sint32 x2 = pos.x + atari2.x;
	Sint32 y2 = pos.y + atari2.y;

	Sint32 sx,sy;
	viiSub::GetScroll_s( sx,sy );

	x1 = x1/100-sx;
	y1 = y1/100-sy;
	x2 = x2/100-sx;
	y2 = y2/100-sy;

	viiDraw::Box(x1,y1,x2,y2,PRIO_DEBUG,gxTrue,n,ATR_DFLT);
}


//----------------------------------------------------------
//----------------------------------------------------------
//あたり判定第６弾
//----------------------------------------------------------
//----------------------------------------------------------
//　●　　←先端
//　┃　
//　┃　　←ミドル
//　┃　
//　●　　←終端

#define TILESIZE (8)

Sint32 CObj::adjust_move(Sint32 mode)
{
	if( m_bAtariOff ) return gxTrue;

	move_mode_ = mode;
	switch(move_mode_){
	case 0:
		KyoyouRange = 8;	//１ブロック分
		collision_check();
		break;
	case 1:
		KyoyouRange = 8;	//１ブロック分
		collision_check();
		break;
	case 2:
		KyoyouRange = 32;	//４ブロック分
		collision_check();
		break;
	}

	m_bNoCheckHalfWall = gxFalse;

	return gxTrue;
}


Sint32 CObj::is_hit(Sint32 x,Sint32 y)
{
	//グローバル座標から壁のタイプを得る

	Sint32 type = pGame->pBg->get_kabetype(x/100,y/100);

	switch(type){
	case 1:
	case 2:
	case 3:
	case 12:
	case 13:
	case 14:
	case 15:
		return type;
	}
	return 0;
}

gxBool CObj::is_landnow()
{
	//現在着地中か調査
	Sint32 ax,ay,type;

	ax = pos.x/100;
	ay = pos.y/100;	
	type = pGame->pBg->get_kabetype(ax,ay+2);	//必ず２ドット下を調べる（念のため２ドット）

	Sint32 mx=ax,my=ay;
	viiSub::GetScreenPosition(mx,my);
	if(DEBUG_ATARIMAP){
		viiDraw::Box(mx-1,my-1,mx+1,my+1,PRIO_DEBUG,gxTrue,0xffffff00,ATR_DFLT);
	}
	switch(type){
	case 2:
		if(mov.y<0) return gxFalse;
		return gxTrue;
	case 1:
	case 3:
	case 12:
	case 13:
	case 14:
	case 15:
		return gxTrue;
	}
	return gxFalse;
}


Sint32 CObj::get_tile_xy(Sint32 ax,Sint32 ay)
{
	//グローバル座標からタイルパターン番号を取り出す

	ax = ax/(TILESIZE*100);
	ay = ay/(TILESIZE*100);

	return get_tile(ax,ay);
}


Sint32 CObj::get_tile(Sint32 ax,Sint32 ay)
{
	//タイル座標からタイルパターン番号を割り出す

	Sint32 type = pGame->pBg->get_kabetype8(ax,ay);

	switch(type){
	case 1:
	case 3:
	case 12:
	case 13:
	case 14:
	case 15:
		return type;
	}
	return 0;
}


//---------------------------------------------
//第９弾　当たり判定
//当たり判定の優先順位を設けて、引っかかるまで
//判定する。最後のほうまで引っかからなければ
//かなり精度の高い判定が可能。二重に判定して
//矛盾させない当たり判定方法。
//床下チェックで引っかかった場合には
//---------------------------------------------
/*
　　②　　
　　　　　
③　　　④
　　　　　
　　①　　
*/

//#define ATARI_DEBUG_

void CObj::collision_check()
{
	//矩形による衝突チェック
	Sint32 w = 20*100;
	Sint32 h = 40*100;
	Sint32 x,y,x1,y1,x2,y2;
	gxBool adjust = gxFalse;
	gxBool y_adjust = gxFalse;	//Ｙ座標補正フラグ
	w_ = w;
	h_ = h;

	//----------------------------------------------------------------
	//まずは全体がオブジェクトにあたっていなければなにも判定しない
	//----------------------------------------------------------------

	//移動させてみる
	x = pos.x + mov.x;
	y = pos.y + mov.y;
	x1 = x-w/2;
	y1 = y-h;
	x2 = x+w/2;
	y2 = y;
	if(!check_allblock(x1,y1,x2,y2)) return;
	viiDbg::printf(200,100,"全体衝突あり");

	if(check_side(pos.x,pos.y,mov.x,mov.y)) {
		adjust = gxTrue;
	}

/*
	if(is_hit(x,y)){
		//動かす前から床下でぶつかっていれば
		//補正しない、っていうか判断不能
		return;
	}
*/
	//------------------------------------------------
	//Ｙの補正
	//------------------------------------------------

	adjust = gxFalse;

	//床下チェック(半床はここでのみチェックする)
	if(check_under(pos.x,pos.y,mov.x,mov.y))
	{

#ifdef ATARI_DEBUG_
		viiDbg::printf(200,110,"床の補正をした。Ｙ座標をいじった");
#endif
		y_adjust = gxTrue;	//コレ以降Ｙの座標はいじらない
	}

	//天井チェック
	if(y_adjust == gxFalse)
	{
		//Ｙ座標補正がなければ天井のチェックをして補正する
		if(check_top(pos.x,pos.y-h,mov.x,mov.y))
		{

#ifdef ATARI_DEBUG_
		viiDbg::printf(200,120,"天井の補正をした。Ｙ座標をいじった");
#endif
			y_adjust = gxTrue;	//コレ以降Ｙの座標はいじらない
		}
	}

	//------------------------------------------------
	//Ｘの補正
	//------------------------------------------------

	if(check_side(pos.x,pos.y,mov.x,mov.y)) {
		adjust = gxTrue;
	}
}

gxBool CObj::check_allblock(Sint32 x1,Sint32 y1,Sint32 x2,Sint32 y2)
{
	//-----------------------------------------------
	//矩形がブロックにぶつかっているかチェックする
	//-----------------------------------------------
	Sint32 tile;

	x1 = x1/800;
	y1 = y1/800;
	x2 = x2/800;
	y2 = y2/800;
	y2 ++;
	x2 ++;

#if 0//ATARI_DEBUG_
	Sint32 ax1,ay1,ax2,ay2;
	ax1 = x1*8;
	ay1 = y1*8;
	ax2 = x2*8;
	ay2 = y2*8;
	viiSub::GetScreenPosition(ax1,ay1);
	viiSub::GetScreenPosition(ax2,ay2);
	viiDraw::Box(ax1,ay1,ax2,ay2,PRIO_DEBUG_TIP,gxTrue,ARGB(0x80,0xff,0xff,0x00));
#endif

	for(Sint32 y=y1; y<=y2;y++){
		for(Sint32 x=x1; x<=x2;x++){
			//すべてのタイルをチェック
			tile = pGame->pBg->get_kabetype8(x,y);
			switch(tile){
			case 1:
			case 3:
			case 12:
			case 13:
			case 14:
			case 15:
				return gxTrue;
				break;
			case 2:	//半壁はくだりのみ判定する
				if(mov.y>=0)	return gxTrue;
				break;
			}

		}
	}
	return gxFalse;
}



gxBool CObj::check_under(Sint32 x,Sint32 y,Sint32 &mx,Sint32 &my)
{
	//-----------------------------------------------
	//足元のスグ下の１ドットを検査
	//Ｙ座標のみ補正する
	//-----------------------------------------------

	switch(pGame->pBg->get_kabetype8(x/800,(y+100)/800)){
	case 1:
	case 2:
	case 3:
	case 12:	//下のななめタイルだった
	case 13:
		land_cnt = 3;
		break;
	case 14:
	case 15:
		break;
	}

	if(my < 0){
		land_cnt = 0;
		return gxFalse;
	}

	//坂からの移動であることが確定
	//途中で■ブロックが現れると斜め処理が途切れるので
	//⊿ブロックとして無理やり補正する。
	if(land_cnt > 0){
		land_cnt --;
	}else{
		//空中にいた
		return gxFalse;
	}

	Sint32 mem_y = my;
	gxBool mission_complete = gxFalse;

	if(is_hit(x+mx,y+my)){
		//最初から埋まっている(上にあげていく)
		//case 1
		//　　　　⊿□
		//　　→⊿□□
		//　　⊿□□□

		//case 2
		//　　　□□□
		//　　→□□□
		//　　□□□□

		for(Sint32 i=0;i<KyoyouRange;i++){			//8ドットまで許容範囲(空きがあるまであげていく)
			switch(is_hit(x+mx,y+my)){
			case 1:
			case 3:
			case 12:	//斜めの壁のときは上にあげていく
			case 13:
				my -= 100;
				break;
			case 2:		//半壁はくだりのみ判定する
				if( !m_bNoCheckHalfWall )
				{
	//				if(mov.y>0)	my -= 100;		//途中でＭＹが０未満になるので、入ってきた当初のY増分値を参照する
					if(mem_y>=0)	my -= 100;
					if(mem_y< 0 )	goto CheckStop;
				}
				break;
			case 14:
			case 15:
			default:
				//空き発見
				mission_complete = gxTrue;
				break;
			}
		}
	}else{

		if(move_mode_ == 1)	return gxFalse;	//宇宙モードでは補正させない

		//移動先であたっていなかった
		//無理やり壁にそわせるよう下にさげていく
		//　　　←
		//　　　⊿□
		//　　⊿□□
		for(Sint32 i=0;i<4;i++){			//４ドットまで許容範囲
			if(is_hit(x+mx,y+my)){
				my -= 100;				//あたったところから１ドット上にあげる
				mission_complete = gxTrue;
				break;
			}
			my += 100;
		}
	}

CheckStop:

	if( mission_complete == gxFalse){
		//適切な場所が見つからなかった
		//うまりっぱなし or いつまでも空白なので床磁石しない
		//　←
		//　□□
		//　□□←
		//こんな場合が考えられる
		my = mem_y;		//元に戻す
		return gxFalse;
	}

	//とりあえず空中にいる（１ドット以内に壁があるはず）
	for(Sint32 i=0;i<100;i++){
		if(is_hit(x+mx,y+my)){
			break;
		}
		my ++;	// 1/100ピクセルずらす
	}

	//ぶつかったところから 1/100ピクセル上に戻してぶつかるギリギリに補正する
	my --;	// 1/100ピクセルずらす

	//壁にそった状態完了

	return gxTrue;
}


gxBool CObj::check_top(Sint32 x,Sint32 y,Sint32 &mx,Sint32 &my)
{
	//-----------------------------------------------
	//天井の１ドットを検査
	//Ｙ座標のみ補正する
	//-----------------------------------------------

	switch(pGame->pBg->get_kabetype8((x+mx)/800,(y+my)/800)){
	case 1:
	case 2:
	case 3:
	case 14:	//上のななめタイルだった
	case 15:
		top_cnt = 3;
		break;
		//こいつらはないのと同じ
	case 12:
	case 13:
		break;
	}

	//坂からの移動であることが確定
	//途中で■ブロックが現れると斜め処理が途切れるので
	//⊿ブロックとして無理やり補正する。
	if(top_cnt > 0){
		top_cnt --;
	}else{
		//空中にいた
		return gxFalse;
	}

	Sint32 mem_y = my;
	gxBool mission_complete = gxFalse;

	if(is_hit(x+mx,y+my)){
		//最初から埋まっている(下にさげていく)
		//case 1
		//　　　　⊿□
		//　　→⊿□□
		//　　⊿□□□

		//case 2
		//　　　□□□
		//　　→□□□
		//　　□□□□

		for(Sint32 i=0;i<KyoyouRange;i++){			//8ドットまで許容範囲(空きがあるまであげていく)
			switch(is_hit(x+mx,y+my)){
			case 1:
			case 3:
			case 14:
			case 15:
				my += 100;
				break;
			case 12:
			case 13:
			case 2:		//半壁は判定の対象外
			default:
				//空き発見
				mission_complete = gxTrue;
				break;
			}
		}
	}else{

		//移動先であたっていなかった（天井は磁石機能ないのでそわせない。無視する）

	}

	if( mission_complete == gxFalse){
		//適切な場所が見つからなかった
		//うまりっぱなし or いつまでも空白なので床磁石しない
		my = mem_y;		//元に戻す
		return gxFalse;
	}

	//とりあえず空中にいる（１ドット以内に壁があるはず）
	for(Sint32 i=0;i<100;i++){
		if(is_hit(x+mx,y+my)){
			break;
		}
		my --;	// 1/100ピクセルずらす
	}

	//ぶつかったところから 1/100ピクセル下に戻してぶつかるギリギリに補正する
	my ++;	// 1/100ピクセルずらす

	//壁にそった状態完了

	return gxTrue;
}

//--------------------------
//これめちゃ重要
//--------------------------
#define YOKO_HABA1 (1)
#define YOKO_HABA2 (3)
//--------------------------

gxBool CObj::check_side(Sint32 ax,Sint32 ay,Sint32 &mx,Sint32 &my)
{
	//-----------------------------------------------
	//横座標のチェック
	//-----------------------------------------------
	Sint32 y1 = (ay-h_)/800;
	Sint32 y2 = (ay)/800+1;
	Sint32 x1 = (ax)/800-YOKO_HABA1;	
	Sint32 x2 = x1 + YOKO_HABA2;
	//縦幅決定

	Sint32 bx1,by1;
	Sint32 max_left = -1;
	Sint32 max_right= -1;
	Sint32 m_left=-1,m_right=-1;
	Sint32 center_x = 8*(ax)/800;

	gxBool flag = gxFalse;
	for(Sint32 x=x1;x<x2;x++){
		flag = gxFalse;
		for(Sint32 y=y1;y<y2;y++){
			//左端

			bx1 = x*8;
			by1 = y*8;
			viiSub::GetScreenPosition(bx1,by1);
			switch(get_tile(x,y)){
			case 1:
			case 3:
#ifdef ATARI_DEBUG_
				viiDraw::Box(bx1,by1,bx1+7,by1+7,PRIO_DEBUG,gxTrue,ARGB(0x80,0xff,0x00,0x00));
#endif
				flag = gxTrue;
				break;
			case 12:
			case 13:
			case 14:
			case 15:
			case 0:
				if(m_left == -1 || m_left < x*8){
					m_left = x*8;
				}
				if(m_right == -1 || m_right < x*8+7){
					m_right = x*8+7;
				}
#ifdef ATARI_DEBUG_
				viiDraw::Box(bx1,by1,bx1+7,by1+7,PRIO_DEBUG,gxTrue,ARGB(0x80,0xff,0xff,0x00));
#endif
				break;
			}
		}
		if(flag == gxFalse){
			//赤１つもなし

				if(max_left == -1 || m_left < max_left){
					if(m_left < center_x)
						max_left = m_left;
				}
				if(max_right == -1 || m_right > max_right){
					if(m_right > center_x)
						max_right = m_right;
				}

		}
	}

	//目新しい横幅がみつからなかった
	if(max_left==-1){
		max_left = center_x;
	}
	if(max_right==-1){
		max_right = center_x;
	}
//	if( max_left == -1 && max_right==-1)
//	{
//		//毒
//		return gxFalse;
//	}

	if(ax+mx < max_left*100){
		mx = (max_left*100) - ax;
	}else if(ax+mx > max_right*100){
		mx = (max_right*100) - ax;
	}
#ifdef ATARI_DEBUG_


	viiSub::GetScreenPosition(max_left,by1);
	GGX_DrawLine(max_left,0,max_left,240,PRIO_DEBUG,ARGB(0x80,0xff,0xff,0x00));

	viiSub::GetScreenPosition(max_right,by1);
	GGX_DrawLine(max_right,0,max_right,240,PRIO_DEBUG,ARGB(0x80,0x00,0xff,0xff));

	viiSub::GetScreenPosition(center_x,by1);
	GGX_DrawLine(center_x,0,center_x,240,PRIO_DEBUG,ARGB(0x80,0xff,0xff,0xff));
#endif

	return gxTrue;

}


gxBool CObj::easy_collision_check()
{
	//簡易当たり判定（足元しかみない）

	if(!is_kabetype( is_hit(pos.x+mov.x,pos.y+mov.y) )) return gxFalse;	//高速化

	KyoyouRange = 16;

	switch(is_hit(pos.x+mov.x,pos.y+mov.y)){
	case 1:
	case 2:
	case 3:
	case 12:
	case 13:
	case 14:
	case 15:
		if(!check_under(pos.x,pos.y,mov.x,mov.y)){
			if(is_kabetype( is_hit(pos.x,pos.y+mov.y) )){
				land_cnt = 1;
				check_under(pos.x,pos.y,mov.x,mov.y);
				if(is_kabetype( is_hit(pos.x,pos.y+mov.y) )){
					mov.y = 0;
				}
			}
		}
		break;
	default:
		return gxFalse;
	}


	if(is_kabetype( is_hit(pos.x+mov.x,pos.y+mov.y) )){
		mov.x = 0;
	}



	return gxTrue;

}

gxBool CObj::is_kabetype(Sint32 type)
{
	switch(type){
	case 2:
		if(mov.y >0) return gxTrue;
		break;
	case 1:
	case 3:
	case 12:
	case 13:
	case 14:
	case 15:
		return gxTrue;
	}

	return gxFalse;
}

