#include <jni.h>
#include <string>
#include "Utils/LogUtils.h"
#include "CppDemo/HelloWorldDemo.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_lijiahao_nativecpp_library_NativeCppLibrary_initLogFile(JNIEnv *env, jclass clazz,
                                                                 jstring log_dir) {
    // 转换Java字符串到C++字符串
    const char* nativeLogDir = env->GetStringUTFChars(log_dir, nullptr);

    // 调用初始化方法（添加参数传递）
    LogUtils::init(nativeLogDir);  // 使用头文件中的const std::string&参数版本


    LogUtils::info("Native_Ljh_Test", "native_C++ helloworld");
    // 释放JNI字符串资源
    env->ReleaseStringUTFChars(log_dir, nativeLogDir);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_lijiahao_nativecpp_library_NativeCppLibrary_runDemo(JNIEnv *env, jclass clazz) {
    HelloWorldDemo::logHelloWorld();
}