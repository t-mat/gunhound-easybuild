//--------------------------------------------
//
//�@�_�b�V���{�[�h���̃p�b�h�R���t�B�O
//
//--------------------------------------------
#include <gunhound/gga2.h>
#include <gunhound/gga2x.h>
#include "CDashBoard.h"
#include "CDashPadConfig.h"

#define GGA2 0

#define KEYBOARD_DEC KEYSIGN01
#define KEYBOARD_CAN KEYSIGN02
#define KEYBOARD_DEC2 KEYSIGN11
#define KEYBOARD_DASHOPEN (KEYSIGN12)

gxSprite SprController[]={
	{ enTpgDashBoard,96,176+16*0,40,16,0,8},
	{ enTpgDashBoard,96,176+16*1,72,16,0,8},
};

enum {
	enPadCfgJoyStickIndex,
	enPadCfgJoyButton,
	enPadCfgDevice,
//	enPadCfgCancel,
	enPadCfgDecision,
	enPadCfgMax,

	enCfgScrollMax=enPadCfgMax,
	enPadScrollMax = 8,
	enDeviceLimit = 6,//enPadScrollMax,
};

enum {
	enBtnCfgJoyUp,
	enBtnCfgJoyDown,
	enBtnCfgJoyRight,
	enBtnCfgJoyLeft,
	enBtnCfgJoyShot,
	enBtnCfgJoyJump,
	enBtnCfgJoySub,
	enBtnCfgJoyDash,
	enBtnCfgJoyFix,
	enBtnCfgJoyShift,
	enBtnCfgJoyPause,
//	enBtnCfgCancel,
	enBtnCfgDecision,
	enBtnCfgMax,

	enBtnScrollMax = 8,
};

enum {
	enDvcXA,
	enDvcYA,
	enDvcZA,
	enDvcRX,
	enDvcRY,
	enDvcRZ,
	enDvcP0,
	enDvcP1,
	enDvcP2,
	enDvcP3,
	enDvcRB,
	enDvcDec,
	enDvcMax,
	enDvcScrollMax = 8,
};

Sint32 g_PadBtnPosTbl[][2] = {
	{32+280,148},
	{32+280,174},
	{32+294,161},
	{32+266,161},
	{32+345,161},
	{32+360,174},
	{32+360,148},
	{32+376,161},
	{32+282,120},
	{32+360,120},
	{32+320,149},
};

static Sint32 PadIndex[]={
	//�{�^��ID(���X�g�̏ォ�珇�Ƀ{�^��ID)
	0,
	2,
	1,
	3,
	7,
	4,
	8,
	5,
	10,
	11,
	15,
};

CDashPadConfig::CDashPadConfig()
{
	//--------------------------------------------
	//�R���t�B�O�J�n
	//--------------------------------------------

	m_Pos.x = 0;
	m_Pos.y = 0;
	m_Pos.z = 251;

	for(Sint32 ii=0;ii<enPadCfgLayerMax;ii++)
	{
		m_sSelect[ii] = 0;
		m_sScroll[ii] = 0;
	}

	for(Sint32 jj=0;jj<ARRAY_LENGTH(PadIndex);jj++)
	{
		m_bButtonOn[jj] = gxFalse;
	}

	m_ChangeLayer = enPadCfgLayeNone;
	m_ConfigLayer = enPadCfgLayerMain;

	m_sPadSelectMax = 0;

	m_bPadInit = gxTrue;

	m_sSeqWaitMode = 0;
	m_bEnd = gxFalse;

	m_sRotation = 0;

	m_bPadNumOver = gxFalse;
	m_bNext       = gxFalse;

	if( (g_StSaveData.BtnConfig.useDevice[0]+1) > GetDiviceNum() )
	{
		//�r���Ńp�b�h��������Ă����ꍇ�ɂ͎g�p���Ȃ��A�ɃZ�b�g����
		m_bPadNumOver = gxTrue;
	}

	m_bAutoSeq       = 0;
	m_bReleaseCheck = gxFalse;
}


CDashPadConfig::~CDashPadConfig()
{
	
	
	
}


