package com.lv.qiang.wtsp;

import android.annotation.TargetApi;
import android.content.Context;
import android.content.Intent;
import android.media.projection.MediaProjectionManager;
import android.os.Build;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.KeyEvent;

import com.lv.qiang.common.ShotApplication;
import com.lv.qiang.common.Tools;
import com.lv.qiang.service.ScreenshotService;
import com.lv.qiang.touch.Nexus5;

@TargetApi(Build.VERSION_CODES.LOLLIPOP)
public class MainActivity extends AppCompatActivity {
    private Intent screenIntent = null;
    private int REQUEST_MEDIA_PROJECTION = 1;
    private MediaProjectionManager mMediaProjectionManager;
    private static final String TAG = MainActivity.class.getName();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        init();
        getMediaProject();
        startService();
    }
    private void init(){
        if(Tools.isPortAvailable(10151)){
            Nexus5 nexus5 = new Nexus5();
            nexus5.exe("/data/local/tmp/server &");
        }
    }
    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQUEST_MEDIA_PROJECTION && data != null) {
            ((ShotApplication)getApplication()).setResultCode(resultCode);
            ((ShotApplication)getApplication()).setIntent(data);
            ((ShotApplication)getApplication()).setMediaProjectionManager(mMediaProjectionManager);
            screenIntent = new Intent(getApplicationContext(), ScreenshotService.class);
            startService(screenIntent);
        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            moveTaskToBack(false);
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }

    @Override
    protected void onDestroy() {
        Log.d(TAG, "onDestroy: ");
        stopService();
        super.onDestroy();
    }

    private void startService(){
        starScreenshot();
    }

    private void stopService(){
        if(screenIntent != null){
            stopService(screenIntent);
        }
    }

    private void getMediaProject(){
        mMediaProjectionManager =
                (MediaProjectionManager)getApplication().getSystemService(Context.MEDIA_PROJECTION_SERVICE);
    }

    private void starScreenshot(){
        startActivityForResult(mMediaProjectionManager.createScreenCaptureIntent(), REQUEST_MEDIA_PROJECTION);
    }
}