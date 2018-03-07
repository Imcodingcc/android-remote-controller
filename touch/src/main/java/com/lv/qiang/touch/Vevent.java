package com.lv.qiang.touch;

import android.annotation.SuppressLint;

import com.stericson.RootTools.RootTools;
import com.stericson.RootTools.execution.CommandCapture;

class Vevent {
    private static final String TAG = Vevent.class.getName();
    public void executeCommand(String command) throws Exception{
        CommandCapture cmd = new CommandCapture(0, command);
        RootTools.getShell(true).add(cmd);
    }
    @SuppressLint("DefaultLocale")
    void sendevent(String event_num, int param_1, int param_2, int param_3) {
        try {
            String cmd = String.format("sendevent /dev/input/%s %d %d %d", event_num, param_1, param_2, param_3);
            executeCommand(cmd);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
