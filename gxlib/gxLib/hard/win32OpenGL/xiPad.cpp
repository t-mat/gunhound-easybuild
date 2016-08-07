//------------------------------------------------------
//
// コントローラー制御( Direct Input version8使用)
//
//------------------------------------------------------
#include <gxLib.h>
#include <gxLib/gx.h>
#include <gxLib/gxPadManager.h>
#include "xiPad.h"

#pragma comment(lib,"xinput.lib ")

#define ILLEGAL_BTNID ( 0xFFFFFFFF )

int   g_NumJoyStickDeviceNum;						//ジョイスティック検出数
int   g_NowSettingDeviceNo;							//現在設定中のデバイスID

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
	enItemDButonId = 0,	//デジタルボタン番号 1～16
	enItemJoyBtnId = 0,	//デバイスのボタン番号 JOY_U～
	enItemDeviceId = 1,	//デバイスのID パッド機器の番号
};

//-----------------------------------------------------------------
//ゲームパッド関連情報を管理
//-----------------------------------------------------------------
CXiPad::CXiPad()
{
	gxLib::DebugLog("【Control】Xiパッドデバイスを初期化開始");

	//m_bFirstCaribration = gxTrue;
	m_bUseController = gxTrue;

	for(Uint32 ii=0;ii<JOYMAX;ii++)
	{
		for(Uint32 jj=0;jj<GDBTN_MAX;jj++)
		{
			m_bEnable[ii][jj] = gxTrue;
		}
	}

	g_NumJoyStickDeviceNum = m_uGamingDeviceNum = 0;		//デバイスの数はゼロ
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

	//コンフィグデータからの書き戻し
	AssignPadData();

}

CXiPad::~CXiPad()
{
	//--------------------------------------------------
	//ジョイパッドデバイスを解放する
	//--------------------------------------------------

	//コンフィグデータを書き戻す

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

	// 現在のパッドデータを gxLib::SaveData.Assign[][] に反映
	SavePadData();
}


//DirectInputの初期化
void CXiPad::Init()
{
	InitXInput();
	gxLib::DebugLog("【Control】XIパッドデバイスの初期化完了");
}


bool CXiPad::InitXInput()
{
	//---------------------------------------------------------
	//DirectInputオブジェクトを作成する
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
		//JOYSTICK検出できず
        return gxFalse;
    }

	for(int n=0;n<g_NumJoyStickDeviceNum;n++)
	{
		g_NowSettingDeviceNo = n;
	}

	//検出されたジョイパッド数を記録
	m_uGamingDeviceNum = g_NumJoyStickDeviceNum;

	gxLib::DebugLog("【Control】使用可能なパッド数は%d",m_uGamingDeviceNum);

	return gxTrue;
}

