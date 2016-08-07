//------------------------------------------------------
//
// �R���g���[���[����( Direct Input version8�g�p)
//
//------------------------------------------------------
#include <gxLib.h>
#include <gxLib/gx.h>
#include <gxLib/gxPadManager.h>
#include "xiPad.h"

#pragma comment(lib,"xinput.lib ")

#define ILLEGAL_BTNID ( 0xFFFFFFFF )

int   g_NumJoyStickDeviceNum;						//�W���C�X�e�B�b�N���o��
int   g_NowSettingDeviceNo;							//���ݐݒ蒆�̃f�o�C�XID

SINGLETON_DECLARE_INSTANCE( CXiPad )
#define AXIS_RANGE (65535>>1)

static Sint32 JoyIndexTbl[]=
{
	JOY_U,
	JOY_R,
	JOY_D,
	JOY_L,

	BTN_1,
	BTN_2,
	BTN_3,
	BTN_4,

	BTN_5,
	BTN_6,
	BTN_7,
	BTN_8,

	BTN_9,
	BTN_10,
	BTN_11,
	BTN_12,

	BTN_13,
	BTN_14,
	BTN_15,
	BTN_16,

	BTN_17,
	BTN_18,
	BTN_19,
	BTN_20,

	BTN_21,
	BTN_22,
	BTN_23,
	BTN_24,

	BTN_25,
	BTN_26,
	BTN_27,
	BTN_28,
};

enum {
	enItemDButonId = 0,	//�f�W�^���{�^���ԍ� 1�`16
	enItemJoyBtnId = 0,	//�f�o�C�X�̃{�^���ԍ� JOY_U�`
	enItemDeviceId = 1,	//�f�o�C�X��ID �p�b�h�@��̔ԍ�
};

//-----------------------------------------------------------------
//�Q�[���p�b�h�֘A�����Ǘ�
//-----------------------------------------------------------------
CXiPad::CXiPad()
{
	gxLib::DebugLog("�yControl�zXi�p�b�h�f�o�C�X���������J�n");

	//m_bFirstCaribration = gxTrue;
	m_bUseController = gxTrue;

	for(Uint32 ii=0;ii<JOYMAX;ii++)
	{
		for(Uint32 jj=0;jj<GDBTN_MAX;jj++)
		{
			m_bEnable[ii][jj] = gxTrue;
		}
	}

	g_NumJoyStickDeviceNum = m_uGamingDeviceNum = 0;		//�f�o�C�X�̐��̓[��
	g_NowSettingDeviceNo   = 0;

	m_uPadButtonPush[0] = 0x00000000;
	m_uPadButtonPush[1] = 0x00000000;

	for(Uint32 kk=0;kk<JOYMAX;kk++)
	{
		for(Uint32 jj=0;jj<GDBTN_MAX;jj++)
		{
			m_stButtonAssign[kk][jj].uPlayer = 0;
			m_stButtonAssign[kk][jj].uButtonBit = 0;// ILLEGAL_BTNID;
		}
	}

	//�R���t�B�O�f�[�^����̏����߂�
	AssignPadData();

}

CXiPad::~CXiPad()
{
	//--------------------------------------------------
	//�W���C�p�b�h�f�o�C�X���������
	//--------------------------------------------------

	//�R���t�B�O�f�[�^�������߂�

	for(Sint32 id=0;id<PLAYER_MAX; id++)
	{
		for(Sint32 jj=0;jj<BTN_MAX;jj++)
		{
			for(Sint32 id=0;id<PLAYER_MAX;id++)
			{
				gxLib::SaveData.Assign[ id ][ jj ].uDevice = ILLEGAL_BTNID;
				gxLib::SaveData.Assign[ id ][ jj ].uButton = ILLEGAL_BTNID;
			}
		}
	}

	// ���݂̃p�b�h�f�[�^�� gxLib::SaveData.Assign[][] �ɔ��f
	SavePadData();
}


//DirectInput�̏�����
void CXiPad::Init()
{
	InitXInput();
	gxLib::DebugLog("�yControl�zXI�p�b�h�f�o�C�X�̏���������");
}


