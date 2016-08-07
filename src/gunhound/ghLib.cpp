//------------------------------------------------------------
//
// �V�^���C�u�����Ƃ̂��܍��킹�R�[�h
//
//------------------------------------------------------------

#include <gunvalken.h>

//---------------------------------------------
//���܍��킹�p
JOYSTICK_T Joy[enPlayerMax];
ButtonConfig_t undoConfig;

typedef struct StLocaValue {
	char LocalFilePath[255];
	Sint32 val[32];
}StLocaValue;

StLocaValue g_LocalValue;
//�O���[�o���ϐ�

ghLib::SAVEDATA_T g_StSaveData;

Float32 gxAbs( Float32 n )
{
	return ABS( n );
}
Sint32  gxAbs( Sint32 n )
{
	return ABS( n );
}

//�e�N�X�`���t�@�C���ǂݍ��݁iBMP or TGA�j
gxBool LoadTexture(int pg , gxChar *pBmpfilename,int col)
{
	return gxLib::LoadTexture( pg , pBmpfilename , col );
}

gxBool ReadTexture(int pg, Uint8 *pMemory,Uint32 sz,int col)
{
	return gxLib::ReadTexture( pg , pMemory, sz , col );
}

gxBool UploadTexture(bool force_reading )
{
	return gxLib::UploadTexture( force_reading );
}

//�X�v���C�g�\��
int PutSpriteDX(
	int x,		int y,	int prio,
	int page,	int u, 	int v,	int w,	int h,
	 //�I�v�V����
	 int cx,	int cy,
	 int atr,	 unsigned long col,
	 float rot, float scl_x,float scl_y
	)
{
	return gxLib::PutSprite(
		x , y , prio ,
		page , u , v , w ,h ,cx , cy,
		atr , col , rot , scl_x , scl_y );
}

//�|���S���O���t�B�b�N�\��
int PutGraphics(
	 int type,
	 int x,	int y,	int prio,
	 GRA_POS_T *pos,
	 //�I�v�V����
	  int atr,
	  float rot,
	  float scl_x,float scl_y
	 )
{
	//GRA_BOXF
	gxLib::DrawColorBox(
		pos[0].x +x, pos[0].y+y ,pos[0].col,
		pos[1].x +x, pos[1].y+y ,pos[1].col,
		pos[2].x +x, pos[2].y+y ,pos[2].col,
		pos[3].x +x, pos[3].y+y ,pos[3].col,
		prio,
		atr
		);

	//CBellowsSprite�Ŏg�p�A������CBellowsSprite���g���Ă��Ȃ��̂Ŋ���
	//GRA_SPRITE

	return 0;
}

void GGX_Putsprite(gxSprite *p,int x,int y,int prio,int atr,unsigned long col,float scl,float rot)
{
	//-------------------------
	//�X�v���C�g��`�悷��
	//-------------------------

	PutSpriteDX(
		x,y,prio,
		p->page,p->u,p->v,p->w,p->h,
		//�I�v�V����
		p->cx,p->cy,
		atr,
		col,
		rot,scl,scl
		);

}

void GGX_DrawPixel(int x,int y,int z,int col,int atr)
{
	//------------------------------------
	//�_��`�悷��
	//------------------------------------

/*
	GRA_POS_T pos[4];

	pos[0].x=(float)x;
	pos[0].y=(float)y;
	pos[0].z=(float)z;
	pos[0].col=col;
*/
	//PutGraphics( GRA_PIXEL,0,0,z,pos,atr);
	gxLib::DrawPoint( x , y , z , atr , col );

}


