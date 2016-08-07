//-------------------------------
//�ǂƂ̂����蔻��I�u�W�F�N�g
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
	//�f�o�b�O�\��
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
//�����蔻���U�e
//----------------------------------------------------------
//----------------------------------------------------------
//�@���@�@����[
//�@���@
//�@���@�@���~�h��
//�@���@
//�@���@�@���I�[

#define TILESIZE (8)

Sint32 CObj::adjust_move(Sint32 mode)
{
	if( m_bAtariOff ) return gxTrue;

	move_mode_ = mode;
	switch(move_mode_){
	case 0:
		KyoyouRange = 8;	//�P�u���b�N��
		collision_check();
		break;
	case 1:
		KyoyouRange = 8;	//�P�u���b�N��
		collision_check();
		break;
	case 2:
		KyoyouRange = 32;	//�S�u���b�N��
		collision_check();
		break;
	}

	m_bNoCheckHalfWall = gxFalse;

	return gxTrue;
}


Sint32 CObj::is_hit(Sint32 x,Sint32 y)
{
	//�O���[�o�����W����ǂ̃^�C�v�𓾂�

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
	//���ݒ��n��������
	Sint32 ax,ay,type;

	ax = pos.x/100;
	ay = pos.y/100;	
	type = pGame->pBg->get_kabetype(ax,ay+2);	//�K���Q�h�b�g���𒲂ׂ�i�O�̂��߂Q�h�b�g�j

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
	//�O���[�o�����W����^�C���p�^�[���ԍ������o��

	ax = ax/(TILESIZE*100);
	ay = ay/(TILESIZE*100);

	return get_tile(ax,ay);
}


