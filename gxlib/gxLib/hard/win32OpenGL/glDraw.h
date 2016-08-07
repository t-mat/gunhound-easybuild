// --------------------------------------------------------
/*

*/
// --------------------------------------------------------

#include "glut.h"
#include "glFont.h"

class CGLDraw
{
public:

	CGLDraw();
	~CGLDraw();

	gxBool Init();
	void Action();
	void Draw();
	void FlipScreen();

	void Resume()
	{
		resetDevice();
		CTexManager::GetInstance()->UploadTexture( gxTrue );
	}

	void ReadTexture(Sint32 sBank);

	SINGLETON_DECLARE( CGLDraw );


private:

	void render();
	gxBool resetDevice();

	void getVideoDeviceInfo();

	GLuint m_sTexIndex[CTexManager::enMasterPageNum];

	HGLRC m_hRC;	// OpenGL Rendering Context
	GLFont m_Font;

/*
	LPDIRECT3D9        		m_pDirect3D;
	LPDIRECT3DDEVICE9  		m_pDirect3DDevice;
	D3DPRESENT_PARAMETERS	m_pDirect3DPrePara;
	LPDIRECT3DTEXTURE9		m_D3dTexture[CTexManager::enMasterPageNum];
	LPD3DXFONT				m_pDirect3DFont;
*/
};


