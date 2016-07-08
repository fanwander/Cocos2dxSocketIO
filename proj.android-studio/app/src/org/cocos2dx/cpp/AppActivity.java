/****************************************************************************
Copyright (c) 2015 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import android.util.Log;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.json.JSONObject;

import java.net.MalformedURLException;

import io.socket.*;

public class AppActivity extends Cocos2dxActivity
{
    // C++側のメソッドを呼ぶ宣言
    public static native void nativeReceiveMessage(String event, String message);
    public static native void nativeOnSocketConnected();
    public static native void nativeOnSocketDisconnected();
    public static native void nativeOnSocketError(String errorMessage);

    static private SocketIO socket = null;
    private static final String TAG = "Cocos2dxSocketIOSample";


    // Socket.IO接続
    public static void connectToSocketIO(String host, int port) throws MalformedURLException
    {
        String url = String.format("http://%s:%d", host, port);
        socket = new SocketIO(url);
        socket.connect(iocallback);

        Log.d(TAG, "Socket 接続しようとします");
    }

    // Socketにイベント送信
    public static void emitToSocketIO(String event, String message)
    {
        if (socket != null && socket.isConnected())
        {
            // イベント送信
            socket.emit(event, message);
            Log.d(TAG, "emit. " + event + " , " + message);
        }
        else if(socket != null && !socket.isConnected())
        {
            // Socket接続されていないのに送信しようとした場合
            AppActivity.nativeOnSocketError("Socket is not connected.");
        }
    }

    // Socket.IO切断
    public static void disconnectFromSocketIO()
    {
        if(socket != null && socket.isConnected())
        {
            socket.disconnect();

            Log.d(TAG, "Socket 切断します");
        }
    }

    // Socket.IOコールバック関数集合
    private static IOCallback iocallback = new IOCallback()
    {
        @Override
        public void onConnect()
        {
            Log.d(TAG, "Socket 接続されました");
            AppActivity.nativeOnSocketConnected();
        }

        @Override
        public void onDisconnect()
        {
            Log.d(TAG, "Socket 切断されました");
            AppActivity.nativeOnSocketDisconnected();
        }

        @Override
        public void onMessage(JSONObject json, IOAcknowledge ack)
        {
            Log.d(TAG, "Socket onMessage");
        }

        @Override
        public void onMessage(String data, IOAcknowledge ack)
        {
            Log.d(TAG, "Socket onMessage");
        }

        @Override
        public void on(String event, IOAcknowledge ack, Object... args)
        {
            // final JSONObject message = (JSONObject)args[0];
            Log.d(TAG, "received . " + args[0]);
            AppActivity.nativeReceiveMessage(event, String.valueOf(args[0]));
        }

        @Override
        public void onError(SocketIOException socketIOException)
        {
            Log.d(TAG, "Socket エラー発生しました");
            socketIOException.printStackTrace();
            AppActivity.nativeOnSocketError(socketIOException.getLocalizedMessage());
        }
    };
}
