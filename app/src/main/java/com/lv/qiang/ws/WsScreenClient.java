package com.lv.qiang.ws;

import android.util.Log;

import com.koushikdutta.async.http.AsyncHttpClient;
import com.lv.qiang.common.ConstValue;
import com.lv.qiang.thread.SendJpgThread;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingDeque;

public class WsScreenClient {

    public WsScreenClient(BlockingQueue<byte[]> dataList){
        AsyncHttpClient.getDefaultInstance().websocket(ConstValue.wsScreen, null, (ex, webSocket) -> {
            if(ex != null){
                ex.printStackTrace();
                return;
            }
            BlockingQueue<Boolean> sendList = new LinkedBlockingDeque<>(3);
            sendList.offer(true);
            sendList.offer(true);
            new SendJpgThread(webSocket, sendList, dataList).start();
            webSocket.setStringCallback(s -> sendList.offer(true));
        });
    }
}
