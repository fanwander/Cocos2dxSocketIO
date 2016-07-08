

#ifndef __NATIVE_CODE_LAUNCHER_JNI__
#define __NATIVE_CODE_LAUNCHER_JNI__

#include <jni.h>
#include <string>

extern "C"
{
	// Socket.IO関連処理
	extern void connectToSocketIOJNI(char const *host,int port);
	extern void emitToSocketIOJNI(char const *event,char const *message);
	extern void disconnectFromSocketIOJNI();
}

#endif // __NATIVE_CODE_LAUNCHER_JNI__