void GGX_DrawBox(int x1,int y1,int x2,int y2,int z,gxBool flag,int col,int atr)
{
	//------------------------------------
	//�l�p������`��
	//flag (true=BOXFILL / false=BOX)
	//------------------------------------
/*
	GRA_POS_T pos[4];

	pos[0].x=(float)x1;
	pos[0].y=(float)y1;
	pos[0].z=(float)z;
	pos[0].col=col;

	pos[1].x=(float)x2;
	pos[1].y=(float)y1;
	pos[1].z=(float)z;
	pos[1].col=col;

	pos[2].x=(float)x2;
	pos[2].y=(float)y2;
	pos[2].z=(float)z;
	pos[2].col=col;

	pos[3].x=(float)x1;
	pos[3].y=(float)y2;
	pos[3].z=(float)z;
	pos[3].col=col;

	if(flag){
		PutGraphics( GRA_BOXF,0,0,z,pos,atr);
	}else{
		PutGraphics( GRA_BOX,0,0,z,pos,atr);
	}
*/

	gxLib::DrawBox( x1 , y1 , x2 , y2 , z , flag , atr , col );
}


void GGX_DrawTriangle(int x1,int y1,int x2,int y2,int x3,int y3,int z,int col,int atr)
{
	//------------------
	//�O�p��`�悷��
	//------------------
/*
	GRA_POS_T pos[4];

	pos[0].x=(float)x1;
	pos[0].y=(float)y1;
	pos[0].z=(float)z;
	pos[0].col=col;

	pos[1].x=(float)x2;
	pos[1].y=(float)y2;
	pos[1].z=(float)z;
	pos[1].col=col;

	pos[2].x=(float)x3;
	pos[2].y=(float)y3;
	pos[2].z=(float)z;
	pos[2].col=col;

	PutGraphics( GRA_TRIANGLE,0,0,z,pos,atr);
*/

	gxLib::DrawTriangle( x1 , y1 , x2 , y2 , x3 , y3 ,z , gxFalse , atr , col );

}


void GGX_DrawLine(int x1,int y1,int x2,int y2,int z,int col,int atr)
{
	//------------------
	//����`�悷��
	//------------------
/*
	GRA_POS_T pos[4];

	pos[0].x=(float)x1;
	pos[0].y=(float)y1;
	pos[0].z=(float)z;
	pos[0].col=col;

	pos[1].x=(float)x2;
	pos[1].y=(float)y2;
	pos[1].z=(float)z;
	pos[1].col=col;

	PutGraphics( GRA_LINE,0,0,z,pos,atr);
*/
	gxLib::DrawLine( x1 , y1 , x2 , y2 , z , atr , col );
}

/*
void GGX_DrawBoxGradation(int x1,int y1,int x2,int y2,int z,int atr,int col1,int col2,int col3,int col4)
{
	//--------------------------------------------
	//����O���fBOXF
	//--------------------------------------------
	GRA_POS_T pos[4];
	pos[0].x=x1*1.f;
	pos[0].y=y1*1.f;
	pos[0].z=z*1.f;
	pos[0].col=col1;

	pos[1].x=x2*1.f;
	pos[1].y=y1*1.f;
	pos[1].z=z*1.f;
	pos[1].col=col2;

	pos[2].x=x2*1.f;
	pos[2].y=y2*1.f;
	pos[2].z=z*1.f;
	pos[2].col=col3;

	pos[3].x=x1*1.f;
	pos[3].y=y2*1.f;
	pos[3].z=z*1.f;
	pos[3].col=col4;

	PutGraphics( GRA_BOXF,0,0,z,pos,atr);
}
*/

void msg_printf(int x,int y,int pr,char *msg,...)
{
	gxLib::Printf( x , y , pr , ATR_DFLT , ARGB_DFLT , msg );
}
void msg_printfwithCol(int x,int y,int pr,unsigned int col,char *msg,...)
{
	gxLib::Printf( x , y , pr , ATR_DFLT , col , msg );
}
int PutStringDX(int x,int y,int prio,char *msg,unsigned long col)
{
	return 0;
}
void dbg_printf(char *msg,...)
{
	gxLib::DebugLog( msg );
}

gxBool ghLib::IsFullScreen()
{
	return gxFalse;
	//return !GGA.ModeWindow;
}

gxBool ghLib::ChangeWindowMode( gxBool bFlag )
{
	//ChangeScreenMode( GGA.windowSize );

	return gxTrue;
}


