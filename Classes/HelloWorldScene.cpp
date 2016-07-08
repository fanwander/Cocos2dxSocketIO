#include "HelloWorldScene.h"
#include "SocketIOScene.hpp"
#include "SocketManager.hpp"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
	
    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	Size winSize = Director::getInstance()->getWinSize();
	
	// 次の画面へ
	ui::Button *connectButton = ui::Button::create();
	connectButton->setTitleText("接続");
	connectButton->setTitleFontSize(42);
	connectButton->setPosition(Vec2(winSize.width * 0.5f,winSize.height * 0.8f));
	connectButton->addTouchEventListener([=](Ref* pSender,ui::Widget::TouchEventType type)
									   {
										   if(type == ui::Widget::TouchEventType::ENDED)
										   {
											   // Socket接続
											   SocketManager::getInstance()->connect();
										   }
									   });
	this->addChild(connectButton);
	
    
    return true;
}

void HelloWorld::onEnter()
{
	Director::getInstance()->getEventDispatcher()->addCustomEventListener("SocketConnected", [=](EventCustom *event){
		// 接続成功したらシーン遷移
		Scene *scene = SocketIOScene::createScene();
		TransitionFade *transition = TransitionFade::create(1.0f, scene, Color3B::WHITE);
		Director::getInstance()->replaceScene(transition);
	});
	
	Layer::onEnter();
}
void HelloWorld::onExit()
{
	Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("SocketConnected");
	
	Layer::onExit();
}
