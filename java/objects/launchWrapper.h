#pragma once
#include "../../jni/jni.h"
#include "classloader.h"

jobject loaderClass = nullptr;

jobject GetFabricLoader(JNIEnv* env)
{
    if (loaderClass != nullptr)
    {
        return loaderClass;
    }
    loaderClass = findFabricLoader(env);
    if (loaderClass == nullptr)
    {
        printf("Error(GetFabricLoader): Minecraft instance is not likely 1.21 fabric, stopping...\n");
        return nullptr;
    }
    return loaderClass;
}