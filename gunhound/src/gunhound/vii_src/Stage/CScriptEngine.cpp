//------------------------------------------------------
//
// CSV �X�N���v�g�G���W��(�ʏ�p)
//
//------------------------------------------------------
#include <gunvalken.h>
#include <gxLib/Util/csv/CCsv.h>
#include "CScriptEngine.h"

CScriptEngine::CScriptEngine(char** pTable)
{
	//------------------------------------------------------
	//�X�N���v�g�G���W���i�R���X�g���N�^�j
	//------------------------------------------------------

	sMaxLine = 0;

	for(Sint32 ii=0; ii<enMaxArguments ;ii++)
	{
		m_sArgBuf[ii] = (char*)malloc(enMaxm_sArgBuf);
	}

	m_sCommand = 0;

	m_pCommandTable = pTable;
	m_pCsv = NULL;
}


CScriptEngine::~CScriptEngine()
{
	//------------------------------------------------------
	//�X�N���v�g�G���W���i�f�X�g���N�^�j
	//------------------------------------------------------

	for(Sint32 ii=0; ii<enMaxArguments ;ii++)
	{
		free( m_sArgBuf[ii] );
		m_sArgBuf[ii] = NULL;
	}

	if( m_pCsv ) delete m_pCsv;
}


gxBool CScriptEngine::Load( gxChar *pName )
{
	//------------------------------------------------------
	//CSV�t�@�C���̓ǂݍ���
	//------------------------------------------------------

	if( m_pCsv ) delete m_pCsv;

	m_pCsv = new CCsv();
	m_pCsv->LoadFile(pName);

	sMaxLine = m_pCsv->GetHeight();

	return gxTrue;
}


gxBool CScriptEngine::LoadMemory( Uint8 *pData , Uint32 sz )
{
	//------------------------------------------------------
	//CSV�t�@�C���̓ǂݍ���
	//------------------------------------------------------

	if( m_pCsv )
	{
		delete m_pCsv;
	}

	m_pCsv = new CCsv();
	m_pCsv->ReadFile( pData, sz );

	sMaxLine = m_pCsv->GetHeight();

	return gxTrue;
}


void CScriptEngine::Analysis( )
{
	//------------------------------------------------------
	//CSV��͂��s��
	//------------------------------------------------------

	Main();

}

void CScriptEngine::Read(Sint32 sLine)
{
	//------------------------------------------------------
	//�t�@�C���P�ʂ̃R�}���h����͂���
	//------------------------------------------------------

	m_sCommand = GetCommand(sLine);

	for(Sint32 nn=0;nn<enMaxArguments;nn++)
	{
		char *p;
		p = GetArg(sLine,nn);
		if( p )
		{
			
			memcpy( m_sArgBuf[nn],p,enMaxm_sArgBuf);
		}
		else
		{
			m_sArgBuf[nn][0] = 0x00;
		}
	}
}


Sint32 CScriptEngine::GetCommandIndex()
{
	return m_sCommand;

}


char** CScriptEngine::GetArguments()
{

	return m_sArgBuf;

}

//------------------------------------------------------
//�v���C�x�[�g�֐�
//------------------------------------------------------
Sint32 CScriptEngine::GetLineMax()
{
	//------------------------------------------------------
	//CSV�t�@�C���̃}�b�N�X�s��Ԃ�
	//------------------------------------------------------

	return sMaxLine;
}


void CScriptEngine::Main()
{
	//------------------------------------------------------
	//�t�@�C���P�ʂ̃R�}���h����͂���
	//------------------------------------------------------

	for(Sint32 ii=0;ii<sMaxLine;ii++)
	{
		Read( ii );
//		ExecuteCommand(sCmd,m_sArgBuf);
	}

}


char* CScriptEngine::GetArg(Sint32 sLine , Sint32 sIndex )
{
	//------------------------------------------------------
	//��������͂���
	//------------------------------------------------------

	char *p=NULL;

	p = m_pCsv->GetCell(enCommand+sIndex+1,sLine);

	return p;
}


Sint32 CScriptEngine::GetCommand(Sint32 sLine)
{
	//------------------------------------------------------
	//��������͂���i���C���P�ʁj
	//------------------------------------------------------

	char *p;
	Sint32 cmdNo=-1;
	p = m_pCsv->GetCell(enCommand,sLine);

	cmdNo = GetCommand(p);

	return cmdNo;
}

Sint32 CScriptEngine::GetCommand(char* p)
{
	//------------------------------------------------------
	//��������͂���i������j
	//------------------------------------------------------

	Sint32 n = 0;
	Sint32 l1 = strlen(p);
	Sint32 l2 = 0;
	Sint32 len = 0;

	while( m_pCommandTable[n]!=NULL )
	{
		l2 = strlen( m_pCommandTable[n] );
		len = (l1 > l2 )? l1 : l2;

		if( strncmp( m_pCommandTable[n],p,len) == 0)
		{
			return n;
		}
		n ++;
	}

	return enCommandNone;
}

void CScriptEngine::ExecuteCommand(Sint32 sCmd , char **pArg)
{
	//------------------------------------------------------
	//�R�}���h�����s����(���g�p)
	//------------------------------------------------------

	switch(sCmd){
	case 0:	//"Command_None",
		break;
	case 1:	//"cmd_set",
		break;
	case 2:	//"msg_end",
		break;
	case 3:	//"cmd",
		break;

	//--------------------------------
	//�t�@�C���ǂݍ��݊֘A
	//--------------------------------

	case 4:	//"[LoadTexture]",
		LoadTexture( atoi(pArg[0]), pArg[1] ,0xff00ff00 );
		break;

	case 5:	//"[Update]",
		UploadTexture();
		break;
	}

}

