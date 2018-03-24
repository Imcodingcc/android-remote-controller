package com.lv.qiang.ws;

import com.koushikdutta.async.http.AsyncHttpClient;
import com.koushikdutta.async.http.WebSocket;
import com.lv.qiang.common.ConstValue;
import com.lv.qiang.touch.Nexus5;

import org.json.JSONException;
import org.json.JSONObject;


public class WsOptClient {
    private Nexus5 nexus5 = new Nexus5();

    private static WsOptClient wsOptClient;
    public WebSocket ws = null;
    public static WsOptClient getDefault(){
        if(wsOptClient == null){
            wsOptClient = new WsOptClient();
        }
        return wsOptClient;
    }

    private WsOptClient(){
        nexus5.switchIme();
        AsyncHttpClient.getDefaultInstance().websocket(ConstValue.wsController, null, (ex, webSocket) -> {
            if(ex !=null){
                ex.printStackTrace();
                return;
            }
            ws = webSocket;
            webSocket.setStringCallback(this::sendMsg);
        });
    }

    private void sendMsg(String s){
        try {
            JSONObject jsonObject = new JSONObject(s);
            if(jsonObject.getInt("code") == 3){
                nexus5.input(jsonObject.getString("msg"));
            }else{
                nexus5.sendEvent(s);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }
}
