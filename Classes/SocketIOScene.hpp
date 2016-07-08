//
//  SocketIOScene.hpp
//  Cocos2dxSocketIOSample
//
//  Created by おかひろ on 2016/07/07.
//
//

#ifndef SocketIOScene_hpp
#define SocketIOScene_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class SocketIOScene : public cocos2d::Layer
{
public:
	Sprite *_square;
	
public:
	static cocos2d::Scene* createScene();
	
	virtual bool init();
	
	virtual void onEnter();
	virtual void onExit();
	
	CREATE_FUNC(SocketIOScene);
};

#endif /* SocketIOScene_hpp */
