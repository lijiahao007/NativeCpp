package com.lijiahao.nativecpp.library;

public class NativeCppLibrary {
    static {
        System.loadLibrary("nativecpp");
    }

    public static native void initLogFile(String logDir);
    public static native void logNative(String msg);

}