Sint32 CObj::get_tile(Sint32 ax,Sint32 ay)
{
	//�^�C�����W����^�C���p�^�[���ԍ�������o��

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
//��X�e�@�����蔻��
//�����蔻��̗D�揇�ʂ�݂��āA����������܂�
//���肷��B�Ō�̂ق��܂ň���������Ȃ����
//���Ȃ萸�x�̍������肪�\�B��d�ɔ��肵��
//���������Ȃ������蔻����@�B
//�����`�F�b�N�ň������������ꍇ�ɂ�
//---------------------------------------------
/*
�@�@�A�@�@
�@�@�@�@�@
�B�@�@�@�C
�@�@�@�@�@
�@�@�@�@�@
*/

//#define ATARI_DEBUG_

void CObj::collision_check()
{
	//��`�ɂ��Փ˃`�F�b�N
	Sint32 w = 20*100;
	Sint32 h = 40*100;
	Sint32 x,y,x1,y1,x2,y2;
	gxBool adjust = gxFalse;
	gxBool y_adjust = gxFalse;	//�x���W�␳�t���O
	w_ = w;
	h_ = h;

	//----------------------------------------------------------------
	//�܂��͑S�̂��I�u�W�F�N�g�ɂ������Ă��Ȃ���΂Ȃɂ����肵�Ȃ�
	//----------------------------------------------------------------

	//�ړ������Ă݂�
	x = pos.x + mov.x;
	y = pos.y + mov.y;
	x1 = x-w/2;
	y1 = y-h;
	x2 = x+w/2;
	y2 = y;
	if(!check_allblock(x1,y1,x2,y2)) return;
	viiDbg::printf(200,100,"�S�̏Փ˂���");

	if(check_side(pos.x,pos.y,mov.x,mov.y)) {
		adjust = gxTrue;
	}

/*
	if(is_hit(x,y)){
		//�������O���珰���łԂ����Ă����
		//�␳���Ȃ��A���Ă��������f�s�\
		return;
	}
*/
	//------------------------------------------------
	//�x�̕␳
	//------------------------------------------------

	adjust = gxFalse;

	//�����`�F�b�N(�����͂����ł̂݃`�F�b�N����)
	if(check_under(pos.x,pos.y,mov.x,mov.y))
	{

#ifdef ATARI_DEBUG_
		viiDbg::printf(200,110,"���̕␳�������B�x���W����������");
#endif
		y_adjust = gxTrue;	//�R���ȍ~�x�̍��W�͂�����Ȃ�
	}

	//�V��`�F�b�N
	if(y_adjust == gxFalse)
	{
		//�x���W�␳���Ȃ���ΓV��̃`�F�b�N�����ĕ␳����
		if(check_top(pos.x,pos.y-h,mov.x,mov.y))
		{

#ifdef ATARI_DEBUG_
		viiDbg::printf(200,120,"�V��̕␳�������B�x���W����������");
#endif
			y_adjust = gxTrue;	//�R���ȍ~�x�̍��W�͂�����Ȃ�
		}
	}

	//------------------------------------------------
	//�w�̕␳
	//------------------------------------------------

	if(check_side(pos.x,pos.y,mov.x,mov.y)) {
		adjust = gxTrue;
	}
}

gxBool CObj::check_allblock(Sint32 x1,Sint32 y1,Sint32 x2,Sint32 y2)
{
	//-----------------------------------------------
	//��`���u���b�N�ɂԂ����Ă��邩�`�F�b�N����
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
			//���ׂẴ^�C�����`�F�b�N
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
			case 2:	//���ǂ͂�����̂ݔ��肷��
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
	//�����̃X�O���̂P�h�b�g������
	//�x���W�̂ݕ␳����
	//-----------------------------------------------

	switch(pGame->pBg->get_kabetype8(x/800,(y+100)/800)){
	case 1:
	case 2:
	case 3:
	case 12:	//���̂ȂȂ߃^�C��������
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

	//�₩��̈ړ��ł��邱�Ƃ��m��
	//�r���Ł��u���b�N�������Ǝ΂ߏ������r�؂��̂�
	//���u���b�N�Ƃ��Ė������␳����B
	if(land_cnt > 0){
		land_cnt --;
	}else{
		//�󒆂ɂ���
		return gxFalse;
	}

	Sint32 mem_y = my;
	gxBool mission_complete = gxFalse;

	if(is_hit(x+mx,y+my)){
		//�ŏ����疄�܂��Ă���(��ɂ����Ă���)
		//case 1
		//�@�@�@�@����
		//�@�@��������
		//�@�@��������

		//case 2
		//�@�@�@������
		//�@�@��������
		//�@�@��������

		for(Sint32 i=0;i<KyoyouRange;i++){			//8�h�b�g�܂ŋ��e�͈�(�󂫂�����܂ł����Ă���)
			switch(is_hit(x+mx,y+my)){
			case 1:
			case 3:
			case 12:	//�΂߂̕ǂ̂Ƃ��͏�ɂ����Ă���
			case 13:
				my -= 100;
				break;
			case 2:		//���ǂ͂�����̂ݔ��肷��
				if( !m_bNoCheckHalfWall )
				{
	//				if(mov.y>0)	my -= 100;		//�r���łl�x���O�����ɂȂ�̂ŁA�����Ă���������Y�����l���Q�Ƃ���
					if(mem_y>=0)	my -= 100;
					if(mem_y< 0 )	goto CheckStop;
				}
				break;
			case 14:
			case 15:
			default:
				//�󂫔���
				mission_complete = gxTrue;
				break;
			}
		}
	}else{

		if(move_mode_ == 1)	return gxFalse;	//�F�����[�h�ł͕␳�����Ȃ�

		//�ړ���ł������Ă��Ȃ�����
		//�������ǂɂ��킹��悤���ɂ����Ă���
		//�@�@�@��
		//�@�@�@����
		//�@�@������
		for(Sint32 i=0;i<4;i++){			//�S�h�b�g�܂ŋ��e�͈�
			if(is_hit(x+mx,y+my)){
				my -= 100;				//���������Ƃ��납��P�h�b�g��ɂ�����
				mission_complete = gxTrue;
				break;
			}
			my += 100;
		}
	}

CheckStop:

	if( mission_complete == gxFalse){
		//�K�؂ȏꏊ��������Ȃ�����
		//���܂���ςȂ� or ���܂ł��󔒂Ȃ̂ŏ����΂��Ȃ�
		//�@��
		//�@����
		//�@������
		//����ȏꍇ���l������
		my = mem_y;		//���ɖ߂�
		return gxFalse;
	}

	//�Ƃ肠�����󒆂ɂ���i�P�h�b�g�ȓ��ɕǂ�����͂��j
	for(Sint32 i=0;i<100;i++){
		if(is_hit(x+mx,y+my)){
			break;
		}
		my ++;	// 1/100�s�N�Z�����炷
	}

	//�Ԃ������Ƃ��납�� 1/100�s�N�Z����ɖ߂��ĂԂ���M���M���ɕ␳����
	my --;	// 1/100�s�N�Z�����炷

	//�ǂɂ�������Ԋ���

	return gxTrue;
}


gxBool CObj::check_top(Sint32 x,Sint32 y,Sint32 &mx,Sint32 &my)
{
	//-----------------------------------------------
	//�V��̂P�h�b�g������
	//�x���W�̂ݕ␳����
	//-----------------------------------------------

	switch(pGame->pBg->get_kabetype8((x+mx)/800,(y+my)/800)){
	case 1:
	case 2:
	case 3:
	case 14:	//��̂ȂȂ߃^�C��������
	case 15:
		top_cnt = 3;
		break;
		//������͂Ȃ��̂Ɠ���
	case 12:
	case 13:
		break;
	}

	//�₩��̈ړ��ł��邱�Ƃ��m��
	//�r���Ł��u���b�N�������Ǝ΂ߏ������r�؂��̂�
	//���u���b�N�Ƃ��Ė������␳����B
	if(top_cnt > 0){
		top_cnt --;
	}else{
		//�󒆂ɂ���
		return gxFalse;
	}

	Sint32 mem_y = my;
	gxBool mission_complete = gxFalse;

	if(is_hit(x+mx,y+my)){
		//�ŏ����疄�܂��Ă���(���ɂ����Ă���)
		//case 1
		//�@�@�@�@����
		//�@�@��������
		//�@�@��������

		//case 2
		//�@�@�@������
		//�@�@��������
		//�@�@��������

		for(Sint32 i=0;i<KyoyouRange;i++){			//8�h�b�g�܂ŋ��e�͈�(�󂫂�����܂ł����Ă���)
			switch(is_hit(x+mx,y+my)){
			case 1:
			case 3:
			case 14:
			case 15:
				my += 100;
				break;
			case 12:
			case 13:
			case 2:		//���ǂ͔���̑ΏۊO
			default:
				//�󂫔���
				mission_complete = gxTrue;
				break;
			}
		}
	}else{

		//�ړ���ł������Ă��Ȃ������i�V��͎��΋@�\�Ȃ��̂ł��킹�Ȃ��B��������j

	}

	if( mission_complete == gxFalse){
		//�K�؂ȏꏊ��������Ȃ�����
		//���܂���ςȂ� or ���܂ł��󔒂Ȃ̂ŏ����΂��Ȃ�
		my = mem_y;		//���ɖ߂�
		return gxFalse;
	}

	//�Ƃ肠�����󒆂ɂ���i�P�h�b�g�ȓ��ɕǂ�����͂��j
	for(Sint32 i=0;i<100;i++){
		if(is_hit(x+mx,y+my)){
			break;
		}
		my --;	// 1/100�s�N�Z�����炷
	}

	//�Ԃ������Ƃ��납�� 1/100�s�N�Z�����ɖ߂��ĂԂ���M���M���ɕ␳����
	my ++;	// 1/100�s�N�Z�����炷

	//�ǂɂ�������Ԋ���

	return gxTrue;
}

//--------------------------
//����߂���d�v
//--------------------------
#define YOKO_HABA1 (1)
#define YOKO_HABA2 (3)
//--------------------------

gxBool CObj::check_side(Sint32 ax,Sint32 ay,Sint32 &mx,Sint32 &my)
{
	//-----------------------------------------------
	//�����W�̃`�F�b�N
	//-----------------------------------------------
	Sint32 y1 = (ay-h_)/800;
	Sint32 y2 = (ay)/800+1;
	Sint32 x1 = (ax)/800-YOKO_HABA1;	
	Sint32 x2 = x1 + YOKO_HABA2;
	//�c������

	Sint32 bx1,by1;
	Sint32 max_left = -1;
	Sint32 max_right= -1;
	Sint32 m_left=-1,m_right=-1;
	Sint32 center_x = 8*(ax)/800;

	gxBool flag = gxFalse;
	for(Sint32 x=x1;x<x2;x++){
		flag = gxFalse;
		for(Sint32 y=y1;y<y2;y++){
			//���[

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
			//�ԂP���Ȃ�

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

	//�ڐV�����������݂���Ȃ�����
	if(max_left==-1){
		max_left = center_x;
	}
	if(max_right==-1){
		max_right = center_x;
	}
//	if( max_left == -1 && max_right==-1)
//	{
//		//��
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
	//�ȈՓ����蔻��i���������݂Ȃ��j

	if(!is_kabetype( is_hit(pos.x+mov.x,pos.y+mov.y) )) return gxFalse;	//������

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

