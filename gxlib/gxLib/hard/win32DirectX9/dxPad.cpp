//------------------------------------------------------
//
// コントローラー制御( Direct Input version8使用)
//
//------------------------------------------------------

#include <gxLib.h>
#include <gxLib/gx.h>
//#include <gxLib/hard/machine.h>
//#include <gxLib/gxOrderManager.h>
//#include <gxLib/gxTexManager.h>
//#include <gxLib/gxSoundManager.h>
#include <gxLib/gxPadManager.h>
//#include <gxLib/gxRender.h>
#include "../win32cmn/win32cmn.h"
#include "dxPad.h"

#pragma comment(lib, "DXGUID.LIB")
#pragma comment(lib, "dinput8.lib")

#define AXIS_RANGE (1000)

#define ILLEGAL_BTNID ( 0xFFFFFFFF )

// PS4 コントローラーを PC に普通に挿した時に設定されるハードウェア名
static const char* PS4ControllerName = "Wireless Controller";

//--------------------------------------------------------------------
//コールバックによるローカル変数設定不可なグローバルな変数たち
//--------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,VOID* pContext );
BOOL CALLBACK EnumObjectsCallback  ( const DIDEVICEOBJECTINSTANCE* pdidoi,VOID* pContext );
BOOL CALLBACK EnumAxesCallback     ( const DIDEVICEOBJECTINSTANCE* pdidoi,VOID* pContext );

//--------------------------------------------------------------------
//コールバックによるローカル変数設定不可なグローバルな変数たち
//--------------------------------------------------------------------

int   g_NumJoyStickDeviceNum;						//ジョイスティック検出数
int   g_NowSettingDeviceNo;							//現在設定中のデバイスID
LPDIRECTINPUT8        g_pDirectInput     = NULL;	//DirectInputインターフェース
LPDIRECTINPUTDEVICE8  g_pJoystickDevice[JOYMAX];	//ジョイスティックデバイス数

SINGLETON_DECLARE_INSTANCE( CDxPad )

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
CDxPad::CDxPad()
{
	gxLib::DebugLog("【Control】パッドデバイスを初期化開始");

	m_bFirstCaribration = gxTrue;
	m_bUseController = gxTrue;

	for(Uint32 ii=0;ii<JOYMAX;ii++)
	{
		g_pJoystickDevice[ii] = NULL;	//デバイスポインタをクリア
		g_pEffect[ii] 		  = NULL;	//FFエフェクト用ポインタをクリア
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
//			m_sConvButton[kk][jj] = ILLEGAL_BTNID;
			m_stButtonAssign[kk][jj].uPlayer = 0;
			m_stButtonAssign[kk][jj].uButtonBit = ILLEGAL_BTNID;
		}
	}

	//コンフィグデータからの書き戻し
	AssignPadData();

#ifdef GX_DEBUG
	//-----------------
/*
	for(Uint32 jj=0;jj<2;jj++)
	{
		for(Uint32 ii=0;ii<GDBTN_MAX;ii++)
		{
			gxLib::DebugLog( "PADCONF[dev%02d , btn%02d] = (pl%02x/btn%02x)",
				jj,ii,
				m_stButtonAssign[ jj ][ ii ].uPlayer,
				m_stButtonAssign[ jj ][ ii ].uButtonBit );
		}
	}
*/
	//-----------------
#endif
}

CDxPad::~CDxPad()
{
	//--------------------------------------------------
	//ジョイパッドデバイスを解放する
	//--------------------------------------------------

	for(int i=0;i<g_NumJoyStickDeviceNum;i++)
	{
		if(g_pJoystickDevice[i])
		{
			g_pJoystickDevice[i]->Unacquire();
			SAFE_RELEASE( g_pJoystickDevice[i] );
		}
	    SAFE_RELEASE( g_pEffect[i] );
	}
    SAFE_RELEASE( g_pDirectInput );

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

	//-----------------
/*
	for(Uint32 jj=0;jj<2;jj++)
	{
		for(Uint32 ii=0;ii<GDBTN_MAX;ii++)
		{
			gxLib::DebugLog( "PADCONF[dev%02d , btn%02d] = (pl%02x/btn%02x)",
				jj,ii,
				m_stButtonAssign[ jj ][ ii ].uPlayer,
				m_stButtonAssign[ jj ][ ii ].uButtonBit );
		}
	}
*/
	//-----------------
	// 現在のパッドデータを gxLib::SaveData.Assign[][] に反映
	SavePadData();
}