bool CXiPad::InitXInput()
{
	//---------------------------------------------------------
	//DirectInput�I�u�W�F�N�g���쐬����
	//---------------------------------------------------------

	g_NumJoyStickDeviceNum = 0;

    DWORD dwResult;

    for( DWORD i = 0; i < JOYMAX; i++ )
    {
        dwResult = XInputGetState( i, &GamePad360[i].state );

        if( dwResult == ERROR_SUCCESS )
		{
            GamePad360[i].bConnected = gxTrue;
			g_NumJoyStickDeviceNum ++;
		}
        else
		{
            GamePad360[i].bConnected = gxFalse;
		}
    }

	if(g_NumJoyStickDeviceNum == 0)
	{
		//JOYSTICK���o�ł���
        return gxFalse;
    }

	for(int n=0;n<g_NumJoyStickDeviceNum;n++)
	{
		g_NowSettingDeviceNo = n;
	}

	//���o���ꂽ�W���C�p�b�h�����L�^
	m_uGamingDeviceNum = g_NumJoyStickDeviceNum;

	gxLib::DebugLog("�yControl�z�g�p�\�ȃp�b�h����%d",m_uGamingDeviceNum);

	return gxTrue;
}

void CXiPad::Action()
{

	//���t���[���ʂ鏈��
	Uint32 i;

	m_uPadButtonPush[0] = 0x00000000;
	m_uPadButtonPush[1] = 0x00000000;

	for(i=0;i<m_uGamingDeviceNum;i++)
	{
		//�g�p���Ȃ��@�\�ɂ��Đ�������

		bool bFlag;
		bFlag = true;

		if( !m_bUseController )
		{
			//���������R���g���[���[���g�p���Ȃ��ꍇ�ɂ͑S�Ẵ{�^����F�������Ȃ�
			bFlag = gxFalse;
		}

	    DWORD dwResult;

        dwResult = XInputGetState( i, &GamePad360[i].state );

        if( dwResult != ERROR_SUCCESS )
		{
            GamePad360[i].bConnected = gxFalse;
		}

		if( !GamePad360[i].bConnected ) continue;

		m_sGamingDevice[i]._bUseAxisX			= (m_bEnable[i][0]  )? bFlag : gxFalse;
		m_sGamingDevice[i]._bUseAxisY			= (m_bEnable[i][1]  )? bFlag : gxFalse;
		m_sGamingDevice[i]._bUseAxisZ			= (m_bEnable[i][2]  )? bFlag : gxFalse;
		m_sGamingDevice[i]._bUseRotationX		= (m_bEnable[i][3]  )? bFlag : gxFalse;
		m_sGamingDevice[i]._bUseRotationY		= (m_bEnable[i][4]  )? bFlag : gxFalse;
		m_sGamingDevice[i]._bUseRotationZ		= (m_bEnable[i][5]  )? bFlag : gxFalse;
		m_sGamingDevice[i]._bUsePOV0			= (m_bEnable[i][6]  )? bFlag : gxFalse;
		m_sGamingDevice[i]._bUsePOV1			= (m_bEnable[i][7]  )? bFlag : gxFalse;
		m_sGamingDevice[i]._bUsePOV2			= (m_bEnable[i][8]  )? bFlag : gxFalse;
		m_sGamingDevice[i]._bUsePOV3			= (m_bEnable[i][9]  )? bFlag : gxFalse;
//		m_sGamingDevice[i]._bUseSlider0		= (m_bEnable[i][10] )? bFlag : gxFalse;
//		m_sGamingDevice[i]._bUseSlider1		= (m_bEnable[i][11] )? bFlag : gxFalse;
		m_sGamingDevice[i]._bUseForceFeedBack	= (m_bEnable[i][10] )? bFlag : gxFalse;

		UpdateGamingDevices(i);
	}

	rumble();

	if( CGameGirl::GetInstance()->IsPadConfigMode() )
	{
		CGXPadConfig::GetInstance()->Action();

		if( CGXPadConfig::GetInstance()->IsConfigFinish() )
		{
			CGXPadConfig::GetInstance()->Init();
			CGameGirl::GetInstance()->SetPadConfig(gxFalse);
		}

		CGXPadConfig::GetInstance()->Draw();
	}

}



