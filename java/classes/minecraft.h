#pragma once
#include "../../jni/jni.h"
#include "../../util/classFinder.h"
#include "../objects/launchWrapper.h"

jobject minecraftInstance = NULL;
jclass minecraftClass = NULL;

jobject GetMinecraftInstance(JNIEnv* env)
{
    if (minecraftInstance != NULL)
    {
        return minecraftInstance;
    }
    jclass mcClass = loadClass(env, GetFabricLoader(env), "net.minecraft.class_310");
    if (mcClass == NULL)
    {
        printf("Error(GetMinecraftInstance): Caching minecraft failed(minecraftClass is NULL)\n");
        return NULL;
    }
    jmethodID getMinecraft = env->GetStaticMethodID(mcClass, "method_1551", "()Lnet/minecraft/class_310;");
    if (getMinecraft == NULL)
    {
        printf("Error(GetMinecraftInstance): Caching minecraft failed(getMinecraft method is not found)\n");
        return NULL;
    }
    jobject mcInstance = env->CallStaticObjectMethod(mcClass, getMinecraft);
    if (mcInstance == NULL)
    {
        printf("Error(GetMinecraftInstance): Caching minecraft failed(mcInstance is null)\n");
        return NULL;
    }
    minecraftClass = mcClass;
    minecraftInstance = mcInstance;
    return mcInstance;
}

jclass GetMinecraftClass()
{
    if (minecraftClass == NULL)
    {
        printf("Error(GetMinecraftClass): Please run GetMinecraftInstance first\n");
        return NULL;
    }
    return minecraftClass;
}

int GetRightClickDelay()
{
    jfieldID rightClickDelayField = env->GetFieldID(minecraftClass, "field_1752", "I");
    if (rightClickDelayField == NULL)
    {
        printf("Error(GetRightClickDelay): Field is null\n");
        return 0;
    }
    return env->GetIntField(minecraftInstance, rightClickDelayField);
}

void SetRightClickDelay(int max)
{
    jfieldID rightClickDelayField = env->GetFieldID(minecraftClass, "field_1752", "I");
    if (rightClickDelayField == NULL)
    {
        printf("Error(SetRightClickDelay): Field is null\n");
        return;
    }
    int delay = env->GetIntField(minecraftInstance, rightClickDelayField);
    if (delay > max)
    {
        env->SetIntField(minecraftInstance, rightClickDelayField, max);
    }
    //No free memory needed
}

jobject GetWorld()
{
    jfieldID levelField = env->GetFieldID(minecraftClass, "field_1687", "Lnet/minecraft/class_638;");
    if (levelField == NULL)
    {
        printf("Error(GetWorld): Level field is null\n");
    }
    jobject level = env->GetObjectField(minecraftInstance, levelField);

    //No free memory needed
    return level;
}

jobject GetEntityLastBlockPos(jobject entity)
{
    if (entity == NULL)
    {
        printf("Error(GetEntityLastBlockPos): Entity provided is null\n");
        return NULL;
    }
    jclass entityClass = env->GetObjectClass(entity);
    if (entityClass == NULL)
    {
        printf("Error(GetEntityLastBlockPos): Object class of entity does not exist\n");
        return NULL;
    }
    jfieldID blockPosField = env->GetFieldID(entityClass, "field_6268", "Lnet/minecraft/class_2338;");
    if (blockPosField == NULL)
    {
        printf("Error(GetEntityLastBlockPos): Block pos field doesn't exist\n");
        return NULL;
    }
    jobject blockPos = env->GetObjectField(entity, blockPosField);
    if (blockPos == NULL)
    {
        printf("Error(GetEntityLastBlockPos): Block pos is null\n");
    }
    env->DeleteLocalRef(entity);// Free mem
    return blockPos;
}