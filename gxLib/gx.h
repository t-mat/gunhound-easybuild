//--------------------------------------------------
//
// gx.h
// 
//
//--------------------------------------------------
#ifndef _GX_H_
#define _GX_H_

#define StepRepeatFrm (5)

class CGameGirl
{
public:

	CGameGirl();
	~CGameGirl();

	void makeWindow();

	void resume();

	void SetExitWindow()
	{
		m_bMainLoop = false;
	}

	void Action();
	void Init();
	void Main();
	void End();

	void SetResume()
	{
		m_bResume = gxTrue;
	}

	gxBool IsSyoriochi()
	{
		return m_bSyoriochi;
	}

	gxBool IsPause()
	{
		return m_bPause;
	}
	void SetPause( gxBool bPause )
	{
		m_bPause = bPause;
	}
	void SetKomaokuri(gxBool bKomaokuri)
	{
		if( m_sStepFrm >= StepRepeatFrm ) m_sStepFrm = 0;
	}

	Uint32 GetCounter()
	{
		return m_uGameCounter;
	}

	//各ハードウェアから報告してもらう
	void ReportMemoryStatus( Uint32 uNow , Uint32 uTotal , Uint32 uMax )
	{
		m_uMemoryTotal   = uTotal;
		m_uMemoryMaximum = uMax;
		m_uMemoryUse     = uNow;
	}

	void GetMemoryRemain( Uint32* uNow , Uint32* uTotal , Uint32* uMax )
	{
		//メモリ残量
		*uTotal = m_uMemoryTotal;
		*uMax   = m_uMemoryMaximum;
		*uNow   = m_uMemoryUse;
	}

	gxBool IsExist()
	{
		//アプリ側からゲームを抜ける処理
		return m_bMainLoop;
	}

	Uint32 SetTime( Sint32 Year , Sint32 Month , Sint32 Day , Sint32 Hour , Sint32 Min , Sint32 Sec , Sint32 MilliSec , Uint32 uSec )
	{
		//現在時刻を得る

		m_TimeYear = Year;
		m_TimeMonth= Month;
		m_TimeDay  = Day;
		m_TimeHour = Hour;
		m_TimeMin  = Min;
		m_TimeSec  = Sec;
		m_TimeMSec = MilliSec;

		m_TimeUSec = uSec;

		return 0;
	}

	Uint32 GetTime(Sint32 *pYear, Sint32 *pMonth, Sint32 *pDay, Sint32 *pHour, Sint32 *pMin, Sint32 *pSec, Sint32 *pMSec )
	{
		*pYear  = m_TimeYear;
		*pMonth = m_TimeMonth;
		*pDay   = m_TimeDay;
		*pHour  = m_TimeHour;
		*pMin   = m_TimeMin;
		*pSec   = m_TimeSec;
		*pMSec  = m_TimeMSec;

		return m_TimeUSec;
	}

	Uint32 GetStartTime()
	{
		return m_uStartTime;
	}

	void SetReset()
	{
		m_bResetButton = gxTrue;
	}

	void SetPadConfig( gxBool bModeOn = gxTrue )
	{
		m_bPadDeviceConfigMode = bModeOn;
	}

	gxBool IsPadConfigMode()
	{
		return m_bPadDeviceConfigMode;
	}

	SINGLETON_DECLARE( CGameGirl );

private:

	void init();
	void main();
	void end();

	Uint32  m_uTime[8];
	Float32 m_fWorks[8];

	gxBool drawInit();
	gxBool drawMain();
	gxBool drawEnd();

	gxBool soundInit();
	gxBool soundMain();
	gxBool soundEnd();

	gxBool movieInit();
	gxBool movieMain();
	gxBool movieEnd();

	gxBool inputInit();
	gxBool inputMain();
	gxBool inputEnd();

	gxBool gameMain();
	gxBool gameEnd();

	gxBool vsyncWait();
	gxBool flip();

	Sint32 m_sStepFrm;

	gxBool m_bMainLoop;

	gxBool m_bResume;

	gxBool m_bSyoriochi;
	gxBool m_bPause;

	Sint32 m_sTimer;
	Uint32 m_uGameCounter;

	//メモリ管理
	Uint32 m_uMemoryTotal;
	Uint32 m_uMemoryMaximum;
	Uint32 m_uMemoryUse;

	//時刻管理用
	Sint32 m_TimeYear;
	Sint32 m_TimeMonth;
	Sint32 m_TimeDay;
	Sint32 m_TimeHour;
	Sint32 m_TimeMin;
	Sint32 m_TimeSec;
	Sint32 m_TimeMSec;
	Uint32 m_TimeUSec;	//これだけ特殊

	Uint32 m_uStartTime;

	gxBool m_bResetButton;
	gxBool m_bPadDeviceConfigMode;
};


//extern CGameGirl gga;
//#include "gxDebug.h"

#endif