Float32* CXiPad::GetAnalogInfo(int n)
{
	//�A�i���O����Ԃ�

	static Float32 AnalogData[32]={
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
	};

	Sint32 joyIndex;
	Sint32 btnIndex;

	CGameingDevice *pGameDevice;

	if( n == 0 )
	{
		//�A�i���O�P�̌��o
		joyIndex = GetJoyIndex(BTN_ANALOG1);
		btnIndex = GetJoyButton(BTN_ANALOG1);
	}
	else
	{
		//�A�i���O�Q�̌��o
		joyIndex = GetJoyIndex(BTN_ANALOG2);
		btnIndex = GetJoyButton(BTN_ANALOG2);
	}

	pGameDevice = &m_sGamingDevice[joyIndex];

	switch( btnIndex ){
	case GDBTN_AXIS_XA:
	case GDBTN_AXIS_XS:
	case GDBTN_AXIS_YA:
	case GDBTN_AXIS_YS:
		AnalogData[0] = 1.f*pGameDevice->_Axis.x/65535.0f;
		AnalogData[1] = 1.f*pGameDevice->_Axis.y/65535.0f;
		break;
	case GDBTN_ROT_XA:
	case GDBTN_ROT_XS:
	case GDBTN_ROT_YA:
	case GDBTN_ROT_YS:
		AnalogData[0] = 1.f*pGameDevice->_Rotation.x/65535.0f;
		AnalogData[1] = 1.f*pGameDevice->_Rotation.y/65535.0f;
		break;
	case GDBTN_AXIS_ZA:
	case GDBTN_AXIS_ZS:
	case GDBTN_ROT_ZA:
	case GDBTN_ROT_ZS:
		AnalogData[0] = 1.f*pGameDevice->_Rotation.z/65535.0f;
		AnalogData[1] = 1.f*pGameDevice->_Axis.z/65535.0f;
		break;
	}

	return AnalogData;
}

void CXiPad::UpdateGamingDevices(int n)
{
	//-----------------------------------------
	//�n�[�h�E�F�A�����X�V����
	//-----------------------------------------
	CGameingDevice *pGameDevice = &m_sGamingDevice[n];

	if( !GamePad360[n].bConnected ) return;

	WORD Buttons = GamePad360[n].state.Gamepad.wButtons;

	//��Ԃ̎擾�ɐ�������

	//---------------------------------
	//AXIS�ɂ���
	//---------------------------------
/*
	if( m_bFirstCaribration )
	{
		m_bFirstCaribration = gxFalse;
		pGameDevice->_caribrate.x = js.lX;
		pGameDevice->_caribrate.y = js.lY;
		pGameDevice->_caribrate.z = js.lZ;

		pGameDevice->_caribrateR.z = js.lRz;
	}
*/

	pGameDevice->_Axis.x 		= GamePad360[n].state.Gamepad.sThumbLX;
	pGameDevice->_Axis.y 		=  GamePad360[n].state.Gamepad.sThumbLY;
	pGameDevice->_Axis.z 		= (Buttons & XINPUT_GAMEPAD_LEFT_THUMB)? 1 : 0;

	pGameDevice->_Rotation.x 	= GamePad360[n].state.Gamepad.sThumbRX;
	pGameDevice->_Rotation.y 	= GamePad360[n].state.Gamepad.sThumbRY;
	pGameDevice->_Rotation.z 	= (Buttons & XINPUT_GAMEPAD_RIGHT_THUMB)? 1 : 0;

	//---------------------------------
	//�X���C�_�[�R���g���[���ɂ���
	//---------------------------------
	pGameDevice->_Slider0 = GamePad360[n].state.Gamepad.bLeftTrigger;
	pGameDevice->_Slider1 = GamePad360[n].state.Gamepad.bRightTrigger;

	//---------------------------------
	//�n�b�g�X�C�b�`�ɂ���
	//---------------------------------
	pGameDevice->_Pov0 = 0;
	if( Buttons & XINPUT_GAMEPAD_DPAD_UP    ) pGameDevice->_Pov0 |= JOY_U;
	if( Buttons & XINPUT_GAMEPAD_DPAD_RIGHT ) pGameDevice->_Pov0 |= JOY_R;
	if( Buttons & XINPUT_GAMEPAD_DPAD_DOWN  ) pGameDevice->_Pov0 |= JOY_D;
	if( Buttons & XINPUT_GAMEPAD_DPAD_LEFT  ) pGameDevice->_Pov0 |= JOY_L;

	pGameDevice->_Pov1 = 0;
	pGameDevice->_Pov2 = 0;
	pGameDevice->_Pov3 = 0;

	pGameDevice->_button[0] = ( Buttons & XINPUT_GAMEPAD_A )			 ? 1:0;
	pGameDevice->_button[1] = ( Buttons & XINPUT_GAMEPAD_B )			 ? 1:0;
	pGameDevice->_button[2] = ( Buttons & XINPUT_GAMEPAD_X )			 ? 1:0;
	pGameDevice->_button[3] = ( Buttons & XINPUT_GAMEPAD_Y )			 ? 1:0;
	pGameDevice->_button[4] = ( Buttons & XINPUT_GAMEPAD_START )		 ? 1:0;
	pGameDevice->_button[5] = ( Buttons & XINPUT_GAMEPAD_BACK )			 ? 1:0;
	pGameDevice->_button[6] = ( Buttons & XINPUT_GAMEPAD_LEFT_THUMB )	 ? 1:0;		//���A�i���O�����L�[���p�b�h���ɉ������ꍇ
	pGameDevice->_button[7] = ( Buttons & XINPUT_GAMEPAD_RIGHT_THUMB )	 ? 1:0;		//�E�A�i���O�����L�[���p�b�h���ɉ������ꍇ
	pGameDevice->_button[8] = ( Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER ) ? 1:0;
	pGameDevice->_button[9] = ( Buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER )? 1:0;

	for(Uint32 ii=0;ii<GDBTN_MAX;ii++)
	{
		if(pGameDevice->IsPress( (GAMINGDEVICE_BTNID)ii ))
		{
			Sint32 sPlayer = m_stButtonAssign[n][ii].uPlayer;
			if( m_stButtonAssign[n][ii].uButtonBit != ILLEGAL_BTNID )
			{
				m_uPadButtonPush[sPlayer] |= m_stButtonAssign[n][ii].uButtonBit;
			}
/*
			//�A�i���O�X�e�B�b�N�̓��͂��\���L�[�Ƌ��L����
			switch(ii) {
			case GDBTN_AXIS_XA:	m_uPadButtonPush[sPlayer] |= m_stButtonAssign[n][GDBTN_POV0_R].uButtonBit;	break;
			case GDBTN_AXIS_XS:	m_uPadButtonPush[sPlayer] |= m_stButtonAssign[n][GDBTN_POV0_L].uButtonBit;	break;
			case GDBTN_AXIS_YA:	m_uPadButtonPush[sPlayer] |= m_stButtonAssign[n][GDBTN_POV0_U].uButtonBit;	break;
			case GDBTN_AXIS_YS:	m_uPadButtonPush[sPlayer] |= m_stButtonAssign[n][GDBTN_POV0_D].uButtonBit;	break;
			}
*/
		}
	}
}