Sint32& ghLib::Value(Sint32 n)
{
	return	g_LocalValue.val[n];
}
/*

char* ghLib::GetLocalFilePath()
{

	return g_LocalValue.LocalFilePath;

}


void ghLib::SetLocalFilePath(char *p)
{

	//CWin32tool::GetFilePath( p, &g_LocalValue.LocalFilePath[0] );

}

*/

gxBool ghLib::SaveConfig()
{
	//�Z�[�u�f�[�^��ۑ�����

	//WriteFile( _FILENAME_ENVIRONMENT_ ,(char *)&g_StSaveData,sizeof(gxLib::SAVEDATA_T));

	return gxTrue;

}


gxBool ghLib::LoadConfig()
{
	//�Z�[�u�f�[�^��ǂݍ���

	Uint8 *pData;
	Uint32 uSize;

	pData = gxLib::LoadFile( _FILENAME_ENVIRONMENT_ , &uSize );

	if( pData == NULL )
	{
		SetConfigDefault();
		return gxFalse;
	}

	memcpy( &g_StSaveData , pData , uSize );

	delete[] pData;

/*
	if(ReadFile( _FILENAME_ENVIRONMENT_ ,(char*)&g_StSaveData,sizeof(gxLib::SAVEDATA_T)) < 0)
	{
		gxLib::SetConfigDefault();
		return gxFalse;
	}
*/

	return gxTrue;
}

gxBool ghLib::SetConfigDefault()
{
	//���������̃Z�[�u�f�[�^���f�t�H���g�ɂ���

	g_StSaveData.windowSize = 0;
	g_StSaveData.version_no = 0;
	g_StSaveData.wideScreen = 0;
	//gxLib::SetDefaultPadCongig();

	memset( &g_StSaveData.free[0] , 0x00, sizeof(Uint32)*ghLib::enSaveDataFreeArea );

	return gxTrue;
}


void ghLib::SetDefaultPadCongig()
{
	//--------------------------------------------
	//�f�t�H���g�̃p�b�h�f�[�^��ݒ肷��
	//--------------------------------------------
	
	for(int i=0;i<16;i++)
	{
		for(int j=0;j<16;j++)
		{
			g_StSaveData.BtnConfig.enable[i][j] = 1;	//all usable
		}
	}

	g_StSaveData.BtnConfig.bUseController = 0;	//�g�p���Ȃ�

	for(int ii=0;ii<enPlayerMax;ii++)
	{
		g_StSaveData.BtnConfig.useDevice[ii] = 0;
		for(int iii=0;iii<32;iii++)
		{
			g_StSaveData.BtnConfig.assign[ii][iii] = -1;
		}
	}

	undoConfig = g_StSaveData.BtnConfig;

}


void ghLib::SetRumble(int player,int time,int x,int y)
{
	//--------------------------------------------
	//�U����ݒ肷��
	//--------------------------------------------

	gxLib::SetRumble( player , 0 , time );//, x , y );

}

Sint32 ghLib::GetJoypadDir(int pl,int mode)
{
	//--------------------------
	//���͕�����Ԃ��B
	//--------------------------

	int j,dir=0;

	if(mode == JOY_PUSH){
		j=Joy[pl].psh&(JOY_U|JOY_L|JOY_R|JOY_D);
	}
	if(mode == JOY_TRIG){
		j=Joy[pl].psh&(JOY_U|JOY_L|JOY_R|JOY_D);
	}

	switch(j){
	case JOY_R:
		dir = 3;
		break;
	case JOY_D:
		dir = 5;
		break;
	case JOY_L:
		dir = 7;
		break;
	case JOY_U:
		dir = 1;
		break;

	case JOY_R|JOY_D:
		dir = 4;
		break;
	case JOY_D|JOY_L:
		dir = 6;
		break;
	case JOY_L|JOY_U:
		dir = 8;
		break;
	case JOY_U|JOY_R:
		dir = 2;
		break;
	}

	return dir;
}

