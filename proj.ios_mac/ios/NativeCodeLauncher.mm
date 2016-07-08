

#include "NativeCodeLauncher.h"
#include "NativeCodeLauncher_objc.h"


// Socket.IO関連処理
static void static_connectToSocketIO(const char* host,int port)
{
	[[NativeCodeLauncher sharedManager] connectToSocketIO: [NSString stringWithUTF8String:host] port: port];
}
static void static_emitToSocketIO(const char* event,const char* message)
{
	[[NativeCodeLauncher sharedManager] emitToSocketIO:[NSString stringWithUTF8String:event]
											   message:[NSString stringWithUTF8String:message]];
}
static void static_disconnectFromSocketIO()
{
	[[NativeCodeLauncher sharedManager] disconnectFromSocketIO];
}



namespace Cocos2dExt
{
	// ソケット接続
	void NativeCodeLauncher::connectToSocketIO(const char* host,int port)
	{
		static_connectToSocketIO(host,port);
	}
	// イベントとメッセージをソケットに送信
	void NativeCodeLauncher::emitToSocketIO(const char* event,const char* message)
	{
		static_emitToSocketIO(event,message);
	}
	// ソケット切断
	void NativeCodeLauncher::disconnectFromSocketIO()
	{
		static_disconnectFromSocketIO();
	}
}