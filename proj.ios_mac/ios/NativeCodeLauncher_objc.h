

//#import "CocosDenshion.h"
#if __IPHONE_OS_VERSION_MIN_REQUIRED >= 30000
#import <AVFoundation/AVFoundation.h>
#else
#import "CDXMacOSXSupport.h"
#endif


@interface NativeCodeLauncher : NSObject{
	
}

// シングルトンオブジェクトを返す
+ (NativeCodeLauncher *)sharedManager;

// Socket.IO
-(void)connectToSocketIO:(NSString *)host port:(int) port;
-(void)emitToSocketIO:(NSString *)event message:(NSString *)message;
-(void)disconnectFromSocketIO;


@end