void CDashPadConfig::SetPadButtonOn(Sint32 n)
{
	//--------------------------------------------
	//�{�^���������Ă��邩�H�i�f�B�X�v�p�����j
	//--------------------------------------------

	switch( n ){
	case JOY_U:		m_bButtonOn[0] = gxTrue;	break;
	case JOY_D:		m_bButtonOn[1] = gxTrue;	break;
	case JOY_R:		m_bButtonOn[2] = gxTrue;	break;
	case JOY_L:		m_bButtonOn[3] = gxTrue;	break;

	case BTN_X:		m_bButtonOn[4] = gxTrue;	break;
	case BTN_A:		m_bButtonOn[5] = gxTrue;	break;
	case BTN_Y:		m_bButtonOn[6] = gxTrue;	break;
	case BTN_B:		m_bButtonOn[7] = gxTrue;	break;

	case BTN_L1:	m_bButtonOn[8] = gxTrue;	break;
	case BTN_R1:	m_bButtonOn[9] = gxTrue;	break;

	case BTN_START:	m_bButtonOn[10] = gxTrue;	break;
	}

}

gxBool CDashPadConfig::IsEnd()
{
	//--------------------------------------------
	//�I���������H
	//--------------------------------------------

	return m_bEnd;
}

void CDashPadConfig::Action()
{
	//--------------------------------------------
	//�L�[�{�[�h���͎�t
	//--------------------------------------------
	m_sRotation ++;

	m_KeyBoardRepeat = 0;

	if( gxLib::KeyBoard((KEYSIGN_U)&enStatRepeat) ) m_KeyBoardRepeat  |= JOY_U;
	if( gxLib::KeyBoard((KEYSIGN_D)&enStatRepeat) ) m_KeyBoardRepeat  |= JOY_D;
	if( gxLib::KeyBoard((KEYSIGN_L)&enStatRepeat) ) m_KeyBoardRepeat  |= JOY_L;
	if( gxLib::KeyBoard((KEYSIGN_R)&enStatRepeat) ) m_KeyBoardRepeat  |= JOY_R;
	if( gxLib::KeyBoard((KEYSIGN_R)&enStatRepeat) ) m_KeyBoardRepeat  |= enBtnDecision;
	if( gxLib::KeyBoard((KEYBOARD_DEC2)&enStatTrig)) m_KeyBoardRepeat |= enBtnDecision;
	if( gxLib::KeyBoard((KEYBOARD_CAN)&enStatTrig) ) m_KeyBoardRepeat |= enBtnCancel;

	switch(m_ConfigLayer){
	case enPadCfgLayerMain:
		SelectMain();
		break;
	case enPadCfgLayerPad:
		SelectPad();
		break;
	case enPadCfgLayerButton:
		SelectBtn();
		break;
	case enPadCfgLayerDevice:
		SelectDvc();
		break;
	default:
		break;
	}
}


void CDashPadConfig::SelectMain()
{
	//--------------------------------------
	//���C���Z���N�^�[
	//--------------------------------------
	Sint32 _Scroll = m_sScroll[enPadCfgLayerMain];
	Sint32 _Select = m_sSelect[enPadCfgLayerMain];

	Sint32 sBtnTbl[]={
		JOY_U,
		JOY_D,
		JOY_L,
		JOY_R,
		BTN_A,
		BTN_B,
		BTN_X,
		BTN_Y,
		BTN_L1,
		BTN_R1,
		BTN_START,
	};

	for(Sint32 nn=0;nn<enBtnCfgDecision;nn++)
	{
		if( Joy[0].psh&sBtnTbl[nn] )
		{
			SetPadButtonOn( sBtnTbl[nn] );
		}
	}

	if( m_KeyBoardRepeat&JOY_U)
	{
		sysMenu::Play( enSoundMenuSel );
		if( _Select > 0 )       _Select --;
		if( _Select < _Scroll ) _Scroll = _Select;
	}
	else if( m_KeyBoardRepeat&JOY_D)
	{
		sysMenu::Play( enSoundMenuSel );
		if( _Select < enPadCfgMax-1           ) _Select ++;
		if( _Select >= _Scroll+enCfgScrollMax ) _Scroll = _Select-enCfgScrollMax+1;
	}
	else if( m_KeyBoardRepeat&enBtnDecision)
	{
		//�R�R�����͎b��ŃG���^�[�L�[������{�^���ƂȂ�܂�
		switch( _Select ){
		case enPadCfgJoyStickIndex:
			m_ChangeLayer = enPadCfgLayerPad;		//�p�b�h�Z���N�g��
			break;
		case enPadCfgJoyButton:
			if( g_StSaveData.BtnConfig.bUseController )
			{
				m_stBkButtonConfig = g_StSaveData.BtnConfig;	//�o�b�N�A�b�v
				m_bPadInit    = gxTrue;
				m_ChangeLayer = enPadCfgLayerButton;	//�p�b�h�Z���N�g��
				sysMenu::Play( enSoundMenuDec );
			}
			else
			{
				sysMenu::Play( enSoundMenuCan );
			}
			break;
		case enPadCfgDevice:
			if( g_StSaveData.BtnConfig.bUseController )
			{
				m_ChangeLayer = enPadCfgLayerDevice;	//�p�b�h�Z���N�g��
				sysMenu::Play( enSoundMenuDec );
			}
			else
			{
				sysMenu::Play( enSoundMenuCan );
			}
			break;
//		case enPadCfgCancel:
		case enPadCfgDecision:
			ghLib::SaveConfig();
			m_bEnd = gxTrue;
			break;
		}
	}
	else if( m_KeyBoardRepeat&enBtnCancel)
	{
		m_bEnd = gxTrue;
	}

	m_sScroll[enPadCfgLayerMain] = _Scroll;
	m_sSelect[enPadCfgLayerMain] = _Select;
}


