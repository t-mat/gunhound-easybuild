//------------------------------------------------------
//
// �R���g���[���[����( XInput�g�p)
//
//------------------------------------------------------
#include <XInput.h>

#define JOYMAX (16)
#define enPlayerMax (PLAYER_MAX)
//#define GAME_BTN_MAX (32)

typedef struct PadStat_t {
	int x,y,z;
}PadStat_t;

enum GAMINGDEVICE_BTNID{
	//�Q�[�~���O�f�o�C�X�̊��蓖�Ē�`
	GDBTN_NONE = -1,
	GDBTN_BUTTON00=0,
	GDBTN_BUTTON01,
	GDBTN_BUTTON02,
	GDBTN_BUTTON03,
	GDBTN_BUTTON04,
	GDBTN_BUTTON05,
	GDBTN_BUTTON06,
	GDBTN_BUTTON07,
	GDBTN_BUTTON08,
	GDBTN_BUTTON09,
	GDBTN_BUTTON10,
	GDBTN_BUTTON11,
	GDBTN_BUTTON12,
	GDBTN_BUTTON13,
	GDBTN_BUTTON14,
	GDBTN_BUTTON15,

	GDBTN_AXIS_XA,
	GDBTN_AXIS_XS,

	GDBTN_AXIS_YA,
	GDBTN_AXIS_YS,

	GDBTN_AXIS_ZA,
	GDBTN_AXIS_ZS,

	GDBTN_ROT_XA,
	GDBTN_ROT_XS,

	GDBTN_ROT_YA,
	GDBTN_ROT_YS,

	GDBTN_ROT_ZA,
	GDBTN_ROT_ZS,

	GDBTN_POV0_U,
	GDBTN_POV0_R,
	GDBTN_POV0_D,
	GDBTN_POV0_L,

	GDBTN_POV1_U,
	GDBTN_POV1_R,
	GDBTN_POV1_D,
	GDBTN_POV1_L,

	GDBTN_POV2_U,
	GDBTN_POV2_R,
	GDBTN_POV2_D,
	GDBTN_POV2_L,

	GDBTN_POV3_U,
	GDBTN_POV3_R,
	GDBTN_POV3_D,
	GDBTN_POV3_L,

	GDBTN_SLIDER0_U,
	GDBTN_SLIDER0_D,

	GDBTN_SLIDER1_U,
	GDBTN_SLIDER1_D,

	DUMMY1=63,

	GDBTN_MAX,
};

//----------------------------------------------------
//�n�[�h�E�F�A�I�ȃp�b�h���
//----------------------------------------------------
class CGameingDevice
{
public:
	CGameingDevice();
	~CGameingDevice();
	gxBool IsPress(GAMINGDEVICE_BTNID id);
	Sint32 		_id;
	gxChar 		_hardwarename[256];	//�@��̖��O
	PadStat_t 	_caribrate;
	PadStat_t 	_caribrateR;
	gxBool 		_button[128];		//�{�^�����́iUSB�p�b�h�̃{�^���P�Q�W���j
	PadStat_t	_Axis;				//�A�i���O�X�e�B�b�N�l
	PadStat_t	_Rotation;			//�A�i���O�X�e�B�b�N�l
	Sint32 		_Pov0;				//�A�i���O�X�e�B�b�N�l
	Sint32 		_Pov1;				//�A�i���O�X�e�B�b�N�l
	Sint32 		_Pov2;				//�A�i���O�X�e�B�b�N�l
	Sint32 		_Pov3;				//�A�i���O�X�e�B�b�N�l
	Sint32 		_Slider0;			//�A�i���O�X�e�B�b�N�l
	Sint32 		_Slider1;			//�A�i���O�X�e�B�b�N�l

	//------------------------------------------------------
	//�T�|�[�g��
	//------------------------------------------------------
	Sint32 _num_pov;						//�n�b�g�X�C�b�`��
	Sint32 _num_slider;					//�X���C�_�[�R���g���[����
	gxBool _bPov[4];						//�n�b�g�X�C�b�`
	gxBool _bAxisX,_bAxisY,_bAxisZ;		//�T�|�[�g����Ă���AXIS
	gxBool _bAxisRX,_bAxisRY,_bAxisRZ;	//�T�|�[�g����Ă���AXIS
	gxBool _bSlider[2];					//�T�|�[�g����Ă���SLIDER
	gxBool _bRumble;						//�U���\���H
	Sint32  _ForceFeefBackAxis;			//�t�H�[�X�t�B�[�h�o�b�N�̎���

	//-------------------------------------------------------
	//�e���̎g�p�̉�
	//-------------------------------------------------------
	gxBool _bUseAxisX;
	gxBool _bUseAxisY;
	gxBool _bUseAxisZ;
	gxBool _bUseRotationX;
	gxBool _bUseRotationY;
	gxBool _bUseRotationZ;
	gxBool _bUsePOV0;
	gxBool _bUsePOV1;
	gxBool _bUsePOV2;
	gxBool _bUsePOV3;
	gxBool _bUseSlider0;
	gxBool _bUseSlider1;
	gxBool _bUseForceFeedBack;

