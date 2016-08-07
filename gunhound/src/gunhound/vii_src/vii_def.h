#ifndef _VII_DEF_H_
#define _VII_DEF_H_


//�e�N�X�`���y�[�W�̐؂蕪��
enum {
	TPG_00=16*0,//�풓256
	TPG_01=16*1,//�풓512
	TPG_02=16*2,//�t���[256
	TPG_03=16*3,//�t���[256
	TPG_04=16*4,//�t���[256/512
	TPG_05=16*5,//�t���[256/512
	TPG_06=16*6,//�t���[256/512
	TPG_07=16*7,//�t���[256/512
};


enum {
	//-------------------------------
	//�y�[�W�O:256x256�풓�G���A
	//-------------------------------
	TPG_ONMEM256_01 = TPG_00,
	TPG_ONMEM256_02,
	TPG_ONMEM256_03,
	TPG_ONMEM256_04,
	TPG_ONMEM256_05,
	TPG_ONMEM256_06,
	TPG_ONMEM256_07,
	TPG_ONMEM256_08,
	TPG_ONMEM256_09,
	TPG_ONMEM256_10,
	TPG_ONMEM256_11,
	TPG_ONMEM256_12,
	TPG_ONMEM256_13,
	TPG_ONMEM256_14,
	TPG_ONMEM256_15,
	TPG_ONMEM256_16,

	//-------------------------------
	//�y�[�W�P:512x512�풓�G���A
	//-------------------------------
	TPG_ONMEM512_01 = TPG_01,
	TPG_ONMEM512_02 = TPG_01+2,
	TPG_ONMEM512_03 = TPG_01+8,
	TPG_ONMEM512_04 = TPG_01+10,

	//--------------------------------------------------------
	//�y�[�W�Q:256x256�t���[�G���A�i�X�e�[�W�ŗL�̓G�Ȃǁj
	//--------------------------------------------------------
	TPG_ENEMY256_01 = TPG_02,
	TPG_ENEMY256_02,
	TPG_ENEMY256_03,
	TPG_ENEMY256_04,
	TPG_ENEMY256_05,
	TPG_ENEMY256_06,
	TPG_ENEMY256_07,
	TPG_ENEMY256_08,
	TPG_ENEMY256_09,
	TPG_ENEMY256_10,
	TPG_ENEMY256_11,
	TPG_ENEMY256_12,
	TPG_ENEMY256_13,
	TPG_ENEMY256_14,
	TPG_ENEMY256_15,
	TPG_ENEMY256_16,

	//--------------------------------------------------------
	//�y�[�W�R:256x256�t���[�G���A�i�G�t�F�N�g�ȂǏ��������́j
	//--------------------------------------------------------
	TPG_SMALL256_01 = TPG_03,
	TPG_SMALL256_02,
	TPG_SMALL256_03,
	TPG_SMALL256_04,
	TPG_SMALL256_05,
	TPG_SMALL256_06,
	TPG_SMALL256_07,
	TPG_SMALL256_08,
	TPG_SMALL256_09,
	TPG_SMALL256_10,
	TPG_SMALL256_11,
	TPG_SMALL256_12,
	TPG_SMALL256_13,
	TPG_SMALL256_14,
	TPG_SMALL256_15,
	TPG_SMALL256_16,

	//--------------------------------------------------------
	//�y�[�W�S:512x512�t���[�G���A�i�h�����J�[�A�V���g���Ȃǁj
	//--------------------------------------------------------
	TPG_LARGE512_01 = TPG_04,
	TPG_LARGE512_02 = TPG_04+2,
	TPG_LARGE512_03 = TPG_04+8,
	TPG_LARGE512_04 = TPG_04+10,

	//-------------------------------
	//�y�[�W�T:256x256�X�e�[�W�w�i�P
	//-------------------------------
	TPG_BG1_256_01 = TPG_05,
	TPG_BG1_256_02,
	TPG_BG1_256_03,
	TPG_BG1_256_04,
	TPG_BG1_256_05,
	TPG_BG1_256_06,
	TPG_BG1_256_07,
	TPG_BG1_256_08,
	TPG_BG1_256_09,
	TPG_BG1_256_10,
	TPG_BG1_256_11,
	TPG_BG1_256_12,
	TPG_BG1_256_13,
	TPG_BG1_256_14,
	TPG_BG1_256_15,
	TPG_BG1_256_16,

	//-------------------------------
	//�y�[�W�U:256x256�X�e�[�W�w�i�Q
	//-------------------------------
	TPG_BG2_256_01 = TPG_06,
	TPG_BG2_256_02,
	TPG_BG2_256_03,
	TPG_BG2_256_04,
	TPG_BG2_256_05,
	TPG_BG2_256_06,
	TPG_BG2_256_07,
	TPG_BG2_256_08,
	TPG_BG2_256_09,
	TPG_BG2_256_10,
	TPG_BG2_256_11,
	TPG_BG2_256_12,
	TPG_BG2_256_13,
	TPG_BG2_256_14,
	TPG_BG2_256_15,
	TPG_BG2_256_16,