void CDashPadConfig::SelectPad()
{
	//--------------------------------------
	//�p�b�h�Z���N�^�[
	//--------------------------------------
	Sint32 _Scroll = m_sScroll[enPadCfgLayerPad];
	Sint32 _Select = m_sSelect[enPadCfgLayerPad];

	if( m_bPadInit )
	{
		//�I�����̐����v�Z����
		m_sPadSelectMax = GetDiviceNum()+1;
		if( g_StSaveData.BtnConfig.bUseController == 0 )
		{
			//�g�p���Ȃ��ɍ��킹��
			_Select = 0;
		}
		else
		{
			//�g�p����p�b�h�ԍ��ɍ��킹�Ă����Ă�����
			_Select = g_StSaveData.BtnConfig.useDevice[0]+1;
			if(_Select > GetDiviceNum() || m_bPadNumOver )
			{
				//�p�b�h�������Ă����肵����h�g�p���Ȃ��h�ɂ��킹�Ă���
				_Select = 0;
			}
		}

		m_bPadInit = gxFalse;
	}

	if( m_KeyBoardRepeat&JOY_U)
	{
		sysMenu::Play( enSoundMenuSel );
		if( _Select > 0 )       _Select --;
		if( _Select < _Scroll ) _Scroll = _Select;
	}
	else if( m_KeyBoardRepeat&JOY_D)
	{
		sysMenu::Play( enSoundMenuSel );
		if( _Select < m_sPadSelectMax-1           ) _Select ++;
		if( _Select >= _Scroll+enPadScrollMax     ) _Scroll = _Select-enPadScrollMax+1;
	}
	else if( m_KeyBoardRepeat&enBtnDecision)
	{
		//�R�R�����͎b��ŃX�y�[�X�L�[������{�^���ƂȂ�܂�
		sysMenu::Play( enSoundMenuDec );
		if( _Select == 0 )
		{
			g_StSaveData.BtnConfig.bUseController = 0;
		}
		else
		{
			m_bPadNumOver = gxFalse;
			g_StSaveData.BtnConfig.bUseController = 1;
			g_StSaveData.BtnConfig.useDevice[0] = _Select-1;
		}
		m_ChangeLayer = enPadCfgLayerMain;	//�p�b�h�Z���N�g��

	}
	else if( m_KeyBoardRepeat&enBtnCancel)
	{
		m_ChangeLayer = enPadCfgLayerMain;	//�p�b�h�Z���N�g��
//		m_bEnd = gxTrue;
	}

	m_sScroll[enPadCfgLayerPad] = _Scroll;
	m_sSelect[enPadCfgLayerPad] = _Select;
}