//DirectInputの初期化
void CDxPad::Init()
{
	InitDirectInput();
	gxLib::DebugLog("【Control】パッドデバイスの初期化完了");
}


bool CDxPad::InitDirectInput()
{
	//---------------------------------------------------------
	//DirectInputオブジェクトを作成する
	//---------------------------------------------------------

    // DirectInputサブシステムに登録しDirectInputオブジェクトを構築。
    // そのオブジェクトへのポインタを得てDirectInputインターフェースとする。
    if( FAILED( DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION,IID_IDirectInput8, (VOID**)&g_pDirectInput, NULL ) ) ) {
		return gxFalse;
	}

    // Look for a simple joystick we can use for this sample program.
    if( FAILED( g_pDirectInput->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback,NULL, DIEDFL_ATTACHEDONLY) ) ) {
		return gxFalse;
	}

	//コールバックに一度いきます【EnumJoysticksCallback】

	if(g_NumJoyStickDeviceNum == 0) {
		//JOYSTICK検出できず
        return gxFalse;
    }

	for(int n=0;n<g_NumJoyStickDeviceNum;n++)
	{
		g_NowSettingDeviceNo = n;

	    if( FAILED( g_pJoystickDevice[n]->SetDataFormat( &c_dfDIJoystick2 ) ) )
	    {
	        return gxFalse;
		}

		//----------------------------------------------------------------------------
	    // 協調レベルの設定
		//----------------------------------------------------------------------------
	    if( FAILED( g_pJoystickDevice[n]->SetCooperativeLevel( g_pWindows->m_hWindow, DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) )
	    {
	        return gxFalse;
		}
		//ジョイスティックの「機能」を列挙する。
		//コールバック関数内でユーザーインターフェースにある要素を見つけて
		//AXISの範囲を決定する。
		// ※DIDFT_ALL　サポートする機能をすべて調べる
	    if( FAILED( g_pJoystickDevice[n]->EnumObjects( EnumObjectsCallback, (VOID*)g_pWindows->m_hWindow, DIDFT_ALL ) ) )
	    {
			return gxFalse;
		}

		//フォースフィードバック対応デバイスの軸数を調べる
	    if ( FAILED( g_pJoystickDevice[n]->EnumObjects( EnumAxesCallback, (VOID*)&m_sGamingDevice[n]._ForceFeefBackAxis, DIDFT_AXIS ) ) )
	    {
			return gxFalse;
		}

		//軸数が２つ以上あるものはGGAでは、それ以上エフェクトの対象としない
	    if( m_sGamingDevice[n]._ForceFeefBackAxis > 2 )	m_sGamingDevice[n]._ForceFeefBackAxis = 2;

		//フォースフィードバック可能か？
		CheckForceFeedBackEffect(n);

		// PS4 コントローラーチェック
		if(strcmp(m_sGamingDevice[n]._hardwarename, PS4ControllerName) == 0)
		{
			m_sGamingDevice[n]._bPS4Controller = gxTrue;
		}
	}

	//検出されたジョイパッド数を記録
	m_uGamingDeviceNum = g_NumJoyStickDeviceNum;

	gxLib::DebugLog("【Control】使用可能なパッド数は%d",m_uGamingDeviceNum);

	return true;
}

