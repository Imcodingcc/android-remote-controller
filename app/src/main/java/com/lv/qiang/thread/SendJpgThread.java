package com.lv.qiang.thread;

import com.koushikdutta.async.http.WebSocket;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;

public class SendJpgThread extends Thread {

    private WebSocket webSocket;
    private String TAG = SendJpgThread.class.getName();
    private BlockingQueue<Boolean> sendList;
    private BlockingQueue<byte[]> dataList;
    public SendJpgThread(WebSocket webSocket, BlockingQueue<Boolean> sendList, BlockingQueue<byte[]> dataList){
        this.webSocket = webSocket;
        this.sendList = sendList;
        this.dataList = dataList;
    }

    @Override
    public void run() {
        super.run();
        while(true) {
            try {
                if (sendList.poll(10L, TimeUnit.SECONDS) != null) {
                    byte[] a;
                    a = dataList.take();
                    webSocket.send(a);
                } else {
                   webSocket.close();
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
