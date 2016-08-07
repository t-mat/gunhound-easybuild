// --------------------------------------------------------------------
//
// ‚Â‚¶‚Â‚Ü‚ ‚í‚¹—p‹¤’Êƒwƒbƒ_
// Cocos—p
// --------------------------------------------------------------------
#ifndef _COCOS2d_MACHINE_H_
#define _COCOS2d_MACHINE_H_

#define USE_OPENGL

#include "cocos2d.h"
#include "AppDelegate.h"

USING_NS_CC;

enum {
	eCmdChgTexture,
	eCmdNoneTexture,
	eCmdChgAttributeAlphaAdd,
	eCmdChgAttributeAlphaSub,
	eCmdChgAttributeAlphaCrs,
	eCmdChgAttributeAlphaRvs,
	eCmdChgAttributeAlphaNml,
	eCmdChgAttributeAlphaXor,
	eCmdChgAttributeAlphaScr,
	eCmdRenderSquare,
	eCmdRenderPoint,
	eCmdRenderLineStrip,
	eCmdRenderLineNormal,
	eCmdRenderTriangle,
	eCmdRenderFont,
};

typedef struct StCustomVertex
{
	Float32 x,y,z,rhw;
	Uint32  col;
	Float32 u,v;
} StCustomVertex;


class CCommandList
{
public:
	Uint16 eCommand;
	Uint16 arg[4];
	Sint16 x,y;
	void*  pString;
	Float32 opt;
private:

};

#define KEYBOARD_ESCAPE      ((Uint32)(EventKeyboard::KeyCode::KEY_ESCAPE))
#define KEYBOARD_BACKSPACE   ((Uint32)(EventKeyboard::KeyCode::KEY_BACKSPACE))
#define KEYBOARD_TAB         ((Uint32)(EventKeyboard::KeyCode::KEY_TAB))
#define KEYBOARD_RETURN      ((Uint32)(EventKeyboard::KeyCode::KEY_RETURN))
#define KEYBOARD_SHIFT       ((Uint32)(EventKeyboard::KeyCode::KEY_SHIFT))
#define KEYBOARD_RSHIFT      ((Uint32)(EventKeyboard::KeyCode::KEY_RIGHT_SHIFT))
#define KEYBOARD_CTRL        ((Uint32)(EventKeyboard::KeyCode::KEY_CTRL))
#define KEYBOARD_RCTRL       ((Uint32)(EventKeyboard::KeyCode::KEY_RIGHT_CTRL))
#define KEYBOARD_ALT         ((Uint32)(EventKeyboard::KeyCode::KEY_ALT))
#define KEYBOARD_RALT        ((Uint32)(EventKeyboard::KeyCode::KEY_RIGHT_ALT))
#define KEYBOARD_ARROW_UP    ((Uint32)(EventKeyboard::KeyCode::KEY_UP_ARROW))
#define KEYBOARD_ARROW_DOWN  ((Uint32)(EventKeyboard::KeyCode::KEY_DOWN_ARROW))
#define KEYBOARD_ARROW_LEFT  ((Uint32)(EventKeyboard::KeyCode::KEY_LEFT_ARROW))
#define KEYBOARD_ARROW_RIGHT ((Uint32)(EventKeyboard::KeyCode::KEY_RIGHT_ARROW))
#define KEYBOARD_SPACE       ((Uint32)(EventKeyboard::KeyCode::KEY_SPACE))
#define KEYBOARD_ENTER       ((Uint32)(EventKeyboard::KeyCode::KEY_RETURN))
#define KEYBOARD_F1          ((Uint32)(EventKeyboard::KeyCode::KEY_F1))
#define KEYBOARD_F2          ((Uint32)(EventKeyboard::KeyCode::KEY_F2))
#define KEYBOARD_F3          ((Uint32)(EventKeyboard::KeyCode::KEY_F3))
#define KEYBOARD_F4          ((Uint32)(EventKeyboard::KeyCode::KEY_F4))
#define KEYBOARD_F5          ((Uint32)(EventKeyboard::KeyCode::KEY_F5))
#define KEYBOARD_F6          ((Uint32)(EventKeyboard::KeyCode::KEY_F6))
#define KEYBOARD_F7          ((Uint32)(EventKeyboard::KeyCode::KEY_F7))
#define KEYBOARD_F8          ((Uint32)(EventKeyboard::KeyCode::KEY_F8))
#define KEYBOARD_F9          ((Uint32)(EventKeyboard::KeyCode::KEY_F9))
#define KEYBOARD_F10         ((Uint32)(EventKeyboard::KeyCode::KEY_F10))
#define KEYBOARD_F11         ((Uint32)(EventKeyboard::KeyCode::KEY_F11))
#define KEYBOARD_F12         ((Uint32)(EventKeyboard::KeyCode::KEY_F12))
#define KEYBOARD_0           ((Uint32)(EventKeyboard::KeyCode::KEY_0))
#define KEYBOARD_1           ((Uint32)(EventKeyboard::KeyCode::KEY_1))
#define KEYBOARD_2           ((Uint32)(EventKeyboard::KeyCode::KEY_2))
#define KEYBOARD_3           ((Uint32)(EventKeyboard::KeyCode::KEY_3))
#define KEYBOARD_4           ((Uint32)(EventKeyboard::KeyCode::KEY_4))
#define KEYBOARD_5           ((Uint32)(EventKeyboard::KeyCode::KEY_5))
#define KEYBOARD_6           ((Uint32)(EventKeyboard::KeyCode::KEY_6))
#define KEYBOARD_7           ((Uint32)(EventKeyboard::KeyCode::KEY_7))
#define KEYBOARD_8           ((Uint32)(EventKeyboard::KeyCode::KEY_8))
#define KEYBOARD_9           ((Uint32)(EventKeyboard::KeyCode::KEY_9))
#define KEYBOARD_A           ((Uint32)(EventKeyboard::KeyCode::KEY_A))
#define KEYBOARD_B           ((Uint32)(EventKeyboard::KeyCode::KEY_B))
#define KEYBOARD_C           ((Uint32)(EventKeyboard::KeyCode::KEY_C))
#define KEYBOARD_D           ((Uint32)(EventKeyboard::KeyCode::KEY_D))
#define KEYBOARD_E           ((Uint32)(EventKeyboard::KeyCode::KEY_E))
#define KEYBOARD_F           ((Uint32)(EventKeyboard::KeyCode::KEY_F))
#define KEYBOARD_G           ((Uint32)(EventKeyboard::KeyCode::KEY_G))
#define KEYBOARD_H           ((Uint32)(EventKeyboard::KeyCode::KEY_H))
#define KEYBOARD_I           ((Uint32)(EventKeyboard::KeyCode::KEY_I))
#define KEYBOARD_J           ((Uint32)(EventKeyboard::KeyCode::KEY_J))
#define KEYBOARD_K           ((Uint32)(EventKeyboard::KeyCode::KEY_K))
#define KEYBOARD_L           ((Uint32)(EventKeyboard::KeyCode::KEY_L))
#define KEYBOARD_M           ((Uint32)(EventKeyboard::KeyCode::KEY_M))
#define KEYBOARD_N           ((Uint32)(EventKeyboard::KeyCode::KEY_N))
#define KEYBOARD_O           ((Uint32)(EventKeyboard::KeyCode::KEY_0))
#define KEYBOARD_P           ((Uint32)(EventKeyboard::KeyCode::KEY_P))
#define KEYBOARD_Q           ((Uint32)(EventKeyboard::KeyCode::KEY_Q))
#define KEYBOARD_R           ((Uint32)(EventKeyboard::KeyCode::KEY_R))
#define KEYBOARD_S           ((Uint32)(EventKeyboard::KeyCode::KEY_S))
#define KEYBOARD_T           ((Uint32)(EventKeyboard::KeyCode::KEY_T))
#define KEYBOARD_U           ((Uint32)(EventKeyboard::KeyCode::KEY_U))
#define KEYBOARD_V           ((Uint32)(EventKeyboard::KeyCode::KEY_V))
#define KEYBOARD_W           ((Uint32)(EventKeyboard::KeyCode::KEY_W))
#define KEYBOARD_X           ((Uint32)(EventKeyboard::KeyCode::KEY_X))
#define KEYBOARD_Y           ((Uint32)(EventKeyboard::KeyCode::KEY_Y))
#define KEYBOARD_Z           ((Uint32)(EventKeyboard::KeyCode::KEY_Z))

