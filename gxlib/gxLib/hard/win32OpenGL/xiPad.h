//------------------------------------------------------
//
// コントローラー制御( XInput使用)
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
	//ゲーミングデバイスの割り当て定義
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
//ハードウェア的なパッド情報
//----------------------------------------------------
class CGameingDevice
{
public:
	CGameingDevice();
	~CGameingDevice();
	gxBool IsPress(GAMINGDEVICE_BTNID id);
	Sint32 		_id;
	gxChar 		_hardwarename[256];	//機器の名前
	PadStat_t 	_caribrate;
	PadStat_t 	_caribrateR;
	gxBool 		_button[128];		//ボタン入力（USBパッドのボタン１２８個分）
	PadStat_t	_Axis;				//アナログスティック値
	PadStat_t	_Rotation;			//アナログスティック値
	Sint32 		_Pov0;				//アナログスティック値
	Sint32 		_Pov1;				//アナログスティック値
	Sint32 		_Pov2;				//アナログスティック値
	Sint32 		_Pov3;				//アナログスティック値
	Sint32 		_Slider0;			//アナログスティック値
	Sint32 		_Slider1;			//アナログスティック値

	//------------------------------------------------------
	//サポート状況
	//------------------------------------------------------
	Sint32 _num_pov;						//ハットスイッチ数
	Sint32 _num_slider;					//スライダーコントロール数
	gxBool _bPov[4];						//ハットスイッチ
	gxBool _bAxisX,_bAxisY,_bAxisZ;		//サポートされているAXIS
	gxBool _bAxisRX,_bAxisRY,_bAxisRZ;	//サポートされているAXIS
	gxBool _bSlider[2];					//サポートされているSLIDER
	gxBool _bRumble;						//振動可能か？
	Sint32  _ForceFeefBackAxis;			//フォースフィードバックの軸数

	//-------------------------------------------------------
	//各軸の使用の可否
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

	gxBool _bPS4Controller;	// gxFalse:通常のパッド、gxTrue:PS4 コントローラー

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
		//外部から呼ばれるデバイスのボタンの押され具合
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

	//アナログスティックの情報を返す
	Float32* CXiPad::GetAnalogInfo(int n);

	CGameingDevice 	m_sGamingDevice[JOYMAX];		//ハードウェアパッド情報

	//コンフィグ用
	gxBool KeyAssign( Sint32 sPlayerId , Sint32 nButton );
	Uint32 GetJoyIndex ( Uint32 joyBtnId , Sint32 sId = -1 );
	Uint32 GetJoyButton( Uint32 joyBtnId , Sint32 sId = -1 );

	// オプションなどでのキャンセル処理用（GUNHOUND（Steam）用）
	void BackupData(Sint32 sPlayerId = 0);				// 指定されたプレイヤー番号の現在のパッドデータをすべて保存
	void RestoreData(Sint32 sPlayerId = 0);				// 指定されたプレイヤー番号の backupData() されたデータに復帰

	void ClearPadData(void);							// 設定されているパッド値クリア（何も設定されていない状態にする）
	void AssignDefaultPadData(void);					// デフォルトパッドデータを gxLib::SaveData.Assign[][] に設定後、パッドに設定 
	void AssignPadData(void);							// 現在 gxLib::SaveData.Assign[][] に設定されている値をパッドに設定 
	void SavePadData(void);								// 現在のパッドデータを gxLib::SaveData.Assign[][] に反映

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
		//各ボタンにアサインされているgxPad情報
		Uint32 uPlayer;
		Uint32 uButtonBit;

	} StAssignButtons;

	bool InitXInput();
//	void CheckForceFeedBackEffect(int device_id);
	void UpdateGamingDevices(int n);			//ハードウェア情報を更新

	void clearButtonData( Uint32 joyBtnId , Sint32 sPlayerID );

	void rumble();

	Uint32 m_uGamingDeviceNum;				//検出されたパッドの数

//	LPDIRECTINPUTEFFECT   g_pEffect[JOYMAX];			//フォースフィードバック対応エフェクト
//	HWND gHwndController;
//	gxBool m_bFirstCaribration;
	gxBool m_bUseController;
	gxBool m_bEnable[JOYMAX][GDBTN_MAX];//GAME_BTN_MAX];
	Uint32 m_uPadButtonPush[enPlayerMax];

	//変換用のボタン設定（）
	//デバイスJOYMAXのボタンGDBTN_MAXはnButton(BTN_Aなど)に該当させる
//	Uint32 m_sConvButton[JOYMAX][GDBTN_MAX];

	StAssignButtons m_stButtonAssign[JOYMAX][GDBTN_MAX];

	StAssignButtons m_stBackupButtonAssign[GDBTN_MAX];	// バックアップ用
};


