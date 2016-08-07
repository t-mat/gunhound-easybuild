class CStageBase;
class CStage{
private:
#define STAGE_INIT (0)
	Sint32 timer_;
	Sint32 stg_no;
	Sint32 area;
	Sint32 ActSeq;
	void message_start(Sint32 n,Sint32 msg_id);
	void initseq();
	void mainseq();

	CStageBase *m_pStageBase;

	void stage_debug();
	void stage0();
		void area_init_stage00();
		void stage0_area01();
		void stage0_area02();
		void stage0_area03();
		void stage0_area04();

/*
	void stage1();
		void area_init_stage01();
		void stage1_area01();
		void stage1_area02();
		void stage1_area03();
		void stage1_area04();

	void stage2();
		void area_init_stage02();
		void stage2_area01();
		void stage2_area02();
		void stage2_area03();
*/
/*
	void stage3();
		void area_init_stage03();
		void stage3_area01();
		void stage3_area02();
		void stage3_area03();
		void stage3_area04();
		void stage3_area05();
		void stage3_area06();
	void stage4();
		void area_init_stage04();
		void stage4_area1();
		void stage4_area2();
		void stage4_area3();
		void stage4_area4();
	void stage5();
		void area_init_stage05();
		void stage5_area1();	//�󒆐�
		void stage5_area2();	//�ړ�
		void stage5_area3();	//��n����
		void stage5_area4();	//�c�C���T�C�|�b�h
		void stage5_area5();	//�V���g���i�G�X�g���[�W���j����
		void stage5_area6();	//�G�X�g���[�W����
	void stage6();
		void area_init_stage06();
		void stage6_area1();	//�V���[�e�B���O
		void stage6_area2();	//�V���[�e�B���O�I��
		void stage6_area3();	//���A����
		void stage6_area4();	//���{�X
		void stage6_area5();	//���A�Q
		void stage6_area6();	//���
		void stage6_area7();	//�΋�C
		void stage6_area8();	//�{�X��
	void stage7();
		void area_init_stage07();
		void stage7_area1();	//�V���[�e�B���O
		void stage7_area2();	//�V���[�e�B���O�I��
		void stage7_area3();	//���A����
		void stage7_area4();	//���{�X
		void stage7_area5();	//���A�Q
		void stage7_area6();	//���
		void stage7_area7();	//�΋�C
		void stage7_area8();	//���{�X��
		void stage7_area9();	//�{�X��
		void stage7_area10();	//�{�X��
		void stage7_enddemo();	//�G���f�h�f��
*/

	//-----------------------------
	//�f�o�b�O�p
	//-----------------------------
	void stage99();
		void stage99_area01();

	Sint32 scx,scy;
	gxBool flag[64];
	Sint32  val [64];
	Sint32  msgflg[64];


public:
	CStage(Sint32 no);
	~CStage();
	void action();
	void draw();
};

#define GAME_NONE (-1)
