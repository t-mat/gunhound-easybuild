//--------------------------------------------------
//
// gxDebug.h
//
//
//--------------------------------------------------


#ifndef _GXDEBUG_H_
#define _GXDEBUG_H_

class gxDebug
{
public:

	gxDebug();
	~gxDebug();
	void CheckPoint(Sint32 n);
	void CheckEnd();
	void Draw();

	static Sint32 m_DebugSwitch[8];
	static gxBool m_bMasterDebugSwitch;

	SINGLETON_DECLARE( gxDebug );

private:

	void drawBar( Sint32 n , Float32 ratio );

	Uint32  m_uTime[32];
	Uint32  m_uOldTime[32];

	Float32 m_fWorks[32];

	typedef struct StInfo
	{
		Sint32 OrderNum;
		Sint32 SubOrderNum;
		Uint32 start;
		Sint32 drawCnt;
		Float32 fps;
		Sint32  vtx_max;
		Sint32  idx_max;
	} StInfo;

	StInfo m_stInfo;
};

#endif
