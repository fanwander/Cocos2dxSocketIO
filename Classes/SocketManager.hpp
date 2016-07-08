//
//  SocketManager.hpp
//  Cocos2dxSocketIOSample
//
//  Created by おかひろ on 2016/07/07.
//
//

#ifndef SocketManager_hpp
#define SocketManager_hpp

#include "cocos2d.h"

USING_NS_CC;

class SocketManager
{
private:
	static SocketManager* mManager;
	
public:
	SocketManager();
	
	static SocketManager* getInstance();
	
	// ソケット接続/切断
	void connect();
	void disconnect();
	// 接続された
	void onConnected();
	// 切断された
	void onDisconnected();
	// エラーが発生した
	void onError(std::string error);
	// メッセージを送信
	void emitMessage(std::string event,std::string message);
	// メッセージを受け取った時の処理
	void receiveMessage(std::string event,std::string message);
};

#endif /* SocketManager_hpp */
