#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

USING_NS_CC;

typedef struct StJoyStick
{
    int mx;
    int my;
    int psh;
} StJoyStick;

class HelloWorld : public cocos2d::Layer
{
public:

    enum {
        enTextureMax = 32,
    };
    void update(float delta);
    
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    static HelloWorld* GetInstance()
    {
        return s_pInstance;
    }
    
    bool LoadTexture( int page , char *pFileName );
    bool PutSprite( int x , int y , int prio , int page , int u , int v , int w , int h , int cx , int cy , int atr , int argb , float r , float fx ,float fy );
    void LoadAudio();
    void PlayAudio();
    StJoyStick* GetJoyStat( int player );
    void Action();

    void InitAction();
    void Render();
    void Upload( Sint32 sBank );
    void Play();

private:

    void render(void *pOdr );
    
    typedef struct StTexture
    {
        StTexture()
        {
            w = h = depth = 0;
            length = 0;
            pData = NULL;
            m_pTex = NULL;
            pSpriteBatchNode = NULL;
        }
        unsigned char *pData;
        unsigned int length;
        
        int w,h,depth;
        CCTexture2D *m_pTex;
        SpriteBatchNode *pSpriteBatchNode;
    } StTexture;

    StTexture *m_pTexture;
    StJoyStick m_JoyStick[2];

    typedef struct StMouse{
    
        int mouseX;
        int mouseY;
        bool buttonL;
        bool buttonR;
    }StMouse;
    StMouse m_Mouse;

    int m_sCount;

    Label *m_pLabel;
    static HelloWorld *s_pInstance;

	typedef struct StSoundStat
	{
		gxChar fileName[256];
		void init()
		{
			fileName[0] = 0x00;
		}
	} StSoundStat;

	typedef struct StPlayStat
	{
		Sint32 playID;
		Sint32 fileID;
		void init()
		{
			playID = 0;
			fileID = 0;
		}

	} StPlayStat;

	Sint32 m_KeyBoard[256];

	void setBlendFunc( Sint32 atr , ccBlendFunc *pBlend );

	StPlayStat m_sRefSoundID[ MAX_SOUND_NUM ];
	StSoundStat soundFile[MAX_SOUND_NUM];
};

#endif // __HELLOWORLD_SCENE_H__
