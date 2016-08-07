#define enBgTypeMax (4)

//#define SCRX (SCR_W*30)						//����ʃT�C�Y(9600)VMP�̃f�[�^��������̂ŏ���͕ς��Ȃ��悤��
//#define SCRY (SCR_H*6)						//�c��ʃT�C�Y(1440)
//#define MAPTIP_POS_CALC(a,b) ((b)*(SCRX/MAP_BLOCK_SIZE)+(a))
//#define MAP_POSCALC(a,b) ((b)*(SCRX/8)+(a))	//�����蔻��}�b�v�ʒu
//#define MAPMAX_BLOCK_MAX  (SCRX/MAP_BLOCK_SIZE*SCRY/MAP_BLOCK_SIZE)	//120000

#define MAP_BLOCK_SIZE (16)					//�P�u���b�N�̃T�C�Y

//1��ʂ̃f�[�^�T�C�Y�i�}�b�N�X�l�v�Z�p�ŁA���ۂ̃T�C�Y�Ƃ͈قȂ�j
#define MAP_WIDTH  (320*30)	//����ʃT�C�Y(9600)VMP�̃f�[�^��������̂ŏ���͕ς��Ȃ��悤��
#define MAP_HEIGHT (240*6)	//�c��ʃT�C�Y(1440)

//�P�e�N�X�`��(256x256)�Ɏ��܂�u���b�N�̐�
#define MAP256_BLOCK_MAX  (256/MAP_BLOCK_SIZE*256/MAP_BLOCK_SIZE)	//64

//.MAP�f�[�^�̃t�@�C���T�C�Y
#define MAP_FILE_SIZE (sizeof(Sint16)* (MAP_WIDTH*MAP_HEIGHT)/(MAP_BLOCK_SIZE*MAP_BLOCK_SIZE))

//.VMP�f�[�^�̃t�@�C���T�C�Y
#define VMP_DATA_SIZE ((MAP_WIDTH*MAP_HEIGHT)/(8*8)) 		//(40*10*30*10)			//�����蔻��}�b�v�T�C�Y


#define STAR_MAX (200)
//---------------------------------------------------
//�a�f�֘A�̃A�^������p�}�b�s���O�f�[�^(*.VMP)
//---------------------------------------------------
typedef struct StVitmap {
	char format[8];					//VII_HITMAP_DATA0
	Sint8 dummy[4];
	Sint16 sWidth;					//�z�艡���i9660�j
	Sint16 sHeight;					//�z��c���i9660�j
	Sint8 uHantei[VMP_DATA_SIZE];	//�����蔻��o�b�t�@
} StVitmap;

//---------------------------------------------------
//�a�f�֘A�̃O���t�B�b�N�`�b�v�}�b�s���O�f�[�^(*.MAP)
//---------------------------------------------------
typedef struct MapFileData_t {
/*
	char fmt[14];
	char version;
	char num;
	char filename[16];
	Uint16 ptn[MAP_FILE_SIZE];
*/
	char fmt[8];
	char dummy[2];
	short sWidth;	//ver.4�ȍ~�Œǉ�
	short sHeight;	//ver.4�ȍ~�Œǉ�
	char version;
	char num;
	char filename[16];
	Uint16 ptn[MAP_FILE_SIZE];

} MapFileData_t;


typedef struct StRaster {
	Sint32 sSpeed;	//���炵���x(8)
	Sint32 sWidth;	//�ׂ���(32)
	Sint32 sDetail;	//���炵��MAX(4)
	gxBool bEnable;	//�L���^����(False)
}StRaster;

typedef struct StMapData
{
	//----------------------------------------------------------
	MapFileData_t tips;	//�O���t�@�C������f�[�^����͂����
	//----------------------------------------------------------
	Sint32   page;
	Sint32   x,y;
	StRaster Raster;
	gxBool   disp;
}StMapData;

class CBackGround
{
public:
	Sint32 offset_x;
	Sint32 offset_y;

	CBackGround();
	~CBackGround();

	void action();
	void Draw();
	void scroll_control( Sint32 x,Sint32 y );
	gxBool LoadVmpFile(char *filename);
	gxBool LoadVmpData(Uint8 *pData);

	void LoadMapFile(Sint32 n,gxChar *filename);
	void LoadMapData(Sint32 n, gxChar *filename);

	Sint32 get_kabetype(Sint32 x,Sint32 y);
	Sint32 get_kabetype8(Sint32 x,Sint32 y);

	void SetSkyColor(Sint32 HalfAvr, Sint32 col1,Sint32 Col2);

	void ResetFilter()
	{
		//�t�B���^�[�i�w�i�F�̐F���j�����Z�b�g����
		for(Sint32 ii=0;ii<enBgTypeMax; ii++)
		{
			SetLayerFilter( ii , 0xFFFFFFFF );
			m_bgAtribute[ii] = ATR_DFLT;
		}
	}

	void SetFilter(Sint32 col)
	{
		//�t�B���^�[�i�w�i�F�̐F���j���Z�b�g����
		for(Sint32 ii=0;ii<enBgTypeMax; ii++)
		{
			SetLayerFilter( ii , col );
			m_bgAtribute[ii] = ATR_DFLT;
		}
	}

	void SetLayerFilter(Sint32 n , Sint32 col , Sint32 sAtr=ATR_DFLT)
	{
		//�t�B���^�[�i�w�i�F�̐F���j���Z�b�g����
		m_bgFilterColor[n] = col;
		m_bgAtribute[n]    = sAtr;
	}

