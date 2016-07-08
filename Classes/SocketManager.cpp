//
//  SocketManager.cpp
//  Cocos2dxSocketIOSample
//
//  Created by おかひろ on 2016/07/07.
//
//

#include "SocketManager.hpp"
#include "NativeCodeLauncher.h"

SocketManager* SocketManager::mManager = NULL;

#pragma mark - 初期化

SocketManager::SocketManager()
{
	
}

SocketManager* SocketManager::getInstance()
{
	if(mManager == NULL)
	{
		mManager = new SocketManager();
	}
	
	return mManager;
}

// Socket接続
void SocketManager::connect()
{
	// 接続先のホスト、ポートは変更
	Cocos2dExt::NativeCodeLauncher::connectToSocketIO("192.168.100.1", 3150);
}
// Socket切断
void SocketManager::disconnect()
{
	Cocos2dExt::NativeCodeLauncher::disconnectFromSocketIO();
}

// Socket接続された
void SocketManager::onConnected()
{
	// Cocos2d-xのスレッドに実行させる。これがないと、Androidの描画関連がうまくいかないことがある。
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
		// イベントを通知
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("SocketConnected");
	});
}
// Socket切断された
void SocketManager::onDisconnected()
{
	// Cocos2d-xのスレッドに実行させる。これがないと、Androidの描画関連がうまくいかないことがある。
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
		// イベントを通知
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("SocketDisconnected");
	});
}
// Socketでエラーが発生した
void SocketManager::onError(std::string error)
{
	// Cocos2d-xのスレッドに実行させる。これがないと、Androidの描画関連がうまくいかないことがある。
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
		// イベントを通知
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("SocketError");
	});
}
// イベントを送信
void SocketManager::emitMessage(std::string event, std::string message)
{
	Cocos2dExt::NativeCodeLauncher::emitToSocketIO(event.c_str(),message.c_str());
}
// イベントを受信した
void SocketManager::receiveMessage(std::string event, std::string message)
{
	// Cocos2d-xのスレッドに実行させる。これがないと、Androidの描画関連がうまくいかないことがある。
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
		// イベントを通知
		EventCustom customEvent("SocketEventReceived");
		auto messageValue = Value(message);
		customEvent.setUserData(&messageValue);
		// 受け取った内容をEventCustomに入れて通知
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&customEvent);
	});
}


