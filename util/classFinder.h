#pragma once
#include "../jni/jni.h"

jclass loadClass(JNIEnv* env, jobject modClassLoader, const char* cls) {
    jstring className = env->NewStringUTF(cls);
    jclass temp = env->GetObjectClass(modClassLoader);
    jmethodID loadClassMethod = env->GetMethodID(temp, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    jclass loadedClass = (jclass)env->CallObjectMethod(modClassLoader, loadClassMethod, className);
    env->DeleteLocalRef(className);
    env->DeleteLocalRef(temp);
    return loadedClass;
}