//*************************************************************************************************************
//�f�o�C�X��Ԃ��Ǘ�
//*************************************************************************************************************

CGameingDevice::CGameingDevice()
{
	_num_pov = 0;
	_num_slider = 0;

	memset( &_Axis , 0x00 , sizeof(PadStat_t) );
	memset( &_Rotation , 0x00 , sizeof(PadStat_t) );
	memset( &_caribrate , 0x00 , sizeof(PadStat_t) );
	sprintf(_hardwarename, "Unconnected");

	_Pov0 = 0;
	_Pov1 = 0;
	_Pov2 = 0;
	_Pov3 = 0;
	_Slider0 = 0;
	_Slider1 = 0;

	//�T�|�[�g�󋵂�������
	_bPov[0]    = gxFalse;			//�n�b�g�X�C�b�`0
	_bPov[1]    = gxFalse;			//�n�b�g�X�C�b�`1
	_bPov[2]    = gxFalse;			//�n�b�g�X�C�b�`2
	_bPov[3]    = gxFalse;			//�n�b�g�X�C�b�`3
	_num_slider = 0;				//�X���C�_�[�R���g���[����
	_bAxisX =
	_bAxisY =
	_bAxisZ = gxFalse;				//�T�|�[�g����Ă���AXIS
	_bAxisRX =
	_bAxisRY =
	_bAxisRZ = gxFalse;				//�T�|�[�g����Ă���AXIS
	_bRumble = gxFalse;				//�U���\���H


	//�g�p���邩�ǂ���(��{�I�ɂ͑S���g�p���邱�Ƃɂ���)
	_bUseAxisX
	= _bUseAxisY
	= _bUseAxisZ
	= _bUseRotationX
	= _bUseRotationY
	= _bUseRotationZ
	= _bUsePOV0
	= _bUsePOV1
	= _bUsePOV2
	= _bUsePOV3
	= _bUseSlider0
	= _bUseSlider1
	= _bUseForceFeedBack = true;

	for(int i=0;i<128;i++) {
		_button[i] = gxFalse;
	}

	_bSlider[0] = _bSlider[1] = gxFalse;
	_ForceFeefBackAxis = 0;

	_bPS4Controller = gxFalse;
}


CGameingDevice::~CGameingDevice()
{
	
	
}