	void getscroll( Sint32 &x,Sint32 &y )
	{
		//�X�N���[���C�`���擾����
		x = scx_;
		y = scy_;
	}
	void setScroll100( Sint32 x,Sint32 y ){
		scx_ = x/100;
		scy_ = y/100;
	}

	void set_atari_flag(){
		no_judge_flag = gxFalse;
	}
	void cancel_atari_flag(){
		no_judge_flag = gxTrue;
	}
	void SetPos( Sint32 n,Sint32 x,Sint32 y ){
		//�e�a�f���C���[�̍��W��ݒ肷��
		if(n<0 || n>=enBgTypeMax) return;	//���S�\�t�g
		stMapData[n].x = x;
		stMapData[n].y = y;
	}

	Sint32 get_vmpdat(Sint32 x,Sint32 y)
	{
		//������u�l�o�f�[�^�𓾂�
		if(x<0 || y<0) return 0;
		if(x>=(m_sScreenX/8) || y>=(m_sScreenY/8)) return 0;
		Uint32 u8 = (Uint8)stVitmap.uHantei[ getVmpPos(x,y) ];
		Sint32 s32 = (Sint32)u8;
		return s32;
	}

	void make_vmptip(Sint32 x,Sint32 y,Sint32 kabetype=3)
	{
		//������u�l�o�f�[�^���쐬
		if(x<0 || y<0) return;
		if(x>=(m_sScreenX/8) || y>=(m_sScreenY/8)) return;
		stVitmap.uHantei[ getVmpPos(x,y) ] = kabetype;
	}

	void del_vmptip(Sint32 x,Sint32 y){
		//������u�l�o�f�[�^���폜
		if(x<0 || y<0) return;
		if(x>=(m_sScreenX/8) || y>=(m_sScreenY/8)) return;
		stVitmap.uHantei[ getVmpPos(x,y) ] = 0;
	}

	void del_maptip(Sint32 n,Sint32 x,Sint32 y){
		//�O���t�B�b�N�}�b�v�`�b�v���폜����
		if(x<0 || y<0 || x>=stMapData[n].tips.sWidth/*m_sScreenX*//MAP_BLOCK_SIZE || y>=stMapData[n].tips.sHeight/*m_sScreenY*//MAP_BLOCK_SIZE) return;
		short *q;
		q = (short*)&stMapData[n].tips.ptn[0];
		q[getMapTipPos( n, x , y )] = 0;
	}

	void set_disp(Sint32 n,gxBool flag)
	{
		stMapData[n].disp = flag;
	}


	void SetMapScreen(Sint32 x , Sint32 y )
	{
		m_sScreenX = x;
		m_sScreenY = y;
	}

	Sint32 getWidth( )
	{
		return m_sScreenX;
	}

	Sint32 getHeight( )
	{
		return m_sScreenY;
	}

	Sint32 getMapTipPos( Sint32 n , Sint32 x, Sint32 y )
	{
		return y*(stMapData[n].tips.sWidth/MAP_BLOCK_SIZE)+x;
	}

	Sint32 getVmpPos(Sint32 x, Sint32 y)
	{
		Sint32 n = y*( m_sScreenX / 8 )+x;

		return  ( n < 0 || n >= VMP_DATA_SIZE)? 0 : n;
	}

	void SetPriority(Sint32 n , Sint32 prio )
	{
		m_sChangePrio[n] = prio;
	}

	void DebugTipDraw(gxBool bFlag = gxTrue)
	{
		m_bDebugTipDraw = bFlag;
	}

	void SetRaster( Sint32 sLayer , StRaster* p_stRaster=NULL );

	gxBool IsBgObj( Sint32 x, Sint32 y);

private:

	typedef struct StStar
	{
		Sint16 x,y,layer;
	}StStar;

	//------------------------------------------------

	Sint32 m_sScreenX,m_sScreenY;
	Sint32 m_sChangePrio[enBgTypeMax];

	//------------------------------------------------

	gxBool no_judge_flag;
	Sint32 scx_,scy_;	//�X�N���[�����W
	Sint32 sx_,sy_;	//�����蔻��̍��W
	Sint32 repeat_x_goal_;
	Sint32 repeat_x_start_;
	StVitmap stVitmap;
	StMapData stMapData[enBgTypeMax];
	void debug_mode();
	void pos_cnv(Sint32 &x,Sint32 &y);
	void debugMapDraw(Sint32 x,Sint32 y,Sint32 no);
	void star_screen();
//	short stx[STAR_MAX],sty[STAR_MAX],slr[STAR_MAX];
	StStar stStar[STAR_MAX];
	void PutSprMapParts(Sint32 x,Sint32 y,Sint32 prio,Sint32 no,Sint32 bg_no,Sint32 sFilter ,Sint32 sAtr);
	void disp(Sint32 pg);

	Sint32 m_bgFilterColor[enBgTypeMax];	//�t�B���^�[�F
	Sint32 m_bgAtribute[enBgTypeMax];	//�t�B���^�[�F
	Sint32 sColor[2];
	Sint32 sHalfAvr;

	gxBool m_bDebugTipDraw;
	gxBool m_bDummy;
};


enum
{
	//-----------------------------------
	//�ǂ̃^�C�v
	//-----------------------------------

	enKabeTypeNone = 0,
	enKabeTypeBlock  = 1,
	enKabeTypeBlock2 = 3,
	enKabeTypehalf   = 2,

	enKabeTypeTriangleUL   = 12,
	enKabeTypeTriangleUR   = 13,
	enKabeTypeTriangleDR   = 14,
	enKabeTypeTriangleDL   = 15,
	enKabeTypeUnderGround  = 16,

	enKabeTypeGake         = 4,
//	enKabeTypeThrough = 4,
};