void CDxPad::CheckForceFeedBackEffect(int device_id )
{
	//-------------------------------------------
	//フォースフィードバック検出
	//-------------------------------------------

    DWORD           rgdwAxes[2]     = { DIJOFS_X, DIJOFS_Y };
    LONG            rglDirection[2] = { 0, 0 };
    DICONSTANTFORCE cf              = { 0 };
    DIEFFECT eff;
    ZeroMemory( &eff, sizeof(eff) );
    eff.dwSize                  = sizeof(DIEFFECT);
    eff.dwFlags                 = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
    eff.dwDuration              = INFINITE;
    eff.dwSamplePeriod          = 0;
    eff.dwGain                  = DI_FFNOMINALMAX;
    eff.dwTriggerButton         = DIEB_NOTRIGGER;
    eff.dwTriggerRepeatInterval = 0;
    eff.cAxes                   = m_sGamingDevice[device_id]._ForceFeefBackAxis;
    eff.rgdwAxes                = rgdwAxes;
    eff.rglDirection            = rglDirection;
    eff.lpEnvelope              = 0;
    eff.cbTypeSpecificParams    = sizeof(DICONSTANTFORCE);
    eff.lpvTypeSpecificParams   = &cf;
    eff.dwStartDelay            = 0;

    //エフェクト用のポインタを作成
    if( FAILED( g_pJoystickDevice[device_id]->CreateEffect( GUID_ConstantForce, &eff, &g_pEffect[device_id], NULL ) ) )
    {
		m_sGamingDevice[device_id]._bRumble = gxFalse;
		gxLib::DebugLog("【Control】デバイス%dでFFB不可",device_id);
		return;
    }

	//エフェクト用のポインタが確保できれば準備をすすめる
	if( g_pEffect[device_id] ) {
		m_sGamingDevice[device_id]._bRumble = true;	//振動可能
		g_pEffect[device_id]->Start( 1, 0 );
		gxLib::DebugLog("【Control】デバイス%dでFFB可能",device_id);
	}
	else
	{
		gxLib::DebugLog("【Control】デバイス%dでFFB不可",device_id);
	}
}


void CDxPad::Action()
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
		gxBool PadConfig(Sint32 padID);

		CGXPadConfig::GetInstance()->Action();

		if( CGXPadConfig::GetInstance()->IsConfigFinish() )
		{
			CGXPadConfig::GetInstance()->Init();
			CGameGirl::GetInstance()->SetPadConfig( gxFalse );
		}

		CGXPadConfig::GetInstance()->Draw();
	}

}



Float32* CDxPad::GetAnalogInfo(int n)
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
	if(g_pJoystickDevice[joyIndex] == NULL) return AnalogData;

	switch( btnIndex ){
	case GDBTN_AXIS_XA:
	case GDBTN_AXIS_XS:
	case GDBTN_AXIS_YA:
	case GDBTN_AXIS_YS:
		AnalogData[0] = 1.f*pGameDevice->_Axis.x/AXIS_RANGE;
		AnalogData[1] = 1.f*pGameDevice->_Axis.y/AXIS_RANGE;
		break;
	case GDBTN_ROT_XA:
	case GDBTN_ROT_XS:
	case GDBTN_ROT_YA:
	case GDBTN_ROT_YS:
		AnalogData[0] = 1.f*pGameDevice->_Rotation.x/AXIS_RANGE;
		AnalogData[1] = 1.f*pGameDevice->_Rotation.y/AXIS_RANGE;
		break;
	case GDBTN_AXIS_ZA:
	case GDBTN_AXIS_ZS:
	case GDBTN_ROT_ZA:
	case GDBTN_ROT_ZS:
		AnalogData[0] = 1.f*pGameDevice->_Rotation.z/AXIS_RANGE;
		AnalogData[1] = 1.f*pGameDevice->_Axis.z/AXIS_RANGE;
		break;
	}

	return AnalogData;
}

