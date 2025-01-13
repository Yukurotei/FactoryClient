#pragma once

#include "../../jni/jni.h"

jobject findFabricLoader(JNIEnv* env) {
    jclass loaderClass = env->FindClass("net/fabricmc/loader/impl/launch/FabricLauncherBase");
    if (loaderClass == NULL) {
        printf("Error: FabricLauncherBase class not found\n");
        return NULL;
    }
    jfieldID launcherfield = env->GetStaticFieldID(loaderClass, "launcher", "Lnet/fabricmc/loader/impl/launch/FabricLauncher;");
    if (launcherfield == NULL) {
        printf("Error: launcher field not found\n");
        return NULL;
    }
    jobject launcher = env->GetStaticObjectField(loaderClass, launcherfield);
    if (launcher == NULL) {
        printf("Error: launcher object null found\n");
        return NULL;
    }
    env->DeleteLocalRef(loaderClass);
    loaderClass = env->FindClass("net/fabricmc/loader/impl/launch/FabricLauncher");
    
    jmethodID methodID = env->GetMethodID(loaderClass, "getTargetClassLoader", "()Ljava/lang/ClassLoader;");
    if (methodID == nullptr) {
        return NULL;
    }
    jobject classLoader = env->CallObjectMethod(launcher, methodID);
    if (classLoader == nullptr) {
        return NULL;
    }
    env->DeleteLocalRef(loaderClass);
    return classLoader;
}