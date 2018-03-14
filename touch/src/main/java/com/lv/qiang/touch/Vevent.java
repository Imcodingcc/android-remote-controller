package com.lv.qiang.touch;

import android.annotation.SuppressLint;

import com.stericson.RootTools.RootTools;
import com.stericson.RootTools.execution.CommandCapture;

class Vevent {
    void executeCommand(String command) throws Exception{
        CommandCapture cmd = new CommandCapture(0, command);
        RootTools.getShell(true).add(cmd);
    }
}