void CDashPadConfig::SelectBtn()
{
	//--------------------------------------
	//�{�^���Z���N�^�[
	//--------------------------------------
	gxBool bDown = gxFalse;
	gxBool bDec  = gxFalse;
	Sint32 sControl = m_KeyBoardRepeat;

	if( m_sSeqWaitMode >= 100 )
	{
		//�{�^���̉�����҂�
		if( SelectBtnWait() )
		{
			m_bAutoSeq = 10;
		}
		return;
	}

	switch( m_bAutoSeq ){
	case 0:
		break;
	case 10:
		//���i�Ɉڍs����
		sControl = 0;
		bDown    = gxTrue;
		m_bAutoSeq = 20;
		break;
	case 20:
		//�{�^�������҂�
		sControl = 0;
		bDec = gxTrue;
		m_bAutoSeq = 99;
		break;
	case 99:
		m_bAutoSeq = 0;
		return;
	}


	Sint32 _Scroll = m_sScroll[enPadCfgLayerButton];
	Sint32 _Select = m_sSelect[enPadCfgLayerButton];

	if( sControl&JOY_U )
	{
		m_sRotation = 0;
		sysMenu::Play( enSoundMenuSel );
		if( _Select > 0 )       _Select --;
		if( _Select < _Scroll ) _Scroll = _Select;
	}
	else if( sControl&JOY_D || bDown )
	{
		m_sRotation = 0;
		sysMenu::Play( enSoundMenuSel );
		if( _Select < enBtnCfgMax-1           ) _Select ++;
		if( _Select >= _Scroll+enBtnScrollMax ) _Scroll = _Select-enBtnScrollMax+1;
	}
	else if( sControl&enBtnDecision || bDec )
	{
		//�R�R�����͎b��ŃX�y�[�X�L�[������{�^���ƂȂ�܂�
		sysMenu::Play( enSoundMenuDec );

		switch( _Select ){
		case enBtnCfgDecision:
//		case enBtnCfgCancel:
			sysMenu::Play( enSoundMenuDec );
			m_ChangeLayer = enPadCfgLayerMain;	//�p�b�h�Z���N�g��
			break;
		default:
			m_bReleaseCheck = gxFalse;
			m_sSeqWaitMode = 100;
			break;
		}
	}
	else if( sControl&enBtnCancel)
	{
		g_StSaveData.BtnConfig = m_stBkButtonConfig;	//�o�b�N�A�b�v
		sysMenu::Play( enSoundMenuCan );
		m_ChangeLayer = enPadCfgLayerMain;	//�p�b�h�Z���N�g��
		//m_bEnd = gxTrue;
	}

	m_sScroll[enPadCfgLayerButton] = _Scroll;
	m_sSelect[enPadCfgLayerButton] = _Select;
}


gxBool CDashPadConfig::SelectBtnWait()
{
	//--------------------------------------
	//�{�^���Z���N�^�[�i������҂��j
	//--------------------------------------
	Sint32 device_id,sRet = 0;
	Sint32 _Select = m_sSelect[enPadCfgLayerButton];

	if( ( Joy[0].psh ) == 0 )
	{
		m_bReleaseCheck = gxTrue;
	}


	if( m_KeyBoardRepeat&enBtnCancel)
	{
		//�L�����Z��
		sysMenu::Play( enSoundMenuCan );
		m_sSeqWaitMode = 0;
		return gxFalse;
	}

	//�{�^��������҂�
	if( !m_bReleaseCheck ) return gxFalse;

	device_id = g_StSaveData.BtnConfig.useDevice[0];

#if GGA2
	sRet = CPadControl::GetInstance()->CheckDeviceButtonPress( device_id );
#endif

	if( sRet >= 0 )
	{
		m_sSeqWaitMode = 0;
		g_StSaveData.BtnConfig.assign[0][ PadIndex[_Select] ] = sRet;
		return gxTrue;
	}

	return gxFalse;

}