	gxBool _bPS4Controller;	// gxFalse:�ʏ�̃p�b�h�AgxTrue:PS4 �R���g���[���[

private:
	gxBool IsPressButton (gxBool& btn);
	gxBool IsPressAxis   (Sint32& axis,Sint32 dir);
	gxBool IsPressPov    (Sint32 &pov ,Sint32 urdl);
	gxBool IsPressSlider (Sint32 &sldr,Sint32 dir);
};


class CXiPad
{
public:

	CXiPad();
	~CXiPad();

	void Init();
	void Action();

	Sint32 CheckDeviceButtonPress(Sint32 device_id)
	{
		//�O������Ă΂��f�o�C�X�̃{�^���̉�����
		for(int i=0;i<GDBTN_MAX;i++)
		{
			if(m_sGamingDevice[device_id].IsPress( (GAMINGDEVICE_BTNID)(i) ) )
			{
				return i;
			}
		}
		return -1;
	}

	Uint32 GetPressButton(Sint32 n = 0)
	{
		return m_uPadButtonPush[n];
	}

	//�A�i���O�X�e�B�b�N�̏���Ԃ�
	Float32* CXiPad::GetAnalogInfo(int n);

	CGameingDevice 	m_sGamingDevice[JOYMAX];		//�n�[�h�E�F�A�p�b�h���

	//�R���t�B�O�p
	gxBool KeyAssign( Sint32 sPlayerId , Sint32 nButton );
	Uint32 GetJoyIndex ( Uint32 joyBtnId , Sint32 sId = -1 );
	Uint32 GetJoyButton( Uint32 joyBtnId , Sint32 sId = -1 );

	// �I�v�V�����Ȃǂł̃L�����Z�������p�iGUNHOUND�iSteam�j�p�j
	void BackupData(Sint32 sPlayerId = 0);				// �w�肳�ꂽ�v���C���[�ԍ��̌��݂̃p�b�h�f�[�^�����ׂĕۑ�
	void RestoreData(Sint32 sPlayerId = 0);				// �w�肳�ꂽ�v���C���[�ԍ��� backupData() ���ꂽ�f�[�^�ɕ��A

	void ClearPadData(void);							// �ݒ肳��Ă���p�b�h�l�N���A�i�����ݒ肳��Ă��Ȃ���Ԃɂ���j
	void AssignDefaultPadData(void);					// �f�t�H���g�p�b�h�f�[�^�� gxLib::SaveData.Assign[][] �ɐݒ��A�p�b�h�ɐݒ� 
	void AssignPadData(void);							// ���� gxLib::SaveData.Assign[][] �ɐݒ肳��Ă���l���p�b�h�ɐݒ� 
	void SavePadData(void);								// ���݂̃p�b�h�f�[�^�� gxLib::SaveData.Assign[][] �ɔ��f

	Uint32 GetUseableDeviceNum()
	{
		return m_uGamingDeviceNum;
	}

	SINGLETON_DECLARE( CXiPad );

private:

	struct ControlerState
	{
	    XINPUT_STATE state;
	    bool bConnected;
	};
	ControlerState GamePad360[JOYMAX];

	typedef struct StAssignButtons
	{
		//�e�{�^���ɃA�T�C������Ă���gxPad���
		Uint32 uPlayer;
		Uint32 uButtonBit;

	} StAssignButtons;

	bool InitXInput();
//	void CheckForceFeedBackEffect(int device_id);
	void UpdateGamingDevices(int n);			//�n�[�h�E�F�A�����X�V

	void clearButtonData( Uint32 joyBtnId , Sint32 sPlayerID );

	void rumble();

	Uint32 m_uGamingDeviceNum;				//���o���ꂽ�p�b�h�̐�

//	LPDIRECTINPUTEFFECT   g_pEffect[JOYMAX];			//�t�H�[�X�t�B�[�h�o�b�N�Ή��G�t�F�N�g
//	HWND gHwndController;
//	gxBool m_bFirstCaribration;
	gxBool m_bUseController;
	gxBool m_bEnable[JOYMAX][GDBTN_MAX];//GAME_BTN_MAX];
	Uint32 m_uPadButtonPush[enPlayerMax];

	//�ϊ��p�̃{�^���ݒ�i�j
	//�f�o�C�XJOYMAX�̃{�^��GDBTN_MAX��nButton(BTN_A�Ȃ�)�ɊY��������
//	Uint32 m_sConvButton[JOYMAX][GDBTN_MAX];

	StAssignButtons m_stButtonAssign[JOYMAX][GDBTN_MAX];

	StAssignButtons m_stBackupButtonAssign[GDBTN_MAX];	// �o�b�N�A�b�v�p
};


