package com.lv.qiang.common;

import android.graphics.Bitmap;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.net.ServerSocket;

public class Tools {

    public static byte[] bitmapToByteArray(Bitmap bitmap) {
        ByteArrayOutputStream baos;
        byte[] bitmapBytes;
        baos = new ByteArrayOutputStream();
        bitmap.compress(Bitmap.CompressFormat.JPEG, 10, baos);
        bitmapBytes = baos.toByteArray();
        try {
            baos.flush();
            baos.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return bitmapBytes;
    }

    public static boolean isRoot() {
        boolean root = false;
        try {
            root = !((!new File("/system/bin/su").exists())
                    && (!new File("/system/xbin/su").exists()));

        } catch (Exception ignored) {
        }
        return root;
    }

    public static boolean isPortAvailable(int port) {
        try {
            ServerSocket server = new ServerSocket(port);
            System.out.println("The port is available.");
            server.close();
            return true;
        } catch (IOException e) {
            System.out.println("The port is occupied.");
        }
        return false;
    }
}