void CDashPadConfig::SelectDvc()
{
	//--------------------------------------
	//�f�o�C�X�ڍאݒ�
	//--------------------------------------
	Sint32 _Scroll = m_sScroll[enPadCfgLayerDevice];
	Sint32 _Select = m_sSelect[enPadCfgLayerDevice];

	if( m_KeyBoardRepeat&JOY_U)
	{
		sysMenu::Play( enSoundMenuSel );
		if( _Select > 0 )       _Select --;
		if( _Select < _Scroll ) _Scroll = _Select;
	}
	else if( m_KeyBoardRepeat&JOY_D)
	{
		sysMenu::Play( enSoundMenuSel );
		if( _Select < enDvcMax-1           ) _Select ++;
		if( _Select >= _Scroll+enDvcScrollMax ) _Scroll = _Select-enDvcScrollMax+1;
	}
	else if( m_KeyBoardRepeat&enBtnDecision)
	{
		//�R�R�����͎b��ŃX�y�[�X�L�[������{�^���ƂȂ�܂�
			sysMenu::Play( enSoundMenuDec );

			if( _Select == enDvcDec )
			{
				m_ChangeLayer = enPadCfgLayerMain;	//�p�b�h�Z���N�g��
			}
			else
			{
				Sint32 sDeviceID = g_StSaveData.BtnConfig.useDevice[0];
				g_StSaveData.BtnConfig.enable[sDeviceID][_Select] = 1 - g_StSaveData.BtnConfig.enable[sDeviceID][_Select];
			}
	}
	else if( m_KeyBoardRepeat&enBtnCancel)
	{
		m_ChangeLayer = enPadCfgLayerMain;	//�p�b�h�Z���N�g��
//		m_bEnd = gxTrue;
	}

	m_sScroll[enPadCfgLayerDevice] = _Scroll;
	m_sSelect[enPadCfgLayerDevice] = _Select;
}


void CDashPadConfig::Draw()
{
	//--------------------------------------------
	//�p�b�h�̑I��
	//--------------------------------------------
	switch(m_ConfigLayer){
	case enPadCfgLayerMain:
		DrawMain();
		break;
	case enPadCfgLayerPad:
		DrawPadSel();
		break;
	case enPadCfgLayerButton:
		DrawBtnSel();
		break;
	case enPadCfgLayerDevice:
		DrawDvcSel();
		break;
	default:
		break;
	}

	//--------------------------------------------
	//�p�b�h��\������
	//--------------------------------------------
	Sint32 sCol = 0xFFFFFFFF;
	if( m_ConfigLayer == enPadCfgLayerButton )
	{
		sCol = 0xFFFFFFFF;
	}
	else
	{
		sCol = 0xFF808080;
	}

#if GGA2
	if( CPadControl::GetInstance()->IsUseController() )
#else
	if (0)
#endif
	{
		if( m_bPadNumOver )
		{
			//�ݒ�����R���g���[���[�������Ȃ��Ƃ�
			sysMenu::Sprite( 32+320 ,164 ,251, 0,336,160 ,176,96,88,48 ,ATR_DFLT, 0xFF010101 );
			sysMenu::Selectf(48+0, 290,83, "�s���ȃR���g���[���[[ %02d ]",g_StSaveData.BtnConfig.useDevice[0]+1);
		}
		else
		{
			//����
			sysMenu::Sprite( 32+320 ,164 ,251, 0,336,160,176,96,88,48 ,ATR_DFLT, sCol );
			sysMenu::Selectf(48+0, 290,83, "�R���g���[���[[ %02d ]",g_StSaveData.BtnConfig.useDevice[0]+1);
		}
	}
	else
	{
		//���ݒ�
		sysMenu::Sprite( 32+320 ,164 ,251, 0,336,160 ,176,96,88,48 ,ATR_DFLT, 0xFF010101 );
		sysMenu::Selectf(32+0, 260,164, "�R���g���[���[���ݒ�");
	}

	//--------------------------------------------
	//���ӏ���
	//--------------------------------------------
	if( m_sRotation%320 < 160 )
	{
		CDashBoard::GetInstance()->SetAnnounce( "�L�[�{�[�h�ő��삵�Ă��������B" );
	}
	else
	{
		CDashBoard::GetInstance()->SetAnnounce( "�����ENTER�L�[�ł�" );
	}
	if( m_ChangeLayer != enPadCfgLayeNone )
	{
		//--------------------------------------------
		//���C���[�ύX
		//--------------------------------------------
		m_ConfigLayer = m_ChangeLayer;
		m_ChangeLayer = enPadCfgLayeNone;
	}
}