void CDxPad::UpdateGamingDevices(int n)
{
	//-----------------------------------------
	//ハードウェア情報を更新する
	//-----------------------------------------
    HRESULT     hr;

	CGameingDevice *pGameDevice = &m_sGamingDevice[n];
	if(g_pJoystickDevice[n] == NULL) return;

	LPDIRECTINPUTDEVICE8 pPadDevice = g_pJoystickDevice[n];
	DIJOYSTATE2 js;           // ジョイスティックの状態を取得する構造体変数
	
	//ポーリング
	hr = pPadDevice->Poll();

	if( FAILED( hr ) )
	{
		//入力ストリームが途中で絶たれた。特殊なリセット処理を行い再度接続を試みる
		hr = pPadDevice->Acquire();
		while( hr  == DIERR_INPUTLOST )
		{
			//ロストしているだけなら再接続できるまで待つ
			hr = pPadDevice->Acquire();
		}
		return ;	//次の機会に接続再開してみる
	}

	if( FAILED(pPadDevice->GetDeviceState( sizeof(DIJOYSTATE2), &js ) ) )
	{
		//ジョイスティックの状態取得に失敗
		return ;
	}

	//状態の取得に成功した

	//---------------------------------
	//AXISについて
	//---------------------------------
	if( m_bFirstCaribration )
	{
		m_bFirstCaribration = gxFalse;
		pGameDevice->_caribrate.x = js.lX;
		pGameDevice->_caribrate.y = js.lY;
		pGameDevice->_caribrate.z = js.lZ;

		pGameDevice->_caribrateR.z = js.lRz;
	}

//	pGameDevice->_Axis.x 		= (js.lX-pGameDevice->_caribrate.x)/0x08;
//	pGameDevice->_Axis.y 		= (js.lY-pGameDevice->_caribrate.y)/0x08;
//	pGameDevice->_Axis.z 		= (js.lZ-pGameDevice->_caribrate.z)/0x08;
//	pGameDevice->_Axis.x 		= js.lX;
//	pGameDevice->_Axis.y 		= js.lY;
//	pGameDevice->_Axis.z 		= js.lZ;
	pGameDevice->_Axis.x 		= js.lX-pGameDevice->_caribrate.x;
	pGameDevice->_Axis.y 		= js.lY-pGameDevice->_caribrate.y;
	pGameDevice->_Axis.z 		= js.lZ-pGameDevice->_caribrate.z;
	if(!pGameDevice->_bPS4Controller)
	{
		pGameDevice->_Rotation.x 	= js.lRx;
		pGameDevice->_Rotation.y 	= js.lRy;
	}
	else
	{
		pGameDevice->_Rotation.x 	= (js.lRx + AXIS_RANGE) / 2;
		pGameDevice->_Rotation.y 	= (js.lRy + AXIS_RANGE) / 2;
	}
	pGameDevice->_Rotation.z 	= js.lRz-pGameDevice->_caribrateR.z;

	//---------------------------------
	//スライダーコントロールについて
	//---------------------------------
	pGameDevice->_Slider0 = js.rglSlider[0];
	pGameDevice->_Slider1 = js.rglSlider[1];

	//---------------------------------
	//ハットスイッチについて
	//---------------------------------
	pGameDevice->_Pov0 = js.rgdwPOV[0];
	pGameDevice->_Pov1 = js.rgdwPOV[1];
	pGameDevice->_Pov2 = js.rgdwPOV[2];
	pGameDevice->_Pov3 = js.rgdwPOV[3];

    for( int i = 0; i < 128; i++ )
    {
        if ( js.rgbButtons[i] & 0x80 )
        {
			pGameDevice->_button[i] = 1;
		}
		else
		{
			pGameDevice->_button[i] = 0;
		}
	}

	for(Uint32 ii=0;ii<GDBTN_MAX;ii++)
	{
		if(pGameDevice->IsPress( (GAMINGDEVICE_BTNID)ii ))
		{
			Sint32 sPlayer = m_stButtonAssign[n][ii].uPlayer;
			if( m_stButtonAssign[n][ii].uButtonBit != ILLEGAL_BTNID )
			{
				m_uPadButtonPush[sPlayer] |= m_stButtonAssign[n][ii].uButtonBit;
			}
		}
	}
}


BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,VOID* pContext )
{
	//-----------------------------------------------------------------------------
	//使用できるジョイパッドをコールバックにより列挙します。
	//１つでもみつけたら デバイスインターフェース(LPDIRECTINPUT8)を
	//プレイに使用できるように作成します。
	//※逆にいうと、１つもみつからなかったらLPDIRECTINPUT8には何も入らない。
	//-----------------------------------------------------------------------------


    // Obtain an interface to the enumerated joystick.
	if(FAILED( g_pDirectInput->CreateDevice( pdidInstance->guidInstance, &g_pJoystickDevice[g_NumJoyStickDeviceNum], NULL ) ) )
	{
		// デバイスを確認できなかったので使用できない。(おそらく接続されていない）
	    return DIENUM_STOP;
	}

	if( ( g_NumJoyStickDeviceNum+1 ) > JOYMAX)
	{
		//１６本以上は検出しない
		return DIENUM_CONTINUE;
	}

	//ハードウェア名を記録
	sprintf(CDxPad::GetInstance()->m_sGamingDevice[g_NumJoyStickDeviceNum]._hardwarename,"%s",pdidInstance->tszInstanceName);
//GX_LOG("Pad name -> %d : %s", g_NumJoyStickDeviceNum, pdidInstance->tszInstanceName);

/*
	if( ( g_NumJoyStickDeviceNum+1 ) > JOYMAX)
	{
		//１６本以上は検出しない
	    return DIENUM_STOP;
	}
*/

	g_NumJoyStickDeviceNum ++;

	return DIENUM_CONTINUE;
}

