package com.lv.qiang.touch;

import android.graphics.Rect;

import java.io.DataOutputStream;
import java.io.IOException;

public class RootedAction {
    private Process su;
    private DataOutputStream outputStream;
    public RootedAction(){
    }

    public void execCommand(String command) throws IOException, InterruptedException {
        su = Runtime.getRuntime().exec("su");
        outputStream = new DataOutputStream(su.getOutputStream());
        outputStream.writeBytes(command);
        outputStream.flush();
        outputStream.writeBytes("exit\n");
        outputStream.flush();
        outputStream.close();
        su.waitFor();
        su.destroy();
    }

    public void tap(Rect rect) throws IOException, InterruptedException {
        int left = rect.left + 1;
        int top = rect.top + 1;
        String action = "input tap " + left + " " + top + "\n";
        execCommand(action);
    }

    public void longPress(Rect rect) throws IOException, InterruptedException {
        int left = rect.left + 1;
        int top = rect.top + 1;
        String action = "input touchscreen swipe " + left + " " + top + " " + left + " " + top + " " + 1500 + "\n";
        execCommand(action);
    }

    public void back(int times) throws IOException, InterruptedException {
        String action = "input keyevent 4";
        for (int i = times; i > 0; i--) {
            execCommand(action);
        }
    }
}
