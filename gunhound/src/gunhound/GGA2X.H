//--------------------------------------------------
//
// GGA関数のラッパー（お手軽版）
//
//--------------------------------------------------
#ifndef _GGA2X_H_
#define _GGA2X_H_

#include "gga2.h"
#include <gxLib/Util/actManager/CActManager.h>
#include <gxLib/Util/hitManager/CCollisionManager.h>

#define ASSERT(n) assert(n)

void GGX_Putsprite(gxSprite *p,int x,int y,int prio,int atr=ATR_DEFAULT,unsigned long col=ARGB_DFLT,float scl=1.0f,float rot=0.0f);
void GGX_DrawPixel(int x,int y,int z,int col=ARGB_DFLT,int atr=ATR_DEFAULT);
void GGX_DrawBox(int x1,int y1,int x2,int y2,int z,gxBool flag,int col=ARGB_DFLT,int atr=ATR_DEFAULT);
void GGX_DrawLine(int x1,int y1,int x2,int y2,int z,int col=ARGB_DFLT,int atr=ATR_DEFAULT);
void GGX_DrawTriangle(int x1,int y1,int x2,int y2,int x3,int y3,int z,int col=ARGB_DFLT,int atr=ATR_DEFAULT);

typedef struct ghPos
{
	Sint32 x,y,z,r;
} ghPos;


int     gxGetHomingDirection(Sint32 my_angle , Sint32 target_angle);
Float32 gxGetHomingDirection(Float32 my_angle,Float32 target_angle);
Float32 gxAbs( Float32 n );
Sint32  gxAbs( Sint32 n );

#endif	//_GGA2X_H_
