//----------------------------------------------------------
//
// appMain for Cocos2Dx ver3.9
//
//----------------------------------------------------------
#include <gxLib.h>
#include "SimpleAudioEngine.h"
#include "appMain.h"

#include <gxLib/gx.h>
#include <gxLib/gxOrderManager.h>
#include <gxLib/gxTexManager.h>
#include <gxLib/gxRender.h>
#include <gxLib/gxPadManager.h>
#include <gxLib/gxSoundManager.h>


void MakeThread( void (*pFunc)(void*) , void * pArg ){};
Sprite *m_pSpr;
gxBool vSync( gxBool bNoWait){return gxTrue;};
void Flip(){};
void Resume(){};
void Movie(){};
void Clock(){};
gxBool PadConfig(Sint32 padNo, Uint32 button) { return gxTrue; };


HelloWorld *HelloWorld::s_pInstance = NULL;

void InitAction()
{
	HelloWorld::GetInstance()->InitAction();
};

void Upload( Sint32 sBank )
{
	HelloWorld::GetInstance()->Upload( sBank );
}

void Render()
{
	HelloWorld::GetInstance()->Render();
}

void LogDisp(char* pString)
{
	CCLOG( pString );
};

Uint8* LoadFile( const gxChar* pFileName , Uint32* pLength )
{

    gxChar fileName[256];
    Uint32 len = strlen(pFileName);
    Uint32 num = 0;

    for(Uint32 ii=0;ii<len;ii++)
    {
/*
 if( pFileName[num] == 'ｴｴ' )
       {
           fileName[ii] = '/';
           fileName[ii+1] = 0x00;
           continue;
       }
       else
*/
           if( pFileName[num] == '\\' )
       {
           fileName[ii] = '/';
           fileName[ii+1] = 0x00;
           num +=1;
           continue;
       }
        fileName[ii] = pFileName[num];
        fileName[ii+1] = 0x00;
        num ++;
    }
    
    ssize_t sz = 1024;
	Uint8* pData = CCFileUtils::sharedFileUtils()->getFileData( fileName, "rb", &sz );

	*pLength = sz;

	return pData;

};

gxBool SaveFile( const gxChar* pFileName , Uint8* pReadBuf , Uint32 uSize )
{
	String str1 = CCFileUtils::sharedFileUtils()->getWritablePath(); 
	String str2;
	str2.appendWithFormat("%s%s", str1.getCString(), pFileName);

	cocos2d::Data data;
	data.copy(pReadBuf, uSize);

	bool ret = 	CCFileUtils::sharedFileUtils()->writeDataToFile( data , str2.getCString() );

	return ret;
};

void Play()
{
	HelloWorld::GetInstance()->Play();
}


//----------------------------------------------------------------
//
// Cocos とのつなぎ込み
//
//----------------------------------------------------------------


Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	//Director::getInstance()->runWithScene( scene );

	// return the scene
	return scene;
}


// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	s_pInstance = this;

	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	// add "HelloWorld" splash screen"
	//auto sprite = Sprite::create("HelloWorld.png");
	//m_pSpr = Sprite::create("shtimg.png");//HelloWorld.png");

	// position the sprite on the center of the screen
	//m_pSpr->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	// add the sprite as a child to this layer
	//    this->addChild(sprite, 0);
	//this->addChild( m_pSpr, 0);

	this->scheduleUpdate();

	m_sCount = 0;

	m_pTexture = new StTexture[enTextureMax];

	//------------------------------------------------------------------------------
	// Mouse
	//------------------------------------------------------------------------------
	auto mouseListener = EventListenerMouse::create();

	mouseListener->onMouseMove = [=](Event* event)
	{
		auto mouse = (EventMouse*)event;

		//cursor->setPosition(Point(mouse->getCursorX(), mouse->getCursorY()) / EGL_SCALE);

		m_Mouse.mouseX = mouse->getCursorX();
		m_Mouse.mouseY = mouse->getCursorY();
	};

	mouseListener->onMouseDown = [=](Event* event)
	{
		auto mouse = (EventMouse*)event;
		int btn = mouse->getMouseButton();
		if (btn == MOUSE_BUTTON_LEFT)
		{
			m_Mouse.buttonL = true;
		}
		if (btn == MOUSE_BUTTON_RIGHT)
		{
			m_Mouse.buttonR = true;
		}
	};

	mouseListener->onMouseUp = [=](Event* event)
	{
		auto mouse = (EventMouse*)event;
		int btn = mouse->getMouseButton();
		if (btn == MOUSE_BUTTON_LEFT)
		{
			m_Mouse.buttonL = false;
		}
		if (btn == MOUSE_BUTTON_RIGHT)
		{
			m_Mouse.buttonR = false;
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

	//------------------------------------------------------------------------------
	// Touch
	//------------------------------------------------------------------------------

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [&](Touch* touch, Event* event)
	{
		m_Mouse.buttonL = true;
		Vec2 pos = touch->getLocation();
		m_Mouse.mouseX = pos.x;
		m_Mouse.mouseY = pos.y;
		return true;
	};

	touchListener->onTouchMoved = [&](Touch* touch, Event* event)
	{
		m_Mouse.buttonL = true;
		Vec2 pos = touch->getLocation();
		m_Mouse.mouseX = pos.x;
		m_Mouse.mouseY = pos.y;
		return true;
	};

	touchListener->onTouchEnded = [&](Touch* touch, Event* event)
	{
		m_Mouse.buttonL = false;
		Vec2 pos = touch->getLocation();
		m_Mouse.mouseX = pos.x;
		m_Mouse.mouseY = pos.y;
		return true;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//------------------------------------------------------------------------------
	//KeyBoard
	//------------------------------------------------------------------------------

	for( Sint32 ii=0;ii<256; ii++ )
	{
		m_KeyBoard[ii] = 0x00;
	}

	auto keyBoardListener = EventListenerKeyboard::create();

	keyBoardListener->onKeyPressed = [&]( EventKeyboard::KeyCode key, Event* event)
	{
		Sint32 index = (Sint32)key;
		m_KeyBoard[index] = 1;
		//CPadManager::GetInstance()->SetKeyDown( index );
		//return true;
	};

	keyBoardListener->onKeyReleased = [&]( EventKeyboard::KeyCode key, Event* event)
	{
		Sint32 index = (Sint32)key;
		m_KeyBoard[index] = 2;
		//CPadManager::GetInstance()->SetKeyUp( index );
		//return true;
	};

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);
    //	ゲームコントローラ関連イベントハンドラを登録する。
	//	auto controllerListener = EventListenerKeyboard::create();// EventListenerController::create();
    //	コントローラの検出を開始する。
	//	Controller::startDiscoveryController();


	gxLib::LoadConfig();	//移植時注意！！！！

	for( Sint32 ii=0;ii<MAX_SOUND_NUM; ii++ )
	{
		soundFile[ ii ].init();
		m_sRefSoundID[ii].init();
	}

	return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
	gxLib::SaveConfig();	//移植時注意！！！！

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}



void HelloWorld::update(float delta)
{
	//位置と回転を設定する（ 成功 ）

	m_sCount++;

	Action();

	return;
}


void HelloWorld::InitAction()
{
	//メインループの初期化

    //スプライトのリクエストを初期化する

//    if( m_pTexture[0].pSpriteBatchNode ) m_pTexture[0].pSpriteBatchNode->removeAllChildrenWithCleanup(true);
//    if( m_pTexture[1].pSpriteBatchNode ) m_pTexture[1].pSpriteBatchNode->removeAllChildrenWithCleanup(true);

	this->removeAllChildrenWithCleanup(true);

    //マウスの入力を検出する

    Size visibleSize = Director::getInstance()->getVisibleSize();
    m_JoyStick[0].mx = m_Mouse.mouseX;
    m_JoyStick[0].my = visibleSize.height - m_Mouse.mouseY;

    int push = 0;

    if( m_Mouse.buttonL ) push |= MOUSE_L;
    if( m_Mouse.buttonR ) push |= MOUSE_R;

    m_JoyStick[0].psh = push;

	CPadManager::GetInstance()->SetPadInfo( 0 , push );
	CPadManager::GetInstance()->SetPadInfo( 1 , 0x00000000 );
	CPadManager::GetInstance()->SetMousePosition( m_JoyStick[0].mx , m_JoyStick[0].my );

	//キーボードの状態を伝える

	for( Sint32 ii=0; ii<256; ii++ )
	{
		if( m_KeyBoard[ii] == 0x01 )
		{
			CPadManager::GetInstance()->SetKeyDown( ii );
		}
		else if( m_KeyBoard[ii] == 0x02 )
		{
			CPadManager::GetInstance()->SetKeyUp( ii );
		}

		m_KeyBoard[ii] = 0x00;
	}

}


void HelloWorld::Action()
{
    int page = 0;

	CGameGirl::GetInstance()->Main();
}



void HelloWorld::Render()
{

	Sint32 sMax = COrderManager::GetInstance()->GetOrderNum();

	Uint32* pZsortBuf = COrderManager::GetInstance()->getZsortBuf();

    for(Uint16 ii=0; ii<sMax; ii++)
    {
        StOrder* pOdr = COrderManager::GetInstance()->get( (Uint32)pZsortBuf[ii] );
        render(pOdr);
        while( pOdr->m_pNextOrder )
        {
            pOdr = pOdr->m_pNextOrder;
            render(pOdr);
        }
        
    
    }

}


void HelloWorld::render( void *_pOdr )
{
    StOrder *pOdr = (StOrder*)_pOdr;
		switch( pOdr->sType ){
		case enOrderTypePoint:
			{
			    Size visibleSize = Director::getInstance()->getVisibleSize();

				Float32 a, r, g, b;
				a = 1.f*((pOdr->color[0]&0xff000000)>>24)/255;
				r = 1.f*((pOdr->color[0]&0x00ff0000)>>16)/255;
				g = 1.f*((pOdr->color[0]&0x0000ff00)>>8)/255;
				b = 1.f*((pOdr->color[0]&0x000000ff)>>0)/255;

				auto draw = DrawNode::create();
				draw->drawDot(Vec2( pOdr->x1[0] , visibleSize.height - pOdr->y1[0] ), 1.0f, Color4F( r, g, b , a ) );

			    ccBlendFunc blend;
				setBlendFunc( pOdr->sAtribute, &blend);
				draw->setBlendFunc( blend );

				// drawDot(ポジション, 半径, 色);
				addChild( draw );
			}
			break;

		case enOrderTypeTriangle:
			{
			    Size visibleSize = Director::getInstance()->getVisibleSize();

				auto draw = DrawNode::create();

				Float32 a, r, g, b;
				a = 1.f*((pOdr->color[0]&0xff000000)>>24)/255;
				r = 1.f*((pOdr->color[0]&0x00ff0000)>>16)/255;
				g = 1.f*((pOdr->color[0]&0x0000ff00)>>8)/255;
				b = 1.f*((pOdr->color[0]&0x000000ff)>>0)/255;

				draw->drawTriangle(
					Vec2( pOdr->x1[0] , visibleSize.height - pOdr->y1[0] ),
					Vec2( pOdr->x1[1] , visibleSize.height - pOdr->y1[1] ),
					Vec2( pOdr->x1[2] , visibleSize.height - pOdr->y1[2] ),
					Color4F( r, g, b , a ) );

			    ccBlendFunc blend;
				setBlendFunc( pOdr->sAtribute, &blend);
				draw->setBlendFunc( blend );

				//drawTriangle(１番目の点,２番目の点,３番目の点,色);			}
				addChild( draw );
			}
			break;
		case enOrderTypeLine:
			{
			    Size visibleSize = Director::getInstance()->getVisibleSize();

				Sint32 x1 = pOdr->x1[0];
				Sint32 y1 = visibleSize.height - pOdr->y1[0];

				Sint32 x2 = pOdr->x1[1];
				Sint32 y2 = visibleSize.height - pOdr->y1[1];

				Float32 a, r, g, b;

				a = 1.f*((pOdr->color[0]&0xff000000)>>24)/255;
				r = 1.f*((pOdr->color[0]&0x00ff0000)>>16)/255;
				g = 1.f*((pOdr->color[0]&0x0000ff00)>>8)/255;
				b = 1.f*((pOdr->color[0]&0x000000ff)>>0)/255;

				auto draw = DrawNode::create();
				draw->drawSegment( Vec2( x1 , y1 ), Vec2( x2 , y2  ), 1.0f, Color4F( r , g ,b , a ) );
				//drawSegment(スタートポイント,エンドポイント,太さ,色);			break;

			    ccBlendFunc blend;
				setBlendFunc( pOdr->sAtribute, &blend);
				draw->setBlendFunc( blend );

				addChild( draw );
			}
			break;

		case enOrderTypWired:
			{
			    Size visibleSize = Director::getInstance()->getVisibleSize();
			    ccBlendFunc blend;

				Sint32 x1 = pOdr->x;
				Sint32 y1 = visibleSize.height - pOdr->y;
				Sint32 x2 = pOdr->x + pOdr->w;
				Sint32 y2 = visibleSize.height - (pOdr->y + pOdr->h);

				Float32 a, r, g, b;

				a = 1.f*((pOdr->color[0]&0xff000000)>>24)/255;
				r = 1.f*((pOdr->color[0]&0x00ff0000)>>16)/255;
				g = 1.f*((pOdr->color[0]&0x0000ff00)>>8)/255;
				b = 1.f*((pOdr->color[0]&0x000000ff)>>0)/255;

				auto draw = DrawNode::create();
				draw->drawSegment( Vec2( x1 , y1 ) , Vec2( x2 , y1 ), 1.0f, Color4F( r , g ,b , a ) );

				setBlendFunc( pOdr->sAtribute, &blend);
				draw->setBlendFunc( blend );

				addChild( draw );

				draw = DrawNode::create();
				draw->drawSegment( Vec2( x2 , y1 ) , Vec2( x2 , y2 ), 1.0f, Color4F( r , g ,b , a ) );

				setBlendFunc( pOdr->sAtribute, &blend);
				draw->setBlendFunc( blend );

				addChild( draw );

				draw = DrawNode::create();
				draw->drawSegment( Vec2( x2 , y2 ) , Vec2( x1 , y2 ), 1.0f, Color4F( r , g ,b , a ) );
				setBlendFunc( pOdr->sAtribute, &blend);
				draw->setBlendFunc( blend );
				addChild( draw );

				draw = DrawNode::create();
				draw->drawSegment( Vec2( x1 , y2 ) , Vec2( x1 , y1 ), 1.0f, Color4F( r , g ,b , a ) );
				setBlendFunc( pOdr->sAtribute, &blend);
				draw->setBlendFunc( blend );
				addChild( draw );
				//drawSegment(スタートポイント,エンドポイント,太さ,色);			break;
			}
			break;

		case enOrderTypeNoneTexPolygon:
			{
				break;

			    Size visibleSize = Director::getInstance()->getVisibleSize();

				std::vector<Vec2> vecs;

				Sint32 x = pOdr->x;
				Sint32 y = visibleSize.height - pOdr->y;

 				vecs.push_back(Vec2( x            , y ) );
 				vecs.push_back(Vec2( x + pOdr->w  , y ) );
 				vecs.push_back(Vec2( x + pOdr->w  , y - pOdr->h) );
 				vecs.push_back(Vec2( x + 0        , y - pOdr->h) );

				Float32 a, r, g, b;

				a = 1.f*((pOdr->color[0]&0xff000000)>>24)/255;
				r = 1.f*((pOdr->color[0]&0x00ff0000)>>16)/255;
				g = 1.f*((pOdr->color[0]&0x0000ff00)>>8)/255;
				b = 1.f*((pOdr->color[0]&0x000000ff)>>0)/255;

 				auto draw = DrawNode::create();
				draw->drawPolygon( &vecs[0], 4, Color4F( r  , g , b , a ), 0, Color4F(0.f,0.f,0.f,0.f) );
				//drawPolygon(点の配列, 点の数, 色, アウトラインの太さ,アウトラインの色);			break;

			    ccBlendFunc blend;
				setBlendFunc( pOdr->sAtribute, &blend);
				draw->setBlendFunc( blend );
				addChild( draw );
			}
			break;

		case enOrderTypeTexturePolygon:
			PutSprite( pOdr->x , pOdr->y , pOdr->pr , pOdr->pg , pOdr->u , pOdr->v , pOdr->w , pOdr->h , pOdr->cx , pOdr->cy , pOdr->sAtribute , pOdr->color[0] , pOdr->rz , pOdr->sx ,pOdr->sy );
			break;

		case enOrderTypeTextureTriangle:
			break;

		case enOrderTypeFont:
			break;

		default:
			break;
		}

}




bool HelloWorld::LoadTexture( int page , char *pFileName )
{
	if( m_pTexture[page].pData == NULL )
	{
		unsigned char *pData = NULL;

		m_pTexture[page].w  = 256;
		m_pTexture[page].h  = 256;
		m_pTexture[page].depth = 4;
		m_pTexture[page].length = m_pTexture[page].w * m_pTexture[page].h * m_pTexture[page].depth;
		pData = (unsigned char*)malloc(m_pTexture[page].length);
		m_pTexture[page].pData = pData;
		memset( pData , 0xff , m_pTexture[page].length );

		Size size;
		size.setSize( m_pTexture[page].w , m_pTexture[page].h );

		m_pTexture[page].m_pTex = new CCTexture2D();
		m_pTexture[page].m_pTex->initWithData( pData, m_pTexture[page].length, Texture2D::PixelFormat::RGBA8888, m_pTexture[page].w, m_pTexture[page].h, size );

		if( pFileName == NULL )
		{
			m_pTexture[page].pSpriteBatchNode = SpriteBatchNode::createWithTexture(m_pTexture[page].m_pTex);
		}
		else
		{
			m_pTexture[page].pSpriteBatchNode = SpriteBatchNode::create( pFileName );
		}

		this->addChild(  m_pTexture[page].pSpriteBatchNode );

	}

	return true;
}

bool HelloWorld::PutSprite( int x , int y , int prio , int page , int u , int v , int w , int h , int cx , int cy , int atr , int argb , float r , float fx ,float fy )
{
    auto hoge = Sprite::create();

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 pos;
    pos.x = x;
    pos.y = (visibleSize.height - y);

    hoge->setPosition( pos.x , pos.y );

    Rect rect;
    Vec2 anchor;
    anchor.set( 1.0f*cx/w , 1.0f*(h-cy)/h);

	u += (((page%16) % 4) * 256);
	v += (((page % 16) / 4) * 256);

	page = page / 16;

	rect.setRect(u,v,w,h);

    hoge->setTexture( m_pTexture[page].m_pTex );

    hoge->setTextureRect( rect );

    hoge->setAnchorPoint(anchor);
    hoge->setRotation( r );
    hoge->setScale(fx , fy );
    Color3B color( (argb&0x00ff0000)>>16 , (argb&0x0000ff00)>>8 , (argb&0x000000ff)>>0 );
    hoge->setColor( color );
    hoge->setOpacity( (argb&0xff000000)>>24 );

    ccBlendFunc blend;

	setBlendFunc(atr, &blend);

    hoge->setBlendFunc(blend);

	addChild(hoge);

//    m_pTexture[page].pSpriteBatchNode->addChild(hoge);

	return true;
}

void HelloWorld::LoadAudio()
{

}

void HelloWorld::PlayAudio()
{

}


StJoyStick* HelloWorld::GetJoyStat( int player )
{
    
    return &m_JoyStick[player];
}


void HelloWorld::Upload( Sint32 sBank )
{
	gxChar *pFileName = NULL;
	
    {
        Size size;

		if( m_pTexture[sBank].m_pTex == NULL )
		{
			m_pTexture[sBank].m_pTex = new CCTexture2D();
		}

        if( pFileName == NULL )
        {
			m_pTexture[sBank].depth  = 4;
	        m_pTexture[sBank].length = CTexManager::enMasterWidth * CTexManager::enMasterHeight * m_pTexture[sBank].depth;
	        m_pTexture[sBank].pData   = CTexManager::GetInstance()->GetAtlasTexture( sBank )->GetTexelImage();

			Uint8 *pData = (Uint8*)malloc( m_pTexture[sBank].length );
			Sint32 h = CTexManager::enMasterHeight;
			for( Sint32 y=0;y<h;y++)
			{
				for( Sint32 x=0;x<CTexManager::enMasterWidth;x++)
				{
					Uint32 *pDst = (Uint32*)&pData[((y*CTexManager::enMasterWidth) + x) * 4];
					Uint32 *pSrc = (Uint32*)(&m_pTexture[sBank].pData[((h - y-1)*(CTexManager::enMasterWidth) + x) * 4] );
					*pDst = *pSrc;
				}
			}

			m_pTexture[sBank].pData = pData;
	        size.setSize( CTexManager::enMasterWidth, CTexManager::enMasterHeight );
	        m_pTexture[ sBank ].m_pTex->initWithData(m_pTexture[sBank].pData, m_pTexture[ sBank ].length, Texture2D::PixelFormat::RGBA8888/*RGBA8888*/, CTexManager::enMasterWidth, CTexManager::enMasterHeight, size );
            m_pTexture[ sBank ].pSpriteBatchNode = SpriteBatchNode::createWithTexture( m_pTexture[ sBank ].m_pTex );

			free( pData );
        }

        this->addChild(  m_pTexture[sBank].pSpriteBatchNode );
    }
}


void HelloWorld::Play()
{
	for(Sint32 ii=0; ii<MAX_SOUND_NUM; ii++)
	{
		StPlayInfo *pInfo;

		pInfo = CSoundManager::GetInstance()->GetPlayInfo( ii );

		if( pInfo->bReq )
		{
			if( pInfo->uStatus & enSoundReqStop )
			{
				//StopSound( ii );
				Sint32 id = m_sRefSoundID[ ii ].playID;

				if (ii == 0)
				{
					CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
				}
				else
				{
					CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(id);
				}
			}

			if( pInfo->uStatus & enSoundReqLoad )
			{
				int fileID = -1;
				
				for( Sint32 jj=0; jj<MAX_SOUND_NUM; jj++ )
				{
					if( strcmp( soundFile[jj].fileName , pInfo->fileName ) == 0 )
					{
						//既にある
						fileID = ii;
						break;
					}
				}
				
				if( fileID == -1 )
				{
					//新規
				
					for( Sint32 jj=0; jj<MAX_SOUND_NUM; jj++ )
					{
						if( soundFile[jj].fileName[0] == 0x00 )
						{
							fileID = ii;
							break;
						}
					}

					if( fileID == -1 )
					{
						//空きもないので処理中止
						continue;
					}

					if (soundFile[fileID].fileName[0])//pInfo->fileName[0] )
					{
						//既に音が入っている場合はその音を消す
						if (ii == 0)
						{
							//CocosDenshion::SimpleAudioEngine::sharedEngine()->bgm willPlayBackgroundMusic();
						}
						else
						{
							CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect(soundFile[fileID].fileName);// pInfo->fileName );
						}
					}

					//新規ファイル読み込み
					if (ii == 0 )
					{
						CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(pInfo->fileName);
					}
					else
					{
						CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(pInfo->fileName);
					}
					sprintf( soundFile[fileID].fileName , "%s" , pInfo->fileName );
					SAFE_DELETES( pInfo->m_pTempBuffer );
					pInfo->m_uTempSize = 0;
				}

				m_sRefSoundID[ ii ].fileID = fileID;

				//read( ii );
				

			}
			if( pInfo->uStatus & enSoundReqPlay )
			{
				Sint32 n = m_sRefSoundID[ ii ].fileID;
				Sint32 id = 0;

				if (ii == 0 )
				{
					CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(soundFile[n].fileName, pInfo->bLoop );
				}
				else
				{
					id = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(soundFile[n].fileName, pInfo->bLoop);

				}
				m_sRefSoundID[ ii ].playID = id;
			}

			if( pInfo->uStatus & enSoundReqVolume )
			{
//				SetVolume( ii );
			}

			if (pInfo->uStatus & enSoundReqChangeFreq)
			{
//				SetFrequency(ii);
			}

			pInfo->bReq = gxFalse;
			pInfo->uStatus = enSoundReqNone;
		}

	}
}

void HelloWorld::setBlendFunc( Sint32 atr , ccBlendFunc *pBlend )
{
    ccBlendFunc blend;

	switch( atr&0x0000000f ){
	//case ATR_DFLT		://(0x00000000)
	case ATR_ALPHA_NML	://(0x00000000)
	    //ブレンドノーマル
	    blend.src = GL_SRC_ALPHA;//GL_ONE_MINUS_DST_COLOR;
	    blend.dst = GL_ONE_MINUS_SRC_ALPHA;//GL_ONE;
		break;
	case ATR_ALPHA_ADD	://(0x00000001)		//加算
	    //加算
	    blend.src = GL_SRC_ALPHA;
	    blend.dst = GL_ONE;
		break;
	case ATR_ALPHA_SUB	://(0x00000002)		//減算
	    //減算
	    blend.src = GL_ZERO;
	    blend.dst = GL_ONE_MINUS_SRC_COLOR;
		break;
	case ATR_ALPHA_CRS   ://(0x00000003)		//乗算
	    //乗算
	    blend.src = GL_ZERO;
	    blend.dst = GL_SRC_COLOR;
		break;
	case ATR_ALPHA_RVS 	://(0x00000004)		//反転
	    //反転
	    blend.src = GL_ONE_MINUS_DST_COLOR;
	    blend.dst = GL_ZERO;
		break;
	case ATR_ALPHA_XOR 	://(0x00000005)		//XOR
	    //XOR
	    blend.src = GL_ONE_MINUS_DST_COLOR;
	    blend.dst = GL_ONE_MINUS_SRC_COLOR;
		break;
	case ATR_ALPHA_SCR 	://(0x00000006)		//スクリーン乗算
	    //スクリーン
	    blend.src = GL_ONE_MINUS_DST_COLOR;
	    blend.dst = GL_ONE;
		break;
	default:
	    //
	    blend.src = GL_SRC_ALPHA;//GL_ONE_MINUS_DST_COLOR;
	    blend.dst = GL_ONE_MINUS_SRC_ALPHA;//GL_ONE;
		break;
	}

	*pBlend = blend;
}

/*
void onConnectedController(Controller* controller, Event* event)
{
    log("Game controller connected");
}
 
void onDisconnectedController(Controller* controller, Event* event)
{
    log("Game controller disconnected");
}
*/