#define KEYSIGN_U (KEYBOARD_ARROW_UP)
#define KEYSIGN_D (KEYBOARD_ARROW_DOWN)
#define KEYSIGN_L (KEYBOARD_ARROW_LEFT)
#define KEYSIGN_R (KEYBOARD_ARROW_RIGHT)

#define KEYSIGN01 (KEYBOARD_Z)		//ƒ{ƒ^ƒ“‚P
#define KEYSIGN02 (KEYBOARD_X)		//ƒ{ƒ^ƒ“‚Q
#define KEYSIGN03 (KEYBOARD_C)		//ƒ{ƒ^ƒ“‚R
#define KEYSIGN04 (KEYBOARD_A)		//ƒ{ƒ^ƒ“‚S
#define KEYSIGN05 (KEYBOARD_S)		//ƒ{ƒ^ƒ“‚T
#define KEYSIGN06 (KEYBOARD_D)		//ƒ{ƒ^ƒ“‚U
#define KEYSIGN07 (KEYBOARD_SHIFT)	//ƒ{ƒ^ƒ“‚k
#define KEYSIGN08 (KEYBOARD_RSHIFT)	//ƒ{ƒ^ƒ“‚q
#define KEYSIGN09 (KEYBOARD_CTRL)	//ƒ{ƒ^ƒ“L2
#define KEYSIGN10 (KEYBOARD_RCTRL)	//ƒ{ƒ^ƒ“R2
#define KEYSIGN11 (KEYBOARD_SPACE)	//‚r‚d‚k‚d‚b‚s
#define KEYSIGN12 (KEYBOARD_ENTER)	//‚r‚s‚`‚q‚s


#endif	//_COCOS2d_MACHINE_H_


