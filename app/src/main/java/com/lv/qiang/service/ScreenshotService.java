package com.lv.qiang.service;

import android.annotation.SuppressLint;
import android.annotation.TargetApi;
import android.app.Service;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.PixelFormat;
import android.hardware.display.DisplayManager;
import android.hardware.display.VirtualDisplay;
import android.media.Image;
import android.media.ImageReader;
import android.media.projection.MediaProjection;
import android.media.projection.MediaProjectionManager;
import android.os.Build;
import android.os.IBinder;
import android.support.annotation.Nullable;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.WindowManager;

import com.lv.qiang.common.ShotApplication;
import com.lv.qiang.common.Tools;
import com.lv.qiang.ws.WsOptClient;
import com.lv.qiang.ws.WsScreenClient;

import java.nio.ByteBuffer;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

@TargetApi(Build.VERSION_CODES.LOLLIPOP)
public class ScreenshotService extends Service
{

    private int windowWidth = 0;
    private int windowHeight = 0;
    private int mScreenDensity = 0;
    public static int mResultCode = 0;
    public static Intent mIntent = null;
    private ImageReader mImageReader = null;
    private VirtualDisplay mVirtualDisplay = null;
    private MediaProjection mMediaProjection = null;
    private BlockingQueue<byte[]> jpgQueue = new LinkedBlockingQueue<>(10);
    public static MediaProjectionManager mediaProjectionManager = null;

    @Override
    public void onCreate()
    {
        super.onCreate();
        new WsScreenClient(jpgQueue);
        WsOptClient.getDefault();
        createSurface();
        startOnUIThread();
    }

    private void createSurface(){
        WindowManager windowManager = (WindowManager) getApplication().getSystemService(WINDOW_SERVICE);
        assert windowManager != null;
        DisplayMetrics metrics = new DisplayMetrics();
        windowManager.getDefaultDisplay().getRealMetrics(metrics);
        mScreenDensity = metrics.densityDpi;
        windowWidth = metrics.widthPixels;
        windowHeight = metrics.heightPixels;
        mImageReader = ImageReader.newInstance(windowWidth/2, windowHeight/2, PixelFormat.RGBA_8888, 5);
        mImageReader.setOnImageAvailableListener(reader -> {
            Image image = reader.acquireLatestImage();
            if(image != null){
                captureAndOffer(image);
                image.close();
            }
        }, null);
    }

    public void createMediaProject(){
        mediaProjectionManager = ((ShotApplication)getApplication()).getMediaProjectionManager();
        mIntent = ((ShotApplication)getApplication()).getIntent();
        mResultCode = ((ShotApplication)getApplication()).getResultCode();
        mediaProjectionManager = ((ShotApplication)getApplication()).getMediaProjectionManager();
        mMediaProjection = mediaProjectionManager.getMediaProjection(mResultCode, mIntent);
    }

    private void createVirtualDisplay(){
        mVirtualDisplay = mMediaProjection.createVirtualDisplay("screen-mirror",
                windowWidth/2, windowHeight/2, mScreenDensity, DisplayManager.VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR,
                mImageReader.getSurface(), null, null);
    }

    public void startVirtualDisplay(){
        createMediaProject();
        createVirtualDisplay();
    }

    public void startOnUIThread(){
        startVirtualDisplay();
    }

    private void captureAndOffer(Image image){
        int width = image.getWidth();
        int height = image.getHeight();
        final Image.Plane[] planes = image.getPlanes();
        final ByteBuffer buffer = planes[0].getBuffer();
        int pixelStride = planes[0].getPixelStride();
        int rowStride = planes[0].getRowStride();
        int rowPadding = rowStride - pixelStride * width;
        Bitmap bitmap = Bitmap.createBitmap(width + rowPadding / pixelStride, height, Bitmap.Config.ARGB_8888);
        bitmap.copyPixelsFromBuffer(buffer);
        bitmap = Bitmap.createBitmap(bitmap, 0, 0, width, height);
        Bitmap finalBitmap = bitmap;
        new Thread(() -> {
            byte[] a = Tools.bitmapToByteArray(finalBitmap);
            if(!jpgQueue.offer(a)){
                Log.d("drop", "drop");
            }
        }).start();
    }

    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    private void stopAndClear() {
        if (mVirtualDisplay == null) {
            return;
        }
        mVirtualDisplay.release();
        mVirtualDisplay = null;
        if (mMediaProjection != null) {
            mMediaProjection.stop();
            mMediaProjection = null;
        }
    }

    @Override
    public void onDestroy()
    {
        stopAndClear();
        super.onDestroy();
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }
}