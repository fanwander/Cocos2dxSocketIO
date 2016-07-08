

#import "NativeCodeLauncher_objc.h"
//#import "EAGLView.h"
#import "AppController.h"
#import "RootViewController.h"
#import "AZSocketIO.h"
#import "SocketManager.hpp"

@implementation NativeCodeLauncher
{
	AZSocketIO *socketIO;
}


// シングルトンオブジェクト
static NativeCodeLauncher *sharedData_ = nil;

// シングルトンオブジェクトを返す
+ (NativeCodeLauncher *)sharedManager
{
	@synchronized(self)
	{
		if(!sharedData_)
		{
			sharedData_ = [NativeCodeLauncher new];
		}
	}
	
	return sharedData_;
}
- (id)init
{
	self = [super init];
	if(self)
	{
		// 初期化
		
	}
	
	return self;
}



// ソケット接続
-(void)connectToSocketIO:(NSString *)host port:(int) port
{
	// ホストとポート番号を指定してAZSocketIOインスタンス生成
	socketIO = [[AZSocketIO alloc] initWithHost:host andPort:[NSString stringWithFormat:@"%d",port] secure:NO];
	socketIO.reconnectionDelay = 1.0;
	socketIO.reconnectionLimit = 4;
	socketIO.maxReconnectionAttempts = 1;
	
	// 接続開始
	[socketIO connectWithSuccess:^{
		NSLog(@"Socket接続成功.");
		// 接続成功
		SocketManager::getInstance()->onConnected();
	} andFailure:^(NSError *error) {
		NSLog(@"Socket接続失敗. error: %@", error);
		// 接続失敗
		NSString *errorMessage = [NSString stringWithFormat:@"%@", error];
		SocketManager::getInstance()->onError([errorMessage UTF8String]);
	}];
	
	// イベントを受けとった時
	[socketIO setEventReceivedBlock:^(NSString *eventName, id data) {
		// ただのキャストではうまくいかない
		NSString *message = [NSString stringWithFormat:@"%@",data[0]];
		NSLog(@"Event : %@, message : %@",eventName,message);
		// イベントとメッセージを渡して仕分け
		SocketManager::getInstance()->receiveMessage([eventName UTF8String], [message UTF8String]);
	}];
	
	// エラーを受信したときに実行されるBlocks
	[socketIO setErrorBlock:^(NSError *error) {
		NSLog(@"error: %@", error);
		// エラー
		NSString *errorMessage = [NSString stringWithFormat:@"%@", error];
		SocketManager::getInstance()->onError([errorMessage UTF8String]);
	}];
	
 
	// 切断されたときに実行されるBlocks
	[socketIO setDisconnectedBlock:^{
		NSLog(@"Socket切断完了.");
		// 切断
		SocketManager::getInstance()->onDisconnected();
	}];
}
// イベントとメッセージをソケットに送信
-(void)emitToSocketIO:(NSString *)event message:(NSString *)message
{
	[socketIO emit:event args:message error:NULL];
}
// ソケット切断
-(void)disconnectFromSocketIO
{
	[socketIO disconnect];
}


@end