gxBool CGameingDevice::IsPress(GAMINGDEVICE_BTNID id)
{
	//------------------------------------------------------
	//�{�^����������Ă��邩�ǂ����f�W�^�����͂ɕϊ����ĕԂ�
	//------------------------------------------------------

	switch(id) {
	//BUTTON
	case GDBTN_BUTTON00:
	case GDBTN_BUTTON01:
	case GDBTN_BUTTON02:
	case GDBTN_BUTTON03:
	case GDBTN_BUTTON04:
	case GDBTN_BUTTON05:
	case GDBTN_BUTTON06:
	case GDBTN_BUTTON07:
	case GDBTN_BUTTON08:
	case GDBTN_BUTTON09:
	case GDBTN_BUTTON10:
	case GDBTN_BUTTON11:
	case GDBTN_BUTTON12:
	case GDBTN_BUTTON13:
	case GDBTN_BUTTON14:
	case GDBTN_BUTTON15:
		return IsPressButton(_button[id-GDBTN_BUTTON00]);

	//AXIS
	case GDBTN_AXIS_XA:
	case GDBTN_AXIS_XS:
		if(!_bUseAxisX) return gxFalse;
		return IsPressAxis(_Axis.x,id-GDBTN_AXIS_XA);
	case GDBTN_AXIS_YA:
	case GDBTN_AXIS_YS:
		if(!_bUseAxisY) return gxFalse;
		return IsPressAxis(_Axis.y,id-GDBTN_AXIS_YA);
	case GDBTN_AXIS_ZA:
	case GDBTN_AXIS_ZS:
		if(!_bUseAxisZ) return gxFalse;
		return IsPressAxis(_Axis.z,id-GDBTN_AXIS_ZA);

	//ROTATION
	case GDBTN_ROT_XA:
	case GDBTN_ROT_XS:
		if(!_bUseRotationX) return gxFalse;
		return IsPressAxis(_Rotation.x,id-GDBTN_ROT_XA);
	case GDBTN_ROT_YA:
	case GDBTN_ROT_YS:
		if(!_bUseRotationY) return gxFalse;
		return IsPressAxis(_Rotation.y,id-GDBTN_ROT_YA);
	case GDBTN_ROT_ZA:
	case GDBTN_ROT_ZS:
		if(!_bUseRotationZ) return gxFalse;
		return IsPressAxis(_Rotation.z,id-GDBTN_ROT_ZA);

	//POV
	case GDBTN_POV0_U:
	case GDBTN_POV0_R:
	case GDBTN_POV0_D:
	case GDBTN_POV0_L:
		if(!_bUsePOV0) return gxFalse;
		return IsPressPov(_Pov0,id-GDBTN_POV0_U);

	case GDBTN_POV1_U:
	case GDBTN_POV1_R:
	case GDBTN_POV1_D:
	case GDBTN_POV1_L:
		if(!_bUsePOV1) return gxFalse;
		return IsPressPov(_Pov1,id-GDBTN_POV1_U);

	case GDBTN_POV2_U:
	case GDBTN_POV2_R:
	case GDBTN_POV2_D:
	case GDBTN_POV2_L:
		if(!_bUsePOV2) return gxFalse;
		return IsPressPov(_Pov2,id-GDBTN_POV2_U);

	case GDBTN_POV3_U:
	case GDBTN_POV3_R:
	case GDBTN_POV3_D:
	case GDBTN_POV3_L:
		if(!_bUsePOV3) return gxFalse;
		return IsPressPov(_Pov3,id-GDBTN_POV3_U);

	//SLIDER
	case GDBTN_SLIDER0_U:
	case GDBTN_SLIDER0_D:
		if(!_bUseSlider0) return gxFalse;
		return IsPressSlider(_Slider0,id-GDBTN_SLIDER0_U);

	case GDBTN_SLIDER1_U:
	case GDBTN_SLIDER1_D:
		if(!_bUseSlider1) return gxFalse;
		return IsPressSlider(_Slider1,id-GDBTN_SLIDER1_U);
	}

	return gxFalse;
}

gxBool CGameingDevice::IsPressButton( gxBool& btn)
{
	//�{�^����������Ă��邩�ǂ����`�F�b�N

	return (btn)? gxTrue : gxFalse;
}

gxBool CGameingDevice::IsPressAxis( Sint32& axis , Sint32 dir)
{
	//�A�i���O�X�e�B�b�N���|����Ă��邩�ǂ����`�F�b�N
	int d = 0;

	if(axis < -AXIS_RANGE/2) d = -1;
	if(axis >  AXIS_RANGE/2) d =  1;

	if(dir == 0 && 	d == 1)  return gxTrue;
	if(dir == 1 && 	d == -1) return gxTrue;

	return gxFalse;
}