//----------------------------------------------------
//
//�T�E���h�֘A�̃��b�p�[
//
//----------------------------------------------------
int ghLib::LoadWaveFile(long no, char *file)	//SoundLoad
{
	return gxLib::LoadAudio(no,file);
}

int ghLib::ReadWaveData(long no, unsigned char *pWaveBuf , Uint32 uSize )	// SoundLoadFromMemory
{
	return 	gxLib::ReadAudio(no, pWaveBuf, uSize);
}

void ghLib::PlayWave(Sint32 no,gxBool bLoop,Sint32 vol,Uint64 current )	//SoundPlay
{
	gxLib::PlayAudio( no , bLoop );//,vol,current);
}

void ghLib::StopWave( Sint32 no , Sint32 frm )	//SoundStop
{
	gxLib::StopAudio( no , frm );
}

/*
//void ghLib::SetMasterVolume(int vol,int pan)	//SoundMasterControl
//{
//	gxLib::SetAudioMasterVolume( vol );//, pan );
//}
*/
/*
//Uint32 ghLib::GetPlayPosition(Sint32 no )	//SoundGetPosion
//{
//	return 0;
////	return SoundGetPosion( no );
//}
//
//Uint32 ghLib::GetSoundLength(Sint32 no )	//SoundGetLength
//{
//	return 0;
////	return SoundGetLength( no );
//}
*/
/*
//StSoundInfo* ghLib::GetWaveInfo(Sint32 no)	//GetSoundInfo
//{
//	return GetSoundInfo( no );
//}
*/


void ghLib::SetSoundVolume(int no , int vol )	//SoundVolumeControl
{
	gxLib::SetAudioVolume( no , vol );
}

gxBool ghLib::IsSoundPlay(int n)	//SoundIsPlay
{
	return gxLib::IsAudioPlay( n );
}

Sint32 ghLib::GetMouseWheelNum(Sint32 sMax)
{
	//�}�E�X�̃z�C�[�����ݒl���[n�`�{n�̊ԂŕԂ�
	//Sint32 sCnt = CPadControl::GetInstance()->Mouse._WheelCnt.y;

	//if( sCnt >=  sMax ) sCnt =  sMax;
	//if( sCnt <= -sMax ) sCnt = -sMax;

	//CPadControl::GetInstance()->Mouse._WheelCnt.y = sCnt;

	//	return sCnt;

	return 0;
}

void ghLib::AdjustControl()
{
	
	Joy[0].psh       = gxLib::Joy(0)->psh;
	Joy[0].trg       = gxLib::Joy(0)->trg;
	Joy[0].rep       = gxLib::Joy(0)->rep;
	Joy[0].analog_x  = gxLib::Joy(0)->ax;
	Joy[0].analog_y  = gxLib::Joy(0)->ay;
	Joy[0].analog_z  = 0;
	Joy[0].analog_rx = gxLib::Joy(0)->rx;
	Joy[0].analog_ry = gxLib::Joy(0)->ry;
	Joy[0].analog_rz = 0;
	Joy[0].mx        = gxLib::Joy(0)->mx;
	Joy[0].my        = gxLib::Joy(0)->my;

#if 0
	Joy[0].dragnow;
	Joy[0].drag1;	//�h���b�O�J�n�ʒu
	Joy[0].drag2;	//�h���b�O�I���ʒu
	Joy[0].scr;	//�X�N���[���|�W�V����

	Sint16	x, y;		/* �����X�e�B�b�N�̕ϗ�. x,y �� -0x8000�`0�`0x7fff �ɂȂ� ��mouse�̂Ƃ��͍��W*/
	Sint16	cx, cy;		/* mm�n��x,y�����߂邽�߂̕ϐ�. ��mouse�̂Ƃ��͌��ݏ������̍��W */
	long	dw,dh;		/* �V						*/
	char name[0xff];
	Sint32 stick_num;
	Sint32 repeat[20];
#endif
}
