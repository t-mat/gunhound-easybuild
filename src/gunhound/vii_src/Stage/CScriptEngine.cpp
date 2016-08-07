//------------------------------------------------------
//
// CSV スクリプトエンジン(玉城用)
//
//------------------------------------------------------
#include <gunvalken.h>
#include <gxLib/Util/csv/CCsv.h>
#include "CScriptEngine.h"

CScriptEngine::CScriptEngine(char** pTable)
{
	//------------------------------------------------------
	//スクリプトエンジン（コンストラクタ）
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
	//スクリプトエンジン（デストラクタ）
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
	//CSVファイルの読み込み
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
	//CSVファイルの読み込み
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
	//CSV解析を行う
	//------------------------------------------------------

	Main();

}

void CScriptEngine::Read(Sint32 sLine)
{
	//------------------------------------------------------
	//ファイル単位のコマンドを解析する
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
//プライベート関数
//------------------------------------------------------
Sint32 CScriptEngine::GetLineMax()
{
	//------------------------------------------------------
	//CSVファイルのマックス行を返す
	//------------------------------------------------------

	return sMaxLine;
}


void CScriptEngine::Main()
{
	//------------------------------------------------------
	//ファイル単位のコマンドを解析する
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
	//引数を解析する
	//------------------------------------------------------

	char *p=NULL;

	p = m_pCsv->GetCell(enCommand+sIndex+1,sLine);

	return p;
}


Sint32 CScriptEngine::GetCommand(Sint32 sLine)
{
	//------------------------------------------------------
	//引数を解析する（ライン単位）
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
	//引数を解析する（文字列）
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
	//コマンドを実行する(未使用)
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
	//ファイル読み込み関連
	//--------------------------------

	case 4:	//"[LoadTexture]",
		LoadTexture( atoi(pArg[0]), pArg[1] ,0xff00ff00 );
		break;

	case 5:	//"[Update]",
		UploadTexture();
		break;
	}

}

