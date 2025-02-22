#include <jni.h>
#include <string>
#include "Utils/LogUtils.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_lijiahao_nativecpp_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    LogUtils::init();

    LogUtils::debug("Native_Ljh_Test", "Helloworld LogUtil C++");

    LogUtils::shutdown();

    return env->NewStringUTF(hello.c_str());
}