gxBool CGameingDevice::IsPressPov( Sint32 &pov , Sint32 urdl)
{
	//---------------------------------------------------------------------
	//POV�����͂���Ă��邩�ǂ���
	//---------------------------------------------------------------------

	//---------------------------------------------------------------------
	//������Ă���{�^���ƃ��N�G�X�g��������v����Ή�����Ă��邱�Ƃɂ���
	//---------------------------------------------------------------------
	switch(urdl){
	case 0:	//��
		if(pov & JOY_U) return gxTrue;
		break;
	case 1:	//�E
		if(pov & JOY_R) return gxTrue;
		break;
	case 2:	//��
		if(pov & JOY_D) return gxTrue;
		break;
	case 3:	//��
		if(pov & JOY_L) return gxTrue;
		break;
	}

	return gxFalse;
}

gxBool CGameingDevice::IsPressSlider( Sint32 &sldr , Sint32 dir)
{
	//�O�`�Q�T�T�̒l���Ԃ��Ă���

	if( sldr > 200 ) return gxTrue;

	return gxFalse;
}


Uint32 CXiPad::GetJoyButton( Uint32 joyBtnId , Sint32 sID )
{
	//�T������gx�{�^��ID���g�p���Ă���f�o�C�X�{�^����T��
	//���ׂẴf�o�C�X����{��

	for(Uint32 jj=0;jj<JOYMAX;jj++)
	{
		for(Uint32 ii=0;ii<GDBTN_MAX;ii++)
		{
			//if( m_sConvButton[ jj ][ ii ] == joyBtnId )
			if( m_stButtonAssign[ jj ][ ii ].uButtonBit == joyBtnId )
			{
				if( sID == -1 || sID == m_stButtonAssign[ jj ][ ii ].uPlayer )
				{
					return ii;
				}
			}
		}
	}

	return ILLEGAL_BTNID;
}


Uint32 CXiPad::GetJoyIndex( Uint32 joyBtnId , Sint32 sID )
{
	//--------------------------------------------
	//Player sID �̃{�^���ijoyBtnId�j���A�ǂ�
	//�ǂ̃W���C�p�b�h���g�����Ԃ�
	//�T������gx�{�^��ID���g�p���Ă���f�o�C�X�C���f�b�N�X��Ԃ�
	//--------------------------------------------

	for(Uint32 jj=0;jj<JOYMAX;jj++)
	{
		for(Uint32 ii=0;ii<GDBTN_MAX;ii++)
		{
			if( m_stButtonAssign[ jj ][ ii ].uButtonBit == joyBtnId )
			{
				if( sID == -1 || sID == m_stButtonAssign[ jj ][ ii ].uPlayer )
				{
					return  jj;
				}
			}
		}
	}

	return ILLEGAL_BTNID;
}


void CXiPad::clearButtonData( Uint32 joyBtnId , Sint32 sPlID )
{
	//�w��̃p�b�h�r�b�g������
	//�R���t�B�O�̍ۂɉߋ��̏�����������

	for(Uint32 jj=0;jj<JOYMAX;jj++)
	{
		for(Uint32 ii=0;ii<GDBTN_MAX;ii++)
		{
			if( m_stButtonAssign[ jj ][ ii ].uButtonBit == joyBtnId  )
			{
			
				if( sPlID == m_stButtonAssign[ jj ][ ii ].uPlayer )
				{
					m_stButtonAssign[ jj ][ ii ].uButtonBit  = 0x00;
				}
			}
		}
	}
}


//-------------------------------------------------------------
//
// �p�b�h�R���t�B�O
//
//-------------------------------------------------------------

