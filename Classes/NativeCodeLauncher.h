

#ifndef _NATIVE_CODE_LAUNCHER_H_
#define _NATIVE_CODE_LAUNCHER_H_

#include <stddef.h>
#include <string>

namespace Cocos2dExt {

    class NativeCodeLauncher
    {
    public:
		// Socket.IO関連
		static void connectToSocketIO(const char* host,int port);
		static void emitToSocketIO(const char* event,const char* message);
		static void disconnectFromSocketIO();
    };

} // end of namespace Cocos2dExt

#endif // _NATIVE_CODE_LAUNCHER_H_
