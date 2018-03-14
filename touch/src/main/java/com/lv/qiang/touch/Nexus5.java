package com.lv.qiang.touch;

import android.util.Log;

import com.stericson.RootTools.RootTools;
import com.stericson.RootTools.exceptions.RootDeniedException;
import com.stericson.RootTools.execution.CommandCapture;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.concurrent.TimeoutException;

public class Nexus5 {
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
        executeCommand(action);
    }

    public void switchIme() {
        String action = "ime set com.lv.qiang.wtsp/com.lv.qiang.service.AdbIME";
        executeCommand(action);
    }

    public void executeCommand(String command){
        CommandCapture cmd = new CommandCapture(0, command);
        try {
            RootTools.getShell(true).add(cmd);
        } catch (IOException | TimeoutException | RootDeniedException e) {
            e.printStackTrace();
        }
    }
}