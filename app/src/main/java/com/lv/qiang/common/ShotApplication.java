package com.lv.qiang.common;

import android.accessibilityservice.AccessibilityService;
import android.app.Application;
import android.content.Intent;
import android.media.projection.MediaProjectionManager;

public class ShotApplication extends Application {
    private int resultCode;
    private Intent intent;
    private MediaProjectionManager mMediaProjectionManager;

    public int getResultCode(){
        return resultCode;
    }

    public Intent getIntent(){
        return intent;
    }

    public MediaProjectionManager getMediaProjectionManager(){
        return mMediaProjectionManager;
    }

    public void setResultCode(int resultCode){
        this.resultCode = resultCode;
    }

    public void setIntent(Intent intent1){
        this.intent = intent1;
    }

    public void setMediaProjectionManager(MediaProjectionManager mMediaProjectionManager){
        this.mMediaProjectionManager = mMediaProjectionManager;
    }
}
