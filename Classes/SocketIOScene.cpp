//
//  SocketIOScene.cpp
//  Cocos2dxSocketIOSample
//
//  Created by おかひろ on 2016/07/07.
//
//

#include "SocketIOScene.hpp"
#include "HelloWorldScene.h"
#include "SocketManager.hpp"

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"


Scene* SocketIOScene::createScene()
{
	auto scene = Scene::create();
	auto layer = SocketIOScene::create();
	scene->addChild(layer);
	
	return scene;
}

bool SocketIOScene::init()
{
	if ( !Layer::init() )
	{
		return false;
	}
	
	Size winSize = Director::getInstance()->getWinSize();
	
	// 説明
	Label *descriptionLabel = Label::createWithSystemFont("四角形を動かすと、接続している全ての端末で動きます", "", 24);
	descriptionLabel->setPosition(Vec2(winSize.width * 0.5f,winSize.height * 0.9f));
	this->addChild(descriptionLabel);
	
	// 切断
	ui::Button *disconnectButton = ui::Button::create();
	disconnectButton->setTitleText("切断");
	disconnectButton->setTitleFontSize(42);
	disconnectButton->setPosition(Vec2(winSize.width * 0.5f,winSize.height * 0.1f));
	disconnectButton->addTouchEventListener([=](Ref* pSender,ui::Widget::TouchEventType type)
										 {
											 if(type == ui::Widget::TouchEventType::ENDED)
											 {
												 Scene *scene = HelloWorld::createScene();
												 TransitionFade *transition = TransitionFade::create(1.0f, scene, Color3B::WHITE);
												 Director::getInstance()->replaceScene(transition);
												 
												 SocketManager::getInstance()->disconnect();
											 }
										 });
	this->addChild(disconnectButton);
	
	// 四角形スプライト
	_square = Sprite::create("res/square.png");
	_square->setPosition(winSize * 0.5f);
	this->addChild(_square);
	
	// スプライトを指で動かせる
	auto listenerForSprite = EventListenerTouchOneByOne::create();
	listenerForSprite->setSwallowTouches(true);
	listenerForSprite->onTouchBegan = [=](Touch* touch, Event* event)
	{
		Vec2 pos = touch->getLocation();
		
		if(_square->getBoundingBox().containsPoint(pos))
		{
			_square->setPosition(pos);
			return true;
		}
		
		return false;
	};
	listenerForSprite->onTouchMoved = [=](Touch* touch, Event* event)
	{
		Vec2 pos = touch->getLocation();
		
		_square->setPosition(pos);
		
		// Spriteの座標をJSONにして送信（実際は、0.2秒おきなどの間隔で送信したほうが良い）
		rapidjson::StringBuffer s;
		rapidjson::Writer<rapidjson::StringBuffer> writer(s);
		writer.StartObject();
		writer.String("x");
		writer.Int(_square->getPositionX());
		writer.String("y");
		writer.Int(_square->getPositionY());
		writer.EndObject();
		// 送信
		SocketManager::getInstance()->emitMessage("touch", s.GetString());
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerForSprite, this);
	
	
	
	return true;
}


void SocketIOScene::onEnter()
{
	Director::getInstance()->getEventDispatcher()->addCustomEventListener("SocketEventReceived", [=](EventCustom *event){
		// Spriteの移動メッセージを受け取った
		auto messageValue = (Value*)event->getUserData();
		
		rapidjson::Document doc;
		doc.Parse<rapidjson::kParseDefaultFlags>(messageValue->asString().c_str());
		
		// 座標を取得してSpriteのいちを設定
		int x = doc["x"].GetInt();
		int y = doc["y"].GetInt();
		_square->setPosition(Vec2(x,y));
	});
	
	Director::getInstance()->getEventDispatcher()->addCustomEventListener("SocketError", [=](EventCustom *event){
		// エラーが発生した場合はSocket切断してシーン遷移
		Scene *scene = HelloWorld::createScene();
		TransitionFade *transition = TransitionFade::create(1.0f, scene, Color3B::WHITE);
		Director::getInstance()->replaceScene(transition);
		
		SocketManager::getInstance()->disconnect();
	});
	
	Layer::onEnter();
}
void SocketIOScene::onExit()
{
	Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("SocketEventReceived");
	Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("SocketError");
	
	Layer::onExit();
}
