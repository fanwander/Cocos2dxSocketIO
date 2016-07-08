

#include "NativeCodeLauncherJni.h"
#include <android/log.h>
#include "platform/android/jni/JniHelper.h"
#include "SocketManager.hpp"

#define  LOG_TAG    "NativeCodeLauncher"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
//#define  CLASS_NAME "org/cocos2dx/lib/Cocos2dxActivity"
#define CLASS_NAME "org/cocos2dx/cpp/AppActivity"

typedef struct JniMethodInfoEx_
{
    JNIEnv *    env;
    jclass      classID;
    jmethodID   methodID;
} JniMethodInfoEx;

extern "C"
{
	// get env and cache it
	static JNIEnv* getJNIEnv(void)
	{
		JNIEnv *env = 0;

		// get jni environment
		if (cocos2d::JniHelper::getJavaVM()->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
		{
			LOGD("Failed to get the environment using GetEnv()");
		}

		if (cocos2d::JniHelper::getJavaVM()->AttachCurrentThread(&env, 0) < 0)
		{
			LOGD("Failed to get the environment using AttachCurrentThread()");
		}

		return env;
	}

	// get class and make it a global reference, release it at endJni().
	static jclass getClassID(JNIEnv *pEnv)
	{
		jclass ret = pEnv->FindClass(CLASS_NAME);
		if (! ret)
		{
			LOGD("Failed to find class of %s", CLASS_NAME);
		}

		return ret;
	}

	static bool getStaticMethodInfo(JniMethodInfoEx &methodinfo, const char *methodName, const char *paramCode)
    {
		jmethodID methodID = 0;
		JNIEnv *pEnv = 0;
		bool bRet = false;

        do 
        {
			pEnv = getJNIEnv();
			if (! pEnv)
			{
				break;
			}

            jclass classID = getClassID(pEnv);

            methodID = pEnv->GetStaticMethodID(classID, methodName, paramCode);
            if (! methodID)
            {
                LOGD("Failed to find static method id of %s", methodName);
                break;
            }

			methodinfo.classID = classID;
			methodinfo.env = pEnv;
			methodinfo.methodID = methodID;

			bRet = true;
        } while (0);

        return bRet;
    }
	
	
	// Socket.IO
	void connectToSocketIOJNI(char const *host,int port)
	{
		JniMethodInfoEx methodInfo;
		
		if (!getStaticMethodInfo(methodInfo, "connectToSocketIO", "(Ljava/lang/String;I)V"))
		{
			return;
		}
		jstring hostArg = methodInfo.env->NewStringUTF(host);
		
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,hostArg,port);
		methodInfo.env->DeleteLocalRef(hostArg);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
	void emitToSocketIOJNI(char const *event,char const *message)
	{
		JniMethodInfoEx methodInfo;
		
		if (!getStaticMethodInfo(methodInfo, "emitToSocketIO", "(Ljava/lang/String;Ljava/lang/String;)V"))
		{
			return;
		}
		
		jstring eventArg = methodInfo.env->NewStringUTF(event);
		jstring messageArg = methodInfo.env->NewStringUTF(message);
		
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, eventArg,messageArg);
		methodInfo.env->DeleteLocalRef(eventArg);
		methodInfo.env->DeleteLocalRef(messageArg);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
	void disconnectFromSocketIOJNI()
	{
		JniMethodInfoEx methodInfo;
		
		if (!getStaticMethodInfo(methodInfo, "disconnectFromSocketIO", "()V"))
		{
			return;
		}
		
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
	// メッセージを受け取った時の処理
	void Java_org_cocos2dx_cpp_AppActivity_nativeReceiveMessage(JNIEnv *env,jobject thiz,
																  jstring event,jstring message)
	{
		const char *eventChar = env->GetStringUTFChars(event,0);
		const char *messageChar = env->GetStringUTFChars(message,0);
		
		SocketManager::getInstance()->receiveMessage(eventChar,messageChar);
		
		env->ReleaseStringUTFChars(event, eventChar);
		env->ReleaseStringUTFChars(message, messageChar);
	}
	// Socket接続された時
	void Java_org_cocos2dx_cpp_AppActivity_nativeOnSocketConnected(JNIEnv *env,jobject thiz)
	{
		SocketManager::getInstance()->onConnected();
	}
	// Socket切断された時
	void Java_org_cocos2dx_cpp_AppActivity_nativeOnSocketDisconnected(JNIEnv *env,jobject thiz)
	{
		SocketManager::getInstance()->onDisconnected();
	}
	// Socketでエラーが発生した時
	void Java_org_cocos2dx_cpp_AppActivity_nativeOnSocketError(JNIEnv *env,jobject thiz,jstring errorMessage)
	{
		const char *errorMessageChar = env->GetStringUTFChars(errorMessage,0);
		
		SocketManager::getInstance()->onError(errorMessageChar);
		
		env->ReleaseStringUTFChars(errorMessage, errorMessageChar);
	}
}