void CDashPadConfig::DrawMain()
{
	//--------------------------------------------
	//���C���Z���N�^
	//--------------------------------------------
	Sint32 x,y;
	static char *PadString[]={
		"�R���g���[���[�I��",
		"�{�^���ݒ�",
		"�f�o�C�X�ڍ�",
//		"�ݒ���L�����Z������",
		"�ݒ芮��",
	};

	x = 32;
	y = 96;

	Sint32 nn = 0,ii=0;

	//���X�g�\��
	Sint32 _Scroll = m_sScroll[enPadCfgLayerMain];
	Sint32 _Select = m_sSelect[enPadCfgLayerMain];


	for(ii=0; ii<enBtnCfgMax-1; ii++)
	{
		if( m_bButtonOn[ii] )
		{
			Float32 fScl;
			fScl = gxLib::Cos( 1.f*((m_sRotation*2)%360) )*1.2f;
			sysMenu::Sprite( g_PadBtnPosTbl[ii][0] , g_PadBtnPosTbl[ii][1] , m_Pos.z+1 , 0, 32,160,32,32,16,16 ,ATR_ALPHA_PLUS,0x8000ffff, 0 , fScl, fScl);

			fScl = gxLib::Cos( 1.f*((m_sRotation*2+90)%360) )*1.2f;
			sysMenu::Sprite( g_PadBtnPosTbl[ii][0] , g_PadBtnPosTbl[ii][1] , m_Pos.z+1 , 0, 32,160,32,32,16,16 ,ATR_ALPHA_PLUS,0x80FFFF00, 0 , fScl, fScl);

			m_bButtonOn[ii] = gxFalse;
		}
	}

	for(ii=_Scroll; ii<_Scroll+enCfgScrollMax; ii++)
	{
		Sint32 sPattern = 0;
		if( !g_StSaveData.BtnConfig.bUseController )
		{
			switch( ii ){
			case 1:
			case 2:
				sPattern = -1;
				break;
			}
		}
		if(ii==_Select) sPattern = 1;

		sysMenu::Selectf( sPattern , x,y+16*nn, PadString[ii] );
		nn ++;
	}

}

void CDashPadConfig::DrawPadSel()
{
	//--------------------------------------------
	//�p�b�h�̃Z���N�^
	//--------------------------------------------
	Sint32 x,y,z;
	Sint32 sAdd=0;

	//���X�g�\��
	Sint32 _Scroll = m_sScroll[enPadCfgLayerPad];
	Sint32 _Select = m_sSelect[enPadCfgLayerPad];

	x = 32;
	y = 96;
	z = 250;

	if( GetDiviceNum() == 0 )
	{
		//�p�b�h���Ȃ���
		sysMenu::Printf( x-16,y,ARGB_DFLT, "�R���g���[���[��������܂���");
	}
	else
	{
		sysMenu::Printf( x,y-16,ARGB_DFLT, "%d�̃R���g���[���[��������܂���" ,GetDiviceNum() );

		sysMenu::Selectf( (_Select==0)? 1 : 0 ,x,y+sAdd*16+16 , "�g�p���Ȃ�");
		sAdd ++;

		Sint32 sMax = GetDiviceNum();
		if(sMax>=8) sMax = 8;

		for(Sint32 ii=0;ii<sMax;ii++ )
		{
			sysMenu::Selectf( (_Select==ii+1)? 1 : 0 , x,y+sAdd*16+16 , "�R���g���[���[[ %02d ]",ii+1 );//,CPadControl::GetInstance()->_GamingDevice[ii]._hardwarename);
			sAdd ++;
		}
	}


}