gxBool CXiPad::KeyAssign( Sint32 sPlayerId , Sint32 nButton )
{
	//---------------------------------------------
	//�v���C���[�ԍ��ƁA�{�^����n���Đݒ���s��
	//---------------------------------------------

	static Sint32 m_sConfigSeq = 0;
	static gxBool m_bNoPress = gxFalse;

	switch(m_sConfigSeq){
	case 0:		//������
		m_sConfigSeq = 10;
		m_bNoPress = gxFalse;
		gxLib::DebugLog("�yPadConfig�z������");
		break;

	case 10:
		if( m_bNoPress )
		{
			m_sConfigSeq = 100;
			m_bNoPress = gxFalse;
		}
		break;

	case 100:	//�{�^���������̑҂�

		if( !m_bNoPress )
		{
			//�{�^�������ꂽ
			gxLib::DebugLog("�yPadConfig�z�{�^���͂Ȃ����̑҂�");
			m_sConfigSeq = 200;
		}
		break;

	case 200:	//�{�^���͂Ȃ����̑҂�

		if( m_bNoPress )
		{
			//�{�^���͂Ȃ��ꂽ
			m_sConfigSeq = 0;
			return gxTrue;
		}
		break;

	case 900:		//�I��
		break;
	}

	gxBool bDecision = gxFalse;
	gxBool bPress = gxFalse;

	for(Uint32 jj=0;jj<m_uGamingDeviceNum;jj++)
	{
		//�Q�[���f�o�C�X�̂ǂ̃{�^����������Ă��邩�`�F�b�N����

		for(Sint32 ii=0;ii<GDBTN_MAX;ii++)
		{
			if( m_sGamingDevice[jj].IsPress( (GAMINGDEVICE_BTNID)ii ) )
			{
				//������Ă���{�^�������o����

				bPress = gxTrue;

				if( m_sConfigSeq == 100 )
				{
					bDecision = gxTrue;

					clearButtonData( nButton , sPlayerId );

					//�f�o�C�X���̃{�^������nButton(BTN_A�Ȃ�)�ɊY��������

					//m_sConvButton[jj][ ii ] = nButton;
					m_stButtonAssign[jj][ ii ].uPlayer = sPlayerId;
					m_stButtonAssign[jj][ ii ].uButtonBit = nButton;
					gxLib::DebugLog("�yPadConfig�z[%d-%d] (PL:%d / BTN:0x%02X)", jj , ii , sPlayerId , nButton );
				}
				break;
			}
		}
		if( bDecision ) break;
	}

	if( !bPress )
	{
		m_bNoPress = gxTrue;
	}
	else
	{
		m_bNoPress = gxFalse;
	}

	return gxFalse;
}

// �I�v�V�����Ȃǂł̃L�����Z�������p�iGUNHOUND�iSteam�j�p�j
// �w�肳�ꂽ�v���C���[�ԍ��̌��݂̃p�b�h�f�[�^�����ׂĕۑ�
void CXiPad::BackupData(Sint32 sPlayerId/* = 0*/)
{
	for(Sint32 i = 0; i < GDBTN_MAX; ++i) {
		this->m_stBackupButtonAssign[i] = this->m_stButtonAssign[sPlayerId][i];
	}
}

// �w�肳�ꂽ�v���C���[�ԍ��� backupData() ���ꂽ�f�[�^�ɕ��A
void CXiPad::RestoreData(Sint32 sPlayerId/* = 0*/)
{
	for(Sint32 i = 0; i < GDBTN_MAX; ++i) {
		this->m_stButtonAssign[sPlayerId][i] = this->m_stBackupButtonAssign[i];
	}
}

// �ݒ肳��Ă���p�b�h�l�N���A�i�����ݒ肳��Ă��Ȃ���Ԃɂ���j
void CXiPad::ClearPadData(void)
{
	for(Uint32 id = 0; id < PLAYER_MAX; id++)
	{
		for(Uint32 ii = 0;ii < ARRAY_LENGTH(JoyIndexTbl); ii++)
		{
			gxLib::SaveData.Assign[ id ][ ii ].uButton = ILLEGAL_BTNID;
			gxLib::SaveData.Assign[ id ][ ii ].uDevice = ILLEGAL_BTNID;
		}
	}

	{
		for(Uint32 id = 0; id < JOYMAX; id++)
		{
			for(Uint32 ii = 0;ii < GDBTN_MAX; ii++)
			{
				m_stButtonAssign[ id ][ ii ].uPlayer = 0;
				m_stButtonAssign[ id ][ ii ].uButtonBit = ILLEGAL_BTNID;
			}
		}
	}
}

// �f�t�H���g�p�b�h�f�[�^�� gxLib::SaveData.Assign[][] �ɐݒ��A�p�b�h�ɐݒ� 
void CXiPad::AssignDefaultPadData(void)
{
	static const Uint8 s_default_padconfig[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x13, 0x00, 0x10, 0x00, 0x12, 0x00, 0x11, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x00, 0x02,
		0x00, 0x03, 0xff, 0xff, 0x00, 0x05, 0x00, 0x04, 0xff, 0xff, 0xff, 0xff, 0x00, 0x06, 0x00, 0x07,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	};

	size_t dataSize = sizeof(s_default_padconfig);
	if(sizeof(gxLib::StSaveData) != dataSize) {	// �T�C�Y���Ⴄ�̂�
		return;									// �����ŏI��
	}

	// �f�t�H���g�f�[�^�ݒ�
	memcpy(&gxLib::SaveData, s_default_padconfig, dataSize);
	AssignPadData();
}

