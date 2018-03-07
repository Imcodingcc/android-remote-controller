package com.lv.qiang.ws;

import com.koushikdutta.async.http.AsyncHttpClient;
import com.lv.qiang.common.ConstValue;
import com.lv.qiang.touch.Nexus5;


public class WsOptClient {
    private Nexus5 nexus5 = new Nexus5();

    public WsOptClient(){
        nexus5.switchIme();
        AsyncHttpClient.getDefaultInstance().websocket(ConstValue.wsController, null, (ex, webSocket) -> {
            if(ex !=null){
                ex.printStackTrace();
                return;
            }
            webSocket.setStringCallback(s -> {
                nexus5.sendEvent(s);
            });
        });
    }
}
