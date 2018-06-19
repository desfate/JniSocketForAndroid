//
//  android 工具类 jni接口写在这
//

#ifndef CPPSUPPORT_UTILS_ANDROID_H
#define CPPSUPPORT_UTILS_ANDROID_H

//MAX log message length
#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MAX_LOG_MESSAGE_LENGTH 256
//MAX data buffer size
#define MAX_BUFFER_SIZE 80

JNIEnv *my_env;
jobject myobj;

//保存jniEnv
static void InitJNIEnv(JNIEnv* env, jobject obj){
    my_env = env;
    myobj = obj;
}

//打印日志到java环境中
static void LogMessage(const char* format, ...) {

    //cache log method ID
    static jmethodID methodID = NULL;
    if (methodID == NULL) {
        jclass clazz = my_env->GetObjectClass(myobj);
        methodID = my_env->GetMethodID(clazz, "logMessage",
                                    "(Ljava/lang/String;)V");

        my_env->DeleteLocalRef(clazz);
    }

    if (methodID != NULL) {
        char buffer[MAX_BUFFER_SIZE];

        //将可变参数输出到字符数组中
        va_list ap;
        va_start(ap, format);
        vsnprintf(buffer, MAX_LOG_MESSAGE_LENGTH, format, ap);
        va_end(ap);

        //转换成java字符串
        jstring message = my_env->NewStringUTF(buffer);
        if (message != NULL) {
            my_env->CallVoidMethod(myobj, methodID, message);
            my_env->DeleteLocalRef(message);
        }
    }
}

//通过异常类和异常信息抛出异常
static void ThrowException( const char* className,
                           const char* message) {

    jclass clazz = my_env->FindClass(className);
    if (clazz != NULL) {
        my_env->ThrowNew(clazz, message);
        my_env->DeleteLocalRef(clazz);
    }
}

//通过异常类和错误号抛出异常
static void ThrowErrnoException( const char* className,
                                int errnum) {

    char buffer[MAX_LOG_MESSAGE_LENGTH];

    //通过错误号获得错误消息
    if (-1 == strerror_r(errnum, buffer, MAX_LOG_MESSAGE_LENGTH)) {
        strerror_r(errno, buffer, MAX_LOG_MESSAGE_LENGTH);
    }

    ThrowException(className, buffer);
}

#endif //CPPSUPPORT_UTILS_ANDROID_H
