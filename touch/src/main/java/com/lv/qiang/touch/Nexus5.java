package com.lv.qiang.touch;

import android.util.Log;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.Socket;

public class Nexus5 {
    private Vevent vevent = new Vevent();
    private String TAG = "Nexus5";

    public void sendEvent(String opt) {
            try {
                Socket socket = new Socket(ConstValue.socketController, 10151);
                DataInputStream input = new DataInputStream(socket.getInputStream());
                DataOutputStream out = new DataOutputStream(socket.getOutputStream());
                out.write(opt.getBytes());
                out.close();
                input.close();
                socket.close();
            } catch (Exception e) {
                Log.d(TAG, "touch: " + "client error");
            }
    }

    public void input(String text) {
        String action = "am broadcast -a ADB_INPUT_TEXT --es msg " + text;
        try {
            vevent.executeCommand(action);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void switchIme() {
        String action = "ime set com.lv.qiang.wtsp/com.lv.qiang.service.AdbIME";
        try {
            vevent.executeCommand(action);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void exe(String cmd){
        try {
            vevent.executeCommand(cmd);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}