enum {
	enMaxArguments = 32,
	enMaxm_sArgBuf    = 256,

	enCommand = 0,
	enCommandMax = 256,

	enCommandNone = 0,
};

class CScriptEngine
{

public:

	CScriptEngine(char** pTable);
	~CScriptEngine();
	gxBool Load( gxChar* pScriptName );
	gxBool LoadMemory( Uint8* pData ,Uint32 sz );
	void   Analysis( );
	Sint32 GetLineMax();
	void   Read(Sint32 sLine);

	Sint32 GetCommandIndex();
	char** GetArguments();

private:
	void Main();

	Sint32 GetCommand(Sint32 sLine);
	Sint32 GetCommand(char* p);

	char* GetArg(Sint32 sLine , Sint32 sIndex );
	void ExecuteCommand(Sint32 sCmd , char **pArg);

	Sint32 sMaxLine;
	Sint32 m_sCommand;
	char* m_sArgBuf[enMaxArguments];
	char** m_pCommandTable;
	CCsv *m_pCsv;
};