void CDashPadConfig::DrawBtnSel()
{
	//--------------------------------------------
	//�{�^���̃Z���N�^�\��
	//--------------------------------------------
	Sint32 x,y;
	static char *PadString[]={
		"(��)���o�[��",
		"(��)���o�[��",
		"(��)���o�[�E",
		"(��)���o�[��",
		"(�`)�V���b�g",
		"(�y)�W�����v/�L�����Z��",
		"(�r)�p���`",
		"(�w)�_�b�V��/����",
		"(�p)�ˊp�z�[���h",
		"(�v)�����V�t�g",
		"(SPACE)�|�[�Y",
		"�ݒ芮��",
	};

	x = 32;
	y = 88;

	Sint32 nn = 0;

	//���X�g�\��
	Sint32 _Scroll = m_sScroll[enPadCfgLayerButton];
	Sint32 _Select = m_sSelect[enPadCfgLayerButton];

	if( _Scroll > 0 )
	{
		sysMenu::Sprite( m_Pos.x+96 ,m_Pos.y+y-16 , m_Pos.z , 0, 0,176,32,16,16,8 ,ATR_FLIP_Y,0xFFFFFFFF);
	}
	else
	{
		sysMenu::Sprite( m_Pos.x+96 ,m_Pos.y+y-16 , m_Pos.z , 0, 0,176,32,16,16,8 ,ATR_FLIP_Y,0x80808080 );
	}

	for(Sint32 ii=_Scroll; ii<_Scroll+enBtnScrollMax; ii++)
	{
		if( (ii == _Select && ii<enBtnCfgDecision ) )
		{
			Float32 fScl;
			fScl = gxLib::Cos( 1.f*((m_sRotation*2)%360) )*1.2f*100;
			sysMenu::Sprite( g_PadBtnPosTbl[ii][0] , g_PadBtnPosTbl[ii][1] , m_Pos.z+1 , 0, 32,160,32,32,16,16 ,ATR_ALPHA_PLUS,0x8000ffff, 0 , fScl, fScl);

			fScl = gxLib::Cos( 1.f*((m_sRotation*2+90)%360) )*1.2f*100;
			sysMenu::Sprite( g_PadBtnPosTbl[ii][0] , g_PadBtnPosTbl[ii][1] , m_Pos.z+1 , 0, 32,160,32,32,16,16 ,ATR_ALPHA_PLUS,0x80FFFF00, 0 , fScl, fScl);
		}

		m_bButtonOn[ii] = gxFalse;

		if( m_sSeqWaitMode )
		{
			sysMenu::Selectf( 0 , x,y+16*nn, PadString[ii] );
		}
		else
		{
			sysMenu::Selectf( (ii==_Select)? 1 : 0 , x,y+16*nn, PadString[ii] );
		}

		if( ii<enBtnCfgDecision )
		{
			if( m_sSeqWaitMode && (ii==_Select) )
			{
				sysMenu::Selectf( 1, x+120,y+16*nn, "�R���g���[���[�̃{�^���������Ă�������" );
			}
			else
			{
#if GGA2
				sysMenu::Selectf( (ii==_Select)? 1 : 0 , x+148,y+16*nn, GetDeviceButtonID( g_StSaveData.BtnConfig.assign[0][ PadIndex[ ii] ] ) );
#endif
			}
		}
		nn ++;
	}

	if( _Scroll+enBtnScrollMax < enBtnCfgMax )
	{
		sysMenu::Sprite( m_Pos.x+96 ,y+8+nn*16 , m_Pos.z , 0, 0,176,32,16,16,8 ,ATR_DFLT,0xFFFFFFFF);
	}
	else
	{
		sysMenu::Sprite( m_Pos.x+96 ,y+8+nn*16 , m_Pos.z , 0, 0,176,32,16,16,8 ,ATR_DFLT,0x80808080);
	}
}


Sint32 CDashPadConfig::GetDiviceNum()
{
	Sint32 num = 0;

#if GGA2
	num = CPadControl::GetInstance()->_GamingDeviceNum;
#endif

	if( num >= enDeviceLimit )
	{
		num = enDeviceLimit;
	}

	return num;
}

