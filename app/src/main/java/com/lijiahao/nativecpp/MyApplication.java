package com.lijiahao.nativecpp;

import android.app.Application;
import android.os.Environment;

import com.lijiahao.nativecpp.library.NativeCppLibrary;

import java.io.File;

public class MyApplication extends Application {

    @Override
    public void onCreate() {
        super.onCreate();

        File downloadDir = Environment.getExternalStoragePublicDirectory(
                Environment.DIRECTORY_DOWNLOADS);
        File logDir = new File(downloadDir, "NativeCppLogs");
        NativeCppLibrary.initLogFile(logDir.getAbsolutePath());
    }
}