	//-------------------------------
	//�y�[�W�V:256x256�X�e�[�W�w�i�R
	//-------------------------------
	TPG_BG3_256_01 = TPG_07,
	TPG_BG3_256_02,
	TPG_BG3_256_03,
	TPG_BG3_256_04,
	TPG_BG3_256_05,
	TPG_BG3_256_06,
	TPG_BG3_256_07,
	TPG_BG3_256_08,
	TPG_BG3_256_09,
	TPG_BG3_256_10,
	TPG_BG3_256_11,
	TPG_BG3_256_12,
	TPG_BG3_256_13,
	TPG_BG3_256_14,
	TPG_BG3_256_15,
	TPG_BG3_256_16,

	TPG_DETAIL_MAX,

	//--------------------------------------------------------
	//�y�[�W�S�`�V:512x512�u���[�t�B���O�p
	//--------------------------------------------------------
	TPG_BRIEFING = TPG_04,
};

enum{
	//�e�N�X�`���y�[�W�ݒ�

	//�I��������
	TPG_SYSTEM		= TPG_ONMEM256_01,
	TPG_FACE		= TPG_ONMEM256_02,
	TPG_HUMAN		= TPG_ONMEM256_03,
	TPG_VALKEN		= TPG_ONMEM256_04,
	TPG_EFFECT		= 12,//TPG_ONMEM256_05,
	TPG_CLOUD		= 14,//TPG_ONMEM256_06,
	TPG_ENEMY01		= TPG_ONMEM256_07,
	TPG_ENEMY02		= TPG_ONMEM256_08,
	TPG_ENEMY03		= TPG_ONMEM256_09,	//���C�m�X�P�ʂ̓G
	TPG_ENEMY04		= TPG_ONMEM256_10,
	TPG_ENEMY05		= TPG_ONMEM256_11,
	TPG_ENEMY06		= TPG_ONMEM256_12,
	TPG_ENEMY07		= TPG_ONMEM256_13,
	TPG_ENEMY08		= TPG_ONMEM256_14,
	TPG_SEARCHLIGHT	= TPG_ONMEM256_14,

	TPG_KANJI		= TPG_ONMEM512_01,
	TPG_COMMON		= TPG_ONMEM512_02,
	TPG_ETC			= TPG_ONMEM512_03,
	TPG_ETC2		= TPG_ONMEM512_04,

	//�X�e�[�W�ŗL�i��������ւ��j����������
	TPG_SCENEOBJ	= TPG_SMALL256_01,	//��Ƃ�
	TPG_AT			= TPG_SMALL256_02,	//�`�s�Ȃ�
	TPG_ENEMY07B	= TPG_SMALL256_03,	//����ȓG

	//�X�e�[�W�ŗL�i��������ւ��j�傫������
	TPG_ENEMY_TRAIN	= TPG_LARGE512_01,
	TPG_BUILDBORG   = TPG_LARGE512_01,	//�r���h�{�[�O
	TPG_SHUTTLE     = TPG_LARGE512_01,	//�V���g��
	TPG_MARSGEAR    = TPG_LARGE512_01,	//�}�[�Y�M�A���i

	//----------------------------------------------------------------------
	//���L�e�N�X�`���y�[�W(�擪���珇�ɓǂ܂��̂ōŌ�̃y�[�W���s���ɂȂ�)
	//----------------------------------------------------------------------
	TPG_BG			= TPG_BG1_256_01,
	TPG_ANM			= TPG_BRIEFING,
};

enum {
	//���ʉ�
	SE_CHAKUCHI=2,
	SE_DASH,
	SE_ENEMY_OUT,
	SE_JUMP,
	SE_PUNCH,
	SE_VULCAN1,
	SE_VULCAN2,
	SE_MISSILE,
	SE_KIRIKAE,
	SE_ITEMGET,
	SE_POWERUP,
	SE_HPGET,
	SE_KUSUDAMA,
	SE_SHIELD,
	SE_JUMP_START,
	SE_WINDOW_OPEN,
	SE_WINDOW_CLOSE,
	SE_WINDOW_CLICK,
	SE_STAGECLEAR,
	SE_STARSHIP,
	SE_EXPLOSION,
	SE_ELEVATOR,
	SE_ELEVATOR2,
	SE_BOSSSTART,
	SE_DASH_NOW,

	SE_LASER,
	SE_NAPALM1,
	SE_NAPALM2,
	SE_FALL,
	SE_3WAY,
	SE_KUSUDAMA2,
	SE_PLAYEROUT,
	SE_VIIHIT,
	SE_ENEBEAM,
	SE_ENEMISSILE,
	SE_DANGER,
	SE_BOMB_SMALL,
	SE_WALKNOISE,
	SE_TIBIYARARE,
	SE_PUNCHHIT,
	SE_VERSIS_LANDING,

	SE_STAGE_A,
	SE_STAGE_B,

	SE_SLASHPUNCH = SE_WALKNOISE,
	SE_ETC        = SE_WALKNOISE,

	SE_CARRYOFF   = SE_STAGE_B,
	SE_BIGBEAM   = SE_STAGE_B,
	SE_SILEN   = SE_STAGE_B,
};

enum{
	//�T�E���h�ԍ�
	SE_START,
};


enum {
//#define enBgTypeBack   (1)
//#define enBgTypeRear   (2)
//#define enBgTypeFrnt  (3)

	enBgTypeStar,
	enBgTypeBack,
	enBgTypeRear,
	enBgTypeFrnt,
	enBgTypeMax,
};

#endif
