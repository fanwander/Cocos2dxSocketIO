

#include "NativeCodeLauncher.h"
#include "NativeCodeLauncherJni.h"

namespace Cocos2dExt
{
	// Socket.IO関連処理
	void NativeCodeLauncher::connectToSocketIO(char const *host,int port)
	{
		connectToSocketIOJNI(host,port);
	}
	void NativeCodeLauncher::emitToSocketIO(char const *event,char const *message)
	{
		emitToSocketIOJNI(event,message);
	}
	void NativeCodeLauncher::disconnectFromSocketIO()
	{
		disconnectFromSocketIOJNI();
	}
}