void CXiPad::Action()
{

	//毎フレーム通る処理
	Uint32 i;

	m_uPadButtonPush[0] = 0x00000000;
	m_uPadButtonPush[1] = 0x00000000;

	for(i=0;i<m_uGamingDeviceNum;i++)
	{
		//使用しない機能について制限する

		bool bFlag;
		bFlag = true;

		if( !m_bUseController )
		{
			//そもそもコントローラーを使用しない場合には全てのボタンを認識させない
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
	//アナログ情報を返す

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
		//アナログ１の検出
		joyIndex = GetJoyIndex(BTN_ANALOG1);
		btnIndex = GetJoyButton(BTN_ANALOG1);
	}
	else
	{
		//アナログ２の検出
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
	//ハードウェア情報を更新する
	//-----------------------------------------
	CGameingDevice *pGameDevice = &m_sGamingDevice[n];

	if( !GamePad360[n].bConnected ) return;

	WORD Buttons = GamePad360[n].state.Gamepad.wButtons;

	//状態の取得に成功した

	//---------------------------------
	//AXISについて
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
	//スライダーコントロールについて
	//---------------------------------
	pGameDevice->_Slider0 = GamePad360[n].state.Gamepad.bLeftTrigger;
	pGameDevice->_Slider1 = GamePad360[n].state.Gamepad.bRightTrigger;

	//---------------------------------
	//ハットスイッチについて
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
	pGameDevice->_button[6] = ( Buttons & XINPUT_GAMEPAD_LEFT_THUMB )	 ? 1:0;		//左アナログ方向キーをパッド奥に押した場合
	pGameDevice->_button[7] = ( Buttons & XINPUT_GAMEPAD_RIGHT_THUMB )	 ? 1:0;		//右アナログ方向キーをパッド奥に押した場合
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
			//アナログスティックの入力を十字キーと共有する
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
//デバイス状態を管理
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

	//サポート状況を初期化
	_bPov[0]    = gxFalse;			//ハットスイッチ0
	_bPov[1]    = gxFalse;			//ハットスイッチ1
	_bPov[2]    = gxFalse;			//ハットスイッチ2
	_bPov[3]    = gxFalse;			//ハットスイッチ3
	_num_slider = 0;				//スライダーコントロール数
	_bAxisX =
	_bAxisY =
	_bAxisZ = gxFalse;				//サポートされているAXIS
	_bAxisRX =
	_bAxisRY =
	_bAxisRZ = gxFalse;				//サポートされているAXIS
	_bRumble = gxFalse;				//振動可能か？


	//使用するかどうか(基本的には全部使用することにする)
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
	//ボタンが押されているかどうかデジタル入力に変換して返す
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
	//ボタンが押されているかどうかチェック

	return (btn)? gxTrue : gxFalse;
}

gxBool CGameingDevice::IsPressAxis( Sint32& axis , Sint32 dir)
{
	//アナログスティックが倒されているかどうかチェック
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
	//POVが入力されているかどうか
	//---------------------------------------------------------------------

	//---------------------------------------------------------------------
	//押されているボタンとリクエスト方向が一致すれば押されていることにする
	//---------------------------------------------------------------------
	switch(urdl){
	case 0:	//上
		if(pov & JOY_U) return gxTrue;
		break;
	case 1:	//右
		if(pov & JOY_R) return gxTrue;
		break;
	case 2:	//下
		if(pov & JOY_D) return gxTrue;
		break;
	case 3:	//左
		if(pov & JOY_L) return gxTrue;
		break;
	}

	return gxFalse;
}

gxBool CGameingDevice::IsPressSlider( Sint32 &sldr , Sint32 dir)
{
	//０～２５５の値が返ってくる

	if( sldr > 200 ) return gxTrue;

	return gxFalse;
}


Uint32 CXiPad::GetJoyButton( Uint32 joyBtnId , Sint32 sID )
{
	//探したいgxボタンIDを使用しているデバイスボタンを探す
	//すべてのデバイスから捜索

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
	//Player sID のボタン（joyBtnId）が、どの
	//どのジョイパッドを使うか返す
	//探したいgxボタンIDが使用しているデバイスインデックスを返す
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
	//指定のパッドビットを消去
	//コンフィグの際に過去の情報を消去する

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
// パッドコンフィグ
//
//-------------------------------------------------------------

gxBool CXiPad::KeyAssign( Sint32 sPlayerId , Sint32 nButton )
{
	//---------------------------------------------
	//プレイヤー番号と、ボタンを渡して設定を行う
	//---------------------------------------------

	static Sint32 m_sConfigSeq = 0;
	static gxBool m_bNoPress = gxFalse;

	switch(m_sConfigSeq){
	case 0:		//初期化
		m_sConfigSeq = 10;
		m_bNoPress = gxFalse;
		gxLib::DebugLog("【PadConfig】初期化");
		break;

	case 10:
		if( m_bNoPress )
		{
			m_sConfigSeq = 100;
			m_bNoPress = gxFalse;
		}
		break;

	case 100:	//ボタン押されるの待ち

		if( !m_bNoPress )
		{
			//ボタン押された
			gxLib::DebugLog("【PadConfig】ボタンはなされるの待ち");
			m_sConfigSeq = 200;
		}
		break;

	case 200:	//ボタンはなされるの待ち

		if( m_bNoPress )
		{
			//ボタンはなされた
			m_sConfigSeq = 0;
			return gxTrue;
		}
		break;

	case 900:		//終了
		break;
	}

	gxBool bDecision = gxFalse;
	gxBool bPress = gxFalse;

	for(Uint32 jj=0;jj<m_uGamingDeviceNum;jj++)
	{
		//ゲームデバイスのどのボタンが押されているかチェックする

		for(Sint32 ii=0;ii<GDBTN_MAX;ii++)
		{
			if( m_sGamingDevice[jj].IsPress( (GAMINGDEVICE_BTNID)ii ) )
			{
				//押されているボタンを検出した

				bPress = gxTrue;

				if( m_sConfigSeq == 100 )
				{
					bDecision = gxTrue;

					clearButtonData( nButton , sPlayerId );

					//デバイスｎのボタンｍはnButton(BTN_Aなど)に該当させる

					//m_sConvButton[jj][ ii ] = nButton;
					m_stButtonAssign[jj][ ii ].uPlayer = sPlayerId;
					m_stButtonAssign[jj][ ii ].uButtonBit = nButton;
					gxLib::DebugLog("【PadConfig】[%d-%d] (PL:%d / BTN:0x%02X)", jj , ii , sPlayerId , nButton );
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

// オプションなどでのキャンセル処理用（GUNHOUND（Steam）用）
// 指定されたプレイヤー番号の現在のパッドデータをすべて保存
void CXiPad::BackupData(Sint32 sPlayerId/* = 0*/)
{
	for(Sint32 i = 0; i < GDBTN_MAX; ++i) {
		this->m_stBackupButtonAssign[i] = this->m_stButtonAssign[sPlayerId][i];
	}
}

// 指定されたプレイヤー番号の backupData() されたデータに復帰
void CXiPad::RestoreData(Sint32 sPlayerId/* = 0*/)
{
	for(Sint32 i = 0; i < GDBTN_MAX; ++i) {
		this->m_stButtonAssign[sPlayerId][i] = this->m_stBackupButtonAssign[i];
	}
}

// 設定されているパッド値クリア（何も設定されていない状態にする）
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

// デフォルトパッドデータを gxLib::SaveData.Assign[][] に設定後、パッドに設定 
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
	if(sizeof(gxLib::StSaveData) != dataSize) {	// サイズが違うので
		return;									// ここで終了
	}

	// デフォルトデータ設定
	memcpy(&gxLib::SaveData, s_default_padconfig, dataSize);
	AssignPadData();
}

// 現在 gxLib::SaveData.Assign[][] に設定されている値をパッドに設定 
void CXiPad::AssignPadData(void)
{
	for(Uint32 id=0;id<PLAYER_MAX;id++)
	{
		for(Uint32 ii=0;ii<ARRAY_LENGTH(JoyIndexTbl);ii++)
		{
	//		Sint32 BtnID    = gxLib::SaveData.m_sPadConfig[ii][enItemDButonId];
	//		Sint32 joyIndex = gxLib::SaveData.m_sPadConfig[ii][enItemDeviceId];

			//どのデバイスのどのボタンでgxボタンを使うか？
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

// 現在のパッドデータを gxLib::SaveData.Assign[][] に反映
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
	//振動

	for(int n=0;n<g_NumJoyStickDeviceNum;n++)
	{
		//if( !m_sGamingDevice[ n ]._bRumble ) continue;

		//該当yのプレイヤー使用中のデバイスでなければ continue;

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