void CDashPadConfig::DrawDvcSel()
{
	//--------------------------------------------
	//�f�o�C�X�̃Z���N�^
	//--------------------------------------------
	Sint32 x,y;
	static char *PadString[]={
		"X-AXIS",
		"Y-AXIS",
		"Z-AXIS",
		"X-ROTATION",
		"Y-ROTATION",
		"Z-ROTATION",
		"POV 0",
		"POV 1",
		"POV 2",
		"POV 3",
		"RUMBLE",
		"�ݒ芮��",
	};

	x = 32;
	y = 88;

	Sint32 nn = 0;

	//���X�g�\��
	Sint32 _Scroll = m_sScroll[enPadCfgLayerDevice];
	Sint32 _Select = m_sSelect[enPadCfgLayerDevice];

	if( _Scroll > 0 )
	{
//		sysMenu::Printf( 32,y-16,ARGB_DFLT, "��");
		sysMenu::Sprite( m_Pos.x+96 ,m_Pos.y+y-16 , m_Pos.z , 0, 0,176,32,16,16,8 ,ATR_FLIP_Y,0xFFFFFFFF);
	}
	else
	{
		sysMenu::Sprite( m_Pos.x+96 ,m_Pos.y+y-16 , m_Pos.z , 0, 0,176,32,16,16,8 ,ATR_FLIP_Y,0xF0808080);
	}
	Sint32 sDeviceID = g_StSaveData.BtnConfig.useDevice[0];

	for(Sint32 ii=_Scroll; ii<_Scroll+enDvcScrollMax; ii++)
	{
		gxBool bEnable = gxTrue;
		gxBool bNoDisp = gxFalse;
		gxBool bSelect = (_Select==ii)? 1 : 0;

#if GGA2
		switch(ii){
		case enDvcXA:
			bEnable = (CPadControl::GetInstance()->_GamingDevice[sDeviceID]._bAxisX )? gxTrue : gxFalse;
			break;
		case enDvcYA:
			bEnable = (CPadControl::GetInstance()->_GamingDevice[sDeviceID]._bAxisY )? gxTrue : gxFalse;
			break;
		case enDvcZA:
			bEnable = (CPadControl::GetInstance()->_GamingDevice[sDeviceID]._bAxisZ )? gxTrue : gxFalse;
			break;
		case enDvcRX:
			bEnable = (CPadControl::GetInstance()->_GamingDevice[sDeviceID]._bAxisRX)? gxTrue : gxFalse;
			break;
		case enDvcRY:
			bEnable = (CPadControl::GetInstance()->_GamingDevice[sDeviceID]._bAxisRY)? gxTrue : gxFalse;
			break;
		case enDvcRZ:
			bEnable = (CPadControl::GetInstance()->_GamingDevice[sDeviceID]._bAxisRZ)? gxTrue : gxFalse;
			break;
		case enDvcP0:
			bEnable = (CPadControl::GetInstance()->_GamingDevice[sDeviceID]._bPov[0])? gxTrue : gxFalse;
			break;
		case enDvcP1:
			bEnable = (CPadControl::GetInstance()->_GamingDevice[sDeviceID]._bPov[1])? gxTrue : gxFalse;
			break;
		case enDvcP2:
			bEnable = (CPadControl::GetInstance()->_GamingDevice[sDeviceID]._bPov[2])? gxTrue : gxFalse;
			break;
		case enDvcP3:
			bEnable = (CPadControl::GetInstance()->_GamingDevice[sDeviceID]._bPov[3])? gxTrue : gxFalse;
			break;
		case enDvcRB:
			bEnable = (CPadControl::GetInstance()->_GamingDevice[sDeviceID]._bRumble)? gxTrue : gxFalse;
			break;
		default:
			bNoDisp = gxTrue;
			break;
		}
#endif

		sysMenu::Selectf( bSelect , x,y+16*nn ,"%s",PadString[ii] );

		if(!bNoDisp)
		{
			//�\��OK�̍��ڂ̎�
			if( !bEnable )
			{
				//�g�p�s��
				sysMenu::Printf( x+8*10,y+16*nn,ARGB_DFLT, "----" );
			}
			else
			{
				//�g�p�̉ۂ�\��
				//sysMenu::Printf( x+8*10,y+16*nn,ARGB_DFLT, "%s" , ( g_StSaveData.BtnConfig.enable[sDeviceID][ii] )? "�g�p����" : "�g�p���Ȃ�" );
				sysMenu::Selectf( bSelect , x+8*10,y+16*nn,"%s" , ( g_StSaveData.BtnConfig.enable[sDeviceID][ii] )? "�g�p����" : "�g�p���Ȃ�" );
			}
		}

		nn ++;

	}

	if( _Scroll+enDvcScrollMax < enDvcMax )
	{
		sysMenu::Sprite( m_Pos.x+96 ,m_Pos.y+nn*16+96 , m_Pos.z , 0, 0,176,32,16,16,8 ,ATR_DFLT,0xFFFFFFFF);
	}
	else
	{
		sysMenu::Sprite( m_Pos.x+96 ,m_Pos.y+nn*16+96 , m_Pos.z , 0, 0,176,32,16,16,8 ,ATR_DFLT,0xF0808080);
	}
	//�J�[�\���\��
	sysMenu::Printf( x-16,y+16*(_Select-_Scroll),ARGB_DFLT, "��");
}