//-----------------------------------------------------------------------------
// ジョイスティックの機能が検出されるたびにコールされる (axes, buttons, POVs) 
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,VOID* pContext )
{
	int n = g_NowSettingDeviceNo;
	//HWND hDlg = (HWND)pContext;

	//-------------------------------------------------------------------
	// ジョイスティックの軸が検出されれば、その範囲（－１０００～＋１０００）を設定する
	//-------------------------------------------------------------------

    if( pdidoi->dwType & DIDFT_AXIS ) {
        DIPROPRANGE diprg; 
        diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
        diprg.diph.dwHow        = DIPH_BYID; 
        diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin              = -AXIS_RANGE; 
        diprg.lMax              = +AXIS_RANGE; 

		// AXISの範囲を設定する
        if( FAILED( g_pJoystickDevice[n]->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
        {
            return DIENUM_STOP;
        }
    }

	//-------------------------------------------------------------------
    // ジョイスティックがサポートしている機能を調査する
	//-------------------------------------------------------------------

    if (pdidoi->guidType == GUID_XAxis) {
		CDxPad::GetInstance()->m_sGamingDevice[n]._bAxisX = true;
    }
    if (pdidoi->guidType == GUID_YAxis) {
		CDxPad::GetInstance()->m_sGamingDevice[n]._bAxisY = true;
    }
    if (pdidoi->guidType == GUID_ZAxis) {
		CDxPad::GetInstance()->m_sGamingDevice[n]._bAxisZ = true;
    }
    if (pdidoi->guidType == GUID_RxAxis) {
		CDxPad::GetInstance()->m_sGamingDevice[n]._bAxisRX = true;
    }
    if (pdidoi->guidType == GUID_RyAxis) {
		CDxPad::GetInstance()->m_sGamingDevice[n]._bAxisRY = true;
    }
    if (pdidoi->guidType == GUID_RzAxis) {
		CDxPad::GetInstance()->m_sGamingDevice[n]._bAxisRZ = true;
    }
    if (pdidoi->guidType == GUID_Slider) {
		switch( CDxPad::GetInstance()->m_sGamingDevice[n]._num_slider++ ) {
		case 0 :
			CDxPad::GetInstance()->m_sGamingDevice[n]._bSlider[0] = true;
			break;

		case 1 :
			CDxPad::GetInstance()->m_sGamingDevice[n]._bSlider[1] = true;
			break;
		}
	}

	if (pdidoi->guidType == GUID_POV) {
		switch( CDxPad::GetInstance()->m_sGamingDevice[n]._num_pov++ ) {
		case 0 :
			//ハットスイッチ１つ
			CDxPad::GetInstance()->m_sGamingDevice[n]._bPov[0] = true;
			break;
		case 1 :
			//ハットスイッチ２つ
			CDxPad::GetInstance()->m_sGamingDevice[n]._bPov[1] = true;
			break;
		case 2 :
			//ハットスイッチ３つ
			CDxPad::GetInstance()->m_sGamingDevice[n]._bPov[2] = true;
			//なぜブレークがないか不明
		case 3 :
			//ハットスイッチ４つ
			CDxPad::GetInstance()->m_sGamingDevice[n]._bPov[3] = true;
		default:
			break;
		}
	}

	return DIENUM_CONTINUE;
}


//-----------------------------------------------------------------------------
// フォースフィードバック対応のデバイスについて
// 有効な軸を機能する数分だけコールバックされる
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumAxesCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,VOID* pContext )
{
    DWORD* pdwNumForceFeedbackAxis = (DWORD*) pContext;

    if( (pdidoi->dwFlags & DIDOI_FFACTUATOR) != 0 )
    {
        (*pdwNumForceFeedbackAxis)++;
    }

    return DIENUM_CONTINUE;
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

bool CGameingDevice::IsPress(GAMINGDEVICE_BTNID id)
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

bool CGameingDevice::IsPressButton(bool& btn)
{
	//ボタンが押されているかどうかチェック

	return (btn)? true : gxFalse;
}

bool CGameingDevice::IsPressAxis(int& axis,int dir)
{
	//アナログスティックが倒されているかどうかチェック
	int d = 0;

	if(axis < -AXIS_RANGE/2) d = -1;
	if(axis >  AXIS_RANGE/2) d =  1;

	if(dir == 0 && 	d == 1)  return true;
	if(dir == 1 && 	d == -1) return true;

	return gxFalse;
}

bool CGameingDevice::IsPressPov(int &pov,int urdl)
{
	//---------------------------------------------------------------------
	//POVが入力されているかどうか
	//---------------------------------------------------------------------

	//---------------------------------------------------------------------
	//どの象限にいるのかチェック
	//---------------------------------------------------------------------
	const int half = 225;

	if(pov < 0) return gxFalse;	//押されていないときは－１を返している

	int rot = pov/10;
	int dir=0;
	if(rot > 3600-half || rot < half) 		 dir = 1;	//上
	if(rot >= half*1 && rot  < half*1 +half*2) dir = 2;	//右斜め上
	if(rot >= half*3 && rot  < half*3 +half*2) dir = 3;	//右
	if(rot >= half*5 && rot  < half*5 +half*2) dir = 4;	//右下
	if(rot >= half*7 && rot  < half*7 +half*2) dir = 5;	//下
	if(rot >= half*9 && rot  < half*9 +half*2) dir = 6;	//左下
	if(rot >= half*11 && rot < half*11+half*2) dir = 7;	//左
	if(rot >= half*13 && rot < half*13+half*2) dir = 8;	//左上

	//---------------------------------------------------------------------
	//象限により押されているボタンに割り振る
	//---------------------------------------------------------------------
	int joy = 0;

	switch(dir) {
	case 1:	joy = 0x01;			break;
	case 2:	joy = 0x01|0x02;	break;
	case 3:	joy = 0x02;			break;
	case 4:	joy = 0x02|0x04;	break;
	case 5:	joy = 0x04;			break;
	case 6:	joy = 0x04|0x08;	break;
	case 7:	joy = 0x08;			break;
	case 8:	joy = 0x08|0x01;	break;
	}

	//---------------------------------------------------------------------
	//押されているボタンとリクエスト方向が一致すれば押されていることにする
	//---------------------------------------------------------------------
	switch(urdl){
	case 0:	//上
		if(joy & 0x01) return true;
		break;
	case 1:	//右
		if(joy & 0x02) return true;
		break;
	case 2:	//下
		if(joy & 0x04) return true;
		break;
	case 3:	//左
		if(joy & 0x08) return true;
		break;
	}

	return gxFalse;
}

bool CGameingDevice::IsPressSlider(int &sldr,int dir)
{

	//スライダーの判定はできないなぁ。過去の値を保存しておくことで可能かも知れないが
	//手元にスライダーがないので判別不可。
	return gxFalse;
}


Uint32 CDxPad::GetJoyButton( Uint32 joyBtnId , Sint32 sID )
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


Uint32 CDxPad::GetJoyIndex( Uint32 joyBtnId , Sint32 sID )
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
//			if( m_sConvButton[ jj ][ ii ] == joyBtnId )
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


void CDxPad::clearButtonData( Uint32 joyBtnId , Sint32 sPlID )
{
	//指定のパッドビットを消去
	//コンフィグの際に過去の情報を消去する

	for(Uint32 jj=0;jj<JOYMAX;jj++)
	{
		for(Uint32 ii=0;ii<GDBTN_MAX;ii++)
		{
			//if( m_sConvButton[ jj ][ ii ]&joyBtnId  )
			if( m_stButtonAssign[ jj ][ ii ].uButtonBit == joyBtnId  )
			{
			
				if( sPlID == m_stButtonAssign[ jj ][ ii ].uPlayer )
				{
					//m_sConvButton[ jj ][ ii ] &= ~joyBtnId;
					//m_stButtonAssign[ jj ][ ii ].uButtonBit  &= ~joyBtnId;
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

gxBool CDxPad::KeyAssign( Sint32 sPlayerId , Sint32 nButton )
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
void CDxPad::BackupData(Sint32 sPlayerId/* = 0*/)
{
	for(Sint32 i = 0; i < GDBTN_MAX; ++i) {
		this->m_stBackupButtonAssign[i] = this->m_stButtonAssign[sPlayerId][i];
	}
}

// 指定されたプレイヤー番号の backupData() されたデータに復帰
void CDxPad::RestoreData(Sint32 sPlayerId/* = 0*/)
{
	for(Sint32 i = 0; i < GDBTN_MAX; ++i) {
		this->m_stButtonAssign[sPlayerId][i] = this->m_stBackupButtonAssign[i];
	}
}

// 設定されているパッド値クリア（何も設定されていない状態にする）
void CDxPad::ClearPadData(void)
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
void CDxPad::AssignDefaultPadData(void)
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
void CDxPad::AssignPadData(void)
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
void CDxPad::SavePadData(void)
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

void CDxPad::rumble()
{
	//振動

    LONG rglDirection[2] = { 180*DI_DEGREES, 0 };
    DICONSTANTFORCE cf;

	for(int n=0;n<g_NumJoyStickDeviceNum;n++)
	{
		if( !m_sGamingDevice[ n ]._bRumble ) continue;

		//該当yのプレイヤー使用中のデバイスでなければ continue;

		Sint32 Motor1 = 0,Motor2 = 0;

		Float32 fRatio[2];
		CPadManager::GetInstance()->GetMotorStat(n, 0, &fRatio[0]);
		CPadManager::GetInstance()->GetMotorStat(n, 1, &fRatio[1]);

		if (fRatio[0] == 0 && fRatio[1] == 0)
		{
			g_pEffect[n]->Stop();
			continue;
		}

        cf.lMagnitude = 1.0f;

	    DIEFFECT eff;
	    ZeroMemory( &eff, sizeof(eff) );

		eff.dwSize                = sizeof(DIEFFECT);
		eff.dwFlags               = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
		eff.cAxes                 = m_sGamingDevice[n]._ForceFeefBackAxis;
		eff.rglDirection          = rglDirection;
		eff.lpEnvelope            = 0;
		eff.cbTypeSpecificParams  = sizeof(DICONSTANTFORCE);
		eff.lpvTypeSpecificParams = &cf;
		eff.dwStartDelay            = 0;

		static int test = 0;

		if( test == 0 )
		{
			g_pEffect[n]->SetParameters(&eff, DIEP_DIRECTION | DIEP_TYPESPECIFICPARAMS | DIEP_START);
			test++;
			HRESULT hr = g_pEffect[n]->Start(1, DIES_SOLO);
			switch (hr) {
			case DI_OK:
				break;
			case DIERR_INCOMPLETEEFFECT:
				break;
			case DIERR_INVALIDPARAM:
				break;
			case DIERR_NOTEXCLUSIVEACQUIRED:
				break;
			case DIERR_NOTINITIALIZED:
				break;
			case DIERR_UNSUPPORTED:
				break;
			default:
				break;
			}
		}



    }

	//エフェクト用のポインタが確保できれば準備をすすめる
//	if( g_pEffect[device_id] )
	{
	}

}


gxBool PadConfig( Sint32 padNo , Uint32 button )
{
	if ( CDxPad::GetInstance()->KeyAssign( padNo , button ) )
	{
		return gxTrue;
	}

	return gxFalse;
}