// ���� gxLib::SaveData.Assign[][] �ɐݒ肳��Ă���l���p�b�h�ɐݒ� 
void CXiPad::AssignPadData(void)
{
	for(Uint32 id=0;id<PLAYER_MAX;id++)
	{
		for(Uint32 ii=0;ii<ARRAY_LENGTH(JoyIndexTbl);ii++)
		{
	//		Sint32 BtnID    = gxLib::SaveData.m_sPadConfig[ii][enItemDButonId];
	//		Sint32 joyIndex = gxLib::SaveData.m_sPadConfig[ii][enItemDeviceId];

			//�ǂ̃f�o�C�X�̂ǂ̃{�^����gx�{�^�����g�����H
			//m_sConvButton[ joyIndex ][ BtnID ] = JoyIndexTbl[ii];

			Sint32 BtnID    = gxLib::SaveData.Assign[ id ][ ii ].uButton;
			Sint32 joyIndex = gxLib::SaveData.Assign[ id ][ ii ].uDevice;

			if( BtnID    == 0xff ) continue;
			if( joyIndex == 0xff ) continue;
			m_stButtonAssign[ joyIndex ][ BtnID ].uPlayer = id;
			m_stButtonAssign[ joyIndex ][ BtnID ].uButtonBit = JoyIndexTbl[ii];

//			gxLib::DebugLog( "[PAD(%d)] %02d = %02d - %02x " , id , JoyIndexTbl[ii] , joyIndex , BtnID );
		}
	}
}

// ���݂̃p�b�h�f�[�^�� gxLib::SaveData.Assign[][] �ɔ��f
void CXiPad::SavePadData(void)
{
	for(Sint32 id=0;id<PLAYER_MAX; id++)
	{
		for(Uint32 n=0;n<ARRAY_LENGTH(JoyIndexTbl);n++)
		{
			//gxLib::SaveData.m_sPadConfig[ n ][ enItemDButonId ] = GetJoyButton( JoyIndexTbl[n] );
			//gxLib::SaveData.m_sPadConfig[ n ][ enItemDeviceId ] = GetJoyIndex ( JoyIndexTbl[n] );
			gxLib::SaveData.Assign[ id ][ n ].uDevice = GetJoyIndex  ( JoyIndexTbl[n] , id );
			gxLib::SaveData.Assign[ id ][ n ].uButton = GetJoyButton ( JoyIndexTbl[n] , id );

			if( gxLib::SaveData.Assign[ id ][ n ].uDevice != ILLEGAL_BTNID )
			{
//				gxLib::DebugLog( "[PAD(%d)] %02d = %02d - %02x " , id , n , gxLib::SaveData.Assign[ id ][ n ].uDevice , gxLib::SaveData.Assign[ id ][ n ].uButton );
			}
		} 
	}
}


void CXiPad::rumble()
{
	//�U��

	for(int n=0;n<g_NumJoyStickDeviceNum;n++)
	{
		//if( !m_sGamingDevice[ n ]._bRumble ) continue;

		//�Y��y�̃v���C���[�g�p���̃f�o�C�X�łȂ���� continue;

		Float32 fRatio[2];
		CPadManager::GetInstance()->GetMotorStat(n, 0, &fRatio[0]);
		CPadManager::GetInstance()->GetMotorStat(n, 1, &fRatio[1]);

		if( fRatio[0] ==0 && fRatio[1] == 0 )
		{
			XINPUT_VIBRATION vibration;
			ZeroMemory( &vibration, sizeof(XINPUT_VIBRATION) );

			vibration.wLeftMotorSpeed  = 0;
			vibration.wRightMotorSpeed = 0;

			XInputSetState( n, &vibration );

			continue;
		}

		XINPUT_VIBRATION vibration;
		ZeroMemory( &vibration, sizeof(XINPUT_VIBRATION) );
		vibration.wLeftMotorSpeed  = (Sint32)(fRatio[0]*65535);
		vibration.wRightMotorSpeed = (Sint32)(fRatio[1]*65535);
		XInputSetState( n, &vibration );

    }
}

gxBool PadConfig( Sint32 padNo , Uint32 button )
{
	if ( CXiPad::GetInstance()->KeyAssign( padNo , button ) )
	{
		return gxTrue;
	}

	return gxFalse;
}


